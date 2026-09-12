/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                G L 1  F R A M E   L A Y E R

GENERAL DESCRIPTION
   This file together with gl1_hw_gsm.c and gl1_hw_gprs.c contains the
   procedures for the frame layer of non portable layer 1.   The procedures
   used herein are intended for sole use by the message layer functions and
   not intended for use by the portable layer 1 software.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw.c#5 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
16/01/20   sc       CR2213654 Do G2X Deinit from inact_isr when GSM is deactivating
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
26/04/18   br       CR2230284 Reset ACI flag to FALSE, during x2g sch reconfirmation if power is less than -94dbm
25/04/18   rv       CR2225983 - GERAN F3 and log pkt Reduction TA.3.0
19/04/18   sp       CR2225702 Revert CR2174600, provide alternative solution.
21/03/18   km       CR2208916 Clear gl1_hw_start_gsm_qta_sent from qta clean up and while moving to inactive mode
28/02/18   km       CR2189789 Handling g2l abort from g2l_trigger_mdsp
27/10/17   sp       CR2132978: Reset Dynamic RxD states also when putting PRx to sleep
13/02/18   nsa      CR2188892 In ssim case we need to release TRM while G2W BPLMN is ongoing
02/02/18   nsa      CR2174600 Not releasing TRM reservations when BPLMN search is ongoing so that there is enough time when GSM resumes for page.
11/08/17   rc       CR2138578 ML logging - GL1 Changes
11/08/16   sk       CR1048655  FW is ordered to read RF Alpha values from RF without which RSSI was 
                    getting floored leading to xCCH failure, if it's first QTA after Stack activation
22/08/16   snjv     CR1056129 Allow Div. to be requested from WarmUp CB when saic,lmm is active
07/06/17   br       CR2053507 Enable Tuneaway in G page reorg mode.
10/08/16   cws      CR1044687 Use ARFCN from pwr sm directly for pwr monitor log packet
17/10/16   cws      CR1078827 Do not check TRM status when sending RxD wake up cmd to RF
19/09/16   cws      CR1067531 Use turn tx off API which checks Tx status before sending Tx sleep req to RF
28/12/15   br       CR 955168 GL1 to disable TX during rach to find bcch state transition 
07/04/16   snjv     CR1000205 Retrieve Rx/Tx Channel Id based on Rx/Tx client id
22/04/16   ksb      CR1005869 To mainlne FEATURE_GL1_SLEEP_DEBUG_STRUCT
15/12/2016  ksb       CR1041734 Error recovery handing at regular interval using NV control
12/12/17   km       CR2158090 Compilation warnings fix
21/04/16   db       CR999886 tx trm status is not reset at the time of sending start_gsm_mode_req in single SIM mode
28/02/17   og       CR2006578. FRs 39363 and 39364.
25/10/17   ng       CR2132231 Reset 'gl1_deep_sleep_pending' if MCPM need not be stopped after G2X Deinit is done
29/10/17   km       CR2133917 GL1 support for TA.3.0 L+L
06/07/17   mk       CR2071218 FR44121 -Synchronous Ncell Quality Monitor - read NV item
02/02/17   br       CR1103497 FR 39735: Allowing TA during PAGE RE-ORG mode in GERAN
17/08/17   br       CR2092505 NV control for logging GDET related F3s
28/09/16  br        CR1071711 Removal of F3 which is causing the ISR overrun.
14/02/17   mk       CR2004610 : FR 40251 (SeNS) - Add NV item to enable SeNS feature
21/04/17   br       CR1041084 FR 36219: GDET – GMSK Modulation Detector
03/01/16   mn       CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
03/04/17   br       CR2027088 Band based ASDIV support during ACQ
15/05/17   akm      CR2047050 Avoid GL1 going to sleep during rude wakeup if RF wakeup CNF is not received
17/05/16   zf       CR1016821: Moving EFS reading out of DPC.
16/12/16   sk       CR1100657  Send GERAN_FW_RFM_ENTER_MODE_CMD only for PRX if wakeup Cnf has come only for it
22/08/16   sk       CR1053862 Reduce the number of error messages.
04/08/16   rv       CR1024678 Band change request shouldn't be called while leaving idle mode, as part of L1_DS_ABORT_IND.
27/05/16   snjv     CR1000909 pass ACI400 switchpoint detection stat from GFW to GRF via GL1
12/07/16   tsk      CR978200 Support Dynamic Rxd with SSIM and WLAN Antenna Sharing configuration.
30/06/16   hd       CR1035752 Send MCPM_GERAN_DATA_STOP_REQ only when it was not sent before
15/06/16   mko      CR1025814:Update the global variable used for GCF flag before registering the NPA callback
29/06/16   hd       CR1035604 Rename gl1_clear_drx_tx_status_at_multi_sim_deact
08/06/16   bg       CR1023927 No need to set Tx band if we're leaving idle in X2G handover case
03/06/16   hd       CR1021365 Send MCPM_GERAN_DATA_STOP_REQ while exiting FTM mode
07/03/15   mko      CR981074: Read the GCF NV first then try to register NPA callback based on GCF NV settings
08/01/16   cws      CR949517 Force asdiv position when requested by RF at certain SAR stateNew
08/02/16   jj       CR872002 GL1 changes for VSWR feature
10/02/16   dg       CR973328 Don't forcefully re-initialize GSM timer on receiving rat change indication from GRR
14/01/16   dg       CR961154 Don't send GERAN stop to MCPM if X tech hasn't sent STOP cnf
12/01/16   sp       CR945229: Ensure Async Command issued only after Enter Mode to GFW in Normal ISR.
15/12/15   snjv     CR952056 regrsn. of 931797: update GFW with EFS items Read 
25/11/15   zf       CR942162: Default not to crash after GFW recovered from delayed ACQ
23/11/15   cgc      CR944459 Fix regression due to CR937488, also requires change in GFW 
23/11/15   sk       CR936213 Read client RGS value while inside the QTA gap for AFC update
24/11/15   cc       CR928622 Use idle Rx Alpha for T2G idle in DR mode
18/11/15   cja      CR938089 Use XtoG RF timing for RX in QTA gap
12/11/15   smd      CR940559 Don't set lower_ccs_pri in QTA gap since use_idle_rx_alpha is not set 
09/11/15   og       CR929332. Don't do immediate logging of power monitors when
                    suspending GSM L1.
11/05/15   cc       CR931979 Move EFS read to l1 task init
03/11/15   sp       CR916957: Update CR909073, log with 0's instead of no logging when yield
23/10/15   cc       CR923517 F3 reduction
16/10/15   pg       CR924813 Enable LIF in FTM Cal for Rx only devices
06/10/15   cja      CR918431 If QTA startup after GSM start/resume initialise MDSP
25/09/15   jk       CR915579:Enable LIF by default for ACQ in FTM Cal mode
23/09/15   zf       CR911400: Don't release Tx with pending lock exchange
28/09/15   aga      CR911414 During QTA RF init send a START GSM RF if needed 
15/08/15   zf       CR887132: Use variant for IRAT CM_GSM_INIT MSGs.
07/09/15   npt      CR898493 GL1 NV control of T2G acquisition GFW error recovery
03/09/15   kb       CR863908:Added new EFS file structure for sleep control
01/09/15   jk       CR899990:Enable LIF by default in FTM mode 
02/09/15   jk       CR897830:Derive IP2 cal support information from the SAWLESS support flag in RFA_RF_GSM_START_GSM_CNF
01/09/15   ws       CR899984 - Enusre gl1_hw_get_rf_gsm_mon_setup_time() uses correct 
                    Idle Rx alpha setting
26/08/15   ws       CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
26/08/15   npt      CR892374 Generate monitor log when leaving GERAN
18/07/15   zf       CR857733: Add X2G CM API for X2G measurement stop duration
21/08/15   cah      CR893877: DR-DSDS: Lower RX ccs priority for sub in Find BCCH state 
20/08/15   dg       CR892911 Remove API l1_g2x_terminate
20/08/15   npt      CR878988 GL1 NV control of GFW recovery features
15/08/15   zf       CR891129: Release TxLM buffer when turn Tx OFF.
12/08/15   jk       CR882186:Don't request DRx device if in warm up ISR,LMM and SAIC is disabled
14/08/15   hd       CR889027: Register G2W MSGR message registration at the stack start
14/08/15   df       CR882861 Improve comments only for gl1_hw_rf_wakeup_req_msg
14/08/14    ws      CR889273 Fix Static declarations of variables which are not Dual data spaced
12/08/15   cws      CR864883 FR27546+FR27544 Add MTPL/power imbalance factors into ASDIV algorithm
12/05/15   jk       CR883632:SAWless support info needs to be handled in RF start GSM CNF message
12/08/15   js       CR848503 GL1 ISR profiling data during multi sim FW RF initialization
06/08/15   cja      CR884573 Featurise for default GFW recovery/crash on CCS error
04/08/15   nk       CR884377 Dont update TX device if TRM resource lock in ON
05/08/15   cws      CR881986 Use only gsm1/gsm2 client for trm asdiv request
17/07/15   ab       CR 871981 Address Static Analysis Warning.
31/07/15   cah      CR882109 FR:26893: RR is basing band prioritisation on Dr mode echoed in START_GSM_MODE_CNF
28/07/15   jk/pa    CR864124:Clear RxD structure every time GERAN START is received. 
23/07/15   pg       CR877745 Reorder multisim FW RF init to reduce critical path through RF wakeup 
05/12/15   sb       CR836803 Fix a wrong featurisation definition with TDS and LTE
24/07/15   aga      CR879086 Remove deprecated APIs used  gl1_hw_cm_wpll_time_us( void )
17/07/15   hd       CR869646 Do not call fws_app_enable() when APP is already enabled while calling FW load
22/07/15   ap       CR877707: Use TRM_RX_BEST/TRM_RX_ANY/TRM_TX_BEST instead of TRM_RXTX_ANY and TRM_RXTX_BEST 
21/07/15   cws      CR873163 fix asdiv/pchxfer compile warnings 
21/07/15   cjl      CR876741 Replace TRM subreasons with system procedure types
21/07/15   pg       CR871012 Apply FTM diversity band support capabilities passed by GRF for cal 
20/07/15   pa       CR875967:Disable Data Rxd on Multimode sub if other sub is non3pp capable.
14/07/15   ap       CR868991 The TRM Release for BPLMN is sometimes too late causing missed PCH 
18/05/15   dg       CR838227 Remove frame_slaming_flag as block level checks are already there
                    to ensure no G2X irat is triggered if TS adjustment is pending
13/07/15   aga      CR870977 Remove deprecated APIs used for CM
17/06/15   cjl      CR853555 Use dynamic connected mode reasons for CS/PS traffic
07/07/15   mc       CR855818 : COEX:MCS_CXM_WWAN_TECH_STATE_IND registrations for X2G IRAT need to indicate CXM_LNK_TYPE_IRAT
01/07/15   jj       CR 864036 avoid registering WCDMA IRAT message when it is already registered 
02/07/15   aga      CR860197 NULL2L workaround Reserve and request serving band for LTE IRAT
30/06/15   ws       CR852519 - Resolve GL1 enum conversion compile warnings
25/06/15   sp       CR856664: Register G2W IRAT messenger handlers on GSM Start mode request by using correct AS ID
18/06/15   cah      CR855156: set useIdleModeRxAlpha when configuring RF and FW for DR-DSDS operation in idle. 
17/06/15   aga      CR855583 Allocate RxLM buffer in the QTA start-up callback if invalid
14/05/15   jk       CR838187:Reset gl1_check_toenable_rxd flag while sending RF sleep request
12/05/15   am       CR828682 Enable Rpush update from all G clients
02/06/15   cja      CR844096 RF meas gsm parameters to use xtoy param type
02/06/15   pg       CR846507 Allow non-signaling RF devices to be controlled by FTM 
29/05/15   pa       CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD.
22/05/15   pg       CR841558 In FTM, use DRx device passed by GRF (if valid), rather than via TRM request  
15/05/15   sp       CR838865: Fix compile errors on TA20 build.
08/05/15   jk        CR834822:Wait for G2L_DEINIT_CNF  before sending MCPM GERAN STOP request
11/05/15   ar       CR836017 Added error fatal if trm is denied in single SIM mode.
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
                    CRXXXXXX: Don't send set_tx_band at L1_GET_TIMING_COMMANDs
                    CRxxxxxx: Don't reset device ID at sleep in single sim mode.
15/04/15   pa       CR822841: Split Rx/Tx GL1 TRM changes for Single SIM.
08/05/15   ar       CR834975 Double the RF tuning parameters in the GL1 for W+W  
05/07/15   zc       CR826075 Band Specific ASDIV Enable
01/05/15   aga      CR831392 Use the appropriate WCDMA client ID gased on static AS_ID mapping
30/04/15   ap       CR811057 FR22272 - TRM Unification  - Logging Changes
29/04/15   npt      CR804870 RF timeline error recovery mechanism
28/04/15   aga      CR829716 Deallocate the rxlm buffers at G2X TA time for all subs
21/04/15   zc       CR806813 Fix ASDIV switch stuck
20/04/15   ip       CR811868 Dual Multisim mode code changes + debug overhaul
20/04/15   nm       CR822587 initilaize the timing info  for DRX chain
09/04/15  dp/cja    CR819956 Thor 2.x RF API change, G2W use MSGR
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session
23/02/15   sjv      CR799313: Using new mcfg APIs replacing efs_get for Geran
31/03/15   pa       CR815621:GRM restructuring for TH 2.0.
25/03/14   gk/npt   CR711799 GL1 changes for triggering recovery mechanism in GFW when RF CCS Events are in incorrect state
20/03/15   cs       CR788591 Updated the changes to enable Parallel SCH by default for Multi-SIM builds
17/03/15   pa       CR808254: Reset RxD GFW state varaibale after turning Rx off.
16/03/15   pg       CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP
12/03/15   jk       CR805523:TRM status is wrongly set to GL1_TRM_GRANTED when RF device is RFM_MAX_DEVICES
12/03/15   pa       CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
24/01/15   jj       CR 799441 Active irat should not reset until id state becomes NULL
05/12/14   sp       CR767083: Override paging multiframe to 2, for PDT tests
24/02/15   cws      CR799501 [ASDIV]Initialize antenna_to_use_for_tx even when TRM is not granted yet
07/01/15   jj       CR777805 Add probe burst flag for diversity rx
27/02/15   cja      CR768169 Add RF Seq Number
27/02/15   am       CR646584 WLAN coex
06/02/15   dv       CR792323 RF API(CR780577)change to ensure pwr mon timeline
20/02/15   js       CR693771: W+T removal. Resolving compilation errors
28/01/15   sk       CR786772 Ensure enter mode command not issued to FW in MEAS IDLE state
18/02/15   jj       CR777865 don't sent ASDIV switch command when TRM denied the RF resource
17/02/15   df       CR779661 Add sleep logging
16/02/15   cja      CR794676 Only set tx band iss to GFW false if RF set Tx band is sent to RF
15/02/15   sp       CR729492 Remove unimportant logging
13/02/15   pa       CR756156:G2X RF device handling using TRM apis.
11/02/15   am       CR778458 Update desense channel list only on enter mode
11/02/15   am       CR719452 Addressing KW GL1 P1 errors
11/02/15   gk       CR710069 Make gl1_qta_wcdma_has_initialised dual data spaced
27/01/15   jj       CR 787210  set the band invalid before tx enable
29/01/15   cs       CR788591 Enable Parallel SCH by default unless Multi-SIM
23/01/15   npt      CR697236 Mainline FEATURE_GERAN_VSTMR
21/01/15   pa       CR758792:RxD support for multi-sim.
07/01/15   jj       CR778334 Don't send set tx band when hw tx off
05/01/15   pa       CR774166: Enable DR on inactivating GL1 task.
12/12/14   cs       CR754853 Add support for Parallel SCH detection handling
16/12/14   jk       CR753423:LIF for Idle,voice and data
23/12/14   dv       CR775079 Update alpha vlaues on every wakeup.Requires GRF CR772694
22/12/14   pa       CR773689: Set RF device to Invalid on RF exit.
15/12/14   pg       CR630281 implementation of RxD support for data/MSRD
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
11/12/14   jk       CR769657:Populate HLinLLin DTM metrics correctly
09/12/14   cja      CR752567 Always send set tx band after Tx enable cnf
08/12/14   ap       CR726899: Update Panic Reset mechanism - remove dependency on TRM
10/12/14   ws       CR768376 Q6 compile warning fixes
12/03/14   am       CR765708 Don't request TRM while deactivating the l1_task
27/11/14   cah      CR762427 do not allow RF device to be overwritten with invalid ID.
03/12/14   npt      CR759467 Fix compiler warnings
02/12/14   cah      CR740519: ensure correct Rxalpha is used in DR-DSDS mode by idle sub.
26/11/14   jj       CR 760174  fix compiler warning
25/11/14   js       CR750188 Set rf_enter_mode_required after exit mode is send to RF
21/11/14   pg       CR760899 Enable Idle RxD for Thor single-SIM
19/11/14   cja      CR758107 Do not define rex timer multiple times - so add rex_undef
17/11/14   npt      CR642426 Added power reporting log packet
13/11/14   aga      CR751710 Allocate RxLM buffer if it has been deallocated
13/11/14   npt      CR746180 Remove KW errors
13/11/14   pa       CR755557:Support for granted band/rf device in Idle mode RxD.
13/11/14   jk       CR735025:Remove unncecessary LIF debug F3s
01/11/14   df       CR748602 Rework and enable adaptive timeline
10/11/14   sjv      CR753434 Return Device 0/1 for G2w&G2L Irat when not in DR Mode
24/09/14   jk       CR729382:Enable LIF feature by default
28/10/14   zf       CR747831 FTM RFFE bus contention workaround
29/10/14   zc		CR747869: Changes on top of CR732421 for ASDIV setting config once on wakeup
28/10/14   pa       CR746908: In waiting for TRM grant state, don't set RF TX band.
07/08/14   xz       CR706001 (update CR700617) Don't stop GSM in MCPM when G2T stop_cnf has not feedback yet.
21/10/14   fan      CR734490 Rework the wake up recovery mechanism
17/10/14   cs       CR735918 Assign supported bands for initial ACQ based on UE support
15/10/14   pg       CR698786 Reset current Tx band on disabling Tx
15/10/14   ak       CR730234 Sleep debug code tidy up
14/10/14   df       CR676084 Sleep debug structure improvements
10/10/14   cos      CR736560 BO 2.x Idle RxD mirror combined exit/sleep requests according to the enter/wakeup modes
07/10/14   pg       CR730132 F3 cleanup
07/10/14   sjv      CR734254  G2L rf devices retrieval for primary and diversity
26/09/14   zf       CR732436: Delay starting ACQ when RF wakes up late
03/10/14   aga      CR732455 Changes required to tune away framework for TH 1.0 single sim build
03/10/14   sjw      CR730114: use correct GFW interface enum for Single SIM mode
02/10/14   sjv      CR733766 Get RF Device from grm_info instead of querying TRM
26/09/14   fan      CR730788 Moving the start RF timer next to setting RF_WAKEUP flag to the commit mask
                          and correctly abort PWR mon
26/09/14   zf       CR730930: Cancel pending GFW commands when RF does not wakeup before next frame to mitigate TRM/RF concurrency delay issue.
02/09/14   dg       CR717969 Clear flag gl1_set_tx_band_cmd_iss_to_gfw before enabling RF for TX
20/05/14   dg       CR613013 Long Term fix on GL1 for TS 34.123 & TC 8.3.11.12 GCF Testcase
                    failure due to RACH failure on GSM cell
29/09/14   cos      CR731340 [BOLT][DR-DSDS] GL1 to check FTM mode to use FTM RXD flags from RF
29/09/14   cgc      CR727137 rework LTE api additions add gl1_hw_get_lte_irat_rf_device()
26/09/14   sjv      CR731259 Correct device id for G2X IRAT
24/09/14   cah      CR729408: Updates to reduce number of TRM API calls in Gl1.
23/09/14   fan      CR727696 Avoid race condition between starting the timer in warmup CB and RF wakeup cnf processing
22/09/14   ap       CR720862: FR 20912: SGLTE+G DSDA with WTR asymmetric GSM support
22/09/14   cgc      CR727137 LTE api additions add gl1_hw_get_lte_irat_rf_device()
08/10/14   cs       CR709170 Always clear enableEPD when GCF is enabled
18/09/14   pm       CR677361: Populating client_id back again, which was removed as part of fix for CR677361
12/09/14   fan      CR636420: Run Sleep commit after GSTMR ISR at wakeup
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
11/09/14   am       CR677361 Addressing KW GL1 P1 errors
09/09/14   jk       CR722274:Device ID to be passed in FW enter mode command
09/09/14   cos      CR720965 adding support for DR-DSDS - rfdev 2 and 3 handling wtr to RxD and calibration
08/09/14   cja      CR715411 Add support of MPH_DEEP_SLEEP_IND
09/09/14   cah      CR718841: GL1 missing support for RFSW changes for RXFE bus contention
07/09/14   js       CR711466:Reset the main command after drx off in W2G handover.
04/09/14   zf       CR702512: Correction in detecting GSTMR interrupt pending
04/09/14   am       CR682760 Addressing KW GL1 P1 errors
03/09/14   df       CR716674 Check hw state before getting quarter symbol count
29/08/14   cos      CR 716812 - Adding missing changes for imrd framework
28/08/14   am       CR716841 enable device id in enter mode command as FW is supporting now
28/05/14   ap       CR671971: Change SGLTE mode on reception of UE_MODE_CHANGE_REQ
25/08/14   df       CR660576 Enhanced timeline debug
22/08/14   cja      CR643648 Desense for channel adjacent to spur
24/07/14   nk       CR699150 W2G IRAT timeline improvement (remove F3s from GL1)
24/06/14   nk       CR683275  Deallocate RxLM buffer if already allocated and error fatal if not able to allocate buffer
23/04/14   jj       CR 653796 function  cannot called inside ASSERT
11/02/14   jk       CR614254:GL1 changes to support IP2 calibration
12/08/14   cah      CR705709 T unable to register in network in SGTDS mode.
08/08/14   ka       CR706866 FR18739: DR-DSDS RxD Idle, part 2 - new TRM clients
06/05/14   dv       CR660054 RXLM enhancement to ensure that no tech de-allocates another techs buffers
31/07/14   zf       CR702575: Prevent sending GFW enter mode command if GFW app is not in the correct mode.
06/08/14   ka       CR699964 Implement Idle RXD support in GL1
06/08/14   df       CR608357 Always reset mdsp asynch enabled flag when calling interface reset
06/08/14   df       CR598015 Avoid silent frame DRX processing when pending firmware activity trigger is set
06/08/14   df       CR590772 - Use of NV items to contol sleep and features
04/08/14   ss       CR704198 Do not prepares Tx band while leaving idle if main_command = L1_DECOUPLE_MULTI_SIM_COMMAND
04/08/15   cja      CR703742 Check IRAT state machine is NULL before isr inact is complete
31/07/14   cos      CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
24/07/14   jk       CR699271:GL1 LIF changes for RxD
28/07/14   zf       CR700730: Removing RXLM buf idx in set GFW app mode
29/07/14   ap       CR695234: Changes for GL1's New Rach interface
30/06/14   dg       CR684715 Set rfm device to RFM_DEVICE_0 only if GBTA is active
22/07/14   ap       CR694548  Ensure RF Enter/Exit when switching from DSDA to DSDS Mode
27/07/14   ap       CR678067: Race condition between APP mode as ACTIVE and Device mode change as DSDA leads to crash
26/07/14   zf       CR700483: Set GFW APP mode before RF init
15/07/14   am       CR693804 Release TRM after TX is turned off
21/07/14   cja      CR696293 In check for async possible check asleep before checking int pending
01/07/14   npt      CR682701 Do not try to run async early go to sleep if normal go to sleep has already started
30/05/14   jj       CR656854  Resolve Critical Klocwork errors in gdrivers code space
21/07/14   jk        CR660801:Support for logging VAMOS log packet 0x5099
03/06/14   xz       CR671667  Set firmware_load_status as FW_LOADED after re-init GRF/GFW.
18/07/14   ap       CR685677: Dont set tx band when entering Decoupled state
18/07/14   pg/pa    CR694601:GERAN L1 support for dedicated WTR hopping with FED architecture
18/07/14   zf       CR696301: Do not wait for RF wakeup CNF if RF has not to been initialized
10/07/14   js       CR593074- ROTA/QBTA Feature implementation
04/07/14   cs       CR688003 GRM functions moved to gpl1_grm_intf.c
02/06/14   jj       CR 688565 re-name function get_gsm_client_from_gas_id to
                    gl1_map_gas_id_to_client_id  under asdiv flag
25/06/14   sk       CR683422 Ensure PT sub votes tx off during multi sim deact before issuing RF sleep
02/07/14   aga      CR675214: G2T TA feature and GTA/G2W TA re-worked optimized solution
02/07/14   mc       CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
18/06/14   cos      CR682031 - GL1 control algorithm and changes for supporting AEQ PHASE 4
01/06/14   jj       CR 687356 Deperecate AOL tuner blank control from GL1 as they are controlled in RFA now
25/06/14   pg/jv    CR673242 avoid calling rf_exit with an invalid device ID
27/06/14   cs       CR685966 Add DR INFO async callback support
10/06/14   jj       CR 668965 merge ASDIV from DI.3.0 to BO 2.0
24/06/14   cs       CR684714 Add back in functionality removed for initial updated TRM API support
24/06/14   df       CR672016: Improve function parameter checking to avoid possible corruption
30/05/14   pg       CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM
19/06/14   cs/ch    CR651795 TRM Updated changes for DR-DSDS
29/04/14   pjr      CR600612 Dedicated WTR Hop initial code drop
06/13/14   zc/smd   CR678923  Gave the right device number during QTA.
18/06/14   ssh      CR681747: Use the fws_get_app_enabled API from FW services
17/06/14   cos      CR681305 BOLT GSM Thermistor Read - cleanup PRX/DRX RF enter mode requests
12/06/14   ssh      CR671029: On G+G wake up collision defer wake up by 1 frame
09/06/14   pg       CR676887 Add RF_sub_id to as_id mapping API
06/06/14   dp       CR677449 make sure cm_terminate is called in T->G HHO
03/06/14   cja      CR673927 Do not call RF sleep, if Rx already off.
01/04/14   cgc      CR637156 timer api change requested by core team, add gl1_create_hw_timer()
30/05/14   ssh      CR673304: Allow QTA gap on multimode sub for SGLTE + G DSDA scenario
29/05/14   cs/pa    CR660831 Use new trm_status definitions specific to GL1
21/05/14   ssh      CR668643: gl1_hw_qta_gap_active() should return FALSE for multimode sub
20/05/14   ssh      CR667235 Whilst reprogramming sleep on WUP cancel always check if all conditions met
16/05/14   nm       CR661550 Check if FW has some pending commands before movidn it to meas idle
13/05/14   cos      CR663593 Moving Therm Read from RF enter mode to TX enable
14/03/14   am       CR629668: Update trm_status in single sim mode also
09/05/14   cos      CR662080 Correcting CM enter/exit confirmation messages usage
08/05/14    sp      CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
02/05/14   zf       CR640262: Need to call RF enter mode after multi SIM init
02/05/14   cja      CR658621 Optimise QTA and T2G F3.
30/04/14   nm       CR654457: G is releasing TRM lock during G2W Handover causing TRM to be allocated to other G sub
25/04/14   ap       CR650286 Handle DEVICE_MODE_INDICATION whilst two G stacks active.
25/04/14   nm       CR654457: G is releasing TRM lock during G2W Handover causing TRM to be allocated to other G sub
25/04/14   cja      CR655404 Use CM RF device ID for QTA and CM in rx burst
17/04/14   cos      CR636543 - Changes for enabling Rx Diversity on Bolt - GL1/GRF intf updates
17/04/14   cos      CR646420 Changes for enabling Rx Diversity on Bolt - GL1/GFW intf updates
17/04/14   jk       CR649527:handling frequency hopping and DTX with Shifted SACCH for all cases
19/03/14   ws       CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
17/03/14   pjr      CR629255: Protect calls to gl1_vs_acquire with critical section
17/04/14   jk       CR649527:handling frequency hopping and DTX with Shifted SACCH for all cases
17/04/14   jj       CR 650872 send switch complete based on  call back from rf in pch crc fail
14/04/14   ap       CR645063: Incorrect device identity being used in Factory Test Mode
11/04/14   cja      CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
11/04/14   pg       CR645875 Fix Bolt multisim compilation errors
10/04/14   pa       CR646424:Bolt: Revert back Dime CR613982:Send therm read value in enable tx command to RF
04/04/14   pg       CR643846 RFLM/FED architecture support for multi-SIM
04/04/14   cah      CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
09/04/14   jk       CR645719:use correct gas_id to retrieve rxlm buffer index while VregOff request
28/03/14   zc       CR640584: The EFS read for ASDIV to support dual sim
09/04/14   jk       CR645719:use correct gas_id to retrieve rxlm buffer index while VregOff request
09/04/14   zc       CR645440: map undefined trm client to client using gas_id during inact
09/04/14   jk       CR645719:use correct gas_id to retrieve rxlm buffer index while VregOff request
19/03/14   nm       CR633705 add check for FIND_SCH_COMMAND before issuing TX band command while leaving idle
26/03/14   dv       CR634656 Only alloacte buffers when G stack starts up
12/02/14   jj       CR596556 Returning switch permission to LTE when GSM is in OOS in SGLTE mode
28/03/14   jk       CR640519:Update the shared variables if gl1_hw_init is called more than once
01/04/14   cgc      CR637156 timer api change requested by core team, add gl1_create_hw_timer()
01/04/14   pjr      CR637457 Remove FEATURE_DUAL/TRIPLE_SIM around gl1_hw_data_init
28/03/12   am       CR637837 Ensure that AFC command is sent to GFW
28/03/14   jk       CR640358:Populate RxD params in RFA_RF_GSM_RX_BURST_REQ even when RxD is disabled
26/03/14   dv       CR639099 During TBF suspend ensure GFW not busy before sending APP mode
21/03/14   jj       CR 636569 UMID change for antenna switch reqest dsda
20/03/14   br       CR635637:Reset FW_loaded to FALSE in gl1_hw_incat,since trm is released and mdsp is alreaay disabled.
19/03/14   ws       CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
19/03/14   cja      CR632037 Use RF Task for X2G
17/03/14   pg       CR632107 Disable MDSP before RF exit for FED architecture
14/03/14   am       CR629668: Update trm_status in single sim mode also
17/03/14   jj       CR 633747 GL1 set antenna command send different  UMID per SUB
17/03/14   ssh      CR631959: Remove forced rude wake up for G+G(+G) case
12/03/14   pa       CR630228:Bolt: Changes due to obsolete FW API cleanup
05/03/14   jj       CR 610751  dime 3.0 dsda Asdiv
03/03/14   cja      CR513250 G band in IRAT replace rfgsm_band_type with rfcom_gsm_band_type
25/02/14   cos      CR608951 GERAN RFLM FED: Pass subscription ID to GERAN RF in all message router payloads
15/02/14   ip/npt   CR547955 Define multi thread usage on wake up from sleep
13/02/14   hd       CR614350 RF Sleep API is called during RAT change also
07/02/14   cja      CR612831 Always do mdsp_init as part of gl1_hw_init
04/02/14   cja      CR611621 Do complete CM terminate before gstmr init
03/10/14   sc       CR628645 : Print F3 when RXLM buffer is allocated
10/03/14   ssh      CR625316: We don't need to check if all the subs are in idle if the device is already in GSM mode.
12/02/14   zf       CR613603: Don't leave idle mode again if not in idle mode
27/02/14   ip       CR608864 Dime3.0 DB timeline changes
28/02/14   aga      CR611797: Exception recieved in L1 ISR hw scheduler frame tick, avoid resetting scheduler at tune back
27/02/14   zc       CR624460 gl1_hw_sglte_mode returns true if either sub is set
27/02/14   ap       CR600310 Enable Channel id support
02/01/14   jk       CR595130: GL1 changes to support VAMOS-II
23/02/14   mc       CR621753 Back out 3WCXM changes due to stability issues
19/02/14   ap       CR600310 GSM L1 to send channel Id to GSM FW corresponding to the primary RF device
19/02/14   zc       CR620061 Updated QTA Rx burst fix
19/02/14   ap       CR618768 In SGLTE+G SS Retain TRM lock for TRAFFIC
21/11/13   dv       CR579907 Back out CR 519537
13/02/14   ap       CR612587: Ensure call RF exit GSM mode when releasing TRM in DSDA mode
13/02/14   ss       CR613982 Send therm read value in enable tx command to RF
11/02/14   ap       CR612459: Deactivation and sync rx occurring simultaneously resulting in GFW crash
07/02/14   nk       CR576899 GBTA feature is not working during DSDA to DSDS fallback
05/02/14   mc       CR609473 SGLTE COEX : GAS needs to register with COEX management
30/01/14   ws       CR607097 Fix FTM mode NS GSM/GPRS call establishment
25/01/14   ka       CR607479: Block illegal access in single sim case
24/01/14  cah       CR598552: enable sleep for DSDS GSTMR+VSTMR
22/01/14   jk       CR606383 GL1 changes to support RxDiversity
22/01/14   ch       CR605045 Deactivate MDSP when processing Stop mode request
21/01/14   cgc      CR602010 TR2.0 merge fix,prevent WTOG BPLMN deactivate
20/01/14   ws       CR597962 - Non Signalling/RF cal mode changed to DSDA to allow Tx chain 2 to
                    be configured in GFW
20/01/14   jk       CR503359 - GL1 changes to support Rx diversity
20/01/14   cos      CR604531 Synchronize interrupt deregistration and ISR handling:
                    GERAN updates for treating VSTMR deregister based on the current task context
20/01/14   cja      CR599086 For all X/G transitions sleep and wake-up gstmr
17/01/14   ch       CR601467 - GSTMR handlers not reset when W request for GSM clock
15/01/14   ws       CR600584 - Corrected setting up gl1_hw_trm_rf_device for multisim
14/01/14   ss       CR600272 Deregister gstmr event from VSTRM while going inactive
14/01/14   ssh      CR597264: GSM uses RF API to choose to use RFM enter mode or VREG+WAKEUP.
14/01/14   jj       CR 600114  tx- diversity in Dime SGLTE part 2
06/01/14   cja      CR596240 Wake-up gstmr and schedule event, even if not re-initialising gstmr
03/01/14   pjr      CR586002: Modified gl1_hw_rf_map_device2_as_id to return correct as_id in dsds and tsts
02/01/14   ssh      CR594609: Add back CR588640 & CR591999, but do not do static allocation for TxLM buffers
27/12/13   sp       CR593682: Back-out CR588640, CR591999
19/12/13   ssh      CR591999: Do not deallocate RxLM/TxLM buffers during G2W TA deact
16/12/13   sp       CR587522: Configure GFW to MultiSim Sys Mode, when SINGLE SIM is inserted.
13/12/13   cja      CR589473 Add setting GfwAppModeType in GfwHostRfmEnterModeCmd
12/12/13   cja      CR585605 Only turn tx on if not already on (so do not get 2 Tx config
12/12/13   zf       CR588640: Do not deallocate RXLM/TXLM buffers when MS sleep/wakeup
09/12/13   cja      CR586971 Remove function mdsp_issue_enter_mode_cmd
07/12/13   js       CR586603 GSM has not called Enter mode
24/12/13   ap       CR 587430 - add support to pass EDGE CORE through new RF interface to dynamic programming
                                of RF HW setting per SGLTE/DSDX/SXLTE+G DSDA requirements
24/12/13   ap       CR592015: Check WTR IRAT capability before allowing IRAT measurements
10/12/13   ws       CR 587586 - Remove DUAL_SIM featurisation in API
05/12/13   mko      CR583458: Setting of tx band is not needed when moving from idle to bcch state
04/12/13    zf      CR572394: Vote RFHAL bus without TRM lock
03/12/13   zf       CR584257: Do not start G+G optimized mode immediately after multi SIM init
29/11/13   ssh      CR583144: On Rude wakeup make sure pending mcpm sleep req is done before initiating a mcpm wake up req.
29/11/13   sp       CR583330: To revert CR553516
02/10/13   jj       CR 543551 Assert cannot have side-effects
29/11/13   js       CR575998: Updating rf device id.
26/11/23   zf       CR581868: Enabled parallel RF init for G+G DSDS only
09/11/13   mc       CR572746: COEX : DSDA G+G VBATT power back-offs for TxTx collisions are not correctly applied
20/11/13   zf       CR579309: Notify FW USTMR count at OLS.
19/11/13   ssh/ip   CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
15/11/13   nm       CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
14/11/13   zc       CR569943:Use correct resource for trm_freq_request
13/11/13   sai      CR572200: Release TRM lock at G stack stop.
13/11/13   js       CR564251: change hard-coded grant to sub1 in TRM init
11/06/13   nk       CR571321: Initialize scheduler data along with FW and RF initialization
12/11/13   sai      CR 571606: Multispace G2W abort tracking variable,ensure G2W TA abort happens
                    only in W + G mode.
08/11/13   ip       CR 547955 - Use multi threads on wake from sleep
06/11/13   nm       CR566342: Release TX vote when turn TX off
31/10/13   sk       CR565410 Register MCPM Tx on call backs in multisim mode
29/10/13   sai      CR 568576: Clear RF Tx status for all Tx shutdown scenarios.
25/10/13   sai      CR 565999: Clear RF Tx status post RF shutdown
19/10/13   sp       CR562963: Skip RF Off on hw task inact, if any other sub is in transfer.
23/10/13   js       CR563463 Avoid unnecessary GTA Exit Mode request by transfer sub
22/10/12   jj       CR 554535 UE to switch to DPDT config if ASDiv not enabled on GSM
21/10/13   zf       CR564667: RF vote has not been relinquished after deactivation in DSDA mode
29/11/13   ssh      CR583144: On Rude wakeup make sure pending mcpm sleep req is done before initiating a mcpm wake up req.
03/12/13   cja      CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
11/26/13   zc       CR564251: change hard-coded grant to sub1 in TRM init
12/02/13   sai      CR572200: Release TRM lock at G stack stop.
12/02/13   js       CR564251: change hard-coded grant to sub1 in TRM init
12/11/13   sai      CR 571606: Multispace G2W abort tracking variable,ensure G2W TA abort happens
                    only in W + G mode.
26/11/13   cja      CR455527 Remove featurisation from files in API directory
11/26/13   nk       CR571321: Initialize scheduler data along with FW and RF initialization
26/11/13   npt/ip   CR582180 Enable sleep on Bolt
11/11/13   npt      CR573442 - Set force re-init flag for GERAN VSTMR when going inactive for RAT change
09/11/13   npt      CR573352 - De-register GERAN VSTMR on GERAN STOP
01/11/13   pa       CR569692: X2G via RF Task.
30/10/13   npt      CR569433 Remove FEATURE_DUAL_SIM around gl1_hw_data_init
19/10/13   sp       CR562963: Skip RF Off on hw task inact, if any other sub is in transfer.
06/11/13   nm       CR566342: Release TX vote when turn TX off
29/10/13   sai      CR 568576: Clear RF Tx status for all Tx shutdown scenarios.
25/10/13   sai      CR 565999: Clear RF Tx status post RF shutdown
11/11/13   ws       CR 573757 Featurise GSMTR-1 registers and Voice services driver
02/10/13   jj       CR 543551 Assert cannot have side-effects
21/10/13   zf       CR564667: RF vote has not been relinquished after deactivation in DSDA mode
22/10/12   jj       CR 554535 UE to switch to DPDT config if ASDiv not enabled on GSM
04/11/13   dg       CR524931 Remove extra condition related to timetick_get() api while calculating delay
04/11/13   cja      CR550457 Set RF device correct, even if not all bands supported by TRM
23/10/13   js       CR563463 Avoid unnecessary GTA Exit Mode request by transfer sub
25/10/13   ws       CR566944 Dime3.0 GTA : Sub 2 calls not going through
26/11/12   jj       CR 424199 Adding Scell measurement for tx-diversity when BA list is empty
02/11/12   jj       CR 402579 Support of Tx Diversity in GERAN.
18/10/13   sp       CR562818: Remove reset of gsac clk required at hw inits.
07/10/13   gk       CR553516 Added non blocking wait for MCPM start
04/10/13   zf       CR554234: Workaround RF vreg OFF for DSDA
08/10/13   ssh      CR554677 - XO shutdown broken in G for TR 1.1
05/10/13   sk       CR554665 Backout the power opt temperorily(CR537690)
05/10/13   js       CR541828 TR1.1 Fixing corner case where CCS state is set to IDLE in two consecutive frames
05/10/13   gk       CR536244 - Call panic reset instead of ERR_FATAL in gl1_hw_handle_sleep_panic()
01/10/13   sk       CR552259 CR sync from 1.0/1.1
01/10/13   js       CR550308  Sync CR's from TR1.0 and TR1.0.1 to TR2.0
19/06/13   zf       CR502280: To turn RF OFF when RAT change.
01/10/13   js       CR550308  Sync CR's from TR1.0 and TR1.0.1 to TR2.0
31/05/13   pg       CR460555 Enable 3GPP Rel10 RACH Power Reduction feature
27/09/13   sp       CR551478: Remove calls to CipherClk in enter/leave idle.
25/09/13   dv       CR549679 set mdsp_init_complete to avoid CCS crash
24/09/13   zf       CR537690: Reduce G wakeup timeline for MS mode
24/09/13   cja      CR520549 Convert W band from RF to SYS_BAND for TRM
09/09/13   ab       CR539370 TR 1.1 GBTA - Transfer Sub should not be given TRM grant in during GBTA Active gap.
22/07/13   ws/sb    CR516296: TRITON|DSDS|GERAN L1 changes for GTA EXIT Mode handling
18/09/13   cja      CR546816 Set MCPM Tx CB correct for gs3.
18/09/13   sk       CR545630 Ensure to relinquish RF vote when the deactivated is deactivated without TRM grantFixes
16/09/13   cs       CR503042 Updated XO RGS handling for Triton
14/09/13   ap/aga   CR545423 - G2W Tune Away Feature
05/09/13   cja      CR538140 Add mcpm_tx_on flag for race condition in sending Tx Config
04/09/13   kb/zf    CR516279: G needs to send MCPM stop request after G2W IRAT change in DS mode
29/08/13   cja      CR535578 Ensure wait for MCPM Geran start during init
29/08/13   sp       CR519537: Enable GSTMR F3s
04/09/13   cs       CR532298 Integrate all wakeup-mgr changes for Multi-SIM
15/08/13   sk       CR524039 GBTA bringup changes
13/08/13   pjr      CR521654 added critical section to protect gsm_l1_common_rfa_task_msgr_client
08/08/13   cja      CR519510 Use DPC to call MCPM, send Tx Config only when MCPM ready
22/06/13   zf       CR503348: Relinquish RF APPS vote when deactivation
14/06/13   pjr      CR490812 added function gl1_hw_get_rf_gsm_warmup_time
08/08/13   dv       CR525765 Changes for DSDS T/G IRAT bringup
05/08/13   cja      CR511042 Separate RF ID for active and CM.
05/08/13   cja      CR498800 Use TRM to get IRAT RF ID
05/08/13   cja      CR497487 New RF IRAT API to add RF ID in G2T.
02/08/13   cs       CR522369 Include residual XO drift handling in CR509747
22/07/13   cja      CR515893 Add debug for W RF device ID out of range.
15/07/13   sp       CR508123: Update panic reset to use multi-sim data space
12/07/13   cs       CR509747 Apply frequency drift to TRM_DENIED SUB(s)
11/07/13   cja      CR511042 Separate RF ID for active and CM.
10/07/13   sai/ws   CR502959: Ensure G2W search state machine not executed for non-MM sub
04/07/13   pjr      CR500712: Acquire VS when gs2 is started in Single Sim Mode
04/07/13   cgc      CR498800 Add gl1_hw_cm_trm_init() and gl1_hw_trm_grant()
02/07/13   sk       CR507513 Ensure GL1 on idle SUB sets FW APP status to idle while coming out of GTA gap
02/07/13   cs       CR507484 Warning turned to error fix
01/07/13   sp       CR508123: Set GL1 MCPM State Correctly, Use of gl1_mcpm_state_type enum
01/07/13   cs       CR506861 Further compile fix from Triton TSTS syncup
27/06/13   cs       CR505623 Compile fix for non-FEATURE_DUAL_ACTIVE
18/06/13   cja      CR498800 Use TRM to get IRAT RF ID
18/06/13   cja      CR497487 New RF IRAT API to add RF ID in G2T.
14/06/13   cja      CR490812 Remove rfgsm_get_rf_warmup_time(), use gl1_rf_gsm_timing_info.rf_warmup_time_qs
31/05/13   br       CR490883 F3 optimization
26/06/13   sai      CR503891: Allow GTA RFM enter mode only for non-stack activation scenarios.
25/06/13   sai      CR504290: Workaround G2W abort overrunning G frame tick at G stack inact.
22/06/13   zf       CR503348: Relinquish RF APPS vote when deactivation
24/06/13   sai/ws   CR502959: Ensure G2W search state machine not executed for non-MM sub.
20/06/13   sai      CR501715: Clear firmware download status at GL1 task start/resume.
19/06/13   cs       Major Triton TSTS Syncup
17/06/13   sai      CR494015: Implemented GERAN wake up cancellation
17/06/13   zf       CR496503: Request TRM when inact GSM for RF operation.
13/06/13   sai      CR495383: Add support for therm read in DSDS mode.
13/06/13   pjr      CR492594 dual dataspaced gl1_hw_gsac_init
13/06/13   am       CR381821 Always perfrom an unconditional release of TCXO
12/06/13   ip       CR359544: Fixed xCCH decodes when option 3 runs in DSDS mode
06/06/13   cs       Further TSTS Updates
04/06/13   zf       CR495879: Disable Opt 3 for dual SIM mode.
16/05/13   ab       CR487396 - Support EGPRS IR Reduced Window in GFW
02/06/13   sai      CR 481896: Ensure GL1-GFW state coherence for W2G BPLMN searches
29/05/13   cja      CR492493 Only initialise G2W driver when W neighbour in list
29/05/13   ap       CR461268: DSDS XO management + async time drift problem
21/02/13   dv       CR448609 Handle T2G HO GFW transition from MEAS mode to ACTIVE mode
23/05/13   zf       CR489054: Use new RF API to Vote RF Vreg ON/OFF.
22/05/13   ws       Support 3rd RFA UMID set for TSTS
23/05/13   ip       CR359541 Option 3 working in G+G and G+W modes
23/05/13   ap       CR491318: Reverted GFW state transitions regression 485912
22/05/13   ap       CR485912: Revised GFW state transitions
20/05/13   npt      CR458450 - Ensure that opt3 is not being processed
                    if there are IRAT activities
20/05/13   npt      CR417466 - Ensure that opt3 is not kicking in
                    if change to GSM only mode is being processed
17/05/13   ws       CR 489041 Fix compile warning with W+G build flavour
16/05/13   ws       CR488367 Ensure Flush ESAIC IIR command is not sent to GFW in DSDS mode
10/05/13   cja      CR486408 Only read and set EFS mdsp debug once for dual SIM init also.
10/05/13   ip/ws    CR486355 Ensure sleep rf vote is on when RF init called
08/05/13   cja      CR480828 Only read and set EFS mdsp debug once
26/04/13   zf       CR477992: Fix G2W IRAT issue
02/05/13   pjr      CR482754 removed gas_id from gl1_hw_efs_read_debug
30/04/13   ws       CR481359 Ensure multi_sim_deact() GTA changes are not executed
                    in DSDA mode
29/04/13   ws       CR481359 enable GTA support
25/04/13   pjr      CR480197 dual dataspaced gl1_hw_cm_status
25/04/13   pjr      CR480105 updated parameters for mdsp_dtm_reset_rx_cs_only
25/04/13   zf       CR479014 Don't turn RF vreg off in DS mode when inact task.
19/04/13   cja      CR477830 Ensure mutex lock/unlock always paired.
18/04/13   zf       CR476301: Enable DSDS MCPM state update to sleep.
18/04/13   cja      CR471261 Reset idle_temp_comp when starting GSM
14/04/13   pjr      CR475908 dual data spaced firmware queue
13/04/13   mc       CR471759 DSDS: QTA Support : Initial code changes & Triton Bring-Up
10/04/13   pg       CR469228 Avoid schedule two tx band events\
08/04/13   pg       CR467228 Do not enable Tx when leaving idle for SUSPEND
03/04/13   pjr      CR470940 moved gl1_vs_acquire location
03/04/13   jk       CR470138:Changes to lock GDRIVER mutex for gl1_hw_gfw_config_cycle_swallower( )
01/04/13   ws       CR466973 - TRITON DSDS BU, fix regression with RF cal introduced in CR467869
31/03/13   ws       CR466973 - Triton DSDS BU: Fixed stability issues with VS driver
28/03/13   jk       CR467869:Changes to send AFC command to FW in FTM mode
26/03/13   ws       CR466973 - DSDS BU, Added GERAN_TASK_ID_GL1_2 for sending tx_confic
24/03/13   ws       CR466973 - Fix F3 msgs for DSDx and fixed stability issues with DSDS BU
21/03/13   cja      CR461030 Add RF CM Enter and Exit
20/03/13   pjr      CR465451 modified init of mdsp private and public data structures
19/03/12   ss       CR465249 Disable OPT3 for PCH decode
07/03/13   dv       CR461080: Introduction of new GSM VS interface
18/03/13   ss       CR464747  Set gl1_state = GL1_MCPM_GERAN_TX_OFF while mcpm update for turning off TX
15/03/13   ws       CR463763 If DSDS or DSDA mode is not set send devmode as SINGLE SIM in GFW app mode
11/03/13   cja      CR460319 Remove EDGE core reset for Dime as MCPM Geran stop does power collapse
06/03/13   ws       CR459993 Add Devmode to GFW APPMODE command for DSDx
06/02/13   jk       CR449735: Resolving KW errors
05/02/13   og       CR433989. Speed up gl1 hw init for T2G TC 4.3.9.5.
31/01/13   cja      CR446356 Set msgr_init_hdr 'from' parameter as MSGR_GERAN_GL1
31/01/13   pg       CR446533: PGSM no longer a valid band type for TRM
23/01/13   jk       CR427351: passing pointer to the thermistor read CCS event buffer to RF
23/01/13   jk       CR431540:Code changes to call RF for exit mode
22/01/13   cja      CR443834 Extend mdsp_awake to indicate FW enable/disable
07/02/13   pjr      CR449199 -  Changed gl1_hw_rf_map_device2_as_id to return the correct as_id
                    based on RFM_DEVICE passed in ftm_mode. Removed feature FEATURE_GERAN_DIME_XPLUSG
                    around ftm_device to use same mechanism with TRITON modem
06/02/13   ws       CR449681 - Don't register TCXO manager on DIME X+G build we are GSM only
01/02/13   ws       CR448039 -  Register with TCXO manager for Multimode subscription and
                    when not in FTM mode
17/01/13   ws       Now send RF driver UMID Sets based on gas id rather than device
16/01/13   ws       Added RFA SUB2 UMID's
04/01/13   cja      CR437603 Remove use of AsyncImmediateCmd, always use AsyncCmd
17/12/12   og       CR413328. Ensure the edge core reset in GSM L1 task does not overlap
                    with the GSTMR HW access in IST29.
14/12/12   cja      CR426248 Wait for HW Tx Off before MCPM request for voice stop.
10/12/12   ws       CR426795: do not turn Tx on if leaving idle to deactivate
05/12/12   pg       CR427361: GFW/RF shared memory interface cleanup
05/12/12   pg       CR426795: do not turn Tx on if leaving idle to deactivate
30/11/12   pg       CR426676: incorrect message entry in MSGR mailbox list
26/11/12   ws       CR424198 - API backward compatible with TRITON DSDA changes
22/11/12   cja      CR414098 For Dime only need W clks if W neighbours
22/11/12   jj       CR407050 - mdsp_load_nv()  is not called in   GL1
09/11/12   pg       CR415749: call gl1_hw_turn_tx_on directly if running in OPT3 ISR
26/10/12   npt      CR413164 - Make sure rf vreg off/on calls do not get preempted by MCPM
23/10/12   pg       CR409233: use RF Task CCS buffer implementation for all X2G measurements
23/10/12   npt      CR413635 - Port sleep code for DIME modem
24/09/12   ky       T2G Async message router changes
04/09/12   cja      CR385323 If gsmtr stalled then disable FW, reset gstmr, re init FW
21/08/12   pg       AFC calculation update for 29 bit rotator on Dime
02/08/12   ky       TRM changes for dime
25/07/12   ky       Moving the message router initialisation for blocking messages to
                    l1 task initialisations
03/07/12   ky       Async message router sleep, app mode, enter mode interface changes
20/06/12   ky       Added support of using new CCS interface and passing new CCS
                    buffer pointers in RF task messages
09/07/12   cja      CR377173 Set initial XO value for acquisition (for blind CGI).
05/07/12   cja      CR375634 Reset gstmr for CM after GSM deep sleep inactive.
05/07/12   npt      CR370974 - Do not process async interrupt if Tx is on
04/07/12   pa       CR376123: G2T ensure GL1 shoudl not send extra cleanup while leaving GSM mode.
19/06/12   cs       CR370453 Use the correct wait sig for the TRM/TCXO Manager timeouts
08/06/12   pa       GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
08/06/12   cja      CR363269 Do not clear IRAT SBI/NonSBI buffers
31/05/12   cja      CR366518 Correct g2t startup/cleanup time decrement
25/05/12   pg       Add support for RF Task
23/05/12   cja      CR363510 In FTM remove race condition for sending Tx config command
02/05/12   cja      CR351986 - Add featurisation (FEATURE_XO_FCR) for new API
29/04/12   cja      CR351986 New API for RGS update
25/04/12   ws       CR 355838 FR 1604 - GERAN changes to cleanup WCDMA- GL1 API
19/04/12   cja      CR349237 Add API for FTM to reset edge core
21/03/12   ws       CR 345606 Improve stability of X2G Rxfe stuck recovery
30/03/12   cja      CR346018 RxFE recovery in FTM
21/03/12   jj       CR340963 Avoid scheduling g2x gap when frame is slammed
29/03/12   cja      CR341225 RxFE recovery for X2G
27/03/12   cja      CR338055 Recovery for Nikel RxFE Stuck
26/03/12   jj       CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
26/03/12   pg       Make FEATURE_MCPM and FEATURE_DYNAMIC_CLOCK_VOLTAGE_SCALING mutually exclusive
19/03/12   pg       Dime modem specific HWIO implementation (no TCXO register support)
14/03/12   pg       Add support for local HWIO macros (msm.h deprecated)
14/03/12   pg       Add support for DAL interrupt controller (tramp deprecated)
05/03/12   cgc      CR339393 add gl1_clkdata_state_init()
10/02/12   ws       CR335103 - Reset medium access mode to Fixed allocation when leaving
                    idle mode. Avoids false HMSC mode detects in dedicated mode.
13/03/12   ws       CR335057 eSACCH buffer flush and restore after inter-cell handovers
15/02/12   dv       Revert change providing hard coded value for gl1_hw_cm_cleanup_cmd_duration_qs
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
10/02/12   ws       CR 336192 - Removed Duplicated lock to gl1_task_mutex_lock()
02/02/12   cja      CR329327 Ensure mdsp init complete before using command buffer
03/01/12   og       CR328336. Expand CR307591 to cover cm init and gl1 task deactivation.
02/02/12   npt      CR326943 - Clear mdsp command buffer on wake up from sleep.
01/02/12   cja      CR327261 Always call rfm_power_clock_off if inact (not just deep sleep)
27/01/12   jj       HMSC generic config command enable
27/01/12   ky       Added mechanism to disable/enable vamos in firmware
20/01/12   npt      CR323780 - Modify warmup time and OLS values when low monitor mode is active - Fixed compilation error
13/01/12   ws       Added Vamos Feature
12/01/12   npt      CR323780 - Modify warmup time and OLS values when low monitor mode is active
12/01/12   npt      CR311544 - Added sleep debug structure
11/01/12   npt      CR315272 - Remove usage of deprecated legacy sleep APIs
07/11/11   jj       CR315253 IQ capture capability for GSM via QRCT
13/12/11   cja      CR325153 Always request MCPM update for Tx on
08/12/11   cja      CR323200 Run MCPM start req via dpc, wait for it to complete
09/08/11   ap       DSDS CR299284 - correction to PDM backup value management
07/11/11   ws       CR 313986 - Add gl1_hw_sleep_assert_okts() to RAT_CHANGE_IND
31/10/11   cja      CR315238 Use rfm_power_clock_off for power collapse when deep sleep inactive
28/10/11   jj       CR308254 Correction to RF interface for the dc spur removal
26/10/11   cja      CR310844 Fix compiler warnings
25/10/11   jj       CR310844 Store RFM IRAT timing variables in GL1
20/10/11   cja      CR313577 Ensure WSTMR on until W requests it for G2W handover
03/10/11   cja      CR310347 Use dpc for MCPM geran stop request
29/09/11   cja      CR309552 Always request MCPM Geran Stop for L1 inactive
26/09/11   cja      CR308551 Call MCPM for hw Tx on in idle, use dpc for MCPM updates in DRX
21/09/11   cja      CR307155 Use FW cleanup overhead for IRAT timings.
20/09/11   cja      CR305705 Set GSMTR DCO on gsm init
19/09/11   og       CR307591. Ensure the deactivation and startup don't overlap at the
                    task level.
08/09/11   jj       CR302378  Added GL1 changes for RSB along with spectral inversion
05/09/11   cja      CR305056. Increase clock speeds for non-signalling mode (FTM)
22/08/11   cja      CR302378. Add API for RF to set spectral inversion per band
26/08/11   og       Port over CR286561.
23/08/11   cja      CR302999 Always allocate TxLM buffer on hw init (even for Rx only)
16/08/11   cja      CR302396. Always request MCPM W neighbour so WSTMR on for sleep
10/08/11   cja      CR301047 Correct MCPM state when Hw Tx Off
05/08/11   ws       CR 299800 - Added temporary workaround to enable STMR for
                    WCDMA sleep controller
20/06/11   cja      Remove Nikel bringup workarounds
15/06/11   cja      Fix compiler warning
14/06/11   cja      Nikel W2G changes (so WFW cleanup)
10/06/11   ws       Changed FW_APP_DISABEL from WCDMA to GERAN
06/07/11   cs       DSDS CR295010 Apply the correct afc adjust between G+G
29/06/11   ap       DSDS CR293360 Eror in Dual SIM Mode HW State Ordering
27/06/11   cs       CR289602 Clear pending RX at sleep for DSDS G+G mode
23/06/11   npt      CR291619 Releasing GSM_EBI_BUS client
15/06/11   cja      CR264690 Correct error in merge
14/06/11   cja      CR264690 Acquisition frequency capture range larger & add multipass for XO
07/06/11   cja      CR290473 Ensure SAIC is enabled when leaving idle mode
04/05/11   cs       DSDS CR286518 always disable G2W Init for C+G/G Only builds
28/04/11   pg       Fix compiler warnings
11/04/11   nf       CR280207 added FEATURE_GSM_OFFLINE_CLK_SCALING
08/04/11   ip       CR282971 GP clock vote updates
08/04/11   pg       Change #if to #ifdef for GL1_9K_2P4_GSM_OFFLINE_CLK_WORKAROUND
27/01/11   nt       CR272422 DSDS remove ERR_FATAL after only one mDSP load attempt
29/03/11   cs       DSDS CR281877 Disable the G2W init for C+G builds
18/03/11   sjw      DSDS CR279820: remove dual SIM mode check from gl1_hw_mdsp_inact()
17/03/11   nf       CR277880 added additional for only running opt3 in Idle
03/02/11   ky       266163:Reverted CR261247 fix. Added three frame delay before deactivating the
                    FW during the panic reset procedure to allow fw to process all the commands
                    and ready for deactivation. Also removed sleep during the inactivation procedure
03/03/11   pg       Remove target specific hardcoded feature definitions from source files
26/02/11   ws       Corrected NPA API's for gl1_hw_multi_sim_fw_rf_init()
22/02/11   ip/cs    CR267278 GP clock vote handled by RF drivers
21/01/11   ws       CR 263081 - Implement NPA Latency node of 1mSec
08/12/10   ws       CR 267264 - Merge NPA clients for CPU resource to 1 client to avoid
                    mutiple npa_cancel_request() which can take 1-2 mSecs to return
09/02/11   cs       CR274275 Always enable GP clock at gl1_hw_init startup to avoid crash
                    when we access the EDGE_CGC_ENA
04/02/11   cs       Remove unnecessary functions left over from 264682 updates
26/01/11   pg       Added FEATURE_GSM_GP_CLK_DISABLE for sourcing GSM clock
                    from MSM in all modes i.e. GP_CLK always disabled
26/01/11   pg       Change control of EDGE CGCs to be via ARM, not MDSP for Pegasus
25/01/11   ws       CR 266163 - Remove call to mdsp_go_to_sleep(gas_id) since this is called
                    After GSM APP has been disabled.
25/01/11   nt       DSDS release both chains upon lock and FW release
21/01/11   cs       CR264682 DSDS Add support for asynch dsp completion interrupt
05/01/11   nt       DSDS - Allow the mdsp shutdown for dual sim modes
21/12/10   cs       Remove unnecessary F3 and PDM setting on XO targets
20/12/10   nf       CR267846 removed !Q6 around rfm_power_clock_off() in LPM
16/12/10   cs       DSDS - Update for 16-bit PDM support will need other updates
01/12/10   cja      CR258384 Update afc_pdm_val to current TXCO manager setting at init.
09/12/10   dv       CR261247 - Avoid mdsp_disable(MDSP_APP_GSM) during panic reset
09/12/10   cs       CR264682 Add support for asynch dsp completion interrupt
02/12/10   nf       9K changed the way clk delays and clk switching is handled
30/11/10   ab       Support EGPRS WS REDUCTION during allocation of dynamic memory in start
                    gsm mode request when FEATURE_GSM_EGPRS_IR_OPT/PER TBF is defined.
22/11/10   og       9K GSM OFFLINE clock workaround for the 2.4 line
18/11/10   nf       9K added handling for clock speed based on efs value
17/10/10   pg       Add missing featurisation around calls for reg/dereg cpu resources
12/10/10   pg       CR263162: Mitigate possibility of GFW interrupts being disabled across a
                    frame boundary during GSM clock source switching on 9K
29/10/10   ky       261817: Added two wait for frame tick in the hw init so that
                    app mode command sent to the fw and fw will process that command
                    Hardcoding the app mode command moved to switch command buffers function
25/10/10   ky       259645: Installed gstmr handler for sending pending commands is not working.
                    Hardcoded the sending these commands in gstmr isr function.
25/10/10   ip       CR261318 Cancel dedicated NPA resource when going inact.
21/10/10   tjw      Replace 2 missed deprecated clk_busy_wait with DALSYS_BusyWait
18/10/10   ky       259645: Installed gstmr handler for sending pending commands. This handler
                    sends the generic config command and app mode command to fw if they are
                    pending
07/09/10   pg       Fix compiler warnings
13/10/10   cja      Fix LINT warnings
14/10/10   ws       FEATURE_GSM_MDSP_AEQ_8PSK - support AEQ Ph2
09/10/10   nf       9K added request for max clock - FWS suspend/resume
07/10/10   ky       254466:Removed fw unused shared variables from the sw code
05/10/10   cs       Ensure rounding in hztopdm and remove overflow in hz_to_ppm
23/09/10   tjw      Replace deprecated clk_busy_wait with DALSYS_BusyWait
03/09/10   ws       Added FEATURE_GSM_MDSP_AEQ_SCH for QDSP6 targets
03/09/10   cs       CR254295 Fix the TX rotator freq error sign for non-XO
06/08/10   pg       Additional F3 for monitoring GSM clock source switching
05/08/10   nf       CR248504 added back in check code prior to GFW APP Disable
04/08/10   cs       CR229805 Fix F3 for 9K with OVERRIDE for source
26/07/10   pg       CR248078: Avoid GSM clock off race condition with MDSP
27/07/10   pm       Featurised TREK changes(CR244447) using FEATURE_GSM_DCDC_VOLTAGE_CONTROL
23/07/10   ip       CR245474 Added more performance logging
23/07/10   ws       Added FEATURE_TMC_TCXOMGR for 8660 target
20/07/10   pg       CR247250: Force GP CLK on and switch GSM clock src to SWC
                    before calling mdsp_init
14/07/10   pg       Add mutex lock around SWC config, as GSM clk is disabled
                    during switch from MND to SWC
12/07/10   pg       CR235492: Switch from MND counter to SWC on entering GSM mode.
08/07/10   cja      CR244447. Add setting AGC_ENA for Trek
01/07/10   nf       Enabling support for 9K RF Power Collapse
22/06/10   cja      CR241259 In acq txco init if frozen use rotator
18/06/10   ip       CR241108 add funtionality to enable/disable SSBI clk
08/06/10   cs       CR241881 Only allocate NPA data structures once
04/06/10   cs       Lint/KW fixes
04/06/10   ip       CR238553 Turn GP clock off when going into LPM
23/05/10   dv       fix compile error in 9k irat update
20/05/10   og       9k irat updates.
07/05/10   ip       CR235204 DCVS settings added for Poseidon 2
27/04/10   cs       CR236460 Latest AFC\XO handling updates
14/05/10   pg       Add missing FEATURE_GSM_GPRS_GENESIS around call to
                    mdsp_set_gsm_app_mode(gas_id)
07/05/10   ip       CR235204 DCVS settings added for Poseidon 2
07/05/10   nf       9K changes for Clock Gating
06/05/10   pg       Set GFW App mode to ACTIVE during init due to FW CRM feature
16/04/10   cja      Add intial set-up of cycle swallower
09/04/10   cja      Remove unecessary error message.
06/04/10   cja      Tidy-up setting of mdsp feature enable and match DSP functionalty.
26/03/10   cs       Always start with the GP clock on
24/03/10   ws       CR 231507 - Removed 9K Bringup code for MODEM_CLK_MISC_CTL_2 and
                    GRFC_PART_RT_CONFIG as no longer required.
24/03/10   ip       CR231770 move the testing of GP clock
22/03/10   nf       Enable Sleep for 9K/GENESIS
22/03/10   ip       CR 230249 Ensure tripler is on !!
18/03/10   ip       CR230381 Removed turning OFFLINE clock on for POSEIDON
12/03/10   cs       CR229926 Improve NPA robustness
12/03/10   nf       SBD:Added second null burst pattern
04/03/10   ip       CR228168 Added DEVMAN code changes for mDSP speed
01/03/10   cja      Remove workaround so cipher clock can be disabled when not required.
15/02/10   cja      For SCMM temporarily remove disabling cipher clock.
04/02/10   ws       Removed leagcy Q4 MDSP services and image for Q6 targets
08/01/10   ws       Removed calling mdsp_hw_gen2axi_init(gas_id) for 9K as A2 drivers
                    now configures this.
23/12/09   ws       Added volatile type for accessing pMdspHostIfBaseStore
14/12/09   ws       Merged AFC changes from MUSTANG to use AGC_AFC_CARRIER_FREQ
                    register correctly
08/12/09   ws       Setup Routing matrix to configure TRK_LO_ADJ
                    on GRFC_0 for GENESIS
25/11/09   ws       Added FWS App enable for QDSP6 and removed mdsp_read code
                    from gl1_hw_mdsp_inact()
24/11/09   tws      Use HWIO_PHYS macro to pass physical address to MDSP.
20/11/09   ws       Add MODEM_CLK_MISC_CTL_2 register overide for 9K bringup
20/11/09   ky       calling mdsp_hw_gen2axi_init for GENESIS target
13/11/09   ky       Change the reg address from virtual to physical in Q6 mdsp_nonsbi_write function call
08/02/10   cja      Update XO
17/12/09   cja      Correct rounding in hztophase and phasetohz for negative conversion factor.
10/11/09   ws       Merge Mutex changes from Mustang baseline
04/11/09   ip       Added first draft of NPA for cpu clock resource
09-10-22   tjw      avoid compiler and lint warnings about unused variables when MSG
                    is defined away to nothing. Complete POSEIDON featurisation of
                    gl1_hw_mdsp_write_reg()
05/10/09   cs       CR190851 GL1 needs to disable DC HW block before loading DSP image and
                    re-enable it after DSP loading to fixes the two spurious SSBI when DSP enabling
                    UMTS RXF
17/09/09   nf       Removed unused APIs
28/08/09   ws       Added if !defined GENESIS modem around MODEM_MDSP_IRQ_SRC_SEL
21/08/09   cs       CR154851 Fix the RX Sweep for newer targets to properly cleardown
31/07/09   ws       POSEIDON - Setup MODEM_MDSP_SRC_SEL register for GSM MDSP IRQ.
                    Workaround will move to Core BSP at some point
23/07/09   ws       Updates to support GENESIS Modem core
16/06/09   ip       Added clock wait to resolve some Poseidon SURF issue
                    - to be removed later
26/06/09   sjw      Remove use of RR public store from L1 (for GCF NV flag)
24/06/09   nf       SBD: added new threshold function
16/06/09   ap       Fix Lint/compile warnings
09/06/09   nf       Added FEATURE_GSM_PCH_SINGLE_BURST_DEC
26/05/09   cs       CR167848 fix for expanded afc_freq I/F to mDSP
29/05/09   ws       FEATURE_GSM_MDSP_AEQ phase 1 for all GMSK bursts
28/05/09   cs       Updates to avoid spurious restriction update when TCXO released
20/05/09   cs/gfr   Add in changes for HW related definitions removed from hw layer
07/06/09   cs       Fix Lint/MOB errors
06/05/09   ip       Removed functional call not required by Poseidon
05/05/09   cs       One more update for CR177277 to use global feature
30/04/09   cs       Update to CR177277 fix to avoid unnecessary debug print
29/04/09   ws       Fixed Lint Warnings for MOB environment
28/04/09   cs       CR177277 Call tcxomgr_set_polarity api for multimode targets
17/04/09   cs       Always clear wait signal before NV reads
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
27/03/09   cs       CR176862 Add the mdsp_go_to_sleep in LPM due to mDSP services
                    changes that now issue a sleep vote for sleep
26/03/08   ws       Added cleanup for Poseidon Targets
13/03/09   og       FEATURE_XO updates.
12/03/09   og       FEATURE_XO support
26/02/09   cs/hy    Add initial XO support for handling common XO and Rot
21/01/09   cs       Add in Mustang support for using chosen sample server
23/01/09   ws       Corrected Featurisation for gl1_hw_tx_on
17/01/09   cs       Mustang changes to disable sleep
16/01/09   agv      CR169242 - Zero the Tx_Xo_Freq_Err on LCU when gl1_hw_init
                    is called, else Tx Xo Freq Err from WCDMA is set for GSM Tx.
15/01/09   agv      Mustang loading of the GSM mDSP via gl1_hw_mdsp_init
                    via call from gl1_hw_cm_gsm_init.
11/01/09   agv      Mustang Tx DPLL added as the target does not have a Tx Rotator.
07/01/09   agv      Change for Mustang GSM Offline clk switch on.
19/12/08   ip       CR167592 Added additional LCU clock switching
04/12/08   cs       CR166349 Remove the enabling of DIV2/4 in TCXO control code
04/12/08   og       Mustang G2W IRAT integration changes.
27/11/08   cs       Lint fixes
25/11/08   cs       Fix IQ logging for AMR WB channels as 8 frame interleave
24/11/08   ws       Corrected MUSTANG featurisation of TRM
21/11/08   agv      CR164875-Resolved High Transmit Frequency Error in EGPRS
14/11/08   ws       Added inclusion of Tramp.h for consistency with clockregime API
11/11/08   ap       Fix Lint compiler warnings
11/11/08   ip       Added Mustang feature to LCU sleep changes
07/11/08   kps      Remove dependency on tcxo_srm if we're using TCXOMGR30
04/11/08   ip       Added LCU clock regime changes
03/11/08   agv      CR162513 - Make sure the UL Timeslot is set when GDM is
                    initialised, and for WtG Handovers is Power Backoff NV is set.
30/10/08   ws       CR 162112 - ESAIC Only reset IIR filter on PCH and NCELL BCCH
15/10/08   og       Ensure tcxgomgr_init is called before requesting it.
01/10/08   cs       Remove the setting of rxf0/1 pads for 1105 target as F/W exports vars
23/09/08   cs       CR149660 Improve CORE_LPM to work for UMTS targets as well
17/09/08   ws       Backed out leaving MDSP clock on at 122 Mhz for LCU since
                    it's not required for B0
15/09/08   ws       CR153489 changed gl1_hw_lpm_gstmr_frame_wait() to avoid infinite loop
                    Only affects targets that define FEATURE_GSM_L1_IMPLEMENT_CORE_LPM.
15/09/08   cs       CR143326 7500-90 LPM wasn't disabling EDGE_CLK or PC_DAC_CLK
15/09/08   cs       CR141866 Add in a new special LPM for 7500 that doesn't touch mdsp, rf, trm etc. for GPS
15/09/08   ws       CR 144248 Enable and disable ESAIC in gl1_msg_set_egprs_mode() and
15/09/08   cs       CR130550 Add in the ability to request a higher/lower clock speed if req'd
                    under the FEATURE_GPRS_USES_HIGH_ARM9_CLOCK
03/09/08   agv      CR152837 - Added support for the new clk_regime API for SSRV0 on LCU.
04/09/08   nf       CR154392 - Clear pending W ISRs after disabling the WSTMR clock
03/09/08   cs       Add clock debug for CLK_HALT_2
28/08/08   cs       Implement the Fast Rx Sweep functionality for FTM
19/08/08   og       Mustang bring-up changes.
22/08/08   nt       Removed compiler warnings
05/08/08   agv      Added changes to mainline to allow A1/B0 to configure SSRV correctly.
28/07/98   hv       Added KxMutex support
25/07/08   nf       CR151098 - Disable Div2 at the end of PCH reception
18/07/08   nt       CR151852:- Changes for WPLT builds
17/07/08   nf       CR147813 - set gl1_hw_gsm_acch_nv_word to 0x01 by default
09/07/08   nt       CR147984 - prevent afc_pdm_val getting set to HW_MDSP_TCXO_PDM_MIN
02/07/08   nt       Within gl1_hw_acq_tcxo_and_rotator_init for TCXO mgr 3.0 the default value
                    needs conversion from gsm2shared units before use.
30/06/08   cs       CR149200 Invert PDM/Rotator sign under FEATURE_UMTS_NEGATE_ALL_TCXOMGR_VALUES
27/06/08   ip       CR144361 Move TCXO PDM clock control to tcxomgr under FEATURE_TCXOMGR_TCXO_PDM
27/06/08   nt       CR147984 - REV2:- Final changes for TCXO reset
23/06/08   nt       CR147984:- added access function gl1_hw_shared2gsm
16/06/08   nt       CR147984:- Changes for TCXO reset.
19/05/08   cs       Klocwork fix for band array index checking
09/05/08   ip       CR142792 Reduction in GSM Awake time line
01/05/08   cs       Fix IQ for E/A/FS to ensure sync works correctly, otherwise misses frames
30/04/08   ws       LCU Hack for SSRV; must be remove for B0 Change speed to 100MHz
                    if it is in EGPRS
29/04/08   cs       Fix the IQ log DPC to avoid double calls to log data
21/04/08   ws       CR 144248 Enable and disable ESAIC in gl1_msg_set_egprs_mode() and
                    improve F3 for SAICS status
09/04/08   ip       Added LCU GSM sleep clock control
07/04/08   cs       Improvements to get effective QXDM IQ logging
27/03/08   nf       Added NV Item handling to R-SACCH and R-FACCH
14/03/08   ws       stability issues for LCU with gl1_hw_disable_clk_resource for GSM TX
07/03/08   agv      Fixed Poke Interface for LCU target.
10/03/08   cs       CR137521 - Fix the DIV2 clock changes to be gated by gsm_only to avoid
                    breaking the W timeline when they are SUSPENDED eg BPLMN etc
07/03/08   cs       CR139697 - Fix the Dora RSSI=0 problem with direct clk_regime call
03/03/08   cs       CR137521 Enable the WSTMR clock when we access the TRK_LO_ADJ
28/02/08   agv      LCU Sample Server was not setup with new clk regime. Some init in gl1.
25/02/08   agv      LCU Sample Server moved to clkregim, and switch on Offline Clk.
27/02/08   cs       Add access functions for g2wIsrLockOut manipulation
14/02/08   cs       CR136944 Use the new g2wIsrLockOut to ensure no mdsp stall when
                    disabling the WSTMR in dedicated and remove ANT1 grounding
07/02/08   ws       Removed Flipping of ESAIC FEature support bit so it is now ON by default
05/02/08   ws       Place FEATURE_GSM_DISABLE_CHIPXN_DIV4 around disabling CHIPXN_DIV_4
                    clock just in case STMR_MISC_256 interrupt happens causing MDSP stall
14/01/08   agv      New feature for Switched Mode Power Supply which will
                    provide better talk time.
14/01/08   cs       Allow for separate IQ logging and saving
11/01/08   cs       CR134941 - Update for 6800 to enable the RX front-end clock at init
14/12/07   ws       merged LCU Bringup and demo changes from 6260 branch
13/12/07   cs       Add back the full CHIPXN_DIV2 support for targets that need it
03/12/07   cs       Fix CR 128185 to compile for targets that dont't support
                    ANT1 gating
24/08/07   agv      Backout CR124272 since MDSPtask optimisations are not pulled in
27/11/07   cs       Fix the IQ logging data for 8 block interleaved data
21/11/07   agv      CR132519 - Fix for Tcxomgr 3.0 PDM return value.
19/11/07   cs       Safety code for missing clock enable statement but clock disable present
09/11/07   nf       Added FEATURE_GSM_R_SACCH
08/11/07   cs       Remove the use of the resource manager for MDSP clock control
                    and add the pdm invert feature
01/11/07   og       Fixed the #ifdef HW_TCXO_PDM_SIGNED to be #if.
29/10/07   cs       CR128185 To ground ANT1 at startup and update to new async
                    mDSP interface CR117937
22/10/07   ws       Add support for ESAIC NV 2508 item control
25/10/07   cs       Remove dependency on l1_sc.h for gsm_only indication for CR117884
21/09/07   cs/ip    CR123278 - Combined GL1 clock control, CR117477, DIV4 control
22/10/07   ws       CR130984 - Enable SAIC in IDLE mode as this improves performance with
                    negligible current degredation
17/10/07   ws       Resolve compile error in previous checkin
02/10/07   agv      CR129426 - New Systems SAIC change from -103 to -107 dBm.
17/09/07   agv      CR128556 - Systems change for SAIC threshold from 102 to 103.
17/09/07   agv      Added I/Q Logging to code base.
05/09/07   cs       Reinstate CR122838 as fixed by calling after rf_init
24/08/07   agv      CR124272 - Always force the mdsp to be reloaded
14/07/07   agv      CR123477 - With stuck system time the TCXO can be
07/09/07   cs       CR122838 reduce GSM sleep current
16/08/07   ws       Corrected Featurinsation for disabling FEATURE_GSM_TCXOMGR30
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
01/08/07   cs       Updates for featurisation to support 7500-90
23/07/07   cs       CR124272 - Always force the mdsp to be reloaded and reset
29/06/07   cs       Fix featirisation for FTM
11/06/07   cs       Fix for FTM TCXO release in sweep test
08/06/07   cs       Fix featurisation for CGPS FEU code
29/05/07   ws       Support for CLKREGIM changes for FEATURE_CLKREGIM_MDSP_SPEED_LOW
                    added registration and deregistration of GSM's 60Mhz resource
16/05/07   cs       Improvements to manage state change better for feu
04/05/07   cs       Updates for TCXO restriction support and rotator push
30/04/07   ws       Changes to Feturisation for first pass GSM Only build
12/02/07   agv      Added TCXO Mgr 3.0 Rotator Push for GPS.
06/02/07   cs       Fix compile if DTM not supported
23/01/07   agv      Switch SAIC off if Idle Mode to reduce current draw.
19/12/06   cs       Extra fix for IQ logging, avoids disabling active channel
04/12/06   cs       Modified trm for ftm use
04/12/06   cs       Updates to tcxo manager
28/11/06   ws       High Lint warning fixes
14/11/06   agv      New clk_regime_config_gsm_clocks save 4-6 mA per rx chain by setting
                    the ADIE clock source to ground when an rx chain is not in use.
10/11/06   ws       new rfm interface for msm7600 protected by FEATURE_MULTI_RX_CHAIN
                    to allow backward compatibility with current interface
23/10/06   agv      Fixed a crash for TCXO Mgr 3.0 when release to WCDMA occurs.
23/10/06   ws       Changed hw_set_system_mode() to use HW_SYSTEM_MODE_GSM
20/10/06   ws       Ensure hw_set_system_mode() is compatible with msm7600 and msm6280
19/10/06   cjs      Revisist the IQ logging chamges for L4 to fix build
18/10/06   agv      Added TRM, TCXO MGR 3.0, and new hw_set_system_mode for 7600.
13/10/06   cjs      CR101170 Fix the IQ logging for MDSP_SAIC and IQ_LOGGING
21/09/06   og       Reset only the cs part of lower layers when connecting DTM
                    assignment in packet transfer (EDTM route).
14/08/06   ws       Removed Registering and de-registering the EDGE clock in
                    gl1_hw_enter_idle() and gl1_hw_leave_idle()
11/07/06   cs       CR104631 - Disable CHIPXN_DIV2 clk regime in GSM TCH mode on 7200
06/26/06   gfr      Move disabling of clocks to task inact to make sure
                    everyone is done using them.
06/14/06   ws       Temporarily removed static around gl1_hw_feature_disabled_saic()
05/18/06   gfr      Support for clock regime resource manager.
05/16/06   pp       Support for new rfm_sleep() interface.
27/03/06   og       Added an F3 HIGH MSG to printout FW SAIC support information.
03/16/06   gw       Added support for TCXO manager active clients under
                    FEATURE_TCXOMGR_ACTIVE_CLIENT.
03/14/06   ws       Removed the rfm_enable/disable_tx function call featurized under
                    FEATURE_MULTI_RF_CHAIN_API
03/08/06   gfr      Mainline FEATURE_GSM_TCXOMGR_SUPPORT, fix bug in determining
                    and setting initial PDM value when switching from ARM
                    to mDSP mode.  Mainline FEATURE_MDSP_MEM_DUMP_ON_ERROR
03/01/06   gfr      Invert the phase increment programmed to the tx rotator
02/17/06   ws       Mainlined SAIC, feature is now on by default.
01/17/06   gfr      Put initial PDM delta on the rotator if the PDM is frozen
                    to avoid unnecessarily unfreezing it.
01/13/06   gfr      Support for new RFM API (FEATURE_RFM_INTERFACE)
12/12/05   gfr      Set aciThreSR to -8 to improve performance
12/08/05   gfr      Modify gl1_hw_save_iq_samples to work with L4
12/02/05   gfr      Leave tx clock on all the time in FTM mode
11/29/05   gfr      Fix enabling/disable of SAIC through the NV item
11/16/05   gfr      Fix typo - renamed mdsp_saic to mdsp_enable_saic
11/04/05   gfr      Reduce USF tx threshold for 8PSK from 3 dB to 0 dB.
10/27/05   gw       Fixed rf sleep / deep sleep featurization.
10/25/05   gfr      Make feature disabling more generic, add support for
                    disabling SAIC
10/18/05   gfr      Use HWIO_RMSK(EDGE_CGC_ENA) instead of magic 0xF to support
                    targets with different EDGE_CGC_ENA register bitwidths
10/17/05   gfr      Add support for GSAC module, disable encryption clock after
                    init, remove some grfc config already done by RF driver
10/03/05   gfr      Make hztophase and phasetohz calculations
                    resolution-independent using HW_PHASE_INCREMENT_RESOLUTION
09/30/05   gfr      Optimize GSM tx clock on period
09/30/05   gfr      Disable ACI detection at startup if flag is set
09/29/05   gfr      Call gl1_hw_sleep_gsm_panic_inact here instead of l1_task.c
09/22/05   gfr      Removed FEATURE_SBI_MDSP_CONTROLLER
09/22/05   gfr      Only call RFGSMCTL_ENA_GSM_GRFC() if it is defined
09/20/05   gfr      Save 8 frames of IQ samples for TCH full-rate
08/30/05   gfr      If DTM is defined register for EDGE CPU resources when
                    leaving idle mode since we cannot change mDSP clock speed
                    during a voice call.
08/19/05   gfr      QLint cleanup
07/20/05   pp       Renamed gl1_hw_handle_mdsp_panic() to gl1_hw_handle_panic()
                    to allow NPL1/PL1 to call PANIC
07/07/05   gfr      Remove obselete nonsbi_write to the channel filter
07/05/05   gfr      Skip mDSP tx phase increment write check when disabling tx
                    if we are inactivating
06/27/05   gfr      Abort gl1_hw_turn_tx_off if we have a pending mdsp nonsbi
                    write to a register that depends on the tx clock.
                    Add channel type to saved debug buffer.
06/16/05   gfr      Added gl1_hw_set_iq_samples_tn
06/10/05   gfr      Move gl1_hw_turn_tx/rx_on/off and disable tx clk regime
06/08/05   yh       Remove call to start mdsp speed at 50MHz. This is done by
                    default in clkregime.
05/25/05   gfr      Make IQ samples saving work in PLT as well.
05/25/05   gfr      Ensure gl1_hw_inact_isr runs atomically during panic inact
05/25/05   gfr      Add channel-specific IQ sample saving enabling
05/17/05   yh       Use channel filter stage2 as default.
05/05/05   gfr      Ensure the output of stage3 is configured
05/04/05   gfr      Add extra delay to mDSP proc trigger
05/03/05   pp       Changes to support 14.10 test case
04/28/05   gfr      Added mdsp_inact
04/25/05   gfr      Temporary hack to force rxBeta to zero.
04/22/05   gfr      Reset mDSP after a panic
04/21/05   yh       Support for panic reset
04/20/05   yh       Remove use of RF fifo
04/14/05   yh       Add mDSP optimization
04/08/05   gfr      Move logging code into gl1_hw_log.c
04/08/05   gfr      Retry mDSP download if it fails
04/07/05   gfr      Treat PDM as signed for target independence
03/18/05   hg       Fix compile issue for WCDMA PLT
02/25/05   gfr      Fix compile issue when rotators are disabled.
02/24/05   yh       Add a parameter to print sleep message only on the third
                    try for a NACKED error.
02/15/05   gfr      Only write to the tx rotator in GSM mode.
02/02/05   gfr      Support for NV based disabling of IR and ACI detection.
01/28/05   yh       gl1_hw_init() takes 2 signals in the parameter and pass them
                    to mdsp_download_and_startup()
01/12/05   gfr      Write 8PSK and GMSK USF SNR thresholds to firmware.
01/10/05   gfr      Pass task_ptr to tcxomgr init and support FEATURE_TCXOMGR_TMC.
04/01/05   gfr      Round negative rotator values correctly when changing bands
12/21/04   gfr      Make IQ logging work when enabled in the middle of a block.
12/14/04   gfr      Write to the tx rotator if it exists.
12/07/04   yhong    Lint cleanup
12/06/04   gfr      Featurize the mDSP SBI controller
11/30/04   gfr      No need to set pa profile until a tx occurs.
11/29/04   yhong    Registering mdsp_switch_command_buffers before calling
                    gl1_hw_wcdma_mdsp_init to avoid waiting forever for ISR tick
11/23/04   bm       - Wait for atmost 4 frames, before deactivating MDSP when MDSP
                      is not responding to GO_TO_SLEEP command
                    - Lint Cleanup
10/25/04   gfr      Split gl1_hw_init into subfunctions, use hw_set_trk_lo_adj
                    to write to the PDM, add TSC to the debug buffer logging.
10/08/04   yhong    Added conflict check for delay frame
09/24/04   bm       Split up the L1 deactivation into TASK and ISR
09/20/04   gfr      Support for logging the mdsp debug buffer.
08/11/04   gfr      Only unfreeze TCXO if necessary during init.
08/10/04   yhong    WCDMA code sent to a new file: gl1_hw_wcdma.c
07/26/04   gfr      Print awakeState if we inactivate while dsp is active
                    and RF fill sample ram cleanup.
06/28/04   gfr      Convert GSM1X PDM correctly.
06/30/04   bm       Featurise clk_regime_init_wcdma_gsm_aclk_val, as not
                    needed for GSM-1X targets
06/23/04   gfr      Lint cleanup.
06/22/04   gfr      Update PDM RGS values when afc is locked.
06/09/04   gw       Check if mDSP is busy before deactivating.
06/08/04   gfr      Take tune time into account for rf sample ram conflict check.
06/01/04   gfr      Support for the TCXO manager.
05/27/04   bm       Fix for CM bug in 2.1 rev, requested by rema
05/27/04   gfr      Added dedicated mode flag to SRM frequency request.
05/20/04   gfr      Mainlined FEATURE_COMMON_PWR_HANDLER
05/19/04   gfr      Added support for FEATURE_RF_AFC
04/16/04   gfr      Fix PDM overflow recovery.
04/27/04   hg       Clean up for 1X/GSM target. Added FEATURE_MULTI_RF_CHAIN_API.
04/26/04   jc       Setup the adie clock for interRAT in the gl1_hw_setup_wcdma_clocks
                    function, rather than in start_wcdma_rf_cmd. This will take care
                    of setting up the clocks for both idle and dedicated mode.
04/09/04   gfr      Rotator now stored as phase instead of Hz.
04/07/04   bm       Don't default PDM, ROT values in gl1_hw_inact_vctcxo_pdm()
                    in case if we return to GSM because of G2WHO failure, we should
                    use the previous values and not the default values.
03/22/04   gfr      Added slope parameter to rfm_update_freq_error_table.
03/19/04   bm/rv    Changes for single ended clock
03/05/04   gfr      Lint cleanup.
02/17/04   gfr      GSM rotator support.
02/11/04   gfr      Support for wcdma reconfirm in GPRS.
02/03/04   bm       Added FEATURE_GSM_PLT around clk_regime_disable in gl1_hw_inact
                    to not to include clk_regime_disbale for PLT builds.
02/02/04   bm       Added a function to set the async_rx_search_width
01/21/04   bm       Added call to clk_regime_set_analog_clk() from
                    gl1_hw_start_wcdma_rf_cmd
01/12/04   bm       removed (void) infront of hw_set_system_mode(GSM) to avoid
                    compilation error on Leopard
01/12/04   gfr      Support for 6 monitors per frame.
01/09/04   bm       gl1_hw cleanup, cleaning up RF_HAS_GSM1X to most extent
01/07/04   bm       Mainlined RF_MULTIMODE
12/10/03   gfr      Compile fix.
12/05/03   bm       Added logging support for MDSPcommands issued,
                    and the corresponding pops
11/25/03   gfr      Roll back setting system mode Leopard change.
11/24/03   jc       Temp fix for RSSI=0 by using autoACK to clear chfilt DMA.
11/18/03   gfr      Removed irrelevant reference to rfnv_task_wait_info_struct
11/13/03   bm       Fix for ACLK_SRC_1 bug: Give ADIE_CHO control to MDSP
11/05/03   gfr      Support for quad-band.
11/03/03   jc       Mainlined Saber changes.
10/22/03   jc       Fix for T3124 by adjusting PDM to last WCDMA value used.
10/19/03   jc       Fix for T3124 where missing one gstmr ftick.
10/20/03   bm/jc    Fixes for RAT switch problem
09/22/03   gfr      Read NV items relevant to NPL1 during init.
                    Save the TSC with the I/Q samples.
09/11/03   bm/jc    Changes to hide SBI APIs, based on Chris comments
09/10/03   gfr      Support for saving I/Q samples to flash.
08/27/03   gfr      Support for FEATURE_GSM_HW_TASK_REQUEST to support dumping
                    mdsp memory to flash.
08/25/03   drl      Fixed clk_regime_set/release_low_cpu_setting call order
08/21/03   gw       Moved RX_SAMPLE clock regime from GSM clocks to WCDMA rx
                    clocks.  This means it will be turned off except during
                    initialization and WCDMA receives.
08/15/03   jc       Mainlined RF_MULTIMODE, deregister handlers first when
                    doing gl1_hw_inact. Remove gtow debug code for agc init.
08/12/03   drl      Added clk_regime_set/release_low_cpu_setting code for MSM6300
08/07/03   tb       Move gtow clock control to new functions for dedi mode.
07/31/03   bm       Set the clock regimes based on whether they are
                    hardware controlled or not.
07/08/03   drl/mk   Added feature flag !GSM_RF_SLEEP in gl1_hw_init_gsm_hw
07/02/03   jc       Removed FEATURE_WCDMA_DEEPSLEEP.
07/01/03   bm/jc    Fix for WCDMA deep sleep. CR31989.
06/20/03   bm/jc    Removed calls to mdsp_wait_for_frame_tick(gas_id) inside gl1_hw_init
                    as this is been handled inside mdsp_nonsbi_write. Part of
                    solution to CR29307, WtoG HO taking too long.
06/23/03   gfr      Interference measures use rx alpha not mon alpha.
06/21/03   jc       Mainlined FEATURE_INHIBIT_PSBI, stage2 output for channel
                    filter.
06/17/03   gfr      Different tune times for monitor versus rx bursts.  Minor
                    lint cleanup.  Fix to allow showing the MDSP PC on debus bug.
06/11/03   gw       Tidied up clock regime handling and made sure DATAPATH_AGC
                    is always enabled by gl1_hw_init().
06/06/03   jc       No need to disable mdsp clock for Leopard when using mdsp
                    services to manage clock switching.
05.27.02   dp       Set mdsp async rx search width to default during hw_init
05/22/03   gw       Removed unneeded code from gl1_hw_mode_sw_handler().
05/21/03   gfr      Compress monitors into available space.
05/20/03   gfr      Common power handler support.
05/19/03   dlr/jc   Turn DSP clock off one frame later when leaving GSM
05/09/03   gfr      Removed prototype for mdsp_build_sbi_burst_buffer
04/29/03   gfr      Added DEBUG_MDSP_DATA_ON_TEST_BUS flag to dump mdsp info
                    on the test bus.
04/28/03   tb       Changes to allow registration/deregistration of MDSP isrs
04/17/03    jc      Disable mdsp a frame later when leaving GSM.
04/04/03   ATM      Replace magic#s and standardized T_RF_ZRF6300 flag(s) to
                    RF_HAS_GSM1X.
04/03/03   drl      Removed Leopard specific warning
03/24/03   jc/to    Leopard cleanup.
03/18/03   gw       Added explicit enabling of WCDMA clk regimes in gl1_hw_init
                    to support Deep Sleep.
03/12/03   pgh      Added FEATURE_GSM_RF_SLEEP around gl1_hw_turn_tx_off in
                    gl1_hw_inact() to enable build for leopard.
03/06/03   dp       Undid the 625qs ARM interrupt delay in WCDMA when entering
                    GSM
02/28/03   pgh      Added gl1_hw_turn_tx_off to hw_inact() so that it will turn
                    off Tx on RAT change
02/26/03   jc       Remove remaining HWTC code. Okay with Revathi/LamL.
                    Change sample ram length to be 14 bits.
02/24/03   mk       Sleep/Standby optimization changes via enable/disable of WCDMA
                    clock regimes. Featurized using FEATURE_GSM_SLEEP_WCDMA_CLK_OPT.
02/24/03   gw       Added support for deep sleep in gl1_hw_inact().
02/24/03   drl      Re-inserted HWIO call in gl1_hw_init due to regression
                    problems with Cheetah.
02/21/03   drl      Deleted HWTC code in gl1_hw_init_gsm_hw as per Jesus Chao
02/21/03   drl      Merge leopard changes onto mainline.
02/20/03   dp       Removed factory test mode per request from Jessie and Xieng
                    (FTM folks).
02/12/03   gw       Added call to gl1_hw_sleep_gsm_shutdown() in gl1_hw_inact().
02/12/03   mk/gw    Moved some code to new files gl1_hw_sleep_ctl.c and
                    gl1_hw_gsm.c.
02/07/02   ATM      Made changes for MDSPSVC API mods (6300)
02/07/03   jc       Fix step3 energies after a POR in gtow.  HW bug.
02/05/03   jc       Put back 40mhz mdsp clock.  Add coarse grain agc for GtoW.
02/04/03   mk       Added functions to allow initialization of the WCDMA Searcher
                    block to be done only once. Also set MDSP clock back to 40MHz
                    (for GTW) and don't set ARM clock here (done elsewhere).
01/24/03   ATM      Logging Cleanup (renamed PDM/MON, removed redundant metrics log)
01/15/03   gw       Added voting mechanism for turning off GSM clock regime. This
                    is required to support GPRS ciphering.
01/13/02   dp       Moved DEBUG_STAGE2_CHANNEL_FILTER to custsurf.h
01/13/03   mk       Changes to support both mdsp hard and soft rx qual. Also removed
                    mdsp taf (determined in L1) and bfi (calculated in L1).
01/09/03   prk      Included err.h for ERR_FATAL prototype.
01/10/03   jc       Correct a merge problem seen in 1.98. gl1_hw_mode_gsm flag
                    was deleted when leaving gsm mode.
01/06/03   gw       Commented out code for maintaining WCDMA reference counter
                    on rude wakeup.
01/02/03   gw       Added support for calling gl1_hw_sleep_gsm_start_sleep()
                    from task context. Removed Brian RF support in sleep code.
                    Added code to disable HKADC on analog die, featurized by
                    FEATURE_GSM_DISABLE_ADIE_HKADC.
12/11/02   ih       Changed gl1_hw_inact to switch SBI mode in the next frame
12/13/02   ATM      removed t_gsm.h per Bala's request
12/13/02   sd       Added RF driver MM API support for CM and cell
                    reselection.
12/12/02   gw       Reinstated call to gstmr_gsm_core_reset() in gl1_hw_init().
12/10/02   gr       Do not perform schedule checking unless the ms is in gsm mode.
12/10/02   gw       Removed code that modified sync rx search width on wake up
                    from sleep.
12/09/02   ATM      Moved call to gstmr_gsm_core_reset() to gstmr_init() for CMHO.
12/06/02   JC       Replaced rfgsm_set_rx_gain_data() with rfgsm_set_rx_gain()
12/06/02   gw       Added suppport for maintaining WCDMA reference counter
                    during GSM sleep.  Required to support GTOW measurements.
11/25/02   ATM      Added Monitor logging & augmented PDM logging w/ abs value
11/22/02   mk       Switch ARM clk to 40MHz in gl1_hw_init
11/07/02   sd       Added RF driver MM API support for CM in function
                    gl1_hw_start_acquisition().
11/06/02   jc       Reset mdsp clk back to 40Mhz.
11/04/02   dp       Defined DEBUG_HW_SELECT_STAGE2_OUTPUT_CHANNEL_FILTER to
                    enable the new channel filter for sync rx and monitors
                    by default
11/03/02   tb       Added support for INTERRAT_GTOW
10/31/02   ih       Added call to switch SBI controller back to ARM mode in
                    gl1_hw_inact() before disabling MDSP.
10/30/02   gw       Changes to support optimization of sleep timeline.  Moved
                    gl1_hw_pdown_up() into separate file gl1_hw_sleep.c.
10/28/02   gw       Changes to async reveive functions to support back-to-back
                    receives.  Required for compressed mode.
10/14/02   ATM      Relocated Terminal Count Adjustment logging to GTMRS
10/10/02   ih       Moved all script-related data structure and functions into
                    the RF driver to resolve circular dependency between HW and
                    RF
10/11/02   gw       Changes to support GSM sleep in dualmode builds on ZIF RF.
10/09/02   JC       Added stage2 channel filter support for monitor bursts.
                    See DEBUG_HW_SELECT_STAGE2_OUTPUT_CHANNEL_FILTER.
                    Replaced HW_RF_WR_GRFC with HW_RF_WR_NONSBI to address
                    register access' within a frame.  Added support for
                    selectively changing the channel filter settings based on
                    type of Rx for systems. Moved spectral inversion control
                    into RF driver.
10/03/02   jc       Added rf_stop_acq_scripts.
09/28/02   jc       Removed FTSM from start/stop acq. Moved in mdsp driver.
09/26/02   ih       Added support for dynamic tx settling time
09/20/02   jc       use shadow register for MISC_CTL_0 access
09/19/02   thh      Added FTM support for GSM.  Work is started to make FTM
                    less intrusive to this module.
09/19/02   ATM      Added Combined ACQ logging
09/17/02   JC       Added DEBUG_CHANNEL_FILTER_DMA_ON_TEST_BUS for J.J. of the
                    MDSP development team.
09/16/02   plm      Featurized declaration of init_gsm_scripts_ptr static variable
                    to eliminate compiler warning.
09/12/02   sd       Added support for RF driver multimode API that includes
                    GSM drivers.
09/10/02   ih       Removed process of static buffers from gl1_hw_rf_init().
                    Created gl1_hw_init_static_buffers() to initialize static
                    buffers when changing bands.
09/09/02   gw/ih    Added MDSP static buffer support.
09/06/02   gr       Support for new uplink power control interface.
09/04/02   plm      Added externs for mdsp_build_sbi_burst_buffer(), rf_init_gsm()
                    and rfgsm_init() to eliminate compiler warnings.
08/21/02   gw       Merged in GSM sleep code that had been on a branch.
08/23/02   dp       Added crc_pass parameter to gl1_hw_was_sch_decoded.  Added
                    function to get current PDM value
08/21/02   AT       Fixed compile error when related to feature
                    FEATURE_RF_ENABLE_GSM_CLK_AT_GPIO3.
08/21/02   JC       Added burst type to rfgsm_compute_rx_power() to be able
                    to distinguish NCELL ACQ from normal and power bursts.
                    This fixes the bug where the RF FIFO gets corrupted in
                    dedicated mode because the NCELL ACQ works on a single
                    frame basis and the serv cell TCH works on a two frame
                    basis.  It would result in a blip in the power before the
                    IDLE frame.
08/20/02   thh      Call RF to tune prior to setting the PA profile.  This
                    allows the Tx frequency compensation to work correctly.
                    GSM clock routed through GPIO3 is now featurized under
                    FEATURE_RF_ENABLE_GSM_CLK_AT_GPIO3.
08/07/02   jc       Corrected merge problem.ERR_FATAL is dsp fails to load.
08/02/02   jc       Wrap FEATURE_WPLT around gl1_hw_init.
07/31/02   ATM      Bugfix!? ypf 0 until I moved call above set of ->good_data
07/31/02   ATM      Bugfix: moved SCH logging for combined case to capture crc failures.
07/26/02   ATM      Added BCCH decode logging, fixed typo: GSTMR.
07/26/02   ATM      Added FCCH/SCH decode logging, GSTMR/PDM adjust logging.
07/25/02   jc       Removed definition of FRAMES_IN_SUPERFRAME. getting elsewhere.
07/09/02   JC       Rename rf_init_for_gsm to rf_init_gsm for consistence with
                    WCDMA.
07/11/02   gw       Replaced references to power_dBm_x16 in metrics rpt with
                    pwr_dBm_x16.
07/09/02   jc       Add call to init SYSTEM_MODE register, and call to configure
                    some GRFCs.
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
06/27/02   thh      Added support for GZIFTRIC.
06/18/02   tb       Changes for mdsp services and adie
06/10/02   gw       Added call to gsleep_init() in gl1_hw_init(), featurised
                    by FEATURE_GSM_SLEEP_CONTROLLER. Added some functions to
                    support sleep.
06/04/02   bbd      Changed FEATURE_GPLT to FEATURE_GSM_PLT
05/20/02   jc       Changed stop acquisition to use TX_BURST for processing space.
                    Needed to support compressed mode.
05/08/02   thh      Added support for multi-mode SBI driver.
05/07/02   plm      Mainlined code associated with CHEETAH_GSM
04/26/02   gw       Added code in gl1_hw_init() to overwirte an mDSP default
                    threshold for BFI.
04/22/02   ih       Moved CHANNEL_FILTER_START/STOP_DELTA into rfgsm.h. New
                    definitions now based on the negative value of
                    RF_RX_CHAIN_TOTAL_DELAY
04/19/02   JC       Added rx_gain_range to gl1_hw_get_syncrx_metrics to
                    enhance burst metrics logging. Also, fixed HW rev per ATM.
04/16/02   thh      Only allow 1 uwire transaction per frame during
                    initialization.  The B4 is not initialized properly when
                    we send it more than one uwire per frame.  Need to
                    investigate this further and fix it.
04/03/02   thh      All SBI/UWIRE accesses now go through the SBI driver.
                    Previously, these accesses went directly to the MDSP
                    interface driver.
                    Updated copyright date.
04/02/02   mk       Added support for CHEETAH_GSM.
04/02/02   ih       Changed the way to determine if pa_precharge is set by using
                    pa_precharge_start_time < pa_start_time
03/29/02   dp       Merged Gabe's BFI and command ID renaming changes
03/25/02   gw       Made call to rf_compute_rx_power in gl1_hw_get_async_metrics
                    unconditional so that RF drivers get unwound properly even
                    when there are mDSP errors.
03/22/02   JC       Minor cleanup in rx_async_bcch_burst, use of dBx16_T.
                    Removed legacy ifdefs.
03/20/02   dp       Changed txlevs to tx power index
03/20/02   ih       Changed rf_pa_precharge_stop_time to change with rf_pa_stop_time
                    to allow allow PA start/stop time calibration. Changed
                    gl1_hw_setup_tx_burst to use calibrated values for PA start/stop
                    time. Removed RF_BRIAN #ifdefs
03/14/03   thh      PA precharge and PA profile setting API changes.
                    Removed DB2000 support.
03/08/02   gw       Changed initialization so that sbi and nonsbi write commands
                    are synchronized to the frame tick (and therefore to the
                    mDSP).
03/06/02   JC       Added optional parameter to hw_was_sch_decoded().
02/15/02   JC       Added support for RSSI to dBm conversion in rx_metrics
                    functions to support AGC in 51-multiframe. Also,
                    renamed gl1_hw_gsm_init() to gl1_hw_init() and gstmr_reset
                    to gstmr_gsm_core_reset().
02/20/02   dp       Changed hw_get_async_rx_metrics interface to return metrics
                    availability(true/false).  Pulled out qdsp_cmd_voc_frame_ref
                    from gl1_hw_align_vfr.  Is unnecessaey and was causing the
                    s/w to crash.
02/15/02   mk       Updated afc/tt.Changed gl1_hw_delay_frame_tick() to support
                    both afc/tt and L1 functionality.
02/14/02   gw       Added support for AGC to async rx functions.
02/13/02   jc       Release the vocoder when terminating the TCH. Add support
                    for ciphering. Induce the VFR alignment in software.
01/22/02   JC       Modified rf_init() interfaces so that the L1 task could
                    register a wait handler for the NV item manager. Also,
                    modified gl1_hw_get_power_measure_results() interface and
                    added logic to this function.  Modified all RX interfaces
                    to use dBx16_T for AGC support and consistence.
02/01/02   mk       Updated to support the latest afc/tt algorithms. Also
01/24/02   AT       Added DOG functionality for QPST
01/15/02   atm      Improved hw_version() function and eliminated compiler warnings
01/21/02   thh      Previously, power level is not set for normal bursts.  So,
                    normal bursts were always transmitted with whatever PA
                    profiles that were set earier by the RACH transmission.
                    Now, power level is set as commanded for normal bursts.
01/16/02   mk       Added support for afc/tt.
12/18/01   dp       Added function gl1_hw_was_tone_detected and changed acquisition
                    offset type to int32 to handle negative offsets from the DSP
12/13/01   thh      Lower-cased the function gl1_hw_*_VCTCXO_PDM() functions.
                    Previously, the gl1_hw_set_vctcxo_pdm() function was
                    writing to the wrong register.  This has been corrected.
12/13/01   gw       Changes to support message/frame layer acquisition.
12/12/01   jc       Fix problem of getting packed data into unpacked struct in
                    _get_syncrx_data() and _was_asyncrx_burst_decoded().
                    Updated the ciphering functions.
12/12/01   gw       Added support for returning the YPF word returned by the
                    mDSP with decoded data.  To use, FEATURE_GPLT must be
                    defined.
12/12/01   thh      Fixed a bug where the gl1_hw_start_tone_detection() was
                    setting up the MON_BURST but uses the RX_BURST.
12/11/01   thh      Added #ifdef's around channel filter general control
                    init codes.
12/10/01   thh      Merged in Brian RF support.
                    Moved remaining functions in hw_gsm.c to this module.
                    Hw_gsm.c is obsoleted.
11/19/01   jc       Added support for voice on full stack on KOVU. Added tch
                    loopback support.
11/02/01   mk       Added KOVU support.
10/31/01   gw/dp    Added support for logging debug info.  Added support for
                    returning metrics in dedicated mode. Changed interface to
                    accept the required number of data bytes when retrieving
                    sync and async decoded data from the DSP.
10/31/01   dp       Implemented fix to eliminate multiple copies when retrieving
                    DSP results in gl1_hw_get_sync_rx_data and
                    gl1_hw_was_async_burst_decoded. Interface for these functions
                    has changed.  gl1_hw_was_async_sch_decoded and
                    gl1_hw_was_async_bcch_decoded has been combined into a single
                    function gl1_hw_was_async_burst_decoded
10/24/01   dp       Added support for surround cell tone detecttion with an
                    interface to pass in a time offset at which to start or
                    stop the tone detection.   Changed async rx to use the
                    PWR_BURST buffer and increased the async Rx search width
                    to 100.
10/23/01   mk/gw    Mainlined code for multiple power measurements per frame.
                    Changed power measurement and async rx code to support
                    firmware PLT0441 buffer organization.
10/16/01   gw       Changed error handling in gl1_hw_power_measure.
10/10/01   gw       Added support for power measurements and Ncell SCH and
                    BCCH reception.
09/25/01   gw/jc    Added syncrx metrics and RACH support.
08/31/01   jc       Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_nv.h"
#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif
#include "comdef.h"


#include "geran_multi_sim.h"

#include "gtmrs.h"
#include "gtmrs_g.h"         /* GSM timer services */
#include "vtmrs_g.h"  /*GERAN VSTMR interface*/

#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_clk_ctl.h"
#include "gl1_hw_clk_ctl_g.h"
#include "gl1_hw_debug.h"
#include "l1_utils.h"
#include "gl1_hwi.h"
#include "gl1_hw_gsac.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "mdsp_intf_shared_vars.h"
#include "mdsp_async_intf.h"

#include "gpl1_dual_sim.h"

#include "gpl1_grm_intf.h"

#include "lte_ml1_ext_api.h"

#include "rfm.h"
#include "rfm_gsm.h"
#include "rfwcdma_core.h"
#include "tcxomgr.h"

#include "ftm.h"
#include "DALSys.h"
#include "geran_msgs.h"
#include "rex.h"

#include "slpc.h"
#include "gl1_hw_sleep_ctl.h"

#include "mcpm_api.h"

#include "gl1_hw_tcxo_mgr3.h"

#include "gl1_mutex.h"
#include "gl1_msg_g.h"
#include "l1i.h"

#include "gfw_sw_intf.h"
#include "fws.h"
#include "mcs_hwio.h"


#include "rxlm_intf.h"
#include "txlm_intf.h"

#include "ftm.h"

#include "fs_public.h"
#include "fs_errno.h"

#include "fs_lib.h"

#include "rfgsm_msg.h"
#include "gprs_mem.h"

#include "rfmeas_msg.h"

/* Add just for rfm_wcdma_convert_band_rfcom_to_rfi */
#include "rfm_wcdma.h"

#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "wmgr.h"
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#include "l1_drx.h"
#include "l1_task.h"
#include "l1i.h"
#include "lm_types.h"
#include "msgr.h"
#include "geran_msg.h"
#include "l1_sc_irat.h"
#include "l1_sc_int.h"
#ifdef  FEATURE_GSM_TO_LTE
#include "l1_sc_lte_irat.h"
#endif /* FEATURE_GSM_TO_LTE */
#include "geran_tasks.h"

#include "gl1_hw_arbitration.h"

#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "gl1_hw_sleep_ctl.h"
#include "gpl1_grm_intf.h"
#include "l1_isr.h"
#include "gl1_msgrif_init.h"

#ifdef FEATURE_GL1_ASDIV_ACQ
#include "gl1_asdiv.h"
#endif

extern void rfm_power_clock_off(void);

extern void gl1_mdsp_panic_reset_callback( gas_id_t gas_id );
/*Wrapper function made to call mdsp_set_tx_config_cmd_pending inside file l1_task.c*/
void gl1_hw_mcpm_call_config_cmd(gas_id_t gas_id);


#ifdef FEATURE_G2X_TUNEAWAY
#include "l1_os.h"
#endif

#include "gl1_arbitrator_cxm.h"
extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[];
extern void l1_call_panic_reset_in_task( l1_panic_reset_type_T error_code, gas_id_t gas_id );

mcpm_tech_type gl1_hw_map_gas_id_to_mcpm_tech( gas_id_t gas_id );

extern void gl1_msg_afc_init( gas_id_t gas_id );

extern rfm_device_enum_type gl1_hw_get_gsm_rf_id_qta(gas_id_t gas_id);

void gl1_hw_multi_sim_gstmr_init( gas_id_t gas_id );

static void gl1_hw_panic_inact_task (gas_id_t gas_id);

#include "mcfg_nv_api.h"

#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY  
#include "gfw_sw_rf_intf.h"
#endif

#ifdef GERAN_L1_IP2CAL
static void gl1_hw_rf_init_ip2cal(uint32 gl1_gsm_ip2cal_rxlm_buf_idx,uint32 gl1_gsm_ip2cal_txlm_buf_idx,
                                    rfm_device_enum_type device,gas_id_t gas_id);
#endif /*GERAN_L1_IP2CAL*/

extern uint8 current_buffer_mod3;
/*===========================================================================

                     DEFINITIONS FOR MODULE

===========================================================================*/
#ifdef FEATURE_DUAL_ACTIVE
#define MAX_GSM_RF_CHAINS 2
#else
#define MAX_GSM_RF_CHAINS 1
#endif

/*===========================================================================

 LOCAL STORAGE

===========================================================================*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
GfwAppModeType gl1_hw_multi_sim_mode[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( GFW_APP_MODE_IDLE ) };
/* Used to allow normal sleep operation in G+G dual sim */
boolean gl1_hw_multi_sim_gonly_mode = FALSE;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#define XCCH_BLOCK_COMMIT_TIMEOUT  13 /* ms */
boolean first_awake_frame_after_sleep[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( FALSE ) };

/* Run-time debug flag to enable extra trk_lo/rotator messaging */
boolean gl1_hw_debug_gsm_afc;

/* Used when waiting for confirmations from TRM */
rex_timer_type  gsm_l1_timeout_timer[NUM_GERAN_DATA_SPACES];
/*removing as it resulting compile error*/
/* =                  { INITIAL_VALUE_S_NEW( 0 ) };*/

#ifdef FEATURE_GSM_QSC_TX_DIV
int antenna_to_use_for_tx [NUM_GERAN_DATA_SPACES] =
                  { INITIAL_VALUE( 0 ) };
extern void  gl1_rf_switch_complete_cb ( rfa_rf_gsm_set_antenna_callback_s *cb_data_ptr);
extern void gl1_hw_switch_antenna_tx_div (gas_id_t gas_id);
void gl1_trm_ant_switch_set_control ( trm_ant_switch_set_ctrl_enum control_req_type ,gas_id_t gas_id);
boolean pending_asdiv_switch_request [NUM_GERAN_DATA_SPACES] ={ INITIAL_VALUE( 0 ) } ;
trm_switch_complete_input_type gl1_switch_complete_input [NUM_GERAN_DATA_SPACES] ;
#endif /* FEATURE_GSM_QSC_TX_DIV */

#if defined( FEATURE_GSM_TX_DIV_ANTNA_SWITCHING) ||defined( FEATURE_GSM_QSC_TX_DIV)
boolean  gl1_hw_rfa_rf_gsm_set_antenna_req_cmd( boolean antenna_pos,gas_id_t gas_id );
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING*/

/*Flag specifies whether TX BAND command is issued to GFW or not. Use this flag
only during first RACH inside function l1_access_isr. Set when
TX BAND CNF is issued to GFW */
boolean gl1_set_tx_band_cmd_iss_to_gfw[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };

/*Flag specifies whether TX ENABLE CNF is received from RF or not. This flag is cleared
when TX_ENABLE_RF is issued to RF and set when CNF is received from RF. This flag is used
in dedicated mode to check whether TX ENABLE CNF is received from RF or not  */
boolean l1_rf_tx_enable_cnf_rcv[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
#ifdef FEATURE_VSWR 
boolean gl1_vswr_meas_requested [NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
boolean gl_hw_use_extended_tx_alpha [NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
#endif /*FEATURE_VSWR*/
/* ----------------------------------------------------------------------- */
/* Flag indicating if we are in gsm/gprs mode or not                       */
/* ----------------------------------------------------------------------- */
boolean gl1_hw_mode_gsm[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( FALSE ) };

/* ----------------------------------------------------------------------- */
/* Flag indicating if the tx chain clock is enabled or not                 */
/* ----------------------------------------------------------------------- */
boolean gl1_hw_tx_on[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( FALSE ) };

#ifdef GERAN_L1_HLLL_LNASTATE
static gl1_defs_dtm_metrics_t  gl1_defs_dtm_metrics[NUM_GERAN_DATA_SPACES] =
         { INITIAL_VALUE_S( {0} )  } ;
#endif /*GERAN_L1_HLLL_LNASTATE*/

/* ----------------------------------------------------------------------- */
/* Set UL TS to 1 for GSM - Prevents Power Backoff reducing transmit pwr.  */
/* ----------------------------------------------------------------------- */
#define GL1_DEFAULT_ONE_TS  1

/* ----------------------------------------------------------------------- */
/* Set the Tx XO Freq Error to zero when GL1 is initialised.If this is not */
/* done the Tx XO Freq Error  from WCDMA is left in the Register,  and GSM */
/* is off by what ever value WCDMA Tx XO was set to before the RAT change. */
/* ----------------------------------------------------------------------- */
#define GL1_TX_XO_FREQ_ERR_ZERO  0

/* ----------------------------------------------------------------------- */
/* MDSP will adjust the c_over_i if it is above the given threshold        */
/* ----------------------------------------------------------------------- */
#ifdef FEATURE_GSM_AMR
#define GSM_AMR_C_OVER_I_THRESHOLD 101
#endif

/* ----------------------------------------------------------------------- */
/*            Poke Interface control define                                */
/* ----------------------------------------------------------------------- */
#define MAX_ASYNC_WRITE_ATTEMPTS 3

/* ----------------------------------------------------------------------- */
/*            GSM Frequency Definitions                                    */
/* ----------------------------------------------------------------------- */
/* GSM Channel bandwidth (in Hz) - 200 kHz */
#define GSM_CHANNEL_BW               200000

/* GSM Channel frequency definitions (in Hz) */
#define BASE_FREQ_GSM_850_BAND       824200000
#define BASE_FREQ_GSM_900_BAND       890000000
#define BASE_FREQ_DCS_1800_BAND      1710200000
#define BASE_FREQ_PCS_1900_BAND      1850200000

#define DL_OFFSET_FREQ_GSM_850_BAND  45000000
#define DL_OFFSET_FREQ_GSM_900_BAND  45000000
#define DL_OFFSET_FREQ_DCS_1800_BAND 95000000
#define DL_OFFSET_FREQ_PCS_1900_BAND 80000000

/* ----------------------------------------------------------------------- */
/* VCTCXO control variables                                                */
/* ----------------------------------------------------------------------- */
/* Mid-band frequencies in MHz */
#define MID_FREQ_GSM_900_BAND   950
#define MID_FREQ_DCS_1800_BAND 1840
#define MID_FREQ_PCS_1900_BAND 1960
#define MID_FREQ_GSM_850_BAND   880

/* Array to convert from sys_band_T to the mid frequency */
int32 midband_freq[] =
{
  MID_FREQ_GSM_900_BAND,  /* SYS_BAND_EGSM_900 */
  MID_FREQ_GSM_900_BAND,  /* SYS_BAND_PGSM_900 */
  MID_FREQ_PCS_1900_BAND, /* SYS_BAND_PCS_1900 */
  MID_FREQ_DCS_1800_BAND, /* SYS_BAND_DCS_1800 */
  MID_FREQ_GSM_850_BAND   /* SYS_BAND_CELL_850 */
};

/* Inverse mid--band frequency */
/* Values are (2^52 / mid_band_freq in hz) */
#define INV_F_DL_LO_GSM_900_BAND    4740631
#define INV_F_DL_LO_DCS_1800_BAND   2447608
#define INV_F_DL_LO_PCS_1900_BAND   2297755
#define INV_F_DL_LO_GSM_850_BAND    5117727

#if defined (FEATURE_GSM_TO_LTE) || defined (FEATURE_GSM_TDS)
boolean gl1_deep_sleep_pending[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( FALSE ) };
#endif

#ifdef GERAN_L1_HLLL_LNASTATE
#define SAWLESS_SUPPORT_GSM_BAND_850     0x0001
#define SAWLESS_SUPPORT_GSM_BAND_900     0x0002
#define SAWLESS_SUPPORT_GSM_BAND_1800    0x0004
#define SAWLESS_SUPPORT_GSM_BAND_1900    0x0008
#endif /*GERAN_L1_HLLL_LNASTATE*/

typedef struct
{
int32 band_frequency_mhz;

/* Holds the accumulated freq offset for XO setting */
int32   gl1_xo_acc_freq_err;
gl1_hw_mdsp_freq_t gl1_hw_gfw_freq_config;
int32   local_active_cswallower_value;

uint16 gl1_hw_gsm_acch_nv_word;

uint16 gl1_hw_features_disabled;

boolean nv_read;
/*Access and control of the value used to adjust the rotator*/
int16 gl1_hw_vco_default;
/* Note the slope is in units of ppm/pdm, expressed as Q16 */
int32 afc_pdm_val;
int32 vctcxo_pdm_slope;

boolean acq_afc_on_vctcxo;


int32 afc_rot_val;

/* RF Link Manager buffer index */
uint32 gl1_gsm_rxlm_buf_idx;
uint32 gl1_gsm_txlm_buf_idx;

  rfm_device_enum_type       gl1_hw_trm_rf_rx_device;
#ifdef FEATURE_GSM_RX_TX_SPLIT
  /* Tx Rf device*/
  rfm_device_enum_type       gl1_hw_trm_rf_tx_device;
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

/* pointer to frame tick ISR that L1 registers */
void (* current_tdma_isr_ptr)(sys_modem_as_id_e_type as_id);


boolean gl1_panic_reset_occurred;

rfgsm_timing_info_type  gl1_rf_gsm_timing_info;

/* count_wait_for_mdsp_sleep is used to check how many frames
 * we have waited for MDSP to go to sleep */
uint8 count_wait_for_mdsp_sleep;


#define GL1_HW_RF_BURST_RX_BUF_NUM           8
#define GL1_HW_RF_BURST_TX_BUF_NUM           4
#define GL1_HW_RF_BURST_NUM_BURSTS_INVALID   0xFF

msgr_client_t gsm_l1_common_rfa_task_msgr_client;
msgr_id_t     gsm_l1_common_rfa_task_msg_id;
int32         gl1_hw_rf_freq_error;
int8          gl1_hw_rf_gsm_band;
rfa_rf_gsm_rx_burst_type   gl1_hw_rf_rx_burst_buf[GL1_HW_RF_BURST_RX_BUF_NUM];
uint8                      gl1_hw_rf_rx_burst_buf_index;
rfa_rf_gsm_tx_burst_type   gl1_hw_rf_tx_burst_buf[GL1_HW_RF_BURST_TX_BUF_NUM];
uint8                      gl1_hw_rf_tx_burst_buf_index;

/* Flag to indicate if the next firmware download should force a reset */
boolean force_mdsp_reset;
boolean rf_enter_mode_required;
boolean gl1_hw_start_gsm_qta_sent;
#ifdef GERAN_L1_IP2CAL
uint32 gl1_gsm_ip2cal_rxlm_buf_idx;
uint32 gl1_gsm_ip2cal_txlm_buf_idx;
boolean ip2cal_supported[RFM_MAX_WAN_DEVICES];
#endif /*GERAN_L1_IP2CAL*/

#if defined ( GERAN_L1_HLLL_LNASTATE ) || defined ( GERAN_L1_IP2CAL )
uint8 gl1_hw_sawless_support[RFM_MAX_WAN_DEVICES];
#endif /* GERAN_L1_HLLL_LNASTATE || GERAN_L1_IP2CAL*/

#ifdef GERAN_L1_HLLL_LNASTATE
rfa_rf_gsm_rx_burst_metrics_type metrics[GL1_HW_RF_BURST_RX_BUF_NUM][RFGSM_MAX_METRICS_ENTRIES];
uint8                            gl1_hw_gsm_burst_metrics_buf_index;

#endif /*GERAN_L1_HLLL_LNASTATE*/
}gl1_hw_data_t;

static gl1_hw_data_t gl1_hw_data[NUM_GERAN_DATA_SPACES];

static gl1_hw_data_t *gl1_hw_data_ptr[NUM_GERAN_DATA_SPACES] =
         { INITIAL_VALUE_INDEXED( &gl1_hw_data ) };
#ifdef FEATURE_GSM_RX_TX_SPLIT
enum
{
   FTM_DEVICE_PRX = 0,
   FTM_DEVICE_DRX,
   FTM_DEVICE_TX,
   FTM_DEVICE_NUM_DEVICES
};

static geran_test_mode_api_device_type ftm_devices[NUM_GERAN_DATA_SPACES];

#else
static rfm_device_enum_type ftm_device[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( RFM_INVALID_DEVICE ) };
#endif

#define HW_PHASE_INCREMENT_RESOLUTION (29)

#ifndef HW_TCXO_PDM_BITWIDTH
  #define HW_TCXO_PDM_SIGNED        1
  #define HW_TCXO_PDM_BITWIDTH      (16)

  #define HW_MDSP_TCXO_PDM_SHIFT    (20)    /* bits 31:20 */
  #define HW_MDSP_TCXO_PDM_BITWIDTH (12)    /* 12 MSBs */
  #define HW_MDSP_TCXO_PDM_MIN      (-2048) /* 0x800 */
  #define HW_MDSP_TCXO_PDM_MAX      (2047)  /* 0x7FF */
#endif /* !HW_TCXO_PDM_BITWIDTH */

#if ( defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) ) && defined( FEATURE_CDMA )
#undef HW_MDSP_TCXO_PDM_SHIFT
#undef HW_MDSP_TCXO_PDM_BITWIDTH
#undef HW_MDSP_TCXO_PDM_MIN
#undef HW_MDSP_TCXO_PDM_MAX

  #define HW_MDSP_TCXO_PDM_SHIFT    (16)    /* bits 31:20 */
  #define HW_MDSP_TCXO_PDM_BITWIDTH (16)    /* 16 MSBs */
  #define HW_MDSP_TCXO_PDM_MIN      (-32768) /* 0x8000 */
  #define HW_MDSP_TCXO_PDM_MAX      (32767)  /* 0x7FFF */

  /*
   * The extra value to avoid truncation when converting from shared to GSM
   * which for dual sim would be 0 as same res in TCXO Manager as us and 1x
   */
  #define HW_MDSP_TCXO_PDM_CONV_VAL (0)
#else
  /*
   * The extra value to avoid truncation when converting from shared to GSM
   * calculated as:
   * 2 ^ ( ( HW_TCXO_PDM_BITWIDTH - HW_MDSP_TCXO_PDM_BITWIDTH ) / 2 ) = 8
   */
  #define HW_MDSP_TCXO_PDM_CONV_VAL (8)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/* Define a type for the PDM value read from the mDSP */
#if HW_TCXO_PDM_SIGNED
typedef int16  mdsp_tcxo_pdm_type;
#else
typedef uint16 mdsp_tcxo_pdm_type;
#endif

/* Convert RF PDM value (16 bit signed) to and from local value */
#if HW_TCXO_PDM_SIGNED
  #define PDM_SHARED2GSM(v) (( (v) + HW_MDSP_TCXO_PDM_CONV_VAL ) >> (16 - HW_MDSP_TCXO_PDM_BITWIDTH))
  #define PDM_GSM2SHARED(v) ((v) << (16 - HW_MDSP_TCXO_PDM_BITWIDTH))
#else
  #define PDM_SHARED2GSM(v) (((v) + 32768) >> (16 - HW_MDSP_TCXO_PDM_BITWIDTH))
  #define PDM_GSM2SHARED(v) (((v) << (16 - HW_MDSP_TCXO_PDM_BITWIDTH)) - 32768)
#endif

  /* The AGC_CARRIER_FREQ_ERR  register is 16bit */
  /* , but the GL1 AFC  loop  is designed for 12 */
  /* bits, so there is a 4 bit left shift needed */
  /* otherwise the  Ferr will get over adjusted. */
#define   GL1_AFC_LOOP_PDM_SHIFT     4


/* Max delta in seconds between recent good system and current time before
   going to the default temperature table PDM value */
#define GSM_MAX_RGS_TIME_DELTA 30

/* Scaling for XO ppm frequency as used by RF driver */
#define XO_RF_FREQ_NORM_SCALE_BITS 14
#define XO_RF_FREQ_NORM_SCALE_VAL  ( 1 << XO_RF_FREQ_NORM_SCALE_BITS )
#define GSM_XO_RF_SCALING_FACTOR   31250

#define MAX_WAIT_FRAMES_FOR_MDSP_SLEEP 3

/* Masks for processing the edge_feature NV item */
#define EDGE_FLAG_FEATURE_DISABLE_MASK        0xFE

/* Masks for dynamically disabling features */
#define GL1_HW_FEATURE_DISABLE_ACI_DETECTION  0x02
#ifdef FEATURE_GSM_MDSP_IR
#define GL1_HW_FEATURE_DISABLE_IR             0x04
#endif
#define GL1_HW_FEATURE_DISABLE_SAIC           0x08

#ifdef FEATURE_GSM_MDSP_ESAIC
#define GL1_HW_FEATURE_DISABLE_ESAIC          0x10
#endif

#define GL1_HW_FEATURE_DISABLE_AEQ            0x20

#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
#define GL1_HW_FEATURE_DISABLE_AEQ_8PSK       0x40
#endif

#ifdef FEATURE_GSM_MDSP_AEQ_SCH
#define GL1_HW_FEATURE_DISABLE_AEQ_SCH        0x80
#endif

#define GL1_HW_FEATURE_DISABLE_NONE           0x00

/* Maximum number of attempts to load the mDSP before giving up */
#define MAX_MDSP_LOAD_ATTEMPTS 3

#define GL1_HW_GSM_ACCH_NV_FLAG_DISABLED 0x00

#define GSM_L1_COMMON_RFA_TASK_PRIORITY  1
#define GSM_L1_COMMON_RFA_TASK_DEPTH     3

uint32 msgr_prof_start_time_init_hdr;
uint32 msgr_prof_start_time_send;
uint32 msgr_prof_end_time;


static msgr_umid_type gsm_l1_common_rfa_task_umid_list [] =
{
#ifdef FEATURE_GSM_RX_TX_SPLIT
  RFA_RF_GSM_START_GSM_CNF,
  RFA_RF_GSM_STOP_GSM_CNF,
  RFA_RF_GSM_RX_SLEEP_CNF,
  RFA_RF_GSM_TX_SLEEP_CNF,
#else
  RFA_RF_GSM_ENTER_MODE_CNF,
  RFA_RF_GSM_EXIT_MODE_CNF,
  RFA_RF_GSM_TX_DISABLE_CNF,
  RFA_RF_GSM_SLEEP_CNF,
#endif
  RFA_RF_GSM_CM_ENTER_CNF,
  RFA_RF_GSM_CM_EXIT_CNF
#ifdef GERAN_L1_IP2CAL
  ,RFA_RF_GSM_INIT_MSM_IP2_CAL_CNF
#endif
};

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static msgr_umid_type gsm_l1_common_rfa_task_umid_list_dev_2 [] =
{
#ifdef FEATURE_GSM_RX_TX_SPLIT
  RFA_RF_GSM_START_GSM_SUB2_CNF,
  RFA_RF_GSM_STOP_GSM_SUB2_CNF,
  RFA_RF_GSM_RX_SLEEP_SUB2_CNF,
  RFA_RF_GSM_TX_SLEEP_SUB2_CNF,
#else
  RFA_RF_GSM_ENTER_MODE_SUB2_CNF,
  RFA_RF_GSM_EXIT_MODE_SUB2_CNF,
  RFA_RF_GSM_TX_DISABLE_SUB2_CNF,
  RFA_RF_GSM_SLEEP_CNF,
#endif
  RFA_RF_GSM_CM_ENTER_SUB2_CNF,
  RFA_RF_GSM_CM_EXIT_SUB2_CNF
#ifdef GERAN_L1_IP2CAL
  ,RFA_RF_GSM_INIT_MSM_IP2_CAL_SUB2_CNF
#endif
};

#define GSM_L1_COMMON_RFA_TASK_DIAG_CNT_DEV_2 (sizeof(gsm_l1_common_rfa_task_umid_list_dev_2)/sizeof(msgr_umid_type))

#if defined ( FEATURE_TRIPLE_SIM )
static msgr_umid_type gsm_l1_common_rfa_task_umid_list_dev_3 [] =
{
#ifdef FEATURE_GSM_RX_TX_SPLIT
  RFA_RF_GSM_START_GSM_SUB3_CNF,
  RFA_RF_GSM_STOP_GSM_SUB3_CNF,
  RFA_RF_GSM_RX_SLEEP_SUB3_CNF,
  RFA_RF_GSM_TX_SLEEP_SUB3_CNF,
#else
  RFA_RF_GSM_ENTER_MODE_CNF,
  RFA_RF_GSM_EXIT_MODE_CNF,
  RFA_RF_GSM_TX_DISABLE_CNF,
  RFA_RF_GSM_SLEEP_CNF,
#endif
  RFA_RF_GSM_CM_ENTER_SUB3_CNF,
  RFA_RF_GSM_CM_EXIT_SUB3_CNF
#ifdef GERAN_L1_IP2CAL
  ,RFA_RF_GSM_INIT_MSM_IP2_CAL_SUB3_CNF
#endif
};
#define GSM_L1_COMMON_RFA_TASK_DIAG_CNT_DEV_3 (sizeof(gsm_l1_common_rfa_task_umid_list_dev_3)/sizeof(msgr_umid_type))
#endif /* FEATURE_TRIPLE_SIM */

extern gl1_hw_arbitration_t gl1_hw_arbitration[];

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#define GSM_L1_COMMON_RFA_TASK_DIAG_CNT (sizeof(gsm_l1_common_rfa_task_umid_list)/sizeof(msgr_umid_type))

typedef struct{
  trm_band_t           band;
  rfm_device_enum_type device;
} gl1_hw_grm_devices_type;

extern rfgsm_modulation_type      rfgsm_modulation_lut[];
static gl1_hw_acq_afc_type afc_mode = GL1_HW_ACQ_AFC_DISABLED;

#ifdef FEATURE_GSM_COEX
extern gl1_hw_arbitration_t gl1_hw_arbitration[];
#endif


extern uint16 gl1_sleep_cnt[NUM_GERAN_DATA_SPACES];
extern gl1_hw_sleep_type gl1_hw_sleep_struct[NUM_GERAN_DATA_SPACES][NUM_OF_GL1_DEBUG_CYCLES];


/* Tracks if a RF_AWAKE_REQ was sent in current wake cycle. Cycles without an
   RF_AWAKE_REQ will be very short and so should not be used in averaging by
   the adaptive timeline */
boolean gl1_hw_rf_wakeup_req_msg[NUM_GERAN_DATA_SPACES]= { INITIAL_VALUE( FALSE ) };

/* ============ACI 400 Switchpoint Detection============ */

/* Global Array to hold ACI 400 status across ARFCNs in GL1*/
boolean gl1_hw_aci_table[NUM_GERAN_DATA_SPACES][RR_MAX_PSCAN_FREQ_SCAN_ARRAY_SIZE];
/*===========================================================================

                           FUNCTION PROTOTYPES

===========================================================================*/
static rfm_device_enum_type gl1_hw_grm_init(gas_id_t gas_id);

void rfm_set_lm_info(rfcom_mode_enum_type rf_mode, lm_type link_mgr, uint32 buffer_index);

/* RF init prototype */
static void gl1_hw_rf_init( uint32 rxlm_buf_idx,
                            rfcom_device_enum_type  device,
                            boolean enter_mode,
                            gas_id_t gas_id
                           );

static void gl1_hw_mdsp_init ( boolean enable_mdsp, gas_id_t gas_id );
static boolean gl1_hw_mdsp_inact (boolean panic_inact,gas_id_t gas_id);

/* TCXO init/inact prototypes */
static void gl1_hw_tcxo_init
(
  rex_tcb_type  *task_ptr,
  void (*task_wait_handler)(rex_sigs_type, gas_id_t),
  rex_sigs_type task_wait_sig,
  rex_sigs_type task_done_sig,
  gas_id_t gas_id
);

static void gl1_hw_tcxo_post_init(gas_id_t gas_id);
void gl1_hw_tcxo_inact( boolean deep_sleep,gas_id_t gas_id );

static void gl1_hw_read_nv_items
(
  rex_tcb_type  *tcb_ptr,
  void (*task_wait_handler)(rex_sigs_type, gas_id_t),
  rex_sigs_type wait_sig,
  gas_id_t gas_id
);

static void gl1_hw_check_disabled_features (gas_id_t gas_id);
boolean gl1_is_sleep_disabled( gas_id_t gas_id );
boolean gl1_asynch_int_dbg( gas_id_t gas_id );
boolean gl1_is_asynch_sleep_enabled( gas_id_t gas_id );
boolean gl1_is_sleep_opt3_enabled( gas_id_t gas_id );
boolean gl1_is_sbd_enabled( gas_id_t gas_id );
boolean gl1_is_early_decode_disabled( gas_id_t gas_id );
boolean gl1_is_adaptive_timeline_disabled( gas_id_t gas_id );
boolean gl1_is_sleep_disabled_with_gcf( gas_id_t gas_id );

/* Callback handler in PL1 for mDSP halts. */
static gl1_hw_panic_cb_type gl1_hw_panic_cb;

/* Are we in GSM only or is WCDMA present */
static boolean gl1_hw_gsm_only_mode[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

#ifdef FEATURE_SGLTE
/* Are we in SGLTE mode */
static boolean gl1_hw_sglte_mode[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
#endif

#if ( defined ( MDSP_REG_mdsprxf0vref ) || defined ( MDSP_REG_mdsprxf1vref ) ) \
    && defined ( FEATURE_WCDMA )
static void gl1_hw_mdsp_rxf_low_power( void );
#endif



mdsp_rf_desense_chan_type gl1_hw_is_arfcn_xo_desense( ARFCN_T arfcn,gas_id_t gas_id );

int32 gl1_hw_get_xo_acc_freq_err( gas_id_t gas_id);
int32 gl1_hw_get_xo_acc_freq_err_in_ppm( gas_id_t gas_id );
void  gl1_hw_update_xo_acc_freq_err( int32 xo_freq_err ,gas_id_t gas_id);

static int32 gl1_hw_get_rx_freq( ARFCN_T arfcn,gas_id_t gas_id );

static void  gl1_hw_store_xo_acc_freq_err( int32 xo_value ,gas_id_t gas_id);

uint32 gl1_efs_mdsp_debug=0;
void gl1_hw_efs_read_mdsp_debug(gas_id_t gas_id);

uint32 gl1_efs_mdsp_diag1 = 0;
uint32 gl1_efs_mdsp_diag2 = 0;
uint32 gl1_efs_mdsp_diag3 = 0;
uint32 gl1_efs_mdsp_diag4 = 0;
void gl1_hw_efs_read_mdsp_diag1(gas_id_t gas_id);
void gl1_hw_efs_read_mdsp_diag2(gas_id_t gas_id);
void gl1_hw_efs_read_mdsp_diag3(gas_id_t gas_id);
void gl1_hw_efs_read_mdsp_diag4(gas_id_t gas_id);

/* Gl1 EFS data*/ 
uint32  gl1_efs_debug = 0;
/* Sleep EFS data*/
uint32  gl1_efs_slp = 0;
uint32  gl1_efs_slp_error_rec=0;


uint8 gl1_vamos_support[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
static void gl1_hw_efs_read_vamos_support( gas_id_t gas_id );

#ifdef FEATURE_GSM_SENS_SUPPORT
uint8 gl1_efs_sens_fr_ctrl = FALSE;
void gl1_hw_efs_read_sens_fr_ctrl(void);
#endif

#ifdef FEATURE_GERAN_LOG_OPT
static uint8 gl1_disable_conditional_logging = 0;
static void gl1_hw_efs_read_disable_conditional_logging_flag( void );
#endif

/* Bitmask for Parallel SCH control set to disable */
#define NV_PARALLEL_SCH_DIS     0x01
#define NV_X2G_PARALLEL_SCH_DIS 0x02

/* Enable by default if no NV control exists */
#define NV_PARALLEL_SCH_DEFAULT_CFG 0

static uint8 gl1_parallel_sch_support = NV_PARALLEL_SCH_DEFAULT_CFG;

#define NV_G2X_IDLE_TUNEAWAY_CFG 3
static uint8 gl1_g2x_idle_tuneaway_support = NV_G2X_IDLE_TUNEAWAY_CFG;
#define NV_G2X_NON_DRX_TA_ENA 0x01
#define NV_G2X_RE_ORG_TA_ENA 0x02
static void gl1_hw_efs_read_parallel_sch_support( void );
static void gl1_hw_efs_read_idle_g2x_tuneaway_support( void );

boolean gl1_hw_is_parallel_sch_enabled( void );
boolean gl1_hw_is_x2g_parallel_sch_enabled( void );



#ifdef GL1_GMSK_MOD_DET
#define NV_GDET_DEFAULT_CFG 0x01
#define NV_GDET_DEBUG_CFG 0x02

#define NV_GDET_THRESHOLD_CFG 666
static uint16 gl1_gdet_support  = NV_GDET_DEFAULT_CFG;
static uint16 gl1_gdet_threshold = NV_GDET_THRESHOLD_CFG;
static void gl1_hw_efs_read_gdet_support( void );
static void gl1_hw_efs_read_gdet_threshold_support( void );
boolean gl1_hw_is_gdet_enabled( void );
boolean gl1_hw_is_gdet_debug_enabled (void);

uint16  gl1_hw_is_gdet_threshold_enabled( void );

#ifdef FEATURE_GSM_SYNQ_SUPPORT
#define BAD_GDET_THRESHOLD_COUNT 3 
#define GDET_RECOVERING_THRESHOLD_COUNT 2 

uint8 gl1_efs_synq_fr_enabled = TRUE;
uint8 gl1_efs_bad_gdet_threshold_cnt = BAD_GDET_THRESHOLD_COUNT;
uint8 gl1_efs_gdet_recovering_threshold_cnt = GDET_RECOVERING_THRESHOLD_COUNT;
uint16 gl1_efs_synq_fr_gdet_threshold = NV_GDET_THRESHOLD_CFG;
void gl1_hw_efs_read_synq_fr_ctrl(void);
#endif

#endif
#ifdef FEATURE_AEQ_PHASE4
uint8 gfw_aeq_phase4_control = 0;
#endif /*FEATURE_AEQ_PHASE4 */

#ifdef FEATURE_GSM_TDS
extern boolean mdsp_decrement_startup_time_offset_flag[];
extern boolean mdsp_decrement_cleanup_time_offset_flag[];
extern uint32 gl1_cleanup_decrement_time_offset[];
#endif

#ifdef FEATURE_GSM_RX_TX_SPLIT
static void gl1_hw_rf_start_gsm(gas_id_t gas_id);
static void gl1_hw_rf_stop_gsm(gas_id_t gas_id);
#else
static void gl1_hw_rf_task_enter_mode(uint32 rxlm_buf_idx, rfm_device_enum_type  device,gas_id_t gas_id);
#endif
static void gl1_hw_rf_enable_tx(uint32 txlm_buf_idx,gas_id_t gas_id);
static void gl1_hw_rf_msgr_send_debug(msgr_umid_type umid, errno_enum_type result,gas_id_t gas_id);
static void gl1_hw_rf_msgr_receive_debug(msgr_umid_type umid,
                                         errno_enum_type result,
                                         msgr_umid_type received_umid,
                                         rfa_rf_gsm_cmd_outcome_t req_result,
                                         gas_id_t gas_id);

static void gl1_hw_combined_or_prx_rf_wakeup(uint32 rxlm_buf_idx,
                                             gas_id_t gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
static boolean gl1_hw_rf_msgr_receive_debug_divrx(msgr_umid_type umid,
                                                  errno_enum_type result,
                                                  msgr_umid_type received_umid,
                                                  rfa_rf_gsm_cmd_outcome_t req_result,
                                                  gas_id_t gas_id);
static void gl1_hw_rf_wakeup_divrx(gas_id_t gas_id);
#endif
#if defined (FEATURE_DUAL_ACTIVE) && defined(FEATURE_SGLTE_DUAL_SIM)
static void gl1_handle_multi_sim_mode_switch( void );
typedef struct{
  sys_modem_device_mode_e_type gl1_msg_new_switched_multi_sim_mode;
  boolean gl1_switch_multi_sim_mode;
  boolean trm_status_granted[NUM_GERAN_DATA_SPACES];
} gl1_handle_mmode_switch_type;
static gl1_handle_mmode_switch_type gl1_handle_mmode_switch =
{ SYS_MODEM_DEVICE_MODE_SINGLE_SIM , FALSE, {INITIAL_VALUE(FALSE)} };
#endif
/* Missing from lte_ml1_ext_api.h*/
extern sys_band_class_e_type lte_ml1_convert_lte_band_to_sys_band(sys_sband_lte_e_type lte_band);

/*
** use timer api timer_get_params()
*/
static void gl1_create_hw_timer( rex_tcb_type  *task_ptr, rex_sigs_type task_wait_sig_1, gas_id_t gas_id);

#define GL1_HW_EDGE_CORE_0 0
#define GL1_HW_EDGE_CORE_1 1

/*===========================================================================

                                FUNCTIONS

===========================================================================*/

/*=============================================================================
FUNCTION gl1_hw_fws_x_tech_enabled

DESCRIPTION
   Checks if either LTE/TDS/WCDMA/1x/HDR techs enabled

Parameters
   None

Return
   TRUE if any of other techs are enabled, if not FALSE.

==============================================================================*/
boolean gl1_hw_fws_check_any_x_tech_enabled(void)
{
  uint32 apps_enabled = gl1_hw_fws_app_enabled();
  if( (apps_enabled & FW_APP_WCDMA) ||
      (apps_enabled & FW_APP_LTE) ||
      (apps_enabled & FW_APP_TDSCDMA) ||
      (apps_enabled & FW_APP_CDMA1X) ||
      (apps_enabled & FW_APP_HDR) )
  {
    return TRUE;
  }
  else
  {
   return FALSE;
  }
}

/*=============================================================================
FUNCTION gl1_hw_fws_app_enabled

DESCRIPTION
   Returns all the APPS enabled in FWS

Parameters
   None

Return
   Returns all the APPS enabled in FWS

==============================================================================*/
uint32 gl1_hw_fws_app_enabled(void)
{
  return (fws_get_app_enabled());
}


/*=============================================================================
FUNCTION gl1_hw_get_edge_core

DESCRIPTION
   Gets the edge core from gas_id and sys mode

Parameters
   gas_id_t gas_id

Return
   Edge core assigned to TxChain

==============================================================================*/

uint8 gl1_hw_get_edge_core(gas_id_t gas_id)
{
  sys_modem_device_mode_e_type gl1_sys_mode = gl1_msg_get_multi_sim_sys_mode();

  /* Only select EDGE-Core-1 if in DSDA mode or FTM mode or SS mode and gas_id is 2
     for Tx-config control */
  if ((gl1_sys_mode == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE) || (ftm_get_mode() == FTM_MODE)
      || (gl1_sys_mode == SYS_MODEM_DEVICE_MODE_SINGLE_SIM))
  {
    if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
    {
      return GL1_HW_EDGE_CORE_0;
    } else
    {
      return GL1_HW_EDGE_CORE_1;
    }
  } else
  {
    return GL1_HW_EDGE_CORE_0;
  }
}

/*=============================================================================
FUNCTION gl1_hw_data_init

DESCRIPTION
   Reset the gl1_hw.c static store data

==============================================================================*/
void gl1_hw_data_init(gas_id_t gas_id)
{
  /* used to initialised gl1_hw_data static structure */
  static boolean gl1_hw_data_store_init[NUM_GERAN_DATA_SPACES]  =
                   { INITIAL_VALUE( FALSE ) };

#if defined ( GERAN_L1_HLLL_LNASTATE ) || defined ( GERAN_L1_IP2CAL )
  rfcom_device_enum_type device;
#endif

  if (gl1_hw_data_store_init[gas_id] == FALSE)
  {
    /* preset static data store */

    gl1_hw_data_ptr[gas_id]->band_frequency_mhz = MID_FREQ_GSM_900_BAND;
    gl1_hw_data_ptr[gas_id]->gl1_xo_acc_freq_err = 0;
    gl1_hw_data_ptr[gas_id]->local_active_cswallower_value = 0;

    gl1_hw_data_ptr[gas_id]->gl1_hw_gsm_acch_nv_word = 0x01;
    gl1_hw_data_ptr[gas_id]->gl1_hw_features_disabled = GL1_HW_FEATURE_DISABLE_NONE;
    gl1_hw_data_ptr[gas_id]->nv_read = FALSE;
    gl1_hw_data_ptr[gas_id]->gl1_hw_vco_default = GL1_VCO_UNDEFINED_SETTING;
    gl1_hw_data_ptr[gas_id]->afc_pdm_val = 0;
    gl1_hw_data_ptr[gas_id]->vctcxo_pdm_slope = 0;
    gl1_hw_data_ptr[gas_id]->force_mdsp_reset = FALSE;

    gl1_hw_data_ptr[gas_id]->acq_afc_on_vctcxo = FALSE;


    gl1_hw_data_ptr[gas_id]->afc_rot_val = 0;

    gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx = MDSP_RXLM_BUF_IDX_INVALID;
    gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx = MDSP_TXLM_BUF_IDX_INVALID;
    gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr = NULL;

    gl1_hw_data_ptr[gas_id]->gl1_panic_reset_occurred = FALSE;
    gl1_hw_data_ptr[gas_id]->count_wait_for_mdsp_sleep = 0;

    gl1_hw_data_ptr[gas_id]->gl1_hw_rf_freq_error = 0;
    gl1_hw_data_ptr[gas_id]->gl1_hw_rf_gsm_band = (int8) RFGSM_BAND_NONE;
    gl1_hw_data_ptr[gas_id]->gl1_hw_rf_rx_burst_buf_index = 0;
    gl1_hw_data_ptr[gas_id]->gl1_hw_rf_tx_burst_buf_index = 0;
    gl1_hw_data_ptr[gas_id]->rf_enter_mode_required = 0;
#ifdef GERAN_L1_IP2CAL
    gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx = MDSP_RXLM_BUF_IDX_INVALID;
    gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx = MDSP_TXLM_BUF_IDX_INVALID;
#endif /*GERAN_L1_IP2CAL*/

#if defined ( GERAN_L1_HLLL_LNASTATE ) || defined ( GERAN_L1_IP2CAL )
    for(device=RFM_DEVICE_0;device<RFM_MAX_WAN_DEVICES;device++)
     {
     gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] = 0;
#ifdef GERAN_L1_IP2CAL
     gl1_hw_data_ptr[gas_id]->ip2cal_supported[device] = FALSE;
#endif  /*GERAN_L1_IP2CAL*/
     }
#endif /*GERAN_L1_HLLL_LNASTATE || GERAN_L1_IP2CAL*/

    gl1_hw_data_store_init[gas_id] = TRUE;
  }
  else
  {
    MSG_GERAN_MED_0_G("gl1_hw_data_store already initialized");
  }

}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

/*===========================================================================

FUNCTION  gl1_hw_multi_sim_tcm_fw_init

DESCRIPTION
  This function initializes TCM FW. This moves FW into
  the measurement active state. This must be called by GL1 after the TRM lock
  has been granted.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_multi_sim_tcm_fw_init(GfwAppModeType state)
{



}

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */


int32 l1_get_band_frequency_mhz(gas_id_t gas_id)
{
  return gl1_hw_data_ptr[gas_id]->band_frequency_mhz;
}

void l1_set_band_frequency_mhz(int32 band_frequency_mhz,gas_id_t gas_id)
{
  gl1_hw_data_ptr[gas_id]->band_frequency_mhz=band_frequency_mhz;
}


#define MAX_ROTATOR_PPM_VAL                    3 /* Actual value is 1 / MAX_ROTATOR_PPM_VAL */

/*===========================================================================

FUNCTION gl1_hw_max_rotator_correction_in_hz

DESCRIPTION
  Returns the max rotator setting in HZ for TXC0 Mgr 3.0, based on the
  max required ppm setting from GPS (in this case 0.33ppm).

DEPENDENCIES
  None

RETURN
  int32 - Max rotator value in Hz

SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_hw_max_rotator_correction_in_hz( gas_id_t gas_id )
{
  int32 max_rot_correction;
  int32 round =
          gl1_hw_get_round_val( gl1_hw_data_ptr[gas_id]->band_frequency_mhz, 1,
                                MAX_ROTATOR_PPM_VAL );

  max_rot_correction = ( ( gl1_hw_data_ptr[gas_id]->band_frequency_mhz + round ) / MAX_ROTATOR_PPM_VAL );

  return( max_rot_correction );
}

/*===========================================================================

FUNCTION  gl1_hw_shared2gsm

DESCRIPTION
   This function returns converted int16 to int32 bit vco value for GL1 PDM.

RETURN

   int32  PDM value for GL1 to use.

===========================================================================*/
int32 gl1_hw_shared2gsm(int16 vco_value)
{
  /* Convert to local units */
  return (PDM_SHARED2GSM(vco_value));
}

/*===========================================================================

FUNCTION  gl1_hw_shared2gsm

DESCRIPTION
   This function returns converted int16 to int32 bit vco value for GL1 PDM.

RETURN

   int32  PDM value for GL1 to use.

===========================================================================*/
int32 gl1_hw_gsm2shared(int32 pdm_value)
{
  /* Convert to local units */
  return (PDM_GSM2SHARED(pdm_value));
}

/*===========================================================================

FUNCTION  gl1_hw_map_gas_id_to_mcpcm_tech

DESCRIPTION
  This function maps gas_id into MCPM_TECH.

===========================================================================*/
mcpm_tech_type gl1_hw_map_gas_id_to_mcpm_tech( gas_id_t gas_id )
{
   /* Get MCPM GSTMR Lock and check GSM clock is on */
   mcpm_tech_type mcpm_tech = MCPM_GERAN_TECH;

   /* Check which GSTMR HW block this handler was fired on*/
   switch ( gas_id )
   {
     case GERAN_ACCESS_STRATUM_ID_1:
       mcpm_tech = MCPM_GERAN_TECH;
     break;

#if defined  ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     case GERAN_ACCESS_STRATUM_ID_2:
       mcpm_tech = MCPM_GERAN1_TECH;
     break;

#if defined ( FEATURE_TRIPLE_SIM )
     case GERAN_ACCESS_STRATUM_ID_3:
       mcpm_tech = MCPM_GERAN2_TECH;
     break;
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

     case GERAN_ACCESS_STRATUM_ID_UNDEFINED:
     default:
       MSG_GERAN_FATAL_0_G( "Unknown GSTMR config" );
     break;
   }

   return ( mcpm_tech );
}

extern void gl1_task_mutex_lock( gas_id_t gas_id );
extern void gl1_task_mutex_unlock( gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_gfw_recovery_read_nv

DESCRIPTION
  Reads nv to find Recovery is allowed or not

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void gl1_gfw_recovery_read_nv(gas_id_t gas_id)
{
  int efs_ret_val = -1;
  boolean gl1_gfw_crash = FALSE;
  /* Get the NV item values */
  efs_ret_val = geran_efs_read_primary(GL1_NV_FW_RECOVERY, (void*)&(gl1_gfw_nv_recovery), sizeof(gl1_gfw_nv_recovery));


  if(efs_ret_val == -1)
{
#ifdef FEATURE_GSM_GFW_CRASH_ON_ERROR
    /* By default initializing to TRUE (force crash), if nothing set.*/
    MSG_GERAN_HIGH_0_G("gl1_gfw_recovery_read_nv: Could not read NV item, force crash on error");
    gl1_gfw_nv_recovery = GL1_GFW_CRASH_IN_FW;
    gl1_gfw_crash = TRUE;
#else
    /* By default initializing to FALSE (enable recovery), if nothing set.*/
    MSG_GERAN_HIGH_0_G("gl1_gfw_recovery_read_nv: Could not read NV item, allow recovery");

    /* to enable specific recovery comment out approriate line below, otherwise it will crash on error*/
    gl1_gfw_nv_recovery = GL1_GFW_RECOVER_AND_MOVE_ON 
                        //| GL1_GFW_ACQ_DELAYED_CRASH_IN_SW
                        //| GL1_GFW_X2G_ACQ_CANCEL_CRASH_IN_SW
                        //| GL1_GFW_RX_BLANKING_CRASH_IN_SW
                        //| GL1_GFW_SEQ_NUM_REC_MON_CRASH_IN_SW
                        | GL1_GFW_SEQ_NUM_REC_RX_CRASH_IN_SW
                        | GL1_GFW_SEQ_NUM_REC_TX_CRASH_IN_SW
                        //| GL1_GFW_SEQ_NUM_REC_T2G_MON_CRASH_IN_SW
                          ;
#endif
  }
  else
  {
      switch (gl1_gfw_nv_recovery & GL1_GFW_RF_RECOVERY_NV_MASK) 
      {
       case GL1_GFW_RECOVER_AND_MOVE_ON:
  {
             MSG_GERAN_HIGH_1_G("gl1_gfw_recovery_read_nv: recovery value read :%d, recover in GFW, do nothing in SW",gl1_gfw_nv_recovery);
             break;
  }

       case GL1_GFW_CRASH_IN_FW:
          {
             gl1_gfw_crash = TRUE;
             MSG_GERAN_HIGH_1_G("gl1_gfw_recovery_read_nv: recovery value read :%d, crash in GFW",gl1_gfw_nv_recovery);
             break;
          }

       case GL1_GFW_CRASH_IN_SW:
          {
             MSG_GERAN_HIGH_1_G("gl1_gfw_recovery_read_nv: recovery value read :%d, crash in SW",gl1_gfw_nv_recovery);
             break;
          }

       case GL1_GFW_CALL_DROP:
          {
             MSG_GERAN_HIGH_1_G("gl1_gfw_recovery_read_nv: recovery value read :%d, call drop in SW",gl1_gfw_nv_recovery);
             break;
          }
       default:
  {
             MSG_GERAN_HIGH_1_G("gl1_gfw_recovery_read_nv: recovery value read :%d, unexpected value ",gl1_gfw_nv_recovery);
             break;
          }
  }


      if ((gl1_gfw_nv_recovery & GL1_GFW_ACQ_DEL_RECOVERY_NV_MASK) == GL1_GFW_ACQ_DELAYED_CRASH_IN_SW)
{
         MSG_GERAN_HIGH_1_G("gl1_gfw_recovery_read_nv: recovery value read :%d, ACQ DELAYED crash",gl1_gfw_nv_recovery);
}

      if ((gl1_gfw_nv_recovery & GL1_GFW_RX_BLANKING_RECOVERY_NV_MASK) == GL1_GFW_RX_BLANKING_CRASH_IN_SW)
      {
         MSG_GERAN_HIGH_1_G("gl1_gfw_recovery_read_nv: recovery value read :%d, RX BLANKING crash",gl1_gfw_nv_recovery);
      }
  }
  mdsp_update_nv_gfw_recovery(gas_id, gl1_gfw_crash);
}

void gl1_hw_init
(
  void                (*tdma_isr_ptr)(sys_modem_as_id_e_type as_id),
  rex_tcb_type       *task_ptr,
  void                (*task_wait_handler)(rex_sigs_type, gas_id_t),
  rex_sigs_type       task_wait_sig_1,
  rex_sigs_type       task_wait_sig_2,
  gl1_init_rflm_type  gsm_init_rflm_state,
  gas_id_t            gas_id
)
{
   ARFCN_T arfcn;

   boolean enable_mdsp_required;
#ifdef FEATURE_GSM_WTR_HOP
   rfm_device_enum_type  device;
#else
   rfcom_device_enum_type  device;
#endif /*FEATURE_GSM_WTR_HOP*/

   lm_status_type lm_status;

   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id( gas_id );

   volatile gl1_mcpm_state_type mcpm_geran_start_req_complete;
   mcpm_tech_type mcpm_tech = gl1_hw_map_gas_id_to_mcpm_tech( gas_id );

#ifdef FEATURE_GSM_RX_DIVERSITY
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   grm_client_enum_t divrx_client = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/

   gl1_task_mutex_lock(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* set the global flag for this */
   gl1_set_gfw_app_mode_status( GFW_APP_MODE_IDLE, gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  gl1_hw_data_ptr[gas_id]->gl1_panic_reset_occurred = FALSE;

   /* If below flag is still TRUE, it triggers 'GL1_MCPM_GERAN_STOP' on receiving G2X deinit confirm
   ** But since GSM is already initializing here, MCPM need not be stopped from there */ 
   gl1_deep_sleep_pending[gas_id] = FALSE;

   gl1_clkdata_state_init(gas_id);
   MSG_GERAN_HIGH_0_G( "MCPM change for GERAN Start");
   gl1_hw_mcpm_set_state(GL1_MCPM_GERAN_STARTING,gas_id);
   gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_START_REQ, GL1_MCPM_GERAN_START,gas_id);

   /* Wait for MCPM Geran Start to complete */
   mcpm_geran_start_req_complete = gl1_hw_mcpm_get_state(gas_id);
   while (mcpm_geran_start_req_complete != GL1_MCPM_GERAN_START)
   {
      gl1_hw_delay(GL1_HW_MCPM_START_STOP_DELAY_LOOP,gas_id);
      mcpm_geran_start_req_complete = gl1_hw_mcpm_get_state(gas_id);
   }

   /* Register callback when coming out of power collapse */
   if( gas_id == GERAN_ACCESS_STRATUM_ID_1 )
   {
       MCPM_Set_Block_Restore_Callback( mcpm_tech, MCPM_TX_BLOCK,
                                        gl1_hw_mcpm_tx_on_gas1_cb );
   }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   else if( gas_id == GERAN_ACCESS_STRATUM_ID_2 )
   {
       MCPM_Set_Block_Restore_Callback( mcpm_tech, MCPM_TX_BLOCK,
                                        gl1_hw_mcpm_tx_on_gas2_cb );
   }
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3)
   {
      MCPM_Set_Block_Restore_Callback( mcpm_tech, MCPM_TX_BLOCK,
                                       gl1_hw_mcpm_tx_on_gas3_cb );
   }
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   gl1_create_hw_timer( task_ptr, task_wait_sig_1, gas_id);

#ifndef FEATURE_WPLT

   /* If CM still active then terminate now */
   if (!gl1_hw_cm_get_status(gas_id))
   {
      enable_mdsp_required = TRUE;
   }
   else
   {
     /* As calling RAT did not call terminate CM, complete this localy here.*/
     gl1_hw_complete_cm_terminate(gas_id);
     enable_mdsp_required = FALSE;
   }

   /* Reset the GSM core - must be done after enabling the clk regimes */
   gstmr_gsm_core_reset();

   /* Flag that we have entered gsm mode */
   gl1_hw_mode_gsm[gas_id] = TRUE;

   /* Initialize sleep controller for GSM. */
   gl1_hw_sleep_gsm_init( gas_id );

   /* Init the gstmr - should be called before gl1_hw_mdsp_init */
   /* for now set gstmr 0 = gas id 1. gstmr 1 = gas_id 2*/
   gl1_hw_multi_sim_gstmr_init( gas_id );

   /* Initialiase mdsp shared interface */
   mdsp_init_MdspHostIfBaseStore(gas_id);

   /* Initialiase mdsp intf data spaces */
   mdsp_init_public_data_space(gas_id);
   mdsp_init_private_data_space(gas_id);

   /* Init the mdsp */
   gl1_hw_mdsp_init(enable_mdsp_required, gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
   MSG_GERAN_HIGH_1_G(GL1_RXD"gsm_init_rflm_state:%d",gsm_init_rflm_state);

      /*If not in FTM mode check the RxD EFS item 71588 and update the RxD control flags accordingly */
      if (ftm_get_mode() != FTM_MODE)
      {
        gl1_hw_get_rxd_efs_item(gas_id);
      }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

   /*
   TRM request to get RF device must be called before gl1_hw_async_intf_app_mode_config .
   if the order is reversed then an invalid RF device will be returned by MCS and channel ID will default to zero.
   This causes coex issues in SGTDS mode as T and G were both using same channel ID i.e 0 instead of G
   being on channel ID 2
     */
   device = gl1_hw_grm_init(gas_id);

   gl1_hw_async_intf_app_mode_config(GFW_APP_MODE_ACTIVE, geran_map_gas_id_to_nas_id(gas_id));

   MSG_GERAN_MED_3_G("GL1 HW Init type %d, RxLM buffer %d, TxLM buffer %d",
                     gsm_init_rflm_state,
                     gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx,
                     gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx);
   if (gsm_init_rflm_state != GL1_HW_INIT_INACTIVE)
   {
     /* Check that RxLM buffer not already allocated */
     gl1_hw_deallocate_rxlm_buf_idx(gas_id);

     /* Allocate RxLM buffer */
     lm_status = rflm_allocate_rx_handle(LM_GSM, LM_GSM, &gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx);
     if (lm_status == LM_SUCCESS)
     {
       MSG_GERAN_MED_1_G("PRX GSM RxLM buffer %d allocated",gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx);
     }
     else
     {
#ifdef FEATURE_QSH_MDUMP
       QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
       ERR_GERAN_FATAL_0_G("Unable to allocate RxLM buffer");
     }

     /* Check that TxLM buffer not already allocated */
     gl1_hw_deallocate_txlm_buf_idx(gas_id);

     /* Allocate TxLM buffer, get buffer index (keep trying until allocated)*/
     lm_status = rflm_allocate_tx_handle(LM_GSM, LM_GSM, &gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx);
     if (lm_status == LM_SUCCESS)
     {
       MSG_GERAN_MED_1_G("GSM TxLM buffer %d allocated",gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx);
     }
     else
     {
#ifdef FEATURE_QSH_MDUMP
       QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
       ERR_GERAN_FATAL_0_G("Unable to allocate TxLM buffer");
     }
#ifdef FEATURE_GSM_RX_DIVERSITY
     if (divrx_client != GRM_NO_CLIENT)
     {
       if (GL1_TRM_GRANTED == grm_get_trm_status(divrx_client, gas_id))
     {
         if (gl1_hw_trm_init_DivRx(gas_id) && gl1_alloc_IMRD_rxdiv_rxlm_buff_idx(gas_id))
     {
           MSG_GERAN_HIGH_3_G(GL1_RXD"gl1_hw_init: rxdiv SINGLE SIM rf id:%i, drx_rxlm_idx =%i, enableIMRD = %i",
                              gl1_hw_get_gsm_rf_id(divrx_client, gas_id),
                              gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id),
                              gl1_get_IMRD_ctl_nv(gas_id));
     }
     else
     {
           MSG_GERAN_ERROR_2_G(GL1_RXD"gl1_hw_init: RxD SS no device or no RXLM buff idx or flag disabled; "
                               "IMRD_rxlm_buff_idx=%i, enableIMRD = %i",
                               gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id),
                               gl1_get_IMRD_ctl_nv(gas_id));
           gl1_hw_cleanup_divrx_rfdevice_info(gas_id);
         }
     }
     }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   }

   gl1_hw_data_ptr[gas_id]->rf_enter_mode_required = TRUE;
   gl1_hw_rf_init(gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx,device, gl1_hw_data_ptr[gas_id]->rf_enter_mode_required, gas_id);

#if defined( FEATURE_GSM_TX_DIV_ANTNA_SWITCHING) ||defined( FEATURE_GSM_QSC_TX_DIV)
/*Set the current configuration in MCS if feature enabled, otherwise use the Config stored in NV 70219*/

   if (gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE1) || gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE2))
   {
      gl1_hw_init_asdiv_preferred_antenna(gas_id, TRM_ANT_SWITCH_CONFIG_DEFAULT);
   }
   else
   {
  		gl1_hw_init_asdiv_preferred_antenna(gas_id, TRM_ANT_SWITCH_CONFIG_DEFAULT);
   }
 
#endif
   /* Init the gsm vctcxo control, must be called after rf init */
   gl1_hw_tcxo_init(task_ptr, task_wait_handler, task_wait_sig_1, task_wait_sig_2,gas_id);

#ifndef FEATURE_GSM_RX_TX_SPLIT
   gl1_hw_async_intf_enter_mode_cmd(GFW_APP_MODE_ACTIVE,
                    gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx,
#ifdef FEATURE_GSM_RX_TX_SPLIT
                                    (uint32)-1,
                                    gl1_hw_get_gfw_entermode_type(RFGSM_PRX_OP_MODE, gas_id),
#else
                                    gl1_hw_get_gfw_entermode_type(RFGSM_PRX_WAKEUP_MODE, gas_id),
#endif
                    device,
                    geran_map_gas_id_to_nas_id(gas_id));

#ifdef FEATURE_GSM_RX_DIVERSITY
   if (GL1_GFW_PRX_DIVRX_ENTER == gl1_hw_get_gfw_enter_status(gas_id)) 
   {
     gl1_hw_set_rxdiv_wait_for_block_start(TRUE, gas_id);
   }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#endif /*FEATURE_GSM_RX_TX_SPLIT*/

   /* Do TCXO post-init */
   gl1_hw_tcxo_post_init(gas_id);

  /* Set initial values for GFW freq correction data */
   /* Update cswallower/DCO for GSTMR, use nominal arfcn */
   gl1_msg_algs_get_arfcn( &arfcn.num, &arfcn.band, gas_id );
   gl1_hw_set_active_cycle_swallower_adj( arfcn,gas_id );

#ifdef GERAN_L1_IP2CAL
   if(((gsm_init_rflm_state == GL1_HW_INIT_NON_SIGNALLING)||(gsm_init_rflm_state == GL1_HW_INIT_RX_TX))
      && (gl1_hw_get_ip2cal_support(gas_id)))
     {
     MSG_GERAN_MED_3_G("GL1 HW Init type %d, IP2RxLM buffer %d, IP2TxLM buffer %d", gsm_init_rflm_state,
                        gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx,
                        gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx);

     /* Check that RxLM buffer not already allocated */
     if(gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx != MDSP_RXLM_BUF_IDX_INVALID)
      {
       MSG_GERAN_ERROR_1_G("GSM IP2Cal RxLM buffer %d already allocated - deallocate now", gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx);
       //rxlm_deallocate_buffer(gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx);
       rflm_deallocate_rx_handle(LM_GSM, LM_GSM, gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx);
       gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx = MDSP_RXLM_BUF_IDX_INVALID;
      }
     /* Allocate RxLM buffer */
     lm_status = rflm_allocate_rx_handle(LM_GSM, LM_GSM, &gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx);
     if(lm_status == LM_SUCCESS)
      {
      MSG_GERAN_MED_1_G("GSM IP2RxLM buffer %d allocated",gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx);
      }
     else
      {
      MSG_GERAN_ERROR_0_G("Unable to allocate IP2RxLM buffer");
      }
     /* Check that TxLM buffer not already allocated */
     if(gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx != MDSP_TXLM_BUF_IDX_INVALID)
      {
      MSG_GERAN_ERROR_1_G("GSM IP2TxLM buffer %d already allocated - deallocate now", gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx);
      rflm_deallocate_tx_handle(LM_GSM, LM_GSM,gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx);
      gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx = MDSP_RXLM_BUF_IDX_INVALID;
      }
     /* Allocate TxLM buffer, get buffer index (keep trying until allocated)*/
     lm_status = rflm_allocate_tx_handle(LM_GSM, LM_GSM, &gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx);
     if(lm_status == LM_SUCCESS)
      {
      MSG_GERAN_MED_1_G("GSM IP2TxLM buffer %d allocated",gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx);
      }
     else
      {
      MSG_GERAN_ERROR_0_G("Unable to allocate IP2TxLM buffer");
      }
     gl1_hw_rf_init_ip2cal(gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx,gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx,
                           device,gas_id);
     }
#endif

   /* Now that init is finished, install the normal frame handler */
   gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr = tdma_isr_ptr;

   if ( gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr != NULL )
   {
     if ( gstmr_is_handler_registered_geran( gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr, as_id ) )
     {
       gstmr_deregister_handler_geran( gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr, as_id );
     }

     gstmr_register_handler_geran(gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr, as_id );
   }

   /* Read any NV items used by NPL1 */
   gl1_hw_read_nv_items(task_ptr, task_wait_handler, task_wait_sig_1,gas_id);

   /* Check FW/SW feature compatibility*/
   mdsp_feature_check (gas_id);

   /* Disable any features requested - must come after reading NV and checking mdsp */
   gl1_hw_check_disabled_features(gas_id);

   /* register NPA cb after reading the GCF NV & updating into gl1_hw_sleep_disable*/
   /*GCF global variable 'gl1_hw_sleep_disable' is updated inside the gl1_hw_check_disabled_features*/
   gl1_hw_sleep_register_npa_cb(gas_id);


#ifdef FEATURE_AEQ_PHASE4
     gl1_hw_efs_read_set_aeq4(gas_id);
#endif /*FEATURE_AEQ_PHASE4  */

   /* Changed by systems from 102 to 103; This is really -103 dBm */
   /* Now changed to -107 dBm ,  and there is a mDSP change also. */

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
   gl1_hw_iq_samples_init( gas_id );

   /* Examples of how to enable specific channels for logging:
    * gl1_hw_iq_samples_force_enable( GL1_HW_SACCH,  TRUE, GERAN_ACCESS_STRATUM_ID_1 );
    * gl1_hw_iq_samples_force_enable( GL1_HW_TCH_FS, TRUE, GERAN_ACCESS_STRATUM_ID_2 );
    * gl1_hw_iq_samples_force_enable( GL1_HW_FACCH,  TRUE, GERAN_ACCESS_STRATUM_ID_1 );
    *
    * GPRS doesn't distinguish individual channels type for data
    * gl1_hw_iq_samples_force_enable( GL1_HW_PDTCHD,  TRUE, GERAN_ACCESS_STRATUM_ID_3 );
    *
    * Also the IQ log packet number is 0x522F in QXDM
    */
#endif  /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */

#ifndef FEATURE_GSM_TO_WCDMA_MSGR
#ifdef FEATURE_WCDMA
   /* Set initial wcdma srch status to off */
   gl1_hw_set_wcdma_srch_on( FALSE );
#endif
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

   /* Configure SAIC at GSM Initialisation now. */
   /* This is needed due to the mDSP  chan type */
   /* processing;  chan type ,and CS are mixed. */
   gl1_hw_cfg_saic( TRUE,gas_id );


   /* At GSM RAT init make sure the UL TS is "1". This */
   /* also makes sure for WtG Handovers the UL is set. */
   gl1_hw_set_num_assigned_tx_slots (GL1_DEFAULT_ONE_TS,gas_id);

#endif /* FEATURE_WPLT */

#ifdef GERAN_L1_ENHANCED_RX
  gl1_hw_enable_lif_in_ftm_cal(FALSE, gas_id);
  if((ftm_get_mode() == FTM_MODE) 
     &&((gsm_init_rflm_state == GL1_HW_INIT_RX_TX) || (gsm_init_rflm_state == GL1_HW_INIT_RX_ONLY))
     && gl1_hw_get_lif_feature_flag(gas_id))
  {
     gl1_hw_enable_lif_in_ftm_cal(TRUE, gas_id);
  }
#endif

  if (ftm_get_mode() == FTM_MODE)
  {
   GDRV_ISR_LOCK(gas_id);
     gl1_hw_gfw_config_cycle_swallower(gas_id);
	 /* Adding half slot delay for sub#2 for FTM to avoid RFFE bus contention when RX on both devices.
     It won't affect normal operation as frame will be aligned during camping. */
     if (GERAN_ACCESS_STRATUM_ID_2 == gas_id)
     {
        gl1_hw_delay_frame_tick(900, gas_id);
     }
     GDRV_ISR_UNLOCK(gas_id);
     MSG_GERAN_HIGH_0_G( "MCPM change for Non-signalling mode");
     gl1_hw_mcpm_state_update(MCPM_GERAN_DATA_START_REQ, GL1_MCPM_GERAN_FTM,gas_id);
  }
  /* GL1_HW_INIT_RX_TX will be used by RF when passing a valid Tx capable RF device */
  if ((gsm_init_rflm_state == GL1_HW_INIT_NON_SIGNALLING)||(gsm_init_rflm_state == GL1_HW_INIT_RX_TX))
  {
     gl1_hw_turn_tx_on(gas_id);
  }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  wmgr_register_rat_cb( gl1_hw_map_gas_id_to_wmgr_client( gas_id ),
                        gl1_hw_sleep_wake_up_manager_cb_handler, NULL );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#if ( defined (FEATURE_GSM_COEX) && defined (FEATURE_SGLTE) ) || defined (FEATURE_WLAN_COEX_SW_CXM)
  /* The GAS is registered with the arbitrator for SGLTE mode to manage L+G COEX*/
#ifndef FEATURE_WLAN_COEX_SW_CXM
  if(gl1_hw_sglte_mode[gas_id])
#endif
  {
    gl1_hw_arbitration[gas_id].registration(FALSE, gas_id);
  }
#endif

#ifdef FEATURE_QTA
    /* This needs to be reset here if it is multimode sub. QTA gap can only be active if the multimode sub is on non-GERAN I-RAT
       But we should avoid it when we are in SGLTE+G DSDA mode, since QTA can happen on multimode sub */
    if(geran_is_multi_mode_gas_id(gas_id)
#ifdef FEATURE_SGLTE
       && !gl1_hw_get_sglte_plus_g_mode(gas_id)
#endif
       )
    {
      MSG_GERAN_MED_0_G("gl1_hw_init: resetting gl1_qta_wcdma_has_initialised flag");
      gl1_hw_cm_set_qta_wcdma_has_initialised(FALSE, gas_id);
    }
#endif

  gl1_task_mutex_unlock(gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_multi_sim_gstmr_init

DESCRIPTION
  This function performs basic initialisation when starting up in dual sim mode

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_multi_sim_gstmr_init( gas_id_t gas_id )
{
   /* Init the gstmr - should be called before gl1_hw_mdsp_init */
   /* for now set gstmr 0 = gas id 1. gstmr 1 = gas_id 2*/
   gstmr_init( gas_id );
}


#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION  gl1_hw_multi_sim_deact

DESCRIPTION
  This function performs any cleanup necessary when releasing the lock
  in dual sim operations.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_multi_sim_deact(gas_id_t gas_id)
{
  ustmr_type start_ustmr, end_ustmr, duration_ustmr, duration_us;
 #ifdef FEATURE_QTA
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif
  gl1_hw_sleep_type  *gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];

  gl1_task_mutex_lock( gas_id );

  start_ustmr = slpc_get_tick();
   gl1_hw_sleep_cycle->start.deact_sleep.start_ustmr = start_ustmr;

  MSG_GERAN_HIGH_0_G( "gl1_hw_multi_sim_deact START exit_mode");

  GL1_HANDLER_PERFORMANCE_LOGGING(START_MULTISIM_INACT_TCXO,gas_id);

  gl1_hw_tcxo_inact( FALSE, gas_id );

  GL1_HANDLER_PERFORMANCE_LOGGING(STOP_MULTISIM_INACT_TCXO,gas_id);

  GL1_HANDLER_PERFORMANCE_LOGGING(START_MULTISIM_INACT_TXRX,gas_id);

#ifdef FEATURE_QTA
  /* do not touch the transmitter and receiver in QTA gap - they are registered with W */
  if (l1_tskisr_blk->qta_in_progress == FALSE)
#endif
      {
    /* always set GFW APP to MEAS_IDLE for every TRM release and TRM reserve_at
         and that we havent reached end of frame before calling set app mode */
      mdsp_is_mdsp_idle(gas_id);

      rex_enter_crit_sect(&gl1_hw_sleep_warmup_cb_crit_sect[gas_id]);

      /* Put GFW back into meas mode when deactivating Sub after TRM lock is released - need to revisit */
      gl1_hw_async_intf_app_mode_config(GFW_APP_MODE_MEASUREMENT, geran_map_gas_id_to_nas_id(gas_id));
	  
      rex_leave_crit_sect(&gl1_hw_sleep_warmup_cb_crit_sect[gas_id]);

    /* Turn off Tx if NOT GTA mode - for GTA mode, Tx will not have been requested, so don't waste time voting it off and printing
       associated F3 messages*/
    if ( !gl1_msg_get_multi_sim_standby_mode() ||
         !OTHER_GSM_CLIENT_IN_TRANSFER(gas_id) )
    {
      if (!gl1_hw_turn_tx_off(TRUE, gas_id))
      {
        MSG_GERAN_ERROR_0_G("GL1 dual sim unable to disable tx");
      }
      else
      {
        gl1_clear_drx_tx_status(gas_id);
      }
    }

    /* for RFLM/FED architecture, for every TRM release or reserve_at, each sub should always call RF_sleep, not RF_exit and
       there is no requirement to check the status of other subs */
    MSG_GERAN_HIGH_0_G("RF to Sleep");
    gl1_hw_turn_rx_off(gas_id);

  }

  GL1_HANDLER_PERFORMANCE_LOGGING(STOP_MULTISIM_INACT_TXRX,gas_id);

#ifdef GERAN_L1_IP2CAL
  if(gl1_hw_get_ip2cal_support(gas_id))
   {
   /* Deallocate IP2Cal RxLM and TxLM buffers */
   gl1_hw_deallocate_ip2cal_txlm_buf_idx(gas_id);
   gl1_hw_deallocate_ip2cal_rxlm_buf_idx(gas_id);
   }
#endif /*GERAN_L1_IP2CAL*/

  gl1_hw_multi_sim_gonly_mode = FALSE;

  end_ustmr = slpc_get_tick();
  duration_ustmr =  ( end_ustmr - start_ustmr );
  duration_us = ( duration_ustmr / USTMR_CLK_FREQUENCY );
  MSG_GERAN_HIGH_1_G( "gl1_hw_multi_sim_deact END duration_us %d", duration_us);
  gl1_hw_sleep_debug_duration ( &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]].start.deact_sleep);

  gl1_task_mutex_unlock(gas_id );
}

#ifdef FEATURE_G2X_TUNEAWAY
void gl1_hw_multi_sim_deact_g2x(gas_id_t gas_id)
{
  ustmr_type start_ustmr, end_ustmr, duration_ustmr, duration_us;
  gl1_task_mutex_lock(gas_id);
  start_ustmr = slpc_get_tick();

  MSG_GERAN_HIGH_0_G("gl1_hw_multi_sim_deact_g2x START");

  gl1_hw_tcxo_inact(FALSE, gas_id);
  /* Initially Required for TBF suspend on a block boundary */
   /* check that the firmware is idle and not processing any commands
           and that we havent reached end of frame before calling set app mode */
   mdsp_is_mdsp_idle(gas_id);

    /* Put GFW back into meas mode when deactivating Sub after TRM lock is released - need to revisit */
    gl1_hw_async_intf_app_mode_config(GFW_APP_MODE_MEASUREMENT, geran_map_gas_id_to_nas_id(gas_id));

  if (!gl1_hw_turn_tx_off(TRUE, gas_id))
  {
    MSG_GERAN_ERROR_0_G("GL1 dual sim unable to disable tx");
  }
  else
  {
    gl1_clear_drx_tx_status(gas_id);
  }

  /* Turn the RX and VREG OFF (for RFLM/FED this is part of rx_off). For MSIM this is not on going to sleep */
  gl1_hw_turn_rx_off(gas_id);

  /* Deallocate RxLM buffer */
    gl1_hw_deallocate_rxlm_buf_idx(gas_id);

#ifdef GERAN_L1_IP2CAL
  if(gl1_hw_get_ip2cal_support(gas_id))
    {
    /* Deallocate IP2Cal RxLM and TxLM buffers */
    gl1_hw_deallocate_ip2cal_txlm_buf_idx(gas_id);
    gl1_hw_deallocate_ip2cal_rxlm_buf_idx(gas_id);
    }
#endif /*GERAN_L1_IP2CAL*/

  end_ustmr = slpc_get_tick();
  duration_ustmr =  (end_ustmr - start_ustmr);
  duration_us = (duration_ustmr / USTMR_CLK_FREQUENCY);
  MSG_GERAN_HIGH_1_G("gl1_hw_multi_sim_deact_g2x END duration_us %d", duration_us);

  gl1_task_mutex_unlock(gas_id);
}
#endif /* FEATURE_G2X_TUNEAWAY */
/*===========================================================================

FUNCTION  gl1_hw_set_multi_sim_sleep

DESCRIPTION
  This function sets the variable to allow the sleep to operate in a G+G MSIM
  setup

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_set_multi_sim_sleep( boolean allowed,gas_id_t gas_id )
  {
#if !defined (FEATURE_DUAL_ACTIVE)
  gl1_hw_multi_sim_gonly_mode = allowed;
#else
  NOTUSED(allowed);
  NOTUSED(gas_id);
#endif /* !FEATURE_DUAL_ACTIVE */
  }

/*===========================================================================

FUNCTION  gl1_hw_multi_sim_init

DESCRIPTION
  This function performs basic initialisation when starting up in dual sim mode

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_multi_sim_init
(
  void          (*tdma_isr_ptr)(sys_modem_as_id_e_type as_id),
  rex_tcb_type  *task_ptr,
  void          (*task_wait_handler)(rex_sigs_type, gas_id_t),
  rex_sigs_type task_wait_sig_1,
  rex_sigs_type task_wait_sig_2,
  gas_id_t      gas_id
)
{
   volatile boolean mcpm_geran_start_req_complete = FALSE;
   uint32 fws_enable_mask = 0;
   boolean app_mode_meas_idle_reqd = FALSE;
   boolean enable_mdsp_required;
   sys_modem_as_id_e_type as_id;
   mcpm_tech_type mcpm_tech;
   
   as_id = geran_map_gas_id_to_nas_id( gas_id );
   mcpm_tech = gl1_hw_map_gas_id_to_mcpm_tech( gas_id );

   gl1_task_mutex_lock(gas_id);

   /* set the global flag for the driver mode */
   gl1_set_gfw_app_mode_status( GFW_APP_MODE_IDLE, gas_id );

   gl1_hw_multi_sim_gonly_mode = FALSE;
   gl1_hw_data_ptr[gas_id]->gl1_panic_reset_occurred = FALSE;

   /* If below flag is still TRUE, it triggers 'GL1_MCPM_GERAN_STOP' on receiving G2X deinit confirm
   ** But since GSM is already initializing here, MCPM need not be stopped from there */ 
   gl1_deep_sleep_pending[gas_id] = FALSE;

   gl1_hw_data_ptr[gas_id]->rf_enter_mode_required = TRUE;

   /* Clear the firmware loaded status here */
   l1_tsk_buffer[gas_id].firmware_loaded = FALSE;

   grm_set_trm_requested_at_warmup(FALSE, l1_tsk_buffer[gas_id].client_id, gas_id);

   l1_tsk_buffer[gas_id].first_multi_sim_init = FALSE;

   gl1_clkdata_state_init(gas_id);
   MSG_GERAN_HIGH_0_G( "MCPM change for GERAN Start");
   gl1_hw_mcpm_set_state(GL1_MCPM_GERAN_STARTING,gas_id);
   gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_START_REQ, GL1_MCPM_GERAN_START,gas_id);

   /* Wait for MCPM Geran Start to complete */
   mcpm_geran_start_req_complete = gl1_hw_mcpm_get_state(gas_id);
   while (mcpm_geran_start_req_complete != GL1_MCPM_GERAN_START)
   {
      gl1_hw_delay(GL1_HW_MCPM_START_STOP_DELAY_LOOP,gas_id);
      mcpm_geran_start_req_complete = gl1_hw_mcpm_get_state(gas_id);
   }

/* Register callback when coming out of power collapse */
   if( gas_id == GERAN_ACCESS_STRATUM_ID_1 )
   {
     MCPM_Set_Block_Restore_Callback( mcpm_tech, MCPM_TX_BLOCK,
                                      gl1_hw_mcpm_tx_on_gas1_cb );
   }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   else if( gas_id == GERAN_ACCESS_STRATUM_ID_2 )
   {
     MCPM_Set_Block_Restore_Callback( mcpm_tech, MCPM_TX_BLOCK,
                                      gl1_hw_mcpm_tx_on_gas2_cb );
   }
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3)
   {
     MCPM_Set_Block_Restore_Callback( mcpm_tech, MCPM_TX_BLOCK,
                                       gl1_hw_mcpm_tx_on_gas3_cb );
   }
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   gl1_create_hw_timer( task_ptr, task_wait_sig_1, gas_id);

#ifndef FEATURE_WPLT
   /* If CM init still active then terminate now so we can full-stack init in a bit
      (Should only happen with T->G HHO) */
   if (!gl1_hw_cm_get_status(gas_id))
   {
     enable_mdsp_required = TRUE;
   }
   else
   {
     /* As calling RAT did not call terminate CM, complete this localy here.*/
     gl1_hw_complete_cm_terminate(gas_id);
     enable_mdsp_required = FALSE;
   }

   /* Reset the GSM core - must be done after enabling the clk regimes */
   gstmr_gsm_core_reset();

   /* Flag that we have entered gsm mode */
   gl1_hw_mode_gsm[gas_id] = TRUE;

   /* Initialize sleep controller for GSM. */
   gl1_hw_sleep_gsm_init( gas_id );

   /* Init the gstmr - should be called before gl1_hw_mdsp_init */
   /* for now set gstmr 0 = gas id 1. gstmr 1 = gas_id 2*/
   gl1_hw_multi_sim_gstmr_init( gas_id );

   fws_enable_mask = mdsp_get_fws_app_mask( gas_id );

   /* Case where we enable GFW inside mdsp_enable, this sets GFW to idle state.
   ** Therefore GL1 SW needs to send set app mode cmd to move GFW to meas_idle
   */
   if(!(gl1_hw_fws_app_enabled() & fws_enable_mask))
   {
     app_mode_meas_idle_reqd = TRUE;
   }

   MSG_GERAN_HIGH_3_G("GERAN FWS APPs enabled:0x%x fws_enable_mask:0x%x app_mode_meas_idle_reqd:%d",
                      gl1_hw_fws_app_enabled(),fws_enable_mask,app_mode_meas_idle_reqd);

   if(enable_mdsp_required)
   {
     /* Enable GFW APP as we are ready to enable GSM */
     mdsp_enable(gas_id);
   }
   else
   {
     /* Set flag for awake as FW already enabled, but private data has been re-initialised */
     mdsp_awake_set(TRUE,gas_id);
   }

   /* Can't initialize mdsp here as GL1 does not have TRM lock. However,
    * init the pMdspHostIfBaseStore as this store may be read without TRM lock
    */
   mdsp_init_MdspHostIfBaseStore(gas_id);

   /* Initialiase mdsp intf data spaces */
   mdsp_init_public_data_space(gas_id);
   mdsp_init_private_data_space(gas_id);

   /* Now that init is finished, install the normal frame handler */
   gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr = tdma_isr_ptr;
   if (tdma_isr_ptr != NULL)
   {
      MSG_GERAN_MED_2_G("Add TDMA ISR Handler 0x%08x, as_id %d ", tdma_isr_ptr, as_id );
      gstmr_register_handler_geran(tdma_isr_ptr, as_id );
   }

   if( app_mode_meas_idle_reqd )
   {
     gl1_hw_async_intf_app_mode_config(GFW_APP_MODE_MEASUREMENT, as_id);
   }

   MSG_GERAN_HIGH_3_G("GL1 HW Init type %d, RxLM buffer %d, TxLM buffer %d", 0,
                                             gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx,
                                             gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx);
     /* Check that RxLM buffer not already allocated */
     gl1_hw_deallocate_rxlm_buf_idx(gas_id);

     /* Allocate RxLM buffer, get buffer index (keep trying until allocated)*/
     if (  !rflm_allocate_rx_handle(LM_GSM, LM_GSM, &gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx) )
     {

#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif 
       ERR_GERAN_FATAL_0_G("Unable to allocate RxLM buffer");
     }
     else
     {
       MSG_GERAN_MED_1_G("GSM RxLM buffer %d allocated",gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx);
     }

#ifdef FEATURE_GSM_RX_DIVERSITY
   MSG_GERAN_HIGH_1_G("FTM Mode for reading RxD Flags:%d",ftm_get_mode());

   /*If not in FTM mode check the RxD EFS item 71588 and update the RxD control flags accordingly */
   if(ftm_get_mode() != FTM_MODE)
   {
     gl1_hw_get_rxd_efs_item(gas_id);
   }
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
//     gl1_hw_rxdiv_data_init(gas_id); //revisit and check when is best to call it
     if(gl1_get_IMRD_ctl_nv(gas_id) && gl1_alloc_IMRD_rxdiv_rxlm_buff_idx(gas_id))
     {
       MSG_GERAN_HIGH_2(GL1_RXD"multi_sim_init IDLE DRX RXLM buff idx=%i allocated, imrd=%i  ",
           gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id),gl1_get_IMRD_ctl_nv(gas_id));
     }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

   /* Read any NV items used by NPL1 */
   gl1_hw_read_nv_items(task_ptr, task_wait_handler, task_wait_sig_1,gas_id);

   /* Check FW/SW feature compatibility*/
   mdsp_feature_check (gas_id);

   /* Disable any features requested - must come after reading NV */
   gl1_hw_check_disabled_features(gas_id);

   /* register NPA cb after reading the GCF NV & updating into gl1_hw_sleep_disable*/
   /*GCF global variable 'gl1_hw_sleep_disable' is updated inside the gl1_hw_check_disabled_features*/
   gl1_hw_sleep_register_npa_cb(gas_id);

#ifdef FEATURE_AEQ_PHASE4
     gl1_hw_efs_read_set_aeq4(gas_id);
#endif /*FEATURE_AEQ_PHASE4 */

#ifndef FEATURE_GSM_TO_WCDMA_MSGR
#if defined( FEATURE_INTERRAT_HANDOVER_GTOW ) || defined( FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
#ifdef FEATURE_WCDMA
   /* Set initial wcdma srch status to off */
   gl1_hw_set_wcdma_srch_on( FALSE );
#endif
#endif /* FEATURE_INTERRAT_HANDOVER_GTOW || FEATURE_INTERRAT_CELL_RESELECTION_GTOW */
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

#endif /* FEATURE_WPLT */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   wmgr_register_rat_cb( gl1_hw_map_gas_id_to_wmgr_client( gas_id ),
                         gl1_hw_sleep_wake_up_manager_cb_handler, NULL );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GSM_COEX
  /* The GAS is registered with the arbitrator for DSDA (de-sense and VBATT) and DSDS (burst tune-away) */
  gl1_hw_arbitration[gas_id].registration(FALSE, gas_id);
#endif

#if defined( FEATURE_GSM_TX_DIV_ANTNA_SWITCHING) ||defined( FEATURE_GSM_QSC_TX_DIV)
  /*Set the current configuration in MCS if feature enabled, otherwise use the Config stored in NV 70219*/
  
  if (gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE1) || gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE2))
  {
#ifdef FEATURE_GL1_ASDIV_ACQ	  
   if(gl1_asdiv_acq_enabled(gas_id))
   {
     gl1_hw_init_asdiv_preferred_antenna(gas_id, TRM_ANT_SWITCH_CONFIG_DEFAULT);
   }
   else
#endif	   
  {
	  gl1_hw_init_asdiv_preferred_antenna(gas_id, TRM_ANT_SWITCH_CONFIG_CURRENT);
  }
  }
  else
  {
	  gl1_hw_init_asdiv_preferred_antenna(gas_id, TRM_ANT_SWITCH_CONFIG_DEFAULT);
  }
 
#endif

  gl1_task_mutex_unlock(gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_multi_sim_mdsp_init

DESCRIPTION
  This function initializes the firmware for DUal sim  and QTA operation:
   - deregister tdma handler
   - Init the mdsp
   - re-register tdma. Ensures switch command buffers executes before tdma handler
 
===========================================================================*/
static void gl1_hw_multi_sim_mdsp_init(boolean use_idle_rx_alpha, gas_id_t gas_id)
{
   boolean enable_mdsp_required = FALSE;
   uint32 use_idle_rx_alpha_uint32 = (uint32)use_idle_rx_alpha;
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id( gas_id );
  
#ifdef FEATURE_G2X_TUNEAWAY
   if(!gpl1_g2x_ta_is_seq_registered_not_G(gas_id))
#endif /* FEATURE_G2X_TUNEAWAY */
   {
     if(gstmr_is_handler_registered_geran(gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr, as_id ))
  {
       gstmr_deregister_handler_geran( gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr, as_id );
     }

     enable_mdsp_required = !((gl1_hw_fws_app_enabled()) & (mdsp_get_fws_app_mask( gas_id )));

     gl1_hw_mdsp_init(enable_mdsp_required, gas_id);

     /*indicate to GFW correct RX alpha to use*/
     mdsp_update_shared_variable(GFW_RX_ALPHA,&use_idle_rx_alpha_uint32,gas_id);

     gstmr_register_handler( gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr, as_id );
     /* re-init the event scheduler */
     gl1_hw_schedule_init(gas_id);
  }
#ifdef FEATURE_G2X_TUNEAWAY
  else
  {
     /* In this case we don't want to initialise the MDSP as we may be suspending  a xfer
        and don't want to clear out all pertinent info.Howveer need to set this flag to
        stop a CCS crash
     */
     mdsp_init_G2X(gas_id);
  }
#endif /* FEATURE_G2X_TUNEAWAY */

}

/*===========================================================================

FUNCTION  gl1_hw_multi_sim_fw_rf_init

DESCRIPTION
  This function initializes the firmware and RF for DUal sim operation, this must
  only be called once the lock has been granted.

  task_wait_handler - Needed by the RF component of hardware to implement
  a polling mechanism on the calling tasks context.

  task_wait_sig_1   - Signal used to notify the calling task that the
  RF driver is ready to proceed.

  task_wait_sig_2   - Signal used for mdsp timeout download.

===========================================================================*/
void gl1_hw_multi_sim_fw_rf_init(
  boolean             debug_flag,
  rex_tcb_type       *task_ptr,
  void (*task_wait_handler)(rex_sigs_type, gas_id_t),
  rex_sigs_type task_wait_sig_1,
  rex_sigs_type       task_wait_sig_2,
  gl1_init_rflm_type  gsm_init_rflm_state,
  grm_client_enum_t   trm_client,
  boolean             wakeup,
  gas_id_t            gas_id
)
{
   ustmr_type start_ustmr, end_ustmr, duration_ustmr, duration_us;
   ARFCN_T arfcn;
   rfm_device_enum_type rf_device;
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   boolean enter_mode = gl1_hw_data_ptr[gas_id]->rf_enter_mode_required;
   lm_status_type lm_status;
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id( gas_id );
   boolean use_idle_rx_alpha = FALSE;

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
   grm_client_enum_t divrx_client =  grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);
   gl1_trm_state_t divrx_trm_status = GL1_TRM_NULL;
   if(divrx_client != GRM_NO_CLIENT)
   {
     divrx_trm_status = grm_get_trm_status(divrx_client, gas_id);
   }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

  GL1_HANDLER_PERFORMANCE_LOGGING(START_FW_RF_INIT,gas_id);

  gl1_task_mutex_lock(gas_id);

  start_ustmr = slpc_get_tick();

  rf_device = grm_get_rf_device(trm_client,  gas_id );

#ifdef FEATURE_GPRS_GBTA
  if ((rf_device == RFM_INVALID_DEVICE) &&
     (gl1_msg_get_multi_sim_standby_mode()) && (IS_GBTA_ACTIVE()))
  {
    rf_device = grm_get_other_g_rf_device(gas_id);
  }
#endif /* FEATURE_GPRS_GBTA */

  if (RFM_INVALID_DEVICE == rf_device)
  {
    /*Still Invalid, Crash*/
    MSG_GERAN_FATAL_0_G( "Invalid RF device" );
  }

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE

  if (GL1_TRM_GRANTED == divrx_trm_status)
  {
    if (TRUE == gl1_hw_trm_init_DivRx(gas_id))
    {
      MSG_GERAN_HIGH_1_G(GL1_RXD" gl1_hw_multi_sim_fw_rf_init: rxdiv rf id:%i",
                         gl1_hw_get_gsm_rf_id(divrx_client, gas_id));
    }
    else
    {
      MSG_GERAN_ERROR_0_G(GL1_RXD" gl1_hw_multi_sim_fw_rf_init: rx_div trm granted but no device");
      gl1_hw_cleanup_divrx_rfdevice_info(gas_id);
    }
  }
  else
  {
    MSG_GERAN_LOW_1_G(GL1_RXD" gl1_hw_multi_sim_fw_rf_init: not granted: %i",divrx_trm_status);
    gl1_hw_cleanup_divrx_rfdevice_info(gas_id);
  }
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */

  /* Update new allocated rf device to local store for use by RF driver msgr */
  gl1_hw_set_gsm_rf_id(rf_device, trm_client, FALSE, gas_id);

  /* Flag that we have entered gsm mode */
  gl1_hw_mode_gsm[gas_id] = TRUE;

   if ( gsm_init_rflm_state != GL1_HW_INIT_INACTIVE )
   {
   /* Check that RxLM buffer not already allocated */
      if (gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx == MDSP_RXLM_BUF_IDX_INVALID)
      {
        /* Allocate RxLM buffer */
        lm_status = rflm_allocate_rx_handle(LM_GSM, LM_GSM, &gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx);
        if (lm_status == LM_SUCCESS)
        {
         MSG_GERAN_MED_1_G("GSM RxLM buffer %d allocated", gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx);
        }
        else
        {
         MSG_GERAN_ERROR_0_G("Unable to allocate RxLM buffer");
        }
      }
    }

  GL1_HANDLER_PERFORMANCE_LOGGING(START_RF_INIT,gas_id);
  /* Initialize RF */
  gl1_hw_rf_init(gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx,rf_device, enter_mode, gas_id);

  GL1_HANDLER_PERFORMANCE_LOGGING(END_RF_INIT,gas_id);

   gl1_hw_sleep_set_rf_on(TRUE, gas_id);

  MSG_GERAN_HIGH_3_G("GL1 HW Init type %d, RxLM buffer %d, Device %d",
                     gsm_init_rflm_state,
                     gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx,
                     rf_device);

   GL1_HANDLER_PERFORMANCE_LOGGING(START_MDSP_INIT,gas_id);
   /*
        DR-DSDS: in DR mode idle mode RX alpha must be used to cope with RXFE bus contention
        issues. Ensure value is correct before calling Rf enter mode.
    */
   if (((l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE) ||
        (l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH)) &&
         gl1_drdsds_enabled(gas_id))
   {
       use_idle_rx_alpha = TRUE;
   }

   gl1_hw_multi_sim_mdsp_init(use_idle_rx_alpha, gas_id);

   GL1_HANDLER_PERFORMANCE_LOGGING(END_MDSP_INIT,gas_id);

   GL1_HANDLER_PERFORMANCE_LOGGING(START_SEND_BLK_CMD,gas_id);

   gl1_hw_async_intf_app_mode_config(GFW_APP_MODE_ACTIVE, geran_map_gas_id_to_nas_id(gas_id));

   GL1_HANDLER_PERFORMANCE_LOGGING(END_SEND_BLK_CMD,gas_id);
   /* Init the gsm vctcxo control, must be called after rf init */
   gl1_hw_tcxo_init(task_ptr, task_wait_handler, task_wait_sig_1, task_wait_sig_2,gas_id);

   /* for RFLM/FED, if enter_mode is FALSE, then an RF_WAKEUP will be called and the enter_mode_cmd will be
      sent from gl1_msgr_if task context on receiving the WAKEUP_CNF from GFW, but if enter_mode is TRUE
      then a full rf_enter_mode would have been called, so need to send an enter_mode_cmd to GFW now */
#ifndef FEATURE_GSM_RX_TX_SPLIT
   if (enter_mode)
   {
      gl1_hw_async_intf_enter_mode_cmd(GFW_APP_MODE_ACTIVE,
                    gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx,
#ifdef FEATURE_GSM_RX_TX_SPLIT
                                       (uint32)-1,
                                       gl1_hw_get_gfw_entermode_type(RFGSM_PRX_OP_MODE, gas_id),
#else
                                       gl1_hw_get_gfw_entermode_type(RFGSM_PRX_WAKEUP_MODE, gas_id),
#endif
                    rf_device,
                    geran_map_gas_id_to_nas_id(gas_id));
      MSG_GERAN_HIGH_0_G("FW RFM enter mode cmd");
   }

#ifdef FEATURE_GSM_RX_DIVERSITY
   if (GL1_GFW_PRX_DIVRX_ENTER == gl1_hw_get_gfw_enter_status(gas_id)) 
   {
     gl1_hw_set_rxdiv_wait_for_block_start(TRUE, gas_id);
   }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#endif /*FEATURE_GSM_RX_TX_SPLIT*/

   /* Now that init is finished, install the normal frame handler */
   if ( gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr != NULL )
   {
      GDRV_ISR_SAVE_LOCK(gas_id);

      gstmr_deregister_handler_geran( gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr, as_id );

      gstmr_register_handler_geran( gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr, as_id );

     GDRV_ISR_SAVE_UNLOCK(gas_id);
   }

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
   gl1_hw_iq_samples_init( gas_id );

   /* Examples of how to enable specific channels for logging:
    * gl1_hw_iq_samples_force_enable( GL1_HW_SACCH,  TRUE, GERAN_ACCESS_STRATUM_ID_1 );
    * gl1_hw_iq_samples_force_enable( GL1_HW_TCH_FS, TRUE, GERAN_ACCESS_STRATUM_ID_2 );
    * gl1_hw_iq_samples_force_enable( GL1_HW_FACCH,  TRUE, GERAN_ACCESS_STRATUM_ID_1 );
       *
       * GPRS doesn't distinguish individual channels type for data
    * gl1_hw_iq_samples_force_enable( GL1_HW_PDTCHD,  TRUE, GERAN_ACCESS_STRATUM_ID_3 );
       *
       * Also the IQ log packet number is 0x522F in QXDM
    */
#endif  /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */

   /* Configure SAIC at GSM Initialisation now. */
   /* This is needed due to the mDSP  chan type */
   /* processing;  chan type ,and CS are mixed. */
      gl1_hw_cfg_saic( TRUE,gas_id );


   /* At GSM RAT init make sure the UL TS is "1". This */
   /* also makes sure for WtG Handovers the UL is set. */
      gl1_hw_set_num_assigned_tx_slots (GL1_DEFAULT_ONE_TS,gas_id);

   /* Init cswallower value to force a cswallower update */
      gl1_hw_data_ptr[gas_id]->local_active_cswallower_value = 0;

      gl1_msg_algs_get_arfcn( &arfcn.num, &arfcn.band,gas_id );
      gl1_hw_set_active_cycle_swallower_adj( arfcn,gas_id );

   end_ustmr = slpc_get_tick();
   duration_ustmr = ( end_ustmr - start_ustmr );
   duration_us = (( duration_ustmr * 107 ) >> 11 );
   MSG_GERAN_HIGH_1_G( "gl1_hw_multi_sim_fw_rf_init END duration_us %d",
                       duration_us );

   GL1_HANDLER_PERFORMANCE_LOGGING(END_FW_RF_INIT,gas_id);

   gl1_hw_multi_sim_gonly_mode = FALSE;
   l1_tskisr_blk->firmware_loaded = TRUE;
   l1_tskisr_blk->firmware_load_status = FW_LOADED;

      gl1_task_mutex_unlock(gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_clear_acc_freq_err_valid

DESCRIPTION
  This function clears the xo_acc_freq_err_valid when the sim becomes inactive
  preventing the stored xo_acc_freq_err from becomes used.



===========================================================================*/
void gl1_hw_clear_acc_freq_err_valid( gas_id_t gas_id)
{
}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION  gl1_hw_release_or_exchange

DESCRIPTION
  Release chain held by specified TRM client, or exchange for G2X inter-RAT

===========================================================================*/
static void gl1_hw_release_or_exchange(grm_client_enum_t client_id, gas_id_t gas_id)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  /* release the currently held RF chain, or exchange it */
  grm_release_or_exchange(client_id, gas_id);
#else
  MSG_GERAN_HIGH_1_G("GDrivers call to grm_release client:%d", client_id);

  grm_release( client_id, gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  return;
}

/*===========================================================================

FUNCTION  gl1_hw_inact_task

DESCRIPTION
  This function performs any cleanup necessary from TASK when leaving GSM mode.

===========================================================================*/
void gl1_hw_inact_task( boolean   deep_sleep,
                        boolean   suspend,
                        gas_id_t  gas_id
                      )
{
   trm_grant_return_enum_type  grm_chain_grant_return;
   trm_get_info_return_type    grm_get_info_return __attribute__((unused));

   volatile ISRTIM_CMD_BLK    *l1_tskisr_blk     = NULL;
   grm_info_t                 *grm_info_p        = NULL;

   gas_id = check_gas_id(gas_id);
   l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   grm_info_p = grm_get_info_ptr_from_client(l1_tskisr_blk->client_id, gas_id);

   if ( grm_info_p )
   {
     grm_info_p->grm_freq_info.num_bands = 1;
     grm_info_p->grm_freq_info.bands[0].band =
       gl1_map_gsm_band_to_sys_band(l1_idle_data_store[gas_id].campedon_cell_ARFCN.band);
   }

   grm_chain_grant_return = grm_get_trm_grant_status(l1_tskisr_blk->client_id, gas_id);

   MSG_GERAN_HIGH_3_G( "GL1 Task Inact deep_sleep %d MSM %d trm_grant_status %d", deep_sleep, gl1_msg_get_multi_sim_mode(), grm_chain_grant_return);

   gl1_task_mutex_lock(gas_id);

   GDRV_ISR_LOCK(gas_id);

   /* Turn off tx power */
   if ( ( grm_chain_grant_return != TRM_DENIAL ) || ( ftm_get_mode() == FTM_MODE ) )
   {
       if (!gl1_hw_turn_tx_off( TRUE,gas_id ))
       {
         MSG_GERAN_ERROR_0_G("Unable to disable tx");
       }
       else
       {
        gl1_clear_drx_tx_status(gas_id);
       }

       /* Always call RF sleep API from here*/
       gl1_hw_turn_rx_off(gas_id);
     }

   gl1_hw_sleep_gsm_shutdown( gas_id );

   /* Clean up GSM vctcxo control */
   gl1_hw_tcxo_inact( deep_sleep,gas_id );

   GDRV_ISR_UNLOCK(gas_id);

   // For FED architecture, MDSP must be disabled before calling RF exit
   mdsp_disable(gas_id);

   /* Deallocate RxLM buffer */
   gl1_hw_deallocate_rxlm_buf_idx(gas_id);

#ifdef GERAN_L1_IP2CAL
   if(gl1_hw_get_ip2cal_support(gas_id))
    {
    /* Deallocate IP2Cal RxLM and TxLM buffers */
    gl1_hw_deallocate_ip2cal_txlm_buf_idx(gas_id);
    gl1_hw_deallocate_ip2cal_rxlm_buf_idx(gas_id);
    }
#endif /*GERAN_L1_IP2CAL*/

    /* For RFLM/FED architecture, each G sub should call RF_exit regardless of the multi-SIM mode or the current scenario. As
       such there is also no requirement to check whether other G subs previously called RF_exit when deactivating. The call
       can also be made without TRM GRANT, as GRF only vote off VREGs (which will have no impact of another sub is still voting
       for them) and nothing actually gets written to the RF device. Finally, RF_HAL voting is now controlled internally within
       GRF, so the GL1 RF HAL voting has been deprecated */
   /* one caveat is that for RFLM/FED, a sub cannot call exit if it has not yet called enter */
   if (!(gl1_hw_data[gas_id].rf_enter_mode_required))
   {
#ifdef FEATURE_GSM_RX_TX_SPLIT
     MSG_GERAN_HIGH_0_G( "RF exit for GERAN Stop");
     gl1_hw_rf_stop_gsm(gas_id);
     gl1_hw_data[gas_id].rf_enter_mode_required = TRUE;
#else
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
     if(RFGSM_PRX_DRX_ENTER_MODE == gl1_hw_get_rf_enter_status(gas_id))
    {
     MSG_GERAN_HIGH_0_G( "RF exit for GERAN Stop - RF exit_mode combined  DRX PRX");
       gl1_hw_rf_task_exit_mode(gas_id,
                                RFGSM_PRX_DRX_EXIT_MODE,
                                gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id));
     gl1_hw_data[gas_id].rf_enter_mode_required = TRUE;
    }
   else
#endif  /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
    {
     MSG_GERAN_HIGH_0_G( "RF exit for GERAN Stop - PRX");
       gl1_hw_rf_task_exit_mode(gas_id,
                                RFGSM_PRX_EXIT_MODE,
                                gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id));
     gl1_hw_data[gas_id].rf_enter_mode_required = TRUE;
    }
#endif
   }

   gl1_hw_set_if_start_gsm_qta_sent(FALSE, gas_id);

   /* Check for possible time overruns before deregistering VSTMR */
   vstmr_snapshot_before_vstmr_deregister (gas_id);
   vstmr_geran_vstmr_deregister (gas_id);

   /* Do not immediate log the power monitors if we are suspending.
    * This can result in wrong arfcn and rssi values getting logged.
	*/
   if( FALSE == suspend )
   {
     /* generate log before leaving GERAN */
     gl1_hw_log_monitor(0,0,GAIN_RANGE_1,gl1_get_serving_cell_arfcn(gas_id),FALSE,TRUE, gas_id);

   }
     
   /*For deep sleep, check if GERAN stop needs to issued to MCPM*/
   if(deep_sleep)
   {
    gl1_hw_inact_deep_sleep(FALSE, gas_id);
   }
   else if (L1_ftm_enabled(gas_id))
   {
     
     MSG_GERAN_HIGH_0_G( "MCPM change for Non-signaling FTM mode STOP");
     gl1_hw_mcpm_state_update(MCPM_GERAN_STOP_REQ, GL1_MCPM_GERAN_STOP,gas_id);
   }

#ifdef FEATURE_GSM_COEX
   /* The GAS is registered with the arbitrator for DSDA (de-sense and VBATT) and DSDS (burst tune-away) */
   gl1_hw_arbitration[gas_id].deregistration(FALSE, gas_id);
#endif

   /* release TRM lock */
   grm_set_trm_status(GL1_TRM_NULL, l1_tskisr_blk->client_id, gas_id);

   /* release the currently held RF chain, or exchange it */
   gl1_hw_release_or_exchange(l1_tskisr_blk->client_id, gas_id);

   /* Undefine L1 timeout timer */
   rex_undef_timer(&gsm_l1_timeout_timer[gas_id]);

 #if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* Set FW loaded to FALSE, trm is released and mdsp is disabled */
   l1_tskisr_blk->firmware_loaded = FALSE;
 #endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /*Try to enable DR on inacting GL1 task. If GL1 hasn't disabled DR,
   then TRM will ignore the request*/
   gl1_drdsds_hybrid_tuneaway_trigger(DRDSDS_STOP_GSM_EVENT,
                                      NULL,
                                      l1_tskisr_blk->client_id,
                                      gas_id);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/


  /* Set local copy of RF device to Invalid on RF Exit */
   gl1_hw_reset_gsm_rf_ids(l1_tskisr_blk->client_id, gas_id);
   
   gl1_hw_set_gsm_only_mode( FALSE, gas_id );

   gl1_task_mutex_unlock(gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_panic_inact_task

DESCRIPTION
  This function performs any cleanup necessary from TASK when leaving GSM mode
  due to Panic Reset.

===========================================================================*/

static void gl1_hw_panic_inact_task (gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK	  *l1_tskisr_blk	 = &l1_tsk_buffer[gas_id];

  gas_id = check_gas_id(gas_id);

  gl1_task_mutex_lock(gas_id);

  GDRV_ISR_LOCK(gas_id);

  (void)gl1_hw_turn_tx_off(TRUE, gas_id);

  gl1_clear_drx_tx_status(gas_id);

  /* Always call RF sleep API from here*/
  gl1_hw_turn_rx_off(gas_id);

  /* Clean up GSM vctcxo control - TRUE for deep_sleep*/
  gl1_hw_tcxo_inact(TRUE, gas_id);

  GDRV_ISR_UNLOCK(gas_id);

  // For FED architecture, MDSP must be disabled before calling RF exit
  mdsp_disable(gas_id);

  /* Deallocate RxLM buffer */
  gl1_hw_deallocate_rxlm_buf_idx(gas_id);

#ifdef GERAN_L1_IP2CAL
  if (gl1_hw_get_ip2cal_support(gas_id))
  {
    /* Deallocate IP2Cal RxLM and TxLM buffers */
    gl1_hw_deallocate_ip2cal_txlm_buf_idx(gas_id);
    gl1_hw_deallocate_ip2cal_rxlm_buf_idx(gas_id);
  }
#endif /*GERAN_L1_IP2CAL*/

  /* For RFLM/FED architecture, each G sub should call RF_exit regardless of the multi-SIM mode or the current scenario. As
     such there is also no requirement to check whether other G subs previously called RF_exit when deactivating. The call
     can also be made without TRM GRANT, as GRF only vote off VREGs (which will have no impact of another sub is still voting
     for them) and nothing actually gets written to the RF device. Finally, RF_HAL voting is now controlled internally within
     GRF, so the GL1 RF HAL voting has been deprecated */
  /* one caveat is that for RFLM/FED, a sub cannot call exit if it has not yet called enter */
  if (!(gl1_hw_data[gas_id].rf_enter_mode_required))
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
    MSG_GERAN_HIGH_0_G( "RF exit for GERAN Stop");
    gl1_hw_rf_stop_gsm(gas_id);
    gl1_hw_data[gas_id].rf_enter_mode_required = TRUE;
#else
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
    if(RFGSM_PRX_DRX_ENTER_MODE == gl1_hw_get_rf_enter_status(gas_id))
    {
      MSG_GERAN_HIGH_0_G("RF exit for GERAN Stop - RF exit_mode combined  DRX PRX");
      gl1_hw_rf_task_exit_mode(gas_id,
                               RFGSM_PRX_DRX_EXIT_MODE,
                               gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id));
    }
    else
#endif	/*FEATURE_GSM_RX_DIVERSITY_IDLE*/
    {
      MSG_GERAN_HIGH_0_G("RF exit for GERAN Stop - PRX");
      gl1_hw_rf_task_exit_mode(gas_id,
                               RFGSM_PRX_EXIT_MODE,
                               gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id));
    }
#endif
  }
  else
  {
    /* ensure to reset enter_mode_req, as we are exiting the sub */
    gl1_hw_data[gas_id].rf_enter_mode_required = FALSE;
  }

  gl1_hw_set_if_start_gsm_qta_sent(FALSE, gas_id);

   /* Check for possible time overruns before deregistering VSTMR */
   vstmr_snapshot_before_vstmr_deregister (gas_id);
   vstmr_geran_vstmr_deregister (gas_id);


  MSG_GERAN_HIGH_0_G("MCPM change for GERAN Stop");
  gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_STOP_REQ, GL1_MCPM_GERAN_STOP, gas_id);
  gstmr_force_reinit(gas_id);


#ifdef FEATURE_GSM_COEX
  /* The GAS is registered with the arbitrator for DSDA (de-sense and VBATT) and DSDS (burst tune-away) */
  gl1_hw_arbitration[gas_id].deregistration(FALSE, gas_id);
#endif

  /* release TRM lock */
  grm_set_trm_status(GL1_TRM_NULL, l1_tskisr_blk->client_id, gas_id);

  /* release the currently held RF chain, or exchange it */
  gl1_hw_release_or_exchange(l1_tskisr_blk->client_id, gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* Set FW loaded to FALSE, trm is released and mdsp is disabled */
  l1_tskisr_blk->firmware_loaded = FALSE;
#endif

  gl1_task_mutex_unlock(gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_inact_isr

DESCRIPTION
  This function performs any cleanup necessary from ISR when leaving GSM mode.
  This should be called until it returns TRUE.
  Set panic_inact to TRUE if this is an inactivation due to a panic.  This
  will force the inactivation and will always return TRUE.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if going to deactivate, FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_inact_isr (boolean panic_inact,gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id( gas_id );

   /* Inactivate the GSAC module */
   if (!gl1_hw_gsac_inact(panic_inact,gas_id)) return FALSE;

#ifdef FEATURE_GSM_TDS
   /* If deactivating then want to abort any startup/cleanup
    * cmds that are being decremented
    */
   if (mdsp_decrement_startup_time_offset_flag[gas_id])
   {
     /* startup hasn't been issued so safe to set both flags to FALSE*/
     mdsp_decrement_startup_time_offset_flag[gas_id] = FALSE;
     mdsp_decrement_cleanup_time_offset_flag[gas_id] = FALSE;
     MSG_GERAN_HIGH_0_G("TDS: reset decrementing startup and cleanup flags");
   }
   else if ((mdsp_decrement_cleanup_time_offset_flag[gas_id]) && (gl1_cleanup_decrement_time_offset[gas_id]))
   {
     gl1_hw_g2t_cleanup(0,gas_id);
     MSG_GERAN_HIGH_0_G("TDS: issued pending cleanup cmd");
   }
#endif
#ifdef  FEATURE_GSM_TO_LTE
     l1_g2l_trigger_mdsp( TRUE, gas_id );
#endif /* FEATURE_GSM_TO_LTE */
   /* Inactivate the mDSP driver - once this returns TRUE we are committed
      to the inactivation */
   if (!gl1_hw_mdsp_inact(panic_inact,gas_id)) return FALSE;

   /* Tick IRAT state machine and check that it is inactive */
   l1_sci_tick_wcdma(gas_id);
   if (l1_sc_irat_state_machine_active(gas_id)) return FALSE;
   if(l1_sc_wcdma_data_ptr->issue_g2x_deinit)
   {
     l1_sc_irat_check_active(0, 0, FALSE, 0, FALSE, gas_id);
     l1_sc_wcdma_data_ptr->issue_g2x_deinit = FALSE;
   }
  /* return if the G2X acivity is on or pending*/
   if (l1_sc_check_irat_active(gas_id)) return FALSE;
   /* Uninstall the main TDMA ISR handler */
   if (gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr != NULL)
   {
      MSG_GERAN_HIGH_2_G("Remove TDMA ISR Handler 0x%08x, as_id %d ", gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr, as_id );
      gstmr_deregister_handler_geran(gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr, as_id );
      gl1_hw_data_ptr[gas_id]->current_tdma_isr_ptr = NULL;
   }

   /* Flag that we have left gsm mode */
   gl1_hw_mode_gsm[gas_id] = FALSE;

   gl1_hw_log_cell_cmds_to_diag (cmd_gsm_inact, TRUE,gas_id);

   return (TRUE);
}

/*===========================================================================

FUNCTION  gl1_hw_inact

DESCRIPTION
  This function performs any cleanup necessary when leaving GSM mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_inact (boolean deep_sleep,gas_id_t gas_id)
{
   while (!gl1_hw_inact_isr(FALSE,gas_id))
   {
      if (rex_is_in_irq_mode())
      {
#ifdef FEATURE_QSH_MDUMP
         QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
         ERR_GERAN_FATAL_0_G("Can't Wait a Frame Tick when inside ISR!");
      }
      mdsp_wait_for_frame_tick(gas_id);
   }

   gl1_hw_inact_task(deep_sleep, FALSE, gas_id);
}

   /*===========================================================================

   FUNCTION  gl1_hw_inact_deep_sleep

   DESCRIPTION
     This function performs any cleanup necessary for deep sleep when GSM is already inactive
     either for a RAT change and the RAT change has been signalled as successful or deep sleep.

   DEPENDENCIES
     None

   RETURN VALUE
     None

   SIDE EFFECTS
     None

   ===========================================================================*/
void gl1_hw_inact_deep_sleep (boolean rat_change_ind, gas_id_t gas_id)
{

    MSG_GERAN_HIGH_1_G("MCPM change for GERAN Stop (rat change %d)", rat_change_ind);

   /*Send request to MCPM for GERAN STOP only if WCDMA, TD and LTE are inactive*/

   if((l1_sc_get_wcdma_irat_mcpm_active(gas_id))
#if defined (FEATURE_GSM_TDS)
   || (l1_sc_get_tds_irat_mcpm_active(gas_id))
#endif
   
#if defined (FEATURE_GSM_TO_LTE)
   || (l1_sc_get_lte_irat_mcpm_active(gas_id))
#endif
    )
    {
      gl1_deep_sleep_pending[gas_id] = TRUE;
    }
    else
    {
    gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_STOP_REQ, GL1_MCPM_GERAN_STOP, gas_id);

      /*Force GSTMR re-initialization only when deep sleep is not for rat change*/
      if(rat_change_ind == FALSE)
      {
        gstmr_force_reinit(gas_id);
      }
}
}

/*============================================================================
FUNCTION  gl1_hw_panic_inact

DESCRIPTION
  This function is used to shut down all devices that NPL1 depends on. This is
  a force shut down due to a panic event, either an upper layer exception or a
  mdsp halt. Similar to gl1_hw_inact, but we don't wait for mdsp tick since
  ISR may be dead.
  Should be called in task context.

===========================================================================*/
void gl1_hw_panic_inact(gas_id_t gas_id)
{
   volatile gl1_mcpm_state_type mcpm_gsm_stop;

   /* Let the sleep controller handler the panic to make sure we are awake */
   gl1_hw_sleep_gsm_panic_inact(gas_id);

   GDRV_ISR_LOCK(gas_id);
   (void) gl1_hw_inact_isr(TRUE,gas_id);
   GDRV_ISR_UNLOCK(gas_id);

   /* Now inact the task-level code */
   gl1_hw_panic_inact_task(gas_id);

   /* Wait for MCPM Stop to complete (started in gl1_hw_inact_task), then reset edge core */
   mcpm_gsm_stop = gl1_hw_mcpm_get_state(gas_id);
   while (mcpm_gsm_stop != GL1_MCPM_GERAN_STOP)
   {
      gl1_hw_delay(GL1_HW_MCPM_START_STOP_DELAY_LOOP,gas_id);
      mcpm_gsm_stop = gl1_hw_mcpm_get_state(gas_id);
   }

   /* Force a gstmr reset on the next startup */
   gstmr_force_reinit(gas_id);
   gl1_hw_data_ptr[gas_id]->gl1_panic_reset_occurred = TRUE;
}

/*===========================================================================

FUNCTION gl1_hw_grm_init

DESCRIPTION
  Request the RF resource for all the bands

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static rfm_device_enum_type gl1_hw_grm_init( gas_id_t gas_id )
{
  /* Allocated device vs band mapping info */
  static gl1_hw_grm_devices_type gl1_hw_grm_devices[MAX_BAND_INFO_GRM] =
                                   { { SYS_BAND_CLASS_NONE, RFM_INVALID_DEVICE } };

   rfm_device_enum_type rfm_current_device_assigned = RFM_INVALID_DEVICE;
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

#ifdef FEATURE_GSM_RX_TX_SPLIT
   grm_client_enum_t tx_client_id = grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id);
   /* WA to reset any residual TRM status for tx client */ 
   if (tx_client_id != GRM_NO_CLIENT)
   {
     grm_set_trm_status(GL1_TRM_NULL, tx_client_id, gas_id);
   }
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

  /* If a specific device was selected for ftm mode then override TRM
     device selection */
#ifdef FEATURE_GSM_RX_TX_SPLIT
  if ( gl1_get_prx_ftm_dev(gas_id) != RFM_INVALID_DEVICE )
  {
    /* In FTM mode*/
    rfm_current_device_assigned = gl1_get_prx_ftm_dev(gas_id);
#else
  if ( ftm_device[gas_id] != RFM_INVALID_DEVICE )
  {
    /* In FTM mode*/
    rfm_current_device_assigned = ftm_device[gas_id];
#endif
#ifdef FEATURE_GSM_RX_DIVERSITY
    if(gl1_get_rxd_ctl_nv(gas_id))
    {
      gl1_hw_trm_init_DivRx(gas_id);
      grm_set_trm_status(GL1_TRM_GRANTED,
                         grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id),
                         gas_id );
    }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
    if ( (rfm_current_device_assigned != RFM_INVALID_DEVICE) 
         &&
        (rfm_current_device_assigned != RFM_MAX_DEVICES))
     {
      /* update trm status as well */
      grm_set_trm_status( GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id );
#ifdef FEATURE_GSM_RX_TX_SPLIT
      if (gl1_get_tx_ftm_dev(gas_id) != RFM_INVALID_DEVICE)
      {
         gl1_hw_set_gsm_rf_id(gl1_get_tx_ftm_dev(gas_id), 
                              grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id),
                              FALSE, 
                              gas_id);
         grm_set_trm_status( GL1_TRM_GRANTED,
                             tx_client_id,
                             gas_id );
         MSG_GERAN_HIGH_1_G( "FTM assigned Tx device:%d", gl1_get_tx_ftm_dev(gas_id) );
      }
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
     }
     }
    else
     {
    trm_grant_return_enum_type grm_chain_grant_return;
    int                        i              = 0;
    grm_client_enum_t          g_client_id    =
                                 l1_tskisr_blk->client_id;
    grm_info_t                *grm_info_p     =
                                 grm_get_info_ptr_from_client(g_client_id, gas_id);
    trm_duration_t             duration       = 33; /*1ms in sys clocks*/

    /* Primary request */

    if ( grm_info_p )
    {
      for ( i = 0;  i < grm_valid_gsm_sys_bands_cnt[gas_id]; i++ )
      {
        gl1_hw_grm_devices[i].band              = grm_valid_gsm_sys_bands[gas_id][i];
        grm_info_p->grm_freq_info.bands[i].band = gl1_hw_grm_devices[i].band;
      }

      grm_info_p->grm_freq_info.num_bands = grm_valid_gsm_sys_bands_cnt[gas_id];
    }

    grm_chain_grant_return =
      grm_request( g_client_id, duration, TRM_TRAFFIC, SYS_PROC_TYPE_NONE, TRM_RX_ANY, GRM_DIVRX_TRM_REQ_INIT );

    /*update local trm_grant_status*/
    grm_set_trm_grant_status(grm_chain_grant_return, g_client_id, gas_id);

    if ( grm_info_p  && (TRM_GRANTED == grm_chain_grant_return))
    {
      rfm_current_device_assigned =
        grm_info_p->grm_request_data.band_alloc[0].dev_allocated;

      for ( i = 0; i < grm_info_p->grm_freq_info.num_bands; i++ )
          {
        gl1_hw_grm_devices[i].device =
          grm_info_p->grm_request_data.band_alloc[i].dev_allocated;

        if ( ( gl1_hw_grm_devices[i].device == RFM_INVALID_DEVICE ) ||
             ( gl1_hw_grm_devices[i].device == RFM_MAX_DEVICES ) )
             {
          MSG_GERAN_ERROR_1_G( "RF chain is not allocated by GRM for %d band",
                               grm_info_p->grm_request_data.band_alloc[i].band.band );

          gl1_hw_grm_devices[i].device = RFM_INVALID_DEVICE;
        }

        if ( rfm_current_device_assigned !=
               grm_info_p->grm_request_data.band_alloc[i].dev_allocated )
               {
          MSG_GERAN_ERROR_0_G( "Different RF devices are allocated for diff bands" );
        }
      }
    }

    if ( grm_chain_grant_return == TRM_GRANTED )
                 {
       grm_retain_lock( g_client_id,
                        (grm_unlock_callback_t)&grm_ignore_unlock_callback,
                        gas_id );
                 }
    else
    {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_FATAL("TRM denied to client in Single SIM mode",0,0,0);
    }

   if ( grm_chain_grant_return == TRM_GRANTED )
                 {
      /* update trm status as well */
      grm_set_trm_status( GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id );
                  }

    /* End of Primary request */
                }

  gl1_hw_set_gsm_rf_id(rfm_current_device_assigned,
                       l1_tskisr_blk->client_id,
                       FALSE,
                       gas_id);

  MSG_GERAN_HIGH_1_G( "GRM assigned RF device:%d",
                       rfm_current_device_assigned );
#ifdef FEATURE_GSM_RX_DIVERSITY
  MSG_GERAN_HIGH_1_G (GL1_RXD"RF device for DRX:%d",
                      gl1_hw_get_gsm_rf_id(
                         grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id),
                         gas_id));
#endif
   return ( rfm_current_device_assigned );
              }

/*===========================================================================

FUNCTION gl1_hw_get_gsm_rf_id

DESCRIPTION
  Returns the RF resource ID for GSM (currently all bands the same ID)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
rfm_device_enum_type gl1_hw_get_gsm_rf_id(grm_client_enum_t grm_client_id,
                                          gas_id_t          gas_id)
{
  rfm_device_enum_type rf_device = RFM_INVALID_DEVICE;
  if (TRUE == grm_is_a_secondary_client(grm_client_id))
  {
#ifdef FEATURE_GSM_RX_DIVERSITY
    rf_device = gl1_hw_get_gsm_rxdiv_rf_id(gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/
  }
#ifdef FEATURE_GSM_RX_TX_SPLIT
  else if (TRUE == grm_is_a_tx_client(grm_client_id)) 
  {
    rf_device = gl1_hw_data_ptr[gas_id]->gl1_hw_trm_rf_tx_device; 
  }
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  else
{
    rf_device = gl1_hw_data_ptr[gas_id]->gl1_hw_trm_rf_rx_device;
  }
  return rf_device;
}

/*===========================================================================

FUNCTION gl1_hw_set_gsm_rf_id

DESCRIPTION
  Set the RF resource ID for GSM (currently all bands the same ID)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_set_gsm_rf_id(rfm_device_enum_type rfm_device, 
                          grm_client_enum_t     grm_client_id,
                          boolean force_set_rfm_device,
                          gas_id_t gas_id)
{
  if((rfm_device == RFM_INVALID_DEVICE) && (FALSE == force_set_rfm_device))
   {
    MSG_GERAN_ERROR_0_G("Attempting to overwrite gl1_hw_trm_rf_device with RFM_INVALID_DEVICE");
     return;
   }

  if (TRUE == grm_is_a_secondary_client(grm_client_id))
  {
#ifdef FEATURE_GSM_RX_DIVERSITY
    gl1_hw_set_gsm_rxdiv_rf_id(rfm_device, gas_id);
#else
    MSG_GERAN_ERROR_2_G(GL1_RXD"RF device %d set for Secondary client %d",
                        rfm_device,
                        grm_client_id);

#endif /*FEATURE_GSM_RX_DIVERSITY*/
  }
#ifdef FEATURE_GSM_RX_TX_SPLIT
  else if (TRUE == grm_is_a_tx_client(grm_client_id)) 
  {
    gl1_hw_data_ptr[gas_id]->gl1_hw_trm_rf_tx_device = rfm_device; 
  }
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  else
  {
    gl1_hw_data_ptr[gas_id]->gl1_hw_trm_rf_rx_device = rfm_device; 
  }
  return;
}

/*===========================================================================

FUNCTION gl1_hw_get_wcdma_rf_id

DESCRIPTION
  Returns the RF resource ID for WCDMA Neighbour

DEPENDENCIES
  None

RETURN VALUE
  RF device Id.

SIDE EFFECTS
  None
===========================================================================*/
rfm_device_enum_type gl1_hw_get_wcdma_rf_id(uint16 W_band, gas_id_t gas_id)
{
  rfm_device_enum_type  device = RFM_INVALID_DEVICE;
  rfi_band_type         band = RFI_NO_BAND_SEL;

  /*this is a union*/
  trm_get_info_data     grm_get_device_input;
  trm_get_info_data    *grm_get_device_input_p;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  trm_set_irat_band_device_info grm_device;
  volatile ISRTIM_CMD_BLK    *l1_tskisr_blk     = &l1_tsk_buffer[gas_id];
#else
  grm_client_enum_t requested_client_id = WCDMA_CLIENT_FOR_THIS_SUB(gas_id);
#endif /* defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */

   grm_get_device_input_p = &grm_get_device_input;
#ifdef FEATURE_WCDMA
   band = rfm_wcdma_convert_band_rfcom_to_rfi( W_band );
#endif
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* In all configuration, GL1 does band registration with TRM and
  TRM allocates RF device.*/
       /*get the device information from grm_info*/
       grm_device = grm_get_irat_rf_dev_from_grm_info((sys_band_class_e_type)band,
                                                      l1_tskisr_blk->client_id,
                                                      gas_id);

  MSG_GERAN_HIGH_1_G("gl1_hw_get_wcdma_rf_id: Primary device returned for G2W Irat is %d",
                     grm_device.primary_device);
       device = grm_device.primary_device;
#else

         /*This function to be used only W*/
  grm_get_device_input_p->band_mapping_info.resource      = TRM_RX_BEST;
   grm_get_device_input_p->band_mapping_info.num_bands     = 1;
   grm_get_device_input_p->band_mapping_info.band[0]       = (sys_band_class_e_type)band;

         device =  grm_get_device_mapping( requested_client_id, grm_get_device_input_p, gas_id );

   if ( device > grm_get_max_valid_rf_device_for_client( requested_client_id ) )
    {
      MSG_GERAN_ERROR_2_G( "GRM returned Invalid RF device for W:%d Max valid:%d",
                          device,
                          grm_get_max_valid_rf_device_for_client( requested_client_id ) );
      device = RFM_INVALID_DEVICE;
          }
#endif /* defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */

     return (device);
     }

/*===========================================================================

FUNCTION gl1_hw_get_lte_rf_id

DESCRIPTION
  Returns the RF resource ID for LTE Neighbour

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
rfm_device_enum_type gl1_hw_get_lte_rf_id(grm_client_enum_t lte_client, sys_sband_lte_e_type lte_band, gas_id_t gas_id)
{
  trm_get_info_data     grm_get_device_input;
  trm_get_info_data    *grm_get_device_input_p;
  rfm_device_enum_type  device = RFM_INVALID_DEVICE;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  trm_set_irat_band_device_info grm_device;
  sys_band_class_e_type bandin;
  volatile ISRTIM_CMD_BLK    *l1_tskisr_blk     = &l1_tsk_buffer[gas_id];
#else
  boolean valid_client = TRUE;
#endif /* defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */

  grm_get_device_input_p = &grm_get_device_input;


#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* In all configuration, GL1 does band registration with TRM and
  TRM allocates RF device.*/
  bandin = lte_ml1_convert_lte_band_to_sys_band( lte_band );
  grm_device = grm_get_irat_rf_dev_from_grm_info( bandin,
                                                  l1_tskisr_blk->client_id,
                                                  gas_id);

  switch (lte_client)
  {
    case GRM_LTE:
    case GRM_LTE_SUB2:
    {
      device = grm_device.primary_device;
      break;
    }

    case GRM_LTE_SECONDARY:
    case GRM_LTE_SUB2_SECONDARY:
    {
      device = grm_device.diversity_device;
      break;
    }

    default:
    {
      MSG_GERAN_ERROR_1_G("gl1_hw_get_lte_rf_id: DBG: ERROR: Unknown Client : %d",lte_client);
      break;
    }
  }
#else
  /* Set device using TRM API */
  switch (lte_client)
  {
    case GRM_LTE:
  {
    MSG_GERAN_HIGH_0_G("LTE PRIMARY device");
      grm_get_device_input_p->band_mapping_info.resource      = TRM_RX_BEST;
      break;
  }

    case GRM_LTE_SECONDARY:
  {
   MSG_GERAN_HIGH_0_G("LTE SECONDARY device");
     grm_get_device_input_p->band_mapping_info.resource      = TRM_RX_DIVERSITY;
      break;
  }

    default:
  {
    MSG_GERAN_ERROR_1_G("ERROR: INVALID Client: %d is neither LTE or LTE_SECONDARY",lte_client);
      valid_client = FALSE;
      break;
    }
  }

  if (valid_client)
  {
  grm_get_device_input_p->band_mapping_info.num_bands     = 1;
  grm_get_device_input_p->band_mapping_info.band[0]       =
    lte_ml1_convert_lte_band_to_sys_band( lte_band );

    device = grm_get_device_mapping( lte_client,
                                     grm_get_device_input_p,
                                     gas_id );

    if ( device > grm_get_max_valid_rf_device_for_client( lte_client ) )
   {
     MSG_GERAN_ERROR_2_G( "GRM returned Invalid RF device for L:%d Max valid:%d",
                            device,
                          grm_get_max_valid_rf_device_for_client( lte_client ) );
       device = RFM_INVALID_DEVICE;
    }
   }
#endif /*FEATURE DUAL SIM || FEATURE TRIPLE SIM*/
   return (device);
}
/*===========================================================================

FUNCTION gl1_hw_rf_init

DESCRIPTION
  Enable and initializes RF hardware.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_hw_rf_init (uint32 rxlm_buf_idx,
                            rfm_device_enum_type  device,
                            boolean enter_mode,
                            gas_id_t gas_id
                            )

{
 mdsp_xo_desense_arfcn_struct xo_desense_chanels ;
 volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   gl1_hw_sleep_type  *gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];

#ifdef RFGSMCTL_ENA_GSM_GRFC
   #error code not present
#endif

   /* only if enter mode is being done */
   if ( enter_mode )
   {
     /*clear the list of chanel required dc spur removal(xo desense)*/
     xo_desense_chanels.G850_chan_list[0]= LAST_ELEMNT_IN_CHNL_LIST;
     xo_desense_chanels.G900_chan_list[0]= LAST_ELEMNT_IN_CHNL_LIST;
     xo_desense_chanels.G1900_chan_list[0]= LAST_ELEMNT_IN_CHNL_LIST;
     xo_desense_chanels.G1800_chan_list[0]= LAST_ELEMNT_IN_CHNL_LIST;
     mdsp_xo_desense_arfcn_gas_id(&xo_desense_chanels ,gas_id);
   }
   mdsp_set_iq_log_gas_id (FALSE ,gas_id);

   /* Enter GSM mode */
   if (rxlm_buf_idx != MDSP_RXLM_BUF_IDX_INVALID)
   {
       /* Save time to vstmr if gl1_hw_rf_init() was called as part of wakeup
          from sleep. If rf_wakeup_to_vstmr_us is non-zero then we have already
          completed wakeup and must not overwrite the value previously saved */
       if (gl1_hw_sleep_cycle->sleep_info.rf_wakeup_to_vstmr_us == 0)
       {
         gl1_hw_sleep_cycle->sleep_info.rf_wakeup_to_vstmr_us = gl1_get_time_next_vsmtr_us(gas_id);
         MSG_GERAN_HIGH_1_G(GL1_SLEEP_HDR" gl1_hw_rf_init time rf wakeup to gstmr %dus",
                            gl1_hw_sleep_cycle->sleep_info.rf_wakeup_to_vstmr_us);
       }
         /* ensure first call to RF Init results in calling RF_Enter_Mode  - all subsequent calls should be for RF_Wakeup
            regardless of whether this is for a real wakeup or a TRM grant event.
            For G2W TA, on G2W deact, RF_Sleep is called, so for re-start after G2W we can also just call RF_Wakeup */
      if ( enter_mode )
         {
            gl1_hw_sleep_cycle->wakeup.rf_awake.start_ustmr = slpc_get_tick();
            gl1_hw_sleep_cycle->sleep_info.parallel_warmup = FALSE;
            gl1_hw_data_ptr[gas_id]->rf_enter_mode_required = FALSE;
#ifdef FEATURE_GSM_RX_TX_SPLIT
         gl1_hw_rf_start_gsm(gas_id);
         gl1_hw_rf_wakeup(rxlm_buf_idx,GL1_PRX_WAKEUP_MODE, gas_id);
         l1_tskisr_blk->rf_wakeup_pending = TRUE;
         MSG_GERAN_HIGH_1_G( "RF GSM START and Rx Wakeup requested for sub=%d",gas_id);             
#else
            gl1_hw_rf_task_enter_mode(rxlm_buf_idx,device,gas_id);
            MSG_GERAN_HIGH_1_G(GL1_SLEEP_HDR" RF INIT: Enter Mode requested for sub=%d",gas_id);
#endif

            gl1_hw_sleep_debug_duration ( &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]].wakeup.rf_awake);
            MSG_GERAN_HIGH_1_G(GL1_SLEEP_HDR" gl1_hw_rf_task_enter_mode duration %dus",
                               gl1_hw_sleep_cycle->wakeup.rf_awake.duration_us);

         }
         else
         {
            gl1_hw_sleep_cycle->wakeup.rf_awake.start_ustmr = slpc_get_tick();
            gl1_hw_sleep_cycle->sleep_info.parallel_warmup = TRUE;

            l1_tskisr_blk->rf_wakeup_pending = TRUE;
         gl1_hw_rf_wakeup(rxlm_buf_idx,GL1_PRX_WAKEUP_MODE, gas_id);
         }

      gl1_hw_sleep_set_rf_on(TRUE, gas_id);
   }
}

/*===========================================================================

FUNCTION gl1_hw_mdsp_init

DESCRIPTION
  Enable and initializes mdsp.

===========================================================================*/
static void gl1_hw_mdsp_init ( boolean enable_mdsp, gas_id_t gas_id )
{
  uint32 vamos_support = 0;

  if (enable_mdsp)
  {
  /* Enable GFW APP as we are ready to enable GSM */
   mdsp_enable(gas_id);
  }
  else
  {
   /* Set flag for awake as FW already enabled, but private data has been re-initialised */
   mdsp_awake_set(TRUE,gas_id);
  }

   /* Initialize the MDSP before the RF and after the gstmr */
   mdsp_init(gas_id);

   /* Set search widths */
   mdsp_set_async_rx_search_width(GL1_HW_SCH_SEARCH_WIDTH_DEFAULT ,gas_id);
   mdsp_set_sync_rx_search_width(GL1_HW_DEFAULT_SEARCH_WIDTH ,gas_id);

#ifdef FEATURE_GSM_GPRS_MSC33
   /* Enable support of multislot class 33 in MDSP */
   mdsp_intf_shared_var_write(GFWSHAREDVARIABLES_EDGE_MSC,(uint16)GFW_MS_CLASS_33 ,gas_id);
   mdsp_updated_shared_variable(GFW_EDGE_SHARED_VARIABLE,gas_id);
#endif /* FEATURE_GSM_GPRS_MSC33 */

#ifdef FEATURE_GSM_AMR
   /* During AMR full and half rate calls, MDSP will adjust the estimated C/I using
    * the adjustment factors if the estimated C/I is above the given threshold
    */
   mdsp_intf_shared_var_write(GFWSHAREDVARIABLES_EDGE_COVERITHRESH,(uint16)GSM_AMR_C_OVER_I_THRESHOLD ,gas_id);
   mdsp_updated_shared_variable(GFW_EDGE_SHARED_VARIABLE,gas_id);
#endif

   vamos_support = gl1_vamos_support[gas_id];
   mdsp_update_shared_variable(GFW_VAMOS_SUPPORT,&vamos_support,gas_id);
   mdsp_update_shared_variable(GFW_DEBUG,&gl1_efs_mdsp_debug,gas_id);

   /* Register NPL1 panic callback handler */
   if ( ftm_get_mode() == FTM_MODE )
   {
      mdsp_gsm_register_panic_cb(NULL ,gas_id);
   }
   else
   {
      mdsp_gsm_register_panic_cb(gl1_mdsp_panic_reset_callback ,gas_id);
   }
}

/*===========================================================================

FUNCTION gl1_hw_mdsp_inact

DESCRIPTION
  Inactivate the mdsp.

===========================================================================*/
static boolean gl1_hw_mdsp_inact (boolean panic_inact,gas_id_t gas_id)
{

#ifdef TEST_FRAMEWORK
   #error code not present
#endif
   /* Don't need to check if mdsp is ready to sleep or not if we are forcing
    * a inact due to a panic. */
   if (!panic_inact)
   {
      /* Check if the mDSP thinks it is busy. */
      if (!mdsp_ready_for_sleep(gas_id) &&
          gl1_hw_data_ptr[gas_id]->count_wait_for_mdsp_sleep <= MAX_WAIT_FRAMES_FOR_MDSP_SLEEP)
      {
         GfwSleepStateStruct sleepState = *(GfwSleepStateStruct *)mdsp_intf_shared_var_read(GFWREADVARIABLESSTRUCT_GFWSLEEPSTATE ,gas_id);
         gl1_hw_data_ptr[gas_id]->count_wait_for_mdsp_sleep++;

         /* If it is not the last try, send as MSG_HIGH */
         if (gl1_hw_data_ptr[gas_id]->count_wait_for_mdsp_sleep <= MAX_WAIT_FRAMES_FOR_MDSP_SLEEP)
         {
            MSG_GERAN_HIGH_1_G("Inactivating GSM while mDSP is busy (gsmAwakeState=0x%04x).",
                     sleepState.gfwSleepState);
         if (mdsp_sleep_error_pending(gas_id)) mdsp_display_sleep_error(FALSE ,gas_id);
         }
         else
         {
            MSG_GERAN_ERROR_1_G("Inactivating GSM while mDSP is busy (gsmAwakeState=0x%04x).",
                      sleepState.gfwSleepState);
            if (mdsp_sleep_error_pending(gas_id)) mdsp_display_sleep_error(TRUE ,gas_id);
         }
         return FALSE;
      }

      /* Reset count_wait_for_mdsp_sleep, in case we have decided to go ahead
         with GSM inactivation */
      gl1_hw_data_ptr[gas_id]->count_wait_for_mdsp_sleep = 0;
   }

   /* Shutdown the mdsp
    */
   mdsp_inact(gas_id);

   return TRUE;

}

/*===========================================================================

FUNCTION gl1_hw_turn_tx_on

DESCRIPTION
  Turn the RF transmit chain on

===========================================================================*/
void gl1_hw_turn_tx_on (gas_id_t gas_id)
{
 if (gl1_hw_tx_on[gas_id] == TRUE)
 {
   MSG_GERAN_MED_0_G( "Request to turn Tx on when HW Tx On already");
 }
 else
 {

   gl1_hw_set_mcpm_tx_on(FALSE, gas_id);
   MSG_GERAN_HIGH_0_G( "MCPM change required for HW Tx On");
   gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_VOICE_START_REQ, GL1_MCPM_GERAN_TX_ON,gas_id);

   /*clear the below flag as RF tx enable is issued to RF*/
   l1_rf_tx_enable_cnf_rcv[gas_id] = FALSE;

   /* Check that TxLM buffer not already allocated */
   if ( gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx != MDSP_TXLM_BUF_IDX_INVALID )
   {
     MSG_GERAN_ERROR_1_G("GSM TxLM buffer %d already allocated - deallocate now",
                         gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx);
     rflm_deallocate_tx_handle(LM_GSM, LM_GSM, gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx);
     gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx = MDSP_RXLM_BUF_IDX_INVALID;
   }

   /* Allocate TxLM buffer, get buffer index (keep trying until allocated)*/
   if ( !rflm_allocate_tx_handle(LM_GSM, LM_GSM, &gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx) )
   {
     MSG_GERAN_ERROR_0_G("Unable to allocate TxLM buffer");
   }

   gl1_hw_rf_enable_tx(gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx,gas_id);

   /* Flag tx as enabled */
   gl1_hw_tx_on[gas_id] = TRUE;
 }
}

/*===========================================================================

FUNCTION gl1_hw_turn_tx_off

DESCRIPTION
  Turn the RF transmit chain off.  Returns TRUE or FALSE to indicate if
  it was successful.  If FALSE, then caller may try again the next frame.

===========================================================================*/
boolean gl1_hw_turn_tx_off( boolean rf_disable_tx,gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK    *l1_tskisr_blk     = &l1_tsk_buffer[gas_id];

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_turn_tx_off: Return TRUE");
   return TRUE;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/


 if (gl1_hw_tx_on[gas_id] == FALSE)
 {
   /* Release TxLM buffer if allocated. */
   gl1_hw_deallocate_txlm_buf_idx(gas_id);
   MSG_GERAN_MED_0_G( "Request to turn Tx off when HW Tx Off already");
 }
 else
 {
   if ( rf_disable_tx )
   {
#ifdef FEATURE_GSM_RX_TX_SPLIT
     gl1_hw_rf_tx_sleep(gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx,
                          gl1_hw_get_gsm_rf_id(grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id), gas_id),
                          gas_id);
#else
     gl1_hw_rf_disable_tx(gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx,
                          gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id,gas_id),
                          gas_id);
#endif
   }

#ifdef FEATURE_GSM_RX_TX_SPLIT
      /* add new TX_EXIT cmd */
      gl1_hw_async_intf_tx_exit_cmd(gas_id);
#else
      mdsp_set_tx_config_cmd_pending(MDSP_TX_CONFIG_NO_CMD_PENDING, gas_id);
#endif
   /* Release TxLM buffer now that Rf and FW Tx sleep/exit are complete */
   gl1_hw_deallocate_txlm_buf_idx(gas_id);

      MSG_GERAN_HIGH_0_G( "MCPM change required for HW Tx Off");
      gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_VOICE_STOP_REQ, GL1_MCPM_GERAN_TX_OFF, gas_id);

   gl1_hw_tx_on[gas_id] = FALSE;
 }
 return TRUE;
}

/*===========================================================================

FUNCTION gl1_hw_turn_rx_on

DESCRIPTION
  Turn the RF receive chain on

===========================================================================*/
void gl1_hw_turn_rx_on (gas_id_t gas_id)
{
   /* Lock interrupts to ensure init
    * completes before GSM frame tick
    * ISR runs.
    */
   //GDRV_ISR_LOCK(gas_id);

   gl1_hw_rf_wakeup(gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx,
                    GL1_PRX_WAKEUP_MODE,
                    gas_id);

   gl1_hw_sleep_set_rf_on(TRUE, gas_id);

   //GDRV_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION gl1_hw_turn_rx_off

DESCRIPTION
  Turn the RF receive chain off

===========================================================================*/
void gl1_hw_turn_rx_off (gas_id_t gas_id)
{
  if (gl1_hw_sleep_get_rf_on(gas_id))
  {
    gl1_hw_rf_sleep(gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx,
                    GL1_PRX_SLEEP_MODE,
                    gas_id);
     gl1_hw_sleep_set_rf_on(FALSE, gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY
    gl1_hw_set_gfw_enter_status(GL1_GFW_INVALID_ENTER, gas_id);
	    /*PRx Sleep would put DRx also to sleep, so no need to run dynamic state again*/
    gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_NULL, gas_id);
    gl1_hw_set_dynamic_rxd_abort(FALSE, gas_id);
    /*Disable Dynamic RxD Handler*/
    gl1_hw_set_dynamic_rxd_tick_required(FALSE, gas_id);
#endif /* FEATURE_GSM_RX_DIVERSITY */
  }
  return;
}

/*===========================================================================

FUNCTION gl1_hw_enter_idle

DESCRIPTION
  Prepares the physical layer hardware for being in Idle Mode.

===========================================================================*/
void gl1_hw_enter_idle (gas_id_t gas_id)
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_enter_idle: Return Immediately");
   return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   gl1_hw_sleep_gsm_enter_idle( gas_id );

   /* SAIC should be On here, but configure to print out SAIC F3 */
   gl1_hw_cfg_saic( TRUE,gas_id );

   /* HW needs to know it's in dedicated for AGC fix. */
   gl1_hw_set_dedicated( FALSE ,gas_id);

   /* Used to inform gl1 that GSM l1 is in IDLE mode */
   gl1_hw_set_idle_mode(TRUE,gas_id);

   /* Flush and restore esaic IIR filter */
   if ( !gl1_msg_get_multi_sim_mode() )
   {
     /* for dual sim this will be done when fw loaded*/
   gl1_hw_flush_esaic_iir_filter(TRUE,TRUE,gas_id);

   /* default RACH power reduction to deactive on entering idle */
   gl1_set_rach_pwr_reduction(FALSE,gas_id);
}

   /*Reset ACI Global Array*/
   gl1_hw_aci_reset_table(gas_id);
}

/*===========================================================================

FUNCTION gl1_hw_leave_idle

DESCRIPTION
  Prepares the physical layer hardware to leave Idle Mode and
  enter dedicated or packet-transfer mode.

===========================================================================*/
void gl1_hw_leave_idle (gas_id_t gas_id)
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   if ( !gl1_hw_get_idle_mode(gas_id) )
   {
       MSG_GERAN_HIGH_0_G("IDLE: gl1_hw_leave_idle: Not in idle mode, Return Immediately");
       return;
   }
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_leave_idle: Return Immediately");
   return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   gl1_hw_sleep_gsm_leave_idle( gas_id );

   /* Inform gl1 we are now leaving IDLE mode */
   gl1_hw_set_idle_mode(FALSE,gas_id);

   /* Enable SAIC (may have been disabled in idle) */
   gl1_hw_cfg_saic( TRUE,gas_id );

   gl1_hw_set_mdsp_asynch_int(FALSE,gas_id);

#ifdef FEATURE_GSM_GPRS_MSC33
   /* Default the scheduler access mode back to FIXED ALLOCATION,
      Avoids false detects of HMSC mode in dedicated
    */
   gl1_hw_schedule_set_medium_access_mode(GL1_DEFS_FIXED_ALLOC,gas_id);
#endif

   MSG_GERAN_MED_2_G("is RF Set Tx Band Reqd - main_command %d, is_panic %d", l1_tskisr_blk->main_command, panic_reset_occured[gas_id]);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* In dual sim mode don't allow set TX band unless firmware is loaded */
   if ( gl1_msg_get_multi_sim_mode() && ( l1_tskisr_blk->firmware_loaded == FALSE ) )
   {
       MSG_GERAN_HIGH_0_G("Cannot call gl1_hw_rf_set_tx_band in dual sim");
   }
   else
#endif
   if (l1_check_for_x2g_ho(gas_id))
   {
     MSG_GERAN_HIGH_0_G("No need to call gl1_hw_rf_set_tx_band from here in X2G HO case");
   }
   /* no need to set Tx band if we're leaving idle to deactivate or suspend */
   else if((l1_tskisr_blk->main_command != L1_DEACTIVATE_COMMAND) &&
      (l1_tskisr_blk->main_command != L1_ACCESS_COMMAND) &&
      (l1_tskisr_blk->main_command != L1_DEDICATED_COMMAND) &&
      (l1_tskisr_blk->main_command != L1_TRANSFER_COMMAND) &&
      (l1_tskisr_blk->main_command != L1_SUSPEND_COMMAND) &&
      (l1_tskisr_blk->main_command != L1_FIND_BCCH_COMMAND) &&
      (l1_tskisr_blk->main_command != L1_FIND_SCH_COMMAND)&&
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      (l1_tskisr_blk->main_command != L1_MULTI_SIM_IDLE_COMMAND) &&
      /* if entering decoupled state then GL1 is releasing current Device
       * and awaiting new TRM grant so dont set tx band
       */
      (l1_tskisr_blk->main_command != L1_CM_HANDOVER_COMMAND_WAIT_FOR_TRM) &&
      (l1_tskisr_blk->main_command != L1_CM_HANDOVER_COMMAND) &&
      (l1_tskisr_blk->main_command != L1_TRANSFER_COMMAND_ABORTING_IDLE) &&
      (l1_tskisr_blk->main_command != L1_DECOUPLE_MULTI_SIM_COMMAND) &&
      (l1_tskisr_blk->main_command != L1_TRANSFER_COMMAND_WAIT_FOR_TRM) &&
      (l1_tskisr_blk->main_command != L1_GET_TIMING_COMMAND)&&
      (l1_tskisr_blk->main_command != L1_GET_TIMING_COMMAND_WAIT_FOR_TRM)&&
      (l1_tskisr_blk->main_command != L1_ENTER_NULL_COMMAND)&&
        /*Will call set_tx_band in transfer init substate instead*/
      !OTHER_GSM_CLIENT_IN_TRANSFER(gas_id) &&
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      !panic_reset_occured[gas_id] )
   {
     /* prepare Tx band now, rather than waiting until first Tx frame - 1 */
     gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(gl1_get_serving_cell_arfcn(gas_id)),gas_id);
   }

   /*Reset ACI Global Array*/
   gl1_hw_aci_reset_table(gas_id);
}

/*===========================================================================

FUNCTION gl1_hw_reset

DESCRIPTION
  Resets fifos in various drivers.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_reset( gas_id_t gas_id )
{
   mdsp_reset_rx_seq_nums(gas_id);
}

/*===========================================================================

FUNCTION gl1_hw_reset_cs_only

DESCRIPTION
  Resets cs fifos in various drivers.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
void gl1_hw_reset_cs_only( gas_id_t gas_id )
{
   mdsp_dtm_reset_rx_cs_only(gas_id);
}
#endif

/*===========================================================================

FUNCTION  GL1_HW_DELAY_FRAME_TICK

DESCRIPTION
  This function prolongs the next TDMA frame tick.  That is the tick is
  always going to occur more than 4.615 ms away from the last one.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_delay_frame_tick (int16 offset,gas_id_t gas_id) /* quarter symbol resolution */
{

  vstmr_adjust_terminal_count_geran (( offset % NUM_QS_PER_FRAME ),
                                   geran_map_gas_id_to_nas_id(gas_id) );

//MSG_GERAN_HIGH_2_G("delay frame tick: Gas id %d, offset %d" ,gas_id,( offset % NUM_QS_PER_FRAME ));
  /* Call function to schedule a frame delay event to check for conflicts */
  gl1_hw_schedule_frame_delay(offset,gas_id);
}

/*===========================================================================
FUNCTION gl1_hw_acq_afc_init

DESCRIPTION
  This function writes the hertz to pdm conversion values to the mdsp.
===========================================================================*/
void gl1_hw_acq_afc_init (gl1_defs_acq_afc_type *data,gas_id_t gas_id)
{
 /* not needed now can be removed */
}

/*===========================================================================
FUNCTION gl1_hw_get_round_val

DESCRIPTION
  Conversion routine to calculate appropriate rounding factor to avoid
  truncation in arithmetic conversions. Need to allow for case when
  scalars are negative and invert retrun value in that case.

  eg Round value for ( ( a * b ) / c ) = ( c / 2 ) unless a * b < 0
     in which case round = -( c / 2 )
     a = input_va;, b = multiplier_val, c = divisor_val
===========================================================================*/
int32 gl1_hw_get_round_val( int32 input_val, int32 multiplier_val,
                            int32 divisor_val )
{
  int32 round_val = ( divisor_val / 2 );

  /* Sign invert only when scalars are negative */
  if ( ( input_val * multiplier_val ) < 0 )
  {
    round_val *= (-1);
  }

  MSG_GERAN_LOW_3( "div_val %d input_val x mult_val %d round_val %d",
           divisor_val, ( input_val * multiplier_val ), round_val );

  return ( round_val );
}

/* Split for 16-bit PDM to allow for larger numerator */
#define HZ_TO_PDM_SCALE_BITS 16

#define HZ_TO_PDM_NUM_SCALE_BITS   12
#define HZ_TO_PDM_NUM_SCALE_VAL    ( 1 << HZ_TO_PDM_NUM_SCALE_BITS )
#define HZ_TO_PDM_DENOM_SCALE_BITS ( HZ_TO_PDM_SCALE_BITS - HZ_TO_PDM_NUM_SCALE_BITS )
#define HZ_TO_PDM_DENOM_SCALE_VAL  ( 1 << HZ_TO_PDM_DENOM_SCALE_BITS )

#define HZ_TO_PDM_SCALE_VAL  ( 1 << HZ_TO_PDM_SCALE_BITS )

/*===========================================================================
FUNCTION gl1_hw_hztopdm/pdmtohz

DESCRIPTION
  Conversion routines from Hertz to PDM and vice versa for writing to the
  vctcxo pdm.  Uses the vctcxo pdm slope and the center band frequency.
  Note the slope is in Q16.
===========================================================================*/
int32 gl1_hw_hztopdm( int32 hz,gas_id_t gas_id )
{
  int32 ans = 0, round;

  /* Sanity check the division */
  if ( ( gl1_hw_data_ptr[gas_id]->band_frequency_mhz == 0 ) || ( gl1_hw_data_ptr[gas_id]->vctcxo_pdm_slope == 0 ) )
  {
    MSG_GERAN_ERROR_2_G( "hztopdm: division by 0! (%d, %d)",
              gl1_hw_data_ptr[gas_id]->band_frequency_mhz, gl1_hw_data_ptr[gas_id]->vctcxo_pdm_slope);

  }
  else
  {

    int32 pdm_band_slope = ( gl1_hw_data_ptr[gas_id]->band_frequency_mhz * gl1_hw_data_ptr[gas_id]->vctcxo_pdm_slope );

    round = gl1_hw_get_round_val( pdm_band_slope, 1,
                                  HZ_TO_PDM_DENOM_SCALE_VAL );

    pdm_band_slope =
      ( ( pdm_band_slope + round ) / HZ_TO_PDM_DENOM_SCALE_VAL );

    round = gl1_hw_get_round_val( hz, HZ_TO_PDM_NUM_SCALE_VAL,
                                  pdm_band_slope );

    ans   = ( ( ( hz * HZ_TO_PDM_NUM_SCALE_VAL ) + round ) /
              pdm_band_slope );
  }

  return ( ans );  /*lint !e703*/
}


/* Split for 16-bit PDM to allow for larger numerator */
#define PDM_TO_HZ_NUM_SCALE_BITS   4
#define PDM_TO_HZ_NUM_SCALE_VAL    ( 1 << PDM_TO_HZ_NUM_SCALE_BITS )
#define PDM_TO_HZ_DENOM_SCALE_BITS ( HZ_TO_PDM_SCALE_BITS - PDM_TO_HZ_NUM_SCALE_BITS )
#define PDM_TO_HZ_DENOM_SCALE_VAL  ( 1 << PDM_TO_HZ_DENOM_SCALE_BITS )

int32 gl1_hw_pdmtohz( int32 pdm,gas_id_t gas_id )
{
  int32 ans;
  int32 pdm_band_slope = ( gl1_hw_data_ptr[gas_id]->band_frequency_mhz * gl1_hw_data_ptr[gas_id]->vctcxo_pdm_slope );

  int32 round =
    gl1_hw_get_round_val( pdm_band_slope, 1, PDM_TO_HZ_NUM_SCALE_VAL );

  pdm_band_slope =
   ( ( pdm_band_slope + round ) / PDM_TO_HZ_NUM_SCALE_VAL );

  round = gl1_hw_get_round_val( pdm, pdm_band_slope,
                                PDM_TO_HZ_DENOM_SCALE_VAL );

  ans = ( ( ( pdm * pdm_band_slope ) + round ) /
          PDM_TO_HZ_DENOM_SCALE_VAL );

  return ( ans );  /*lint !e704*/
}

/*===========================================================================
FUNCTION gl1_hw_hztophase/phasetohz

DESCRIPTION
  Conversion routines from Hertz to phase and vice versa for reading and
  writing to the mdsp driver.
  The phase units are in degrees/sample, with for example 2^19 resolution.
  Samples arrive at 13MHz, so the conversion (in 10 bit precision) is:
    phase = f * 2^19 / 13e6 =~ f * 41 / 1024 = (f * 41) >> 10
    freq  = p * 13e6 / 2^19 =~ p * 25391 / 1024 = (p * 25391) >> 10
  To round the estimate, add half the divisor (ie +512).
  Note that to increase frequency we decrease the phase, hence the "-".

  If these values are changed, make sure the MAX_ROTATOR_TO_PDM value
  still results in a phase change >= 1, preferably an integer.
===========================================================================*/
/* Define factors for the conversion based on the resolution - this cannot
   be done as a macro since the Q10 integer math overflows 32 bits */
#if HW_PHASE_INCREMENT_RESOLUTION == 19
  #define HZTOPHASE_FACTOR_Q10     41   /* 2^19 / 13e6 * 1024 */
  #define PHASETOHZ_FACTOR_Q10  25391   /* 13e6 / 2^19 * 1024 */
#elif HW_PHASE_INCREMENT_RESOLUTION == 22
  /* For cleanup FEATURE_GSM_GPRS_LCU the sign has changed */
  #define HZTOPHASE_FACTOR_Q10   -7929  /* (2^22 * 24) / 13e6 * 1024 */
  #define PHASETOHZ_FACTOR_Q10   -132   /* 13e6 / (24 * 2^22) * 1024 */
#elif HW_PHASE_INCREMENT_RESOLUTION == 23
  #define HZTOPHASE_FACTOR_Q10    661   /* 2^23 / 13e6 * 1024 */
  #define PHASETOHZ_FACTOR_Q10   1587   /* 13e6 / 2^23 * 1024 */
#elif HW_PHASE_INCREMENT_RESOLUTION == 29
  /* Resolution increased to 29 bits due to move to URXFE */
  #define HZTOPHASE_FACTOR_Q10   -63433  /* (2^29 * 24) / 13e6 * 64 */
  #define PHASETOHZ_FACTOR_Q10   -66     /* 13e6 / (24 * 2^29) * 65536 */
#else
  #error HW_PHASE_INCREMENT_RESOLUTION value not supported
#endif

#if HW_PHASE_INCREMENT_RESOLUTION == 29
#define HZ_TO_PHASE_SCALE_BITS 6
#else
#define HZ_TO_PHASE_SCALE_BITS 10
#endif
#define HZ_TO_PHASE_SCALE_VAL  ( 1 << HZ_TO_PHASE_SCALE_BITS )

int32 gl1_hw_hztophase( int32 hz )
{
   int32 ans   = 0;
   int32 round = gl1_hw_get_round_val( hz, HZTOPHASE_FACTOR_Q10,
                                       HZ_TO_PHASE_SCALE_VAL );

   ans = ( ( ( hz * HZTOPHASE_FACTOR_Q10 ) + round ) / HZ_TO_PHASE_SCALE_VAL );

   return -( ans );  /*lint !e704*/
}

#if HW_PHASE_INCREMENT_RESOLUTION == 29
#define PHASE_TO_HZ_SCALE_BITS 16
#else
#define PHASE_TO_HZ_SCALE_BITS HZ_TO_PHASE_SCALE_BITS
#endif
#define PHASE_TO_HZ_SCALE_VAL  ( 1 << PHASE_TO_HZ_SCALE_BITS )

int32 gl1_hw_phasetohz( int32 phase )
{
   int32 ans   = 0;
   int32 round = gl1_hw_get_round_val( phase, PHASETOHZ_FACTOR_Q10,
                                       PHASE_TO_HZ_SCALE_VAL );

   ans = ( ( ( phase * PHASETOHZ_FACTOR_Q10 ) + round ) / PHASE_TO_HZ_SCALE_VAL );

   return -( ans );  /*lint !e704*/
}



/*===========================================================================
FUNCTION gl1_hw_calc_phase_increment

DESCRIPTION
  Calculates what phase increment to pass to the mdsp for activity on the
  given arfcn.  Returns 0 if we do not support rotators.
  Depends on afc_rot_val and band_frequency_mhz being set correctly.
===========================================================================*/
int32 gl1_hw_calc_phase_increment( ARFCN_T arfcn,gas_id_t gas_id )
{

  /* We store away rotator so we can return value without the non-sbi write */
  if ( gl1_hw_is_arfcn_xo_desense( arfcn,gas_id ) != DESENSE_CHAN )
  {
    gl1_hw_rf_update_freq_error( gl1_hw_get_xo_acc_freq_err(gas_id),gas_id );
    gl1_hw_store_rotator_val( 0,gas_id );

  }
  else
  {
    /* We keep rot value stored and always assume rot field holds XO Hz error */
    /* For desense arfcn take stored xo freq error and convert to rot */
    gl1_hw_rf_update_freq_error(0,gas_id);
    gl1_hw_store_rotator_val(
    gl1_hw_convert_normcurrentxofreq_to_rot( arfcn,gas_id ),gas_id );
  }

  MSG_GERAN_LOW_3_G( GL1_AFC_HDR"RX rotator stored as %d for arfcn %d FN:%d",
           gl1_hw_get_rotator_val(gas_id), arfcn.num, GSTMR_GET_FN_GERAN(gas_id) );

  /* For XO as rot derived from XO PPM then avoid any band recalc */
  return ( gl1_hw_get_rotator_val(gas_id) );
}

/*===========================================================================
FUNCTION gl1_hw_set_afc_band

DESCRIPTION
  Sets the center band frequency used for converting hz to ppm/pdm for
  using the rotator and vctcxo using the band that AFC will be operating in.
===========================================================================*/
void gl1_hw_set_afc_band (sys_band_T band, boolean dedicated_mode,gas_id_t gas_id)
{
   /* Sanity check the band */
   if (band > SYS_BAND_CELL_850)
   {
      MSG_GERAN_ERROR_1_G("Invalid AFC band %d",band);

      return;
   }

   /* Save the new band frequency */
   gl1_hw_data_ptr[gas_id]->band_frequency_mhz = midband_freq[band];

/*lint -esym(715,dedicated_mode)*/ /* parameter not used */
}


/*===========================================================================
FUNCTION gl1_hw_tcxo_freq_read

DESCRIPTION
  This function reads the HW freq setting and converts to 12 bits.
  Uses shifts (with test to ensure sign extended correctly).
  Alternative implementation (but note that Q6 divide includes rounding):
     cur_pdm_val = (int16)(HWIO_IN(AGC_CARRIER_FREQ_ERR));
     cur_pdm_val = cur_pdm_val / (1 << GL1_AFC_LOOP_PDM_SHIFT);

===========================================================================*/
int32 gl1_hw_tcxo_freq_read( gas_id_t gas_id )
{
    int16 cur_pdm_val;

  /* For QDSP6 targets we Left shift by  4 for the  12 bit value, and need to */
  /* right shift back by 4 to get the 16 bit value which is used in GL1 */
  /* Need to ensure sign extension on shift, plus rounding */
  /* NOTE:   For Mustang the PDM is always under ARM control in GSM.    */
  cur_pdm_val = (1 <<(GL1_AFC_LOOP_PDM_SHIFT-1));

  if (cur_pdm_val < 0) {
      cur_pdm_val = cur_pdm_val >> GL1_AFC_LOOP_PDM_SHIFT;
      cur_pdm_val |= (0xFFFF << (16 - GL1_AFC_LOOP_PDM_SHIFT));
  }
  else
  {
      cur_pdm_val = cur_pdm_val >> GL1_AFC_LOOP_PDM_SHIFT;
  }

  return ((int32)cur_pdm_val);
}

/*===========================================================================
FUNCTION gl1_hw_acq_tcxo_and_rotator_init

DESCRIPTION
  This function initializes the vctcxo parameters.
===========================================================================*/
void gl1_hw_acq_tcxo_and_rotator_init( gas_id_t gas_id )
{
  tcxomgr_vco_info_type vco_info = gl1_hw_get_tcxomgr_vco( gas_id );

  MSG_GERAN_LOW_1_G( GL1_AFC_HDR"TCXOMgr init vco_info.rot_value:%dXO",
            vco_info.rot_value);

  /* 1. to use FT(temp) directly: gl1_hw_set_xo_acc_freq_err( tcxomgr_ft_curr() );
  ** 2. to use RGS: vco_info = tcxomgr_get_stored_vco(); returns RGS or FT(temp)
  **                gl1_hw_set_xo_acc_freq_err( vco_info.rot_val );
  */

  /* Use rot from XO manager to hold initial FT freq error */
  gl1_hw_set_xo_acc_freq_err( vco_info.rot_value,gas_id );

  MSG_GERAN_LOW_1_G( GL1_AFC_HDR"DPLL init to GL1 local vco_info.rot_value:%dXO",
            vco_info.rot_value);
}

/*===========================================================================
FUNCTION gl1_hw_get_tcxomgr_vco

DESCRIPTION
  This function returns the correct XO manager value depending on
  if this SUB has already acquired and if a valid client value exists.
===========================================================================*/
tcxomgr_vco_info_type gl1_hw_get_tcxomgr_vco( gas_id_t gas_id )
{
  tcxomgr_vco_info_type gl1_vco_info;

  /* If initial acquisition complete then client/instance XO */
  if ( gl1_msg_acq_complete( gas_id ) && gl1_msg_get_multi_sim_mode() )
  {
    tcxomgr_client_info_struct_type gl1_tcxo_get_client_rgs;
    gas_id_t                        tcxo_gas_id;

    gl1_tcxo_get_client_rgs.id    = TCXOMGR_CLIENT_GSM;
    gl1_tcxo_get_client_rgs.as_id =
      gl1_hw_map_gas_id_to_tcxo_asid( gas_id );

    gl1_vco_info = tcxomgr_get_client_rgs( gl1_tcxo_get_client_rgs );

    tcxo_gas_id = gl1_hw_map_tcxo_asid_to_gas_id( gl1_vco_info.as_id );

    /* No valid XO for this SUB so revert to AS_ID then stored */
    if ( gas_id != tcxo_gas_id )
    {
       gl1_vco_info = gl1_hw_get_initial_rgs( gas_id );

       MSG_GERAN_ERROR_0_G( GL1_AFC_HDR"Get Client RGS Mismatched use initial RGS" );
    }
#if defined  FEATURE_TCXOMGR_NEGATE_FREQ_ERROR
    /* Only invert needed for not calling initial_rgs */
    else
  {
      gl1_vco_info.rot_value *= ( -1 );
  }
#endif

    MSG_GERAN_LOW_3_G( GL1_AFC_HDR"Get Client RGS TCXO_ID:%d maps to gas_id:%d value:%dXO",
                        gl1_vco_info.as_id, tcxo_gas_id, gl1_vco_info.rot_value );
  }
  /* Otherwise request seed value from XO manager */
  else
  {
    gl1_vco_info = gl1_hw_get_initial_rgs( gas_id );

    MSG_GERAN_LOW_1_G( GL1_AFC_HDR"Get initial RGS value:%dXO",
                        gl1_vco_info.rot_value );
  }

  return ( gl1_vco_info );
}

/*===========================================================================
FUNCTION gl1_hw_tcxo_init

DESCRIPTION
  This function initializes the vctcxo parameters.
===========================================================================*/
static void gl1_hw_tcxo_init( rex_tcb_type  *task_ptr,
                              void (*task_wait_handler)(rex_sigs_type, gas_id_t),
                              rex_sigs_type task_wait_sig,
                              rex_sigs_type task_done_sig,
                              gas_id_t gas_id  )
{
#if !defined FEATURE_TMC_TCXOMGR
  /* Initialize the TCXO manager */
  (void)tcxomgr_init(task_ptr, task_wait_sig, task_wait_handler);
#endif

#if !defined FEATURE_GERAN_VP
  /* For FTM mode during DSDA testing registering tcxo manager will hit below error_fatal */
  if ( ftm_get_mode() != FTM_MODE )
  {
    gl1_hw_request_tcxo_mgr_3( task_ptr, task_wait_handler,
                               task_wait_sig, task_done_sig, gas_id );
  }
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( !gl1_msg_get_multi_sim_mode() )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
  {
    /* Default the band frequency to GSM if not MSIM. For dual SIM it is initialised to
    this and should not be re-initialsied here, as it will overwrite any current setting */
    gl1_hw_data_ptr[gas_id]->band_frequency_mhz = MID_FREQ_GSM_900_BAND;
  }

  /* Note that at this point the trk_lo might be under ARM control, but
     we have to wait to switch it to mDSP control until the nonsbi_write
     to set the PDM value has been processed by the mDSP.  This might take
     a few frames so the switch to mDSP control is done in
     gl1_hw_tcxo_post_init below. */

  /* Set up the PDM, and Rotator before Acquisition. */
  gl1_hw_acq_tcxo_and_rotator_init(gas_id);
  gl1_msg_set_acq_xo_value(gl1_hw_get_xo_acc_freq_err(gas_id),gas_id);
  gl1_msg_afc_init(gas_id);

#if defined FEATURE_TMC_TCXOMGR
/*lint -esym(715,task_ptr,task_wait_handler,task_wait_sig,task_done_sig)*/ /* parameter not used */
#endif
}

/*===========================================================================
FUNCTION gl1_hw_tcxo_post_init

DESCRIPTION
  This function performs TCXO initialization needed after all other init
  is complete.
===========================================================================*/
static void gl1_hw_tcxo_post_init (gas_id_t gas_id )
{
  /* Clear all frequency error update data including state info */
  gl1_hw_reset_cgps_feu( TRUE, gas_id );
}

/*===========================================================================
FUNCTION gl1_hw_tcxo_inact

DESCRIPTION
  This function prepares the vctcxo for gsm inactivation
===========================================================================*/
void gl1_hw_tcxo_inact( boolean deep_sleep,gas_id_t gas_id  )
{
   MSG_GERAN_HIGH_0_G( GL1_AFC_HDR"Releasing TCXO");

   /*
    * Unconditional release to avoid any situation whereby G does not
    * know that it has TCXO control
    */
   gl1_hw_tcxo_release( gas_id );

   MSG_GERAN_LOW_1_G( GL1_AFC_HDR"Leaving GSM xo_acc_freq_err:%dXO",
             gl1_hw_get_xo_acc_freq_err(gas_id));
}

/*===========================================================================
FUNCTION gl1_hw_afc_locked

DESCRIPTION
  This function is called when the AFC loop is locked.  It informs the TCXO
  manager of this fact and updates the RGS values.
  Should be called periodically or whenever the AFC is changed.

PARAMS
  Boolean - TRUE Idle Mode, FALSE Traffic

RETURN
  None

===========================================================================*/

void gl1_hw_afc_locked( boolean is_idle, gas_id_t gas_id  )
{
   /* for XO, vco(pdm) is always set to 0
    * we use rot to retrieve ppm value applied to DPLL
    * gl1_hw_get_xo_acc_freq_err_in_ppm() returns value in /1024ppm
    * int16 covers max offset of +/-(2^15/1024)=+/-32ppm
    */
   int16 xo_freq_error = (int16)gl1_hw_get_xo_acc_freq_err_in_ppm(gas_id);

#ifdef FEATURE_GERAN_REDUCED_F3
   MSG_GERAN_MED_2_G( GL1_AFC_HDR"AFC locked XO sent to TCXOMgr: %dXO idle:%d",
                      xo_freq_error, is_idle );
#endif

   /* use FEATURE_GSM_TCXOMGR30 format to update RGS, deliver RPUSH */
   if ( !is_idle)
   {
      gl1_hw_update_xo_rgs( xo_freq_error, TCXOMGR_STATE_TRAFFIC, gas_id );
   }
   else
   {
      gl1_hw_update_xo_rgs( xo_freq_error, TCXOMGR_STATE_IDLE, gas_id );
   }

   /* State change so send the rpush */
   if ( gl1_hw_get_cgps_feu_state(gas_id) != is_idle )
   {
      MSG_GERAN_LOW_2_G( "CGPS feu state change is_idle:%d feu_state:%d",
               is_idle, gl1_hw_get_cgps_feu_state(gas_id));

      /* Ensure push is sent when at least a sample has been taken */
      gl1_hw_set_cgps_feu_force_rpush( gas_id );
   }

   MSG_GERAN_LOW_1_G( "CGPS feu state set is_idle:%d",
                      is_idle);

   /* Update current stored state info */
   gl1_hw_set_cgps_feu_state( is_idle, gas_id );
}

/*===========================================================================
FUNCTION gl1_hw_set_vctcxo_pdm_val

DESCRIPTION
  This function sets the TRK_LO_ADJ PDM to the desired value at the next
  frame boundary used by FTM.
===========================================================================*/
void gl1_hw_set_vctcxo_pdm_val( int32 pdm_val, chipxn_control_t TurnClockOff,gas_id_t gas_id )
{
  ( void )gl1_hw_gen_set_vctcxo_pdm_val( pdm_val, TurnClockOff,gas_id );
}

/*===========================================================================
FUNCTION gl1_hw_gen_set_vctcxo_pdm_val

DESCRIPTION
  This function sets the TRK_LO_ADJ PDM to the desired value at the next
  frame boundary used by GSM as we now have a residual rot return.
===========================================================================*/
int32 gl1_hw_gen_set_vctcxo_pdm_val( int32 pdm_val, chipxn_control_t TurnClockOff,gas_id_t gas_id  )
{
  int32 residual_rotator_correction = 0;


  return( residual_rotator_correction );
}

/*===========================================================================
FUNCTION gl1_hw_get_vctcxo_pdm_val

DESCRIPTION
  Returns the current PDM value
===========================================================================*/
int32 gl1_hw_get_vctcxo_pdm_val (gas_id_t gas_id )
{
  return gl1_hw_data_ptr[gas_id]->afc_pdm_val;
}

/*===========================================================================
FUNCTION gl1_hw_convert_tcxomgr_pdm_val

DESCRIPTION
  Returns the default PDM value
===========================================================================*/
int32 gl1_hw_convert_tcxomgr_pdm_val(tcxomgr_vco_type vco_value)
{
   int32 pdm_default_val;

   pdm_default_val = gl1_hw_shared2gsm(vco_value);

   return (pdm_default_val);
}

/*===========================================================================
FUNCTION gl1_hw_get_vctcxo_pdm_default

DESCRIPTION
  Returns the default PDM value
===========================================================================*/
int32 gl1_hw_get_vctcxo_pdm_default_val (void)
{
  return ( 0 );
}


/*===========================================================================
FUNCTION gl1_hw_acq_afc_on_vctcxo

DESCRIPTION
  Used by the acquisition layer to
===========================================================================*/
void gl1_hw_acq_afc_on_vctcxo (boolean active,gas_id_t gas_id )
{
  gl1_hw_data_ptr[gas_id]->acq_afc_on_vctcxo = active;
    }


/*===========================================================================
FUNCTION gl1_hw_set_rotator_val

DESCRIPTION
  This function sets the rotator value.
===========================================================================*/
void gl1_hw_set_rotator_val (int32 new_rot_val,gas_id_t gas_id )
{
  gl1_hw_store_rotator_val( new_rot_val,gas_id );

#if defined ( HWIO_GSM_DSR_SYM_ROT_PHASE_INCR_PHYS )
  /* Write to the mdsp */
  mdsp_nonsbi_write( HWIO_PHYS(GSM_DSR_SYM_ROT_PHASE_INCR), (uint32)gl1_hw_get_rotator_val(gas_id)  ,gas_id);
#endif

  if ( gl1_hw_debug_gsm_afc )
  {
    MSG_GERAN_LOW_2_G( GL1_AFC_HDR"ROT: set to %d FN=%d",
             gl1_hw_get_rotator_val(gas_id), GSTMR_GET_FN_GERAN(gas_id));
  }
}


/*===========================================================================
FUNCTION gl1_hw_store_rotator_val

DESCRIPTION
  This function sets the software copy of the rotator value.  Used when
  the mdsp has done AFC and returned the upper layers the new value.
===========================================================================*/
void gl1_hw_store_rotator_val (int32 new_rot_val,gas_id_t gas_id )
{
  gl1_hw_data_ptr[gas_id]->afc_rot_val = new_rot_val;

  if ( gl1_hw_debug_gsm_afc )
  {
    MSG_GERAN_LOW_2_G( GL1_AFC_HDR"ROT: stored as %d FN:%d",
             gl1_hw_get_rotator_val(gas_id), GSTMR_GET_FN_GERAN(gas_id));
  }
}

/*===========================================================================
FUNCTION gl1_hw_get_rotator_val

DESCRIPTION
  This function returns the current rotator value.
===========================================================================*/
int32 gl1_hw_get_rotator_val (gas_id_t gas_id )
{
  return gl1_hw_data_ptr[gas_id]->afc_rot_val;
}




/*===========================================================================

FUNCTION  gl1_hw_arfcn_to_rfgsm_band

DESCRIPTION
  This function converts an ARFCN to the RF band type.

DEPENDENCIES
  None

RETURN VALUE
  RF band type

SIDE EFFECTS
  None

===========================================================================*/
rfgsm_band_type gl1_hw_arfcn_to_rfgsm_band( ARFCN_T arfcn )
{
   rfgsm_band_type ret_rf_band;

   switch ( arfcn.band )
   {
      case SYS_BAND_DCS_1800:
         ret_rf_band = RFGSM_BAND_GSM1800;
      break;

      case SYS_BAND_PCS_1900:
         ret_rf_band = RFGSM_BAND_GSM1900;
      break;

      case SYS_BAND_CELL_850:
         ret_rf_band = RFGSM_BAND_GSM850;
      break;

      case SYS_BAND_PGSM_900:
      case SYS_BAND_EGSM_900:
      default:
         ret_rf_band = RFGSM_BAND_GSM900;
      break;
   }

  return ( ret_rf_band );
}

/*===========================================================================

FUNCTION  gl1_hw_rfgsm_band_to_sys_band

DESCRIPTION
  This function converts an rfgsm_band_type to sys_band_T

DEPENDENCIES
  None

RETURN VALUE
  sys_band_T

SIDE EFFECTS
  None

===========================================================================*/
sys_band_T gl1_hw_rfgsm_band_to_sys_band( rfgsm_band_type rf_band )
{
   sys_band_T sys_band;

   switch ( rf_band )
   {
      case RFGSM_BAND_GSM1800:
         sys_band = SYS_BAND_DCS_1800;
      break;

      case RFGSM_BAND_GSM1900:
         sys_band = SYS_BAND_PCS_1900;
      break;

      case RFGSM_BAND_GSM850:
         sys_band = SYS_BAND_CELL_850;
      break;

      case RFGSM_BAND_GSM900 :
      default:
         sys_band = SYS_BAND_EGSM_900;
      break;
   }

  return ( sys_band );
}

/*===========================================================================

FUNCTION  gl1_hw_arfcn_to_rfcom_gsm_band

DESCRIPTION
  This function converts an ARFCN to the RF card band type.

DEPENDENCIES
  None

RETURN VALUE
  RF band type

SIDE EFFECTS
  None

===========================================================================*/
rfcom_gsm_band_type gl1_hw_arfcn_to_rfcom_gsm_band( ARFCN_T arfcn )
{
   return ( gl1_hw_arfcn_band_to_rfcom_gsm_band(arfcn.band) );
}

/*===========================================================================

FUNCTION  gl1_hw_arfcn_band_to_rfcom_gsm_band

DESCRIPTION
  This function converts an ARFCN band to the RF com band type.

DEPENDENCIES
  None

RETURN VALUE
  RF band type

SIDE EFFECTS
  None

===========================================================================*/
rfcom_gsm_band_type gl1_hw_arfcn_band_to_rfcom_gsm_band( sys_band_T arfcn_band )
{
   rfcom_gsm_band_type ret_rf_band;

   switch ( arfcn_band )
   {
      case SYS_BAND_DCS_1800:
         ret_rf_band = RFCOM_BAND_GSM1800;
      break;

      case SYS_BAND_PCS_1900:
         ret_rf_band = RFCOM_BAND_GSM1900;
      break;

      case SYS_BAND_CELL_850:
         ret_rf_band = RFCOM_BAND_GSM850;
      break;

      case SYS_BAND_PGSM_900:
      case SYS_BAND_EGSM_900:
      default:
         ret_rf_band = RFCOM_BAND_GSM900;
      break;
   }

  return ( ret_rf_band );
}

/*===========================================================================

FUNCTION gl1_hw_read_nv_items

DESCRIPTION
 Initialize the IQ sample buffer and check for the NV flag to enable
 dumping.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_hw_read_nv_items
(
  rex_tcb_type  *tcb_ptr,
  void (*wait_handler)(rex_sigs_type, gas_id_t),
  rex_sigs_type wait_sig,
  gas_id_t  gas_id
)
{
   uint32 vamos_support = 0;
   nv_item_type   item;
   nv_cmd_type    cmd;

   /* Prepare the NV read command buffer. */
   cmd.cmd        = NV_READ_F;
   cmd.data_ptr   = &item;
   cmd.tcb_ptr    = tcb_ptr;
   cmd.sigs       = wait_sig;
   cmd.done_q_ptr = NULL;

   /* Always make sure this is clear before we start */
   (void)rex_clr_sigs( tcb_ptr, wait_sig );

   if ( !gl1_hw_data_ptr[gas_id]->nv_read )
   {
      cmd.item = NV_EDGE_FEATURE_SUPPORT_I;
      nv_cmd(&cmd);
      wait_handler(wait_sig,gas_id);
      (void) rex_clr_sigs(tcb_ptr, wait_sig);
      if (cmd.status == NV_DONE_S)
      {
         gl1_hw_data_ptr[gas_id]->gl1_hw_features_disabled =
           (item.edge_feature_support & EDGE_FLAG_FEATURE_DISABLE_MASK);
      }

      cmd.item = NV_REPEATED_ACCH_I;
      nv_cmd(&cmd);
      wait_handler(wait_sig,gas_id);
      (void) rex_clr_sigs(tcb_ptr, wait_sig);
      if (cmd.status == NV_DONE_S)
      {
         gl1_hw_data_ptr[gas_id]->gl1_hw_gsm_acch_nv_word = item.repeated_acch;
      }

      /* Set this as the default setting if the nv read fails */
      gl1_hw_data_ptr[gas_id]->gl1_hw_vco_default = GL1_VCO_UNDEFINED_SETTING;

      gl1_hw_data_ptr[gas_id]->nv_read = TRUE;

      gl1_hw_efs_read_mdsp_debug(gas_id);
      gl1_hw_efs_read_mdsp_diag1(gas_id);
      gl1_hw_efs_read_mdsp_diag2(gas_id);
      gl1_hw_efs_read_mdsp_diag3(gas_id);
      gl1_hw_efs_read_mdsp_diag4(gas_id);

      //Update shared variables to FW
      mdsp_update_shared_variable(GFW_DEBUG,&gl1_efs_mdsp_debug,gas_id);
      mdsp_update_shared_variable(GFW_DIAG1,&gl1_efs_mdsp_diag1,gas_id);
      mdsp_update_shared_variable(GFW_DIAG2,&gl1_efs_mdsp_diag2,gas_id);
      mdsp_update_shared_variable(GFW_DIAG3,&gl1_efs_mdsp_diag3,gas_id);
      mdsp_update_shared_variable(GFW_DIAG4,&gl1_efs_mdsp_diag4,gas_id);

      gl1_hw_efs_read_vamos_support(gas_id);

      gl1_hw_efs_read_parallel_sch_support();
      gl1_hw_efs_read_idle_g2x_tuneaway_support();

      gl1_gfw_recovery_read_nv(gas_id);
#ifdef GL1_GMSK_MOD_DET
      gl1_hw_efs_read_gdet_support( );
      gl1_hw_efs_read_gdet_threshold_support( );
#ifdef FEATURE_GSM_SYNQ_SUPPORT
      gl1_hw_efs_read_synq_fr_ctrl();
#endif
#endif
#ifdef FEATURE_GSM_SENS_SUPPORT
      gl1_hw_efs_read_sens_fr_ctrl();
#endif
#ifdef GERAN_L1_ENHANCED_RX
      gl1_hw_efs_read_lif_feature_flag(gas_id);
#endif

      MSG_GERAN_HIGH_0_G( "$$$$$ First GL1 read of NV->" );
   }
else
   {
     vamos_support = gl1_vamos_support[gas_id];

     MSG_GERAN_HIGH_0_G( "##### Cached GL1 NV->" );

     //Update shared variables to FW
     mdsp_update_shared_variable(GFW_DEBUG,&gl1_efs_mdsp_debug,gas_id);
     mdsp_update_shared_variable(GFW_DIAG1,&gl1_efs_mdsp_diag1,gas_id);
     mdsp_update_shared_variable(GFW_DIAG2,&gl1_efs_mdsp_diag2,gas_id);
     mdsp_update_shared_variable(GFW_DIAG3,&gl1_efs_mdsp_diag3,gas_id);
     mdsp_update_shared_variable(GFW_DIAG4,&gl1_efs_mdsp_diag4,gas_id);
     mdsp_update_shared_variable(GFW_VAMOS_SUPPORT,&vamos_support,gas_id);
   }

   MSG_GERAN_HIGH_3_G( "gl1_hw_gprs_anite_gcf:%d gl1_hw_features_disabled:%d gsm_acch_nv_word:%d",
                       mcfg_gcf_nv_get_status(),
                       gl1_hw_data_ptr[gas_id]->gl1_hw_features_disabled,
                       gl1_hw_data_ptr[gas_id]->gl1_hw_gsm_acch_nv_word );

   MSG_GERAN_HIGH_3_G( "gl1_hw_vco_default:%d vamos_support:%d parallel_sch:%d",
                       gl1_hw_data_ptr[gas_id]->gl1_hw_vco_default,
                       vamos_support, gl1_parallel_sch_support );

   mdsp_load_nv(tcb_ptr, wait_handler, wait_sig, gas_id );
}
/*===========================================================================

FUNCTION gl1_hw_cm_efs_read

DESCRIPTION
  Reads EFS needed in CM.

===========================================================================*/
void  gl1_hw_cm_efs_read(gas_id_t gas_id)
   {
  MSG_GERAN_MED_0_G( "Read EFS for CM" );

  gl1_hw_efs_read_mdsp_debug(gas_id);
  gl1_hw_efs_read_mdsp_diag1(gas_id);
  gl1_hw_efs_read_mdsp_diag2(gas_id);
  gl1_hw_efs_read_mdsp_diag3(gas_id);
  gl1_hw_efs_read_mdsp_diag4(gas_id);

  gl1_hw_efs_read_parallel_sch_support();

#ifdef GERAN_L1_ENHANCED_RX
  gl1_hw_efs_read_lif_feature_flag(gas_id);
#endif
   }

/*===========================================================================

FUNCTION gl1_hw_cm_efs_update

DESCRIPTION
  Update EFS to mdsp shared variable needed in CM.

===========================================================================*/
void  gl1_hw_cm_efs_update(gas_id_t gas_id)
  {
     //Update shared variables to FW
     mdsp_update_shared_variable(GFW_DEBUG,&gl1_efs_mdsp_debug,gas_id);
     mdsp_update_shared_variable(GFW_DIAG1,&gl1_efs_mdsp_diag1,gas_id);
     mdsp_update_shared_variable(GFW_DIAG2,&gl1_efs_mdsp_diag2,gas_id);
     mdsp_update_shared_variable(GFW_DIAG3,&gl1_efs_mdsp_diag3,gas_id);
     mdsp_update_shared_variable(GFW_DIAG4,&gl1_efs_mdsp_diag4,gas_id);
  }

/*===========================================================================

FUNCTION gl1_hw_feature_disabled_aci_detection

DESCRIPTION
  Returns if ACI detection is manually disabled or not.

===========================================================================*/
static boolean gl1_hw_feature_disabled_aci_detection (gas_id_t gas_id)
{
   /* If the bit is set, the feature is disabled */
   return (gl1_hw_data_ptr[gas_id]->gl1_hw_features_disabled & GL1_HW_FEATURE_DISABLE_ACI_DETECTION);
}

#ifdef FEATURE_GSM_MDSP_IR
/*===========================================================================

FUNCTION gl1_hw_feature_disabled_ir

DESCRIPTION
  Returns if incremental redundancy is manually disabled or not.

===========================================================================*/
boolean gl1_hw_feature_disabled_ir (gas_id_t gas_id)
{
   /* If the bit is set, the feature is disabled */
   return (gl1_hw_data_ptr[gas_id]->gl1_hw_features_disabled & GL1_HW_FEATURE_DISABLE_IR);
}
#endif

/*===========================================================================

FUNCTION gl1_hw_feature_disabled_saic

DESCRIPTION
  Returns if SAIC is manually disabled or not.

===========================================================================*/
boolean gl1_hw_feature_disabled_saic (gas_id_t gas_id)
{
   /* If the bit is set, the feature is disabled */
   return ( gl1_hw_data_ptr[gas_id]->gl1_hw_features_disabled & GL1_HW_FEATURE_DISABLE_SAIC );
}


#ifdef FEATURE_GSM_MDSP_ESAIC
/*===========================================================================

FUNCTION gl1_hw_feature_disabled_esaic

DESCRIPTION
  Returns if ESAIC is manually disabled or not.

===========================================================================*/
boolean gl1_hw_feature_disabled_esaic (gas_id_t gas_id)
{
   boolean esaic_disabled = FALSE;
   /* If the bit is set or SAIC is disabled, the feature is disabled */
   if ((gl1_hw_data_ptr[gas_id]->gl1_hw_features_disabled & GL1_HW_FEATURE_DISABLE_ESAIC) || gl1_hw_feature_disabled_saic(gas_id))
   {
      esaic_disabled = TRUE;
   }
   return (esaic_disabled);
}
#endif /* FEATURE_GSM_MDSP_ESAIC */

/*===========================================================================

FUNCTION gl1_hw_feature_disabled_aeq

DESCRIPTION
  Returns if AEQ is manually disabled or not.

===========================================================================*/
boolean gl1_hw_feature_disabled_aeq (gas_id_t gas_id)
{
   boolean aeq_disabled = FALSE;
   /* If the bit is set or SAIC is disabled, the feature is disabled */
   if ((gl1_hw_data_ptr[gas_id]->gl1_hw_features_disabled & GL1_HW_FEATURE_DISABLE_AEQ) || gl1_hw_feature_disabled_saic(gas_id))
   {
       aeq_disabled = TRUE;
   }
   return (aeq_disabled);
}


#ifdef FEATURE_GSM_MDSP_AEQ_SCH
/*===========================================================================

FUNCTION gl1_hw_feature_disabled_aeq

DESCRIPTION
  Returns if AEQ is manually disabled or not.

===========================================================================*/
boolean gl1_hw_feature_disabled_aeq_sch (gas_id_t gas_id)
{
   boolean aeq_sch_disabled = FALSE;
   /* If the bit is set the feature is disabled */
   if (gl1_hw_data_ptr[gas_id]->gl1_hw_features_disabled & GL1_HW_FEATURE_DISABLE_AEQ_SCH)
   {
       aeq_sch_disabled = TRUE;
   }
   return (aeq_sch_disabled);
}
#endif  /* FEATURE_GSM_MDSP_AEQ_SCH */

#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
/*===========================================================================

FUNCTION gl1_hw_feature_disabled_aeq

DESCRIPTION
  Returns if AEQ is manually disabled or not.

===========================================================================*/
boolean gl1_hw_feature_disabled_aeq_8psk (gas_id_t gas_id)
{
   boolean aeq_8psk_disabled = FALSE;
   /* If the bit is set or SAIC is disabled, the feature is disabled */
   if ((gl1_hw_data_ptr[gas_id]->gl1_hw_features_disabled & GL1_HW_FEATURE_DISABLE_AEQ_8PSK) || gl1_hw_feature_disabled_saic(gas_id))
   {
       aeq_8psk_disabled = TRUE;
   }
   return (aeq_8psk_disabled);
}
#endif /* FEATURE_GSM_MDSP_AEQ_8PSK*/

/*===========================================================================

FUNCTION gl1_hw_check_disabled_features

DESCRIPTION
  Checks which features are disabled and takes necessary action.

===========================================================================*/
static void gl1_hw_check_disabled_features (gas_id_t gas_id)
{
   if (gl1_hw_data_ptr[gas_id]->gl1_hw_features_disabled != GL1_HW_FEATURE_DISABLE_NONE)
   {
#ifdef FEATURE_GSM_MDSP_IR
      if (gl1_hw_feature_disabled_ir(gas_id))
      {
        MSG_GERAN_HIGH_0_G( "IR Disabled");
      }
#endif

      /* Disable ACI detection if requested */
      if (gl1_hw_feature_disabled_aci_detection(gas_id))
      {
         MSG_GERAN_HIGH_0_G( "ACI Disabled");
         mdsp_gprs_set_aci_detection(FALSE ,gas_id);
      }

      /* Disable SAIC if requested */
      if (gl1_hw_feature_disabled_saic(gas_id))
      {
         MSG_GERAN_HIGH_0_G( "SAIC Disabled");
         mdsp_enable_saic(FALSE ,gas_id);
      }

#ifdef FEATURE_GSM_MDSP_ESAIC
      /* Disable ESAIC if requested, but only check if SAIC is still enabled */
      if (gl1_hw_feature_disabled_esaic(gas_id))
      {
         MSG_GERAN_HIGH_0_G( "ESAIC Disabled");
         mdsp_enable_esaic(FALSE ,gas_id);
      }
#endif /* FEATURE_GSM_MDSP_ESAIC */

      if (gl1_hw_feature_disabled_aeq(gas_id) )
      {
        /* disable AEQ */
         MSG_GERAN_HIGH_0_G( "AEQ Disabled");
         mdsp_enable_aeq(FALSE ,gas_id);
      }

#ifdef FEATURE_GSM_MDSP_AEQ_SCH
     if (gl1_hw_feature_disabled_aeq_sch(gas_id) )
     {
       /* disable AEQ SCH */
       MSG_GERAN_HIGH_0_G("AEQ SCH Disabled");
       mdsp_enable_aeq_sch(FALSE ,gas_id);
     }
#endif  /* FEATURE_GSM_MDSP_AEQ_SCH */

#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
     if (gl1_hw_feature_disabled_aeq_8psk(gas_id) )
     {
       /* disable AEQ 8PSK*/
       MSG_GERAN_HIGH_0_G("AEQ PH2 (8PSK) Disabled");
       mdsp_enable_aeq_8psk(FALSE ,gas_id);
     }
#endif  /* FEATURE_GSM_MDSP_AEQ_8PSK */
   }
   /* check if sleep feauture can be disabled */
   gl1_hw_sleep_disable[gas_id] = gl1_is_sleep_disabled_with_gcf( gas_id );   
   MSG_GERAN_MED_4_G(GL1_SLEEP_HDR" GSM sleep_disabled: %d, gcf_enabled: %d, efs_slp: %d, efs_l1: %d",
   gl1_hw_sleep_disable[gas_id], mcfg_gcf_nv_get_status(), gl1_hw_efs_get_slp(), gl1_hw_efs_get_debug(gas_id) );
}

/*===========================================================================

FUNCTION gl1_hw_set_async_rx_search_width

DESCRIPTION
 Sets the Async RX Search Width.
 Use of Search Width:
 Search width is divided by two and appended to each side of SCH burst length

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_set_async_rx_search_width (uint16 width
#ifdef FEATURE_DUAL_SIM
                                       ,sys_modem_as_id_e_type      as_id
#endif /*FEATURE_DUAL_SIM */
)
{
#ifdef FEATURE_DUAL_SIM
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#else
   gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif
   GDRV_ISR_LOCK(gas_id);

   mdsp_set_async_rx_search_width(width ,gas_id);

   GDRV_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION gl1_hw_register_panic_cb

DESCRIPTION
  Function called by PL1 to register a callback function called whenever
  mDSP halt or bad NPL1/PL1 events are received. The callback handler
  should be able propagate to upper layers the error message. This function
  should be called during initialization of the hardware layer.

DEPENDENCIES
  None

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_register_panic_cb(gl1_hw_panic_cb_type cb,gas_id_t gas_id)
{
   if(!cb)
   {
      MSG_GERAN_ERROR_0_G("No panic cb passed to NPL1");
   }
   else
   {
      gl1_hw_panic_cb = cb;
   }
}

/*===========================================================================

FUNCTION gl1_hw_handle_panic

DESCRIPTION
  This function calls the PL1 callback handler registered in an event of
  panic (mdsp halt or bad NPL1/PL1 situations).

DEPENDENCIES
  gl1_hw_register_panic_cb() should be called ahead of time.

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_handle_panic(gas_id_t gas_id)
{
   if(gl1_hw_panic_cb == NULL)
   {
      MSG_GERAN_FATAL_0_G( "No callback installed" );
   }
   else if(gas_id  !=  GERAN_ACCESS_STRATUM_ID_UNDEFINED)
   {
     gl1_hw_panic_cb(gas_id);
   }
}

/*===========================================================================

FUNCTION gl1_hw_handle_sleep_panic

DESCRIPTION
  This function panic reset command for sleep timeline error.

DEPENDENCIES
  None

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_handle_sleep_panic(gas_id_t gas_id)
{
   /* Let the sleep controller handler the panic to make sure we are awake */
   gl1_hw_sleep_gsm_panic_inact(gas_id);

   /* Initialise sleep controller when panic reset. */
   gl1_hw_sleep_gsm_init(gas_id);
   /* register NPA cb*/
   gl1_hw_sleep_register_npa_cb(gas_id);

   if (geran_get_nv_recovery_restart_enabled(gas_id))
   {
      MSG_GERAN_ERROR_0_G("*** SLEEP TIMELINE FAILURE ***");
      l1_call_panic_reset_in_task(L1_STALLED_ISR,gas_id);
   }
   else
   {
     //ERR_FATAL("*** SLEEP TIMELINE FAILURE ***", 0, 0, 0);
     /*safest solution is not to crash (ERR FATAL) but to panic reset and regain service*/
     l1_call_panic_reset_in_task(L1_STALLED_ISR,gas_id);
   }
}

/*===========================================================================

FUNCTION gl1_hw_get_afc_pdm_val

DESCRIPTION
  Returns the raw PDM for TXC0 Mgr 3.0.

DEPENDENCIES
  None

RETURN
  tcxomgr_vco_type - AFC PDM Value

SIDE EFFECTS
  None
===========================================================================*/
tcxomgr_vco_type gl1_hw_get_afc_pdm_val( gas_id_t gas_id )
{
  return ( (tcxomgr_vco_type) gl1_hw_gsm2shared(gl1_hw_data_ptr[gas_id]->afc_pdm_val) );
}


/*===========================================================================

FUNCTION gl1_hw_set/get_gsm_only_mode

DESCRIPTION
  Store/Return the status of the local gsm_only_mode status.

DEPENDENCIES
  None

RETURN
  None/Current gl1_hw_gsm_only_mode value

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_set_gsm_only_mode( boolean gl1_gsm_only_mode,gas_id_t gas_id )
{
  if ( gl1_hw_gsm_only_mode[gas_id] != gl1_gsm_only_mode )
  {
    MSG_GERAN_HIGH_2_G( "gl1_hw_set_gsm_only_mode: old:%d -> new:%d",
                        gl1_hw_gsm_only_mode[gas_id], gl1_gsm_only_mode );
  }

  gl1_hw_gsm_only_mode[gas_id] = gl1_gsm_only_mode;
}

boolean gl1_hw_get_gsm_only_mode( gas_id_t gas_id )
{
  MSG_GERAN_LOW_1_G( "gl1_hw_get_gsm_only_mode:%d",
                      gl1_hw_gsm_only_mode[gas_id] );

  return ( gl1_hw_gsm_only_mode[gas_id] );
}

#ifdef FEATURE_SGLTE
/*===========================================================================

FUNCTION gl1_hw_set/get_sglte_mode

DESCRIPTION
  Store/Return the status of the local sglte_mode status.

DEPENDENCIES
  None

RETURN
  None/Current sglte value

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_set_sglte_mode( boolean gl1_sglte_mode,gas_id_t gas_id )
{
  if ( gl1_hw_sglte_mode[gas_id] != gl1_sglte_mode )
  {
    MSG_GERAN_HIGH_2_G( "gl1_hw_set_sglte_mode: old:%d -> new:%d",
                        gl1_hw_sglte_mode[gas_id], gl1_sglte_mode );
  }

  GL1_ISR_LOCK(gas_id);
  /* Set SGLTE mode - only expected on gas_id 0*/
  gl1_hw_sglte_mode[gas_id] = gl1_sglte_mode;
  GL1_ISR_UNLOCK(gas_id);
}

boolean gl1_hw_get_sglte_mode( gas_id_t gas_id )
{
  boolean gl1_hw_sglte_enabled = FALSE;
  int i;

  MSG_GERAN_LOW_1_G( "gl1_hw_get_sglte_mode:%d",
                      gl1_hw_sglte_mode[gas_id] );

  for(i = 0; i < NUM_GERAN_DATA_SPACES; i++)
  {
    gl1_hw_sglte_enabled = gl1_hw_sglte_enabled || gl1_hw_sglte_mode[i];
  }
  return ( gl1_hw_sglte_enabled );
}

/*===========================================================================

FUNCTION gl1_hw_get_sglte_plus_g_mode

DESCRIPTION
  Gets the SGLTE + G DSDA mode.

DEPENDENCIES
  None

RETURN
  IF SGLTE + G DSDA mode, then return TRUE or else FALSE

SIDE EFFECTS
  None
===========================================================================*/

boolean gl1_hw_get_sglte_plus_g_mode(gas_id_t gas_id)
{
  /* IF SGLTE + G DSDA mode, then return TRUE or else FALSE */
  if(gl1_hw_get_sglte_mode(gas_id) && (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
#endif /* FEATURE_SGLTE */

/*===========================================================================

FUNCTION gl1_hw_ncell_pch_low_monitor_active

DESCRIPTION
  Return the status of upper layers reduced monitor state.

DEPENDENCIES
  None

RETURN
  Current reduced monitor state status

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_hw_ncell_pch_low_monitor_active( gas_id_t gas_id )
{
  return ( gl1_msg_ncell_pch_low_monitor_active(gas_id) );
}


/*===========================================================================

FUNCTION gl1_hw_acch_get_nv_flag

DESCRIPTION
  Returns the raw repeated ACCH NV word.

DEPENDENCIES
  None

RETURN
  uint16

SIDE EFFECTS
  None
===========================================================================*/
uint16 gl1_hw_acch_get_nv_flag (gas_id_t gas_id)
{
    return gl1_hw_data_ptr[gas_id]->gl1_hw_gsm_acch_nv_word;
}

/*===========================================================================
FUNCTION gl1_hw_get_rx_freq

DESCRIPTION
  This function gets current rf frequency, it should be called after acq
  to normalize error to ppm
===========================================================================*/

static int32 gl1_hw_get_rx_freq( ARFCN_T arfcn,gas_id_t gas_id )
{
  /* Default to G900 incase band not set correctly */
  int32 rx_freq = ( BASE_FREQ_GSM_900_BAND + ( GSM_CHANNEL_BW * arfcn.num )
                    + DL_OFFSET_FREQ_GSM_900_BAND );

  switch ( arfcn.band )
  {
    case SYS_BAND_DCS_1800:
      rx_freq = ( BASE_FREQ_DCS_1800_BAND + ( GSM_CHANNEL_BW * ( arfcn.num - 512 ) )
                  + DL_OFFSET_FREQ_DCS_1800_BAND );
    break;

    case SYS_BAND_PCS_1900:
      rx_freq = ( BASE_FREQ_PCS_1900_BAND + ( GSM_CHANNEL_BW * ( arfcn.num - 512 ) )
                  + DL_OFFSET_FREQ_PCS_1900_BAND );
    break;

    case SYS_BAND_CELL_850:
      rx_freq = ( BASE_FREQ_GSM_850_BAND + ( GSM_CHANNEL_BW * ( arfcn.num - 128 ) )
                  + DL_OFFSET_FREQ_GSM_850_BAND );
    break;

    case SYS_BAND_PGSM_900:
    case SYS_BAND_EGSM_900:
      if ( arfcn.num <= 124 )
      {
        rx_freq = ( BASE_FREQ_GSM_900_BAND + ( GSM_CHANNEL_BW * arfcn.num )
                    + DL_OFFSET_FREQ_GSM_900_BAND );
      }
      else
      {
        rx_freq = ( BASE_FREQ_GSM_900_BAND + ( GSM_CHANNEL_BW * ( arfcn.num - 1024 ) )
                    + DL_OFFSET_FREQ_GSM_900_BAND );
      }
    break;

    default:
      MSG_GERAN_ERROR_1_G( GL1_AFC_HDR"Unsupported RF band: %d",
                 arfcn.band);
    break;
  }

  MSG_GERAN_LOW_3_G( GL1_AFC_HDR"rx_freq %d for arfcn %d band %d",
           rx_freq, arfcn.num, arfcn.band );

  return ( rx_freq );
}

/*===========================================================================
FUNCTION gl1_hw_get_norm_rx_freq

DESCRIPTION
  This function gets current rf frequency, it should be called after acq
  to normalize error to ppm
===========================================================================*/

int32 gl1_hw_get_norm_rx_freq( ARFCN_T arfcn,gas_id_t gas_id )
{
  int32 norm_rx_freq = gl1_hw_get_rx_freq( arfcn,gas_id );

  /* Apply rounding */
  norm_rx_freq += gl1_hw_get_round_val( norm_rx_freq, 1,
                                        XO_RF_FREQ_NORM_SCALE_VAL );

  norm_rx_freq /= XO_RF_FREQ_NORM_SCALE_VAL;

  MSG_GERAN_LOW_3_G( GL1_AFC_HDR"norm_rx_freq %d for arfcn %d band %d",
           norm_rx_freq, arfcn.num, arfcn.band );

  return ( norm_rx_freq );
}

/*===========================================================================
FUNCTION gl1_hw_update_xo_acc_freq_err

DESCRIPTION
  This function sets current xo value, it should be called after acq
  afc when the dsp has modified the pdm value behind our back as we do
  not actually write the new value to the pdm.
===========================================================================*/
void gl1_hw_update_xo_acc_freq_err( int32 xo_freq_err,gas_id_t gas_id )
{
  gl1_hw_set_xo_acc_freq_err( gl1_hw_get_xo_acc_freq_err(gas_id) + xo_freq_err,gas_id );
#ifdef FEATURE_GERAN_REDUCED_F3
  MSG_GERAN_HIGH_3_G( GL1_AFC_HDR"updated by %dXO to %dXO FN:%d",
            xo_freq_err, gl1_hw_get_xo_acc_freq_err(gas_id), GSTMR_GET_FN_GERAN(gas_id) );
#endif
}

/*===========================================================================
FUNCTION gl1_hw_set_xo_acc_freq_err

DESCRIPTION
  This function sets current xo value, normalized to *2^10ppm, it should be
  called after acq afc when the dsp has modified the pdm value behind our
  back as we do not actually write the new value to the pdm.
===========================================================================*/
void gl1_hw_set_xo_acc_freq_err( int32 xo_value,gas_id_t gas_id )
{
  gl1_hw_store_xo_acc_freq_err( xo_value,gas_id );

  gl1_hw_rf_update_freq_error( gl1_hw_get_xo_acc_freq_err(gas_id),gas_id );

  MSG_GERAN_LOW_2_G( GL1_AFC_HDR"set as %dXO FN:%d",
           gl1_hw_get_xo_acc_freq_err(gas_id), GSTMR_GET_FN_GERAN(gas_id));
}

/*===========================================================================
FUNCTION gl1_hw_store_xo_acc_freq_err

DESCRIPTION
  This function sets the software copy of the accumulated frequency error.
  Used when the mdsp has done AFC and returned the upper layers the new value.
===========================================================================*/
static void gl1_hw_store_xo_acc_freq_err( int32 xo_value,gas_id_t gas_id )
{
  gl1_hw_data_ptr[gas_id]->gl1_xo_acc_freq_err = xo_value;
}

/*===========================================================================

FUNCTION gl1_hw_get_xo_acc_freq_err/_in_ppb

DESCRIPTION
  Returns the XO value in Hz/ppb for TXC0 Mgr 3.0.

DEPENDENCIES
  None

RETURN
  int32 - Accumulated XO freq err in ppb value which is the same as actual
          freq error for XO

SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_hw_get_xo_acc_freq_err_in_ppm( gas_id_t gas_id )
{
  return ( gl1_hw_get_xo_acc_freq_err(gas_id) );
}

int32 gl1_hw_get_xo_acc_freq_err( gas_id_t gas_id )
{
  return ( gl1_hw_data_ptr[gas_id]->gl1_xo_acc_freq_err );
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION  gl1_hw_map_gas_id_to_wmgr_client

DESCRIPTION
  This function maps gas_id into WMGR_CLIENT.

===========================================================================*/
wmgr_client_enum_t gl1_hw_map_gas_id_to_wmgr_client( gas_id_t gas_id )
{
   /* Get MCPM GSTMR Lock and check GSM clock is on */
   wmgr_client_enum_t wmgr_client = WMGR_CLIENT_NO_CLIENT;

   /* Check which GSTMR HW block this handler was fired on*/
   switch ( gas_id )
   {
     case GERAN_ACCESS_STRATUM_ID_1:
       wmgr_client = WMGR_CLIENT_GSM1;
     break;

     case GERAN_ACCESS_STRATUM_ID_2:
       wmgr_client = WMGR_CLIENT_GSM2;
     break;

#if defined ( FEATURE_TRIPLE_SIM )
     case GERAN_ACCESS_STRATUM_ID_3:
       wmgr_client = WMGR_CLIENT_GSM3;
     break;
#endif /* FEATURE_TRIPLE_SIM */

     case GERAN_ACCESS_STRATUM_ID_UNDEFINED:
     default:
       MSG_GERAN_FATAL_0_G( "Unknown GAS_ID config" );
     break;
   }

   return ( wmgr_client );
}

/*===========================================================================

FUNCTION  gl1_hw_map_wmgr_client_to_gas_id

DESCRIPTION
  This function maps WMGR_CLIENT into gas_id.

===========================================================================*/
gas_id_t gl1_hw_map_wmgr_client_to_gas_id( wmgr_client_enum_t wmgr_client )
{
   /* Get MCPM GSTMR Lock and check GSM clock is on */
   gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

   /* Check which GSTMR HW block this handler was fired on*/
   switch ( wmgr_client )
   {
     case WMGR_CLIENT_GSM1:
       gas_id = GERAN_ACCESS_STRATUM_ID_1;
     break;

     case WMGR_CLIENT_GSM2:
       gas_id = GERAN_ACCESS_STRATUM_ID_2;
     break;

#if defined ( FEATURE_TRIPLE_SIM )
     case WMGR_CLIENT_GSM3:
       gas_id = GERAN_ACCESS_STRATUM_ID_3;
     break;
#endif /* FEATURE_TRIPLE_SIM */

   default:
       MSG_GERAN_FATAL_1_G( "Unknown wmgr_client %d config",
                            wmgr_client );
     break;
   }

   return ( gas_id );
}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION gl1_hw_normxofreqtofreq

DESCRIPTION
  Converts the XO value in Hz/ppm to actual Hz error.

DEPENDENCIES
  None

RETURN
  int32 - XO PPM value in Hz

SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_hw_normxofreqtofreq( ARFCN_T arfcn, int32 norm_xo_freq,gas_id_t gas_id )
{
  int32 freq;
  int32 norm_rx_freq = ( gl1_hw_get_norm_rx_freq( arfcn,gas_id )/2 );  /* Divide by 2 so that no number overflow */
  freq = norm_rx_freq * norm_xo_freq;

  /* Apply rounding */
  freq += gl1_hw_get_round_val( norm_xo_freq, norm_rx_freq,
                                GSM_XO_RF_SCALING_FACTOR );

  freq /= GSM_XO_RF_SCALING_FACTOR;

  MSG_4( MSG_SSID_GSM_L1, MSG_LEGACY_LOW,
         GL1_AFC_HDR"Norm XO freq %dXO to freq %dHz for ARFCN %d band %d",
         norm_xo_freq, freq, arfcn.num, arfcn.band );

  return ( freq );
}

/*===========================================================================

FUNCTION gl1_hw_freqtonormxofreq

DESCRIPTION
  Returns the freq value in Hz retruned from acquisition
  into F_err*2^14/(F_carr/2^16) for XO.

DEPENDENCIES
  None

RETURN
  int32 - Accumulated XO freq err in ppm value which is the same as actual
          freq error for XO

SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_hw_freqtonormxofreq( ARFCN_T arfcn, int32 fine_freq, gas_id_t gas_id )
{
  /* normalize freq error to F_err*2^14/(F_carr/2^16) */
  int32 norm_xo_freq = ( fine_freq * GSM_XO_RF_SCALING_FACTOR );

  /* Divide by 2 added as GSM_XO_RF_SCALING_FACTOR halved to prevent number overflow */
  int32 norm_rx_freq = (gl1_hw_get_norm_rx_freq( arfcn,gas_id )/2);

  /* Apply rounding */
  norm_xo_freq += gl1_hw_get_round_val( fine_freq, GSM_XO_RF_SCALING_FACTOR,
                                        norm_rx_freq );

  /* f>>14: normalized to 2^-10ppm/LSB */
  norm_xo_freq /= norm_rx_freq;

  MSG_4( MSG_SSID_GSM_L1, MSG_LEGACY_LOW,
         GL1_AFC_HDR"freq %dHz to norm_xo_ff %dXO for ARFCN %d band %d",
         fine_freq, norm_xo_freq, arfcn.num, arfcn.band );

  return ( norm_xo_freq );
}

/*===========================================================================

FUNCTION gl1_hw_convert_normcurrentxofreq_to_rot

DESCRIPTION
  Returns the current rot equivalent of XO ppm.

DEPENDENCIES
  None

RETURN
  int32 - Current XO freq error in rot units

SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_hw_convert_normcurrentxofreq_to_rot( ARFCN_T arfcn,gas_id_t gas_id )
{
  /* Assume when updating XO we use normalised value */
  int32 rot_val = gl1_hw_convert_normxofreq_to_rot(
                    arfcn, gl1_hw_get_xo_acc_freq_err(gas_id),gas_id );

  return ( rot_val );
}

/*===========================================================================

FUNCTION gl1_hw_convert_normxofreq_to_rot

DESCRIPTION
  Returns the current rot equivalent of the delta between desired XO ppm value
  and the currently active value, used for Ncell decoodes that use delta
  as a rotator correction.

DEPENDENCIES
  None

RETURN
  int32 - Current XO freq error in rot units

SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_hw_convert_normxofreq_to_rot( ARFCN_T arfcn, int32 xo_val,gas_id_t gas_id )
{
  /* Assume when updating XO we use normalised value */
  int32 rot_val = gl1_hw_normxofreqtofreq( arfcn, xo_val,gas_id );

  /* Now convert to rotator units */
  rot_val = gl1_hw_hztophase( rot_val );

  return ( rot_val );
}

/*===========================================================================

FUNCTION gl1_hw_set_xo_rx_config

DESCRIPTION
  Decides if we need to apply XO to rotator for desense ARFCN.
  This is called when we config RX to enable correct setting
  when we reconfigure and also if freq hopping enabled.
  Also configures any cycle swallower updates and sets trigger.

DEPENDENCIES
  None

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_set_xo_rx_config( ARFCN_T arfcn, gas_id_t gas_id )
{
  if ( gl1_hw_is_arfcn_xo_desense( arfcn,gas_id ) != DESENSE_CHAN )
  {
    /* Clear the rotator as all correction on XO */
    gl1_hw_set_rotator_val( 0,gas_id );
  }
  else
  {
    /* We keep rot value stored and always assume rot field holds XO Hz error */
    /* For desense arfcn take stored xo freq error and convert to rot */

    gl1_hw_set_rotator_val(
      gl1_hw_convert_normcurrentxofreq_to_rot( arfcn,gas_id ),gas_id );

  }

  MSG_GERAN_LOW_3_G( GL1_AFC_HDR"RX rotator set to %d for arfcn %d FN:%d",
           gl1_hw_get_rotator_val(gas_id), arfcn.num, GSTMR_GET_FN_GERAN(gas_id) );
}

/*===========================================================================

FUNCTION gl1_hw_gfw_dco_freq

DESCRIPTION
  Returns the value of mid-band frequency in MHz.
  Used for setting the parameters for GFW frequency correction.

DEPENDENCIES
  None

SIDE EFFECTS
  Returns mid-band frequency in MHz
  Sets gl1_hw_gfw_freq_config.inv_f_dl_lo (as 2^52 / mid-band freq in Hz)
===========================================================================*/
int32 gl1_hw_gfw_dco_freq( ARFCN_T arfcn, gas_id_t gas_id )
{
  int32 midband_freq_mhz ;

  switch ( arfcn.band )
  {
    case SYS_BAND_DCS_1800:
      midband_freq_mhz = MID_FREQ_DCS_1800_BAND;
      gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.inv_f_dl_lo = INV_F_DL_LO_DCS_1800_BAND;
    break;

    case SYS_BAND_PCS_1900:
      midband_freq_mhz = MID_FREQ_PCS_1900_BAND;
      gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.inv_f_dl_lo = INV_F_DL_LO_PCS_1900_BAND;
    break;

    case SYS_BAND_CELL_850:
      midband_freq_mhz = MID_FREQ_GSM_850_BAND;
      gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.inv_f_dl_lo = INV_F_DL_LO_GSM_850_BAND;
    break;

    case SYS_BAND_PGSM_900:
    case SYS_BAND_EGSM_900:
      midband_freq_mhz = MID_FREQ_GSM_900_BAND;
      gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.inv_f_dl_lo = INV_F_DL_LO_GSM_900_BAND;
    break;

    default:
      midband_freq_mhz = MID_FREQ_GSM_900_BAND;
      gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.inv_f_dl_lo = INV_F_DL_LO_GSM_900_BAND;
    break;
  }

  return ( midband_freq_mhz );
}



/*===========================================================================

FUNCTION gl1_hw_set_active_cycle_swallower_adj

DESCRIPTION
  Calculates the total freq error and inv freq for DCO update by GFW.
  Then sets trigger flag for pickup change at the end of the
  ISR of this frame. Trigger flag avoids multiple updates in a frame.

DEPENDENCIES
  None

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_set_active_cycle_swallower_adj( ARFCN_T arfcn, gas_id_t gas_id )
{
  int32 freq_err;

  if(afc_mode == GL1_HW_ACQ_GSM_XOCAL)
  {
    freq_err = 0;
  }
  else
  {
    freq_err = gl1_hw_get_xo_acc_freq_err(gas_id);
  }

  /* Prevent updates whilst values are changing */
  gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.valid = FALSE;

  /* convert to hz Q6 (based on mid-band frequency) */
  gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.freq_error_hz_q6 = ( ( freq_err * gl1_hw_gfw_dco_freq( arfcn,gas_id ) ) >> 4 );

  gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.valid = TRUE;
  gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.update = TRUE;
}

/*===========================================================================

FUNCTION gl1_hw_gfw_config_cycle_swallower

DESCRIPTION
  If frequency update is required then send frequency update command to MDSP
DEPENDENCIES
  None

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_gfw_config_cycle_swallower( gas_id_t gas_id )
{
   if ( gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.update &&
        gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.valid )
   {
      gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.update = FALSE;

      mdsp_afc_update_cmd( gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.freq_error_hz_q6 , gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.inv_f_dl_lo  ,gas_id);

      MSG_GERAN_MED_2_G("AFC update to GFW %d XO, %d Hz",
               gl1_hw_get_xo_acc_freq_err(gas_id),
               (gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.freq_error_hz_q6 >> 6) );
   }
}
/*===========================================================================

FUNCTION gl1_hw_get_freq_error_hz_q6/inv_f_dl_lo

DESCRIPTION
  Returns the value of freq_error_hz_q6 and inv_f_dl_lo.
  These are used by MDSP for frequency correction.

DEPENDENCIES
  None

SIDE EFFECTS
  Updates values in structure
===========================================================================*/
boolean gl1_hw_get_mdsp_freq_config_data( gl1_hw_mdsp_freq_t *freq_config_data,gas_id_t gas_id )
{
  freq_config_data->freq_error_hz_q6 = gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.freq_error_hz_q6;
  freq_config_data->inv_f_dl_lo = gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.inv_f_dl_lo;
  return ( gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.valid );
}

/*===========================================================================

FUNCTION  gl1_hw_is_arfcn_xo_desense

DESCRIPTION
  Local wrapper for message layer function.

===========================================================================*/
mdsp_rf_desense_chan_type gl1_hw_is_arfcn_xo_desense( ARFCN_T arfcn,gas_id_t gas_id )
{
  return ( gl1_msg_is_arfcn_xo_desense( arfcn,gas_id ) );
}


/*===========================================================================
FUNCTION gl1_hw_pass_xmsi_to_mdsp

DESCRIPTION
  Returns none
===========================================================================*/
void gl1_hw_pass_xmsi_to_mdsp (boolean imsi_valid,  uint8 *imsi_ptr,
                               boolean tmsi_valid,  uint8 *tmsi_ptr,
                               boolean ptmsi_valid, uint8 *ptmsi_ptr,
                               gas_id_t gas_id)
{
    mdsp_copy_xmsi_data(imsi_valid, imsi_ptr,
                        tmsi_valid, tmsi_ptr,
                        ptmsi_valid, ptmsi_ptr ,gas_id);
}

/*===========================================================================
FUNCTION gl1_hw_sbd_get_null_pattern

DESCRIPTION
  Returns uint16
===========================================================================*/
uint16 gl1_hw_sbd_get_null_pattern (gas_id_t gas_id)
{
    return(mdsp_sbd_read_pattern_used(gas_id));
}

/*===========================================================================

FUNCTION gl1_hw_efs_read_mdsp_debug

DESCRIPTION
  EFS general l1 debug parameter with GFW

===========================================================================*/
void gl1_hw_efs_read_mdsp_debug (gas_id_t gas_id)
{
    int efs_error = geran_efs_read_primary(GL1_EFS_MDSP_DEBUG,&gl1_efs_mdsp_debug,sizeof(gl1_efs_mdsp_debug));

    MSG_GERAN_MED_2_G("GL1:EFS MDSP debug %d %d",efs_error,gl1_efs_mdsp_debug);

    if(efs_error == -1)
    {
        gl1_efs_mdsp_debug = 0;
    }
}

/*===========================================================================

FUNCTION gl1_hw_efs_read_mdsp_diag1

DESCRIPTION
  EFS general l1 debug parameter with GFW

===========================================================================*/
void gl1_hw_efs_read_mdsp_diag1 (gas_id_t gas_id)
{
    int efs_error = geran_efs_read_primary(GL1_EFS_MDSP_DIAG1,&gl1_efs_mdsp_diag1,sizeof(gl1_efs_mdsp_diag1));

    MSG_GERAN_HIGH_2_G("GL1:EFS MDSP diaggroup1 %d %d",efs_error,gl1_efs_mdsp_diag1);
  
    if(efs_error == -1)
      {
        gl1_efs_mdsp_diag1 = 0;
      }
}

/*===========================================================================

FUNCTION gl1_hw_efs_read_mdsp_diag2

DESCRIPTION
  EFS general l1 debug parameter with GFW

===========================================================================*/
void gl1_hw_efs_read_mdsp_diag2 (gas_id_t gas_id)
{
    int efs_error = geran_efs_read_primary(GL1_EFS_MDSP_DIAG2,&gl1_efs_mdsp_diag2,sizeof(gl1_efs_mdsp_diag2));

    MSG_GERAN_HIGH_2_G("GL1:EFS MDSP diaggroup2 %d %d",efs_error,gl1_efs_mdsp_diag2);

    if(efs_error == -1)
      {
        gl1_efs_mdsp_diag2 = 0;
      }
}

/*===========================================================================

FUNCTION gl1_hw_efs_read_mdsp_diag3

DESCRIPTION
  EFS general l1 debug parameter with GFW

===========================================================================*/
void gl1_hw_efs_read_mdsp_diag3 (gas_id_t gas_id)
{
    int efs_error = geran_efs_read_primary(GL1_EFS_MDSP_DIAG3,&gl1_efs_mdsp_diag3,sizeof(gl1_efs_mdsp_diag3));

    MSG_GERAN_HIGH_2_G("GL1:EFS MDSP diaggroup3 %d %d",efs_error,gl1_efs_mdsp_diag3);

    if(efs_error == -1)
      {
        gl1_efs_mdsp_diag3 = 0;
      }
}

/*===========================================================================

FUNCTION gl1_hw_efs_read_mdsp_diag4

DESCRIPTION
  EFS general l1 debug parameter with GFW

===========================================================================*/
void gl1_hw_efs_read_mdsp_diag4 (gas_id_t gas_id)
{
    int efs_error = geran_efs_read_primary(GL1_EFS_MDSP_DIAG4,&gl1_efs_mdsp_diag4,sizeof(gl1_efs_mdsp_diag4));

    MSG_GERAN_HIGH_2_G("GL1:EFS MDSP diaggroup4 %d %d",efs_error,gl1_efs_mdsp_diag4);

    if(efs_error == -1)
      {
        gl1_efs_mdsp_diag4 = 0;
      }
}



/*===========================================================================

FUNCTION gl1_hw_efs_read_debug

DESCRIPTION
  EFS general l1 debug parameter

===========================================================================*/
void gl1_hw_efs_read_debug (void)
{
#ifdef GL1_HW_EFS_DEBUG_DISABLE
    int efs_error = ( -1 );
#else
    int efs_error = geran_efs_read_primary(GL1_EFS_DEBUG,&gl1_efs_debug,sizeof(gl1_efs_debug));
#endif /* GL1_HW_EFS_DEBUG_DISABLE */

    MSG_GERAN_MED_2("GL1:EFS debug %d %d",efs_error,gl1_efs_debug);

    if( efs_error == ( -1 ) )
    {
        gl1_efs_debug= 0;
    }
}

/*===========================================================================

FUNCTION gl1_hw_efs_read_slp

DESCRIPTION
  EFS general l1 sleep debug parameter

===========================================================================*/
void gl1_hw_efs_read_slp (void)
{
#ifdef GL1_HW_EFS_DEBUG_DISABLE
    int efs_error = ( -1 );
    int efs_slp_error_rec_error=(-1);
#else
    int efs_error = geran_efs_read_primary(GL1_EFS_SLP, &gl1_efs_slp, sizeof(gl1_efs_slp));
	int efs_slp_error_rec_error =geran_efs_read_primary( GL1_EFS_SLP_ERROR_REC, &gl1_efs_slp_error_rec, sizeof(gl1_efs_slp_error_rec));
	
#endif /* GL1_HW_EFS_DEBUG_DISABLE */
    if( efs_error == ( -1 ) )
    {
        gl1_efs_slp = 0;
    }

  if (efs_slp_error_rec_error==(-1))
   {
     gl1_efs_slp_error_rec=0;
    }
    MSG_GERAN_MED_2("GL1:EFS sleep %d, gl1_efs_slp=0x%X",efs_error, gl1_efs_slp);
}

/*===========================================================================

FUNCTION gl1_hw_efs_get_debug

DESCRIPTION
  EFS general l1 debug parameter

===========================================================================*/
uint32 gl1_hw_efs_get_debug ( gas_id_t gas_id )
{
  /* Check for gas_id match if not always disabled */
  if ( GL1_EFS_DEBUG_GAS_ID_N( gas_id ) & gl1_efs_debug )
  {
    return ( gl1_efs_debug );
  }
  else
  {
    return ( 0 );
  }
}


/*===========================================================================

FUNCTION gl1_hw_efs_get_slp

DESCRIPTION
  EFS general l1 sleep parameter

===========================================================================*/
uint32 gl1_hw_efs_get_slp ( void )
{
  /* We don't want any gas id chk as its available for all subs */
  return ( gl1_efs_slp );
}

uint32 gl1_hw_efs_get_slp_error_rec ( void )
{
  /* We don't want any gas id chk as its available for all subs */

  MSG_GERAN_MED_1("EFS DEBUG  gl1_efs_slp_error_rec=0x%X",gl1_efs_slp_error_rec);
  
  return (  gl1_efs_slp_error_rec );
}


/*===========================================================================

FUNCTION gl1_hw_delay

DESCRIPTION
  GL1 delay mechanism

===========================================================================*/
#define DELAY_CLK_BUSY                  20
#define DELAY_MIN                       100

void gl1_hw_delay (uint16 delay_us,gas_id_t gas_id)
{
    if(delay_us < DELAY_MIN)
    {
        DALSYS_BusyWait(delay_us);
    }
    else
    {
        uint16 max_pot_delay_count=0;
        uint16 max_pot_delay = (delay_us / DELAY_CLK_BUSY);
        max_pot_delay++;

        while((max_pot_delay_count++) < max_pot_delay)
        {
            DALSYS_BusyWait(DELAY_CLK_BUSY);
        }
    }
}

/*===========================================================================

FUNCTION gl1_hw_sleep_reset_mdsp_intf

DESCRIPTION
  Clears out the mdsp interface when we enter sleep to ensure no pending
  commands remain when in MSIM mode. Avoids any issues with incomplete
  command requests being actioned by the firmware when we wakeup for the
  next G activity (could be the other sub).

  This is mainly seen when executing the sleep activity in MSIM after the
  asynch interrupt processing, but caould occur anytime we perform the
  asynch int handling.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_sleep_reset_mdsp_intf( gas_id_t gas_id )
{
  boolean mdsp_asynch_int_ebld = gl1_hw_get_mdsp_asynch_int( gas_id );

  if ( gl1_asynch_int_dbg( gas_id ) )
  {
    MSG_GERAN_HIGH_2_G( "gl1_hw_sleep_reset_mdsp_intf FN:%d Mdsp_asynch_int:%d",
                        GSTMR_GET_FN_GERAN( gas_id ), mdsp_asynch_int_ebld );
  }

  /* Always clear to avoid left when Task sleep completes after Asynch */
  if ( mdsp_asynch_int_ebld )
  {
    gl1_hw_set_mdsp_asynch_int( FALSE, gas_id );
  }

  /* Clear out any pending RX/SBI activity in the command interface */
  mdsp_sleep_reset_mdsp_intf( gas_id );
}

/*===========================================================================

FUNCTION gl1_hw_set_mdsp_asynch_int

DESCRIPTION
  Set/clear the asynch PCH mdsp int trigger, and register the frame
  handler(s) for the frame after the asynch int is received/processed.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_set_mdsp_asynch_int( boolean asynch_int_enable,gas_id_t gas_id )
{
  mdsp_set_asynch_int_enabled( asynch_int_enable  ,gas_id);
}

/*===========================================================================

FUNCTION gl1_hw_get_mdsp_asynch_int

DESCRIPTION
  Returns the asynch PCH mdsp int trigger.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_hw_get_mdsp_asynch_int( gas_id_t gas_id )
{
  return ( mdsp_asynch_dsp_int_enabled( gas_id ) );
}

/*===========================================================================

FUNCTION gl1_hw_is_asynch_int_possible

DESCRIPTION
  Returns the decision if the asynch PCH mdsp int should even be enabled.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/

/*
 * How many symbols into the frame below which we can execute the asynch int
 * safely. This has to take into account the MAX_SYMBOL_COUNT_TO_SLEEP +
 * ASYNCH_INT_MAX_SYMBOL_COUNT_TO_SLEEP_INCREASE (950)
 * that is defined in gl1_hw_sleep_ctl.c and the time it takes to execute the
 * pseudo-frame interrupt (500 qsymbols typically).
 * If we use a faster CPU speed on PCH then this will be higher as the time
 * to execute the frame tick will be less giving a greater margin. Typically
 * the Firmware issues the asynch interrupt about 2100 qsymbols (BO.2.0 update:
 * typically 1700 or 4000) into the frame(although this may increase slightly
 * with extra monitors) which allows plenty of time to execute and make the
 * sleep decision.
 */
#define GSMTR_ASYNCH_INT_MAX_QS_CNT 4250
#define GSMTR_ASYNCH_INT_MIN_QS_CNT QS_PER_SLOT

boolean gl1_hw_is_asynch_int_possible( gas_id_t gas_id )
{
  uint32  start_qsym_cnt         = 0;
  boolean is_asynch_int_possible = FALSE;

  gl1_hw_sleep_type  *gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];

  /* Check awake first, otherwise as vtsmr deregistered get crash when getting qsymbol count */
  if (gl1_hw_get_sleep_state(gas_id) == GL1_HW_SLEEP_AWAKE)
  {
    start_qsym_cnt = gstmr_rd_qsymbol_count_geran( geran_map_gas_id_to_nas_id( gas_id ) );

    /* Make sure do not have a pending gstmr and we are not too close
     * to the next real gstmr interrupt to avoid updating hw/sw counts
     * just before a real gstmr comes in. We need to have disabled SAIC
     * as well to allow for enough time to execute the pseudo-frame tick
     * and execute the sleep code.
     * Also make sure no active WCDMA search is active as we will try
     * and action this after the PCH receive which we will cancel in
     * processing the asynch int.
     * So that the timeline is sufficient then we need to be in the reduced
     * power monitors state.
     * Another check is required if the FEE is active as no need to attempt
     * the asynch int processing as no opportunity for sleep and this will just
     * consume more current needlessly.
     * Last check is to make sure we don't have any activity already
     * booked into the DRX manager that would cause the asynch to fail. */
    if ( ( gl1_is_asynch_sleep_enabled( gas_id ) || gl1_is_sleep_opt3_enabled( gas_id ) )
         && !gstmr_is_int_pending(gas_id)
         && ( start_qsym_cnt < GSMTR_ASYNCH_INT_MAX_QS_CNT )
         && ( start_qsym_cnt > GSMTR_ASYNCH_INT_MIN_QS_CNT )
         && !gstmr_get_asynch_int_executing(gas_id)
         && gl1_msg_is_asynch_int_possible(gas_id)
         && gl1_drx_asynch_int_sleep_possible_precheck(gas_id)
         && gl1_hw_get_idle_mode(gas_id)
         && !gl1_hw_tx_on[gas_id]
         && !gl1_msg_get_rat_change_active( gas_id )
         && !L1_background_HPLMN_search_in_progress( gas_id )
       )
    {
      is_asynch_int_possible = TRUE;
    }

    gl1_hw_sleep_cycle->async_msg.async_sleep           = is_asynch_int_possible;
    gl1_hw_sleep_cycle->async_msg.async_NV_poss         = gl1_is_asynch_sleep_enabled( gas_id );
    gl1_hw_sleep_cycle->async_msg.async_option3_NV_poss = gl1_is_sleep_opt3_enabled( gas_id );
    gl1_hw_sleep_cycle->async_msg.start_ustmr           = slpc_get_tick();
    gl1_hw_sleep_cycle->async_msg.frame_number          = gl1_get_FN(gas_id);
    gl1_hw_sleep_cycle->async_msg.start_qsc             = start_qsym_cnt;
    gl1_hw_sleep_cycle->async_msg.async_exec            = gstmr_get_asynch_int_executing(gas_id);
    gl1_hw_sleep_cycle->async_msg.async_poss            = gl1_msg_is_asynch_int_possible( gas_id );
    gl1_hw_sleep_cycle->async_msg.precheck              = gl1_drx_asynch_int_sleep_possible_precheck( gas_id );
    gl1_hw_sleep_cycle->async_msg.int_pend              = gstmr_is_int_pending( gas_id );
    gl1_hw_sleep_cycle->async_msg.irat_active           = gl1_msg_get_rat_change_active( gas_id );
    gl1_hw_sleep_cycle->async_msg.tx_on                 = gl1_hw_tx_on[gas_id];
#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
    MSG_GERAN_HIGH_1_G( "Async interrupt flag %d",gl1_hw_sleep_cycle->async_msg.async_sleep );
#endif

    if ( gl1_asynch_int_dbg( gas_id ) )
    {
      MSG_GERAN_HIGH_2_G( "GSTMR_is_asynch_int_poss qs_cnt:%d gstmr_asynch_int_executing:%d",
                          start_qsym_cnt, gstmr_get_asynch_int_executing( gas_id ) );
      if ( !gl1_is_asynch_sleep_enabled( gas_id ) )
      {
        MSG_GERAN_HIGH_3_G( "is_asynch_int_possible:%d msg_is_asynch_poss:%d gl1_hw_tx_on:%d",
                            is_asynch_int_possible, gl1_msg_is_asynch_int_possible( gas_id ),
                            gl1_hw_tx_on[gas_id] );
      }
      else
      {
        MSG_GERAN_HIGH_4_G( "sleep_possible_precheck:%d is_asynch_int_possible:%d msg_is_asynch_poss:%d gl1_hw_tx_on:%d",
        gl1_drx_asynch_int_sleep_possible_precheck( gas_id ), is_asynch_int_possible,
        gl1_msg_is_asynch_int_possible( gas_id ),gl1_hw_tx_on[gas_id]);
      }
    }
  }
  else
  {
#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
    MSG_GERAN_ERROR_0_G("Received mdsp asynch PCH int while sleep_state is not GL1_HW_SLEEP_AWAKE");
#endif
  }
  return ( is_asynch_int_possible );
}

/*===========================================================================

FUNCTION gl1_hw_get_gsm_rxlm_buf_idx

DESCRIPTION
  Returns None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
uint32 gl1_hw_get_gsm_rxlm_buf_idx( gas_id_t gas_id )
{
   return(gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx);
}

/*===========================================================================

FUNCTION gl1_hw_set_gsm_rxlm_buf_idx

DESCRIPTION
  Returns None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_set_gsm_rxlm_buf_idx( uint32 gl1_gsm_rxlm_buf_idx ,gas_id_t gas_id )
{
  gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx = gl1_gsm_rxlm_buf_idx ;
  MSG_GERAN_LOW_1_G("GSM RxLM buffer %d allocated",gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx);
}

/*===========================================================================

FUNCTION gl1_hw_deallocate_rxlm_buf_idx

DESCRIPTION
  Returns None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_deallocate_rxlm_buf_idx( gas_id_t gas_id )
{
   if (gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx != MDSP_RXLM_BUF_IDX_INVALID)
   {
      if (!rflm_deallocate_rx_handle(LM_GSM, LM_GSM, gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx))
      {
        MSG_GERAN_ERROR_1_G("Unable to de-allocate RxLM buffer %d",gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx);
      }
      else
      {
        MSG_GERAN_LOW_1_G("GSM RxLM buffer %d deallocated",gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx);
        gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx = MDSP_RXLM_BUF_IDX_INVALID;
      }
   }
#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_hw_deallocate_rxlm_buf_idx_DivRx( gas_id );
#endif /*FEATURE_GSM_RX_DIVERSITY*/
}

/*===========================================================================

FUNCTION gl1_hw_deallocate_txlm_buf_idx

DESCRIPTION
  Returns None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_deallocate_txlm_buf_idx(gas_id_t gas_id)
{
   if (gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx != MDSP_TXLM_BUF_IDX_INVALID)
   {
     if (!rflm_deallocate_tx_handle(LM_GSM, LM_GSM, gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx))
     {
       MSG_GERAN_ERROR_1_G("Unable to de-allocate TxLM buffer %d",gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx);
     }
     else
     {
       MSG_GERAN_LOW_1_G("GSM TxLM buffer %d deallocated",gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx);
       gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx = MDSP_TXLM_BUF_IDX_INVALID;
     }
   }
}

/*===========================================================================

FUNCTION gl1_hw_set_rf_gsm_rx_times

DESCRIPTION
  Sets RF Rx timing, used for QTA/IRAT

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_set_rf_gsm_rx_time(int32 rx_setup_time, int32 idle_rx_setup_time, gas_id_t gas_id)
{
   gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info.rx_rf_setup_time_qs = rx_setup_time;
   gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info.mon_rf_setup_time_qs = rx_setup_time;
   gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info.rx_idle_rf_setup_time_qs = idle_rx_setup_time;
   gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info.mon_idle_rf_setup_time_qs = idle_rx_setup_time;
}

/*===========================================================================

FUNCTION gl1_hw_get_rf_gsm_rx_setup_time

DESCRIPTION
  Returns Rx Rf set-up time

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_hw_get_rf_gsm_rx_setup_time( gas_id_t gas_id )
{

#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
  if((l1_tsk_buffer[gas_id].l1_state == L1_MULTI_SIM_IDLE ||
    l1_tsk_buffer[gas_id].l1_state == L1_MULTI_SIM_FIND_BCCH) &&
     gl1_drdsds_enabled(gas_id))
  {
     return (gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info.rx_idle_rf_setup_time_qs);
  }
#endif

  return(gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info.rx_rf_setup_time_qs);
}
/*===========================================================================

FUNCTION gl1_hw_get_rf_gsm_mon_setup_time

DESCRIPTION
  Returns Monitor Rf set-up time

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_hw_get_rf_gsm_mon_setup_time( gas_id_t gas_id )
{
#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
  if( ((l1_tsk_buffer[gas_id].l1_state == L1_MULTI_SIM_IDLE) ||
       (l1_tsk_buffer[gas_id].l1_state == L1_MULTI_SIM_FIND_BCCH)) &&
      gl1_drdsds_enabled(gas_id) )
  {
     return (gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info.mon_idle_rf_setup_time_qs);
  }
#endif
  return(gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info.mon_rf_setup_time_qs);
}

/*===========================================================================

FUNCTION gl1_hw_get_rf_gsm_tx_setup_time

DESCRIPTION
  Returns Tx Rf set-up time

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_hw_get_rf_gsm_tx_setup_time( gas_id_t gas_id )
{
  return(gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info.tx_rf_setup_time_qs);
}

#ifdef FEATURE_VSWR 
/*===========================================================================

FUNCTION gl1_hw_get_rf_gsm_tx_vswr_setup_time

DESCRIPTION
  Returns Tx Rf set-up time

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_hw_get_rf_gsm_tx_vswr_setup_time( gas_id_t gas_id )
{
  return(gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info.tx_rf_setup_time_vswr_qs );
}
#endif /*FEATURE_VSWR*/

/*===========================================================================

FUNCTION gl1_hw_get_rf_gsm_warmup_time

DESCRIPTION
  Returns Tx Rf warm up time

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
uint32 gl1_hw_get_rf_gsm_warmup_time( gas_id_t gas_id )
{
  uint32 rf_warmup_time = (uint32)gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info.rf_warmup_time_qs;

  /* If a bad warmup received then substitute with a worst case */
  if ( rf_warmup_time < GL1_HW_SAFE_RF_WARMUP_TIME )
  {
    MSG_GERAN_ERROR_2_G("SLEEP:Invalid rf_warmup_time %d using %d instead",
               rf_warmup_time, GL1_HW_SAFE_RF_WARMUP_TIME);

    rf_warmup_time = GL1_HW_SAFE_RF_WARMUP_TIME;
  }

  return ( rf_warmup_time );
}

#ifdef GERAN_L1_IP2CAL
/*===========================================================================

FUNCTION gl1_hw_get_rf_gsm_ip2cal_setup_time

DESCRIPTION
  Returns IP2 cal set up time

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_hw_get_rf_gsm_ip2cal_setup_time( gas_id_t gas_id )
{
  return(gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info.ip2_cal_setup_time_qs);
}
#endif


/*===========================================================================

FUNCTION gl1_hw_cm_x2g_startup_cmd_duration_qs

DESCRIPTION
  Returns the W2G startup command duration in quarter symbols.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
uint16  gl1_hw_cm_x2g_startup_cmd_duration_qs( sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  return  (rf_meas_x2g_irat_info_param[gas_id].start_duration);
}


/*===========================================================================

FUNCTION gl1_hw_cm_x2g_power_meas_cmd_overhead_qs

DESCRIPTION
  Returns the W2G power measurement command overhead in quarter symbols.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
uint16  gl1_hw_cm_x2g_power_meas_cmd_overhead_qs( sys_modem_as_id_e_type as_id  )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  return  (rf_meas_x2g_irat_info_param[gas_id].power_meas_setup_duration);
}

/*===========================================================================

FUNCTION gl1_hw_cm_x2g_power_meas_cmd_overhead_qs

DESCRIPTION
  Returns the W2G power measurement command overhead in quarter symbols.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
uint16  gl1_hw_cm_x2g_power_meas_cmd_long_overhead_qs( sys_modem_as_id_e_type as_id  )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  return  (rf_meas_x2g_irat_info_param[gas_id].power_meas_alpha_rffe);
}

/*===========================================================================

FUNCTION gl1_hw_cm_x2g_stop_cmd_duration_qs

DESCRIPTION
  Returns the duration between X2G RF STOP trigger and CCS release
  in quarter symbols.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
uint16  gl1_hw_cm_x2g_stop_cmd_duration_qs( sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  return  (rf_meas_x2g_irat_info_param[gas_id].power_meas_gamma);
}


/*===========================================================================

FUNCTION gl1_hw_cm_x2g_cleanup_cmd_duration_qs

DESCRIPTION
  Returns the W2G cleanup command duration in quarter symbols.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
uint16  gl1_hw_cm_x2g_cleanup_cmd_duration_qs( sys_modem_as_id_e_type as_id )
{
    gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
	return  (rf_meas_x2g_irat_info_param[gas_id].cleanup_duration + GFW_X2G_CLEANUP_OVERHEAD);
}

/*===========================================================================

FUNCTION gl1_hw_cm_x2g_wpll_time_us

DESCRIPTION
  Returns the W2G W PLL settling time in us

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
uint16  gl1_hw_cm_x2g_wpll_time_us( sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  return  (rf_meas_x2g_irat_info_param[gas_id].pll_settling_time);
}

/*===========================================================================

FUNCTION gl1_hw_has_reset_occurred

DESCRIPTION
  Returns whether GL1 has run a panic reset.
  Used by FTM code to detect if a panic reset has occurred

===========================================================================*/
boolean gl1_hw_has_reset_occurred (gas_id_t gas_id)
{
  return gl1_hw_data_ptr[gas_id]->gl1_panic_reset_occurred;
  }


/*===========================================================================

FUNCTION  gl1_hw_rf_init_rfa_task_msgr_mailbox

DESCRIPTION
  Create a message router mailbox for receiving blocking RFA Task CNF messages.
  The mailbox will be used for receiving any CNF messages from msgr_send() commands
  which require GL1 task to block until the command has been completed
  e.g. rfm_enter_mode().

===========================================================================*/
errno_enum_type gl1_hw_rf_init_rfa_task_msgr_mailbox(gas_id_t gas_id)
{
  errno_enum_type res = msgr_client_create(&gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client);

  if( res != E_SUCCESS)
  {
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_0_G(" msgr_client_create Failure");
  }

  res = msgr_client_add_mq(
         "GSM L1 Common RFA Task Mailbox",
         &gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
         (uint8)GSM_L1_COMMON_RFA_TASK_PRIORITY,
         (uint32)GSM_L1_COMMON_RFA_TASK_DEPTH,
         (uint32)sizeof(u_geran_rf_task_msgr),       // ideally this should be a common CNF message size
         &gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msg_id) ;


  if( res != E_SUCCESS)
  {
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_0_G(" msgr_client_add_mq Failure");
  }

  /* for dsda mode gas1 queue for legacy rfa mgr, gas2 queue for sub2 rfa mgr (device 2)
   * for MSIM mode gas1 and gas2 queue for legacy msgr No usage of sub2 rfa msgr
   * for x+g build. legacy msgr and sub2 msgr need registering to gas1
   */
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    res = msgr_register_block(MSGR_RFA_RF_GSM,
         &gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
         gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msg_id,
         gsm_l1_common_rfa_task_umid_list,
         (uint32)GSM_L1_COMMON_RFA_TASK_DIAG_CNT);
  }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
 res = msgr_register_block(MSGR_RFA_RF_GSM,
           &gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
           gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msg_id,
           gsm_l1_common_rfa_task_umid_list_dev_2,
           (uint32)GSM_L1_COMMON_RFA_TASK_DIAG_CNT_DEV_2);
  }
#if defined ( FEATURE_TRIPLE_SIM )
  else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
  {
    res = msgr_register_block(MSGR_RFA_RF_GSM,
           &gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
           gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msg_id,
           gsm_l1_common_rfa_task_umid_list_dev_3,
           (uint32)GSM_L1_COMMON_RFA_TASK_DIAG_CNT_DEV_3);
  }
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */


  if( res != E_SUCCESS)
  {
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_0_G(" msgr_register_block Failure");
  }


  return res;
}
#ifndef FEATURE_GSM_RX_TX_SPLIT
/*===========================================================================

FUNCTION  gl1_hw_rf_task_enter_mode

DESCRIPTION
  Send MSGR message to RF Task to run GSM enter mode. Use MSGR blocking wait
  to receive enter mode completion confimation from RF Task before continuing

===========================================================================*/
static void gl1_hw_rf_task_enter_mode(uint32 rxlm_buf_idx, rfm_device_enum_type  device,gas_id_t gas_id)
{
  int16 index = 0;

#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    uint32 use_idle_rx_alpha = (uint32)FALSE;
#endif

  gas_id = check_gas_id(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
   /*
         DR-DSDS: in DR mode idle mode RX alpha must be used to cope with RXFE bus contention 
         issues. Ensure value is correct before calling Rf enter mode.
     */
   if (((l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE) || 
        (l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH)) && 
        gl1_drdsds_enabled(gas_id))
   {
        use_idle_rx_alpha = TRUE;
        
   }
   /*indicate to GFW correct RX alpha to use*/
   mdsp_update_shared_variable(GFW_RX_ALPHA,&use_idle_rx_alpha,gas_id);   
     
#endif

  GDRV_RFA_MSGR_LOCK(gas_id);
  gl1_hw_rf_task_enter_mode_req(rxlm_buf_idx, device, gas_id);
  gl1_hw_rf_task_enter_mode_cnf(gas_id);
  GDRV_RFA_MSGR_UNLOCK(gas_id);

  /* Initialise current GSM band as INVALID on initialising RF */
  gl1_hw_set_invalid_rf_gsm_band(gas_id);

  /* Initialise RF Task Rx burst buffers */
  for (index=0; index<GL1_HW_RF_BURST_RX_BUF_NUM; index++)
  {
    gl1_hw_data_ptr[gas_id]->gl1_hw_rf_rx_burst_buf[index].num_rx_bursts = GL1_HW_RF_BURST_NUM_BURSTS_INVALID;
  }
  /* Initialise RF Task Tx burst buffers */
  for (index=0; index<GL1_HW_RF_BURST_TX_BUF_NUM; index++)
  {
    gl1_hw_data_ptr[gas_id]->gl1_hw_rf_tx_burst_buf[index].num_tx_slots = GL1_HW_RF_BURST_NUM_BURSTS_INVALID;
  }
}

/*===========================================================================

FUNCTION  gl1_hw_rf_task_enter_mode_req

DESCRIPTION
  Send MSGR message to RF Task to run GSM enter mode. This function does
  not block to wait the confirmation from RF

===========================================================================*/
void gl1_hw_rf_task_enter_mode_req(uint32 rxlm_buf_idx,
                                   rfm_device_enum_type device,
                                   gas_id_t gas_id)
{
  rfa_rf_gsm_enter_mode_req_s rf_gsm_enter_mode_req;
  errno_enum_type result;
  msgr_umid_type rfa_umid_to_send = NULL;
  msgr_umid_type rfa_rf_gsm_enter_mode_req[]     = {RFA_RF_GSM_ENTER_MODE_REQ,
                                                    RFA_RF_GSM_ENTER_MODE_SUB2_REQ
#ifdef FEATURE_TRIPLE_SIM
                                                   ,RFA_RF_GSM_ENTER_MODE_SUB3_REQ
#endif /* FEATURE_TRIPLE_SIM */
                                                   };
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  rfm_device_enum_type divrx_rf_device = RFM_MAX_WAN_DEVICES;
  grm_client_enum_t divrx_client = GRM_NO_CLIENT;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = NULL;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

  gas_id = check_gas_id(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  divrx_client =  grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);
  /*No grant: no need to wakeup*/
  if(grm_get_trm_status(divrx_client, gas_id) == GL1_TRM_GRANTED)
  {
    divrx_rf_device = gl1_hw_get_gsm_rf_id(divrx_client, gas_id);
  }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

    rfa_umid_to_send = rfa_rf_gsm_enter_mode_req[gas_id];

    /* initialise msgr header */
    msgr_init_hdr(&rf_gsm_enter_mode_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);

    /* populate payload */
    rf_gsm_enter_mode_req.init_writes_buff_ptr = (void*)mdsp_get_gfw_init_event_buffer(gas_id);

    rf_gsm_enter_mode_req.timing_info = &gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info;
    rf_gsm_enter_mode_req.buffer_id = 0;
    rf_gsm_enter_mode_req.sub_id = gas_id;
    rf_gsm_enter_mode_req.cnf_required = TRUE;

    rf_gsm_enter_mode_req.rxlm_buf_index.prx_idx = (lm_handle_type)rxlm_buf_idx;
    rf_gsm_enter_mode_req.prx_device = device;

    rf_gsm_enter_mode_req.rxlm_buf_index.drx_idx = (uint32)(-1);
    rf_gsm_enter_mode_req.drx_device = RFM_INVALID_DEVICE;
    rf_gsm_enter_mode_req.enter_mode = RFGSM_PRX_ENTER_MODE;

#ifdef FEATURE_GSM_RX_DIVERSITY
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  MSG_GERAN_HIGH_3_G(GL1_RXD"@IMRD_ENTER  IMRD/RXd enabled=%d, TRM rfdevIMRD=%d, IMRD_rxlm_buff=%d",
                     gl1_get_IMRD_ctl_nv(gas_id),
                     divrx_rf_device,
                     gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id));
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
  if(divrx_rf_device < RFM_MAX_WAN_DEVICES)
    {
      if(MDSP_RXLM_BUF_IDX_INVALID == gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id))
      {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
      MSG_GERAN_HIGH_0(GL1_RXD"@IMRD_ENTER DRX RXLM is INVALID; Trying to reallocate it. ");
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG    */
    if(gl1_alloc_IMRD_rxdiv_rxlm_buff_idx(gas_id))
        {
          rf_gsm_enter_mode_req.rxlm_buf_index.drx_idx = (lm_handle_type)(gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id));
        rf_gsm_enter_mode_req.drx_device = divrx_rf_device;
          rf_gsm_enter_mode_req.enter_mode = RFGSM_PRX_DRX_ENTER_MODE;

        gl1_hw_set_rf_enter_status(RFGSM_PRX_DRX_ENTER_MODE, gas_id);
        gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_WAKEUP_MODE, gas_id);
        }
        else
        {
        MSG_GERAN_ERROR_0_G(GL1_RXD"@IMRD_ENTER CANNOT ALLOCATE DRX RXLM BUFF IDX ");
          rf_gsm_enter_mode_req.rxlm_buf_index.drx_idx = (uint32)(-1);
          rf_gsm_enter_mode_req.drx_device = RFM_INVALID_DEVICE;
          rf_gsm_enter_mode_req.enter_mode = RFGSM_PRX_ENTER_MODE;

        gl1_hw_set_rf_enter_status(RFGSM_PRX_ENTER_MODE, gas_id);
        gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_SLEEP_MODE, gas_id);
        }
      }
      else
      {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
      MSG_GERAN_HIGH_0_G(GL1_RXD"@IMRD_ENTER DRX RXLM buff is already alocated, using it.");
#endif
        rf_gsm_enter_mode_req.rxlm_buf_index.drx_idx = (lm_handle_type)(gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id));
      rf_gsm_enter_mode_req.drx_device = divrx_rf_device;
        rf_gsm_enter_mode_req.enter_mode = RFGSM_PRX_DRX_ENTER_MODE;

      gl1_hw_set_rf_enter_status(RFGSM_PRX_DRX_ENTER_MODE, gas_id);
      gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_WAKEUP_MODE, gas_id);
      }
    }
    else
    {
      rf_gsm_enter_mode_req.rxlm_buf_index.drx_idx = (uint32)(-1);
      rf_gsm_enter_mode_req.drx_device = RFM_INVALID_DEVICE;
      rf_gsm_enter_mode_req.enter_mode = RFGSM_PRX_ENTER_MODE;

    gl1_hw_set_rf_enter_status(RFGSM_PRX_ENTER_MODE, gas_id);
    gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_SLEEP_MODE, gas_id);
    }
  MSG_GERAN_HIGH_5_G("rf enter_mode_req prx_idx=%i prx_dev=%i drx_idx=%i .drx_device=%i, "
                     "enter_mode=%i",
                     rf_gsm_enter_mode_req.rxlm_buf_index.prx_idx,
                     rf_gsm_enter_mode_req.prx_device,
                     rf_gsm_enter_mode_req.rxlm_buf_index.drx_idx,
                     rf_gsm_enter_mode_req.drx_device,
                     rf_gsm_enter_mode_req.enter_mode);
#endif /* FEATURE_GSM_RX_DIVERSITY*/
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
	  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

    /* send msgr message to RF Task */
    result = msgr_send(&rf_gsm_enter_mode_req.hdr, sizeof(rf_gsm_enter_mode_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_rf_task_enter_mode_cnf

DESCRIPTION
  Wait for the response from RF Task to GSM enter mode. Use MSGR blocking wait
  to receive enter mode completion confimation from RF Task before continuing

===========================================================================*/
void gl1_hw_rf_task_enter_mode_cnf(gas_id_t gas_id)
{
  rfa_rf_gsm_enter_mode_cnf_s rf_gsm_enter_mode_cnf;

  errno_enum_type result;
  uint32 bytes_recvd;
  msgr_umid_type rfa_umid_to_rcv = NULL;
#ifdef GERAN_L1_HLLL_LNASTATE
  uint8 i = 0;
  rfcom_device_enum_type device;
#endif

  msgr_umid_type rfa_rf_gsm_enter_mode_cnf[] = {RFA_RF_GSM_ENTER_MODE_CNF,
                                                RFA_RF_GSM_ENTER_MODE_SUB2_CNF
#ifdef FEATURE_TRIPLE_SIM
                                               ,RFA_RF_GSM_ENTER_MODE_SUB3_CNF
#endif /* FEATURE_TRIPLE_SIM */
                                               };

    rfa_umid_to_rcv = rfa_rf_gsm_enter_mode_cnf[gas_id];

    result = msgr_receive(&gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
        (uint8 *)&rf_gsm_enter_mode_cnf,
        (uint32)sizeof(rfa_rf_gsm_enter_mode_cnf_s),
        &bytes_recvd);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
/*  Good to have for now, until it gets stable enough, check the cnf result feedback from RF. Not mature enough to rely on them and make any decisions here yet  */
if(ftm_get_mode() != FTM_MODE)
  {
    MSG_GERAN_MED_3("RF enter_mode_cnf: prx_result=%i, drx_result=%i, req_result=%i ",
      rf_gsm_enter_mode_cnf.prx_device_req_result,
      rf_gsm_enter_mode_cnf.drx_device_req_result,
                    rf_gsm_enter_mode_cnf.req_result);
  }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

#ifdef GERAN_L1_IP2CAL
    if(ftm_get_mode() == FTM_MODE)
      {
      gl1_hw_data_ptr[gas_id]->ip2cal_supported = rf_gsm_enter_mode_cnf.ip2_cal_supported;
      MSG_GERAN_HIGH_1_G("ip2cal_supported:%d",gl1_hw_data_ptr[gas_id]->ip2cal_supported);
      }
#endif /*GERAN_L1_IP2CAL*/

    /* sanity check and print debug for msgr_receive */
    gl1_hw_rf_msgr_receive_debug(rfa_umid_to_rcv,
        result,
        rf_gsm_enter_mode_cnf.hdr.id,
        rf_gsm_enter_mode_cnf.req_result,
        gas_id);

#ifdef GERAN_L1_HLLL_LNASTATE
  for(device=RFM_DEVICE_0;device<RFM_MAX_WAN_DEVICES;device++)
   {
    for(i=0;i<(RFCOM_NUM_GSM_BANDS-1);i++)
  {
     gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] |= (rf_gsm_enter_mode_cnf.sawless_sup[device][i] << i);
    }

    if (gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] != 0)
    {
      MSG_GERAN_HIGH_2_G("Sawless support for band:0x%x for device:%d",
                         gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device],
                        device);
    }
  }
#endif /*GERAN_L1_HLLL_LNASTATE*/
}
#endif /* FEATURE_GSM_RX_TX_SPLIT */
/*===========================================================================

FUNCTION  gl1_hw_rf_wakeup

DESCRIPTION
  Send MSGR message to RF Task to run RF Wakeup. Use MSGR blocking wait
  to receive wakeup completion confimation from RF Task before continuing

===========================================================================*/
void gl1_hw_rf_wakeup(uint32 rxlm_buf_idx,
                      gl1_hw_wakeup_mode_type wakeup_mode,
                      gas_id_t gas_id)
{
  switch (wakeup_mode)
  {
    case GL1_PRX_WAKEUP_MODE:
    case GL1_PRX_DRX_WAKEUP_MODE:
      gl1_hw_combined_or_prx_rf_wakeup(rxlm_buf_idx, gas_id);
      gl1_hw_sleep_set_rf_cnf_pending(TRUE, gas_id);
      break;

#ifdef FEATURE_GSM_RX_DIVERSITY
    case GL1_DRX_WAKEUP_MODE:
      gl1_hw_rf_wakeup_divrx(gas_id);
      gl1_hw_sleep_set_rf_cnf_pending(TRUE, gas_id);
      break;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

    case GL1_INVALID_WAKEUP_MODE:
    default:
     MSG_GERAN_ERROR_1_G("Invalid RF wakeup Mode %d", wakeup_mode);
      break;
      }
  return;
  }

#ifndef FEATURE_GSM_RX_TX_SPLIT
/*===========================================================================

FUNCTION  gl1_hw_combined_or_prx_rf_wakeup

DESCRIPTION
  Send MSGR message to RF Task to run RF Wakeup. Use MSGR blocking wait
  to receive wakeup completion confimation from RF Task before continuing

===========================================================================*/
static void gl1_hw_combined_or_prx_rf_wakeup(uint32 rxlm_buf_idx,
                                             gas_id_t gas_id)
{
  rfa_rf_gsm_wakeup_req_s rf_gsm_wakeup_req;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  errno_enum_type result;
  msgr_umid_type rfa_umid_to_send = NULL;

#ifdef FEATURE_GSM_RX_DIVERSITY
  rfm_device_enum_type divrx_rf_device = RFM_MAX_WAN_DEVICES;
  grm_client_enum_t divrx_client = GRM_NO_CLIENT;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

  gas_id = check_gas_id(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
  divrx_client =  grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);

  /*No grant: no need to wakeup*/
  if(grm_get_trm_status(divrx_client, gas_id) == GL1_TRM_GRANTED)
  {
    divrx_rf_device = gl1_hw_get_gsm_rf_id(divrx_client, gas_id);
  }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_WAKEUP_REQ;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_WAKEUP_SUB2_REQ;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
   {
    rfa_umid_to_send = RFA_RF_GSM_WAKEUP_SUB3_REQ;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initial wakeup msgr header */
  msgr_init_hdr(&rf_gsm_wakeup_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */

  rf_gsm_wakeup_req.sbi_init_writes_buff_ptr = (void*)mdsp_get_gfw_init_event_buffer(gas_id);

  rf_gsm_wakeup_req.buffer_id = 0;
  rf_gsm_wakeup_req.sub_id = gas_id;
  rf_gsm_wakeup_req.cnf_required = TRUE;
  rf_gsm_wakeup_req.rxlm_buf_index.prx_idx = (lm_handle_type)rxlm_buf_idx;
  rf_gsm_wakeup_req.prx_device = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);
  rf_gsm_wakeup_req.timing_info = &gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info;

#ifdef FEATURE_GSM_RX_DIVERSITY
  if (divrx_rf_device < RFM_MAX_WAN_DEVICES)
  {
    if (MDSP_RXLM_BUF_IDX_INVALID == gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id))
    {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
      MSG_GERAN_HIGH_0_G(GL1_RXD"@IMRD_WAKEUP DRX RXLM is INVALID; Trying to reallocate it. ");
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
      if (gl1_alloc_IMRD_rxdiv_rxlm_buff_idx(gas_id))
      {
        rf_gsm_wakeup_req.rxlm_buf_index.drx_idx = (lm_handle_type) (gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id));
        rf_gsm_wakeup_req.drx_device = divrx_rf_device;
        rf_gsm_wakeup_req.wakeup_mode = RFGSM_PRX_DRX_WAKEUP_MODE;
        gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_WAKEUP_MODE, gas_id);
      }
      else
      {
        MSG_GERAN_ERROR_0_G(GL1_RXD"@IMRD_WAKEUP  CANNOT ALLOCATE DRX RXLM BUFF IDX ");
        rf_gsm_wakeup_req.rxlm_buf_index.drx_idx = (uint32) (-1);
        rf_gsm_wakeup_req.drx_device = RFM_INVALID_DEVICE;
        rf_gsm_wakeup_req.wakeup_mode = RFGSM_PRX_WAKEUP_MODE;
        gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_SLEEP_MODE, gas_id);
      }
    }
    else
    {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
      MSG_GERAN_HIGH_0_G(GL1_RXD"@IMRD_WAKEUP DRX RXLM buff is already alocated, using it.");
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/

      rf_gsm_wakeup_req.rxlm_buf_index.drx_idx = (lm_handle_type) (gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id));
      rf_gsm_wakeup_req.drx_device = divrx_rf_device;
      rf_gsm_wakeup_req.wakeup_mode = RFGSM_PRX_DRX_WAKEUP_MODE;
      gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_WAKEUP_MODE, gas_id);
    }
  }
  else
  {
    rf_gsm_wakeup_req.rxlm_buf_index.drx_idx = (uint32) (-1);
    rf_gsm_wakeup_req.drx_device = RFM_INVALID_DEVICE;
    rf_gsm_wakeup_req.wakeup_mode = RFGSM_PRX_WAKEUP_MODE;
    gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_SLEEP_MODE, gas_id);
  }
  MSG_GERAN_HIGH_5_G(GL1_SLEEP_HDR"rf_gsm_wakeup_req prx_idx=%i prx_device=%i drx_idx=%i drx_device=%i, wakeup_mode=%i",
                     rf_gsm_wakeup_req.rxlm_buf_index.prx_idx,
                     rf_gsm_wakeup_req.prx_device,
                     rf_gsm_wakeup_req.rxlm_buf_index.drx_idx,
                     rf_gsm_wakeup_req.drx_device,
      rf_gsm_wakeup_req.wakeup_mode);
#else
  rf_gsm_wakeup_req.rxlm_buf_index.drx_idx = (uint32)(-1);
  rf_gsm_wakeup_req.drx_device = RFM_INVALID_DEVICE;
  rf_gsm_wakeup_req.wakeup_mode = RFGSM_PRX_WAKEUP_MODE;
#endif /* FEATURE_GSM_RX_DIVERSITY*/

  GDRV_RFA_MSGR_LOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
	#error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

  /* send msgr message to RF Task */
  result = msgr_send(&rf_gsm_wakeup_req.hdr, sizeof(rf_gsm_wakeup_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);
  /* Tracks if the wake up request was sent in the first place or not */
  gl1_hw_set_rf_wakeup_req_msg(gas_id, ((result == E_SUCCESS) ? TRUE : FALSE));

  if ((gl1_hw_sleep_get_in_warmup_check(gas_id)==TRUE) && (gl1_hw_is_forced_wakeup(gas_id)==FALSE))
  {
  gl1_hw_sleep_set_commit_mask(RF_WAKEUP_COMMIT, gas_id);
  gl1_hw_rf_result_monitor_timer_start(XCCH_BLOCK_COMMIT_TIMEOUT,gas_id );
  }

  GDRV_RFA_MSGR_UNLOCK(gas_id);

}

/*===========================================================================

FUNCTION  gl1_hw_rf_sleep

DESCRIPTION
  Send MSGR message to RF Task to run RF Sleep. Use MSGR blocking wait
  to receive wakeup completion confimation from RF Task before continuing

===========================================================================*/
void gl1_hw_rf_sleep(uint32 rxlm_buf_idx, gl1_hw_sleep_mode_type sleep_mode, gas_id_t gas_id)
{
  rfa_rf_gsm_sleep_req_s rf_gsm_sleep_req;
  rfa_rf_gsm_sleep_cnf_s rf_gsm_sleep_cnf;
  errno_enum_type result;
  uint32 bytes_recvd;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  msgr_umid_type rfa_umid_to_send = NULL;
  msgr_umid_type rfa_umid_to_rcv = NULL;

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  rfm_device_enum_type divrx_rf_device = RFM_MAX_WAN_DEVICES;
  grm_client_enum_t divrx_client = GRM_NO_CLIENT;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

  gas_id = check_gas_id(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
  /*No grant: no need to wakeup*/
  if(gl1_hw_get_rf_sleep_wakeup_mode(gas_id) == GL1_RF_PRX_WAKEUP_DIVRX_WAKEUP_MODE)
  {
    divrx_client = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);
    divrx_rf_device = gl1_hw_get_gsm_rf_id(divrx_client,gas_id);
  }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_SLEEP_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_SLEEP_CNF;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_SLEEP_SUB2_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_SLEEP_SUB2_CNF;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
   {
     rfa_umid_to_send = RFA_RF_GSM_SLEEP_SUB3_REQ;
     rfa_umid_to_rcv  = RFA_RF_GSM_SLEEP_SUB3_CNF;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initial wakeup msgr header */
  msgr_init_hdr(&rf_gsm_sleep_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */

  rf_gsm_sleep_req.buffer_id = 0;
  rf_gsm_sleep_req.sub_id = gas_id;
  rf_gsm_sleep_req.cnf_required = TRUE;

  switch (sleep_mode)
  {
    case GL1_DRX_SLEEP_MODE:
      rf_gsm_sleep_req.prx_device = RFM_INVALID_DEVICE;
      rf_gsm_sleep_req.drx_device = divrx_rf_device;
      rf_gsm_sleep_req.sleep_mode = RFGSM_DRX_SLEEP_MODE;
      gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_SLEEP_MODE, gas_id);
      break;
    case GL1_PRX_DRX_SLEEP_MODE:
    case GL1_PRX_SLEEP_MODE:
      rf_gsm_sleep_req.prx_device = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY
      if (divrx_rf_device < RFM_MAX_WAN_DEVICES)
  {
        rf_gsm_sleep_req.drx_device = divrx_rf_device;
    rf_gsm_sleep_req.sleep_mode = RFGSM_PRX_DRX_SLEEP_MODE;
  }
  else
  {
    rf_gsm_sleep_req.drx_device = RFM_INVALID_DEVICE;
    rf_gsm_sleep_req.sleep_mode = RFGSM_PRX_SLEEP_MODE;
  }
      gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_SLEEP_DIVRX_SLEEP_MODE, gas_id);
      MSG_GERAN_HIGH_3_G("rf_gsm_sleep_req prx_device=%i drx_device=%i sleep_mode=%i",
                         rf_gsm_sleep_req.prx_device,
                         rf_gsm_sleep_req.drx_device,
                         rf_gsm_sleep_req.sleep_mode);
#else
  rf_gsm_sleep_req.drx_device = RFM_INVALID_DEVICE;
  rf_gsm_sleep_req.sleep_mode = RFGSM_PRX_SLEEP_MODE;
#endif /* FEATURE_GSM_RX_DIVERSITY */
      break;
    default:
      break;
  }

  GDRV_RFA_MSGR_LOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rf_gsm_sleep_req.hdr, sizeof(rf_gsm_sleep_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

    /******* TODO: IF IN MSIM Mode we need to always check GAS ID 1, There will be no registration of
      ****** Device 0 CNF UMID's on gas id 2
      *****/
  /* blocking wait for TX Enable CNF message from RF Task */
    result = msgr_receive(&gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
                        (uint8 *)&rf_gsm_sleep_cnf,
                        (uint32)sizeof(rfa_rf_gsm_sleep_cnf_s),
                        &bytes_recvd);
  GDRV_RFA_MSGR_UNLOCK(gas_id);


  /* sanity check and print debug for msgr_receive */
  gl1_hw_rf_msgr_receive_debug(rfa_umid_to_rcv,
                               result,
                               rf_gsm_sleep_cnf.hdr.id,
                               rf_gsm_sleep_cnf.req_result,
                               gas_id);

  if ( gl1_msg_get_multi_sim_sys_mode() != SYS_MODEM_DEVICE_MODE_SINGLE_SIM )
  {
    if (rf_gsm_rx_sleep_req.prx_device != RFM_INVALID_DEVICE) 
    {
      gl1_hw_set_gsm_rf_id(RFM_INVALID_DEVICE,
                         l1_tskisr_blk->client_id,
                         TRUE, 
                         gas_id);
}

    if ((rf_gsm_rx_sleep_req.drx_device != RFM_INVALID_DEVICE)
        && (divrx_client != GRM_NO_CLIENT))
    {
      gl1_hw_set_gsm_rf_id(RFM_INVALID_DEVICE,
                         divrx_client,
                         TRUE, 
                         gas_id);
    }
  }
  return;
}
#endif /* FEATURE_GSM_RX_TX_SPLIT */

/*===========================================================================

FUNCTION gl1_hw_rf_enable_tx

DESCRIPTION
  Send MSGR message to RF Task to run GSM Tx enable. Use MSGR blocking wait
  to receive enter mode completion confimation from RF Task before continuing

===========================================================================*/
static void gl1_hw_rf_enable_tx(uint32 txlm_buf_idx,gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#ifdef FEATURE_GSM_RX_TX_SPLIT
  gl1_hw_rf_tx_wakeup(txlm_buf_idx,
                      gl1_hw_get_gsm_rf_id(grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id), gas_id),
                               gas_id);
#else
  gl1_hw_rf_enable_tx_device(txlm_buf_idx,
                             gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id),
                             gas_id);
#endif
}

#ifndef FEATURE_GSM_RX_TX_SPLIT
/*===========================================================================

FUNCTION gl1_hw_rf_enable_tx_device

DESCRIPTION
  Send MSGR message to RF Task to run GSM Tx enable. Use MSGR blocking wait
  to receive enter mode completion confimation from RF Task before continuing.
  Take rf device as a parameter

===========================================================================*/
void gl1_hw_rf_enable_tx_device(uint32 txlm_buf_idx, rfm_device_enum_type device, gas_id_t gas_id)
{
  rfa_rf_gsm_tx_enable_req_s rfa_rf_gsm_tx_enable_req;
  errno_enum_type result;

  msgr_umid_type rfa_umid_to_send = NULL;

  gas_id = check_gas_id(gas_id);
  /* Force set Tx band on next Tx band request */
  gl1_hw_set_invalid_rf_gsm_band(gas_id);
  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_TX_ENABLE_REQ;

  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_TX_ENABLE_SUB2_REQ;

  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
   {
     rfa_umid_to_send = RFA_RF_GSM_TX_ENABLE_SUB3_REQ;

  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initialise msgr header */
  msgr_init_hdr(&rfa_rf_gsm_tx_enable_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */
  rfa_rf_gsm_tx_enable_req.txlm_buf_idx = (lm_handle_type)txlm_buf_idx;
  rfa_rf_gsm_tx_enable_req.device = device;
  rfa_rf_gsm_tx_enable_req.buffer_id = 0;
  rfa_rf_gsm_tx_enable_req.sub_id = gas_id;
  rfa_rf_gsm_tx_enable_req.cnf_required = TRUE;
  rfa_rf_gsm_tx_enable_req.therm_read_buff_ptr = (void*)mdsp_get_gfw_therm_read_event_buffer(gas_id);

#ifdef FEATURE_GSM_RX_TX_SPLIT
  rfa_rf_gsm_tx_enable_req.tx_enter_mode_buff_ptr = (void*)mdsp_get_gfw_init_event_buffer(gas_id);
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
	  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

  /* send msgr message to RF Task */
  result = msgr_send(&rfa_rf_gsm_tx_enable_req.hdr, sizeof(rfa_rf_gsm_tx_enable_req));

  GDRV_RFA_MSGR_LOCK(gas_id);
  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

  GDRV_RFA_MSGR_UNLOCK(gas_id);

}

/*===========================================================================

FUNCTION gl1_hw_rf_disable_tx

DESCRIPTION
  Send MSGR message to RF Task to run GSM Tx disable. Use MSGR blocking wait
  to receive enter mode completion confimation from RF Task before continuing

===========================================================================*/
void gl1_hw_rf_disable_tx(uint32 txlm_buf_idx, rfm_device_enum_type device, gas_id_t gas_id)
{
  rfa_rf_gsm_tx_disable_req_s rfa_rf_gsm_tx_disable_req;
  errno_enum_type result;
  rfa_rf_gsm_tx_disable_cnf_s rf_gsm_tx_disable_cnf;
  uint32 bytes_recvd;
  msgr_umid_type rfa_umid_to_send = NULL;
  msgr_umid_type rfa_umid_to_rcv = NULL;

  gas_id = check_gas_id(gas_id);

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_TX_DISABLE_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_TX_DISABLE_CNF;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_TX_DISABLE_SUB2_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_TX_DISABLE_SUB2_CNF;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
   {
    rfa_umid_to_send = RFA_RF_GSM_TX_DISABLE_SUB3_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_TX_DISABLE_SUB3_CNF;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initialise msgr header */
  msgr_init_hdr(&rfa_rf_gsm_tx_disable_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */
  rfa_rf_gsm_tx_disable_req.txlm_buf_idx = (lm_handle_type)txlm_buf_idx;
  rfa_rf_gsm_tx_disable_req.device = device;
  rfa_rf_gsm_tx_disable_req.buffer_id = 0;
  rfa_rf_gsm_tx_disable_req.sub_id = gas_id;
  rfa_rf_gsm_tx_disable_req.cnf_required = TRUE;

  GDRV_RFA_MSGR_LOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
	#error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rfa_rf_gsm_tx_disable_req.hdr, sizeof(rfa_rf_gsm_tx_disable_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

    /******* TODO: IF IN MSIM Mode we need to always check GAS ID 1, There will be no registration of
      ****** Device 0 CNF UMID's on gas id 2
      *****/
  /* blocking wait for TX Enable CNF message from RF Task */
    result = msgr_receive(&gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
                        (uint8 *)&rf_gsm_tx_disable_cnf,
                        (uint32)sizeof(rfa_rf_gsm_tx_disable_cnf_s),
                        &bytes_recvd);
  GDRV_RFA_MSGR_UNLOCK(gas_id);

  /* sanity check and print debug for msgr_receive */
  gl1_hw_rf_msgr_receive_debug(rfa_umid_to_rcv,
                               result,
                               rf_gsm_tx_disable_cnf.hdr.id,
                               rf_gsm_tx_disable_cnf.req_result,
                               gas_id);

  }

/*===========================================================================

FUNCTION  gl1_hw_rf_task_exit_mode

DESCRIPTION
  Send MSGR message to RF Task to run GSM exit mode. Use MSGR blocking wait
  to receive exit mode completion confimation from RF Task before continuing

===========================================================================*/
void gl1_hw_rf_task_exit_mode (
                  gas_id_t gas_id,
                  rfgsm_exit_mode_type exit_mode,
                  rfm_device_enum_type gsm_rf_id
                )
{
  rfa_rf_gsm_exit_mode_req_s  rf_gsm_exit_mode_req;
  rfa_rf_gsm_exit_mode_cnf_s  rf_gsm_exit_mode_cnf;
  errno_enum_type result;
  uint32 bytes_recvd;
  msgr_umid_type rfa_umid_to_send = NULL;
  msgr_umid_type rfa_umid_to_rcv = NULL;

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = NULL;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

  gas_id = check_gas_id(gas_id);

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_EXIT_MODE_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_EXIT_MODE_CNF;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_EXIT_MODE_SUB2_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_EXIT_MODE_SUB2_CNF;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
   {
     rfa_umid_to_send = RFA_RF_GSM_EXIT_MODE_SUB3_REQ;
     rfa_umid_to_rcv  = RFA_RF_GSM_EXIT_MODE_SUB3_CNF;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initialise msgr header */
  msgr_init_hdr(&rf_gsm_exit_mode_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */

  rf_gsm_exit_mode_req.buffer_id = 0;
  rf_gsm_exit_mode_req.sub_id = gas_id;
  rf_gsm_exit_mode_req.cnf_required = TRUE;
  rf_gsm_exit_mode_req.exit_mode = exit_mode;

  switch(exit_mode)
    {
     case RFGSM_PRX_EXIT_MODE:
    rf_gsm_exit_mode_req.prx_device = (lm_handle_type)gsm_rf_id;
    rf_gsm_exit_mode_req.drx_device = (lm_handle_type)RFM_INVALID_DEVICE;
    break;

     case RFGSM_DRX_EXIT_MODE:
    rf_gsm_exit_mode_req.drx_device = (lm_handle_type)gsm_rf_id;
    rf_gsm_exit_mode_req.prx_device = (lm_handle_type)RFM_INVALID_DEVICE;
    break;

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
   case RFGSM_PRX_DRX_EXIT_MODE:
    l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    rf_gsm_exit_mode_req.prx_device = (lm_handle_type)gsm_rf_id;
    rf_gsm_exit_mode_req.drx_device = (lm_handle_type)gl1_hw_get_gsm_rf_id(
                                            grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id),
                                            gas_id);
    break;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

     default:
    MSG_GERAN_FATAL_1_G("INVALID RF EXIT MODE RECEIVED!!! exit_mode=%i; defaulting it to PRX exit only",exit_mode);
    rf_gsm_exit_mode_req.prx_device= (lm_handle_type)gsm_rf_id;
    rf_gsm_exit_mode_req.drx_device= (lm_handle_type)RFM_INVALID_DEVICE;
    break;
    }
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  MSG_GERAN_HIGH_3_G("rf_gsm_exit_mode_req prx_device=%i, drx_device=%i, exit_mode=%i ",
    rf_gsm_exit_mode_req.prx_device,
    rf_gsm_exit_mode_req.drx_device,
                   exit_mode);
#endif

  GDRV_RFA_MSGR_LOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
      #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rf_gsm_exit_mode_req.hdr, sizeof(rf_gsm_exit_mode_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

  /* blocking wait for exit mode cnf message from RF Task */
  result = msgr_receive(&gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
                        (uint8 *)&rf_gsm_exit_mode_cnf,
                        (uint32)sizeof(rfa_rf_gsm_exit_mode_cnf_s),
                        &bytes_recvd);

  GDRV_RFA_MSGR_UNLOCK(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  if(ftm_get_mode() != FTM_MODE)
    {
    MSG_GERAN_HIGH_4_G("rf_gsm_exit_mode_cnf: prx_device_req_result=%i, "
                       "drx_device_req_result=%i,req_result=%i, exit_mode=%i ",
        rf_gsm_exit_mode_cnf.prx_device_req_result,
        rf_gsm_exit_mode_cnf.drx_device_req_result,
        rf_gsm_exit_mode_cnf.req_result,
                       exit_mode);
    }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/


  /* sanity check and print debug for msgr_receive */
  gl1_hw_rf_msgr_receive_debug(rfa_umid_to_rcv,
                               result,
                               rf_gsm_exit_mode_cnf.hdr.id,
                               rf_gsm_exit_mode_cnf.req_result,
                               gas_id);
}
#endif /* FEATURE_GSM_RX_TX_SPLIT */
/*===========================================================================

FUNCTION  gl1_hw_rf_rx_burst

DESCRIPTION
  Send MSGR message to RF Task for Rx burst processing

===========================================================================*/
void gl1_hw_rf_rx_burst(uint16                    num_rx_bursts,
                        rfgsm_rx_chan_params_type rx_chan_params[]
                        ,rfgsm_rx_diversity_chan_params_type div_rx_chan_params[]
                        ,boolean                  enableRxDiv
                        ,mdsp_burst_mode          mdsp_burst_mode
                        ,gas_id_t                  gas_id
                        )
{
  #ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
  GfwRxEvent *rx_event;
  #endif  
  rfa_rf_gsm_rx_burst_req_s rf_gsm_rx_burst_req;
  errno_enum_type           result;
  msgr_umid_type rfa_umid_to_send = NULL;
  int16 index = 0;
  rfa_rf_gsm_rx_burst_type *rx_burst_ptr;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

  gas_id = check_gas_id(gas_id);

  l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  /* allocate the Rx Burst memory from modem heap - do not use stack, as memory will only get deallocated on receiving
     a MSGR CNF message from RF Task */
  //rx_burst_ptr = (rfa_rf_gsm_rx_burst_type *) GPRS_MEM_MALLOC( sizeof(rfa_rf_gsm_rx_burst_type) );
  rx_burst_ptr = &gl1_hw_data_ptr[gas_id]->gl1_hw_rf_rx_burst_buf[gl1_hw_data_ptr[gas_id]->gl1_hw_rf_rx_burst_buf_index++];
  rf_gsm_rx_burst_req.rx_burst_params    = rx_burst_ptr;

  /* keep index within buffer bounds */
  gl1_hw_data_ptr[gas_id]->gl1_hw_rf_rx_burst_buf_index &= 0x7;

  if(num_rx_bursts > RFA_RF_GSM_MAX_RX_ACTIVITIES)
  {
      MSG_GERAN_ERROR_1_G("Too many Rx burst activities to schedule, num=%d",num_rx_bursts);
      return;
  }

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_RX_BURST_REQ;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_RX_BURST_SUB2_REQ;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
   {
     rfa_umid_to_send = RFA_RF_GSM_RX_BURST_SUB3_REQ;
   }
#endif /* FEATURE_TRIPLE_SIM */

  /* initialise msgr header */
  msgr_init_hdr(&rf_gsm_rx_burst_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */

  rf_gsm_rx_burst_req.buffer_id = mdsp_current_buffer_mod3(gas_id);
  MSG_GERAN_LOW_3_G("RF Rx burst: buffer_id =%d, burst_type=%d num_bursts=%d",rf_gsm_rx_burst_req.buffer_id, mdsp_burst_mode, num_rx_bursts);
  rf_gsm_rx_burst_req.sub_id = gas_id;
  rf_gsm_rx_burst_req.cnf_required = TRUE;

  rf_gsm_rx_burst_req.rx_burst_params->num_rx_bursts = num_rx_bursts;

#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
  /*
     GL1 when in Idle mode should set the flag gl1_lower_rx_ccs_prio as part of rx burst payload parameters. RF based on the flag will lower the CCS
     event priority for RX as well as pass a delay to adjust wtr rffe start time to handle bus contention delay.
     RF will also update two sets of RX alpha (Normal and worst case alpha) when queried by GL1. GL1 should use the worst case alpha for Idle Mode RX
   */
  if (((l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE) ||
       (l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH)) &&
      gl1_drdsds_enabled(gas_id) &&
      (!gl1_hw_qta_gap_active(gas_id))
        /* temporary change since we don't set use_idle_rx_alpha in QTA gap right now */
     )
  {
    rf_gsm_rx_burst_req.rx_burst_params->gl1_lower_rx_ccs_prio = TRUE;
  }
  else
  {
    rf_gsm_rx_burst_req.rx_burst_params->gl1_lower_rx_ccs_prio = FALSE;
  }
#else
  rf_gsm_rx_burst_req.rx_burst_params->gl1_lower_rx_ccs_prio = FALSE;
#endif

#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
  if (mdsp_burst_mode == MON_MODE)
  {
     l1_physical_layer_power_data( NUM_POWER_MONITORS, num_rx_bursts, gas_id);
  } else
  {
     l1_physical_layer_power_data( NUM_ACTIVE_DL_SLOTS, num_rx_bursts, gas_id );
  }

  if (enableRxDiv == TRUE)
  {
     l1_physical_layer_power_data( RXD_STATUS, TRUE, gas_id );
  }
#endif

  for(index=0; index<num_rx_bursts; index++)
  {
    ARFCN_T loc_arfcn;
    loc_arfcn.num = rx_chan_params[index].arfcn;
    loc_arfcn.band = gl1_hw_rfgsm_band_to_sys_band(rx_chan_params[index].band);
    /*
    MSG_GERAN_MED_5_G("ACI_DBG: Mode: %d Arfcn:%d Band:%d in Structure Arfcn:%d Band:%d", mdsp_burst_mode
                        ,rx_chan_params[index].arfcn, gl1_hw_rfgsm_band_to_sys_band(rx_chan_params[index].band)
                        ,loc_arfcn.num, loc_arfcn.band );
    */

    /* copy in chan params - if same RF structure is to be used, then could convert to memcpy */
    /* assume same burst type for all bursts for now i.e. either RX_BURST or MON_BURST */
    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].burst_type = rx_chan_params[index].burst_type;
    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].band       = rx_chan_params[index].band;
    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].arfcn      = rx_chan_params[index].arfcn;
    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].freq_err   = rx_chan_params[index].freq_err;
    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].rx_lvl_dbm = rx_chan_params[index].rx_lvl_dbm;
    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].gain_ptr   = rx_chan_params[index].gain_ptr;
    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].lna_range  = rx_chan_params[index].lna_range;
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].probe_burst = rx_chan_params[index].probe_burst;
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */

#ifdef GERAN_L1_ENHANCED_RX
    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].enh_rx_params.enh_rx_enable_flag = rx_chan_params[index].enh_rx_params.enh_rx_enable_flag;
    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].enh_rx_params.enh_rx_offset_freq = rx_chan_params[index].enh_rx_params.enh_rx_offset_freq;
#endif /*GERAN_L1_ENHANCED_RX*/

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
    MSG_GERAN_HIGH_3_G(GL1_RXD"AGC_LOOP rf_gsm_rx_burst_req DRX burst info for RF:rx_lvl_dbm=%i, gain_ptr=%i,lna_range=%i ",
    div_rx_chan_params[index].rx_lvl_dbm,div_rx_chan_params[index].gain_ptr,div_rx_chan_params[index].lna_range);
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG */
    rf_gsm_rx_burst_req.rx_burst_params->rx_diversity_chan_params[index].rx_lvl_dbm = div_rx_chan_params[index].rx_lvl_dbm;
    rf_gsm_rx_burst_req.rx_burst_params->rx_diversity_chan_params[index].gain_ptr   = div_rx_chan_params[index].gain_ptr;
    rf_gsm_rx_burst_req.rx_burst_params->rx_diversity_chan_params[index].lna_range  = div_rx_chan_params[index].lna_range;
    rf_gsm_rx_burst_req.rx_burst_params->rx_diversity_chan_params[index].probe_burst= div_rx_chan_params[index].probe_burst;
    rf_gsm_rx_burst_req.rx_burst_params->gfw_rf_burst_event[index] = (void*)mdsp_get_gfw_event_buffer(mdsp_burst_mode,index, gas_id);
    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].rx_burst_type = (rfgsm_rx_burst_type) mdsp_burst_mode;
#ifdef GERAN_L1_HLLL_LNASTATE
    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].high_lin_mode_active = rx_chan_params[index].high_lin_mode_active;
#endif /*GERAN_L1_HLLL_LNASTATE*/
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
		rx_event = (GfwRxEvent *)(rf_gsm_rx_burst_req.rx_burst_params->gfw_rf_burst_event[index]);
		if ( rx_event != NULL )
		{
		  rx_event->ccsEvent.rflmStartData = 0xFFFFFFFF;
		  rx_event->ccsEvent.rflmStopData = 0xFFFFFFFF;
		}
#endif

    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].rf_sequence_number = rx_chan_params[index].rf_sequence_number;

    switch (mdsp_burst_mode)
    {
      /*For Monitors , No need to supply ACI type to GRF as HL Switchpoint will be used in this case 
        For Acquisition HL Switch points needs to be used in RF So Set aci to ACI_400 */
      case MON_MODE:
        rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].aci_flag = FALSE; 
        break;
      case ACQ_MODE:
        if ( gl1_hw_cm_status[gas_id].initialised == TRUE)
	  	{
	  	  if(rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].rx_lvl_dbm < (-94*16))
	  	  {
	  	    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].aci_flag = FALSE;
	  	  }
		  else
		  {
		    rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].aci_flag = TRUE;
		  }
	  	}
		else
		{
          rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].aci_flag = TRUE; 
		} 
        break;
      default:
        /*Update the ACI type*/
        rf_gsm_rx_burst_req.rx_burst_params->rx_chan_params[index].aci_flag = gl1_hw_get_aci_fr_arfcn(loc_arfcn, gas_id);
        break;
  }

  }

  {
      if ( gl1_hw_cm_status[gas_id].initialised == TRUE
#ifdef FEATURE_QTA
           || (l1_tskisr_blk->qta_in_progress)
#endif
         )
      {
         /* For the sub which is performing QTA or IRAT right now, RF device number is stored
            in cm structure.
         */
         rf_gsm_rx_burst_req.device = gl1_hw_cm_get_trm_rf_device(gas_id);
      }
      else
      {
          rf_gsm_rx_burst_req.device = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);
      }

#ifdef FEATURE_GSM_RX_DIVERSITY
     rf_gsm_rx_burst_req.rx_diversity_device = gl1_hw_get_gsm_rf_id(
                                          grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id),
                                          gas_id);
     rf_gsm_rx_burst_req.enable_rx_diversity = enableRxDiv;
#else
     rf_gsm_rx_burst_req.rx_diversity_device = RFM_INVALID_DEVICE;
     rf_gsm_rx_burst_req.enable_rx_diversity = FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY*/
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
      #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
      /* send msgr message to RF Task */
      result = msgr_send(&rf_gsm_rx_burst_req.hdr, sizeof(rf_gsm_rx_burst_req));

      /* sanity check and print debug for msgr_send */
      gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);
  }
}

/*===========================================================================

FUNCTION  gl1_hw_rf_set_tx_band

DESCRIPTION
  Send MSGR message to RF Task for Set Tx Band

===========================================================================*/
void gl1_hw_rf_set_tx_band(rfgsm_band_type band,gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  if(band != gl1_hw_data_ptr[gas_id]->gl1_hw_rf_gsm_band)
  {
    MSG_GERAN_HIGH_2_G("GL1 band change request: new band=%d; prev band=%d",band, gl1_hw_data_ptr[gas_id]->gl1_hw_rf_gsm_band);

    gl1_set_tx_band_cmd_iss_to_gfw[gas_id] = FALSE;

    gl1_hw_rf_set_tx_band_device(band,
#ifdef FEATURE_GSM_RX_TX_SPLIT
                                 gl1_hw_get_gsm_rf_id(grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id), gas_id),
#else
                                 gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id,gas_id),
#endif
                                 gas_id);
  }
}

/*===========================================================================

FUNCTION  gl1_hw_rf_set_tx_band_device

DESCRIPTION
  Send MSGR message to RF Task for Set Tx Band. Take the RF device as a
  parameter and does not check if the band was changed

===========================================================================*/
void gl1_hw_rf_set_tx_band_device(rfgsm_band_type band, rfcom_device_enum_type device, gas_id_t gas_id)
{
  rfa_rf_gsm_set_tx_band_req_s rfa_rf_gsm_set_tx_band_req;
  errno_enum_type              result;
  msgr_umid_type rfa_umid_to_send = NULL;

  gas_id = check_gas_id(gas_id);

  /* Update band so set Tx band only sent for band change */
  gl1_hw_data_ptr[gas_id]->gl1_hw_rf_gsm_band = band;

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_SET_TX_BAND_REQ;
  }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_SET_TX_BAND_SUB2_REQ;
  }
#if defined ( FEATURE_TRIPLE_SIM )
  else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
  {
    rfa_umid_to_send = RFA_RF_GSM_SET_TX_BAND_SUB3_REQ;
  }
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */


  GL1_HANDLER_PERFORMANCE_LOGGING(START_TX_RF_SET_BAND,gas_id);
  /* initialise msgr header */
  msgr_init_hdr(&rfa_rf_gsm_set_tx_band_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */
  rfa_rf_gsm_set_tx_band_req.device = device;
  rfa_rf_gsm_set_tx_band_req.band = band;
  rfa_rf_gsm_set_tx_band_req.sbi_set_tx_band_buff_ptr = (void*)mdsp_get_gfw_tx_band_event_buffer(gas_id);
  rfa_rf_gsm_set_tx_band_req.buffer_id = 0;
  rfa_rf_gsm_set_tx_band_req.sub_id = gas_id;
  rfa_rf_gsm_set_tx_band_req.cnf_required = TRUE;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rfa_rf_gsm_set_tx_band_req.hdr, sizeof(rfa_rf_gsm_set_tx_band_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_get_rf_gsm_band

DESCRIPTION
  Returns the current RF band

===========================================================================*/
int8 gl1_hw_get_rf_gsm_band(gas_id_t gas_id)
{
  return gl1_hw_data_ptr[gas_id]->gl1_hw_rf_gsm_band;
}

/*===========================================================================

FUNCTION  gl1_hw_set_invalid_rf_gsm_band

DESCRIPTION
  Sets gsm RF band to none, so force set tx band

===========================================================================*/
void gl1_hw_set_invalid_rf_gsm_band(gas_id_t gas_id)
{
   gl1_hw_data_ptr[gas_id]->gl1_hw_rf_gsm_band = (int8) RFGSM_BAND_NONE;
}

/*===========================================================================

FUNCTION  gl1_hw_rf_tx_burst

DESCRIPTION
  Send MSGR message to RF Task for Tx Burst processing

===========================================================================*/
boolean    tx_blnk_cnt[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
void gl1_hw_rf_tx_burst(uint8                    num_assigned,
                        uint8                    num_slots,
                        rfgsm_band_type          band,
                        uint16                   arfcn,
                        int32                    freq_error,
                        uint16                   pa_index[GL1_DEFS_MAX_ASSIGNED_UL_TS],
                        gl1_defs_modulation_type mod[GL1_DEFS_MAX_ASSIGNED_UL_TS],
                        int16                    coex_backoff_desense[GL1_DEFS_MAX_ASSIGNED_UL_TS],
                        int16                    coex_backoff_vbatt[GL1_DEFS_MAX_ASSIGNED_UL_TS],
                        uint32                   rf_seq_num,
                        gas_id_t                 gas_id)
{
  #ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY   
  GfwTxEvent *tx_event;
  #endif
  rfa_rf_gsm_tx_burst_req_s rfa_rf_gsm_tx_burst_req;
  errno_enum_type           result;
  uint16                    slot;
  msgr_umid_type rfa_umid_to_send = NULL;
  rfa_rf_gsm_tx_burst_type *tx_burst_ptr = NULL;
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  gas_id = check_gas_id(gas_id);

  /* allocate the Rx Burst memory from modem heap - do not use stack, as memory will only get deallocated on receiving
     a MSGR CNF message from RF Task */
  //rfa_rf_gsm_tx_burst_type *tx_burst_ptr = (rfa_rf_gsm_tx_burst_type *) GPRS_MEM_MALLOC( sizeof(rfa_rf_gsm_tx_burst_type) );
  tx_burst_ptr  = &gl1_hw_data_ptr[gas_id]->gl1_hw_rf_tx_burst_buf[gl1_hw_data_ptr[gas_id]->gl1_hw_rf_tx_burst_buf_index++];
  rfa_rf_gsm_tx_burst_req.tx_burst_params = tx_burst_ptr;
  /* keep index within buffer bounds */
  gl1_hw_data_ptr[gas_id]->gl1_hw_rf_tx_burst_buf_index &= 0x3;

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_TX_BURST_REQ;
  }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_TX_BURST_SUB2_REQ;
  }
#if defined ( FEATURE_TRIPLE_SIM )
  else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
  {
    rfa_umid_to_send = RFA_RF_GSM_TX_BURST_SUB3_REQ;
  }
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
  l1_physical_layer_power_data( NUM_ACTIVE_UL_SLOTS, num_slots, gas_id);
#endif
  /* initialise msgr header */
  msgr_init_hdr(&rfa_rf_gsm_tx_burst_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */
#ifdef FEATURE_GSM_RX_TX_SPLIT
  rfa_rf_gsm_tx_burst_req.device = gl1_hw_get_gsm_rf_id(grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id), gas_id);
#else
  rfa_rf_gsm_tx_burst_req.device = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);
#endif
  rfa_rf_gsm_tx_burst_req.buffer_id = mdsp_current_buffer_mod3(gas_id);
  rfa_rf_gsm_tx_burst_req.sub_id = gas_id;
  rfa_rf_gsm_tx_burst_req.cnf_required = TRUE;
  /* populate frame based info*/
  rfa_rf_gsm_tx_burst_req.tx_burst_params->num_tx_slots              = num_slots;
#ifdef FEATURE_GSM_QSC_TX_DIV
  //rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_blanking               = tx_blnk_cnt[gas_id];
#endif /* FEATURE_GSM_QSC_TX_DIV */
  rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_chan_params.band       = band;
  rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_chan_params.arfcn      = arfcn;
  rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_chan_params.freq_err   = freq_error;
  rfa_rf_gsm_tx_burst_req.tx_burst_params->num_assigned_uplink_slots = num_assigned;
  /* populate slot based info*/
  for(slot=0; slot<num_slots; slot++)
  {
#ifdef FEATURE_VSWR   
    if ((gl1_vswr_meas_requested [gas_id]) && (slot == 0))	  
	 {
		 gl_hw_use_extended_tx_alpha [gas_id] = TRUE ; 
	 }
#endif /*FEATURE_VSWR*/
    rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_pwr_ctl_params[slot].power_level = pa_index[slot];
    rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_pwr_ctl_params[slot].modulation  = (rfgsm_modulation_type)rfgsm_modulation_lut[mod[slot]];
    rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_pwr_ctl_params[slot].modulation  = (rfgsm_modulation_type)rfgsm_modulation_lut[mod[slot]];
#ifdef FEATURE_GSM_COEX
    rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_pwr_ctl_params[slot].backoff[RFGSM_COEX_DESENSE_BACKOFF] = coex_backoff_desense[slot];
    rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_pwr_ctl_params[slot].backoff[RFGSM_COEX_VBATT_BACKOFF] = coex_backoff_vbatt[slot];
#else
    rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_pwr_ctl_params[slot].backoff[RFGSM_COEX_DESENSE_BACKOFF] = 0;
    rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_pwr_ctl_params[slot].backoff[RFGSM_COEX_VBATT_BACKOFF] = 0;
#endif
  }
  rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_chan_params.rf_sequence_number = rf_seq_num;
  rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_burst_event_ptr = mdsp_get_gfw_event_buffer(TX_MODE, TX_BURST, gas_id);
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY   
	tx_event = (GfwTxEvent *)(rfa_rf_gsm_tx_burst_req.tx_burst_params->tx_burst_event_ptr);
	if ( tx_event != NULL )
	{
	  tx_event->start.rflmStart = 0xFFFFFFFF;
	  tx_event->stop.rflmStop = 0xFFFFFFFF;
	}
#endif  

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rfa_rf_gsm_tx_burst_req.hdr, sizeof(rfa_rf_gsm_tx_burst_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

}

/*===========================================================================

FUNCTION  gl1_hw_rf_idle_frame_procesing

DESCRIPTION
  Send MSGR message to RF Task to run any RF idle fraem processing

===========================================================================*/
void gl1_hw_rf_idle_frame_processing(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  rfa_rf_gsm_idle_frame_processing_req_s rfa_rf_gsm_idle_frame_processing_req;
  errno_enum_type result;
  msgr_umid_type rfa_umid_to_send = NULL;


  gas_id = check_gas_id(gas_id);

  if (gl1_msg_get_multi_sim_mode())
  {
    if(TRM_DENIAL == grm_get_trm_grant_status(l1_tskisr_blk->client_id, gas_id))
    {
      MSG_GERAN_ERROR_2_G("gl1_hw_rf_idle_frame_processing: Blocked since client:%d does not have TRM lock, MM mode:%d",
                          (l1_tskisr_blk->client_id),gl1_msg_get_multi_sim_mode());
      return;
    }
  }

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_IDLE_FRAME_PROCESSING_REQ;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_IDLE_FRAME_PROCESSING_SUB2_REQ;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
  else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
  {
    rfa_umid_to_send = RFA_RF_GSM_IDLE_FRAME_PROCESSING_SUB3_REQ;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initialise msgr header */
  msgr_init_hdr(&rfa_rf_gsm_idle_frame_processing_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload  - there is currently no payload for this message */
#ifdef FEATURE_GSM_RX_TX_SPLIT
  rfa_rf_gsm_idle_frame_processing_req.device = gl1_hw_get_gsm_rf_id(grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id), gas_id);
#else
  rfa_rf_gsm_idle_frame_processing_req.device = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);
#endif

  if( RFM_INVALID_DEVICE ==  rfa_rf_gsm_idle_frame_processing_req.device )
{
    /* Don't send the idle frame processing request to RF till a valid RF device is available */
     MSG_GERAN_ERROR_0_G("Not sending RF GSM IDLE FRAME PROCESSING - device invalid");
	 return;
  }

  rfa_rf_gsm_idle_frame_processing_req.sub_id = gas_id;
  rfa_rf_gsm_idle_frame_processing_req.cnf_required = FALSE;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rfa_rf_gsm_idle_frame_processing_req.hdr, sizeof(rfa_rf_gsm_idle_frame_processing_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

}

/*===========================================================================

FUNCTION  gl1_hw_rf_cm_enter

DESCRIPTION
  Send MSGR message to RF Task to run GSM CM enter.

===========================================================================*/
void gl1_hw_rf_cm_enter(gas_id_t gas_id)
{
  rfa_rf_gsm_cm_enter_req_s rfa_rf_gsm_cm_enter_req;
  errno_enum_type result;
  rfa_rf_gsm_cm_enter_cnf_s rfa_rf_gsm_cm_enter_cnf;
  uint32 bytes_recvd;
  msgr_umid_type rfa_umid_to_send = NULL;
  msgr_umid_type rfa_umid_to_rcv = NULL;
  int16 index;
#ifdef GERAN_L1_HLLL_LNASTATE
  uint8 i = 0;
  rfcom_device_enum_type device;
#endif

  gas_id = check_gas_id(gas_id);

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_CM_ENTER_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_CM_ENTER_CNF;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_CM_ENTER_SUB2_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_CM_ENTER_SUB2_CNF;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
  else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
  {
    rfa_umid_to_send = RFA_RF_GSM_CM_ENTER_SUB3_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_CM_ENTER_SUB3_CNF;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initialise msgr header */
  msgr_init_hdr(&rfa_rf_gsm_cm_enter_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload  - there is currently no payload for this message */
  rfa_rf_gsm_cm_enter_req.device = gl1_hw_cm_get_trm_rf_device(gas_id);
  rfa_rf_gsm_cm_enter_req.timing_info = &gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info;
  rfa_rf_gsm_cm_enter_req.buffer_id = 0;
  rfa_rf_gsm_cm_enter_req.sub_id = gas_id;
  rfa_rf_gsm_cm_enter_req.cnf_required = TRUE;

  GDRV_RFA_MSGR_LOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rfa_rf_gsm_cm_enter_req.hdr, sizeof(rfa_rf_gsm_cm_enter_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

   /* blocking wait for IRAT params CNF message from RF Task */
  result = msgr_receive(&gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
                        (uint8 *)&rfa_rf_gsm_cm_enter_cnf,
                        (uint32)sizeof(rfa_rf_gsm_cm_enter_cnf_s),
                        &bytes_recvd);
  GDRV_RFA_MSGR_UNLOCK(gas_id);

  /* sanity check and print debug for msgr_receive */
  gl1_hw_rf_msgr_receive_debug(rfa_umid_to_rcv,
                               result,
                               rfa_rf_gsm_cm_enter_cnf.hdr.id,
                               rfa_rf_gsm_cm_enter_cnf.req_result,
                               gas_id);

#ifdef GERAN_L1_HLLL_LNASTATE
  for(device=RFM_DEVICE_0;device<RFM_MAX_WAN_DEVICES;device++)
   {
   for(i=0;i<(RFCOM_NUM_GSM_BANDS-1);i++)
    {
    gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] |= (rfa_rf_gsm_cm_enter_cnf.sawless_sup[device][i] << i);
     }

     if (gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] != 0)
     {
       MSG_GERAN_HIGH_2_G("Sawless support for band:0x%x for device:%d",
                          gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device],
                       device);
    }
   }
#endif /*GERAN_L1_HLLL_LNASTATE*/

  /* Initialise current GSM band as INVALID on initialising RF */
  gl1_hw_data_ptr[gas_id]->gl1_hw_rf_gsm_band = (int8) RFGSM_BAND_NONE;

  /* Initialise RF Task Rx burst buffers */
  for (index=0; index<GL1_HW_RF_BURST_RX_BUF_NUM; index++)
  {
    gl1_hw_data_ptr[gas_id]->gl1_hw_rf_rx_burst_buf[index].num_rx_bursts = GL1_HW_RF_BURST_NUM_BURSTS_INVALID;
  }
  /* Initialise RF Task Tx burst buffers */
  for (index=0; index<GL1_HW_RF_BURST_TX_BUF_NUM; index++)
  {
    gl1_hw_data_ptr[gas_id]->gl1_hw_rf_tx_burst_buf[index].num_tx_slots = GL1_HW_RF_BURST_NUM_BURSTS_INVALID;
  }
}

/*===========================================================================

FUNCTION  gl1_hw_rf_cm_exit

DESCRIPTION
  Send MSGR message to RF Task to run GSM CM exit.

===========================================================================*/
void gl1_hw_rf_cm_exit(gas_id_t gas_id)
{
  rfa_rf_gsm_cm_exit_req_s rfa_rf_gsm_cm_exit_req;
  errno_enum_type result;
  rfa_rf_gsm_cm_exit_cnf_s rfa_rf_gsm_cm_exit_cnf;
  uint32 bytes_recvd;
  msgr_umid_type rfa_umid_to_send = NULL;
  msgr_umid_type rfa_umid_to_rcv = NULL;

  gas_id = check_gas_id(gas_id);

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_CM_EXIT_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_CM_EXIT_CNF;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_CM_EXIT_SUB2_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_CM_EXIT_SUB2_CNF;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
  else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
  {
    rfa_umid_to_send = RFA_RF_GSM_CM_EXIT_SUB3_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_CM_EXIT_SUB3_CNF;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initialise msgr header */
  msgr_init_hdr(&rfa_rf_gsm_cm_exit_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);

  /* populate payload  - there is currently no payload for this message */
  rfa_rf_gsm_cm_exit_req.device = gl1_hw_cm_get_trm_rf_device(gas_id);
  rfa_rf_gsm_cm_exit_req.cnf_required = TRUE;
  rfa_rf_gsm_cm_exit_req.buffer_id = 0;
  rfa_rf_gsm_cm_exit_req.sub_id = gas_id;

  GDRV_RFA_MSGR_LOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rfa_rf_gsm_cm_exit_req.hdr, sizeof(rfa_rf_gsm_cm_exit_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

   /* blocking wait for IRAT params CNF message from RF Task */
  result = msgr_receive(&gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
                        (uint8 *)&rfa_rf_gsm_cm_exit_cnf,
                        (uint32)sizeof(rfa_rf_gsm_cm_exit_cnf_s),
                        &bytes_recvd);
  GDRV_RFA_MSGR_UNLOCK(gas_id);

  /* sanity check and print debug for msgr_receive */
  gl1_hw_rf_msgr_receive_debug(rfa_umid_to_rcv,
                               result,
                               rfa_rf_gsm_cm_exit_cnf.hdr.id,
                               rfa_rf_gsm_cm_exit_cnf.req_result,
                               gas_id);
}
/*===========================================================================

FUNCTION  gl1_hw_rf_update_freq_error

DESCRIPTION
  Keeps a store of the current frequency error to be applied via DPLL

===========================================================================*/
void gl1_hw_rf_update_freq_error(int32 freq_err,gas_id_t gas_id)
{
  gl1_hw_data_ptr[gas_id]->gl1_hw_rf_freq_error = freq_err;
}

/*===========================================================================

FUNCTION  gl1_hw_rf_get_freq_error

DESCRIPTION
  Gets a copy of the current stored frequency error to be applied via DPLL

===========================================================================*/
int32 gl1_hw_rf_get_freq_error(gas_id_t gas_id)
{
  return gl1_hw_data_ptr[gas_id]->gl1_hw_rf_freq_error;
}
/*===========================================================================

FUNCTION  gl1_hw_rf_msgr_send_debug

DESCRIPTION
  Debug sanity check for Message Router send

===========================================================================*/
void gl1_hw_rf_msgr_send_debug(msgr_umid_type umid, errno_enum_type result, gas_id_t gas_id)
{
  if (result != E_SUCCESS)
  {
    MSG_GERAN_ERROR_2_G("UMID 0x%x: msgr_send failed: %d", umid, result);
  }
#ifdef FEATURE_GSM_RFA_TASK_DEBUG
  else
  {
    MSG_GERAN_HIGH_2_G("UMID 0x%x: msgr_send success: %d", umid, result);
  }
#endif
}

/*===========================================================================

FUNCTION  gl1_hw_rf_msgr_receive_debug

DESCRIPTION
  Debug sanity check for Message Router receive

===========================================================================*/
void gl1_hw_rf_msgr_receive_debug(msgr_umid_type umid,
                                  errno_enum_type result,
                                  msgr_umid_type received_umid,
                                  rfa_rf_gsm_cmd_outcome_t req_result,
                                  gas_id_t gas_id)
{
  if (result != E_SUCCESS)
  {
    MSG_GERAN_FATAL_2_G( "UMID 0x%x: msgr_receive failed: %d", umid, result );
    return;
  }
#ifdef FEATURE_GSM_RFA_TASK_DEBUG
  else
  {
    MSG_GERAN_MED_2_G("UMID 0x%x: msgr_receive success: %d", umid, result);
  }
#endif
  /* check that we received the expected CNF UMID */
  if(received_umid != umid)
  {
    MSG_GERAN_ERROR_2_G("Expected UMID 0x%x: Received UMID 0x%x", umid, received_umid);
  }
  /* check that RF MSGR Req completed successfully */
  if(req_result != RFA_RF_GSM_SUCCESS)
  {
    MSG_GERAN_ERROR_2("UMID 0x%x: RF Task reported failure: %d", umid, req_result);
  }
}

/*===========================================================================

FUNCTION  gl1_hw_rf_check_active_buffers

DESCRIPTION
  Provides ability to check if there are any outstanding RF Task activities i.e.
  whether all Rx and Tx burst requests have been completed by RF Task.
RETURN
  TRUE  - RF task still has active RF buffers to populate
  FALSE - RF Task has completed all oustanding RF buffer population
===========================================================================*/
boolean gl1_hw_rf_check_active_buffers(gas_id_t gas_id)
{
   int16 index;
   boolean rf_task_active = FALSE;

   for (index=0; index<GL1_HW_RF_BURST_RX_BUF_NUM; index++)
   {
      if(gl1_hw_data_ptr[gas_id]->gl1_hw_rf_rx_burst_buf[index].num_rx_bursts != GL1_HW_RF_BURST_NUM_BURSTS_INVALID)
          rf_task_active = TRUE;
   }

   for (index=0; index<GL1_HW_RF_BURST_TX_BUF_NUM; index++)
   {
      if(gl1_hw_data_ptr[gas_id]->gl1_hw_rf_tx_burst_buf[index].num_tx_slots != GL1_HW_RF_BURST_NUM_BURSTS_INVALID)
          rf_task_active = TRUE;
   }

   return rf_task_active;
}

/*===========================================================================

FUNCTION gl1_hw_rf_map_device2_as_id

DESCRIPTION
  Converts rf device to AS_ID
===========================================================================*/

sys_modem_as_id_e_type gl1_hw_rf_map_device2_as_id(rfm_device_enum_type device,ftm_mode_type ftm_mode)
{
  sys_modem_as_id_e_type asigned_as_id = SYS_MODEM_AS_ID_1;

  if (ftm_mode != FTM_MODE)
  {
    /* Check which gas id has the requested chain allocated via TRM */
    if ((gl1_hw_get_gsm_rf_id(l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_1].client_id, GERAN_ACCESS_STRATUM_ID_1) == device) &&
        ((grm_get_trm_status(l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_1].client_id, GERAN_ACCESS_STRATUM_ID_1) == GL1_TRM_GRANTED) ||
         (grm_get_trm_status(l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_1].client_id, GERAN_ACCESS_STRATUM_ID_1) == GL1_TRM_RETAINED_FOR_ACCESS)))
    {
      asigned_as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
    }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    else if ((gl1_hw_get_gsm_rf_id(l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_2].client_id, GERAN_ACCESS_STRATUM_ID_2) == device) &&
             ((grm_get_trm_status(l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_2].client_id, GERAN_ACCESS_STRATUM_ID_2) == GL1_TRM_GRANTED) ||
              (grm_get_trm_status(l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_2].client_id, GERAN_ACCESS_STRATUM_ID_2) == GL1_TRM_RETAINED_FOR_ACCESS)))
    {
      asigned_as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_2);
    }

#if defined ( FEATURE_TRIPLE_SIM )
    else if ((gl1_hw_get_gsm_rf_id(l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_3].client_id, GERAN_ACCESS_STRATUM_ID_3) == device) &&
             ((grm_get_trm_status(l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_3].client_id, GERAN_ACCESS_STRATUM_ID_3) == GL1_TRM_GRANTED) ||
              (grm_get_trm_status(l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_3].client_id, GERAN_ACCESS_STRATUM_ID_3) == GL1_TRM_RETAINED_FOR_ACCESS)))
    {
      asigned_as_id = geran_map_gas_id_to_nas_id( GERAN_ACCESS_STRATUM_ID_3 );
    }
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#ifndef FEATURE_GSM_RX_TX_SPLIT
    ftm_device[geran_map_nas_id_to_gas_id(asigned_as_id)] = RFM_INVALID_DEVICE;
#endif
  }
  else
  {
#ifndef FEATURE_DUAL_SIM
    asigned_as_id = SYS_MODEM_AS_ID_1;
#else
    /* Return SYS_MODEM_AS_ID_1 if RFM_DEVICE_0, SYS_MODEM_AS_ID_2 otherwise.
     * For TSTS No specific as id needed here as as_id-2 only needed for DSDA
     */
    if ( device == RFM_DEVICE_0 )
    {
      asigned_as_id = SYS_MODEM_AS_ID_1;
    }
    else
    {
      asigned_as_id =  SYS_MODEM_AS_ID_2;
    }
#endif
#ifndef FEATURE_GSM_RX_TX_SPLIT
    ftm_device[geran_map_nas_id_to_gas_id(asigned_as_id)] = device;
#endif
  }

  return(asigned_as_id);
}

/*===========================================================================

FUNCTION gl1_hw_rf_map_rf_sub_id_to_as_id

DESCRIPTION
  Converts rf device to AS_ID
===========================================================================*/
sys_modem_as_id_e_type gl1_hw_rf_map_rf_sub_id_to_as_id(uint8 rf_sub_id)
{
   /* Conversion from rf_sub_id to as_id is dependent upon the sub_id passed to RF
      in each GL1->RF MSGR payload. Currently the sub_id passed is directly the gas_id,
      hence we can directly use gas_id to as_id conversion here, but in future this
      may change. */
   return(geran_map_gas_id_to_nas_id((gas_id_t)rf_sub_id));
}


/*===========================================================================

FUNCTION  gl1_hw_init_async_intf_msgr_mailbox

DESCRIPTION
  Create a message router mailbox for receiving blocking FW CNF messages.
  The mailbox will be used for receiving any CNF messages from msgr_send() commands
  which require GL1 task/ISR to block until the command has been completed
  e.g. enter_mode().

===========================================================================*/
boolean gl1_hw_init_async_intf_msgr_mailbox(sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  mdsp_async_intf_init(gas_id);
  return E_SUCCESS;
}

/*===========================================================================

FUNCTION  gl1_hw_async_intf_app_mode_config

DESCRIPTION
  Send MSGR message to GFW Task to run app mode config. Use MSGR blocking wait
  to receive app mode config rsp from GFW before continuing

===========================================================================*/
void gl1_hw_async_intf_app_mode_config(GfwAppModeType app_mode, sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  GfwHostAppModeConfigCmd gfwHostAppModeConfigCmd;
  sys_modem_device_mode_e_type gl1_multi_sim_sys_mode;

#ifdef FEATURE_GSM_COEX_FW_CXM
  /* Get the Rx CxM Channel Id using the Rx Client*/
  cxm_channel_out_type      cxm_channel_out = garb_return_cxm_channel_id( gl1_get_current_grm_client_id(gas_id), gas_id);
#endif

  /* populate payload */
  gfwHostAppModeConfigCmd.mode         = app_mode;

#ifdef FEATURE_GSM_COEX_FW_CXM
  gfwHostAppModeConfigCmd.channel_id0 = cxm_channel_out.rx_channel_id;
#else
  gfwHostAppModeConfigCmd.channel_id0 = 0;
#endif

  gl1_hw_app_mode_cfg_mutex_lock(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

  if ( app_mode == gl1_get_gfw_app_mode(gas_id) )
  {
    /* app mode is already in the correct state */
    MSG_GERAN_HIGH_0_G("app mode is already in the correct state");
    gl1_hw_app_mode_cfg_mutex_unlock(gas_id);
    return;
  }
#endif

  gl1_multi_sim_sys_mode = gl1_msg_get_multi_sim_sys_mode();

  switch (gl1_multi_sim_sys_mode)
  {
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY:
     {
       gfwHostAppModeConfigCmd.devMode = GFW_DEVICE_MODE_DSDS;
       break;
     }

#if defined ( FEATURE_TRIPLE_SIM )
     case SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY:
       gfwHostAppModeConfigCmd.devMode = GFW_DEVICE_MODE_TSTS;
     break;
#endif /* FEATURE_TRIPLE_SIM */

     case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE:
     {
       gfwHostAppModeConfigCmd.devMode = GFW_DEVICE_MODE_DSDA;
       break;
     }
#endif  /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

     case SYS_MODEM_DEVICE_MODE_SINGLE_SIM:
     {
       gfwHostAppModeConfigCmd.devMode = GFW_DEVICE_MODE_SS;
       break;
     }

     default:
     {
#ifdef FEATURE_SGLTE
       if(gl1_hw_sglte_mode[gas_id])
       {
          /* DSDA for single SIM SGLTE (for COEX management) */
          gfwHostAppModeConfigCmd.devMode = GFW_DEVICE_MODE_DSDA;
       }
       else
#endif
       {
          /* single sim mode */
          gfwHostAppModeConfigCmd.devMode = GFW_DEVICE_MODE_SS;
          break;
       }
     }

  }
#if defined ( FEATURE_DUAL_ACTIVE )
  if ( ftm_get_mode() == FTM_MODE )
  {
    gfwHostAppModeConfigCmd.devMode = GFW_DEVICE_MODE_DSDA;
  }

#endif

  MSG_GERAN_HIGH_3_G("GFW App mode %d devmode %d, chain id %d"
                     ,app_mode
                     ,gfwHostAppModeConfigCmd.devMode
                     ,gfwHostAppModeConfigCmd.channel_id0);

  GL1_HANDLER_PERFORMANCE_LOGGING(START_MDSP_ASYNC_CMD_SEND_TO_FW,gas_id);

  /* send msgr message to GFW */
  (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_APP_MODE_CONFIG_CMD,
                                          &gfwHostAppModeConfigCmd,
                                          NULL,
                                          gas_id);
  
  GL1_HANDLER_PERFORMANCE_LOGGING(END_MDSP_ASYNC_CMD_RESP_FW,gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  gl1_set_gfw_app_mode_status(app_mode, gas_id);
#endif

#if defined (FEATURE_DUAL_ACTIVE) && defined(FEATURE_SGLTE_DUAL_SIM)
  if (app_mode != GFW_APP_MODE_ACTIVE)
  {
    /* App is moving to non Active
     * check if both apps are not active and if mmode needs
     * to be switched*
     */
    gl1_handle_multi_sim_mode_switch();
  }
#endif /* FEATURE_DUAL_ACTIVE */

  gl1_hw_app_mode_cfg_mutex_unlock(gas_id);
}

#if defined ( FEATURE_DUAL_ACTIVE )
/*===========================================================================

FUNCTION  gl1_hw_async_intf_dev_mode_config

DESCRIPTION
  Send MSGR message to GFW Task to run device mode command.

===========================================================================*/

void gl1_hw_async_intf_dev_mode_cmd(sys_modem_device_mode_e_type devMode, gas_id_t gas_id)
{
    GfwHostAsyncDeviceModeCmd gfwHostAsyncDeviceModeCmd;

    switch(devMode)
    {
       case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY:
       {
            gfwHostAsyncDeviceModeCmd.devMode = GFW_DEVICE_MODE_DSDS;
            break;
       }
       case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE:
       {
            gfwHostAsyncDeviceModeCmd.devMode = GFW_DEVICE_MODE_DSDA;
            break;
       }
       default:
            break;
    }

    MSG_GERAN_HIGH_1_G("GFW Dev mode:: devmode %d",gfwHostAsyncDeviceModeCmd.devMode);
    /* send msgr message to GFW */
    (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_DEVICE_MODE_CMD,
                                            &gfwHostAsyncDeviceModeCmd,
                                            NULL,
                                            gas_id);

}
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION  gl1_get_gfw_app_mode

DESCRIPTION
  return current GFW app mode

===========================================================================*/
GfwAppModeType gl1_get_gfw_app_mode(gas_id_t gas_id)
{
  MSG_GERAN_LOW_1_G("get_gfw_app_mode %d",gl1_hw_multi_sim_mode[gas_id]);

  return gl1_hw_multi_sim_mode[gas_id];
}

/*===========================================================================

FUNCTION  gl1_set_gfw_app_mode_status

DESCRIPTION
  store current GFW app mode

===========================================================================*/
void gl1_set_gfw_app_mode_status(GfwAppModeType gfw_app_mode, gas_id_t gas_id)
{
  MSG_GERAN_LOW_1_G("set_gfw_app_mode %d",gfw_app_mode);

    gl1_hw_multi_sim_mode[gas_id] = gfw_app_mode;
}

/*===========================================================================

FUNCTION  gl1_hw_check_granted_device_is_irat_compatabile

DESCRIPTION
  Checks to see if the RF device GERAN is currently granted is able to do
  W measurements.
  Returns TRUE if W measurements can be done on current allocated RF device

===========================================================================*/
boolean gl1_hw_check_granted_device_is_irat_compatabile(uint16 W_chn, gas_id_t gas_id)
{
#ifdef FEATURE_WCDMA
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  rfcom_wcdma_band_type W_band       = rfwcdma_core_convert_chan_to_band(W_chn);
  rfm_device_enum_type irat_rf_device = gl1_hw_get_wcdma_rf_id(W_band, gas_id);
  rfm_device_enum_type rf_device =
    gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id );

  if ((rf_device == RFM_INVALID_DEVICE) || (irat_rf_device == RFM_INVALID_DEVICE))
  {
    MSG_GERAN_HIGH_0_G("TRM returned Invalid RF device ");
  } else if ((irat_rf_device != rf_device) && gl1_msg_get_multi_sim_mode())
  {
    MSG_GERAN_HIGH_0_G("Incompatabile RF device");
  } else
  {
    MSG_GERAN_HIGH_1_G("Compatabile RF device for IRAT and MSim Mode is %d",gl1_msg_get_multi_sim_mode());
    return (TRUE);
  }
#endif
  return (FALSE);
}
/*===========================================================================

FUNCTION  gl1_hw_check_granted_device_is_LTE_irat_compatabile

DESCRIPTION
  Checks to see if the RF device GERAN is currently granted is able to do
  L measurements.
  Returns TRUE if L measurements can be done on current allocated RF device

===========================================================================*/
boolean gl1_hw_check_granted_device_is_LTE_irat_compatabile(lte_earfcn_t lte_dl_earfcn, gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  sys_sband_lte_e_type lte_band = SYS_SBAND_LTE_EUTRAN_BAND_MAX;

  rfm_device_enum_type rf_device, irat_rf_device;

  if( !lte_ml1_common_band_get_band_from_dl_earfcn(lte_dl_earfcn, &lte_band) )
  {
    /*True if passed in DL earfcn is valid, FALSE otherwise*/
    MSG_GERAN_ERROR_0_G("DL earfcn is not valid");
  }

  rf_device = grm_get_rf_device( l1_tskisr_blk->client_id, gas_id );

  irat_rf_device = gl1_hw_get_lte_rf_id(LTE_CLIENT_FOR_THIS_SUB(gas_id), lte_band,   gas_id);

  if ((rf_device == RFM_INVALID_DEVICE) || (irat_rf_device == RFM_INVALID_DEVICE))
  {
    MSG_GERAN_HIGH_2_G("TRM returned Invalid RF device g=%d irat=%d", rf_device, irat_rf_device );
  } else
  {

     MSG_GERAN_HIGH_2_G("Compatible RF device for IRAT  pri=%d and MSIM Mode: %d" ,  irat_rf_device , gl1_msg_get_multi_sim_mode());

     return (TRUE);
  }
  return (FALSE);
}

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION  gl1_hw_async_intf_enter_mode_cmd

DESCRIPTION
  Send MSGR message to GFW to program the RF buffer. Use MSGR blocking wait
  to receive enter mode rsp from GFW before continuing

===========================================================================*/
void gl1_hw_async_intf_enter_mode_cmd(GfwAppModeType app_mode,
                                       uint32 rxlm_buff_idx,
#ifdef FEATURE_GSM_RX_TX_SPLIT
                                       uint32 txlm_buff_idx,
                                       uint8 entermode,
#else
                                       GfwEnterModeType entermode,
#endif
                                       rfm_device_enum_type device,
                                       sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  GfwHostRfmEnterModeCmd gfwHostRfmEnterModeCmd;
#ifdef FEATURE_GSM_RX_TX_SPLIT
#ifdef FEATURE_GSM_COEX_FW_CXM
  /*Get the Tx CxM Channel ID using the Tx Client*/
  cxm_channel_out_type     cxm_channel_out = garb_return_cxm_channel_id(
	                                                 grm_map_rx_to_tx_client_id(gl1_get_current_grm_client_id(gas_id) , gas_id), 
                                                     gas_id);
#endif
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( app_mode != gl1_get_gfw_app_mode(gas_id) )
  {
    /* app mode is not in the correct state */
    MSG_GERAN_ERROR_2_G("GFW app mode %d is not in the correct state %d",
                         gl1_get_gfw_app_mode(gas_id), app_mode);
    return;
  }
#endif

#ifdef FEATURE_GSM_RX_TX_SPLIT
#ifdef FEATURE_GSM_COEX_FW_CXM
  gfwHostRfmEnterModeCmd.tx_channel_id0 = cxm_channel_out.tx_channel_id;
#else
  gfwHostRfmEnterModeCmd.tx_channel_id0 = 0;
#endif
  gfwHostRfmEnterModeCmd.rxlmBufIndex    = (uint32) -1;
  gfwHostRfmEnterModeCmd.rxlmDrxBufIndex = (uint32) -1;
  gfwHostRfmEnterModeCmd.txlmBufIndex    = (uint32) -1;

  if(entermode & GFW_ENTER_MODE_PRX_BIT) 
  {
      gfwHostRfmEnterModeCmd.rxlmBufIndex       = rxlm_buff_idx;
  }
  if(entermode & GFW_ENTER_MODE_DRX_BIT) 
  {
    gfwHostRfmEnterModeCmd.rxlmDrxBufIndex = gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id);
  }
  if(entermode & GFW_ENTER_MODE_TX_BIT) 
  {
      gfwHostRfmEnterModeCmd.txlmBufIndex       = txlm_buff_idx;
  }
#else
  switch (entermode)
  {
    case GFW_ENTER_MODE_PRX:
      gfwHostRfmEnterModeCmd.rxlmBufIndex       = rxlm_buff_idx;
      gfwHostRfmEnterModeCmd.rxlmDrxBufIndex = (uint32) -1;
      break;

    case GFW_ENTER_MODE_DRX:
      gfwHostRfmEnterModeCmd.rxlmBufIndex = (uint32) -1;
      gfwHostRfmEnterModeCmd.rxlmDrxBufIndex = gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id);
      break;

    case GFW_ENTER_MODE_COMBINED:
      gfwHostRfmEnterModeCmd.rxlmBufIndex       = rxlm_buff_idx;
      gfwHostRfmEnterModeCmd.rxlmDrxBufIndex = gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id);
      break;

    default:
      break;
  }
#endif
  gfwHostRfmEnterModeCmd.enterMode          = entermode;
  gfwHostRfmEnterModeCmd.mode               = app_mode;
  gfwHostRfmEnterModeCmd.deviceID = device;

  /* send msgr message to GFW */
  (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_RFM_ENTER_MODE_CMD, &gfwHostRfmEnterModeCmd, NULL, gas_id);

  MSG_GERAN_HIGH_5_G("GFW enter_mode_cmd: device %d rxlm idx %d, txlm idx %d, entermode=%d, QS=%d",
                     device,
                     rxlm_buff_idx,
                     txlm_buff_idx,
                     entermode,
                     gstmr_rd_qsymbol_count_geran(as_id));
}

/*===========================================================================

FUNCTION  gl1_hw_async_intf_set_tx_band_cmd

DESCRIPTION
  Send MSGR message to GFW to program the RF buffer. Use MSGR blocking wait
  to receive tx eneter mode rsp from GFW before continuing

===========================================================================*/
void gl1_hw_async_intf_set_tx_band_cmd(sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  GfwHostRfmSetTxBandCmd gfwHostRfmSetTxBandCmd;

  /* send msgr message to GFW */
  (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_RFM_SET_TX_BAND_CMD, &gfwHostRfmSetTxBandCmd, NULL, gas_id);

  /* Set TX band command issued to GFW*/
  gl1_set_tx_band_cmd_iss_to_gfw[gas_id] = TRUE;
  MSG_GERAN_MED_0_G("Set Tx band command issued to GFW");
}

/*===========================================================================

FUNCTION  gl1_hw_async_intf_wtr_cfg_cmd

DESCRIPTION
  Send MSGR message to GFW to update the RXLM index during WTR hopping.
  Use MSGR blocking wait to receive rsp from GFW before continuing

===========================================================================*/
#ifdef FEATURE_GSM_WTR_HOP
void gl1_hw_async_intf_wtr_cfg_cmd(uint32 buffer_index, rfm_device_enum_type device,
    sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  GfwHostWtrConfigCmd gfwHostWtrConfigCmd;
  cxm_channel_in_type cxm_channel_in;
  cxm_channel_out_type cxm_channel_out;
  cxm_tech_type cxm_tech[] = {CXM_TECH_GSM1, CXM_TECH_GSM2, CXM_TECH_GSM3};

  gas_id = check_gas_id(gas_id);

  cxm_channel_in.techid = cxm_tech[gas_id];
  cxm_channel_in.rf_device = device;
  cxm_channel_out = cxm_get_channel_id(cxm_channel_in);

  gfwHostWtrConfigCmd.rxlmBufIndex = buffer_index;
  gfwHostWtrConfigCmd.channel_id = cxm_channel_out.rx_channel_id;
  gfwHostWtrConfigCmd.deviceID = device;

  MSG_GERAN_HIGH_3_G("Sending GERAN_FW_WTR_CFG_CMD, RXLM index: %d, Channel Id: %d RF device %d",
      gfwHostWtrConfigCmd.rxlmBufIndex, gfwHostWtrConfigCmd.channel_id, gfwHostWtrConfigCmd.deviceID);

  /* send msgr message to GFW */
  (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_WTR_CFG_CMD, &gfwHostWtrConfigCmd, NULL, gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_wtr_hop_config_tx

DESCRIPTION
  Configure FW for tx during wtr hopping

===========================================================================*/
void gl1_hw_wtr_hop_config_tx(uint32 buffer_index, rfm_device_enum_type device, gas_id_t gas_id)
{
  cxm_channel_in_type cxm_channel_in;
  cxm_channel_out_type cxm_channel_out;
  cxm_tech_type cxm_tech[] = {CXM_TECH_GSM1, CXM_TECH_GSM2, CXM_TECH_GSM3};

  gas_id = check_gas_id(gas_id);

  /* Obtain chain id for the tx device */
  cxm_channel_in.techid = cxm_tech[gas_id];
  cxm_channel_in.rf_device = device;
  cxm_channel_out = cxm_get_channel_id(cxm_channel_in);

  mdsp_gsm_config_tx_channel_id(buffer_index, cxm_channel_out.tx_channel_id, gas_id);
}

#endif /* FEATURE_GSM_WTR_HOP */

/*===========================================================================

FUNCTION  gl1_hw_set_afc_mode

DESCRIPTION
  In FTM mode this function is called to set afc_mode accordingly. If GSM XO CAL is being done in FTM mode AFC is
  set to  GL1_HW_ACQ_GSM_XOCAL. For GSM XO CAL need to make sure that no frequency correction is applied to
  the received IQ samples in both GL1 and GFW.

===========================================================================*/
void gl1_hw_set_afc_mode(gl1_hw_acq_afc_type AFC)
{
  afc_mode = AFC;
}

/*===========================================================================

FUNCTION  gl1_hw_combined_tcxo_frozen

DESCRIPTION
   This function will return the frozen state of TCXO Mgr 3 or SRM.

RETURN

   boolean state of TCXO Manager or SRM

===========================================================================*/
boolean gl1_hw_combined_tcxo_frozen( void )
{
   return ( FALSE );
}



#if defined( FEATURE_GSM_TX_DIV_ANTNA_SWITCHING) ||defined( FEATURE_GSM_QSC_TX_DIV)
/*===========================================================================
FUNCTION  gl1_hw_rfa_rf_gsm_set_antenna_req_cmd
===========================================================================*/
boolean  gl1_hw_rfa_rf_gsm_set_antenna_req_cmd( boolean antenna_pos, gas_id_t gas_id)
{
  rfa_rf_gsm_set_antenna_req_s  rfa_rf_gsm_set_antenna_req_cmd;
  errno_enum_type result;
  trm_get_info_return_type get_granted_info;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = NULL;
  msgr_umid_type rfa_umid_to_send = NULL;

  gas_id = check_gas_id(gas_id);
  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  
  /*Flag gets cleared in switch_complete_cb*/
  set_pending_asdiv_switch_request(antenna_pos, gas_id);
	
  if ( TRM_GRANTED != grm_get_granted (gl1_map_gas_id_to_client_id( gas_id),&get_granted_info, gas_id ) )
  {
    /* done to satisfy static analysis  */
    const trm_switch_complete_input_type temp_switch_complete_input = gl1_switch_complete_input[gas_id];

     MSG_GERAN_ERROR_0_G("TRM is not granted, do not send set_antenna_req_cmd");
    if (  ( ( (temp_switch_complete_input.client) >= TRM_1X) && 
            ( (temp_switch_complete_input.client) <= TRM_LAST_CLIENT)) )
	 {
       clear_pending_asdiv_switch_request(gas_id);
      trm_ant_switch_div_switch_cancel(temp_switch_complete_input);
      return FALSE;
	 }
  }	
  
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
     rfa_umid_to_send = RFA_RF_GSM_SET_ANTENNA_REQ;
    
  }
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
     rfa_umid_to_send = RFA_RF_GSM_SET_ANTENNA_SUB2_REQ;

  }
#if defined ( FEATURE_TRIPLE_SIM )
  else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
  {
  /* RFA_RF_GSM_MSG_ID_SET_ANTENNA_SUB3 is not yet defined in rf */
  //rfa_umid_to_send = RFA_RF_GSM_MSG_ID_SET_ANTENNA_SUB3;

  }
#endif /* FEATURE_TRIPLE_SIM */
  else
  {
    MSG_GERAN_FATAL_0_G( "Invalid gas_id" );
    return FALSE;
  }

  rfa_rf_gsm_set_antenna_req_cmd.device = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);

  rfa_rf_gsm_set_antenna_req_cmd.cnf_required = TRUE;
  rfa_rf_gsm_set_antenna_req_cmd.ant_pos = antenna_pos;
  rfa_rf_gsm_set_antenna_req_cmd.sub_id  = gas_id ;
  rfa_rf_gsm_set_antenna_req_cmd.l1_cb_func =(rfgsm_l1_cb_type )gl1_rf_switch_complete_cb ;

    /* initialise msgr header */

  msgr_init_hdr(&rfa_rf_gsm_set_antenna_req_cmd.hdr,MSGR_GERAN_GL1,rfa_umid_to_send );
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
		#error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

   /* send msgr message to RF Task */
  result = msgr_send(&rfa_rf_gsm_set_antenna_req_cmd.hdr, sizeof(rfa_rf_gsm_set_antenna_req_cmd));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result, gas_id);

  return TRUE;
}
#ifdef FEATURE_GERAN_SAR 
/*===========================================================================

FUNCTION  gl1_hw_check_rf_switch_disable

DESCRIPTION
  RF can disable the switching algorithm due to SAR state change. GL1 needs to check with RF and force the switch to
  a certain fix position

===========================================================================*/

boolean gl1_hw_check_rf_switch_disable(gas_id_t gas_id)
{
  rfm_sar_forced_antenna_enum_type result;
  rfm_sar_forced_antenna_input_param_type input_parm;

  input_parm.device = gl1_hw_data_ptr[gas_id]->gl1_hw_trm_rf_rx_device;
  input_parm.rfmode = RFM_EGSM_MODE;
  result = rfm_get_sar_forced_antenna(&input_parm);

  /*need to force asdiv to the retuned position and disable asdiv*/
  /* RFM_SAR_ANTENNA_CONFIG_1 is swap position, corresponds to antenna_to_use_for_tx[gas_id]==2 
     RFM_SAR_ANTENNA_CONFIG_0 is pass through position, corresponds to antenna_to_use_for_tx[gas_id]==1*/

  if(((antenna_to_use_for_tx[gas_id]==2) && (result == RFM_SAR_ANTENNA_CONFIG_0))||
	   ((antenna_to_use_for_tx[gas_id]==1) && (result == RFM_SAR_ANTENNA_CONFIG_1)))
  {
	MSG_GERAN_HIGH_1_G("ASDIV disable by RF, force position to %d ", result);
	gl1_hw_switch_antenna_tx_div(gas_id);

	if((antenna_to_use_for_tx[gas_id]==2) && (result == RFM_SAR_ANTENNA_CONFIG_0))
	{
		antenna_to_use_for_tx[gas_id]=1;
	}
	else if((antenna_to_use_for_tx[gas_id]==1) && (result == RFM_SAR_ANTENNA_CONFIG_1))
	{
	 antenna_to_use_for_tx[gas_id]=2;
	}

	gl1_hw_asdiv_mtpl_store[gas_id].RF_disable_switch = TRUE;
  }
  else if((result == RFM_SAR_ANTENNA_CONFIG_0) || (result == RFM_SAR_ANTENNA_CONFIG_1))
  {
    MSG_GERAN_HIGH_1_G("ASDIV disable by RF, force position to %d ", result);
	gl1_hw_asdiv_mtpl_store[gas_id].RF_disable_switch = TRUE;
  }
  else if ((gl1_hw_asdiv_mtpl_store[gas_id].RF_disable_switch == TRUE)&&(result == RFM_SAR_ANTENNA_NONE))
  {
    gl1_hw_asdiv_mtpl_store[gas_id].RF_disable_switch = FALSE;
    MSG_GERAN_HIGH_0_G("ASDIV resume by RF");
}

  return gl1_hw_asdiv_mtpl_store[gas_id].RF_disable_switch;
  
}
#endif
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */

#ifdef FEATURE_GSM_RX_DIVERSITY
/*===========================================================================

FUNCTION  gl1_hw_rf_task_enter_mode_DivRx

DESCRIPTION
  Send MSGR message to RF Task to run GSM enter mode. Use MSGR blocking wait
  to receive enter mode completion confimation from RF Task before continuing

===========================================================================*/
boolean gl1_hw_rf_task_enter_mode_DivRx(uint32 rxlm_buf_idx, rfcom_device_enum_type device, gas_id_t gas_id)
{
   rfa_rf_gsm_enter_mode_req_s rf_gsm_enter_mode_req;
   rfa_rf_gsm_enter_mode_cnf_s rf_gsm_enter_mode_cnf;

  errno_enum_type result;
  uint32 bytes_recvd;
  msgr_umid_type rfa_umid_to_send = NULL;
  msgr_umid_type rfa_umid_to_rcv = NULL;
  boolean drx_enter_mode_successful = FALSE;

  gas_id = check_gas_id(gas_id);


#if defined FEATURE_GSM_RX_DIVERSITY_IDLE
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_ENTER_MODE_REQ;
    rfa_umid_to_rcv = RFA_RF_GSM_ENTER_MODE_CNF;
  }
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_ENTER_MODE_SUB2_REQ;
    rfa_umid_to_rcv = RFA_RF_GSM_ENTER_MODE_SUB2_CNF;
  }
#if defined ( FEATURE_TRIPLE_SIM )
  else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
  {
    rfa_umid_to_send = RFA_RF_GSM_ENTER_MODE_SUB3_REQ;
    rfa_umid_to_rcv = RFA_RF_GSM_ENTER_MODE_SUB3_CNF;
  }
#endif /* FEATURE_TRIPLE_SIM */

#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

  GDRV_RFA_MSGR_LOCK(gas_id);

  /* initialise msgr header */
  msgr_init_hdr(&rf_gsm_enter_mode_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */
  rf_gsm_enter_mode_req.init_writes_buff_ptr = (void*)mdsp_get_gfw_init_event_buffer_DivRx(gas_id);
  rf_gsm_enter_mode_req.buffer_id = 0;
  rf_gsm_enter_mode_req.sub_id = gas_id;
  rf_gsm_enter_mode_req.cnf_required = TRUE;

  rf_gsm_enter_mode_req.rxlm_buf_index.drx_idx = (lm_handle_type)rxlm_buf_idx;

  rf_gsm_enter_mode_req.rxlm_buf_index.prx_idx = (uint32)(-1);
  rf_gsm_enter_mode_req.drx_device = device;
  rf_gsm_enter_mode_req.prx_device = RFM_INVALID_DEVICE;
  rf_gsm_enter_mode_req.enter_mode = RFGSM_DRX_ENTER_MODE;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
	#error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

  /* send msgr message to RF Task */
  result = msgr_send(&rf_gsm_enter_mode_req.hdr, sizeof(rf_gsm_enter_mode_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result, gas_id);

  /* blocking wait for RF enter mode CNF message from RF Task */
  result = msgr_receive(&gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
                        (uint8 *)&rf_gsm_enter_mode_cnf,
                        (uint32)sizeof(rfa_rf_gsm_enter_mode_cnf_s),
                        &bytes_recvd);

  /* sanity check and print debug for msgr_receive */
  drx_enter_mode_successful = gl1_hw_rf_msgr_receive_debug_divrx(rfa_umid_to_rcv,
                                                                 result,
                                                                 rf_gsm_enter_mode_cnf.hdr.id,
                                                                 rf_gsm_enter_mode_cnf.req_result,
                                                                 gas_id);

  GDRV_RFA_MSGR_UNLOCK(gas_id);

  MSG_GERAN_HIGH_1_G("drx_enter_mode_successful:%d",drx_enter_mode_successful);

  return(drx_enter_mode_successful);

}

#ifndef FEATURE_GSM_RX_TX_SPLIT
/*===========================================================================

FUNCTION  gl1_hw_async_intf_enter_mode_cmd_DivRx

DESCRIPTION
  Send MSGR message to GFW to program the RF buffer. Use MSGR blocking wait
  to receive enter mode rsp from GFW before continuing

===========================================================================*/
void gl1_hw_async_intf_enter_mode_cmd_DivRx(GfwAppModeType app_mode,
                                            uint32 rxlm_buff_idx,
                                            GfwEnterModeType entermode,
                                            sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  GfwHostRfmEnterModeCmd gfwHostRfmEnterModeCmd;
  GfwHostRfmEnterModeRsp gfwHostRfmEnterModeRsp;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( app_mode != gl1_get_gfw_app_mode(gas_id) )
  {
    /* app mode is not in the correct state */
    MSG_GERAN_ERROR_2_G("GFW app mode %d is not in the correct state %d",
                         gl1_get_gfw_app_mode(gas_id), app_mode);
    return;
  }
#endif

  MSG_GERAN_HIGH_1_G("DRX enter_mode_cmd on as_id:%d",as_id);

  gfwHostRfmEnterModeCmd.rxlmBufIndex     = (uint32) (-1);
  gfwHostRfmEnterModeCmd.rxlmDrxBufIndex  = rxlm_buff_idx;
  gfwHostRfmEnterModeCmd.enterMode      = entermode;
  gfwHostRfmEnterModeCmd.mode         = app_mode;

  /* send msgr message to GFW */

  (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_RFM_ENTER_MODE_CMD,
                                          &gfwHostRfmEnterModeCmd,
                                          &gfwHostRfmEnterModeRsp,
                                          gas_id);
}
#endif

#ifdef FEATURE_GSM_RX_TX_SPLIT
/*===========================================================================

FUNCTION  gl1_hw_async_intf_tx_exit_cmd

DESCRIPTION
  Send MSGR message to GFW to program the RF buffer. Use MSGR blocking wait
  to receive enter mode rsp from GFW before continuing

===========================================================================*/
void gl1_hw_async_intf_tx_exit_cmd(gas_id_t gas_id)
{
  GfwHostTxExitCmd gfwHostTxExitCmd;
  GfwHostTxExitRsp gfwHostTxExitRsp;

  MSG_GERAN_HIGH_1_G("Tx_exit_mode_cmd on gas_id:%d",gas_id);
  
  /* send msgr message to GFW */
  (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_TX_EXIT_CMD, &gfwHostTxExitCmd, &gfwHostTxExitRsp, gas_id);
}
#endif

/*===========================================================================
	
FUNCTION  gl1_hw_rf_msgr_receive_debug_divrx

DESCRIPTION
  Debug sanity check for Message Router receive
     
===========================================================================*/
static boolean gl1_hw_rf_msgr_receive_debug_divrx(msgr_umid_type umid,
                             errno_enum_type result,
                             msgr_umid_type received_umid,
                             rfa_rf_gsm_cmd_outcome_t req_result,
                             gas_id_t gas_id)
{
  boolean enter_mode_result = TRUE;

  if (result != E_SUCCESS)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_2_G("UMID 0x%x: msgr_receive failed: %d", umid, result);
  }
#ifdef FEATURE_GSM_RFA_TASK_DEBUG
  else
  {
    MSG_GERAN_MED_2_G("UMID 0x%x: msgr_receive success: %d", umid, result);
  }
#endif
  /* check that we received the expected CNF UMID */
  if(received_umid != umid)
  {
    MSG_GERAN_ERROR_2_G("Expectd UMID 0x%x: Received UMID 0x%x", umid, received_umid);
  }
  /* check that RF MSGR Req completed successfully */
  if(req_result != RFA_RF_GSM_SUCCESS)
  {
    enter_mode_result = FALSE;
    MSG_GERAN_ERROR_2_G("UMID 0x%x: RF Task reported failure: %d", umid, req_result);
  }

  return(enter_mode_result);
}
#endif


static void gl1_hw_efs_read_vamos_support( gas_id_t gas_id )
  {
    int efs_error;
    uint32 vamos_support = 0;
    efs_error = geran_efs_read_per_sub(GERAN_EFS_VAMOS_SUPPORT,&gl1_vamos_support[gas_id],sizeof(gl1_vamos_support[0]),geran_map_gas_id_to_nas_id(gas_id));

    MSG_GERAN_HIGH_2("GL1:EFS VAMOS Support %d %d",efs_error,gl1_vamos_support[gas_id]);

    if(efs_error == -1)
  {
        gl1_vamos_support[gas_id] = 1;
  }

    MSG_GERAN_HIGH_2("GL1:EFS VAMOS Support %d %d",efs_error,gl1_vamos_support[gas_id]);

    vamos_support = gl1_vamos_support[gas_id];

    mdsp_update_shared_variable(GFW_VAMOS_SUPPORT,&vamos_support,gas_id);
}

/*===========================================================================

FUNCTION
  gl1_hw_efs_read_parallel_sch_support

DESCRIPTION
  Read in the default NV settings for Parallel SCH control

RETURN VALUE
  None.

SIDE EFFECTS
  Updates global NV control setting in gl1_parallel_sch_support.

===========================================================================*/
static void gl1_hw_efs_read_parallel_sch_support( void )
{
  int efs_error =
        geran_efs_read_primary( GL1_PARALLEL_SCH_SUPPORT,
                 &gl1_parallel_sch_support,
                 sizeof( gl1_parallel_sch_support ) );

  if ( efs_error == -1 )
  {
    /*
     * Enable by default if no NV control exists
     */
    gl1_parallel_sch_support = NV_PARALLEL_SCH_DEFAULT_CFG;
  }

  MSG_GERAN_HIGH_2( "GL1:EFS EFS_Status:%d Parallel SCH Support:0x%x",
                    efs_error, gl1_parallel_sch_support );
  }
/*===========================================================================

FUNCTION
  gl1_hw_efs_read_idle_g2x_tuneaway_support

DESCRIPTION
  Read in the default NV settings for g2x idle tuneaway control

RETURN VALUE
  None.

SIDE EFFECTS
  Updates global NV control setting in g2x idle tuneaway control.

===========================================================================*/
static void gl1_hw_efs_read_idle_g2x_tuneaway_support( void )
{
  int efs_error =
        geran_efs_read_primary( GL1_G2X_IDLE_TUNEAWAY_SUPPORT,
                 &gl1_g2x_idle_tuneaway_support,
                 sizeof( gl1_g2x_idle_tuneaway_support ) );

  if ( efs_error == -1 )
  {
    /*
     * Enable by default if no NV control exists
     */
    gl1_g2x_idle_tuneaway_support = NV_G2X_IDLE_TUNEAWAY_CFG;
  }

  MSG_GERAN_HIGH_2( "GL1:EFS EFS_Status:%d g2x idle tuneaway support:0x%x",
                    efs_error, gl1_g2x_idle_tuneaway_support );
}

/*===========================================================================

FUNCTION
  gl1_hw_is_parallel_sch_enabled

DESCRIPTION
  Check status of Parallel SCH control for G only mode.

RETURN VALUE
  TRUE by default
  FALSE if in FTM mode and/or disable bit
  NV_PARALLEL_SCH_DIS set in NV

SIDE EFFECTS
  None.

===========================================================================*/
boolean gl1_hw_is_parallel_sch_enabled( void )
{
  boolean gl1_is_parallel_sch_enabled = TRUE;
  boolean ftm_active                  =
           ( ftm_get_mode() == FTM_MODE );

  /* Not allowed in FTM or if disable bit set in NV */
  if ( ( gl1_parallel_sch_support & NV_PARALLEL_SCH_DIS ) ||
       ftm_active )
  {
    gl1_is_parallel_sch_enabled = FALSE;
  }

  MSG_GERAN_LOW_2( "GL1 Parallel SCH Enabled:%d FTM active:%d",
                   gl1_is_parallel_sch_enabled, ftm_active );

  return ( gl1_is_parallel_sch_enabled );
}

/*===========================================================================

FUNCTION
  gl1_hw_is_x2g_parallel_sch_enabled

DESCRIPTION
  Check status of Parallel SCH control for X2G only mode.

RETURN VALUE
  TRUE by default
  FALSE if in FTM mode/G support disabled and/or disable bit
  NV_X2G_PARALLEL_SCH_DIS set in NV

SIDE EFFECTS
  None.

===========================================================================*/
boolean gl1_hw_is_x2g_parallel_sch_enabled( void )
{
  boolean gl1_is_x2g_parallel_sch_enabled = TRUE;

  /* Not allowed in FTM and if GL1 mode is not supported */
  if ( ( gl1_parallel_sch_support & NV_X2G_PARALLEL_SCH_DIS ) ||
       !gl1_hw_is_parallel_sch_enabled() )
  {
    gl1_is_x2g_parallel_sch_enabled = FALSE;
  }

  MSG_GERAN_LOW_1( "X2G Parallel SCH Enabled:%d",
                   gl1_is_x2g_parallel_sch_enabled );

  return ( gl1_is_x2g_parallel_sch_enabled );
}
/*===========================================================================

FUNCTION
  gl1_hw_is_g2x_tuneaway_non drx_enabled

DESCRIPTION
  Check status of g2x tuneawayin non drx mode.

RETURN VALUE
  TRUE by default


SIDE EFFECTS
  None.

===========================================================================*/
boolean gl1_hw_is_g2x_non_drx_ta_enabled( void )
{
  boolean gl1_is_non_drx_ta_enabled = FALSE;
  boolean ftm_active                  =
           ( ftm_get_mode() == FTM_MODE );

  /* Not allowed in FTM or if disable bit set in NV */
  if ( ( gl1_g2x_idle_tuneaway_support & NV_G2X_NON_DRX_TA_ENA ) ||
       ftm_active )
  {
    gl1_is_non_drx_ta_enabled = TRUE;
  }

  MSG_GERAN_HIGH_2( "GL1 idle non drx ta Enabled:%d FTM active:%d",
                   gl1_is_non_drx_ta_enabled, ftm_active );

  return ( gl1_is_non_drx_ta_enabled );
}

/*===========================================================================

FUNCTION
  gl1_hw_is_g2x_reorg_ta_enabled

DESCRIPTION
  Check status of g2x reorg ta allowed or not

RETURN VALUE
  FALSE by default


SIDE EFFECTS
  None.

===========================================================================*/
boolean gl1_hw_is_g2x_reorg_ta_enabled( void )
{
  boolean gl1_is_reorg_ta_enabled = FALSE;
    boolean ftm_active                  =
           ( ftm_get_mode() == FTM_MODE );

  /* Not allowed in FTM and if GL1 mode is not supported */
  if ( ( gl1_g2x_idle_tuneaway_support & NV_G2X_RE_ORG_TA_ENA ) ||
        ftm_active)
    {
    gl1_is_reorg_ta_enabled = TRUE;
    }

  MSG_GERAN_HIGH_1( "GL1 idle reorg ta Enabled::%d",
                   gl1_is_reorg_ta_enabled );

  return ( gl1_is_reorg_ta_enabled );
}

#ifdef FEATURE_VAMOS_II
boolean gl1_hw_get_vamos2_support( gas_id_t gas_id )
{
 boolean vamos_support = FALSE;
 vamos_support = (gl1_vamos_support[gas_id] == 2)?TRUE:FALSE;
 return(vamos_support);
}

uint8 gl1_hw_get_vamos_nv( gas_id_t gas_id )
{
return(gl1_vamos_support[gas_id]);
}
#endif /*FEATURE_VAMOS_II*/
#ifdef GL1_GMSK_MOD_DET

/*===========================================================================
FUNCTION
  gl1_hw_efs_read_gdet_support
DESCRIPTION
  Read in the default NV settings 

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/

static void gl1_hw_efs_read_gdet_support( void )
{
  int efs_error =
        geran_efs_read_primary( GL1_GDET_SUPPORT,
                 &gl1_gdet_support,
                 sizeof( gl1_gdet_support ) );

  if ( efs_error == -1 )
  {
    gl1_gdet_support = NV_GDET_DEFAULT_CFG;
  }
  MSG_GERAN_HIGH_2( "GL1:EFS EFS_Status:%d gdet Support:0x%x",
                    efs_error, gl1_gdet_support );
}
/*===========================================================================
FUNCTION
  gl1_hw_efs_read_gdet_threshold_support
DESCRIPTION
  Read in the default NV settings 

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/
static void gl1_hw_efs_read_gdet_threshold_support( void )
{
  int efs_error =
        geran_efs_read_primary( GL1_GDET_THRESHOLD,
                 &gl1_gdet_threshold,
                 sizeof( gl1_gdet_threshold ) );
  if ( efs_error == -1 )
  {
    gl1_gdet_threshold = NV_GDET_THRESHOLD_CFG;
  }
  MSG_GERAN_HIGH_2( "GL1:EFS EFS_Status:%d gdet Support:%d",
                    efs_error, gl1_gdet_threshold );
}
/*===========================================================================
FUNCTION
  gl1_hw_is_gdet_enabled
DESCRIPTION
  Read in the default NV settings 

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/
boolean gl1_hw_is_gdet_enabled( void )
{
 boolean gdet_enabled = TRUE;

  MSG_GERAN_HIGH_1( " GDET Enabled:%d",
                   gl1_gdet_support );
 if(!(gl1_gdet_support & NV_GDET_DEFAULT_CFG))
 {
  gdet_enabled = FALSE;
 }
 return(gdet_enabled);
}
/*===========================================================================
FUNCTION
  gl1_hw_is_gdet_enabled
DESCRIPTION
  Read in the default NV settings 

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/
boolean gl1_hw_is_gdet_debug_enabled( void )
{
 boolean gl1_gdet_debug_enabled = FALSE;

 if((gl1_gdet_support & NV_GDET_DEBUG_CFG))
 {
  gl1_gdet_debug_enabled = TRUE;
 }
 return(gl1_gdet_debug_enabled);
}

uint16  gl1_hw_is_gdet_threshold_enabled( void )
{

  MSG_GERAN_HIGH_1( " GDET threshold :%d",
                   gl1_gdet_threshold );

  return(gl1_gdet_threshold);
}
#endif


#ifdef FEATURE_GERAN_LOG_OPT
/*===========================================================================

FUNCTION
  gl1_hw_efs_read_disable_conditional_logging_flag

DESCRIPTION
  Read in the default NV settings and get the conditional logging disable flag setting
  NV 74011

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/

static void gl1_hw_efs_read_disable_conditional_logging_flag( void )
{
  int efs_error =
         geran_efs_read_primary( GL1_NV_DISABLE_CONDITIONAL_LOGGING,
                  &gl1_disable_conditional_logging,
                  sizeof( gl1_disable_conditional_logging ) );

  if ( efs_error == -1 )
  {
    /*
     * if NV is not set, or set to 0 the conditional logging will be enabled
     */
    gl1_disable_conditional_logging = 0;
  }

  MSG_GERAN_HIGH_2( "GL1:EFS EFS_Status:%d gl1_disable_conditional_logging:0x%x", 
     efs_error, gl1_disable_conditional_logging );

}

/*===========================================================================

FUNCTION
  gl1_hw_disable_conditional_logging

DESCRIPTION
  get the status of NV setting for conditional logging disable flag
RETURN VALUE
  status of the flag

SIDE EFFECTS
  
===========================================================================*/

uint8 gl1_hw_disable_conditional_logging( void )
{
  return ( gl1_disable_conditional_logging );
}
#endif

/*===========================================================================

FUNCTION  gl1_create_hw_timer

DESCRIPTION
    Create a GL1 HW timer 1
    to check already created or not use timer api timer_get_params()


===========================================================================*/


static void gl1_create_hw_timer( rex_tcb_type  *task_ptr, rex_sigs_type task_wait_sig_1, gas_id_t gas_id)
{

   timer_attrib_struct_type gsm_l1_timeout_timer_attrib;

   /* Get the params for the timer */
   timer_get_params(&gsm_l1_timeout_timer[gas_id], &gsm_l1_timeout_timer_attrib);

   /* Timer to use to handle any timeout in TRM/TCXO grants */
   if ( gsm_l1_timeout_timer_attrib.tcb_sigs_obj != NULL )
   {
     /* if the timer structure contains a TCB already */
     /* make sure it is delinked in REX's timer list */
     (void)rex_clr_timer( &gsm_l1_timeout_timer[gas_id] );
   }

   /* Create a timeout if GL1 does not get the Primary Antenna */
   rex_def_timer( &gsm_l1_timeout_timer[gas_id], task_ptr, task_wait_sig_1 );
}



/*===========================================================================

FUNCTION  gl1_hw_get_rxlm_buff_index

DESCRIPTION
    Returns rxlm buffer index

===========================================================================*/

uint32 gl1_hw_get_rxlm_buff_index(gas_id_t gas_id)
{
  return(gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx);
}

/*===========================================================================

FUNCTION  gl1_hw_set_rxlm_buff_index

DESCRIPTION
  Updates the value of the RXLM buffer index

===========================================================================*/
void gl1_hw_set_rxlm_buff_index(uint32 idx, gas_id_t gas_id)
{
  gl1_hw_data[gas_id].gl1_gsm_rxlm_buf_idx = idx;
}

/*===========================================================================

FUNCTION  gl1_hw_get_txlm_buff_index

DESCRIPTION
  Returns the TXLM buffer index

===========================================================================*/
uint32 gl1_hw_get_txlm_buff_index(gas_id_t gas_id)
{
  return gl1_hw_data[gas_id].gl1_gsm_txlm_buf_idx;
}

/*===========================================================================

FUNCTION  gl1_hw_set_txlm_buff_index

DESCRIPTION
  Updates the value of the TXLM buffer index

===========================================================================*/
void gl1_hw_set_txlm_buff_index(uint32 idx, gas_id_t gas_id)
{
  gl1_hw_data[gas_id].gl1_gsm_txlm_buf_idx = idx;
}

#ifndef FEATURE_GSM_RX_TX_SPLIT
/*===========================================================================

FUNCTION  gl1_hw_mcpm_call_config_cmd

DESCRIPTION
    Returns nothing

===========================================================================*/
void gl1_hw_mcpm_call_config_cmd(gas_id_t gas_id)
{

  mdsp_set_tx_config_cmd_pending(gl1_hw_data_ptr[gas_id]->gl1_gsm_txlm_buf_idx, gas_id);
}
#endif
#ifdef FEATURE_AEQ_PHASE4
/*===========================================================================

FUNCTION gl1_hw_efs_read_set_aeq4

DESCRIPTION
 Retrieve the AEQ control flag from EFS NV item 73504. Feature is enabled by default
 and can be disabled from NV control by setting the NV item to 0.

===========================================================================*/

void gl1_hw_efs_read_set_aeq4(gas_id_t gas_id)
{
    int efs_error;
    gl1_hw_dtm_rx_aeq4_type mdsp_aeq4_ctl_info;

    mdsp_aeq4_ctl_info = gl1_get_aeq4_info(gas_id);
    efs_error = geran_efs_read_primary(GL1_EFS_MDSP_AEQPH4_CTL,&gfw_aeq_phase4_control,sizeof(gfw_aeq_phase4_control));

    MSG_GERAN_HIGH_2_G("GL1 EFS NV for AEQ4 efs_error=%d gfw_aeq_phase4_control=%d",efs_error,gfw_aeq_phase4_control);

    if(efs_error == -1)
      {
        gfw_aeq_phase4_control = 1;
        MSG_GERAN_ERROR_1_G("GL1: EFS NV for AEQ4 MISSING! Setting gfw_aeq_phase4_control from default as ENABLED : %d",gfw_aeq_phase4_control);
      }

    mdsp_aeq4_ctl_info.control_nv = (gfw_aeq_phase4_control == TRUE ? 1 : 0);
    gl1_set_aeq4_info(mdsp_aeq4_ctl_info,AEQ4_UPDATE_NV,gas_id);

}
#endif /*FEATURE_AEQ_PHASE4 */

#ifdef FEATURE_GSM_QSC_TX_DIV
void set_pending_asdiv_switch_request( boolean antenna_pos , gas_id_t gas_id )
{
  grm_client_enum_t grm_client_id;
  
  gas_id = check_gas_id(gas_id); 
  pending_asdiv_switch_request[gas_id] = TRUE ;
  grm_client_id = gl1_map_gas_id_to_client_id(gas_id);
  grm_client_id = check_asdiv_grm_client(grm_client_id);
  gl1_switch_complete_input[gas_id].client = grm_map_grm_client_id_to_trm_client_id(grm_client_id);
  if (antenna_pos )
  {
  gl1_switch_complete_input[gas_id].config = TRM_ANT_SWITCH_DIV_CONFIG_1 ;
  }
  else
  {
  gl1_switch_complete_input[gas_id].config = TRM_ANT_SWITCH_DIV_CONFIG_0 ;
  }
}

void clear_pending_asdiv_switch_request( gas_id_t gas_id )
{

 pending_asdiv_switch_request[gas_id] = FALSE ;
}

boolean read_pending_asdiv_switch_request( gas_id_t gas_id )
{

return pending_asdiv_switch_request [gas_id]  ;
}
#endif /* FEATURE_GSM_QSC_TX_DIV */

#if defined (FEATURE_DUAL_ACTIVE) && defined(FEATURE_SGLTE_DUAL_SIM)

/*===========================================================================

FUNCTION gl1_handle_multi_sim_mode_switch

DESCRIPTION
  This function checks if a multi mode switch is required.
  If mmode switch needed and both apps are not Active make
  the switch and update the global geran multi mode state

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Must be called with mutex lock
===========================================================================*/
static void gl1_handle_multi_sim_mode_switch( void )
{
  int8 i;
  gl1_trm_state_t trm_status = GL1_TRM_NULL;
  if (FALSE == gl1_handle_mmode_switch.gl1_switch_multi_sim_mode)
  {
    /* no switch required */
    return;
  }

  if ((GFW_APP_MODE_ACTIVE == gl1_get_gfw_app_mode(GERAN_ACCESS_STRATUM_ID_1))
      ||  (GFW_APP_MODE_ACTIVE == gl1_get_gfw_app_mode(GERAN_ACCESS_STRATUM_ID_2)))
  {
    /* one or both apps are active - cant make the mmode switch */
    return;
  }
  /* If here then both apps are not active and a mmode switch is required*/
  gl1_handle_mmode_switch.gl1_switch_multi_sim_mode = FALSE;
  gl1_msg_set_multi_sim_sys_mode(gl1_handle_mmode_switch.gl1_msg_new_switched_multi_sim_mode);
  if( gl1_msg_get_multi_sim_sys_mode() != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE )
  {
    /* entering non dsda state so set sglte mode false as this is for dsda only.*/
    gl1_hw_set_sglte_mode(FALSE, GERAN_ACCESS_STRATUM_ID_1);
    for (i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++)
    {
      trm_status = grm_get_trm_status(l1_tsk_buffer[i].client_id, GERAN_MAP_DATA_SPACE_INDEX_TO_GAS_ID(i));
      if ((GL1_TRM_GRANTED == trm_status) ||
          (GL1_TRM_RETAINED_FOR_ACCESS == trm_status))

      {
        gl1_handle_mmode_switch.trm_status_granted[i] = TRUE;
      }
    }

  }
  MSG_HIGH("Device Mode: %d", gl1_msg_get_multi_sim_sys_mode(),0,0);
  return;
}

/*===========================================================================

FUNCTION gl1_set_switch_multi_sim_mode

DESCRIPTION
  This function is called when there is a device mode indication from GRR
  indicating that a multi mode switch is required. Set state to be switched and
  indicate that a switch is required.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_set_switch_multi_sim_mode(sys_modem_device_mode_e_type new_multi_mode)
{
  gl1_hw_app_mode_cfg_mutex_lock(GERAN_ACCESS_STRATUM_ID_1);
  gl1_hw_app_mode_cfg_mutex_lock(GERAN_ACCESS_STRATUM_ID_2);
  /* mutex to prevent race conditions*/
  gl1_handle_mmode_switch.gl1_switch_multi_sim_mode = TRUE;
  gl1_handle_mmode_switch.gl1_msg_new_switched_multi_sim_mode = new_multi_mode;
  gl1_handle_multi_sim_mode_switch();
  gl1_hw_app_mode_cfg_mutex_unlock(GERAN_ACCESS_STRATUM_ID_1);
  gl1_hw_app_mode_cfg_mutex_unlock(GERAN_ACCESS_STRATUM_ID_2);
}

/*===========================================================================

FUNCTION gl1_is_rf_exit_needed_dsda_to_dsds_switch

DESCRIPTION
  This function is called from gl1_hw_multi_sim_deact to determine if a chain had been
  granted at the time a DEVICE MODE Change DSDA-> DSDS was initiated. In DSDA RF enter
  and exit are called.
  If this function returns TRUE then parrallel RF should be disabled.
  This ensures that rf exit mode is called.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if a chain had been
  granted at the time a DEVICE MODE Change DSDA-> DSDS was initiated

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_is_rf_exit_needed_dsda_to_dsds_switch(void)
{
  int8 i;
  for (i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++)
  {
    if (gl1_handle_mmode_switch.trm_status_granted[i] == TRUE)
    {
      return TRUE;
    }
  }
  return FALSE;
}
#endif /* FEATURE_DUAL_ACTIVE */

/* Run-time debug flag to enable extra asynch interrupt handling messaging */
boolean gl1_is_rf_sleep_disabled( gas_id_t gas_id )
{
  boolean gl1_is_rf_sleep_disbld = FALSE;

  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_SLEEP_RF ) )
  {
    gl1_is_rf_sleep_disbld = TRUE;
  }

  return ( gl1_is_rf_sleep_disbld );
}

boolean gl1_is_fw_sleep_disabled( gas_id_t gas_id )
{
  boolean gl1_is_fw_sleep_disbld = FALSE;

  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_SLEEP_GFW ) )
  {
    gl1_is_fw_sleep_disbld = TRUE;
  }

  return ( gl1_is_fw_sleep_disbld );
}

/* Run-time debug flag to disable extra asynch interrupt handling messaging
   This feature is enabled by default                                       */
boolean gl1_asynch_int_dbg( gas_id_t gas_id )
{
#ifdef FEATURE_GSM_ASYNCH_INT_DEBUG
  boolean gl1_asynch_int_dbg_enabled = TRUE;

  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_ASYNCH_DEBUG ) )
  {
    gl1_asynch_int_dbg_enabled = FALSE;
  }
#else
  boolean gl1_asynch_int_dbg_enabled = FALSE;

  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_ASYNCH_DEBUG ) )
  {
    gl1_asynch_int_dbg_enabled = TRUE;
  }
#endif /* GL1_EFS_SLP_DISABLE_ASYNCH_DEBUG */
  return ( gl1_asynch_int_dbg_enabled );
}

/* Run-time debug flag to disable asynch sleep handling 
   This feature is enabled by default                   */
boolean gl1_is_asynch_sleep_enabled( gas_id_t gas_id )
{
#ifdef FEATURE_ASYNCHRONOUS_SLEEP
  boolean gl1_is_asynch_sleep_ebld = TRUE;

  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_ASYNCH_SLEEP ) )
  {
    gl1_is_asynch_sleep_ebld = FALSE;
  }
#else
  boolean gl1_is_asynch_sleep_ebld = FALSE;

  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_ASYNCH_SLEEP ) )
  {
    gl1_is_asynch_sleep_ebld = TRUE;
  }
#endif /* FEATURE_ASYNCHRONOUS_SLEEP */
  return ( gl1_is_asynch_sleep_ebld );
}

/* Run-time debug flag to disable asynch interrupt handling for SBD only
   Note: Enabling ASYNCH SLEEP will always disable OPT3.
   So by default opt3 is disabled, to enable it via EFS ASYNC sleep bit needs to 
   be disabled */
boolean gl1_is_sleep_opt3_enabled( gas_id_t gas_id )
{
#ifdef FEATURE_GSM_ASYNCH_INT_OPTION3
  boolean gl1_is_sleep_opt3_ebld  = TRUE;

  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_OPTION3 ) )
  {
    gl1_is_sleep_opt3_ebld = FALSE;
  }
#else
  boolean gl1_is_sleep_opt3_ebld = FALSE;

  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_OPTION3 ) )
  {
    gl1_is_sleep_opt3_ebld = TRUE;
  }
#endif /* FEATURE_GSM_ASYNCH_INT_OPTION3 */

  /*
   * Special case as OPT3 is a subset of Asynch Sleep
   * avoid ever having both enabled using any combination of
   * NV and/or build feature
   */
  if ( gl1_is_sleep_opt3_ebld &&
       gl1_is_asynch_sleep_enabled( gas_id ) )
  {
    gl1_is_sleep_opt3_ebld = FALSE;
  }
  return ( gl1_is_sleep_opt3_ebld );
}

/* Run-time debug flag to disable SBD for sub */
boolean gl1_is_sbd_enabled( gas_id_t gas_id )
{
#ifdef FEATURE_GSM_PCH_SINGLE_BURST_DEC
  boolean gl1_is_sbd_ebld = TRUE;

  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_SBD ) )
  {
    gl1_is_sbd_ebld = FALSE;
  }
#else
  boolean gl1_is_sbd_ebld = FALSE;
  
  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_SBD ) )
  {
    gl1_is_sbd_ebld = TRUE;
  }
#endif /* FEATURE_GSM_PCH_SINGLE_BURST_DEC */
  return ( gl1_is_sbd_ebld );
}

/* Run-time debug flag to disable Early Decode for sub */
boolean gl1_is_early_decode_disabled( gas_id_t gas_id )
{
#ifdef FEATURE_DISABLE_EARLY_DECODE
  boolean gl1_is_early_decode_disbld = TRUE;

  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_EARLY_DECODE ) )
  {
    gl1_is_early_decode_disbld = FALSE;
  }
#else
  boolean gl1_is_early_decode_disbld = FALSE;

  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_EARLY_DECODE ) )
  {
    gl1_is_early_decode_disbld = TRUE;
  }
#endif /* FEATURE_DISABLE_EARLY_DECODE */
  return ( gl1_is_early_decode_disbld );
  }

boolean gl1_is_drx_debug_enabled(gas_id_t gas_id )
{
  /* debug en/dis per sub reqd, so used GL1_HW_EFS_DEBUG_ENABLED() insted of GL1_HW_EFS_SLP_ENABLED() */
  return GL1_HW_EFS_DEBUG_ENABLED( GL1_EFS_DEBUG_REDUCE_DRX_CYCLE );  
}

#ifdef GERAN_L1_IP2CAL
/*===========================================================================

FUNCTION  gl1_hw_rf_init_ip2cal

DESCRIPTION
    Send RFA_RF_GSM_INIT_MSM_IP2_CAL_REQ to RF

===========================================================================*/
static void gl1_hw_rf_init_ip2cal(uint32 gl1_gsm_ip2cal_rxlm_buf_idx,uint32 gl1_gsm_ip2cal_txlm_buf_idx,
                                  rfm_device_enum_type device,gas_id_t gas_id)
{
  rfa_rf_gsm_ip2_cal_msm_req_s rfa_rf_gsm_ip2_cal_msm_req;
  rfa_rf_gsm_ip2_cal_msm_cnf_s rfa_rf_gsm_ip2_cal_msm_cnf;
  errno_enum_type result;
  msgr_umid_type rfa_umid_to_send = NULL;
  msgr_umid_type rfa_umid_to_rcv = NULL;
  uint32 bytes_recvd = 0;

  if(gas_id == GERAN_ACCESS_STRATUM_ID_1)
    {
       rfa_umid_to_send = RFA_RF_GSM_INIT_MSM_IP2_CAL_REQ;
       rfa_umid_to_rcv  = RFA_RF_GSM_INIT_MSM_IP2_CAL_CNF;
    }
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
    {
       rfa_umid_to_send = RFA_RF_GSM_INIT_MSM_IP2_CAL_SUB2_REQ;
       rfa_umid_to_rcv  = RFA_RF_GSM_INIT_MSM_IP2_CAL_SUB2_CNF;
    }
#if defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_3)
    {
       rfa_umid_to_send = RFA_RF_GSM_INIT_MSM_IP2_CAL_SUB3_REQ;
       rfa_umid_to_rcv  = RFA_RF_GSM_INIT_MSM_IP2_CAL_SUB3_CNF;
    }
#endif
  else
    {
      MSG_GERAN_FATAL_0_G( "Invalid gas_id" );
    }

  GDRV_RFA_MSGR_LOCK(gas_id);

  msgr_init_hdr(&rfa_rf_gsm_ip2_cal_msm_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);

  /* populate payload */
  rfa_rf_gsm_ip2_cal_msm_req.ip2_rxlm_buffer_idx = (lm_handle_type)gl1_gsm_ip2cal_rxlm_buf_idx;
  rfa_rf_gsm_ip2_cal_msm_req.ip2_txlm_buffer_idx = (lm_handle_type)gl1_gsm_ip2cal_txlm_buf_idx;
  rfa_rf_gsm_ip2_cal_msm_req.device = device;
  rfa_rf_gsm_ip2_cal_msm_req.sub_id = gas_id;
  rfa_rf_gsm_ip2_cal_msm_req.cnf_required = TRUE;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rfa_rf_gsm_ip2_cal_msm_req.hdr, sizeof(rfa_rf_gsm_ip2_cal_msm_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

  MSG_GERAN_HIGH_1_G("gl1_hw_rf_ip2_init: rfa_umid_to_send:0x%x",rfa_umid_to_send);

  /* blocking wait for CNF message from RF Task */
  result = msgr_receive(&gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
                       (uint8 *)&rfa_rf_gsm_ip2_cal_msm_cnf,
                       (uint32)sizeof(rfa_rf_gsm_ip2_cal_msm_cnf),
                       &bytes_recvd);

  gl1_hw_rf_msgr_receive_debug(rfa_umid_to_rcv,
                               result,
                               rfa_rf_gsm_ip2_cal_msm_cnf.hdr.id,
                               rfa_rf_gsm_ip2_cal_msm_cnf.req_result,
                               gas_id);
  GDRV_RFA_MSGR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_rf_ip2_cal

DESCRIPTION
    Send RFA_RF_GSM_START_IP2_CAL_REQ to RF

===========================================================================*/
void gl1_hw_rf_ip2_cal(gas_id_t gas_id)
{
  rfa_rf_gsm_start_ip2_cal_req_s rfa_rf_gsm_start_ip2_cal_req;
  errno_enum_type result;
  msgr_umid_type rfa_umid_to_send = NULL;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  if(gas_id == GERAN_ACCESS_STRATUM_ID_1)
   {
   rfa_umid_to_send = RFA_RF_GSM_START_IP2_CAL_REQ;
   }
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
   {
   rfa_umid_to_send = RFA_RF_GSM_START_IP2_CAL_SUB2_REQ;
   }
#if defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_3)
   {
   rfa_umid_to_send = RFA_RF_GSM_START_IP2_CAL_SUB3_REQ;
   }
#endif
  else
   {
   MSG_GERAN_FATAL_0_G( "Invalid gas_id" );
   }

  msgr_init_hdr(&rfa_rf_gsm_start_ip2_cal_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);

  /* populate payload */
  rfa_rf_gsm_start_ip2_cal_req.device       = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);
  rfa_rf_gsm_start_ip2_cal_req.sub_id       = gas_id;
  rfa_rf_gsm_start_ip2_cal_req.cnf_required = TRUE;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  result = msgr_send(&rfa_rf_gsm_start_ip2_cal_req.hdr, sizeof(rfa_rf_gsm_start_ip2_cal_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

  MSG_GERAN_HIGH_0_G("RFA_RF_GSM_START_IP2_CAL_REQ sent");
}

/*===========================================================================

FUNCTION gl1_hw_deallocate_ip2cal_rxlm_buf_idx

DESCRIPTION
  Returns None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_deallocate_ip2cal_rxlm_buf_idx( gas_id_t gas_id )
{
   if (gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx != MDSP_RXLM_BUF_IDX_INVALID)
   {
      if (!rflm_deallocate_rx_handle(LM_GSM, LM_GSM, gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx))
      {
        MSG_GERAN_ERROR_1_G("Unable to de-allocate IP2Cal RxLM buffer %d",gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx);
      }
      else
      {
        MSG_GERAN_HIGH_1_G("GSM IP2Cal RxLM buffer %d dealloacted",gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx);
      }
      gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx = MDSP_RXLM_BUF_IDX_INVALID;
   }
}

/*===========================================================================

FUNCTION gl1_hw_deallocate_ip2cal_txlm_buf_idx

DESCRIPTION
  Returns None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_deallocate_ip2cal_txlm_buf_idx(gas_id_t gas_id)
{
   if (gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx != MDSP_TXLM_BUF_IDX_INVALID)
   {
     if (!rflm_deallocate_tx_handle(LM_GSM, LM_GSM,gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx))
     {
       MSG_GERAN_ERROR_1_G("Unable to de-allocate IP2Cal TxLM buffer %d",gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx);
     }
     else
     {
       MSG_GERAN_HIGH_1_G("GSM IP2Cal TxLM buffer %d deallocated",gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx);
     }
     gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx = MDSP_TXLM_BUF_IDX_INVALID;
   }
}

/*===========================================================================

FUNCTION  gl1_hw_get_ip2cal_support

DESCRIPTION
    Returns TRUE if IP2Cal is supported by WTR

===========================================================================*/
boolean gl1_hw_get_ip2cal_support(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  rfcom_device_enum_type device = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);

  if( device >= RFM_MAX_WAN_DEVICES )
  {
    MSG_GERAN_LOW_0_G("Invalid WAN device");
    return(FALSE);
  }
  else
  {
    return(gl1_hw_data_ptr[gas_id]->ip2cal_supported[device]);
  }
}

/*===========================================================================

FUNCTION  gl1_hw_get_ip2cal_rxlm_buff_index

DESCRIPTION
    Returns RXLM buffer index for IP2Cal

===========================================================================*/
uint32 gl1_hw_get_ip2cal_rxlm_buff_index(gas_id_t gas_id)
{
return(gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_rxlm_buf_idx);
}

/*===========================================================================

FUNCTION  gl1_hw_get_ip2cal_txlm_buff_index

DESCRIPTION
    Returns TXLM buffer index for IP2Cal

===========================================================================*/
uint32 gl1_hw_get_ip2cal_txlm_buff_index(gas_id_t gas_id)
{
return(gl1_hw_data_ptr[gas_id]->gl1_gsm_ip2cal_txlm_buf_idx);
}
#endif /*GERAN_L1_IP2CAL*/

#ifdef GERAN_L1_HLLL_LNASTATE
void gl1_hw_update_dtm_metrics(gl1_defs_metrics_lnastate *gl1_metrics_lnastate,uint32 fn,
                               gl1_hw_update_metrics_t update_metrics,gas_id_t gas_id)
{
  uint8 i = 0;

  switch(update_metrics)
  {
  case UPDATE_CS_METRICS:
     gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.updated_cs_metrics = TRUE;
     gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.fn_cs_metrics      = fn;
     gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.cs_metrics.NoOfChannels = gl1_metrics_lnastate->NoOfChannels;
     for(i=0;i<gl1_metrics_lnastate->NoOfChannels;i++)
      {
      gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.cs_metrics.gsm_metrics[i].arfcn       = gl1_metrics_lnastate->gsm_metrics[i].arfcn;
      gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.cs_metrics.gsm_metrics[i].rx_lvl_dbm  = gl1_metrics_lnastate->gsm_metrics[i].rx_lvl_dbm;
      gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.cs_metrics.gsm_metrics[i].snr         = gl1_metrics_lnastate->gsm_metrics[i].snr;
      gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.cs_metrics.gsm_metrics[i].jdet_value  = gl1_metrics_lnastate->gsm_metrics[i].jdet_value;
      }
     break;

  case UPDATE_PS_METRICS:
     gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.updated_ps_metrics = TRUE;
     gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.fn_ps_metrics      = fn;
     gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.ps_metrics.NoOfChannels = gl1_metrics_lnastate->NoOfChannels;
     for(i=0;i<gl1_metrics_lnastate->NoOfChannels;i++)
      {
      gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.ps_metrics.gsm_metrics[i].arfcn       = gl1_metrics_lnastate->gsm_metrics[i].arfcn;
      gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.ps_metrics.gsm_metrics[i].rx_lvl_dbm  = gl1_metrics_lnastate->gsm_metrics[i].rx_lvl_dbm;
      gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.ps_metrics.gsm_metrics[i].snr         = gl1_metrics_lnastate->gsm_metrics[i].snr;
      gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.ps_metrics.gsm_metrics[i].jdet_value  = gl1_metrics_lnastate->gsm_metrics[i].jdet_value;
      }
     break;

  default:
     MSG_GERAN_HIGH_0_G("In DTM burst metrics were not updated for making HL/LL LNA state decision");
     break;
  }
}

void gl1_hw_reset_dtm_metrics(gas_id_t gas_id)
{
  memset(&gl1_defs_dtm_metrics[gas_id],0,sizeof(gl1_defs_dtm_metrics_t));
  MSG_GERAN_LOW_1_G("DTM burst metrics for HL/LL decision cleared in FN:%d",
                     GSTMR_GET_FN_GERAN(gas_id));
}

void gl1_hw_send_dtm_burst_metrics(uint32 fn,gas_id_t gas_id)
{
  gl1_defs_metrics_lnastate gl1_metrics_lnastate;
  uint8 i = 0;
  uint8 j = 0;
  uint8 k = 0;

  gl1_metrics_lnastate.NoOfChannels = 0;

  if((gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.updated_cs_metrics)
      &&
     (fn == gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.fn_cs_metrics))
   {
   gl1_metrics_lnastate.NoOfChannels += gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.cs_metrics.NoOfChannels;
   for(i=0;i<gl1_metrics_lnastate.NoOfChannels;i++)
     {
     gl1_metrics_lnastate.gsm_metrics[i].arfcn      = gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.cs_metrics.gsm_metrics[i].arfcn;
     gl1_metrics_lnastate.gsm_metrics[i].rx_lvl_dbm = gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.cs_metrics.gsm_metrics[i].rx_lvl_dbm;
     gl1_metrics_lnastate.gsm_metrics[i].snr        = gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.cs_metrics.gsm_metrics[i].snr;
     gl1_metrics_lnastate.gsm_metrics[i].jdet_value = gl1_defs_dtm_metrics[gas_id].gl1_hw_cs_metrics.cs_metrics.gsm_metrics[i].jdet_value;
     }
   }

  if((gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.updated_ps_metrics)
      &&
     (fn == gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.fn_ps_metrics))
   {
   gl1_metrics_lnastate.NoOfChannels += gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.ps_metrics.NoOfChannels;
   for(j=i,k=0;k<gl1_metrics_lnastate.NoOfChannels;j++,k++)
{
     gl1_metrics_lnastate.gsm_metrics[j].arfcn      = gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.ps_metrics.gsm_metrics[k].arfcn;
     gl1_metrics_lnastate.gsm_metrics[j].rx_lvl_dbm = gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.ps_metrics.gsm_metrics[k].rx_lvl_dbm;
     gl1_metrics_lnastate.gsm_metrics[j].snr        = gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.ps_metrics.gsm_metrics[k].snr;
     gl1_metrics_lnastate.gsm_metrics[j].jdet_value = gl1_defs_dtm_metrics[gas_id].gl1_hw_ps_metrics.ps_metrics.gsm_metrics[k].jdet_value;
     }
}

  if(gl1_metrics_lnastate.NoOfChannels)
   {
   gl1_hw_rf_burst_metrics(&gl1_metrics_lnastate,gas_id);

   /*Clear DTM burst metrics after sending RFA_RF_GSM_GET_METRICS_REQ to RF*/
   gl1_hw_reset_dtm_metrics(gas_id);
   }

}

void gl1_hw_rf_burst_metrics(gl1_defs_metrics_lnastate *gl1_metrics_lnastate,
gas_id_t gas_id)
{
  rfa_rf_gsm_process_burst_metrics_req_s rfa_rf_gsm_metrics;
  rfa_rf_gsm_rx_burst_metrics_type *rf_burst_metrics_ptr = NULL;
  errno_enum_type result;
  msgr_umid_type rfa_umid_to_send = NULL;
  uint8 i = 0;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];


  if(gas_id == GERAN_ACCESS_STRATUM_ID_1)
   {
   rfa_umid_to_send = RFA_RF_GSM_PROCESS_BURST_METRICS_REQ;
   }
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
   {
   rfa_umid_to_send = RFA_RF_GSM_PROCESS_BURST_METRICS_SUB2_REQ;
   }
#if defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_3)
 {
   rfa_umid_to_send = RFA_RF_GSM_PROCESS_BURST_METRICS_SUB3_REQ;
 }
#endif
 else
 {
   MSG_GERAN_FATAL_0_G( "Invalid gas_id" );
 }

  msgr_init_hdr(&rfa_rf_gsm_metrics.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);

  /* populate payload */
  rfa_rf_gsm_metrics.device          = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);
  rfa_rf_gsm_metrics.sub_id          = gas_id;
  rfa_rf_gsm_metrics.cnf_required    = TRUE;
  rfa_rf_gsm_metrics.num_metrics_entries    = gl1_metrics_lnastate->NoOfChannels;
  MSG_GERAN_HIGH_1_G("NoOfChannels:%d",gl1_metrics_lnastate->NoOfChannels);

  if( rfa_rf_gsm_metrics.num_metrics_entries > MAX_NUMBER_OF_CHANNELS)
   {
   MSG_GERAN_ERROR_2_G("NoOfChannels %d greater than max number allowed:%d", rfa_rf_gsm_metrics.num_metrics_entries,
                       MAX_NUMBER_OF_CHANNELS);
}

  for(i=0;i<rfa_rf_gsm_metrics.num_metrics_entries;i++)
{

   MSG_GERAN_HIGH_1_G("Index:%d",i);

   rf_burst_metrics_ptr = &gl1_hw_data_ptr[gas_id]->metrics[gl1_hw_data_ptr[gas_id]->gl1_hw_gsm_burst_metrics_buf_index][i];
   rfa_rf_gsm_metrics.metrics[i] = rf_burst_metrics_ptr;
   rfa_rf_gsm_metrics.metrics[i]->band         = gl1_hw_arfcn_to_rfgsm_band(gl1_metrics_lnastate->gsm_metrics[i].arfcn);
   rfa_rf_gsm_metrics.metrics[i]->arfcn        = gl1_metrics_lnastate->gsm_metrics[i].arfcn.num;
   rfa_rf_gsm_metrics.metrics[i]->rx_lvl_dbm   = gl1_metrics_lnastate->gsm_metrics[i].rx_lvl_dbm;
   rfa_rf_gsm_metrics.metrics[i]->snr          = gl1_metrics_lnastate->gsm_metrics[i].snr;
   rfa_rf_gsm_metrics.metrics[i]->jdet_value   = gl1_metrics_lnastate->gsm_metrics[i].jdet_value;
   }

  gl1_hw_data_ptr[gas_id]->gl1_hw_gsm_burst_metrics_buf_index++;
  gl1_hw_data_ptr[gas_id]->gl1_hw_gsm_burst_metrics_buf_index &= 0x07;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  result = msgr_send(&rfa_rf_gsm_metrics.hdr, sizeof(rfa_rf_gsm_metrics));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

  MSG_GERAN_HIGH_0_G("RFA_RF_GSM_PROCESS_METRICS_REQ is sent");

}

/*===========================================================================

FUNCTION  gl1_hw_gsm_band_supported

DESCRIPTION
  Check if the requested band is supported by the diversity Rx device

===========================================================================*/
boolean gl1_hw_get_sawless_HLLL_support(sys_band_T band,gas_id_t gas_id)
{
  boolean band_supported = FALSE;
  rfcom_device_enum_type device;
 volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

device = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);

  if( device >= RFM_MAX_WAN_DEVICES )
  {
    MSG_GERAN_LOW_0_G("invalid WAN device");
    return (band_supported);
  }

  switch(band)
  {
    case SYS_BAND_EGSM_900:
    case SYS_BAND_PGSM_900:
        band_supported = (gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] & SAWLESS_SUPPORT_GSM_BAND_900)?TRUE:FALSE;
        break;

    case SYS_BAND_DCS_1800:
        band_supported = (gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] & SAWLESS_SUPPORT_GSM_BAND_1800)?TRUE:FALSE;
        break;

    case SYS_BAND_PCS_1900:
        band_supported = (gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] & SAWLESS_SUPPORT_GSM_BAND_1900)?TRUE:FALSE;
        break;

    case SYS_BAND_CELL_850:
        band_supported = (gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] & SAWLESS_SUPPORT_GSM_BAND_850)?TRUE:FALSE;
        break;

    default:
        band_supported = FALSE;
        break;
  }
  return (band_supported);
}

/*===========================================================================

FUNCTION  gl1_hw_reset_sawless_support_flag

DESCRIPTION
  Resets sawless support flags.

===========================================================================*/
void gl1_hw_reset_sawless_support_flag(gas_id_t gas_id)
{
  rfcom_device_enum_type device;

  for(device=RFM_DEVICE_0;device<RFM_MAX_WAN_DEVICES;device++)
   {
   gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] = 0;
   }
}
#endif /*GERAN_L1_HLLL_LNASTATE*/

/*===========================================================================

 FUNCTION  gl1_get_gfw_imrd_entermodetype

 DESCRIPTION
 GFW ENTER MODE TYPE, based on Idle RxD inputs.

 ===========================================================================*/
#ifdef FEATURE_GSM_RX_TX_SPLIT
uint8 gl1_hw_get_gfw_entermode_type(uint8 wakeup_mode, gas_id_t gas_id)
#else
GfwEnterModeType gl1_hw_get_gfw_entermode_type(uint8 wakeup_mode, gas_id_t gas_id)
#endif
{
#ifdef FEATURE_GSM_RX_DIVERSITY
  gl1_hw_rf_sleep_wakeup_mode_type sleep_wake_up_type = gl1_hw_get_rf_sleep_wakeup_mode(gas_id);
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t divrx_client = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);

  if ((GL1_TRM_GRANTED == grm_get_trm_status(divrx_client, gas_id))
      && (MDSP_RXLM_BUF_IDX_INVALID != gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id))
#ifdef FEATURE_GSM_RX_TX_SPLIT
      &&(RFGSM_PRX_DRX_OP_MODE == wakeup_mode) 
#else
      &&(RFGSM_PRX_DRX_WAKEUP_MODE == wakeup_mode)
#endif
      )
{
    MSG_GERAN_LOW_1_G(GL1_RXD"COMBINED PRX/DRX GFW enter_mode_cmd agc_Rxd=%i",
                       gl1_get_rxdiv_agc_start(gas_id));

    gl1_hw_set_gfw_enter_status(GL1_GFW_PRX_DIVRX_ENTER,gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
    /* default RxD for Data state to TRUE */
    if (gl1_get_MSRD_ctl_nv(gas_id))
    {
       gl1_set_enable_rxdiversity_data(TRUE, gas_id);
       gl1_set_enable_rxdiversity_data_dynamic(TRUE, gas_id);
    }
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */
#ifdef FEATURE_GSM_RX_TX_SPLIT
    return (GFW_ENTER_MODE_PRX_BIT | GFW_ENTER_MODE_DRX_BIT);
#else
    return (GFW_ENTER_MODE_COMBINED);
#endif
  }
  else
  {
    MSG_GERAN_LOW_1_G("PRX GFW enter_mode_cmd agc_Rxd=%i",
                         gl1_get_rxdiv_agc_start(gas_id));
      gl1_set_enable_rxdiversity(FALSE,gas_id);
    gl1_hw_set_gfw_enter_status(GL1_GFW_PRX_ENTER,gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
    /* default RxD for Data state to TRUE */
    if (gl1_get_MSRD_ctl_nv(gas_id))
    {
       gl1_set_enable_rxdiversity_data(FALSE, gas_id);
       gl1_set_enable_rxdiversity_data_dynamic(FALSE, gas_id);
      }
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */
#ifdef FEATURE_GSM_RX_TX_SPLIT
    return (GFW_ENTER_MODE_PRX_BIT);
#else
      return (GFW_ENTER_MODE_PRX);
#endif
   }
#else
#ifdef FEATURE_GSM_RX_TX_SPLIT
  return (GFW_ENTER_MODE_PRX_BIT);
#else
  return (GFW_ENTER_MODE_PRX);
#endif

#endif //FEATURE_GSM_RX_DIVERSITY
}

/*===========================================================================

 FUNCTION  gl1_get_prx_ftm_dev

 DESCRIPTION
Returns ftm_device stored locally.

 ===========================================================================*/
rfm_device_enum_type gl1_get_prx_ftm_dev(gas_id_t gas_id)
{
#ifdef FEATURE_GSM_RX_TX_SPLIT
    return ftm_devices[gas_id].prx;
#else
    return ftm_device[gas_id];
#endif
}
#ifdef FEATURE_GSM_RX_TX_SPLIT
#ifdef FEATURE_GSM_RX_DIVERSITY
/*===========================================================================

 FUNCTION  gl1_get_drx_ftm_dev

 DESCRIPTION
Returns ftm_device stored locally.

 ===========================================================================*/
rfm_device_enum_type gl1_get_drx_ftm_dev(gas_id_t gas_id)
{
    return ftm_devices[gas_id].drx;
  }

/*===========================================================================

 FUNCTION  gl1_get_drx_ftm_band_support

 DESCRIPTION
Returns ftm_device stored locally.

 ===========================================================================*/
uint16 gl1_get_drx_ftm_band_support(gas_id_t gas_id, int16 band)
  {
    return ftm_devices[gas_id].drx_band_support[band] << band;
}
#endif /*FEATURE_GSM_RX_DIVERSITY*/
/*===========================================================================

 FUNCTION  gl1_get_tx_ftm_dev

 DESCRIPTION
Returns ftm_device stored locally.

 ===========================================================================*/
rfm_device_enum_type gl1_get_tx_ftm_dev(gas_id_t gas_id)
    {
    return ftm_devices[gas_id].tx;
}

/*===========================================================================

FUNCTION  gl1_hw_rf_set_ftm_devices

DESCRIPTION
  Sets PRx, DRx & Tx RFM devices for FTM.

===========================================================================*/
void gl1_hw_rf_set_ftm_devices(geran_test_mode_api_device_type devices, gas_id_t gas_id)
{
   uint16 i;

   ftm_devices[gas_id].prx = devices.prx;
   ftm_devices[gas_id].drx = devices.drx;
   ftm_devices[gas_id].tx  = devices.tx;

   for (i=0;i<RFGSM_MAX_UNIQUE_RFCOM_BANDS;i++)
   {
      ftm_devices[gas_id].drx_band_support[i] = devices.drx_band_support[i];
   }
}


void gl1_hw_rf_set_ftm_devices_chain_select(geran_test_mode_api_device_type devices, gas_id_t gas_id)
{
   uint16 i;

  gl1_rxd_control_type rxd_ctl_flag;

  MSG_GERAN_HIGH_1_G("ftm rx chain %d", devices.ftm_rxchain );

  switch( devices.ftm_rxchain )
  {
  case  GSM_ONLY_PRX_ENABLED:

      rxd_ctl_flag.enablePrx = TRUE;
      rxd_ctl_flag.RxdForcedOn = FALSE;
      rxd_ctl_flag.enableMSRD  = FALSE;
      rxd_ctl_flag.enableIMRD  = FALSE;
      rxd_ctl_flag.enableDrx   = FALSE;
      rxd_ctl_flag.enableRxd   = FALSE;
      rxd_ctl_flag.threshold   = 0;

      /*Update the RxD control flags*/
      gl1_set_rxd_control_flags(rxd_ctl_flag,gas_id);

      break;

  case  GSM_ONLY_DRX_ENABLED:

      rxd_ctl_flag.enablePrx   = FALSE;
      rxd_ctl_flag.RxdForcedOn = TRUE;
      rxd_ctl_flag.enableMSRD  = TRUE;
      rxd_ctl_flag.enableIMRD  = TRUE;
      rxd_ctl_flag.enableDrx   = TRUE;
      rxd_ctl_flag.enableRxd   = TRUE;
      rxd_ctl_flag.threshold   = 0;

      /*Update the RxD control flags*/
      gl1_set_rxd_control_flags(rxd_ctl_flag,gas_id);

      break;

  case  GSM_BOTH_PRX_DRX_ENABLED:

      rxd_ctl_flag.enablePrx   = TRUE;
      rxd_ctl_flag.RxdForcedOn = TRUE;
      rxd_ctl_flag.enableMSRD  = TRUE;
      rxd_ctl_flag.enableIMRD  = TRUE;
      rxd_ctl_flag.enableDrx   = TRUE;
      rxd_ctl_flag.enableRxd   = TRUE;
      rxd_ctl_flag.threshold   = 0;

      /*Update the RxD control flags*/
      gl1_set_rxd_control_flags(rxd_ctl_flag,gas_id);

      break;

  case GSM_NO_ACTION:
      /* Nothing to do here, the NV item will take over */
      break;

  default:
      MSG_GERAN_ERROR_1_G("invalid ftm rx chain %d", devices.ftm_rxchain );
      break;
  } 

   ftm_devices[gas_id].prx = devices.prx;
   ftm_devices[gas_id].drx = devices.drx;
   ftm_devices[gas_id].tx  = devices.tx;

   for (i=0;i<RFGSM_MAX_UNIQUE_RFCOM_BANDS;i++)
   {
      ftm_devices[gas_id].drx_band_support[i] = devices.drx_band_support[i];
   }
}

/*===========================================================================

FUNCTION  gl1_hw_rf_reset_ftm_devices

DESCRIPTION
  Resets PRx, DRx & Tx RFM devices after FTM activity stops.

===========================================================================*/
void gl1_hw_rf_reset_ftm_devices(gas_id_t gas_id)
{
   uint16 i;

   ftm_devices[gas_id].prx = RFM_INVALID_DEVICE;
   ftm_devices[gas_id].drx = RFM_INVALID_DEVICE;
   ftm_devices[gas_id].tx  = RFM_INVALID_DEVICE;

   for (i=0;i<RFGSM_MAX_UNIQUE_RFCOM_BANDS;i++)
   {
      ftm_devices[gas_id].drx_band_support[i] = 0;
   }
}
#endif

/*===========================================================================

FUNCTION gl1_hw_rf_wakeup_req_msg_check / gl1_hw_set_rf_wakeup_req_msg

DESCRIPTION
  This function returns if a rf_wakeup_req was sent for this wake/sleep cycle
  or not. Cycles without an RF awake are not used in the adaptive timeline
  averaging algorithm as they will be unusually short.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_rf_wakeup_req_msg_check(gas_id_t gas_id )
{
  return gl1_hw_rf_wakeup_req_msg[gas_id];
}


void gl1_hw_set_rf_wakeup_req_msg(gas_id_t gas_id, boolean rfWakeUpSent)
{
  gl1_hw_rf_wakeup_req_msg[gas_id] = rfWakeUpSent;
}
/*===========================================================================

FUNCTION gl1_hw_set_gfw_config_cycle_valid_state

DESCRIPTION
  Sets the config state

DEPENDENCIES
  None

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/

void gl1_hw_set_gfw_config_cycle_valid_state(gas_id_t gas_id, boolean isValid)
{
  gl1_hw_data_ptr[gas_id]->gl1_hw_gfw_freq_config.update = isValid;
}

/*===========================================================================
FUNCTION  gl1_hw_get_next_rf_seq_num
DESCRIPTION
  Returns next number in sequence for RF seq num (range 1 to 0xFFFD)
===========================================================================*/
#define GL1_HW_RF_SEQ_NUM_MAX 0xFFFE
uint32 gl1_hw_get_next_rf_seq_num(void)
{
   static uint32 gl1_hw_rf_seq_num = 0; /* static_dual_spaced_ignore */

   gl1_hw_rf_seq_num += 1;
   if (gl1_hw_rf_seq_num == GL1_HW_RF_SEQ_NUM_MAX)
   {
      gl1_hw_rf_seq_num = 1;
   }

   return (gl1_hw_rf_seq_num);
}

/*===========================================================================

FUNCTION is_hw_tx_on

DESCRIPTION
  return hw_hw_tx_on status

DEPENDENCIES
  None

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/

boolean is_hw_tx_on (gas_id_t gas_id )
{
 return gl1_hw_tx_on[gas_id] ;
}

#ifndef FEATURE_GSM_RX_TX_SPLIT
#ifdef FEATURE_GSM_RX_DIVERSITY
/*===========================================================================

FUNCTION  gl1_hw_rf_wakeup_divrx

DESCRIPTION
  Send MSGR message to RF Task to run RF Wakeup. Use MSGR blocking wait
  to receive wakeup completion confimation from RF Task before continuing

===========================================================================*/
static void gl1_hw_rf_wakeup_divrx(gas_id_t gas_id)
{
  rfa_rf_gsm_wakeup_req_s rf_gsm_wakeup_req;
  errno_enum_type result;
  msgr_umid_type rfa_umid_to_send = NULL;
  rfm_device_enum_type divrx_rf_device;
  grm_client_enum_t divrx_client = GRM_NO_CLIENT;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = NULL;

  gas_id = check_gas_id(gas_id);

  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  divrx_client = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);

  /*No grant: no need to wakeup*/
  if(grm_get_trm_status(divrx_client, gas_id) != GL1_TRM_GRANTED)
  {
    return;
  }

  divrx_rf_device = gl1_hw_get_gsm_rf_id(divrx_client, gas_id);

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_WAKEUP_REQ;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_WAKEUP_SUB2_REQ;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
   {
    rfa_umid_to_send = RFA_RF_GSM_WAKEUP_SUB3_REQ;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initial wakeup msgr header */
  msgr_init_hdr(&rf_gsm_wakeup_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */

  rf_gsm_wakeup_req.sbi_init_writes_buff_ptr = (void*)mdsp_get_gfw_init_event_buffer(gas_id);

  rf_gsm_wakeup_req.buffer_id = 0;
  rf_gsm_wakeup_req.sub_id = gas_id;
  rf_gsm_wakeup_req.cnf_required = TRUE;
  rf_gsm_wakeup_req.rxlm_buf_index.prx_idx = -1;
  rf_gsm_wakeup_req.prx_device = RFM_INVALID_DEVICE;
  rf_gsm_wakeup_req.rxlm_buf_index.drx_idx = (lm_handle_type) (gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id));
  rf_gsm_wakeup_req.drx_device = divrx_rf_device;
  rf_gsm_wakeup_req.wakeup_mode = RFGSM_DRX_WAKEUP_MODE;
  rf_gsm_wakeup_req.timing_info = NULL;

  MSG_GERAN_HIGH_3_G(GL1_RXD"gl1_hw_rf_wakeup_DivRx drx_idx=%i drx_device=%i, wakeup_mode=%i",
                     rf_gsm_wakeup_req.rxlm_buf_index.drx_idx,
                     rf_gsm_wakeup_req.drx_device,
                     rf_gsm_wakeup_req.wakeup_mode);

  GDRV_RFA_MSGR_LOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rf_gsm_wakeup_req.hdr, sizeof(rf_gsm_wakeup_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);


  GDRV_RFA_MSGR_UNLOCK(gas_id);

}
#endif /*FEATURE_GSM_RX_DIVERSITY*/
#endif
#ifdef FEATURE_WLAN_COEX_SW_CXM
/*===========================================================================

FUNCTION  gl1_hw_async_intf_cxm_policy_cmd

DESCRIPTION
  Send MSGR message to GFW with active CXM policy

===========================================================================*/
boolean gl1_hw_async_intf_cxm_policy_cmd( cxm_active_config_v01_s policy, gas_id_t gas_id )
{
  if( mdsp_awake(gas_id) )
  {
    GfwWlanCxmConfigCmd gfwWlanCxmConfigCmd;
    gfwWlanCxmConfigCmd.message = policy;

    MSG_GERAN_HIGH_1_G("GFW update active_cxm_policy %x",gfwWlanCxmConfigCmd.message.active_cxm_policy);

    /* send msgr message to GFW */
    mdsp_async_intf_send_blocking_cmd(GERAN_FW_WLAN_CXM_POLICY_CMD,
                  &gfwWlanCxmConfigCmd,
                  NULL,
                  gas_id);

    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
#endif /* FEATURE_WLAN_COEX_SW_CXM */
#ifdef FEATURE_GSM_RX_TX_SPLIT
/*===========================================================================

FUNCTION  gl1_hw_rf_start_gsm

DESCRIPTION
  Send MSGR message to RF Task to run START GSM. This initialises GRF SW
  data structures, but does not enable nor interact with the WTR.

===========================================================================*/
void gl1_hw_rf_start_gsm(gas_id_t gas_id)
{
  rfa_rf_gsm_start_gsm_req_s rfa_rf_gsm_start_gsm_req;
  errno_enum_type result;
  rfa_rf_gsm_start_gsm_cnf_s rfa_rf_gsm_start_gsm_cnf;
  uint32 bytes_recvd;
  msgr_umid_type rfa_umid_to_send = NULL;
  msgr_umid_type rfa_umid_to_rcv = NULL;
  int16 index;

#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  uint32 use_idle_rx_alpha = (uint32)FALSE;
#endif

#ifdef GERAN_L1_HLLL_LNASTATE
  uint8 i = 0;
  rfcom_device_enum_type device;
#endif

  gas_id = check_gas_id(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
   /*
         DR-DSDS: in DR mode idle mode RX alpha must be used to cope with RXFE bus contention
         issues. Ensure value is correct before calling Rf enter mode.
     */
  if (((l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE) ||
       (l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH)) &&
       gl1_drdsds_enabled(gas_id))
  {
    use_idle_rx_alpha = TRUE;

  }
  /*indicate to GFW correct RX alpha to use*/
  mdsp_update_shared_variable(GFW_RX_ALPHA,&use_idle_rx_alpha,gas_id);

#endif

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_START_GSM_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_START_GSM_CNF;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_START_GSM_SUB2_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_START_GSM_SUB2_CNF;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
  else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
  {
    rfa_umid_to_send = RFA_RF_GSM_START_GSM_SUB3_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_START_GSM_SUB3_CNF;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initialise msgr header */
  msgr_init_hdr(&rfa_rf_gsm_start_gsm_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */
  rfa_rf_gsm_start_gsm_req.sub_id       = gas_id;
  rfa_rf_gsm_start_gsm_req.cnf_required = TRUE;

  GDRV_RFA_MSGR_LOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rfa_rf_gsm_start_gsm_req.hdr, sizeof(rfa_rf_gsm_start_gsm_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

   /* blocking wait for IRAT params CNF message from RF Task */
  result = msgr_receive(&gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
                        (uint8 *)&rfa_rf_gsm_start_gsm_cnf,
                        (uint32)sizeof(rfa_rf_gsm_start_gsm_cnf_s),
                        &bytes_recvd);
  GDRV_RFA_MSGR_UNLOCK(gas_id);

  /* sanity check and print debug for msgr_receive */
  gl1_hw_rf_msgr_receive_debug(rfa_umid_to_rcv,
                               result,
                               rfa_rf_gsm_start_gsm_cnf.hdr.id,
                               rfa_rf_gsm_start_gsm_cnf.req_result,
                               gas_id);

#if defined ( GERAN_L1_HLLL_LNASTATE ) || defined ( GERAN_L1_IP2CAL )
  for(device=RFM_DEVICE_0;device<RFM_MAX_WAN_DEVICES;device++)
  {
    for(i=0;i<(RFCOM_NUM_GSM_BANDS-1);i++)
    {
      gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] |= (rfa_rf_gsm_start_gsm_cnf.sawless_sup[device][i] << i);
    }

    if (gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] != 0)
    {
      MSG_GERAN_HIGH_2_G("Sawless support for band:0x%x for device:%d",
                         gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device],
                         device);
    }
#ifdef GERAN_L1_IP2CAL
    gl1_hw_data_ptr[gas_id]->ip2cal_supported[device] = (gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] != 0)?TRUE:FALSE;
#endif /*GERAN_L1_IP2CAL*/
  }
#endif /*GERAN_L1_HLLL_LNASTATE || GERAN_L1_IP2CAL*/

  /* Initialise current GSM band as INVALID on initialising RF */
  gl1_hw_data_ptr[gas_id]->gl1_hw_rf_gsm_band = (int8) RFGSM_BAND_NONE;

  /* Initialise RF Task Rx burst buffers */
  for (index=0; index<GL1_HW_RF_BURST_RX_BUF_NUM; index++)
  {
    gl1_hw_data_ptr[gas_id]->gl1_hw_rf_rx_burst_buf[index].num_rx_bursts = GL1_HW_RF_BURST_NUM_BURSTS_INVALID;
  }
  /* Initialise RF Task Tx burst buffers */
  for (index=0; index<GL1_HW_RF_BURST_TX_BUF_NUM; index++)
  {
    gl1_hw_data_ptr[gas_id]->gl1_hw_rf_tx_burst_buf[index].num_tx_slots = GL1_HW_RF_BURST_NUM_BURSTS_INVALID;
  }
}

/*===========================================================================

FUNCTION  gl1_hw_rf_stop_gsm

DESCRIPTION
  Send MSGR message to RF Task to run STOP GSM. This cleans GRF SW
  data structures, but does not disable nor interact with the WTR.

===========================================================================*/
void gl1_hw_rf_stop_gsm(gas_id_t gas_id)
{
  rfa_rf_gsm_stop_gsm_req_s rfa_rf_gsm_stop_gsm_req;
  errno_enum_type result;
  rfa_rf_gsm_stop_gsm_cnf_s rfa_rf_gsm_stop_gsm_cnf;
  uint32 bytes_recvd;
  msgr_umid_type rfa_umid_to_send = NULL;
  msgr_umid_type rfa_umid_to_rcv = NULL;

  gas_id = check_gas_id(gas_id);

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_STOP_GSM_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_STOP_GSM_CNF;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_STOP_GSM_SUB2_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_STOP_GSM_SUB2_CNF;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
  else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
  {
    rfa_umid_to_send = RFA_RF_GSM_STOP_GSM_SUB3_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_STOP_GSM_SUB3_CNF;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initialise msgr header */
  msgr_init_hdr(&rfa_rf_gsm_stop_gsm_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */
  rfa_rf_gsm_stop_gsm_req.sub_id       = gas_id;
  rfa_rf_gsm_stop_gsm_req.cnf_required = TRUE;

  GDRV_RFA_MSGR_LOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rfa_rf_gsm_stop_gsm_req.hdr, sizeof(rfa_rf_gsm_stop_gsm_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

   /* blocking wait for IRAT params CNF message from RF Task */
  result = msgr_receive(&gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
                        (uint8 *)&rfa_rf_gsm_stop_gsm_cnf,
                        (uint32)sizeof(rfa_rf_gsm_stop_gsm_cnf_s),
                        &bytes_recvd);
  GDRV_RFA_MSGR_UNLOCK(gas_id);

  /* sanity check and print debug for msgr_receive */
  gl1_hw_rf_msgr_receive_debug(rfa_umid_to_rcv,
                               result,
                               rfa_rf_gsm_stop_gsm_cnf.hdr.id,
                               rfa_rf_gsm_stop_gsm_cnf.req_result,
                               gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_combined_or_prx_rf_wakeup

DESCRIPTION
  Send MSGR message to RF Task to run RF Wakeup. Use MSGR blocking wait
  to receive wakeup completion confimation from RF Task before continuing

===========================================================================*/
static void gl1_hw_combined_or_prx_rf_wakeup(uint32 rxlm_buf_idx,
                                             gas_id_t gas_id)
{
  rfa_rf_gsm_rx_wakeup_req_s rf_gsm_rx_wakeup_req;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  errno_enum_type result;
  msgr_umid_type rfa_umid_to_send = NULL;

#ifdef FEATURE_GSM_RX_DIVERSITY
  rfm_device_enum_type divrx_rf_device = RFM_MAX_WAN_DEVICES;
  grm_client_enum_t divrx_client = GRM_NO_CLIENT;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

  gas_id = check_gas_id(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
  divrx_client =  grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);

  if((gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_SINGLE_SIM ) 
      &&
      (gl1_hw_is_saic_dis_in_lmm_mode(gas_id) == TRUE))
  {
    MSG_GERAN_HIGH_1_G("ANT_SHARING:Don't send combined wake up (%d) in Single Sim mode if SAIC is disabled in LMM mode",
      divrx_client);
  }
  else
  {
     /*No grant: no need to wakeup*/
     if(grm_get_trm_status(divrx_client, gas_id) == GL1_TRM_GRANTED)
     {
       divrx_rf_device = gl1_hw_get_gsm_rf_id(divrx_client, gas_id);

       MSG_GERAN_HIGH_1_G("ANT_SHARING:DivRx Device granted for grm_get_trm_status = %d",
         divrx_rf_device);
     }
     else
     {
       MSG_GERAN_HIGH_1_G("ANT_SHARING:DivRx Device not granted grm_get_trm_status = %d",
         grm_get_trm_status(divrx_client, gas_id));
     }
  }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_RX_WAKEUP_REQ;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_RX_WAKEUP_SUB2_REQ;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
   {
    rfa_umid_to_send = RFA_RF_GSM_RX_WAKEUP_SUB3_REQ;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initial wakeup msgr header */
  msgr_init_hdr(&rf_gsm_rx_wakeup_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */

  rf_gsm_rx_wakeup_req.gfw_buff_ptr = (void*)mdsp_get_gfw_init_event_buffer(gas_id);

  rf_gsm_rx_wakeup_req.sub_id = gas_id;
  rf_gsm_rx_wakeup_req.cnf_required = TRUE;
  rf_gsm_rx_wakeup_req.rxlm_buf_index.prx_idx = (lm_handle_type)rxlm_buf_idx;
  rf_gsm_rx_wakeup_req.prx_device = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);
  rf_gsm_rx_wakeup_req.timing_info = &gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info;

#ifdef FEATURE_GSM_RX_DIVERSITY
  if (divrx_rf_device < RFM_MAX_WAN_DEVICES)
  {
    if (MDSP_RXLM_BUF_IDX_INVALID == gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id))
    {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
      MSG_GERAN_HIGH_0_G(GL1_RXD"@IMRD_WAKEUP DRX RXLM is INVALID; Trying to reallocate it. ");
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
      if (gl1_alloc_IMRD_rxdiv_rxlm_buff_idx(gas_id))
      {
        rf_gsm_rx_wakeup_req.rxlm_buf_index.drx_idx = (lm_handle_type) (gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id));
        rf_gsm_rx_wakeup_req.drx_device             = divrx_rf_device;
        rf_gsm_rx_wakeup_req.op_mode                = RFGSM_PRX_DRX_OP_MODE;
        gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_WAKEUP_MODE, gas_id);
      }
      else
      {
        MSG_GERAN_ERROR_0_G(GL1_RXD"@IMRD_WAKEUP  CANNOT ALLOCATE DRX RXLM BUFF IDX ");
        rf_gsm_rx_wakeup_req.rxlm_buf_index.drx_idx = (uint32) (-1);
        rf_gsm_rx_wakeup_req.drx_device             = RFM_INVALID_DEVICE;
        rf_gsm_rx_wakeup_req.op_mode                = RFGSM_PRX_OP_MODE;
        gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_SLEEP_MODE, gas_id);
      }
    }
    else
    {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
      MSG_GERAN_HIGH_0_G(GL1_RXD"@IMRD_WAKEUP DRX RXLM buff is already alocated, using it.");
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/

      rf_gsm_rx_wakeup_req.rxlm_buf_index.drx_idx = (lm_handle_type) (gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id));
      rf_gsm_rx_wakeup_req.drx_device             = divrx_rf_device;
      rf_gsm_rx_wakeup_req.op_mode                = RFGSM_PRX_DRX_OP_MODE;
      gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_WAKEUP_MODE, gas_id);
    }
  }
  else
  {
    rf_gsm_rx_wakeup_req.rxlm_buf_index.drx_idx = (uint32) (-1);
    rf_gsm_rx_wakeup_req.drx_device             = RFM_INVALID_DEVICE;
    rf_gsm_rx_wakeup_req.op_mode                = RFGSM_PRX_OP_MODE;
    gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_SLEEP_MODE, gas_id);
  }
  MSG_GERAN_HIGH_5_G("rf_gsm_rx_wakeup_req prx_idx=%i prx_device=%i drx_idx=%i drx_device=%i, wakeup_mode=%i",
                     rf_gsm_rx_wakeup_req.rxlm_buf_index.prx_idx,
                     rf_gsm_rx_wakeup_req.prx_device,
                     rf_gsm_rx_wakeup_req.rxlm_buf_index.drx_idx,
                     rf_gsm_rx_wakeup_req.drx_device,
                     rf_gsm_rx_wakeup_req.op_mode);
#else
  rf_gsm_rx_wakeup_req.rxlm_buf_index.drx_idx = (uint32)(-1);
  rf_gsm_rx_wakeup_req.drx_device = RFM_INVALID_DEVICE;
  rf_gsm_rx_wakeup_req.op_mode = RFGSM_PRX_OP_MODE;
#endif /* FEATURE_GSM_RX_DIVERSITY*/

  GDRV_RFA_MSGR_LOCK(gas_id);
  /* send msgr message to RF Task */
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  result = msgr_send(&rf_gsm_rx_wakeup_req.hdr, sizeof(rf_gsm_rx_wakeup_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);
  /* Tracks if the wake up request was sent in the first place or not */
  gl1_hw_set_rf_wakeup_req_msg(gas_id, ((result == E_SUCCESS) ? TRUE : FALSE));

  if ((gl1_hw_sleep_get_in_warmup_check(gas_id)==TRUE) && (gl1_hw_is_forced_wakeup(gas_id)==FALSE))
  {
    gl1_hw_sleep_set_commit_mask(RF_WAKEUP_COMMIT, gas_id);
    gl1_hw_rf_result_monitor_timer_start(XCCH_BLOCK_COMMIT_TIMEOUT,gas_id );
  }

  GDRV_RFA_MSGR_UNLOCK(gas_id);

}
#ifdef FEATURE_GSM_RX_DIVERSITY
/*===========================================================================

FUNCTION  gl1_hw_rf_wakeup_divrx

DESCRIPTION
  Send MSGR message to RF Task to run RF Wakeup for Diversity Rx only.
  Use MSGR non-blocking wait to receive wakeup completion confimation in
  gl1_msgr_if task context.

===========================================================================*/
static void gl1_hw_rf_wakeup_divrx(gas_id_t gas_id)
{
  rfa_rf_gsm_rx_wakeup_req_s rf_gsm_rx_wakeup_req;
  errno_enum_type result;
  msgr_umid_type rfa_umid_to_send = NULL;
  rfm_device_enum_type divrx_rf_device;
  grm_client_enum_t divrx_client = GRM_NO_CLIENT;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = NULL;

  gas_id = check_gas_id(gas_id);

  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  divrx_client = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);

  /*RxD allocation should have been checked already, do not check trm status here*/

  divrx_rf_device = gl1_hw_get_gsm_rf_id(divrx_client, gas_id);

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_RX_WAKEUP_REQ;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_RX_WAKEUP_SUB2_REQ;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
   {
    rfa_umid_to_send = RFA_RF_GSM_RX_WAKEUP_SUB3_REQ;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initial wakeup msgr header */
  msgr_init_hdr(&rf_gsm_rx_wakeup_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */

  rf_gsm_rx_wakeup_req.gfw_buff_ptr = (void*)mdsp_get_gfw_init_event_buffer(gas_id);

  rf_gsm_rx_wakeup_req.sub_id                 = gas_id;
  rf_gsm_rx_wakeup_req.cnf_required           = TRUE;
  rf_gsm_rx_wakeup_req.rxlm_buf_index.prx_idx = -1;
  rf_gsm_rx_wakeup_req.prx_device             = RFM_INVALID_DEVICE;
  rf_gsm_rx_wakeup_req.rxlm_buf_index.drx_idx = (lm_handle_type) (gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id));
  rf_gsm_rx_wakeup_req.drx_device             = divrx_rf_device;
  rf_gsm_rx_wakeup_req.op_mode                = RFGSM_DRX_OP_MODE;
  rf_gsm_rx_wakeup_req.timing_info            = &gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info;

  MSG_GERAN_HIGH_3_G(GL1_RXD"gl1_hw_rf_wakeup_DivRx drx_idx=%i drx_device=%i, wakeup_mode=%i",
                     rf_gsm_rx_wakeup_req.rxlm_buf_index.drx_idx,
                     rf_gsm_rx_wakeup_req.drx_device,
                     rf_gsm_rx_wakeup_req.op_mode);

  GDRV_RFA_MSGR_LOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rf_gsm_rx_wakeup_req.hdr, sizeof(rf_gsm_rx_wakeup_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);


  GDRV_RFA_MSGR_UNLOCK(gas_id);
}
#endif
/*===========================================================================

FUNCTION  gl1_hw_rf_tx_wakeup

DESCRIPTION
  Send MSGR message to RF Task to run RF Wakeup for Tx.
  Use MSGR non-blocking wait to receive wakeup completion confimation in
  gl1_msgr_if task context.

===========================================================================*/
void gl1_hw_rf_tx_wakeup(uint32 txlm_buf_idx,
                         rfm_device_enum_type device,
                         gas_id_t gas_id)
{
  rfa_rf_gsm_tx_wakeup_req_s rf_gsm_tx_wakeup_req;
  errno_enum_type result;
  msgr_umid_type rfa_umid_to_send = NULL;

  gas_id = check_gas_id(gas_id);

  gl1_hw_set_invalid_rf_gsm_band(gas_id);

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_TX_WAKEUP_REQ;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_TX_WAKEUP_SUB2_REQ;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
   {
    rfa_umid_to_send = RFA_RF_GSM_TX_WAKEUP_SUB3_REQ;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initial wakeup msgr header */
  msgr_init_hdr(&rf_gsm_tx_wakeup_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */

  rf_gsm_tx_wakeup_req.gfw_buff_ptr = (void*)mdsp_get_gfw_init_event_buffer(gas_id);

  rf_gsm_tx_wakeup_req.sub_id              = gas_id;
  rf_gsm_tx_wakeup_req.cnf_required        = TRUE;
  rf_gsm_tx_wakeup_req.txlm_buf_index      = txlm_buf_idx;
  rf_gsm_tx_wakeup_req.tx_device           = device;
  rf_gsm_tx_wakeup_req.timing_info         = &gl1_hw_data_ptr[gas_id]->gl1_rf_gsm_timing_info;
  rf_gsm_tx_wakeup_req.therm_read_buff_ptr = (void*)mdsp_get_gfw_therm_read_event_buffer(gas_id);

  MSG_GERAN_HIGH_2_G("rf_gsm_tx_wakeup_req txlm_idx=%i tx_device=%i",
                     rf_gsm_tx_wakeup_req.txlm_buf_index,
                     rf_gsm_tx_wakeup_req.tx_device);

  GDRV_RFA_MSGR_LOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rf_gsm_tx_wakeup_req.hdr, sizeof(rf_gsm_tx_wakeup_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);


  GDRV_RFA_MSGR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_rf_sleep

DESCRIPTION
  Send MSGR message to RF Task to run RF Sleep for Rx. Use MSGR blocking wait
  to receive wakeup completion confimation from RF Task before continuing

===========================================================================*/
void gl1_hw_rf_sleep(uint32 rxlm_buf_idx, 
                     gl1_hw_sleep_mode_type sleep_mode, 
                     gas_id_t gas_id)
{
  rfa_rf_gsm_rx_sleep_req_s rf_gsm_rx_sleep_req;
  rfa_rf_gsm_rx_sleep_cnf_s rf_gsm_rx_sleep_cnf;
  errno_enum_type result;
  uint32 bytes_recvd;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  msgr_umid_type rfa_umid_to_send = NULL;
  msgr_umid_type rfa_umid_to_rcv = NULL;

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  rfm_device_enum_type divrx_rf_device = RFM_MAX_WAN_DEVICES;
  grm_client_enum_t divrx_client = GRM_NO_CLIENT;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

  gas_id = check_gas_id(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
  /*No grant: no need to wakeup*/
  if(gl1_hw_get_rf_sleep_wakeup_mode(gas_id) == GL1_RF_PRX_WAKEUP_DIVRX_WAKEUP_MODE)
  {
    divrx_client = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);
    divrx_rf_device = gl1_hw_get_gsm_rf_id(divrx_client,gas_id);
  }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_RX_SLEEP_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_RX_SLEEP_CNF;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_RX_SLEEP_SUB2_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_RX_SLEEP_SUB2_CNF;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
   {
     rfa_umid_to_send = RFA_RF_GSM_RX_SLEEP_SUB3_REQ;
     rfa_umid_to_rcv  = RFA_RF_GSM_RX_SLEEP_SUB3_CNF;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initial wakeup msgr header */
  msgr_init_hdr(&rf_gsm_rx_sleep_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */

  rf_gsm_rx_sleep_req.sub_id = gas_id;
  rf_gsm_rx_sleep_req.cnf_required = TRUE;

  switch (sleep_mode)
  {
    case GL1_DRX_SLEEP_MODE:
      rf_gsm_rx_sleep_req.prx_device = RFM_INVALID_DEVICE;
      rf_gsm_rx_sleep_req.drx_device = divrx_rf_device;
      rf_gsm_rx_sleep_req.op_mode    = RFGSM_DRX_OP_MODE;
      gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_SLEEP_MODE, gas_id);
      break;
    case GL1_PRX_DRX_SLEEP_MODE:
    case GL1_PRX_SLEEP_MODE:
      rf_gsm_rx_sleep_req.prx_device = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY
      if (divrx_rf_device < RFM_MAX_WAN_DEVICES)
      {
        rf_gsm_rx_sleep_req.drx_device = divrx_rf_device;
        rf_gsm_rx_sleep_req.op_mode    = RFGSM_PRX_DRX_OP_MODE;;
      }
      else
      {
        rf_gsm_rx_sleep_req.drx_device = RFM_INVALID_DEVICE;
        rf_gsm_rx_sleep_req.op_mode    = RFGSM_PRX_OP_MODE;;
      }
      gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_SLEEP_DIVRX_SLEEP_MODE, gas_id);
      MSG_GERAN_HIGH_3_G("rf_gsm_rx_sleep_req prx_device=%i drx_device=%i sleep_mode=%i",
                         rf_gsm_rx_sleep_req.prx_device,
                         rf_gsm_rx_sleep_req.drx_device,
                         rf_gsm_rx_sleep_req.op_mode);
#else
    rf_gsm_rx_sleep_req.drx_device = RFM_INVALID_DEVICE;
    rf_gsm_rx_sleep_req.op_mode = RFGSM_PRX_OP_MODE;;
#endif /* FEATURE_GSM_RX_DIVERSITY */
      break;
    default:
      break;
  }

  GDRV_RFA_MSGR_LOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rf_gsm_rx_sleep_req.hdr, sizeof(rf_gsm_rx_sleep_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

    /******* TODO: IF IN MSIM Mode we need to always check GAS ID 1, There will be no registration of
      ****** Device 0 CNF UMID's on gas id 2
      *****/
  /* blocking wait for TX Enable CNF message from RF Task */
  result = msgr_receive(&gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
                        (uint8 *)&rf_gsm_rx_sleep_cnf,
                        (uint32)sizeof(rfa_rf_gsm_rx_sleep_cnf_s),
                        &bytes_recvd);
  GDRV_RFA_MSGR_UNLOCK(gas_id);


  /* sanity check and print debug for msgr_receive */
  gl1_hw_rf_msgr_receive_debug(rfa_umid_to_rcv,
                               result,
                               rf_gsm_rx_sleep_cnf.hdr.id,
                               rf_gsm_rx_sleep_cnf.req_result,
                               gas_id);
  if ( gl1_msg_get_multi_sim_sys_mode() != SYS_MODEM_DEVICE_MODE_SINGLE_SIM )
  {
    if (rf_gsm_rx_sleep_req.prx_device != RFM_INVALID_DEVICE) 
    {
      gl1_hw_set_gsm_rf_id(RFM_INVALID_DEVICE,
                         l1_tskisr_blk->client_id,
                         TRUE, 
                         gas_id);
    }

    if ((rf_gsm_rx_sleep_req.drx_device != RFM_INVALID_DEVICE)
        && (divrx_client != GRM_NO_CLIENT))
    {
      gl1_hw_set_gsm_rf_id(RFM_INVALID_DEVICE,
                         divrx_client,
                         TRUE, 
                         gas_id);
    }
  }
  return;
}


/*===========================================================================

FUNCTION  gl1_hw_rf_tx_sleep

DESCRIPTION
  Send MSGR message to RF Task to run RF Sleep for Tx. Use MSGR blocking wait
  to receive wakeup completion confimation from RF Task before continuing

===========================================================================*/
void gl1_hw_rf_tx_sleep(uint32 txlm_buf_idx,
                        rfm_device_enum_type device,
                        gas_id_t gas_id)
{
  rfa_rf_gsm_tx_sleep_req_s rf_gsm_tx_sleep_req;
  rfa_rf_gsm_tx_sleep_cnf_s rf_gsm_tx_sleep_cnf;
  errno_enum_type result;
  uint32 bytes_recvd;
  msgr_umid_type rfa_umid_to_send = NULL;
  msgr_umid_type rfa_umid_to_rcv = NULL;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = NULL;

  gas_id = check_gas_id(gas_id);

  l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  /* initialise msgr header based on subscription we are requesting*/
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rfa_umid_to_send = RFA_RF_GSM_TX_SLEEP_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_TX_SLEEP_CNF;
  }
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    rfa_umid_to_send = RFA_RF_GSM_TX_SLEEP_SUB2_REQ;
    rfa_umid_to_rcv  = RFA_RF_GSM_TX_SLEEP_SUB2_CNF;
  }
#endif
#if defined ( FEATURE_TRIPLE_SIM )
   else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
   {
     rfa_umid_to_send = RFA_RF_GSM_TX_SLEEP_SUB3_REQ;
     rfa_umid_to_rcv  = RFA_RF_GSM_TX_SLEEP_SUB3_CNF;
  }
#endif /* FEATURE_TRIPLE_SIM */

  /* initial wakeup msgr header */
  msgr_init_hdr(&rf_gsm_tx_sleep_req.hdr,MSGR_GERAN_GL1,rfa_umid_to_send);
  /* populate payload */

  rf_gsm_tx_sleep_req.sub_id = gas_id;
  rf_gsm_tx_sleep_req.cnf_required = TRUE;
  rf_gsm_tx_sleep_req.tx_device = device;
  
  GDRV_RFA_MSGR_LOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* send msgr message to RF Task */
  result = msgr_send(&rf_gsm_tx_sleep_req.hdr, sizeof(rf_gsm_tx_sleep_req));

  /* sanity check and print debug for msgr_send */
  gl1_hw_rf_msgr_send_debug(rfa_umid_to_send, result,gas_id);

    /******* TODO: IF IN MSIM Mode we need to always check GAS ID 1, There will be no registration of
      ****** Device 0 CNF UMID's on gas id 2
      *****/
  /* blocking wait for TX Enable CNF message from RF Task */
  result = msgr_receive(&gl1_hw_data_ptr[gas_id]->gsm_l1_common_rfa_task_msgr_client,
                        (uint8 *)&rf_gsm_tx_sleep_cnf,
                        (uint32)sizeof(rfa_rf_gsm_tx_sleep_cnf_s),
                        &bytes_recvd);
  GDRV_RFA_MSGR_UNLOCK(gas_id);


  /* sanity check and print debug for msgr_receive */
  gl1_hw_rf_msgr_receive_debug(rfa_umid_to_rcv,
                               result,
                               rf_gsm_tx_sleep_cnf.hdr.id,
                               rf_gsm_tx_sleep_cnf.req_result,
                               gas_id);

#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#ifdef FEATURE_GSM_RX_TX_SPLIT
  if ( gl1_msg_get_multi_sim_sys_mode() != SYS_MODEM_DEVICE_MODE_SINGLE_SIM )
  {
    /* Release Tx Chain when there is no resource lock and no pending TRM lock exchange */
    if ((FALSE == grm_get_resource_lock_state(gas_id)) &&
        (GERAN_EXCHANGE_NULL == geran_get_exchange_on_release(gas_id)))
    {
      grm_release_tx_chain(grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id), gas_id);
      /*Sleep Tx, reset rf device to invalid*/
      gl1_hw_set_gsm_rf_id(RFM_INVALID_DEVICE,
                           grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id),
                           TRUE, 
                           gas_id);
    }
  }
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
#endif /* (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */

}

#endif /*FEATURE_GSM_RX_TX_SPLIT*/
/*===========================================================================

FUNCTION gl1_hw_reset_gsm_rf_ids

DESCRIPTION
  Reset Rx, Tx and Diversity Rf Ids.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_reset_gsm_rf_ids(grm_client_enum_t     grm_client_id,
                             gas_id_t              gas_id)
{
  gl1_hw_set_gsm_rf_id(RFM_INVALID_DEVICE, grm_client_id, TRUE, gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
  gl1_hw_set_gsm_rf_id(RFM_INVALID_DEVICE, 
                       grm_map_prx_to_divrx_client_id(grm_client_id, gas_id), 
                       TRUE, 
                       gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef FEATURE_GSM_RX_TX_SPLIT
   gl1_hw_set_gsm_rf_id(RFM_INVALID_DEVICE, 
                        grm_map_rx_to_tx_client_id(grm_client_id, gas_id), 
                        TRUE, 
                        gas_id);
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  return;
}
/*===========================================================================

FUNCTION gl1_hw_sub_non3gpp_capable

DESCRIPTION
  Check Non 3GPP Subscription capability.

DEPENDENCIES
  None

RETURN VALUE
  TRUE: SUB is non 3GPP capable.
  FALSE: Else.

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_hw_sub_non3gpp_capable(gas_id_t gas_id)
{
  boolean sub_non3gpp_capable = FALSE;
  uint32 sub_capability_mask = geran_retrieve_subscription_capabilities(gas_id);

  if ((sub_capability_mask & SYS_SYS_MODE_MASK_CDMA)
      || (sub_capability_mask & SYS_SYS_MODE_MASK_HDR)
      ) 
  {
    sub_non3gpp_capable = TRUE;
  }

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  MSG_GERAN_HIGH_2_G(GL1_RXD"gl1_hw_sub_non3gpp_capable sub_capability_mask %d "
                            "sub_non3gpp_capable %d",
                     sub_capability_mask, 
                     sub_non3gpp_capable);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/

  return sub_non3gpp_capable;
}

/*===========================================================================

FUNCTION gl1_hw_is_saic_dis_in_lmm_mode

DESCRIPTION
  Returns TRUE if SAIC is disabled and UE is in low monitor mode

DEPENDENCIES
  None

RETURN VALUE
  TRUE: SUB is non 3GPP capable.
  FALSE: Else.

SIDE EFFECTS
  None
===========================================================================*/

boolean gl1_hw_is_saic_dis_in_lmm_mode(gas_id_t gas_id)
{
  boolean gl1_is_saic_dis_in_lmm_mode = FALSE;

  if( 
     (gl1_hw_ncell_pch_low_monitor_active(gas_id) == TRUE)
      && 
     (gl1_msg_get_saic_disable_in_idle(gas_id) == TRUE))
    {
    gl1_is_saic_dis_in_lmm_mode = TRUE;
    }

return (gl1_is_saic_dis_in_lmm_mode);
}

/*===========================================================================

FUNCTION gl1_hw_start_gsm_qta

DESCRIPTION
  Send RF start and MDSP initialisation at qta start if necesary

DEPENDENCIES
  None


SIDE EFFECTS
  None
===========================================================================*/

void gl1_hw_start_gsm_qta(gas_id_t gas_id)
{
#ifdef FEATURE_GSM_RX_TX_SPLIT
      if(gl1_hw_data_ptr[gas_id]->rf_enter_mode_required)
      {
        gl1_hw_data_ptr[gas_id]->rf_enter_mode_required = FALSE;
        gl1_hw_rf_start_gsm(gas_id);
        MSG_GERAN_HIGH_0_G( "QTA: RF GSM START and Rx Wakeup requested ");
        gl1_hw_set_if_start_gsm_qta_sent(TRUE, gas_id);
      }        
#else
         /* Need to be implemented with new requirements on  RF  */
#endif

      if (mdsp_init_completed(gas_id ) == FALSE)
      {
         gl1_hw_multi_sim_mdsp_init(FALSE, gas_id);
      }
}

/*============ACI 400 Switchpoint Detection============*/

/*===========================================================================
FUNCTION gl1_hw_aci_reset_table 
 
DESCRIPTION 
  To Reset the Global GL1 Data Array for ACI Status for the called Sub
  to default ACI value defined in RF
 
DEPENDENCIES 
  No Checks done on gas_id as its assumed that its already done in Calling function
 
RETURN VALUE 
  None
 
SIDE EFFECTS 
  None
===========================================================================*/
void gl1_hw_aci_reset_table(gas_id_t gas_id)
{
  memset( &gl1_hw_aci_table[gas_id], FALSE, (RR_MAX_PSCAN_FREQ_SCAN_ARRAY_SIZE * sizeof(boolean)));
}

/*===========================================================================
FUNCTION gl1_hw_set_aci_fr_arfcn 
 
DESCRIPTION 
  Stores the ACI Detected in GFW in the global GL1 Array
  - At the moment the input is a boolean indicating ACI 400 is detected or not
  based on which Gl1 sets the RF Enum
  Array is indexed based on Arfcns and gas_id
 
DEPENDENCIES 
  No Checks done on gas_id as its assumed that its already done in Calling function
  Index value to be used with Arfcn are retrieved using function gl1 hw get arfcn index 
 
RETURN VALUE 
  None
 
SIDE EFFECTS 
  None
===========================================================================*/
void gl1_hw_set_aci_fr_arfcn(ARFCN_T arfcn, boolean aci_in, gas_id_t gas_id)
{
  uint32 index = gl1_hw_get_arfcn_index(arfcn, gas_id);
  if (index != INVALID_INDEX)
  {
    gl1_hw_aci_table[gas_id][index] = aci_in;
    MSG_GERAN_LOW_2_G("SET ACI for Arfcn: %d ACI flag: %d ", arfcn.num,  gl1_hw_aci_table[gas_id][index]);
  }
  else
  {
    MSG_GERAN_ERROR_1_G("ERROR: Unable to Set ACI as index is invalid for Arfcn: %d", arfcn.num);
  }
}


/*===========================================================================
FUNCTION gl1_hw_get_aci_fr_arfcn 
 
DESCRIPTION 
  Gets the ACI Value cached in Global Gl1 Array based on ARFCN supplied
  for RF to apply the ACI 400 based switchpoint
  Which is passed on to GL1 from GFW
 
DEPENDENCIES 
  No Checks done on gas_id as its assumed that its already done in Calling function
  Index value to be used with Arfcn are retrieved using function gl1 hw get arfcn index 
 
RETURN VALUE 
  rfgsm_aci_enum_type (Defined in RFA)
 
SIDE EFFECTS 
  None
===========================================================================*/
boolean gl1_hw_get_aci_fr_arfcn(ARFCN_T arfcn, gas_id_t gas_id)
{
  uint32 index = gl1_hw_get_arfcn_index(arfcn, gas_id);

  if (index != INVALID_INDEX)
  {
    MSG_GERAN_LOW_3_G("GET ACI for Arfcn: %d ACI flag: %d index:%d", arfcn.num,  gl1_hw_aci_table[gas_id][index], index);
    return gl1_hw_aci_table[gas_id][index];
  }
  else
  {
    MSG_GERAN_ERROR_1_G("ERROR: Unable to Get ACI as index is invalid for Arfcn: %d use FALSE", arfcn.num);
    return FALSE;
  }
}

#ifdef FEATURE_GSM_SENS_SUPPORT
/*===========================================================================

FUNCTION gl1_hw_efs_read_sens_fr_ctrl

DESCRIPTION
  This function read the NV item for SENS feature control

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void gl1_hw_efs_read_sens_fr_ctrl( void )
{

    int efs_error =
          geran_efs_read_primary( GL1_EFS_SENS_FR_CONTROL,
                   &gl1_efs_sens_fr_ctrl,
                   sizeof( gl1_efs_sens_fr_ctrl) );

  if(efs_error == -1)
  {
      /*
       * Enable by default if NV control not exists
       */
      gl1_efs_sens_fr_ctrl = TRUE;
    }

    MSG_GERAN_HIGH_2( "GL1:EFS EFS_Status:%d SENs FR NV Status:%d", efs_error, gl1_efs_sens_fr_ctrl );

  }

/*===========================================================================

FUNCTION gl1_hw_get_sens_fr_ctrl

DESCRIPTION
  This function returns the SENS control flag based on NV

DEPENDENCIES
  None

RETURN VALUE
  gl1_efs_sens_fr_ctrl

SIDE EFFECTS
  None
===========================================================================*/

uint8 gl1_hw_get_sens_fr_ctrl(void)
{
  return gl1_efs_sens_fr_ctrl;
}
#endif

#ifdef FEATURE_GSM_SYNQ_SUPPORT
/*===========================================================================

FUNCTION gl1_hw_efs_read_sens_fr_ctrl

DESCRIPTION
  This function read the NV item for SYNQ feature control
  Update fields for feature enable/disable & gdet threshold
  If NV is not properly read, enable feature by default and threshols as 666 <2.8>

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void gl1_hw_efs_read_synq_fr_ctrl( void )
{
    uint32 gl1_synq_fr_ctrl_flag = 0;
    int efs_error =
          geran_efs_read_primary( GL1_EFS_SYNQ_FR_CONTROL,
                   &gl1_synq_fr_ctrl_flag,
                   sizeof( gl1_synq_fr_ctrl_flag) );
    
    if ( efs_error == -1 )
    {
      /*  Enable by default if NV control not exists  */
      gl1_efs_synq_fr_enabled  = TRUE;
    }
    else
    {
      gl1_efs_synq_fr_enabled = (gl1_synq_fr_ctrl_flag & 1);
    }


    /* Read threshold of consecutive poor gdet scores to trigger SCH acq*/
    if( (gl1_synq_fr_ctrl_flag >> 1) & 0x0F )
    {
      gl1_efs_bad_gdet_threshold_cnt = (uint8)( (gl1_synq_fr_ctrl_flag >> 1) & 0x0F );
    }
    else
    {
      gl1_efs_bad_gdet_threshold_cnt = BAD_GDET_THRESHOLD_COUNT;
    }



    /* Read threshold of consecutive good gdet scores to avoid SCH acq based on synq algo*/
    if((gl1_synq_fr_ctrl_flag >> 5) & 0x07)
    {
      gl1_efs_gdet_recovering_threshold_cnt = (uint8)((gl1_synq_fr_ctrl_flag >> 5) & 0x07);
    }
    else
    {
      gl1_efs_gdet_recovering_threshold_cnt = GDET_RECOVERING_THRESHOLD_COUNT;
    }



    if(gl1_synq_fr_ctrl_flag >> 8)
    {
      gl1_efs_synq_fr_gdet_threshold = (uint16)(gl1_synq_fr_ctrl_flag >> 8);
    }
    else
    {
      gl1_efs_synq_fr_gdet_threshold = NV_GDET_THRESHOLD_CFG;
    }

    MSG_GERAN_HIGH_2( "GL1:EFS EFS_Status:%d SYNQ FR NV Status:%d", efs_error, gl1_synq_fr_ctrl_flag );

}

/*===========================================================================

FUNCTION gl1_hw_get_synq_fr_enable

DESCRIPTION
  This function returns the SyNQ feature enable\disable value based on NV

DEPENDENCIES
  None

RETURN VALUE
  gl1_hw_get_synq_fr_enabled

SIDE EFFECTS
  None
===========================================================================*/

uint8 gl1_hw_get_synq_fr_enabled(void)
{
  return gl1_efs_synq_fr_enabled;
}

/*===========================================================================

FUNCTION gl1_hw_get_synq_fr_gdet_threshold

DESCRIPTION
  This function returns the gdet threshold value for SyNQ feature based on NV

DEPENDENCIES
  None

RETURN VALUE
  gl1_efs_sens_fr_ctrl

SIDE EFFECTS
  None
===========================================================================*/

uint16 gl1_hw_get_synq_fr_gdet_threshold(uint8 * bad_gdet_threshold, uint8 * gdet_recovering_threshold)
{

    *bad_gdet_threshold = gl1_efs_bad_gdet_threshold_cnt;
    *gdet_recovering_threshold = gl1_efs_gdet_recovering_threshold_cnt;

    return gl1_efs_synq_fr_gdet_threshold;
}

#endif
boolean gl1_hw_get_if_start_gsm_qta_sent(gas_id_t gas_id)
{
   return gl1_hw_data_ptr[gas_id]->gl1_hw_start_gsm_qta_sent;
}

void gl1_hw_set_if_start_gsm_qta_sent(boolean value, gas_id_t gas_id)
{
   gl1_hw_data_ptr[gas_id]->gl1_hw_start_gsm_qta_sent = value;
}

/* EOF */
