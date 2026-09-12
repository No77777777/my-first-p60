/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      G L 1  F R A M E   L A Y E R

GENERAL DESCRIPTION
  This module contains declarations to support Layer 1 sleep.  These
  functions are called by the DRX manager in (portable) Layer 1 and
  in turn call functions in the RF drivers, sleep controller driver
  and FEE driver.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2003-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_sleep_ctl.c#5 $
$DateTime: 2020/06/01 08:04:41 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
01/06/20   sal      CR2699233 Changed panic reset to error fatal at the time of error recovery attempt failed
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
15/05/19   sal      CR2251068 Reverse GL1_EFS_DEBUG_ENABLE_SLEEP_WITH_GCF behavior
14/03/18   km       CR2017741 RRBP_HI is yielded when collision with PCH collision
02/05/17   br       CR2040677 GL1 to enable rxd for them immediate CCCH block if quality goes less than 32767
06/02/18   rv       CR2069764: Excessive F3 logging reduction in GL1
30/08/16   tsk      CR1058305: Update to 0x5096 RxD log packet with additional details.
22/04/16   ksb      CR1005869 To mainlne FEATURE_GL1_SLEEP_DEBUG_STRUCT
15/12/2016  ksb       CR1041734 Error recovery handing at regular interval using NV control
15/12/17   ap       CR1103567 FR36133: Extended LTE Signaling (ELS) to manage tune-away gaps in DSDS 
03/01/16   mn       CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
15/05/17   akm      CR2047050 Avoid GL1 going to sleep during rude wakeup if RF wakeup CNF is not received
04/08/16   mn       CR1049816  Protecting Sleep warmup recovery under Lock during sleep wakeup commit.
12/07/16   tsk      CR987216 For SSIM Idle mode RxD should be gated by Idle mode RxD NV.
12/07/16   tsk      CR978200 Support Dynamic Rxd with SSIM and WLAN Antenna Sharing configuration.
13/05/16   km       CR1012528 DTF sleep scenario "Forced error Recovery"
09/05/16   sm       CR1002861 Added GL1 QSH handler : TRM Updation (Reserve/Cancel)   
07/03/15   mko      CR981074: Read the GCF NV first then try to register NPA callback based on GCF NV settings
08/12/15   dp       CR942828 check GFW awake status before trying to wakeup or sleep GFW
23/11/15   dp       CR929648 reduce power usage when waiting for TRM during find_bcch
21/11/15   ip/mko   CR938381 In DSDS mode, add larger threshold to error recovery, prevents trying to recover if less than half a frame to run
16/11/15   zf       CR939824 Clear entered_warmup_recovery flag after abort CCCH
22/10/15   zf       CR923858 Rework Sleep error recovery in QTA 
18/09/15   cws      CR907895 Change unnecessary F3 to LOW 
11/09/15   mal      CR905443 Increase GSM warmup time guard for page wakeup in non-LMM
31/08/15   mko      CR896740 sleep error recovery changes in QTA gap
26/08/15   mko      CR895256 Apply CR880038 changes only for multisim mode
03/09/15   kb       CR863908 Added new EFS file structure for sleep control
25/08/15   fan      CR893794 Page sharing not happening due to missing BLOCK_QUALITY indication
26/08/15   ws       CR896163 Fix compile errors for undefining FEATURE_DUAL_SIM
20/08/15   fan      CR893599 Remove flag indicating that other SUB is going active check during cancelation
15/08/15   zf       CR891237 Change QS to OLS calculation
15/08/15   df       CR882861 Improve commit threshold logic to use GRF estimate HW tran exec time
11/08/15   fan/akh  CR871795 FR19329: Avoid cancelled CCCH booking after rude wakeup
07/08/15   mko      CR880038: Add frame boundary/int pending checks after putting mdsp to sleep 
04/08/15   mko      CR878199: Setting QTA warmup time to 5msec
23/07/15   pg       CR877745 Reorder multisim FW RF init to reduce critical path through RF wakeup 
26/07/15   mko      CR878199 When sleep commit is successful and scheduled the VSTRM interrupt then reset in_warmup to FALSE 
30/07/15   fan/akh  CR880067 FR19329: Disable support for non-CCCH activity cancellation
29/07/15   fan/akh  CR873850 FR19329: Missed pch notification timestamp correction
24/07/15   df       CR868476 Adaptive timeline bounces in idle
23/07/15   fan/akh  CR865360 Use correct TRM reservation period when doing wakeup cancelation
23/07/15   akh      CR876576 Avoid TRM adjustment to include warmup when already included
22/07/15   akh      CR873836 Deadlock while deregistering vstmr handler from start_sleep
15/07/15   fan/akh  CR857332 FR19329: WMGR support for rescheduling non-PCH activities
08/07/15   akh      CR862342 Handle delay during start sleep and avoids incorrect FN on rude wakeup
08/07/15   df       CR815683 Avoid error recovery when G2W scripts are built during wakeup
06/07/15   sp       CR862778: Reduce F3 logging in Idle as its impacting QTA timeline
25/06/15   akh      CR852484 Re-enable FR19329 WMGR cancel on TA20
24/06/15   akh      CR852484 Disable FR19329 WMGR cancel on TA20
23/06/15   fan/akh  CR859092 Adapt WMGR Cancel handling to RACH Rescheduling in DSDA, DR-DSDS
23/06/15   kb       CR744398 Enhance Rude wakeup
23/06/15   akh      CR859232 FR19329: Deadlock due to GRM access from wkup mgr cb
19/06/15   fan/akh  CR851771 FR19329: Crash due to vstmr access during sleep
19/06/15   kb/ak/fa CR854009 FR19329: GSM making wrong TRM request
12/06/15   akh      CR849775 Wkup Mgr fixes - wakeup mgr running twice during GTS, debug code
04/06/15   fan/akh  CR848455 Fix for multiple handlings of same cancelation request
12/06/15   cja      CR850999 Correct qta cleanup for QTA gap for G2W IRAT
12/06/15   akh      CR852568 Debug code for wkup mgr cancellation
08/06/15   ip       CR847902 Miss first burst in QTA gap when warm up time is too short instead of missing PCH
29/05/15   fan      CR843017 Correct handling of pending_deferred_wmgr_event flag and gl1_hw_wkupmgr_wakeup_point_fn
22/05/15   mko      CR842062 Set send_unknown_block_ind to TRUE after CCCH is deferred to next DRX cycle
01/06/15   df       CR803641 Disable preemption when making commit to OLS decision
21/05/15   cja      CR839948 For QTA gap build preload scripts in QTA startup, send to GFW in cm startup
21/05/15   df       CR801738 Increase adaptive warmup step size during error recovery
20/05/15   fan      CR837443 - Remove CR788527 (temporary fix until WMGR cancel mainlined)
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
07/05/15   ip       CR833523 Clear Error Recovery flag when leaving Idle
07/05/15   fan/akh  CR834237 FR19329 Full G wake-up cancellation + code cleanup
07/05/15   npt      CR821672 GERAN power logging update  
30/04/15   ip       CR799995 FR26290 Sleep when X tech is in connected state
24/04/15   fan      CR796481 Full G wakeup cancellation
21/04/15   zf/cs    CR808438 Correction on GPS timetag reporting in wakeup
20/04/15   ip       CR811868 Dual Multisim mode code changes + debug overhaul
17/04/15   ip       CR823442 Fix warm up time when sleep from QTA to 5ms
31/03/15   pa       CR815621:GRM restructuring for TH 2.0.
16/03/15   pg       CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP
20/02/15   ab       CR773948 GL1 Support for provding Dumps and Metrics for QSH
31/03/15   pa       CR815621:GRM restructuring for TH 2.0. 
16/03/15   pg       CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP  
27/02/15   am       CR646584 WLAN coex
20/02/15   kb       CR788527 Modify the minimum adaptive warmup timeline for Extended paging case.
22/01/15   zf       CR784496: Enable sleep when BG traffic enabled.
17/02/15   df       CR779661 Enforce upper limit adaptive timeline & add logging
17/02/15   cah      CR777305 Replace depricated TRM type TRM_DENIED with TRM_DENIAL
02/02/15   jk       CR789189:Don't reset LIF table when waking up from sleep.
23/01/15   npt      CR697236 Mainline FEATURE_GERAN_VSTMR
16/12/14   jk       CR753423:LIF for Idle,voice and data
18/12/14   npt      CR758440 Take pending frame extension into account for sleep duration calculation
12/12/14   df       CR768848 Don't automatically commit from warmup cb during QTA
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
04/12/14   df       CR765200 Only reset adaptive timeline if 2 err recovery in same bank
26/11/14   jj       CR 760174 fix  compiler warning
17/11/14   npt      CR642426 Added power reporting log packet
01/11/14   df       CR748602 Rework and enable adaptive timeline
07/11/14   df       CR752087 Correct propagation error in CR734490
21/10/14   ssh      CR740899: Avoid sending MCPM wake up req before sleep req is processed.
21/10/14    fan     CR734490 Rework the wake up recovery mechanism
17/10/14   ws       CR730418 Correct GPS timetagging being out by 1 TDMA frame
17/10/14   ssh      CR737668: Reset ISR overrun condition if leaving or entering idle after rude wake up
15/10/14   ak       CR730234 Improvements to handling of sleep commit mask, and code tidy up
14/10/14   df       CR676084 Sleep debug structure improvements
14/10/14   zf       CR737010 Update FN when forced wakeup
07/10/14   pg       CR730132 F3 cleanup
06/10/14   pg       CR734405 Thor compilation errors
26/09/14   fan      CR730788 Moving the start RF timer next to setting RF_WAKEUP flag to the commit mask
                          and correctly abort PWR mon
01/10/14   df       CR611845 SBD interface updates for missed wakeup
24/09/14   cja      CR726441 In QTA wait for RF startup scripts to built before sending X2G startup
23/09/14   fan      CR727696 Avoid race condition between starting the timer in warmup CB and RF wakeup cnf processing
30/05/14   zf       CR662056: CMCC power test opt
24/09/14   cgc      CR729443 the correct fix for CR715098 prevent mutex deadlock.
18/09/14   zf       CR715098: De-register VSTMR immediately after start SLPC
12/09/14   ssh      CR723206: GSTMR odd even check API expects as_id not gas_id
12/09/14   fan      CR717902 Increase timeline when F3 is enabled
12/09/14   fan      CR636420: Run Sleep commit after GSTMR ISR at wakeup
09/09/14   cja      CR721468 In QTA do not prebuild scripts on forced wakeup
03/09/14   ssh      CR712967: Do not suspend TRM release for 2 frames if in low monitor mode
18/08/14   zf       CR710909: Ignore MCPM timeline CB if it has passed programmed wakeup point
25/08/14   df       CR660576 Enhanced timeline debug
28/07/14   zf       CR700704: Update sleep duration early in forced wakeup.
13/08/14   dg       CR690573 Remove compilation error
11/08/14   fan      CR707576 - remove changes of CR689276 Temporarily increase warmup time to 9ms and remove LMM reduction
11/08/14   fan       CR703945 Adaptive warm up timeline
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
31/07/14   npt      CR701530 Make sure wake-up is ongoing before trying to start VSTMR
30/07/14   ssh      CR701245: Avoid running an adjoining activity after rude wake up if the FW/RF is being initialized in that frame
28/07/14   ssh      CR700687 Handle X2G start up from warm up ISR
28/07/14   cja      CR697957 For QTA startup run build scripts in DPC task
25/07/14   ssh      CR612864 Do not run MCPM config request from WMGR CB handlers [Missing Merge]
03/07/14   zf       CR686704 In X+G DSDA mode, if wakeup conflict between two subs, delay G wakeup by one frame.
21/07/14   zf       CR697899 Correction on early go to sleep offset calculation
16/07/14   ssh      CR689277 On wake up cancel do trm release only if TRM status is denied and TRM/GRM reserve reasons match
25/06/14   fan      CR564208 Make sure we can handle panic reset whilst waking up from sleep anytime before we do sleep commit
19/06/14   npt      CR682701 Do not try to run async early go to sleep if normal go to sleep has already started
30/05/14   jj       CR656854 Resolve Critical Klocwork errors in gdrivers code space
08/05/14   hd       CR660875 Set the flag init_odd_even_check in gl1_hw_mcpm_restore_g_stmr_cb()
18/07/14   zf       CR696301 Do not wait for RF wakeup CNF if RF has not to been initialized
12/06/14   pg       CR661021 Fix GPS time snapshot before sleep for early go to sleep
11/06/14   npt      CR630396 Wake-up error recovery mechanism
04/07/14   cs       CR688003 GRM functions moved to gpl1_grm_intf.c
08/07/14   cja      CR690023 For QTA prevent prebuilding scripts, if building in progress
03/07/14   pg       CR689276 Temporarily increase warmup time to 9ms and remove LMM reduction
10/06/14   jj       CR 668965 merge ASDIV from DI.3.0 to BO 2.0
24/06/14   df       CR672016 Improve function parameter checking to avoid possible corruption
30/05/14   pg       CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM
19/06/14   ssh      CR679781 Clear any pending deferred WMGR events if trm reservation done prior to sleep
18/06/14   ssh      CR598637 Missing Merge
17/06/14   ssh      CR627985 [Missing merge] Skip first burst decode when waking up from sleep inside a QTA gap
19/06/14   cs/ch    CR651795 TRM Updated changes for DR-DSDS
12/06/14   ssh      CR671029 On G+G wake up collision defer wake up by 1 frame
06/06/14   ssh      CR650736: Offload the wake up call back for rude wakeups to DPC
03/06/14   cja      CR673927 Do not call RF sleep, if Rx already off.
29/05/14   cs/pa    CR660831 Use new trm_status definitions specific to GL1
21/05/14   ssh      CR668643 gl1_hw_qta_gap_active() should return FALSE for multimode sub
20/05/14   ssh      CR667235 Whilst reprogramming sleep on WUP cancel always check if all conditions met
14/05/14   cja      CR664278 Only set qta_cleanup_pending false when cm startup run.
                             For T2G share preloaded scripts between IRAT and QTA.
12/05/14   zc       CR660275 Do not set asdiv wakeup config unless granted TRM
05/05/14   npt      CR659178 Do not start sleep on first wake-up frame
05/05/14   npt      CR659600 Bolt multi-SIM sleep changes
28/03/14   zc       CR640583 QTA ASDIV sleep/wakeup transition fix
24/04/14   pg       CR655101 Compilation error fixes due to CR644441
11/04/14   cja      CR644441 Bolt: Preload script for QTA
09/04/14   ap       CR640266 L1 is unable to sleep because X2G reselect in progress is always indicated
07/04/14   cja      CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
01/04/14   xz       CR634903 Clear maintain_tds_ref_count timely if it is not forewakeup
01/04/14   ip       CR549949 Reduce warm up time for non LMM
28/03/14   zf       CR630392 Adjust early gts offset with pending TT adjustment
28/03/14   ssh      CR621230 Use extended warm up time in the event of G+G(+G) wake up collisions to avoid warm up ISR overrun
24/03/14   zf       CR637372 When wup cancelled, release TRM in DPC to avoid mutex deadlock
13/03/14   jj       CR  629747 DSDS: register inactive mode  before GL1 goes inactive
18/03/14   zf       CR634968 Ignore MCPM collision adjust if less than 8msec before scheduled wakeup
17/03/14   ssh      CR631959 Remove forced rude wake up for G+G(+G) case
19/03/14   ws       CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
14/03/14   sp       CR631864 Set Wakeup deferred FN correctly, so as to handle send of Missed Page/Prio Inv correctly.
14/03/14   ip       CR631131 Ignore MCPM collision adjust when EARLY decode active
14/03/14   ip       CR631187 move mdsp awake after Vreg on and before RF awake
10/03/13   ka       CR628885 Remove unused functions
13/03/14   ip       CR631044 Add some margin to warm up timeline
12/03/14   zf       CR630045 Release TRM when wakeup cancelled.
11/03/14   npt      CR626807 Increase warm-up time in non-LMM mode
06/03/14   npt      CR627339 Reduce warm-up time in LMM when f3's are disabled and USB is disconnected
05/03/14   jj       CR 610751  dime 3.0 dsda Asdiv
05/03/14   npt      CR626661 Re-enable parallel wake-up
03/03/14   npt      CR622060 Rework wake-up
13/03/14   ssh      CR631223 Force rude wake for all idle subs when transfer starts in G+G(+G) mode
12/03/14   ssh      CR627985 Skip first burst decode when waking up from sleep inside a QTA gap
12/03/14   ssh      CR629612 Do not clear total error recovery frames when it is next frame after sleep
19/02/14   sk       CR602282 Ensure Priority inversion applied after the deferred wake up
27/02/14   ip       CR608864 Dime3.0 DB timeline changes
26/02/14   jj       CR 623436 setting prefered config in dsds asdiv during sleep wake up
25/02/14   ssh      CR622707: Extend ignore the first QTA gap logic to SGLTE + G DSDA mode as well
24/02/14   ip       CR622111 - Remove GSTMR_INIT from MCPM geran restore callback
25/02/14   ssh      CR621741: GL1 does not release TRM lock after QTA gap when it self denies TRM as SCE schedules within QTA
22/02/14   ssh      CR621659: Reset the l1_warmup_isr_first_qta_ignore_in_progress after warm up ISR irrespective or rude wake up or not.
21/02/14   ssh      CR620675: Run silent ISR during GTS only when the GSTMR was already switched off.
20/02/14   ssh      CR613866: If TRM self-denied in the first QTA gap we should avoid running any [means all] activity
09/01/14   zf       CR598637: DRX only updates TRM booking if DRX activity is matching with the TRM booking
18/02/14   ssh      CR616652: Use both gstmr_is_int_pending and mod4 FN to check for pending GSTMR interrupt.
15/02/14   ip/npt   CR547955 - Define multi thread usage on wake up from sleep
13/02/14   npt      CR346414 - Removing the first GSTMR tick in PCH awake timeline
07/02/14   ssh      CR612864: Do not run MCPM config request from WMGR CB handlers
05/02/14   ssh      CR612396: Incorrect HAL vote mechanism means no sleep req to MCPM during GTS
05/02/14   ssh      CR608558: Run error recovery even during the first wake up after the collision detection.
03/02/14   jj       CR 610761 registering different config when QTA wakeup during dsds asiv
27/01/14   zf       CR604501: Extending MCPM warmup time when wakeup conflict
03/02/14   ssh      CR609484: Incorrect usage of USTMR ticks whilst GTS pending GSTMR detection.
03/02/14   ssh      CR610637:FEATURE_MSIM_VSTMR checkin introduced compilation errors
30/01/14   npt      CR608373 - Enable opt3 on Bolt/Remove unused apis
29/01/14   npt      CR602149 - Use vstmr_geran_view_adjust_offset api for time tracking adjustment
29/01/14   ws       CR608946 - Fixed Sleep timeline bug where we are always out 1 FN
24/01/14   cah      CR598552: enable sleep for DSDS GSTMR+VSTMR
23/01/14   npt      CR606388 Temp disable opt3
21/01/14   cgc      CR602010 TR2.0 merge fix,prevent WTOG BPLMN deactivate
20/01/14   jk       CR503359 - GL1 changes to support Rx diversity
19/01/13   dv       CR602224 QTA crash because TRM lock released after wakeup from sleep
15/01/14   ssh      CR600467:GL1 to implement a GTS recovery mechanism if there is a ISR overrun
14/01/14   jj       CR600114 tx- diversity in dime SGLTE
14/01/14   ssh      CR598609: Do not acquire mutex in the warm up CB till the MCPM sleep req is actioned
13/12/13   cja      CR589473 Add setting GfwAppModeType in GfwHostRfmEnterModeCmd
12/12/13   ssh      CR588188: ISR overrun recovery mechanism
11/12/13   cja      CR588195  Change to prevent CR478040 Blacklisting error
10/12/13    ws      CR 587586 - Remove DUAL_SIM featurisation in API
06/12/13   ssh      CR566871: maintain_<tech>_ref_count_change isn't dataspaced.
26/11/13   npt/ip   CR582180 Enable sleep on Bolt
22/11/13   cja      CR455527 Remove featurisation from files in API directory
08/01/14   js       CR594196: Resetting GBTA state in sleep warm up.
12/12/13   ssh      CR588188: ISR overrun recovery mechanism
06/12/13   ssh      CR566871: maintain_<tech>_ref_count_change isn't dataspaced.
04/12/13   zf       CR572394: Vote RFHAL bus without TRM lock
29/11/13   ssh      CR583144: On Rude wakeup make sure pending mcpm sleep req is done before initiating a mcpm wake up req.
01/12/13   sk       CR583361: Correcting the multisim sys mode check
29/11/13   zf       CR583313: To clear trm_requested_at_warmup flag after warmup ISR
28/11/13   ip/zf    CR582954: Prevent false detection for G in G+W when we check for committing to OLS
28/11/13   zf       CR582521: Extend the timeline by 150us.
26/11/13   ip/zf    CR581867: Adjust sleep timeline for RF init.
21/11/13   ssh      CR579864: GL1 sleep wake up error recovery counter to be reset when we commit wake up form recovery warm up CB.
26/11/13   ssh      CR581680: Geran sleep recovery isn't attempted due to incorrect int32 (-ve value) and uint32 comparison.
25/11/13   ip/ssh   CR580882:Extend warmup time in MS case when the power opts are disabled
02/10/13   pjr      CR552448: DDR Frequency plan implementation - l1_arbitration modification
21/11/13   zf       CR579808: Ignore the first GBTA/Partial QBTA gap.
20/11/13   zf       CR579309: Notify FW USTMR count at OLS.
19/11/13   ssh/ip   CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
14/11/13   zf       CR575593: For QTA, do not init RF FW in warmup.
08/11/13   ip       CR 547955 - Use multi threads on wake from sleep
28/10/13   ip       CR562197: Remove RF chain depenency on MCPM de configure
05/11/13   ssh      CR570747: DINOSTABLE-23645|Optimise the warm up time for LMM when in single sim mode
11/10/13   zf       CR552306: Notify GFW OLS USTMR time.
29/11/13   ssh      CR583144: On Rude wakeup make sure pending mcpm sleep req is done before initiating a mcpm wake up req.
28/11/13   cja      CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
22/11/13   cja      CR455527 Remove featurisation from files in API directory
21/11/13   ssh      CR579864: GL1 sleep wake up error recovery counter to be reset when we commit wake up form recovery warm up CB.
11/18/13   ip       CR562197 - Set RF Vreg off for current SUB
05/11/13   ssh      CR570747: DINOSTABLE-23645|Optimise the warm up time for LMM when in single sim mode
11/11/13   ws       CR 573757 Featurise GSMTR-1 registers and Voice services driver
31/10/13   cgc      CR569801  Add wrapper functions gl1_cm_hw_mcpm_restore_cb()
18/10/13   npt      CR542638 - Changes to support sleep on Bolt
11/10/13   zf       CR552306: Notify GFW OLS USTMR time.
15/10/13   js       CR508118: Send missed block ind if wakeup for CCCH cancelled
08/10/13   js       CR519541: Add wakeup from sleep during QTA support in idle ISR state mc,block sleep in QTA mode
05/10/13   sk       CR554665 Backout the power opt temperorily(CR537690)
20/09/13   zf       CR546809: Check RF ON per sub in DSDA.
03/09/13   npt      CR524770 - GL1 changes to support RFLM FED
02/10/13   ssh      CR553276 Decode xCCH after missing frames due to sleep wakeup error recovery [MERGE ERROR for CR385548]
02/10/13   ian/ssh  CR529717 GL1 warmup time enhancement in LMM to reduce power consumption
02/10/13  ssh      CR467013 - Disable task pre-emption during GSM sleep programming
24/09/13   zf       CR537690: Reduce G wakeup timeline for MS mode
11/09/13   pjr      CR521061 Removed deprecated header file rfgsm.h
03/09/13   ip       CR536229 Use ERR_FATAL
03/09/13   fan      CR537338 - QDSS events for GL1 timeline Profiling
10/09/13   cs       CR532298 Integrate all wakeup-mgr changes for Multi-SIM
10/09/13   mko      CR537561:GSM wakeup is deferred by 39 frames when wakeup conflict observed
14/08/13   npt      CR528491 - Use mod4096 instead of mode4 for interactions with FW
24/07/13   sai      CR514479: Defer wakeup for collision avoidance by 16 frames instead of 8.
10/07/13   sai      CR506991: Defer wakeup for collision avoidance by 8 frames instead of 6.
10/07/13   sai      CR509527: Ensure wakeup cancel enabled after MCPM sleep req completed.
09/07/13   sai      CR505662: Handle wakeup cancellation and early wakeup concurrency.
19/08/13   fan      CR530376 - GL1 starts sleep before slpc OLS serviced, so that slpc can not configure sleep duration and GL1 will not wakeup
15/08/13   ws       CR524752 back out this CR
14/08/13   sai      CR528501: Do not clear late_ols_offset at OLS for wakeup with QTA supported
13/08/13   fan      CR521324 - Do not use the OLS sleep controller callback to reprogram the terminal count
07/08/13   zf       CR524752: When QTA enabled, don't run gstmr_isr during warmup.
25/07/13   sai      CR515382: Adjust short frame duration at wakeup from sleep.
01/07/13   sai      CR506781: Cancel wakeup only if time left until warmup int is less than 40 ms.
14/06/13   pjr      CR490812 replaced gl1_hw_get_rf_warmup_time with gl1_hw_get_rf_gsm_warmup_time
31/07/13   ip       CR518821 - reduce the warmup time
04/07/13   zf       CR501765: Extend SS wakeup timeline to 4.5ms
10/07/13   ip       CR508975 - Adjust thresolds for CS power goals
01/07/13   ip       CR506628 - FC power code changes
26/06/13   cs       CR505156 Latest TSTS Interface updates
25/06/13   sai      CR503864: Fix MCPM-WMGR mutex deadlock when W and G go to sleep at the same time.
20/06/13   cs       Major Triton TSTS Syncup
17/06/13   sai      CR494015: Implemented GERAN wake up cancellation
14/06/13   pjr      CR490812 replaced gl1_hw_get_rf_warmup_time with gl1_hw_get_rf_gsm_warmup_time
10/06/13   zf       CR498540: Don't request mutex in ending_cb.
11/09/13   npt      CR489302 - Run the wake up ISR as part of the warm up interrupt routine
                    in GSM only mode on normal wake-up only
01/06/13   zf       CR494798: Don't start sleep before OLS.
23/05/13   ip       CR488554  Change for faster CPU clock speed
24/05/13   kb/npt   CR459207 - Maintain TDSCDMA frame reference count through
                    GSM sleep when there are TDSCDMA neighbours
22/05/13   ap       CR485912: Revised GFW state transitions
23/05/13   cs       More TSTS Updates
16/05/13   cs       TSTS Updates
20/05/13   ip       CR481692: Enabled scheduled wake ups
17/05/13   kb/pg    CR479833 - mark wakeup_complete on sleep state being AWAKE or NULL
17/05/13   kb/npt   CR385548 - Decode PCH after sleep wakeup error recovery.
17/05/13   kb/pg    CR464918 - Apply correct OLS mod4 frame num for warmup error recovery
16/05/13   kb/npt   CR457944 - Disable parity check if wake-up recovery has kicked in
07/02/13   pg       CR449164: include TT adjustment in sleep calculation
13/05/13   jk       CR463435:Using DAL interface to calculate GSM sleep time tag
26/04/13   zf       CR477992: Fix G2W IRAT issue
25/04/13   pjr      CR480197 dual dataspaced gl1_hw_cm_status
17/04/13   zf       CR476301: Enable DSDS MCPM state update to sleep.
16/04/13   zf       CR473679: Do not issue RF_ENTER mode command in DSDA mode.
12/04/13   zf       CR474876: Don't change sleep state to AWAKE in warmup.
04/04/13   ip       CR471225 - Fix for G2W timeline
27/03/13   ws       CR467440 - Removed call to gstmr_rd_symbol_count() in
                    gl1_hw_sleep_set_terminal_count()
24/03/13   ws       CR466973 - Fix missing GAS ID in gl1_hw_mcpm_gts_apc()
31/01/13   ss       CR447506 - Dime GSTMR V1/V2 compatibility changes
06/02/13   zf       CR408404: Return after panic reset when int pending and wakeup failure.
05/02/13   zf       CR400343: Using ISR_STALLED to handle sleep timeline failure
28/02/13   npt      CR447792 - Maintain frame reference count should not be
                    disabled until sleep cycle is completed
26/02/13   ws       Added MCPM_GERAN1 tech enum for GSM only MCPM callback
16/01/13   npt      CR439839 - Re-enable GSTMR IRQ after WCDMA power collapse
03/01/13   npt      CR430322 - Modify sleep debug structure
03/01/13   npt      CR433842 - Ensure that forced wake up will not be processed
                    if normal wakeup has already started
14/12/12   cja      CR426248 Wait for HW Tx Off before MCPM request for voice stop.
06/12/12   npt      CR428630 - Maintain WCDMA frame reference count while in sleep
06/12/12   npt      CR346414 - Remove first GSTMR after wake-up
06/12/12   npt      CR426809 - Modified forced wake-up calculations
20/11/12   npt      CR422880 - Modify wake-up recovery thresholds on Dime
14/11/12   npt      CR420954 - Added changes for power collapse
09/11/12   npt      CR380795 - Added core CPU vdd NPA client handle
08/11/12   npt      CR419262 - Disabled opt4
23/10/12   npt      CR413635 - Port sleep code for DIME modem
04/09/12   npt      CR388546 - Change in GL1 sleep recovery mechanism when only WCDMA timeline overruns
03/09/12   npt      CR394199 - Added WCDMA recovery wakeup callback function
03/08/12   npt      CR383592 - Reduce WCDMA warm-up time to avoid GSM and WCDMA
                    wake-up interrupts firing at the same time
13/07/12   npt      CR366747 - Switch GSM clock on before calling sleepctl_wakeup_now function in panic reset
12/07/12   npt      CR377917 - Increase warm-up time to accommodate for extended MCPM activities
10/07/12   npt      CR377473 - Do not send MCPM requests if TCXO shutdown is prevented
04/07/12   npt      CR374036 - Reduce warm-up time when low monitor mode is not active
04/07/12   npt      CR372860 - Remove unnecessary error fatal in warm-up callback function
04/07/12   npt      CR372530 - Reduce warm-up time in low monitor mode
04/07/12   npt      CR373673 - Provide additional debug information in case of insufficient warm-up time
                    and dynamically modify warm-up time
28/06/12   npt      CR372829 - Change warm-up time to cater for extended RF antenna tune
20/06/12   npt      CR370607 - Modify the minimum warmup time for commit to wake-up sleep controller processing
08/06/12   pa       GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
14/06/12   cja      CR366662 In MCPM cb toggle gstmr reset rather than set re-initialisation (which also deregisters handlers)
31/05/12   npt      CR360829 - Print out warning message when MCPM or RF clks on/off
                    takes longer than budgeted for
24/05/12   npt      CR361228 - Print out warning message when TCXO shutdown is prevented
27/04/12   npt      CR356317 - Backing out CR342931
25/04/12    ws      CR 355838 FR 1604 - GERAN changes to cleanup WCDMA- GL1 API
24/04/12   npt      CR354790 - Remove old sleep debug (under DEBUG_GL1_SLEEP) as it is not needed
                    after FEATURE_GL1_SLEEP_DEBUG_STRUCT is introduced
20/04/12   npt      CR348900 - Switch RF clks off when the sleepctl is armed
16/04/12   npt      CR342931 - Modified original change
16/04/12   npt      CR348903 - Reduce OLS by 10 symbols to avoid ISR overruns
13/04/12   npt      CR342931 - De-register WCDMA sleepctl and reduce warm-up time in low monitor mode
13/04/12   npt      CR342934 - Reduced normal warm-up time in non-LMM mode from 9ms to 6.5ms
30/03/12   cgc      Fix Dime compile errors
14/03/12   cgc      Add support for new sleep controller,slpc, FEATURE_GSM_SLPC_CONTROLLER
14/03/12   pg       Add support for local HWIO macros (msm.h deprecated)
14/03/12   pg       Add support for QuRT
14/03/12   pg       Add support for DAL interrupt controller (tramp deprecated)
21/03/12   ws       CR 345606 Improve stability of X2G Rxfe stuck recovery
29/03/12   cja      CR341225 RxFE recovery for X2G
20/03/12   npt      CR344882 - Increase allowed processing time in commit to wake up process
16/03/12   npt      CR307481 - Cancelling forced wakeup if wakeup interrupt has already run
16/03/12   npt      CR343683 - Modification of commit to wakeup logic
08/03/12   ip       CR326699 - Code changed for wake up recovery mechanism
28/02/12   npt      CR338599 - Reduced warm-up time for non-scheduled wakeup
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
02/02/12   npt      CR332455 - Reduced OLS value in LMM in order to avoid ISR overruns
02/02/12   npt      CR326943 - Clear mdsp command buffer on wake up from sleep
25/01/12   npt      CR311544 - Sleep data debug structure - fixed compilation errors
24/01/12   npt      CR329486 - Remove rfm_power_clock_on() function call from forced wakeup function handler.
24/01/12   npt      CR330084 - Fixed boundary overrun in sleep debug structure
23/01/12   npt      CR330947 - Allowed additinal 2ms for forced wake up processing
                    and removed unnecessary f3 messages
20/01/12   npt      CR315272 - Remove usage of deprecated legacy sleep APIs - Fixed compilation errors
12/01/12   npt      CR325775 - Added check for wake up in the past issue
12/01/12   npt      CR323780 - Modify warmup time when low monitor mode is not active
12/01/12   npt      CR324398 - Change of mcpm/rf clks on function calls in warmup callback function
12/01/12   npt      CR311544 - Added sleep debug structure - FEATURE_GL1_SLEEP_DEBUG_STRUCT
11/01/12   npt      CR315272 - Remove usage of deprecated legacy sleep APIs
04/01/12   cja      CR316946 Update W frame number during sleep
25/11/11   npt      CR320854 - Increased warmup time for 1.5ms and modified calculation of
                    delta between non-scheduled and scheduled warmup time
                    to make it flexible for any further changes of warmup times
