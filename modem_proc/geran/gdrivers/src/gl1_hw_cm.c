/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     G L 1  F R A M E   L A Y E R  f o r  C O M P R E S S E D    M O D E

GENERAL DESCRIPTION
   This module contains the procedures for the frame layer of non portable
   layer 1.   The procedures used herein are intended for sole use by the
   message layer functions and not intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS
  gl1_hw_cm_cleanup
  gl1_hw_cm_get_power_measure_results
  gl1_hw_cm_gsm_init
  gl1_hw_cm_power_measure
  gl1_hw_cm_startup

INTERNAL FUNCTIONS
  gl1_hw_cm_rf_init


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_cm.c#7 $
$DateTime: 2020/06/07 04:13:53 $ $Author: pwbldsvc $


when       who      what, where, why
--------   ---      ----------------------------------------------------------
05/06/20   sc       CR2703314 TDS Removal on Agatti- Moving G2T/T2G code under TDS feature
20/05/20   sc       CR2677798 Changing variable last_state to gl1_t2g_last_state.
11/05/20   nv       CR2683020 Adding debug F3s to check active_carrier_idx_mask in QTA Start up
03/11/20   sc       CR2621105 Debug prints to track last_state in T2G
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
18/01/16   zf       CR961805: Set first burst flag in x2g_startup 
06/04/18   akm      CR2216412 Moving G2X de-init in QTA cleanup
21/03/18   km       CR2208916 Clear gl1_hw_start_gsm_qta_sent from qta clean up and while moving to inactive mode
27/04/17   dg       CR2038208 Print current Qs value during L2G/W2G startup and during CXM register command sent to GFW
09/02/18   km       CR2184116 Set g2l_abort to TRUE in case of QTA gap abort
09/11/17   ng       CR2140861 Do not allow Non Irat activity in QTA gap opened for IRAT activity
01/02/18   km       CR2178750 Set l1_irat_reserve_at_sent to False when G2L is skipped in QTA to defer the gap
31/10/16   cc       CR1080513 Fix QTA event log buffer overflow
09/01/18   km       CR2166879 G2L abort handling in QTA/non-QTA scenarios
11/10/16   zf       CR1076218: Mutex protect QTA event logging
09/09/16   sk       CR1059236 Remove error log.
19/08/16   dp       CR1055018 Make FIND_BCCH_MODE QTA's check for invalid device granted from TRM
01/07/16   dp       CR1035983 Crash only in normal wakeup for missing QTA CleanUp post 47 Frames 
30/06/16   snjv     CR1033634 Crash only in MSim Idle Sub for missing QTA CleanUp post 47 Frames 
28/04/16   sk       CR1008819 Force a crash if QTA cleanup is not done on time
30/06/16   snjv     CR1034806 Address KW Errors- uninitialized var used in trm qta startup
29/10/17   km       CR2133917 GL1 support for TA.3.0 L+L
27/07/16   zf       CR1046704: Using MSGR variant
19/09/16   ng       CR1067345 Allow GFW to use expired X2G preload offset instead of crashing
22/08/16   sk       CR1053862 Reduce the number of error messages.
19/02/16   bg       CR979066 No need band checking, when QTA for BCCH read has triggered in FIND BCCH state
01/02/16   bg       CR951395 Skip QTA if the allocated device does not support band
23/11/15   sk       CR936213 Read client RGS value while inside the QTA gap for AFC update
24/11/15   cc       CR928622 Use idle Rx Alpha for T2G idle in DR mode
23/11/15   js       CR939644 Adding QTA mutex lock in X2G IRAT scenario 
18/11/15   cja      CR938089 Use XtoG RF timing for RX in QTA gap
12/11/15   cja      CR937562 Do not report parallel sch decoded if crc failed
05/11/15   cc       CR931979 Move EFS read to l1 task init
03/11/15   smd      CR934524 Back out CR929729 temporarily
22/10/15   zf       CR923858 Rework Sleep error recovery in QTA 
28/10/15   cja      CR922380 Correct X2G parallel SCH frequency results for desense channels
26/10/15   zf       CR929874: Handle failed GFW sleep request
23/10/15   smd      CR929729 Use idle Rx alpha for QDTA and QCTA
06/10/15   cja      CR918431 If QTA startup after GSM start/resume initialise MDSP
05/10/15   js       CR915574 Setting QTA initialized after bulding RF scripts in QTA gap
30/09/15   cws      CR913773 Use TRM allocated device to get cxm channel ID for CM preload/startup 
28/09/15   aga      CR911414 During QTA RF init send a START GSM RF if needed
18/09/15   cws      CR907895 Change unnecessary F3 to LOW 
08/09/15   cja      CR904063 Only reset X2g_Cmd_Info if T2G (so not cleared by other sub)
31/08/15   mko      CR896740:sleep error recovery changes in QTA gap
04/09/15   cja      CR901406 For L2G clear preload flag on rf meas exit (like T2G, not W2G)
03/09/15   ab       CR900163 - CXM Freq ID support for X2G IRAT ( non QTA )
26/08/15   ws       CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
18/07/15   zf       CR857733: Remove cmCleanupAlpha variable.
24/08/15   mc       CR874662 : CXM Freq ID support for X2G startup and X2G preload
20/08/15   am       CR890659 Ensure NAS ID to TCXO ID conversion is correct
15/08/15   zf       CR890312: Remove obsoleted RF abort function
03/08/15   jj       CR879940  disable_power_opt15 for W2G
14/08/14   ws       CR889273 Fix Static declarations of variables which are not Dual data spaced
07/08/15   cja      CR844078 Remove unused members from gl1_hw_cm_rf_info_t structure
07/08/15   cja      CR885161 Handle W2G prebuild before rf meas exit of previous gap
05/08/15   hd       CR884936: Handle QTA STARTUP and CLEANUP messages in MSGR IF
31/07/15   zf       CR881498: Do x2g_startup during X2G_Preload
06/08/15   js       CR885926 Removing API declaration from WL1 and declaring in GL1 interface file
04/08/15   aga      CR880762 Remove QTA deprecated APIs code
30/07/15   cja      CR881503 In QTA cleanup logging, use gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode
29/07/15   cja      CR880020 Set gl1_hw_cm_serving_rat_mode in cm init and QTA startup
28/07/15   mko      CR863121: Call CM exit scripts when after Ncell SCH decode in QBTA
23/07/15   cja      CR878466 Force abort in rf meas exit if scripts not used.
17/07/15   mc       CR871205 : Incorrect CXM Channel ID assigned to W2G activities in W+W mode
15/07/15   cja      CR871031 Remove forcing gas id 1 for T2G/G2T (required for W+W).
15/07/15   dv       CR872917 W+W:Crash in SS mode on SUB2 when doing X2G IRAT due to invalid Device ID 
14/07/15   js       CR872071 : Increasing QTA CXM margin and correcting condition for x2g starup check
09/07/15   mc       CR855818 : COEX:MCS_CXM_WWAN_TECH_STATE_IND registrations for X2G IRAT need to indicate CXM_LNK_TYPE_IRAT
06/07/15   sp       CR862778: Reduce F3 logging in Idle as its impacting QTA timeline
03/07/15   jj       CR 865562 Remove featurisation introduced for CR839611
25/06/15   zf       CR861339: Correction on checking QTA Startup margin
26/06/15   js       CR860602 Reset FW load flag when lock is extended in QTA gap/correcting offset check.
24/06/15   aga      CR856009 Check gas_id to avoid overflow of array in QTA handler 
22/05/15   ws       CR790493 Check BSIC decoded in SCH matches requested BSIC from RR
19/06/15   js       CR855745 Correcting check when X2G startup got delayed from warmup 
12/06/15   jk       CR821180:Feature flag cleanup
18/06/15   cah      CR855156: Clear useIdleModeRxAlpha when configuring RF and FW for QTA. 
06/01/15   js       CR814275 GL1 changes for ROTA interface during extended paging
18/05/15   jj       CR 839611 New skip last measurement in T2G if command is late 
17/06/15   aga      CR855583 Allocate RxLM buffer in the QTA start-up callback if invalid
16/06/15   zf       CR855367: Adding extra delay to avoid update HW FN before frame ends
12/06/15   js       CR850684 Change QBTA offset for X2G startup
09/06/15   cja      CR850999 Correct qta cleanup for QTA gap for G2W IRAT
09/06/15   cja      CR844096 RF meas gsm parameters to use xtoy param type
05/06/15   zf       CR849387: Fix mis-matched function prototype for set_qta_cleanup_pending.
05/06/15   js       CR832817: LTE API for L2G QBTA, removing changes done in 785901
03/06/15   zf       CR847986: Align HW FN during warmup
04/06/15   sp       CR848458: Restrict CR844167 changes to Tabasco only.
29/05/15   cja      CR839948 For QTA gap build preload scripts in QTA startup, send to GFW in cm startup
29/05/15   mc       CR840262 : COEX handling for Dual-Multimode DR-DSDS
28/05/15   sp       CR844167: Fetch G support device from TRM in multisim mode at CM init
26/05/15   cja      CR843257 In CM startup pass TQP from RF to GFW, rather than RF device
19/05/15   zf       CR839883: Issue QTA startup command at the frame before QTA gap starts
19/05/15   zf       CR840672: New WL1 GL1 interface to indicate IRAT in QTA operation
14/05/15   dp       CR838495 Fix broken power fields in SCH metrics log packet
12/05/15   zf       CR836865: Check margin for QTA startup in warm up.
07/05/15   zf       CR832962: Issuing QTA_startup command at the frame QTA gap starts.
11/05/15   jj       CR 833676 set correct IRAT mode for RF in case of W SUB 2
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
09/05/15   ch/pa    CRXXXXXX Split RX/TX changes for MSIM/SSIM
08/05/15   ar       CR834975 Double the RF tuning parameters in the GL1 for W+W  
07/05/15   mc       CR830988 COEX Channel ID selection for X2G IRAT measurements does not support TRM_UMTS2
30/04/15   cja      CR830934 Ignore QTA startup with invalid RF device, indicate as empty gap
29/04/15   cja      CR800657 QTA gap for G2W IRAT
28/04/15   aga      CR829716 Remove safety checks no longer valid with FEATURE_DUAL_WCDMA enabled for all subs
31/03/15   zf       CR811937 Cleanup CM preload API.
15/04/15   jj       CR822769 Negate the SCH timing offset in x2g 
23/04/15   cja      CR826469 Clear gl1_hw_first_gap_after_preload in qta exit
20/04/15   ip       CR811868 Dual Multisim mode code changes + debug overhaul
20/04/15   cja      CR825734 QTA use GL1 RF structure so data still valid for rf meas exit.
16/04/15   smd      CR822202 Dual space data for IRAT and QTA
16/04/15   cja      CR822202 Changes for QTA gap rf meas exit (part 2)
16/04/15   smd      CR822202 Remove unused static variables. 
16/04/15   cja      CR822202 Changes for QTA gap rf meas exit.
16/04/15   zf       CR822202 Removing rfmeas_exit from qta_cleanup. 
16/04/15   smd      CR822202 Dual data space for preload flag.
16/04/15   aga      CR822202 TRM interface for QTA
15/04/15   cja      CR822684 Changes for NULL2G RF API
14/04/15   zf       CR822344: Using new RF common API for gl1_hw_cm_get_rf_irat_timing_info
09/04/15 cja/jj     CR819956 Thor 2.x RF API Changes
06/04/15   zf       CR818238: Add X2G IRAT APIs using common RF param struct
31/03/15   pa       CR815621:GRM restructuring for TH 2.0.
30/03/15   mc       CR773788 : COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
23/03/15   pg       CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP
17/03/15   jk       CR805689:ARFCN not logged correctly  in GSM_NCELL_ASYNC_METRICS, 0x507D
11/12/14   js       CR769555: Don't issue X2G start up as immediate async command in X+G qta
09/03/15   cs       CR802822 Remove power measure cap for DESENSE channel
10/03/15   cja      CR805612 Handle conventional SCH found in same ISR as FCCH
05/03/15   cja      CR803591 Ignore stale SCH data for parallel FCCH/SCH
23/02/15   js       CR785901: GERAN GL1 to support L2G QBTA
04/03/15   br       CR802289 QTA:GL1 need to set QTA gap as empty gap, if deactivation is going on when gl1 recived qta startup
07/01/15   jj       CR777805 Add probe burst flag for diversity rx
27/02/15   cja      CR768169 Add RF Seq Number
02/25/15   smd      CR800283 Register garb callback function for inactive sub too if modem is in DR mode.
17/02/15   br       CR796211 QTA:Don't start QTA from GL1, if gl1 deactivation is going on.
23/02/15   mc       CR759541 Enable COEX managment for X2G IRAT activities in DR-DSDA
22/01/15   zf       CR784496: Enable sleep when BG traffic enabled.
20/02/15   js       CR693771: W+T removal. Resolving compilation errors
18/02/15   cah      CR782091: Make sure X2G uses correct Rxalpha in DR mode.
22/01/15   zf       CR785609: Define gl1_hw_qta_event_data as a global variable
09/02/15   jj       CR 793229 making debug f3 for crash in the middle of the burst under NV control
17/11/14   jj       CR 752202 GL1 - bolt - Debug support for crashing in the
                      middle of rx burst for a given range of rssi value
01/02/15   js       CR789847: In T2G while CM init abort RF scripts.
23/01/15   jk       CR788012:x2gRxfeConfigAlpha set to 30 for T2G and for other X2G set to 0
15/02/15   sp       CR729492 Remove unimportant logging
13/02/15   js       CR782437 Providing API to WL1 for ROTA enhancement
11/02/15   gk     CR710069: Make gl1_qta_wcdma_has_initialised dual data spaced
04/02/15   cja      CR789095 Store X2G acquisition tags to use for SCH results
03/02/15   cja      CR790483 Featurise T2G parallel FCCH/SCH, so removed from Thor 1.x
28/01/15   cja      CR767058 T2G using parallel FCCH/SCH
27/01/15   cja      CR780982 For CM SCH, get metrics for correct burst
27/01/15   npt      CR697236 Mainline FEATURE_GERAN_VSTMR
23/01/15   npt      CR786002 Fix compilation errors on Thor when FEATURE_DUAL_SIM is enabled
21/01/15   cja      CR784601 For L2G script prebuild (every gap) clear first gap after preload in cm startup
09/01/15   dg       CR779348 SKip index 0 while retrieving power meas. results if the flag skipfirstburst is set
07/01/15   cja      CR768190 Add gl1_hw_cm_cleanup_done_cancel (for rf meas exit) in X2G rf abort.
15/12/14   ws       CR771244 GL1: Compile warnings for LTE,1X,GERAN build flavour
12/12/14   cja      CR754853 Add support for Parallel SCH detection handling
15/12/14   cja      CR769459 Cancel QTA gap if preload or startup too late, cancel T2G gap if scripts not prebuilt.
26/10/14   xz       CR758513 distinguish which RAT is coming for CM initialization by task_ptr
10/12/14   ws       CR768376 Q6 compile warning fixes
04/12/14   dp       CR766103: Fix race condition cleanup of prev gap and startup of next
03/12/14   npt      CR759467 Fix compiler warnings
02/12/14   cja      CR760267 For X2G SCH decode structure make sch_data an array and add metrics
01/12/14   cja      CR764639 Use FRAME_TICK_SETUP_MARGIN_QS rather than mdsp_ftsm to determine if immediate preload.
26/11/14   jj       CR 760174 fix  compiler warning
14/11/14   cja      CR754529 when prebuilding scripts, have preload offset as immediate
12/11/14   cja      CR754776 Changes for L2G IRAT prebuilding scripts, but preload command still part of cm startup
11/11/14   sjv/cja  CR754278 Fixing KW errors on BO.2.0.x
07/11/14   cja      CR751049 Add cm FCCH and SCH results API (ready for parallel FCCH/SCH)
11/10/14   js       CR752885 Ensure GL1 should  not go to sleep if RF abort is pending in empty QTA scenario
27/10/14   xz       CR745396 Set Correct RFM when initialize X2G IRAT compressed mode
27/10/14   cja      CR746760 In X2G power measure tie results buffer index to state machine
30/10/14   cja      CR747066 For X2G do not wake up MDSP if already disabled  by CM terminate
09/10/14   js       CR736257 Don't set rxlm buffer index to INVALID in T+G IRAT
27/10/14   cja      CR746760 In X2G power measure tie results buffer index to state machine
15/09/14   xz       CR720639  void frame number wrap up issue in gl1_hw_x2g_acq_ft_handler
17/10/14   cja      CR730201 Send rf meas exit after cleanup rather than waiting inline for RF scripts
17/10/14   jk       CR741194:Initialize LIF params to 0 for L2G measurements
16/10/14   sjv/cja  CR736060 WRF interface changes, structure type ref. modified for rfm_meas_common_script_exit()
06/10/14   pg       CR734405 Thor compilation errors
03/10/14   cja      CR733950 In CM init, ensure GFW sleep after generic config command has been sent.
29/09/14   nk       CR730078  Set GFW mode as active during CM gap to avoid meas clash with device mode change
29/09/14   cja      CR731807 If previous QTA gap cleanup not complete at startup do not abort RF scritps
26/09/14   sjv      CR731259 Correct device id for G2X IRAT
24/09/14   cja      CR726441 In QTA wait for RF startup scripts to built before sending X2G startup
24/09/14   cja      CR728333 Set X2G header target RF device ID to match target parameter device ID
17/09/14   cja      CR720892 Ensure x2g RF abort in CM terminate for T2G
15/09/14   cja      CR722557 Prevent preemption when sending immediate preload command
01/09/14   dg       CR716901 Clear flag stop_acq_offset_valid on issuing GFW_STOP_ACQ_CMD to GFW
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
11/09/14   ws       CR723099 Resolve GL1 compile warnings
09/09/14   jk       CR722274:Device ID to be passed in FW enter mode command
05/09/14   cja      CR717628 For mulitSIM mode set IRAT/QTA target RF device to be same as source device
14/04/14   pjr      CR641577 Replaced FN with Seq Number in cxm log table
28/08/14   am       CR716841 enable device id in enter mode command as FW is supporting now
25/08/14   js       CR712175 Abort RF scripts if QTA cleanup is not done and other tech triggered CM gap.
08/04/14   js       CR695319 :In T2G don't set first gap after preload to false if startup script are not actully used.
22/08/14   cja      CR643648 Desense for channel adjacent to spur
20/08/14   cja      CR711566 CM init/terminate during QTA gap
19/08/14   cja      CR709767 Only clear gl1_hw_first_gap_after_preload when commands sent to GFW
13/08/14   cja      CR709452 Race condition between QTA cleanup and QTA fw rf init.
12/08/14   cja      CR707710  Optimise preloading timeline for L+G QTA
07/08/14   js       CR705278 : In W+G scenario in empty qta gap call RF API with gap aborted set to TRUE
07/08/14   og       CR706449. Fix compilation errors when dual sim is not defined.
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
04/08/14   cja      CR700197 Use GL1 RxLM buffer for QTA (unless shared with IRAT)
28/07/14   ssh      CR700687: Handle X2G start up from warm up ISR
10/07/14   jk       CR688587:GL1 changes to support LIF feature
28/07/14   cja      CR697957 For QTA startup run build scripts in DPC task
27/07/14   ap       CR678067: Race condition between APP mode as ACTIVE and Device mode change as DSDA leads to crash
15/05/14   jj       CR 664355 Memclr rx_chan_params to avoid junk values in rx params send to rf
30/05/14   jj       CR656854  Resolve Critical Klocwork errors in gdrivers code space
27/05/14   xz       CR669952 extend gfwhost_cmdbuf_offsets as mutimode space
18/07/14   pg/pa    CR694601:GERAN L1 support for dedicated WTR hopping with FED architecture
10/07/14   js       CR593074- ROTA/QBTA Feature implementation
04/07/14   cs       CR688003 GRM functions moved to gpl1_grm_intf.c
08/07/14   cja      CR690023 For QTA prevent prebuilding scripts, if building in progress
13/06/14   ka       CR675110:TRM is released in extended paging scenario - limit the amount of tracing we are spewing
07/07/14   cja      CR689991 Increase preload command action time to 750qs.
03/07/14   og       CR652720. L2G DSDA bring-up updates.
27/06/14    ss      CR686989 Add HDR support to gl1_hw_qta_startup API
24/06/14   og       CR638097. L2G DSDA support.
30/05/14   pg       CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM
17/06/14   dp       CR680677 gl1_hw_cm_startup_geran takes as_id. Not gas_id.
19/06/14   cs/ch    CR651795 TRM Updated changes for DR-DSDS
06/13/14   zc/smd   CR678923 Gave the right device number during QTA.
12/06/14   ssh      CR671029: On G+G wake up collision defer wake up by 1 frame
10/06/14   cja      CR678091 Correct test for T2G queued too far away (CR630086)
09/06/14   cja      CR670351 For QTA empty gap wait for FW preload before aborting RF scripts
06/06/14   us       CR677025 Not calling Rf script abort for empty T2G QTA gap
30/05/14   ssh      CR673304: Allow QTA gap on multimode sub for SGLTE + G DSDA scenario
21/05/14   ssh      CR668643: gl1_hw_qta_gap_active() should return FALSE for multimode sub
15/05/14   am       CR664658 Ensure GFW is awake during initialization of mdsp
14/05/14   cja      CR664278 Only set qta_cleanup_pending false when cm startup run.
                             For T2G share preloaded scripts between IRAT and QTA.
13/05/14   cja      CR663249 If X2G RF abort not need to call RF abort, return success
12/05/14   cja      CR662836 Add support for qta_cleanup_pending to Bolt.
08/05/14   pg       CR662030 Populate sub_id in payload or irat_timing_info
23/04/14   ss       CR630086 Avoid scheduling T2G gaps if more than 4 frames away
07/05/14   cja      CR658621 Optimise QTA and T2G F3.
07/05/14   cja      CR656165 Add external API for CM gap complete
02/05/14   cja      CR658607 Clear any unused prebuilt scripts at CM terminate
29/04/14   cja      CR657347 Correct store of tags for x2g power measure with RF task
14/04/14   ss       CR646261 FR GSM CM INIT via message in GL1 task context
25/04/14   cja      CR655328 Prevent deadlock by doing nothing for mdsp sleep cmd in X2G or QTA
25/04/14   cja      CR655404 Use CM RF device ID for QTA and CM in rx burst
24/04/14   pg       CR655101 Compilation error fixes due to CR644441
17/04/14   cos      CR636543 - Changes for enabling Rx Diversity on Bolt - GL1/GRF intf updates
17/04/14   cos      CR646420 Changes for enabling Rx Diversity on Bolt - GL1/GFW intf updates
14/04/14   cja      CR649182 If cleanup has second async message do not send message in cm cleanup.
14/04/14   ap       CR645063: Incorrect device identity being used in Factory Test Mode
11/04/14   cja      CR644441 Bolt: Preload script for QTA
11/04/14   cja      CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
11/04/14   pg       CR645875 Fix Bolt multisim compilation errors
09/04/14   jk       CR645719:use correct gas_id to retrieve rxlm buffer index while VregOff request
04/04/14   cja      CR641832 Leave check of IRAT buf idx to RF.
04/04/14   cah      CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
26/03/14   cja      CR638920 Add check for CM initialised when prebuilding statrup/cleanup scripts.
26/03/14   ab       CR642231 : COEX : GL1 Arbitrator to allow upto 10 Monitor Freq required for X2G to prevent Invalid Freq Id
                    and possible memory corruption
26/03/14   ab       CR639078 : COEX : CXM Priority and Freq ID for cm_power_meas
24/03/14   br       CR635881 In QTA gap call rf meas exit only if rffw intialized.
19/03/14   ws       CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
19/03/14   cja      CR632037 Use RF Task for X2G
14/03/14   pa       CR631907:Bolt: Fixed compilation errors on enabling FR_17618 feature.
14/03/14   us       CR628225: Fixing compilation warning due to unintialized gl1_skip_first_burst  flag introduced in CR 562238
12/03/14   pa       CR630228:Bolt: Changes due to obsolete FW API cleanup
11/03/14   sc       CR628645 : Print F3 when RXLM buffer is allocated
12/03/14   ssh      CR627985 Skip first burst decode when waking up from sleep inside a QTA gap
09/03/14   dv       CR628618 For QTA ensure rf_rxlm_update set before startup scripts built
07/03/14   dv       CR628324 Short term QTA fix for L2G BGTraffic transition
06/03/14   dv       CR623655 Ensure RF cleanup scripts are built
06/03/14   ap       CR626781. SGLTE+G IRAT is using an incorrect device
03/03/14   cja      CR625957 After CM cleanup check MDSP is ready to sleep before sending sleep
27/02/14   cja      CR622900 Ignore CM terminate if CM not initialised
27/02/14   og       CR606569. Correction to T2G CXM logging.
26/02/14   ssh      CR623243: GL1 not able to detect source tech correctly on empty L+G QTA gap leading to crash
25/02/14   cos      CR608951 GERAN RFLM FED: Pass subscription ID to GERAN RF in all message router payloads
25/02/14   ssh      CR621741: GL1 does not release TRM lock after QTA gap when it self denies TRM as SCE schedules within QTA
19/02/14   zc     CR620061 Updated QTA Rx burst fix
19/02/14   zc     CR620061: Updated QTA Rx burst fix
19/02/14   am       CR604674 Take spur into account for T2G acquisition
19/02/14   zc     CR620061 Updated QTA Rx burst fix
18/02/14   cja      CR619248 Add check for prebuilt preload time before x2g startup
14/02/14   us       CR609569 Ensure correct GFW interface offset is getting stored
12/02/14   cja      CR610689 Add API gstmr_rd_hw_fn_qs (synchronised FN and qs)
12/02/14   ssh      CR613924: Skip the first burst if we are in QTA gap during wake up transition.
13/02/14   zc       CR615778: Keep track of the AS_ID used for the QTA gap
12/02/14   ssh      CR613924: Skip the first burst if we are in QTA gap during wake up transition.
07/02/14   dv       CR612960 In QTA if cleanup scripts never built ensure RF aborts.
06/02/14   cgc      CR598061 Corrections to use of mdsp_intf_shared_var_read(), dereference pointer
07/02/14   br       CR578331 Race condition between  compress mode termination and clear msg  idbuffer
30/01/14   cja      CR609441 For pre-built scripts (T2G) set correct offset for preload command
30/01/14   npt      CR608373 Enable opt3 on Bolt/Remove unused apis
27/01/14   cos      CR604531 Synchronize interrupt deregistration and ISR handling:
                             GERAN updates for treating VSTMR deregister based on the current task
27/01/14   pa     CR607716:GL1 changes related to legacy GFW interface changes.
27/01/14   dv       CR598500 L2G QTA: GFW receives a wrong time offset for a sync rx command
24/01/14   dg       CR592932: Removed Prevent tool compiler warnings
20/01/14   jk       CR503359 - GL1 changes to support Rx diversity
19/01/13   dv       CR602224 QTA crash because TRM lock released after wakeup from sleep
15/01/14   cja      CR599834 Set gl1_hw_cm_serving_rat_info.mode in cm init (even for VSTMR)
15/01/14   us       CR600771 Merge error correction in CR562238
14/01/14   og       CR589868. T+G DSDA support.
13/01/14   am       CR592893 Cap power measurement result on spur channels
10/01/14   cja      CR598696 Set all RF parameters in X2G
31/12/13   xz       CR594706 record more status for compressed mode
09/12/13   sk       CR586833 Set CM startup offset to 4600qs
08/01/14   cja      CR593115 Add passing RxLM index in preload command to FW
08/01/14   us       CR562238 Partial T2G gap usage for RSSI measurement
07/01/14   dv       CR596992: X2G NCELL FB search QTA crash
06/01/14   cja      CR596240 Wake-up gstmr and schedule event, even if not re-initialising gstmr
02/01/13   dv       CR592228 RxAGC flooring after few QTAs
17/12/13   cgc      CR590787 Increase GL1_FW_CXM_CM_START_UP_OFFSET so it is in next frame
13/12/13   cja      CR589473 Add setting GfwAppModeType in GfwHostRfmEnterModeCmd
06/12/13   cja      CR585478 Check for stale acq before sending RSP to TL1.
10/12/13   ws       CR 587586 - Remove DUAL_SIM featurisation in API
02/12/13   sp       CR583373 Check FW status, before requesting for Sleep in CM.
12/12/13   cja      CR585605 Only turn tx on if not already on (so do not get 2 Tx config commands)
11/12/13   cja      CR570704 Add T2G RF pre-building startup/cleanup scripts
12/11/13   cja      CR575034 Add IRAT preload script for X2G.
29/11/13   am       CR578966 gstmr_rd_qsymbol_count() should be called only once for the check in while loop
29/11/13   sp       CR580821: Add F3s to track CM mode configurations.
26/11/13   dv       CR580909 Ensure of a 'empty' QTA gap the pending cleanup flag is set
23/11/13   nk       CR574473 Regression of CR567199
22/11/13   sk       CR576246 Ensure Clenup scripts status not cleared while populating startup scripts inside the QTA gap
21/11/13   zf       CR579808: Can't deinit QTA FW without inited.
19/11/13   ssh/ip   CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
15/11/13   nm       CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
14/11/13   cja      CR554146 Remove F3 to reduce processor load in T2G
09/24/13   sc       CR538584 T2G Statistical runtime histogram for performance evaluation
12/11/13   sk       CR542303 Set CM Startup offset to 5150qs for partial QBTA
24/10/13   dv       CR567199 Ensure QTA cleanup not issued if startup hasn't been issued
18/10/13   sk       CR563336 ERR_FATAL if compressed mode initialised on non multimode SUB
03/12/13   cja      CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
29/11/13   am       CR578966 gstmr_rd_qsymbol_count() should be called only once for the check in while loop
22/11/13   cja      CR455527 Remove featurisation from files in API directory
20/11/13   cja      CR570704 Add T2G RF pre-building startup/cleanup scripts
15/11/13   ka       CR576391 Store LTE tuneaway parameters on call of gl1_hw_qta_startup()
12/11/13   sk       CR542303 Set CM Startup offset to 5150qs for partial QBTA
04/11/13   pa       CR570702:Featurisation of WCDMA DBDC W2G
01/11/13   pa       CR569692: X2G via RF Task.
09/24/13   sc       CR538584 T2G Statistical runtime histogram for performance evaluation
19/11/13   ws       CR578570 Fix compile warning when DSDS is defined and FEATURE_GSM_TDS_DEBUG is not
11/18/13   sk       CR563336 ERR_FATAL if compressed mode initialised on non multimode SUB
12/11/13   dv       CR574473 Regression of CR567199
24/10/13   dv       CR567199 Ensure QTA cleanup not issued if startup hasn't been issued
25/10/13   ss       CR567291 Replicate W2G RF script building for startup and cleanup from  CM to QTA
11/10/13   cja      CR554146 Remove F3 to reduce processor load in T2G
01/10/13   cja      CR550069 Correct T2G num_source (regression of CR547477)
04/10/13   sk       CR553842 Ensure to pass RF header with RFM meas APIs for QTA cleanup
02/10/13   cja      CR550678 Decrease T2G startup alpha from 150 to 130qs
01/10/13   sk       CR552259 CR sync from 1.0/1.1
27/09/13   cgc      CR523552 add support for FR 16939 L+DSDS LTA/QTA
19/09/13   cc       CR547477 WCDMA DBDC W2G support
19/07/13   gk       CR515617:Don't delay the interrupt if W has the lock and its a rude wakeup
24/09/13   cja      CR539766 Set num_tasks to 0 when clearing CCS buffers + add RF exit for cm complete
17/09/13   cja      CR546121 Send generic config for x2g start alpha before first x2g startup.
21/06/13   ss       CR501843: Pass correct parameter to RF when T2G gap is aborted
16/09/13   cs       CR503042 Updated XO RGS handling for Triton
11/09/13   sk       CR542303 Set CM Startup offset to 4600qs for partial QBTA
06/09/13   dv       CR533706 G cell RSSI looks floored always for T2G
03/09/13   sk       CR537487 Correcting the merge error that is over-writing the cleanup source param
03/09/13   npt      CR524770 - GL1 changes to support RFLM FED
23/08/13   sk       CR533467 Partial QBTA bringup on 2.0 in DSDS
21/08/13   npt      CR531633 - Remove FEATURE_DIME_MODEM dependencies
19/08/13   dv       CR531207 fix compile error in CR526341 when TDS enabled
19/08/13   sk       CR531086 Ensure T2G specific code doesnt get executed in TSTS WGG mode
16/08/13   xz       CR529627 When terminate Compressed mode, also clear message id buffer.
13/08/13   zf       CR528357: For QTA, issue X2G_STARTUP_CMD without offset.
14/08/13   dv       CR526341  T2G QTA changes
23/07/13   og       CR503871. Don't send the async command if the host i/f memory ptr is not setup
23/07/13   ss       CR517408 Check stop_acq_pending while issuing X2G_STOP_ACQ in acq ft handler
08/08/13   cja      CR524892 QTA uses immediate cmd after startup/cleanup, but should not inc current buffer mod3
07/08/13   ss       CR514444 Use as_id to read symbol count instead of gas_id
06/08/13   cja      CR519743 Set gl1_rf_gsm_timing_info in compressed mode.
02/08/13   cja      CR516212 For T2G set RF header for rfm exit.
01/08/13   cja      CR511042 Separate RF ID for active and CM.
31/07/13   cja      CR498800 Use TRM to get IRAT RF ID
31/07/13   cja      CR514255 Only add generic config commands when issue message to FW
31/07/13   cja      CR481423 New RF IRAT API to add RF ID
02/08/13   sk       CR522533 Ensure CM startup offset is 5150qs inside the partial QBTA gap
29/07/13   sk       CR519663 Partial QBTA bringup changes
22/07/13   og       CR484448 Ensure cm gsm init frame tick delay is not close to boundary and
                    mutex lock/unlock need to be removed
11/07/13   cja      CR511042 Separate RF ID for active and CM.
21/06/13   ss       CR501843: Pass correct parameter to RF when T2G gap is aborted
04/07/13   cgc      CR498800 Add gl1_hw_cm_trm_init() and gl1_hw_trm_grant()
19/06/13   cs       Major Triton TSTS Syncup
18/06/13   cja      CR498800 Use TRM to get IRAT RF ID
13/06/13   xz       CR497861 When error happened in STARTUP CMD, also queued SCH/ACQ/PWR CMD
31/05/13   br       CR490883 F3 optimization
13/06/13   cja      CR499799 Set RF traget parameteres after LTE conversion function
04/06/13   cja      CR481423 New RF IRAT API to add RF ID
31/05/13   cja      CR494996 For Stop Acq if offset is 5000 - 5625 in N+2 force async int
21/06/13   ss       CR501843: Pass correct parameter to RF when T2G gap is aborted
10/06/13   cja      CR494996 For Stop Acq if offset is 5000 - 5625 in N+2 force async int
13/06/13   xz       CR497861 When error happened in STARTUP CMD, also queued SCH/ACQ/PWR CMD
05/06/13   ss       CR496482 T2G : clear gap cmd and rf_ccs_irat_buffer only if issued
07/06/13   sai      CR 497681: RXAGC drop in W-FACH leading to OOS
02/06/13   sai      CR 481896: Ensure GL1-GFW state coherence for W2G BPLMN searches
29/05/13   ss       CR493635 Initialize correct GSTRM duing X2G compressed mode
17/04/13   dv       CR476103 Initialise X2G Handler data structures
29/05/13   cja      CR486839 Fix Klockwork error introduced by CR464446
29/05/13   dv       CR488644 Regression of CR464446
24/05/13   cs       More MCPM TSTS Updates
17/05/13   cs       TSTS Updates
03/05/13   xz       CR483086 Regression of CR464446
21/02/13   dv       CR448609 Handle T2G HO GFW transition from MEAS mode to ACTIVE mode
18/03/13   xz       CR464446 For T2G, changes for message id usage between TL1 and GL1.
22/05/13   ap       CR485912: Revised GFW state transitions
01/06/12   ss       CR437969 Set  FTSM to zero while sending commands to GFW in X2G compressed mode
20/05/13   cja      CR486893 Support G2T gap offset in range (5000+FW margin) to (5000 + FTSM)
17/05/13   ss       CR486978 Print gas_id and as_id during cm init and terminate
16/05/13   og       CR463285. Mutex protect setting the frame tick delay in W2G.
27/04/13   cja      CR481303 Allow an extra frame for MDSP cleanup
27/04/13   og       CR405322. Handle the hyperframe wrap around for T2G stop acq correctly.
25/04/13   pjr      CR480197 dual dataspaced gl1_hw_cm_status
25/04/13   ss       CR479809 Pass as_id while calling mdsp_wakeup function
24/04/13   pg       CR449656: GSTMR WCDMA Tick delay may not be 625
20/04/13   ws       CR 478028 - resolved link errors with enabling QTA
19/04/13   dv       CR458558 T2G:Don't issue GCC from ISR
19/04/13   mc       CR477448 DSDS: QTA Support : Initial code changes & Triton Bring-Up - fix build errors
17/04/13   cja      CR467946 Cancel cleanup done when CM terminate
13/04/13   mc       CR471759 DSDS: QTA Support : Initial code changes & Triton Bring-Up
27/03/13   cja      CR468678 Set cmCleanupAlpha shared variable for FW for T2G
25/03/13   cja      CR466302 Don't increment current_buf_mod3 if already done by immediate command
25/03/13   cja      CR462223 Clear monitor CCS buffers if gap aborted, reduce F3
22/03/13   cja      CR461030 Add RF CM Enter and Exit
21/03/13   ws       CR466426 - Initialise public,private and shared interface in cm_init()
01/03/13   cja      CR450178 In T2G add call of rfm exit after long or delayed gaps.
28/02/13   cja      CR455032 Changes for build without TD feature
26/02/13   ws       Added MCPM_GERAN1 tech enum for GSM only MCPM callback
06/02/13   jk       CR449735: Resolving KW errors
06/02/13   cja      CR449181 Send abort exit to RF if send message has error
05/02/13   og       CR446308. Ensure the HMSC flag is set to FALSE via the RF API when WCDMA
                    is active and scheduling IRAT W2G measurements.
05/02/13   cja      CR440314. Don't issue commands using DPC in compressed mode init.
04/02/13   og       CR428921. fix remaining hyper frame wrap around checks.
01/02/13   cja      CR448030 Clear gap commands for aborted gap
31/01/13   pg       CR446164 Reset IRAT RF CCS event buffer status to IDLE
25/01/13   cja      CR445512 Register MCPM call backs in cm init
23/01/13   cja      CR443889 Tidy up rat_info.mode
22/01/13   cja      CR443834 Extend mdsp_awake to indicate FW enable/disable
19/01/13   cja      CR438712 Set FN to match command FN offset for mdsp immediate cmd
17/01/13   cja      CR439545 If gap aborted set IRAT CCS scripts to idle
10/01/13   og       CR407309. Crash due to cleared RXLM buffer access by GFW.
03/01/13   cja      CR437176 Set source and target parameters for TD CM RF scripts
20/12/12   pg       CR434756: send enter_mode command after getting IRAT timing info
19/12/12   cja      CR411308 Dime T2G changes (msg router to GFW)
17/12/12   og       CR407749. NULL gsm cell pointer fix.
07/12/12   cja      CR429173 Remove Dime compiler warnings
05/12/12   pg       CR427361: GFW/RF shared memory interface cleanup
28/11/12   pg       CR425504: W2G CM issues due to different Rx Alphas passed to GFW and WL1
28/11/12   ws       CR424774 Featurise setting source params for cm_startup based on TD,LTE
                    featurisation
26/11/12   ss       CR407678 Avoid issuing T2G stop acq command twice when stop acq
                    and cleanup offset is 5000QS
19/11/12   cja      CR422874 Add pseudo Hw FN for Dime
14/11/12   dv       CR420976 T2G: Queued cmds are entered twice in scheduler
                             causing err_flag to be set
14/11/12   npt      CR420954 - Added changes for power collapse
14/11/12   pg       CR419723: Further updates for WG IRAT on Dime
07/11/12   dv       CR418770 T2G: Race condition between GL1 issuing async int
                             from gstmr_isr & GFW clearing buffer
06/11/12   dv       CR417464 T2G: Crash when GFW does not see CMDS in async int
05/11/12   cja      CR409227 For CM init allow time for all generic config commands
01/11/12   dv       CR416987 T2G: Crash caused by incomplete fix for CR400143
26/10/12   dv       CR400143 T2G: Crash caused when in 1 GSM frame 2 sets of API
                             calls are made which are not for back to back gaps
03/10/12   dv       CR404964: Race condition when 2 T2G CMDs issued in 1 G frame
12/10/12   pg       CR409233: use RF Task CCS buffer implementation for all X2G measurements
09/10/12   cja      CR407407 Prevent async int in wrong frame after frame wrap
24/09/12   ky       T2G Async message router changes
24/09/12   og       CR402775.T2G BSIC triple buffered.
17/09/12   dv       CR400279: For offsets above 5000, check we issue async int
                    in correct frame
10/09/12   dv       CR382139 ftick parity error causing T2G cmds not to be issued.
07/09/12   cja      CR385323 If restart gsmtr also restart MDSP
04/09/12   og       CR395649. T2G Acquisition and BSIC decode enhancements.
04/09/12    og      CR391191. Prevent T2G RSSI 0 results.
15/08/12   cja      CR385484 Add flag for async interrupt in num cmds
07/08/12   dv       CR385872: T2G threshold of allowing ftick to process command too low
07/08/12   dv       CR371181 GFW CMDS picked up twice due to race condition
                    between ftick and async int
25/07/12   dv       CR381449: T2G BSIC RECONF fails due to handler uninstalling early
25/07/12   dv       CR382265 unify Rx Alpha value used between TL1 and GFW
09/07/12   cja      CR361706 Ensure generic config command sent at correct time
04/07/12   cja      CR375634 Reset gstmr for CM after GSM deep sleep inactive.
19/06/12   ss       CR:365989 - Furthur update to this CR
19/06/12   dv       CR371669: G2T Crash due to seq_num being re-used
18/06/12   dv       CR362062: Ensure that SCH handler for T2G not uninstalled too early.
11/06/12   ss       CR361180. Cleanup is written to the cpu/dsp i/f after async interrupt
                    for pwr_msr and sch when the time offset is in the 5000-5625 range.
11/06/12   ss       CR365989 stop acq and cleanup using second async interrupt in FT.
11/06/12   og       CR369985. Wrong RSSI values during T2G dedicated and idle modes.
08/06/12   cja      CR363269 Do not clear IRAT SBI/NonSBI buffers
29/05/12   og       CR365872. Stop the power measurement state in T2G from getting
                    activated while pipelining the commands
29/05/12   og       CR365870. Set the SCH uncertainty window to zero in T2G.
29/05/12   og       CR363662. Take the async interrupt into account when booking x2g
                    events.
29/05/12   dv       CR365112 T2G:incorrect FN processing at hyperframe wrap
25/05/12   pg       Add support for RF Task
18/05/12   og       CR361191, CR361954. Wrong RSSI results occasionally get correlated
                    with the commanded arfcns.
17/05/12   og       CR362647 Stop generic config command updates in T2G gaps
14/05/12   og       CR361366. Allow the x2g SCH reconf handler to run in parallel
                    with the x2g power measurement handler.
14/05/12   og       CR361351. quad buffer the power measurement state machine
                    to allow gsm cells > 10 to be measured.
11/05/12   og       CR359890. Prevent incorrect queuing of x2g command nodes.
10/05/12   og       CR360664. Limit rfm exit (with no abort) calls to non-T2G scenarios.
10/05/12   cja      CR347056 Add rfm meas common exit call
06/05/12   og       CR358480. Triple buffer the power meas state machine and
                    increase the robustness of the x2g search routine.
06/05/12   dv       CR358962 Reduce x2gALPHA value for T2G from 175 to 150
03/05/12   og       CR358496. Remove the sending of generic config commands before
                    startup in TDS IRAT.
03/05/12   dv       CR358061 Re-introduce T2G: reduce x2gStartAlpha var for gFW
27/04/12   dv       CR356695 Fix bug in calcualting when to issue stop_acq/cleanup cmds
30/04/12   og       CR357390. Missing SCH reconfirmation response from GSM drivers to TL1.
29/04/12   cja      CR357033  Ensure async int if send command around frame tick
28/04/12   og       CR357019. GSM and MDSP drivers changes to reduce stack usage.
27/04/12   og       CR356911. T2G updates to remove incorrect ADD_FN( ) usage
                    and limit stop acq rejection to idle mode.
25/04/12   og       CR356465. T2G updates for TDS-1361 and 1379.
23/04/12   og       Compilation issues due to RFM_WCDMA_MODE, replaced now with
                    RFCOM_WCDMA_MODE.
23/04/12   og       Ensure source rat is set correctly for non TDSCDMA cases.
                    Completes the fix for CR355124.
23/04/12   og       CR355127. T2G scheduler updates.
23/04/12   og       Avoid resetting the source rat mode in cm terminate and use the correct
                    one when getting the Rf parameters.CR355124.
23/04/12   dv       CR353377 Clear SBI/GRFC buffers When T2G Compressed mode terminated
19/04/12   cja      CR349237 Add API for FTM to reset edge core
15/04/12   og       Further changes to get T2G reselection and redirection working.
                    Part of CR352228 too.
14/04/12   og       CR352228. T2G updates and fixes.
12/04/12   cgc      CR350760 prevent WL1 hangs on cm terminate if gstmr is off + debug
13/04/12   dv       CR351946 Fix bug in CR351577 - default alpha used for W2G, L2G
12/04/12   dv       CR351577 T2G:reduce x2gStartAlpha value
12/04/12   dv       CR351558 Ensure num_cmds reset after each set of T2G calls
11/04/12   og       CR350683 Correct returning TRUE/FALSE from x2g_cleanup().
10/04/12   og       CR350683. Provide x2g cleanup error info to TL1.
07/04/12   og       CR350221. Back out CR48578.
05/04/12   rc       CR349728 t2g rf api profiling
21/03/12   dv       CR349573 T2G:Crash caused by multiple API calls in 1 GSM frame
21/03/12   dv       CR348841 T2G: Increase offset threshold for programming events to occur in next frame
05/04/12   og       CR349656  Improve stability of T2G & add service layer queing
21/03/12   ws       CR 345606 Improve stability of X2G Rxfe stuck recovery
14/03/12   pg       Add support for local HWIO macros (msm.h deprecated)
03/04/12   og       CR348932. T2G scheduling and power measurement error handling updates.
03/04/12   ws       CR348586 Fix compile error for targets that don't support TDS
02/04/12   dv       T2G:CR348512 remove FTSM restriction for startup/cleanup values
02/04/12   og       CR348586. Use the startup command to gate the writing of any further
                    commands to the host-mdsp i/f.
02/04/12   og       CR348578. Disable GFW APP resume and suspend in T2G.
02/04/12   og       CR348576. Expand the X2G startup time offset rejection criterea.
02/04/12   og       CR348585. Ensure tone detect and crc pass are reset during error
                    conditions before the response message is sent to TL1.
02/04/12   og       CR348572. Don't completely trust the HW FN counter.
02/04/12   og       CR348578. Increase the GFW command overhead from 250 to 300 QS.
02/04/12   og       CR348574. Update the gl1_hw_schedule to support X2G IRAT.
02/04/12   rc       CR348608 T2G profiling whereby the array index was not getting incremented correctly
02/04/12   dv       T2G:CR348535 release semaphore if leaving x2g_cleanup early
29/03/12   dv       CR347854:Enable error msg handling for T2G
29/03/12   dv       CR347854:Enable error msg handling for T2G
29/03/12   dv       T2G:CR347217 Add further checks before issuing cmds to GFW
29/03/12   cja      CR338055 Recovery for Nikel RxFE Stuck
28/03/12   rc       CR347451 t2g api profiling
28/03/12   ws       backed out CR 345370 as it breaks W2G compressed mode. GFW is still asleep
27/03/12   mmk      T2G:CR346004 - resolve BSIC confirm bugs
23/03/12   dv       T2G:CR345652 - stale value of QS read causing crash
20/03/12   dv       T2G:CR343786 Make scheduler mechanism flexible enough to deal with multiple power measures
20/03/12   dv       CR345386 Fix FCCH/BSIC detection bugs for T2G
20/03/12   dv       CR345370 - remove unnecessary call to mdsp_awake()
14/03/12   ws       CR343351 T2G startup Qbit offset wrapping around and ended up > 5000 Qbits
07/03/12   dv       T2G:CR341852 Improvements for consistent BSIC performance
07/03/12   dv       CR341856 T2G Remove redundant code causing crash
07/03/12   ws       CR 341565 correct merge error in gl1_hw_cm_mode_sw_handler()
                    ensure mdsp_prepare_for_sleep() is always called
06/03/12   dv       T2G:CR341595 Place more F3 under Feature def to reduce API runtime
01/03/12   jj       CR324166 Reducing the GFW shut-down overhead in W2G idle iRAT
28/02/12   ws       CR 340345 - Corrected merge error in setting up script_type in new RF driver API
15/02/12   dv       T2G:Read metrics when SCH burst detected
15/02/12   dv       Overwrite changes with that from T2G FCCH/SCH bringup
10/02/12   og       CR335121 Do not go to sleep if GCF test
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
06/02/12   cja      CR331207 Wakeup MDSP before sending generic config command
03/01/12   og       CR328336. Expand CR307591 to cover cm init and gl1 task deactivation.
23/01/12   cja      CR331370 Correct cleanup parameter setting for L2G for WTR
17/01/12   og       CR331370: RF WTR X2G GSM drivers changes.
05/12/11   npt      CR315272 - Remove usage of deprecated legacy sleep APIs
27/10/11   cja      CR315397 RFM MEAS functions to be deprecated and removed
26/10/11   cja      CR313666 Fix compiler warnings
19/10/11   ws       CR 313666 Remove hw_* system drivere API's as they are deprecated
                    on Nikel Modem
12/10/11   ws       CR312154 - Featurise remaining clkrgm API calls uder FEATURE_MCPM
29/09/11   og       CR309168 Ensure generic config command sent before cm startup command
22/09/11   cja      CR307155 Replace RF timing definitions with RF API
21/09/11   cja      CR305705 Remove reset of GSTMR
20/09/11   cja      CR307007 Set acq XO value in CM startup.
19/09/11   og       CR307591. Ensure the deactivation and startup don't overlap at the
                    task level.
13/09/11   sjw      DSDS CR303569 - reworked rude wakeups for Compressed Mode
08/09/11   jj       CR302378  Added GL1 changes for RSB along with spectral inversion
20/08/11   cja      CR301307 Register MCPM callback in CM init
11/08/11   cja      CR301096 Pass index to RF driver for startup and cleanup buffers
11/08/11   cja      CR301040 Send AFC update in same frame as CM startup
04/08/11   jj       CR299772 removed gstmr_force_reinit from gl1_hw_cm_gsm_init
01/08/11   ws       CR299007 Reset GSTMR in gl1_hw_cm_gsm_terminate() to avoid crash
                    when WL1 deactivates
01/07/11   nt       DSDS 288704 change to move the wakeup point for GSM in compressed mode
29/06/11   cja      CR294221 - Force gstmr re-init after W activity
17/06/11   cja      Clean up Nikel BU workarounds.
15/06/11   cja      Nikel G2W changes (so WFW does startup)
14/06/11   cja      Force GSMTR re-initialisation in CM init for Nikel
01/06/11   ws       Configure MODEM_CLK_GSM_TRIG to clock DCO update
26/05/11   cja      Add initial set-up of EDGE clock DCO
25/05/11   nf       CR284274 added FEATURE_GSM_LEAVE_GP_CLK_ON
09/05/11   cja      Nikel add RF API for timing info
03/05/11   cja      Nikel W2G use cycle swallower
28/04/11   cja      Add Nikel freq control by GFW
20/04/11   cja      Nikel GFW interface change
07/04/11   cja      Nikel IRAT API changes
30/03/11   cja      Initial Nikel IRAT
21/03/11   cja      Initial changes for Nikel
18/03/11   ws       Added Initial MCPM updates
14/03/11   nt       DSDS change to move the wakeup time for GSM in compressed mode.
03/03/11   pg       Remove target specific hardcoded feature definitions from source files
28/02/11   ip       Fix compilation issue.
14/03/11   nt       DSDS change to move the wakeup time for GSM in compressed mode.
03/03/11   pg       Remove target specific hardcoded feature definitions from source files
28/02/11   ip       Fix compilation issue.
08/04/11   pg       Change #if to #ifdef for GL1_9K_2P4_GSM_RXF0_SAMP_CLK_WORKAROUND
30/03/11   nf       CR281423 switch all clocks are cm init
22/02/11   ip       CR267278 GP clock vote handled by RF drivers
07/02/11   nf       CR273728 added gl1_hw_cm_is_cleanup_done()
28/01/11   cs       CR269985 Use the WCDMA mode vote duting cm_init for GP clcok voting in RF
26/01/11   pg       Added FEATURE_GSM_GP_CLK_DISABLE for sourcing GSM clock
                    from MSM in all modes i.e. GP_CLK always disabled
14/03/11   nt       DSDS change to move the wakeup time for GSM in compressed mode.
03/03/11   pg       Remove target specific hardcoded feature definitions from source files
28/02/11   ip       Fix compilation issue.
20/12/10   nf       9K GFW awake check now uses mdsp_awake()
08/12/10   og       Correction to API behaviour.
08/12/10   nf       Added GFW awake api for WL1
08/12/10   ws       CR 265617 - Add 2 frame delay at end of gl1_hw_cm_mdsp_init to
                    GFW has setup ADie clock source select
02/12/10   nf       9K changed the way clk delays and clk switching is handled
25/11/10   og       CR265384: Delay setting GFW to sleep after CM by 2 frames
22/11/10   og       GSM RXF0 SAMP clock workaround for 9k 2.4 line.
08/11/10   nf       9K suspend resume
04/11/10   ky       CR 262848:added null pionter tcb ptr check in gl1_hw_cm_mdsp_init function
02/11/10   og       Adding bandwidth to the cm info startup i/f.
29/10/10   ky       261817: Added two wait for frame tick in the hw init so that
                    app mode command sent to the fw and fw will process that command
                    Hardcoding the app mode command moved to switch command buffers function
25/10/10   ky       259645: Installed gstmr handler for sending pending commands is not working.
                    Hardcoded the sending these commands in gstmr isr function.
22/10/10   og       Add missing FEATURE_GSM_GPRS_GENESIS featurisation.
21/10/10   pg       Fix compilation error for non QDSP6 MDSP
18/10/10   ky       259645: Installed gstmr handler for sending pending commands. This handler
                    sends the generic config command and app mode command to fw if they are
                    pending
14/10/10   pg       Fix compiler warnings
13/10/10   cja      Fix LINT warnings
09/10/10   nf       9K added FWS suspend/resume
07/10/10   ky       254466:Removed fw unused shared variables from the sw code
20/09/10   og       Added LTE clock regime client support.
23/09/10   tjw      Replace deprecated clk_busy_wait with DALSYS_BusyWait
20/04/10   cs       Fix for CR233619 to correctly apply larger procdelay for CM meas on POSEIDON
06/08/10   pg       Add GSM clock source sanity check when entering CM
03/08/10   og       Correct GFW_MON_ALPHA initialisation in gl1_hw_cm_rf_init().
21/06/10   cs       Fix XO logging for CM_STARTUP and Lint indents
09/06/10   og       removal of CM startup MODEM_TX_CTL hack.
23/05/10   dv       fix compile error in original 9k irat update
20/05/10   dv       Added missing MODEM_TX_CTL write from Osamas
                    9k IRAT checkin
20/05/10   dv       Osamas 9k irat updates.
20/05/10   og       9k irat updates.
16/04/10   cja      Add intial setup of tripler and cycle swallower
10-04-06   tjw      Compiler and Lint warnings
17/03/10   og       Further x2g and g2x updates.
11/03/10   ws       Resolved compile warning with previous checkin
10/03/10   ws       Further featurisation of G2X and X2G
05/03/10   og       Correct the rf band format when calling rf api in x2g.
01/03/10   og       g2x and x2g updates.
26/02/10   ws       Added FEATURE_GSM_RFA_IRAT to protect new RF API
24/02/10   og       Add new RF API function calls.
23/02/10   og       Added x2g startup and cleanup commands.
22/02/10   og       Increase the maximum number of compressed mode power
                    monitors to 10 per frame.
19/02/10   og       Update the CM API to support the new RF API on 9k.
11/24/09   tws      Use HWIO_PHYS macro to pass physical address to MDSP.
13/11/09   ky       Change the reg address from virtual to physical in Q6 mdsp_nonsbi_write function call
08/02/10   cja      Update XO
22/01/10   cja      Remove setting of MDSP Alpha for CM, done by RF driver.
29/10/09   ws       Give MDSP sample server control during CM activities
09-10-22   tjw      include rfm_meas for fn prototypes
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
26/03/09   ws       Added CLKREGIM_HAS_GET_MDSP_ACLK_VAL deom taxis modem
13/03/09   og       FEATURE_XO updates.
25/02/02   cs       Add in support for FreqCommon logging
02/02/09   cs       Fix the LCU build for previous updates
27/01/09   cs       Add latest changes for Mustang iRAT
17/01/09   cs       Revisit the previous char removal as still failed to compile
                    for Mustang
16/01/09   nt       Removal of invalid characters that broke compilation
15/01/09   agv      Mustang loading of the GSM mDSP via gl1_hw_mdsp_init
                    via call from gl1_hw_cm_gsm_init.
10/12/08   agv      Fixed compile error for gl1_hw_tx_on in Mustang.
02/12/08   cs       Add in new split gl1_hw_clk_ctl.h from gl1_hw.h
21/11/08   agv      CR164875-Resolved High Transmit Frequency Error in EGPRS
11/11/08   ap       Fix Lint compiler warnings
21/08/08   og       Mustang bring-up changes.
18/07/08   agv      Changes for LCU so the SSRV is set correctly for CM.
05/03/08   cs       Fix the featurisation of the OKTS change for WPLT to build
07/02/08   cs       CR137106 For the non deep sleep case assert the OKTS for GL1
                    only when WL1 has gone into init and asserted its NOKTS
19/11/07   ws       CR 123029 - Removed clk_regime_set_analog_clk()
21/09/07   cs/ip    Unified GL1 clock control
17/07/07   ws       CR123746 Add LNA_GAIN_RANGE to rfm_set_rx_gain() API
21/05/07   ws       CR 113185 - avoiding the grfc_stop_data interrupt by setting rxbeta to zero
30/04/07   ws       Changes to Feturisation for first pass GSM Only build
06/03/07   agv      Removed MDSP_REG_mdspfwantclkcfg for 7600 as now set by mDSP.
07/17/06   gfr      Wait an extra frame before turning off EDGE clock.
06/16/06   gfr      Fix corner case where EDGE clock was disabled at the start
                    of the CM gap.
05/20/06   gfr      Support for clock regime resource manager.
05/18/06   gfr      Support for enabling the EDGE clock during CM activity.
09/03/06   gfr      Mainline FEATURE_GSM_TCXOMGR_SUPPORT
01/13/06   gfr      Support for new RFM API (FEATURE_RFM_INTERFACE)
11/04/05   gfr      Added #include "customer.h"
09/29/05   gfr      Renamed gl1_hw_tx_clk_enabled to gl1_hw_tx_on
09/22/05   gfr      Removed FEATURE_SBI_MDSP_CONTROLLER
08/19/05   gfr      QLint cleanup
07/07/05   gfr      Remove obselete nonsbi_write to the channel filter
06/10/05   gfr      Mainline FEATURE_HARDWARE_CONTROLLED_CLK_REGIMES
05/05/05   gfr      Ensure the output of stage3 is configured
04/28/05   gfr      Support for mdsp_cm_inact and improved mdsp_cm_init
04/20/05   yh       Remove use of RF fifo
04/08/05   gfr      Move logging code into gl1_hw_log.c
02/02/05   yh       Calls to give/terminate SBI control to mDSP done in hw.c
                    as it is target specific
01/24/05   gfr      Use new CM mdsp buffer types
01/24/05   yhong    Cleanup for W2G build
12/06/04   gfr      Featurize the mDSP SBI controller
11/01/04   gfr      Compile hacks for 6275
09/10/04   bm       Updating MDSP_grfcDataRegShadow for FEATURE_WCDMA_MULTIBAND
07/22/04   gfr      Sanity check the number of power measures.
06/02/04   gfr      Lint cleanup.
06/01/04   gfr      Support for the TCXO manager.
05/27/04   bm       Fix for CM bug in 2.1 rev, requested by rema
05/19/04   gfr      Added support for FEATURE_RF_AFC
04/27/04   hg       Added FEATURE_MULTI_RF_CHAIN_API.
04/09/04   gfr      Use new rotator api.
04/07/04   bm       Don't store PDM, ROT values if we have not started measuring
                    This is to fix resuming to GSM incase of G2WHO failures
03/30/04   jc       Moved single ended clock down a few lines. Caused dog
                    timeouts as switch_cmd_buffers not loaded yet.
03/10/04   bm/rv    Changes for single ended clock + remove l1 hack
03/05/04   gfr      Lint cleanup.
02/17/04   gfr      GSM rotators support.
02/02/04   bm       Set the async_rx_search_width in gl1_hw_cm_gsm_init
01/26/04    jc      Forgot to add FEATURE_TGL14_TIMING flag around the code.
01/22/04    jc      Added monalpha for tgl14 changes. Requires fw and rf driver.
01/20/04   eh       Added call to rfgsmctl_cm_init to program RF GRFC.
12/05/03    bm      Added logging support for MDSPcommands issued,
                    and the corresponding pops
11/24/03   jc       Temp fix for RSSI=0 by using autoACK to clear chfilt DMA.
11/05/03   gfr      Support for quad-band.
11/03/03   jc       Mainlined Saber changes.
10/28/03   jc       Correct error_fatal that can occur when leaving wcdma mode
                    and gsm clocks are off.
10/22/03   jc       Fix for T3124 by adjusting PDM to last WCDMA value used.
10/19/03   jc       Fix for T3124 where missing one gstmr ftick.
10/20/03   bm/jc    Fixes for RAT switch problem
09/11/03   bm/jc    Changes to hide SBI APIs, based on Chris comments
08/15/03   jc       Changes to handle Siemens tight cm pattern.
08/05/03   jc       Mainlined FEATURE_INHIBIT_PSBI, consolidated multiple
                    mdsp_grfc_xxx() calls. Fix for back to back CM gaps.
06/20/03   gfr      Compressed mode power measures use rx alpha not mon alpha.
06/18/03   gfr      Added new parameter to rfgsm_setup_mon_burst.
04/30/03   jc       Put GRFC 2,8,9 changes back.
04/18/03   jc       Backout changes for GRFC 2,8,9 causing GSM w/o QXDM issues.
04/04/03   jcw      Added sbi_suspend to gl1_hw_cm_startup.
04/02/03   jc       Mainline RF_MULTIMODE, remove outpw test code.
03/19/03   rv       Modified settings for GRFC 2,8 and 9 to improve standby current.
02/18/03   jc       Per request from Anil, enable gsm clk regime here.
11/11/02   ih       Added calls to suspend SBI activies when we enter CM.
12/13/02   sd       Changed call from rfgsm_tune_mon() to rfgsm_tune_rx() to
                    get rid of tune type dependency.
12/09/02   ATM      Removed magic "500" from frame delay per conversation with
                    jchao and esy (New constant in KOVU_GTMRS.00.00.19)
12/06/02   JC       Replaced rfgsm_set_rx_gain_data() with rfgsm_set_rx_gain()
12/05/02   eh       Set GRFCs back to static mode first before setting its output
                    state in gl1_hw_cm_rf_init().
11/26/02   eh       Modified gl1_hw_cm_rf_init():
                    - Added more comments.
                    - Removed writes to GPIO_FUNC_SEL_1 and GPIO_ALT_FUNC_SEL.
                      They are done by bio_init() now.
                    - Added initialization for GRFC4 for CM. Set to high normally
                      for WCDMA and toggled to low for GSM during CM gap.
11/06/02   jc       Reset seq nums when entering compressed mode
11/05/02   sd       Added RF driver multimode API calls to support CM
                    intersystem measurements.
10/17/02   jc       Remove setting channel filter , and sbiDelay.
10/16/02   jc       Add gl1_hw_cm_gsm_terminate().
10/14/02   jc       Use MAX_CM_RSSI_MEASUREMENTS instead of gsm one.
10/11/02   ih       Changed startup/cleanup to call rfgsm_setup_cm_startup/
                    cleanup instead of gl1_hw. Changed gl1_hw_setup_mon_burst
                    to rfgsm_setup_mon_burst. (moved to resolve circular dep)
10/7/02    jc       Use RF_MDSP_RX_ALPHA_QS instead of hardcoded value.
10/01/02   jc       Added new argument to rfgsm_compute_rx_power().
09/28/02   jc       Removed RF_BRIAN code.
09/12/02   sd       Added support for RF driver multimode API that includes
                    GSM drivers.
08/07/02   jc       Added RF_init section for ZIF cm.  Change SBI to 6.5Mhz
08/02/02   jc       Removed duplicate functions. Add this file to released VU.
06/28/02   JC       Add call to delay gstmr arm interrupt.add safety check
                    for zero power measures which is not allowed.
06/24/02   jc       For cm_power_measure reverse order of for loop.
04/05/02   jc       Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#include "comdef.h"
#include "memory.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_clk_ctl.h"
#include "gl1_hwi.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"

#include "vtmrs_g.h"

#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "mdsp_debug.h"
#include "mdsp_intf_shared_vars.h"
#include "rfm.h"
#include "rfm_meas.h"
#include "l1_drx.h"
#include "gl1_mutex.h"
#include "l1i.h"

#include "gl1_hw_tcxo_mgr3.h"

#include "rfmeas_types.h"
#include "rfm_device_types.h"

#ifdef FEATURE_GSM_TDS
#include "rfmeas_tdscdma.h"
#endif

#include "rfm.h"
#include "rfm_gsm.h"
#include "rxlm_intf.h"
#include "sys_type.h"
#include "mcs_hwio.h"
#include "geran_msgs.h"

#include "l1i.h"
#include "l1_task.h"

#include "gfw_sw_intf.h"

#include "DALSys.h"

#include "modem_fw_memmap.h"
#include "rflm_ccs_irat_intf.h"

#include "gl1_msgi.h"
#include "gl1_msg_g.h"
#include "gtmrs_g.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "geran_dual_sim_g.h"

#include "gpl1_grm_intf.h"

#include "mcpm_api.h"

#include "gpl1_dual_sim.h"

#include "rcinit.h"

#include "l1_sc_irat.h"
#include "l1_sc_lte_irat.h"

#ifdef FEATURE_LTE_TO_GSM
#include "lte_ml1_irat_ext_api.h"
#endif

#include "gl1_hw_arbitration.h"
#ifdef  FEATURE_GSM_COEX
#include  "l1_log.h"
#endif /* FEATURE_GSM_COEX */

#include "gl1_hw.h"

static void gl1_hw_cm_mdsp_init (rex_tcb_type  *task_ptr,
                                 void          (*task_wait_handler)(rex_sigs_type),
                                 rex_sigs_type task_wait_sig,
                                 rex_sigs_type task_timeout_sig,
                                 gas_id_t  gas_id
                                );
void gl1_hw_complete_cm_terminate( gas_id_t gas_id );
void gl1_hw_cm_get_rf_irat_timing_info (gl1_hw_cm_rat_t source_rat, gas_id_t gas_id);
void gl1_hw_x2g_build_startup_cleanup_script_geran (const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr, gas_id_t gas_id);
static void gl1_hw_cm_cleanup_done_cancel (gas_id_t gas_id);
acq_decode_rpt gl1_hw_cm_fcch_rpt;
static gl1_hw_rx_tags_type  gl1_hw_cm_acq_tag;

#ifdef FEATURE_QTA

void gl1_hw_cm_qta_gap_exit(boolean empty_gap, gas_id_t gas_id);
#define GL1_HW_CM_QTA_ASYNC_CM_LATENCY_QS 1500
/* For FED add extra time for preload script */
#define GL1_HW_CM_QTA_STARTUP_LATENCY_QS 2500

#define QTA_CLEANUP_FW_PRELOAD_WAIT_FRAMES   2

#define QTA_WAKEUP_WAIT_MARGIN_US            10
#define ALLOWED_IMMEDIATE_PRELOAD_MARGIN     200

#ifdef QTA_EVENT_DATA_LOGGING
gl1_hw_qta_event_data_t  gl1_hw_qta_event_data[GL1_HW_QTA_EVENT_DATA_LOG_SIZE];
static uint8  gl1_hw_qta_event_data_index = 0;
#endif /* QTA_EVENT_DATA_LOGGING */

/* Flag that tracks qta cleanup status */

#if defined ( FEATURE_QTA ) && defined ( FEATURE_CXM_QTA )

boolean gl1_hw_get_rx_cmd_in_qta_issued_flag(gas_id_t gas_id);
int8 gl1_hw_get_rx_cmd_in_qta_issued_count(gas_id_t gas_id);
#endif

typedef struct
{
   const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr;
   gas_id_t gas_id;
}  gl1_hw_cm_build_startup_param_type;

gl1_hw_cm_build_startup_param_type gl1_hw_cm_build_startup_dpc_param;

#endif /* FEATURE_QTA */

static void gl1_hw_cm_rf_init(gas_id_t gas_id);
extern boolean l1_is_delaying_interrupt_needed( gas_id_t gas_id );

extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[];

extern gl1_hw_arbitration_t gl1_hw_arbitration[];

void     gl1_hw_init_t2g_handler_sm(void);


gl1_hw_cm_rf_info_t           gl1_qta_cm_rf_info;


#if defined ( FEATURE_QTA ) && defined ( FEATURE_CXM_QTA )   
#define _GL1_HW_X2G_DATA_INIT_VALUE { 0, FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE, FALSE, 0,0,RFCOM_PARKED_MODE,RFCOM_PARKED_MODE,FALSE,FALSE,FALSE, 0, 0, 0, NULL, RFM_INVALID_DEVICE, FALSE,0 }
#else
#define _GL1_HW_X2G_DATA_INIT_VALUE { 0, FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE, FALSE, 0,0,RFCOM_PARKED_MODE,RFCOM_PARKED_MODE,FALSE,FALSE,FALSE, 0, 0, 0, NULL, RFM_INVALID_DEVICE}
#endif /* defined ( FEATURE_QTA ) && defined ( FEATURE_CXM_QTA )*/

gl1_hw_x2g_data_type    gl1_hw_x2g_data[NUM_GERAN_DATA_SPACES]=
                                   { INITIAL_VALUE( _GL1_HW_X2G_DATA_INIT_VALUE ) };

#ifdef FEATURE_GSM_TDS
static uint8 num_pwr_meas;
#endif
/* mdsp cmd buf address*/
typedef struct
{
GfwX2GStartupCmd  *startup_ptr;
GfwX2GPwrMsrCmd    *pwrmsr_ptr;
GfwX2GCleanupCmd  *cleanup_ptr;
}gfwhost_cmdbuf;

static gfwhost_cmdbuf gfwhost_cmdbuf_offsets[NUM_GERAN_DATA_SPACES];

#define CM_Q6_WAIT_BEFORE_SLEEP 10000

#define  MAX_uS_WINDOW_DELAY    (7)

gl1_hw_cm_status_t  gl1_hw_cm_status[NUM_GERAN_DATA_SPACES] =
                      { INITIAL_VALUE_S( FALSE ) };

#if defined FEATURE_QBTA
typedef struct {
boolean gl1_hw_full_qbta;
boolean gl1_hw_build_scripts_in_qbta;
}  gl1_hw_qbta_data_type;

#define _GL1_HW_QBTA_DATA_INIT_VALUE  { FALSE, FALSE }
static gl1_hw_qbta_data_type  gl1_hw_qbta_data[NUM_GERAN_DATA_SPACES] =
                                   { INITIAL_VALUE( _GL1_HW_QBTA_DATA_INIT_VALUE ) };
#endif

#define GL1_CM_CLEANUP_DELAY_FRAMES 4

ARFCN_T cm_rssi_arfcn[NUM_GERAN_DATA_SPACES][MAX_CM_RSSI_MEASUREMENTS];

#if defined (FEATURE_QTA) && defined ( FEATURE_CXM_QTA)
/*
** Rx alpha = 200, to ensure startup is in next frame, > 5000
*/
#define GL1_FW_CXM_CM_START_UP_OFFSET 4600
#define GL1_FW_CXM_CM_CLEAN_UP_OFFSET 0

#define GL1_QTA_CXM_MARGIN ((3* QS_PER_SLOT)+ 1000)
#endif

#ifdef  FEATURE_GSM_COEX
#define PWR_MEAS_LENGTH_QS (MDSP_PWR_MEAS_LEN * 4)
#endif

rfm_meas_xtoy_irat_info_param_type  rf_meas_x2g_irat_info_param[NUM_GERAN_DATA_SPACES];
rfm_meas_param_type                 rfm_meas_x2g_header_target_param;

static rfm_device_enum_type   gl1_hw_get_gsm_rf_id_from_trm(gas_id_t gas_id);

/* Defines for delay waiting for X2G RF processing before rf meas exit */
#define GL1_HW_X2G_WAIT_RF_MAX_DELAY  6000 //us
#define GL1_HW_X2G_WAIT_RF_DELAY_STEP 100  //us

void gl1_hw_x2g_wait_rf_buffer_completion(gas_id_t gas_id);

#define GL1_HW_CM_GFW_PRELOAD_CMD_ACTION_MARGIN_QS  (1500)


#define GL1_HW_SCH_INVALID_METRICS_INDEX 0xFF



gl1_hw_cm_sch_tags_type gl1_hw_cm_sch_tags[NUM_GERAN_DATA_SPACES][Gl1_HW_CM_NUM_SCH_METRICS];

#ifdef FEATURE_GSM_TDS
/* GFW requires 400qbits from ftick interupt to process cmd.
 * if a STARTUP offset is above this value we KNOW that
 * ALL offset should be adjusted to be relative to next ftick
 */
#define GL1_HW_CM_T2G_ADJSUT_STARTUP_TIME_INFO_TRIG_VAL_QS 5400

/* Time GFW needs between GL1 issuing commands and GFW being
 * able to action those commands in QS
 */
/* Allow extra time for RF processing if using RF Task */
#define GL1_HW_CM_GFW_CMD_ACTION_MARGIN_QS  (600)



//margin_qs_bin_index_t margin_qs_bin;
uint8 num_of_pwr_measures;

uint8 gl1_gfw_cmd_action_margin_qs_bin[X2G_TRANSITION_PREPARATORY_PHASES][NO_OF_BINS_PER_PHASE] = {{0}};

/* x2gStartAlpha value has a default value of 200 in GFW
 * Other RATS don't change this as its not time critical for them
 * but for T2G it is time Critical - so use 130
 */

#define GL1_HW_GFW_X2G_START_ALPHA_T2G_QS 130
#define GL1_HW_GFW_X2G_START_ALPHA_DEFAULT_QS 200
#define GL1_HW_GFW_CM_CLEANUP_ALPHA_T2G_QS 163
#define GL1_HW_GFW_CM_CLEANUP_ALPHA_DEFAULT_QS 0
#define GL1_HW_GFW_CM_T2G_RXFE_CONFIG_ALPHA  30
#define GL1_HW_GFW_CM_X2G_RXFE_CONFIG_ALPHA  0

gl1_hw_x2g_startup_info_t           gl1_hw_x2g_startup_info;
volatile gl1_hw_x2g_cleanup_info_t  gl1_hw_x2g_cleanup_info;

gl1_hw_x2g_pwr_meas_info_t  gl1_hw_x2g_pwr_meas_info;


/* @@OG  */
#define  ACQ_MEAS_STATE_INACTIVE  (0U)
#define  ACQ_MEAS_STATE_ACTIVE    (1U)

cm_acq_state_machine_type  cm_acq_state_machine[ACQ_MEAS_NUM_SM];

#ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
uint8  cm_acq_state_machine_active_array[ACQ_MEAS_NUM_SM];
uint8  cm_acq_state_machine_num_active = 0;
#endif
uint8                      cm_acq_sm = 0;


#define  SCH_MEAS_STATE_INACTIVE  (0U)
#define  SCH_MEAS_STATE_ACTIVE    (1U)

#define ACQ_HANDLER_TIMEOUT_FRAMES 15


#define  SCH_MEAS_NUM_SM           3

typedef struct
{
  uint8                       sch_meas_state;

/* Used to ensure we pick up the correct metrics buffer when the decoded
 * data is ready.
 */
  uint16                      seq_num;
  uint8                       sch_metrics_buffer_idx;
  uint8                       sch_data_buffer_idx;
  uint16                      sch_metrics_seq_num;

  gl1_defs_metrics_rpt        sch_metrics;

  boolean                     async_sch_decode_result;
  boolean                     gl1_hw_x2g_sch_rsp_sent;      /* @@TODO: update the rest of the code to use the sch state machine */

  gsm_irat_sch_burst_rsp_t    gl1_hw_x2g_sch_store;

  gl1_hw_x2g_acq_info_t       gl1_hw_x2g_sch_info;

  uint8                       uinstall_sch_handler_dec_frame;

  gl1_msg_decode_rpt          sch_decode_results;

  uint16                      timing_offset;

} sch_state_machine_type;

/* @@OG  */
sch_state_machine_type  sch_state_machine[SCH_MEAS_NUM_SM];

uint8                   cm_sch_sm = 0;


gl1_hw_rx_tags_type            stored_tags;


#define  POWER_MEAS_STATE_INACTIVE    (0U)
#define  POWER_MEAS_STATE_COMMAND     (1U)
#define  POWER_MEAS_STATE_ACTIVE      (2U)
#define  POWER_MEAS_STATE_FIRST_READ  (3U)
#define  POWER_MEAS_STATE_SECOND_READ (4U)




cm_pwr_meas_state_machine_type  cm_pwr_meas_state_machine[POWER_MEAS_NUM_SM];

uint8                   cm_pwr_sm = 0;



gl1_hw_x2g_api_type gl1_t2g_last_state = NONE_GL1;
gas_id_t  gl1_hw_t2g_gas_id =  GERAN_ACCESS_STRATUM_ID_1;


uint32 startup_qs;
uint32 pwr_sch_tone_meas_qs;
uint32 cleanup_qs;


int32  gl1_hw_x2g_offset_adjust( uint32               g_frame_number,
                 uint16               offset,
                 gl1_hw_x2g_api_type  x2g_api_type,
                 uint8                num,
                 gas_id_t             gas_id
                 );

boolean  send_err_msg = FALSE;

t2g_rf_api_profiling_type t2g_rf_api_profiling_struct[MAX_PROFILING_LOG][RF_API_PROFILING_NUM];

uint16                         t2g_rf_api_profiling_index = 0;
uint16                  t2g_api_profiling_index = 0;
t2g_api_profiling_type  t2g_api_profiling_struct[MAX_PROFILING_LOG];



void gl1_profiling_t2g_rf_api(uint16 index , t2g_rf_api_type rf_type , boolean start,gas_id_t gas_id)
{
  gsmtr_fn_qs_type current_fn_qs;

  current_fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);
  t2g_rf_api_profiling_struct[index][rf_type].rf_type = rf_type;

  if(start)
  {
      t2g_rf_api_profiling_struct[index][rf_type].start_time.qs = current_fn_qs.qs;
      t2g_rf_api_profiling_struct[index][rf_type].start_time.fn = current_fn_qs.fn;
  }
  else
  {
      t2g_rf_api_profiling_struct[index][rf_type].end_time.qs = current_fn_qs.qs;
      t2g_rf_api_profiling_struct[index][rf_type].end_time.fn = current_fn_qs.fn;
      t2g_rf_api_profiling_struct[index][rf_type].runtime = (t2g_rf_api_profiling_struct[index][rf_type].end_time.fn -
          t2g_rf_api_profiling_struct[index][rf_type].start_time.fn ) * 5000 + t2g_rf_api_profiling_struct[index][rf_type].end_time.qs
          - t2g_rf_api_profiling_struct[index][rf_type].start_time.qs;
  }
}

static inline void gl1_profiling_t2g_api(uint16 index, gl1_hw_x2g_api_type api_type, boolean start,gas_id_t gas_id)
{
  gsmtr_fn_qs_type current_fn_qs;

  current_fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);

  switch (api_type)
  {
  case STARTUP:
    t2g_api_profiling_struct[index].startup_api_time.api_type = api_type;
    if(start)
    {
      t2g_api_profiling_struct[index].startup_api_time.start_time.qs = current_fn_qs.qs;
      t2g_api_profiling_struct[index].startup_api_time.start_time.fn = current_fn_qs.fn;
    }
    else
    {
      t2g_api_profiling_struct[index].startup_api_time.end_time.qs = current_fn_qs.qs;
      t2g_api_profiling_struct[index].startup_api_time.end_time.fn = current_fn_qs.fn;
      startup_qs = current_fn_qs.qs;
    }
    break;
  case CLEANUP:
    t2g_api_profiling_struct[index].cleanup_api_time.api_type = api_type;
    if(start)
    {
      t2g_api_profiling_struct[index].cleanup_api_time.start_time.qs = current_fn_qs.qs;
      t2g_api_profiling_struct[index].cleanup_api_time.start_time.fn = current_fn_qs.fn;

    }
    else
    {
      t2g_api_profiling_struct[index].cleanup_api_time.end_time.qs = current_fn_qs.qs;
      t2g_api_profiling_struct[index].cleanup_api_time.end_time.fn = current_fn_qs.fn;
      cleanup_qs = current_fn_qs.qs;
    }
    break;
  default:
    t2g_api_profiling_struct[index].cmd_api_time.api_type = api_type;
    if(start)
    {
      t2g_api_profiling_struct[index].cmd_api_time.start_time.qs = current_fn_qs.qs;
      t2g_api_profiling_struct[index].cmd_api_time.start_time.fn = current_fn_qs.fn;

    }
    else
    {
      t2g_api_profiling_struct[index].cmd_api_time.end_time.qs = current_fn_qs.qs;
      t2g_api_profiling_struct[index].cmd_api_time.end_time.fn = current_fn_qs.fn;
      pwr_sch_tone_meas_qs = current_fn_qs.qs;

    }
    break;
  }
}
static inline void gl1_profiling_calulate_t2g_api_runtime(uint16 index)
{
  t2g_api_profiling_struct[index].startup_api_time.runtime =
    (t2g_api_profiling_struct[index].startup_api_time.end_time.fn - t2g_api_profiling_struct[index].startup_api_time.start_time.fn)*5000 +
    t2g_api_profiling_struct[index].startup_api_time.end_time.qs - t2g_api_profiling_struct[index].startup_api_time.start_time.qs;
  t2g_api_profiling_struct[index].cmd_api_time.runtime =
    (t2g_api_profiling_struct[index].cmd_api_time.end_time.fn - t2g_api_profiling_struct[index].cmd_api_time.start_time.fn)*5000 +
    t2g_api_profiling_struct[index].cmd_api_time.end_time.qs - t2g_api_profiling_struct[index].cmd_api_time.start_time.qs;
  t2g_api_profiling_struct[index].cleanup_api_time.runtime =
    (t2g_api_profiling_struct[index].cleanup_api_time.end_time.fn - t2g_api_profiling_struct[index].cleanup_api_time.start_time.fn)*5000 +
    t2g_api_profiling_struct[index].cleanup_api_time.end_time.qs - t2g_api_profiling_struct[index].cleanup_api_time.start_time.qs;
  t2g_api_profiling_struct[index].api_runtime =  (t2g_api_profiling_struct[index].cleanup_api_time.end_time.fn - t2g_api_profiling_struct[index].startup_api_time.start_time.fn)*5000 +
    t2g_api_profiling_struct[index].cleanup_api_time.end_time.qs - t2g_api_profiling_struct[index].startup_api_time.start_time.qs;
}

/* Storage structures for queuing x2g command parameters */
typedef struct
{
  gl1_hw_x2g_api_type  cmd_id;
  uint32               gl1_hw_x2g_api_issue_fn;
  uint32               g_frame_number;

  uint16               offset;

  gl1_hw_cm_rf_info_t  cm_rf_info;
  boolean              x2g_rf_rxlm_update;

} gl1_hw_x2g_startup_cmd_params_t;


typedef struct
{
  gl1_hw_x2g_api_type  cmd_id;
  uint32               gl1_hw_x2g_api_issue_fn;
  uint32               g_frame_number;

  uint32               offset;

} gl1_hw_x2g_cleanup_cmd_params_t;


typedef struct
{
   gl1_hw_x2g_api_type   cmd_id;
   uint32                gl1_hw_x2g_api_issue_fn;
   uint32                g_frame_number;


   int16                 signal_strength;
   uint16                timing_offset;
   int16                 frequency_offset;
   ARFCN_T               ARFCN;
   gl1_hw_rx_tags_type   tags;
   boolean               reconfirm;
   void*                 gsm_cell_ptr;

} gl1_hw_x2g_async_sch_cmd_params_t;


typedef struct
{
   gl1_hw_x2g_api_type   cmd_id;
   uint32                gl1_hw_x2g_api_issue_fn;
   uint32                g_frame_number;


   uint16                timing_offset;
   int16                 frequency_offset;

   ARFCN_T               ARFCN;
   dBx16_T               rx_signal_strength;

   gl1_hw_rx_tags_type   tags;
   boolean               sch_decode;

   void*                 gsm_cell_ptr;

} gl1_hw_x2g_start_acq_cmd_params_t;


typedef struct
{
  gl1_hw_x2g_api_type  cmd_id;
  uint32               gl1_hw_x2g_api_issue_fn;
  uint32               g_frame_number;


  uint16               offset;

} gl1_hw_x2g_stop_acq_cmd_params_t;


typedef struct
{
   gl1_hw_x2g_api_type         cmd_id;
   uint32                      gl1_hw_x2g_api_issue_fn;
   ARFCN_T                     ARFCN[MAX_CM_RSSI_MEASUREMENTS_REQ];
   dBx16_T                     rx_signal_strength[MAX_CM_RSSI_MEASUREMENTS_REQ];
   uint8                       num;
   uint16                      offset[MAX_CM_RSSI_MEASUREMENTS_REQ];
   gl1_hw_burst_position_type  position[MAX_CM_RSSI_MEASUREMENTS_REQ];
   gl1_hw_rx_tags_type         tags[MAX_CM_RSSI_MEASUREMENTS_REQ];

   uint32                      g_frame_number[MAX_CM_RSSI_MEASUREMENTS_REQ];
   uint16                      offset2;
   uint32                      g_frame_number2;
} gl1_hw_x2g_pwr_meas_cmd_params_t;


typedef union
{
  gl1_hw_x2g_startup_cmd_params_t    startup_cmd;
  gl1_hw_x2g_cleanup_cmd_params_t    cleanup_cmd;
  gl1_hw_x2g_async_sch_cmd_params_t  async_sch_cmd;
  gl1_hw_x2g_start_acq_cmd_params_t  start_acq_cmd;
  gl1_hw_x2g_stop_acq_cmd_params_t   stop_acq_cmd;
  gl1_hw_x2g_pwr_meas_cmd_params_t   pwr_meas_cmd;

} gl1_hw_x2g_cmd_data_u;

/* Queue typedef and associated macros */

#define  GL1_HW_X2G_CMD_QUEUE_SIZE  (10U)
#define  IS_QUEUE_EMPTY( queue_head_ptr )  ( (queue_head_ptr) == NULL ? TRUE: FALSE )

boolean  cm_startup_issued  = FALSE;
boolean  queued_x2g_startup = FALSE;

typedef struct gl1_x2g_command_queue_T
{
    /* Num frames till node is taken off the queue */
  /* not used */
    uint8                            fn_countdown;

  /* Information holding part */
    gl1_hw_x2g_cmd_data_u            cmd_data;

  /* Pointer to next element in queue */
    struct gl1_x2g_command_queue_T*  next_ptr;

} gl1_x2g_command_queue_t;


/* Head and tail of queue */
static gl1_x2g_command_queue_t*  head_ptr = NULL;
static gl1_x2g_command_queue_t*  tail_ptr = NULL;


gl1_x2g_command_info_t   X2g_Cmd_Info[GL1_HW_X2G_CMD_MAX];

static uint16 cm_current_id_num = 0;

uint16 gl1_hw_x2g_get_current_id_num (void)
{
    return cm_current_id_num++;
}

void gl1_hw_x2g_init_info(void)
{
  uint16 index = 0;

  for(index = 0; index<GL1_HW_X2G_CMD_MAX; index++)
  {
#ifdef FEATURE_GSM_TDS_DEBUG
    if(X2g_Cmd_Info[index].state != FREE)
    {
        MSG_GERAN_HIGH_2("GL1_HW:gl1_hw_x2g_init_info msgid %d,task %d",X2g_Cmd_Info[index].message_id,X2g_Cmd_Info[index].cmd_id);
    }
#endif
    X2g_Cmd_Info[index].cmd_id = NONE_GL1;
    X2g_Cmd_Info[index].state  = FREE;
  }
}


uint16 gl1_hw_x2g_get_index(gl1_hw_x2g_api_type task, gl1_hw_x2g_command_state state)
{
  uint16 i =0;

  if(FREE == state)
  {
    for(i=0; i<GL1_HW_X2G_CMD_MAX; i++)
    {
    if(X2g_Cmd_Info[i].state == state)
    {
      return i;
    }
    }
  }
  else
  {
  for(i=0; i<GL1_HW_X2G_CMD_MAX; i++)
  {
      if((X2g_Cmd_Info[i].state == state) && (X2g_Cmd_Info[i].cmd_id == task))
    {
        return i;
    }
  }
  }

  if(i == GL1_HW_X2G_CMD_MAX)
  { 
#ifdef FEATURE_QSH_MDUMP
          QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
          ERR_GERAN_FATAL_0("GL1_HW: X2g_Cmd_Info buffer overflow");
  }

  return i;
}

uint16 gl1_hw_x2g_set_info(uint16 num, ARFCN_T* uarfcn, gl1_hw_x2g_api_type task, gl1_hw_x2g_command_state status)
{
  uint16 i =0, j = 0;
  uint16 index = 0;
  boolean found = FALSE;
  gl1_hw_x2g_command_state local_status = FREE;

  if(status == USING)
    local_status = PENDING;
  else
    local_status = status;

  for(i=0; i < GL1_HW_X2G_CMD_MAX; i++)
  {
    if((X2g_Cmd_Info[i].cmd_id == task)&&(X2g_Cmd_Info[i].state == local_status)&&(num == X2g_Cmd_Info[i].num_uarfcns))
    {
      for(j = 0; j < X2g_Cmd_Info[i].num_uarfcns; j++)
      {
        if(X2g_Cmd_Info[i].UARFCN[j].num != uarfcn[j].num)
        break;
      }

      if(j == X2g_Cmd_Info[i].num_uarfcns)
      {
        found = TRUE;
        index = i;
        break;
      }
    }
  }

#ifdef FEATURE_GSM_TDS_DEBUG
    MSG_GERAN_HIGH_3("GL1_HW:gl1_hw_x2g_set_info task %d, status %d, found %d",task,status, found);
#endif

  if(!found)
  {
    if ( num > MAX_CM_RSSI_MEASUREMENTS_REQ )
    {
     num = MAX_CM_RSSI_MEASUREMENTS_REQ;

    }
    index = gl1_hw_x2g_get_index(task, FREE);
    X2g_Cmd_Info[index].message_id    = gl1_hw_x2g_get_current_id_num();
  X2g_Cmd_Info[index].cmd_id      = task;
  X2g_Cmd_Info[index].num_uarfcns   = num;
  X2g_Cmd_Info[index].state         = status;
  for(i = 0; i < num; i++)
  {
    X2g_Cmd_Info[index].UARFCN[i]   = uarfcn[i];
  }
  }
  else if(status == USING)
  {
  X2g_Cmd_Info[index].state = USING;
  }

#ifdef FEATURE_GSM_TDS_DEBUG
    MSG_GERAN_HIGH_1("GL1_HW:gl1_hw_x2g_set_info msgid %d",X2g_Cmd_Info[index].message_id);
#endif

  return X2g_Cmd_Info[index].message_id;
}

void gl1_hw_x2g_update_info(gl1_hw_x2g_api_type task, gl1_hw_x2g_command_state status)
{
  uint16    i=0;

  for(i = 0; i < GL1_HW_X2G_CMD_MAX; i++)
  {
      if((X2g_Cmd_Info[i].cmd_id == task)&&(X2g_Cmd_Info[i].state == USING))
      {
        X2g_Cmd_Info[i].state = status;
#ifdef FEATURE_GSM_TDS_DEBUG
        MSG_GERAN_HIGH_3("GL1_HW:gl1_hw_x2g_update_info msgid %d, status %d, task %d",X2g_Cmd_Info[i].message_id,status, task);
#endif
        return;
      }

    if((X2g_Cmd_Info[i].cmd_id == task)&&(X2g_Cmd_Info[i].state == ABORTING)&&(status == X2g_Cmd_Info[i].state))
    {
#ifdef FEATURE_GSM_TDS_DEBUG
        MSG_GERAN_HIGH_3("GL1_HW:gl1_hw_x2g_update_info has been aborted before msgid %d",X2g_Cmd_Info[i].message_id,0, 0);
#endif
        return;
    }
  }
#ifdef FEATURE_QSH_MDUMP
  QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
  ERR_GERAN_FATAL_2("GL1_HW: gl1_hw_x2g_update_info index and task is not aligment, status %d, task %d", status,task);

}

void gl1_hw_x2g_clear_info(uint16 msgid, gl1_hw_x2g_api_type task)
{
  uint16   i=0;

  for(i = 0; i < GL1_HW_X2G_CMD_MAX; i++)
  {
    if((msgid == X2g_Cmd_Info[i].message_id)&&(X2g_Cmd_Info[i].cmd_id == task))
    {
      X2g_Cmd_Info[i].cmd_id = NONE_GL1;
      X2g_Cmd_Info[i].state  = FREE;
#ifdef FEATURE_GSM_TDS_DEBUG
      MSG_GERAN_HIGH_2("GL1_HW:gl1_hw_x2g_clear_info msgid %d,task %d",X2g_Cmd_Info[i].message_id,task);
#endif
    return;
    }
  }
#ifdef FEATURE_QSH_MDUMP
  QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
  ERR_GERAN_FATAL_2("GL1_HW: gl1_hw_x2g_clear_info index and task is not aligment, index %d, task %d", msgid,task);

}



/* Used for adding nodes to the tail of the queue */
static void  gl1_hw_x2g_add_node( gl1_x2g_command_queue_t **node_pptr,
                                  gl1_x2g_command_queue_t **head_pptr,
                                  gl1_x2g_command_queue_t **tail_pptr )
{
    if( NULL == ( (*head_pptr)->next_ptr ) )
    {
        /* If there is one element in the queue
         * then extra care must be taken when inserting
         * the second node.
         */
        (*head_pptr)->next_ptr = *node_pptr;
    }

    (*node_pptr)->next_ptr = NULL;
    (*tail_pptr)->next_ptr = *node_pptr;
    (*tail_pptr)           = *node_pptr;

}

/* Used for removing nodes off the head of the queue */
static void  gl1_hw_x2g_remove_node( gl1_x2g_command_queue_t **node_pptr,
                                     gl1_x2g_command_queue_t **head_pptr,
                                     gl1_x2g_command_queue_t **tail_pptr )
{
    if( (*node_pptr) == (*head_pptr) )
    {
        /* The node is on the head of the queue. */

        /* Point to the next element in the queue */
        (*node_pptr) = (*node_pptr)->next_ptr;

        /* Re position the head ptr */
        (*head_pptr) = (*node_pptr);

        if( NULL == (*node_pptr) )
        {
            /* If this is the only node in the queue
             * then re position the tail ptr.
             */
            (*tail_pptr) = NULL;
        }
    }
    else
    {
        /* The node is on the tail of the queue or
         * in between.
         */
        gl1_x2g_command_queue_t* node_before_ptr =
          (*head_pptr);

        while( node_before_ptr->next_ptr != (*node_pptr) )
        {
            /* Search the queue for the node which is before
             * the one we wish to remove.
             * This search starts at the head of the queue and
             * ends at the tail.
             */
            node_before_ptr = node_before_ptr->next_ptr;
        }

        /* Remove the node by connecting the one before it
         * to the next element in the queue.
         */
        node_before_ptr->next_ptr = (*node_pptr)->next_ptr;

        /* Unlink the node */
        (*node_pptr)->next_ptr = NULL;

        if( (*node_pptr) == (*tail_pptr) )
        {
            /* If the removed node was on the tail
             * then re position the tail ptr.
             */
            (*tail_pptr) = node_before_ptr;

            /* Point to the next element in the queue */
            (*node_pptr) = NULL;
        }
        else
        {
            /* Move the node ptr to the following
             * node in the queue.
             */
            (*node_pptr) = node_before_ptr->next_ptr;
        }
    }
}

void  gl1_hw_x2g_create_cmd_node( gl1_hw_x2g_cmd_data_u*  x2g_cmd_params_ptr )
{
    /* static Array to avoid dynamically allocating memory */
    static gl1_x2g_command_queue_t  gl1_x2g_command_queue[ GL1_HW_X2G_CMD_QUEUE_SIZE ];
    static uint8                    element = 0;

    /* Used to choose the correct pipelined array element */
    gl1_x2g_command_queue_t*  node_ptr = &( gl1_x2g_command_queue[ element ] );
    gl1_hw_x2g_cmd_data_u*    data_ptr = &( node_ptr->cmd_data );

    if( ++element >= GL1_HW_X2G_CMD_QUEUE_SIZE )
    {
        element -= GL1_HW_X2G_CMD_QUEUE_SIZE;
    }

    switch( x2g_cmd_params_ptr->startup_cmd.cmd_id )
  {
  case  STARTUP:
      data_ptr->startup_cmd = x2g_cmd_params_ptr->startup_cmd;

    break;

  case  PWR_MEAS:
      data_ptr->pwr_meas_cmd = x2g_cmd_params_ptr->pwr_meas_cmd;

    break;

  case  TONE_DETECT:
      data_ptr->start_acq_cmd = x2g_cmd_params_ptr->start_acq_cmd;

    break;

  case  STOP_TONE_DETECT:
      data_ptr->stop_acq_cmd = x2g_cmd_params_ptr->stop_acq_cmd;

    break;

  case  SCH_DETECT:
      data_ptr->async_sch_cmd = x2g_cmd_params_ptr->async_sch_cmd;

    break;

  case  CLEANUP:
      data_ptr->cleanup_cmd = x2g_cmd_params_ptr->cleanup_cmd;

    break;

  default:
      MSG_GERAN_ERROR_1(" Unknown command %d ",  x2g_cmd_params_ptr->startup_cmd.cmd_id);

    return;
  }

    /* Attach node to the tail of the queue */
    if( FALSE != IS_QUEUE_EMPTY( head_ptr ) )
    {
        /* Attach a node to the empty queue,
         * special handling for head and tail
         * pointers required.
         */
        head_ptr = node_ptr;
        tail_ptr = node_ptr;
        node_ptr->next_ptr = NULL;
    }
    else
    {
        gl1_hw_x2g_add_node( &node_ptr, &head_ptr, &tail_ptr);
    }
}


void  gl1_hw_x2g_search_cmd_queue( gas_id_t gas_id )
{
    /* start at head of queue */
    gl1_x2g_command_queue_t*  node_ptr = head_ptr;

    /* Avoid getting the FN multiple times */
  uint32  current_FN = GSTMR_GET_FN_GERAN(gas_id );

    /* Search the queue for any nodes which are
     * due now to be converted to commands.
     */
    while( node_ptr != NULL )
    {
        uint32  current_fn_plus_one = ADD_FN( current_FN, 1 );

  if( ( current_fn_plus_one == node_ptr->cmd_data.startup_cmd.gl1_hw_x2g_api_issue_fn )
      &&( node_ptr->cmd_data.startup_cmd.cmd_id == STARTUP )
      )
    {
      MSG_GERAN_HIGH_0_G("Run queued X2G Startup ");
      gl1_hw_x2g_startup_geran( node_ptr->cmd_data.startup_cmd.offset,
                          &node_ptr->cmd_data.startup_cmd.cm_rf_info,
                          node_ptr->cmd_data.startup_cmd.g_frame_number,
                          node_ptr->cmd_data.startup_cmd.x2g_rf_rxlm_update,
                          geran_map_gas_id_to_nas_id(gas_id)
                        );


          gl1_hw_x2g_remove_node( &node_ptr,
                                    &head_ptr,
                                    &tail_ptr );


            while( node_ptr != NULL )
            {
              boolean cleanup = FALSE;

              switch( node_ptr->cmd_data.startup_cmd.cmd_id )
              {

                case  PWR_MEAS:
                {
                  gl1_hw_x2g_power_measure_geran( &node_ptr->cmd_data.pwr_meas_cmd.ARFCN[0],
                                            &node_ptr->cmd_data.pwr_meas_cmd.rx_signal_strength[0],
                                            node_ptr->cmd_data.pwr_meas_cmd.num,
                                            &node_ptr->cmd_data.pwr_meas_cmd.offset[0],
                                            &node_ptr->cmd_data.pwr_meas_cmd.position[0],
                                            &node_ptr->cmd_data.pwr_meas_cmd.tags[0],
                                            &node_ptr->cmd_data.pwr_meas_cmd.g_frame_number[0],
                                        node_ptr->cmd_data.pwr_meas_cmd.offset2,
                                      node_ptr->cmd_data.pwr_meas_cmd.g_frame_number2,
                                            geran_map_gas_id_to_nas_id(gas_id)
                                           );
                  gl1_hw_x2g_remove_node( &node_ptr,
                                          &head_ptr,
                                          &tail_ptr );
                  break;
                }

              case  TONE_DETECT:
              {

                (void) gl1_hw_x2g_start_acquisition_geran( node_ptr->cmd_data.start_acq_cmd.ARFCN,
                                                      node_ptr->cmd_data.start_acq_cmd.rx_signal_strength,
                                                      node_ptr->cmd_data.start_acq_cmd.timing_offset,
                                                      node_ptr->cmd_data.start_acq_cmd.frequency_offset,
                                                      node_ptr->cmd_data.start_acq_cmd.sch_decode,
                                                      &node_ptr->cmd_data.start_acq_cmd.tags,
                                                      node_ptr->cmd_data.start_acq_cmd.g_frame_number,
                                                      node_ptr->cmd_data.start_acq_cmd.gsm_cell_ptr,
                                                      geran_map_gas_id_to_nas_id(gas_id)
                                                    );

                  gl1_hw_x2g_remove_node( &node_ptr,
                                          &head_ptr,
                                          &tail_ptr
                                        );
                  break;
             }

             case  STOP_TONE_DETECT:
             {
               gl1_hw_x2g_stop_acquisition(node_ptr->cmd_data.stop_acq_cmd.offset,
                                          node_ptr->cmd_data.stop_acq_cmd.g_frame_number
                                          );

               gl1_hw_x2g_remove_node( &node_ptr,
                                       &head_ptr,
                                       &tail_ptr );
               break;
             }

            case  SCH_DETECT:
            {
              gl1_hw_x2g_sch_burst_geran( node_ptr->cmd_data.async_sch_cmd.ARFCN,
                                    node_ptr->cmd_data.async_sch_cmd.signal_strength,
                                    node_ptr->cmd_data.async_sch_cmd.timing_offset,
                                    node_ptr->cmd_data.async_sch_cmd.frequency_offset,
                                    &node_ptr->cmd_data.async_sch_cmd.tags,
                                    node_ptr->cmd_data.async_sch_cmd.g_frame_number,
                                    node_ptr->cmd_data.async_sch_cmd.reconfirm,
                                    node_ptr->cmd_data.async_sch_cmd.gsm_cell_ptr,
                                    geran_map_gas_id_to_nas_id(gas_id)
                                   );

              gl1_hw_x2g_remove_node( &node_ptr,
                                      &head_ptr,
                                     &tail_ptr );
              break;
            }
            case  CLEANUP:
            {
              cleanup = TRUE;

              (void) gl1_hw_x2g_cleanup_geran( node_ptr->cmd_data.cleanup_cmd.offset,
                                         node_ptr->cmd_data.cleanup_cmd.g_frame_number,
                                         geran_map_gas_id_to_nas_id(gas_id)
                                        );

               gl1_hw_x2g_remove_node( &node_ptr,
                                      &head_ptr,
                                      &tail_ptr
                                      );
               break;
             }

           default:
           {
            MSG_GERAN_ERROR_1_G(" Unknown command %d ", node_ptr->cmd_data.startup_cmd.cmd_id);
            break;
           }
        }


      if( cleanup )
      {
            break;
      }
    }  /* Second while loop */

        }
        else if( current_fn_plus_one > node_ptr->cmd_data.startup_cmd.gl1_hw_x2g_api_issue_fn  )
    {

          gl1_hw_x2g_remove_node( &node_ptr,
                                  &head_ptr,
                                  &tail_ptr );
    }
        else
        {
            /* Look in the next node */
            node_ptr = node_ptr->next_ptr;
        }
    } /* End of while node_ptr != NULL */
}

static void  gl1_hw_x2g_empty_cmd_queue( void )
{
    /* Dont check whether or not the pointers are set
     * It is quicker to just reset them.
     */
    head_ptr = NULL;
    tail_ptr = NULL;
}

/* The following function runs every frame does the following:
 *
 * 1- Calls the x2g command search routine.
 *
 * 2- Calls the gl1 hw schedule table maintenance routine.
 */
static void  gl1_hw_x2g_ftick_hdlr( sys_modem_as_id_e_type as_id
 )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  /* run the gl1 hw sched maintenance */
  gl1_hw_schedule_frame_tick( GSTMR_GET_FN_GERAN(gas_id ),gas_id );

  /* Search the x2g command queue */
  gl1_hw_x2g_search_cmd_queue(gas_id);
}

static boolean  rf_rxlm_update = TRUE;

#endif  /* FEATURE_GSM_TDS */

rfm_device_enum_type gl1_hw_cm_get_rf_device(gas_id_t gas_id)
{
  return gl1_hw_x2g_data[gas_id].gl1_hw_cm_trm_rf_device;
}

rfcom_mode_enum_type gl1_hw_cm_get_cm_rat_type(gas_id_t gas_id)
{
  return gl1_hw_x2g_data[gas_id].gl1_hw_cm_cxm_source_rat;
}

/*===========================================================================

FUNCTION  gl1_hw_cm_power_measure

DESCRIPTION
  This function initiates one to nine power measurements in the next frame.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_cm_power_measure
(
   ARFCN_T                     ARFCN[MAX_CM_RSSI_MEASUREMENTS],               /* Absolute Radio Frequency Channels  */
   dBx16_T                     rx_signal_strength[MAX_CM_RSSI_MEASUREMENTS],  /* Signal Strengths at Antenna        */
   uint8                       num,                                           /* Number of measurements to make     */
   uint16                      offset[MAX_CM_RSSI_MEASUREMENTS],              /* When to make a measurement in qs   */
   gl1_hw_burst_position_type  position[MAX_CM_RSSI_MEASUREMENTS],            /* position of burst within a gap     */
   gl1_hw_rx_tags_type         tags[MAX_CM_RSSI_MEASUREMENTS]
#ifdef FEATURE_DUAL_SIM
   ,sys_modem_as_id_e_type      as_id
#endif /*FEATURE_DUAL_SIM*/
)
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/
   gl1_hw_cm_power_measure_geran(ARFCN, rx_signal_strength, num, offset, position, tags, as_id);
   return;
}
/*===========================================================================

FUNCTION  gl1_hw_cm_power_measure_geran

DESCRIPTION
  This function initiates one to nine power measurements in the next frame. Used
  internally by geran


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_cm_power_measure_geran
(
   ARFCN_T                     ARFCN[MAX_CM_RSSI_MEASUREMENTS],               /* Absolute Radio Frequency Channels  */
   dBx16_T                     rx_signal_strength[MAX_CM_RSSI_MEASUREMENTS],  /* Signal Strengths at Antenna        */
   uint8                       num,                                           /* Number of measurements to make     */
   uint16                      offset[MAX_CM_RSSI_MEASUREMENTS],              /* When to make a measurement in qs   */
   gl1_hw_burst_position_type  position[MAX_CM_RSSI_MEASUREMENTS],            /* position of burst within a gap     */
   gl1_hw_rx_tags_type         tags[MAX_CM_RSSI_MEASUREMENTS],
   sys_modem_as_id_e_type      as_id
)
{
   uint8 i;
   uint32 rf_seq_num[MAX_CM_RSSI_MEASUREMENTS];
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

#ifdef  FEATURE_GSM_COEX
   int32  fw_offset_to_meas_start;
   int32  pwr_meas_req_time;
#endif /* FEATURE_GSM_COEX */

#ifdef GERAN_L1_HLLL_LNASTATE
   boolean sawless_support[MAX_CM_RSSI_MEASUREMENTS] = {0};
#endif

   rfgsm_rx_chan_params_type            rx_chan_params[MAX_CM_RSSI_MEASUREMENTS];
   rfgsm_rx_diversity_chan_params_type  div_rx_chan_params[MAX_CM_RSSI_MEASUREMENTS];

#ifdef  FEATURE_GSM_COEX
   static mdsp_gprs_monitor_coex_params  coex_params_x2g[MAX_CM_RSSI_MEASUREMENTS] = { {0 , 0} };
#endif  /* FEATURE_GSM_COEX  */
   memset (rx_chan_params,0,( sizeof (rfgsm_rx_chan_params_type)*MAX_CM_RSSI_MEASUREMENTS));
#ifdef  FEATURE_GSM_TDS
   if( !cm_startup_issued )
   {
     MSG_GERAN_ERROR_0_G("gl1 hw cm power meas cmd ignored ");

   send_err_msg = TRUE;
     return;
   }
#endif

   /* Sanity check the number of bursts */
   if (num == 0 || num > MAX_CM_RSSI_MEASUREMENTS)
   {
      MSG_GERAN_ERROR_1_G("Invalid num of pwr measures %d",num);
      if (num == 0) return;
      num = MAX_CM_RSSI_MEASUREMENTS;
   }

#ifdef  FEATURE_GSM_COEX
   /* Initialise the list of monitor ARFCNs to be arbitrated */
   for(i=0;i<num;i++)
   {
      gl1_hw_arbitration[gas_id].monitors.monitor[i].arfcn = ARFCN[i];
   }

   fw_offset_to_meas_start = gl1_hw_get_rf_gsm_mon_setup_time(gas_id);

   pwr_meas_req_time = fw_offset_to_meas_start + PWR_MEAS_LENGTH_QS;

   gl1_hw_arbitration[gas_id].monitors.count = num;

   gl1_hw_arbitration[gas_id].notify_event( ARBITRATE_MONITORS, gas_id );
#endif /* FEATURE_GSM_COEX */

   GL1_HANDLER_PERFORMANCE_LOGGING(START_PWR_MEAS_RF,gas_id);

   for (i = 0; i < num; i++)
   {
     rx_chan_params[i].burst_type = RF_MON_BURST;
     rx_chan_params[i].band       = gl1_hw_arfcn_to_rfgsm_band(ARFCN[i]);
     rx_chan_params[i].arfcn      = (uint16)ARFCN[i].num;
     rx_chan_params[i].rx_lvl_dbm = rx_signal_strength[i];
     rx_chan_params[i].lna_range  = NULL;
     rx_chan_params[i].gain_ptr   = &tags[i].gain;
     rx_chan_params[i].freq_err   = gl1_hw_rf_get_freq_error(gas_id);
     cm_rssi_arfcn[gas_id][i]             = ARFCN[i];
     div_rx_chan_params[i].probe_burst = NULL;
#ifdef FEATURE_GSM_RX_DIVERSITY
     div_rx_chan_params[i].rx_lvl_dbm = 0;
     div_rx_chan_params[i].gain_ptr = &tags[i].gain;
#else
     div_rx_chan_params[i].gain_ptr = NULL;
#endif /*FEATURE_GSM_RX_DIVERSITY*/
#ifdef FEATURE_GSM_RF_DEBUG_NV
   if ( mdsp_rf_debug_NV_enable_g [gas_id] )
   {

      MSG_GERAN_HIGH_3_G("   rx_signal_strength[i]  = %d rx_chan_params[i].band  = %d  arfcns[i].num =%d", rx_signal_strength[i], rx_chan_params[i].band, ARFCN[i].num);

      MSG_GERAN_HIGH_3_G(" gl1_hw_cm_power_measure_geran  mdsp_rf_debug_NV_enable_g[0] = %d mdsp_rf_debug_upper_power_limit_g = %d mdsp_rf_debug_lower_power_limit_g = %d",
                                 mdsp_rf_debug_NV_enable_g[gas_id],mdsp_rf_debug_upper_power_limit_g[gas_id] ,mdsp_rf_debug_lower_power_limit_g[gas_id]);
      MSG_GERAN_HIGH_3_G(" gl1_hw_cm_power_measure_geran  mdsp_rf_debug_band_g[0] = %d mdsp_rf_debug_arfcn_g = %d  i=%d",mdsp_rf_debug_band_g [gas_id],mdsp_rf_debug_arfcn_g[gas_id],i);

      if ( (mdsp_rf_debug_NV_enable_g [gas_id])&&
            ((rx_signal_strength[i]/16)<= mdsp_rf_debug_upper_power_limit_g [gas_id])&&
            ((rx_signal_strength[i]/16)>= mdsp_rf_debug_lower_power_limit_g [gas_id])&&
            (rx_chan_params[i].band == mdsp_rf_debug_band_g [gas_id])&&
            (ARFCN[i].num == mdsp_rf_debug_arfcn_g[gas_id] )
            )
      {
         MSG_GERAN_HIGH_1_G(" flag_for_fw_pwr_meas_crash  = %d ",get_flag_for_fw_pwr_meas_crash(i ,gas_id ));
         //  SET the RF  bebug crash flag true
         set_flag_for_fw_pwr_meas_crash(i,TRUE,gas_id);
      }
      else
      {
         set_flag_for_fw_pwr_meas_crash(i,FALSE,gas_id);
      }
   }
#endif /* FEATURE_GSM_RF_DEBUG_NV */
#ifdef GERAN_L1_HLLL_LNASTATE
     rx_chan_params[i].high_lin_mode_active  = &(tags[i].sawless_lnastate);
     sawless_support[i] = gl1_hw_get_sawless_HLLL_support(ARFCN[i].band,gas_id);
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GERAN_L1_ENHANCED_RX
     rx_chan_params[i].enh_rx_params.enh_rx_enable_flag = FALSE;
     rx_chan_params[i].enh_rx_params.enh_rx_offset_freq = 0;
#endif /*GERAN_L1_ENHANCED_RX*/

#ifdef  FEATURE_GSM_COEX
     coex_params_x2g[i].coex_priority   = gl1_hw_arbitration[gas_id].monitors.monitor[i].priority;
     coex_params_x2g[i].coex_desense_id = gl1_hw_arbitration[gas_id].monitors.monitor[i].desense_id;

     /* Log power measurement burst in cxm_log_table */
     l1_log_cxm_new_entry( mdsp_get_current_seq_num(gas_id),
                           ARFCN[i].num,
                           CXM_LOG_MONITOR,
                           offset[i] + pwr_meas_req_time - 96,
                           CXM_LOG_UNKNOWN_VALUE, 0, 0, 0,
                           CXM_LOG_ACTIVITY_RX,
                           gas_id );

#endif /* FEATURE_GSM_COEX */

     rx_chan_params[i].rf_sequence_number = gl1_hw_get_next_rf_seq_num();
     rf_seq_num[i] = rx_chan_params[i].rf_sequence_number;
   }

   MSG_GERAN_MED_2_G("PwrMeas: Start RF scripts num:%d, qs:%d",num, gstmr_rd_qsymbol_count_geran(as_id));
   gl1_hw_rf_rx_burst((uint16)num,
                      rx_chan_params
                      ,div_rx_chan_params
                      ,FALSE
                      ,MON_MODE
                      ,gas_id
                     );

#ifdef  FEATURE_GSM_COEX
 mdsp_cm_power_measure(num, &(ARFCN[0]), offset, rf_seq_num, &coex_params_x2g[0],
#ifdef GERAN_L1_HLLL_LNASTATE
                       sawless_support,
#endif
                       gas_id);
#else
 mdsp_cm_power_measure(num, offset,rf_seq_num,
#ifdef GERAN_L1_HLLL_LNASTATE
                       sawless_support,
#endif
                       gas_id);
#endif /* FEATURE_GSM_COEX */

   gl1_hw_log_cell_cmds_to_diag (cmd_cm_power_measure, FALSE,gas_id);
}


void gl1_hw_l2g_power_measure
(
   ARFCN_T                     ARFCN[MAX_CM_RSSI_MEASUREMENTS],               /* Absolute Radio Frequency Channels  */
   dBx16_T                     rx_signal_strength[MAX_CM_RSSI_MEASUREMENTS],  /* Signal Strengths at Antenna        */
   uint8                       num,                                           /* Number of measurements to make     */
   uint16                      offset[MAX_CM_RSSI_MEASUREMENTS],              /* When to make a measurement in qs   */
   gl1_hw_burst_position_type  position[MAX_CM_RSSI_MEASUREMENTS],            /* position of burst within a gap     */
   gl1_hw_rx_tags_type         tags[MAX_CM_RSSI_MEASUREMENTS],
#ifdef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type      as_id,
#endif
   boolean                     high_priority
)
{
   uint8 i;
   uint32 rf_seq_num[MAX_CM_RSSI_MEASUREMENTS];
#ifdef FEATURE_DUAL_SIM
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#else
   gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif

#ifdef  FEATURE_GSM_COEX
   int32  fw_offset_to_meas_start;
   int32  pwr_meas_req_time;
#endif /* FEATURE_GSM_COEX */

   rfgsm_rx_chan_params_type            rx_chan_params[MAX_CM_RSSI_MEASUREMENTS];
   rfgsm_rx_diversity_chan_params_type  div_rx_chan_params[MAX_CM_RSSI_MEASUREMENTS];

#ifdef GERAN_L1_HLLL_LNASTATE
   boolean sawless_support[MAX_CM_RSSI_MEASUREMENTS] = {0};
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef  FEATURE_GSM_COEX
   static mdsp_gprs_monitor_coex_params  coex_params_x2g[MAX_CM_RSSI_MEASUREMENTS] = { {0 , 0} };
#endif  /* FEATURE_GSM_COEX  */


   /* Sanity check the number of bursts */
   if (num == 0 || num > MAX_CM_RSSI_MEASUREMENTS)
   {
      MSG_GERAN_ERROR_1_G("Invalid num of pwr measures %d",num);
      if (num == 0) return;
      num = MAX_CM_RSSI_MEASUREMENTS;
   }

#ifdef  FEATURE_GSM_COEX
   /* Initialise the list of monitor ARFCNs to be arbitrated */
   for(i=0;i<num;i++)
   {
      gl1_hw_arbitration[gas_id].monitors.monitor[i].arfcn = ARFCN[i];
   }

   fw_offset_to_meas_start = gl1_hw_get_rf_gsm_mon_setup_time(gas_id);

   pwr_meas_req_time = fw_offset_to_meas_start + PWR_MEAS_LENGTH_QS;

   gl1_hw_arbitration[gas_id].monitors.count = num;
   gl1_hw_arbitration[gas_id].monitors.l2g_high_priority = high_priority;
   gl1_hw_arbitration[gas_id].notify_event( ARBITRATE_MONITORS, gas_id );

#endif /* FEATURE_GSM_COEX */

   GL1_HANDLER_PERFORMANCE_LOGGING(START_PWR_MEAS_RF,gas_id);

   for (i = 0; i < num; i++)
   {
     rx_chan_params[i].burst_type = RF_MON_BURST;
     rx_chan_params[i].band       = gl1_hw_arfcn_to_rfgsm_band(ARFCN[i]);
     rx_chan_params[i].arfcn      = (uint16)ARFCN[i].num;
     rx_chan_params[i].rx_lvl_dbm = rx_signal_strength[i];
     rx_chan_params[i].lna_range  = NULL;
     rx_chan_params[i].gain_ptr   = &tags[i].gain;
     rx_chan_params[i].freq_err   = gl1_hw_rf_get_freq_error(gas_id);
     cm_rssi_arfcn[gas_id][i]             = ARFCN[i];
     div_rx_chan_params[i].probe_burst = NULL;

#ifdef FEATURE_GSM_RF_DEBUG_NV
     if ( mdsp_rf_debug_NV_enable_g [gas_id] )
     {

     MSG_GERAN_HIGH_3_G("   rx_signal_strength[i] = %d rx_chan_params[i].band  = %d  arfcns[i].num =%d",rx_signal_strength[i], rx_chan_params[i].band ,ARFCN[i].num);
     MSG_GERAN_HIGH_3_G(" gl1_hw_l2g_power_measure  mdsp_rf_debug_NV_enable_g[0] = %d mdsp_rf_debug_upper_power_limit_g = %d mdsp_rf_debug_lower_power_limit_g = %d",
                                 mdsp_rf_debug_NV_enable_g[gas_id],mdsp_rf_debug_upper_power_limit_g[gas_id] ,mdsp_rf_debug_lower_power_limit_g[gas_id]);
     MSG_GERAN_HIGH_3_G(" gl1_hw_l2g_power_measure  mdsp_rf_debug_band_g[0] = %d mdsp_rf_debug_arfcn_g = %d  i=%d",mdsp_rf_debug_band_g [gas_id],mdsp_rf_debug_arfcn_g[gas_id],i);

     if ( (mdsp_rf_debug_NV_enable_g [gas_id])&&
            ((rx_signal_strength[i]/16)<= mdsp_rf_debug_upper_power_limit_g [gas_id])&&
            ((rx_signal_strength[i]/16)>= mdsp_rf_debug_lower_power_limit_g [gas_id])&&
            (rx_chan_params[i].band == mdsp_rf_debug_band_g [gas_id])&&
            (ARFCN[i].num == mdsp_rf_debug_arfcn_g[gas_id] )
            )

     {
         MSG_GERAN_HIGH_1_G(" flag_for_fw_pwr_meas_crash  = %d ",get_flag_for_fw_pwr_meas_crash(i ,gas_id ));
         //  SET the RF  bebug crash flag true
         set_flag_for_fw_pwr_meas_crash(i,TRUE,gas_id);
     }
     else
     {
         set_flag_for_fw_pwr_meas_crash(i,FALSE,gas_id);
     }
     }
#endif /* FEATURE_GSM_RF_DEBUG_NV */

#ifdef FEATURE_GSM_RX_DIVERSITY
     div_rx_chan_params[i].rx_lvl_dbm = 0;
     div_rx_chan_params[i].gain_ptr = &tags[i].gain;
#else
     div_rx_chan_params[i].gain_ptr = NULL;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef GERAN_L1_HLLL_LNASTATE
     rx_chan_params[i].high_lin_mode_active  = &(tags[i].sawless_lnastate);
     sawless_support[i] = gl1_hw_get_sawless_HLLL_support(ARFCN[i].band,gas_id);
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GERAN_L1_ENHANCED_RX
     rx_chan_params[i].enh_rx_params.enh_rx_enable_flag = FALSE;
     rx_chan_params[i].enh_rx_params.enh_rx_offset_freq = 0;
#endif /*GERAN_L1_ENHANCED_RX*/

#ifdef  FEATURE_GSM_COEX
     coex_params_x2g[i].coex_priority   = gl1_hw_arbitration[gas_id].monitors.monitor[i].priority;
     coex_params_x2g[i].coex_desense_id = gl1_hw_arbitration[gas_id].monitors.monitor[i].desense_id;

     /* Log power measurement burst in cxm_log_table */
     l1_log_cxm_new_entry( mdsp_get_current_seq_num(gas_id),
                           ARFCN[i].num,
                           CXM_LOG_MONITOR,
                           offset[i] + pwr_meas_req_time - 96,
                           CXM_LOG_UNKNOWN_VALUE, 0, 0, 0,
                           CXM_LOG_ACTIVITY_RX,
                           gas_id );

#endif /* FEATURE_GSM_COEX */

     rx_chan_params[i].rf_sequence_number = gl1_hw_get_next_rf_seq_num();
     rf_seq_num[i] = rx_chan_params[i].rf_sequence_number;

   }

#ifdef  FEATURE_DUAL_SIM
   MSG_GERAN_MED_2_G("PwrMeas: Start RF scripts num:%d, qs:%d",num, gstmr_rd_qsymbol_count_geran(as_id));
#endif

   gl1_hw_rf_rx_burst((uint16)num,
                      rx_chan_params
                      ,div_rx_chan_params
                      ,FALSE
                      ,MON_MODE
                      ,gas_id
                     );

#ifdef  FEATURE_GSM_COEX
 mdsp_cm_power_measure(num, &(ARFCN[0]), offset, rf_seq_num, &coex_params_x2g[0],
#ifdef GERAN_L1_HLLL_LNASTATE
                       sawless_support,
#endif
                       gas_id);
#else
 mdsp_cm_power_measure(num, offset,rf_seq_num,
#ifdef GERAN_L1_HLLL_LNASTATE
                       sawless_support,
#endif
                       gas_id);
#endif /* FEATURE_GSM_COEX */

   gl1_hw_log_cell_cmds_to_diag (cmd_cm_power_measure, FALSE,gas_id);
}
/*===========================================================================

FUNCTION  gl1_hw_cm_store_acq_tag

DESCRIPTION
  Stores acq tag, so that it cna be used to calculate Rx level in dBm for parallel SCH.
===========================================================================*/
void gl1_hw_cm_store_acq_tag(gl1_hw_rx_tags_type *tags)
{
   gl1_hw_cm_acq_tag = *tags;
}
/*===========================================================================

FUNCTION  gl1_hw_cm_store_acq_tag

DESCRIPTION
  Return pointer to acq tag.
===========================================================================*/
gl1_hw_rx_tags_type *gl1_hw_cm_get_acq_tag (void)
{
   return(&gl1_hw_cm_acq_tag);
}

/*===========================================================================

FUNCTION  gl1_hw_cm_was_tone_detected

DESCRIPTION
  This function determines if a tone was detected and returns the tone
  offset returned by the DSP if the FCCH was successfully decoded
===========================================================================*/
boolean gl1_hw_cm_was_tone_detected
(
   ARFCN_T arfcn,  /* Used for logging */
   gl1_hw_cm_tone_results_t  *gl1_hw_cm_tone_results_ptr,
   sys_modem_as_id_e_type as_id
)
{
  boolean tone_found = FALSE;
  gl1_cell_acq_decode_status_t gl1_cell_detected_status;
  acq_decode_rpt  acq_sch_decode_data = { 0 };
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id );

  gl1_hw_cm_fcch_rpt.arfcn = arfcn;

  gl1_cell_detected_status =
    gl1_hw_was_gsm_cell_detected( arfcn, GL1_HW_INVALID_BSIC, &acq_sch_decode_data, &gl1_hw_cm_fcch_rpt, gas_id );

  /* Clear was_sch_decoded as it would be TRUE even if SCH crc failed - set it later if SCH decoded */
  gl1_hw_cm_fcch_rpt.was_sch_decoded = FALSE;

  if ((gl1_cell_detected_status == FCCH_FOUND)
     || (gl1_cell_detected_status == PARALLEL_SCH_DECODED)
     || (gl1_cell_detected_status == CONVENTIONAL_SCH_DECODED))
  {
     gl1_hw_cm_tone_results_ptr->tone_offset = gl1_hw_cm_fcch_rpt.time_offset;
     gl1_hw_cm_tone_results_ptr->coarse_freq = gl1_hw_cm_fcch_rpt.coarse_freq;
     gl1_hw_cm_tone_results_ptr->afc_freq = gl1_hw_cm_fcch_rpt.afc_freq;
     gl1_hw_cm_tone_results_ptr->snr = gl1_hw_cm_fcch_rpt.snr;

     /* Fine freq return value is the total frequency error (in Hz), so calculate here 
     (remove any initial rotator correction if desense channel) */
     gl1_hw_cm_fcch_rpt.fine_freq = 
        gl1_hw_tone_detected_total_freq_offset( arfcn, gl1_hw_cm_fcch_rpt.afc_freq, gas_id );
     gl1_hw_cm_tone_results_ptr->fine_freq = gl1_hw_cm_fcch_rpt.fine_freq;         
 
     /* If an FCCH has been detected */
     /* If conventional SCH then there will also have been FCCH */
     if ((gl1_cell_detected_status == FCCH_FOUND)
         || (gl1_cell_detected_status == CONVENTIONAL_SCH_DECODED))
     {
       /* Put an SNR thresh on the CM tone detect to ensure good detect */
       if (gl1_hw_cm_fcch_rpt.snr > GL1_HW_CM_TONE_MIN_SNR_THRESH)
       {
         tone_found = TRUE;
       }

       MSG_GERAN_HIGH_4_G("X2G Acq FCCH found arfcn %d, freq error %d Hz, offset %d, snr %d",
                        arfcn.num,
                        gl1_hw_cm_fcch_rpt.fine_freq,
                        gl1_hw_cm_fcch_rpt.time_offset,
                        gl1_hw_cm_fcch_rpt.snr);

     }

     /* If an SCH has been detected */
     if ((gl1_cell_detected_status == PARALLEL_SCH_DECODED)
        || (gl1_cell_detected_status == CONVENTIONAL_SCH_DECODED))
     {
       /* Copy relevant data from acq_sch_decode_data */
       gl1_hw_cm_fcch_rpt.id = acq_sch_decode_data.id;
       gl1_hw_cm_fcch_rpt.snr = acq_sch_decode_data.snr;
       gl1_hw_cm_fcch_rpt.time_offset = acq_sch_decode_data.time_offset;

       gl1_hw_cm_tone_results_ptr->tone_offset = gl1_hw_cm_fcch_rpt.time_offset - QS_PER_FRAME;

       COPY_SCH_DATA( &gl1_hw_cm_fcch_rpt.sch_data[0],
                        &acq_sch_decode_data.sch_data[0] );

       MSG_GERAN_HIGH_3_G("X2G Acq FCCH, SCH decoded arfcn %d, freq error %d Hz, detection status %d",
                        arfcn.num,
                        gl1_hw_cm_tone_results_ptr->fine_freq,
                        gl1_cell_detected_status);

       gl1_hw_cm_fcch_rpt.was_sch_decoded = TRUE;
     }

     gl1_hw_cm_tone_results_ptr->id = gl1_hw_cm_fcch_rpt.id;
  }
  gl1_hw_cm_tone_results_ptr->sch_decoded = gl1_hw_cm_fcch_rpt.was_sch_decoded;
  gl1_hw_cm_tone_results_ptr->schedule_error = gl1_hw_cm_fcch_rpt.schedule_error;

  return(tone_found);
}
/*===========================================================================

FUNCTION  gl1_hw_cm_was_sch_decoded

DESCRIPTION
  This function polls the MDSP buffers to verify whether there are
  SCH results available.  The availability of results indicates that
  the MDSP has detected and decoded an SCH.  The CRC is a strong indicator
  that the SCH burst is decoded correctly.  The SCH burst contains
  a 10-bit CRC.
===========================================================================*/
boolean gl1_hw_cm_was_sch_decoded
(
   ARFCN_T arfcn,              /* Used for logging */
   boolean sch_burst,          /* TRUE indicates get results for gl1_hw_rx_async_sch_burst, FALSE for acquisition results */
   gl1_hw_rx_tags_type *tags,  /* Contains gain settings used */
   gl1_hw_cm_sch_results_t  *gl1_hw_cm_sch_results_ptr,
   sys_modem_as_id_e_type as_id
)
{
   boolean  results_available;
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
   gl1_hw_cm_sch_results_ptr->schedule_error = FALSE;

   if (sch_burst == FALSE)
   {
      acq_decode_rpt *gl1_hw_cm_fcch_rpt_ptr = &gl1_hw_cm_fcch_rpt;

      if (gl1_hw_cm_fcch_rpt_ptr->was_sch_decoded == FALSE)
      {
        gl1_hw_sch_decode_data *gl1_hw_was_sch_decode_data_p;

        gl1_hw_sch_decode_data  gl1_hw_was_sch_decode_data = { 0 };

        gl1_hw_was_sch_decode_data_p = &gl1_hw_was_sch_decode_data;

        results_available = gl1_hw_was_sch_decoded_geran( arfcn,
                                                          GL1_HW_INVALID_BSIC,
                                                          gl1_hw_was_sch_decode_data_p,
                                                          as_id );

        /* Copy out local data to external struct data */
        gl1_hw_cm_sch_results_ptr->id = gl1_hw_was_sch_decode_data_p->id;
        gl1_hw_cm_sch_results_ptr->acq_time_offset = gl1_hw_was_sch_decode_data_p->time_offset;
        gl1_hw_cm_sch_results_ptr->crc_pass = gl1_hw_was_sch_decode_data_p->crc_pass;
        gl1_hw_cm_sch_results_ptr->schedule_error = gl1_hw_was_sch_decode_data_p->schedule_error;

        COPY_SCH_DATA( &gl1_hw_cm_sch_results_ptr->sch_data[0],
                       &gl1_hw_was_sch_decode_data_p->sch_data[0]);

      }
      else
      {
        /* Results already available  from parallel SCH at time of FCCH */
        results_available = TRUE;
        gl1_hw_cm_sch_results_ptr->id = gl1_hw_cm_fcch_rpt_ptr->id;
        gl1_hw_cm_sch_results_ptr->acq_time_offset = gl1_hw_cm_fcch_rpt_ptr->time_offset;
        gl1_hw_cm_sch_results_ptr->crc_pass = gl1_hw_cm_fcch_rpt_ptr->was_sch_decoded;
        gl1_hw_cm_sch_results_ptr->schedule_error = gl1_hw_cm_fcch_rpt_ptr->schedule_error;

        COPY_SCH_DATA( &gl1_hw_cm_sch_results_ptr->sch_data[0],
                       &gl1_hw_cm_fcch_rpt_ptr->sch_data[0] );

      }
      gl1_hw_cm_sch_results_ptr->freq_offset = gl1_hw_cm_fcch_rpt_ptr->fine_freq;


      if (results_available)
      {
        gl1_hw_cm_sch_results_ptr->snr = gl1_hw_cm_fcch_rpt_ptr->snr;

        gl1_msg_compute_rx_power(gl1_hw_cm_fcch_rpt_ptr->rssi,
                                 &gl1_hw_cm_fcch_rpt_ptr->pwr_dBm_x16,
                                 &tags->gain.range,
                                 &gl1_hw_cm_acq_tag.gain);

        gl1_hw_cm_sch_results_ptr->pwr_dBm_x16 = gl1_hw_cm_fcch_rpt_ptr->pwr_dBm_x16;

        MSG_GERAN_HIGH_5_G("X2G Acq SCH decode %d, arfcn %d, snr %d, rssi %d (dBm x 16), gain_range %d",
                           gl1_hw_cm_sch_results_ptr->crc_pass,
                           arfcn.num,
                           gl1_hw_cm_fcch_rpt_ptr->snr,
                           gl1_hw_cm_fcch_rpt_ptr->pwr_dBm_x16,
                           tags->gain.range);

      }
      gl1_hw_cm_sch_results_ptr->sch_timing_offset = 0;
      gl1_hw_cm_sch_results_ptr->metrics_valid = results_available;
   }
   else
   {
      gl1_defs_rx_hdr_struct  rpt_hdr;

      results_available = gl1_hw_get_async_rx_data_geran( GL1_HW_SCH,
                                                          &rpt_hdr,
                                                          &gl1_hw_cm_sch_results_ptr->sch_data[0],
                                                          (uint16)MAX_SCH_OCTETS,
                                                          TRUE,
                                                          as_id);

      gl1_hw_cm_sch_results_ptr->crc_pass = rpt_hdr.good_data;

      if (results_available)
      {
         gl1_defs_metrics_rpt metrics_rpt;
         uint8 i;
         uint8 tags_index = Gl1_HW_CM_NUM_SCH_METRICS;

         /* Get seq_num and metrics index */
         for (i=0; i<Gl1_HW_CM_NUM_SCH_METRICS; i++)
         {
            if ((arfcn.num == gl1_hw_cm_sch_tags[gas_id][i].arfcn.num) && (gl1_hw_cm_sch_tags[gas_id][i].metrics_index != GL1_HW_SCH_INVALID_METRICS_INDEX))
            {
               tags_index = i;
               break;
            }
         }

         gl1_hw_cm_sch_results_ptr->schedule_error = rpt_hdr.schedule_error;

         if (tags_index == Gl1_HW_CM_NUM_SCH_METRICS)
         {
            gl1_hw_cm_sch_results_ptr->metrics_valid = FALSE;
            MSG_GERAN_MED_0("X2G SCH metrics not valid");
         }
         else
         {
             gl1_hw_cm_sch_results_ptr->metrics_valid =
                   gl1_hw_x2g_get_async_sch_rx_metrics( &metrics_rpt,
                                                        TRUE,
                                                        tags,
                                                        gl1_hw_cm_sch_tags[gas_id][tags_index].metrics_index,
                                                        gl1_hw_cm_sch_tags[gas_id][tags_index].seq_num,
                                                        arfcn,
                                                        as_id);

             gl1_hw_cm_sch_tags[gas_id][tags_index].metrics_index = GL1_HW_SCH_INVALID_METRICS_INDEX;

             gl1_hw_cm_sch_results_ptr->snr = metrics_rpt.snr;
             gl1_hw_cm_sch_results_ptr->pwr_dBm_x16 = metrics_rpt.pwr_dBm_x16;

             MSG_GERAN_HIGH_5_G("X2G SCH decode %d, arfcn %d, snr %d, rssi %d (dBm x 16), valid %d",
                                gl1_hw_cm_sch_results_ptr->crc_pass,
                                arfcn.num,
                                metrics_rpt.snr,
                                metrics_rpt.pwr_dBm_x16,
                                gl1_hw_cm_sch_results_ptr->metrics_valid);

             gl1_hw_cm_sch_results_ptr->freq_offset = metrics_rpt.freq_offset;
             /* offset from GFW is being negated such that
                 GFW negative offset means that the received SCH is later than expected.*/
             gl1_hw_cm_sch_results_ptr->sch_timing_offset = - metrics_rpt.timing_offset;
             gl1_hw_cm_sch_results_ptr->acq_time_offset = - metrics_rpt.timing_offset;
         }
      }
      else
      {
         gl1_hw_cm_sch_results_ptr->metrics_valid = FALSE;
      }
   }

   return(results_available);
}

#ifdef FEATURE_GSM_TDS

/*===========================================================================

FUNCTION  gl1_hw_cm_x2g_power_measure

DESCRIPTION

===========================================================================*/
void   gl1_hw_cm_x2g_power_measure
(
   ARFCN_T                     ARFCN[MAX_CM_RSSI_MEASUREMENTS],               /* Absolute Radio Frequency Channels  */
   dBx16_T                     rx_signal_strength[MAX_CM_RSSI_MEASUREMENTS],  /* Signal Strengths at Antenna        */
   uint8                       num,                                           /* Number of measurements to make     */
   uint8                       results_buffer,                                /* Buffer index for GFW results       */
   uint16                      offset[MAX_CM_RSSI_MEASUREMENTS],              /* When to make a measurement in qs   */
   gl1_hw_burst_position_type  position[MAX_CM_RSSI_MEASUREMENTS],            /* position of burst within a gap     */
   gl1_hw_rx_tags_type         tags[MAX_CM_RSSI_MEASUREMENTS],
   sys_modem_as_id_e_type      as_id
)
{
   uint8  i;
   uint32 rf_seq_num[MAX_CM_RSSI_MEASUREMENTS];
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
   void *cmdbuf_ptr=NULL;

#ifdef  FEATURE_GSM_COEX
   int32  fw_offset_to_meas_start;
   int32  pwr_meas_req_time;
#endif  /* FEATURE_GSM_COEX */

   rfgsm_rx_chan_params_type            rx_chan_params[MAX_CM_RSSI_MEASUREMENTS];
   rfgsm_rx_diversity_chan_params_type  div_rx_chan_params[MAX_CM_RSSI_MEASUREMENTS];

#ifdef  FEATURE_GSM_COEX
    static mdsp_gprs_monitor_coex_params  coex_params_x2g[MAX_CM_RSSI_MEASUREMENTS] = { {0 , 0} };
#endif /* FEATURE_GSM_COEX */
   memset (rx_chan_params,0,( sizeof (rfgsm_rx_chan_params_type)*MAX_CM_RSSI_MEASUREMENTS));
#ifdef  FEATURE_GSM_TDS
   if( !cm_startup_issued )
   {
     MSG_GERAN_ERROR_0_G("gl1 hw cm power meas cmd ignored ");

     send_err_msg = TRUE;
     return;
   }
#endif

   /* Sanity check the number of bursts */
   if (num == 0 || num > MAX_CM_RSSI_MEASUREMENTS)
   {
      MSG_GERAN_ERROR_1_G("Invalid num of pwr measures %d",num);
      if (num == 0)
      {
        return;
      }
      num = MAX_CM_RSSI_MEASUREMENTS;
   }

#ifdef  FEATURE_GSM_COEX

  /* Initialise the list of monitor ARFCNs to be arbitrated */
  for(i=0;i<num;i++)
  {
     gl1_hw_arbitration[gas_id].monitors.monitor[i].arfcn = ARFCN[i];
  }
  fw_offset_to_meas_start = gl1_hw_get_rf_gsm_mon_setup_time(gas_id);

  pwr_meas_req_time = fw_offset_to_meas_start + PWR_MEAS_LENGTH_QS;

  gl1_hw_arbitration[gas_id].monitors.count = num;

  gl1_hw_arbitration[gas_id].notify_event( ARBITRATE_MONITORS, gas_id );
#endif  /* FEATURE_GSM_COEX */

   GL1_HANDLER_PERFORMANCE_LOGGING(START_PWR_MEAS_RF,gas_id);

   for (i = 0; i < num; i++)
   {
     rx_chan_params[i].burst_type = RF_MON_BURST;
     rx_chan_params[i].band       = gl1_hw_arfcn_to_rfgsm_band(ARFCN[i]);
     rx_chan_params[i].arfcn      = (uint16)ARFCN[i].num;
     rx_chan_params[i].rx_lvl_dbm = rx_signal_strength[i];
     rx_chan_params[i].lna_range  = NULL;
     rx_chan_params[i].gain_ptr   = &tags[i].gain;
     rx_chan_params[i].freq_err   = gl1_hw_rf_get_freq_error(gas_id);

#ifdef GERAN_L1_ENHANCED_RX
     rx_chan_params[i].enh_rx_params.enh_rx_enable_flag = FALSE;
     rx_chan_params[i].enh_rx_params.enh_rx_offset_freq = 0;
#endif /*GERAN_L1_ENHANCED_RX*/
#ifdef FEATURE_GSM_RF_DEBUG_NV
     if ( mdsp_rf_debug_NV_enable_g [gas_id] )
     {

     MSG_GERAN_HIGH_3_G("   rx_signal_strength[i]  = %d rx_chan_params[i].band  = %d  arfcns[i].num =%d",rx_signal_strength[i], rx_chan_params[i].band ,ARFCN[i].num);
     MSG_GERAN_HIGH_3_G(" gl1_hw_cm_x2g_power_measure  mdsp_rf_debug_NV_enable_g[0] = %d mdsp_rf_debug_upper_power_limit_g = %d mdsp_rf_debug_lower_power_limit_g = %d",
                                 mdsp_rf_debug_NV_enable_g[gas_id],mdsp_rf_debug_upper_power_limit_g[gas_id] ,mdsp_rf_debug_lower_power_limit_g[gas_id]);
     MSG_GERAN_HIGH_3_G(" gl1_hw_cm_x2g_power_measure  mdsp_rf_debug_band_g[0] = %d mdsp_rf_debug_arfcn_g = %d  i=%d",mdsp_rf_debug_band_g [gas_id],mdsp_rf_debug_arfcn_g[gas_id],i);


     if ( (mdsp_rf_debug_NV_enable_g [gas_id])&&
            ((rx_signal_strength[i]/16)<= mdsp_rf_debug_upper_power_limit_g [gas_id])&&
            ((rx_signal_strength[i]/16)>= mdsp_rf_debug_lower_power_limit_g [gas_id])&&
            (rx_chan_params[i].band == mdsp_rf_debug_band_g [gas_id])&&
            (ARFCN[i].num == mdsp_rf_debug_arfcn_g[gas_id] )
            )

     {
          MSG_GERAN_HIGH_1_G(" flag_for_fw_pwr_meas_crash  = %d ",get_flag_for_fw_pwr_meas_crash(i ,gas_id ));
         //  SET the RF  bebug crash flag true
         set_flag_for_fw_pwr_meas_crash(i,TRUE,gas_id);
     }
     else
     {
         set_flag_for_fw_pwr_meas_crash(i,FALSE,gas_id);
     }
     }
#endif /* FEATURE_GSM_RF_DEBUG_NV */
     div_rx_chan_params[i].rx_lvl_dbm = 0;
     div_rx_chan_params[i].lna_range = NULL;
     div_rx_chan_params[i].probe_burst = NULL;
#ifdef FEATURE_GSM_RX_DIVERSITY
     div_rx_chan_params[i].gain_ptr = &tags[i].gain;
#else
     div_rx_chan_params[i].gain_ptr = NULL;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef GERAN_L1_HLLL_LNASTATE
     rx_chan_params[i].high_lin_mode_active  = &(tags[i].sawless_lnastate);
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef  FEATURE_GSM_COEX
     coex_params_x2g[i].coex_priority   = gl1_hw_arbitration[gas_id].monitors.monitor[i].priority;
     coex_params_x2g[i].coex_desense_id = gl1_hw_arbitration[gas_id].monitors.monitor[i].desense_id;

     /* Log power measurement burst in cxm_log_table */
     l1_log_cxm_new_entry( mdsp_get_current_seq_num(gas_id),
                           ARFCN[i].num,
                           CXM_LOG_MONITOR,
                           offset[i] + pwr_meas_req_time - 96,
                           CXM_LOG_UNKNOWN_VALUE, 0, 0, 0,
                           CXM_LOG_ACTIVITY_RX,
                           gas_id );

#endif /* FEATURE_GSM_COEX */

     rx_chan_params[i].rf_sequence_number = gl1_hw_get_next_rf_seq_num();
     rf_seq_num[i] = rx_chan_params[i].rf_sequence_number;

   }

   gl1_hw_rf_rx_burst((uint16)num,
                      rx_chan_params
                      ,div_rx_chan_params
                      ,FALSE
                      ,MON_MODE
                      ,gas_id
                     );
#ifdef  FEATURE_GSM_COEX
 mdsp_x2g_power_measure(num, results_buffer, &(ARFCN[0]), offset, rf_seq_num, &coex_params_x2g[0], gas_id,&cmdbuf_ptr);
#else
 mdsp_x2g_power_measure(num, results_buffer, &(ARFCN[0]), offset, rf_seq_num, gas_id,&cmdbuf_ptr);
#endif  /* FEATURE_GSM_COEX */

 gfwhost_cmdbuf_offsets[gas_id].pwrmsr_ptr=(GfwX2GPwrMsrCmd *)cmdbuf_ptr;
 #ifdef FEATURE_GSM_TDS_DEBUG2
 MSG_GERAN_HIGH_1_G("power measurment cmd written at %x in the mdsp interface",gfwhost_cmdbuf_offsets[gas_id].pwrmsr_ptr);
 #endif

   gl1_hw_log_cell_cmds_to_diag (cmd_cm_power_measure, FALSE,gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_x2g_power_measure

DESCRIPTION
  This function initiates one to 20 power measurements in the next n frames.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint16 gl1_hw_x2g_power_measure
(
   ARFCN_T                     ARFCN[MAX_CM_RSSI_MEASUREMENTS_REQ],               /* Absolute Radio Frequency Channels  */
   dBx16_T                     rx_signal_strength[MAX_CM_RSSI_MEASUREMENTS_REQ],  /* Signal Strengths at Antenna        */
   uint8                       num,                                               /* Number of measurements to make     */
   uint16                      offset[MAX_CM_RSSI_MEASUREMENTS_REQ],              /* When to make a measurement in qs   */
   gl1_hw_burst_position_type  position[MAX_CM_RSSI_MEASUREMENTS_REQ],            /* position of burst within a gap     */
   gl1_hw_rx_tags_type         tags[MAX_CM_RSSI_MEASUREMENTS_REQ],
   uint32                      g_frame_number[MAX_CM_RSSI_MEASUREMENTS_REQ],
   uint16            offset2,
   uint32            g_frame_number2
#ifdef FEATURE_DUAL_SIM
   ,sys_modem_as_id_e_type      as_id
#endif /*FEATURE_DUAL_SIM */
)
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/

   return gl1_hw_x2g_power_measure_geran(ARFCN,rx_signal_strength,num,offset,position,tags,g_frame_number,offset2,g_frame_number2,as_id);
}

/*===========================================================================

FUNCTION  gl1_hw_x2g_power_measure_geran

DESCRIPTION
  This function initiates one to 20 power measurements in the next n frames.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint16 gl1_hw_x2g_power_measure_geran
(
   ARFCN_T                     ARFCN[MAX_CM_RSSI_MEASUREMENTS_REQ],               /* Absolute Radio Frequency Channels  */
   dBx16_T                     rx_signal_strength[MAX_CM_RSSI_MEASUREMENTS_REQ],  /* Signal Strengths at Antenna        */
   uint8                       num,                                               /* Number of measurements to make     */
   uint16                      offset[MAX_CM_RSSI_MEASUREMENTS_REQ],              /* When to make a measurement in qs   */
   gl1_hw_burst_position_type  position[MAX_CM_RSSI_MEASUREMENTS_REQ],            /* position of burst within a gap     */
   gl1_hw_rx_tags_type         tags[MAX_CM_RSSI_MEASUREMENTS_REQ],
   uint32                      g_frame_number[MAX_CM_RSSI_MEASUREMENTS_REQ],
   uint16            offset2,
   uint32            g_frame_number2,
   sys_modem_as_id_e_type      as_id
)
{
  static uint16  offset_local[MAX_CM_RSSI_MEASUREMENTS_REQ];
  uint8          i;
  int32          offset_adjust;
  uint8          num_cmds = 0;
  uint8          sm_num = 0;
  gas_id_t       gas_id =  geran_map_nas_id_to_gas_id(as_id);
  uint16         current_seq_num = mdsp_get_current_seq_num( gas_id);
  num_of_pwr_measures = num;

  if(send_err_msg)
  {
    
    gl1_t2g_last_state = PWR_MEAS;
    MSG_GERAN_HIGH_0_G("GL1_HW:gl1_hw_x2g_power_measure at the begnning" );
  }


    if( queued_x2g_startup )
  {
      /* Avoid allocating on the stack */
      static gl1_hw_x2g_pwr_meas_cmd_params_t  pwr_meas_cmd; /* static_dual_spaced_ignore */

    uint16  idx = 0;

    while( idx < MAX_CM_RSSI_MEASUREMENTS_REQ )
    {
      pwr_meas_cmd.g_frame_number[idx]     = g_frame_number[idx];
        pwr_meas_cmd.ARFCN[idx]              = ARFCN[idx];
        pwr_meas_cmd.rx_signal_strength[idx] = rx_signal_strength[idx];
        pwr_meas_cmd.offset[idx]             = offset[idx];
        pwr_meas_cmd.position[idx]           = position[idx];
        pwr_meas_cmd.tags[idx]               = tags[idx];

    idx++;
    }

      pwr_meas_cmd.gl1_hw_x2g_api_issue_fn = g_frame_number[0];

    /* Queue it one frame only */
    pwr_meas_cmd.num     = num;

    pwr_meas_cmd.cmd_id  = PWR_MEAS;
  pwr_meas_cmd.offset2 =offset2;
  pwr_meas_cmd.g_frame_number2=g_frame_number2;

    gl1_hw_x2g_create_cmd_node( (gl1_hw_x2g_cmd_data_u *) &pwr_meas_cmd );

    MSG_GERAN_HIGH_3_G("GL1_HW:gl1_hw_x2g_power_measure queueing",0,0, 0 );

    return  gl1_hw_x2g_set_info(1, &ARFCN[0], PWR_MEAS, PENDING);
  }

  if ( (gl1_hw_x2g_startup_info.g_fn > g_frame_number[0])
    ||( send_err_msg   )
    )
  {
    MSG_GERAN_ERROR_3_G( "X2G: g_frame_number %d is in the past - do nothing: startup_fn %d GFN %d",
               g_frame_number[0],
         gl1_hw_x2g_startup_info.g_fn,
         GSTMR_GET_FN_GERAN(gas_id)
       );

  send_err_msg = TRUE;
  gl1_t2g_last_state = PWR_MEAS;
 

    return  gl1_hw_x2g_set_info(num, &ARFCN[0], PWR_MEAS, ABORTING);
  }

  for( i=0;i<POWER_MEAS_NUM_SM;i++)
  {
    if( cm_pwr_meas_state_machine[i].power_meas_state == POWER_MEAS_STATE_INACTIVE)
    {
      sm_num = i;
      break;
    }

    if( i == (POWER_MEAS_NUM_SM - 1 ) )
    {
      MSG_ERROR("TDS X2G: All %d state machines in use!!",POWER_MEAS_NUM_SM,0,0);

   send_err_msg = TRUE;
   gl1_t2g_last_state   = PWR_MEAS;
  


      return  gl1_hw_x2g_set_info(num, &ARFCN[0], PWR_MEAS, ABORTING);
  }
  }
  cm_pwr_sm = sm_num;
  cm_pwr_meas_state_machine[sm_num].rssi_meas_result.cmd_fn = GSTMR_GET_FN_GERAN(gas_id);
  cm_pwr_meas_state_machine[cm_pwr_sm].rssi_meas_result.gl1_skip_last_burst=FALSE;
  gl1_hw_x2g_pwr_meas_info.startup_offset2 = offset2;
  gl1_hw_x2g_pwr_meas_info.startup_g_fn2    = g_frame_number2;

#ifdef FEATURE_GSM_TDS_DEBUG2
    MSG_GERAN_HIGH_2_G("X2G:2nd offset =%d,and frame no.=%d",offset2,g_frame_number2);
#endif

  if( num > MAX_CM_RSSI_MEASUREMENTS_REQ )
  {
    /* Limit to 10 */
    num = MAX_CM_RSSI_MEASUREMENTS_REQ;
  }

  /* adjust offsets to be relative to X2G_STARTUP frame reference accumulate number of
     pwr measurements for initial 2 frame period i.e. offset < 10000 */
  for( i=0; i < num; i++ )
  {

    /*Ensure that a frame tick wrap between startup and this call is taken into account*/
    offset_adjust = gl1_hw_x2g_offset_adjust( g_frame_number[i],
                                          offset[i],
                        PWR_MEAS,
                        num,
                        gas_id
                      );

    /* need to check for hyperframe wraparound */
    offset_local[i] = offset[i] + (offset_adjust * QS_PER_FRAME);

    /* This check should naturally limit the number of cmds issued to
     * MAX_CM_RSSI_MEASUREMENTS. ASSUMPTION is that calling function
     * KNOWS that we are limited to 10 pwr meas/frame
     * Implicit assumption that offset values given by X RAT are in increasing order
     */
    if (offset_local[i] < (2*QS_PER_FRAME))
    {
    /* Increment the number of measurements to be issued this frame. */
      num_cmds++;
    }

    cm_pwr_meas_state_machine[sm_num].rssi_meas_result.meas.ARFCN[i] = ARFCN[i];
#ifdef FEATURE_GSM_TDS_DEBUG2
        MSG_GERAN_HIGH_2_G("Arfcn[%d] = %d",i,ARFCN[i].num);
#endif

  } /* for (i=0; i < num; i++) */

  cm_pwr_meas_state_machine[sm_num].rssi_meas_result.meas.num = num;
  num_pwr_meas=num_cmds;

  if( num_cmds > 0 )
  {
    for( i=0; i<num_cmds; i++ )
    {
      cm_pwr_meas_state_machine[sm_num].rssi_meas_result.meas.tags[i] = tags[i];
    }

    /* call core CM power measure function */
    gl1_hw_cm_x2g_power_measure( ARFCN,
                             rx_signal_strength,
                             num_cmds,
                             sm_num,
                             offset_local,
                             position,
                             cm_pwr_meas_state_machine[sm_num].rssi_meas_result.meas.tags,
                             geran_map_gas_id_to_nas_id(gas_id));

    cm_pwr_meas_state_machine[sm_num].power_meas_state = POWER_MEAS_STATE_COMMAND;

    memset( cm_pwr_meas_state_machine[sm_num].rssi_meas_result.meas.samples,
        0,
        MAX_CM_RSSI_MEASUREMENTS_REQ*sizeof(gl1_hw_measurement_T)
        );

    cm_pwr_meas_state_machine[sm_num].rssi_meas_result.num_cmd_idx = 0;
    cm_pwr_meas_state_machine[sm_num].rssi_meas_result.num_cmd_idx++;

    cm_pwr_meas_state_machine[sm_num].rssi_meas_result.result_idx = 0;
    cm_pwr_meas_state_machine[sm_num].rssi_meas_result.num_cmd    = num_cmds;
    cm_pwr_meas_state_machine[sm_num].rssi_meas_result.cmd_issued = num_cmds;

    cm_pwr_meas_state_machine[sm_num].rssi_meas_result.struct_in_use = TRUE;
    cm_pwr_meas_state_machine[sm_num].rssi_meas_result.seq_num       = current_seq_num;


  }    /* if( num_cmds > 0 ) */
  else
  {
    MSG_GERAN_ERROR_0_G("X2G: num power measurements == 0");

    cm_pwr_meas_state_machine[sm_num].rssi_meas_result.struct_in_use = FALSE;
    cm_pwr_meas_state_machine[sm_num].rssi_meas_result.seq_num       = 0xffff;/*invalid*/

  cm_pwr_meas_state_machine[sm_num].power_meas_state = POWER_MEAS_STATE_INACTIVE;

    send_err_msg = TRUE;
    gl1_t2g_last_state   = PWR_MEAS;
    
   
  }

  gl1_profiling_t2g_api(t2g_api_profiling_index,PWR_MEAS, FALSE,gas_id);

  if(send_err_msg)
    cm_pwr_meas_state_machine[sm_num].rssi_meas_result.seq_num = gl1_hw_x2g_set_info(num, &ARFCN[0], PWR_MEAS, ABORTING);
  else
    cm_pwr_meas_state_machine[sm_num].rssi_meas_result.seq_num = gl1_hw_x2g_set_info(num, &ARFCN[0], PWR_MEAS, USING);

  return cm_pwr_meas_state_machine[sm_num].rssi_meas_result.seq_num;

}

/*===========================================================================

FUNCTION  gl1_hw_x2g_start_acquisition

DESCRIPTION
  Initiates an X2G acquisition

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

uint16  gl1_hw_x2g_start_acquisition
(
   ARFCN_T               ARFCN,
   dBx16_T               rx_signal_strength,
   uint16                timing_offset,
   int16                 frequency_offset,
   boolean               sch_decode,
   gl1_hw_rx_tags_type*  tags_ptr,
   uint32                g_frame_number,
   void*                 gsm_cell_ptr
#ifdef FEATURE_DUAL_SIM
   ,sys_modem_as_id_e_type      as_id
#endif /*FEATURE_DUAL_SIM */
)
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/

   return gl1_hw_x2g_start_acquisition_geran(ARFCN,rx_signal_strength,timing_offset,
             frequency_offset, sch_decode,
             tags_ptr,g_frame_number,gsm_cell_ptr,as_id);
}
/*===========================================================================

FUNCTION  gl1_hw_x2g_start_acquisition_geran

DESCRIPTION
  Initiates an X2G acquisition

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

uint16  gl1_hw_x2g_start_acquisition_geran
(
   ARFCN_T               ARFCN,
   dBx16_T               rx_signal_strength,
   uint16                timing_offset,
   int16                 frequency_offset,
   boolean               sch_decode,
   gl1_hw_rx_tags_type  *tags_ptr,
   uint32                g_frame_number,
   void*                 gsm_cell_ptr,
   sys_modem_as_id_e_type      as_id
)
{
  int32 offset_adjust;
  uint16  local_timing_offset;
  uint8   sm_num = 0;
  uint8   i;
  gas_id_t       gas_id = geran_map_nas_id_to_gas_id(as_id);

  /* Store gas_id to be used by x2g stop acq */
  gl1_hw_t2g_gas_id = gas_id;

  if(send_err_msg)
  {
    gl1_t2g_last_state  = TONE_DETECT;
MSG_GERAN_HIGH_0_G("GL1_HW:gl1_hw_x2g_start_acquisition at the begnning");
  }


  if(  queued_x2g_startup )
  {
    /* Avoid allocating on the stack */
    static gl1_hw_x2g_start_acq_cmd_params_t  start_acq_cmd; /* static_dual_spaced_ignore */

    start_acq_cmd.g_frame_number          = g_frame_number;
    start_acq_cmd.gl1_hw_x2g_api_issue_fn = g_frame_number;

  /* Queue it one frame only */
  start_acq_cmd.timing_offset       = timing_offset;

    start_acq_cmd.ARFCN               = ARFCN;
    start_acq_cmd.rx_signal_strength  = rx_signal_strength;
  start_acq_cmd.frequency_offset    = frequency_offset;
    start_acq_cmd.sch_decode          = sch_decode;

    start_acq_cmd.tags                = *(tags_ptr);

    start_acq_cmd.gsm_cell_ptr        = gsm_cell_ptr;

  start_acq_cmd.cmd_id              = TONE_DETECT;

  gl1_hw_x2g_create_cmd_node( (gl1_hw_x2g_cmd_data_u *) &start_acq_cmd );

  MSG_GERAN_HIGH_3_G(" queuing start acq fn %d g %d t %d ", GSTMR_GET_FN_GERAN(gas_id), g_frame_number, timing_offset );

    return  gl1_hw_x2g_set_info(1, &ARFCN, TONE_DETECT, PENDING);
  }

#ifdef FEATURE_GSM_TDS_DEBUG
  MSG_3(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "X2G ACQ: timing_offset %d, GFN %d current qs value %d",
          timing_offset,GSTMR_GET_FN_GERAN(gas_id), gstmr_rd_qsymbol_count_geran(as_id));
#endif

  for( i=0;i<ACQ_MEAS_NUM_SM;i++)
  {
    if( cm_acq_state_machine[i].acq_meas_state == ACQ_MEAS_STATE_INACTIVE)
  {
      sm_num = i;
      break;
  }
    if( i == (ACQ_MEAS_NUM_SM - 1 ) )
  {
      MSG_ERROR("TDS X2G: All 5 ACQ state machines in use!!",0,0,0);
      send_err_msg = TRUE;
      gl1_t2g_last_state  = TONE_DETECT;
   

      return  gl1_hw_x2g_set_info(1, &ARFCN, TONE_DETECT, ABORTING);
  }
  }

  /* This is needed by the x2g_cleanup routine incase of errors */
  cm_acq_sm = sm_num;

  cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_store.acq_start_frame      = g_frame_number;
  cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_store.acq_win_start_offset = timing_offset;
  cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_store.gsm_cell_ptr         = gsm_cell_ptr;

  if( send_err_msg != FALSE  )
  {
    gl1_t2g_last_state  = TONE_DETECT;
 
    return  gl1_hw_x2g_set_info(1, &ARFCN, TONE_DETECT, ABORTING);
  }

  cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.cleanup_async_int_used = FALSE;
  cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.cleanup_pending = FALSE;
  cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.stop_acq_pending = FALSE;

  gl1_profiling_t2g_api(t2g_api_profiling_index, TONE_DETECT, TRUE,gas_id);


  /* reset x2g_start_acq offset to be relative to x2g_startup g_frame_number */
  offset_adjust = gl1_hw_x2g_offset_adjust( g_frame_number,
                                          timing_offset,
                      TONE_DETECT,
                      1,
                      gas_id
                      );
  /* need to check for hyperframe wraparound */
  local_timing_offset = timing_offset + (offset_adjust * QS_PER_FRAME);

#ifdef FEATURE_GSM_TDS_DEBUG
  MSG_5(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "X2G ACQ: startup_info.g_fn %d g_frame_number %d offset_adjust %d, timing_offset %d current qs value %d",
           gl1_hw_x2g_startup_info.g_fn, g_frame_number, offset_adjust,timing_offset, gstmr_rd_qsymbol_count_geran(as_id)));
#endif

  if(local_timing_offset < (2*QS_PER_FRAME))
  {
    if( ( cm_startup_issued )
    &&(  send_err_msg == FALSE  )
    )
  {
      gl1_hw_sch_decode_t sch_decode_mode = T2G_CONVENTIONAL_SCH_DECODE;

#ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
      if (gl1_hw_x2g_get_sch_decode_mode( sch_decode ) == X2G_PARALLEL_SCH_DECODE)
      {
         sch_decode_mode = T2G_PARALLEL_SCH_DECODE;
      }
#endif

      /* call core Acq processing */
      gl1_hw_start_acquisition_geran( ARFCN,
                                rx_signal_strength,
                                local_timing_offset,
                                frequency_offset,
                                sch_decode_mode,
                                GL1_HW_ACQ_AFC_ON_ROTATOR,
                                tags_ptr,
                                FALSE,      
                                as_id
                               );

    /*Store values*/
      cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.arfcn.band = ARFCN.band;
      cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.arfcn.num  = ARFCN.num;

      cm_acq_state_machine[sm_num].sch_decode = sch_decode;

      cm_acq_state_machine[sm_num].id                             = 0;
      cm_acq_state_machine[sm_num].uinstall_acq_handler_dec_frame = ACQ_HANDLER_TIMEOUT_FRAMES;

      cm_acq_state_machine[sm_num].tone_detect_result = FALSE;
      cm_acq_state_machine[sm_num].sch_decode_result  = FALSE;

      cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_rsp_sent = FALSE;

      cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.acq_start_frame      = g_frame_number;
      cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.acq_win_start_offset = timing_offset;
      cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.gsm_cell_ptr         = gsm_cell_ptr;
  }
  else
  {
      send_err_msg = TRUE;

  }

  }
  else
  {
    send_err_msg = TRUE;
  }

  gl1_profiling_t2g_api(t2g_api_profiling_index, TONE_DETECT, FALSE,gas_id);

  if(send_err_msg)
    cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.message_id = gl1_hw_x2g_set_info(1, &ARFCN, TONE_DETECT, ABORTING);
  else
    cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.message_id = gl1_hw_x2g_set_info(1, &ARFCN, TONE_DETECT, USING);

  return  cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.message_id;

}

/*===========================================================================

FUNCTION  gl1_hw_x2g_stop_acquisition

DESCRIPTION
  X2G stop acquisition handler.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_x2g_stop_acquisition ( uint16 offset, uint32 g_frame_number )
{

  int32   offset_adjust;
  uint32  local_offset;
  uint32  current_fn;
#ifdef FEATURE_GSM_TDS_DEBUG
  sys_modem_as_id_e_type      as_id;
#endif
  uint8   sm_num = 0;


  gas_id_t       gas_id =  gl1_hw_t2g_gas_id;
#ifdef FEATURE_GSM_TDS_DEBUG
  as_id = geran_map_gas_id_to_nas_id(gas_id);
#endif
  if(  queued_x2g_startup )
  {
    /* Avoid allocating on the stack */
    static gl1_hw_x2g_stop_acq_cmd_params_t  stop_acq_cmd; /* static_dual_spaced_ignore */

    stop_acq_cmd.g_frame_number          = g_frame_number;
    stop_acq_cmd.gl1_hw_x2g_api_issue_fn = g_frame_number;

    /* Queue it one frame only */
    stop_acq_cmd.offset       = offset;
    stop_acq_cmd.cmd_id       = STOP_TONE_DETECT;

    gl1_hw_x2g_create_cmd_node( (gl1_hw_x2g_cmd_data_u *) &stop_acq_cmd );

    MSG_GERAN_HIGH_3_G(" queued stop acq fn %d g %d t %d ",GSTMR_GET_FN_GERAN(gas_id),g_frame_number,offset );

    return;
  }

  gl1_profiling_t2g_api(t2g_api_profiling_index, STOP_TONE_DETECT, TRUE,gas_id);


  sm_num = cm_acq_sm;


  /* assume that X is able to schedule the command for the next FT, so the async
     int is not required. Adjust the offset relative to the next GSM frame (where the
     command is scheduled for) */
#ifdef FEATURE_GSM_TDS_DEBUG
  MSG_5(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "X2G ACQ Stop: g_frame_number %d, offset %d, GFN %d current qs value %d cleanup_offset %d",
        g_frame_number,offset,GSTMR_GET_FN_GERAN(gas_id), gstmr_rd_qsymbol_count_geran(as_id), gl1_hw_x2g_cleanup_info.offset);
#endif

  /* reset x2g_start_acq offset to be relative to x2g_startup g_frame_number */
  offset_adjust = gl1_hw_x2g_offset_adjust( g_frame_number,
                                          offset,
                      STOP_TONE_DETECT,
                      1,
                      gas_id
                      );


  /* need to check for hyperframe wraparound */
  local_offset = offset +  offset_adjust * QS_PER_FRAME;


#ifdef FEATURE_GSM_TDS_DEBUG
  MSG_4(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "X2G ACQ Stop: offset_adjust %d, new offset %d GFN %d current qs value %d",
          offset_adjust,local_offset,GSTMR_GET_FN_GERAN(gas_id), gstmr_rd_qsymbol_count_geran(as_id));
#endif


  /* TDS does not use this function in the way it was intended.
   *  They call stop acquisition before cleanup. They provide
   *  pre-validated offset value which they want executed - they
   *  don't want to be able to just halt at point of call
   */

  if((local_offset >= 2*QS_PER_FRAME)&&( send_err_msg == FALSE ))
  {

    if (cm_acq_state_machine[sm_num].sch_decode)
    {
      current_fn = GSTMR_GET_FN_GERAN(gas_id);

      /* Take care of wrap around scenarios around 2715647 */
      if( IS_FRAME_NUM_LATER(current_fn, cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_g_fn))
      {
        send_err_msg = TRUE;

        MSG_GERAN_ERROR_3_G(" stop acq error current fn %d fn %d offset %d ",current_fn,cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_g_fn,offset );
        return;
      }
    }

    /* Let tone detect handler issue the stop command */
    cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_offset_valid = TRUE;
    cm_acq_state_machine[sm_num].acq_meas_state                            = ACQ_MEAS_STATE_ACTIVE;
    cm_acq_state_machine[sm_num].seq_num                                   = mdsp_get_current_seq_num(gas_id);
    cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_pending   = TRUE;


    MSG_GERAN_HIGH_1_G("T2G ACQ: stop acq deferred to ACQ Handler - offset passed %d",
           local_offset);

  }
  else
  {
    if( ( cm_startup_issued )&&( send_err_msg == FALSE ))
    {
      /* Assumption here is that async interrupt will always be used */
      gl1_hw_stop_acquisition_geran(local_offset,gas_id);

      cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_offset_valid = FALSE;
      cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_rsp_sent = FALSE;

      cm_acq_state_machine[sm_num].acq_meas_state = ACQ_MEAS_STATE_ACTIVE;
      cm_acq_state_machine[sm_num].seq_num        = mdsp_get_current_seq_num(gas_id);

      MSG_GERAN_HIGH_1_G("T2G ACQ: stop acq issued - offset passed %d",
              local_offset);

    }
    else
    {
      send_err_msg = TRUE;
    }
  }

#ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
  /* Add sm_num into array of active sm, so can determine the order that they have been set */
  if ((cm_acq_state_machine[sm_num].acq_meas_state == ACQ_MEAS_STATE_ACTIVE) && (cm_acq_state_machine_num_active < ACQ_MEAS_NUM_SM))
  {
     cm_acq_state_machine_active_array[cm_acq_state_machine_num_active] = sm_num;
     cm_acq_state_machine_num_active += 1;
  }
#endif

  gl1_profiling_t2g_api(t2g_api_profiling_index, STOP_TONE_DETECT, FALSE,gas_id);


}

/*===========================================================================

FUNCTION  gl1_hw_x2g_sch_burst

DESCRIPTION
  X2G ASYNC SCH

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
uint16 gl1_hw_x2g_sch_burst
(
   ARFCN_T               ARFCN,
   int16                 signal_strength,
   uint16                timing_offset,
   int16                 frequency_offset,
   gl1_hw_rx_tags_type*  tags_ptr,
   uint32                g_frame_number,
   boolean               reconfirm,
   void*                 gsm_cell_ptr
#ifdef FEATURE_DUAL_SIM
                                ,sys_modem_as_id_e_type      as_id
#endif /*FEATURE_DUAL_SIM */
)
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/
   return gl1_hw_x2g_sch_burst_geran(ARFCN,signal_strength,timing_offset,frequency_offset,tags_ptr,g_frame_number,reconfirm,gsm_cell_ptr,as_id);
}
/*===========================================================================

FUNCTION  gl1_hw_x2g_sch_burst_geran

DESCRIPTION
  X2G ASYNC SCH

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
uint16 gl1_hw_x2g_sch_burst_geran
(
   ARFCN_T                ARFCN,
   int16                  signal_strength,
   uint16                 timing_offset,
   int16                  frequency_offset,
   gl1_hw_rx_tags_type*   tags_ptr,
   uint32                 g_frame_number,
   boolean                reconfirm,
   void*                  gsm_cell_ptr,
   sys_modem_as_id_e_type as_id
)
{
  int32  offset_adjust;
  uint16 local_timing_offset;
  uint8  sm_num = 0;
  uint8  i;
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  if(send_err_msg)
  {
    gl1_t2g_last_state  = SCH_DETECT;
        MSG_GERAN_HIGH_0_G("GL1_HW:gl1_hw_x2g_sch_burst at the begnning");
  }

  if(queued_x2g_startup)
  {
    /* Avoid allocating on the stack */
    static gl1_hw_x2g_async_sch_cmd_params_t  async_sch_cmd; /* static_dual_spaced_ignore */

    MSG_GERAN_HIGH_2_G("X2G: unexpected x2g_sch_burst offset: offset = %d; x2g_startup_offset=%d",
              timing_offset, gl1_hw_x2g_startup_info.offset );

    async_sch_cmd.g_frame_number          = g_frame_number;
    async_sch_cmd.gl1_hw_x2g_api_issue_fn = g_frame_number;

    /* Queue it one frame only */
    async_sch_cmd.timing_offset    = timing_offset;

    async_sch_cmd.ARFCN            = ARFCN;
    async_sch_cmd.signal_strength  = signal_strength;
    async_sch_cmd.frequency_offset = frequency_offset;

    async_sch_cmd.tags             = *tags_ptr;

    async_sch_cmd.cmd_id           = SCH_DETECT;

    async_sch_cmd.reconfirm        = reconfirm;
    async_sch_cmd.gsm_cell_ptr     = gsm_cell_ptr;

    gl1_hw_x2g_create_cmd_node( (gl1_hw_x2g_cmd_data_u *) &async_sch_cmd  );

    MSG_GERAN_HIGH_3_G("queued sch fn %d g %d t %d", GSTMR_GET_FN_GERAN(gas_id),g_frame_number, timing_offset );

    return gl1_hw_x2g_set_info(1, &ARFCN, SCH_DETECT, PENDING);
  }

  for(i=0;i<SCH_MEAS_NUM_SM;i++)
  {
    if(sch_state_machine[i].sch_meas_state == SCH_MEAS_STATE_INACTIVE)
    {
      sm_num = i;
      break;
    }
    if(i == (SCH_MEAS_NUM_SM - 1))
    {
      MSG_ERROR("TDS X2G: All %d SCH state machines in use!!",SCH_MEAS_NUM_SM,0,0);
      send_err_msg = TRUE;
      gl1_t2g_last_state  = SCH_DETECT;
   
      return  gl1_hw_x2g_set_info(1, &ARFCN, SCH_DETECT, ABORTING);
    }
  }

  cm_sch_sm = sm_num;

  sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_store.gsm_cell_ptr   = gsm_cell_ptr;
  sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_store.reconfirm      = reconfirm;
  sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_store.sch_frame      = g_frame_number;

  sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.cleanup_async_int_used = FALSE;
  sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.cleanup_pending = FALSE;
  gl1_profiling_t2g_api(t2g_api_profiling_index,SCH_DETECT, TRUE,gas_id);

#ifdef  FEATURE_GSM_TDS_DEBUG
  MSG_GERAN_HIGH_3_G("X2G SCH: timing offset %d, freq_offset %d, g_fn %d",
           timing_offset,frequency_offset,g_frame_number );
#endif

  /* reset x2g_start_acq offset to be relative to x2g_startup g_frame_number */
  offset_adjust = gl1_hw_x2g_offset_adjust(g_frame_number,
                                           timing_offset,
                                           SCH_DETECT,
                                           1,
                                           gas_id);

  /* need to check for hyperframe wraparound */
  local_timing_offset = timing_offset + (offset_adjust * QS_PER_FRAME);

  if(local_timing_offset < (2*QS_PER_FRAME))
  {
    if(cm_startup_issued &&
       send_err_msg == FALSE)
    {
      /* save gain data passed in */
      stored_tags.gain = tags_ptr->gain;

      /* call core SCH processing */
      gl1_hw_x2g_rx_async_sch_burst_on_rotator(
                             ARFCN,
                             signal_strength,
                             local_timing_offset,
                             frequency_offset,
                             &stored_tags,
                             &sch_state_machine[sm_num].sch_metrics_buffer_idx,
                             &sch_state_machine[sm_num].sch_metrics_seq_num,
                             &sch_state_machine[sm_num].sch_data_buffer_idx,
                             as_id);

      memset(&sch_state_machine[sm_num].sch_metrics,
             0,
             sizeof(sch_state_machine[sm_num].sch_metrics));

      sch_state_machine[sm_num].gl1_hw_x2g_sch_store.gsm_cell_ptr   = gsm_cell_ptr;
      sch_state_machine[sm_num].gl1_hw_x2g_sch_store.reconfirm      = reconfirm;
      sch_state_machine[sm_num].gl1_hw_x2g_sch_store.sch_frame      = g_frame_number;

      sch_state_machine[sm_num].sch_meas_state = SCH_MEAS_STATE_ACTIVE;

      sch_state_machine[sm_num].gl1_hw_x2g_sch_rsp_sent = FALSE;
      sch_state_machine[sm_num].async_sch_decode_result = FALSE;

      sch_state_machine[sm_num].timing_offset = timing_offset;

      sch_state_machine[sm_num].uinstall_sch_handler_dec_frame = 8;
    }
    else
    {
      send_err_msg = TRUE;
    }

    sch_state_machine[sm_num].gl1_hw_x2g_sch_store.arfcn.band = ARFCN.band;
    sch_state_machine[sm_num].gl1_hw_x2g_sch_store.arfcn.num  = ARFCN.num;
  }
  else
  {
    send_err_msg = TRUE;
  }

  gl1_profiling_t2g_api(t2g_api_profiling_index,SCH_DETECT, FALSE,gas_id);

  if(send_err_msg)
    sch_state_machine[sm_num].gl1_hw_x2g_sch_store.message_id = gl1_hw_x2g_set_info(1, &ARFCN, SCH_DETECT, ABORTING);
  else
    sch_state_machine[sm_num].gl1_hw_x2g_sch_store.message_id = gl1_hw_x2g_set_info(1, &ARFCN, SCH_DETECT, USING);

  return  sch_state_machine[sm_num].gl1_hw_x2g_sch_store.message_id;
}

/*===========================================================================

FUNCTION  gl1_hw_x2g_pwr_meas_ft_handler

DESCRIPTION
  GSTMR handler function which can be installed to load any remaining x2g
  commands at the end of the next FT processing.
  Once all commands have been issued, deregister the handler.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gl1_hw_x2g_pwr_meas_ft_handler( sys_modem_as_id_e_type      as_id )
{
  uint8  state_machine_num = 0, i;
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

#ifdef GERAN_L1_HLLL_LNASTATE
  gl1_defs_metrics_lnastate gl1_metrics_lnastate;
  uint8 SawlessHLLLState[MAX_NUMBER_OF_CHANNELS] = {0};
#endif /*GERAN_L1_HLLL_LNASTATE*/

  /* Check both state machines */
  while( state_machine_num < POWER_MEAS_NUM_SM )
  {
    switch( cm_pwr_meas_state_machine[ state_machine_num ].power_meas_state )
    {
    case  POWER_MEAS_STATE_COMMAND:
    case  POWER_MEAS_STATE_ACTIVE:
    case  POWER_MEAS_STATE_FIRST_READ:
    case  POWER_MEAS_STATE_SECOND_READ:

#ifdef FEATURE_GSM_TDS_DEBUG
        MSG_GERAN_MED_2_G("pwr meas state %d sm %d",
              cm_pwr_meas_state_machine[ state_machine_num ].power_meas_state,
              state_machine_num );
#endif
      break;


    case POWER_MEAS_STATE_INACTIVE:
    default:

    if( (state_machine_num == 0  )
    ||(state_machine_num == 1  )
    ||(state_machine_num == 2  )
        ||(state_machine_num == 3  ))
  {
    /* Check the other state machine */
    state_machine_num++;
    continue;
  }

    break;
  }

  switch( cm_pwr_meas_state_machine[ state_machine_num ].power_meas_state )
  {

  //case POWER_MEAS_STATE_FIRST_READ:
  case POWER_MEAS_STATE_SECOND_READ:
      {
    /* Check for results and send them back.*/
    if(  gl1_hw_x2g_get_power_measure_results( &cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.meas.samples[0],
                                               &cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.num_cmd,
                                               &cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.meas.tags[0],
                                               state_machine_num,
                                               as_id,
                                               cm_pwr_meas_state_machine[state_machine_num].rssi_meas_result.gl1_skip_last_burst))
  {

      /* original call for sending the RSSI_MEAS_RSP was here */

      cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.num_cmd_idx = 0;
      cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.result_idx  = 0;

      cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.num_cmd = 0;

    cm_pwr_meas_state_machine[ state_machine_num ].power_meas_state = POWER_MEAS_STATE_SECOND_READ;

#ifdef GERAN_L1_HLLL_LNASTATE
      gl1_metrics_lnastate.NoOfChannels = 0;
      for(i=0;i<cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.meas.num;i++)
         {
         if(gl1_hw_get_sawless_HLLL_support(cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.meas.ARFCN[i].band,gas_id))
           {
           if(i > MAX_NUMBER_OF_CHANNELS)
             {
             MSG_GERAN_ERROR_2_G("NoOfChannels %d greater than max number allowed:%d",i,MAX_NUMBER_OF_CHANNELS);
             }
           gl1_metrics_lnastate.gsm_metrics[i].arfcn      = cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.meas.ARFCN[i];
           gl1_metrics_lnastate.gsm_metrics[i].rx_lvl_dbm = cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.meas.samples[i].dBm_x16;
           gl1_metrics_lnastate.gsm_metrics[i].snr        = INVALID_SNR;
           gl1_metrics_lnastate.gsm_metrics[i].jdet_value = cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.meas.samples[i].jdet_value;
           gl1_metrics_lnastate.NoOfChannels++;
           SawlessHLLLState[i] = cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.meas.samples[i].SawlessHLLLState;
           }
        }
      if(gl1_metrics_lnastate.NoOfChannels)
        {
        gl1_hw_rf_burst_metrics(&gl1_metrics_lnastate,gas_id);
        gl1_hw_log_HLin_LLin_metrics(&gl1_metrics_lnastate,SawlessHLLLState,gas_id);
        }
#endif /*GERAN_L1_HLLL_LNASTATE*/

  }
  else
  {
#ifdef FEATURE_GSM_TDS_DEBUG
        MSG_GERAN_MED_1_G("TDS X2G: no pwr meas returned GFN %d", GSTMR_GET_FN_GERAN(gas_id));
#endif
    cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.meas.num = 0;
  }
      }
 break;

 default:

  break;

 }

   if( cm_pwr_meas_state_machine[ state_machine_num ].power_meas_state == POWER_MEAS_STATE_SECOND_READ )
   {
     L1_send_TDS_RSSI_MEAS_RSP  ( &cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result, as_id );

     memset(  cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.meas.samples,
          0,
               MAX_CM_RSSI_MEASUREMENTS_REQ*sizeof(gl1_hw_measurement_T));

     cm_pwr_meas_state_machine[ state_machine_num ].power_meas_state = POWER_MEAS_STATE_INACTIVE;

     cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.gl1_skip_last_burst=FALSE;

     gl1_hw_x2g_clear_info(cm_pwr_meas_state_machine[ state_machine_num ].rssi_meas_result.seq_num, PWR_MEAS);
   }
   else
   {
     if( POWER_MEAS_STATE_INACTIVE != cm_pwr_meas_state_machine[ state_machine_num ].power_meas_state )
     {
       cm_pwr_meas_state_machine[ state_machine_num ].power_meas_state++;
     }
   }

   if( cm_pwr_meas_state_machine[ state_machine_num ].power_meas_state > POWER_MEAS_STATE_SECOND_READ )
   {
     cm_pwr_meas_state_machine[ state_machine_num ].power_meas_state = POWER_MEAS_STATE_INACTIVE;
   }

   state_machine_num++;
  }  /* while( state_machine_num < 5 ) */

  for( i=0;i<POWER_MEAS_NUM_SM;i++)
  {
    if( cm_pwr_meas_state_machine[i].power_meas_state != POWER_MEAS_STATE_INACTIVE)
    {
      break;
    }
    if( i == (POWER_MEAS_NUM_SM -1) )
    {
      gstmr_deregister_handler_geran( gl1_hw_x2g_pwr_meas_ft_handler, as_id );
      MSG_GERAN_HIGH_0_G("power meas handler removed");
    }
  }

  /* if we have an x2g_cleanup pending then send it*/
  if((gl1_hw_x2g_pwr_meas_info.cleanup_pending == TRUE)
     && (gl1_hw_x2g_pwr_meas_info.cleanup_g_fn == GSTMR_GET_FN_GERAN(gas_id)))
  {
    gl1_hw_x2g_pwr_meas_info.cleanup_pending = FALSE;
    /* call core x2g_cleanup processing */
    gl1_hw_cm_cleanup_geran( gl1_hw_x2g_pwr_meas_info.cleanup_offset,as_id );
  }
}


/*===========================================================================

FUNCTION  gl1_hw_x2g_acq_ft_handler

DESCRIPTION
  GSTMR handler function which polls for FB/SB results & sends results message
  back to TDS.
  Once complete, deregister the handler.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static boolean gl1_hw_was_x2g_tone_detected
(
   ARFCN_T arfcn,
   uint16 *id,
   int32  *tone_offset,
   int32 *coarse_freq,
   int32 *fine_freq,
   int32 *afc_freq,
   uint16 *snr,
   uint16  seq_num,   /* used for results correlation */
   boolean *schedule_error,
   gas_id_t gas_id
)
{
  acq_decode_rpt local_fcch_rpt;

  boolean results_available = gl1_hw_x2g_was_tone_detected( arfcn,
                                                            &local_fcch_rpt,
                                                            seq_num,
                                                            gas_id
                                                           );

  if (results_available)
  {
    *id = local_fcch_rpt.id;

    *tone_offset = local_fcch_rpt.time_offset;

    *coarse_freq = local_fcch_rpt.coarse_freq;

    *fine_freq = local_fcch_rpt.fine_freq;

    *afc_freq = local_fcch_rpt.afc_freq;

    *snr = local_fcch_rpt.snr;

    *schedule_error = local_fcch_rpt.schedule_error;

    MSG_GERAN_HIGH_3_G("Tone Detect: snr %d afc_freq %d seq_num %d",
    local_fcch_rpt.snr,local_fcch_rpt.fine_freq, seq_num );
  }

  return  results_available;
}

void gl1_hw_x2g_acq_ft_handler(sys_modem_as_id_e_type      as_id)
{
  /* 9 frames BETWEEN FB/SB frames. Therefore
   * get tone/sch detect in frames 11/12 worst case.
   * For this worst case scenario X2g command would
   * have been issued on a FB frame itself therefore
   * this handler run on the next frame.
   */
  uint8   sm_num,i,sm_loop;
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  uint32  current_fn   = GSTMR_GET_FN_GERAN(gas_id);

#ifdef FEATURE_GSM_NB_JAMMER_DETECTION
  uint32 index = INVALID_INDEX;
  extern   gl1_hw_nb_jdet_data_t *gl1_hw_nb_jdet_data_ptr[NUM_GERAN_DATA_SPACES];
#endif /* FEATURE_GSM_NB_JAMMER_DETECTION */

#ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
  uint8   local_cm_acq_state_machine_active_array[ACQ_MEAS_NUM_SM];
  uint8   num_sm_loops = cm_acq_state_machine_num_active;

  if(num_sm_loops > ACQ_MEAS_NUM_SM)
  {
     num_sm_loops = ACQ_MEAS_NUM_SM;
     MSG_GERAN_ERROR_2_G("X2G too many acq SM active %d (max %d)", cm_acq_state_machine_num_active, ACQ_MEAS_NUM_SM);
  }

  /* Set local copy of sm array as sm array could get updated during the main SM loop */
  for( i=0; i < num_sm_loops; i++)
  {
    local_cm_acq_state_machine_active_array[i] = cm_acq_state_machine_active_array[i];
  }
#else
  uint8   num_sm_loops = ACQ_MEAS_NUM_SM;
#endif

  for( sm_loop=0; sm_loop < num_sm_loops; sm_loop++)
  {
#ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
    /* Get results for sm in the order they were scheduled */
    sm_num = local_cm_acq_state_machine_active_array[sm_loop];
#else
    sm_num = sm_loop;
#endif

    if( cm_acq_state_machine[sm_num].acq_meas_state != ACQ_MEAS_STATE_INACTIVE )
    {
      int32  coarse_freq, fine_freq, afc_freq;

      if ( (cm_acq_state_machine[sm_num].tone_detect_result &&
             (cm_acq_state_machine[sm_num].sch_decode &&
             !cm_acq_state_machine[sm_num].sch_decode_result ))
          )
      {
        gl1_hw_sch_decode_data *gl1_hw_was_sch_decode_data_p;
        gl1_hw_sch_decode_data  gl1_hw_was_sch_decode_data = { 0 };
        gl1_hw_was_sch_decode_data_p = &gl1_hw_was_sch_decode_data;

        cm_acq_state_machine[sm_num].sch_decode_result =
          gl1_hw_was_sch_decoded_geran( cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.arfcn,
                                        GL1_HW_INVALID_BSIC,
                                        gl1_hw_was_sch_decode_data_p,
                                        as_id );

        cm_acq_state_machine[sm_num].id =
          gl1_hw_was_sch_decode_data_p->id;
        cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_offset =
          gl1_hw_was_sch_decode_data_p->time_offset;
        cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.schedule_error = gl1_hw_was_sch_decode_data_p->schedule_error;
        if (gl1_hw_was_sch_decode_data_p->crc_pass == 0)
        {
          cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_crc_pass = FALSE;
        }
        else
        {
          cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_crc_pass = TRUE;
        }

        COPY_SCH_DATA( &cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_data[0],
                       &gl1_hw_was_sch_decode_data_p->sch_data[0] );

        MSG_GERAN_HIGH_4_G("TDS X2G ACQ: SCH result CRC %d  arfcn %d time_offset %d GFN %d",
                            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_crc_pass,
                            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.arfcn.num,
                            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_offset,
                            current_fn  );
      }

 #ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
    /* If parallel FCCH/SCH enabled and no tone detected check for SCH */
      if ((gl1_hw_is_x2g_parallel_sch_enabled() == TRUE) &&
             (cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.tone_detect == FALSE))
      {
         gl1_hw_cm_tone_results_t  *gl1_hw_cm_tone_results_ptr;
         gl1_hw_cm_tone_results_t  gl1_hw_cm_tone_results_data = { 0 };
         gl1_hw_cm_tone_results_ptr = &gl1_hw_cm_tone_results_data;

         if( FALSE != gl1_hw_cm_was_tone_detected (cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.arfcn,
                                                   gl1_hw_cm_tone_results_ptr,
                                                   as_id))
         {
            /* Tone detected */
            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.tone_detect  = TRUE;
            cm_acq_state_machine[sm_num].tone_detect_result = TRUE;

            cm_acq_state_machine[sm_num].id = gl1_hw_cm_tone_results_ptr->id;
            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.tone_offset = gl1_hw_cm_tone_results_ptr->tone_offset;
            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.freq_offset = gl1_hw_cm_tone_results_ptr->fine_freq;
            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.snr = gl1_hw_cm_tone_results_ptr->snr;
            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_crc_pass = FALSE;


            MSG_GERAN_HIGH_3_G("TDS X2G ACQ: arfcn %d time_offset %d freq_offset %d",
                            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.arfcn.num,
                            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.tone_offset,
                            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.freq_offset);

         }
         else if (gl1_hw_cm_tone_results_ptr->sch_decoded == TRUE)
         {
            /* Parallel SCH, data already stored in gl1_hw_cm_fcch_rpt */
            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.tone_detect  = TRUE;
            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_crc_pass = TRUE;
            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.tone_offset = gl1_hw_cm_tone_results_ptr->tone_offset;
            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.freq_offset = gl1_hw_cm_tone_results_ptr->fine_freq;
            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_offset = 0;
            COPY_SCH_DATA( &cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_data[0],
                            &gl1_hw_cm_fcch_rpt.sch_data[0] );

            cm_acq_state_machine[sm_num].sch_decode_result = TRUE;
            MSG_GERAN_HIGH_4_G("TDS X2G ACQ: Parallel SCH result CRC %d  arfcn %d time_offset %d GFN %d",
                                cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_crc_pass,
                                cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.arfcn.num,
                                cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_offset,
                                current_fn  );
         }
      }
      else
      {
        if (cm_acq_state_machine[sm_num].sch_decode_result == FALSE)
        {
#endif
           if( FALSE != gl1_hw_was_x2g_tone_detected( cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.arfcn,
                                                      &cm_acq_state_machine[sm_num].id,
                                                      &cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.tone_offset,
                                                      &coarse_freq,
                                                      &fine_freq,
                                                      &afc_freq,
                                                      &cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.snr,
                                                      cm_acq_state_machine[sm_num].seq_num,
                                                      &cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.schedule_error,
                                                      gas_id)
                                                    )
           {
             ARFCN_T arfcn = cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.arfcn;

             cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.freq_offset = gl1_hw_tone_detected_total_freq_offset( arfcn, afc_freq, gas_id );

             if( FALSE == cm_acq_state_machine[sm_num].tone_detect_result )
             {
               cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.tone_detect  = TRUE;
               cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_crc_pass = FALSE;
             }

#ifdef FEATURE_GSM_TDS_DEBUG
             MSG_8(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
                   "TDS X2G ACQ: tone id %d, snr %d arfcn %d fine_freq %d afc_freq %d acq_sm %d tone_offset %d GFN %d",
                   cm_acq_state_machine[sm_num].id,
                   cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.snr,
                   cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.arfcn.num,
                   fine_freq,
                   afc_freq,
                   sm_num,
                   cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.tone_offset,
                   current_fn  );
#endif

             cm_acq_state_machine[sm_num].tone_detect_result = TRUE;
#ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
          }
        }
#endif
      }

#ifdef FEATURE_GSM_TDS_DEBUG
      MSG_GERAN_HIGH_3_G("TDS X2G ACQ Handler: cleanup_async_int_used %d cleanup: g_frame_number %d_offset %d",
               cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_async_int_used,
               cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_g_fn,
               cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_offset
            );
#endif

  /* if x2g_cleanup already issued then unistall handler at right frame*/
  if ( (cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_pending == FALSE) &&
        cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_async_int_used
      )
  {
    /*wait 3 more frame before uninstall*/
    if (cm_acq_state_machine[sm_num].sch_decode)
    {
      /*wait 4 more frame before uninstall*/
          cm_acq_state_machine[sm_num].uinstall_acq_handler_dec_frame = 7;
    }
    else
    {
      /*wait 3 more frame before uninstall*/
          cm_acq_state_machine[sm_num].uinstall_acq_handler_dec_frame = 4;
    }
    cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_async_int_used = FALSE;
  }

      if (cm_acq_state_machine[sm_num].sch_decode)    /* idle mode code */
  {
        if ( cm_acq_state_machine[sm_num].tone_detect_result && cm_acq_state_machine[sm_num].sch_decode_result)
    {
      /*Only send message after gap end otherwise TL1 will schdule another activity within gap!*/
          if (!cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_pending
    && (current_fn >= ADD_FN( cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_g_fn, 3)))
      {
            L1_send_TDS_ACQ_RSP(&cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store, as_id);

            MSG_GERAN_HIGH_3_G("TDS X2G ACQ: Tone/SCH found - stop_acq_offset_valid %d stop_acq_offset %d cleanup_offset %d",
                      cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_offset_valid,
                      cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_offset ,
                      cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_offset );

            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_rsp_sent = TRUE;
            cm_acq_state_machine[sm_num].tone_detect_result      = FALSE;

            cm_acq_state_machine[sm_num].id = 0;

            cm_acq_state_machine[sm_num].uinstall_acq_handler_dec_frame = 1;

            cm_acq_state_machine[sm_num].sch_decode        = FALSE;
            cm_acq_state_machine[sm_num].sch_decode_result = FALSE;

            gl1_hw_x2g_clear_info(cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.message_id, TONE_DETECT);
      }
    }
    else
    {
      /* handled when deregistering the handler */
      /* tone detected with no SCH decoded
       * or no tone and no sch
       */
    }
  }
      else if (cm_acq_state_machine[sm_num].tone_detect_result)   /* dedicated mode code */
  {
    /*Only send message after gap end otherwise TL1 will schdule another activity within gap!*/
        if (!cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_pending
  && (current_fn >= ADD_FN( cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_g_fn, 3)))
    {
          L1_send_TDS_ACQ_RSP(&cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store, as_id);

          MSG_GERAN_HIGH_3_G("TDS X2G ACQ: Tone found (non combined case)- ACQ_sm %d stop_acq_offset %d seq_num %d",
                sm_num,
                cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_offset ,
                cm_acq_state_machine[sm_num].seq_num );

          cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_rsp_sent = TRUE;
          cm_acq_state_machine[sm_num].tone_detect_result = FALSE;

          cm_acq_state_machine[sm_num].id = 0;

          cm_acq_state_machine[sm_num].uinstall_acq_handler_dec_frame = 1;
          cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.tone_detect = FALSE;

         gl1_hw_x2g_clear_info(cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.message_id, TONE_DETECT);
    }
  }


   if(cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_pending == TRUE)
  {

#ifdef  FEATURE_GSM_TDS_DEBUG
      MSG_GERAN_HIGH_1_G("T2G ACQ Handler: stop acq offset valid %d",
                    cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_offset_valid);
#endif

      if (cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_offset_valid)
      {
        if (cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_g_fn == current_fn)
        {
      if( cm_startup_issued )
      {
                    cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_pending = FALSE;
                    gl1_hw_stop_acquisition_geran(cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_offset,gas_id);
                    cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_rsp_sent = FALSE;
      }
      else
      {
                     send_err_msg = TRUE;
      }

                  cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_offset_valid = FALSE;


#ifdef  FEATURE_GSM_TDS_DEBUG
          MSG_GERAN_HIGH_3_G("T2G ACQ Handler: stop acq issued - stop_acq_g_fn %d offset passed %d, offset valid %d",
                        cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_g_fn,
                        cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_offset,
                        cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.stop_acq_offset_valid);
#endif
        }
      }
   }

    /* if we have an x2g_cleanup pending */
   if(cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_pending == TRUE)
   {
          if (cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_g_fn == current_fn)
      {
        if (cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_pending)
        {
          cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_pending = FALSE;
          /* If the condition that the start_up is in current frame N and issued using Asyn but the cleanup is N+2 frame with offset 0 - 625 CR365989 is detected
           * Generate Async interrupt to give the command to FW
                */
          if (cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.seperate_cleanup_async_required == TRUE)
          {
            /* Clear clean-up info pending so that no async cmd in gl1_hw_cm_cleanup */
            gl1_hw_x2g_cleanup_info.pending = FALSE;
            gl1_hw_cm_cleanup_geran(cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_offset,as_id);

            /* generate asynchronous interrupt to FW, from which FW will begin command processing */
            mdsp_async_intf_send_immediate_cmd(cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_g_fn, TRUE, gas_id);

            MSG_GERAN_HIGH_3_G("X2G: cleanup commands sent via 2nd async cleanup_g_fn=%d; current_fn = %d; current_qs = %d",
            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_g_fn,
            GSTMR_GET_FN_GERAN(gas_id),
            gstmr_rd_qsymbol_count_geran(as_id));

            cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.seperate_cleanup_async_required = FALSE;
          }
          else
          {
             /* call core x2g_cleanup processing */
             gl1_hw_cm_cleanup_geran(cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_offset,as_id);
          }
          cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.cleanup_async_int_used = TRUE;
        }

#ifdef  FEATURE_GSM_TDS_DEBUG
        MSG_GERAN_HIGH_3_G("TDS X2G ACQ Handler:  cleanup issued - GFN %d acq_frame_cnt %d cleanup_offset %d",
                  current_fn,acq_frame_cnt,cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_offset);
#endif

        if (cm_acq_state_machine[sm_num].sch_decode)
        {
          /*wait 4 more frame before uninstall*/
          cm_acq_state_machine[sm_num].uinstall_acq_handler_dec_frame = 7;
        }
        else
        {
          /*wait 3 more frame before uninstall*/
          cm_acq_state_machine[sm_num].uinstall_acq_handler_dec_frame = 4;
        }
        cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_async_int_used = FALSE;
      }
    }

#ifdef  FEATURE_GSM_TDS_DEBUG
  MSG_3(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "TDS X2G ACQ Handler: cleanup offset %d uinstall_acq_handler_dec_frame %d cleanup_async_int_used %d",
               cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_offset,
               cm_acq_state_machine[sm_num].uinstall_acq_handler_dec_frame,
               cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_async_int_used);
#endif

  cm_acq_state_machine[sm_num].uinstall_acq_handler_dec_frame--;

  /* acq_frame_cnt - 15 frames + upto 4 frames polling afterwards+ 1 for margin - should really be 17.*/
  if( cm_acq_state_machine[sm_num].uinstall_acq_handler_dec_frame == 0 )
  {

    /* deregister the handler when all the state machines are inactive
     * check outside of the loop
     */
    //  gstmr_deregister_handler_geran(gl1_hw_x2g_acq_ft_handler);

    cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_async_int_used = FALSE;
    cm_acq_state_machine[sm_num].uinstall_acq_handler_dec_frame             = ACQ_HANDLER_TIMEOUT_FRAMES;

    /* If message hasn't been sent send it now */
    if (!cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_rsp_sent)
    {
      /* Add check that this is not stale acquisition before sending result to TL1 */
      if ((SUB_FN(current_fn,cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_g_fn)) > (ACQ_HANDLER_TIMEOUT_FRAMES + 1))
      {
         MSG_GERAN_ERROR_3_G( "TDS X2G ACQ Handler: Too late, TDS ACQ RSP message not sent fn %d cleanup fn %d sm %d",
                current_fn, cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_info.cleanup_g_fn, sm_num);
      }
      else
      {
      L1_send_TDS_ACQ_RSP(&cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store, as_id);

            MSG_GERAN_HIGH_3_G( "TDS X2G ACQ Handler: Late TDS ACQ RSP message sent- GFN %d sch_crc_pass %d ACQ_sm %d",
                   current_fn, cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_crc_pass, sm_num);
      }
      cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_rsp_sent = TRUE;

      gl1_hw_x2g_clear_info(cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.message_id, TONE_DETECT);
      /* Stop acquistion is taken care of by TL1 so need for it here*/
    }

    /* reset flag */

    cm_acq_state_machine[sm_num].tone_detect_result = FALSE;

    cm_acq_state_machine[sm_num].sch_decode_result = FALSE;
    cm_acq_state_machine[sm_num].id = 0;

    cm_acq_state_machine[sm_num].sch_decode                        = FALSE;
    cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.tone_detect  = FALSE;
    cm_acq_state_machine[sm_num].gl1_hw_x2g_acq_store.sch_crc_pass = 0;

    cm_acq_state_machine[sm_num].acq_meas_state = ACQ_MEAS_STATE_INACTIVE;

#ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
    /* Remove sm_num from the the array, move other sm_num up in array */
    cm_acq_state_machine_num_active -= 1;

    if (cm_acq_state_machine_num_active != 0)
    {
      uint8  j=0;

      if (cm_acq_state_machine_num_active >= ACQ_MEAS_NUM_SM)
      {
         cm_acq_state_machine_num_active = (ACQ_MEAS_NUM_SM - 1);
         MSG_GERAN_ERROR_0_G("X2G too many acq SM active");
      }

      for (i=0;i<cm_acq_state_machine_num_active;i++)
      {
        if (cm_acq_state_machine_active_array[i] ==  sm_num)
        {
           j=1;
        }
        cm_acq_state_machine_active_array[i] = cm_acq_state_machine_active_array[i+j];
      }
    }
    cm_acq_state_machine_active_array[cm_acq_state_machine_num_active] = 0xFF;
#endif
  }

  } /* if( cm_acq_state_machine[sm_num].acq_meas_state != ACQ_MEAS_STATE_INACTIVE ) */

  }  /* for ( sm_loop ) */

#ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
  if ( cm_acq_state_machine_num_active == 0)
  {
      gstmr_deregister_handler_geran(gl1_hw_x2g_acq_ft_handler,as_id);
      MSG_GERAN_HIGH_1_G("ACQ handler removed GFN %d",current_fn);
  }
#else
  for( i=0;i<ACQ_MEAS_NUM_SM;i++)
  {
    if( cm_acq_state_machine[i].acq_meas_state != ACQ_MEAS_STATE_INACTIVE)
    {
      break;
    }
    if( i == (ACQ_MEAS_NUM_SM -1) )
    {
      gstmr_deregister_handler_geran(gl1_hw_x2g_acq_ft_handler,as_id);
      MSG_GERAN_HIGH_1_G("ACQ handler removed GFN %d",current_fn);
    }
  }
#endif
}


/*===========================================================================

FUNCTION  gl1_hw_x2g_sch_ft_handler

DESCRIPTION
  GSTMR handler function which polls for SB result & sends results message
  back to TDS.
  Once complete, deregister the handler.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_x2g_sch_ft_handler(sys_modem_as_id_e_type      as_id)
{
  uint8  sm_num = 0, i;
#ifdef GERAN_L1_HLLL_LNASTATE
  gl1_defs_metrics_lnastate gl1_metrics_lnastate;
  uint8 SawlessHLLLState[MAX_NUMBER_OF_CHANNELS] = {0};
#endif

  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  while( sm_num < SCH_MEAS_NUM_SM )
  {
    if( sch_state_machine[sm_num].sch_meas_state == SCH_MEAS_STATE_ACTIVE )
    {

      if( FALSE == sch_state_machine[sm_num].async_sch_decode_result )
  {
        sch_state_machine[sm_num].async_sch_decode_result =

          gl1_hw_x2g_get_async_rx_sch_data( &sch_state_machine[sm_num].sch_decode_results.sch.hdr,
                                            sch_state_machine[sm_num].sch_decode_results.sch.data,
                                            (uint16)MAX_SCH_OCTETS,
                                            ((sch_state_machine[sm_num].uinstall_sch_handler_dec_frame == 1)?TRUE:FALSE),
                                            sch_state_machine[sm_num].sch_data_buffer_idx,
                                            sch_state_machine[sm_num].sch_metrics_seq_num,
                                            as_id
                                           );

  if( sch_state_machine[sm_num].async_sch_decode_result )
  {
          if( sch_state_machine[sm_num].sch_decode_results.sch.hdr.good_data == 2 )
  {
            boolean  metrics_valid = gl1_hw_x2g_get_async_sch_rx_metrics( &sch_state_machine[sm_num].sch_metrics,
                                                                          TRUE,
                                                                          &(stored_tags),
                                                                          sch_state_machine[sm_num].sch_metrics_buffer_idx,
                                                                          sch_state_machine[sm_num].sch_metrics_seq_num,
                                                                          sch_state_machine[sm_num].gl1_hw_x2g_sch_store.arfcn,
                                                                          as_id
                                                                          );
     /*Populate schedule error status in sch_store */
          sch_state_machine[sm_num].gl1_hw_x2g_sch_store.schedule_error = sch_state_machine[sm_num].sch_decode_results.sch.hdr.schedule_error ;
      if( metrics_valid )
      {
    /*Check for large offsets and invalidate results if present */
    if(  ( sch_state_machine[sm_num].sch_metrics.timing_offset < (-(GL1_HW_X2G_TDS_SCH_SEARCH_WIDTH_QS/2)) )
       ||( sch_state_machine[sm_num].sch_metrics.timing_offset > (GL1_HW_X2G_TDS_SCH_SEARCH_WIDTH_QS/2)  )
    )
      {
            if( sch_state_machine[sm_num].sch_decode_results.sch.hdr.good_data == 2 )
    {
      MSG_GERAN_ERROR_2_G("TDS X2G SCH Handler: reset CRC result due to large sch time_offset %d sch_decode_result %d",
                        sch_state_machine[sm_num].sch_metrics.timing_offset,
                        sch_state_machine[sm_num].async_sch_decode_result);

              sch_state_machine[sm_num].sch_decode_results.sch.hdr.good_data = 0;
    }
    }
  }
    else
    {
        sch_state_machine[sm_num].sch_decode_results.sch.hdr.good_data = 0;
        sch_state_machine[sm_num].sch_metrics.timing_offset = 0;
  }
#ifdef GERAN_L1_HLLL_LNASTATE
     gl1_metrics_lnastate.NoOfChannels = 0;
     if(gl1_hw_get_sawless_HLLL_support(sch_state_machine[sm_num].gl1_hw_x2g_sch_store.arfcn.band,gas_id))
      {
       gl1_metrics_lnastate.NoOfChannels              = 1;
       gl1_metrics_lnastate.gsm_metrics[0].arfcn      = sch_state_machine[sm_num].gl1_hw_x2g_sch_store.arfcn;
       gl1_metrics_lnastate.gsm_metrics[0].rx_lvl_dbm = sch_state_machine[sm_num].sch_metrics.pwr_dBm_x16;
       gl1_metrics_lnastate.gsm_metrics[0].snr        = sch_state_machine[sm_num].sch_metrics.snr;
       gl1_metrics_lnastate.gsm_metrics[0].jdet_value = sch_state_machine[sm_num].sch_metrics.jdet_value;
       SawlessHLLLState[0] = sch_state_machine[sm_num].sch_metrics.SawlessHLLLState;
       gl1_hw_rf_burst_metrics(&gl1_metrics_lnastate,gas_id);
      }
     if(gl1_metrics_lnastate.NoOfChannels)
      {
      gl1_hw_log_HLin_LLin_metrics(&gl1_metrics_lnastate,SawlessHLLLState,gas_id);
      }
#endif
  }
   }

  }  /* if( FALSE == async_sch_decode_result ) */

      if ( ( sch_state_machine[sm_num].gl1_hw_x2g_sch_info.cleanup_pending == FALSE ) &&
           sch_state_machine[sm_num].gl1_hw_x2g_sch_info.cleanup_async_int_used)
  {
    /*wait 3 more frame before uninstall*/
        sch_state_machine[sm_num].uinstall_sch_handler_dec_frame             = 4;
        sch_state_machine[sm_num].gl1_hw_x2g_sch_info.cleanup_async_int_used = FALSE;
  }


      if( sch_state_machine[sm_num].async_sch_decode_result )
  {
#ifdef FEATURE_GSM_TDS_DEBUG
    MSG_GERAN_HIGH_2_G("TDS X2G SCH Handler: crc_pass %d, sch time_offset %d",
                 sch_state_machine[sm_num].sch_decode_results.sch.hdr.good_data ,sch_state_machine[sm_num].sch_metrics.timing_offset);
#endif

      if( FALSE == sch_state_machine[sm_num].gl1_hw_x2g_sch_rsp_sent )
  {
          if(  sch_state_machine[sm_num].timing_offset < (GL1_HW_CM_ASYNC_RX_CMD_DURATION_QS/2) )
      {
            sch_state_machine[sm_num].gl1_hw_x2g_sch_store.sch_frame = SUB_FN( sch_state_machine[sm_num].gl1_hw_x2g_sch_store.sch_frame, 1);
      }
      else if( sch_state_machine[sm_num].timing_offset > ((GL1_HW_CM_ASYNC_RX_CMD_DURATION_QS/2)+5000) )
      {
            sch_state_machine[sm_num].gl1_hw_x2g_sch_store.sch_frame = ADD_FN( sch_state_machine[sm_num].gl1_hw_x2g_sch_store.sch_frame, 1);
      }

          L1_send_TDS_SCH_MEAS_RSP( &sch_state_machine[sm_num].gl1_hw_x2g_sch_store,
                        &sch_state_machine[sm_num].sch_decode_results.sch.hdr,
                                    sch_state_machine[sm_num].sch_decode_results.sch.data,
                                    &sch_state_machine[sm_num].sch_metrics,
                                    as_id );

          sch_state_machine[sm_num].gl1_hw_x2g_sch_rsp_sent        = TRUE;

          gl1_hw_x2g_clear_info(sch_state_machine[sm_num].gl1_hw_x2g_sch_store.message_id, SCH_DETECT);
      }

      sch_state_machine[sm_num].uinstall_sch_handler_dec_frame = 1;
  }
  else
  {
#ifdef FEATURE_GSM_TDS_DEBUG
    MSG_4(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
          "TDS X2G SCH Handler: sch decode %d cleanup_pending %d GFN %d qs %d",
              sch_state_machine[sm_num].async_sch_decode_result,sch_state_machine[sm_num].gl1_hw_x2g_sch_info.cleanup_pending, GSTMR_GET_FN_GERAN(), gstmr_rd_qsymbol_count(as_id));
#endif
  }


  /* if we have an x2g_cleanup pending */
  if((sch_state_machine[sm_num].gl1_hw_x2g_sch_info.cleanup_pending == TRUE)
     && (sch_state_machine[sm_num].gl1_hw_x2g_sch_info.cleanup_g_fn == GSTMR_GET_FN_GERAN(gas_id)))
  {
      sch_state_machine[sm_num].gl1_hw_x2g_sch_info.cleanup_pending = FALSE;
      /* call core x2g_cleanup processing */
      gl1_hw_cm_cleanup_geran( sch_state_machine[sm_num].gl1_hw_x2g_sch_info.cleanup_offset,as_id );
      sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.cleanup_async_int_used = TRUE;

#ifdef FEATURE_GSM_TDS_DEBUG
      MSG_GERAN_HIGH_3_G("TDS X2G SCH: GFN %d sch decode %d cleanup_offset %d",
                   GSTMR_GET_FN_GERAN(gas_id),
           sch_state_machine[sm_num].async_sch_decode_result,
           sch_state_machine[sm_num].gl1_hw_x2g_sch_info.cleanup_offset);
#endif
  }  /* if(gl1_hw_x2g_sch_info.cleanup_pending == TRUE) */


  /* GFW needs 3 slots from end of the acq to process results
   * if end of ACQ is within 3 slots of frame tick, then unistall
   * handler at next frame tick to ensure result is available.
   * Since cleanup ALREADY issued then we may be able to unistall
   * handler
   */
#ifdef FEATURE_GSM_TDS_DEBUG
  MSG_GERAN_HIGH_3_G("TDS X2G SCH: cleanup_async_int_used %d cleanup_offset %d uinstall_sch_handler_dec_frame %d",
               sch_state_machine[sm_num].gl1_hw_x2g_sch_info.cleanup_async_int_used,
               sch_state_machine[sm_num].gl1_hw_x2g_sch_info.cleanup_offset,
               sch_state_machine[sm_num].uinstall_sch_handler_dec_frame);
#endif

      sch_state_machine[sm_num].uinstall_sch_handler_dec_frame--;

      if (sch_state_machine[sm_num].uinstall_sch_handler_dec_frame == 0)
  {

    /* deregister the handler */
        // gstmr_deregister_handler_geran( gl1_hw_x2g_sch_ft_handler );

        sch_state_machine[sm_num].gl1_hw_x2g_sch_info.cleanup_async_int_used = FALSE;

#ifdef  FEATURE_GSM_TDS_DEBUG
    MSG_GERAN_HIGH_3_G("T2G SCH HANDLER: uninstall handler -  crc_pass %d, sch time_offset %d",
                 sch_state_machine[sm_num].sch_decode_results.sch.hdr.good_data ,
         sch_state_machine[sm_num].sch_metrics.timing_offset,
         0 );
#endif

        sch_state_machine[sm_num].uinstall_sch_handler_dec_frame = 8;

    /* If message hasn't been sent send it now */
        if (!sch_state_machine[sm_num].gl1_hw_x2g_sch_rsp_sent)
    {
          if(  sch_state_machine[sm_num].timing_offset < (GL1_HW_CM_ASYNC_RX_CMD_DURATION_QS/2) )
    {
            sch_state_machine[sm_num].gl1_hw_x2g_sch_store.sch_frame = SUB_FN( sch_state_machine[sm_num].gl1_hw_x2g_sch_store.sch_frame, 1);
      }
      else if( sch_state_machine[sm_num].timing_offset > ((GL1_HW_CM_ASYNC_RX_CMD_DURATION_QS/2)+5000) )
      {
            sch_state_machine[sm_num].gl1_hw_x2g_sch_store.sch_frame = ADD_FN( sch_state_machine[sm_num].gl1_hw_x2g_sch_store.sch_frame, 1);
    }

          L1_send_TDS_SCH_MEAS_RSP( &sch_state_machine[sm_num].gl1_hw_x2g_sch_store,
                        &sch_state_machine[sm_num].sch_decode_results.sch.hdr,
                                    sch_state_machine[sm_num].sch_decode_results.sch.data,
                                    &sch_state_machine[sm_num].sch_metrics,
                                    as_id );

          sch_state_machine[sm_num].gl1_hw_x2g_sch_rsp_sent              = TRUE;

          gl1_hw_x2g_clear_info(sch_state_machine[sm_num].gl1_hw_x2g_sch_store.message_id, SCH_DETECT);
    }

        sch_state_machine[sm_num].async_sch_decode_result                = FALSE;
      sch_state_machine[sm_num].sch_decode_results.sch.hdr.good_data = FALSE;

    sch_state_machine[sm_num].sch_meas_state = SCH_MEAS_STATE_INACTIVE;

  } /* if (uinstall_sch_handler_dec_frame == 0) */


    } /* if state machine is active */

    sm_num++;
  }

  for( i=0;i<SCH_MEAS_NUM_SM;i++)
  {
    if( sch_state_machine[i].sch_meas_state != SCH_MEAS_STATE_INACTIVE)
    {
      break;
    }
    if( i == (SCH_MEAS_NUM_SM -1) )
    {
      gstmr_deregister_handler_geran( gl1_hw_x2g_sch_ft_handler,as_id );
    }
  }
}

void gl1_hw_x2g_margin_qs_histogram_bin_selection(uint16 startup_send_cmd_offset){
uint8 bin_type_index = 0;
uint16 early_margin_bin_qs = 0;
uint16 late_margin_bin_qs = 0;

static uint8 counter;
     counter++;

     if (startup_qs < (startup_send_cmd_offset - STARTUP_EARLY_MARGIN_QS)) {
        gl1_gfw_cmd_action_margin_qs_bin[_STARTUP][EARLY_BIN]++;
     }
     else if ((startup_qs >= (startup_send_cmd_offset - STARTUP_EARLY_MARGIN_QS)) && \
                 (startup_qs < (startup_send_cmd_offset - STARTUP_LATE_MARGIN_QS))) {
           gl1_gfw_cmd_action_margin_qs_bin[_STARTUP][MIDDLE_BIN]++;
     }
     else
        gl1_gfw_cmd_action_margin_qs_bin[_STARTUP][LATE_BIN]++;

    switch (gl1_t2g_last_state){
    case NONE_GL1:
    case STARTUP:
    case PWR_MEAS:
       switch(num_of_pwr_measures){
       case PWR_MEAS_1:
          bin_type_index = PWR_MEAS_1;
          early_margin_bin_qs = PWR_MEAS_1_EARLY_MARGIN_QS;
          late_margin_bin_qs = PWR_MEAS_1_LATE_MARGIN_QS;
          break;
       case PWR_MEAS_2:
          bin_type_index = PWR_MEAS_2;
          early_margin_bin_qs = PWR_MEAS_2_EARLY_MARGIN_QS;
          late_margin_bin_qs = PWR_MEAS_2_LATE_MARGIN_QS;
          break;
       case PWR_MEAS_3:
          bin_type_index = PWR_MEAS_3;
          early_margin_bin_qs = PWR_MEAS_3_EARLY_MARGIN_QS;
          late_margin_bin_qs = PWR_MEAS_3_LATE_MARGIN_QS;
          break;
       case PWR_MEAS_4:
          bin_type_index = PWR_MEAS_4;
          early_margin_bin_qs = PWR_MEAS_4_EARLY_MARGIN_QS;
          late_margin_bin_qs = PWR_MEAS_4_LATE_MARGIN_QS;
          break;
       case PWR_MEAS_5:
          bin_type_index = PWR_MEAS_5;
          early_margin_bin_qs = PWR_MEAS_5_EARLY_MARGIN_QS;
          late_margin_bin_qs = PWR_MEAS_5_LATE_MARGIN_QS;
          break;
       case PWR_MEAS_6:
          bin_type_index = PWR_MEAS_6;
          early_margin_bin_qs = PWR_MEAS_6_EARLY_MARGIN_QS;
          late_margin_bin_qs = PWR_MEAS_6_LATE_MARGIN_QS;
          break;
       case PWR_MEAS_7:
          bin_type_index = PWR_MEAS_7;
          early_margin_bin_qs = PWR_MEAS_7_EARLY_MARGIN_QS;
          late_margin_bin_qs = PWR_MEAS_7_LATE_MARGIN_QS;
          break;
       case PWR_MEAS_8:
          bin_type_index = PWR_MEAS_8;
          early_margin_bin_qs = PWR_MEAS_8_EARLY_MARGIN_QS;
          late_margin_bin_qs = PWR_MEAS_8_LATE_MARGIN_QS;
          break;
       default:
          break;
    }
       break;
    case SCH_DETECT:
       bin_type_index = _SCH_DETECT;
       early_margin_bin_qs = SCH_DETECT_EARLY_MARGIN_QS;
       late_margin_bin_qs = SCH_DETECT_LATE_MARGIN_QS;
       break;

    case TONE_DETECT:
       bin_type_index = _TONE_DETECT;
       early_margin_bin_qs = TONE_DETECT_EARLY_MARGIN_QS;
       late_margin_bin_qs = TONE_DETECT_LATE_MARGIN_QS;
       break;
    default:
       break;
    }//end of switch

    if (pwr_sch_tone_meas_qs < (startup_send_cmd_offset - early_margin_bin_qs)) {
       gl1_gfw_cmd_action_margin_qs_bin[bin_type_index][EARLY_BIN]++;
    }
    else if ((pwr_sch_tone_meas_qs >= (startup_send_cmd_offset - early_margin_bin_qs)) && \
             (pwr_sch_tone_meas_qs < (startup_send_cmd_offset - late_margin_bin_qs))) {
       gl1_gfw_cmd_action_margin_qs_bin[bin_type_index][MIDDLE_BIN]++;
    }
    else
       gl1_gfw_cmd_action_margin_qs_bin[bin_type_index][LATE_BIN]++;

    if (counter == LIMIT_MARGIN_QS_BIN_COUNTER) {
 #ifdef  FEATURE_GSM_TDS_DEBUG
      //Print all margin_QS values and reset the margin_QS array
       MSG_GERAN_HIGH_3("Variation in quarter symbol values during startup phase - %d are early, %d are correct and %d are late\n", gl1_gfw_cmd_action_margin_qs_bin[_STARTUP][EARLY_BIN],\
                        gl1_gfw_cmd_action_margin_qs_bin[_STARTUP][MIDDLE_BIN], gl1_gfw_cmd_action_margin_qs_bin[_STARTUP][LATE_BIN]);
       MSG_GERAN_HIGH_3("Variation in quarter symbol values during 1 power measure phase  - %d are early, %d are correct and %d are late\n", gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_1][EARLY_BIN],\
                        gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_1][MIDDLE_BIN], gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_1][LATE_BIN]);
       MSG_GERAN_HIGH_3("Variation in quarter symbol values during 1 power measure phase - %d are early, %d are correct and %d are late\n", gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_2][EARLY_BIN],\
                        gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_2][MIDDLE_BIN], gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_2][LATE_BIN]);
       MSG_GERAN_HIGH_3("Variation in quarter symbol values during 1 power measure phase - %d are early, %d are correct and %d are late\n", gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_3][EARLY_BIN],\
                        gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_3][MIDDLE_BIN], gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_3][LATE_BIN]);
       MSG_GERAN_HIGH_3("Variation in quarter symbol values during 1 power measure phase - %d are early, %d are correct and %d are late\n", gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_3][EARLY_BIN],\
                        gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_4][MIDDLE_BIN], gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_4][LATE_BIN]);
       MSG_GERAN_HIGH_3("Variation in quarter symbol values during 1 power measure phase - %d are early, %d are correct and %d are late\n", gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_3][EARLY_BIN],\
                        gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_5][MIDDLE_BIN], gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_5][LATE_BIN]);
       MSG_GERAN_HIGH_3("Variation in quarter symbol values during 1 power measure phase - %d are early, %d are correct and %d are late\n", gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_3][EARLY_BIN],\
                       gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_6][MIDDLE_BIN], gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_6][LATE_BIN]);
       MSG_GERAN_HIGH_3("Variation in quarter symbol values during 1 power measure phase - %d are early, %d are correct and %d are late\n", gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_3][EARLY_BIN],\
                        gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_7][MIDDLE_BIN], gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_7][LATE_BIN]);
       MSG_GERAN_HIGH_3("Variation in quarter symbol values during 1 power measure phase - %d are early, %d are correct and %d are late\n", gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_3][EARLY_BIN],\
                        gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_8][MIDDLE_BIN], gl1_gfw_cmd_action_margin_qs_bin[PWR_MEAS_8][LATE_BIN]);
       MSG_GERAN_HIGH_3("Variation in quarter symbol values during Tone Detect - %d are early, %d are correct and %d are late\n", gl1_gfw_cmd_action_margin_qs_bin[_TONE_DETECT][EARLY_BIN],\
                        gl1_gfw_cmd_action_margin_qs_bin[_SCH_DETECT][MIDDLE_BIN], gl1_gfw_cmd_action_margin_qs_bin[_SCH_DETECT][LATE_BIN]);
       MSG_GERAN_HIGH_3("Variation in quarter symbol values during SCH Detect phase - %d are early, %d are correct and %d are late\n", gl1_gfw_cmd_action_margin_qs_bin[_SCH_DETECT][EARLY_BIN],\
                        gl1_gfw_cmd_action_margin_qs_bin[_TONE_DETECT][MIDDLE_BIN], gl1_gfw_cmd_action_margin_qs_bin[_TONE_DETECT][LATE_BIN]);
       MSG_GERAN_HIGH_3("Variation in quarter symbol values during Cleanup phase - %d are early, %d are correct and %d are late\n", gl1_gfw_cmd_action_margin_qs_bin[_CLEANUP][EARLY_BIN],\
                        gl1_gfw_cmd_action_margin_qs_bin[_CLEANUP][MIDDLE_BIN], gl1_gfw_cmd_action_margin_qs_bin[_CLEANUP][LATE_BIN]);
#endif
       memset(gl1_gfw_cmd_action_margin_qs_bin, 0 , X2G_TRANSITION_PREPARATORY_PHASES * NO_OF_BINS_PER_PHASE * sizeof(uint8));

       startup_qs = 0;
       cleanup_qs = 0;
       pwr_sch_tone_meas_qs = 0;
       counter = 0;

    }

  }



/*===========================================================================

FUNCTION  gl1_hw_x2g_send_commands

DESCRIPTION
  Sends the first batch (or all) x2g commands to MDSP, either via an asynchronous
  interrupt or via the next frame tick.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_x2g_send_commands(gas_id_t gas_id)
{
  gsmtr_fn_qs_type    current_fn_qs;
  uint32 fn_diff, i;
  gl1_hw_x2g_info_t   send_cmds;
  sys_modem_as_id_e_type      as_id;

  if( send_err_msg )
  {
    return;
  }

  as_id = geran_map_gas_id_to_nas_id(gas_id);


#ifdef  FEATURE_GSM_TDS_DEBUG
  MSG_GERAN_HIGH_1_G("X2G send X2G cmds: last_state %d",gl1_t2g_last_state);
#endif

  if(gl1_t2g_last_state == PWR_MEAS)
  {
    send_cmds.startup_g_fn = gl1_hw_x2g_pwr_meas_info.startup_g_fn;
    send_cmds.startup_offset = gl1_hw_x2g_pwr_meas_info.startup_offset;
  }
  else if (gl1_t2g_last_state == TONE_DETECT)
  {
    send_cmds.startup_g_fn = cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.startup_g_fn;
    send_cmds.startup_offset = cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.startup_offset;
  }
  else if (gl1_t2g_last_state == SCH_DETECT )
  {
    send_cmds.startup_g_fn = sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.startup_g_fn;
    send_cmds.startup_offset = sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.startup_offset;
  }
  else
  {
    send_cmds.startup_g_fn = gl1_hw_x2g_startup_info.g_fn;
    send_cmds.startup_offset = gl1_hw_x2g_startup_info.offset;
#ifdef FEATURE_GSM_TDS_DEBUG
        MSG_GERAN_MED_0_G("TDS X2G send_cmds - send_cmds_ptr pointer using gl1_hw_x2g_startup_info");
#endif
  }

  /* read current FN, QS */
  current_fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);

#ifdef  FEATURE_GSM_TDS_DEBUG
  if ( (current_fn_qs.fn) != GSTMR_GET_FN_GERAN(gas_id) )
  {
    MSG_GERAN_MED_2_G("X2G send X2G cmds: HW/SW frame number mismatch sw_fn =%d, hw_fn=%d",
             GSTMR_GET_FN_GERAN(gas_id), current_fn_qs.fn);
  }
#endif

 fn_diff =SUB_FN(gl1_hw_x2g_pwr_meas_info.startup_g_fn2,send_cmds.startup_g_fn);
  if(fn_diff>1)
  {
    fn_diff=1;
  }

  /* if current FN matches x2g_startup reference FN, commands MUST go in Async int
     else FN should match on next frame, so allow commands to go out on next FT */
  if(current_fn_qs.fn == send_cmds.startup_g_fn)
  {
   if ( ((current_fn_qs.qs >= ((send_cmds.startup_offset) - GL1_HW_CM_GFW_CMD_ACTION_MARGIN_QS))&&((  gl1_t2g_last_state   != PWR_MEAS)||((  gl1_t2g_last_state   == PWR_MEAS)&&(num_pwr_meas==1)))) ||
      ((current_fn_qs.qs >= ((QS_PER_FRAME*fn_diff+gl1_hw_x2g_pwr_meas_info.startup_offset2) - GL1_HW_CM_GFW_CMD_ACTION_MARGIN_QS))&&(  gl1_t2g_last_state   == PWR_MEAS) ))
    {
      gl1_gfw_cmd_action_margin_qs_bin[_CLEANUP][LATE_BIN]++;
      MSG_GERAN_ERROR_3_G("X2G: Too late in startup frame: startup_g_fn = %d; x2g_startup_offset = %d; margin qs %d",
               send_cmds.startup_g_fn,
               send_cmds.startup_offset,
               (send_cmds.startup_offset - current_fn_qs.qs) );

     if(gl1_t2g_last_state ==PWR_MEAS )
     {
     if(num_pwr_meas==1)
       {
      MSG_GERAN_ERROR_1_G("X2G: Number of power measurement cmd =%d",num_pwr_meas);
       }
     else
       {
       MSG_GERAN_ERROR_3_G("X2G: 2nd startup offset too late: startup_g_fn2 = %d; x2g_startup_offset2 = %d; current qs %d",
             gl1_hw_x2g_pwr_meas_info.startup_g_fn2,
             gl1_hw_x2g_pwr_meas_info.startup_offset2,
              current_fn_qs.qs );
       }
     }
      /*indicate err in  msg to TL1, will be dealt with in gl1_hw_x2g_cleanup */
      send_err_msg = TRUE;
    }
    else
    {
    if (  (current_fn_qs.qs >= ((send_cmds.startup_offset) - GL1_HW_CM_GFW_CMD_ACTION_MARGIN_QS)) && (  gl1_t2g_last_state   == PWR_MEAS)&&(num_pwr_meas>1)  )
         {
      MSG_GERAN_ERROR_3_G("X2G: Too late in startup frame: startup_g_fn = %d; x2g_startup_offset = %d; current qs %d",
               send_cmds.startup_g_fn,
               send_cmds.startup_offset,
               current_fn_qs.qs );
      send_cmds.startup_g_fn = gl1_hw_x2g_pwr_meas_info.startup_g_fn2;
            send_cmds.startup_offset = gl1_hw_x2g_pwr_meas_info.startup_offset2;

      MSG_GERAN_HIGH_3_G("X2G: Delaying X2G startup to: new startup_g_fn = %d; new x2g_startup_offset = %d; current qs %d",
               send_cmds.startup_g_fn,
               send_cmds.startup_offset,
                current_fn_qs.qs );

      gfwhost_cmdbuf_offsets[gas_id].startup_ptr->time_offset  =send_cmds.startup_offset;
      gfwhost_cmdbuf_offsets[gas_id].pwrmsr_ptr->skipLastBurst =TRUE;
      cm_pwr_meas_state_machine[cm_pwr_sm].rssi_meas_result.gl1_skip_last_burst=TRUE;
  

       if(fn_diff==1)
        {
        for(i=0;i< num_pwr_meas;i++)
              {
             gfwhost_cmdbuf_offsets[gas_id].pwrmsr_ptr->ctrl[i].offset -=QS_PER_FRAME;
               }
        gfwhost_cmdbuf_offsets[gas_id].cleanup_ptr->time_offset -=QS_PER_FRAME;
        }

#ifdef FEATURE_GSM_TDS_DEBUG2
          MSG_GERAN_HIGH_1_G("Skipfirstburst flag = %d",gfwhost_cmdbuf_offsets[gas_id].pwrmsr_ptr->skipFirstBurst);
#endif
          }
      /* generate asynchronous interrupt to FW, from which FW will begin command processing */
      mdsp_async_intf_send_immediate_cmd(send_cmds.startup_g_fn, TRUE, gas_id);

    gl1_hw_x2g_startup_info.async_int_executed = TRUE;

      MSG_GERAN_HIGH_3_G("X2G: commands sent via async int: startup_g_fn=%d; x2g_startup_offset = %d; current_qs = %d",
               send_cmds.startup_g_fn,
               send_cmds.startup_offset,
               gstmr_rd_qsymbol_count_geran(as_id));

      gl1_profiling_t2g_api(t2g_api_profiling_index, CLEANUP, FALSE,gas_id);

      gl1_profiling_calulate_t2g_api_runtime(t2g_api_profiling_index);

     if ((current_fn_qs.qs < (send_cmds.startup_offset - CLEANUP_LATE_MARGIN_QS)) && (current_fn_qs.qs > CLEANUP_EARLY_MARGIN_QS))
     {
          gl1_gfw_cmd_action_margin_qs_bin[_CLEANUP][MIDDLE_BIN]++;
     }
     else if (current_fn_qs.qs < CLEANUP_EARLY_MARGIN_QS )
     {
          gl1_gfw_cmd_action_margin_qs_bin[_CLEANUP][EARLY_BIN]++;
  }
  }
      gl1_hw_x2g_margin_qs_histogram_bin_selection(send_cmds.startup_offset);
  }
  /* else, commands can go out on next FT if for next frame */
  else
  {
    if ( send_cmds.startup_g_fn == ADD_FN(current_fn_qs.fn,1))
    {
      MSG_GERAN_HIGH_3_G("X2G: commands sent for next frame: startup_g_fn = %d; x2g_startup_offset = %d; current_fn = %d",
               send_cmds.startup_g_fn,
               send_cmds.startup_offset,
               current_fn_qs.fn);

      mdsp_async_intf_send_immediate_cmd(send_cmds.startup_g_fn, TRUE, gas_id);

    }
    else
    {
      /*indicate err in  msg to TL1, will be dealt with in gl1_hw_x2g_cleanup */
      send_err_msg = TRUE;

      MSG_GERAN_ERROR_3_G("X2G: Missed startup frame: startup_g_fn = %d; x2g_startup_offset = %d; current_fn = %d",
               send_cmds.startup_g_fn,
               send_cmds.startup_offset,
               current_fn_qs.fn);
    }
  }
}
#endif


/*===========================================================================

FUNCTION  gl1_hw_cm_get_power_measure_results

DESCRIPTION
  Returns 9 rssi values.

DEPENDENCIES
  gl1_hw_cm_power_measure must have been called two frames previously

RETURN VALUE
  samples       - array of 9 measured powers (received signal strength)
  num           - number of results to process. This number should match
                  the num parameter used in calling the
                  gl1_hw_cm_power_measure() function.
  tags          - Pointers to where the gain will be stored

  (It is the responsiblity of the calling function to provide storage for
   results, regardless of how many bursts were recieved.  It is also
   the responsibility of the calling function to know how many bursts were
   received.)

  TRUE  - power results were available
  FALSE - power results were not available

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_cm_get_power_measure_results
(
   gl1_hw_measurement_T  samples[],
   uint8                 num,             /* Number of results to process  */
   gl1_hw_rx_tags_type   tags[]
#ifdef FEATURE_DUAL_SIM
   ,sys_modem_as_id_e_type      as_id
#endif
)
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/
   return gl1_hw_cm_get_power_measure_results_geran(samples,num,tags,as_id);
}
/*===========================================================================

FUNCTION  gl1_hw_cm_get_power_measure_results_geran

DESCRIPTION
  Returns 9 rssi values.

DEPENDENCIES
  gl1_hw_cm_power_measure must have been called two frames previously

RETURN VALUE
  samples       - array of 9 measured powers (received signal strength)
  num           - number of results to process. This number should match
                  the num parameter used in calling the
                  gl1_hw_cm_power_measure() function.
  tags          - Pointers to where the gain will be stored

  (It is the responsiblity of the calling function to provide storage for
   results, regardless of how many bursts were recieved.  It is also
   the responsibility of the calling function to know how many bursts were
   received.)

  TRUE  - power results were available
  FALSE - power results were not available

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_cm_get_power_measure_results_geran
(
   gl1_hw_measurement_T  samples[],
   uint8                 num,             /* Number of results to process  */
   gl1_hw_rx_tags_type   tags[],
   sys_modem_as_id_e_type      as_id
)
{
  boolean status;
  static uint32   rssi_values[MAX_CM_RSSI_MEASUREMENTS] = {0};
  static boolean  yield[MAX_CM_RSSI_MEASUREMENTS] = {FALSE};
  static boolean ScheduleError[MAX_CM_RSSI_MEASUREMENTS] = {FALSE};
  uint8  i;
#ifdef GERAN_L1_HLLL_LNASTATE
  uint16 jdetvalues[MAX_NUMBER_OF_CHANNELS] = {0};
  uint8 SawlessHLLLState[MAX_NUMBER_OF_CHANNELS] = {0};
  gl1_defs_metrics_lnastate gl1_metrics_lnastate;
#endif /*GERAN_L1_HLLL_LNASTATE*/

  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);


  memset( &yield[0], FALSE, (sizeof(boolean)*MAX_CM_RSSI_MEASUREMENTS) );
  memset( &ScheduleError[0], FALSE, (sizeof(boolean)*MAX_CM_RSSI_MEASUREMENTS) );


  status = mdsp_cm_get_power_measure_results( rssi_values,
                                              &yield[0],
                                              &ScheduleError[0],
#ifdef GERAN_L1_HLLL_LNASTATE
                                              jdetvalues,
#endif /*GERAN_L1_HLLL_LNASTATE*/
                                              gas_id);

#ifdef FEATURE_GSM_TDS_DEBUG
  MSG_GERAN_HIGH_2_G("TDS get pwr rslts: num %d, status %d", num, status);
#endif

#ifdef GERAN_L1_HLLL_LNASTATE
  gl1_metrics_lnastate.NoOfChannels = 0;
#endif /*GERAN_L1_HLLL_LNASTATE*/

  /* Prevent array overflow */
  if (num > MAX_CM_RSSI_MEASUREMENTS)
  {
     num = MAX_CM_RSSI_MEASUREMENTS;
  }

  for (i = 0; i < num; i++)
  {
    samples[i].status =
      gl1_msg_compute_rx_power(rssi_values[i],
                               &samples[i].dBm_x16,
                               &samples[i].rx_gain_range,
                               &tags[i].gain);

#ifdef FEATURE_GSM_TDS_DEBUG
      MSG_GERAN_HIGH_3_G("T2G DEBUG GET PWR MEAS RSLTS: rssi_value %d dbm_x16 %d gain.valx16 %d",
                rssi_values[i], samples[i].dBm_x16, tags[i].gain.valx16);
#endif

    samples[i].yield = yield[i];
    samples[i].ScheduleError = ScheduleError[i];

#ifdef GERAN_L1_HLLL_LNASTATE
    if(gl1_hw_get_sawless_HLLL_support(cm_rssi_arfcn[gas_id][i].band,gas_id))
      {
      samples[i].jdet_value = jdetvalues[i];
      samples[i].SawlessHLLLState = tags[i].sawless_lnastate;
      if(i > MAX_NUMBER_OF_CHANNELS)
        {
        MSG_GERAN_ERROR_2_G("NoOfChannels %d greater than max number allowed:%d",i,MAX_NUMBER_OF_CHANNELS);
        }
      gl1_metrics_lnastate.gsm_metrics[i].arfcn      = cm_rssi_arfcn[gas_id][i];
      gl1_metrics_lnastate.gsm_metrics[i].rx_lvl_dbm = samples[i].dBm_x16;
      gl1_metrics_lnastate.gsm_metrics[i].snr        = INVALID_SNR;
      gl1_metrics_lnastate.gsm_metrics[i].jdet_value = jdetvalues[i];
      gl1_metrics_lnastate.NoOfChannels++;
      SawlessHLLLState[i] = samples[i].SawlessHLLLState;
      }
#endif /*GERAN_L1_HLLL_LNASTATE*/
  }

#ifdef GERAN_L1_HLLL_LNASTATE
  if(gl1_metrics_lnastate.NoOfChannels)
    {
    gl1_hw_rf_burst_metrics(&gl1_metrics_lnastate,gas_id);
    gl1_hw_log_HLin_LLin_metrics(&gl1_metrics_lnastate,SawlessHLLLState,gas_id);
    }
#endif /*GERAN_L1_HLLL_LNASTATE*/

  gl1_hw_log_cell_cmds_to_diag (cmd_cm_get_power_measure_results, FALSE,gas_id);

  return status;
}

#ifdef FEATURE_GSM_TDS
boolean gl1_hw_x2g_get_power_measure_results
(
   gl1_hw_measurement_T  samples[],
   uint8                 *num_ptr,             /* Number of results to process  */
   gl1_hw_rx_tags_type   tags[],
   uint8                 results_buffer,
   sys_modem_as_id_e_type      as_id,
   boolean             gl1_skip_last_meas
)
{
  boolean status;
  static uint32  rssi_values[MAX_CM_RSSI_MEASUREMENTS] = {0};
  static boolean  yield[MAX_CM_RSSI_MEASUREMENTS] = {FALSE};
  static boolean  ScheduleError[MAX_CM_RSSI_MEASUREMENTS] = {FALSE};
  uint8  i = 0, num =0;


#ifdef GERAN_L1_HLLL_LNASTATE
  uint16 jdetvalues[MAX_CM_RSSI_MEASUREMENTS] = {0};
#endif /*GERAN_L1_HLLL_LNASTATE*/

  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  memset( &yield[0], FALSE, (sizeof(boolean)*MAX_CM_RSSI_MEASUREMENTS) );
  memset( &ScheduleError[0], FALSE, (sizeof(boolean)*MAX_CM_RSSI_MEASUREMENTS) );

  status = mdsp_x2g_get_power_measure_results( rssi_values, &yield[0],&ScheduleError[0], results_buffer , num_ptr,
#ifdef GERAN_L1_HLLL_LNASTATE
                                                        jdetvalues,
#endif /*GERAN_L1_HLLL_LNASTATE*/
                                                        gas_id );

  num = *num_ptr;

  if (num == 0 || status == FALSE)
  {
    MSG_GERAN_HIGH_2_G("No Results for T2G PMon Gap status %d num %d",status,num);
    return  status;
  }

  /* Prevent array overflow */
  if (num > MAX_CM_RSSI_MEASUREMENTS)
  {
     num = MAX_CM_RSSI_MEASUREMENTS;
  }

  if(gl1_skip_last_meas)
  {
     MSG_GERAN_HIGH_2_G("last RSSI returned from gfw skipping  measurement %d num %d",gl1_skip_last_meas,num);
     num -= 1;
     if (num == 0)
     {
       return  status;
     }
  }


  for (i = 0; (i < num) ; i++)
  {
     samples[i].status =
      gl1_msg_compute_rx_power(rssi_values[i],
                   &samples[i].dBm_x16,
                   &samples[i].rx_gain_range,
                   &tags[i].gain);

 #ifdef FEATURE_GSM_TDS_DEBUG2
     MSG_GERAN_HIGH_2_G(" RSSI %d written for TL1 = %d ",rssi_values[i],samples[i].dBm_x16);
 #endif

#ifdef  FEATURE_GSM_COEX
     samples[i].yield = yield[i];
#endif /* FEATURE_GSM_COEX */

     samples[i].ScheduleError = ScheduleError[i];

#ifdef GERAN_L1_HLLL_LNASTATE
     samples[i].jdet_value = jdetvalues[i];
     samples[i].SawlessHLLLState = tags[i].sawless_lnastate;
#endif /*GERAN_L1_HLLL_LNASTATE*/
  }

  gl1_hw_log_cell_cmds_to_diag (cmd_cm_get_power_measure_results, FALSE,gas_id);

  return  status;
  
}
#endif //FEATURE_GSM_TDS
/*===========================================================================

FUNCTION  gl1_hw_cm_mode_sw_handler

DESCRIPTION
  This function is called by the GSTMR ISR x frame after it was registered to
  flush SBI queue to the ARM controller and resume normal SBI operation.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_cm_mode_sw_handler( sys_modem_as_id_e_type as_id )
{
  boolean gfw_asleep = FALSE;
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  (gl1_hw_x2g_data[gas_id].gl1_hw_cm_cleanup_wait_frames)--;

  if( gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode != RFCOM_TDSCDMA_MODE )
  {
    if(gl1_hw_x2g_data[gas_id].gl1_hw_cm_cleanup_wait_frames < (GL1_CM_CLEANUP_DELAY_FRAMES-1))
    {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      if (gl1_get_gfw_app_mode(gas_id) != GFW_APP_MODE_IDLE)
#endif
      {
        if (mdsp_ready_for_sleep(gas_id))
        {
          gfw_asleep = mdsp_prepare_for_sleep(gas_id);
          if (gfw_asleep == FALSE)
          {
             mdsp_display_sleep_error(TRUE ,gas_id);
          }
          else
          { 
            gl1_hw_x2g_data[gas_id].gl1_hw_cm_cleanup_wait_frames = 0;
          }
        }
      }
    }
  }
  /* When we hit 0 cleanup wait frames the mDSP is done */
  if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_cleanup_wait_frames == 0)
  {
    MSG_GERAN_MED_1_G(GL1_SLEEP_HDR" CM GFW asleep %d",gfw_asleep);
    gstmr_deregister_handler_geran(gl1_hw_cm_mode_sw_handler,as_id);
    gl1_hw_x2g_data[gas_id].gl1_hw_cm_gap_complete = TRUE;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    /* Reset GFW mode to MEAS. We had set it to ACTIVE at CM startup cmd issue
        *  to avoid device mode chang   */
    gl1_set_gfw_app_mode_status( GFW_APP_MODE_IDLE, gas_id );
#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */
  }
}


/*===========================================================================

FUNCTION  gl1_hw_cm_cleanup_done_delay

DESCRIPTION
  This function is called to tell this driver when the cm_cleanup should be
  finished.  It takes the mdsp one frame to execute it's job.   Or two frames
  if the cm_cleanup is to happen in the FTSM of the next frame.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_hw_cm_cleanup_done_delay (uint8 frames_delayed,gas_id_t gas_id)
{
  /* Set the new wait delay to the max value */
  if (frames_delayed > gl1_hw_x2g_data[gas_id].gl1_hw_cm_cleanup_wait_frames)
  {
    gl1_hw_x2g_data[gas_id].gl1_hw_cm_cleanup_wait_frames = frames_delayed;
  }

  /* Register the cleanup handler if necessary */
  if (!gstmr_is_handler_registered_geran(gl1_hw_cm_mode_sw_handler,geran_map_gas_id_to_nas_id(gas_id)))
  {
    gstmr_register_handler_geran(gl1_hw_cm_mode_sw_handler,geran_map_gas_id_to_nas_id(gas_id));
  }
}


/*===========================================================================

FUNCTION  gl1_hw_cm_cleanup_done_cancel

DESCRIPTION
  This function is called to tell this driver when to cancel the cleanup
  since another startup has arrived.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_hw_cm_cleanup_done_cancel (gas_id_t gas_id)
{
  /* If we have a pending cleanup, abort it since another gap has started */
  if (gstmr_is_handler_registered_geran(gl1_hw_cm_mode_sw_handler,geran_map_gas_id_to_nas_id(gas_id)))
  {
    /* Deregister the cleanup handler if necessary */
    gstmr_deregister_handler_geran(gl1_hw_cm_mode_sw_handler,geran_map_gas_id_to_nas_id(gas_id));

    /* Clear number of wait frames */
    gl1_hw_x2g_data[gas_id].gl1_hw_cm_cleanup_wait_frames = 0;
    gl1_hw_x2g_data[gas_id].gl1_hw_cm_gap_complete = TRUE;
  }
}

/* this stores the frame tick setup margin for the non compressed mode. It is restored after isssuing the cleanup command*/
static uint16 prev_frame_tick_setup_margin = 0;
/*===========================================================================

FUNCTION  gl1_hw_cm_startup

DESCRIPTION
  Calls to invoke the CM startup at the beginning of the gap.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void  gl1_hw_cm_startup( uint16                      offset,
                                const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr
#ifdef FEATURE_DUAL_SIM
                                ,sys_modem_as_id_e_type      as_id
#endif /* FEATURE_DUAL_SIM*/
                                 )
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/

   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

   gl1_hw_cm_startup_geran(offset,cm_rf_info_ptr,TRUE,as_id);
   gl1_hw_x2g_data[gas_id].gl1_hw_first_gap_after_preload = FALSE;
   return;
}

#define UNUSED_PRELOAD_BUF_INDEX    0xFF

/*===========================================================================

DESCRIPTION
  Calls to invoke the CM startup at the beginning of the gap.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_cm_build_startup_script (const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr, gas_id_t gas_id)
{
  rfm_meas_setup_param_type  startup_rfm_meas_gsm_setup;
  rfcom_gsm_band_type gsm_band;

  /* Update the target band */
  gsm_band = gl1_hw_arfcn_band_to_rfcom_gsm_band(cm_rf_info_ptr->target_band);  /* Default is RFCOM_BAND_GSM900 */
  if (gsm_band != gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->target_param.gsm_params.band)
  {
     MSG_GERAN_HIGH_2_G("X2G Startup band %d, differs from meas enter band %d",
                        gsm_band, gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->target_param.gsm_params.band);
     gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->target_param.gsm_params.band = gsm_band;
  }

  startup_rfm_meas_gsm_setup.setup_params               = cm_rf_info_ptr->common_rf_structure_ptr ;
  if(startup_rfm_meas_gsm_setup.setup_params->script_type != RFM_MEAS_PROGRAM_NULL2X)
  {
     startup_rfm_meas_gsm_setup.setup_params->script_type  = RFM_MEAS_PROGRAM_STARTUP;
  }


  if( RFM_MEAS_COMMON_SUCCESS != rfm_meas_common_script_build_scripts( &startup_rfm_meas_gsm_setup, NULL ) )
  {
  #ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_0_G("startup rfm failed");
  }
  gl1_hw_x2g_data[gas_id].gl1_hw_cm_preload_sent = FALSE;
}

/*===========================================================================

FUNCTION  gl1_hw_cm_startup

DESCRIPTION
  Calls to invoke the CM startup at the beginning of the gap.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void  gl1_hw_cm_startup_geran( uint16                      offset,
                               const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr,
                               boolean                     is_first_burst,
                               sys_modem_as_id_e_type      as_id
                             )
{
  boolean awake;
  uint32 use_idle_rx_alpha;
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

#ifdef FEATURE_GSM_TDS
  cm_startup_issued = TRUE;
#endif


  /* Cancel any pending cleanup done processing, and then clear gap complete flag */
  gl1_hw_cm_cleanup_done_cancel(gas_id);

  gl1_hw_x2g_data[gas_id].gl1_hw_cm_gap_complete = FALSE;

  /* For L2G and W2G IRAT need to wakeup MDSP because it is asleep between IRAT gaps */
  if( gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode != RFM_TDSCDMA_MODE 
         && !gl1_hw_qta_gap_active(gas_id) )
  {
     awake = mdsp_wakeup_geran(TRUE , as_id);
     MSG_GERAN_HIGH_2_G(GL1_SLEEP_HDR" CM wakeup %d curr_qs %d",awake,gstmr_rd_qsymbol_count_geran(as_id));
  }

  gl1_hw_qta_mutex_lock( gas_id ); 

  if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded == FALSE)
  {
     if( NULL != cm_rf_info_ptr)
     {
#if defined (FEATURE_QBTA)
        if(!gl1_hw_get_qbta_scripts_status(gas_id))
#endif
        {
           gl1_hw_cm_build_startup_script( cm_rf_info_ptr, gas_id);
        }
     }
     else
     {
        MSG_GERAN_ERROR_0_G(" NULL cm_rf_info_ptr ");
        gl1_hw_qta_mutex_unlock( gas_id ); 
        return;
     }
  }

  gl1_hw_qta_mutex_unlock( gas_id );

  /* Check current rx alpha state in FW, update to FW if it's different */
  if (cm_rf_info_ptr->use_long_rx_alpha != mdsp_get_rx_alpha_state(gas_id))
  {
    use_idle_rx_alpha = (uint32)cm_rf_info_ptr->use_long_rx_alpha;
    mdsp_update_shared_variable(GFW_RX_ALPHA, &use_idle_rx_alpha, gas_id);
    mdsp_issue_generic_config_command(gas_id);
  }

#ifdef FEATURE_GSM_TDS_DEBUG
    MSG_GERAN_HIGH_1_G("STARTUP: RF STOP %d qs", gstmr_rd_qsymbol_count_geran(as_id));
#endif

  /* ftsm is restored back in cleanup function */
  prev_frame_tick_setup_margin = mdsp_ftsm(gas_id);

   if( gl1_hw_qta_gap_active(gas_id) == TRUE )
   {
       mdsp_set_ftsm_geran(625, gas_id);
   }
   else
   {
       switch( gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode )
       {
           case RFCOM_WCDMA_MODE:
           case RFCOM_TDSCDMA_MODE:
                mdsp_set_ftsm_geran(0, gas_id);
                break;
           case RFCOM_LTE_MODE:
                mdsp_set_ftsm_geran(625, gas_id);
                break;
          default:
                MSG_GERAN_ERROR_1_G( "gl_hw_cm_startup called in unknown mode:%d",
                                     gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode );
                break;

     }
  }

  /* Send preload command to FW */
  if ((gl1_hw_x2g_data[gas_id].gl1_hw_cm_preload_sent == FALSE) && (gl1_hw_x2g_data[gas_id].gl1_hw_cm_nbr_pretune_offset_time != 0))
  {
     uint16 preload_offset;
     boolean immediate_preload = FALSE;
     gsmtr_fn_qs_type    current_fn_qs;

     /* For QTA gap allow immediate preload as already have timeline checks and home RAT is not running */
     if (gl1_hw_qta_gap_active(gas_id))
     {
        immediate_preload = TRUE;

        /* Input params for X2G arbitration */
        gl1_hw_arbitration[gas_id].x2g.arfcn = gl1_hw_x2g_data[gas_id].gl1_hw_cm_x2g_coex_params.arfcn;
        gl1_hw_arbitration[gas_id].x2g.interband_monitors = gl1_hw_x2g_data[gas_id].gl1_hw_cm_x2g_coex_params.interband_monitors;
     }
     else /* IRAT gap , ARFCN information is not known , worst case use UNKNOWN_FREQID */
     {
       //gl1_hw_arbitration[gas_id].x2g.arfcn is UNKNOWN
       gl1_hw_arbitration[gas_id].x2g.interband_monitors = TRUE;
     }

     /* Perform arbitration of activities prior to configuring RF and sending mDSP commands */
     gl1_hw_arbitration[gas_id].notify_event(ARBITRATE_X2G, gas_id);

     /* Output params from X2G arbitration */
     gl1_hw_x2g_data[gas_id].gl1_hw_cm_x2g_coex_params.activity_priority    = gl1_hw_arbitration[gas_id].x2g.activity_priority;
     gl1_hw_x2g_data[gas_id].gl1_hw_cm_x2g_coex_params.no_activity_priority = gl1_hw_arbitration[gas_id].x2g.no_activity_priority;
     gl1_hw_x2g_data[gas_id].gl1_hw_cm_x2g_coex_params.desense_id           = gl1_hw_arbitration[gas_id].x2g.desense_id;

     /* read current FN, QS */
     current_fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);

     gl1_hw_x2g_data[gas_id].gl1_hw_cm_preload_sent = TRUE;

     /* If preload needed in frame before startup send command now */
     if (offset < (gl1_hw_x2g_data[gas_id].gl1_hw_cm_nbr_pretune_offset_time + FRAME_TICK_SETUP_MARGIN_QS))
     {
        uint16 startup_offset_next_frame = QS_PER_FRAME + offset;

        preload_offset = startup_offset_next_frame - gl1_hw_x2g_data[gas_id].gl1_hw_cm_nbr_pretune_offset_time;
        if ( current_fn_qs.qs >= (preload_offset - ALLOWED_IMMEDIATE_PRELOAD_MARGIN))
        {
	  immediate_preload = TRUE;
	  MSG_GERAN_HIGH_2_G("GL1 delayed: allow GFW to preload with expired offset; current qs %d, preload offset %d", current_fn_qs.qs, preload_offset);
        }
        mdsp_cm_preload(preload_offset,
                        offset,
                        cm_rf_info_ptr->rxlm_gsm_buf_idx,
                        cm_rf_info_ptr->target_rf_device,
                        immediate_preload,
                        cm_rf_info_ptr->is_a_qta_gap,
                        TRUE,
                        gl1_hw_x2g_data[gas_id].gl1_hw_cm_x2g_coex_params,
                        gas_id);
        mdsp_async_intf_send_immediate_cmd(GSTMR_GET_FN_GERAN(gas_id), FALSE,gas_id);
        MSG_GERAN_HIGH_2_G("Immediate cmd for preload, preload offset %d, startup offset %d", preload_offset, offset);
     }
     else
     {
        preload_offset = offset - gl1_hw_x2g_data[gas_id].gl1_hw_cm_nbr_pretune_offset_time;
	if ( current_fn_qs.qs >= (preload_offset - ALLOWED_IMMEDIATE_PRELOAD_MARGIN))
	{
	  immediate_preload = TRUE;
	  MSG_GERAN_HIGH_2_G("GL1 delayed: allow GFW to preload with expired offset; current qs %d, preload offset %d", current_fn_qs.qs, preload_offset);
	}
        mdsp_cm_preload(preload_offset,
                        offset,
                        cm_rf_info_ptr->rxlm_gsm_buf_idx,
                        cm_rf_info_ptr->target_rf_device,
                        immediate_preload,
                        cm_rf_info_ptr->is_a_qta_gap,
                        TRUE,
                        gl1_hw_x2g_data[gas_id].gl1_hw_cm_x2g_coex_params,
                        gas_id);
     }
  }

  gfwhost_cmdbuf_offsets[gas_id].startup_ptr= (GfwX2GStartupCmd*)mdsp_cm_startup( offset,
                   0,
                   cm_rf_info_ptr->common_rf_structure_ptr->target_param.gsm_params.rx_dev_handle[DEV_HANDLE_PRX].lm_handle,
                   cm_rf_info_ptr->common_rf_structure_ptr->target_param.gsm_params.tqp_dev,
                   cm_rf_info_ptr->target_rf_device,
                   cm_rf_info_ptr->is_a_qta_gap,
                   cm_rf_info_ptr->common_rf_structure_ptr->header.source_tech,
                   gl1_hw_x2g_data[gas_id].gl1_hw_cm_x2g_coex_params,
                   is_first_burst,
                   gas_id);
#ifdef FEATURE_GSM_TDS_DEBUG2
    MSG_GERAN_HIGH_1_G("starup  cmd written at %x in the mdsp interface",gfwhost_cmdbuf_offsets[gas_id].startup_ptr);
#endif

  {
     ARFCN_T arfcn;
     int32 xo_value;

     /* not for NBCCH in QBTA as RGS is not updated */
     if ( !gl1_msg_is_async_bcch( gas_id ) )
     {
       tcxomgr_vco_info_type vco_info;
       /* inside the QTA gap read Client RGS, as possibly the subscription RGS can get 
          updated by other tech while doing an IRAT activity */
       if ( gl1_hw_qta_gap_active(gas_id))
       {
         vco_info = gl1_hw_get_tcxomgr_vco( gas_id );
         xo_value = vco_info.rot_value;
       }
       else
       {
         vco_info = gl1_hw_get_initial_rgs( gas_id );
         xo_value = vco_info.rot_value;
       }
     }
     else
     {
       xo_value = gl1_hw_get_xo_acc_freq_err(gas_id);
     }

    /* Ensure we log before setting XO to get FreqCommon */
    gl1_log_afc_adjust( xo_value, 0, LOG_AFC_CM_STARTUP,gas_id );

    gl1_msg_set_acq_xo_value( xo_value,gas_id );
    gl1_hw_set_xo_acc_freq_err( xo_value,gas_id );

    /* Update cswallower/DCO for GSTMR, use nominal arfcn */
    gl1_msg_algs_get_arfcn( &arfcn.num, &arfcn.band,gas_id );
    gl1_hw_set_active_cycle_swallower_adj( arfcn,gas_id );
    gl1_hw_gfw_config_cycle_swallower(gas_id);
  }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
 /* Set GFW mode as ACTIVE here as we dont want to change
  * DEVICE mode change between a CM Gap*/
  gl1_set_gfw_app_mode_status( GFW_APP_MODE_ACTIVE, gas_id );
#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */

  gl1_hw_log_cell_cmds_to_diag (cmd_cm_startup, FALSE,gas_id);
} /* gl1_hw_cm_startup_geran */

#ifdef FEATURE_QTA
#ifdef QTA_EVENT_DATA_LOGGING
/*===========================================================================
FUNCTION  gl1_hw_log_qta_event

DESCRIPTION

===========================================================================*/
void gl1_hw_log_qta_event(  sys_modem_as_id_e_type  as_id,
                            gas_id_t                gas_id,
                            gl1_hw_cm_rat_t         srv_tech,
                            gl1_hw_qta_event_t      qta_event)
{
  gl1_hw_qta_event_data_t *data_ptr = NULL;

  gl1_hw_qta_mutex_lock( gas_id );
  if(++gl1_hw_qta_event_data_index >= GL1_HW_QTA_EVENT_DATA_LOG_SIZE)
  {
    gl1_hw_qta_event_data_index = 0; // reset the index
  }

  data_ptr = &gl1_hw_qta_event_data[gl1_hw_qta_event_data_index];

  data_ptr->as_id_of_srv_tech = as_id;
  data_ptr->gas_id = gas_id;
  data_ptr->srv_tech = srv_tech;
  data_ptr->qta_event = qta_event;
  data_ptr->frame_num = GSTMR_GET_FN_GERAN(gas_id);
  data_ptr->gl1_qta_has_initialised = gl1_hw_cm_get_qta_wcdma_has_initialised(gas_id);

  gl1_hw_qta_mutex_unlock( gas_id );
}
#endif /* QTA_EVENT_DATA_LOGGING */

/*===========================================================================

FUNCTION  gl1_hw_get_gsm_rf_id_qta

DESCRIPTION
  Return the AS_ID of the sub whose QTA was requested

===========================================================================*/

rfm_device_enum_type gl1_hw_get_gsm_rf_id_qta(gas_id_t gas_id)
{
  return gl1_hw_x2g_data[gas_id].gl1_hw_cm_trm_rf_device;
}

/*===========================================================================

FUNCTION  gl1_hw_qta_return_cm_as_id

DESCRIPTION
  Return the AS_ID of the sub whose QTA was requested

===========================================================================*/

sys_modem_as_id_e_type gl1_hw_qta_return_cm_as_id(gas_id_t gas_id)
{
  MSG_GERAN_LOW_1("Value of the gl1_qta_cm_sys_modem_as_id = %d", gl1_hw_x2g_data[gas_id].gl1_qta_cm_sys_modem_as_id);
  return gl1_hw_x2g_data[gas_id].gl1_qta_cm_sys_modem_as_id;
}

/*===========================================================================

FUNCTION  gl1_hw_trm_qta_startup

DESCRIPTION
  This function is called from TRM shortly before a QTA gap to notify
  GSM of the parameters to be used for QTA/CM startup.

===========================================================================*/

void gl1_hw_trm_qta_startup(geran_gl1_qta_msg_type* qta_data_p, gas_id_t gas_id)
{
  rfm_meas_common_param_type *gl1_qta_rf_structure_ptr;
  lm_status_type lm_status;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  sys_band_class_e_type grm_last_reserved_band = SYS_BAND_CLASS_NONE;

  MSG_GERAN_MED_0_G(" TRM -> QTA Start-up ");
  MSG_GERAN_HIGH_1_G("TRM -> QTA Start-up: DEBUG: qta_data_p: active_carrier_idx_mask : %d", qta_data_p->rf_params->source_param.lte_params.active_carrier_idx_mask);

  gl1_hw_qta_mutex_lock( gas_id );
  /* If previous QTA gap cleanup not complete end it now */
  /* so RF exit uses the same RF structure as RF enter */
  if (gl1_hw_cm_get_qta_wcdma_has_initialised(gas_id))
  {
     MSG_GERAN_ERROR_0_G("QTA startup before previous gap completed, exit it now"); 
     gl1_hw_cm_qta_gap_exit(TRUE, gas_id);  
  }
  gl1_hw_qta_mutex_unlock( gas_id );

 
#ifdef FEATURE_QTA_IN_FINDBCCH
  if(l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH)
  {
    /* FIND_BCCH mode doesn't use a frame manager PRI, so get its registered 
       band from FIND_BCCH mode state variables */
    grm_last_reserved_band = gl1_map_gsm_band_to_sys_band(L1_multi_sim_bcch_get_reservation_band(gas_id));
  }
  else
#endif /* FEATURE_QTA_IN_FINDBCCH */
  {
    grm_last_reserved_band = grm_get_trm_band_from_g_pri(l1_get_fm_trm_priority(gas_id ), gas_id);
  }
  
  /* We should skip QTA if the allocated device does not support the band we reserved.
     Example of how that might happen:
      /* In rare case, GL1 might have cancelled the previous reservation and made a new one. If QTA is started right
         before GL1 changes the reservation, the allocated device for QTA is based on the previous reservation. This
         device might not support the new band of the new reservation. Here is one example, GL1 has reserved W band
         for G2W IRAT in QTA. A new WCDMA list coming from RR, so GL1 cancelled G2W IRAT and made a new reservation
         for next GSM page with G band. The cancellation happens after TRM has started QTA process. Therefore when GL1
         receives QTA startup callback, the allocated device in the callback is based on W band. Inside GL1, the next
         QTA should be for GSM page. GL1 starts to build RF script for GSM band on the allocated device for W band. Some RF
         cards have RF devices which support W band only. We would see RF crashes. */
  if(grm_last_reserved_band == SYS_BAND_CLASS_NONE ||
   FALSE == grm_check_device_supported(gl1_map_gas_id_to_client_id(gas_id),
                                       grm_last_reserved_band,
                                       qta_data_p->dev_allocated[0],
                                       gas_id))
  {
    MSG_GERAN_HIGH_3_G("QTA startup: the allocated device %d does not support the last reserved band %d pri %d, don't start QTA",
                       (uint16)qta_data_p->dev_allocated[0],
                       (uint16)grm_last_reserved_band, 
                       (uint16)l1_get_fm_trm_priority(gas_id));
    return; 
  }

  /* Copy RF structure to GL1 RF structure (QTA shares with G2X IRAT) */
  gl1_qta_rf_structure_ptr = gl1_get_irat_rf_structure_ptr(gas_id);  
  *gl1_qta_rf_structure_ptr = *qta_data_p->rf_params;
  gl1_qta_cm_rf_info.common_rf_structure_ptr = gl1_qta_rf_structure_ptr; 
  gl1_qta_cm_rf_info.target_rf_device        = qta_data_p->dev_allocated[0]; /* DRX device not considred?*/
  
  MSG_GERAN_HIGH_1_G("DEBUG:gl1_qta_cm_rf_info: active_carrier_idx_mask : %d", gl1_qta_cm_rf_info.common_rf_structure_ptr->source_param.lte_params.active_carrier_idx_mask);

  l1_sc_g2l_build_script_pending[gas_id] = FALSE;

  if (!l1_sc_x2g_ta_irat_gap(gas_id))
  {
     /*To be updated with a direct and complete mapping*/
     gl1_qta_cm_rf_info.target_band             = gl1_map_sys_band_to_gsm_band(grm_last_reserved_band); /* TBD: Cast is done*/
  
     gl1_qta_cm_rf_info.rxlm_gsm_buf_idx = gl1_hw_get_gsm_rxlm_buf_idx(gas_id);
     if (gl1_qta_cm_rf_info.rxlm_gsm_buf_idx == MDSP_RXLM_BUF_IDX_INVALID)
     {
       /* Allocate RxLM buffer */
       lm_status = rflm_allocate_rx_handle(LM_GSM, LM_GSM, &gl1_qta_cm_rf_info.rxlm_gsm_buf_idx);
       if (lm_status == LM_SUCCESS)
       {
         MSG_GERAN_MED_1_G("GSM RxLM buffer %d allocated in QTA Start-up", gl1_qta_cm_rf_info.rxlm_gsm_buf_idx);
         gl1_hw_set_gsm_rxlm_buf_idx(gl1_qta_cm_rf_info.rxlm_gsm_buf_idx, gas_id);
       }
       else
       {
         MSG_GERAN_ERROR_0_G("Unable to allocate RxLM buffer");
       }
     }
     gl1_hw_x2g_data[gas_id].qta_gap_for_g2x_irat = FALSE;
  }
  else
  {
     gl1_hw_x2g_data[gas_id].qta_gap_for_g2x_irat = TRUE;
  }

  gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode = gl1_qta_rf_structure_ptr->header.source_tech;

  gl1_qta_cm_rf_info.is_a_qta_gap = TRUE;

  gl1_hw_qta_startup(&gl1_qta_cm_rf_info,qta_data_p, geran_map_gas_id_to_nas_id(gas_id));
  
}

/*===========================================================================

FUNCTION  gl1_hw_trm_qta_cleanup

DESCRIPTION
  This function is called from TRM shortly after a QTA gap has completed.

===========================================================================*/
void gl1_hw_trm_qta_cleanup( geran_gl1_qta_msg_type* qta_data_p , gas_id_t gas_id )
{
      
  MSG_GERAN_MED_0_G(" TRM -> QTA Clean-up ");
  
  gl1_hw_qta_cleanup(qta_data_p->as_id);
  
}

/*===========================================================================

FUNCTION gl1_hw_reset_qta_flags_skipping_gap

DESCRIPTION
  This function will reset all QTA related flags in case of QTA recovery

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_reset_qta_flags_skipping_gap(gas_id_t gas_id)
{
   set_qta_cleanup_pending(TRUE,gas_id);
   gl1_hw_set_qta_cm_fw_rf_init_status(gas_id, FALSE);
   gl1_hw_x2g_data[gas_id].gl1_hw_first_gap_after_preload = TRUE;
   gl1_set_gfw_app_mode_status( GFW_APP_MODE_IDLE, gas_id );
   MSG_GERAN_HIGH_2_G("Clear MDSP Commands as StartUpCmd is too late rf_init_status=%d,cleanupPend=%d",
                               gl1_hw_get_qta_cm_fw_rf_init_status(gas_id),
                               get_qta_cleanup_pending(gas_id));
   
}
/*===========================================================================

FUNCTION  gl1_hw_qta_startup

DESCRIPTION
  This function is called from WCDMA shortly before a QTA gap to notify
  GSM of the parameters to be used for CM startup.

===========================================================================*/
void gl1_hw_qta_startup( const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr,
                         geran_gl1_qta_msg_type*             qta_data_p,
                         sys_modem_as_id_e_type              as_id )
{
  gsmtr_fn_qs_type current_fn_qs;
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  if(TRUE == gl1_check_if_getting_deactivated(gas_id))
  {
    MSG_GERAN_HIGH_0_G("GL1 getting deactivated. dont start QTA ");
    gl1_hw_x2g_data[gas_id].gl1_empty_qta_gap = TRUE;
    gl1_hw_cm_set_qta_gap_for_g2x_irat(FALSE, gas_id);
    return;
  }

  if(gl1_qta_cm_rf_info.target_rf_device == RFM_INVALID_DEVICE)
  {
    MSG_GERAN_HIGH_1_G("QTA Startup with invalid target device %d",gl1_qta_cm_rf_info.target_rf_device);
    gl1_hw_x2g_data[gas_id].gl1_empty_qta_gap = TRUE;
    gl1_hw_cm_set_qta_gap_for_g2x_irat(FALSE, gas_id);
    return;
  }

  if (gl1_hw_x2g_data[gas_id].qta_gap_for_g2x_irat)
  {
    l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
    uint16 freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;
    /* set nQTA duration */
    //gl1_hw_set_nqta_resource_release_duration(qta_data_p->nqta_resource_release_duration, gas_id);
    /* Build G2L scripts if QTA gap is for LTE search */
    if ( (l1_get_fm_trm_priority(gas_id ) == FM_PRI_LTE_IDSEARCH) &&
         (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_LTE) &&
         (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_WAIT) )
    {
      /* If DRX chain is not granted by TRM, skip measurement */
      if (gl1_hw_set_g2l_devinfo_for_qta(qta_data_p, gas_id))
      {
        g2l_irat_x2l_meas_enter( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn , gas_id);
        L1_send_LTE_CPHY_IRAT_MEAS_BUILD_RF_SCRIPT_REQ(gas_id);
     
        l1_sc_g2l_build_script_pending[gas_id] = TRUE;
        l1_sc_g2l_empty_qta_gap[gas_id]        = TRUE;

        /* If gap gets aborted, this is to notify ML1 that scripts were built but not 
           executed. Set this to FALSE while issuing TIMED_SRCH_REQ */
        l1_sc_wcdma_data_ptr->g2x_aborted = TRUE;
        
        MSG_GERAN_HIGH_2_G("G2L:LTE_CPHY_IRAT_MEAS_BUILD_RF_SCRIPT_REQ Started earfcn=%d idx=%d",
                          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn,
                          freq_idx);
      }
      else
      {
        l1_fm_clear_required_frames( FM_PRI_LTE_IDSEARCH, gas_id );
        l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent = FALSE;
        gl1_hw_cm_set_qta_gap_for_g2x_irat(FALSE, gas_id);
        return;
      }
    }
  }

#if defined (FEATURE_QBTA)
  /* set QBTA flag to TRUE only for the case of W2G QTA and QBTA enabled by NV */
  if(gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode ==  RFM_IMT_MODE)
  {
     if(gl1_get_qbta_nv_support())
     {
       gl1_hw_set_QBTA_flag(gas_id,TRUE);
     }
  }
#if defined(FEATURE_LTE_TO_GSM)
  else if(gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode ==  RFCOM_LTE_MODE)
  {
    /* Using source tech AS ID for this LTE API */
    if(lte_ml1_irat_geran_qbta_enabled(gl1_qta_cm_rf_info.common_rf_structure_ptr->header.source_sub_id))

    {
      gl1_hw_set_QBTA_flag(gas_id,TRUE);
    }
    else
    {
      MSG_GERAN_HIGH_0_G("QBTA NV for LTE is not SET");
    }
  }
#endif
  else
   {
     gl1_hw_set_QBTA_flag(gas_id,FALSE);   
   }
#endif

  gl1_hw_qta_mutex_lock( gas_id );
  gl1_hw_x2g_data[gas_id].gl1_qta_cm_sys_modem_as_id              = as_id;
  gl1_hw_x2g_data[gas_id].gl1_hw_rx_count_in_qta                  = 0;
  gl1_hw_x2g_data[gas_id].gl1_empty_qta_gap                       = FALSE;
  gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded             = FALSE;
  gl1_hw_x2g_data[gas_id].gl1_hw_first_gap_after_preload          = FALSE;
  set_qta_cleanup_pending(TRUE, gas_id);
  gl1_hw_qta_mutex_unlock( gas_id );
  gl1_hw_x2g_data[gas_id].gl1_hw_cm_trm_rf_device = cm_rf_info_ptr->target_rf_device; 
  if (!gl1_hw_x2g_data[gas_id].qta_gap_for_g2x_irat)
  {
     gl1_hw_start_gsm_qta( gas_id );
     geran_l1_x2g_irat_enter(cm_rf_info_ptr,as_id);
  }

  current_fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);
  MSG_GERAN_HIGH_2_G("QTA Startup: FN %d, qs %d",current_fn_qs.fn,current_fn_qs.qs);

  /* Prebuild RF scripts  */

  if (!gl1_hw_x2g_data[gas_id].qta_gap_for_g2x_irat)
  {
     /* Get IRAT timings from RF */
     gl1_hw_cm_get_rf_irat_timing_info (gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode, gas_id);

     /* +Try to run this in the GL1 task context for now */
     /* Build scripts in DPC task so high priority but do not delay XL1 */
     /*gl1_hw_cm_build_startup_dpc (&gl1_qta_cm_rf_info, gas_id);*/
     gl1_hw_cm_build_startup_if_not_prebuilt (&gl1_qta_cm_rf_info,gas_id);
     /* -Try to run this in the GL1 task context for now */
  }

  gl1_hw_cm_set_qta_wcdma_has_initialised(TRUE, gas_id);
  
#ifdef QTA_EVENT_DATA_LOGGING
  gl1_hw_log_qta_event(as_id, gas_id, gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode, GL1_HW_QTA_STARTUP);
#endif

}

/*===========================================================================

FUNCTION  gl1_hw_qta_cleanup

DESCRIPTION
  This function is called from WCDMA shortly after a QTA gap has completed.

===========================================================================*/
void gl1_hw_qta_cleanup( sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  if(!gl1_hw_qta_gap_active(gas_id))
  {
     MSG_GERAN_HIGH_0_G( "QTA cleanup, but no QTA startup");
     return;
  }

  /* Mutex lock for race condtion between cleanup and QTA fwrf init */
  gl1_hw_qta_mutex_lock( gas_id );

  MSG_GERAN_HIGH_2_G("QTA Cleanup: cleanup pending %d, cm_fw_rf_init %d",get_qta_cleanup_pending(gas_id),gl1_hw_get_qta_cm_fw_rf_init_status(gas_id));

  /* In case of empty QTA gap allow time for preload to complete */
  if(get_qta_cleanup_pending(gas_id))
  {
    MSG_GERAN_HIGH_0_G ("Empty QTA gap");
    gl1_hw_cm_qta_gap_exit(TRUE, gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_EMPTY_QTA_GAP);
#endif
  }
  else
  {
   if(gl1_hw_get_qta_cm_fw_rf_init_status(gas_id))
   {
     /* If in the middle of QTA gap, delay cleanup until qta fw/rf deinit */
     gl1_hw_x2g_data[gas_id].gl1_qta_early_cleanup = TRUE;
   }
   else
   {
     gl1_hw_cm_qta_gap_exit(FALSE, gas_id);
   }
  }

  gl1_hw_set_if_start_gsm_qta_sent(FALSE, gas_id);
  
  gl1_hw_qta_mutex_unlock( gas_id );

#if defined (FEATURE_QBTA)

  if(gl1_hw_check_in_QBTA(gas_id)|| gl1_hw_get_qbta_scripts_status(gas_id) )
  {
    int i;
    gl1_hw_set_QBTA_flag(gas_id,FALSE);

    for (i=0 ; i<NUM_GERAN_DATA_SPACES ; i++)
    {
      gl1_hw_qbta_data[i].gl1_hw_build_scripts_in_qbta=FALSE;
    }
  }
#endif

#ifdef QTA_EVENT_DATA_LOGGING
  gl1_hw_log_qta_event(as_id, gas_id, gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode, GL1_HW_QTA_CLEANUP);
#endif
}

#if defined (FEATURE_WCDMA)
/*===========================================================================

FUNCTION  gl1_hw_get_next_qta_gap_params

DESCRIPTION
  This function is called from GL1 to set the G idle sub reserve time in SCLKs .

INPUT PARAMETERS
  start time and duartion address.
===========================================================================*/
void gl1_hw_get_next_qta_gap_params(uint32 *start_sclks, uint32 *end_sclks , sys_modem_as_id_e_type as_id)
{

   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
   *start_sclks = gl1_hw_x2g_data[gas_id].gl1_hw_qta_start_slcks_w;
   *end_sclks =  gl1_hw_x2g_data[gas_id].gl1_hw_qta_stop_slcks_w;
   MSG_GERAN_LOW_2_G("qta_start slks = %d qta_durat slks=%d",*start_sclks,*end_sclks);
}
#endif

#if defined (FEATURE_QBTA)

/*===========================================================================

FUNCTION  gl1_hw_fill_next_qta_gap_params

DESCRIPTION
  This function is called from GL1 to set the G idle sub reserve time in SCLKs .

INPUT PARAMETERS
  start time and duartion in SLCKS
===========================================================================*/
void gl1_hw_fill_next_qta_gap_params( uint32 when,gas_id_t gas_id )
{
   trm_time_t 			  qta_start,qta_durat;
   qta_start = 0;
   qta_durat = 0;

   qta_start =	when + QTA_START_OFFSET_SLKS ;
   qta_durat = qta_start + GSTMR_NO_OF_SYS_CLKS_IN_FRAME + QTA_CLEANUP_AFTER_FIRST_FRAME_SLKS;
   
   gl1_hw_x2g_data[gas_id].gl1_hw_qta_start_slcks_w = qta_start;
   gl1_hw_x2g_data[gas_id].gl1_hw_qta_stop_slcks_w = qta_durat; 
   MSG_GERAN_LOW_2_G("qta_start slks = %d qta_durat slks=%d",
            gl1_hw_x2g_data[gas_id].gl1_hw_qta_start_slcks_w,
                       gl1_hw_x2g_data[gas_id].gl1_hw_qta_stop_slcks_w);
}


/*===========================================================================

FUNCTION  gl1_hw_check_in_QBTA

DESCRIPTION
  This function whether QTA or QBTA will be done.

  Return:
  TRUE  if QTA gap opened by WCDMA: in this case FULL QBTA is done
  FALSE if QTA gap is Opened by TDSCDMA in this normal QTA.
===========================================================================*/
boolean gl1_hw_check_in_QBTA( gas_id_t gas_id )
{
   return (gl1_hw_qbta_data[gas_id].gl1_hw_full_qbta);
}

/*===========================================================================
FUNCTION  gl1_hw_set_QBTA_flag
DESCRIPTION
  This function sets QBTA flag.
===========================================================================*/
void gl1_hw_set_QBTA_flag(gas_id_t gas_id, boolean val)
{
   gl1_hw_qbta_data[gas_id].gl1_hw_full_qbta = val;
   MSG_GERAN_HIGH_1_G("Full Qbta flag = %d",gl1_hw_qbta_data[gas_id].gl1_hw_full_qbta);
}

/*===========================================================================

FUNCTION gl1_hw_get_qbta_scripts_status

DESCRIPTION
  This return in_qbta is true or false

INPUT PARAMETERS
  gas_id

===========================================================================*/
boolean gl1_hw_get_qbta_scripts_status(gas_id_t gas_id)
{
   return gl1_hw_qbta_data[gas_id].gl1_hw_build_scripts_in_qbta;
}


/*===========================================================================

FUNCTION gl1_hw_set_qbta_scripts_status

DESCRIPTION
  This set in_qbta as true or false

INPUT PARAMETERS
  gas_id and boolean val

===========================================================================*/
void gl1_hw_set_qbta_scripts_status(gas_id_t gas_id,boolean status)
{
   gl1_hw_qbta_data[gas_id].gl1_hw_build_scripts_in_qbta = status;
}
#endif

/*===========================================================================

FUNCTION  gl1_hw_qta_fw_rf_init

DESCRIPTION
  This function initializes the firmware and RF for QTA operation, this must
  only be called once the lock has been granted.

  gl1_hw_qta_startup() must have been called by WCDMA prior to this

INPUT PARAMETERS
  arfcn     -- ARFCN of the QTA CCCH, NFCCH, NSCH or NBCCH activity 
  interband_monitors - indicates if the QTA activity contains interband
                       monitors (monscan on a band different to the
                       serving cell band).
  imm_start -- TRUE: when calling from warmup ISR, it needs immediately
                     start cm_startup.
               FALSE: otherwise

===========================================================================*/
boolean gl1_hw_qta_fw_rf_init(boolean imm_start,
                              uint32  offset,
                              boolean qbta,
                              ARFCN_T arfcn,
                              boolean interband_monitors,
                              boolean is_first_burst,
                              gas_id_t gas_id)
{
  uint32 gap_margin;
  gsmtr_fn_qs_type current_fn_qs;
  uint32                    use_idle_rx_alpha = (uint32)FALSE;
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  MSG_GERAN_LOW_0_G("Calling gl1_hw_qta_fw_rf_init");

  gl1_hw_qta_mutex_lock( gas_id );

  if(!gl1_hw_cm_get_qta_wcdma_has_initialised(gas_id))
  {
    MSG_GERAN_HIGH_0_G("Can't start QTA gap - QTA startup not initialised");
    gl1_hw_qta_mutex_unlock( gas_id );
    return FALSE;
  }

  if(gl1_hw_get_qta_cm_fw_rf_init_status(gas_id))
  {
    MSG_GERAN_HIGH_0_G("qta fwrf already initialised");
    gl1_hw_qta_mutex_unlock( gas_id );
    return TRUE;
  }

  if(!l1_tskisr_blk->qta_in_progress)
  {
    gl1_hw_qta_mutex_unlock( gas_id );
    return FALSE;
  }

  if (gl1_hw_x2g_data[gas_id].qta_gap_for_g2x_irat)
  {
     set_qta_cleanup_pending(FALSE, gas_id);
     MSG_GERAN_HIGH_0_G("QTA gap for IRAT G fw/rf not initialised");
     gl1_hw_qta_mutex_unlock( gas_id );
     return TRUE;
  }

  /* If startup, cleanup, preload not pre-built then build now */
  if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded == FALSE)
  {
     gl1_hw_cm_build_startup_if_not_prebuilt (&gl1_qta_cm_rf_info, gas_id);
     MSG_GERAN_ERROR_0_G("QTA: generating preload script late");
  }

  if (gl1_hw_sleep_get_in_warmup_check(gas_id))
  {
    uint32 qs_to_ols = gl1_hw_sleep_get_qs_to_ols(gas_id);

    /* To issue QTA startup command in the same frame QTA gap starts */
    if (qs_to_ols > QS_PER_FRAME)
    {
      if ((qs_to_ols - QS_PER_FRAME) < QS_PER_FRAME)
      {
        uint32 wait_time_us = CONVERT_QS_TO_US(qs_to_ols - QS_PER_FRAME);

        wait_time_us += QTA_WAKEUP_WAIT_MARGIN_US;
        DALSYS_BusyWait(wait_time_us);
        MSG_GERAN_MED_1_G("Too early for QTA startup, wait for frame boundary %d us", wait_time_us);
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Wake up too early %d, do not init fw", qs_to_ols);
        gl1_hw_qta_mutex_unlock( gas_id );
        return FALSE;
      }
    }
    GSTMR_SET_HWFN(gas_id);
  }

  /* Only to abort if it is late for the first frame. */
  if (gl1_hw_x2g_data[gas_id].gl1_hw_first_gap_after_preload == TRUE)
  {
    /* gl1_hw_cm_nbr_pretune_offset_time margin is required for preload,
       GL1_QTA_CXM_MARGIN margin is required for startup. */
    if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_preload_sent == FALSE)
    {
      if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_nbr_pretune_offset_time > GL1_QTA_CXM_MARGIN)
      {
         gap_margin = gl1_hw_x2g_data[gas_id].gl1_hw_cm_nbr_pretune_offset_time;
      }
      else
      {
         gap_margin = GL1_QTA_CXM_MARGIN;
      }
    }
    else
    {
      gap_margin = GL1_QTA_CXM_MARGIN;
    }

    current_fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);
    /* QTA Startup is in the next frame */
    if (current_fn_qs.qs > (QS_PER_FRAME + offset - gap_margin))
    {
      MSG_GERAN_ERROR_3_G("Too late for QTA gap, do not init fw: qs %d, offset %d margin %d",current_fn_qs.qs,offset,gap_margin);
      gl1_hw_qta_mutex_unlock( gas_id );
      return FALSE;
     }
  }

  set_qta_cleanup_pending(FALSE, gas_id);
  gl1_hw_x2g_data[gas_id].gl1_qta_early_cleanup = FALSE;
  gl1_hw_set_qta_cm_fw_rf_init_status(gas_id,TRUE);

#if defined (FEATURE_QBTA)
  gl1_hw_set_qbta_scripts_status( gas_id, qbta);
#endif

  gl1_hw_qta_mutex_unlock( gas_id );

  current_fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);

  gl1_hw_x2g_data[gas_id].gl1_hw_cm_x2g_coex_params.arfcn = arfcn;
  gl1_hw_x2g_data[gas_id].gl1_hw_cm_x2g_coex_params.interband_monitors = interband_monitors;

  /*If we're in a QTA gap idle RX alpha for DR-DSDS must not be used*/
  mdsp_update_shared_variable(GFW_RX_ALPHA,&use_idle_rx_alpha,gas_id);

  MSG_GERAN_HIGH_3_G("start qta_fw_rf_init FN %d, qs%d, startup offset %d",current_fn_qs.fn,current_fn_qs.qs,offset);
  gl1_hw_cm_startup_geran(offset, &gl1_qta_cm_rf_info, is_first_burst, geran_map_gas_id_to_nas_id(gas_id));
  gl1_hw_x2g_data[gas_id].gl1_hw_first_gap_after_preload = FALSE;
  current_fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);
  MSG_GERAN_HIGH_2_G("end qta_fw_rf_init FN %d, qs%d",current_fn_qs.fn,current_fn_qs.qs);

  return TRUE;
}

/*===========================================================================

FUNCTION  gl1_hw_qta_fw_rf_deinit

DESCRIPTION
  This function deinitializes the firmware and RF after QTA operation, this
  must called before releasing TRM. This is currently run from task context.

  The X2G cleanup command is issued asynchronously and we wait around for
  sufficient time for GFW to pick up the command and execute it before
  returning. This is to ensure the cleanup is complete before handing TRM
  back to the oposite tech.

  There is room for improvement here : The cleanup command could be issued
  from the ISR and signal to L1 task in the TRM reserve message the timestamp
  at which the cleanup command was issued. From this L1 task could calculate
  how much longer it needs to wait around to allow the cleanup to complete
  before releasing TRM. The advantage of this is that the cleanup command can
  run in parallel with any L1 task latency rather than back to back - this
  would help optimize the QTA timeline.

===========================================================================*/
void gl1_hw_qta_fw_rf_deinit(gas_id_t gas_id, uint32 offset, boolean in_qbta)
{
  uint32 current_qs, offset_qs;
  boolean async_cmd;

  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);


  if(!gl1_hw_get_qta_cm_fw_rf_init_status(gas_id) )
  {
     if (gl1_hw_x2g_data[gas_id].qta_gap_for_g2x_irat)
     {
        MSG_GERAN_HIGH_0_G("QTA gap for IRAT, no fw/rf deinit");
     }
     return;
  }

  /* Must not be pre-empted between calculating the offset and issuing the command ! */
  REX_DISABLE_PREMPTION();

#if  defined (FEATURE_QBTA)
  gl1_hw_set_qbta_scripts_status(gas_id,in_qbta);
#endif
  current_qs = gstmr_rd_qsymbol_count_geran(as_id);

#if defined ( FEATURE_QTA ) && defined ( FEATURE_CXM_QTA )
  offset_qs = offset;
#else
  offset_qs = current_qs + GL1_HW_CM_QTA_ASYNC_CM_LATENCY_QS;

  if(offset_qs > (QS_PER_FRAME + GL1_HW_CM_GFW_CMD_ACTION_MARGIN_QS))
  {
    /* This scenario is not expected but if encountered this will happen in the next frame */
    mdsp_cm_cleanup(  (offset_qs - QS_PER_FRAME),
                     1,
                     FALSE,
                     gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle,
                     gas_id
                   );
    async_cmd = FALSE;
  }
  else
#endif
  {
    mdsp_cm_cleanup( offset_qs,
                     1,
                     FALSE,
                     gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle,
                     gas_id
                   );
    async_cmd = TRUE;

    /* Trigger the async command so that FW picks up the X2G Cleanup immediately */
#if defined (FEATURE_QBTA)
    if(!gl1_hw_get_qbta_scripts_status(gas_id))
#endif
    {
       mdsp_async_intf_send_immediate_cmd(GSTMR_GET_FN_GERAN(gas_id), FALSE, gas_id);
    }
  }

  REX_ENABLE_PREMPTION();

  MSG_GERAN_HIGH_3("qta fwrf deinit async %d, current_qs=%d offset_qs=%d", async_cmd, current_qs, offset_qs);

  gl1_hw_qta_mutex_lock( gas_id );

  gl1_hw_set_qta_cm_fw_rf_init_status(gas_id,FALSE);

  if(gl1_hw_x2g_data[gas_id].gl1_qta_early_cleanup == TRUE)
  {
     gl1_hw_x2g_data[gas_id].gl1_qta_early_cleanup = FALSE;
     gl1_hw_cm_qta_gap_exit(FALSE, gas_id);
  }

  /* Reset GFW mode to MEAS. We had set it to ACTIVE at CM startup cmd issue
      *  to avoid device mode chang   */
  gl1_set_gfw_app_mode_status( GFW_APP_MODE_IDLE, gas_id );

  gl1_hw_qta_mutex_unlock( gas_id );

}

boolean gl1_hw_get_qta_cm_fw_rf_init_status ( gas_id_t gas_id )
{
  MSG_GERAN_LOW_1(" QTA  get fw_rf_init_status: %d", gl1_hw_x2g_data[gas_id].gl1_qta_fw_rf_initialised);
  return (gl1_hw_x2g_data[gas_id].gl1_qta_fw_rf_initialised);
}

void gl1_hw_set_qta_cm_fw_rf_init_status (gas_id_t gas_id, boolean status )
{
  MSG_GERAN_HIGH_1(" QTA  set fw_rf_init_status: %d", status);
  gl1_hw_x2g_data[gas_id].gl1_qta_fw_rf_initialised = status;
}

boolean gl1_hw_cm_get_qta_wcdma_has_initialised(gas_id_t gas_id)
{
  return (gl1_hw_x2g_data[gas_id].gl1_qta_wcdma_has_initialised);
}

void gl1_hw_cm_set_qta_wcdma_has_initialised(boolean val, gas_id_t gas_id)
{
  gl1_hw_x2g_data[gas_id].gl1_qta_wcdma_has_initialised = val;
}

void gl1_hw_cm_set_qta_gap_for_g2x_irat(boolean val, gas_id_t gas_id)
{
  gl1_hw_x2g_data[gas_id].qta_gap_for_g2x_irat= val;
}

/*===========================================================================

FUNCTION  gl1_hw_cm_qta_gap_exit

DESCRIPTION
  This function completes the QTA gap exit

===========================================================================*/
void gl1_hw_cm_qta_gap_exit(boolean empty_gap, gas_id_t gas_id)
{
     gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded    = FALSE;
     gl1_hw_x2g_data[gas_id].gl1_hw_first_gap_after_preload = FALSE;
     if (!gl1_hw_x2g_data[gas_id].qta_gap_for_g2x_irat)
     {
        geran_l1_x2g_irat_exit ( &gl1_qta_cm_rf_info,
                                 empty_gap,
                                 geran_map_gas_id_to_nas_id(gas_id));

        MSG_GERAN_MED_1_G("QTA RF meas exit, abort=%d",empty_gap);
     }
     else
     {
        /* If G2x shutdown pending, then do it now */
        l1_sc_g2x_pending_shutdown(gas_id);
     }
     gl1_hw_cm_set_qta_wcdma_has_initialised(FALSE, gas_id);
}
#endif /* FEATURE_QTA */

/*===========================================================================

FUNCTION  gl1_hw_qta_gap_active

DESCRIPTION
  This function returns an indication of whether a QTA gap is currently open on that particular sub

  This is misleading , gap is not active until we get lock ?

===========================================================================*/
boolean gl1_hw_qta_gap_active(gas_id_t gas_id)
{
#ifdef FEATURE_QTA
  if (gl1_hw_cm_get_qta_wcdma_has_initialised(gas_id) || gpl1_get_grm_qta_state(gas_id))
  {
    return(TRUE);
  }
  else
#endif /* FEATURE_QTA */
  {
    return(FALSE);
  }
}


/*===========================================================================
FUNCTION  gl1_hw_get_qta_gap_for_g2x_irat

DESCRIPTION
  This function returns an indication of whether a QTA gap is open g2x irat on that particular sub

===========================================================================*/
boolean gl1_hw_get_qta_gap_for_g2x_irat(gas_id_t gas_id)
{
   return (gl1_hw_x2g_data[gas_id].qta_gap_for_g2x_irat);
}

#ifdef FEATURE_GSM_TDS

/*===========================================================================

FUNCTION  gl1_hw_x2g_startup

DESCRIPTION
  Calls to invoke the CM startup at the beginning of the gap.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_x2g_startup(uint16                      offset,
                        const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr,
                        uint32                      g_frame_number,
                        boolean                     x2g_rf_rxlm_update
#ifdef FEATURE_DUAL_SIM
                        ,sys_modem_as_id_e_type      as_id
#endif /*FEATURE_DUAL_SIM */
                        )
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/

   gl1_hw_x2g_startup_geran(offset, cm_rf_info_ptr, g_frame_number, x2g_rf_rxlm_update, as_id);
   return;
}

/*===========================================================================

FUNCTION  gl1_hw_x2g_startup

DESCRIPTION
  Calls to invoke the CM startup at the beginning of the gap.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_x2g_startup_geran(uint16                      offset,
                        const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr,
                        uint32                      g_frame_number,
                        boolean                     x2g_rf_rxlm_update,
                        sys_modem_as_id_e_type      as_id)
{
  uint32 x2g_fn_start;
  uint32 x2g_qs_start;
  uint32 sw_fn;

  uint32  x2g_fn_start_plus_one;
  gas_id_t       gas_id =  geran_map_nas_id_to_gas_id(as_id);

  GDRV_ISR_SAVE_LOCK(gas_id);

  /* initialization */
  send_err_msg            = FALSE;

  if( queued_x2g_startup )
  {
    send_err_msg = TRUE;

  return;
  }

  sw_fn = GSTMR_GET_FN_GERAN(gas_id);

  MSG_GERAN_HIGH_3_G("X2G Startup FN %d, offset %d, SW FN %d",g_frame_number, offset, sw_fn);

  /* Check for scritps prebuilt and startup too close to pre-load, reject gap if too close */
  if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded == FALSE)
  {
     MSG_GERAN_HIGH_0_G("X2G Startup before preload built");
     gl1_hw_x2g_build_startup_cleanup_script_geran (cm_rf_info_ptr, gas_id);
  }
  if (gl1_hw_x2g_data[gas_id].gl1_hw_first_gap_after_preload == TRUE)
  {
     uint32 time_after_preload;

     time_after_preload = offset + (SUB_FN(g_frame_number, gl1_hw_x2g_data[gas_id].gl1_hw_prebuilt_fn) * 5000);
     time_after_preload -= gl1_hw_x2g_data[gas_id].gl1_hw_preload_qs;
     if (time_after_preload < gl1_hw_x2g_data[gas_id].gl1_hw_cm_nbr_pretune_offset_time)
     {
       MSG_GERAN_HIGH_2_G("X2G Startup too close to preload FN %d, offset %d", gl1_hw_x2g_data[gas_id].gl1_hw_prebuilt_fn, 
                          gl1_hw_x2g_data[gas_id].gl1_hw_preload_qs);
       send_err_msg = TRUE;
       return;
     }
  }

  if (++t2g_api_profiling_index >= MAX_PROFILING_LOG)
  {
    t2g_api_profiling_index = 0;
  }

  gl1_profiling_t2g_api(t2g_api_profiling_index, STARTUP, TRUE,gas_id);

  t2g_api_profiling_struct[t2g_api_profiling_index].gap_time_info.fn = g_frame_number;
  t2g_api_profiling_struct[t2g_api_profiling_index].gap_time_info.qs = offset;

  x2g_fn_start = gl1_hw_x2g_offset_adjust(g_frame_number, offset, STARTUP,1,gas_id);
  x2g_qs_start = gstmr_rd_qsymbol_count_geran(as_id);
  /* Add correction in case frame and qs not from the same frame */
  if (x2g_fn_start != gstmr_rd_hw_frame_count_geran(as_id))
  {
     x2g_fn_start = ADD_FN( x2g_fn_start, 1);
     x2g_qs_start = gstmr_rd_qsymbol_count_geran(as_id);
  }

  if (x2g_fn_start != sw_fn)
  {
     MSG_GERAN_HIGH_3_G("X2G Startup: SW FN %d, not match HW FN %d (qs %d) ", sw_fn, x2g_fn_start, x2g_qs_start);
     x2g_fn_start = sw_fn;
     x2g_qs_start += QS_PER_FRAME;
  }

  x2g_fn_start_plus_one = ADD_FN( x2g_fn_start, 1);


  /* reset x2g key info */
  gl1_hw_x2g_startup_info.async_int_executed = FALSE;
  gl1_hw_x2g_cleanup_info.pending = FALSE;
  gl1_hw_x2g_startup_info.pending = TRUE;
  gl1_hw_x2g_pwr_meas_info.cleanup_pending = FALSE;

#ifdef FEATURE_GSM_TDS_DEBUG
  MSG_4(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "X2G STARTUP: current : GFN %d qs %d  - g_frame_number %d startup offset %d",
         x2g_fn_start, gstmr_rd_qsymbol_count_geran(as_id),g_frame_number, offset);
#endif

  /* check which frame we are in relative to the g_frame_number timing reference
     and determine whether there is sufficient time for FW to process the command
     and whether the command should be sent via an asynchronous interrupt or via
     normal FT processing*/
  if(g_frame_number == x2g_fn_start)
  {
    if(offset < (x2g_qs_start+GL1_HW_CM_GFW_CMD_ACTION_MARGIN_QS))
    {
      /* X2G_STARUP is too late, flag error and print error message */
      MSG_GERAN_ERROR_3_G("X2G startup cmd is too late: g_frame_number=%d; offset=%d; current qs=%d",
                 g_frame_number, offset, x2g_qs_start);

    send_err_msg = TRUE;

    }
    else if(offset > (QS_PER_FRAME+GL1_HW_CM_GFW_CMD_ACTION_MARGIN_QS))
    {
        offset -= QS_PER_FRAME;
        g_frame_number = ADD_FN(g_frame_number,1);

        MSG_GERAN_HIGH_3_G("X2G startup  - change GFN/offset for next ftick: new offset =%d; g_frame_number=%d; current_qs=%d",
                offset, g_frame_number, x2g_qs_start);
    }
  }
  else if(g_frame_number == x2g_fn_start_plus_one )
  {
    if(offset < GL1_HW_CM_GFW_CMD_ACTION_MARGIN_QS)
    {
      /* offset is relative to next FT, but is too close to start of frame, so must be scheduled in async interrupt before next FT.
         offset must also be adjusted to be relative to current frame tick */
      offset += QS_PER_FRAME;
      g_frame_number = SUB_FN(g_frame_number,1);
    }
  }
  else
  {
   /* Check for startup frame in the past */
   /* Take care of wrap around scenarios */
   if( IS_FRAME_NUM_LATER(x2g_fn_start , g_frame_number))
   {
    gl1_hw_x2g_startup_info.g_fn = 0xFFFFFFFF;
    MSG_GERAN_ERROR_3_G("X2G: startup FN %d has passed current FN %d - do nothing: startup FN set to %d",
              g_frame_number,x2g_fn_start,gl1_hw_x2g_startup_info.g_fn );

    send_err_msg = TRUE;
   }
  }

  if(IS_FRAME_NUM_LATER(g_frame_number, ADD_FN( GSTMR_GET_FN_GERAN(gas_id), 4)))
  {
     MSG_GERAN_ERROR_2_G("X2G: Do not add command to queue. Startup commands > 4 frames away , current_fn =%d , gap_start_fn = %d ", GSTMR_GET_FN_GERAN(gas_id ) , g_frame_number );
     send_err_msg = TRUE;
  }
  else if( IS_FRAME_NUM_LATER(g_frame_number, ADD_FN( GSTMR_GET_FN_GERAN(gas_id), 1))  )
  {
    /* Avoid allocating on the stack */
    static gl1_hw_x2g_startup_cmd_params_t  startup_cmd; /* static_dual_spaced_ignore */


    startup_cmd.g_frame_number          = g_frame_number;
    startup_cmd.gl1_hw_x2g_api_issue_fn = g_frame_number;

  /* Queue it one frame only */
  startup_cmd.offset     = offset;
  startup_cmd.cm_rf_info = *cm_rf_info_ptr;
  startup_cmd.cmd_id     = STARTUP;
  startup_cmd.x2g_rf_rxlm_update = x2g_rf_rxlm_update;

  gl1_hw_x2g_create_cmd_node( (gl1_hw_x2g_cmd_data_u *) &startup_cmd );

    MSG_GERAN_ERROR_3_G(" queued x2g startup fn %d g %d t %d ", sw_fn, g_frame_number, offset );

  queued_x2g_startup = TRUE;
  }

  /*deal with error flag or a queued startup*/
  if( send_err_msg || queued_x2g_startup  )
  {
    return;
  }

  gl1_hw_x2g_startup_info.g_fn = g_frame_number;
  gl1_hw_x2g_startup_info.offset = offset;

  if( offset < 10000  )
  {
  /*Add to scheduler */
    if( FALSE == gl1_hw_schedule_x2g_gap_startup( offset,gas_id ) )
    {
      /* No need to unlock mutex because TL1 will continue calling cleanup */

      /* Send a message to TL1 */
    send_err_msg = TRUE;
      MSG_GERAN_ERROR_0_G(" conflict !!!! ");
    return;
  }

#ifdef FEATURE_GSM_TDS_DEBUG
    MSG_GERAN_HIGH_2_G("X2G startup: stored values: offset =%d; g_frame_number=%d",
              offset, g_frame_number);
#endif

    if ( mdsp_commands_pending(gas_id) )
    {
      MSG_GERAN_ERROR_0_G("X2G startup: MDSP Command buffer not empty");
    }

   if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded == FALSE)
   {
    rf_rxlm_update = x2g_rf_rxlm_update;
   }

    /* core CM startup processing */
    gl1_hw_cm_startup_geran( offset, cm_rf_info_ptr, TRUE, as_id);
    gl1_hw_x2g_startup_info.pending = FALSE;

  #ifdef FEATURE_GSM_TDS_DEBUG
    x2g_fn_start = gstmr_rd_hw_frame_count_geran(as_id);
    x2g_qs_start = gstmr_rd_qsymbol_count_geran(as_id);
    MSG_GERAN_HIGH_3_G("X2G STARTUP: current : GFN %d qs %d odd_even %d", x2g_fn_start, x2g_qs_start, gstmr_rd_odd_even());
  #endif

  }
  else
  {
    MSG_GERAN_ERROR_1_G("X2G Offset = %d  (> 10000)", offset);
  }

  gl1_profiling_t2g_api(t2g_api_profiling_index, STARTUP, FALSE,gas_id);

} /* gl1_hw_x2g_startup_geran */
#endif
/*===========================================================================

FUNCTION  gl1_hw_cm_build_cleanup_script

DESCRIPTION
  Call to build CM RF cleanup script

DEPENDENCIES

RETURN VALUE
SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_cm_build_cleanup_script (gas_id_t gas_id)
{
  static rfm_meas_setup_param_type     cleanup_rfm_meas_gsm_setup; /* static_dual_spaced_ignore */

  cleanup_rfm_meas_gsm_setup.setup_params               = gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr ;

  if(cleanup_rfm_meas_gsm_setup.setup_params->script_type == RFM_MEAS_PROGRAM_NULL2X)
  {
     MSG_GERAN_MED_0_G("X2G: NULL2X, so no RF cleanup");
  }
  else
  {
     cleanup_rfm_meas_gsm_setup.setup_params->script_type  = RFM_MEAS_PROGRAM_CLEANUP;
     if( RFM_MEAS_COMMON_SUCCESS != rfm_meas_common_script_build_scripts( &cleanup_rfm_meas_gsm_setup, NULL ) )
     {
#ifdef FEATURE_QSH_MDUMP
       QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
       ERR_GERAN_FATAL_0_G(" cleanup rfm failed ");
     }
  }
}

/*===========================================================================

FUNCTION  gl1_hw_cm_cleanup

DESCRIPTION
  Call to invoke the CM cleanup at the end of the gap.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_cm_cleanup ( uint16 offset
#ifdef FEATURE_DUAL_SIM
                                ,sys_modem_as_id_e_type      as_id
#endif /* FEATURE_DUAL_SIM*/
)
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/

   gl1_hw_cm_cleanup_geran(offset, as_id);
   return;
}
/*===========================================================================

FUNCTION  gl1_hw_cm_cleanup_geran

DESCRIPTION
  Call to invoke the CM cleanup at the end of the gap.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_cm_cleanup_geran ( uint16 offset,sys_modem_as_id_e_type      as_id)
{
  uint8  frames_delayed;
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

#ifdef  FEATURE_GSM_TDS
  if( !cm_startup_issued )
  {
    send_err_msg = TRUE;
    return;
  }
#endif

  if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded == FALSE)
  {
     gl1_hw_cm_build_cleanup_script(gas_id);
  }

#ifdef FEATURE_GSM_TDS_DEBUG
  MSG_GERAN_HIGH_3_G("CM_CLEANUP: - issue cleanup offset %d qs %d GFN %d", offset,gstmr_rd_qsymbol_count(gas_id),GSTMR_GET_FN_GERAN(gas_id));
#endif

  gfwhost_cmdbuf_offsets[gas_id].cleanup_ptr=(GfwX2GCleanupCmd*)mdsp_cm_cleanup( offset,
                   1,
                   FALSE,
                   gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->target_param.gsm_params.rx_dev_handle[DEV_HANDLE_PRX].lm_handle,
                   gas_id);

  /* Restore the ftsm value to what it was before startup was called*/
  mdsp_set_ftsm_geran( prev_frame_tick_setup_margin , gas_id );

#ifdef  FEATURE_GSM_TDS
  /* For TD only send rf exit here if cleanup not sent from x2g cleanup function */
  if( RFCOM_TDSCDMA_MODE != gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode || gl1_hw_x2g_cleanup_info.pending )
#endif
  {
    /* issue x2g commands immediately to avoid command being delayed by RF processing */
    mdsp_issue_async_msg(GFW_ASYNC_CMD, gas_id);
  }
  /* Let the mdsp finish its work before turning off the clock.  Three frames
     seems like overkill but in some cases is required. */
  frames_delayed = GL1_CM_CLEANUP_DELAY_FRAMES;
  gl1_hw_cm_cleanup_done_delay(frames_delayed,gas_id);

  gl1_hw_log_cell_cmds_to_diag (cmd_cm_cleanup, FALSE,gas_id);

#ifdef FEATURE_GSM_TDS
  gl1_hw_x2g_cleanup_info.pending = FALSE;
  cm_startup_issued = FALSE;
#endif

}

#ifdef FEATURE_QTA
/*===========================================================================

FUNCTION  gl1_hw_cm_build_startup_if_not_prebuilt

DESCRIPTION
  If startup not already pre-built then build startup and cleanuup scripts

===========================================================================*/
void gl1_hw_cm_build_startup_if_not_prebuilt (const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr, gas_id_t gas_id)
{
 if (gl1_hw_x2g_data[gas_id].qta_gap_for_g2x_irat == FALSE)
 {
   gl1_hw_qta_mutex_lock( gas_id );
   MSG_GERAN_HIGH_1_G("QTA: Build startup scripts, preloaded %d",gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded);
   if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded == FALSE)
   {
      gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded = TRUE;
      gl1_hw_qta_mutex_unlock( gas_id );
      gl1_hw_x2g_build_startup_cleanup_script_geran (cm_rf_info_ptr, gas_id);
   }
   else
   {
      gl1_hw_qta_mutex_unlock( gas_id );
   }
 }
}
#endif

/*===========================================================================
FUNCTION  gl1_hw_x2g_build_startup_cleanup_script

DESCRIPTION
  Calls pre-build CM RF startup and cleannup scripts.

===========================================================================*/
void gl1_hw_x2g_build_startup_cleanup_script_geran (const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr,
                                                    gas_id_t gas_id)
{
  boolean build_cleanup = FALSE;

  if( NULL != cm_rf_info_ptr )
  {
    /* Lock QTA mutex whilst building startup script to reduce chance of startup being sent to FW before preload */
    gl1_hw_qta_mutex_lock( gas_id );
    if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded == FALSE || gl1_hw_x2g_data[gas_id].gl1_hw_first_gap_after_preload == FALSE)
    {
       build_cleanup = TRUE;
       gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded = TRUE;
       gl1_hw_x2g_data[gas_id].gl1_hw_first_gap_after_preload = TRUE;
       gl1_hw_cm_build_startup_script(cm_rf_info_ptr, gas_id);
       MSG_GERAN_HIGH_0_G("CM startup script pre built");
    }
    gl1_hw_qta_mutex_unlock( gas_id );

    /* Send preload command to FW */
    if ( (gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode == RFM_TDSCDMA_MODE) && (gl1_hw_qta_gap_active(gas_id) == FALSE)
          && (gl1_hw_x2g_data[gas_id].gl1_hw_cm_nbr_pretune_offset_time != 0))
    {
       GDRV_ISR_SAVE_LOCK(gas_id);
       if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_preload_sent == FALSE)
       {
          gsmtr_fn_qs_type current_fn_qs;
          uint16 preload_offset = (GL1_HW_CM_GFW_PRELOAD_CMD_ACTION_MARGIN_QS/2);

          /* read current FN and qs */
          current_fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);
          preload_offset += current_fn_qs.qs;

          mdsp_cm_preload(preload_offset,
                          0,
                          cm_rf_info_ptr->rxlm_gsm_buf_idx,
                          cm_rf_info_ptr->target_rf_device,
                          TRUE,
                          cm_rf_info_ptr->is_a_qta_gap,
                          FALSE,
                          gl1_hw_x2g_data[gas_id].gl1_hw_cm_x2g_coex_params,
                          gas_id);
          mdsp_async_intf_send_immediate_cmd(current_fn_qs.fn, FALSE, gas_id);

          MSG_GERAN_HIGH_2_G("Immediate cmd for X2G preload, FN %d, offset %d", current_fn_qs.fn, preload_offset);

          gl1_hw_x2g_data[gas_id].gl1_hw_cm_preload_sent = TRUE;
          gl1_hw_x2g_data[gas_id].gl1_hw_preload_qs = preload_offset + (GL1_HW_CM_GFW_PRELOAD_CMD_ACTION_MARGIN_QS/2);
          gl1_hw_x2g_data[gas_id].gl1_hw_prebuilt_fn = current_fn_qs.fn;
       }
       GDRV_ISR_SAVE_UNLOCK(gas_id);
    }
    if (build_cleanup == TRUE)
    {
       gl1_hw_cm_build_cleanup_script(gas_id);
       MSG_GERAN_HIGH_0_G("CM cleanup script pre built");
    }
  }
  else
  {
    MSG_GERAN_ERROR_0_G("NULL cm_rf_info_ptr for pre build scripts");
  }

}

/*===========================================================================
FUNCTION  gl1_hw_x2g_build_startup_cleanup_script

DESCRIPTION
  Calls pre-build CM RF startup and cleannup scripts.

===========================================================================*/
void gl1_hw_x2g_build_startup_cleanup_script (const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr,
                                              boolean x2g_rf_rxlm_update
#ifdef FEATURE_DUAL_SIM
                                              ,sys_modem_as_id_e_type      as_id
#endif /*FEATURE_DUAL_SIM*/
                        )
{

#ifdef FEATURE_DUAL_SIM
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#else
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif /*FEATURE_DUAL_SIM*/

  if (gl1_hw_cm_get_status(gas_id) == FALSE)
  {
    MSG_GERAN_ERROR_0_G("CM pre build request, but CM not initialised");
  }
  else
  {
#ifdef FEATURE_GSM_TDS
     rf_rxlm_update = x2g_rf_rxlm_update;
#endif
     gl1_hw_x2g_build_startup_cleanup_script_geran(cm_rf_info_ptr, gas_id);
  }
  return;
}

#ifdef FEATURE_GSM_TDS
/*===========================================================================

FUNCTION  gl1_hw_x2g_cleanup

DESCRIPTION
  Call to invoke the CM cleanup at the end of the gap.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_x2g_cleanup ( uint32 offset, uint32 g_frame_number
#ifdef FEATURE_DUAL_SIM
                                ,sys_modem_as_id_e_type      as_id
#endif /*FEATURE_DUAL_SIM */
                             )
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/

   return gl1_hw_x2g_cleanup_geran(offset,g_frame_number,as_id);
}
/*===========================================================================

FUNCTION  gl1_hw_x2g_cleanup_geran

DESCRIPTION
  Call to invoke the CM cleanup at the end of the gap.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_x2g_cleanup_geran ( uint32 offset, uint32 g_frame_number,sys_modem_as_id_e_type      as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  uint32  gfn     = gstmr_rd_hw_frame_count_geran(as_id);
  boolean return_value;
  gl1_hw_x2g_cleanup_info.offset = offset;

  if( queued_x2g_startup )
  {
    /* Avoid allocating on the stack */
    static gl1_hw_x2g_cleanup_cmd_params_t  cleanup_cmd; /* static_dual_spaced_ignore */

    cleanup_cmd.g_frame_number          = g_frame_number;
    cleanup_cmd.gl1_hw_x2g_api_issue_fn = g_frame_number;

    /* Queue it one frame only */
    cleanup_cmd.offset     = offset;
    cleanup_cmd.cmd_id     = CLEANUP;

    gl1_hw_x2g_create_cmd_node( (gl1_hw_x2g_cmd_data_u *) &cleanup_cmd );

    MSG_GERAN_HIGH_3_G(" queued x2g cleanup fn %d g %d t %d ", GSTMR_GET_FN_GERAN(gas_id), g_frame_number, offset );

    queued_x2g_startup = FALSE;

    GDRV_ISR_SAVE_UNLOCK(gas_id);

    return  TRUE;
  }

  gl1_profiling_t2g_api(t2g_api_profiling_index, CLEANUP, TRUE,gas_id);

#ifdef FEATURE_GSM_TDS_DEBUG
  MSG_5(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "T2G Cleanup offset %d frame number %d : GFN %d, qs %d async int. %d",
        offset ,g_frame_number,gfn,gstmr_rd_qsymbol_count_geran(as_id), gl1_hw_x2g_startup_info.async_int_executed);
#endif


  if( IS_FRAME_NUM_LATER(gfn , g_frame_number))
  {
    MSG_GERAN_ERROR_2_G("X2G: current FN %d, cleanup FN %d has passed - do nothing",gfn,g_frame_number);

    /* We haven't issued any commands yet to the FW */
    send_err_msg = TRUE;
  }

  if( !send_err_msg )
  {
#ifdef FEATURE_GSM_TDS_DEBUG
  MSG_GERAN_HIGH_1_G("X2G cleanup: last_state %d",gl1_t2g_last_state);
#endif
  /* async int has not yet fired, so still time to add x2g_cleanup to buffer */
  if(gl1_hw_x2g_startup_info.async_int_executed == FALSE)
  {
    /* reset x2g_cleanup offset to be relative to x2g_startup g_frame_number */
    int32   offset_adjust = gl1_hw_x2g_offset_adjust(g_frame_number,(uint16)offset,CLEANUP,1,gas_id);

    offset += offset_adjust * QS_PER_FRAME;

  if( FALSE == gl1_hw_schedule_x2g_gap_cleanup( offset, TRUE,gas_id ) )
    {
      /* Send a message to TL1 */
      MSG_GERAN_ERROR_0_G(" X2G cleanup conflict (async int not fired) ");

    send_err_msg = TRUE;
    }
    /* if re-adjusted offset is > 2 frames, x2g cleanup will need to wait until a later FT */
    if(offset < (2*QS_PER_FRAME))
    {
      /* core CM cleanup handling */
      gl1_hw_cm_cleanup_geran(offset,as_id);

      if(gl1_t2g_last_state == TONE_DETECT)
      {
        cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.cleanup_async_int_used = TRUE;
      }
      else if (gl1_t2g_last_state == SCH_DETECT )
      {
        sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.cleanup_async_int_used = TRUE;
      }

      /* send all loaded commands to MDSP */
      gl1_hw_x2g_send_commands(gas_id);

      if (!send_err_msg)
      {
        if(gl1_t2g_last_state == PWR_MEAS)
        {
          uint8 i;
          for( i=0;i<POWER_MEAS_NUM_SM;i++)
          {
            if( cm_pwr_meas_state_machine[i].power_meas_state == POWER_MEAS_STATE_COMMAND)
          {
              cm_pwr_meas_state_machine[i].power_meas_state = POWER_MEAS_STATE_ACTIVE;
          }
          }
        }
      }
    }
    /* X2G_cleanup must be stored for a later FT */
    else
    {
    /* send all loaded commands to MDSP anyway - Cleanup can go out via the handler */
      gl1_hw_x2g_send_commands(gas_id);
      gl1_hw_x2g_cleanup_info.pending = TRUE;
      switch( gl1_t2g_last_state )
      {
      case PWR_MEAS:
        gl1_hw_x2g_pwr_meas_info.cleanup_pending = TRUE;
        break;

      case TONE_DETECT:
         cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.cleanup_pending = TRUE;
         break;

      case SCH_DETECT:
         sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.cleanup_pending = TRUE;
         break;

      default:
         break;
      }
    }
  }
  else // Async int has gone, so this must be a cleanup after acquisition, so issue commands for next FT
  {
    /* Assume that X is able to schedule the command for the next FT, so the async
     int is not required. Adjust the offset relative to the next GSM frame (where the
     command is scheduled for) */
    if(offset > QS_PER_FRAME)
    {
      offset -= QS_PER_FRAME;
    }
    else
    {
      offset = 0;
    }

  if( FALSE == gl1_hw_schedule_x2g_gap_cleanup( offset, FALSE,gas_id ) )
    {
      MSG_GERAN_ERROR_0_G(" X2G Cleanup conflict !!!! ");
      send_err_msg = TRUE;
    }

    gl1_hw_cm_cleanup_geran(offset,as_id);
  }

  }  /* if( !send_err_msg ) */

  /* only install handlers if we issued cmds to GFW */
  if (!send_err_msg)
  {
    switch(gl1_t2g_last_state)
    {
    case PWR_MEAS:
      {
        /* Register the remaining power measures handler */
        if (!gstmr_is_handler_registered_geran(gl1_hw_x2g_pwr_meas_ft_handler,as_id))
        {
    #ifdef FEATURE_GSM_TDS_DEBUG
        MSG_GERAN_HIGH_2_G("T2G PWR MEAS HAndler: Install handler GFN %d qs %d ",GSTMR_GET_FN_GERAN(gas_id), gstmr_rd_qsymbol_count_geran(as_id));
    #endif
          gstmr_register_handler_geran(gl1_hw_x2g_pwr_meas_ft_handler,as_id);
        }
      }
      break;

    case TONE_DETECT:
      {
        /* Register the acq handler */
        if (!gstmr_is_handler_registered_geran(gl1_hw_x2g_acq_ft_handler,as_id))
        {
          gstmr_register_handler_geran(gl1_hw_x2g_acq_ft_handler,as_id);
    #ifdef FEATURE_GSM_TDS_DEBUG
          MSG_GERAN_HIGH_2_G("T2G ACQ HANDLER: Install handler GFN %d qs %d ", GSTMR_GET_FN_GERAN(gas_id), gstmr_rd_qsymbol_count_geran(as_id));
    #endif
        }
      }
      break;

    case SCH_DETECT:
      {
        /* Register the acq handler */
        if (!gstmr_is_handler_registered_geran(gl1_hw_x2g_sch_ft_handler,as_id))
        {
          gstmr_register_handler_geran(gl1_hw_x2g_sch_ft_handler,as_id);
    #ifdef FEATURE_GSM_TDS_DEBUG
          MSG_GERAN_HIGH_2_G("T2G SCH HANDLER: Install handler GFN %d qs %d", GSTMR_GET_FN_GERAN(gas_id), gstmr_rd_qsymbol_count_geran(as_id));
    #endif
        }
      }
      break;

     default:
        MSG_GERAN_ERROR_1_G("T2G CLeanup: last state %d not set correctly",gl1_t2g_last_state);
      break;

    }

    gl1_hw_x2g_update_info(gl1_t2g_last_state, USED);
  }     /* if(!send_err_msg) */
  else  /* error message is set to remove */
  {
    gl1_hw_x2g_update_info(gl1_t2g_last_state, ABORTING);

    switch(gl1_t2g_last_state)
    {
    case PWR_MEAS:
      {
        gl1_hw_x2g_rssi_meas_ctl_t pwr_meas_err_msg;
        uint16 i = 0, index = 0;
        pwr_meas_err_msg.error_flag = TRUE;
        pwr_meas_err_msg.gl1_skip_last_burst=FALSE;
        index = gl1_hw_x2g_get_index(PWR_MEAS, ABORTING);
        pwr_meas_err_msg.meas.num   = X2g_Cmd_Info[index].num_uarfcns;

        /* Klockwork - ensure no array overflow */
        if (pwr_meas_err_msg.meas.num > MAX_CM_RSSI_MEASUREMENTS_REQ)
    {
           pwr_meas_err_msg.meas.num = MAX_CM_RSSI_MEASUREMENTS_REQ;
    }

  for(i = 0; i < pwr_meas_err_msg.meas.num; i++)
    {
          pwr_meas_err_msg.meas.ARFCN[i] = X2g_Cmd_Info[i].UARFCN[i];
    }

        pwr_meas_err_msg.seq_num = X2g_Cmd_Info[index].message_id;

        if( cm_pwr_meas_state_machine[cm_pwr_sm].power_meas_state == POWER_MEAS_STATE_COMMAND )
    {
          cm_pwr_meas_state_machine[cm_pwr_sm].power_meas_state = POWER_MEAS_STATE_INACTIVE;
    }

        L1_send_TDS_RSSI_MEAS_RSP(&pwr_meas_err_msg, as_id);

        gl1_hw_x2g_clear_info(X2g_Cmd_Info[index].message_id, PWR_MEAS);

        break;
      }

    case TONE_DETECT:
      {
        gsm_irat_acquisition_rsp_t acq_err_msg;
        uint16 index = 0;

        acq_err_msg.schedule_error = TRUE;
        acq_err_msg.error_flag = TRUE;
        acq_err_msg.tone_detect = FALSE;
        acq_err_msg.sch_crc_pass = FALSE;

        acq_err_msg.acq_start_frame      = cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_store.acq_start_frame ;
        acq_err_msg.acq_win_start_offset = cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_store.acq_win_start_offset ;
        acq_err_msg.gsm_cell_ptr         = cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_store.gsm_cell_ptr  ;

        index = gl1_hw_x2g_get_index(TONE_DETECT, ABORTING);

        acq_err_msg.message_id   = X2g_Cmd_Info[index].message_id;
        acq_err_msg.arfcn = X2g_Cmd_Info[index].UARFCN[0];

#ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
        if (cm_acq_state_machine[cm_acq_sm].acq_meas_state == ACQ_MEAS_STATE_ACTIVE)
        {
           cm_acq_state_machine[cm_acq_sm].acq_meas_state = ACQ_MEAS_STATE_INACTIVE;

           /* Remove cm_acq_sm from the end of the array */
           cm_acq_state_machine_active_array[cm_acq_state_machine_num_active] = 0xFF;
           cm_acq_state_machine_num_active -= 1;
        }
#else
        cm_acq_state_machine[cm_acq_sm].acq_meas_state = ACQ_MEAS_STATE_INACTIVE;
#endif
        L1_send_TDS_ACQ_RSP  (&acq_err_msg, as_id);

        cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_rsp_sent = TRUE;

        gl1_hw_x2g_clear_info(X2g_Cmd_Info[index].message_id, TONE_DETECT);


        break;
      }

    case SCH_DETECT:
      {
        gsm_irat_sch_burst_rsp_t sch_err_msg;
        gl1_defs_rx_hdr_struct   empty_header_struct;
        uint8                    empty_data[MAX_SCH_OCTETS] = {0,0,0,0};
        gl1_defs_metrics_rpt     empty_metrics_struct;
        uint16 index = 0;

        empty_header_struct.good_data = 0;
        empty_metrics_struct.timing_offset = 0;
        sch_err_msg.error_flag = TRUE;
	sch_err_msg.schedule_error = TRUE;
        sch_err_msg.crc_pass   = FALSE;

        index = gl1_hw_x2g_get_index(SCH_DETECT, ABORTING);

        sch_err_msg.message_id = X2g_Cmd_Info[index].message_id;
        sch_err_msg.arfcn      = X2g_Cmd_Info[index].UARFCN[0];

        sch_err_msg.gsm_cell_ptr = sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_store.gsm_cell_ptr;
        sch_err_msg.reconfirm    = sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_store.reconfirm;
        sch_err_msg.sch_frame    = sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_store.sch_frame;

        sch_state_machine[cm_sch_sm].sch_meas_state = SCH_MEAS_STATE_INACTIVE;

        L1_send_TDS_SCH_MEAS_RSP( &sch_err_msg,
                                  &empty_header_struct,
                                  empty_data,
                                  &empty_metrics_struct,
                                  as_id );

        sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_rsp_sent = TRUE;

        gl1_hw_x2g_clear_info(X2g_Cmd_Info[index].message_id, SCH_DETECT);

        break;
      }

    default:
      MSG_GERAN_ERROR_1_G("T2G : Cmds not sent but unknown state %d no error mssage sent to TL1 ", gl1_t2g_last_state);
      break;

    }

  }

  /* This is done here instead of in gl1_hw_x2g_offset_adjust()
   * so that states can be used in this function
   */
  gl1_t2g_last_state = NONE_GL1;


  if( send_err_msg )
  {
    gl1_hw_schedule_remove_x2g_startup(gas_id );
    cm_startup_issued = FALSE;
    mdsp_clear_gap_cmds(gas_id);
    MSG_GERAN_ERROR_0_G("X2G Gap aborted - clear mdsp cmds");
    /*reset flag*/
    send_err_msg = FALSE;
    return_value = FALSE;
  }
  else
  {
    gl1_hw_x2g_data[gas_id].gl1_hw_first_gap_after_preload = FALSE;
    return_value = TRUE;
  }

  GDRV_ISR_SAVE_UNLOCK(gas_id);
  return return_value;

}

#endif

/*===========================================================================

FUNCTION  gl1_hw_cm_gsm_init

DESCRIPTION
  Function to initialize the GSM Layer 1 interfaces that are required for
  compressed mode operation.  Interfaces include GSTMR, MDSP, RF.
  This function should be invoked whenever the WCDMA stack initializes.
  It also registers a TDMA frame processing handler.

===========================================================================*/

extern void gl1_task_mutex_lock( gas_id_t gas_id );
extern void gl1_task_mutex_unlock( gas_id_t gas_id );
boolean gl1_hw_force_set_app_mode_cmd = FALSE;

void gl1_hw_cm_gsm_init
(
  void           (* tdma_isr_ptr)(sys_modem_as_id_e_type as_id),
  rex_tcb_type*  task_ptr,
  void           (* task_wait_handler)(rex_sigs_type),
  rex_sigs_type  task_wait_sig,
  rex_sigs_type task_wait_sig_1

#ifdef FEATURE_DUAL_SIM
  ,sys_modem_as_id_e_type as_id
#endif /*FEATURE_DUAL_SIM*/
)
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/

     rfcom_mode_enum_type x2g_serving_rat;

   /*Need to know calling function is TD or not*/
  if (rcinit_lookup_rextask("tds_l1") == task_ptr   )
  {
      x2g_serving_rat = RFM_TDSCDMA_MODE ;
  }else if((rcinit_lookup_rextask("wcdma_l1"  ) == task_ptr   )||
           (rcinit_lookup_rextask("wcdma_l1_0") == task_ptr   )||
           (rcinit_lookup_rextask("wcdma_l1_1") == task_ptr   ))
  {
      x2g_serving_rat =  RFM_IMT_MODE;
  }
  else
  {
      x2g_serving_rat =  RFM_LTE_MODE;

  }

  return gl1_hw_cm_gsm_init_geran(tdma_isr_ptr,task_ptr,task_wait_handler,task_wait_sig,task_wait_sig_1,as_id , x2g_serving_rat );

}

/*===========================================================================

FUNCTION  gl1_hw_cm_gsm_init_geran

DESCRIPTION
  Function to initialize the GSM Layer 1 interfaces that are required for
  compressed mode operation.  Interfaces include GSTMR, MDSP, RF.
  This function should be invoked whenever the WCDMA stack initializes.
  It also registers a TDMA frame processing handler.

===========================================================================*/
void gl1_hw_cm_gsm_init_geran
(
  void           (* tdma_isr_ptr)(sys_modem_as_id_e_type as_id),
  rex_tcb_type*  task_ptr,
  void           (* task_wait_handler)(rex_sigs_type),
  rex_sigs_type  task_wait_sig,
  rex_sigs_type task_wait_sig_1,
  sys_modem_as_id_e_type as_id ,
  rfcom_mode_enum_type x2g_serving_rat
)
{

  boolean gfw_asleep;
  boolean frame_tick_running;

  uint32  fws_status = 0;
  uint32 fws_disable_mask = 0;

  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  MSG_GERAN_HIGH_2_G("Initializing compressed mode %d as_id:%d",
                     gl1_hw_cm_status[gas_id].initialised, as_id  );

  gl1_hw_cm_status[gas_id].ActionValue = Initializing;

  gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded    = FALSE;
  gl1_hw_x2g_data[gas_id].gl1_hw_first_gap_after_preload = FALSE;

  gl1_task_mutex_lock(gas_id);

  fws_disable_mask = mdsp_get_fws_app_mask( gas_id );

   MSG_GERAN_HIGH_2_G("GERAN FWS APPs enabled: %x fws_disable_mask:%x ",gl1_hw_fws_app_enabled(),fws_disable_mask);

   if(gl1_hw_fws_app_enabled() & fws_disable_mask)
   {
     fws_status = fws_app_disable(fws_disable_mask);
     gl1_hw_force_set_app_mode_cmd = TRUE;
     MSG_GERAN_HIGH_3_G("GERAN FWS APP Disable status %d [%d]",fws_status,GSTMR_GET_FN_GERAN(gas_id),gl1_hw_force_set_app_mode_cmd);
   }

  /* reset gl1_hw layer */
  gl1_hw_reset(gas_id);

  /* Init the gstmr */
  gstmr_init( gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* advise the DRX module that Compressed Mode is starting */
  (void)gl1_drx_cm_starting(gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  /* always asserts sleep at the end of this function */


#ifdef FEATURE_GSM_TDS
  /*Need to know calling function is TD or not*/
  if( x2g_serving_rat == RFM_TDSCDMA_MODE )
  {
    gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode = RFCOM_TDSCDMA_MODE;
    MSG_GERAN_HIGH_1_G("gl1_hw_cm_gsm_init for T2G, RAT=%d", x2g_serving_rat);
  }
  else
#endif /* FEATURE_GSM_TDS */
    {
      if(x2g_serving_rat == RFM_LTE_MODE)
      {
         gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode = RFCOM_LTE_MODE;
         MSG_GERAN_HIGH_1_G("gl1_hw_cm_gsm_init for L2G, RAT=%d", x2g_serving_rat);
      }
      else
      {
         gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode = RFCOM_WCDMA_MODE;
         MSG_GERAN_HIGH_1_G("gl1_hw_cm_gsm_init for W2G, RAT=%d", x2g_serving_rat);
      }
    }

/* Init the CM mDSP */
  gl1_hw_cm_mdsp_init(task_ptr, task_wait_handler, task_wait_sig, task_wait_sig_1,gas_id);

  gl1_hw_cm_rf_init(gas_id);

  /* Check FW/SW feature compatibility*/
  mdsp_feature_check (gas_id);

  /* Update EFS status to MDSP shared variable used for CM */
  gl1_hw_cm_efs_update(gas_id);

  /* Allow RF generic config command to be sent */
  frame_tick_running = mdsp_wait_for_frame_tick_non_blocking( task_ptr, task_wait_sig_1,gas_id );
  if(!frame_tick_running)
  {
    MSG_GERAN_ERROR_0_G("gl1_hw_cm_gsm_init frame tick not running");
  }

  /* Have ISR lock to force wait for ISR to complete before sending GFW sleep */
  GDRV_ISR_SAVE_LOCK(gas_id);

  /* Set after generic config commands have been sent */
  mdsp_set_mdsp_compressed_mode(gas_id);

  /* Now that initialization is finished we can install the normal frame */
  /* tick handler.                                                       */
  if (tdma_isr_ptr != NULL)
  {
    gstmr_register_handler_geran(tdma_isr_ptr,as_id);
  }

#ifdef FEATURE_GSM_TDS
  /*Need to know calling function is TD or not*/
  if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode == RFCOM_TDSCDMA_MODE)
  {
    gl1_hw_x2g_empty_cmd_queue( );
    gstmr_register_handler_geran( gl1_hw_x2g_ftick_hdlr,as_id );
  }
#endif
  gl1_hw_log_cell_cmds_to_diag (cmd_cm_gsm_init, FALSE,gas_id);

#ifdef FEATURE_GSM_TDS
  if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode != RFCOM_TDSCDMA_MODE)
#endif
  {
    gfw_asleep = mdsp_prepare_for_sleep(gas_id);
    MSG_GERAN_MED_1_G(GL1_SLEEP_HDR" CM GFW asleep %d",gfw_asleep);
  }

  GDRV_ISR_SAVE_UNLOCK(gas_id);

  gl1_hw_x2g_data[gas_id].gl1_hw_cm_gap_complete = TRUE;

  gl1_hw_cm_status[gas_id].initialised = TRUE;

  gl1_hw_force_set_app_mode_cmd = FALSE;

  gl1_task_mutex_unlock(gas_id);
  gl1_hw_cm_status[gas_id].ActionValue = Initialized;

  gl1_hw_cm_status[gas_id].TimestampOn = timetick_get_safe();
#ifdef FEATURE_GSM_COEX
  gl1_hw_arbitration[gas_id].registration(TRUE, gas_id);
#endif

}

boolean gl1_hw_cm_get_status ( gas_id_t gas_id )
{
  return gl1_hw_cm_status[gas_id].initialised;
}

/*===========================================================================

FUNCTION gl1_hw_cm_mdsp_init

DESCRIPTION
  Enable and initializes the mDSP

===========================================================================*/
static void gl1_hw_cm_mdsp_init (rex_tcb_type  *task_ptr,
                                 void          (*task_wait_handler)(rex_sigs_type),
                                 rex_sigs_type task_wait_sig,
                                 rex_sigs_type task_timeout_sig,
                                 gas_id_t  gas_id)
{
  uint8 i;

  /* Init the GSM mDSP */
  mdsp_enable(gas_id);

  GDRV_ISR_SAVE_LOCK(gas_id);

  mdsp_init_MdspHostIfBaseStore(gas_id);
  mdsp_init_public_data_space(gas_id);
  mdsp_init_private_data_space(gas_id);

  GDRV_ISR_SAVE_UNLOCK(gas_id);

  mdsp_init(gas_id);

  /* Initialize the MDSP driver before the RF and after GSTMR,
     the firmware should already be loaded by WCDMA L1 */
  mdsp_cm_init(gas_id);

  /* Clear CM SCH tags */
  for (i=0; i<Gl1_HW_CM_NUM_SCH_METRICS; i++)
  {
     gl1_hw_cm_sch_tags[gas_id][i].metrics_index = GL1_HW_SCH_INVALID_METRICS_INDEX;
  }

  mdsp_set_sync_rx_search_width(GL1_HW_DEFAULT_SEARCH_WIDTH,gas_id);

#ifdef FEATURE_GSM_TDS
  if( gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode != RFCOM_TDSCDMA_MODE )
  {
#endif
    /* Init the asynx RX search width*/
    mdsp_set_async_rx_search_width( GL1_HW_CM_SCH_SEARCH_WIDTH_QS/4  ,gas_id);
#ifdef FEATURE_GSM_TDS
  }
  else
  {
    gl1_hw_x2g_init_info();
    mdsp_set_async_rx_search_width( GL1_HW_X2G_TDS_SCH_SEARCH_WIDTH_QS/4  ,gas_id);
  }
#endif
}

/*===========================================================================

FUNCTION gl1_hw_cm_get_trm_rf_device

DESCRIPTION
  Returns CM mode RF device ID
===========================================================================*/

rfm_device_enum_type gl1_hw_cm_get_trm_rf_device( gas_id_t gas_id )
{
   return (gl1_hw_x2g_data[gas_id].gl1_hw_cm_trm_rf_device);
}


/*===========================================================================

FUNCTION gl1_hw_get_trm_rf_id

DESCRIPTION
  Returns the RF resource ID for WCDMA Neighbour

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static rfm_device_enum_type gl1_hw_get_gsm_rf_id_from_trm( gas_id_t gas_id )
{
  /*this is a union*/
  trm_get_info_data     grm_get_device_input;
  trm_get_info_data    *grm_get_device_input_p;
  rfm_device_enum_type  device;

  grm_get_device_input_p = &grm_get_device_input;

  grm_get_device_input_p->band_mapping_info.resource      = TRM_RX_BEST_POSSIBLE;
  grm_get_device_input_p->band_mapping_info.num_bands     = 1;
  grm_get_device_input_p->band_mapping_info.band[0]       = SYS_BAND_GSM_EGSM_900;

  device =
    grm_get_device_mapping( gl1_map_gas_id_to_client_id(gas_id),
                            grm_get_device_input_p, gas_id );

  if (device == RFM_INVALID_DEVICE)
  {
    device = RFM_DEVICE_0;

    MSG_GERAN_ERROR_1_G ( GL1_DRDS_DBG_HDR"new trm api reports invalid device for client_id:%d",
                          grm_get_device_input_p->client_info.requested_client_id );
  }

   return (device);
}
/*===========================================================================

FUNCTION gl1_hw_cm_get_rf_irat_timing_info

DESCRIPTION
  Enable and initializes RF hardware.
  Note:  Very similar to gl1_hw_rf_init().

===========================================================================*/
void gl1_hw_cm_get_rf_irat_timing_info (gl1_hw_cm_rat_t source_rat, gas_id_t gas_id)
{
  rfm_meas_irat_info_type             rf_meas_irat_info;
  rfm_meas_result_type                rf_meas;

  rf_meas_irat_info.header.source_tech  = source_rat;
  rf_meas_irat_info.header.num_source_item = 1;
  rf_meas_irat_info.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device = gl1_hw_x2g_data[gas_id].gl1_hw_cm_trm_rf_device;
  rf_meas_irat_info.header.source_param_v2[RFM_MEAS_PCELL].band = 0;

  rf_meas_irat_info.header.target_tech  = RFM_EGSM_MODE;
  rf_meas_irat_info.header.num_target_item = 1;
  rf_meas_irat_info.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device = gl1_hw_x2g_data[gas_id].gl1_hw_cm_trm_rf_device;;
  rf_meas_irat_info.header.target_param_v2[RFM_MEAS_PCELL].band = RFCOM_BAND_GSM900;

  rf_meas_irat_info.irat_info_param = &rf_meas_x2g_irat_info_param[gas_id];
  rf_meas_irat_info.tech_specific_info_param = (void*)mdsp_get_gfw_init_event_buffer(gas_id);
  rf_meas_irat_info.sub_id = gas_id;

  rf_meas = rfm_meas_common_get_irat_info_param( &rf_meas_irat_info, NULL );
  if(rf_meas != RFM_MEAS_COMMON_SUCCESS)
  {
    MSG_GERAN_ERROR_0_G("rfm_meas_common_get_irat_info_param reported failure");
  }

   /* Store the pretune times, set by get_irat_info (in blocking call so RF will have completed by here  */
   gl1_hw_x2g_data[gas_id].gl1_hw_cm_nbr_pretune_offset_time = rf_meas_irat_info.nbr_pretune_offset_time;

   /* Update gl1_rf_gsm_timing_info for QTA/IRAT */
   gl1_hw_set_rf_gsm_rx_time((int32)rf_meas_x2g_irat_info_param[gas_id].power_meas_setup_duration,
                             (int32)rf_meas_x2g_irat_info_param[gas_id].power_meas_alpha_rffe,
                             gas_id);

}


/*===========================================================================

FUNCTION gl1_hw_cm_rf_init

DESCRIPTION
  Enable and initializes RF hardware.
  Note:  Very similar to gl1_hw_rf_init().

===========================================================================*/
static void gl1_hw_cm_rf_init (gas_id_t gas_id)
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   uint32                    use_idle_rx_alpha = (uint32)FALSE;
#endif



  gl1_hw_x2g_data[gas_id].gl1_hw_cm_trm_rf_device = gl1_hw_get_gsm_rf_id_from_trm(gas_id);


  MSG_GERAN_HIGH_1_G("X2G: gl1_hw_cm_rf_init rfm device=%d", gl1_hw_x2g_data[gas_id].gl1_hw_cm_trm_rf_device);

  gl1_hw_rf_cm_enter(gas_id);

 /*In DR mode we vary the rxalpha for RFE bus contention in idle mode,
  in X2G and all other modes default RXalpha must be used
  */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  mdsp_update_shared_variable(GFW_RX_ALPHA,&use_idle_rx_alpha,gas_id);
#endif

#ifdef FEATURE_GSM_TDS
  /*Need to know calling function is TD or not*/
  if ( gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode == RFCOM_TDSCDMA_MODE )
  {
    mdsp_intf_shared_var_write(GFWSHAREDVARIABLES_RX_X2GSTARTALPHA,(uint16)GL1_HW_GFW_X2G_START_ALPHA_T2G_QS ,gas_id);
    mdsp_intf_shared_var_write(GFWSHAREDVARIABLES_RX_X2GRXFECONFIGALPHA,(uint16)GL1_HW_GFW_CM_T2G_RXFE_CONFIG_ALPHA ,gas_id);
  }
  else
  {
    /* For other X2G RATs ensure default value restored.*/
    mdsp_intf_shared_var_write(GFWSHAREDVARIABLES_RX_X2GSTARTALPHA,(uint16)GL1_HW_GFW_X2G_START_ALPHA_DEFAULT_QS ,gas_id);
    mdsp_intf_shared_var_write(GFWSHAREDVARIABLES_RX_X2GRXFECONFIGALPHA,(uint16)GL1_HW_GFW_CM_X2G_RXFE_CONFIG_ALPHA ,gas_id);
  }
  MSG_GERAN_HIGH_2_G("X2G:Update x2g_Start_alpha %d, x2gRxfeConfigAlpha:%d",
                      *(uint16*)mdsp_intf_shared_var_read(GFWSHAREDVARIABLES_RX_X2GSTARTALPHA ,gas_id),
                      *(uint16*)mdsp_intf_shared_var_read(GFWSHAREDVARIABLES_RX_X2GRXFECONFIGALPHA ,gas_id));
  mdsp_updated_shared_variable(GFW_RX_SHARED_VARIABLE,gas_id);
#endif

  /* Set source tech to serving_rat_info.mode, set in cm init. */
   gl1_hw_cm_get_rf_irat_timing_info (gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode, gas_id);

   gl1_hw_async_intf_enter_mode_cmd(GFW_APP_MODE_MEASUREMENT,
                    gl1_hw_get_rxlm_buff_index(gas_id),
#ifdef FEATURE_GSM_RX_TX_SPLIT
                                    gl1_hw_get_txlm_buff_index(gas_id),
                                    GFW_ENTER_MODE_PRX_BIT,
#else
                  GFW_ENTER_MODE_PRX,
#endif
                  gl1_hw_x2g_data[gas_id].gl1_hw_cm_trm_rf_device,
                  geran_map_gas_id_to_nas_id(gas_id) );

} /* end of gl1_hw_cm_rf_init */

/*===========================================================================

FUNCTION gl1_hw_cm_terminate

DESCRIPTION
  Cleanup the gl1_hw_cm layer interface.

DEPENDENCIES
  gl1_hw_cm_gsm_init() must be called first.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_hw_cm_terminate ( boolean shutdown_fw, gas_id_t gas_id )
{
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id( gas_id );

  MSG_GERAN_HIGH_3_G("Terminating compressed mode %d shutdown_fw %d ts=%d", gl1_hw_cm_status[gas_id].initialised, shutdown_fw, gl1_hw_cm_status[gas_id].TimestampOn);

  if (gl1_hw_cm_status[gas_id].initialised == FALSE)
  {
     MSG_GERAN_ERROR_0_G("CM Terminate request, but CM not initialised");
     return;
  }

  gl1_hw_cm_status[gas_id].ActionValue = Terminating;
  gl1_task_mutex_lock(gas_id);

/* Cancel any pending cleanup done processing */
  GDRV_ISR_SAVE_LOCK(gas_id);
  gl1_hw_cm_cleanup_done_cancel(gas_id);

#ifdef FEATURE_GSM_TDS
  /*Need to know calling function is TD or not*/
  if( gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode == RFM_TDSCDMA_MODE )
  {
  /* For the scenario where T2G APIs have been called and then
     * CM terminate is called
     */

    gl1_hw_x2g_empty_cmd_queue( );
    gstmr_deregister_handler_geran( gl1_hw_x2g_ftick_hdlr,as_id );
    if (gstmr_is_handler_registered_geran(gl1_hw_x2g_pwr_meas_ft_handler,as_id))
    {
       gstmr_deregister_handler_geran( gl1_hw_x2g_pwr_meas_ft_handler,as_id );
    }
    if (gstmr_is_handler_registered_geran(gl1_hw_x2g_acq_ft_handler,as_id))
    {
       gstmr_deregister_handler_geran( gl1_hw_x2g_acq_ft_handler,as_id );
    }
    if (gstmr_is_handler_registered_geran(gl1_hw_x2g_sch_ft_handler,as_id))
    {
       gstmr_deregister_handler_geran( gl1_hw_x2g_sch_ft_handler,as_id );
    }
    gl1_hw_x2g_init_info();
    gl1_hw_init_t2g_handler_sm();
  }
#endif

  /* Shutdown the mdsp */
  mdsp_cm_inact(shutdown_fw, gas_id);

  GDRV_ISR_SAVE_UNLOCK(gas_id);

  gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded = FALSE;

  /* RF CM exit */
  gl1_hw_rf_cm_exit(gas_id);

#ifdef GERAN_L1_HLLL_LNASTATE
  /*Reset sawless support flags*/
  gl1_hw_reset_sawless_support_flag(gas_id);
#endif

  gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode = RFCOM_PARKED_MODE;

  mdsp_set_ftsm_geran(FRAME_TICK_SETUP_MARGIN_QS, gas_id);

  gl1_hw_log_cell_cmds_to_diag (cmd_cm_gsm_terminate, TRUE,gas_id);

  /* Check for possible time overruns before deregistering VSTMR */
  vstmr_snapshot_before_vstmr_deregister (gas_id);
  /* Deregister VSTMR */
  vstmr_geran_vstmr_deregister (gas_id);

  gl1_hw_cm_status[gas_id].initialised = FALSE;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* advise the DRX module that Compressed Mode is ending */
  gl1_drx_cm_shutdown(gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  gl1_task_mutex_unlock(gas_id);

  gl1_hw_cm_status[gas_id].ActionValue = NoneAI;

  gl1_hw_cm_status[gas_id].TimestampOff = timetick_get_safe();

#ifdef FEATURE_GSM_COEX
   /* The X2G GAS is registered with the arbitrator for DSDA and DR-DSDS only */
   gl1_hw_arbitration[gas_id].deregistration(TRUE, gas_id);
#endif


}

/*===========================================================================

FUNCTION gl1_hw_cm_gsm_terminate

DESCRIPTION
  Cleanup the gl1_hw_cm layer interface.
DEPENDENCIES
  gl1_hw_cm_gsm_init() must be called first.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#ifdef FEATURE_DUAL_SIM
extern void gl1_hw_cm_gsm_terminate( sys_modem_as_id_e_type      as_id )
#else
extern void gl1_hw_cm_gsm_terminate( void )
#endif /*FEATURE_DUAL_SIM*/
{
#ifdef FEATURE_DUAL_SIM
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#else
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
  sys_modem_as_id_e_type      as_id = SYS_MODEM_AS_ID_NONE;
#endif

  MSG_GERAN_HIGH_2_G("Terminating compressed mode for as_id = %d gas_id = %d",
                     as_id , gas_id );

  gl1_hw_cm_terminate(TRUE, gas_id );
}

/*===========================================================================

FUNCTION gl1_hw_complete_cm_terminate

DESCRIPTION
  Cleanup the gl1_hw_cm layer interface.

DEPENDENCIES
  gl1_hw_cm_gsm_init() must be called first.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_complete_cm_terminate( gas_id_t gas_id )
{
  boolean awake;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id( gas_id );

  if ( !mdsp_awake(gas_id) )
  {
    awake = mdsp_wakeup_geran( TRUE, as_id );
    MSG_GERAN_HIGH_1_G("GFW Asleep - wakeup %d with CM flag TRUE",awake);
  }

  gl1_hw_cm_terminate(FALSE, gas_id );

  /* ensure GFW is awake */
  awake = mdsp_wakeup_geran( FALSE, as_id );
  MSG_GERAN_HIGH_1_G("GFW wakeup %d",awake);

}

/*===========================================================================

FUNCTION gl1_hw_cm_is_gsm_fw_awake

DESCRIPTION
  Whether GFW is asleep.

===========================================================================*/
boolean gl1_hw_cm_is_gsm_fw_awake (
#ifdef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type      as_id
#else
   void
#endif /*FEATURE_DUAL_SIM*/
   )
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/
    return(mdsp_awake(geran_map_nas_id_to_gas_id(as_id)));
}

/*===========================================================================

FUNCTION gl1_is_gfw_error_detected

DESCRIPTION
  Whether GFW has detected an error that requires CM panic reset.

===========================================================================*/
#ifdef FEATURE_DUAL_SIM
boolean gl1_is_gfw_error_detected( sys_modem_as_id_e_type      as_id )
#else
boolean gl1_is_gfw_error_detected( void )
#endif /*FEATURE_DUAL_SIM*/
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/
  return gl1_is_gfw_error_detected_geran(as_id);
}

/*===========================================================================

FUNCTION gl1_is_gfw_error_detected_geran

DESCRIPTION
  Whether GFW has detected an error that requires CM panic reset.

===========================================================================*/
boolean gl1_is_gfw_error_detected_geran (sys_modem_as_id_e_type      as_id)
{
  return mdsp_cm_check_for_error_recovery(geran_map_nas_id_to_gas_id(as_id));
}

/*===========================================================================

FUNCTION gl1_hw_cm_is_gap_complete

DESCRIPTION
  Whether GL1 gap os complete, cleanup processing is done.

===========================================================================*/
boolean gl1_hw_cm_is_gap_complete (sys_modem_as_id_e_type      as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  return gl1_hw_x2g_data[gas_id].gl1_hw_cm_gap_complete;
}


/*===========================================================================
FUNCTION  gl1_hw_get_gsm_timing_info
DESCRIPTION
  Returns GSM timing information (not RF timing).

INPUT PARAMETERS
  Pointer to structure for results
===========================================================================*/
void gl1_hw_get_gsm_timing_info( gl1_x2g_timing_info_t  *gl1_x2g_info )
{
/* Inform home RAT of GSM timing */
   gl1_x2g_info->time_meas = GL1_HW_CM_PWR_MEAS_CMD_DURATION_QS;
   gl1_x2g_info->time_bsic = GL1_HW_CM_ASYNC_RX_CMD_DURATION_QS;
}

#if defined ( FEATURE_QTA ) && defined ( FEATURE_CXM_QTA )
/*===========================================================================

FUNCTION gl1_hw_set_rx_cmd_issued_in_qta

DESCRIPTION
   records the fact that a RX cmd has been issued

===========================================================================*/
void gl1_hw_set_rx_cmd_issued_in_qta(boolean flag, gas_id_t gas_id)
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   if (l1_tskisr_blk->qta_in_progress)
   {
     gl1_hw_x2g_data[gas_id].gl1_hw_rx_flag_in_qta = flag;
     if (gl1_hw_x2g_data[gas_id].gl1_hw_rx_flag_in_qta)
     {
        if (gl1_hw_x2g_data[gas_id].gl1_qta_fw_rf_initialised)
        {
          gl1_hw_x2g_data[gas_id].gl1_hw_rx_count_in_qta++;
        }
        else
        {
           /* for case  such as extended paging where TRM not released,
            * but we need to generate cleanup scripts for 2nd QTA
            */
          gl1_hw_x2g_data[gas_id].gl1_hw_rx_count_in_qta=0;
          gl1_hw_x2g_data[gas_id].gl1_hw_rx_flag_in_qta = FALSE;
        }
     }
     else
     {
        gl1_hw_x2g_data[gas_id].gl1_hw_rx_count_in_qta=0;
        gl1_hw_x2g_data[gas_id].gl1_hw_rx_flag_in_qta = FALSE;
     }

     MSG_GERAN_LOW_3_G("QTA: CMD Issued %d No. of RX CMDS issued %d qta_fw_rf %d",
               gl1_hw_x2g_data[gas_id].gl1_hw_rx_flag_in_qta,
               gl1_hw_x2g_data[gas_id].gl1_hw_rx_count_in_qta,
               gl1_hw_x2g_data[gas_id].gl1_qta_fw_rf_initialised);
   }
}

/*===========================================================================

FUNCTION gl1_hw_get_rx_cmd_in_qta_issued_flag

DESCRIPTION
   records the fact that a RX cmd has been issued

===========================================================================*/
boolean gl1_hw_get_rx_cmd_in_qta_issued_flag (gas_id_t gas_id)
{
  return (gl1_hw_x2g_data[gas_id].gl1_hw_rx_flag_in_qta);
}

/*===========================================================================

FUNCTION gl1_hw_get_rx_cmd_in_qta_issued_count

DESCRIPTION
   records the fact that a RX cmd has been issued

===========================================================================*/
int8 gl1_hw_get_rx_cmd_in_qta_issued_count (gas_id_t gas_id)
{
  return (gl1_hw_x2g_data[gas_id].gl1_hw_rx_count_in_qta);
}

#endif


#ifdef FEATURE_GSM_TDS
/*===========================================================================

FUNCTION gl1_hw_x2g_offset_adjust

DESCRIPTION


===========================================================================*/

int32 gl1_hw_x2g_offset_adjust( uint32               g_frame_number,
                                uint16               offset,
                                gl1_hw_x2g_api_type  x2g_api_type,
                                uint8                num,
                                gas_id_t             gas_id
                              )
{

  /* read current FN */
  gsmtr_fn_qs_type current_fn_qs;

  int32 offset_adjust=0;
  static uint8 local_num=0; /* static_dual_spaced_ignore */
#ifdef FEATURE_GSM_TDS_DEBUG
  static uint16 stored_current_qs=0; /* static_dual_spaced_ignore */

  MSG_GERAN_HIGH_1_G("X2G offset adjust: last_state %d",gl1_t2g_last_state);
#endif
  current_fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);

  switch (x2g_api_type)
  {
  case STARTUP:

    /* we have no idea which X2G activity this startup is for*/
    gl1_hw_x2g_startup_info.g_fn = g_frame_number;

    /* need to know offset this occured at - startup could occur just before a frame tick */
    gl1_hw_x2g_startup_info.offset_at_api_call = current_fn_qs.qs;
    gl1_hw_x2g_startup_info.offset             = offset;

    gl1_t2g_last_state = STARTUP;

#ifdef FEATURE_GSM_TDS_DEBUG
    MSG_3(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
          "X2G offset adjust: Startup - current_fn %d, startup_api_call_qs %d, g_frame_number %d",
          current_fn_qs.fn,gl1_hw_x2g_startup_info.offset_at_api_call,gl1_hw_x2g_startup_info.g_fn );
#endif
    local_num =0;

  return current_fn_qs.fn;

  break;

  case PWR_MEAS:
    if (gl1_t2g_last_state == STARTUP)
    {
      static uint8 index =0;

      gl1_hw_x2g_pwr_meas_info.startup_g_fn = gl1_hw_x2g_startup_info.g_fn;
      gl1_hw_x2g_pwr_meas_info.offset_at_api_call = gl1_hw_x2g_startup_info.offset_at_api_call;
      gl1_hw_x2g_pwr_meas_info.startup_offset = gl1_hw_x2g_startup_info.offset;
      gl1_hw_x2g_pwr_meas_info.g_fn = g_frame_number;

      if (local_num == 0)
      {
#ifdef FEATURE_GSM_TDS_DEBUG
        stored_current_qs=current_fn_qs.qs;
#endif
        index =0;
      }

      gl1_hw_x2g_pwr_meas_info.offset[index] = offset;
      index++;

      /*check GFNs - asumption is that Startup & PWR MEAS APIs called in same frame. Need to check for Hyperframe wrap*/
      offset_adjust = SUB_FN(g_frame_number,gl1_hw_x2g_pwr_meas_info.startup_g_fn);


#ifdef FEATURE_GSM_TDS_DEBUG
      MSG_3(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
            "X2G offset adjust: PWR Meas - stored current_qs %d, offset adjust %d, local_num %d",
            stored_current_qs,offset_adjust, local_num);
#endif
      if (local_num == num-1)
      {
        gl1_t2g_last_state = PWR_MEAS;
#ifdef FEATURE_GSM_TDS_DEBUG
        stored_current_qs = 0;
#endif
      }
     
      local_num++;
    }
    else
    {
      MSG_GERAN_ERROR_1_G("X2G offset adjust - Pwr Meas: wrong state %d returning offset_adjust=0", gl1_t2g_last_state);
      offset_adjust =0;
    }
    break;

  case TONE_DETECT:
    if (gl1_t2g_last_state == STARTUP)
    {
      cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.startup_g_fn       = gl1_hw_x2g_startup_info.g_fn;
      cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.offset_at_api_call = gl1_hw_x2g_startup_info.offset_at_api_call;
      cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.startup_offset     = gl1_hw_x2g_startup_info.offset;
      cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.offset             = offset;
      cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.g_fn               = g_frame_number;

      /*check GFNs - asumption is that Startup & PWR MEAS APIs called in same frame. Need to check for Hyperframe wrap*/
      offset_adjust = SUB_FN(g_frame_number,cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.startup_g_fn);

      gl1_t2g_last_state = TONE_DETECT;

#ifdef FEATURE_GSM_TDS_DEBUG
      MSG_3(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
            "X2G offset adjust: Tone Detect - current_qs %d, startup_api_call_qs %d, offset adjust %d ",
            current_fn_qs.qs,
            cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.offset_at_api_call,
            offset_adjust);
#endif

    }
    else
    {
      MSG_GERAN_ERROR_1_G("X2G offset adjust - Tone Detect: wrong state %d returning 0", gl1_t2g_last_state);
      offset_adjust =0;
    }

    break;

  case STOP_TONE_DETECT:
          /*check GFNs - asumption is that Startup & PWR MEAS APIs called in same frame. Need to check for Hyperframe wrap*/
      offset_adjust = SUB_FN(g_frame_number,cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.startup_g_fn);

      cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.stop_acq_g_fn = SUB_FN(g_frame_number,1) ;
      cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.stop_acq_offset = offset;
      cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.seperate_cleanup_async_required = FALSE;

      /* Detect the condition that the start_up is in current frame N and issued using Asyn but the cleanup is N+2 frame with offset 0 - 625 CR365989*/
      if( cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.startup_g_fn == GSTMR_GET_FN_GERAN(gas_id) )
      {
        uint32  startup_gfn_plus_one = ADD_FN( cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.startup_g_fn, 1 );

        if( g_frame_number == startup_gfn_plus_one )
  {
          if( ( offset >= QS_PER_FRAME ) &&( offset <= (QS_PER_FRAME + FRAME_TICK_SETUP_MARGIN_QS) ) )
    {
      /*force the stop_acq to be constructed in the next frame and async command */
            cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.stop_acq_g_fn = g_frame_number;
            cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.seperate_cleanup_async_required = TRUE;
    }
  }
      }

#ifdef FEATURE_GSM_TDS_DEBUG
      MSG_3(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
            "X2G offset adjust: Stop Tone Detect - stop_acq_g_fn %d, stop_acq_offset %d, g_frame_num %d",
            cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.stop_acq_g_fn,
      cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.stop_acq_offset,
      g_frame_number);
#endif

      gl1_t2g_last_state = TONE_DETECT;

#ifdef FEATURE_GSM_TDS_DEBUG
      MSG_3(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
            "X2G offset adjust: Stop Tone Detect - current_qs %d, startup_api_call_qs %d, offset adjust %d",
            current_fn_qs.qs,
      cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.offset_at_api_call,
      offset_adjust);
#endif
    break;

  case SCH_DETECT:
    if (gl1_t2g_last_state == STARTUP)
    {
      sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.startup_g_fn       = gl1_hw_x2g_startup_info.g_fn;
      sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.offset_at_api_call = gl1_hw_x2g_startup_info.offset_at_api_call;
      sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.startup_offset     = gl1_hw_x2g_startup_info.offset;
      sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.offset             = offset;
      sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.g_fn               = g_frame_number;


      /*check GFNs - asumption is that Startup & PWR MEAS APIs called in same frame. Need to check for Hyperframe wrap*/
      offset_adjust = SUB_FN(g_frame_number,sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.startup_g_fn);


      gl1_t2g_last_state = SCH_DETECT;

 #ifdef FEATURE_GSM_TDS_DEBUG
      MSG_3(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
            "X2G offset adjust: SCH - current_qs %d, startup_api_call_qs %d, offset adjust %d",
            current_fn_qs.qs,sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.offset_at_api_call,offset_adjust );
#endif
    }
    else
    {
      MSG_GERAN_ERROR_1_G("X2G offset adjust - SCH: wrong state %d returning 0", gl1_t2g_last_state);
      offset_adjust =0;
    }


    break;



  case CLEANUP:
    local_num=0;
    if (gl1_t2g_last_state == PWR_MEAS)
    {
      /*check GFNs - asumption is that Startup & PWR MEAS APIs called in same frame. Need to check for Hyperframe wrap*/
      offset_adjust = SUB_FN(g_frame_number,gl1_hw_x2g_pwr_meas_info.startup_g_fn);
      gl1_hw_x2g_pwr_meas_info.cleanup_g_fn = SUB_FN(g_frame_number,1);
      gl1_hw_x2g_pwr_meas_info.cleanup_offset = offset;


      MSG_4(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
            "X2G offset adjust: Clean - PWR Meas: current_qs %d, cleanup_g_fn %d, cleanup_offset %d, offset adjust %d",
            current_fn_qs.qs, 
            gl1_hw_x2g_pwr_meas_info.cleanup_g_fn, 
            gl1_hw_x2g_pwr_meas_info.cleanup_offset,
            offset_adjust);

    }
    else if (gl1_t2g_last_state == TONE_DETECT)
    {
      /*check GFNs - asumption is that Startup & PWR MEAS APIs called in same frame. Need to check for Hyperframe wrap*/
      offset_adjust = SUB_FN(g_frame_number, cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.startup_g_fn);

      /* If there is to be a forced async command, ensure offset is in next frame */
      if ((cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.seperate_cleanup_async_required == TRUE) && ( offset < QS_PER_FRAME ))
      {
         offset += QS_PER_FRAME;
      }

      /* Adjust stored gfn so that, in Idle case, handler issues command in N-1*/
      cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.cleanup_g_fn   = SUB_FN(g_frame_number,1);
      cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.cleanup_offset = offset;

      if( cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.startup_g_fn == GSTMR_GET_FN_GERAN(gas_id) )
      {
        uint32  startup_gfn_plus_one = ADD_FN( cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.startup_g_fn, 1 );

        if( g_frame_number == startup_gfn_plus_one )
  {
          if( ( offset >= QS_PER_FRAME )&&( offset <= (QS_PER_FRAME + FRAME_TICK_SETUP_MARGIN_QS) ) )
    {
      /*force the command to be constructed in the next frame*/
            cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.cleanup_g_fn                    = g_frame_number;
            cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.seperate_cleanup_async_required = TRUE;

            MSG_GERAN_HIGH_3_G("X2G N+2 Cleanup Correction Current_FN =%d, issued at %d, to process at %d",
                               GSTMR_GET_FN_GERAN(gas_id),
                               cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.cleanup_g_fn,
                               g_frame_number
                              );
    }
  }
      }

#ifdef FEATURE_GSM_TDS_DEBUG
      MSG_3(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
            "X2G offset adjust: Clean - Tone Detect: current_qs %d, startup_offset %d, offset adjust %d",
            current_fn_qs.qs,
      cm_acq_state_machine[cm_acq_sm].gl1_hw_x2g_acq_info.startup_offset,
      offset_adjust);
#endif
    }
    else if (gl1_t2g_last_state == SCH_DETECT)
    {
      /*check GFNs - asumption is that Startup & PWR MEAS APIs called in same frame. Need to check for Hyperframe wrap*/
      offset_adjust = SUB_FN(g_frame_number,sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.startup_g_fn);
      /* Adjust stored gfn so that, in Idle case, handler issues command in N-1*/
      sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.cleanup_g_fn = SUB_FN(g_frame_number,1);
      sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.cleanup_offset = offset;

#ifdef FEATURE_GSM_TDS_DEBUG
      MSG_3(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
            "X2G offset adjust: Clean - SCH Meas: current_qs %d, startup_offset %d, offset adjust %d",
            current_fn_qs.qs,sch_state_machine[cm_sch_sm].gl1_hw_x2g_sch_info.startup_offset,offset_adjust);
#endif
    }
    else
    {
      MSG_GERAN_ERROR_2_G("X2G offset adjust - Cleanup: wrong State %d, setting offset_adjust %d to 0",gl1_t2g_last_state,offset_adjust);
      offset_adjust =0;
    }
    //last_state = NONE_GL1; - need to do this in X2G cleanup

    break;

  default:
    MSG_GERAN_ERROR_1_G("X2G offset adjust: called from unknown API %d setting offset_adjsut to 0",x2g_api_type);
    offset_adjust =0;
    break;

  }

#ifdef FEATURE_GSM_TDS_DEBUG
  MSG_GERAN_HIGH_1_G("X2G offset adjust: returning offset_adjust = %d",offset_adjust);
#endif
  return offset_adjust;


}

/* read and return flag to indicate whether X2G APi call is from TDSCDMA*/
boolean gl1_hw_cm_read_t2g_active(gas_id_t gas_id)
{
  return (gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode == RFCOM_TDSCDMA_MODE);
}



/*re-initialise all handler memory strcutures*/
void gl1_hw_init_t2g_handler_sm (void)
{
  uint8 i;
  for(i=0;i<POWER_MEAS_NUM_SM;i++)
  {
  memset( &cm_pwr_meas_state_machine[i],0,sizeof(cm_pwr_meas_state_machine_type));
  }
  for(i=0;i<ACQ_MEAS_NUM_SM;i++)
  {
  memset( &cm_acq_state_machine[i],0,sizeof(cm_acq_state_machine_type) );
  }
  for(i=0;i<SCH_MEAS_NUM_SM;i++)
  {
  memset( &sch_state_machine[i],0,sizeof(sch_state_machine_type));
  }
#ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
  cm_acq_state_machine_num_active = 0;
#endif
}
#endif

boolean gl1_hw_cm_read_w2g_active(gas_id_t gas_id)
{
  return (gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode == RFCOM_WCDMA_MODE);
}
#ifdef FEATURE_QTA
boolean gl1_hw_cm_read_l2g_active(gas_id_t gas_id)
{
  return (gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode == RFCOM_LTE_MODE);
}

#endif

/*===========================================================================

FUNCTION  gl1_hw_x2g_wait_rf_buffer_completion

DESCRIPTION
  Called to ensure that all pending GSM RF CCS buffers have been populated in RF Task context
  before calling RF inline to populate G2W startup CCS buffer

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void gl1_hw_x2g_wait_rf_buffer_completion(gas_id_t gas_id)
{
   int16 total_delay = 0;

   /* wait until all outstanding G RF buffers have been populated before allowing X2G meas exit to be populated */
   while((gl1_hw_rf_check_active_buffers(gas_id) == TRUE)
         && (total_delay < GL1_HW_X2G_WAIT_RF_MAX_DELAY))
   {
     gl1_hw_delay(GL1_HW_X2G_WAIT_RF_DELAY_STEP,gas_id);
     total_delay += GL1_HW_X2G_WAIT_RF_DELAY_STEP;

   }

   if(total_delay >= GL1_HW_X2G_WAIT_RF_MAX_DELAY)
   {
     MSG_GERAN_ERROR_1_G("X2G RF buffer wait too long: total_delay=%d us",total_delay);
   }
   else
   {
     MSG_GERAN_HIGH_1_G("X2G RF buffer wait: total_delay=%d us",total_delay);
   }
}

/*===========================================================================

FUNCTION  gl1_hw_cm_set_sch_metrics_tags

DESCRIPTION
  In W2G/L2G SCH receive set the index and seq number for retrieivng the result metrics

===========================================================================*/
void gl1_hw_cm_set_sch_metrics_tags(gas_id_t gas_id, ARFCN_T arfcn, uint8 mdsp_index, uint16 mdsp_seq_num )
{
   static uint8 gl1_hw_cm_sch_tags_index[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };

   gl1_hw_cm_sch_tags_index[gas_id] += 1;
   if (gl1_hw_cm_sch_tags_index[gas_id] >= Gl1_HW_CM_NUM_SCH_METRICS)
   {
      gl1_hw_cm_sch_tags_index[gas_id] = 0;
   }

   gl1_hw_cm_sch_tags[gas_id][gl1_hw_cm_sch_tags_index[gas_id] ].arfcn = arfcn;
   gl1_hw_cm_sch_tags[gas_id][gl1_hw_cm_sch_tags_index[gas_id] ].seq_num = mdsp_seq_num;
   gl1_hw_cm_sch_tags[gas_id][gl1_hw_cm_sch_tags_index[gas_id] ].metrics_index = mdsp_index;
}


#ifdef FEATURE_QTA
/*===========================================================================

FUNCTION gl1_hw_cm_build_startup_apc

DESCRIPTION
  Pre-build startup scripts in DPC task

===========================================================================*/
static void gl1_hw_cm_build_startup_apc( gl1_hw_cm_build_startup_param_type *gl1_hw_cm_dpc_ptr)
{
     gl1_hw_cm_build_startup_if_not_prebuilt (gl1_hw_cm_dpc_ptr->cm_rf_info_ptr, gl1_hw_cm_dpc_ptr->gas_id);
}
/*===========================================================================

FUNCTION gl1_hw_cm_build_startup_dpc

DESCRIPTION
  Pre-build startup scripts in DPC task

===========================================================================*/
void gl1_hw_cm_build_startup_dpc(const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr, gas_id_t gas_id)
{
  gl1_hw_cm_build_startup_param_type *gl1_hw_cm_dpc_ptr = &gl1_hw_cm_build_startup_dpc_param;

  gl1_hw_cm_dpc_ptr->cm_rf_info_ptr = cm_rf_info_ptr;
  gl1_hw_cm_dpc_ptr->gas_id = gas_id;

  if (!rexl4_queue_dpc((rex_apc_func_type)gl1_hw_cm_build_startup_apc, (uint32)gl1_hw_cm_dpc_ptr))
  {
#ifdef FEATURE_QSH_MDUMP
     QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
     ERR_GERAN_FATAL_0("Failed to queue HW CM build script DPC");
  }
}

/*===========================================================================

FUNCTION set_qta_cleanup_pending

DESCRIPTION
  This is used to manage qta cleanup status

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void set_qta_cleanup_pending(boolean status, gas_id_t gas_id)
{
  MSG_GERAN_MED_2_G("set_qta_cleanup_pending old value =%d,new value:%d",gl1_hw_x2g_data[gas_id].qta_cleanup_pending,status);
  gl1_hw_x2g_data[gas_id].qta_cleanup_pending = status;
}

/*===========================================================================

FUNCTION get_qta_cleanup_pending

DESCRIPTION
  This is used to query qta cleanup status

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean get_qta_cleanup_pending(gas_id_t gas_id)
{
  return gl1_hw_x2g_data[gas_id].qta_cleanup_pending;
}

/*===========================================================================

FUNCTION qta_empty_gap_cleanup

DESCRIPTION
  This is used to inform T or W that QTA gap was empty,

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_qta_empty_gap(gas_id_t gas_id)
{
  boolean empty_qta_gap = gl1_hw_x2g_data[gas_id].gl1_empty_qta_gap;
  gl1_hw_x2g_data[gas_id].gl1_empty_qta_gap = FALSE;
  return (get_qta_cleanup_pending(gas_id) || (empty_qta_gap));
}

#endif

/*===========================================================================

FUNCTION  geran_l1_x2g_irat_enter

DESCRIPTION :X RAT Call this function when X2G enter need to sent

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None


===========================================================================*/
void geran_l1_x2g_irat_enter ( const gl1_hw_cm_rf_info_t* irat_meas_rf_params_ptr,
                               sys_modem_as_id_e_type as_id )
{
  rfm_meas_enter_param_type gl1_meas_enter_param ;
  rfm_meas_result_type rfm_result;
  rfcom_gsm_band_type gsm_band;
  gas_id_t gas_id = geran_map_nas_id_to_gas_id (as_id);

  if (NULL==irat_meas_rf_params_ptr )
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_0_G("X2G_SRCH: irat_meas_rf_params_ptr NULL");
  }

  gl1_hw_x2g_data[gas_id].gl1_hw_cm_cxm_source_rat = irat_meas_rf_params_ptr->common_rf_structure_ptr->header.source_tech;
  gl1_hw_x2g_data[gas_id].gl1_hw_cm_trm_rf_device = irat_meas_rf_params_ptr->target_rf_device;

  gsm_band = gl1_hw_arfcn_band_to_rfcom_gsm_band(irat_meas_rf_params_ptr->target_band);  /* Default is RFCOM_BAND_GSM900 */

  gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr = irat_meas_rf_params_ptr->common_rf_structure_ptr;

  /*filling target header */
  gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->header.target_tech = RFM_EGSM_MODE;

  gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device = irat_meas_rf_params_ptr->target_rf_device;
  gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = irat_meas_rf_params_ptr->rxlm_gsm_buf_idx;
  gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->header.target_param_v2[RFM_MEAS_PCELL].band =(uint32) gsm_band;

  gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->header.num_target_item = 1;

  /*filling gsm_params  */
  gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->target_param.gsm_params.rx_dev_handle[DEV_HANDLE_PRX].device = irat_meas_rf_params_ptr->target_rf_device;
  gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->target_param.gsm_params.rx_dev_handle[DEV_HANDLE_PRX].lm_handle = irat_meas_rf_params_ptr->rxlm_gsm_buf_idx;
  gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->target_param.gsm_params.band = gsm_band;
  gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr->target_param.gsm_params.sub_id = as_id;

  gl1_meas_enter_param.enter_params = gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr;
  rfm_result = rfm_meas_common_script_enter(&gl1_meas_enter_param, NULL );
  if (rfm_result == RFM_MEAS_COMMON_FAILURE)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_0_G("X2G_SRCH: RFM enter failure");
  }

}
/*===========================================================================

FUNCTION  geran_l1_x2g_irat_exit

DESCRIPTION : X RAT Call this function when X2G exit need to sent

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None


===========================================================================*/
void  geran_l1_x2g_irat_exit ( const gl1_hw_cm_rf_info_t* irat_meas_rf_params_ptr,
                               boolean abort_flag,
                               sys_modem_as_id_e_type as_id )
{
  rfm_meas_exit_param_type gl1_meas_exit_param;
  rfm_meas_result_type rfm_result;
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  if (NULL == irat_meas_rf_params_ptr )
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_0_G("X2G_SRCH: irat_meas_rf_params_ptr NULL");
  }

  gl1_hw_qta_mutex_lock( gas_id );
  /* For W2G cannot control clear preload flag as rf meas enter / exit interleaved*/
  if (gl1_hw_x2g_data[gas_id].gl1_hw_cm_serving_rat_mode != RFM_IMT_MODE)
  {
     /* Force abort for RF exit if pre-built scripts has not been used. */ 	 
     if (gl1_hw_x2g_data[gas_id].gl1_hw_first_gap_after_preload == TRUE)
     {
        gl1_hw_x2g_data[gas_id].gl1_hw_first_gap_after_preload = FALSE;
        abort_flag = TRUE;
        MSG_GERAN_HIGH_0_G("X2G: Force RF meas abort as unused scripts");
     }
     gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded = FALSE;
  }
  else if (abort_flag == TRUE)
  {
     gl1_hw_x2g_data[gas_id].gl1_hw_cm_startup_preloaded = FALSE;
  }
  gl1_hw_qta_mutex_unlock( gas_id );

  gl1_meas_exit_param.exit_params = irat_meas_rf_params_ptr->common_rf_structure_ptr;
  gl1_meas_exit_param.is_measurement_gap_aborted = abort_flag;

  MSG_GERAN_HIGH_1_G("x2G RF meas exit, abort=%d", abort_flag);

  rfm_result = rfm_meas_common_script_exit(&gl1_meas_exit_param, NULL );
  if (rfm_result == RFM_MEAS_COMMON_FAILURE)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_0_G("X2G_SRCH: RFM exit failure");
  }
}

#ifdef QTA_EVENT_DATA_LOGGING 
/*===========================================================================

FUNCTION  gl1_hw_cm_check_delay_after_qta_startup

DESCRIPTION : It checks the number of frames elapsed after QTA gap start & 
              calls for forced crash if Cleanup has not arrived for long.

===========================================================================*/
void gl1_hw_cm_check_delay_after_qta_startup(gas_id_t gas_id)
{
gl1_hw_qta_event_t qta_event = GL1_HW_QTA_RESET;
  uint32 qta_fn = 0;
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  /* For The Sub in MSim Idle,
     If the last stored event is QTA_STARTUP & 47 frames have elapsed after
     getting QTA_STARTUP, it means QTA_CLEANUP did not arrive in time, so
     force a crash here for debugging this issue.
     
     Only perform crash condition if not a rude-wakeup for QTA though.  In 
     rude wakeup cases the frame number is likely to get set to a earlier point
     at the wakeup time.  This can cause the subtraction in this check to go
     negative which is interpreted as a highly positive number */
  gl1_hw_qta_mutex_lock( gas_id );
  qta_event = gl1_hw_qta_event_data[gl1_hw_qta_event_data_index].qta_event;
  qta_fn = gl1_hw_qta_event_data[gl1_hw_qta_event_data_index].frame_num;
  gl1_hw_qta_mutex_unlock( gas_id );

  if(!gl1_hw_sleep_get_forcedwakeup(gas_id) &&
    (l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE) &&  qta_event == GL1_HW_QTA_STARTUP &&
     SUB_FN(GSTMR_GET_FN_GERAN(gas_id), qta_fn) >= L1_QTA_DURATION_GAP_MAX + 10)
    {
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_1_G("QTA_CLeanup did not arrive for more than %d frames", L1_QTA_DURATION_GAP_MAX+10);
    }
}
#endif /* QTA_EVENT_DATA_LOGGING */