14/11/11   ip       CR318865 Added scheduled wake up feature for 8960 only
08/11/11   npt      CR313042 Modified new wakeup point calculations in forced wakeup scenario
                    to prevent "wakeup in the past" issues (DSDS and SS fix)
19/10/11   cja      CR310225 Correct OLS delay time, include OLS delay in GPS time tag calculation
12/09/11   cja      CR305705 - Force reinit of GSTMR on MCPM cb
20/08/11   cja      CR301307 Register MCPM callback in CM init
18/08/11   cja      CR302396. Correct WSTMR setting during sleep
13/08/11   cja      CR 301388 For sleep/wakeup MCPM config use gl1_hw_mcpm_state_update
12/08/11   ws       CR301207 - Add 4mSec warmup for FW power collapse latency
05/08/11   ws       CR 299800 - Enable Sleep for Nikel MSM8960
12/07/11   npt      CR293222 Modified WCDMA sleep controller timeline
                    in forced wakeup scenario to prevent "wakeup in the past" issues
04/08/11   jj       CR299772 removed gstmr_force_reinit from gl1_hw_mcpm_restore_cb
17/06/11   cja      Clean up Nikel BU workarounds.
15/06/11   cja      In MCPM wake-up call back force gstmr re-init
23/05/11   cja      Changes for Nikel BU (remove FEE)
13/05/11   ws       Comment out sleepctl_init() for bringup, causes crash in NPA
28/04/11   cja      Add MCPM power collapse callback
26/04/11   ws       NikeL CMI change to support wl1api.h
18/04/11   nf       CR283604 panic reset race condition
28/03/11   ws       CR 279980 EBI bus speed increase due to Tx Band change
18/03/11   ws       Added Initial MCPM updates
01/07/11   ky       CR 294531 moving enter and leaving idle clk changes from isr to DPC
05/05/11   ip       CR 286693 Recovery mechanism when RPM delay warmup interrupt
27/06/11   cs       CR289602 Clear pending RX at sleep for DSDS G+G mode
23/06/11   npt      CR 288201 Relinquishing MIPS when going to sleep
17/06/11   ip       CR 276658 Remove RF warmup code no longer required on main
19/05/11   ip       CR 288439 Check RF power collapse in warmup callback
18/06/11   npt      Correcting merge error - CR276782 Canceling 340 MIPS when going to sleep
10/05/11   nf       CR284188 check and recovery for out of sequence wake-up
20/04/11   npt      CR276782 Canceling 340 MIPS when going to sleep
18/04/11   nf       CR283604 panic reset race condition
30/03/11   nt       DSDS CR278826 Not waking up in DSDS mode
28/03/11   ws       CR 279980 EBI bus speed increase due to Tx Band change
16/03/11   sjw      CR279390 - clear the FEE holding timer before trying to redefine it
22/02/11   ip/cs    CR267278 GP clock vote handled by RF drivers
16/02/11   ws       CR 275370 - Disable clk switching,Request 340 MIPS for duration
                    of GERAN RAT
21/01/11   ws       CR 263081 - Implement NPA Latency node of 1mSec
08/12/10   ws       CR 267264 - Merge NPA clients for CPU resource to 1 client to avoid
                    mutiple npa_cancel_request() which can take 1-2 mSecs to return
08/02/11   pg       Remove unused variables when using FEATURE_GSM_GP_CLK_DISABLED
26/01/11   pg       Added FEATURE_GSM_GP_CLK_DISABLED for sourcing GSM clock
                    from MSM in all modes i.e. GP_CLK always disabled
19/01/11   cs       Fix the asynch int max_symbol_count
14/01/11   ip       Enabled Option 4 power feature for 7x30
15/12/10   nt       DSDS - force on the tripler clock always in DSDS
09/12/10   cs       CR264682 Add support for asynch dsp completion interrupt
08/11/10   nf       9K warm up time changes
19/10/10   ip       CR260332 Ensure GP clock is on before sleep contoller
                    function calls in panic reset
08/10/10   ip       CR258096 Decrease on line start parameter to 600qs.
05/10/10   pg       CR258010: Avoid possible incorrect sleep state between disabling
                    GP_CLK and setting sleep state to ASLEEP.
15/09/10   rc       Removed CR251338
15/09/10   ip       CR248185 Increase MARM speed to 384MHz
13/09/10   ip       CR255202 - Re order force wakeup calculations
26/08/10   ip       CR251338 Cancel MARM clock speed request when going to sleep.
16/08/10   tjw      Complete the renaming of gl1_hw_sleep_debug_ending_isr
12/08/10   nf       CR250888 Check for GSM Ending ISR
23/07/10   ip       CR245474 Improve sleep debug perfomance code
23/07/10   ip       CR247948 Reduce the SCMM PCH awake timeline
23/07/10   ip       CR246831 Re order the warmup sleep calculation
23/07/10   dv       CR242019 - Only maintain Frame Reference Count
                    when not in GSM only mode
21/07/10   ip       CR247715 On Forced wakeup turn on GP clock ASAP
01/07/10   nf       Enabling support for 9K RF Power Collapse
22/06/10   ip       CR242020 Reduce warmup time and ols for 7x30,
                    do not cancel IDLE MIPS when going to sleep
18/06/10   ip       CR241108 add funtionality to enable/disable SSBI clk
07/06/10   cja      Fix Lint error
04/06/10   ip       CR240251 - Updates to the FEE calculation
24/05/10   tjw      hwi.h -> hw.h "hwi.h  This file is obselete, please use hw.h"
19/05/10   ip       CR238892 - Turn on GSM IDLE NPA client for Forced warmup
19/05/10   ip       CR237389 - Turn all GSM clocks on for Panic Reset
07/05/10   ip       CR235204 DCVS settings added for Poseidon 2
07/05/10   nf       9K changes for Clock Gating
09/04/10   ip       CR221022 Added extra FEE state to block an estimation if required
06/04/10   ip       CR233495 Add 4mS of warmup time for 7x30 for DCVS eanbled builds.
22/03/10   nf       Enable Sleep for 9K/GENESIS
18/03/10   nf       Check wake-up complete before attempting to start sleep
08/02/10   ws       Disabled rfm_power_clock_on and rfm_power_clock_off() for 9K
08/12/09   ws       Set wakeup_complete to TRUE if both GL1 and WL1 sleep_state
                    are NULL
02/02/10   ip       RF clock on/off now working in RF PLF#22 - therefore enable
26/01/10   ip       Disable RF clock switch during sleep to stop current crash
26/01/10   ip       CR224128 Added SCMM clock gating changes
09/11/09   ip       Added RF calls under sleep control
09/10/22   tjw      avoid compiler and lint warnings about unused variables when MSG
                    is defined away to nothing
03/09/09   nf       CR190704, set GOING_TO_SL before Sleep Int enabled
22/07/09   ip       CR182997, prevents double gl1_drx_wakeup_up issue
01/07/09   ip       CR187107 Ensure wcdma sleep state flag gets checked on
                    sleep ending.
01/07/09   nf       Replaced DEBUG_GL1_CLOCK with DEBUG_GL1_SLEEP
27/05/09   cs/nf    CR183660 Increase the FEE threshold to allow for corrections
                    out of spec sleep crystals during HT/HV testing
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
08/04/09   ip       CR177474 removed the sclk_init(), resets sclk
                    and loses track in RAT switching. Done once on power up.
20/03/09   cs       Fix Lint errors
27/02/09   ip       CR 173712 decode failures from timing errors
30/01/09   ip       CR 167135 Added API call for sleep debug code
30/01/09   nf       CR168086 - No Sleep while W-BPLMN active
16/01/09   cs       Disable the sleep and FEE processing for Mustang this needs
                    a revisit to do it properly
09/01/09   ip       Added additional wakeup time for CR167592
02/01/09   nt       Fix compilation issue for non MUSTANG builds
19/12/08   ip       CR167592  - Added LCU specific clock controll and lcu tcxo updates
08/12/08   cs       Add in the new split gl1_hw.c support
27/11/08   cs       Lint fixes
11/11/08   ip       Added Mustang feature to LCU sleep changes
04/11/08   ip       Added LCU clock regime changes
25/09/08   nf       CR152023 - Increased Forced warm-up by 1ms (M6290 & M6246)
04/09/08   nf       CR150915,CR150519 - always set Sleep enforced_warmup to FALSE
28/07/98   hv       Added KxMutex support
25/07/08   nf       CR151098 - reduced RF Warm-up time, by 1ms
10/07/08   nf       Fixed M7500 compilation errors
19/06/08   nf       6ms Reduction in GSM Awake time line
09/05/08   ip       CR142792 Reduction in GSM Awake time line
28/02/08   ip       CR139759 backout CR129514 as it causes camping problems on 7225 and 7201A
15/02/08   ip       CR138292 Call sleepctl_init independently of WCDMA sleep features.
24/01/08   cs       CR129514 Reduce the warmup time for MPM enabled targets
07/12/07   cs/ip    CR134215 Remove the problem with GL1/WCDMA sleep getting out of synch on
                    early wakeup
08/11/07   cs       Disable new sleep api support until sync with WL1 and remove assert F3
25/10/07   cs       Remove dependency on l1_sc.h for gsm_only indication for CR117884
17/10/07   ws       Corrected bug in OKTS voting in previous version
12/10/07   cs       CR129559 Add support for asserting sleep at task startup
21/09/07   cs       CR123278 - New unified GL1 clock API changes
21/09/07   cs       Add some sanity check on the return from rf warmup time
05/04/07   ip       Increased the OLS value to shorten PCH timeline.
06/08/07   ip       CR120641 - Added prevention of updating sclock with erroneous te
01/08/07   cs       Fix EDGE only build featurisation for 7205/7500-90 etc
11/06/07   ip       Featured hwi.h
05/06/07   ip       sleepctl_init called from within feature as WCDMA now
                    using unified sleep controller
01/03/07   ip       updates for GSM sleep - sleepctl_init
09/15/06   cs       Alter early wakeup margin to 1ms for 7200.
26/09/06   ip       CR 92957 - Changed error fatal to gl1_hw_handle_panic  and
                    added call back function for handling panics in sleepctl
09/06/06   gfr      Merged back in more missing clock regime resource manager
                    changes that got dropped somehow.
02/08/06   ip       Removed lint warning
07/21/06   pp       Support for 1x/umts unification.
06/26/06   gfr      Use WCDMA_STMR instead of GSM_STMR when forcing WCDMA wakeup,
                    support for clock regime resource manager in panic reset.
06/06/06   gfr      CR 91232 Increased MAX_NOT_SLEEPING_COUNT_MDSP to 10.
05/18/06   gfr      Support for clock regime resource manager.
05/16/06   pp       Support for unified sleep task.
04/24/06   gw       Add check for gstmr_count <= 3 when starting sleep.
04/06/06   ip       Use rfgsm_get_rf_warmup_time() to return hardware specific rf
                    warup times supplied by Drivers.
02/06/06   ip       Removed CLK_RGM_GSM_TCXO_WARMUP_TIME_US and
                    GSM_RF_WARMUP_TIME_US
01/13/06   ip       Add function gl1_hw_sleep_gsm_confirm_OKTS to replace macro
                    definition gsleep_confirm_OKTS() in gsleeptask.c, updated to
                    4ms from 50ms guard period. CR85932
12/14/05   gw       Removed references to ERR macro. Added some more comments
                    about the possible causes of errors in the timeline.
10/17/05   gfr      Moved GSAC code into gl1_hw_gsac.c
09/22/05   gfr      Remove FEATURE_SBI_MDSP_CONTROLLER and use
                    clk_regime_enable/disable_gsm_encryption_clk
08/04/05   gw/pp    Re-ordered #define's. Uninstall callbacks when leaving idle
                    to avoid spurious state changes. Use set_warmup_no_wakeup()
                    and sleepctl_block_until_latch_ok() to avoid wakeup/coarse
                    time overwrites within 4 sclks. Removed wcdma warmup cb.
                    Added wcdma profile points.
07/27/05    gw      Updated timeline constants to reflect drivers changes.
07/27/05    gw      Corrected a constant and improved error recovery.
07/27/05    gw      Modified timeline to make GSM go to sleep action more atomic.
                    Tweaked some timeline constants to make the timeline more
                    robust.
07/22/05    pp      Use GSM_RF_WARMUP_TIME_US instead of GSM_RF_WARMUP_TIME_us.
07/21/05    pp      Reset the sleepctl warmup time during initialization.
                    Added some sleep timeline constants for modularization.
07/20/05    pp      Added support to PANIC RESET. Reduced go to sleep and
                    delay OLS intervals back to their optimized values.
07/09/05    gw      Increased go to sleep time again. Added some more debug
                    code.
07/08/05    gw      Increased some intervals in the timeline.
06/21/05    pp      Increased FORCE WAKEUP processing and warmup times.
06/14/05    sv      Fix the ERR_FATAL due to ending the sleep timeline when a
                    PANIC RESET happens.
06/13/05    pp      Increase late OLS time.
06/10/05   gfr      Move gl1_turn_rx/tx_on/off to gl1_hw.c, use generic
                    clk_regime functions instead of clk regime names.
06/08/05    pp      GSM timeline optimizations.
06/04/05    gw      Added mechanism to keep FEE alive for a fixed amount of
                    time after leaving idle mode.  This eliminates the need
                    to redo the estimate after reselections.
                    Mainlined FEATURE_GSM_SLEEP_FEE_FROM_TE (in this file, build
                    as a whole can still turn off this feature).
05/25/05    gw      Added support for profiling sleep timeline.
05/25/05   gfr      Lint cleanup
05/24/05   gw/pp    Improved FEE mechanism.
05/12/05   gw/pp    Removed obsolete debug code. Added support for maintaining
                    WCDMA ref counter during GSM sleep for MSM6275. Mainlined
                    MDSP_SVC_DOES_CLK_SWITCHING. Removed code under feature
                    FEATURE_MODIFY_UART0_RX_DATA_INT_POL_IN_SLEEP.
04/22/05   gfr      Featurize FEE corection with FEATURE_GSM_SLEEP_FEE_FROM_TE
04/21/05    yh      Support for panic reset
04/21/05    bm      Added support for FEE correction using TE
04/11/05   gfr      Added support for sleep timetag
03/25/05    sr      Increased the FORCE_WAKEUP_SAFE_MARGIN_FOR_SLEEP_END to
                    approx. 15ms
03/18/05    bm      Increased the WARMUP time to account for TCXO warmup
03/09/05    yh      Disable rex stat during sleep
03/02/05    yh      Add a parameter to print sleep message only on the third
                    try for a NACKED error.
02/18/05    bm      Workaround to handle force wake up in warmup state
01/31/05    bm      Force wake up fix, incorrectly assuming sys_clks slept
                    Some API changes
01/10/04    bm      New Sleep Controller Support
12/06/04   gfr      Featurize the mDSP SBI controller
08/06/04    bk      Disable ref count check when FEATURE_GSM_EGPRS is defined. The ref
                    count is used in WCDMA which is not present in EGPRS builds.
06/22/04    gw      Changed MAX_NOT_SLEEPING_COUNT_MDSP from 2 to 3.
06/21/04    gw      Changed order of operations in start sleep function to avoid
                    sending mDSP sleep command with no corresponding wake up
                    command.
06/09/04    gw      Added support for checking mDSP status before sleeping
                    under FEATURE_GSM_SLEEP_CHECK_MDSP.
05/14/04    gw      Increased GTS time if FEATURE_GSM_SLEEP_EXTRA_GTS_TIME is
                    defined.
04/29/04   gfr      Lint cleanup.
04/27/04    hg      Added FEATURE_MULTI_RF_CHAIN_API.
03/04/04    gw      Merged in changes to support MSM6250 from branch.
                    Merged in support for sub frame scheduling from another
                    branch.
01/14/04    gw      Changed GL1_HW_FEE_TE_SNR_THRESHOLD from 0dB to 2dB.
10/22/03    gw      Added missing featurization around gl1_hw_advance_stmr().
10/15/03    gw      Moved mDSP clock switching into mDSP services if
                    FEATURE_MDSP_SVC_DOES_CLK_SWITCHING is defined.
09/29/03    gw      Added support fo updating STMR/WCDMA ref count on forced
                    wake up.
08/21/03    gw      Modified gl1_hw_turn_rx_{on|off}() to use multimode RF API.
10/07/03    gw      Added INTLOCKs/INTFREEs in GSM clock voting functions.
07/08/03   drl/mk   Added FEATURE_GSM1X_SLEEP_1X_CLK_OPT and
                    FEATURE_GSM1X_GSM_CLK_SWITCHING
06/26/03    gw      Moved call to sleep controller drivers earlier in start sleep
                    function to avoid roll over of GSMTR while other hardware
                    is disabled.
06/11/03    gw      Moved WCDMA clock regime switching out of
                    gl1_hw_sleep_gsm_enter_idle() to gl1_hw_init().
06/03/03    gw      Removed debug message.  Same info. now comes from L1.
05/23/03    gw      Added debug code to profile wake up timeline under
                    feature DEBUG_GSM_SLEEP_WU_PROFILING.  Added check of
                    frames_awake in gl1_hw_sleep_gsm_setup_ref_count().
04/22/03    gw      Added intlock/intfree in gl1_hw_turn_rx_on() to ensure that
                    initialization of RF completes before GSM frame tick ISR runs.
03/24/03    gw      Added check that GSM clock was turned on in time at wake up.
                    If check fails then GSTMR is reset to restart the frame tick.
02/25/03    gw      Removed control of Tx chain from enter/leave idle functions.
02/24/03    mk      Sleep/Standby optimization changes via enable/disable of WCDMA
                    clock regimes. Featurized using FEATURE_GSM_SLEEP_WCDMA_CLK_OPT.
02/21/03    gw      Made gl1_hw_turn_{rx|tx}_{on|off}() functions avialable
                    externally.
02/21/02    gw      Commented out code to turn off Tx when entering Idle Mode.
02/12/03    gw      Added support for timing error FEE functionality.
02/12/03   mk/gw    Initial revision created by moving code from gl1_hw.c.
==========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif
#include "comdef.h"
#include "mcs_hwio.h"
#include "geran_msgs.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_clk_ctl.h"
#include "gl1_hw_sleep_ctl.h"
#include "gl1_hwi.h"
#include "vtmrs_g.h"

#include "gprsdef_g.h"
#include "rex.h"
#include "rfm.h"

/* CgC removed sleep.h obsolete, sclk.h sleepctl.h moved into gl1_hw_sleep_ctl.h */
#include "l1_fm.h"

#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
#include "wl1api.h"
#endif

#include "gl1_mutex.h"
#include "l1i.h"
#include "timetick.h"
#include "l1_task.h"
#include "l1_os.h"
#include "l1_isr.h"

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "gl1_msg_g.h"
#include "trm.h"
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#include "DALSys.h"
#include "DDIInterruptController.h"
#include "DalDevice.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"

#include "mcpm_api.h"
#include "wmgr.h"

#include "gfw_sw_intf.h"

#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "geran_multi_sim.h"
#include "geran_dual_sim_g.h"
#include "gpl1_dual_sim.h"
#include "gl1_hw_g.h"
#include "mcfg_nv_api.h"
#include "l1_fm.h"
#include "l1_sc.h"
#ifdef FEATURE_MODEM_DDR_MGMT
#include "gl1_arbitrator_utils.h"
#endif /* FEATURE_MODEM_DDR_MGMT */

#ifdef FEATURE_WLAN_COEX_SW_CXM
#include "gl1_arbitrator_cxm.h"
#endif

#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
/*tracer header files*/
#include "tracer.h"
#include "gdrivers_tracer_event_ids.h"
#include "tracer_config.h"
#endif

#include "l1_drx.h"
#include "l1_utils.h"
#include "gtmrs_g.h"
#include "qurt_cycles.h"
#include "l1_sc_irat.h"

#include "mdsp_intf_g.h"

/************************** Temp DIME *****************/
typedef   uint32  sclk_32kHz_estimate_enum_type;
#include "geran_dual_sim_g.h"
/*===========================================================================

                     DEFINITIONS FOR MODULE

===========================================================================*/

/* General message header for sleep TMLN debug */
#define GL1_TMLN_HDR "GL1_TMLN: "

#define OLS_DELAY_TIME    650   /* Time (in symbols) to delay wake up into frame (600symbols = 2.2ms) */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#define MIN_FRAME_BOUNDARY_MARGIN_SCLKS  16      /*0.5msec*/
#endif

/* Threshold for commit/error recovery decisions 150us */
#define GL_HW_SLEEP_COMMIT_THRESHOLD_SCLKS 2000

/* Threshold for non LMM commit/error recovery decisions 500us */
#define GL_HW_SLEEP_COMMIT_THRESHOLD_NO_LMM_SCLKS 6500

#define GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_ACTIVITY_POSITION 12 /*type of activity is at 12 th bit position from right  w.r.t gl1_efs_slp_error_rec(input) */
#define GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_DELAY_POSITION 4
/*delay time is at 4 th position from right in w.r.t gl1_efs_slp_error_rec(input) */
#define GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_CYCLE_POSITION 8
/*No of cycles is at 8th positon from right  w.r.t gl1_efs_slp_error_rec(input) */
#define NO_OF_INTERVAL_OPTIONS 8
#define NO_OF_DELAY_OPTIONS 4
uint16 gl1_efs_slp_regular_int_cycle_lut[NO_OF_INTERVAL_OPTIONS]={0,10,20,30,40,50,60,70}; /*lookup table for choosing no.of cycles at which error recovery should happen */ 
uint16 gl1_efs_slp_regular_int_delay_lut[NO_OF_DELAY_OPTIONS]={2,4,6,8};
/* look up table for choosing the non blocking delay that we want to induce for error recovery */


uint32 optimised_warmup_delta[NUM_GERAN_DATA_SPACES];

static boolean gl1_hw_sleep_cm_active[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* When sleeping (DSDS) and the other tech is in connected state:
     - Wake 1 frame early. Therefore during warmup frame ONLY complete MCPM
       configure and commit to get VSTMR ticking. Don't call
       grm_request_at_warmup() or send GFW cmd in warmup.
     - During next frame (1st VSTMR tick), TRM request will be sent and cmd
       issued to GFW.
     - Minimum gap to sleep is 2 (to avoid attempting to set a wake up point in
       the past).
 
   Note: This flag should only be set/cleared once during a sleep/wake cycle
   (don't perform part of cycle with it set and latter part with it cleared) */
static boolean gl1_hw_sleep_qta_active[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

uint16 gl1_sleep_cnt[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };

gl1_hw_sleep_type gl1_hw_sleep_struct[NUM_GERAN_DATA_SPACES][NUM_OF_GL1_DEBUG_CYCLES];

uint16 gl1_hw_sleep_wgmr_dbg_cnt[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
gl1_hw_sleep_wgmr_dbg_type gl1_hw_sleep_wgmr_dbg[NUM_GERAN_DATA_SPACES][NUM_OF_GL1_DEBUG_CYCLES];

static boolean scheduled = FALSE;
static uint16 warmup = 0;

#ifdef FEATURE_GSM_ADAPTIVE_TMLN

/* Log the decisions made to adjust adaptive tmln after each bank of
   samples is averaged. This makes it possible to determine from a crash dump if
   the tmln had settled or was increasing/decreasing. */

/* Index points to the last valid element of gl1_hw_tmln_log */
uint16               gl1_hw_tmln_log_index[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };

gl1_hw_tmln_log_type gl1_hw_tmln_log[NUM_GERAN_DATA_SPACES][GL1_HW_TMLN_LOG_SIZE];

#endif /* FEATURE_GSM_ADAPTIVE_TMLN */

/* Global to store the Core CPU Latency state */
gl1_hw_corecpu_latency_type gl1_hw_latency_status[NUM_GERAN_DATA_SPACES];
static boolean geran_latency_node_created[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
#ifdef FEATURE_GSM_ADAPTIVE_TMLN
static void gl1_hw_adaptive_warmup_time(gas_id_t gas_id, int16 gap_to_ols_sample, boolean canCommit);
static uint32 gl1_hw_get_adaptive_warmup_time(uint32 wakeup_fn, gas_id_t gas_id );
static boolean gl1_hw_adaptive_is_sample_valid(gas_id_t gas_id);
static uint32 gl1_hw_adaptive_ccs_write_time_margin(gas_id_t gas_id);
static void gl1_hw_reset_adaptive_tmln_data_store(gas_id_t gas_id, boolean firstTimeUse);
static void gl1_hw_adaptive_clear_valid_flag(gas_id_t gas_id);
static void gl1_hw_adaptive_range_check(gas_id_t gas_id);

#endif
/*===========================================================================

                             LOCAL STORAGE

===========================================================================*/
/* ----------------------------------------------------------------------- */
/* Sleep State                                                             */
/* ----------------------------------------------------------------------- */
static gl1_hw_sleep_state_T gl1_hw_sleep_state[NUM_GERAN_DATA_SPACES];


/* TEMP for SCMM to stop current RF clock gating crash */
extern void l1_call_panic_reset_in_task( l1_panic_reset_type_T error_code, gas_id_t gas_id );
extern void gl1_hw_recovery_update_ccch_params(gas_id_t gas_id );
extern void gl1_hw_dtm_frame_tick (gas_id_t gas_id);
extern boolean l1_sc_is_ncell_acq_pending (gas_id_t gas_id );
extern l1_fm_priority_T l1_fm_current_activity[];

/* ----------------------------------------------------------------------- */
/* Duration of most recent (or current) sleep controller sleep.            */
/* ----------------------------------------------------------------------- */

typedef struct
{
  /* GSTMR start sleep request, returns time and phase of timer */
  slpc_tstmr_type   gsm_slpc_start_t_stmr;

  /* GSTMR online start sleep request, time and phase of timer */
  slpc_tstmr_type   gsm_slpc_ols_t_stmr;
}
gsm_slpc_type;

gsm_slpc_type gsm_slpc[NUM_GERAN_DATA_SPACES];


/* ----------------------------------------------------------------------- */
/* Wakeup cancellation support                                             */
/* ----------------------------------------------------------------------- */

/* Variables to support wakeup cancellation in Multi-SIM mode */

/* Tracks the current wake up point FN */
uint32 gl1_hw_wkupmgr_wakeup_point_fn[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( FRAMES_IN_HYPERFRAME ) };

/* Tracks if a wmgr event is pending to be handled after GL1 goes to sleep */
boolean gl1_hw_sleep_any_pending_deferred_wmgr_event[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( FALSE ) };

boolean gl1_hw_sleep_wmgr_cancel_in_prog[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( FALSE ) };

boolean gl1_hw_sleep_wmgr_cancel_after_prog[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( FALSE ) };

uint32 sleep_duration_frames[NUM_GERAN_DATA_SPACES];

uint32 sleep_duration_gsm_clks[NUM_GERAN_DATA_SPACES];

static ustmr_type gl1_hw_ustmr_timetick_at_ols[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( 0 ) };

boolean entered_warmup_recovery[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( FALSE) };

trm_reason_enum_t gl1_hw_sleep_wmgr_cancel_trm_reason[NUM_GERAN_DATA_SPACES];

boolean gl1_hw_sleep_wmgr_pch_cancel_allowed[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE(TRUE) };

uint32 gl1_hw_sleep_wmgr_trm_when_cur[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE(0) };

uint32 gl1_hw_sleep_wmgr_missed_PCH_time[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE(0) };

uint32 gl1_hw_sleep_wmgr_trm_when_at_gts[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE(0) };

uint32 gl1_hw_sleep_wmgr_trm_res_fn_at_gts[NUM_GERAN_DATA_SPACES];

uint32 gl1_hw_sleep_wmgr_warmup_incl_in_trm_fn[NUM_GERAN_DATA_SPACES];

boolean gl1_hw_sleep_wmgr_trm_when_at_gts_valid[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE(FALSE) };

uint32 gl1_hw_sleep_wmgr_result[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( GL1_HW_SLEEP_WMGR_RES_NO_ERROR ) };

uint32 gl1_hw_sleep_next_allowed_ccch_fn[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( NOT_A_FRAME_NUMBER ) };

#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
uint32 gl1_hw_sleep_wmgr_els_warmup_dur_in_sclk[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE(0) };
#endif        

/* ----------------------------------------------------------------------- */
/* Pointer to an L1 function to call when GSTMR is disabled for sleep      */
/* ----------------------------------------------------------------------- */
gl1_hw_sleep_gsm_cb_type sleep_start_cb_ptr[NUM_GERAN_DATA_SPACES];
gl1_hw_sleep_gsm_cb_type sleep_warmup_cb_ptr[NUM_GERAN_DATA_SPACES];

boolean gl1_hw_sleep_disable[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

/* ----------------------------------------------------------------------- */
/* GSM warmup time in micro seconds.                                       */
/* ----------------------------------------------------------------------- */
static uint32 gl1_hw_sleep_warmup_time_us[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };

/* ----------------------------------------------------------------------- */
/* Variables for maintaining WCDMA ref count through sleep.                */
/* ----------------------------------------------------------------------- */
#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
#ifdef FEATURE_SLPC_TRIPLE_SIM
static boolean maintain_tds_ref_count[NUM_GERAN_DATA_SPACES];
static slpc_tstmr_type t_stmr_slam_value;
#endif /* FEATURE_SLPC_TRIPLE_SIM */

static boolean maintain_wcdma_ref_count[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

static slpc_tstmr_type w_stmr_slam_value;

static boolean G_commit[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
#endif

/* ----------------------------------------------------------------------- */
/* Offsets from frame tick for last go to sleep / wake up                  */
/* units are GSM symbols                                                   */
/* ----------------------------------------------------------------------- */
static uint32 early_gts_offset[NUM_GERAN_DATA_SPACES]; /* gts = go to sleep */
static uint32 late_ols_offset[NUM_GERAN_DATA_SPACES];  /* ols = online start */

/* ----------------------------------------------------------------------- */
/* Variables for maintaining VSTMR frame number at GTS and OLS             */
/* ----------------------------------------------------------------------- */
uint16 gl1_hw_frame_number_ols[NUM_GERAN_DATA_SPACES] =
                { INITIAL_VALUE( 0 ) };
uint16 gl1_hw_frame_number_mdsp_wakeup[NUM_GERAN_DATA_SPACES] =
                { INITIAL_VALUE( 0 ) };
uint16 gl1_hw_frame_number_gts[NUM_GERAN_DATA_SPACES] =
                { INITIAL_VALUE( 0 ) };

/* ----------------------------------------------------------------------- */
/* For debug purposes count the number of times the mDSP is not ready to   */
/* sleep.                                                                  */
/* ----------------------------------------------------------------------- */
static uint32 not_sleeping_count_mdsp[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };
static uint32 forced_sleep_count_mdsp[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };

static boolean gl1_hw_sleep_rf_on[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
static boolean gl1_hw_sleep_rf_cnf_pending[NUM_GERAN_DATA_SPACES] = 
                 { INITIAL_VALUE( FALSE ) };
/* ----------------------------------------------------------------------- */
/* For debug purposes count the number of attempts to sleep while not in   */
/* awake sleep state.                                                      */
/* ----------------------------------------------------------------------- */
static uint32 not_ready_to_sleep_count[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };
static boolean scheduled_timeline_adjustment[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
static const char* gl1_hw_latency_node_nm[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( "/core/cpu/vdd" ) };

static const char* gl1_hw_latency_client_nm[NUM_GERAN_DATA_SPACES] =
                 {INITIAL_VALUE_DIFFERENT("gl1_hw_latency_client[0]",
                                          "gl1_hw_latency_client[1]",
                                          "gl1_hw_latency_client[2]")};

npa_client_handle gl1_hw_latency_client[NUM_GERAN_DATA_SPACES];

DalDeviceHandle* gsmSleep_hInterruptHandle[NUM_GERAN_DATA_SPACES];
/* Interrupt vector number - in legacy tramp usage this was the tramp id */
#define TRAMP_SLEEP_INT0_ISR 30

/* ----------------------------------------------------------------------- */
/* Variables to keep track of warm-up time adjustment in recovery          */
/*                                                                         */
/* ----------------------------------------------------------------------- */
static uint8    gl1_hw_consecutive_recovery_counter[NUM_GERAN_DATA_SPACES] =
                  { INITIAL_VALUE( 0 ) };
static uint32   gl1_hw_error_rec_warmup_adjust[NUM_GERAN_DATA_SPACES] =
                  { INITIAL_VALUE( 0 ) };
static boolean forced_wakeup[NUM_GERAN_DATA_SPACES] =
                  { INITIAL_VALUE( FALSE ) };

/* Variable to tracking if there is pending sleep req to MCPM through DPC */
boolean mcpm_sleep_req_pending[NUM_GERAN_DATA_SPACES] =
                  { INITIAL_VALUE( FALSE ) };

static uint8    total_error_recovery_frames[NUM_GERAN_DATA_SPACES] =
                  { INITIAL_VALUE( 0 ) };

static uint8 gl1_hw_sleep_commit_mask [NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE( 0 )};
static boolean in_warmup[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };

rex_timer_type   RF_cmd_RESULT_counter[NUM_GERAN_DATA_SPACES];
uint64 rf_timer_start, rf_timer_stop;
boolean gl1_hw_rf_result_monitor_timer_active[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE) };
#ifdef FEATURE_GSM_ADAPTIVE_TMLN
adaptive_warmup_tmln_struct_type adaptive_warmup_tmln[NUM_GERAN_DATA_SPACES];
#endif

/* Time GRF estimates is required from the end of RF ENTER MODE to
   the start of GFW beta to allow the CCS writes to the RFFE to finish.
   Note: GFW beta is always after the OLS, therefore this estimate can be
   compared to rf_enter_mode_to_ols when making a commit decision.*/
static uint32 gl1_hw_ccs_write_time_estimate_us[NUM_GERAN_DATA_SPACES] =
                  { INITIAL_VALUE(0) };

/* Time from completion RF ENTER MODE to the OLS */
static int32 gl1_hw_enter_mode_completed_to_ols_us[NUM_GERAN_DATA_SPACES] =
                  { INITIAL_VALUE(0) };
/* Not all wakeups require an enter mode, this flag indicates if gl1_hw_enter_mode_completed_to_ols_us
   contains a valid value and therfore must be used in commit decisions */
static boolean gl1_hw_enter_mode_sent_this_wakeup[NUM_GERAN_DATA_SPACES] = 
                  { INITIAL_VALUE( FALSE ) };

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

/*===========================================================================

                             LOCAL MACROS

===========================================================================*/


/*
** CgC Early wakeup from low priority task must complete, and not get delayed by other tasks ( GPS).
**     raise priority to prevent premption on this thread, for qdsp6 this maps to rex_set_pri(MAX)
*/
#define GSM_DETERMINISTIC_TIMING_ENTER \
   REX_DISABLE_PREMPTION();            \
   MSG_GERAN_ERROR_3_G(GL1_SLEEP_HDR" GPS-srt curr pri %d switch to %d %d",old_prio,REX_MAX_PRIORITY,timetick_get_safe()); \

#define GSM_DETERMINISTIC_TIMING_EXIT  \
   REX_ENABLE_PREMPTION();             \



#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
/* Info for next two macros:
 * 3072  = 2^10 * 3
 * 1300  = 2^2  * 5^2 * 13
 * 60000 = 2^5  * 3   * 5^4
 */

/*===========================================================================

 GSM_CLKS_TO_Cx8_CLKS

DESCRIPTION
 Converts from GSM clocks to WCDMA chipx8 clocks.  Return value is
 x * 30.72MHz / 13MHz.

 Argument must be less than ((2^32) - 1) / 768 = 5,592,405 ~ 430ms

===========================================================================*/
/* #define GSM_CLKS_TO_Cx8_CLKS(x) (((x)*768UL)/325UL) */

/*===========================================================================

MACRO GSM_FRAMES_TO_Cx8_CLKS

DESCRIPTION
 Converts from GSM frames to WCDMA chipx8 clocks.  Return value is

 x * 30.72MHz / (13MHz/60,000)

 Argument must be less than ((2^32) - 1) / 1,843,200 =  2330 ~ 10.7s

===========================================================================*/
/* #define GSM_FRAMES_TO_Cx8_CLKS(x) (((x)*1843200UL)/13UL) */
/* Commented out to stop lint complaining its not referenced. */
#endif  /* ...INTERRAT... */

#define GL1_HW_SLEEP_SCHEDULED_WARM_UP 7000
#define GL1_HW_SLEEP_NONSCHEDULED_WARM_UP 15000
#define GL1_HW_SLEEP_NONSCHEDULED_CM_WARM_UP 3000

/* 2msec to allow for MCPM API latency, 4msec to allow for FW power collapse */
#define GL1_HW_SLEEP_MCPM_WARMUP 6000
#define GL1_HW_SLEEP_GSM_CLK_FOR_8MSEC  (8000*GSM_CLK_FREQUENCY)
#define GL1_HW_SLEEP_MCPM_DELAY_FRAMES  1

/* warmup up time delta for scheduled wakeups */
#define GL1_HW_SLEEP_WARMUP_ADJUST      13000
#define GL1_HW_SLEEP_WARMUP_ADJUST_TWO_SUB_CLASH  3000
#define GL1_HW_SLEEP_WARMUP_ADJUST_THREE_SUB_CLASH  4500
/* additional warmup up time delta for scheduled wakeups when low montior mode is active */
#define GL1_HW_SLEEP_LMM_WARMUP_ADJUST  1000
#define GL1_HW_SLEEP_LMM_DIAG_MASK_ADJUST  2300
#define GL1_HW_SLEEP_LMM_F3_MASK_ADJUST  1500

/* OLS delta for scheduled wakeups when low montior mode is active */
//#define GL1_HW_SLEEP_LMM_OLS_ADJUST     340
#define GL1_HW_SLEEP_LMM_OLS_ADJUST     0

#define GL1_HW_SLEEP_FIELD_WARMUP_ADJUST  0



extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[];

/*===========================================================================

                   PROTOTPYES FOR INTERNAL FUNCTIONS

===========================================================================*/
static int64 gl1_hw_sys_clks_until_wakeup( gas_id_t gas_id );
static void gl1_hw_sleep_gsm_warmup_cb( gas_id_t gas_id );
static void gl1_hw_sleep_gsm_warmup_gas_1_cb( void );
#ifdef FEATURE_DUAL_SIM
static void gl1_hw_sleep_gsm_warmup_gas_2_cb( void );
#endif
#if defined ( FEATURE_TRIPLE_SIM )
static void gl1_hw_sleep_gsm_warmup_gas_3_cb( void );
#endif /* FEATURE_TRIPLE_SIM */
static void gl1_hw_sleep_warmup_recover( gas_id_t gas_id );
static void gl1_hw_msim_sleep_commit(gas_id_t gas_id);
static boolean gl1_hw_ok_to_sleep( gas_id_t gas_id );
static boolean gl1_hw_mdsp_sleep( boolean *forced, gas_id_t gas_id );

void gl1_hw_sleep_set_rf_on( boolean rf_on, gas_id_t gas_id );
boolean gl1_hw_sleep_get_rf_on( gas_id_t gas_id );

uint32 gl1_hw_sleep_get_sys_clk_to_ols( gas_id_t gas_id );
uint32 gl1_hw_sleep_get_qs_to_ols( gas_id_t gas_id );

/* Added for the error recovery mechanism */

static void gl1_hw_increment_frame_number( int32 num, gas_id_t gas_id );

static void gl1_hw_sleep_set_in_warmup_check( boolean warmup, gas_id_t gas_id );

#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )

#ifdef FEATURE_GSM_TDS
uint32 gsm_clks_to_tds_cx8_clks( uint32 x );
#endif /*FEATURE_GSM_TDS*/
#endif /* ...INTERRAT... */


static uint32 gl1_hw_normal_warmup_time( gas_id_t gas_id );
static uint32 gl1_hw_forced_warmup_time( gas_id_t gas_id );
static uint32 gl1_hw_symbols_to_delay_ols( gas_id_t gas_id );
static uint32 gl1_hw_max_symbol_count_to_sleep( gas_id_t gas_id );
static void gl1_hw_sleep_set_forcedwakeup(gas_id_t gas_id, boolean isForcedWakeup);
static void gl1_hw_sleep_set_warmup_cb(gas_id_t gas_id, boolean isDPC);
static void gl1_hw_sleep_gsm_warmup_cb_dpc( gas_id_t gas_id, slpc_event_callback_type w_cb);
static void gl1_hw_sleep_gsm_warmup_gas_1_cb_dpc( void );
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static void gl1_hw_sleep_gsm_warmup_gas_2_cb_dpc( void );
#if defined ( FEATURE_TRIPLE_SIM )
static void gl1_hw_sleep_gsm_warmup_gas_3_cb_dpc( void );
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
extern boolean l1_get_background_wcdma_plmn_scan_active( gas_id_t gas_id );
extern void l1_set_background_wcdma_plmn_scan_active( boolean background_wcdma_plmn_scan_active, gas_id_t gas_id );
#endif

static void gl1_hw_mcpm_slp_tmln_gas_1_cb( boolean optimise_gsm_sleep_timeline );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static void gl1_hw_mcpm_slp_tmln_gas_2_cb( boolean optimise_gsm_sleep_timeline );

#if defined ( FEATURE_TRIPLE_SIM )
static void gl1_hw_mcpm_slp_tmln_gas_3_cb( boolean optimise_gsm_sleep_timeline );
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

static void gl1_hw_mcpm_slp_tmln_cb( boolean optimise_gsm_sleep_timeline, gas_id_t gas_id );

static void gl1_hw_core_cpu_latency_avail_gas_1_cb( void *context,
                                                    unsigned int event_type,
                                                    void *data,
                                                    unsigned int data_size );
static void gl1_hw_core_cpu_latency_avail_gas_2_cb( void *context,
                                                    unsigned int event_type,
                                                    void *data,
                                                    unsigned int data_size );

#if defined ( FEATURE_TRIPLE_SIM )
static void gl1_hw_core_cpu_latency_avail_gas_3_cb( void *context,
                                                    unsigned int event_type,
                                                    void *data,
                                                    unsigned int data_size );
#endif /* FEATURE_TRIPLE_SIM */

static void gl1_hw_core_cpu_latency_avail_cb( void *context,
                                              unsigned int event_type,
                                              void *data,
                                              unsigned int data_size,
                                              gas_id_t gas_id );

static void gl1_hw_mcpm_restore_cb_geran( gas_id_t gas_id );

static void gl1_hw_sleep_wake_up_manager_cb_handler_dpc( gas_id_t gas_id );

static int32 gl1_hw_get_enter_mode_completed_to_ols_us(gas_id_t gas_id);
static boolean gl1_hw_check_enter_mode_sent_this_wakeup(gas_id_t gas_id);
static void gl1_hw_reset_time_enter_mode_completed(gas_id_t gas_id);
uint32 gl1_hw_get_ccs_write_time_estimate(gas_id_t gas_id);

/* Added for EFS sleep control mechanism */
boolean gl1_is_qdss_logging_enabled( gas_id_t gas_id );
boolean gl1_is_sleep_disabled_with_gcf( gas_id_t gas_id );
boolean gl1_is_sleep_disabled( gas_id_t gas_id );
boolean gl1_is_adaptive_timeline_disabled( gas_id_t gas_id );
/*===========================================================================

                      EXTERNALLY AVAILABLE FUNCTIONS

===========================================================================*/
#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
/*===========================================================================

FUNCTION  gl1_hw_sleep_maintain_ref_count

DESCRIPTION
  Controls whether the WCDMA/TDS reference counter is maintained during sleep.
  By default ref count is not maintianed during sleep since doing so may
  increase awake time.

  maintain_ref_count: TRUE  - the correct value will be loaded into ref
                              count at the end of sleep
                      FALSE - value in ref count is arbitrary after sleep

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_SLPC_TRIPLE_SIM
void gl1_hw_sleep_maintain_ref_count( gl1_hw_alt_id_T alt_id, boolean maintain_ref_count, gas_id_t gas_id )
#else
void gl1_hw_sleep_maintain_ref_count( boolean maintain_ref_count, gas_id_t gas_id )
#endif /* FEATURE_SLPC_TRIPLE_SIM */
{

#ifdef FEATURE_SLPC_TRIPLE_SIM
      if ( alt_id == WCDMA )
      {
      maintain_wcdma_ref_count[gas_id] = maintain_ref_count;

      if ( maintain_wcdma_ref_count[gas_id] )
      {
         MSG_GERAN_MED_0_G( GL1_SLEEP_HDR" WCDMA ref count during sleep ON" );
      }
      else
      {
         MSG_GERAN_MED_0_G( GL1_SLEEP_HDR" WCDMA ref count during sleep OFF" );
      }
      }
      else if ( alt_id == TDS )
      {
         maintain_tds_ref_count[gas_id] = maintain_ref_count;

         if ( maintain_tds_ref_count[gas_id] )
         {
            MSG_GERAN_MED_0_G(GL1_SLEEP_HDR" TDS ref count during sleep ON");
         }
         else
         {
            MSG_GERAN_MED_0_G(GL1_SLEEP_HDR" TDS ref count during sleep OFF");
         }
      }
#else
     maintain_wcdma_ref_count[gas_id] = maintain_ref_count;

      if ( maintain_wcdma_ref_count[gas_id] )
      {
         MSG_GERAN_MED_0_G( GL1_SLEEP_HDR" WCDMA ref count during sleep ON" );
      }
      else
      {
         MSG_GERAN_MED_0_G( GL1_SLEEP_HDR" WCDMA ref count during sleep OFF" );
      }
#endif /* FEATURE_SLPC_TRIPLE_SIM */

}

#endif


/*===========================================================================

FUNCTION gl1_hw_sleep_rf_wakeup_end

DESCRIPTION
  Called when RF returns wake up CNF

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_rf_wakeup_end(gas_id_t gas_id)
{

   gl1_hw_sleep_type  *gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];

   gl1_hw_sleep_debug_duration ( &(gl1_hw_sleep_cycle->wakeup.rf_awake) );

   MSG_GERAN_LOW_1_G( GL1_SLEEP_HDR" GSM RF parallel tasks COMPLETED rf_vreg/awake = %dus ",
                        gl1_hw_sleep_cycle->wakeup.rf_awake.duration_us);

}

/*===========================================================================

FUNCTION gl1_hw_sleep_debug_duration

DESCRIPTION
  Common debug timeline calculation

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_debug_duration (gsm_sleep_common_type *p)
{
    p->end_ustmr = slpc_get_tick();
    p->duration_ustmr = (p->end_ustmr - p->start_ustmr);
    p->duration_us = ((( p->duration_ustmr ) * 107 ) >> 11 );
}

/*===========================================================================

FUNCTION gl1_hw_wakeup_complete

DESCRIPTION
  Indicate if GL1 and WCDMA wakeup timelines finished

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if both finished FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_wakeup_complete( gas_id_t gas_id )
{
  boolean wakeup_complete = FALSE;

  GDRV_ISR_LOCK(gas_id);

  /* GSM Timeline only support for MPM sleep controller */
  if((((gl1_hw_sleep_state[gas_id] == GL1_HW_SLEEP_AWAKE) || (gl1_hw_sleep_state[gas_id] == GL1_HW_SLEEP_NULL))&&(!gl1_drx_first_awake_frame(gas_id)))|| gl1_hw_sleep_commit_mask[gas_id] == CLEAR_COMMIT)
  {
    wakeup_complete = TRUE;
  }

  GDRV_ISR_UNLOCK(gas_id);

  if(wakeup_complete == FALSE)
  {
      MSG_GERAN_MED_1_G(GL1_SLEEP_HDR" Wake-up Not Complete hw %d",
                        gl1_hw_sleep_state[gas_id]);
  }

  return ( wakeup_complete );
}

#ifdef FEATURE_GSM_ADAPTIVE_TMLN
/*===========================================================================

FUNCTION gl1_hw_reset_adaptive_tmln_data_store

DESCRIPTION
  Resets the data store. If its first time use then the whole data store is initialised.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_hw_reset_adaptive_tmln_data_store(gas_id_t gas_id, boolean firstTimeUse)
{
  uint8 index_counter = 0;
  adaptive_warmup_tmln_struct_type *tmln_ptr = &adaptive_warmup_tmln[gas_id];

   /* Save data to TMLN log. This makes it possible to determine from a crash
      dump if the tmln had settled or was increasing/decreasing. */
  gl1_hw_tmln_log_type *tmln_log_ptr = NULL;

  gl1_hw_tmln_log_index[gas_id]++;
  if (gl1_hw_tmln_log_index[gas_id] >= GL1_HW_TMLN_LOG_SIZE)
  {
    gl1_hw_tmln_log_index[gas_id] = 0;
  }

  tmln_log_ptr = &gl1_hw_tmln_log[gas_id][gl1_hw_tmln_log_index[gas_id]];

  tmln_log_ptr->first_time_use            = firstTimeUse;
  tmln_log_ptr->index                     = tmln_ptr->index;
  tmln_log_ptr->error_cnt                 = tmln_ptr->error_cnt;
  tmln_log_ptr->current_warmup_time       = tmln_ptr->current_warmup_time;
  tmln_log_ptr->average_gap_to_ols        = tmln_ptr->average_gap_to_ols;
  tmln_log_ptr->highest_CCS_Write_time_us = tmln_ptr->highest_CCS_Write_time_us;
  tmln_log_ptr->step_size                 = tmln_ptr->step_size;

  /* Reset the samples */
  for (index_counter = 0; index_counter < ADAPTIVE_TMLN_SAMPLE_COUNT; index_counter++)
  {
    tmln_ptr->samples[index_counter] = tmln_ptr->initial_warmup_time;
  }

  tmln_ptr->index              = 0;
  tmln_ptr->average_gap_to_ols = 0;
  tmln_ptr->error_cnt          = 0;
  tmln_ptr->valid              = FALSE;

  /* We might be entering Idle from RACH/DEDI/TRANS activity.
     In which case we needn't reset these values. */
  if(firstTimeUse)
  {
     tmln_ptr->initial_warmup_time           = INITIAL_WARMUP_TIME;
     tmln_ptr->current_warmup_time           = INITIAL_WARMUP_TIME;
     tmln_ptr->step_size                     = WARMUP_STEP_SIZE;
     tmln_log_ptr->highest_CCS_Write_time_us = 0;
  }
}


/*===========================================================================

FUNCTION gl1_hw_set_adaptive_warmup_time

DESCRIPTION
  Set the new warm up duration to be used for next wake up from sleep
  If called from inside QTA set to fixed value of QTA_WARMUP_TIME

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_set_adaptive_warmup_time( gas_id_t gas_id, uint32 set_warmup_time )
{
  adaptive_warmup_tmln_struct_type *tmln_ptr = &adaptive_warmup_tmln[gas_id];
  tmln_ptr->current_warmup_time = set_warmup_time;
}
/*===========================================================================

FUNCTION gl1_hw_get_adaptive_warmup_time

DESCRIPTION
  Return the new warm up duration to be used for next wake up from sleep.

  Parameter wakeup_fn is the SW FN for the first VSTMR tick.

DEPENDENCIES
  None

RETURN VALUE
  Adaptive warmup time (us).

SIDE EFFECTS
  None

===========================================================================*/
static uint32 gl1_hw_get_adaptive_warmup_time(uint32 wakeup_fn, gas_id_t gas_id)
{
  adaptive_warmup_tmln_struct_type *tmln_ptr             = &adaptive_warmup_tmln[gas_id];
  uint32                           required_warmup_us    = 0;
  boolean                          wcdma_reconf_possible = FALSE;
  boolean                          next_wakeup_ccch      = FALSE;
  
  required_warmup_us = tmln_ptr->current_warmup_time;

  /* If the next wakeup is a PCH, and a WCDMA reconfirmation is to be done then 
     additional RF scripts will be built during warmup requiring extra time. Add 
     this now to avoid an error recovery. 

     Minor issues to consider improving during adaptive timeline FR:
     (1) Warmup time is increased if next wakeup is any CCCH, but ideally this 
         should only occur for PCHs, since W reconfirmations do not occur with 
         BCCHs etc.
     (2) If a wakeup cancellation occurs, we should potentially consider 
         adjusting warmup duration. This would cover cases where we were waking
         for a PCH with a WCDMA reconfirmation which was cancelled and changed 
         to some other wakeup type not requiring additional warmup time. */
  wcdma_reconf_possible = l1_sc_is_reconf_next_pch_possible(gas_id);
  next_wakeup_ccch      = l1_fm_is_fn_ccch(wakeup_fn, gas_id);
  if (wcdma_reconf_possible &&
      next_wakeup_ccch) 
  {
    required_warmup_us += ADDITIONAL_G2W_SCRIPT_TIME_US;
  }

  MSG_GERAN_HIGH_4_G("TMLN: LMM=%d, wcdma_reconf_in_progress=%d, next_wakeup_ccch=%d, warm up time=%d",
                     gl1_hw_ncell_pch_low_monitor_active(gas_id), 
                     wcdma_reconf_possible,
                     next_wakeup_ccch,
                     required_warmup_us);

  return (required_warmup_us);
}

/*===========================================================================

FUNCTION gl1_hw_adaptive_is_sample_valid

DESCRIPTION
  GL1 runs ~99% of the time xCCH activity (takes longer) than other activities (NCELL, BPLMN etc.,)
  This means using any activity times other than xCCH skews the adaptive timeline calculations.
  Hence we should pick up the sample towards calculation only if not Rude wakeup, xCCH activity and
  only if RF wakeup Req has been done.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the sample to be saved is a valid sample or else FALSE

SIDE EFFECTS
  None

===========================================================================*/
static boolean gl1_hw_adaptive_is_sample_valid(gas_id_t gas_id)
{
  boolean ret_val = FALSE; // by default the sample is invalid

  /* Does this sample meets all the criteria such that we can use it towards avg calculation
   * error threshold not reached, only a xCCH activity, not a rude wake up and
   * only if a RF wake up req message is sent to RF.
   */
  if(gl1_check_if_current_activity_is_xcch(gas_id) &&
     !gl1_hw_sleep_get_forcedwakeup(gas_id) &&
      gl1_hw_rf_wakeup_req_msg_check(gas_id))
  {
    ret_val = TRUE;
    gl1_hw_set_rf_wakeup_req_msg(gas_id, FALSE); // Reset for the next cycle */
  }
  return ret_val;
}

/*===========================================================================

FUNCTION gl1_hw_adaptive_ccs_write_time_margin

DESCRIPTION
  Updates the margin we use on top of CCS write times, whilst making a decision
  to step up/down the warm up time.

DEPENDENCIES
  None.

RETURN VALUE
  ccs_write_time_margin

SIDE EFFECTS
  None

===========================================================================*/
static uint32 gl1_hw_adaptive_ccs_write_time_margin(gas_id_t gas_id)
{
  adaptive_warmup_tmln_struct_type *tmln_ptr = &adaptive_warmup_tmln[gas_id];
  uint32 ccs_write_time_margin = WARMUP_STEP_SIZE * 1.5; // default value

  /* When the very first error recovery is hit, we decrease the step size we deal
   * in. Hence the margin has to be accordingly updated as well. */
  if(tmln_ptr->step_size < WARMUP_STEP_SIZE)
  {
    ccs_write_time_margin = WARMUP_STEP_SIZE;
  }

  return ccs_write_time_margin;
}

/*===========================================================================

FUNCTION gl1_hw_adaptive_clear_valid_flag

DESCRIPTION
 Clears the valid flag in the adaptive timeline structure. If this flag is
 subsequently set, then this cycle should be used to adapt the wakeup timeline.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_adaptive_clear_valid_flag(gas_id_t gas_id)
{
  gl1_hw_adaptive_set_valid_flag(gas_id, FALSE, 0);
}

/*===========================================================================

FUNCTION gl1_hw_adaptive_set_valid_flag

DESCRIPTION
 Sets the valid flag in the adaptive timeline structure which indicates the
 current wake cycle is valid and can be used to calculate adjustments to the
 wakeup timeline. Also, saves RF's estimate of how long it will take the SSBIs
 to be written over the CCS bus.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
  void gl1_hw_adaptive_set_valid_flag(gas_id_t gas_id, boolean valid, uint32 CCS_write_time)
{
  adaptive_warmup_tmln_struct_type *tmln_ptr = &adaptive_warmup_tmln[gas_id];

  rex_enter_crit_sect(&gl1_hw_sleep_warmup_tmln_crit_sect[gas_id]);

  tmln_ptr->valid = valid;

  if (CCS_write_time > tmln_ptr->highest_CCS_Write_time_us)
  {
    tmln_ptr->highest_CCS_Write_time_us = CCS_write_time;
  }

  if(CCS_write_time > 1000)
  {
    MSG_GERAN_ERROR_2_G(GL1_TMLN_HDR" CCS_write_time=%dus from RF is unexpectedly high! highest_CCS_Write_time_us=%dus",
                        CCS_write_time,
                        tmln_ptr->highest_CCS_Write_time_us);
  }

  rex_leave_crit_sect(&gl1_hw_sleep_warmup_tmln_crit_sect[gas_id]);
}

/*===========================================================================

FUNCTION gl1_hw_adaptive_warmup_time

DESCRIPTION
  Calculate the sleep warmup time average, it updates based on
  ADAPTIVE_TMLN_SAMPLE_COUNT number of warmups.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_hw_adaptive_warmup_time(gas_id_t gas_id,
                                        int16 gap_to_ols_sample,
                                        boolean canCommit)
{
  adaptive_warmup_tmln_struct_type *tmln_ptr = &adaptive_warmup_tmln[gas_id];
  boolean is_sample_applicable = gl1_hw_adaptive_is_sample_valid(gas_id);

  gl1_hw_sleep_type   *gl1_hw_sleep_cycle = NULL;

   rex_enter_crit_sect(&gl1_hw_sleep_warmup_tmln_crit_sect[gas_id]);

#ifdef FEATURE_QTA
   if(gl1_hw_qta_gap_active(gas_id) || OTHER_GSM_CLIENT_IN_TRANSFER(gas_id))
   {
     gl1_hw_set_adaptive_warmup_time(gas_id, QTA_WARMUP_TIME);
     MSG_GERAN_HIGH_0_G(GL1_TMLN_HDR"This wakeup was for QTA or Other sub is in transfer");
   }
   /* Check it was a valid warm up timeline, this is based on RF awake is called */
   else
#endif
   if (tmln_ptr->valid == TRUE)
   {
      /* Sanity check the size of the returned gap */
      if (gap_to_ols_sample < MAX_GAP_2_STMR_TIME)
      {
          if(tmln_ptr->index == ADAPTIVE_TMLN_SAMPLE_COUNT)
          {
            /* Collected enough samples to average and consider adjusting warmup times */
            tmln_ptr->average_gap_to_ols >>= ADAPTIVE_TMLN_RIGHT_SHIFT_COUNT; // div by 8

            /* No error recovery, but avg gap is LESS than min gap needed for CCS events to finish */
            if ( tmln_ptr->average_gap_to_ols < (int32)tmln_ptr->highest_CCS_Write_time_us )
            {
              tmln_ptr->current_warmup_time += tmln_ptr->step_size;
            }
           /* No error recovery, and avg gap is MORE than min gap needed for CCS events to finish */
            else if (tmln_ptr->average_gap_to_ols  >
                    (int32)(tmln_ptr->highest_CCS_Write_time_us + gl1_hw_adaptive_ccs_write_time_margin(gas_id )))
            {
              tmln_ptr->current_warmup_time -= tmln_ptr->step_size;
            }

            gl1_hw_adaptive_range_check(gas_id);

            MSG_GERAN_HIGH_4_G(GL1_TMLN_HDR" average gap=%dus highest_CCS_Write_time_us=%d current_warmup=%dus step_size=%dus",
                tmln_ptr->average_gap_to_ols,
                tmln_ptr->highest_CCS_Write_time_us,
                tmln_ptr->current_warmup_time,
                tmln_ptr->step_size);

            gl1_hw_reset_adaptive_tmln_data_store(gas_id, FALSE);
            tmln_ptr->last_good_warmup_time = tmln_ptr->current_warmup_time;
          }

          /* Array index overran */
          else if (tmln_ptr->index > ADAPTIVE_TMLN_SAMPLE_COUNT)
            {
            /* Something may have gone wrong, reset and start again */
            MSG_GERAN_HIGH_1_G(GL1_TMLN_HDR" Reset adaptive warm up, index overflow %d", tmln_ptr->index);
            gl1_hw_reset_adaptive_tmln_data_store(gas_id, FALSE);
          }
          /* This is catch all including logging every interative index value*/
          else
          {
              if (!canCommit)
              {
                /* Error recovery kicked in. From now on we need to deal in smaller step sizes */
                tmln_ptr->error_cnt++;
                tmln_ptr->step_size = WARMUP_STEP_SIZE/2;
              }

              /* store and accummulate every sample where error count is less than threshold */
              if (is_sample_applicable && (tmln_ptr->error_cnt < ERROR_COUNT_THRESHOLD))
              {
                tmln_ptr->samples[tmln_ptr->index] = gap_to_ols_sample;
                tmln_ptr->average_gap_to_ols += tmln_ptr->samples[tmln_ptr->index];
                tmln_ptr->index++;

                /* Index gets printed from 1 upwards and not actual array index which would be 0 upwards */
                MSG_GERAN_HIGH_5_G(GL1_TMLN_HDR" index %d gap sample %d  accummulate total %d #error %d valid_sample %d",
                                   tmln_ptr->index,
                                   gap_to_ols_sample,
                                   tmln_ptr->average_gap_to_ols,
                                   tmln_ptr->error_cnt,
                                   is_sample_applicable);
              }
              /* Too many errors hit */
              else if(tmln_ptr->error_cnt >= ERROR_COUNT_THRESHOLD)
              {
                /* reset too many errors */
                MSG_GERAN_HIGH_3_G(GL1_TMLN_HDR" Reset adaptive warm up, too many errors %d: index %d gap sample %d",
                                   tmln_ptr->error_cnt,
                                   tmln_ptr->index,
                                   gap_to_ols_sample);

                gl1_hw_reset_adaptive_tmln_data_store(gas_id, FALSE);
                tmln_ptr->current_warmup_time += ADAPTIVE_ERR_RECOVERY_STEP_SIZE_US;

                gl1_hw_adaptive_range_check(gas_id);
              }
          }


          gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];
      gl1_hw_sleep_cycle->sleep_info.adaptive_warmup_us = tmln_ptr->current_warmup_time;
          gl1_hw_sleep_cycle->sleep_info.diag_f3_trace_disabled = !diag_f3_trace_enabled_check();
          gl1_hw_sleep_cycle->sleep_info.diag_mask_disabled = !diag_masks_enabled();
      }
      else
      {
        MSG_GERAN_ERROR_1_G(GL1_TMLN_HDR" warm up rather large,  gap to ols %d", gap_to_ols_sample);
      }
  }

  rex_leave_crit_sect(&gl1_hw_sleep_warmup_tmln_crit_sect[gas_id]);
}

/*===========================================================================

FUNCTION gl1_hw_adaptive_range_check

DESCRIPTION
  Check the warmup time is within allowed bounds.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_hw_adaptive_range_check(gas_id_t gas_id)
{
  adaptive_warmup_tmln_struct_type *tmln_ptr = &adaptive_warmup_tmln[gas_id];
   
  if (tmln_ptr->current_warmup_time < MIN_WARMUP_TIME)
  {
    tmln_ptr->current_warmup_time = MIN_WARMUP_TIME;
  }
  if (tmln_ptr->current_warmup_time > MAX_WARMUP_TIME)
  {
    /* For very large warmup times GFW receives cmds >1 frame in
       advance which causes a crash. This check avoids that.*/
    tmln_ptr->current_warmup_time = MAX_WARMUP_TIME;
  }
}
#endif /* FEATURE_GSM_ADAPTIVE_TMLN */

/*===========================================================================

FUNCTION gl1_hw_normal_warmup_time

DESCRIPTION
  Calculate the normal warmup time

DEPENDENCIES
  None.

RETURN VALUE
  normal tcxo and rf warmup time in mSec

SIDE EFFECTS
  None

===========================================================================*/
static uint32 gl1_hw_normal_warmup_time( gas_id_t gas_id )
{
  uint32 normal_warmup_time = ( GL1_HW_SLEEP_SCHEDULED_WARM_UP + gl1_hw_error_rec_warmup_adjust[gas_id] );
  uint32 normal_non_scheduled_warmup_time = GL1_HW_SLEEP_NONSCHEDULED_WARM_UP;
  gl1_hw_sleep_type   *gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];

  gl1_hw_sleep_cycle->sleep_info.returned_rf_warmup_usec = normal_warmup_time;
  gl1_hw_sleep_cycle->sleep_info.diag_f3_trace_disabled = !diag_f3_trace_enabled_check();
  gl1_hw_sleep_cycle->sleep_info.diag_mask_disabled = !diag_masks_enabled();

  if( gl1_hw_error_rec_warmup_adjust[gas_id] != 0 )
  {
      MSG_GERAN_ERROR_1_G(GL1_SLEEP_HDR" Non-optimal power saving mode is on; warm-up adjustment of %d usec applied",
                           gl1_hw_error_rec_warmup_adjust[gas_id]);
  }

  if (scheduled_timeline_adjustment[gas_id])
  {
    if (gl1_hw_ncell_pch_low_monitor_active( gas_id ))
    {
        normal_warmup_time -= GL1_HW_SLEEP_LMM_WARMUP_ADJUST;
        if (!diag_masks_enabled() && !diag_f3_trace_enabled_check())
        {
            normal_warmup_time -= GL1_HW_SLEEP_LMM_DIAG_MASK_ADJUST;
        }
      MSG_GERAN_HIGH_3_G(GL1_SLEEP_HDR" Optimised MCPM, LMM: Warmup normal %d, daig mask ? %d, diag_f3 ? %d",
                           normal_warmup_time, diag_masks_enabled(), diag_f3_trace_enabled_check());

        optimised_warmup_delta[gas_id] = normal_non_scheduled_warmup_time - normal_warmup_time;

      scheduled = TRUE;
      warmup = normal_warmup_time;
        return ( normal_warmup_time );
    }
    else
    {
        /* Field optimisation */
      normal_warmup_time -= GL1_HW_SLEEP_FIELD_WARMUP_ADJUST;

      MSG_GERAN_HIGH_1_G(GL1_SLEEP_HDR" Scheduled MCPM, !LMM: Warmup normal %d",
                         normal_warmup_time);
      optimised_warmup_delta[gas_id] = normal_non_scheduled_warmup_time - normal_warmup_time;

      scheduled = TRUE;
      warmup = normal_warmup_time;
      return ( normal_warmup_time );
    }
  }
  else
  {
      scheduled = FALSE;
      warmup = normal_non_scheduled_warmup_time;

    MSG_GERAN_HIGH_2_G(GL1_SLEEP_HDR" Non SRR: Warmup normal %d increased to %d usec",
                        normal_warmup_time, normal_non_scheduled_warmup_time);
    return( normal_non_scheduled_warmup_time );
  }
}

/*===========================================================================

FUNCTION gl1_hw_forced_warmup_time

DESCRIPTION
  Calculate the forced warmup time
DEPENDENCIES
  None.

RETURN VALUE
  forced tcxo and rf warmup time in mSec

SIDE EFFECTS
  None

===========================================================================*/
static uint32 gl1_hw_forced_warmup_time( gas_id_t gas_id )
{
    uint32 forced_warmup_time;

    /* determine whether this forced warmup is to service Compressed Mode */
    if (gl1_hw_sleep_cm_active[gas_id])
    {
        /* Reduce forced warm up to 3mS when W is in CM */
        forced_warmup_time = 3000;

        MSG_GERAN_HIGH_1_G( GL1_SLEEP_HDR" Forced Warmup in CM %d",
                            forced_warmup_time );
    }
    else
    {
        //forced_warmup_time = gl1_hw_get_rf_gsm_warmup_time(gas_id);
        forced_warmup_time = 15000;
    }
    return ( forced_warmup_time );
}


/*===========================================================================

FUNCTION gl1_hw_sleep_warmup_dur_in_sclks

DESCRIPTION
  Calculate the number of sclks with which we neeed to
  extend the TRM reservation time during wakeup manager cancellation
DEPENDENCIES
  None.

RETURN VALUE
  warmup duration in sclks

SIDE EFFECTS
  None

===========================================================================*/

int32 gl1_hw_sleep_warmup_dur_in_sclks(gas_id_t gas_id)
{
  int32 warmup_dur_in_sclks;

  warmup_dur_in_sclks = timetick_cvt_to_sclk(gl1_hw_sleep_warmup_time_us[gas_id], T_USEC);
  warmup_dur_in_sclks -= NUM_SCLKS_PER_GSM_FRAME;

  /* adjustment for wkup cancellation */
  if(gl1_hw_sleep_wmgr_cancel_in_prog[gas_id] && !gl1_hw_sleep_wmgr_trm_when_at_gts_valid[gas_id])
  {
    warmup_dur_in_sclks += NUM_SCLKS_PER_GSM_FRAME;
    MSG_GERAN_MED_1_G("WMGR_SLEEP: gts invalid, warmup_dur: %d", warmup_dur_in_sclks);
  }

  return warmup_dur_in_sclks;
}

/*===========================================================================

FUNCTION gl1_hw_symbols_to_delay_ols

DESCRIPTION
  Calculate the number of symbols to delay the online start
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
static uint32 gl1_hw_symbols_to_delay_ols( gas_id_t gas_id )
{
  uint32 ols_delay_time = 0;

  if ( scheduled_timeline_adjustment[gas_id]
       && gl1_hw_ncell_pch_low_monitor_active( gas_id )
       && gl1_msg_get_multi_sim_mode() )
  {
        ols_delay_time = OLS_DELAY_TIME;
        ols_delay_time = 0;
    }

#ifdef FEATURE_QTA
  if(IS_WCDMA_QTA_SUPPORTED())
  {
    MSG_GERAN_HIGH_1_G( "Clear OLS delay:%d during QTA",
                        ols_delay_time );

    ols_delay_time = 0;
  }
  else
#endif
  if ( ols_delay_time )
  {
    MSG_GERAN_HIGH_1_G( GL1_SLEEP_HDR"OLS %d",
                        ols_delay_time );
  }

  return ( ols_delay_time );
}

/*===========================================================================

FUNCTION gl1_hw_sleep_get_forcedwakeup

DESCRIPTION
  Returns if we are currently doing forced (rude/unscheduled) wakeup
DEPENDENCIES
  None.

RETURN VALUE
  TRUE if forced wakeup. FALSE if scheduled wakeup.

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_sleep_get_forcedwakeup(gas_id_t gas_id)
{
   return forced_wakeup[gas_id];
}

/*===========================================================================

FUNCTION gl1_hw_sleep_set_forcedwakeup

DESCRIPTION
  Sets the forced wake up state
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_hw_sleep_set_forcedwakeup(gas_id_t gas_id, boolean isForcedWakeup)
{
   forced_wakeup[gas_id] = isForcedWakeup;

   /* Whenever there is a RF init after a rude wake, the RF init can take longer up to ~1frame as it is in PWR DN state
      So, whenever forced wake up is set, then we should also indicate ourselves that RF may be in deep sleep state.
      On force wake up we set this flag, reset it immediately after first RF init post rude wake or when GTS or sleep init
   */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   gl1_hw_set_rf_in_deep_sleep_whilst_forced_wakeup(gas_id, isForcedWakeup);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION gl1_hw_set_rf_in_deep_sleep_whilst_forced_wakeup/gl1_hw_get_rf_in_deep_sleep_whilst_forced_wakeup

DESCRIPTION
  Set/Get functions for rf_in_deep_sleep_whilst_forced_wakeup

===========================================================================*/
void gl1_hw_set_rf_in_deep_sleep_whilst_forced_wakeup(gas_id_t gas_id, boolean flagVal)
{
   l1_tsk_buffer[gas_id].rf_in_deep_sleep_whilst_forced_wakeup = flagVal;
}

boolean gl1_hw_get_rf_in_deep_sleep_whilst_forced_wakeup(gas_id_t gas_id)
{
   return (l1_tsk_buffer[gas_id].rf_in_deep_sleep_whilst_forced_wakeup);
}
#endif  /* #if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */

/*===========================================================================

FUNCTION gl1_hw_sleep_set_warmup_cb

DESCRIPTION
  Sets the wakeup call back for the wake up event from SLPC
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_hw_sleep_set_warmup_cb(gas_id_t gas_id, boolean isDPC)
{
  slpc_id_type slpc_from_gas = gl1_hw_slpc_from_gas_id( gas_id );

  switch ( gas_id )
  {
    case GERAN_ACCESS_STRATUM_ID_1:
      if(!isDPC)
      {
        slpc_set_wakeup_callback( slpc_from_gas, gl1_hw_sleep_gsm_warmup_gas_1_cb );
      }
      else
      {
        slpc_set_wakeup_callback( slpc_from_gas, gl1_hw_sleep_gsm_warmup_gas_1_cb_dpc );
      }
     break;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     case GERAN_ACCESS_STRATUM_ID_2:
       if(!isDPC)
       {
         slpc_set_wakeup_callback( slpc_from_gas, gl1_hw_sleep_gsm_warmup_gas_2_cb );
       }
       else
       {
         slpc_set_wakeup_callback( slpc_from_gas, gl1_hw_sleep_gsm_warmup_gas_2_cb_dpc );
       }
     break;
#endif

#if defined ( FEATURE_TRIPLE_SIM )
     case GERAN_ACCESS_STRATUM_ID_3:
       if(!isDPC)
       {
         slpc_set_wakeup_callback( slpc_from_gas, gl1_hw_sleep_gsm_warmup_gas_3_cb );
       }
       else
       {
         slpc_set_wakeup_callback( slpc_from_gas, gl1_hw_sleep_gsm_warmup_gas_3_cb_dpc );
       }
     break;
#endif /* FEATURE_TRIPLE_SIM */

     default:
       MSG_GERAN_FATAL_0_G( "Invalid gas_id" );
     break;
  }

}

/*===========================================================================

FUNCTION GL1_HW_SLEEP_GSM_INIT

DESCRIPTION
  Initialize the sleep controller drivers.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_gsm_init( gas_id_t gas_id )
{
   DALResult dal_result;
   mcpm_tech_type mcpm_tech;
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = NULL;

   gas_id = check_gas_id(gas_id);

   l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   mcpm_tech = gl1_hw_map_gas_id_to_mcpm_tech( gas_id );

   gl1_hw_sleep_state[gas_id]                  = GL1_HW_SLEEP_NULL;

   gl1_hw_sleep_set_in_warmup_check(FALSE, gas_id);

   sleep_start_cb_ptr[gas_id]                  = NULL;
   sleep_warmup_cb_ptr[gas_id]                 = NULL;
   gl1_hw_sleep_clear_missed_frames(gas_id);
#ifdef FEATURE_GSM_ADAPTIVE_TMLN
   gl1_hw_reset_adaptive_tmln_data_store(gas_id, TRUE);
#endif
   gl1_hw_consecutive_recovery_counter[gas_id] = 0;

   gl1_hw_save_ccs_write_time_estimate(0, gas_id);
   gl1_hw_reset_time_enter_mode_completed(gas_id);

   gl1_hw_sleep_set_rf_on(FALSE, gas_id);
   gl1_hw_sleep_set_rf_cnf_pending(FALSE, gas_id);

   l1_tskisr_blk->l1_isr_called_from_sleep_warmup_isr = FALSE;
   l1_tskisr_blk->rf_wakeup_pending                   = FALSE;

   gl1_hw_sleep_set_forcedwakeup(gas_id, FALSE);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   gl1_hw_sleep_set_qta_active(gas_id, FALSE);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

   /* Register callback when coming out of power collapse */
   switch ( gas_id )
   {
      case GERAN_ACCESS_STRATUM_ID_1:
       MCPM_NPA_Set_Sleep_Timeline_Callback( mcpm_tech, gl1_hw_mcpm_slp_tmln_gas_1_cb );
     break;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     case GERAN_ACCESS_STRATUM_ID_2:
       MCPM_NPA_Set_Sleep_Timeline_Callback( mcpm_tech, gl1_hw_mcpm_slp_tmln_gas_2_cb );
      break;

#if defined ( FEATURE_TRIPLE_SIM )
     case GERAN_ACCESS_STRATUM_ID_3:
       MCPM_NPA_Set_Sleep_Timeline_Callback( mcpm_tech, gl1_hw_mcpm_slp_tmln_gas_3_cb );
      break;
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

     default:
     break;
   }

  /* Ensure DAL is initialised */
  DALSYS_InitMod(NULL);

  /* Get interrupt controller driver handle */
  dal_result = DAL_DeviceAttach(DALDEVICEID_INTERRUPTCONTROLLER,
                                &gsmSleep_hInterruptHandle[gas_id]);
  ASSERT( dal_result == DAL_SUCCESS );

#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )

#ifdef FEATURE_SLPC_TRIPLE_SIM
   maintain_wcdma_ref_count[gas_id] = FALSE;
   maintain_tds_ref_count[gas_id]   = FALSE;
#endif /* FEATURE_SLPC_TRIPLE_SIM */
#endif /* IRAT */

   early_gts_offset[gas_id] = 0;
   late_ols_offset[gas_id]  = 0;


   MSG_GERAN_MED_1_G(GL1_SLEEP_HDR" GSM state SLEEP_NULL %d",
                     timetick_get_safe());
}

/*===========================================================================

FUNCTION gl1_hw_sleep_register_npa_cb

DESCRIPTION
 Registering the callback with NPA
DEPENDENCIES
 None.

RETURN VALUE
 None

SIDE EFFECTS
None

===========================================================================*/
void gl1_hw_sleep_register_npa_cb(gas_id_t gas_id)
{
   if( gl1_hw_sleep_disable[gas_id] == TRUE )
   {
       if ( geran_latency_node_created[gas_id] == FALSE )
       {
         switch ( gas_id )
         {
           case GERAN_ACCESS_STRATUM_ID_1:
             npa_resource_available_cb( gl1_hw_latency_node_nm[gas_id],
                                        gl1_hw_core_cpu_latency_avail_gas_1_cb, NULL );
           break;

           case GERAN_ACCESS_STRATUM_ID_2:
             npa_resource_available_cb( gl1_hw_latency_node_nm[gas_id],
                                        gl1_hw_core_cpu_latency_avail_gas_2_cb, NULL );
           break;

#if defined ( FEATURE_TRIPLE_SIM )
           case GERAN_ACCESS_STRATUM_ID_3:
             npa_resource_available_cb( gl1_hw_latency_node_nm[gas_id],
                                        gl1_hw_core_cpu_latency_avail_gas_3_cb, NULL );
           break;
#endif /* FEATURE_TRIPLE_SIM */

           default:
           break;
         }

         gl1_hw_latency_status[gas_id].gl1_hw_latency_sclk = 1;
         gl1_hw_latency_status[gas_id].gl1_hw_latency_sent = FALSE;
         geran_latency_node_created[gas_id] = TRUE;
       }
   }
}
/*===========================================================================

FUNCTION GL1_HW_SLEEP_GSM_SHUTDOWN

DESCRIPTION
  Prepares the sleep controller drivers for a phone power down or a
  mode change.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_gsm_shutdown( gas_id_t gas_id )
{
}

/*===========================================================================

FUNCTION    GL1_HW_SLEEP_GSM_ENTER_IDLE

DESCRIPTION
  Prepares the physical layer hardware for being in Idle Mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Disables Tx chain.

===========================================================================*/
void gl1_hw_sleep_gsm_enter_idle( gas_id_t gas_id )
{
   slpc_id_type slpc_from_gas = GL1_SLPC_GSM_0;
   gas_id = check_gas_id(gas_id);

   slpc_from_gas = gl1_hw_slpc_from_gas_id( gas_id );

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_sleep_gsm_enter_idle: Return Immediately");

   return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   GDRV_ISR_LOCK(gas_id);
#ifdef FEATURE_GSM_ADAPTIVE_TMLN
   gl1_hw_reset_adaptive_tmln_data_store(gas_id, FALSE);
#endif
   gl1_hw_sleep_state[gas_id]                  = GL1_HW_SLEEP_AWAKE;
   total_error_recovery_frames[gas_id]         = 0;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   gl1_hw_set_rf_in_deep_sleep_whilst_forced_wakeup(gas_id, FALSE);
#endif
   gl1_hw_consecutive_recovery_counter[gas_id] = 0;
   gl1_hw_sleep_set_in_warmup_check(FALSE, gas_id);
   gl1_hw_sleep_set_rf_cnf_pending(FALSE, gas_id);

   switch ( gas_id )
   {
     case GERAN_ACCESS_STRATUM_ID_1:
       slpc_set_wakeup_callback( slpc_from_gas, gl1_hw_sleep_gsm_warmup_gas_1_cb );
     break;
#if defined ( FEATURE_DUAL_SIM )
     case GERAN_ACCESS_STRATUM_ID_2:
       slpc_set_wakeup_callback( slpc_from_gas, gl1_hw_sleep_gsm_warmup_gas_2_cb );
     break;
#endif
#if defined ( FEATURE_TRIPLE_SIM )
     case GERAN_ACCESS_STRATUM_ID_3:
       slpc_set_wakeup_callback( slpc_from_gas, gl1_hw_sleep_gsm_warmup_gas_3_cb );
     break;
#endif /* FEATURE_TRIPLE_SIM */

     default:
     break;
   }
   GDRV_ISR_UNLOCK(gas_id);

   MSG_GERAN_MED_3_G( GL1_SLEEP_HDR"Enter Idle GSM state:%d SLPC:%d @ %d",
                      gl1_hw_sleep_state[gas_id], slpc_from_gas, timetick_get_safe() );
}

/*===========================================================================

FUNCTION    GL1_HW_SLEEP_GSM_LEAVE_IDLE

DESCRIPTION
  Prepares the physical layer hardware to leave Idle Mode and
  enter Dedicated Mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Re-enables Tx chain.

===========================================================================*/
void gl1_hw_sleep_gsm_leave_idle( gas_id_t gas_id )
{
   slpc_id_type slpc_from_gas = gl1_hw_slpc_from_gas_id( gas_id );
   idle_data_T      *l1_idle_data_ptr = &l1_idle_data_store[gas_id];
   GDRV_ISR_LOCK(gas_id);
   l1_idle_data_ptr->idle_power_opt = 0;
   gl1_hw_error_rec_warmup_adjust[gas_id] = 0;
   gl1_hw_consecutive_recovery_counter[gas_id] = 0;
   total_error_recovery_frames[gas_id]  = 0;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   gl1_hw_set_rf_in_deep_sleep_whilst_forced_wakeup(gas_id, FALSE);
#endif
   gl1_hw_sleep_set_entered_recovery(gas_id, FALSE);

   gl1_hw_sleep_state[gas_id]       = GL1_HW_SLEEP_NULL;

   GDRV_ISR_UNLOCK(gas_id);

   MSG_GERAN_MED_3_G( GL1_SLEEP_HDR" Leave Idle GSM state:%d SLPC:%d @ %d",
                      gl1_hw_sleep_state[gas_id], slpc_from_gas, timetick_get_safe() );

}

/*===========================================================================

FUNCTION GL1_HW_SLEEP_GSM_START_SLEEP

DESCRIPTION
  Starts the physical layer sleep process.
  The sleep controller time line is started.
  Note that GL1_ISR_SAVE_LOCK() is already done by caller of this function.

DEPENDENCIES
  gl1_hw_sleep_set_terminal_count must be called when the sleep period ends.

  This function should be called with interrupts locked.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_sleep_gsm_start_sleep( uint32  frames_to_sleep,
                                      uint32  frames_awake,
                                      boolean check_too_early,
                                      uint32  wakeup_fn,
                                      gas_id_t gas_id )
{
   uint32  gstmr_count;
   uint32  gstmr_count_at_gts;
   boolean gstmr_int;
   boolean far_from_end_of_frame;
   ustmr_type  awake_duration_us;
   /* Function return value, default value is TRUE */
   boolean ret_val = TRUE;
   boolean mdsp_woke_up;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   uint32 gstrm_count_remaining_qs;
   uint32 start_sclks;
   uint32 end_sclks;
   uint32 current_sclks;
#endif
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

   slpc_err_type slpc_err;
   vstmr_gstmr_t time;
   slpc_tstmr_type slpc_start_time;


   /* return the id of GSM SLPC */
   slpc_id_type slpc_from_gas = gl1_hw_slpc_from_gas_id( gas_id );

   boolean forced_mdsp_to_sleep = FALSE;

   vstmr_geran_ext_frame_u  gstmr_ext_frame_var;
   int      as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

   do
   {

  gl1_hw_sleep_type  *gl1_hw_sleep_cycle = &(gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]]);

  gl1_hw_sleep_cycle->start.wakeup_frame_number = wakeup_fn;
  gl1_hw_sleep_cycle->start.go_to_sleep_frame_number = gl1_get_FN(gas_id);
  gl1_hw_sleep_cycle->start.go_to_sleep.start_ustmr = slpc_get_tick();
#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
  QDSS_logging( gas_id, GERAN_SLEEP_GOING_TO_SLEEP_1);
#endif

  MSG_GERAN_HIGH_4_G(GL1_SLEEP_HDR" GSM start sleep FTS %d FA %d check %d wakeup_fn %d",
                     frames_to_sleep,frames_awake,check_too_early, wakeup_fn);


  /* conditional check to see if we are completely awake before we start sleep */
  if ( !gl1_hw_wakeup_complete( gas_id ) )
  {
       MSG_GERAN_ERROR_1_G(GL1_SLEEP_HDR" GSM wake-up still not complete g %d",
                           gl1_hw_sleep_state[gas_id]);
       ret_val = FALSE;
       /* Exit do-while loop here */
       break;
   }

   late_ols_offset[gas_id] = gl1_hw_symbols_to_delay_ols( gas_id );
   total_error_recovery_frames[gas_id] = 0;
   /* A number of conditions must be met before we
    * can start the sleep timeline. Below each
    * condition is checked in turn. If a condition
    * is not met then this function immediately
    * returns FALSE.
    */

   /* Check current NPL1 state is OK for sleeping */
   if ( ( frames_awake == 0 ) ||
        !gl1_hw_ok_to_sleep( gas_id ) )
   {
      ret_val = FALSE;
      /* Exit do-while loop here */
      break;
   }


   /* This is time-critical processing, so disallow other tasks/interrupts
    ** to pre-empt GL1 starting here
    */
   REX_DISABLE_PREMPTION();

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* Check current position in frame */
   start_sclks = timetick_get_safe();
#endif
   /* Read gstmr_count as QS to avoid loss of precision */
   gstmr_count = gstmr_rd_qsymbol_count_geran(as_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* From remaining gstmr count, calcute the frame boundary end */
   gstrm_count_remaining_qs = QS_PER_FRAME - gstmr_count;
#endif

   /* now convert gstmr_count from QS to sym */
   gstmr_count = gstmr_count >> 2;
   gstmr_int   = gstmr_is_int_pending( gas_id );

   if ( ( !gstmr_int ) &&
        ( gstmr_count > MIN_SYMBOL_COUNT_TO_SLEEP ) &&
        ( gstmr_count < gl1_hw_max_symbol_count_to_sleep( gas_id ) )  )
   {
     /* success case , can proceed with sleep as we are > 1.5ms from end of frame */
     far_from_end_of_frame = TRUE;

     /* Start SLPC here so that we have correct frame starting point.
      * Otherwise, if SLPC is started when FN has rolledover, we will
      * get incorrect start time which will be into next frame.
      * (slpc_start() returns time and phase starting point of t_stmr)
      */
     slpc_start_time = slpc_start( slpc_from_gas );
   }
   else
   {
     /* failure case, cannot proceed with sleep as too near to end of frame.
      * handle failure case later as we don't want to keep preemption disabled for long time.
      */
     far_from_end_of_frame = FALSE;
   }

   /* Allow other tasks/interrupts to pre-empt GL1 starting here */
   REX_ENABLE_PREMPTION();

   MSG_GERAN_HIGH_4_G(GL1_SLEEP_HDR" Start Sleep MS_Mode %d gstmr:%dqs Threshold:%dqs FarFrmEof: %d",
                        gl1_msg_get_multi_sim_mode(), (gstmr_count<<2), (gl1_hw_max_symbol_count_to_sleep( gas_id )<<2),
                        far_from_end_of_frame);

   /* handle failure case if we are near to end of frame */
   if(!far_from_end_of_frame)
   {
      /* cannot proceed with sleep as we are very near to end of frame, so FN may rollover */

      MSG_GERAN_MED_3_G(GL1_SLEEP_HDR" Start Fail pend %d gcount %dqs time %d",
                        gstmr_int,gstmr_count,timetick_get_safe());

      /* We've attempted a GTS but failed. But if next ISR is to be ignored, then that should be cleared here. */
      if(gl1_drx_get_asynch_int_ignore_gstmr(gas_id))
      {
        gl1_drx_set_asynch_int_ignore_gstmr( FALSE, gas_id );
      }
      ret_val = FALSE;
      /* Exit do-while loop here */
      break;
   }

   /* Check mDSP is ready to sleep */
   if ( !gl1_hw_mdsp_sleep( &forced_mdsp_to_sleep, gas_id ) )
   {
      MSG_GERAN_MED_0_G(GL1_SLEEP_HDR" MDSP could not sleep");

      /* abort SLPC as it was started earlier */
      slpc_abort( slpc_from_gas );

      ret_val = FALSE;
      /* Exit do-while loop here */
      break;
   }

   /* While putting FW to suspend state, Fws_suspend() is taking 3msec as LTE
   * is enabling the FW app at the same time(LTE is holding the fws_mutex)
   * adding the checks after putting FW to sleep check for gstmr int pending &
   * frame boundary margin (added this after discussing with Ian!!)
   *Boundary check only for multisim mode and not needed for single sim mode
   */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   if(gl1_msg_get_multi_sim_mode())
   {
     /*frame boundary checks after putting mdsp to sleep*/
     gstmr_int   = gstmr_is_int_pending( gas_id );
     current_sclks = timetick_get_safe();

     /* from remaining gstmr count, calcute the frame boundary end */
     end_sclks  = start_sclks + timetick_cvt_to_sclk(GL1_CONVERT_QS_TO_US(gstrm_count_remaining_qs),T_USEC) - MIN_FRAME_BOUNDARY_MARGIN_SCLKS;


     if( gstmr_int || (((int32) (current_sclks-end_sclks)) >= 0) )
     {
       /* We've attempted a GTS but failed. But if next ISR is to be ignored, then that should be cleared here. */
       if(gl1_drx_get_asynch_int_ignore_gstmr(gas_id))
       {
         gl1_drx_set_asynch_int_ignore_gstmr( FALSE, gas_id );
       }

       /* abort SLPC as it was started earlier */
       slpc_abort( slpc_from_gas );
       ret_val = FALSE;

       /* MDSP is already put to sleep so wakeup mdsp, near to frame boundary (or) gstmr int is pending */
       mdsp_woke_up = mdsp_wakeup_geran(FALSE, as_id);

       MSG_GERAN_HIGH_4_G("SLEEP: Start Fail after mdsp sleep pend %d, mdsp_woke_up %d, current_sclks %d, end_sclks %d",
             gstmr_int, mdsp_woke_up, current_sclks, end_sclks );
      
       /* Exit do-while loop here */
       break;
     }
   }
#endif/*FEATURE_DUAL_SIM */

   /* --------------------------------------------------------------------------------------- */
   /* POINT OF NO RETURN: Now, we cannot stop going to sleep as sleep cmd to fw has been sent */
   /* --------------------------------------------------------------------------------------- */

   gl1_hw_sleep_state[gas_id] = GL1_HW_SLEEP_GOING_TO_SLEEP;

   gl1_hw_sleep_set_forcedwakeup(gas_id, FALSE);

#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
   MSG_GERAN_MED_2_G(GL1_SLEEP_HDR" GSM state SLEEP_GOING_TO_SLEEP gl1hw %d, time %d",
                     gl1_hw_sleep_state[gas_id],timetick_get_safe());
#endif

#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
   /*Inform MCPM about critical scenario OFF now if needed*/
   if(critical_scn_updated_to_mcpm[gas_id] == TRUE)
   {
     gl1_clkdata_speed(FALSE, L1_TIME_LINE_ERROR, 0,L1_TIME_LINE_DELAY, gas_id);
     critical_scn_updated_to_mcpm[gas_id] = FALSE;
   }
#endif

   /* For HW based sleep controller ( pre DIME ), ARM the device, for RPM sleep controller,
      request sleep controller to start sleep process  */
#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
   if ( maintain_wcdma_ref_count[gas_id] )
   {
       /* return the id of WCDMA SLPC */
      slpc_id_type wcdma_slpc_from_gas = gl1_hw_wcdma_slpc_from_gas_id( gas_id );

      w_stmr_slam_value.time = 0;

       MSG_GERAN_HIGH_2_G(GL1_SLEEP_HDR" Enable alt SLPC G %d W %d", slpc_from_gas, wcdma_slpc_from_gas );
       slpc_enable_alt_id( slpc_from_gas, wcdma_slpc_from_gas, &w_stmr_slam_value );
   }
#endif

#ifdef FEATURE_SLPC_TRIPLE_SIM
   if ( maintain_tds_ref_count[gas_id] )
   {
       t_stmr_slam_value.time = 0;
       slpc_enable_alt_id( SLPC_GSM, SLPC_TDSCDMA, &t_stmr_slam_value );
   }
#endif /* FEATURE_SLPC_TRIPLE_SIM */

   gsm_slpc[gas_id].gsm_slpc_start_t_stmr = slpc_start_time;

   time.all = gsm_slpc[gas_id].gsm_slpc_start_t_stmr.time;

   G_commit[gas_id] = FALSE;

   /* Turn off the mDSP clock */
   /* Clear any pending firmware requests as about to sleep and maybe wakeup on other sub */
   gl1_hw_sleep_reset_mdsp_intf( gas_id );

   mdsp_go_to_sleep( gas_id );

   if(!gstmr_get_asynch_int_executing(gas_id))
   {
       gl1_ustmr_gstmr_snapshot_before_sleep(gas_id);
   }

   /* ignore any ISR that follows when the ISR mutex is released */
   gl1_drx_set_asynch_int_ignore_gstmr( TRUE, gas_id );

   /* Deadlock will happen if gstmr_start_sleep() is called with GL1_ISR_SAVE_LOCK()
    * as gstmr_start_sleep() waits on isr_sync_flag which is like mutex for vstmr.
    * Also note that ISR was already locked by the caller of this function.
    *
    *  gstmr_start_sleep()      sequence:
    *     GL1_ISR_SAVE_UNLOCK()
    *     wait till isr_sync_flag = 1 <- Wait
    *  vstmr_geran_event_isr()  sequence:
    *     isr_sync_flag = 0
    *     GL1_ISR_SAVE_LOCK()         <- Wait
    */
   GL1_ISR_SAVE_UNLOCK(gas_id);

   gstmr_count_at_gts = gstmr_start_sleep( 0, gas_id );

   GL1_ISR_SAVE_LOCK(gas_id);

   gl1_drx_set_asynch_int_ignore_gstmr( FALSE, gas_id );

   MSG_GERAN_HIGH_4_G( GL1_SLEEP_HDR" VSTMR OFF @ GTS VSTMR FN %d, OLS VSTMR FN %d, count_at_GTS=%d, slpc start time [15,0] %d",
                        gl1_hw_frame_number_gts[gas_id],
                        gl1_hw_frame_number_ols[gas_id],
                        gstmr_count_at_gts,
                        gsm_slpc[gas_id].gsm_slpc_start_t_stmr.time);

   gstmr_ext_frame_var.value.value = atomic64_read((&(gtmrs_data[as_idx].geran_view->ext_frame_data.value)));

   if (gstmr_ext_frame_var.ext_frame_tc > (NUM_GSM_CLKS_PER_FRAME + DEF_QSYMB_TIME_TRACKING_ADJ*G_STMR_QSYM_TO_RAW_13MHZ) )
   {
      early_gts_offset[gas_id] = ( gstmr_ext_frame_var.ext_frame_tc - time.raw_13M );
   }
   else
   {
      early_gts_offset[gas_id] = ( NUM_GSM_CLKS_PER_FRAME - 1 - time.raw_13M );
   }

   /* increase frame count as we run first isr in warm up interrupt */
   frames_to_sleep++;
   gl1_hw_frame_number_gts[gas_id] = time.frame_num;
   gl1_hw_frame_number_mdsp_wakeup[gas_id] = (gl1_hw_frame_number_gts[gas_id] + frames_to_sleep)&(GERAN_VSTMR_MAX_FN - 1);
   gl1_hw_frame_number_ols[gas_id] = (gl1_hw_frame_number_mdsp_wakeup[gas_id] + 1)&(GERAN_VSTMR_MAX_FN - 1);
   sleep_duration_gsm_clks[gas_id] = (NUM_GSM_CLKS_PER_FRAME * frames_to_sleep)
                                   + (NUM_GSM_CLKS_PER_SYMB * late_ols_offset[gas_id])
                                   + (early_gts_offset[gas_id]);
   sleep_duration_frames[gas_id] = frames_to_sleep;

/* Set the warm up and duration for G sleep */
#if defined (FEATURE_GSM_ADAPTIVE_TMLN)
   if(gl1_is_adaptive_timeline_disabled(gas_id))
   {
     gl1_hw_sleep_warmup_time_us[gas_id] = gl1_hw_normal_warmup_time( gas_id );
   }
   else
   {
   gl1_hw_sleep_warmup_time_us[gas_id] = gl1_hw_get_adaptive_warmup_time(wakeup_fn, gas_id );
   }
#else
   gl1_hw_sleep_warmup_time_us[gas_id] = gl1_hw_normal_warmup_time( gas_id );

#endif

   slpc_err = slpc_set_warmup( slpc_from_gas, gl1_hw_sleep_warmup_time_us[gas_id] );
   if (slpc_err != SLPC_ERR_NO_ERR)
   {
#ifdef FEATURE_QSH_MDUMP
     QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
     ERR_GERAN_FATAL_1_G( GL1_SLEEP_HDR" SLPC ERROR return for slpc_set_warmup %d",
                 slpc_err);
   }

   slpc_err = slpc_set_duration( slpc_from_gas, sleep_duration_gsm_clks[gas_id] );
   if (slpc_err != SLPC_ERR_NO_ERR)
   {
#ifdef FEATURE_QSH_MDUMP
     QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_1_G( GL1_SLEEP_HDR" SLPC ERROR return for slpc_set_duration %d",
                 slpc_err);
   }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   gl1_hw_wkupmgr_wakeup_point_fn[gas_id] = l1_fm_get_trm_reserved_fn(gas_id);
#else
   gl1_hw_wkupmgr_wakeup_point_fn[gas_id] = NOT_A_FRAME_NUMBER;
#endif

   gl1_hw_sleep_cycle->sleep_info.gsm_clk_sleep_duration = sleep_duration_gsm_clks[gas_id];
   gl1_hw_sleep_cycle->sleep_info.gsm_clk_sleep_duration_frames  = frames_to_sleep;
   gl1_hw_sleep_cycle->wakeup.wakeup_delay.programmed_to_run = slpc_get_wakeup_tick( slpc_from_gas );
   gl1_hw_sleep_cycle->start.rf_rx_off.start_ustmr = slpc_get_tick();
#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
   QDSS_logging(gas_id,GERAN_SLEEP_RF_RX_OFF_START_1);
#endif

   if (gl1_msg_get_multi_sim_mode() == FALSE)
   {
       gl1_hw_turn_rx_off( gas_id );
   }

#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
      QDSS_logging(gas_id,GERAN_SLEEP_RF_RX_OFF_END_1);
#endif
      gl1_hw_sleep_debug_duration ( &(gl1_hw_sleep_cycle->start.rf_rx_off) );

   if ( forced_mdsp_to_sleep )
   {
      MSG_GERAN_ERROR_1_G(GL1_SLEEP_HDR" mDSP forced to sleep %d",
                 forced_sleep_count_mdsp[gas_id]);
   }

   if ( mdsp_sleep_error_pending( gas_id ) )
   {
      mdsp_display_sleep_error( TRUE, gas_id );
   }

#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
   QDSS_logging(gas_id,GERAN_SLEEP_RF_VREG_OFF_START_1);
   QDSS_logging(gas_id,GERAN_SLEEP_RF_VREG_OFF_END_1);
#endif


      /*
       * We reduce the wakeup FN by the offset on a asynch PCH as the current FN
       * has already been incremented when we perform the asynch interrupt handling
       */
      gl1_drx_update_counters( (int32)((frames_to_sleep -1) - gl1_drx_get_pch_offset(gas_id) ), TRUE, gas_id );   /* defer to sleep cb */

#ifdef FEATURE_WLAN_COEX_SW_CXM
      /* pass sleep duration in ms (rounded down)*/
      gl1_drx_garb_send_sleep_duration(TRUE, sleep_duration_gsm_clks[gas_id]/(QSYM_SCLK_SCALING_DENOM*1000), gas_id);
#endif


#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      if ( gl1_msg_get_multi_sim_mode() )
      {
        int32 warmup_dur_in_sclks;
        
        warmup_dur_in_sclks = gl1_hw_sleep_warmup_dur_in_sclks(gas_id);

        if( warmup_dur_in_sclks )
        {
#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
          gl1_hw_sleep_wmgr_els_warmup_dur_in_sclk[gas_id] = warmup_dur_in_sclks;
#endif 
          /* Adding sleep warmup time into TRM booking.
           * Remove one frame from the booking as one gstmr_isr will be run during warmup.
           * Also, store current grm/trm reservation.
           */
          grm_reserve_at_gts( warmup_dur_in_sclks, wakeup_fn, gas_id);
        }
        else
        {
          /* Store current grm/trm reservation */
          grm_info_t *grm_info_p;
          grm_info_p = grm_get_cur_reservation(wakeup_fn, gas_id);
          grm_store_cur_reservation(grm_info_p, gas_id);
        }
      }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
      MSG_GERAN_MED_3_G(GL1_SLEEP_HDR" GSM frames_to_sleep %d late_ols_offset[gas_id] %d early_gts_offset[gas_id] %d",
            frames_to_sleep, late_ols_offset[gas_id], early_gts_offset[gas_id] );
      MSG_GERAN_HIGH_2_G( GL1_SLEEP_HDR" GSM START cb mcpm DPC call=%dus rf sleep=%dus",
                          gl1_hw_sleep_cycle->start.mcpm.duration_us,
                          gl1_hw_sleep_cycle->start.rf_rx_off.duration_us);
#endif

      /* Acquire Mutex
       * Wait for wkup cancel to end before setting state to ASLEEP. We need to protect
       * in and out of GL1_HW_SLEEP_ASLEEP transition
       */
      MSG_GERAN_MED_0_G( "WMGR_SLEEP: ML start_sleep");
      gl1_hw_sleep_wmgr_mutex_lock(gas_id);
      
      gl1_hw_sleep_state[gas_id] = GL1_HW_SLEEP_ASLEEP;
      
      /* Relinquish Mutex */
      MSG_GERAN_MED_0_G( "WMGR_SLEEP: MU start_sleep");
      gl1_hw_sleep_wmgr_mutex_unlock(gas_id);
      
      /* Dime, update DRX manager here */
      if ( sleep_start_cb_ptr[gas_id] )
      {
          (*sleep_start_cb_ptr[gas_id])( gas_id );
      }

   gl1_hw_sleep_cycle->sleep_info.egts_offset_gsm_clks = early_gts_offset[gas_id];
#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
   QDSS_logging(gas_id,GERAN_SLEEP_CONFIG_DONE_1); /*frames to sleep..???*/
#endif
   gl1_hw_sleep_debug_duration ( &(gl1_hw_sleep_cycle->start.go_to_sleep) );
   gl1_hw_sleep_cycle->sleep_info.late_ols_offset_usec = ( late_ols_offset[gas_id] * 3.69 );
   awake_duration_us = (((gl1_hw_sleep_cycle->start.go_to_sleep.end_ustmr - gl1_hw_sleep_cycle->wakeup.mcpm.end_ustmr) * 107) >> 11);
   gl1_hw_sleep_cycle->sleep_info.awake_duration_us = awake_duration_us;

   MSG_GERAN_HIGH_2_G( GL1_SLEEP_HDR" GSM START Sleep duration = %dus Total awake timeline exc MCPM %dus ",
                       gl1_hw_sleep_cycle->start.go_to_sleep.duration_us, awake_duration_us );

  } while(0);

  #ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  /* For WLAN and Antenna sharing configuration, If SAIC/AEQ is inactive during
     sleep callback then send TRM release for Drx client to disable IDLE RxD.
     Checking 'ret_val' to ensure that we are indeed going to sleep. ret_val is TRUE for awake cases */
  MSG_GERAN_HIGH_4_G(GL1_RXD"ANT_SHARING: Enter sleep check to release rxd for IDLE RxD."
    "(Saic, MSIM, forced Ret, IdleRxd NV) = (%d, %d, %d, %d)", gl1_hw_get_saic_state(gas_id),
    gl1_msg_get_multi_sim_mode(), ret_val, gl1_get_IMRD_ctl_nv(gas_id));

  if ((gl1_msg_get_multi_sim_mode() == FALSE)
       && (ret_val != FALSE)
       && (gl1_hw_get_saic_state(gas_id) == FALSE)
       && (gl1_get_IMRD_ctl_nv(gas_id) == TRUE)
       && (gl1_get_rxd_ctl_nv(gas_id) == TRUE)
      )
  {
    volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    grm_client_enum_t divrx_client_id = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);

    if (grm_get_trm_status(divrx_client_id, gas_id) == GL1_TRM_GRANTED)
    {
      /* grm_release_rx_sec releases TRM for rxdiv client when the trm status is one of the following:
         1) GL1_TRM_GRANTED, 2) GL1_TRM_CALLBACK_PENDING, or 3) GL1_TRM_RETAINED_FOR_ACCESS
      */
      grm_release_rx_sec(divrx_client_id, gas_id);
      MSG_GERAN_HIGH_0_G(GL1_RXD"ANT_SHARING: Release TRM lock at sleep start and when rxd inactive");
    }
    else
    {
      MSG_GERAN_HIGH_2_G(GL1_RXD"ANT_SHARING: Entering sleep with DivRx client (%d) TRM Status %d",
        divrx_client_id, grm_get_trm_status(divrx_client_id, gas_id));
    }
  }
  #endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */

  return ret_val;
}

/*===========================================================================

FUNCTION gl1_hw_sleep_start_from_drx

DESCRIPTION
  Starts the physical layer sleep process.
  The sleep controller time line is started.

DEPENDENCIES
  gl1_hw_sleep_set_terminal_count must be called when the sleep period ends.

  This function should be called with interrupts locked.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_start_from_drx( gas_id_t gas_id )
{

  gl1_hw_sleep_type  *gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];

   gl1_hw_sleep_cycle->start.mcpm.start_ustmr = slpc_get_tick();
#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
   QDSS_logging( gas_id, GERAN_SLEEP_MCPM_START_1);
#endif

   /* We only want to do this if RF has been turned off,
      this for W+G is done in task context and not from start sleep */
   if (gl1_hw_sleep_get_rf_on(gas_id) == FALSE)
   {
      MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" MCPM de conf from DPC call");
    /* Since sleep req is scheduled through DPC we need to know to track if the sleep req is pending or not.
     * This helps us in the rude wake up scenarios, where we don't end up running wake up before sleep req
     * is scheduled through DPC */
    mcpm_sleep_req_pending[gas_id] = TRUE;

   if (!rexl4_queue_dpc((rex_apc_func_type)gl1_hw_mcpm_gts_apc, gas_id))
   {
       MSG_GERAN_ERROR_0_G("Failed to queue GTS MCPM update DPC");
   }

   /* If NV item for sleep is not set and gcf flag enabled, then we won't go to sleep */
   if( gl1_hw_sleep_disable[gas_id] == TRUE )
   {
       /* disable going to sleep / don't go to sleep */
       if ( gl1_hw_latency_client[gas_id] )
       {
           npa_issue_required_request(gl1_hw_latency_client[gas_id],
                                      gl1_hw_latency_status[gas_id].gl1_hw_latency_sclk);

           gl1_hw_latency_status[gas_id].gl1_hw_latency_sent = TRUE;
       }
       else
       {
           MSG_GERAN_ERROR_0_G(GL1_SLEEP_HDR" GL1 latency client handle null");
       }
   }
   }
   else
   {
      MSG_GERAN_ERROR_0_G(GL1_SLEEP_HDR" RF ON, No MCPM De Configure");
   }

#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
   l1_log_physical_layer_power_info(TRUE,gas_id);
#endif

#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
   QDSS_logging( gas_id, GERAN_SLEEP_MCPM_END_1);
#endif
   gl1_hw_sleep_debug_duration ( &(gl1_hw_sleep_cycle->start.mcpm) );
  return;
}
/*===========================================================================
FUNCTION GL1_HW_SCLKS_UNTIL_WAKEUP

DESCRIPTION
  Determines the minimum time before a wakeup point

DEPENDENCIES
  None

RETURN VALUE
  Number of gsm clks until wakeup

SIDE EFFECTS
  None

===========================================================================*/
int64 gl1_hw_sys_clks_until_wakeup( gas_id_t gas_id )
{
   int64  sys_clks_till_wup;
   uint64 sys_clks_slept = slpc_get_sysclk_count( gl1_hw_slpc_from_gas_id( gas_id ) );

   sys_clks_till_wup = (uint64)(sleep_duration_gsm_clks[gas_id] - (gl1_hw_sleep_warmup_time_us[gas_id])*GSM_CLK_FREQUENCY) - sys_clks_slept;

   return( sys_clks_till_wup );
}

slpc_id_type gl1_hw_slpc_from_gas_id( gas_id_t gas_id )
{
   slpc_id_type slpc_from_gas = GL1_SLPC_GSM_0;

   switch ( gas_id )
   {
     case GERAN_ACCESS_STRATUM_ID_1:
       slpc_from_gas = GL1_SLPC_GSM_0;
     break;

     case GERAN_ACCESS_STRATUM_ID_2:
       slpc_from_gas = GL1_SLPC_GSM_1;
     break;

#if defined ( FEATURE_TRIPLE_SIM )
     case GERAN_ACCESS_STRATUM_ID_3:
       slpc_from_gas = GL1_SLPC_GSM_2;
     break;
#endif /* FEATURE_TRIPLE_SIM */

     default:
       MSG_GERAN_FATAL_0_G( "Invalid gas_id" );
     break;
   }

   return ( slpc_from_gas );
}


/*===========================================================================

FUNCTION gl1_hw_wcdma_slpc_from_gas_id

DESCRIPTION
  Returns the WCMDA SLPC id number for gas_id

DEPENDENCIES
  None

RETURN VALUE
  WCDMA id from the SLPC

SIDE EFFECTS
  None, will work if Dual Multisim defined

===========================================================================*/
slpc_id_type gl1_hw_wcdma_slpc_from_gas_id( gas_id_t gas_id )
{
   slpc_id_type wcdma_slpc_from_gas = GL1_SLPC_WCDMA_0;

   switch ( gas_id )
   {
     case GERAN_ACCESS_STRATUM_ID_1:
       wcdma_slpc_from_gas = GL1_SLPC_WCDMA_0;
     break;

     case GERAN_ACCESS_STRATUM_ID_2:
#ifdef FEATURE_DUAL_WCDMA
      /* protected this way so when called with gas_id=2
         when feature FEATURE_DUAL_WCDMA not defined it will
         return  default GL1_SLPC_WCDMA_0 */
      wcdma_slpc_from_gas = GL1_SLPC_WCDMA_1;
#endif /* FEATURE_DUAL_WCDMA */
      break;


   default:
       MSG_GERAN_FATAL_0_G( "Invalid gas_id" );
     break;
   }

   return ( wcdma_slpc_from_gas );
}

/*===========================================================================

FUNCTION GL1_HW_SLEEP_GSM_FORCE_WAKEUP

DESCRIPTION
  Ends a period of physical layer sleep prematurely.

DEPENDENCIES
  Physical layer sleep must have previously been initiatd by a call to
  gl_hw_sleep_gsm_start_sleep.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint32 gl1_hw_sleep_gsm_force_wakeup( gas_id_t gas_id )
{
   uint32   frames_early, forced_warmup_time;
   boolean  enough_time_to_wakeup = FALSE;
   uint32   forced_wakeup_threshold;
   int64    sys_clks_till_wup;

  slpc_err_type slpc_err = SLPC_ERR_NO_ERR;
   slpc_id_type  slpc_from_gas = gl1_hw_slpc_from_gas_id( gas_id );

   if ( gl1_hw_sleep_state[gas_id] != GL1_HW_SLEEP_ASLEEP )
   {
       /* No force wakeup */
       MSG_GERAN_ERROR_1_G( GL1_SLEEP_HDR" Forced wake up not possible gl1_hw_sleep_state[gas_id] %d",
                            gl1_hw_sleep_state[gas_id]);
       return ( 0 );
   }

   /* Ensure that gsm force wakeup processing and wakeup cancellation processing
    * do not happen simultaneously. Wait for wkup cancel to end before starting
    * warmup - protect into and out of GL1_HW_SLEEP_ASLEEP transition
    */
   MSG_GERAN_MED_0_G( "WMGR_SLEEP: ML force_wakeup");
   gl1_hw_sleep_wmgr_mutex_lock(gas_id);

   REX_DISABLE_PREMPTION( );

   forced_wakeup_threshold = FORCE_WAKEUP_PROC_TIME;

   if(scheduled_timeline_adjustment[gas_id])
   {
       forced_wakeup_threshold += (optimised_warmup_delta[gas_id]*GSM_CLK_FREQUENCY);
   }

   sys_clks_till_wup = gl1_hw_sys_clks_until_wakeup (gas_id);
   enough_time_to_wakeup = ( sys_clks_till_wup > forced_wakeup_threshold );

   MSG_GERAN_HIGH_1_G( GL1_SLEEP_HDR" Forced wake up enough_time_to_wakeup %d", enough_time_to_wakeup);

   /* Check for minimum number of gsm clocks before wakeup point*/
   if( enough_time_to_wakeup  )
   {
       uint64 sys_clks_slept              = 0;
      uint64 sleep_clks_end               = 0;
       uint32 new_sleep_duration_frames   = 0;
       uint32 new_sleep_duration_gsm_clks = 0;

       gl1_hw_sleep_type  *gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];

       gl1_hw_sleep_cycle->forced_wakeup = TRUE;
       /* Early wake up is going to happen */
       gl1_hw_sleep_state[gas_id] = GL1_HW_EARLY_WAKEUP;

       gl1_hw_sleep_set_forcedwakeup(gas_id, TRUE);

       /* Since we're going to wakeup on a frame boundary make sure that
        * late online start offset is 0.
        */
       late_ols_offset[gas_id] = 0;

       /* Tell the sleep controller to end the sleep interval on the next praticable
        * frame boundary.  To do this the sleep controller needs to know the
        * early go to sleep offset.
        */

       /* retrieve the sleep clocks, sys clocks slept, and then reprogram the
        * sleep ending time
        */
       sys_clks_slept =  slpc_get_sysclk_count( slpc_from_gas );

       if ( sys_clks_slept >= early_gts_offset[gas_id] )
       {
          sys_clks_slept -= early_gts_offset[gas_id] ;
       }
       else
       {
          /* Force wakeup was called before the original gts GSTMR tick */
          sys_clks_slept = 0;
       }

       /* sleep_clks_end = processing_time + warmp_up_time */
       forced_warmup_time = gl1_hw_forced_warmup_time( gas_id );

       sleep_clks_end = forced_warmup_time*GSM_CLK_FREQUENCY + FORCE_WAKEUP_PROC_TIME;

       new_sleep_duration_frames = ( ( sys_clks_slept + sleep_clks_end ) / NUM_GSM_CLKS_PER_FRAME );
       /*
        * Increment gsm_frames_to_sleep to count for any reminder with NUM_GSM_CLKS_PER_FRAME, and to
        * align sleep end with GSTMR tick
        */
       new_sleep_duration_frames++;

       new_sleep_duration_gsm_clks = ( ( new_sleep_duration_frames * NUM_GSM_CLKS_PER_FRAME ) + early_gts_offset[gas_id] );

       /*Using a different CB for rude wake up*/
       gl1_hw_sleep_set_warmup_cb(gas_id, TRUE);

       frames_early = ( sleep_duration_frames[gas_id] - new_sleep_duration_frames );

       /* reduce one frame here as this will not run in warm up interrupt */
       frames_early--;

       gl1_hw_frame_number_ols[gas_id] = (gl1_hw_frame_number_gts[gas_id] + new_sleep_duration_frames + 1)&(GERAN_VSTMR_MAX_FN - 1);

       if ( sleep_duration_frames[gas_id] < new_sleep_duration_frames )
       {
          MSG_GERAN_ERROR_2_G( GL1_SLEEP_HDR" Forced wake up failure: last_sleep_duration=%d, new_sleep_duration=%d",
                     sleep_duration_frames[gas_id], new_sleep_duration_frames);
       }

       sleep_duration_frames[gas_id]   = new_sleep_duration_frames;
       sleep_duration_gsm_clks[gas_id] = new_sleep_duration_gsm_clks;

       slpc_err = slpc_set_duration_and_warmup( slpc_from_gas, new_sleep_duration_gsm_clks, forced_warmup_time);

       if (slpc_err != SLPC_ERR_NO_ERR)
       {
           MSG_GERAN_ERROR_1_G(GL1_SLEEP_HDR" SLPC ERROR return for slpc_set_duration_and_warmup during FORCED warmup %d", slpc_err );
       }


       gl1_hw_sleep_cycle->sleep_info.used_warmup_usec = forced_warmup_time;
       gl1_hw_sleep_cycle->sleep_info.scheduled = FALSE;
       gl1_hw_sleep_cycle->sleep_info.gsm_clk_sleep_duration = new_sleep_duration_gsm_clks;

       gl1_drx_update_counters( -(int32)frames_early, TRUE, gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
       gl1_hw_wkupmgr_wakeup_point_fn[gas_id] = l1_fm_get_trm_reserved_fn(gas_id);
#else
        gl1_hw_wkupmgr_wakeup_point_fn[gas_id] = NOT_A_FRAME_NUMBER;
#endif

       MSG_GERAN_MED_3_G(GL1_SLEEP_HDR" GSM state EARLY_WAKEUP %d frames early=%d FN %d",
                           timetick_get_safe(),frames_early, GSTMR_GET_FN_GERAN(gas_id));
   }
   else
   {
      /* No early wakeup */
      frames_early = 0;
   }

   REX_ENABLE_PREMPTION( );

   MSG_GERAN_MED_0_G( "WMGR_SLEEP: MU force_wakeup");
   gl1_hw_sleep_wmgr_mutex_unlock(gas_id);

   return ( frames_early );
}
/*===========================================================================

FUNCTION GL1_HW_SLEEP_GSM_REGISTER_START_CB

DESCRIPTION
  This function may be used to register a callback function that will be
  called once the sleep controller is running from the sleep clock and
  no longer requires the TCXO.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_gsm_register_start_cb( gl1_hw_sleep_gsm_cb_type cb, gas_id_t gas_id )
{
   sleep_start_cb_ptr[gas_id] = cb;
}

/*===========================================================================

FUNCTION GL1_HW_SLEEP_GSM_REGISTER_WARMUP_CB

DESCRIPTION
  This function may be used to register a callback function that will be
  called when the sleep controller interrupts the ARM to begin the TCXO
  warm up period.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_gsm_register_warmup_cb( gl1_hw_sleep_gsm_cb_type cb, gas_id_t gas_id )
{
   sleep_warmup_cb_ptr[gas_id] = cb;
}

/*===========================================================================

FUNCTION  gl1_hw_sleep_gsm_panic_inact

DESCRIPTION
  Immediately end the sleep timeline and turn all the clocks back on.  The
  RF may or may not be on after calling this function.  If this function is
  called during a sleep cycle then the GSTMR will be restarted at a random
  point.  It is assumed that this will be followed by a full reset of L1.

  This function should only perform actions that are required to allow a
  reset of L1 to proceed without a crash.  Anything that can wait until
  L1 is brought up again should be done then (i.e. in gl1_hw_init() or
  gl1_hw_sleep_gsm_init() etc).

===========================================================================*/
void gl1_hw_sleep_gsm_panic_inact( gas_id_t gas_id )
{
    slpc_id_type slpc_from_gas = gl1_hw_slpc_from_gas_id( gas_id );

    MSG_GERAN_MED_0_G(GL1_SLEEP_HDR"panic reset");

    slpc_set_wakeup_callback( slpc_from_gas, NULL );
    slpc_set_ols_callback( slpc_from_gas, NULL );

    gl1_hw_sleep_set_in_warmup_check(FALSE, gas_id);

    /* this should only be called before wake up and after sleep start */
   if ( (gl1_hw_sleep_state[gas_id] == GL1_HW_SLEEP_ASLEEP) || (gl1_hw_sleep_state[gas_id] == GL1_HW_SLEEP_WARMUP))
   {
       gl1_hw_sleep_state[gas_id] = GL1_HW_SLEEP_NULL;

   if(!g1l_hw_is_mcpm_idle (gas_id))
  {
       MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR"abort sleep controller, turn on GSM clock via MCPM");
       gl1_hw_mcpm_state_update(MCPM_GERAN_WAKE_UP_REQ, GL1_MCPM_GERAN_IDLE,gas_id);
  }
#ifdef FEATURE_MODEM_DDR_MGMT
       if (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_SINGLE_SIM)
       {
         /* when trm = reason is invalid , band value is invalid */
         trm_update_tech_state(TRM_GSM1,TRM_NUM_REASONS,SYS_BAND_GSM_EGSM_900);
       }
#endif /* FEATURE_MODEM_DDR_MGMT */

#ifdef FEATURE_WLAN_COEX_SW_CXM
       /* for wake-up duration passed is the time when RF is used */
       gl1_drx_garb_send_sleep_duration(FALSE, 0, gas_id);
#endif

       slpc_abort( slpc_from_gas );
   }
}

/*===========================================================================

FUNCTION  gl1_hw_calc_gsm_time_from_sclk

DESCRIPTION
  This function estimates current GSM time using the sleep clock.  Should
  be called with interrupts locked and only if the sleep controller is
  active.

===========================================================================*/
boolean gl1_hw_calc_gsm_time_from_sclk( uint32 *fn, uint32 *qsym, gas_id_t gas_id )
{

   gl1_get_gsm_time_tag(gas_id,(uint32*)fn,(uint32*)qsym);

   return ( TRUE );
}

/*===========================================================================

FUNCTION  gl1_get_time_next_vsmtr_us

DESCRIPTION
  Returns time in microseconds to the next vstmr tick.

===========================================================================*/
uint32 gl1_get_time_next_vsmtr_us(gas_id_t gas_id)
{
  uint64  sys_clks_slept      = 0;
  int32  time_to_vstmr_sclks = 0;
  int32  time_to_vstmr_us    = 0;

  gas_id = check_gas_id(gas_id);

  /* time to next vstmr = (time we planned to sleep for)-(time slept so far) */

  sys_clks_slept = slpc_get_sysclk_count( gl1_hw_slpc_from_gas_id(gas_id) );

  time_to_vstmr_sclks = (int32)(sleep_duration_gsm_clks[gas_id] - sys_clks_slept);

  /* convert system clks to microseconds */
  time_to_vstmr_us = time_to_vstmr_sclks / 13;

  return time_to_vstmr_us;
}

/*===========================================================================

                            INTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION GL1_HW_INCREMENT_FRAME_NUMBER

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_hw_increment_frame_number ( int32 num, gas_id_t gas_id )
{
   gl1_hw_msg_inc_fn (num, gas_id);
}

boolean gl1_hw_is_forced_wakeup(gas_id_t gas_id)
{
    return forced_wakeup[gas_id];
}
boolean gl1_hw_sleep_get_in_warmup_check( gas_id_t gas_id )
{
    return in_warmup[gas_id];
}

/*===========================================================================

FUNCTION gl1_hw_sleep_get_in_warmup_check

DESCRIPTION
While entering the warmup, in_warmup flag is set to TRUE and during the exit it is set to FALSE

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static void gl1_hw_sleep_set_in_warmup_check( boolean warmup, gas_id_t gas_id )
{
    in_warmup[gas_id] = warmup;
}

/*===========================================================================

FUNCTION GL1_HW_SLEEP_GET_SYS_CLK_TO_OLS

DESCRIPTION
  Return a number of GSM clocks to OLS.

DEPENDENCIES
  None.

RETURN VALUE
  GSM clocks to OLS

SIDE EFFECTS
  None

===========================================================================*/
uint32 gl1_hw_sleep_get_sys_clk_to_ols( gas_id_t gas_id )
{
  uint32 sys_clks_slept, sys_clks_till_gsm_rtc_on;

  sys_clks_slept = slpc_get_sysclk_count( gl1_hw_slpc_from_gas_id( gas_id ) );

  if ( sys_clks_slept > sleep_duration_gsm_clks[gas_id] )
  {
    sys_clks_till_gsm_rtc_on = 0;
  }
  else
  {
    sys_clks_till_gsm_rtc_on =
      ( sleep_duration_gsm_clks[gas_id] - sys_clks_slept );
  }

  return sys_clks_till_gsm_rtc_on;
}

/*===========================================================================

FUNCTION GL1_HW_SLEEP_GET_QS_TO_OLS

DESCRIPTION
  Return a number of QS to OLS.

DEPENDENCIES
  None.

RETURN VALUE
  QS to OLS

SIDE EFFECTS
  None

===========================================================================*/
uint32 gl1_hw_sleep_get_qs_to_ols( gas_id_t gas_id )
{
  /* QS = SYS_CLK / 12. I.e. 60000 SYS_CLK yields 5000.6 QS */
  uint32 qs_till_ols =
    ( (( gl1_hw_sleep_get_sys_clk_to_ols( gas_id ) >> 2) * 2731 ) >> 13 );

  return ( qs_till_ols );
}

void gl1_hw_sleep_set_commit_mask( gl1_hw_sleep_commit_T gl1_hw_sleep_commit_check, gas_id_t gas_id )
{
  gl1_hw_sleep_type  *gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];

  rex_enter_crit_sect(&gl1_hw_sleep_warmup_cb_crit_sect[gas_id]);

  /* check if event was already added */
  if ( !(gl1_hw_sleep_commit_mask[gas_id] & gl1_hw_sleep_commit_check) )
  {
    /* normal case: commit was not done previously */
    switch (gl1_hw_sleep_commit_check)
    {
      case CLEAR_COMMIT:
        gl1_hw_sleep_commit_mask[gas_id] = CLEAR_COMMIT;
        // MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" CLEAR_COMMIT added");
        break;
      case RF_WAKEUP_COMMIT:
        // MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" COMMIT RF WAKEUP added");
        break;
      case RX_BURST_COMMIT:
        gl1_hw_sleep_cycle->wakeup.rf_rx_on.start_ustmr = slpc_get_tick();
        // MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" COMMIT RX BURST added");
        break;
      case TX_BURST_COMMIT:
        // MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" COMMIT TX BURST added");
        break;
      case WAKEUP_GSTMR_ISR_COMMIT:
        // MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" COMMIT GSTMR WAKEUP ISR added");
        break;
      default:
        MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" COMMIT unknown flag");
        break;
    }

    gl1_hw_sleep_commit_mask[gas_id] |= gl1_hw_sleep_commit_check;

    MSG_GERAN_DEBUG_OPT_2_G(GL1_SLEEP_HDR" COMMIT status gl1_hw_sleep_commit_check %d, gl1_hw_sleep_commit_mask[gas_id] %d", gl1_hw_sleep_commit_check, gl1_hw_sleep_commit_mask[gas_id]);

  } /* if (gl1_hw_sleep_commit_mask[gas_id] & gl1_hw_sleep_commit_check) */
  else
  {
    /* error case: commit was already done, not doing it again */
    MSG_GERAN_ERROR_2_G(GL1_SLEEP_HDR" COMMIT event already added: gl1_hw_sleep_commit_check %d, gl1_hw_sleep_commit_mask[gas_id] %d", gl1_hw_sleep_commit_check, gl1_hw_sleep_commit_mask[gas_id]);
  }

  rex_leave_crit_sect(&gl1_hw_sleep_warmup_cb_crit_sect[gas_id]);

}

/*===========================================================================

FUNCTION GL1_HW_SLEEP_SET_COMMIT_CHECK

DESCRIPTION
Set all necessary flags before starting the frame tick

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_set_commit_check( gl1_hw_sleep_commit_T gl1_hw_sleep_commit_check, gas_id_t gas_id )
{
  gl1_hw_sleep_type  *gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];

  rex_enter_crit_sect(&gl1_hw_sleep_warmup_cb_crit_sect[gas_id]);

  switch (gl1_hw_sleep_commit_check)
  {
    case CLEAR_COMMIT:
      /* Error: this is not supported in order to avoid unnecessary check before gl1_hw_sleep_commit() and to avoid possible recursion when gl1_hw_sleep_commit() is called. The caller should use gl1_hw_sleep_set_commit_mask() instead */
      // MSG_GERAN_ERROR_0_G(GL1_SLEEP_HDR" Error: CLEAR_COMMIT completed not supported here. Use gl1_hw_sleep_set_commit_mask() instead.");
      break;
    case RF_WAKEUP_COMMIT:
      // MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" COMMIT RF WAKEUP completed");
      break;
    case RX_BURST_COMMIT:
      gl1_hw_sleep_cycle->wakeup.rf_rx_on.end_ustmr = slpc_get_tick();
      gl1_hw_sleep_cycle->wakeup.rf_rx_on.duration_us = (((gl1_hw_sleep_cycle->wakeup.rf_rx_on.end_ustmr - gl1_hw_sleep_cycle->wakeup.rf_awake.end_ustmr) * 107 ) >> 11 );
      gl1_hw_sleep_cycle->wakeup.rf_rx_to_vstmr_us = gl1_get_time_next_vsmtr_us(gas_id);
      // MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" COMMIT RX BURST completed");
      break;
    case TX_BURST_COMMIT:
      // MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" COMMIT TX BURST completed");
      break;
    case WAKEUP_GSTMR_ISR_COMMIT:
      // MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" COMMIT GSTMR WAKEUP ISR completed");
      break;
    default:
      MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" COMMIT unknown flag");
      break;
  }

  if (gl1_hw_sleep_commit_mask[gas_id] & gl1_hw_sleep_commit_check)
  {
    /* normal case: complete came for a previously added event */

    /* clear the event (marked as completed) */
    gl1_hw_sleep_commit_mask[gas_id] &= ~gl1_hw_sleep_commit_check;

    /* check if all events are completed */
    if (gl1_hw_sleep_commit_mask[gas_id] == CLEAR_COMMIT)
    {
      /* all events are complete */
      MSG_GERAN_HIGH_1_G(GL1_SLEEP_HDR" COMMIT to Vstmr, gl1_hw_sleep_commit_check %d", gl1_hw_sleep_commit_check);

      gl1_hw_sleep_commit( gas_id );
    }
    else
    {
      /* some events are still pending, so we cannot commit yet */
      // MSG_GERAN_HIGH_2_G(GL1_SLEEP_HDR" No COMMIT events pending , gl1_hw_sleep_commit_check %d, gl1_hw_sleep_commit_mask[gas_id] %d", gl1_hw_sleep_commit_check, gl1_hw_sleep_commit_mask[gas_id]);
    } /* else - if (gl1_hw_sleep_commit_mask[gas_id] == CLEAR_COMMIT) */
  } /* if (gl1_hw_sleep_commit_mask[gas_id] & gl1_hw_sleep_commit_check) */
  else
  {
    /* ERROR: we received complete event before event was added */
    MSG_GERAN_ERROR_2_G(GL1_SLEEP_HDR" Error: complete event came before event was added, or complete came more than once. gl1_hw_sleep_commit_check %d, gl1_hw_sleep_commit_mask[gas_id] %d", gl1_hw_sleep_commit_check, gl1_hw_sleep_commit_mask[gas_id] );
  } /* else - if (gl1_hw_sleep_commit_mask[gas_id] & gl1_hw_sleep_commit_check) */

  rex_leave_crit_sect(&gl1_hw_sleep_warmup_cb_crit_sect[gas_id]);

}

/*===========================================================================

FUNCTION GL1_HW_SLEEP_COMMIT

DESCRIPTION
Commit to wakeup if there is no ISR overrun; trigger the recovery mechanism otherwise
Caller should be in critical section "gl1_hw_sleep_warmup_cb_crit_sect[gas_id])"

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_commit( gas_id_t gas_id )
{
  uint32              sys_clks_slept;
  int32               sys_clks_till_gsm_rtc_on = 0;
  int32               commit_threshold_clks    = 0;
  boolean             commit_successful        = FALSE;
  int32               frames_early;
  uint16              prev_fn_ols;
  uint16              efs_delay;
  uint8               efs_cycle;
  uint8               efs_delay_index;
  uint8              efs_cycle_index;
  int               curr_activity_count;
  l1_fm_priority_T   required_activity,curr_activity;
  uint32             timer_start,timer_end;


#ifdef FEATURE_GSM_ADAPTIVE_TMLN
  int16               gap_to_ols_sample        = 0;
#endif /* FEATURE_GSM_ADAPTIVE_TMLN */
  gl1_hw_sleep_type  *gl1_hw_sleep_cycle       = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_HANDLER*/

  gl1_hw_rf_result_monitor_timer_deactivate(gas_id);

  /*This piece of code is for DTF scenario Forced error recovery which adds delay specified through QSH command*/
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
	 #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_HANDLER*/

if ( GL1_HW_EFS_SLP_ENABLED_ERROR_REC(GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_ON,gas_id) )
	  {
	  curr_activity = l1_fm_get_running_activity(gas_id);
	

 
	  required_activity =(l1_fm_priority_T) (GL1_HW_EFS_SLP_ENABLED_ERROR_REC (GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_ACTIVITY,gas_id)>>GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_ACTIVITY_POSITION);
	  efs_delay_index= (GL1_HW_EFS_SLP_ENABLED_ERROR_REC (GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_DELAY,gas_id)>>GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_DELAY_POSITION);
	  
      efs_delay= gl1_efs_slp_regular_int_delay_lut[efs_delay_index];
	  
	  efs_cycle_index=(GL1_HW_EFS_SLP_ENABLED_ERROR_REC (GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_CYCLE,gas_id)>>GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_CYCLE_POSITION);
	  /*(gl1_hw_efs_get_slp_error_rec()>>(gas_id*16)) & ( GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_CYCLE)>>8 )*/
	  
      efs_cycle=gl1_efs_slp_regular_int_cycle_lut[efs_cycle_index];
	  
	   MSG_GERAN_HIGH_5_G("EFS DEBUG: Current Acitvity = %d	,Required Activity =%d ,Error_Cycle_Count= %d,Delay =%dus, Activity_Count= %d ",curr_activity, required_activity,efs_cycle,efs_delay,gl1_efs_slp_force_error_wakeup_activity_count[gas_id]);

	  if(required_activity==curr_activity)
	  {
	    gl1_efs_slp_force_error_wakeup_activity_count[gas_id]++;
		
	  	curr_activity_count= gl1_efs_slp_force_error_wakeup_activity_count[gas_id];
		
	  	MSG_GERAN_HIGH_1_G("EFS DEBUG: Current Acitvity count = %d",curr_activity_count);
		
	  	if((curr_activity_count % efs_cycle)==0)
		{
		  	gl1_efs_slp_force_error_wakeup_activity_count[gas_id]=0;
			timer_start= timetick_get_safe();
		  	//gl1_hw_delay(efs_delay,gas_id);
		  	gl1_non_blocking_delay( efs_delay,L1_RECOVERY_DELAY_TIMEOUT_SIG,rex_self());
			timer_end= timetick_get_safe();
			MSG_GERAN_HIGH_4_G("EFS DEBUG: Start Timer =%d End Timer = %d DIfference in slcks =%d, delay in ms=%d",timer_start,timer_end,(timer_end-timer_start),(timer_end-timer_start)*4.615/151);
		}
	  
	  }
  	}

  /* Disable preemption and acquire the main mutex needed to commit until we have
     issued the command to GFW, or if entering error recovery we can enable much
     sooner. The code while preemption disabled should be as efficient as possible.
     If we didn't disable preemption we could make a decision to commit when near
     the threshold, get heavily preempted and then find we are issuing the command
     to GFW too late. */
  GDRV_ISR_SAVE_LOCK(gas_id);
  REX_DISABLE_PREMPTION();

  sys_clks_slept = (uint32)slpc_get_sysclk_count( gl1_hw_slpc_from_gas_id( gas_id ) );
  sys_clks_till_gsm_rtc_on = (int32)(sleep_duration_gsm_clks[gas_id] - sys_clks_slept);
  
  gl1_hw_sleep_cycle->sleep_info.commit_gsmclks = sys_clks_till_gsm_rtc_on;
  gl1_hw_sleep_cycle->sleep_info.commit_pcycles = (uint32)qurt_get_core_pcycles();
  gl1_hw_sleep_cycle->sleep_info.commit_enter_mode_to_ols_us = gl1_hw_get_enter_mode_completed_to_ols_us(gas_id);
  gl1_hw_sleep_cycle->sleep_info.commit_enter_mode_this_wakeup = gl1_hw_check_enter_mode_sent_this_wakeup(gas_id);

  if( gl1_hw_sleep_get_forcedwakeup(gas_id) )
  {  
    frames_early = (sys_clks_till_gsm_rtc_on - GL_HW_SLEEP_COMMIT_THRESHOLD_SCLKS)  / NUM_GSM_CLKS_PER_FRAME;

    /* calc if we can start VSTMR early */
    if(frames_early > 0)
    {  
      /* update sw FN */
      gl1_drx_update_counters( -(int32)frames_early, TRUE, gas_id );

      /* update hw FN */
      prev_fn_ols  = gl1_hw_frame_number_ols[gas_id];
      gl1_hw_frame_number_ols[gas_id] = (gl1_hw_frame_number_ols[gas_id] - frames_early)&(GERAN_VSTMR_MAX_FN - 1);
      MSG_GERAN_MED_4_G(GL1_SLEEP_HDR" Force Wakeup: frames_early: %d OLS before: %d, after: %d, SW_FN: %d",
                     frames_early, prev_fn_ols, gl1_hw_frame_number_ols[gas_id],GSTMR_GET_FN_GERAN(gas_id));
    }
  }
 
  /* Set up the threshold value depending on the UE LMM mode */
#ifdef FEATURE_QTA
  if( gl1_hw_qta_gap_active(gas_id) )
  {
    commit_threshold_clks = GL_HW_SLEEP_COMMIT_THRESHOLD_SCLKS;
  }
  else
#endif
  if (!gl1_hw_ncell_pch_low_monitor_active(gas_id))
  {
    commit_threshold_clks = GL_HW_SLEEP_COMMIT_THRESHOLD_NO_LMM_SCLKS;
  }
  else
  {
    commit_threshold_clks = GL_HW_SLEEP_COMMIT_THRESHOLD_SCLKS;
  }

  /* First condition checks for sufficent time before the OLS to schedule the
     VSTMR etc if we do commit. The latter conditions check there was sufficient
     time from the end of the RF ENTER MODE to the OLS for the CCS scripts to be
     sent to the RFFE (but only apply this check for non-forced wakeups and if
     this wakeup included an RF ENTER MODE). */
  if( (sys_clks_till_gsm_rtc_on > commit_threshold_clks)
      &&
      (
        (gl1_hw_sleep_get_forcedwakeup(gas_id) == TRUE)
        ||
        (!gl1_hw_check_enter_mode_sent_this_wakeup(gas_id))
        ||
        (gl1_hw_get_enter_mode_completed_to_ols_us(gas_id) > (int32)gl1_hw_get_ccs_write_time_estimate(gas_id))
      )
    )
  {
    /* Normal scenario, enough time to commit */
    commit_successful = TRUE;
    MSG_GERAN_HIGH_4_G(GL1_SLEEP_HDR" COMMIT sys_clks_till_gsm_rtc_on=%d sys_clk enter_mode_this_wakeup=%d enter_mode_to_ols=%dus ccs_write_time_estimate=%dus", 
                       sys_clks_till_gsm_rtc_on,
                       gl1_hw_check_enter_mode_sent_this_wakeup(gas_id),
                       gl1_hw_get_enter_mode_completed_to_ols_us(gas_id),
                       (int32)gl1_hw_get_ccs_write_time_estimate(gas_id));
    gl1_hw_sleep_set_entered_recovery(gas_id, FALSE);
    gl1_hw_consecutive_recovery_counter[gas_id] = 0;
    /* If we have woken up in QTA GAP, gl1_hw_sleep_get_qta_active will be FALSE as this is set
       at the end of warm up interrupt after commit to Vstmr */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if ( !gl1_hw_sleep_get_forcedwakeup(gas_id) && !gl1_hw_sleep_get_qta_active(gas_id) )
#else
    if ( !gl1_hw_sleep_get_forcedwakeup(gas_id) )
#endif
    {
      mdsp_wakeup_issue_async_msg (GFW_ASYNC_CMD, gas_id, gl1_hw_frame_number_mdsp_wakeup[gas_id] );
    }
    vstmr_schedule_event(gas_id, gl1_hw_frame_number_ols[gas_id], FALSE);

    /*Commit is successful, VSTMR event is scheduled so set in_warmup to FALSE*/
    gl1_hw_sleep_set_in_warmup_check(FALSE, gas_id);
  }
  else
  {
    /* Insufficient time to commit */
    commit_successful = FALSE;
  }
  
  REX_ENABLE_PREMPTION();

  /* Finish non-time critical processing */
#ifdef FEATURE_GSM_ADAPTIVE_TMLN
  gap_to_ols_sample = sys_clks_till_gsm_rtc_on/GSM_CLK_FREQUENCY;
  MSG_GERAN_HIGH_2_G(GL1_TMLN_HDR" Commit/Error gap=%dus commit_successful=%d",
                     gap_to_ols_sample, 
                     commit_successful);
  gl1_hw_adaptive_warmup_time(gas_id, gap_to_ols_sample, commit_successful);
#endif /* FEATURE_GSM_ADAPTIVE_TMLN */
  if(!commit_successful)
  {
    MSG_GERAN_HIGH_4_G(GL1_SLEEP_HDR" ERROR RECOVERY sys_clks_till_gsm_rtc_on=%d enter_mode_this_wakeup=%d enter_mode_to_ols=%dus ccs_write_time_estimate=%dus", 
                       sys_clks_till_gsm_rtc_on,
                       gl1_hw_check_enter_mode_sent_this_wakeup(gas_id),
                       gl1_hw_get_enter_mode_completed_to_ols_us(gas_id),
                       (int32)gl1_hw_get_ccs_write_time_estimate(gas_id));
    gl1_hw_sleep_warmup_recover( gas_id );
  }
  
  GDRV_ISR_SAVE_UNLOCK(gas_id);

  gl1_hw_sleep_set_commit_mask (CLEAR_COMMIT, gas_id);
  gl1_hw_sleep_set_in_warmup_check(FALSE, gas_id);
}


void gl1_non_blocking_delay( uint16 delay,rex_sigs_type  wait_sig, rex_tcb_type *tcb_ptr)
{
      static rex_timer_type  delay_timer;
  
    /* Following will define a timer which when expires sends the indication thru wait_sig to the tcb_ptr .*/
       rex_def_timer ( &delay_timer, tcb_ptr, wait_sig );
       (void)rex_set_timer( &delay_timer,delay );  /* TIME IN MS*/
       (void)rex_wait (wait_sig);
       (void)rex_clr_sigs( tcb_ptr, wait_sig );

  }

/*===========================================================================

FUNCTION GL1_HW_SLEEP_GSM_WARMUP_CB_DPC

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_hw_sleep_gsm_warmup_gas_1_cb_dpc( void )
{
  gl1_hw_sleep_gsm_warmup_cb_dpc(GERAN_ACCESS_STRATUM_ID_1, gl1_hw_sleep_gsm_warmup_gas_1_cb);
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static void gl1_hw_sleep_gsm_warmup_gas_2_cb_dpc( void )
{
  gl1_hw_sleep_gsm_warmup_cb_dpc(GERAN_ACCESS_STRATUM_ID_2, gl1_hw_sleep_gsm_warmup_gas_2_cb);
}
#endif

#if defined ( FEATURE_TRIPLE_SIM )
static void gl1_hw_sleep_gsm_warmup_gas_3_cb_dpc( void )
{
  gl1_hw_sleep_gsm_warmup_cb_dpc(GERAN_ACCESS_STRATUM_ID_3, gl1_hw_sleep_gsm_warmup_gas_3_cb);
}
#endif /* FEATURE_TRIPLE_SIM */


static void gl1_hw_sleep_gsm_warmup_cb_dpc( gas_id_t gas_id, slpc_event_callback_type w_cb)
{
   if (!rexl4_queue_dpc((rex_apc_func_type)w_cb, 0))
   {
       /* Queuing to DPC failed. So, calling the warm up CB directly */
       MSG_GERAN_ERROR_0_G("Failed to queue warm up cb to DPC");
       gl1_hw_sleep_gsm_warmup_cb(gas_id);
   }

   /* Set the CB back to the non-DPC */
   gl1_hw_sleep_set_warmup_cb(gas_id, FALSE);
}
/*===========================================================================

FUNCTION GL1_HW_SLEEP_GSM_WARMUP_CB

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_hw_sleep_gsm_warmup_gas_1_cb( void )
{
    gl1_hw_sleep_gsm_warmup_cb( GERAN_ACCESS_STRATUM_ID_1 );
}
#ifdef FEATURE_DUAL_SIM
static void gl1_hw_sleep_gsm_warmup_gas_2_cb( void )
{
    gl1_hw_sleep_gsm_warmup_cb( GERAN_ACCESS_STRATUM_ID_2 );
}
#endif

#if defined ( FEATURE_TRIPLE_SIM )
static void gl1_hw_sleep_gsm_warmup_gas_3_cb( void )
{
    gl1_hw_sleep_gsm_warmup_cb( GERAN_ACCESS_STRATUM_ID_3 );
}
#endif /* FEATURE_TRIPLE_SIM */


static void gl1_hw_sleep_gsm_warmup_cb( gas_id_t gas_id )
{
  boolean mdsp_woke_up = FALSE;

  gl1_hw_sleep_type  *gl1_hw_sleep_cycle;

  /* return the id of GSM SLPC */
  slpc_id_type slpc_from_gas = gl1_hw_slpc_from_gas_id( gas_id );
  /* return the id of WCDMA SLPC */
  slpc_id_type wcdma_slpc_from_gas = gl1_hw_wcdma_slpc_from_gas_id( gas_id );
  idle_data_T      *l1_idle_data_ptr = &l1_idle_data_store[gas_id];
  mcpm_tech_type mcpm_tech = gl1_hw_map_gas_id_to_mcpm_tech( gas_id );
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);
  uint32 warmup_ts;
  uint32 trm_when;
  uint32 trm_fn;
  l1_fm_priority_T trm_pri;
  uint32 this_FN;

#ifdef FEATURE_MODEM_DDR_MGMT
  ARFCN_T arfcn;
#endif /* FEATURE_MODEM_DDR_MGMT */

  ustmr_type programmed_to_run_ustmr = 0;
  ustmr_type actually_run_ustmr      = slpc_get_tick();

#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
  QDSS_logging( gas_id, GERAN_SLEEP_WAKEUP_START_1);
#endif /* FEATURE_GL1_SLEEP_QDSS_LOG */

  l1_idle_data_ptr->idle_power_opt = MCPM_Get_PwrOpt_Level(mcpm_tech);

  gl1_hw_sleep_set_in_warmup_check(TRUE, gas_id);
  gl1_hw_sleep_set_rf_cnf_pending(FALSE, gas_id);
  warmup_ts = timetick_get_safe();

  trm_when = gl1_hw_sleep_wmgr_trm_when_cur[gas_id];

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  trm_fn  = l1_fm_get_trm_reserved_fn(gas_id);
  trm_pri = l1_get_fm_trm_priority(gas_id);
#else
  trm_fn  = NOT_A_FRAME_NUMBER;
  trm_pri = FM_PRI_RESERVED;
#endif
  this_FN = GSTMR_GET_FN_GERAN(gas_id);

  if(gl1_hw_sleep_get_forcedwakeup(gas_id))
  {
    /* rude wakeup */
    MSG_GERAN_HIGH_5_G(GL1_SLEEP_HDR" GERAN Warm up interrupt ts RW %u, Rude Wake - 1, FN: %d, wmgr_can: %d, trm_pri: %d, trm_fn: %d",
     warmup_ts, this_FN, gl1_hw_sleep_wmgr_cancel_in_prog[gas_id], trm_pri, trm_fn );
  }
  else
  {
    if(gl1_hw_sleep_wmgr_cancel_in_prog[gas_id])
    {
      /* wkup cancellation wakeup */ 
      MSG_GERAN_HIGH_5_G(GL1_SLEEP_HDR" GERAN Warm up interrupt ts CW %u, Rude Wake - 0, FN: %d, trm_when: %u, trm_diff: %d, trm_pri: %d",
       warmup_ts, this_FN, trm_when, (warmup_ts - trm_when), trm_pri);
    }
    else
    {
      /* normal wakeup */ 
      MSG_GERAN_HIGH_5_G(GL1_SLEEP_HDR" GERAN Warm up interrupt ts NW %u, Rude Wake - 0, FN: %d, trm_when: %u, trm_diff: %d, trm_pri: %d",
       warmup_ts, this_FN, trm_when, (warmup_ts - trm_when), trm_pri);
    }
  }

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( gl1_msg_get_multi_sim_mode() && (!gl1_hw_sleep_get_forcedwakeup(gas_id)) && !gl1_hw_sleep_get_qta_active(gas_id) )
  {
    /* Request TRM lock. */
    grm_request_at_warmup(gas_id);
  }
  else
#endif
  {
    #ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
    /* For WLAN and Antenna sharing configuration, If SAIC/AEQ is active during
       sleep warmup callback then send TRM request and notify for Drx client to
       enable IDLE RxD.
       Note: The TRM grant using grant callback can be delayed due the way the
       TRM grants are handled and this will result in IDLE RxD being activated
       after few frames. */
    MSG_GERAN_HIGH_4_G(GL1_RXD"ANT_SHARING: Send TRM R&NE at sleep warmup for IDLE RxD."
      "(Saic, MSIM, forced Wake, IdleRxd NV) = (%d, %d, %d, %d)", gl1_hw_get_saic_state(gas_id),
      gl1_msg_get_multi_sim_mode(), gl1_hw_sleep_get_forcedwakeup(gas_id), gl1_get_IMRD_ctl_nv(gas_id));

    if ((!gl1_msg_get_saic_disable_in_idle(gas_id))
         && (gl1_msg_get_multi_sim_mode() == FALSE)
         && (gl1_hw_sleep_get_forcedwakeup(gas_id) == FALSE)
         && (gl1_get_IMRD_ctl_nv(gas_id) == TRUE)
         && (gl1_get_rxd_ctl_nv(gas_id) == TRUE)
      )
    {
      MSG_GERAN_HIGH_0_G(GL1_RXD"ANT_SHARING:Request TRM R&NE at sleep warmup for IDLE RxD.");
      grm_request_divrx_at_warmup(gas_id);
    }

     gl1_hw_set_fwakeup_rxd_log(gl1_hw_sleep_get_forcedwakeup(gas_id), gas_id);

    #endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */
  }

  /* Scheduled Sleep request may be pending in DPC.
   * We shouldn't do a MCPM wake up req till the pending sleep req is finished */
   while (mcpm_sleep_req_pending[gas_id] == TRUE)
   {
     MSG_GERAN_LOW_1_G("MCPM GERAN_SLEEP pending in DPC, delay MCPM wake up req by %d", GL1_HW_MCPM_START_STOP_DELAY_LOOP);
     gl1_hw_delay(GL1_HW_MCPM_START_STOP_DELAY_LOOP,gas_id);
   }

  /* This is intended to prevent the GSTMR interrupt from
   * being serviced before the SBI mode has been returned
   * to mDSP mode.
   */

  GDRV_ISR_LOCK(gas_id);
  REX_STAT_DISABLE();
#ifdef FEATURE_GSM_ADAPTIVE_TMLN
  gl1_hw_adaptive_clear_valid_flag(gas_id);
#endif

  /* Acquire Mutex
   * Wait for wkup cancel to end before starting warmup. We need to protect
   * in and out of GL1_HW_SLEEP_ASLEEP transition
   */
  MSG_GERAN_MED_0_G( "WMGR_SLEEP: ML warmup_cb");
  gl1_hw_sleep_wmgr_mutex_lock(gas_id);

  gl1_hw_sleep_state[gas_id] = GL1_HW_SLEEP_WARMUP;

  MSG_GERAN_MED_2_G( "WMGR_SLEEP: warmup_cb: after_prog: %d, cancel_in_prog: %d",
    gl1_hw_sleep_wmgr_cancel_after_prog[gas_id], gl1_hw_sleep_wmgr_cancel_in_prog[gas_id]);

  gl1_hw_sleep_wmgr_pch_cancel_allowed[gas_id] = TRUE;
  gl1_hw_sleep_wmgr_cancel_in_prog[gas_id] = FALSE;
  gl1_hw_sleep_wmgr_cancel_after_prog[gas_id] = FALSE;

  /* Relinquish Mutex */
  MSG_GERAN_MED_0_G( "WMGR_SLEEP: MU warmup_cb");
  gl1_hw_sleep_wmgr_mutex_unlock(gas_id);

  G_commit[gas_id] = FALSE;

  /* Reset flags used later in commit decision */
  gl1_hw_reset_time_enter_mode_completed(gas_id);

  /* point to current buffer */
  gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];

  /* set last wake up to FALSE */
  gl1_hw_sleep_cycle->active = FALSE;

  /* This was stored at end of last sleep cycle */
  programmed_to_run_ustmr = gl1_hw_sleep_cycle->wakeup.wakeup_delay.programmed_to_run;

  /* increment the pointer to next buffer */
  if ( gl1_sleep_cnt[gas_id] < (NUM_OF_GL1_DEBUG_CYCLES - 1) )
  {
    gl1_sleep_cnt[gas_id] ++;
  }
  else
  {
    gl1_sleep_cnt[gas_id] = 0;
  }

  gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];
  memset( gl1_hw_sleep_cycle, 0, sizeof( *gl1_hw_sleep_cycle ) );

  gl1_hw_sleep_cycle->sleep_info.low_monitor_mode = gl1_hw_ncell_pch_low_monitor_active( gas_id );
  gl1_hw_sleep_cycle->sleep_info.msim_sys_mode = gl1_msg_get_multi_sim_sys_mode();
  gl1_hw_sleep_cycle->sleep_info.scheduled = scheduled;
  gl1_hw_sleep_cycle->sleep_info.used_warmup_usec = warmup;
  gl1_hw_sleep_cycle->active = TRUE;
  gl1_hw_sleep_cycle->wakeup.total_wakeup_time.start_ustmr = actually_run_ustmr;
  gl1_hw_sleep_cycle->ols.ols_delay.programmed_to_run = slpc_get_ols_tick( slpc_from_gas );
  gl1_hw_sleep_cycle->efs_debug_setting = gl1_hw_efs_get_debug(gas_id);
  gl1_hw_sleep_cycle->wakeup.wakeup_delay.actually_run = actually_run_ustmr;
  gl1_hw_sleep_cycle->wakeup.wakeup_delay.delay = gl1_hw_sleep_cycle->wakeup.wakeup_delay.actually_run - programmed_to_run_ustmr;
  gl1_hw_sleep_cycle->wakeup.wakeup_delay.delay_us = ((( gl1_hw_sleep_cycle->wakeup.wakeup_delay.delay ) * 107 ) >> 11 );

/* Function not needed if MCPM is defined*/
  gl1_hw_sleep_cycle->wakeup.mcpm.start_ustmr = slpc_get_tick();
  gl1_hw_sleep_cycle->wakeup.pre_mcpm_activities_us = (uint32)(((gl1_hw_sleep_cycle->wakeup.mcpm.start_ustmr
                                                                 - actually_run_ustmr) * 107) >> 11);
#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
  QDSS_logging( gas_id, GERAN_SLEEP_MCPM_WAKE_UP_REQ_1);
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( gl1_msg_get_multi_sim_mode() )
  {
    MSG_GERAN_HIGH_2_G( "MCPM change for GERAN Wakeup GOnly %d  MSM %d",
                     gl1_hw_multi_sim_gonly_mode, gl1_hw_multi_sim_mode);
  }
  else
#endif
  {
    MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR "MCPM change for GERAN Wakeup !MSM" );
  }

  gl1_hw_mcpm_state_update(MCPM_GERAN_WAKE_UP_REQ, GL1_MCPM_GERAN_IDLE,gas_id);

#ifdef FEATURE_WLAN_COEX_SW_CXM
  if ( !forced_wakeup[gas_id] )
  {
    /* for wake-up duration passed is the time when RF is used, this time is 1 gsm frame,
     * rounding down the time
     */
    gl1_drx_garb_send_sleep_duration(FALSE, 4, gas_id);
  }
  else
  {
    /* for force wake-up pass duration as 0 */
    gl1_drx_garb_send_sleep_duration(FALSE, 0, gas_id);
  }
#endif

  if( gl1_hw_sleep_disable[gas_id] == TRUE )
  {
    /* we are in test equipment, where we cannot sleep */

    if ( gl1_hw_latency_client[gas_id] )
    {
      npa_complete_request(gl1_hw_latency_client[gas_id]);
      gl1_hw_latency_status[gas_id].gl1_hw_latency_sent = FALSE;
    }
    else
    {
      MSG_GERAN_ERROR_0_G(GL1_SLEEP_HDR" GL1 latency client handle null");
    }
  }

  gl1_hw_sleep_debug_duration ( &(gl1_hw_sleep_cycle->wakeup.mcpm) );
#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
  QDSS_logging( gas_id, GERAN_SLEEP_MCPM_WAKE_UP_REQ_END_1);
#endif
  gl1_hw_sleep_cycle->wakeup.awake_init.start_ustmr = slpc_get_tick();
  vstmr_wake_up(gas_id);
  gl1_hw_sleep_set_commit_mask(WAKEUP_GSTMR_ISR_COMMIT, gas_id);
  gl1_hw_sleep_state[gas_id] = GL1_HW_SLEEP_AWAKE;
  gsm_slpc[gas_id].gsm_slpc_ols_t_stmr.time = gsm_slpc[gas_id].gsm_slpc_start_t_stmr.time;

  G_commit[gas_id] = slpc_complete_wakeup( gl1_hw_slpc_from_gas_id( gas_id ), G_COMMIT_PROC_TIME, &gsm_slpc[gas_id].gsm_slpc_ols_t_stmr );

  mdsp_woke_up = mdsp_wakeup_geran(FALSE, as_id);


   gl1_hw_sleep_debug_duration ( &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]].wakeup.awake_init);

#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
  MSG_GERAN_MED_1_G(GL1_SLEEP_HDR" GSM state SLEEP_WARMUP %d",
                   timetick_get_safe());
#endif

  gl1_hw_sleep_cycle->wakeup.rf_awake.start_ustmr = slpc_get_tick();
  gl1_hw_sleep_cycle->sleep_info.parallel_warmup = TRUE;
#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
  QDSS_logging( gas_id, GERAN_SLEEP_WAKEUP_RF_RX_ON_START_1);
#endif



  if (
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    (!gl1_msg_get_multi_sim_mode()) &&
#endif
    (!gl1_hw_sleep_cm_active[gas_id]))
  {
    gl1_hw_turn_rx_on( gas_id );
  }



   gl1_hw_sleep_debug_duration ( &(gl1_hw_sleep_cycle->wakeup.rf_awake) );
#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
   QDSS_logging( gas_id, GERAN_SLEEP_WAKEUP_RF_RX_ON_END_1);
   QDSS_logging( gas_id, GERAN_SLEEP_WAKEUP_GSTMR_ON_START_1);
#endif




#ifdef FEATURE_SLPC_TRIPLE_SIM
#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
   /* Only disable other tech slamming if we are maintaining reference count, ie G sub has W neighbour  */
   if ( maintain_wcdma_ref_count[gas_id] )
   {
     MSG_GERAN_HIGH_2_G( GL1_SLEEP_HDR" Disable alt SLPC G %d W %d", slpc_from_gas, wcdma_slpc_from_gas );
     slpc_disable_alt_id( slpc_from_gas, wcdma_slpc_from_gas );
   }

#endif
   slpc_disable_alt_id ( SLPC_GSM, SLPC_TDSCDMA );
#else
#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
   slpc_disable_alt_id ( SLPC_WCDMA );
#endif
#endif /* FEATURE_SLPC_TRIPLE_SIM */


    gl1_hw_sleep_cycle->wakeup.isr.start_ustmr = slpc_get_tick();
    /* run the execution handlers if not a forced wake up*/
    gl1_hw_msim_sleep_commit(gas_id);
   gl1_hw_sleep_debug_duration ( &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]].wakeup.isr);


/* Set this hers after ISR and COmmit to OLS have finished to not bias their operation */
/* This setting is used to inform G sleep that we are in an active QTA GAP             */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  gl1_hw_sleep_set_qta_active(gas_id, gl1_hw_qta_gap_active(gas_id));
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */


  if ( sleep_warmup_cb_ptr[gas_id] )
  {
    /* calls gl1_drx_warmup_int_callback() */
    (*sleep_warmup_cb_ptr[gas_id])( gas_id );
  }

  /* UNLOCK interrupts moved too here for CR182997 */
  /* protect the above cb function as its changing the DRX manager state */
  /* Then unlock interrupts, stops GSTMR interrupting drx manager state change */
  REX_STAT_ENABLE();
  GDRV_ISR_UNLOCK(gas_id);

#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
  MSG_GERAN_MED_1_G(GL1_SLEEP_HDR" GSM state SLEEP_WARMUP complete, int unlocked %d",
        timetick_get_safe() );
#endif

  if ( !mdsp_woke_up )
  {
    MSG_GERAN_ERROR_0_G(GL1_SLEEP_HDR" mDSP didn't confirm wake up");
  }

  if ( mdsp_sleep_error_pending( gas_id ) )
  {
    mdsp_display_sleep_error( TRUE, gas_id );
  }

  /* For accurate timeline measurements this must be the
     last piece of code in the warmup_cb - DO NOT MOVE */
  gl1_hw_sleep_debug_duration ( &(gl1_hw_sleep_cycle->wakeup.total_wakeup_time) );

  gl1_hw_sleep_cycle->sleep_info.warmup_end_to_gstmr_us = gl1_get_time_next_vsmtr_us(gas_id);

  MSG_GERAN_HIGH_5_G(GL1_SLEEP_HDR" delay=%dus pre_mcpm=%dus mcpm=%dus total=%dus warmup_to_vstmr=%dus",                    
                     gl1_hw_sleep_cycle->wakeup.wakeup_delay.delay_us,
                     gl1_hw_sleep_cycle->wakeup.pre_mcpm_activities_us,
                     gl1_hw_sleep_cycle->wakeup.mcpm.duration_us,
                     gl1_hw_sleep_cycle->wakeup.total_wakeup_time.duration_us,
                     gl1_hw_sleep_cycle->sleep_info.warmup_end_to_gstmr_us);
}


/*===========================================================================

FUNCTION GL1_HW_SLEEP_WARMUP_RECOVER

DESCRIPTION
  This function calculates the amount of latency from sleep wakeup
  or during sleep warm up interrupt
DEPENDENCIES
  Call from within int locked code

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_hw_sleep_warmup_recover( gas_id_t gas_id )
{
   uint32 g_slept = 0;
   uint32 error_recovery_gsm_clks = 0;
   uint32 error_recovery_frames = 1;
   uint32 new_sleep_duration_gsm_clks = 0;
   slpc_id_type  slpc_from_gas = GL1_SLPC_GSM_0;
   sys_modem_as_id_e_type as_id;

   gas_id = check_gas_id(gas_id);

   as_id = geran_map_gas_id_to_nas_id(gas_id);

   slpc_from_gas = gl1_hw_slpc_from_gas_id( gas_id );
   gl1_hw_sleep_set_entered_recovery(gas_id, TRUE);

   /* request the number of GSM clocks we have slept for */
   g_slept = slpc_get_sysclk_count( slpc_from_gas );

   /* If time to GSM RTC is not zero, extend timeline by one frame
      as we have not overrun GSTMR by more than 15 sclks. If GSM RTC is zero,
      we have overrun the wake up GSTMR, attempt error recovery meachanism */
   if ( (g_slept) > sleep_duration_gsm_clks[gas_id] )
   {
       /* we have overrun the wakeup GSTMR/Ending interrupt */
       /* depending on how much by, extend timeline accordingly */

       /*LTE app enable takes around 3 to 4msec and G FWS resume is getting delayed 
                so adding larger threshold to error recovery, prevents trying to recover if less than half a frame to run*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     if ( gl1_msg_get_multi_sim_mode() )
     {
       error_recovery_gsm_clks = (( g_slept- sleep_duration_gsm_clks[gas_id] ) + ERROR_RECOVERY_G_GUARD_DSDS_GSM_CLKS);
     }
     else
#endif
     {
        error_recovery_gsm_clks = (( g_slept- sleep_duration_gsm_clks[gas_id] ) + ERROR_RECOVERY_G_GUARD_GSM_CLKS + ERROR_RECOVERY_TIMELINE_GSM_CLKS);
     }
       error_recovery_frames =  ( error_recovery_gsm_clks / NUM_GSM_CLKS_PER_FRAME ) + 1;
   }

       total_error_recovery_frames[gas_id] += error_recovery_frames;

       /* Maximum timeline extension currently, wake up in one frame */
       if ( total_error_recovery_frames[gas_id] > 3 )
       {
#ifdef FEATURE_GSM_SLEEP_ERROR_RECOVERY
          MSG_GERAN_ERROR_3_G(GL1_SLEEP_HDR" Error recovery attempt FAILED. g_slept %d duration %d error_recovery_frames %d",
                              g_slept,sleep_duration_gsm_clks[gas_id],error_recovery_frames);
          gl1_hw_handle_sleep_panic( gas_id );
          return;
#else
#ifdef FEATURE_QSH_MDUMP
          QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif 
          ERR_GERAN_FATAL_3_G(GL1_SLEEP_HDR" Error recovery attempt FAILED. g_slept %d duration %d error_recovery_frames %d",
                              g_slept,sleep_duration_gsm_clks[gas_id],error_recovery_frames);
#endif
       }

   gl1_hw_frame_number_mdsp_wakeup[gas_id]= (gl1_hw_frame_number_mdsp_wakeup[gas_id] + total_error_recovery_frames[gas_id])&(GERAN_VSTMR_MAX_FN - 1);
   gl1_hw_frame_number_ols[gas_id] = (gl1_hw_frame_number_mdsp_wakeup[gas_id]+1)&(GERAN_VSTMR_MAX_FN - 1);
   GSTMR_DISABLE_ODD_EVEN_CHECK_GERAN(as_id);
   gl1_hw_increment_frame_number ( error_recovery_frames, gas_id );

   gl1_hw_sleep_update_scheduled_activity(gas_id);
   gl1_hw_sleep_set_entered_recovery(gas_id, FALSE);

   new_sleep_duration_gsm_clks = sleep_duration_gsm_clks[gas_id] + (error_recovery_frames * NUM_GSM_CLKS_PER_FRAME);
   MSG_GERAN_HIGH_2_G(GL1_SLEEP_HDR" Error recovery attempt. g_slept %d, duration %d",g_slept,sleep_duration_gsm_clks[gas_id]);
   MSG_GERAN_HIGH_3_G(GL1_SLEEP_HDR" Error recovery attempt. error_recovery_frames %d error_recovery_gsm_clks %d new duration %d",
            error_recovery_frames, error_recovery_gsm_clks, new_sleep_duration_gsm_clks);

   gl1_hw_consecutive_recovery_counter[gas_id] = 0;
   total_error_recovery_frames[gas_id] = 0;
   sleep_duration_gsm_clks[gas_id] = new_sleep_duration_gsm_clks;
   sleep_duration_frames[gas_id]  += error_recovery_frames;
}


/*===========================================================================

FUNCTION GL1_HW_SLEEP_SET_MISSED_FRAMES

DESCRIPTION
Set the number of missed frames due to error recovery

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_hw_sleep_set_missed_frames( gas_id_t gas_id, uint8 num_frames )
{
    total_error_recovery_frames[gas_id] += num_frames;
#ifdef DEBUG_GSM_SLEEP_ERROR_RECOVERY
    if ( num_frames )
    {
      MSG_GERAN_HIGH_1_G(GL1_SLEEP_HDR"gl1_hw_sleep_set_missed_frames %d",
                          total_error_recovery_frames[gas_id] );
    }
#endif
}

/*===========================================================================

FUNCTION GL1_HW_SLEEP_GET_MISSED_FRAMES

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  Number of missed frames due to error recovery

SIDE EFFECTS
  None

===========================================================================*/
uint8 gl1_hw_sleep_get_missed_frames( gas_id_t gas_id )
{
#ifdef DEBUG_GSM_SLEEP_ERROR_RECOVERY
    if ( total_error_recovery_frames[gas_id] )
    {
      MSG_GERAN_HIGH_1_G(GL1_SLEEP_HDR"gl1_hw_sleep_get_missed_frames %d",
                          total_error_recovery_frames[gas_id] );
    }
#endif
    return ( total_error_recovery_frames[gas_id] );
}

/*===========================================================================

FUNCTION GL1_HW_SLEEP_CLEAR_MISSED_FRAMES

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_clear_missed_frames( gas_id_t gas_id )
{
    total_error_recovery_frames[gas_id] = 0;
}

/*===========================================================================

FUNCTION GSM_CLKS_TO_TDS_CX8_CLKS

DESCRIPTION
  Converts from GSM clocks to TDS chipx8 clocks.  Return value is
  x * 10.24MHz / 13MHz.

  13MHz -> 10.24MHz (1024/1300 = 256/325 = (2^8)/(5^2*13))
  Calulation is done like this to avoid overflow or too much underflow.
  Comments show significant bits.

  Argument must be less than ((2^32) - 1) / 16 = 268,435,455 ~ 258sec

PARAMETERS
  x - value in gsm clks.

RETURN VALUE
  cx8 clks equivalent to the passed in gsm clks

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_GSM_TDS
uint32 gsm_clks_to_tds_cx8_clks( uint32 x )

{
   x *= 16UL; /* 13.14 */
   x /= 25UL; /*  9.18 */
   x *= 16UL; /* 13.14 */
   x /= 13UL; /* 10.18 (blink) */

   return x;  /* x in cx8 */
}
#endif

/*===========================================================================

FUNCTION GL1_HW_OK_TO_SLEEP

DESCRIPTION
  Checks if sleep controller diver state is OK for going to sleep.

SIDE EFFECTS
  Might ERR_FATAL or panic reset if not OK to go to sleep.

===========================================================================*/
static boolean gl1_hw_ok_to_sleep( gas_id_t gas_id )
{
   boolean OK = FALSE;

      if ( gl1_hw_sleep_state[gas_id] == GL1_HW_SLEEP_AWAKE )
      {
         not_ready_to_sleep_count[gas_id] = 0;

         OK = TRUE;
      }
      else
      {
         if ( ++not_ready_to_sleep_count[gas_id] == MAX_NOT_READY_TO_SLEEP_COUNT )
         {
           MSG_GERAN_ERROR_1_G(GL1_SLEEP_HDR" GSM state %d", gl1_hw_sleep_state[gas_id]);

            gl1_hw_handle_sleep_panic( gas_id );
         }
      }
   return ( OK );
}

/*===========================================================================

FUNCTION GL1_HW_MDSP_SLEEP

DESCRIPTION
  Try to put the mDSP firmware to sleep. This may fail.  This function
  does not turn off the mDSP clk regime.

  After MAX_NOT_SLEEPING_COUNT_MDSP failures the firmware will be forced
  to sleep. In this case forced will be set to TRUE.

RETURN VALUE
  TRUE  - Firmware went to sleep it of OK to turn of mDSP clock.
  FASLE - Firmware did not go to sleep.

SIDE EFFECTS
  None

===========================================================================*/
static boolean gl1_hw_mdsp_sleep( boolean *forced, gas_id_t gas_id )
{
   boolean mdsp_asleep = FALSE;

   if ( mdsp_prepare_for_sleep( gas_id ) )
   {
      /* mDSP firmware went to sleep */
      mdsp_asleep             = TRUE;
      not_sleeping_count_mdsp[gas_id] = 0;
   }
   else
   {
      /* mDSP NAKed sleep - count how many times */
      not_sleeping_count_mdsp[gas_id]++;

      if ( not_sleeping_count_mdsp[gas_id] < MAX_NOT_SLEEPING_COUNT_MDSP )
      {
         MSG_GERAN_HIGH_1_G(GL1_SLEEP_HDR" mDSP not ready to sleep. %d",
                   not_sleeping_count_mdsp[gas_id]);

         if ( mdsp_sleep_error_pending( gas_id ) )
         {
           mdsp_display_sleep_error( FALSE, gas_id );
         }
      }
      else
      {
         /* mDSP NAKed too many times - ignore it and go to sleep anyway */
         forced_sleep_count_mdsp[gas_id]++;

         *forced                 = TRUE;
         mdsp_asleep             = TRUE;
         not_sleeping_count_mdsp[gas_id] = 0;

      }
   }

   return ( mdsp_asleep );
}


/*===========================================================================

FUNCTION GL1_HW_SLEEP_SET_RF_ON

DESCRIPTION
  Check if turning RF ON/OFF is allowed in multi-sim mode.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_set_rf_on( boolean rf_on, gas_id_t gas_id )
{
    gl1_hw_sleep_rf_on[gas_id] = rf_on;

    MSG_GERAN_LOW_1_G(GL1_SLEEP_HDR" RF On %d", rf_on);
}

/*===========================================================================

FUNCTION GL1_HW_SLEEP_GET_RF_ON

DESCRIPTION
  Check if turning RF ON/OFF is allowed in multi-sim mode.

RETURN VALUE
  TRUE  - RF is ON.
  FASLE - RF is OFF.

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_sleep_get_rf_on( gas_id_t gas_id )
{
    MSG_GERAN_LOW_1_G(GL1_SLEEP_HDR" Check RF On %d",gl1_hw_sleep_rf_on[gas_id]);

    return(gl1_hw_sleep_rf_on[gas_id]);
}
/*===========================================================================

FUNCTION gl1_hw_sleep_set_rf_cnf_pending

DESCRIPTION
  Check if turning RF WAKEUP CNF is received.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_set_rf_cnf_pending( boolean rf_cnf_pending, gas_id_t gas_id )
{
  gl1_hw_sleep_rf_cnf_pending[gas_id] = rf_cnf_pending;
}
/*===========================================================================

FUNCTION gl1_hw_sleep_get_rf_cnf_pending

DESCRIPTION
  Check if turning RF WAKEUP CNF is received.

RETURN VALUE
  TRUE  - RF WAKEUP CNF is pending.
  FASLE - RF WAKEUP CNF is not pending.

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_sleep_get_rf_cnf_pending( gas_id_t gas_id )
{
  return(gl1_hw_sleep_rf_cnf_pending[gas_id]);
}
/*===========================================================================

FUNCTION gl1_hw_max_symbol_count_to_sleep

DESCRIPTION
  How many symbols into the frame we are allowed to start the sleep timeline.

RETURN VALUE
  Latest symbol count into the frame for sleep to be started

SIDE EFFECTS
  None

===========================================================================*/
static uint32 gl1_hw_max_symbol_count_to_sleep( gas_id_t gas_id )
{
  uint32 max_symbol_count_sleep = MAX_SYMBOL_COUNT_TO_SLEEP;

  /*
   * Only add extra margin to allow for a slightly later sleep timeline
   * in the case when we are processing the asynch interrupt from the mDSP
   * or the silent frame after an asynch int
   */
  if ( gstmr_get_asynch_int_executing( gas_id ) ||
       gl1_drx_get_asynch_int_ignore_gstmr( gas_id ) )
  {
    max_symbol_count_sleep += ASYNCH_INT_MAX_SYMBOL_COUNT_TO_SLEEP_INCREASE;
  }

  return ( max_symbol_count_sleep );
}

#define G_STMR_FCW_CTL_13MHZ                  0x2B555555

/*===========================================================================

FUNCTION gl1_cm_hw_mcpm_restore_cb

DESCRIPTION
  Wrapper function for gl1_hw_mcpm_restore_cb_geran

===========================================================================*/
void gl1_cm_hw_mcpm_restore_cb (sys_modem_as_id_e_type as_id)
{
   gas_id_t       gas_id =  geran_map_nas_id_to_gas_id(as_id);
   gl1_hw_mcpm_restore_cb_geran(gas_id);
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_restore_cb

DESCRIPTION
  Callback from MCPM to restore registers after power collapse.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_mcpm_restore_gas_1_cb( void )
{
    gl1_hw_mcpm_restore_cb_geran( GERAN_ACCESS_STRATUM_ID_1 );
}

void gl1_hw_mcpm_restore_gas_2_cb( void )
{
    gl1_hw_mcpm_restore_cb_geran( GERAN_ACCESS_STRATUM_ID_2 );
}

#if defined ( FEATURE_TRIPLE_SIM )
void gl1_hw_mcpm_restore_gas_3_cb( void )
{
  gl1_hw_mcpm_restore_cb_geran( GERAN_ACCESS_STRATUM_ID_3 );
}
#endif /* FEATURE_TRIPLE_SIM */

static void gl1_hw_mcpm_restore_cb_geran ( gas_id_t gas_id )
{
   ARFCN_T arfcn;

   gas_id = check_gas_id(gas_id);

   MSG_GERAN_ERROR_0_G( GL1_SLEEP_HDR" RESET GSTMR on wake up from sleep via gl1_hw_mcpm_restore_cb");

   /* Init the gstmr */
   gstmr_init( gas_id );

   /* Update cswallower/DCO for GSTMR, use nominal arfcn */
   gl1_msg_algs_get_arfcn( &arfcn.num, &arfcn.band, gas_id );
   gl1_hw_set_active_cycle_swallower_adj( arfcn, gas_id );
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_slp_tmln_cb

DESCRIPTION
  Callback from MCPM to use optimised sleep timeline or extended sleep timeline.
  Optimised: Normal sceduled Paging decode etc reduced in length
  Non Optimised: Rude wake up or non scheduled wake ups.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_hw_mcpm_slp_tmln_gas_1_cb( boolean optimise_gsm_sleep_timeline )
{
  gl1_hw_mcpm_slp_tmln_cb( optimise_gsm_sleep_timeline,
                           GERAN_ACCESS_STRATUM_ID_1 );
}
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static void gl1_hw_mcpm_slp_tmln_gas_2_cb( boolean optimise_gsm_sleep_timeline )
{
  gl1_hw_mcpm_slp_tmln_cb( optimise_gsm_sleep_timeline,
                           GERAN_ACCESS_STRATUM_ID_2 );
}

#if defined ( FEATURE_TRIPLE_SIM )
void gl1_hw_mcpm_slp_tmln_gas_3_cb( boolean optimise_gsm_sleep_timeline )
{
  gl1_hw_mcpm_slp_tmln_cb( optimise_gsm_sleep_timeline,
                           GERAN_ACCESS_STRATUM_ID_3 );
}
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

static void gl1_hw_mcpm_slp_tmln_cb (boolean optimise_gsm_sleep_timeline, gas_id_t gas_id)
{
    slpc_err_type slpc_err;
    slpc_id_type slpc_from_gas = gl1_hw_slpc_from_gas_id( gas_id );
    mcpm_request_parms_type gl1_mcpm_request_parms;

      if ((optimise_gsm_sleep_timeline == FALSE) &&
          (slpc_get_wakeup_tick(slpc_from_gas) > slpc_get_tick()))
      {
          sleep_duration_gsm_clks[gas_id] += ( NUM_GSM_CLKS_PER_FRAME * GL1_HW_SLEEP_MCPM_DELAY_FRAMES );
          sleep_duration_frames[gas_id] += GL1_HW_SLEEP_MCPM_DELAY_FRAMES;

          slpc_err = slpc_set_duration( slpc_from_gas, sleep_duration_gsm_clks[gas_id] );
          if (slpc_err != SLPC_ERR_NO_ERR)
          {
              MSG_GERAN_ERROR_1_G( GL1_SLEEP_HDR" SLPC ERROR return for slpc_set_duration %d", slpc_err );

              /* When slpc failed to update timeline, restore sleep duration */
              sleep_duration_gsm_clks[gas_id] -= ( NUM_GSM_CLKS_PER_FRAME * GL1_HW_SLEEP_MCPM_DELAY_FRAMES );
              sleep_duration_frames[gas_id] -= GL1_HW_SLEEP_MCPM_DELAY_FRAMES;
          }
          else
          {
              /* Let sleep error recovery to recover from delay. */
              gl1_hw_sleep_set_missed_frames(gas_id, GL1_HW_SLEEP_MCPM_DELAY_FRAMES);
              GSTMR_ADVANCE_FN_GERAN(GL1_HW_SLEEP_MCPM_DELAY_FRAMES, gas_id);
          }

          gl1_mcpm_request_parms.update_info = MCPM_WKTIME_RSRC_UPDATE;
          switch ( gas_id )
          {
          case GERAN_ACCESS_STRATUM_ID_1:
              MCPM_Config_Modem( MCPM_GERAN_PARMS_UPDATE_REQ, &gl1_mcpm_request_parms );
              break;

          case GERAN_ACCESS_STRATUM_ID_2:
              MCPM_Config_Modem( MCPM_GERAN1_PARMS_UPDATE_REQ, &gl1_mcpm_request_parms );
              break;

  #if defined ( FEATURE_TRIPLE_SIM )
          case GERAN_ACCESS_STRATUM_ID_3:
              MCPM_Config_Modem( MCPM_GERAN2_PARMS_UPDATE_REQ, &gl1_mcpm_request_parms );
              break;
  #endif /* FEATURE_TRIPLE_SIM */

          default:
              break;
          }
      }

  scheduled_timeline_adjustment[gas_id] = TRUE;
  MSG_GERAN_MED_3_G(GL1_SLEEP_HDR" optimise_gsm_sleep_timeline %d sleep_duration_frames %d sleep_duration_gsm_clks %d",
                    optimise_gsm_sleep_timeline, sleep_duration_frames[gas_id], sleep_duration_gsm_clks[gas_id]);
}

/*==========================================================================

  FUNCTION      gl1_hw_core_cpu_latency_avail_cb

  DESCRIPTION   Create NPA core CPU latency client

  PARAMETERS    None

  RETURN VALUE  None.

  SIDE EFFECTS  None.

==========================================================================*/
static void gl1_hw_core_cpu_latency_avail_gas_1_cb( void *context,
                                                    unsigned int event_type,
                                                    void *data,
                                                    unsigned int data_size )
{
    gl1_hw_core_cpu_latency_avail_cb( context,
                                      event_type,
                                      data,
                                      data_size,
                                      GERAN_ACCESS_STRATUM_ID_1 );
}

static void gl1_hw_core_cpu_latency_avail_gas_2_cb( void *context,
                                                    unsigned int event_type,
                                                    void *data,
                                                    unsigned int data_size )
{
    gl1_hw_core_cpu_latency_avail_cb( context,
                                      event_type,
                                      data,
                                      data_size,
                                      GERAN_ACCESS_STRATUM_ID_2 );
}


#if defined ( FEATURE_TRIPLE_SIM )
static void gl1_hw_core_cpu_latency_avail_gas_3_cb( void *context,
                                                    unsigned int event_type,
                                                    void *data,
                                                    unsigned int data_size )
{
    gl1_hw_core_cpu_latency_avail_cb( context,
                                      event_type,
                                      data,
                                      data_size,
                                      GERAN_ACCESS_STRATUM_ID_3 );
}
#endif /* FEATURE_TRIPLE_SIM */

static void gl1_hw_core_cpu_latency_avail_cb( void *context,
                                              unsigned int event_type,
                                              void *data,
                                              unsigned int data_size,
                                              gas_id_t gas_id )
{
    gas_id = check_gas_id(gas_id);
    gl1_hw_latency_client[gas_id] = npa_create_sync_client(gl1_hw_latency_node_nm[gas_id],
                                                 gl1_hw_latency_client_nm[gas_id],
                                                 NPA_CLIENT_REQUIRED);
    if( !gl1_hw_latency_client[gas_id] )
    {
        MSG_GERAN_ERROR_0_G(GL1_SLEEP_HDR" GL1 latency client handle null");
    }
    else
    {
        MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" GL1 latency client handle successfully created");
    }
}


/*===========================================================================

FUNCTION gl1_sleepctl_error_feedback

DESCRIPTION
     Added wrapper for Dime migration, units for error TBD

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
void gl1_sleepctl_error_feedback( sclk_enum_type sclkRat, sys_err_t sysclk_error, gas_id_t gas_id)
{
    /* CgC api prototypes not yet defined */
     slpc_error_feedback( sclkRat, sysclk_error );   /* CgC need rescale sclk ->us ? */
}

/*===========================================================================

FUNCTION gl1_sleepctl_get_ustmr_at_ols

DESCRIPTION
     Return USTMR count at OLS

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
ustmr_type gl1_sleepctl_get_ustmr_at_ols( gas_id_t gas_id )
{
    ustmr_type gl1_hw_ustmr_timetick_at_ols_local = 0;
    slpc_id_type slpc_from_gas = gl1_hw_slpc_from_gas_id( gas_id );

    /* The value of gl1_hw_ustmr_timetick_at_ols will be value from last commit if the wake up commit has not been done*/
    if(G_commit[gas_id])
    {
        gl1_hw_ustmr_timetick_at_ols_local = gl1_hw_ustmr_timetick_at_ols[gas_id];
    }
    else
    {
        gl1_hw_ustmr_timetick_at_ols_local = slpc_get_ols_tick( slpc_from_gas ) & SLEEP_USTMR_TIMETICK_COUNT_MASK;
    }

    return gl1_hw_ustmr_timetick_at_ols_local;
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

#ifdef FEATURE_WCDMA
/*===========================================================================

FUNCTION gl1_hw_sleep_wcdma_active

DESCRIPTION
     Notification that the WCDMA access stratum is active, so that GL1
     can ensure that it does not try and maintain their sleep timeline

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_wcdma_active(const boolean is_active, gas_id_t gas_id)
{
#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW)
  if (is_active)
  {
    maintain_wcdma_ref_count[gas_id] = FALSE;
  }
#endif /* FEATURE_INTERRAT_HANDOVER_GTOW || FEATURE_INTERRAT_CELL_RESELECTION_GTOW */

  gl1_hw_sleep_cm_active[gas_id] = is_active;
}
#endif /* FEATURE_WCDMA */

/*===========================================================================

FUNCTION gl1_hw_sleep_set_any_pending_deferred_wmgr_event
         /gl1_hw_sleep_clear_any_pending_deferred_wmgr_event

DESCRIPTION
  The following 3 functions are to set/get/clear the gl1_hw_sleep_any_pending_deferred_wmgr_event flag

DEPENDENCIES
  None

RETURN VALUE
  TRUE if allowed or else FALSE

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_set_any_pending_deferred_wmgr_event(gas_id_t gas_id)
{
  gl1_hw_sleep_any_pending_deferred_wmgr_event[gas_id] = TRUE;
}

void gl1_hw_sleep_clear_any_pending_deferred_wmgr_event(gas_id_t gas_id)
{
  gl1_hw_sleep_any_pending_deferred_wmgr_event[gas_id] = FALSE;
}

/*===========================================================================

FUNCTION gl1_hw_sleep_wup_cancel_reprogram_allowed

DESCRIPTION
  This function checks if the SLPC reprogram is allowed

DEPENDENCIES
  None

RETURN VALUE
  TRUE if allowed or else FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_sleep_wup_cancel_reprogram_allowed(gas_id_t gas_id)
{
  boolean ret_val = TRUE; /* T => okay to proceed with cancellation */
  uint64  wup_tick = slpc_get_wakeup_tick( gl1_hw_slpc_from_gas_id( gas_id ) );
  uint64  cur_tick = slpc_get_tick();
  int64  gap_to_wup =  (int64) (wup_tick - cur_tick);
  trm_reason_enum_t cancelled_activity_trm_reason;

  /* 1. Get TRM cancellation reason */
  cancelled_activity_trm_reason = grm_get_reason( gl1_map_gas_id_to_client_id(gas_id), gas_id);

  /* 2. Exit if TRM cancellation reason is different since cancellation was pended */
  if( cancelled_activity_trm_reason != gl1_hw_sleep_wmgr_cancel_trm_reason[gas_id] )
  {
    /* TRM cancellation reason has changed since cancellation, so exit */
    MSG_GERAN_HIGH_2_G("WMGR_SLEEP: WARN: Cancellation reason changed, so exiting. prev trm_reason: %d, new trm_reason: %d",
      gl1_hw_sleep_wmgr_cancel_trm_reason[gas_id], cancelled_activity_trm_reason);

    /* store error code */
    gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ERR_TRM_REASON_CHNGD;

    /* failed */
    ret_val = FALSE;
  }

  /* 3. Sleep cannot be reprogrammed if sleep state is not GL1_HW_SLEEP_ASLEEP */
  if(gl1_hw_sleep_state[gas_id] != GL1_HW_SLEEP_ASLEEP)
  {
    /* store error code */
    gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ERR_NOT_IN_ASLEEP;

    /* failed */
    ret_val = FALSE;
  }

  /* 4. Cannot proceed if sleep duration is less than 4 frames */
  if(sleep_duration_frames[gas_id] <= GL1_SLEEP_WUP_CANCEL_MIN_SLEEP_FRAMES)
  {
    /* store error code */
    gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ERR_INSUF_SLEEP_DUR;

    /* failed */
    ret_val = FALSE;
  }

  /* 5. Cannot proceed if gap to wake up time is less than 4 frames */
  if(gap_to_wup <= GL1_SLEEP_MIN_GAP_TO_WUP_CANCEL)
  {
    /* store error code */
    gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ERR_SLPC_INSUF_GAP;

    MSG_GERAN_MED_4_G("WMGR_SLEEP: WARN: Insuf gap: gap_to_wup: %ld, reqd: %ld, wup_tick: %ld, cur_tick: %ld",
      gap_to_wup, GL1_SLEEP_MIN_GAP_TO_WUP_CANCEL, wup_tick, cur_tick);

    /* failed */
    ret_val = FALSE;
  }        

  MSG_GERAN_MED_5_G("WMGR_SLEEP: SLPC reprog allowed: %d, gl1_hw_sleep_state: %d, sleep_duration_frames: %d, gap_to_wup: %ld, cur_trm_reason: %d",
    ret_val, gl1_hw_sleep_state[gas_id], sleep_duration_frames[gas_id], gap_to_wup, cancelled_activity_trm_reason);

  return ret_val;
}


/*===========================================================================

FUNCTION gl1_hw_sleep_reprogram_slpc

DESCRIPTION
  This function only reprograms SLPC during WMGR event handling.

DEPENDENCIES
  None

RETURN VALUE
  Returns TRUE if successful or else FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_sleep_reprogram_slpc(uint32 num_frames, gas_id_t gas_id)
{
   slpc_err_type slpc_err;
   slpc_id_type  slpc_from_gas = gl1_hw_slpc_from_gas_id( gas_id );
   uint32 sleep_duration_frames_reprog;
   uint32 sleep_duration_gsm_clks_reprog;

   MSG_GERAN_LOW_2_G( "WMGR_SLEEP: reprogram_slpc sleep_duration_frames: %d, extend with num_frames: %d", sleep_duration_frames[gas_id], num_frames );

   sleep_duration_frames_reprog   = sleep_duration_frames[gas_id]   + num_frames;
   sleep_duration_gsm_clks_reprog = sleep_duration_gsm_clks[gas_id] + ( NUM_GSM_CLKS_PER_FRAME * num_frames );

   slpc_err = slpc_set_warmup( slpc_from_gas, gl1_hw_sleep_warmup_time_us[gas_id] );
   if (slpc_err != SLPC_ERR_NO_ERR)
   {
      MSG_GERAN_ERROR_1_G( "WMGR_SLEEP: SLPC ERROR return for slpc_set_warmup %d", slpc_err );
      return FALSE;
   }

   slpc_err = slpc_set_duration( slpc_from_gas, sleep_duration_gsm_clks_reprog);
   if (slpc_err != SLPC_ERR_NO_ERR)
   {
      MSG_GERAN_ERROR_1_G( "WMGR_SLEEP: SLPC ERROR return for slpc_set_duration %d", slpc_err );

      return FALSE;  /* Reprogram attempt unsuccessful. */
   }

   MSG_GERAN_LOW_5_G("WMGR_SLEEP: Reprog SLPC: warmup_time_us: %d, prev slp_dur_frames: %d, new slp_dur_frames: %d, prev slp_dur_sclks: %d, new slp_dur_sclks: %d",
      gl1_hw_sleep_warmup_time_us[gas_id], 
      sleep_duration_frames[gas_id],
      sleep_duration_frames_reprog,
      sleep_duration_gsm_clks[gas_id],
      sleep_duration_gsm_clks_reprog);

   /* SLPC reprogramming successful, so update the vars */
   sleep_duration_frames[gas_id]     = sleep_duration_frames_reprog;
   sleep_duration_gsm_clks[gas_id]   = sleep_duration_gsm_clks_reprog;

   /* update sw FN */
   GSTMR_ADVANCE_FN_GERAN(num_frames, gas_id);
  
   /* update hw FN */
   gl1_hw_frame_number_mdsp_wakeup[gas_id] = (gl1_hw_frame_number_gts[gas_id] + sleep_duration_frames[gas_id])&(GERAN_VSTMR_MAX_FN - 1);
   gl1_hw_frame_number_ols[gas_id] = (gl1_hw_frame_number_mdsp_wakeup[gas_id] + 1)&(GERAN_VSTMR_MAX_FN - 1);

   return TRUE;
}

/*===========================================================================

FUNCTION gl1_hw_handle_wakeup_conflict

DESCRIPTION
  Handles wake up conflict.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_handle_wakeup_conflict(gas_id_t gas_id)
{
  uint32  prev_wkpt_fn;
  uint32 gap_to_activity;
  boolean slpc_reprog_successful = FALSE;
  boolean cancelled_activity_is_pch;
  boolean prev_wmgr_cancel_in_prog = gl1_hw_sleep_wmgr_cancel_in_prog[gas_id];
  uint32 next_activity_fn;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  /* store prev wkup pt for printing */
  prev_wkpt_fn = gl1_hw_wkupmgr_wakeup_point_fn[gas_id];

  MSG_GERAN_MED_3_G("WMGR_SLEEP: handle_wakeup_conflict: prev_wkpt_fn: %d, prev cancel_in_prog: %d, trm_reason: %d",
    prev_wkpt_fn,
    gl1_hw_sleep_wmgr_cancel_in_prog[gas_id],
    gl1_hw_sleep_wmgr_cancel_trm_reason[gas_id]);


  /* 1. Check if cancelled activity is PCH */
  if( (gl1_hw_sleep_wmgr_cancel_trm_reason[gas_id] == TRM_DEMOD_PAGE) &&
      (l1_tskisr_blk->reservation_for_bcch == FALSE) )
  {
    /* cancelled activity is PCH */
    cancelled_activity_is_pch = TRUE;
  }
  else
  {
    /* cancelled activity is not PCH */
    cancelled_activity_is_pch = FALSE;
  }

  /* 2. Stop L1 from accessing VSTMR */
  gl1_hw_sleep_wmgr_cancel_in_prog[gas_id] = TRUE;

  /* 3. Frame manager stuff, cancel current CCCH, book next CCCH, 
   *    finding gap to next activity
   */
  gap_to_activity = l1_fm_wmgr_rebook_activity(gas_id);                             /* WMGR_CORE */

  /* 4. Reprog possible only if gap > 0 */
  if(gap_to_activity > 0)
  {
    /* 5. Reprog SLPC */
    slpc_reprog_successful = gl1_hw_sleep_reprogram_slpc(gap_to_activity, gas_id);  /* WMGR_CORE */

    if(slpc_reprog_successful)
    {
      /* Reprog SLPC was successful */

      /* 6. Find FN for the next activity which has been reserved by us */
      next_activity_fn  = l1_fm_get_trm_reserved_fn(gas_id);

      MSG_GERAN_MED_2_G("WMGR_SLEEP: After SLPC Prog: gap_to_activity is: %d, reprog_fn: %d",
        gap_to_activity, next_activity_fn);

      /* 7. Check if initial programmed Wakeup FN is close to CCCH booking,
       *    TRM reason is TRM_DEMOD_PAGE then set the unknown block indication
       */
      if( cancelled_activity_is_pch == TRUE )
      {
        gl1_hw_sleep_wmgr_missed_PCH_time[gas_id] = gl1_hw_sleep_wmgr_trm_when_cur[gas_id] + gl1_hw_sleep_warmup_dur_in_sclks(gas_id);
        MSG_GERAN_MED_0_G("Missed xCCH indication will be sent for the PCH that we just cancelled");
        L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND(gas_id);
      }

      /* 8. Indicates that wkup cancellation has happened */
      gl1_hw_sleep_wmgr_cancel_after_prog[gas_id] = TRUE;

      /* 9. WMGR event has been handled now. Update to new wkup pt */
      gl1_hw_wkupmgr_wakeup_point_fn[gas_id] = next_activity_fn;
    }
    else
    {
      MSG_GERAN_HIGH_3_G("WMGR_SLEEP: Reprog failed: gap_to_activity: %d, SLPC reprog status: %d, trm_reason: %d",
        gap_to_activity, slpc_reprog_successful, gl1_hw_sleep_wmgr_cancel_trm_reason[gas_id]);

      /* restore flag if wkup cancellation was unsuccessful */
      gl1_hw_sleep_wmgr_cancel_in_prog[gas_id] = prev_wmgr_cancel_in_prog;

      /* store error code */
      gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ERR_SLPC_REPROG_FAILED;
    }
  }
  else
  {
    /* gl1_hw_sleep_wmgr_cancel_in_prog[gas_id] = FALSE; */
    MSG_GERAN_HIGH_3_G("WMGR_SLEEP: Reprog failed: gap_to_activity: %d, SLPC reprog status: %d, trm_reason: %d",
      gap_to_activity, slpc_reprog_successful, gl1_hw_sleep_wmgr_cancel_trm_reason[gas_id]);

    /* restore flag if wkup cancellation was unsuccessful */
    gl1_hw_sleep_wmgr_cancel_in_prog[gas_id] = prev_wmgr_cancel_in_prog;
  }
}

/*===========================================================================

FUNCTION gl1_hw_sleep_handle_deferred_wakeup_conflict

DESCRIPTION
  Handles deferred wake up conflict. If the sleep is not programmed already, then any wake up cancellation
  should be deferred.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_handle_deferred_wakeup_conflict(gas_id_t gas_id)
{
  /* Acquire Mutex */
  MSG_GERAN_MED_0_G( "WMGR_SLEEP: ML handle_defer");
  gl1_hw_sleep_wmgr_mutex_lock(gas_id);

  if ( gl1_msg_get_multi_sim_standby_mode() && 
    gl1_hw_sleep_any_pending_deferred_wmgr_event[gas_id] )
  {
    /* run the state machine */
    gl1_hw_sleep_wgmr_sm(gas_id);
  }

  /* Release Mutex */
  MSG_GERAN_MED_0_G( "WMGR_SLEEP: MU handle_defer");
  gl1_hw_sleep_wmgr_mutex_unlock(gas_id);
}

/*===========================================================================

FUNCTION gl1_hw_sleep_wgmr_sm

DESCRIPTION
  Top level handler for WMGR_RESULT_WAKEUP_CANCEL event which checks if wakeup
  can be defered and performs the wakeup cancellation.
  It is called when state is GL1_HW_SLEEP_ASLEEP.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_wgmr_sm(gas_id_t gas_id)
{
  uint32  prev_wkpt_fn;
  boolean wkup_cancel_reprog_allowed;
  boolean wkup_cancel_successful;
  int32   gap;

  /* Initialise with no error */
  gl1_hw_sleep_wmgr_result[gas_id] = GL1_HW_SLEEP_WMGR_RES_NO_ERROR;

  /* store prev wkup pt for printing */
  prev_wkpt_fn = gl1_hw_wkupmgr_wakeup_point_fn[gas_id];

  MSG_GERAN_MED_3_G("WMGR_SLEEP: SM_START: cancel_in_prog: %d, event pending: %d, prev_wkpt_fn: %d",
    gl1_hw_sleep_wmgr_cancel_in_prog[gas_id],
    gl1_hw_sleep_any_pending_deferred_wmgr_event[gas_id],
    prev_wkpt_fn);

  /* Check several conditions to know if cancellation can proceed.
   * eg: sleep status to be ASLEEP, and TRM reason to be same as when cancellation,
   * current wakeup point is not very near, etc
   */
  wkup_cancel_reprog_allowed = gl1_hw_sleep_wup_cancel_reprogram_allowed(gas_id);   /* WMGR_CORE */
  if ( wkup_cancel_reprog_allowed )
  {
    /* Ok to proceed: So handle the wakeup conflict */ 
    gl1_hw_sleep_handle_wakeup_conflict(gas_id);                                    /* WMGR_CORE */
  }

  /* Clear the wakeup cancel flags as it has been handled now */
  gl1_hw_sleep_clear_any_pending_deferred_wmgr_event(gas_id);

  /* Find if wkup cancellation was successful */
  wkup_cancel_successful = ((gl1_hw_sleep_wmgr_result[gas_id] & GL1_HW_SLEEP_WMGR_RES_ERR_MASK) == GL1_HW_SLEEP_WMGR_RES_NO_ERROR) &&
    (prev_wkpt_fn != gl1_hw_wkupmgr_wakeup_point_fn[gas_id]);

  /* Find gap used by wkup cancellation */
  gap = SUB_FN(gl1_hw_wkupmgr_wakeup_point_fn[gas_id], prev_wkpt_fn);

  /* Print message showing Wmgr was successful or it failed */
  if( wkup_cancel_successful )
  {
    /* reprog successful */
    MSG_GERAN_MED_4_G("WMGR_SLEEP: SM_END: WkupCan Passed: 0x%08x, prev_wkpt_fn: %d, new_wkpt_fn: %d, gap: %d",
      gl1_hw_sleep_wmgr_result[gas_id],
      prev_wkpt_fn,
      gl1_hw_wkupmgr_wakeup_point_fn[gas_id],
      gap);
  }
  else
  {
    /* reprog failed */
    MSG_GERAN_HIGH_4_G("WMGR_SLEEP: SM_END: WkupCan Failed: 0x%08x, prev_wkpt_fn: %d, new_wkpt_fn: %d, gap: %d",
      gl1_hw_sleep_wmgr_result[gas_id],
      prev_wkpt_fn,
      gl1_hw_wkupmgr_wakeup_point_fn[gas_id],
      gap);
  }

  {
    /* Update WMGR debug structure */
    gl1_hw_sleep_wgmr_dbg_type *p_gl1_hw_sleep_wgmr_dbg;
    uint16 l1_hw_sleep_wgmr_dbg_cnt;

    /* update gl1_hw_sleep_wgmr_dbg_cnt */
    l1_hw_sleep_wgmr_dbg_cnt = ADD_MOD(gl1_hw_sleep_wgmr_dbg_cnt[gas_id], 1, NUM_OF_GL1_DEBUG_CYCLES);
    gl1_hw_sleep_wgmr_dbg_cnt[gas_id] = l1_hw_sleep_wgmr_dbg_cnt;
    p_gl1_hw_sleep_wgmr_dbg = &gl1_hw_sleep_wgmr_dbg[gas_id][l1_hw_sleep_wgmr_dbg_cnt];

    /* update debug structure */
    p_gl1_hw_sleep_wgmr_dbg->cancel_successful = wkup_cancel_successful;
    p_gl1_hw_sleep_wgmr_dbg->gap = gap;
    p_gl1_hw_sleep_wgmr_dbg->prev_wkpt_fn = prev_wkpt_fn;
    p_gl1_hw_sleep_wgmr_dbg->new_wkpt_fn = gl1_hw_wkupmgr_wakeup_point_fn[gas_id];
    p_gl1_hw_sleep_wgmr_dbg->wmgr_result_code = gl1_hw_sleep_wmgr_result[gas_id];
  }

}

/*===========================================================================

FUNCTION gl1_hw_sleep_wake_up_manager_cb_handler

DESCRIPTION
  This function is the call back we register with wakeup manager module and this
  function is called by wakeup manager module during
  TRM reserve procedure (not called in single sim)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_wake_up_manager_cb_handler( wmgr_client_enum_t client,
                                              wmgr_result_type result,
                                              uint32 transition,
                                              uint32 reserved,
                                              void *user_data )
{
  gas_id_t gas_id = check_gas_id(gl1_hw_map_wmgr_client_to_gas_id( client ));

  /* FR19329: Disable wakeup cancellation if bit is set in l1_sleep */
  if( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_WKUP_CANCEL ) )
  {
    MSG_GERAN_MED_0("WMGR_SLEEP: ignore wup mngr cb");
    return;
  }

  /* If not in DSDS/TSTS mode or if not in idle or any other subs going active */
  if(
    ( l1_tsk_buffer[gas_id].l1_state != L1_MULTI_SIM_IDLE )
    || !gl1_msg_get_multi_sim_standby_mode() )
  {
    MSG_GERAN_MED_2_G("WMGR_SLEEP: Ignoring WMGR event - %d, curr_sub_l1_state - %d",
    result, l1_tsk_buffer[gas_id].l1_state);
    return;
  }

  if(result == WMGR_RESULT_WAKEUP_CANCEL)
  {
    if (!rexl4_queue_dpc((rex_apc_func_type)gl1_hw_sleep_wake_up_manager_cb_handler_dpc, gas_id))
    {
      /* Queuing to DPC failed */
      MSG_GERAN_ERROR_0_G("WMGR_SLEEP: Failed to queue wmgr canc cb to DPC");
    }
  }
  else
  {
    MSG_GERAN_LOW_1_G("WMGR_SLEEP: WMGR event (non cancellable) - %d", result);
  }
}

/*===========================================================================

FUNCTION gl1_hw_sleep_wake_up_manager_cb_handler_dpc

DESCRIPTION
  DPC to do GRM/TRM reservation/query procedure (cannot be done from callback)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_hw_sleep_wake_up_manager_cb_handler_dpc( gas_id_t gas_id )
{
  trm_reason_enum_t cur_activity_trm_reason;

  MSG_GERAN_MED_0_G("WMGR_SLEEP: wmgr_cb dpc WMGR_RESULT_WAKEUP_CANCEL");

  GDRV_ISR_LOCK(gas_id);

  /* Acquire Mutex */
  MSG_GERAN_MED_0_G( "WMGR_SLEEP: ML wmgr_dpc");
  gl1_hw_sleep_wmgr_mutex_lock(gas_id);

  /* find the current trm reason */
  cur_activity_trm_reason = grm_get_reason( gl1_map_gas_id_to_client_id(gas_id), gas_id);

  if(!gl1_hw_sleep_any_pending_deferred_wmgr_event[gas_id])
  {
    /* normal case: wkup cancel not already pending / ongoing */

    /* can continue with wkup cancellation */
    gl1_hw_sleep_wmgr_cancel_trm_reason[gas_id] = cur_activity_trm_reason;

    /* handle it now if we are in asleep */
    if(gl1_hw_sleep_state[gas_id] ==  GL1_HW_SLEEP_ASLEEP)
    {
      gl1_hw_sleep_wgmr_sm(gas_id);
    }
    else
    {
      /* wkup cancel cannot be handled now, do it later when we are in ASLEEP mode */
      MSG_GERAN_MED_3_G("WMGR_SLEEP: WMGR event deferred. trm_reason: %d, sleep_state: %d, l1_state: %d",
        cur_activity_trm_reason, gl1_hw_sleep_state[gas_id], l1_tsk_buffer[gas_id].l1_state);

      /* set it to pending / ongoing */
      gl1_hw_sleep_set_any_pending_deferred_wmgr_event(gas_id);
    }
  }
  else
  {
    /* received another wakeup_cancel while first one was getting executed */
    MSG_GERAN_MED_2_G("WMGR_SLEEP: WMGR already pending, ignored. prev_trm_reason: %d, new_trm_reason: %d",
      gl1_hw_sleep_wmgr_cancel_trm_reason[gas_id], cur_activity_trm_reason);
  }

  /* Relinquish Mutex */
  MSG_GERAN_MED_0_G( "WMGR_SLEEP: MU wmgr_dpc");
  gl1_hw_sleep_wmgr_mutex_unlock(gas_id);

  GDRV_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION gl1_hw_sleep_wmgr_mutex_lock

DESCRIPTION
  WMGR mutex lock.
  To avoid deadloc: 
    To be called after ISR lock has been done.
    TRM lock should be done only after this WMGR mutex lock.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_wmgr_mutex_lock(gas_id_t gas_id)
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  gas_id = check_gas_id(gas_id);
  rex_enter_crit_sect(&gl1_hw_sleep_wakeup_cancel_crit_sect[gas_id]);
  MSG_GERAN_LOW_0_G( "WMGR_SLEEP: ML");
#endif /* DUAL_SIM/TRIPLE_SIM*/
}



/*===========================================================================

FUNCTION gl1_hw_sleep_wmgr_mutex_lock

DESCRIPTION
  WMGR mutex lock.
  To avoid deadloc: 
    To be called after ISR lock has been done.
    TRM lock should be done only after this WMGR mutex lock.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_wmgr_mutex_unlock(gas_id_t gas_id)
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  gas_id = check_gas_id(gas_id);
  rex_leave_crit_sect(&gl1_hw_sleep_wakeup_cancel_crit_sect[gas_id]);
  MSG_GERAN_LOW_0_G( "WMGR_SLEEP: MU");
#endif /* DUAL_SIM/TRIPLE_SIM*/
}

/*===========================================================================

FUNCTION  gl1_hw_sleep_wmgr_calc_grm_when_time

DESCRIPTION
  This function determines the required TRM request period in sclks, during WMGR cancelation,
  factoring in the time during the frame at which this is called.

DEPENDENCIES
  None

RETURN VALUE
  TRM period request in sclcks.

SIDE EFFECTS
  None
===========================================================================*/
trm_time_t gl1_hw_sleep_wmgr_calc_grm_when_time(uint32 requested_FN, gas_id_t gas_id)
{
  /* used when wakeup cancellation is in progress */
  int32 gap_to_trm_num_frames;
  int32 gap_to_trm_sclk;
  boolean new_trm_is_after_gts_wkup_pt;
  trm_time_t trm_when;

  if(gl1_hw_sleep_wmgr_trm_when_at_gts_valid[gas_id])
  {
    gap_to_trm_num_frames = SUB_FN(requested_FN, gl1_hw_sleep_wmgr_trm_res_fn_at_gts[gas_id]);

    if( gap_to_trm_num_frames >= 0)
    {
      new_trm_is_after_gts_wkup_pt = TRUE;
    }
    else
    {
      new_trm_is_after_gts_wkup_pt = FALSE;
    
      /* remove negative sign */
      gap_to_trm_num_frames = -gap_to_trm_num_frames;
    }

    gap_to_trm_sclk = gpl1_multi_sim_convert_frames_to_sclcks(gap_to_trm_num_frames);
    
    if(new_trm_is_after_gts_wkup_pt == FALSE)
    {
      /* add negative sign back again */
      gap_to_trm_sclk = -gap_to_trm_sclk;
    }

    trm_when = gl1_hw_sleep_wmgr_trm_when_at_gts[gas_id] + gap_to_trm_sclk;
  }
  else
  {
    int32 warmup_dur_in_sclks;
    
    warmup_dur_in_sclks = gl1_hw_sleep_warmup_dur_in_sclks(gas_id);

    trm_when = timetick_get_safe()
      + gpl1_multi_sim_calculate_grm_reservation_time( requested_FN, gas_id )
      - warmup_dur_in_sclks;

    MSG_GERAN_MED_2_G("WMGR_SLEEP: gts invalid, trm_fn: %d, when: %d", requested_FN, trm_when);
  }

  return trm_when;
}

/*===========================================================================

FUNCTION GL1_HW_SET_QTA_ACTIVE

DESCRIPTION
  Sets QTA_TRAFFIC flag to status at wake up from sleep.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - wake up in QTA gap or FALSE - normal wake up

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_set_qta_active( gas_id_t gas_id, boolean qta_active )
{
#ifdef FEATURE_SLEEP_FROM_QTA
  MSG_GERAN_HIGH_1_G(GL1_SLEEP_HDR" Set qta_active %d", qta_active );
  gl1_hw_sleep_qta_active[gas_id] = qta_active;
#else
  MSG_GERAN_HIGH_1_G(GL1_SLEEP_HDR" qta_active is FALSE ", qta_active );
  gl1_hw_sleep_qta_active[gas_id] = FALSE;
#endif /* FEATURE_SLEEP_FROM_QTA  */
}

/*===========================================================================

FUNCTION GL1_HW_GET_QTA_ACTIVE

DESCRIPTION
  Returns setting of QTA_TRAFFIC flag.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE or FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_sleep_get_qta_active( gas_id_t gas_id )
{
  MSG_GERAN_LOW_1_G(GL1_SLEEP_HDR" Get qta_active %d", gl1_hw_sleep_qta_active[gas_id] );
  return (gl1_hw_sleep_qta_active[gas_id]);
}


#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
/*===========================================================================

FUNCTION QDSS_logging

DESCRIPTION
   This function helps to log QDSS events

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void QDSS_logging( gas_id_t gas_id,tracer_event_id_t event_id )
{
   if (gl1_is_qdss_logging_enabled(gas_id))
   {
    #if defined( FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM )
        if(gas_id == GERAN_ACCESS_STRATUM_ID_1)
            {
            tracer_event_simple(event_id);
            }
        else if(gas_id == GERAN_ACCESS_STRATUM_ID_2)
            {
            switch(event_id)
                {
                case GERAN_CUSTOM_EVT_1_1:
                    tracer_event_simple(GERAN_CUSTOM_EVT_1_2);
                break;
                case GERAN_CUSTOM_EVT_2_1:
                    tracer_event_simple(GERAN_CUSTOM_EVT_2_2);
                break;
                case GERAN_CUSTOM_EVT_3_1:
                    tracer_event_simple(GERAN_CUSTOM_EVT_3_2);
                break;
                case GERAN_SLEEP_GOING_TO_SLEEP_1:
                    tracer_event_simple(GERAN_SLEEP_GOING_TO_SLEEP_2);
                break;
                case GERAN_SLEEP_RF_RX_OFF_START_1:
                    tracer_event_simple(GERAN_SLEEP_RF_RX_OFF_START_2);
                break;
                case GERAN_SLEEP_RF_RX_OFF_END_1:
                    tracer_event_simple(GERAN_SLEEP_RF_RX_OFF_END_2);
                break;
                case GERAN_SLEEP_RF_VREG_OFF_START_1:
                    tracer_event_simple(GERAN_SLEEP_RF_VREG_OFF_START_2);
                break;
                case GERAN_SLEEP_RF_VREG_OFF_END_1:
                    tracer_event_simple(GERAN_SLEEP_RF_VREG_OFF_END_2);
                break;
                case GERAN_SLEEP_CONFIG_DONE_1:
                    tracer_event_simple(GERAN_SLEEP_CONFIG_DONE_2);
                break;
                case GERAN_SLEEP_MCPM_START_1:
                    tracer_event_simple(GERAN_SLEEP_MCPM_START_2);
                break;
                case GERAN_SLEEP_MCPM_END_1:
                    tracer_event_simple(GERAN_SLEEP_MCPM_END_2);
                break;
                case GERAN_SLEEP_RECOV_WAKEUP_START_1:
                    tracer_event_simple(GERAN_SLEEP_RECOV_WAKEUP_START_2);
                break;
                case GERAN_SLEEP_RECOV_WAKEUP_END_1:
                    tracer_event_simple(GERAN_SLEEP_RECOV_WAKEUP_END_2);
                break;
                case GERAN_SLEEP_WAKEUP_START_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_START_2);
                break;
                case GERAN_SLEEP_MCPM_WAKE_UP_REQ_1:
                    tracer_event_simple(GERAN_SLEEP_MCPM_WAKE_UP_REQ_2);
                break;
                case GERAN_SLEEP_MCPM_WAKE_UP_REQ_END_1:
                    tracer_event_simple(GERAN_SLEEP_MCPM_WAKE_UP_REQ_END_2);
                break;
                case GERAN_SLEEP_WAKEUP_RF_VREG_ON_START_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_RF_VREG_ON_START_2);
                break;
                case GERAN_SLEEP_WAKEUP_RF_VREG_ON_END_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_RF_VREG_ON_END_2);
                break;
                case GERAN_SLEEP_WAKEUP_RF_RX_ON_START_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_RF_RX_ON_START_2);
                break;
                case GERAN_SLEEP_WAKEUP_RF_RX_ON_END_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_RF_RX_ON_END_2);
                break;
                case GERAN_SLEEP_WAKEUP_GSTMR_ON_START_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_GSTMR_ON_START_2);
                break;
                case GERAN_SLEEP_WAKEUP_GSTMR_ON_END_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_GSTMR_ON_END_2);
                break;
                case GERAN_SLEEP_WAKEUP_END_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_END_2);
                break;
                case GERAN_SLEEP_OLS_START_1:
                    tracer_event_simple(GERAN_SLEEP_OLS_START_2);
                break;
                case GERAN_SLEEP_WAKEUP_OLS_END_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_OLS_END_2);
                break;
                case GERAN_DRX_GSTMR2_START_1:
                    tracer_event_simple(GERAN_DRX_GSTMR2_START_2);
                break;
                case GERAN_DRX_GSTMR1_START_1:
                    tracer_event_simple(GERAN_DRX_GSTMR1_START_2);
                break;
                default: break;
            }
            }
#if defined ( FEATURE_TRIPLE_SIM )
        else if(gas_id == GERAN_ACCESS_STRATUM_ID_3)
            {
            switch(event_id)
                {
                case GERAN_CUSTOM_EVT_1_1:
                    tracer_event_simple(GERAN_CUSTOM_EVT_1_3);
                break;
                case GERAN_CUSTOM_EVT_2_1:
                    tracer_event_simple(GERAN_CUSTOM_EVT_2_3);
                break;
                case GERAN_CUSTOM_EVT_3_1:
                    tracer_event_simple(GERAN_CUSTOM_EVT_3_3);
                break;
                case GERAN_SLEEP_GOING_TO_SLEEP_1:
                    tracer_event_simple(GERAN_SLEEP_GOING_TO_SLEEP_3);
                break;
                case GERAN_SLEEP_RF_RX_OFF_START_1:
                    tracer_event_simple(GERAN_SLEEP_RF_RX_OFF_START_3);
                break;
                case GERAN_SLEEP_RF_RX_OFF_END_1:
                    tracer_event_simple(GERAN_SLEEP_RF_RX_OFF_END_3);
                break;
                case GERAN_SLEEP_RF_VREG_OFF_START_1:
                    tracer_event_simple(GERAN_SLEEP_RF_VREG_OFF_START_3);
                break;
                case GERAN_SLEEP_RF_VREG_OFF_END_1:
                    tracer_event_simple(GERAN_SLEEP_RF_VREG_OFF_END_3);
                break;
                case GERAN_SLEEP_CONFIG_DONE_1:
                    tracer_event_simple(GERAN_SLEEP_CONFIG_DONE_3);
                break;
                case GERAN_SLEEP_MCPM_START_1:
                    tracer_event_simple(GERAN_SLEEP_MCPM_START_3);
                break;
                case GERAN_SLEEP_MCPM_END_1:
                    tracer_event_simple(GERAN_SLEEP_MCPM_END_3);
                break;
                case GERAN_SLEEP_RECOV_WAKEUP_START_1:
                    tracer_event_simple(GERAN_SLEEP_RECOV_WAKEUP_START_3);
                break;
                case GERAN_SLEEP_RECOV_WAKEUP_END_1:
                    tracer_event_simple(GERAN_SLEEP_RECOV_WAKEUP_END_3);
                break;
                case GERAN_SLEEP_WAKEUP_START_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_START_3);
                break;
                case GERAN_SLEEP_MCPM_WAKE_UP_REQ_1:
                    tracer_event_simple(GERAN_SLEEP_MCPM_WAKE_UP_REQ_3);
                break;
                case GERAN_SLEEP_MCPM_WAKE_UP_REQ_END_1:
                    tracer_event_simple(GERAN_SLEEP_MCPM_WAKE_UP_REQ_END_3);
                break;
                case GERAN_SLEEP_WAKEUP_RF_VREG_ON_START_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_RF_VREG_ON_START_3);
                break;
                case GERAN_SLEEP_WAKEUP_RF_VREG_ON_END_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_RF_VREG_ON_END_3);
                break;
                case GERAN_SLEEP_WAKEUP_RF_RX_ON_START_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_RF_RX_ON_START_3);
                break;
                case GERAN_SLEEP_WAKEUP_RF_RX_ON_END_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_RF_RX_ON_END_3);
                break;
                case GERAN_SLEEP_WAKEUP_GSTMR_ON_START_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_GSTMR_ON_START_3);
                break;
                case GERAN_SLEEP_WAKEUP_GSTMR_ON_END_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_GSTMR_ON_END_3);
                break;
                case GERAN_SLEEP_WAKEUP_END_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_END_3);
                break;
                case GERAN_SLEEP_OLS_START_1:
                    tracer_event_simple(GERAN_SLEEP_OLS_START_3);
                break;
                case GERAN_SLEEP_WAKEUP_OLS_END_1:
                    tracer_event_simple(GERAN_SLEEP_WAKEUP_OLS_END_3);
                break;
                case GERAN_DRX_GSTMR2_START_1:
                    tracer_event_simple(GERAN_DRX_GSTMR2_START_3);
                break;
                case GERAN_DRX_GSTMR1_START_1:
                    tracer_event_simple(GERAN_DRX_GSTMR1_START_3);
                break;
                default: break;
        }
            }
#endif /* FEATURE_TRIPLE_SIM */
#else
  tracer_event_simple(event_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
  }  /* if (gl1_is_qdss_logging_enabled(gas_id))  */
}
#endif /* #ifdef FEATURE_GL1_SLEEP_QDSS_LOG */
/*===========================================================================
FUNCTION gl1_hw_get_sleep_state

DESCRIPTION
  Return current gl1_hw_sleep_state
===========================================================================*/
gl1_hw_sleep_state_T gl1_hw_get_sleep_state(gas_id_t gas_id)
{
    return(gl1_hw_sleep_state[gas_id]);
}

/*===========================================================================
FUNCTION l1_rf_result_monitor_timer

DESCRIPTION
  creates/defines the monitor timer for rf confirmation
===========================================================================*/
void gl1_hw_rf_result_monitor_timer_init(gas_id_t gas_id )
{
    /* Create a timer for rf confirmation monitoring  */
    memset(&(RF_cmd_RESULT_counter[gas_id]), 0, sizeof(rex_timer_type));
    rex_def_timer_ex( &RF_cmd_RESULT_counter[gas_id],
                    (rex_timer_cb_type)gl1_hw_timer_sleep_commit,
                    (gas_id_t)gas_id );
    gl1_hw_rf_result_monitor_timer_active[gas_id] = FALSE;
}
/*===========================================================================
FUNCTION gl1_hw_rf_result_monitor_timer_deactivate

DESCRIPTION
  Deactivates the rf monitor confirmation timer
===========================================================================*/
void gl1_hw_rf_result_monitor_timer_deactivate( gas_id_t gas_id)
{
if(gl1_hw_rf_result_monitor_timer_active[gas_id] == TRUE)
 {
    /* Disable the timer */
    (void)rex_clr_timer( &RF_cmd_RESULT_counter[gas_id] );
  gl1_hw_rf_result_monitor_timer_active[gas_id] = FALSE;
  rf_timer_stop = timetick_get_safe();
  MSG_GERAN_HIGH_1_G("Timer stopped at %d",rf_timer_stop);
 }
}

/*===========================================================================
FUNCTION gl1_hw_rf_result_monitor_timer_start

DESCRIPTION
  Starts the rf result monitor  timer for the time period defined by
  time_to_wait_for_rf_cnf
===========================================================================*/
void gl1_hw_rf_result_monitor_timer_start(uint32 time_to_wait_for_rf_cnf, gas_id_t gas_id )
{
  (void)rex_set_timer( &RF_cmd_RESULT_counter[gas_id],
                         time_to_wait_for_rf_cnf );
  gl1_hw_rf_result_monitor_timer_active[gas_id] = TRUE;
  rf_timer_start = timetick_get_safe();
  MSG_GERAN_HIGH_2_G("Timer started at %d, count for %d ms",rf_timer_start, time_to_wait_for_rf_cnf);
}

/*===========================================================================

FUNCTION GL1_HW_TIMER_SLEEP_COMMIT

DESCRIPTION
Commit to wakeup if there is no ISR overrun; trigger the recovery mechanism otherwise
as no RF callback received
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_timer_sleep_commit( gas_id_t gas_id )
{
  boolean execute_err_fatal = FALSE;

  rex_enter_crit_sect(&gl1_hw_sleep_warmup_cb_crit_sect[gas_id]);

  if(gl1_hw_rf_result_monitor_timer_active[gas_id])
  {
    MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" Wake up timer expired, no RF Wake up CNF Received");
    gl1_hw_sleep_commit(gas_id);
  }
  else if(gl1_hw_sleep_state[gas_id] == GL1_HW_SLEEP_AWAKE)
  {
    MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" Wake up timer expired, but we have already commited or timer not properly deactivated");
  }
  else
  {
    MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" Wake up timer expired, but we were not doing wakeup");
    execute_err_fatal = TRUE;
  }

  rex_leave_crit_sect(&gl1_hw_sleep_warmup_cb_crit_sect[gas_id]);

  if(execute_err_fatal)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    /* Execute ERR_FATAL() only after critical section has finished */
    ERR_FATAL("RexTimer error, it was not deactivated properly",0,0,0);
  }
}
/*===========================================================================

FUNCTION gl1_hw_sleep_update_scheduled_activity

DESCRIPTION
  When a error recovery happens we have to update the acitivities already scheduled in the warm up
  ISR. This function will go update the currently scheduled activity.

DEPENDENCIES
  Only to be called from warm up recovery.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_sleep_update_scheduled_activity(gas_id_t gas_id)
{
   /* Only to be run if there is Error recovery condition hit */
   if(!gl1_hw_sleep_get_entered_recovery(gas_id))
   {
     return;
   }

   gl1_update_scheduled_activity(gas_id);
}
/*===========================================================================

FUNCTION gl1_hw_sleep_get_entered_recovery/gl1_hw_sleep_set_entered_recovery

DESCRIPTION
  Get and Set functions for entered recovery

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_sleep_get_entered_recovery(gas_id_t gas_id)
{
  return (entered_warmup_recovery[gas_id]);
}

void gl1_hw_sleep_set_entered_recovery(gas_id_t gas_id, boolean enteredRecovery)
{
  entered_warmup_recovery[gas_id] = enteredRecovery;
}

/*===========================================================================

FUNCTION gl1_hw_get_frame_number_mdsp_wakeup

DESCRIPTION
  Get gl1_hw_frame_number_mdsp_wakeup value

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint16 gl1_hw_get_frame_number_mdsp_wakeup(gas_id_t gas_id)

{
  return (gl1_hw_frame_number_mdsp_wakeup[gas_id]);
}

/*===========================================================================

FUNCTION gl1_hw_get_frame_number_ols

DESCRIPTION
  Get gl1_hw_get_frame_number_ols value

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint16 gl1_hw_get_frame_number_ols(gas_id_t gas_id)

{
  return (gl1_hw_frame_number_ols[gas_id]);
}

 /*===========================================================================

 FUNCTION gl1_hw_msim_sleep_commit

 DESCRIPTION
   This helper function makes a decision of commit to wake up for
   both MSIM and SSSS cases and then calls gl1_hw_sleep_commit.

 DEPENDENCIES
   Only to be called from warm up CB.
   Always runs from SLPC task context.

 RETURN VALUE
   None

 SIDE EFFECTS
   None

 ===========================================================================*/
static void gl1_hw_msim_sleep_commit(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if (!gl1_hw_sleep_get_forcedwakeup(gas_id) && !gl1_hw_sleep_get_qta_active(gas_id))
#else
  if (!gl1_hw_sleep_get_forcedwakeup(gas_id) )
#endif
  {
    /* normal wakeup (not forced/rude wakeup) */
    l1_tskisr_blk->l1_isr_called_from_sleep_warmup_isr = TRUE;

    /* Gstmr: run all the registered handlers*/
    wakeup_gstmr_isr(gas_id);

    /* Call to mark GSTMR_ISR as completed and if all other
       pre-commit checks are present then will commit */
    gl1_hw_sleep_set_commit_check (WAKEUP_GSTMR_ISR_COMMIT, gas_id);

    l1_tskisr_blk->l1_isr_called_from_sleep_warmup_isr = FALSE;
  }
  else
  {
        if (!gl1_hw_sleep_get_forcedwakeup(gas_id))
    {
      /* Need to increment Frame number here as we don't run ISR when BG traffic active */
      GSTMR_INC_FN_BY_ONE(gas_id);
    }
    /* forced/rude wakeup. so commit "now" as we are not waking up GSTMR/RF/RX/TX.
     * Also, we assume that GSTMR should be already awake (as its rude wakeup),
     * and it (GSTMR) will do the RF/RX/TX wakeup at the right time.
     */
     gl1_hw_sleep_commit( gas_id );
  }

  MSG_GERAN_HIGH_2_G (GL1_SLEEP_HDR" VSTMR GSM commit, schedule event, ols vstmr frame%d, current vstmr frame %d",gl1_hw_frame_number_ols[gas_id],vstmr_rd_modMaxFN_frame_num(gas_id));

}

/*===========================================================================

FUNCTION gl1_hw_get_last_sleep_debug_entry

DESCRIPTION
   This function gets the debug info for the most recent wake-up
===========================================================================*/
gl1_hw_sleep_type* gl1_hw_get_last_sleep_debug_entry(gas_id_t gas_id)
{
  uint8 cnt = gl1_sleep_cnt[gas_id];
  if( cnt >= NUM_OF_GL1_DEBUG_CYCLES )
  {
     cnt = 0;
  }

  return &gl1_hw_sleep_struct[gas_id][cnt];
}

 /*===========================================================================

 FUNCTION gl1_hw_save_ccs_write_time_estimate / gl1_hw_get_ccs_write_time_estimate

 DESCRIPTION
   Saves the GRF estimate of the CCS write time for the current RF_AWAKE_REQ.
  
   This is the time GRF estimates is required from the end of RF ENTER MODE to
   the start of GFW beta to allow the CCS writes to the RFFE to finish.
  
   Note: GFW beta is always after the OLS, therfore this estimate can be compared
   to rf_enter_mode_to_ols when making a commit decision.

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None

 ===========================================================================*/
void gl1_hw_save_ccs_write_time_estimate(uint32 ccs_write_time_us, gas_id_t gas_id)
{
  gl1_hw_ccs_write_time_estimate_us[gas_id] = ccs_write_time_us;
}

uint32 gl1_hw_get_ccs_write_time_estimate(gas_id_t gas_id)
{
  return gl1_hw_ccs_write_time_estimate_us[gas_id];
}

 /*===========================================================================

 FUNCTION gl1_hw_save_time_enter_mode_completed
          gl1_hw_get_enter_mode_completed_to_ols_us
          gl1_hw_check_enter_mode_sent_this_wakeup
          gl1_hw_reset_time_enter_mode_completed
 
 DESCRIPTION
   These functions maintain gl1_hw_enter_mode_completed_to_ols_us which 
   is the time from RF ENTER MODE being completed to the OLS. Not all wakeups
   include an RF ENTER MODE, gl1_hw_enter_mode_sent_this_wakeup indicates
   whether current wakeup includes one.

 ===========================================================================*/
void gl1_hw_save_time_enter_mode_completed(gas_id_t gas_id)
{
  gl1_hw_enter_mode_completed_to_ols_us[gas_id] = gl1_get_time_next_vsmtr_us(gas_id);
  gl1_hw_enter_mode_sent_this_wakeup[gas_id]    = TRUE;
}

static int32 gl1_hw_get_enter_mode_completed_to_ols_us(gas_id_t gas_id)
{
  return gl1_hw_enter_mode_completed_to_ols_us[gas_id];
}

static boolean gl1_hw_check_enter_mode_sent_this_wakeup(gas_id_t gas_id)
{
  return gl1_hw_enter_mode_sent_this_wakeup[gas_id];
}

static void gl1_hw_reset_time_enter_mode_completed(gas_id_t gas_id)
{
  gl1_hw_enter_mode_completed_to_ols_us[gas_id] = 0;
  gl1_hw_enter_mode_sent_this_wakeup[gas_id]    = FALSE;
}

/*===========================================================================

FUNCTION gl1_is_sleep_disabled

DESCRIPTION
   Run-time debug flag to disbale sleep

===========================================================================*/
boolean gl1_is_sleep_disabled( gas_id_t gas_id )
{
  return GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_SLEEP );
}

/*===========================================================================

FUNCTION gl1_is_adaptive_timeline_disabled

DESCRIPTION
    Run-time debug flag to disable adaptive timeline

===========================================================================*/
boolean gl1_is_adaptive_timeline_disabled( gas_id_t gas_id )
{
  /* With the adaptive timeline disabled, that sub (or both) should fall back
     to using the static timeline */
  return GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_ADAPTIVE_TIMELINE );
}

/*===========================================================================

FUNCTION gl1_is_qdss_logging_enabled

DESCRIPTION
    Run-time debug flag to enable QDSS logging

===========================================================================*/
boolean gl1_is_qdss_logging_enabled(gas_id_t gas_id)
{
  return GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_QDSS_LOG_ENABLE );
}

/*===========================================================================

FUNCTION gl1_is_sleep_disabled_with_gcf

DESCRIPTION
   This function checks if sleep is disabled with Test Sim.
   With a test SIM inserted sleep is usually disabled. 
   But this can be overridden, and sleep can be enabled with a test sim
   by adding the appropriate EFS file.

===========================================================================*/
boolean gl1_is_sleep_disabled_with_gcf(gas_id_t gas_id)
{
   if( (mcfg_gcf_nv_get_status() == TRUE) && 
      (GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_ENABLE_SLEEP_WITH_GCF ) == TRUE) )
  {
    /* sleep is disabled in this scenario */
    return TRUE;
  }
  else
  {
    /* normal case */
    return FALSE;
  }
}

/*===========================================================================

FUNCTION gl1_hw_sleep_try_fw_sleep

DESCRIPTION
   This function will check if GFW is done processing its tasks.
   If so, it puts GFW to sleep.

   This function is meant to be paired with gl1_hw_sleep_try_fw_wakeup()

===========================================================================*/
boolean gl1_hw_sleep_try_fw_sleep(gas_id_t gas_id)
{
  boolean gfw_asleep = FALSE;

  if(mdsp_awake(gas_id))
  {
    if (mdsp_ready_for_sleep(gas_id))
    {
      gfw_asleep = mdsp_prepare_for_sleep(gas_id);
    }
  }
  else
  {
    gfw_asleep = TRUE;
  }

  return gfw_asleep;
}

/*===========================================================================

FUNCTION gl1_hw_sleep_try_fw_wakeup

DESCRIPTION
   This function will attempt to wakeup GFW.  

   This call is meant to be paired with gl1_hw_sleep_try_fw_sleep().

===========================================================================*/
boolean gl1_hw_sleep_try_fw_wakeup(gas_id_t gas_id)
{
  if(mdsp_awake(gas_id))
  {
    return TRUE;
  }
  else
  {
    return mdsp_wakeup_geran(FALSE, geran_map_gas_id_to_nas_id(gas_id));
  }
}


/* EOF */
