/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             L 1   I S R

GENERAL DESCRIPTION
   This module handles the TDMA frame processing.  It maintains the L1
   state machine and is directed by the L1 task.

EXTERNALIZED FUNCTIONS
  l1_tdma_1_ISR
    Procedure to service the TDMA frame interrupt.

  L1_increment_frame_counters
    Procedure to increment and manage the TDMA frame number.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_isr.c#7 $
  $DateTime: 2024/09/09 06:12:21 $ $Author: pwbldsvc $

when       who       what, where, why
--------   ---       ---------------------------------------------------------
12/02/19   ng        CR2386174 Defer Panic reset until IRAT state machine is clear, remove block in QSH handler
20/02/20   rks      CR2624582 setting frame tick signal in suspend mode.
04/02/20   nv		CR2615257 Reverting the hack made to handle L1_TIME_LINE_DELAY scenario in HA as part of FR:51554
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
09/04/18   rks       CR2012120 Don't move to transfer state without FW loaded after PS RACH TA gap
12/09/17   sn        CR2064615 Remove debug crash condition added as part of packet transfer error recovery mechanism
07/02/18   rc        CR2184167 Sending MAC_L1_RECOVERY_CNF after L1 moved to Idle State.
19/04/18   sp        CR2225702 Revert CR2174600, provide alternative solution.
02/02/18   nsa       CR2174600 Not releasing TRM reservations when BPLMN search is ongoing so that there is enough time when GSM resumes for page.
14/09/17   ng        CR2109695 Do not do panic reset from QSH until G2X state machine is clear
01/02/18   km        CR2157534 Debug crash to root cause G2L INIT stuck
01/02/18   km        CR2093644 Activate SCE when IDLE COMMAND is set in idle mode
12/06/17   rv        CR2058216: Fix for gs1:L1 stuck in FIND_BCCH mode in SSIM.
18/12/17   rc        CR2160237 GBTA Active count check is not valid when l1_idle_non_drx_timer_flag is TRUE.
31/10/17   sn        CR2134426 Race condition of BPLMN search suspension evethough power measurement handlers are active  
12/09/17   sn        CR2103955 Delay the Transition from Transfer to DTM (EDTM) when SC meas in progress
24/01/18   sp        CR2099959 Wait one more frame before processing StopModeReq in WTOG_BPLMN state if it was followed by MPH_RESUME_GSM_MODE_FOR_BPLMN_REQ.
11/09/17   rc       CR2105004 Debug code to check why client_id exchange is not happening in transfer mode init.
24/08/17   sn        CR2097137 GBTA Active count check is not valid when IDLE sub is in MM NON DRX mode.
07/07/06   br        CR1033654 During deactivation(moving from ded to inactive state), sync to time slot 0 before moving to Inactive state.
05/10/16   ng        CR1093500 Irat Init/Deinit Code Change
15/11/16   zf/ng     CR1081607: Stop forced measurement when update cell list
12/08/17   sn        CR2155306 Race condition of not releasing TX chain when moving from Transfer to IDLE
22/04/16   ksb       CR1005869 To mainlne FEATURE_GL1_SLEEP_DEBUG_STRUCT
28/04/16   sk        CR1008819 Force a crash if QTA cleanup is not done on time
08/09/17   sn        CR1061730 FR43082: TA during PS Procedures- GL1 changes
08/09/17   nm        CR2103941 Do not schedule IRAT when in 2 phase access
16/03/17   mn        CR2020307 FR40846:Packet transfer Error Recovery mechanism.
31/08/17   nm        CR2098538 Abort monitors and own PCH/BCCH when transition from Single block to UL TBF
08/08/17   nm        CR2040229 Enable PCH read in 2 phase access for both Idle sub and data sub using GBTA
13/07/16   nm        CR1040020 Fix compilation issues with fEATURE_DUAL_DATA enabled(required for G2L TA FR)
03/11/15   ss        CR916273 FR 29852: AsDiv in Acquisition
21/07/16   sm        CR1044487 Added DTF handler to trigger panic reset.
15/07/16   gmr       CR1039177 Change TRM callback function to accept more requests
02/05/16   mko       CR1010446: Clear the GBTA count after ISR processing and add drx tick to prevent the sleep(for error fatal) if GBTA is still active
26/04/16   sn        CR999729 Sending of Rel confirm received delayed till tuneway.is completed.
29/03/16   bg        CR989643 Set signal GPL1_WAIT_FOR_ISR_TO_GET_FN in NULL_MODE also.
24/03/16   mn        CR954186: Release TBF if pending
22/02/16   sn		 CR971490  Ensure TX exit command will be given when move from PTM to Idle only after DSP is ready for idle
12/01/16   sk        CR960922 Ensure GBTA active frame count is reset at the end of the GBTA gap
11/11/15   smd       CR938347 In SS mode GL1 wait one more frame for RF and FW getting ready 
12/11/15   nm        CR932874 move the startime if FW is not loaded and we are in single block config
27/10/15   sp        CR928056: Dont release TRM based on frame duration inside BPLMN_TRM_WAIT state 
28/09/15   mko       CR914802 While moving from DTM to transfer mode set the GBTA status to ALLOWED before retaining the lock
09/10/15   akh       CR912421 FN/QS not correct for active ttg, sleep ttg resp delayed by 1s
14/09/15   hd        CR904044 Check GBTA state is active for maximum number from frames only in MULTI SIM IDLE state
21/09/15   nm        CR767743 Using actual duration in trm unlock cb for unlock by event
02/09/15   zf        CR905869 Set trm_status to NULL after grm release
08/07/15   sjv       CR868534 Include Max. FCCH detection duration for RnN for X2G BPLMN 
30/07/15   nm        CR872740 Remove unnecessary frames used in WTOG BPLMN state machine
14/08/14   ws        CR889273 Fix Static declarations of variables which are not Dual data spaced
13/08/15   js        CR885927 Reducing resume time overhead to allow more time availability to WCDMA for BPLMN search
12/08/15   ap        CR888551 MultiSim: GERAN is not releasing device to allow call on WCDMA 
21/07/15   cjl       CR876741 Replace TRM subreasons with system procedure types
15/07/15   ap        CR872166 PH_RANDOM_ACCESS_IND not processed by GL1 while in Packet Transfer Mode
17/06/15   cjl       CR853555 Use dynamic connected mode reasons for CS/PS traffic
24/06/15   ap        CR866727 TRM U:support of Acquisition Cell Select as DR and Acquisition as SR 
25/06/15   dg        CR798863 Adjust GSTMR back to its previous value after X2G HO Fail
30/06/15   ws        CR852519 - Resolve GL1 enum conversion compile warnings
30/06/15   cjl       CR863267 Treat GL1_TRM_RETAINED_FOR_ACCESS as GL1_TRM_GRANTED in *_TRM_WAIT states
23/06/15   sp        CR859168: Clear trm_requested_at_warmup at end of l1 isr
18/06/15   cah       CR748477: DR-DSDS: Reduce F3 load
29/05/15   sjv       CR845608 When panic reset is triggered by L1 and is actioned by L1 Task after ISR finishes, Irat SM abort is not handled
10/06/15   cjl       CR851285 Fix RACH from idle state machine during RR procedure lock
01/06/15   ab        CR846442 : GL1 issue GFW_CONFIGURE_IR_WINDOW_CMD in All states.
                     GFW behaviour modified GFW CR:836872
04/06/15   ab        CR817146 : DSDS: GBTA - Apply Generic Mutex Level Protection for GBTA concurrency
21/05/15   cja       CR839948 For QTA gap build preload scripts in QTA startup, send to GFW in cm startup
08/05/15   mko       CR833476 After TA gap ends in RACH mode, make sure that FW is loaded and Tx is enabled before moving to transfer mode
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
30/04/15   ap        CR811057 FR22272 - TRM Unification  - Logging Changes
29/04/15   npt       CR804870 RF timeline error recovery mechanism
28/04/15   mko       CR828316 Enable G to TD tuneaway during Rach process by default
21/04/15   cs        CR808438 Ensure any pending TT requests are sent before cell
                     information is updated
14/04/15   cja       CR822143 Check srch type in addition to srch state for detecting IRAT stuck.
08/04/15   cja       CR800650 Optimise irat state machine
07/04/15   shr       CR808932: COEX support for DTM
01/04/15   zf        CR796488: Handle L1_ENTER_NULL_COMMAND in l1_null_state
31/03/15   pa        CR815621:GRM restructuring for TH 2.0.
30/03/15   ak        CR815062: GBTA is in active when it is not supposed to be.
30/03/15   nm        CR765891 Allowing TA for TDS during PS RACH
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic
12/03/15   pa        CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
03/04/15   br        CR736774 GL1 logging improvement to ease debugging
17/11/14   dg        CR758006 IRAT improvements to ease debugging
24/02/15   js        CR749012: GL1 changes for LTE removal
04/02/14   nk        CR767734 Allowing TA during 2-phase access
19/02/15   nk        CR787609 Defer TS change if there's an activity still running
23/01/15   dg        CR784516 Set the signal GPL1_WAIT_FOR_ISR_TO_GET_FN in inactive mode
17/02/15   br        CR770962 Reset leave_blpmn while fw loading to prevent concurrent mdsp diable
10/02/15   am        CR793750 Correcting compilation warning
16/02/15   mko       CR775889: Don't set TRM extension flag for LTA SUPPORTED in DTM mode
13/02/15   pa        CR756156:G2X RF device handling using TRM apis.
29/01/15   ggu       CR787068: When deactivate GL1 from Idle state, need to clear the TS info
16/10/14   aga       CR735726  Remove obsolete gta_set_wcdma_blacklisted functionality
12/01/15   sjv       CR756154 Enhance Band Registration for Activities
06/01/15   am        CR390109 Improved DSDS cell reselection
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
08/12/14   ap        CR726899: Update Panic Reset mechanism - remove dependency on TRM
03/12/14   npt       CR759467 Fix compiler warnings
04/12/14   ap        CR766527 Handle MPH_WCDMA_CELL_UPDATE_LIST_REQ in Decoupled RACH state
01/12/14   mko       CR765651: Clear TRM extension flag for TRM_LTA_SUPPORTED when moved from transfer to idle
25/08/14   mko       CR699154: Handling the  WTOG_BPLMN_MULTI_SIM_ENDING  substate correctly in l1_do_WTOG_BPLMN_search()
30/10/14   mko       CR747580: Moving empty QTA cleanup from L1_ISR to l1_tdma_1_ISR
29/10/14   sk        CR746699 Ensure suspension counter doesnt get incremented when the pwr meas handler is removed
22/10/14   ws        CR744361 Data+MMS reduce MSC if commanded by CFCM
17/10/14   nm        CR729413 increment suspension timer for W2G BPLMN power scan if meas in progress true before terminating
15/10/14   ak        CR730234 Sleep debug code tidy up
14/10/14   df        CR676084 Sleep debug structure improvements
06/10/14   sp        Prop : CR734276 Ensure IR config command is not issued inside the QTA gap
03/10/14   aga       CR732455 Changes required to tune away framework for TH 1.0 single sim build
26/09/14   cah       CR729408 Reduce number of TRM API calls in GL1 to reduce impact of processing delays on GL1
20/08/14   sk        CR708206 Ensure GBTA is not allowed in the frame where lowest time slot changes
09/11/14   sk        CR721484 Ensure suspension timer runs for 2 frames after the activity has been aborted
07/09/14   js        CR711466:Reset the main command after drx off in W2G handover.
26/08/14   sk        CR709090 Set minimum BPLMN search durtion to 17 frames
21/07/14   am        CR690871 Restart rach gaurd timer on reception on assignment
22/08/14   cgc       CR713634 obsoleted G2L code and featurisations FEATURE_GSM_G2L_TIMED_SRCH and FEATURE_CM_G2L removed
25/08/14   df        CR660576 Enhanced timeline debug
23/08/14   sp        CR692209 Use Hyperframe macro at conditional checks.
19/08/14   sjv       CR711666 measured client id is not updated from GL1 before passing to TRM (added debug code in this file)
14/08/14   sp        CR663691: F3 added to track DTM sub states
08/07/14   mk        CR687367:Handling deactivate req immediately in WTOG_BPLMN_TRM_WAIT state
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
04/08/14   pa        CR701897: In WTOG BPLMN Resume, check RF device before initialising mult-sim FW and RF.
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
10/07/14   cgc       CR631007 FR21768 enhancement force G2X measurement MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ
27/07/14   ap        CR678067: Race condition between APP mode as ACTIVE and Device mode change as DSDA leads to crash
24/07/14   og        CR684296. Sanity check the gas id in dedicated isr and SCE.
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
07/07/14   cja       CR690067 Make W2G BPLMN suspend/resume for dual SIM more like single SIM
02/07/14   aga       CR675214: G2T TA feature and GTA/G2W TA re-worked optimized solution
02/07/14   mc        CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
30/05/14   ws        CR657674 Data + MMS Phase 1
17/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
09/06/14   cja       CR670351 For QTA empty gap wait for FW preload before aborting RF scripts
29/05/14   cs/pa     CR660831 Use new trm_status definitions specific to GL1
08/05/14   fm        CR660092 Reduce one frame offset for SB TX when SB TS is lower than CCCH TS
08/05/14   sp        CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
08/05/14   br        CR661054 In DSDA mode send  MAC_L1 release cnf to transfer sub  after checking gas id
30/04/14   ws        CR655327 Implement Guard timer for DEDICATED_STOP
22/04/14   sk        CR647044 Ensure BPLMN task substate set to WTOG_BPLMN_MULTI_SIM_SUSPENDING when suspension timer configured
11/04/14   pg        CR645875 Fix Bolt multisim compilation errors
10/04/14   pg        CR646377 Incorrect enum comparison during SUSPEND
04/04/14   cah       CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
19/03/14   ws        CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
12/02/14   dg        CR614480 In SS mode, don't go to sleep if main command is TRANSFER COMMAND
30/01/14   npt       CR606388 Enable opt3 on Bolt/Remove unused apis
23/01/14   npt       CR606388 Temp disable opt3
27/11/13   cja/npt   CR555678 Speed up clock during power scan for RF processing load
13/01/13   mk        CR598020: GS1 is not releasing the TRM till it gets PLMN Search Abort Req
11/02/13   sp        CR488596 To indicate CB Missing indication in dedicated mode too
07/01/13   js        CR597012: Removal of GERAN compilation errors for T+G build
16/01/13   sk        CR439182 Ensure GL1 doesnt get deactivated when Acq suspension timer is running
17/12/13   ap        CR566007: W is not getting the lock for Manual PLMN search when Sub2 is in data call
13/12/13   ap        CR588766: FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS amendment
04/09/13   nk        CR537601 RACH Failure occured
28/11/13   mko       CR579827 set the gsm_stop_req_in_queue to true when changing main command to suspend
02/10/13   pjr       CR552448 DDR Frequency plan implementation - l1_arbitration modification
26/11/13   ws        CR555678 Corrected integration in picking CR's out of order
11/15/13   cja       CR543395 Call MCPM voice stop for duration of PScan in L1_FIND_BCCH
22/11/13   cja       CR455527 Remove featurisation from files in API directory
11/14/13   ab        CR 554820 DSDA GL1 - Update Tx power as per slot transmission for RF COEX
23/10/13   gk        CR555414 Ensure idle mode cnf is sent back in single-sim IDLE
02/09/13   cja       CR536807 Call gl1_hw_turn_tx_off when starting PScan in L1_FIND_BCCH if MCPM not idle mode
14/10/13   cja       CR555678 Speed up clock during power scan for RF processing load
07/10/13   js        CR554760 Corecting the merge error
04/10/13   pjr       CR536133 Changed the place where mdsp_process_cxm_logs is called from
05/10/13   js        CR461049 Cell Reselection Failure when 3 KHz frequency drift is applied on N cell
04/10/13   aga       CR554459 IR memory config cmd is being issue after running state specific l1_isr cb
19/09/13   cja       CR546868 Use gl1_hw_turn_tx_on/off for setting clock speed so Tx Config is sent
16/09/13   cs        CR503042 Updated XO RGS handling for Triton
15/09/13   cja       CR543395 Call MCPM voice stop for duration of PScan in L1_FIND_BCCH
14/09/13   ap/aga    CR545423 - G2W Tune Away Feature
14/09/13   sp        CR460968 Ensure W2G BPLMN Power Scan works correctly [Propogation]
11/09/13   sk        CR537096 Ensure to initialise W2G BPLMN task state correctly when W2G mode started
02/09/13   cja       CR536807 Call gl1_hw_turn_tx_off when starting PScan in L1_FIND_BCCH if MCPM not idle mode
16/07/13   zc      CR513078 Make sure that pwr meas handler is uninstalled in transfer mode
21/08/13   npt       CR531633 - Remove FEATURE_DIME_MODEM dependencies
17/06/13   pa        CR502770: Disabled dpc gps calls in GPLT
15/08/13   sk        CR524039 GBTA bringup changes
08/08/13   dv        CR525765 Changes for DSDS T/G IRAT bringup
09/04/13   cgc       CR453758 add FEATURE_CM_G2L search functionality FR-3009
22/07/13   og        CR499499. Missing release confirmation ID.
02/07/13   aga       CR502832 Fix for Timetag on Sub2 on 8x26 DSDS not working
02/07/13   sk        CR507513 TRM advanced call back integration to Triton-TSTS
25/06/13   cs        Major Triton TSTS Syncup
12/06/13   ip        CR359544: Removed repeated debug
07/06/13   ap        CR496976 Correct the sending of l1 mac tbf rel confirm
06/06/13   ap        CR491896 Delay the release of TRM when pscan ends for MPLMN
23/05/13   ip        CR488553 Option 3 working for SS mode only
23/05/13   jk        CR476088:Delay sending of L1_MAC_SINGLE_BLOCK_SENT till idle init
23/05/13   sjw       CR385159 Handle repeated abort callbacks during panic reset
21/05/13   am        CR460875 Ensure suspension timer is not running when TRM is released in state FW_Loading of
                     W to G BPLMN search
21/05/13   am        CR409195 Dont deactivate when Pscan suspension timer is running
14/03/13   ap        CR460058 DSDS on returning to PS from EDTM , GTA is not re-initialized causing missed pages
20/05/13   cs        TSTS Updates
16/05/13   ws        CR488000 Enanble GPS timetag and compile warning cleanup
29/04/13   ws        CR481359 enable GTA support
13/03/13   mc        CR459487 : COEX Support - GERAN arbitrator framework
14/03/12   ss        CR463697 Disable gps time tag tick
25/02/13   cja       CR390390 Slam to TS0 for G2W handover.
12/02/13   ap        CR 448602 DSDS GTA: UE not able to finish FTP and stuck in bad state
08/11/12   ab        CR 418358 Increase RACH GUARD Timer based on starttime
31/01/13   og        CR446067. Ensure the correct sub is deactivated.
07/11/12   ss/cgc    CR404843: DSDS version check cell ptr l1_sc_balist_check_cell() is within pool address range
02/05/12   ky        CR357955:Going to idle mode from transfer mode, if the other sub is
                     having lock then set main command as L1_DECOUPLE_MULTI_SIM_IDLE_COMMAND
                     so that it wont switch on the DRX
30/11/12   ab        CR423003 - next_params.pIdle_data incorrectly initialised failing during ASYNC SIM swap.
08/10/12   ss        CR397878 Set this signal  to ensure L1 task is not locked out permenantly even in find bcch mode
27/02/13   pg        CR457254 fix CR405396 for dual-SIM image with single SIM config
14/02/13   pjr       CR452272 added support to set multisim mode at activation time
04/01/13   cja       CR435212 Delay sending l1 mac single block sent until in idle state.
03/01/13   npt       CR430322 - Modify sleep debug structure
19/12/12   cja       CR411308 Dime T2G changes (msg router to GFW)
18/12/12   cgc       CR398370 Allow wtog_bplmn suspend to complete before actioning l1_get_gsm_stop_req_in_queue
30/10/12   cja       CR390390 Slam to TS0 for G2W handover.
31/05/12   ab        CR 415136 RX and TX Time Power averaging update using msg router intf.
23/10/12   npt       CR413635 - Port sleep code for DIME modem
18/10/12   pg        CR405396: Avoid switching off Tx clocks until GFW Tx processing has completed
14/09/12   cgc       CR399223 remove legacy code from cr262337.
15/08/12   cja       CR385484 Add flag for async interrupt in num cmds
24/08/11   kb        CR302696 - Added Initialization function to initialize isr data values
10/07/12   pg        CR373325 Check GSM clock before executing GPS timetag
03/07/12   pa        CR375720: Don't delay TBF release in the idle frame.
22/06/12   pa        CR371580: Delay TBF release during G2T activity.
31/05/12   ab        CR 366880 RX and TX Time Power averaging Feature for DSDA as part of SAR Req.
31/05/12   cja       CR366518 Correct g2t startup/cleanup time decrement
18/04/12   ab        CR 324121- Abort CCH one frame early when CCCH ts lower than Single block ts.
10/05/12   jj        CR357596 Ignore the l1 stuck in null mode in FTM mode
24/04/12   cgc       CR353517 Add gps request flag, Mutex protect and add gl1_drx_task_inactive
24/04/12   npt       CR354790 - Remove old sleep debug (under DEBUG_GL1_SLEEP) as it is not needed
                     after FEATURE_GL1_SLEEP_DEBUG_STRUCT is introduced
12/04/12   cja       CR351581 In FTM do not error fatal if stuck.
02/04/12   cgc       CR338501 from 9K: BPLMN search suspend and MPH_STOP_GSM_MODE_REQ transition error.
30/03/12   cgc       CR300303 reapplied as code never got merged.
14/03/12   pg        Add support for QuRT
05/03/12   cgc       CR339393 reworked speedup gl1_clkdata_speed() now used
29/02/12   cja       CR339905 Allow concurrent G2W and channel assignment speed-up
30/01/12   cgc       CR332648 Increase CPU speed on channel assignment transition
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
10/02/12   cja       CR333023 Send IR configure command from ISR not task
06/02/12   cja       CR332659 Always send generic config, to fix WTR KVCal issue
02/02/12   cja       CR328529 Do not add generic config at the end of the frame if empty buffer
24/01/12   npt       CR330084 - Fixed boundary overrun in sleep debug structure
12/01/12   npt       CR311544 - Added sleep debug structure - FEATURE_GL1_SLEEP_DEBUG_STRUCT
20/12/11   cgc       CR326257 Set FT signal from l1_isr_reset_mode()
22/11/11   jj        CR319573 incorporate with review comments
17/11/11   jj        CR 319573 complier error correction
16/11/11   jj        CR 319573   changes for  profiling the modem performance
07/11/11   jj        CR315253 IQ capture capability for GSM via QRCT
31/10/11   cja       CR312694 Do not delay sleep for AFC update command
15/04/11   ap        CR281977. Race condition between Channe Mode modify and
                     MAC_L1_DYNAMIC_TBF CONFIG in DTM Mode.
27/08/11   og        Completing the fix for CR303398.
25/08/11   og        Disable the GFW APP when BPLMN is suspended. CR300303.
28/04/11   cja       Add Nikel freq correction in GFW
09/02/11   og        CR273622. Decrease the available BPLMN search time to take
                     into account initialization delays.
18/01/11   og        CR270335. Remove the fix for CR262337 since the RRC timer
                     dependency no longer exists.
22/07/11   ap        CR297650 DSDS Develop the watchdog deactivate functionality for Dual Sim
06/07/11   nt        DSDS CR:294745 - Add check for TRM_ABORTING to L1_do_background_HPLMN_search()
30/06/11   sjw       CR293419 - use l1_isrtsk_blk pointer in l1_state_log()
27/06/11   cs        CR287491 Move post tick processing for asynch into DPC to
                     allow for mdsp sleep cmd isr to be processed
25/05/11   ap        DSDS CR:289094 - Signal release of the Single block after the mDSP
                     complete has began to terminate
11/04/11   sjw       Featurisation correction for DSDS
31/03/11   nt        CR281165 - Ensure that GSM is awake when panic reset begins
22/03/11   ap        CR279430 DSDS Timing value wraps casusing frame misalignment
28/03/11   nt        DSDS CR:281689 - Signal release of the TBF after the mDSP complete has began to terminate
21/03/11   nt        DSDS CR:278831 - Gl1 attempt to stop mDSP while it is busy
02/03/11   njt       DSDS CR276931 - BPLMN suspension changes
14/12/10   nt        CR: 267338 - W2G BPLMN is not correctly terminated for MPH_STOP_GSM_MODE_REQ
10/01/11   ap        DSDS CR270377 SYNC SIM is disabling L1 ISR while ASYNC SIM is still
                     active when consecutive start/stop GSM mode req
03/12/10   cgc       Featurise previous change with FEATURE_GSM_GPRS_QDSP6
01/12/10   cgc       Offload gl1_timetag_gps_tick() functions to DPC task
22/11/10   ky        Panic reset mechanism is changed. For all errors gl1 first
                     sees an nv item set or not. if this nv item is set then does
                     error fatal immediately other wise gl1 sends signal to grr for
                     recovery
17/11/10   dv        CR262337 - RRC abort timer times out for larger values
                     of drx_interval
23/07/10   ip        CR245474 Added Sleep debug code
08/02/10   cja       XO Update
29/01/10   nf        CR180324 apply to new PCH timeslot in RACH and BCCH state
01/10/09   dv        CR211967 MDSP Halt after changing from EDTM to XFER
16/07/09   cs        CR188876 Always clear l1_bplmn_suspend_in_progress when we
                     start BPLMN to avoid later mdsp crash
13/02/09   ap        Lint fixes for high Warning
02/10/08   cs        Lint fixes
28/08/08   cs        Implement the Fast Rx Sweep functionality for FTM
07/08/08   ap        CR 150098 Change ta_value type for GERAN ENG MODE FEATURE
25/07/08   ap        Added FEATURE_GAN: Changes to support GAN->GSM HO.
22/04/08   ap        CR#144832 Ensure L1 drops back to IDLE when the single block start
                     time elapses on 52 multiframe.
14/03/08   ip        Added PSHO changes under FEATURE_GPRS_PS_HANDOVER
17/01/08   cs        Lint fixes
21/11/07   og        Ensure L1 drops back to IDLE when the single block start
                     time elapses. Resolves CR115529.
18/05/07   cs        Fix time tagging for Aries GPS featurisation
10/05/07   cs        Frequency error update changes for Aries GPS
19/04/07   og        Ensure the stop mode request is serviced when de-activation
                     is interrupted by suspension in bplmn power scan. CR 116586.
05/03/07   cs        Making CR112989 calculation consistent with sc updates
23/02/07   og        Ensure the stop mode cnf is sent to RR, Resolves SR906973 /
                     CR 112398 AMR HO failure.
06/02/07   cs        Move all sig definitions to l1_os.h
23/01/07   og        Resolve RVCT2.1 compiler warnings, merge from the
                     gpl1_hummer.10.00.43.01 branch.
05/01/07   og        Replace err_fatal with panic reset when tbf assignment connected
                     from single block 2-phase access has no valid start time.
19/12/06   nt        CR107800: non_drx timer is started only when leaving transfer.
01/12/06   og        Rewind back the fn counter by one when dropping from DTM mode
                     to transfer mode and cs ts < TS2.
14/11/06   og        Set the tn slam mode when leaving DTM back to XFER.
02/11/06   og        Ensure afc and tt mode is set to PDCH when leaving DTM to
                     packet transfer mode.
30/10/06   og        DTM enhanced release code drop.
07/09/06   og        EDTM updates following testing on Agilent 8960.
31/08/06   og        EDTM updates following host test simulator testing.
24/08/06   og        EDTM updates following host test simulator testing.
17/08/06   og        Basic featured-off EDTM code drop.
05/07/06   og        Lock interrupts when handling stop gsm mode req. Resolves
                     CR95811 / SR833323.
29/06/06   og        Ensure gpl1_dtm_control( ) is not called twice when handling the
                     stop event. Resolves CR93501.
25/05/06   og        Improve PTR reaction time, resolves CR90515.
04/05/06   rm        Fixing CR92108, relocating sending L1_POWER_SCAN_CNF to l1_isr_find_bcch().

07/04/06   nt        Changes for required host test support/compilation see L1_HOST_TEST
05/04/06   nt        Moved handling of L1_DTM_STOP to make it statless for CR:90366
09/02/06   og        Allow one extra TDMA frame inorder to handle long frames
                     associated with high->low TS change. Resolves CR 87369.
06/02/05   nt        Added code to dtm state machine to correctly handle simultaneous
                     CMM and L1_REL.
01/27/06   sv        Remove call to abort power scan in l1_isr.This is done in l1_pscan now.
17/01/06   og        Merge change 280538 from the raven 3.0 branch. Resolves
                     CR 86191 / SR 791121.
01/16/06   sv        Fix CR85766 - UE reset while Stability testing for Abort specific
                     BPLMN testing
12/22/05   rm        Support for L1_PBCCH_RESELECTION_COMMAND in FIND_BCCH mode
12/14/05   sv        Support the changed prototype for MPH_DECODE_BCCH_LIST_CNF.
12/12/05   sv        Allow L1 to abort BPLMN activity before shutting down GSM.
11/15/05   sv        Lint cleanup.
11/07/05   sv        Add support for FEATURE_WTOG_BACKGROUND_PLMN_SEARCH.
08/11/05   nt        Added handling of event L1_DTM_CANCEL_PS_RECONFIG to the DTM FSM.
02/11/05   nt        Added code to DTM ISR to handle the DTM_ASSIGNMENT in DTM mode
28/10/05   nt        Added more sophisticated release handling within the DTM FSM isr.
21/09/05   nt        Made changes to the release handling in the DTM isr.
20/09/05   nt        Fixed break in if/else event handler in state L1_DTM_RUN, in DTM fsm
19/09/05   nt        Correction of DTM related syntax error for current_FN.
19/09/05   nt        Changes made to the DTM state machine to support the
                     reconfig/release and addition of TBF's in DTM. Also added the
                     handling of channel_mode_modify to the state machine during any
                     of the above TBF scenarios.
15/09/05   og        Issue sysinfo failures when transitioning to pkt transfer
                     from idle. Resolves CR 75623.
09/01/05   ws        Linted and code cleanup for FEATURE_GSM_DTM
25/08/05   og        Tick the drx timers independent of L1 mode.
08/19/05   ws        Added missing L1_DEDICATED_TO_DTM_COMMAND to L1_DEDICATED_MODE
08/16/05   sv        Catch the case where we get stuck in FIND_BCCH mode to allow
08/12/05   ws        Added initial FEATURE_GSM_DTM support
07/29/05   gfr       Support for NPL1 DTM interface changes
06/28/05   gw        Lint.
05/25/05   gw        Simplified gl1_deactivate().
15/05/05   og        Removal of arithmetic modulo operations.
05/02/05   gw        Move calls to sleep_enter_mode()/sleep_leave_mode()
                     to task context.
04/25/05   bm        Disable FTM mode after sending STOP_GSM_MODE_CNF, hence
                     removed the call to L1_disable_ftm_mode from ISR
04/21/05   sv        Add support for L1 "Panic Reset"
04/21/05   ws        Transfer_mode. ONly send release confirm when requested
04/15/05   gfr       Catch if we get stuck in random access mode.
24/3/05    ws        Moved Release_confirm sending to after packet transfer mode has stopped since
                     RLC may request to RACH before L1 has returned to idle
21/3/05    og        Updates to handle dl release immediately followed by a
                     queued uplink release.
21/2/05    og        Ensure the resetting of wait for pca fn is independent of
                     the transfer reconfig start time when start time is valid.
02/11/04   bm        Support for Background WCDMA PLMN Scan
02/05/05   bm        BG PLMN scan optimizations
01/26/05   sv        Add support for Frame Manager.
10/01/04   bm        Enhanced Parallel BCCH decode support
13/1/05    og        MS timebase adjustements confined to radio block gap.
01/05/05   dp        Merged dedicated timeline changes for audio optimizations
                     to mainline. Lint cleanup
24/11/04   og        Merge fixes from the saber 4x branch.
17/11/04   og        Fix to ul tbf release confirm being sent.
10/18/04   dlh       Added initialisations to for reselection/start_time flag
10/18/04   dlh       Added the change to L1_GPRS_IDLE_INIT sub-sub_state for 52 idle.
13/10/04   kf        Changes for time slot slamming in transfer.
10/05/04   dlh       Remove frame slamming from 52 idle mode transitions.
10/08/04   ws        If we are reconfiguring on an Idle slot -1 or +1 in Packet transfer wait for
                     1 frame until frame slamming to new lowest timeslot.
09/28/04   ws        Transition from packet access state to transfer state 1 frame earler
                     in case we need to slam to a lower slot
09/28/04   ws        If we are reconfiguring on an Idle slot in Packet transfer wait for
                     1 frame until frame slamming to new lowest timeslot.
09/27/04   bm        Fixing some merge errors
09/24/04   bm        Support for Split up the L1 deactivation into TASK and ISR
09/15/04   ws        Frame slamming and P)RACCH timing fixes for GPRS
08/31/04   ws        Reconfigure Transfer mode 1 frame earlier to stop frame slamming
                     going off in the midle of a FCCH detection
08/26/04   sv        Lint cleanup
08/23/04   dp        Added support for "fast camping" for FTM BER measurements
08/03/04   sv         Changed interface to support full acquisition on a GPRS reselection.
7/21/04    bk        Added frame tick reslamming support
06/28/04   gw        Changed error reporting for stuck in Null Mode.
06/24/04   gw        Removed checking for ISR overruns. This is now done in the
                     GSTMR drivers.
06/22/04   dp        Lint cleanup
06/22/04   sv        Added L1 NV logging mechanism on Error Fatals.
06/11/04   bm        Send START_GSM_MODE_CNF after changing state to NULL,
                     so that task does not receive any commands from RR in INACT mode
06/08/04   dp        Moved checking of abort_find_bcch into if statement so
                     that we don't hit the "Unexpected command" error
08/06/04   kf        setting of idle_data_ptr from find bcch mode in L1_GPRS_IDLE_COMMAND
06/04/04   sv        Removed all references to XXX_mframe_power_XXX.
06/04/04   sv        Check and set "L1_confirm_ID" to appropriate enum values.
06/04/04   DMB       Send MPH_START_IDLE_MODE_CNF when transitioning from RACH to IDLE or GPRS_IDLE
05/26/04   sv        Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters for better
                     code readability.
05/20/04   gfr       Mainlined FEATURE_COMMON_PWR_HANDLER
03/16/04   gw        Broke up l1_isr() into sub-functions (one per state).
03/26/04   ws        Added waiting for packet access isr to abort before changing
                     state. Needed for R&S GCF failure
03/26/04   gfr       Time tagging support.
03/25/04   DMB       Added L1_send_MPH_START_IDLE_MODE_CNF to IDLE_MODE when moving to GPRS_IDLE_MODE
03/24/04   DMB       Added L1_send_MPH_START_IDLE_MODE_CNF to FIND_BCCH (for 51)
03/22/04   gfr       Detect if we are stuck in NULL mode.
03/17/04   dlh       Added L1_send_MPH_START_IDLE_MODE_CNF to FIND_BCCH (for 52)
02/25/04   gw        Lint clean up.
02/09/04   dp        Added support for background HPLMN searching
01/29/04   ws        Added release from IDLE_MODE and GPRS_IDLE_MODE fro GPRS
05/11/03   gfr       Support for quad-band.
09/15/03   dlh       lint errors cleanup.
08/22/03   gfr       Support for hw task requests.
07/29/03   gw        Renamed DEBUG_L1_STATE_LOG to FEATURE_GSM_L1_STATE_LOG.
28/07/03   kf        removed function call gpl1_re_start_surround_cell_52_idle
28/07/03   kf        added l1_tskisr_blk->command = L1_DECODE_PBCCH_COMMAND in pbcch sync.
25/07/03   kf        Bug fix in L1_PBCCH_RESELECTION_COMMAND.
07/22/03   ws        Removed Debug for releasing transfer and returning to idle
07/21/03   dlh       removed remainning drx_off call from 53 idle.
07/21/03   dlh       added rex_set_sigs to transfer sbu state of 52 Idle.
07/18/03   gw        Shifted assignments to idle_data_ptr in l1_isr() to ensure
                     that correct value is assigned.
07/17/03   ws        Added Debug for releasing transfer and returning to idle
07/17/03   ws        Added setting GPL1_WAIT_FOR_ISR_TO_GET_FN signal for idle_mode gprs_idle_mode
                     rach_mode and transfer_mode.
07/15/03   gw        Added logging of states and commands.
16/07/03   kf        modified ncell pbcch reading.
07/11/03   dlh       Added rex_set_sigs and removed drx_off from 52 idle state.
07/11/03   gfr       Lint cleanup.
07/08/03   bk        Moved l1_sc_balist_gprs_set_sync to l1_sc_start to sync
                     BSICs during Idle <-> Transfer mo
07/04/03   ws        modified calling of pbcch isr in IDLE mode and GPRS IDLE mode
07/01/03   ws        Added GPL1_WAIT_FOR_ISR_TO_GET_FN set sigs to transfer sub state of
                     IDLE_MODE state
06/30/03   ag        Handle PBCCH reselection event in L1_IDLE_MODE state.
06/26/03   ws        Replaced rrbp_pending with wait_for_pca_valid
06/26/03   ag        Only process L1_READ_PBCCH when L1_idle_mode_ISR() returns TRUE.
06/24/03   dlh       Added the check if 52 idle finished befrore doing fch/sch
06/19/03   pjr       Added L1_ACQUIRE_BCCH_LIST_COMMAND handler to
                     L1_RANDOM_ACCESS_MODE
06/19/03   gw        Changes for starting/stopping SCE in 52 going in/out of
                     52 Idle Mode.
06/19/03   gw        Changes to starting and stopping of SCE.
06/13/03   pjr       Added L1_PBCCH_RESELECTION case to L1_GPRS_IDLE to ensure
                     synchronisation occurs correctly to new cell.
06/10/03   ws        Moved PBCCH isr from IDLE state to BCCH state
05/27/03   ws        Changed brace for IDLE_COMMAND within GPRS_IDLE_MODE
05/23/03   ws        Updates for BSIC TRANSFER feature
05/27/03   tb        Added a one frame wait when deactivating GSM to allow
                     MDSP commnds to finish before turning off its clock.
05/20/03   gfr       Common power handler support.
05/15/03   ws        Added transition to NULL state from TRANSFER state
05/12/03   dp        Fixed bugs in transferring Ncell BSIC across transitions.
05/12/03   ATM       Moved Frame Number management entirely inside GTMRS
05/07/03   ws        Fixed release confirm cause for releasing within rach sub_state
04/29/03   dp        Changes to preserve Ncell BSIC across transitions.
                     Kick off idle mode surround cell engine in ISR.
05/02/03   gw        Merged in power transfer changes from branch.
05/02/03   ws        Updates for sleep in GPRS PCCCH idle
04/30/03   pjr       Added RESET_BITMAP to L1_TRANSFER_COMMAND when in L1_ACCESS_MODE
04/28/03   gw        Moved calls to gl1_drx_off() inside idle_mode_ISR functions.
                     Changes for preserving power measurements between modes.
04/17/03   DLH       Removed call to 52_Idle_isr when reading PBCCH in 52 IDLE.
04/17/03   DLH       Added improved handling of changing into 52 idle mode
04/10/03   mk        Added frame tick ISR overrun detection.
04/09/03   ws        Removed l1_sci_abort_sb() as l1_sci_abort_fcb() aborts combined
                     fcb/sb acquires
04/09/03   pjr       Added setting of l1_transfer_data.frequency_info_1 to case when
                     going from transfer in single block to transfer dynamic.
04/04/03   gw        Removed calls to l1_sc_balist_reset_counters().  This is now
                     called from the task context.
03/25/03   bk        Merged Revision#61 from mainline onto samsung_3.0 branch
03/26/03   pjr       Added gl1_msg_dl_abort to L1_TRANSFER_COMMAND in L1_TRANSFER_MODE
03/25/03   jc        Abort power scan when deactivate is received.
03/20/03   pjr       Modified leaving of L1_GPRS_IDLE_MODE when going to L1_TRANSFER
                     and L1_RANDOM_ACCESS.
14/03/03   pjr       Modified leaving of L1_GPRS_IDLE_MODE when going to L1_TRANSFER
03/12/03   pjr       Made modifications to leaving and entering GPRS52 idle state.
03/07/03   tb        Added support for handling GtoW handover failure.
07/03/03   DLH       Bug fix for leaving idle flag to transfer flag in 52 idle.
07/03/03   pjr       Modified sending of GPRS release confirm to MAC
06/03/03   kf        Added case L1_READ_PBCCH: to L1_GPRS_IDLE_MODE for reselection pbcch
03/05/03   DLH       Added flag in gpl1_gprs_idle_isr(TRUE/FALSE) to indicate leaving 52 idle.
03/04/03   dp        Fixed div 0 error introduced by GPRS change to handle
                     transition from RANDOM_ACCESS to IDLE
02/28/03   kf        Added code for 52 idle measurements
02/28/03   pjr       Added L1_MODE_XXX parameter to L1_packet_access_ISR and
                     modified L1_GPRS_IDLE_COMMAND when in L1_RANDOM_ACCESS_MODE
02/21/03   gw        Added support for deep sleep.
02/20/02   dp        Enabled frame counting before compressed mode handovers
02/20/03   pjr       Modified L1_GPRS_IDLE_COMMAND when in L1_RANDOM_ACCESS_MODE
                     Modified L1_IDLE_COMMAND in L1_TRANSFER_MODE
02/14/03   DLH       Added mm_non_drx_mode initialisation and 52 gprs idle func.calls.
02/14/03   pjr       Added support to 52mode idle and pbcch, and modified sleep
02/12/03   ws        Added code for GPRS to support reading FN from task context in
                     L1_IDLE_MODE
02/05/03   ws        Updated starting time FN for GPRS for scheduling mod
01/10/03   gw        Changed some SCE function calls to reflect renaming of some
                     BA list functions done as part of SCE clean up.
01/02/03   gw        Changed call to gl1_drx_post_tick() to reflect addition of
                     argument.  Elevated odd_even_count MSG_HIGH to MSG_ERROR.
12/13/02   ws        Reassigned l1_idle_data in access_mode when returning to idle
                     for GPRS
11/25/02   mk        Updated GPRS, added support for FEATURE_GSM_GPRS_PCCCH.
11/15/02   mk        Updated GPRS baseline with release 2.0 functionality.
11/14/02   ws/gw     Restructured GPRS idle state
11/13/02   DLH       Removed some superfluous comments.
11/06/02   ws        Added release type to gpl1_gprs_send_l1_mac_tbf_rel_confirm().
11/01/02   gw        Moved GSMTR odd-even check from L1_increment_frame_counters
                     to L1_tdma_1_ISR so that it can be applied in Idle Mode too.
10/30/02   gw        Added call to gl1_drx_post_tick() at end of l1_tdma_1_ISR
                     as part of sleep timeline optimization.
10/30/02   he        Replaced FN by gl1_get_FN() for GPRS.
                     Removed first_cch_block for GPRS.
10/14/02   ATM       Removed compiler warnings
10/14/02   ATM       Added compressed mode interRAT handover functionality
10/14/02   ATM       Added central get and set FN routines
10/11/02   mk        Merged GPRS release 1 phase2 code to baseline.
10/10/02   mk        Merged GPRS release 1 phase1 code to baseline.
10/04/02   dp        Fix to handle MPH_DECODE_BCCH_LIST_REQ in idle mode
10/04/02   gw        Added code to tell sleep task when GSM becomes inactive.
                     Required to support sleep in dualmode.
09/16/02   plm       Added #include "gl1_hw.h"
#include "gl1_hw_g.h" when FEATURE_MULTIMODE_GSM is defined
                     and added extern for L1_get_bcch_list() to eliminate another
                     compiler warnings.
09/13/02   pjr       Modified confirm_pending process to stop multiple sneding
                     of confirm message to MAC when doing concurrency
09/12/02   pjr       Removed calling of gpl1_gprs_pending_prach_req() and
                     associated control as this functionality now replaced by
                     confirm_pending process..
09/11/02   pjr       Added confirm_pending process to TRANSFER/IDLE_COMMAND
09/10/02   dp        Added support to send back MPH_DECODE_BCCH_LIST_CNF when
                     we get a MPH_DECODE_BCCH_LIST_ABORT_REQ
09/05/02   pjr       Modified moving from transfer to transfer when going from
                     single block to dynamic.
09/04/02   dp        Fixed merge issue - Put back gl1_hw_inact
09/03/02   dp        Added support to handle PH_RANDOM_ACCESS_REQ in
                     FIND_BCCH state.Cleaned up first_cch_block
09/03/02   pjr       Modified moving from transfer to access.
08/28/02   DLH       Remove non_drx_timer flag and added non_drx_func call.
08/27/02   DLH       Set non_drx_timer flag when returning gprs_idle from transfer
08/13/02   dp        Support for parallel acquisition/BCCH decodes
07/10/02   ATM       Added blind-handover support.
07/16/02   jc        Add code to check for FIND_BCCH while in FIND_BCCH state.
07/24/02   pjr       Restructure of L1_ACCESS_STATE to handle change of states
                     correctly when in GPRS mode.
07/29/02   dp        Fix to handle sending back reselection cnf in access mode
07/10/02   dp        Fix to handle another BCCH decode request when we're
                     already in FIND_BCCH state.  Fix to share Ncell info
                     across dedicated modes
07/09/02   pjr       Added calling of gpl1_gprs_pending_prach_req() and
                     associated control.
06/27/02   dp        Added support to decode just the SCH ( as opposed to
                     combined acquisition )
06/20/02   DLH       Added the initialisation of the PL1 CCCH_51 flag in
                     in L1_initialise_isr to ensure GSM 51 is executed at
                     boot up time in the isr.
06/12/02   pjr       Added handling of L1_ACCESS_COMMAND, and L1_IDLE_COMMAND in
                     transfer state
06/10/02   gw        Added call to gl1_drx_tick() in l1_tdma_1_ISR(). Disabled
                     missed frame detection in Idle Mode.
06/10/02   dp        Pulled out changes for BEFORE to AFTER transition to
                     enable seamless transition.  Logic is in l1_ded
05/30/02   pjr/mk    Added PL1 GPRS code hooks under switch FEATURE_GSM_GPRS_L1.
05/30/02   dp        Declared l1_isrtsk_blk as volatile to suppress compiler
                     optimizations
05/24/02   pjr       Added handling of L1_ACCESS_COMMAND in the IDLE state when
                     in 52 idle and chaging to 52 access.
05/22/02   mk        Changed L1_NULL_MODE to handle both SCAN and ACQ commands,
                     changed L1_IDLE_MODE and L1_RANDOM_ACCESS_MODE to start
                     PowerScan only if recieved SCAN command, changed
                     L1_DEDICATED_MODE to handle only DECODE command.
05/20/02   pjr       Added PL1 GPRS code hooks under switch FEATURE_GSM_GPRS_L1.
05/10/02   tb        Added support for FEATURE_MULTIMODE_GSM
04/30/02   mk        Added missed frame tick detection.
04/24/02   dp        Added transition from BEFORE to AFTER channel.  In this case
                     we don't suspend and resume but just go on to initialize the
                     AFTER channel
04/23/02   JC/mk     Added check for L1_SCAN_COMMAND in DEDICATED mode to
                     support l1_ded_if when MPH_CHANNEL_RELEASE_REQ with
                     no valid bcch flag set.
04/17/02   JC        Added support to jump to acquisition while in
                     RANDOM_ACCESS_MODE & IDLE_MODE (bypassing power scan).
04/15/02   mk        Fixed cell timing slam during L1_DECODE_BCCH_COMMAND
                     handling in idle and access modes.
03/04/02   JC        Clean up.  Removed old acquisition code; no more support
                     for separate FCCH detection/SCH decoding.
02/13/02   jc        Added support for ciphering.
02/07/02   gw        Improved handling of l1_tskisr_blk lockout and improved
                     start/stopping of SCE during mode transitions.
02/05/02   JC        Removed reference to TIMETEST_PORT.
01/28/02   ATM       Added calls to l1_change_state() for logging
01/28/02   gw        Disabled accesses to TIMETEST_PORT when TIMETEST is defined.
12/21/01   gw        Changes to support DRX scheduling.
12/18/01   dp        Changes to support aborting SC when switching between modes
12/13/01   gw        Changes to support message/frame layer acquisition.
11/27/01   mk        Added support for L1_IDLE_INIT and L1_ACCESS_INIT
                     substates. Misc cleanup.
11/02/01   mk        Featurized TIMETEST_PORT.
10/31/01   dp        Commented out code to turn on LEDs in TDMA ISR.  Replaced
                     hardcoded LED port with TIMETEST_PORT
09/28/01   jc        Removed some unused variables.
09/20/01   jc        Removed the ccww data services hook.
08/31/01   JC        Added QCT function header blocks and cleanup.  Moved
                     L1_initialise_isr to this module.
05/29/01   JC        Removed SPINCO_SIMULATION code as cleanup.
04/24/01   jc        Removed call to mdsp_clr_current_num_cmd().
04/02/01   JC        Added this header block and modified Frame Number
                     variables.  Also added GSM_ERROR_L1() reference.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "geran_tasks.h"
#include "l1_drx.h"
#include "l1i.h"
#include "l1_isr.h"
#include "rr_l2.h"
#include "geran_msgs.h"
#include "l1_task.h"
#include "l1_sc_irat.h"
#include "l1_sc.h"
#include "l1_utils.h"    /* for l1_state_change */
#include "l1_fm.h"
#include "ftm.h"

#include "gpl1_gprs_if.h"
#include "gpl1_gprs_isr.h"
#include "gpl1_gprs_task.h"
#include "rex.h"
#include "l1_os.h"


#ifdef PL1_FW_SIM
#include "pl1_test_harness.h"
#else
#include "gmactask.h"
#endif

#ifdef DEBUG_GSM_GPRS_LED_TEST
#error code not present
#endif /* DEBUG_GSM_GPRS_LED_TEST */


#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "vtmrs_g.h"

#include "gl1_hw_tcxo_mgr3.h"

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
#include "gl1_msg_g.h"
#endif

#include  "l1_log.h"  /* For L1 NV logging */
#include "geran_eng_mode_info.h"

#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "gl1_msgi.h"

#include "gpl1_dual_sim.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#include "geran_dual_sim_g.h"

#include "gpl1_grm_intf.h"

#include "gl1_hw_sleep_ctl.h"
#include "ftm.h"

#include "gl1_msgi.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#ifdef FEATURE_GSM_TDS
extern int     l1_g2t_delay_tbf_rel;
extern boolean l1_g2t_issue_tbf_rel;
#endif

#ifdef FEATURE_GL1_ASDIV_ACQ
#include "gl1_asdiv.h"
#endif
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#include "gpl1_gprs_error_recovery.h"
#endif/* FEATURE_GERAN_PTM_ERR_RECOVERY */

extern dedicated_data_T l1_dedicated_data[];
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
extern boolean gpl1_g2x_ta_deregister_seq(gas_id_t gas_id);
#endif

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

#if 0
/*lint -esym(652, l1_idle_data) re-define */
#define l1_idle_data dont_access_l1_idle_data_in_this_file
/*lint -esym(750, l1_idle_data) not referenced */
#endif

/* 1 TDMA frame is required to issue a change to L1 State, this needs
 * to be taken into account when l1_change_state( ) is called and the
 * MS is expected to connect to a particular logical channel.
 */
#define  L1_STATE_CHANGE_FN_OFFSET  (1U)

/* This offset is used to account for cases where
 * the L1 State change is associated with a camp timeslot
 * change from high to low. Since this case causes a long
 * TDMA frame then the scheduler must be able to anticipate
 * that in advance.
 */
#define  L1_CAMP_TS_FN_OFFSET  (1U)

/* Max GUARD timer (in TDMA frames )for L1_DEDICATED_STOP SUB STATE before
 * hitting ERR_FATAL
 */
#define  L1_DED_GUARD_FN_MAX   (52U)

// max number of frames that GBTAis active while neither of the subs are in transfer mode
#define L1_MAX_GBTA_FRAMES_NOT_IN_TRANSFER    20

/*===========================================================================

                            LOCAL STORAGE

===========================================================================*/

/*Flag indicating that GL1 state has just changed*/
extern boolean gl1_flag_state_change[];

/*
These two flags are for test purposes only and are used for
switching between idle and SDCCH to allow testing surround cell
operation on the SDCCH for extended periods of time.  These
are run time flags
*/
uint8           jump_to_sdcch         = 0;
uint8           jump_to_idle          = 0;

/* Keep track of how long we are in NULL mode in case RR has abandoned us */
#define L1_MAX_NULL_MODE_FRAMES 6500  /* 30 seconds in NULL mode */

/* Keep track of how long we are in FIND_BCCH mode in case RR has abandoned us */
#define L1_MAX_FIND_BCCH_FRAMES 19500  /* 90 seconds in FIND_BCCH mode */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#define MAX_FRAMES_MDSP_AWAKE_BEFORE_PANIC 10
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
void l1_isr_suspend_mode( gas_id_t gas_id );
#else
static   void    l1_isr_suspend_mode( gas_id_t gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#define  MIN_SUSPEND_RESUME_DURATION 30
#define  MIN_RESUME_TIME_IN_MS 30
#endif

extern uint16 gl1_sleep_cnt[];
extern gl1_hw_sleep_type gl1_hw_sleep_struct[NUM_GERAN_DATA_SPACES][NUM_OF_GL1_DEBUG_CYCLES];

#ifdef FEATURE_USE_ACTUAL_TRM_DUR_UNLOCK
extern gpl1_g2x_ta_data_t  gpl1_g2x_ta_data[NUM_GERAN_DATA_SPACES];
#endif

void gl1_wait_for_dpc( gas_id_t gas_id );

extern void L1_sync_to_current ( l1_sc_bs_T *current_cell, gas_id_t gas_id );


/*===========================================================================

                     PROTOTYPES FOR LOCAL FUNCTIONS

===========================================================================*/
static   void    l1_isr(gas_id_t gas_id);
static   void    l1_isr_inactive_mode( gas_id_t gas_id );
static   void    l1_isr_reset_mode( gas_id_t gas_id );
static   void    l1_isr_null_mode( gas_id_t gas_id );
static   void    l1_isr_find_bcch( gas_id_t gas_id );
static   void    l1_isr_find_sch( gas_id_t gas_id );
static   void    l1_isr_idle_mode( gas_id_t gas_id );
static   void    l1_isr_random_access_mode( gas_id_t gas_id );
static   void    l1_isr_dedicated_mode( gas_id_t gas_id );
static   void    l1_isr_handle_find_bcch_command( boolean  apply_freq_correction, gas_id_t gas_id );
static   void    l1_isr_handle_find_sch_command( boolean  apply_freq_correction, gas_id_t gas_id );
static   void    l1_isr_jump_to_sdcch( gas_id_t gas_id );
static   boolean    gl1_deactivate( gas_id_t gas_id );
extern   void    gl1_begin_INTERRAT_Reconnect(gas_id_t gas_id);
extern   void    gl1_begin_INTERRAT_Handover(gas_id_t gas_id);
#ifndef FEATURE_GL1_GPLT
void    l1_isr_run_gps_tick_handler( gas_id_t gas_id );
#endif
#if defined (FEATURE_G2X_TUNEAWAY) || defined ( FEATURE_GPRS_GBTA)
void    l1_isr_transfer_mode( gas_id_t gas_id );
#else
static   void    l1_isr_transfer_mode( gas_id_t gas_id );

#endif
static   void    L1_do_background_HPLMN_search( gas_id_t gas_id );
static   void    l1_state_log( boolean start_of_isr, gas_id_t gas_id );
#ifdef FEATURE_GSM_DTM
static   void    l1_isr_dtm_mode( gas_id_t gas_id );
#endif

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
void    l1_isr_wtog_bplmn_mode( gas_id_t gas_id );
#else
static   void    l1_isr_wtog_bplmn_mode( gas_id_t gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
static   boolean l1_do_WTOG_BPLMN_search( gas_id_t gas_id );

/* CR#: 262337. Arbitrarily set to 70ms(~15*4.615)
   which is below current RRC abort timer val of 100ms
 */
#define L1_MAX_DRX_INTERVAL_VAL   15

/* 
  while attempting FCCH decode , we require min. 20 frames
	12 for decode window, 3+3 for init/deinit    
        2 Frames for release Overhead
        20 * 4.615 = 92.3 msec
  Hence Rounding and setting min. duration here to 100 msec
*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#define MIN_USEFUL_BPLMN_DURATION_MS 100
#define MIN_USEFUL_BPLMN_DURATION_FRAMES (MIN_USEFUL_BPLMN_DURATION_MS/4.615)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#endif

#ifdef FEATURE_GSM_DTM
static char* dtm_state_array[] = {
  "L1_DTM_NULL",
  "L1_DTM_RUN",
  "L1_DTM_PS_RECONFIG",
  "L1_DTM_CS_RECONFIG",
  "L1_DTM_CS_PS_RECONFIG",
  "L1_DTM_CS_PS_ABORT"
#ifdef FEATURE_GSM_EDTM
  ,"L1_DTM_CS_ABORT"
  ,"L1_DTM_CS_ABORTED_WAIT_FOR_END_OF_BLOCK"
  ,"L1_DTM_CS_ABORT_PS_RECONFIG"
#endif
};
#endif

#ifdef FEATURE_GPRS_GBTA
uint8 gbta_count[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };
#endif

/* Default RACH guard time */
uint32 l1_max_rach_guard_time[NUM_GERAN_DATA_SPACES] =
         { INITIAL_VALUE( L1_MAX_RANDOM_ACCESS_MODE_FRAMES ) };

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
boolean switch_ds_task_pointers = TRUE;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */



#define GL1_STATE_LOG_INIT { 0, 0, 0, 0, 0, FALSE }

static l1_state_log_T current_values[NUM_GERAN_DATA_SPACES] =
  { INITIAL_VALUE(GL1_STATE_LOG_INIT) };

l1_isr_data_t  l1_isr_data[NUM_GERAN_DATA_SPACES];

static void   l1_isr_channel_transition( boolean speedup, gas_id_t gas_id );

boolean l1_sc_balist_check_cell( gas_id_t gas_id, cell_T*  cell_ptr);

#ifdef FEATURE_GTA_2PHASE_ACCESS
extern void gpl1_gprs_reset_G2T_2phase_state_machine(gas_id_t gas_id);
#endif

extern boolean l1_get_non_drx_timer_flag(gas_id_t gas_id);
/*===========================================================================

FUNCTION init_l1_isr_data

DESCRIPTION
             Initializes isr data values .

PARAMS
             None.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
void init_l1_isr_data( gas_id_t gas_id )
{
  memset ( &l1_isr_data[gas_id], 0 , sizeof ( l1_isr_data_t ) );
  l1_isr_data[gas_id].starting_time_elapsed = FALSE;
  l1_isr_data[gas_id].l1_access_isr_send_mac_sb_failure = FALSE;
#ifdef FEATURE_GSM_DTM
  l1_isr_data[gas_id].dtm_command_event = L1_DTM_NULL_COMMAND;
  l1_isr_data[gas_id].old_l1_dtm_mode_state = L1_DTM_NULL;
  l1_isr_data[gas_id].l1_dtm_mode_state = L1_DTM_NULL;
#endif
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
  l1_isr_data[gas_id].leaving_bplmn = FALSE;
  l1_isr_data[gas_id].l1_bplmn_suspend_in_progress = FALSE;
#endif
}


l1_dtm_command_event_t l1_get_dtm_command_event( gas_id_t gas_id )
{
  l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];
  return l1_isr_data_ptr->dtm_command_event;
}
void l1_set_dtm_command_event(l1_dtm_command_event_t dtm_command_event,gas_id_t gas_id)
{
  l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];
  l1_isr_data_ptr->dtm_command_event=dtm_command_event;
}

boolean l1_get_bplmn_suspend_in_progress( gas_id_t gas_id )
{
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
  l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];
  return l1_isr_data_ptr->l1_bplmn_suspend_in_progress;
#else
  return FALSE;
#endif /* FEATURE_WTOG_BACKGROUND_PLMN_SEARCH */
}
void l1_set_bplmn_suspend_in_progress(boolean l1_bplmn_suspend_in_progress,gas_id_t gas_id)
{
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
  l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];
  l1_isr_data_ptr->l1_bplmn_suspend_in_progress = l1_bplmn_suspend_in_progress;
#endif /* FEATURE_WTOG_BACKGROUND_PLMN_SEARCH */

  return;
}

void l1_set_access_isr_send_mac_sb_failure( boolean value,gas_id_t gas_id )
{
  l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];

  l1_isr_data_ptr->l1_access_isr_send_mac_sb_failure = value;
}

boolean l1_get_starting_time_elapsed( gas_id_t gas_id )
{
  l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];

  return l1_isr_data_ptr->starting_time_elapsed;
}

void l1_set_starting_time_elapsed( boolean value,gas_id_t gas_id )
{
  l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];

  l1_isr_data_ptr->starting_time_elapsed = value;
}

boolean l1_get_access_isr_send_mac_sb_failure( gas_id_t gas_id )
{
  l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];

  return l1_isr_data_ptr->l1_access_isr_send_mac_sb_failure;
}



/*===========================================================================

                     PROTOTYPES FOR EXTERNAL FUNCTIONS

===========================================================================*/

/* These should be moved to a header file. */
extern void L1_pscan_resume ( gas_id_t gas_id );
extern void L1_pscan_suspend ( void* dummy, gas_id_t gas_id );
extern boolean L1_background_HPLMN_search_running( gas_id_t gas_id );
extern byte L1_get_last_idle_sub_state ( gas_id_t gas_id );
extern void L1_set_to_sdcch_mode(gas_id_t gas_id);
extern void l1_call_panic_reset_in_task( l1_panic_reset_type_T error_code, gas_id_t gas_id );
extern void gl1_setup_new_channel ( gas_id_t gas_id );

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
extern boolean l1_get_gsm_stop_req_in_queue( gas_id_t gas_id );
extern void l1_set_gsm_stop_req_in_queue(boolean gsm_stop_req_in_queue, gas_id_t gas_id);
#endif /* FEATURE_WTOG_BACKGROUND_PLMN_SEARCH */

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
/* function to perform power avg in task */
extern void  gpl1_call_in_task_do_time_pwr_avg(gas_id_t gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

extern void l1_tick_cbch( gas_id_t gas_id );

/*===========================================================================

FUNCTION  L1_TDMA_1_ISR

DESCRIPTION
  The Layer 1 ISR that triggers the L1 Frame Handler task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  l1_tdma_1_ISR( sys_modem_as_id_e_type as_id )
{
   /* Main TDMA Interrupt */
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

   gas_id = check_gas_id(gas_id);

   /* Tick DRX manager.  To assist debugging the return value of this function
    * indicates whether the rest of the ISR should run or not.
    */
   if ( gl1_drx_tick(gas_id) )
   {
      /* Call the main processing function for layer 1 interrupts. */
      l1_isr(gas_id);

      if ( gstmr_get_asynch_int_executing(gas_id) )
      {
        /* Only SS mode here */
        if ( !gl1_msg_get_multi_sim_mode() )
        {
          if ( !rexl4_queue_dpc( (rex_apc_func_type)gl1_drx_post_tick_dpc,gas_id ))
          {
            MSG_GERAN_ERROR_1_G( "Failed to queue drx_post_tick DPC",
                       0);

            /* Maybe we can go to sleep now */
            gl1_drx_post_tick( FALSE, gas_id );
          }
        }
      }
      else
      {
        /* Maybe we can go to sleep now */
        gl1_drx_post_tick( FALSE, gas_id );
      }

      if (first_awake_frame_after_sleep[gas_id] == TRUE)
      {
          first_awake_frame_after_sleep[gas_id] = FALSE;
      }
   }

   return;
}


#ifndef FEATURE_GL1_GPLT

/*===========================================================================

FUNCTION  l1_isr_run_gps_tick_handler

DESCRIPTION
  Run gps tick handler if FN/QS can be read properly

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  l1_isr_run_gps_tick_handler( gas_id_t gas_id )
{
   volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   if( (!l1_tskisr_blk->l1_isr_called_from_sleep_warmup_isr)
    && (!vstmr_fnqs_rd_disabled_for_slamming(gas_id)) )
   {
     /* FN/QS can be read correctly, so send any pending ttg requests now */
     gl1_timetag_gps_tick( gas_id );
   }

   /* Special SIG to indicate this DPC request has completed */
   (void)rex_set_sigs( l1_task_tcb_read( (gas_id_t)gas_id ),
                       GL1_WAIT_FOR_DPC_TO_FINISH );
}
#endif /* !FEATURE_GL1_GPLT */

/*===========================================================================

FUNCTION  L1_ISR

DESCRIPTION
  The Layer 1 ISR Handler entry point

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_isr( gas_id_t gas_id )
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
   l1_isr_data_t *l1_isr_data_ptr;
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

   l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_isr_data_ptr = &l1_isr_data[gas_id];

   gas_id = check_gas_id(gas_id);

   l1_state_log( TRUE,gas_id );

#ifdef QTA_EVENT_DATA_LOGGING 
   gl1_hw_cm_check_delay_after_qta_startup(gas_id);
#endif /* QTA_EVENT_DATA_LOGGING */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /*Additional Debug to see DR status inside GSM*/
   if (gl1_drds_debug( gas_id)){

      MSG_GERAN_LOW_1_G("DRDS_DBG: G_internal_info for DR capability: %d",grm_gl1_get_dr_status(gas_id));
   }
#endif

   /* Kick the ISR watchdog */
   l1_isr_watchdog_kick(gas_id);

#ifdef FEATURE_DUAL_DATA
   if (gas_id == gl1_get_low_priority_data_sub() )
   {
     if ( (gl1_cfcm_data.gl1_mms_cfcm_state == GL1_GFCM_TIMER_ACTIVE) && (gl1_cfcm_data.gl1_mms_gfn_timer == GSTMR_GET_FN_GERAN(gas_id)) )
     {
        MSG_GERAN_HIGH_0_G("GL1: Send MPH_COEX_MSC_CHANGE_IND Tx/RX=1");
        L1_send_MPH_COEX_MSC_CHANGE_IND(1,1,gas_id);
        /* now change state to WAIT for MMS_TO_FINISH so we can move MSC back to default*/
        gl1_cfcm_data.gl1_mms_cfcm_state = GL1_CFCM_TIMER_EXPIRED;
        gl1_cfcm_data.gl1_mms_gfn_timer = L1_GPRS_INVALID_FN;
     }
   }
#endif

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
  /* update approx every 100ms */
  if (frame_counters[gas_id].FNmod8 == 0)
  {
    /* need to check only tx as rx would also be active */
    if (gpl1_get_tx_pwr_avg_active_state(gas_id) )
    {
      /* Perform average in task time */
       gpl1_call_in_task_do_time_pwr_avg(gas_id);
    }
  }
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_GSM_TDS
   /* if there is a pending startup cmd to issue then check
    * if its due & issue if needed
    */
   if (mdsp_decrement_startup_time_offset_flag[gas_id])
   {
       gl1_hw_g2t_startup_decrement(gas_id);
   }

   /* if there is a pending startup cmd to issue then check
    * if its due & issue if needed
    */
   if (mdsp_decrement_cleanup_time_offset_flag[gas_id])
   {
       gl1_hw_g2t_cleanup_decrement(gas_id);
   }
#endif

#ifdef FEATURE_GSM_COEX_SW_CXM
   /* Notify the arbitrator that L1 ISR state is about to be ticked */
   garb_intf_notify_gstmr_tick(gas_id);
   /* get CXM logs from firmware */
   mdsp_process_cxm_logs(gas_id);
#endif

#ifdef FEATURE_GL1_GPLT
   MSG_GERAN_HIGH_1_G("GPLT DBG: L1 state %d", l1_tskisr_blk->l1_state);
#endif

#ifdef FEATURE_GSM_TO_LTE
  gl1_check_if_g2l_init_stuck(gas_id);
#endif

  if ( gl1_msg_get_multi_sim_mode() )
  {
      /* GL1 checks if GSM holds tx device without rx device. If so GL1 will force
         an err_fatal inside the function. This is for debug.
      */
      grm_validate_tx_chain_grant_status(l1_tskisr_blk->client_id, gas_id);
   }
   switch ( l1_tskisr_blk->l1_state )
   {
      case L1_INACTIVE_MODE:
         l1_isr_inactive_mode(gas_id);
         break;

      case L1_RESET_MODE:
         l1_isr_reset_mode(gas_id);
         break;

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
      case L1_SUSPEND_MODE:
         l1_isr_suspend_mode(gas_id);
         break;
#endif

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
      case L1_WTOG_BPLMN_MODE:
         l1_isr_wtog_bplmn_mode(gas_id);
         break;
#endif

      case L1_NULL_MODE:
         l1_isr_null_mode(gas_id);
         break;

      case L1_FIND_BCCH:
         l1_isr_find_bcch(gas_id);
         break;

      case L1_FIND_SCH:
         l1_isr_find_sch(gas_id);
         break;

      case L1_IDLE_MODE:
         l1_isr_idle_mode(gas_id);
         break;

      case L1_RANDOM_ACCESS_MODE:
         l1_isr_random_access_mode(gas_id);
         break;

      case L1_DEDICATED_MODE:
#ifdef FEATURE_GSM_ISR_PROFILING
         GL1_HANDLER_PERFORMANCE_LOGGING(START_DEDICATED_MODE_HANDLER,gas_id);
#endif /* FEATURE_GSM_ISR_PROFILING */
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
         #error code not present
#endif
         l1_isr_dedicated_mode(gas_id);
#ifdef FEATURE_GSM_ISR_PROFILING
         GL1_HANDLER_PERFORMANCE_LOGGING(STOP_DEDICATED_MODE_HANDLER,gas_id);
#endif /* FEATURE_GSM_ISR_PROFILING */
         break;

      case L1_TRANSFER_MODE:
#ifdef FEATURE_GSM_ISR_PROFILING
         GL1_HANDLER_PERFORMANCE_LOGGING(START_TRANSFER_MODE_HANDLER,gas_id);
#endif /* FEATURE_GSM_ISR_PROFILING   */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         if(l1_tskisr_blk->time_slot_change_pending)
         {
           l1_tskisr_blk->time_slot_change_pending = FALSE;
         }
#endif
         l1_isr_transfer_mode(gas_id);
 #ifdef FEATURE_GSM_ISR_PROFILING
         GL1_HANDLER_PERFORMANCE_LOGGING(STOP_TRANSFER_MODE_HANDLER,gas_id);
#endif /* FEATURE_GSM_ISR_PROFILING  */
         break;

#ifdef FEATURE_GSM_DTM
      case L1_DTM_MODE:
 #ifdef FEATURE_GSM_ISR_PROFILING
         GL1_HANDLER_PERFORMANCE_LOGGING(START_DTM_MODE_HANDLER,gas_id);
#endif /* FEATURE_GSM_ISR_PROFILING  */
         l1_isr_dtm_mode(gas_id);
#ifdef FEATURE_GSM_ISR_PROFILING
         GL1_HANDLER_PERFORMANCE_LOGGING(STOP_DTM_MODE_HANDLER,gas_id);
#endif /* FEATURE_GSM_ISR_PROFILING  */
         break;
#endif
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    case L1_MULTI_SIM_NULL:
         l1_isr_multi_sim_null(gas_id);
         break;

    case L1_MULTI_SIM_IDLE:
         l1_isr_multi_sim_idle(gas_id);
         break;

    case L1_MULTI_SIM_FIND_BCCH:
         l1_isr_multi_sim_find_bcch(gas_id);
         break;

    case L1_MULTI_SIM_INACTIVE:
         gl1_drx_require_next_tick(gas_id);
         (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
         MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_INACTIVE panic reset state");
         l1_isr_multi_sim_inactive( gas_id );
         break;
   case L1_MULTI_SIM_DECOUPLED:
         (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
         l1_isr_multi_sim_decoupled(gas_id);
         break;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

      default:
         MSG_GERAN_ERROR_1_G("Bad L1 state %u", (word)l1_tskisr_blk->l1_state);
         break;
   }

#ifdef FEATURE_GPRS_GBTA
   if(l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE)
   {
     if(( GET_GBTA_STATE() == GRM_GBTA_ACTIVE)
      &&!((l1_idle_data_store[gas_id].paging_data.paging_mode == PAGING_REORGANIZATION) ||
         (l1_idle_data_store[gas_id].paging_data.paging_mode == EXTENDED_PAGING) ||
         (l1_idle_data_store[gas_id].ext_BCCH_mask != 0)||
         (l1_idle_data_store[gas_id].mm_non_drx_mode)||
         (l1_get_non_drx_timer_flag(gas_id))
        )
       )
     {
       if(++gbta_count[gas_id] == L1_MAX_GBTA_FRAMES_NOT_IN_TRANSFER)
       {
#ifdef FEATURE_QSH_MDUMP
         QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
         ERR_GERAN_FATAL_1_G("GBTA is in active when it is not supposed to be. fn:%d",
                             frame_counters[gas_id].FNmod102);
       }
       /*With CR993771 changes idle sub goes to sleep when other G sub in transfer mode*/
       /*added next drx tick to prevent sleep and capture the error fatal*/
       if(gl1_is_any_sub_in_transfer())
       {
         gl1_drx_require_next_tick(gas_id);
       }
     }
     else
     {
       gbta_count[gas_id] = 0;
     }
   }
#endif

/* Check whether G2X IRAT needs to be enabled or disabled */
     if(((check_irat_active[gas_id] & (1 << MODE_CHANGE_REQ_RECEIVED)) != 0) || ((check_irat_active[gas_id] & (1 << UPDATE_LIST_REQ_RECEIVED)) != 0))
   {
     check_irat_active[gas_id] &= ~(1 << MODE_CHANGE_REQ_RECEIVED);
     check_irat_active[gas_id] &= ~(1 << UPDATE_LIST_REQ_RECEIVED);

     l1_sc_irat_check_active( l1_sc_wcdma_data_ptr->sc_wcdma_count, l1_sc_wcdma_data_ptr->rr_params.num_L_freqs, TRUE, l1_sc_wcdma_data_ptr->sc_tds_count,TRUE, gas_id);
   }
 
   /* GFW now handles CONFIGURE_IR_COMMAND in APP_MODE_ACTIVE and MEAS_IDLE
      It shall act on the config parameter set as 
      1. MDSP_CFG_IR_WINDOW_SET  --> 	STATE !=  GFW_APP_MODE_ACTIVE	-> Exception in GFW
      2. MDSP_CFG_IR_RELEASE	-->  APP_MODE_ACTIVE,MEAS_IDLE       -> FREE IR if ALLOCATED. Otherwise IGNORE
   */
   mdsp_gprs_issue_IR_configure_memory( as_id );

#ifndef FEATURE_GL1_GPLT
   /* Tick the frequency error update engine! */
   gl1_hw_cgps_feu_tick( (gas_id_t)gas_id );

   l1_isr_run_gps_tick_handler(gas_id);
#endif

#ifdef FEATURE_GSM_COEX_SW_CXM
   /* Notify the arbitrator that L1 ISR state processing is complete */
   garb_intf_notify_gstmr_post_tick(l1_tskisr_blk->l1_prev_state, l1_tskisr_blk->l1_state, gas_id);
#endif

   /* Send frequency update command to GFW for cycle swallower update */
   /* Do not add command if its the only one in the buffer */
   /*  - this can delay sleep by 1 frame */
   /*  - may mean that there has been ISR overrun, so command would be lost */
   if( mdsp_commands_pending(gas_id) )
   {
      gl1_hw_gfw_config_cycle_swallower(gas_id);
   }

   mdsp_rf_send_iq_samples_to_rf( as_id );

   /* Catch the case where we get stuck in NULL mode */

   if (l1_tskisr_blk->l1_state == L1_NULL_MODE)
   {
      if (++l1_isr_data_ptr->l1_frames_in_null_mode > L1_MAX_NULL_MODE_FRAMES)
      {
         l1_isr_data_ptr->l1_frames_in_null_mode = 0;

       if ( ftm_get_mode() == FTM_MODE )
        {
             MSG_GERAN_HIGH_0_G("Stuck in NULL mode in FTM");
        }
       else
        {

          if(geran_get_nv_recovery_restart_enabled(gas_id))
          {
           MSG_GERAN_ERROR_0_G("L1 stuck in null mode");
           l1_call_panic_reset_in_task(L1_STUCK_IN_NULL_MODE, gas_id);
          }
          else
          {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_GERAN_FATAL_0_G("L1 stuck in null mode");
          }
        }
   }
   }
   else
   {
      l1_isr_data_ptr->l1_frames_in_null_mode = 0;
   }

   /* Catch the case where we get stuck in FIND_BCCH mode */
   if (l1_tskisr_blk->l1_state == L1_FIND_BCCH)
   {
      if (++l1_isr_data_ptr->l1_frames_in_find_bcch > L1_MAX_FIND_BCCH_FRAMES)
      {
        l1_isr_data_ptr->l1_frames_in_find_bcch = 0;
        if ( ftm_get_mode() == FTM_MODE )
        {
         MSG_GERAN_HIGH_0_G("Stuck in FIND BCCH mode in FTM");
        }
        else
        {
         if(geran_get_nv_recovery_restart_enabled(gas_id))
      {
         MSG_GERAN_ERROR_0_G("L1 stuck in FIND_BCCH mode");
         l1_call_panic_reset_in_task(L1_STUCK_IN_FIND_BCCH, gas_id);
      }
         else
         {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_GERAN_FATAL_0_G("L1 stuck in FIND_BCCH mode");
         }
        }
      }
   }
   else
   {
      l1_isr_data_ptr->l1_frames_in_find_bcch = 0;
   }


   /* Catch the case where we get stuck in ACCESS mode */
   if (l1_tskisr_blk->l1_state == L1_RANDOM_ACCESS_MODE)
   {
      if (++l1_isr_data_ptr->l1_frames_in_random_access_mode > l1_max_rach_guard_time[gas_id])
      {
        l1_isr_data_ptr->l1_frames_in_random_access_mode = 0;
        if ( ftm_get_mode() == FTM_MODE )
        {
          MSG_GERAN_HIGH_0_G("Stuck in random access mode in FTM");
        }
        else
        {
         if(geran_get_nv_recovery_restart_enabled(gas_id))
         {
           MSG_GERAN_ERROR_0_G("L1 stuck in random access mode");
           l1_call_panic_reset_in_task(L1_STUCK_IN_ACCESS_MODE, gas_id);
         }
         else
         {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_GERAN_FATAL_0_G("L1 stuck in random access mode");
         }
        }
      }
   }
   else
   {
      l1_isr_data_ptr->l1_frames_in_random_access_mode = 0;
   }

/*Check if previous state is either DEDICATED/Packet transfer mode and current state is neither of the two*/
  if(((l1_tskisr_blk->l1_state != L1_DEDICATED_MODE) && (l1_tskisr_blk->l1_state != L1_TRANSFER_MODE)) &&
    ((l1_tskisr_blk->l1_prev_state == L1_DEDICATED_MODE) || (l1_tskisr_blk->l1_prev_state == L1_TRANSFER_MODE)))
   {
     static uint8 count_check[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };

     /*If the state transition has just happened then clear the count*/
     if(gl1_flag_state_change[gas_id] == TRUE)
     {
       gl1_flag_state_change[gas_id] = FALSE;
       count_check[gas_id] = 0;
     }
     /*Check if either the ded_state or id_in_tbf state is not NULL*/
     if((l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL) &&
        ((l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_DEDICATED) || (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_ID_IN_TBF)))
     {
       MSG_GERAN_HIGH_2_G("srch_state %d count %d", l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,count_check[gas_id]);

       /*Increment the count*/
       count_check[gas_id]++;

       /*If either ded_state or id_in_tbf state doesn't change to NULL 25 frames even after state transition then crash*/
       if(count_check[gas_id] == NO_FRAMES_TO_CHECK_IF_GL1_IS_STUCK)
       {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_3_G("Crash as ded/id_in_tbf state is still not NULL",0,0,0);
       }
     }
     /*Both ded and id_in_tbf states are NULL, SO clear the counter and state change flag*/
     else
     {
       count_check[gas_id] =0;
       gl1_flag_state_change[gas_id] = FALSE;
     }
   }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* Tick transaction manager to Release Transactions*/
   grm_tick_transaction_manager(l1_tskisr_blk->client_id, gas_id);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

   l1_sc_forced_meas_expiry(FALSE, gas_id);  /* check expiry in any state */
   if (grm_get_trm_requested_at_warmup(l1_tskisr_blk->client_id, gas_id))
   {
	 MSG_GERAN_HIGH_0_G("Clear GRM request at warmup flag at end of isr/warmup isr");
	 grm_set_trm_requested_at_warmup(FALSE, l1_tskisr_blk->client_id, gas_id);
   }

   l1_state_log( FALSE,gas_id );

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
   #error code not present
#endif
}


#ifdef FEATURE_GPRS_GBTA
/*===========================================================================

FUNCTION  l1_reset_gbta_active_count

DESCRIPTION
  THis function resets the Gbta active count used for debug purpose

DEPENDENCIES
  none

RETURN VALUE
 none

SIDE EFFECTS
  None

===========================================================================*/

void l1_reset_gbta_active_count ( gas_id_t gas_id )
{
  gbta_count[gas_id] = 0;
}
#endif


#ifdef FEATURE_GSM_DTM
/*===========================================================================

FUNCTION  get_dtm_isr_state

DESCRIPTION
  Returns the current state of the L1 dtm ISR state machine.

DEPENDENCIES
  l1_dtm_mode_state

RETURN VALUE
  l1_dtm_mode_state_t

SIDE EFFECTS
  None

===========================================================================*/
l1_dtm_mode_state_t get_dtm_isr_state( gas_id_t gas_id )
{
  l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];

  return l1_isr_data_ptr->l1_dtm_mode_state;
}

/*===========================================================================

FUNCTION  l1_isr_dtm_mode

DESCRIPTION
  Top level state machine for controlling DTM mode. The event DTM_COMMAND_EVENT
  is used to tick the dtm controlling function when set to EV_NULL_COMMAND, and
  transitions to different states when other values are set.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_isr_dtm_mode( gas_id_t gas_id )
{
volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
dtm_data_T  *dtm_data = l1_tskisr_blk->current_params.L1Data.pDtm_data;
l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];

transfer_data_T *transfer_data_ptr = l1_tskisr_blk->next_params.L1Data.pTransfer_data;
static boolean dtm_transfer_release_confirm_on_exit[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };

   /*For logging*/
   l1_dtm_command_event_t dtm_event = l1_isr_data_ptr->dtm_command_event;
   l1_dtm_mode_state_t dtm_state =  l1_isr_data_ptr->l1_dtm_mode_state;
   l1_dtm_mode_T       dtm_abort_mode = dtm_data->l1_dtm_abort_mode;

   /* Set this signal  to ensure L1 task is
      not locked out permenantly */
   (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);

   l1_isr_G2L_trigger(gas_id);

  /********************************************************
  * L1_DTM_MODE stateless processing                      *
  * --------------------------------                      *
  * This section should contain all the processing that   *
  * is state independant...                               *
  *********************************************************/

  /* STATELESS EVENT HANDLING */

  if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_RESET )
  {
    /*Reset the state machine and any related variables*/
    l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_NULL;
  }
  else
  if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_TRANSFER_STOP )
  {
    MSG_GERAN_MED_0_G("primed for release conf");
    dtm_transfer_release_confirm_on_exit[gas_id] = TRUE;
  }
  else
  /****************************************************************/
  /* Abort DTM mode, waiting for any NPL1 and SCE tasks to finish */
  /****************************************************************/
  if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_STOP )
  {
    l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

    l1_tskisr_blk->main_command = L1_DEDICATED_STOP;

    /* Set the abort mode so that l1_dtm_control() knows to abort CS and PS */
    dtm_data->l1_dtm_abort_mode = L1_DTM_CS_PS;

    if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id), gas_id) )
    {
      /* Abort complete leave DTM mode */
      l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_NULL;
    }
    else
    {
      /* Move to L1_DTM_CS_PS_ABORT to wait for abort to complete */
      l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_PS_ABORT;

      /* Set the dtm_command_event again !
       * gpl1_dtm_control has alread been called once.
       * Need to make sure this does not happen again in
       * this TDMA frame.
       */
      l1_isr_data_ptr->dtm_command_event = L1_DTM_STOP;
    }
  }
  /*********************************************************/
#ifdef FEATURE_GSM_EDTM
  else
  if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_ENHANCED_RELEASE )
  {

    l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

    /* Start to abort the CS channels only */
    l1_tskisr_blk->main_command = L1_DEDICATED_STOP;
    dtm_data->l1_dtm_abort_mode = L1_DTM_CS_ONLY;

    /* Move to L1_DTM_CS_PS_ABORT to wait for abort to complete */
    l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_ABORT;
  }
#endif

  switch ( l1_isr_data_ptr->l1_dtm_mode_state )
  {

  /********************************************************
  * L1_DTM_NULL state                                     *
  * ------------------                                    *
  * This is the inactive state. Nothing will happen until *
  * the correct activation event is set, causing the fsm  *
  * to begin operations and mode to the running state     *
  *********************************************************/
  case L1_DTM_NULL:
    {
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_ACTIVATE )
      {

        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* SCE setup - Startup the DTM mode and state machine*/
        l1_sc_start(L1SCModeDediDTM, gas_id);

        (void) gpl1_dtm_control(gas_id);

        l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;

      }
      else
      {
        MSG_GERAN_ERROR_1_G("L1_DTM_NULL unsupported event %d",l1_isr_data_ptr->dtm_command_event);
      }
    }
    break;

  /********************************************************
  * L1_DTM_RUN state                                      *
  * ------------------                                    *
  * DTM is operational and is running. The main control   *
  * function is ticked until an event causing a transition*
  * is processed                                          *
  *********************************************************/
  case L1_DTM_RUN:
    {
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_NULL_COMMAND )
      {
        /* Just tick the controlling function */
        (void) gpl1_dtm_control(gas_id);

      }
      else
      /****************************************************************/
      /* There is a change to the PS allocation                       */
      /****************************************************************/
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_TRANSFER_RECONFIG )
      {

        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* Is there a starting time and if so has it expired on this frame, if so
        do the reconfig and don't move to the L1_DTM_PS_RECONFIG state */
        if ( transfer_data_ptr->starting_time_valid )
        {
          uint32 current_fn = GSTMR_GET_FN_GERAN( gas_id) ;

          if( transfer_data_ptr->starting_time <=
                                          ADD_FN(current_fn,(NPL1_FN_OFFSET + 2) ) )
          {
            l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;
          }
          else
          {
            /* Move to the L1_DTM_PS_RECONFIG until the reconfig starting time */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_PS_RECONFIG;
            MSG_GERAN_MED_3_G("L1_DTM_PS_RECONFIG curr:%d recon:%d [%d]",
              transfer_data_ptr->starting_time,
              current_fn,
              ADD_FN(current_fn,(NPL1_FN_OFFSET + 2))
              );
          }
        }
        else
        {
          l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;
        }

        if ((l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR) &&
            (l1_transfer_data[gas_id].wait_for_pca_valid))
        {
          MSG_GERAN_HIGH_0_G("rel confirm isr DTM_RUN ");

          /*Remove any MAC_L1_RELEASE from the rr_mac message store in case
          initially this was a full release, that has become a partial release */
          gpl1_dtm_invalidate_mac_l1_release_from_message_store(gas_id);

          /*  Indicate to MAC release procedure is complete */
          gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending, gas_id);

          /* reset relelase type and rel confirm status*/
          l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
          l1_transfer_data[gas_id].confirm_pending = SENT;

          /* Clear wait for pca vars */
          transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
          transfer_data_ptr->wait_for_pca_valid = FALSE;
        }

        /* Tick the control function */
        (void) gpl1_dtm_control(gas_id);

      }
      else
      /****************************************************************/
      /* Abort the PS service and then cease DTM                      */
      /****************************************************************/
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_TRANSFER_STOP )
      {

        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* Set the abort mode so that l1_dtm_control() knows to abort just the PS */
        dtm_data->l1_dtm_abort_mode = L1_DTM_PS_ONLY;

        if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id), gas_id ))
        {
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_NULL;
        }
        else
        {
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_PS_ABORT;
        }

      }
      else
      /****************************************************************/
      /* The CS needs reconfiguring due to a channel mode modify      */
      /****************************************************************/
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_DEDICATED_RECONFIG )
      {

        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* Set the abort mode so that l1_dtm_control() knows to abort just the CS */
        dtm_data->l1_dtm_abort_mode = L1_DTM_CS_ONLY;

        l1_tskisr_blk->main_command = L1_DEDICATED_STOP;

        if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
        {
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;
        }
        else
        {
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_RECONFIG;
        }

      }
      else
      /****************************************************************/
      /* Both the CS and PS are awaiting reconfiguration              */
      /****************************************************************/
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_RECONFIG_BOTH_CS_PS )
      {
        uint32 current_fn = GSTMR_GET_FN_GERAN( gas_id);

        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* Set the abort mode so that l1_dtm_control() knows to abort just the CS */
        dtm_data->l1_dtm_abort_mode = L1_DTM_CS_ONLY;

        l1_tskisr_blk->main_command = L1_DEDICATED_STOP;

        /* Check for the PS reconfig starting time */
        /* Is there a starting time and if so has it expired on this frame, if so
        do the reconfig and don't move to the L1_DTM_PS_RECONFIG state */
        if ( transfer_data_ptr->starting_time_valid )
        {
          if( !IS_FRAME_NUM_LATER( transfer_data_ptr->starting_time, ADD_FN(current_fn,(NPL1_FN_OFFSET+3))))
          {
            l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_RECONFIG;
          }
          else
          {
            /* Move to the L1_DTM_PS_RECONFIG until the reconfig starting time */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_PS_RECONFIG;
          }
        }
        else
        {
          l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_RECONFIG;
        }

        if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
        {
          /* channel mode modify complete. PS reconfig still required? */
          if ( l1_isr_data_ptr->l1_dtm_mode_state == L1_DTM_CS_PS_RECONFIG )
          {
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_PS_RECONFIG;
          }
          else
          {
            /* All reconfig of CS and PS complete */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;
          }
        }

      }
      else
      {
        MSG_GERAN_ERROR_1_G("L1_DTM_RUN unsupported event %d",l1_isr_data_ptr->dtm_command_event);

        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* Tick anyway to keep working */
        (void)gpl1_dtm_control(gas_id);
      }

    }
    break;

  /********************************************************
  * L1_DTM_PS_RECONFIG state                              *
  * ------------------                                    *
  * DTM is running but there is a pending change to the   *
  * PS resources. DTM should stay in this state until the *
  * reconfig takes place, or interruption by a CS action  *
  *********************************************************/
  case L1_DTM_PS_RECONFIG:
    {

      uint32 current_fn = GSTMR_GET_FN_GERAN( gas_id);

      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_NULL_COMMAND )
      {
        /* To get here there must be a valid starting time - has it expired on this
        frame, if so do the reconfig and then move states back to L1_DTM_RUN */
        if ( transfer_data_ptr->starting_time_valid == FALSE )
        {
          l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;

          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;
        }
        else
        if( !IS_FRAME_NUM_LATER(transfer_data_ptr->starting_time,ADD_FN(current_fn,(NPL1_FN_OFFSET+3)) ))
        {

          l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;

          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;

        }

        /* Tick the control function */
        (void) gpl1_dtm_control(gas_id);

      }
      else
      /****************************************************************/
      /* Abort the PS service and then cease DTM                      */
      /****************************************************************/
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_TRANSFER_STOP )
      {

        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* Set the abort mode so that l1_dtm_control() knows to abort just the PS */
        dtm_data->l1_dtm_abort_mode = L1_DTM_PS_ONLY;

        if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id ))
        {
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_NULL;
        }
        else
        {
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_PS_ABORT;
        }

      }
      else
      /****************************************************************/
      /* The CS needs reconfiguring due to a channel mode modify      */
      /****************************************************************/
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_DEDICATED_RECONFIG )
      {
        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* Set the abort mode so that l1_dtm_control() knows to abort just the CS */
        dtm_data->l1_dtm_abort_mode = L1_DTM_CS_ONLY;

        l1_tskisr_blk->main_command = L1_DEDICATED_STOP;

       /* To get here there must be a valid starting time -
        * has it expired on this frame, if so do the reconfig and then
        * move states back to L1_DTM_RUN
        */
        if( !IS_FRAME_NUM_LATER(transfer_data_ptr->starting_time,
                                            ADD_FN(current_fn,(NPL1_FN_OFFSET+3)) ))
        {

          l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;

          /* reconfig the PS and monitor to see if the CS has aborted yet */
          if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
          {
            /* CS and PS both reconfigured */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;
          }
          else
          {
            /* PS reconfigured but not the CS */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_RECONFIG;
          }

        }
        else
        {
          if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
          {
            /* CS Reconfigured not PS - stay in this state */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_PS_RECONFIG;
          }
          else
          {
            /* Neither CS or PS has been reconfigured -
             * move to L1_DTM_CS_PS_RECONFIG
             */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_PS_RECONFIG;
          }
        }

      }
      else
      /***************************************************/
      /* There has been a release from within this state */
      /***************************************************/
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_TRANSFER_RECONFIG )
      {

        if( !IS_FRAME_NUM_LATER(transfer_data_ptr->starting_time,
                                ADD_FN(current_fn,(NPL1_FN_OFFSET+3))))
        {

          l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;

          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;

        }

        if ((l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR) &&
            (l1_transfer_data[gas_id].wait_for_pca_valid))
        {
          MSG_GERAN_HIGH_0_G("rel confirm isr DTM_PS_RECONFIG");

          /*Remove any MAC_L1_RELEASE from the rr_mac message store in case
          initially this was a full release, that has become a partial release */
          gpl1_dtm_invalidate_mac_l1_release_from_message_store(gas_id);

          /*  Indicate to MAC release procedure is complete */
          gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending, gas_id);

          /* reset relelase type and rel confirm status*/
          l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
          l1_transfer_data[gas_id].confirm_pending = SENT;

          /* Clear wait for pca vars */
          transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
          transfer_data_ptr->wait_for_pca_valid = FALSE;
        }

        /* Tick the control function */
        (void) gpl1_dtm_control(gas_id);

      }
      else
      /*********************************************************/
      /* Abondon this reconfig the new Allocation is no longer */
      /* required by MAC and has been released.                */
      /*********************************************************/
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_CANCEL_PS_RECONFIG )
      {
        /* Reset the event */
        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* Clear the starting time out to prevent a new connect ind*/
        l1_transfer_data[gas_id].starting_time_valid = FALSE;
        l1_transfer_data[gas_id].starting_time_elapsed = TRUE;

        l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;

         /* Tick the control function */
        (void) gpl1_dtm_control(gas_id);

      }
      else
      {

        MSG_GERAN_ERROR_1_G("L1_DTM_PS_RECONFIG unsupported event %d",l1_isr_data_ptr->dtm_command_event);

        /* Reset the event */
        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* To get here there must be a valid starting time - has it expired on this
        frame, if so do the reconfig and then move states back to L1_DTM_RUN */
        if( !IS_FRAME_NUM_LATER(transfer_data_ptr->starting_time,
            ADD_FN(current_fn,(NPL1_FN_OFFSET + 3)) ) )
        {

          l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;

          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;

        }

        /* Tick anyway to keep working */
        (void)gpl1_dtm_control(gas_id);

      }


    }
    break;

  /********************************************************
  * L1_DTM_CS_RECONFIG state                              *
  * ------------------                                    *
  * DTM is running but the CS mode is being changed as a  *
  * result of a mph_channel_mode_modify. PS remains       *
  * unaffected.                                           *
  *********************************************************/
  case L1_DTM_CS_RECONFIG:
    {

      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_NULL_COMMAND )
      {

        if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
        {
          l1_sc_start(L1SCModeDediDTM, gas_id);

          /* Abort complete leave DTM mode */
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;
        }

      }
      else
      /****************************************************************/
      /* There is a change to the PS allocation                       */
      /****************************************************************/
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_TRANSFER_RECONFIG )
      {
        uint32 current_fn = GSTMR_GET_FN_GERAN( gas_id);
        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /*
        Is there a starting time and if so has it expired on this frame...Or
        if there is no starting time ?
        then do the PS reconfig and don't move to the L1_DTM_PS_RECONFIG state
        */
        if (
             ( ( transfer_data_ptr->starting_time_valid == TRUE ) &&
               ( transfer_data_ptr->starting_time ==
                                              ADD_FN(current_fn,(NPL1_FN_OFFSET + 2) ) )
             ) ||
             ( transfer_data_ptr->starting_time_valid == FALSE )
           )
        {

          /* This event may have been generated by a TBF release */
          if ((l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR) &&
              (l1_transfer_data[gas_id].wait_for_pca_valid))
          {
            MSG_GERAN_HIGH_0_G("rel confirm isr L1_DTM_CS_RECONFIG");

            /*Remove any MAC_L1_RELEASE from the rr_mac message store in case
            initially this was a full release, that has become a partial release */
            gpl1_dtm_invalidate_mac_l1_release_from_message_store(gas_id);

            /*  Indicate to MAC release procedure is complete */
            gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending, gas_id);

            /* reset relelase type and rel confirm status*/
            l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
            l1_transfer_data[gas_id].confirm_pending = SENT;

            /* Clear wait for pca vars */
            transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
            transfer_data_ptr->wait_for_pca_valid = FALSE;
          }

          l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;

          /* Tick the control function */
          if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
          {
            /* Abort complete leave DTM mode PS and CS reconfigured */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;
          }
          else
          {
            /* PS is reconfigured but CS isn't - stay in this state */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_RECONFIG;
          }

        }
        /*
         * See if the CS has been aborted yet and make a decision on which state to move
         * to knowing that the PS has not yet been reconfigured
         */
        else
        {
         /* Tick the control function */
          if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
          {
            /* Abort stay in this mode */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_PS_RECONFIG;
          }
          else
          {
            /* Neither CS or PS has been reconfigured */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_PS_RECONFIG;
          }
        }
      }
      else
      /****************************************************************/
      /* Abort the PS service and then cease DTM                      */
      /****************************************************************/
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_TRANSFER_STOP )
      {

        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* Set the abort mode so that l1_dtm_control() knows to abort both the PS and CS*/
        dtm_data->l1_dtm_abort_mode = L1_DTM_CS_PS;

        if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
        {
          /* In this case we now want to return to dedicated as there is no transfer ongoing */
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_NULL;
        }
        else
        {
          /* Wait for the CMM to finish and the PS abort to take place in this state
          before returing to dedicated */
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_PS_ABORT;
        }

      }
      else
      {
        MSG_GERAN_ERROR_1_G("L1_DTM_CS_RECONFIG unsupported event %d",l1_isr_data_ptr->dtm_command_event);

        /* Reset the event */
        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
        {
          /* Abort complete leave DTM mode */
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;
        }

      }

    }
    break;

  /********************************************************
  * L1_DTM_CS_PS_RECONFIG state                           *
  * ------------------                                    *
  * There is a pending PS reconfig inconjunction with a   *
  * CS reconfig.                                          *
  *********************************************************/
  case L1_DTM_CS_PS_RECONFIG:
    {

      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_NULL_COMMAND )
      {
        uint32 current_fn = GSTMR_GET_FN_GERAN( gas_id);
        /* Just tick the controlling function */
       /*
        Is there a starting time and if so has it expired on this frame...Or
        if there is no starting time ?
        then do the PS reconfig and don't move to the L1_DTM_PS_RECONFIG state
        */
        if ( transfer_data_ptr->starting_time_valid == FALSE )
        {
          l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;

          /* Tick the control function */
          if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
          {
            /* Abort complete leave DTM mode PS and CS reconfigured */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;
          }
          else
          {
            /* PS is reconfigured but CS isn't - move to CS reconfig state */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_RECONFIG;
          }
        }
        else
        if ( transfer_data_ptr->starting_time
                                    == ADD_FN(current_fn,(NPL1_FN_OFFSET + 2) ) )
        {
          l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;

          /* Tick the control function */
          if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
          {
            /* Abort complete leave DTM mode PS and CS reconfigured */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;
          }
          else
          {
            /* PS is reconfigured but CS isn't - stay in this state */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_RECONFIG;
          }
        }
        /*
         * See if the CS has been aborted yet and make a decision on which state to move
         * to knowing that the PS has not yet been reconfigured
         */
        else
        {
         /* Tick the control function */
          if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
          {
            /* Abort complete for CS now just waiting for PS reconfig */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_PS_RECONFIG;
          }
          else
          {
            /* Neither CS or PS has been reconfigured */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_PS_RECONFIG;
          }
        }
      }
      else
      /****************************************************************/
      /* Abort the PS service and then cease DTM                      */
      /****************************************************************/
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_TRANSFER_STOP )
      {

        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* Set the abort mode so that l1_dtm_control() knows to abort both the PS and CS*/
        dtm_data->l1_dtm_abort_mode = L1_DTM_CS_PS;

        if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
        {
          /* In this case we now want to return to dedicated as there is no transfer ongoing */
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_NULL;
        }
        else
        {
          /* Wait for the CMM and PS Abort to finish together
          in this state before returing to dedicated */
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_PS_ABORT;
        }

      }
      else
      /***************************************************/
      /* There has been a release from within this state */
      /***************************************************/
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_TRANSFER_RECONFIG )
      {

        uint32 current_fn = GSTMR_GET_FN_GERAN( gas_id);

        /*Check if this has been generated by a release and so a confirm is needed*/
        if ((l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR) &&
            (l1_transfer_data[gas_id].wait_for_pca_valid))
        {
          MSG_GERAN_HIGH_0_G("rel confirm isr DTM_CS_PS_RECONFIG");

          /*Remove any MAC_L1_RELEASE from the rr_mac message store in case
          initially this was a full release, that has become a partial release */
          gpl1_dtm_invalidate_mac_l1_release_from_message_store(gas_id);

          /*  Indicate to MAC release procedure is complete */
          gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending, gas_id);

          /* reset relelase type and rel confirm status*/
          l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
          l1_transfer_data[gas_id].confirm_pending = SENT;

          /* Clear wait for pca vars */
          transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
          transfer_data_ptr->wait_for_pca_valid = FALSE;
        }

        /*
        Is there a starting time and if so has it expired on this frame...Or
        if there is no starting time then do the PS reconfig */
        if ( (transfer_data_ptr->starting_time_valid == FALSE) ||
             ((transfer_data_ptr->starting_time_valid == TRUE) &&
              (transfer_data_ptr->starting_time ==
              ADD_FN(current_fn,(NPL1_FN_OFFSET + 2)))
             )
           )
        {
          l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;

          /* Tick the control function */
          if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
          {
            /* Abort complete leave DTM mode PS and CS reconfigured */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;
          }
          else
          {
            /* PS is reconfigured but CS isn't - stay in this state */
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_RECONFIG;
          }
        }
      }
      else
      {
        MSG_GERAN_ERROR_1_G("L1_DTM_CS_PS_RECONFIG unsupported event %d",l1_isr_data_ptr->dtm_command_event);

        /* Reset the event */
        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* Tick anyway to keep working */
        (void)gpl1_dtm_control(gas_id);
      }

    }
    break;

  /********************************************************
  * L1_DTM_CS_PS_ABORT state                              *
  * ------------------                                    *
  * DTM is trying to abort, and will wait here until the  *
  * control function is satisfied that NPL1 has aborted   *
  * and also the SCE                                      *
  *********************************************************/
  case L1_DTM_CS_PS_ABORT:
    {
      /****************************************************************/
      /* The CS needs reconfiguring due to a channel mode modify      */
      /****************************************************************/
      if ( l1_isr_data_ptr->dtm_command_event == L1_DTM_DEDICATED_RECONFIG )
      {

        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;

        /* Set the abort mode so that l1_dtm_control() knows to abort both the CS and PS*/
        dtm_data->l1_dtm_abort_mode = L1_DTM_CS_PS;

        l1_tskisr_blk->main_command = L1_DEDICATED_STOP;
      }

      if(  ( l1_isr_data_ptr->dtm_command_event != L1_DTM_NULL_COMMAND )
         &&( l1_isr_data_ptr->dtm_command_event != L1_DTM_STOP )
        )
      {
        MSG_GERAN_ERROR_1_G("L1_DTM_CS_PS_ABORT unsupported event %d",l1_isr_data_ptr->dtm_command_event);

        /* Reset the event */
        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;
      }

      if( L1_DTM_STOP != l1_isr_data_ptr->dtm_command_event )
      {
        /* Tick the control function and see if the abort has happened */
        if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
        {
          /* If the init mode is CS PS then go back to RUNNING this is an abort to
          reconfigure for a DTM_CHANNEL_ASSIGNMENT*/
          if ( dtm_data->l1_dtm_init_mode == L1_DTM_CS_PS )
          {
            /* SCE setup - Startup the DTM mode and state machine*/
            l1_sc_start(L1SCModeDediDTM, gas_id);

            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_RUN;
          }
          else
          {
            l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_NULL;
          }
        }
      }
      else
      {
        /* Null the DTM command event */
        l1_isr_data_ptr->dtm_command_event = L1_DTM_NULL_COMMAND;
      }
    }
    break;

#ifdef FEATURE_GSM_EDTM
  /**********************************************************
  * L1_DTM_CS_ABORT state                                   *
  * ------------------                                      *
  * Enhanced DTM Release. CS is aborting and the PS channel *
  * is maintained through the release. This state will      *
  * manage the move to TRANSFER mode                        *
  ***********************************************************/
  case L1_DTM_CS_ABORT:
    {
      boolean  blk_boundary = FALSE;
      dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

      switch( frame_counters[gas_id].FNmod13 )
      {
      case  3:
      case  7:
      case  12:



        break;

      default:

        blk_boundary = TRUE;

        break;
      }

      /* Make sure rx and tx activities are switched for this block boundary */

      if ( gpl1_dtm_complete_abort_procedure(gpl1_dtm_control(gas_id),gas_id) )
      {
        /* If abort has happened then its time to check to see if its time to move
         back to the NULL state and to TRANSER MODE. This should happen on th last
        frame of a block boundary */

        if( FALSE != blk_boundary )
        {
          uint8 ta_value;

          /* Beging the enahnced transition here*/
          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_NULL;

          /* Ensure a disable rx tx is issued before the control function
           * unless the abort modes and states already ensure that no rx / tx tasking
           * is attempted in the first place.
           */

          /* Set the transition parameters */
          l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;

          l1_change_state( L1_TRANSFER_MODE, gas_id );


#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
         /* initialise TBF suspension variables */
         gpl1_gprs_init_tuneaway_and_ncell_data( gas_id);

#else

#ifdef FEATURE_GPRS_GBTA
          /* Useful when coming out of DTM */
          gpl1_set_gbta_status(gas_id);
#endif  /*FEATURE_GPRS_GBTA */

          /* initialise TBF suspension variables */
          gpl1_gprs_init_tbf_suspension(gas_id);

#endif /*#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

          /* Do the following only if sure that no assignment or release is
           * pending. Hence need to add suitable protection.
           */

          /* Make sure the pending alloc is setup correctly
           * Might not be necessary.
           */
          if( L1_NULL_ALLOC == transfer_data_ptr->pending_alloc_type )
          {
            if(  ( L1_TRANSFER_COMMAND != l1_tskisr_blk->main_command )
               &&( L1_NO_RELEASE == l1_transfer_data[gas_id].release_pending )
              )
            {
              transfer_data_ptr->pending_alloc_type = transfer_data_ptr->current_alloc_type;
            }
          }

          if( NULL != transfer_data_ptr->dl_tbf_ptr )
          {
            if( NULL == transfer_data_ptr->pending_dl_tbf_ptr )
            {
              transfer_data_ptr->pending_dl_tbf_ptr = transfer_data_ptr->dl_tbf_ptr;
            }
          }

          if( NULL != transfer_data_ptr->dynamic_tbf_ptr )
          {
            transfer_data_ptr->dynamic_tbf_ptr->allow_one_rb_gap = TRUE;
            transfer_data_ptr->dynamic_tbf_ptr->starting_time    = GSTMR_GET_FN_GERAN( gas_id );

            if( NULL == transfer_data_ptr->pending_dynamic_tbf_ptr )
            {
              transfer_data_ptr->pending_dynamic_tbf_ptr = transfer_data_ptr->dynamic_tbf_ptr;
            }
          }

          /* Set both the current and pending frequency pointers */
          transfer_data_ptr->frequency_info_1.frequency_list = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.frequency_list;
          transfer_data_ptr->frequency_info_1.hopping_flag   = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.hopping_flag;
          transfer_data_ptr->frequency_info_1.hsn            = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.HSN;
          transfer_data_ptr->frequency_info_1.maio           = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.MAIO;
          transfer_data_ptr->frequency_info_1.tsc            = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.TSC;

          transfer_data_ptr->frequency_info_ptr         = &( transfer_data_ptr->frequency_info_1 );
          transfer_data_ptr->pending_frequency_info_ptr = transfer_data_ptr->frequency_info_ptr;

          /* Make sure the clocking scheme is changed back to 61 (gprs) or 122 (egprs)
           * Due to different camp on strategies being employed in dtm and transfer modes.
           */
#ifdef FEATURE_GSM_MDSP_ESAIC
          /* With ESAIC we need to clock at the higher rate even when we leave DTM */
          if( TBF_MODE_GPRS == transfer_data_ptr->egprs_or_gprs_tbf )
          {
            gl1_msg_set_egprs_mode( FALSE, FALSE ,gas_id);
            MSG_HIGH("ESAIC supported - NOT deregistering EDGE clk",0,0,0);
          }
#else
          if( TBF_MODE_GPRS == transfer_data_ptr->egprs_or_gprs_tbf )
          {
            gl1_msg_set_egprs_mode( FALSE, TRUE,gas_id );
          }
#endif


          if(  ( L1_CS_TN_SLAM_TN2 == l1_dedicated_data_ptr->l1_dtm_slam_mode )
             &&( l1_get_current_timeslot(gas_id) > 5 )
            )
          {
            gpl1_decrement_fn_on_next_tbase_change( gas_id );
          }

          /* Reset the tn slam mode back to lowest */
          l1_dedicated_data_ptr->l1_dtm_slam_mode = L1_CS_TN_SLAM_LOWEST;

          /* We might need to add/decrement by one tdma frame when doing the
           * dtm to transfer transition.
           * Add necessary checks here.
           */

          /* Prime the pkt info with the latest CS timing advance decoded
           * from the SACCH. This needs to be conveyed to the ptcch module.
           */
          ta_value = gl1_msg_retrieve_ta(gas_id);
          geran_eng_mode_data_write(ENG_MODE_TIMING_ADVANCE,&ta_value,TRUE, gas_id);

          /* Hit the surround cell engine go button */
          l1_sci_go(gas_id);

          /* Set the afc and time tracking mode */
          gl1_msg_set_afc_tt_mode(AFC_TT_MODE_PDCH, gas_id);
        }
        else
        {
          MSG_GERAN_MED_1_G(" Moving to WAIT FOR END of blk %u ", frame_counters[gas_id].FNmod13);

          l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_CS_ABORTED_WAIT_FOR_END_OF_BLOCK;

        } /* End of else not on a blk boundary */
      }   /* End of if abort procedure is completed */
    }     /* End of case L1_DTM_CS_ABORT state */
    break;

  /**********************************************************
  * L1_DTM_CS_ABORTED_WAIT_FOR_END_OF_BLOCK state           *
  * ------------------                                      *
  * Enhanced DTM Release. The CS has been aborted and the   *
  * DTM state machine is waiting for the end of a PS block  *
  * before transitioning to minimise PS data loss           *
  ***********************************************************/
  case L1_DTM_CS_ABORTED_WAIT_FOR_END_OF_BLOCK:
    {
      boolean  blk_boundary = FALSE;
      dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

      switch( frame_counters[gas_id].FNmod13 )
      {
      case  3:
      case  7:
      case  12:



        break;

      default:

        blk_boundary = TRUE;

        break;
      }

      (void)gpl1_dtm_control(gas_id);

      if( FALSE != blk_boundary )
      {
        uint8 ta_value;

        MSG_GERAN_HIGH_0_G("PERFORM EDTM TRANSITION");

        /*Beging the enahnced transition here*/
        l1_isr_data_ptr->l1_dtm_mode_state = L1_DTM_NULL;

        /* Ensure a disable rx tx is issued before the control function
         * unless the abort modes and states already ensure that no rx / tx tasking
         * is attempted in the first place.
         */

        l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;

        l1_change_state( L1_TRANSFER_MODE, gas_id );


#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
        /* initialise TBF suspension variables */
        gpl1_gprs_init_tuneaway_and_ncell_data( gas_id);
       
#else

#ifdef FEATURE_GPRS_GBTA
        /* Useful when coming out of DTM */
        gpl1_set_gbta_status(gas_id);

#endif  /*FEATURE_GPRS_GBTA */

         /* initialise TBF suspension variables */
        gpl1_gprs_init_tbf_suspension(gas_id);
       
#endif /*#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

        /* Do the following only if sure that no assignment or release is
         * pending. Hence need to add suitable protection.
         */

        /* Make sure the pending alloc is setup correctly
         * Might not be necessary.
         */
        if( L1_NULL_ALLOC == transfer_data_ptr->pending_alloc_type )
        {
            if(  ( L1_TRANSFER_COMMAND != l1_tskisr_blk->main_command )
               &&( L1_NO_RELEASE == l1_transfer_data[gas_id].release_pending )
              )
            {
              transfer_data_ptr->pending_alloc_type = transfer_data_ptr->current_alloc_type;
            }
        }

        if( NULL != transfer_data_ptr->dl_tbf_ptr )
        {
          if( NULL == transfer_data_ptr->pending_dl_tbf_ptr )
          {
            transfer_data_ptr->pending_dl_tbf_ptr = transfer_data_ptr->dl_tbf_ptr;
          }
        }

        if( NULL != transfer_data_ptr->dynamic_tbf_ptr )
        {
          transfer_data_ptr->dynamic_tbf_ptr->allow_one_rb_gap = TRUE;
          transfer_data_ptr->dynamic_tbf_ptr->starting_time    = GSTMR_GET_FN_GERAN( gas_id );

          if( NULL == transfer_data_ptr->pending_dynamic_tbf_ptr )
          {
            transfer_data_ptr->pending_dynamic_tbf_ptr = transfer_data_ptr->dynamic_tbf_ptr;
          }
        }

        /* Set both the current and pending frequency pointers */
        transfer_data_ptr->frequency_info_1.frequency_list = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.frequency_list;
        transfer_data_ptr->frequency_info_1.hopping_flag   = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.hopping_flag;
        transfer_data_ptr->frequency_info_1.hsn            = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.HSN;
        transfer_data_ptr->frequency_info_1.maio           = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.MAIO;
        transfer_data_ptr->frequency_info_1.tsc            = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.TSC;

        transfer_data_ptr->frequency_info_ptr         = &( transfer_data_ptr->frequency_info_1 );
        transfer_data_ptr->pending_frequency_info_ptr = transfer_data_ptr->frequency_info_ptr;

        /* Make sure the clocking scheme is changed back to 61 (gprs) or 122 (egprs)
         * Due to different camp on strategies being employed in dtm and transfer modes.
         */
        if( TBF_MODE_GPRS == transfer_data_ptr->egprs_or_gprs_tbf )
        {
          gl1_msg_set_egprs_mode( FALSE, TRUE,gas_id );
        }

        if(  ( L1_CS_TN_SLAM_TN2 == l1_dedicated_data_ptr->l1_dtm_slam_mode )
           &&( l1_get_current_timeslot(gas_id) > 5 )
          )
        {
          gpl1_decrement_fn_on_next_tbase_change( gas_id );
        }

        /* Reset the tn slam mode back to lowest */
        l1_dedicated_data_ptr->l1_dtm_slam_mode = L1_CS_TN_SLAM_LOWEST;

        /* We might need to add/decrement by one tdma frame when doing the
         * dtm to transfer transition.
         * Add necessary checks here.
         */

        /* Prime the pkt info with the latest CS timing advance decoded
         * from the SACCH. This needs to be conveyed to the ptcch module.
         */
        ta_value = gl1_msg_retrieve_ta(gas_id);

        geran_eng_mode_data_write(ENG_MODE_TIMING_ADVANCE,&ta_value,TRUE, gas_id);

        /* Hit the surround cell engine go button */
        l1_sci_go(gas_id);

        /* Set the afc and time tracking mode */
        gl1_msg_set_afc_tt_mode(AFC_TT_MODE_PDCH, gas_id);
      }
      else
      {
        MSG_GERAN_HIGH_1_G("NOT READY TO EDTM TRANSITION FNmod13:%d",frame_counters[gas_id].FNmod13);
      }

    } /* End of case L1_DTM_CS_ABORTED_WAIT_FOR_END_OF_BLOCK state */
    break;

  /********************************************************
  * L1_DTM_CS_ABORT_PS_RECONFIG state                     *
  * ------------------                                    *
  * DTM is trying to abort, and will wait here until the  *
  * control function is satisfied that NPL1 has aborted   *
  * and also the SCE                                      *
  *********************************************************/
  case L1_DTM_CS_ABORT_PS_RECONFIG:
    {

    }
    break;

#endif  /* FEATURE_GSM_EDTM */

  default:
    MSG_GERAN_ERROR_1_G("L1_DTM_MODE fsm in unsupported state %d",l1_isr_data_ptr->l1_dtm_mode_state);
  }

  /* Log any DTM FSM state transitions */
  if ( l1_isr_data_ptr->old_l1_dtm_mode_state != l1_isr_data_ptr->l1_dtm_mode_state )
  {

#ifndef L1_HOST_TEST
    MSG_SPRINTF_2( MSG_SSID_DFLT, MSG_LEGACY_MED,
      "DTM fsm transition new-state:%s old-state:%s"
      ,dtm_state_array[l1_isr_data_ptr->l1_dtm_mode_state]
      ,dtm_state_array[l1_isr_data_ptr->old_l1_dtm_mode_state]);
#else
    #error code not present
#endif
    /*************************************************************/
    /* On leaving DTM i.e. the transition back to the null state */
    /* check to see if any release confirm is required.          */
    /*************************************************************/
    if( l1_isr_data_ptr->l1_dtm_mode_state == L1_DTM_NULL )
    {
      if ( dtm_transfer_release_confirm_on_exit[gas_id] ||
           ((l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR) &&
          (l1_transfer_data[gas_id].release_pending != L1_NO_RELEASE))
         )
      {
        MSG_GERAN_ERROR_0_G("FULL rel confirm isr ");

        /* Indicate to MAC release procedure is complete */
        gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending, gas_id);

        /* reset relelase type and rel confirm status*/
        l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
        l1_transfer_data[gas_id].confirm_pending = SENT;
        dtm_transfer_release_confirm_on_exit[gas_id] = FALSE;
      }
    }

    l1_isr_data_ptr->old_l1_dtm_mode_state = l1_isr_data_ptr->l1_dtm_mode_state;
  }

  /*Log state changes here*/
  if( dtm_event != l1_isr_data_ptr->dtm_command_event ||
      dtm_state != l1_isr_data_ptr->l1_dtm_mode_state ||
      dtm_abort_mode != dtm_data->l1_dtm_abort_mode)
  {
      MSG_GERAN_MED_3_G("l1_isr_dtm: dtm_command_event=%d, l1_dtm_mode_state=%d, l1_dtm_abort_mode=%d",
                         l1_isr_data_ptr->dtm_command_event,
                         l1_isr_data_ptr->l1_dtm_mode_state,
                         dtm_data->l1_dtm_abort_mode);
  }

  /* Trigger CB tick */
  l1_tick_cbch( gas_id );
}
#endif /* FEATURE_GSM_DTM */

/*===========================================================================

FUNCTION  l1_isr_inactive_mode

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_isr_inactive_mode( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /* The only way out of inactive mode
    * is with the L1_ACTIVATE_COMMAND.
    */

   (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);

   if ((l1_tskisr_blk->lockout == FALSE) &&
       (l1_tskisr_blk->main_command == L1_ACTIVATE_COMMAND))
   {

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     if( gl1_msg_get_multi_sim_mode() )
     {
       l1_change_state( L1_MULTI_SIM_NULL, gas_id );
       l1_tskisr_blk->main_command = L1_NULL_COMMAND;
       L1_transfer_new_data( gas_id );

       l1_tskisr_blk->going_active = FALSE;
       /* Tell RR there were no problems starting up L1 */
       L1_send_MPH_START_GSM_MODE_CNF(gas_id);
     }
     else
#endif
     {
       l1_change_state( L1_NULL_MODE, gas_id );
       l1_tskisr_blk->main_command = L1_NULL_COMMAND;
       L1_transfer_new_data( gas_id );

       /* Tell RR there were no problems starting up L1 */
       L1_send_MPH_START_GSM_MODE_CNF(gas_id);
     }
   }
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
   else if (l1_tskisr_blk->main_command == L1_WTOG_BPLMN_COMMAND)
   {
      l1_change_state( L1_WTOG_BPLMN_MODE, gas_id );
      l1_tskisr_blk->main_command = L1_NULL_COMMAND;

      l1_set_bplmn_suspend_in_progress(FALSE,gas_id);
      l1_set_gsm_stop_req_in_queue(FALSE,gas_id);
      L1_transfer_new_data( gas_id );

      /* Tell RR there were no problems starting up L1 */
      L1_send_MPH_START_GSM_MODE_FOR_BPLMN_CNF(gas_id);

      l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_STARTING,gas_id);
      l1_set_wtog_bplmn_task_state(NO_HPLMN_SEARCH,gas_id);
   }
#endif
   /* this happens when we first enter inactive mode */
   else if (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND)
   {
      if (gl1_deactivate(gas_id))
      {
          l1_tskisr_blk->main_command = L1_NULL_COMMAND;

          if (l1_tskisr_blk->command == L1_DEEP_SLEEP_COMMAND)
          {
              L1_send_ISR_DEACT_CNF(TRUE, gas_id);
          }
          else
          {
              L1_send_ISR_DEACT_CNF(FALSE, gas_id);
          }

          l1_tskisr_blk->command = L1_NULL_COMMAND;
      }
      else
      {
          MSG_GERAN_HIGH_0_G("MDSP is not ready for DEACT, wait for a frame");
      }
   }
   else if (l1_tskisr_blk->main_command != L1_NULL_COMMAND)
   {
      MSG_GERAN_HIGH_1_G("Unexpected L1 main_command: %d",l1_tskisr_blk->main_command);
   }
}

/*===========================================================================

FUNCTION  l1_isr_reset_mode

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_isr_reset_mode( gas_id_t gas_id )
{
   /* The only way out of reset mode
    * is with the L1_ACTIVATE_COMMAND.
    */

   /*
   ** CgC set the FT signal for L1 task handlers using gl1_drx_require_and_wait_for_next_tick()
   */
volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);

   if ((l1_tskisr_blk->lockout == FALSE) &&
       (l1_tskisr_blk->main_command == L1_ACTIVATE_COMMAND))
   {
      l1_change_state( L1_NULL_MODE, gas_id );
      l1_tskisr_blk->main_command = L1_NULL_COMMAND;
      L1_transfer_new_data( gas_id );

      /* Tell RR there were no problems starting up L1 */
      L1_send_MPH_START_GSM_MODE_CNF(gas_id);
   }
   else if (l1_tskisr_blk->main_command != L1_NULL_COMMAND)
   {
      MSG_GERAN_HIGH_1_G("Unexpected L1 main_command: %d",l1_tskisr_blk->main_command);
   }
}

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
void l1_isr_suspend_mode( gas_id_t gas_id )
#else
static void l1_isr_suspend_mode( gas_id_t gas_id )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  /* Set this signal	to ensure L1 task is
     not locked out permenantly */
  (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);

    if (l1_tskisr_blk->main_command == L1_SUSPEND_COMMAND)
    {
        uint32 timer_value=0;

        if ((gl1_get_FN( gas_id ) - l1_get_ind_timer_FN(gas_id)) > l1_get_ind_timer_value(gas_id))
        {
            timer_value = 0;
        }
        else
        {
            timer_value = l1_get_ind_timer_value(gas_id) - (gl1_get_FN( gas_id ) - l1_get_ind_timer_FN(gas_id));
        }

        if (timer_value >= (MIN_SUSPEND_RESUME_DURATION)) //to make sure we have enough time to suspend and resume
        {
            MSG_GERAN_HIGH_1_G("timer_value_cnf=%d",((timer_value*4.6)-(MIN_RESUME_TIME_IN_MS)));

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            if ( gl1_msg_get_multi_sim_mode() )
            {
                L1_send_ISR_SUSPEND_CNF((uint32)((timer_value*4.6)-(MIN_RESUME_TIME_IN_MS)), gas_id);

                l1_tskisr_blk->main_command = L1_NULL_COMMAND;
                l1_tskisr_blk->command = L1_NULL_COMMAND;

                /* ensure that the lock is released if this client still has it here */
                if( grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_GRANTED )
                {
                  call_in_task_grm_release( l1_tskisr_blk->client_id );
                }
            }
            else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
            if (gl1_deactivate(gas_id))
            {
                L1_send_ISR_SUSPEND_CNF((uint32)((timer_value*4.6)-(MIN_RESUME_TIME_IN_MS)), gas_id);

                l1_tskisr_blk->main_command = L1_NULL_COMMAND;
                l1_tskisr_blk->command = L1_NULL_COMMAND;
            }
            else
            {
                MSG_GERAN_HIGH_0_G("MDSP is not ready for DEACT, wait for a frame");
            }
        }
        else
        {
            MSG_GERAN_HIGH_0_G("timer_value_cnf=0");
            L1_send_MPH_SUSPEND_GSM_MODE_CNF(0, FALSE, gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            if ( gl1_msg_get_multi_sim_mode() )
            {
              l1_change_state( L1_MULTI_SIM_IDLE, gas_id );
            }
            else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
            {
              l1_change_state( L1_IDLE_MODE, gas_id );
            }

            l1_tskisr_blk->sub_state = L1_IDLE_INIT;
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;
        }
    }
    else if (l1_tskisr_blk->main_command == L1_IDLE_COMMAND)
    {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        if ( gl1_msg_get_multi_sim_mode() )
        {
          l1_change_state( L1_MULTI_SIM_IDLE, gas_id );
        }
        else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
        {
          l1_change_state( L1_IDLE_MODE, gas_id );
        }

        l1_tskisr_blk->sub_state = L1_IDLE_INIT;
        l1_tskisr_blk->main_command = L1_NULL_COMMAND;
        L1_send_MPH_RESUME_GSM_MODE_CNF(gas_id);
    }
    else if (l1_tskisr_blk->main_command == L1_NULL_COMMAND)
    {
        MSG_GERAN_MED_0_G("Null command, ignored in suspended mode");
    }
    else
    {
        MSG_GERAN_ERROR_1_G("Unexpected L1 main_command: %d",l1_tskisr_blk->main_command);
    }
}
#endif

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
void l1_isr_wtog_bplmn_mode( gas_id_t gas_id )
#else
static void l1_isr_wtog_bplmn_mode( gas_id_t gas_id )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  if (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND)
  {
    if(l1_do_WTOG_BPLMN_search(gas_id))
    {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      if ( gl1_msg_get_multi_sim_mode() )
      {
        /* ensure that the lock gets released */
        if ( grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_GRANTED )
        {
          call_in_task_grm_release( l1_tskisr_blk->client_id );
        }

        l1_change_state( L1_INACTIVE_MODE, gas_id );
        l1_tskisr_blk->main_command = L1_NULL_COMMAND;
        l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_SUSPENDED,gas_id);
        l1_set_wtog_bplmn_task_state(NO_HPLMN_SEARCH,gas_id);
        l1_set_bplmn_suspend_in_progress(FALSE,gas_id);
        if (l1_tskisr_blk->command == L1_DEEP_SLEEP_COMMAND)
        {
          MSG_GERAN_ERROR_0_G("Unexpected command in WTOG BPLMN: DEEP_SLEEP from RR");
          L1_send_ISR_DEACT_CNF(TRUE, gas_id);
        }
        else
        {
          L1_send_ISR_DEACT_CNF(FALSE, gas_id);
        }

        /* Make sure this boolean is reset to prevent un-necessary
         * sending of stop mode cnf messages.
         */
        l1_set_gsm_stop_req_in_queue(FALSE,gas_id);

        l1_tskisr_blk->command = L1_NULL_COMMAND;
      }
      else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      if (gl1_deactivate(gas_id))
      {
        l1_change_state( L1_INACTIVE_MODE, gas_id );
        l1_tskisr_blk->main_command = L1_NULL_COMMAND;
        l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_SUSPENDED,gas_id);
        l1_set_wtog_bplmn_task_state(NO_HPLMN_SEARCH,gas_id);
        l1_set_bplmn_suspend_in_progress(FALSE,gas_id);
        if (l1_tskisr_blk->command == L1_DEEP_SLEEP_COMMAND)
        {
          MSG_GERAN_ERROR_0_G("Unexpected command in WTOG BPLMN: DEEP_SLEEP from RR");
          L1_send_ISR_DEACT_CNF(TRUE, gas_id);
        }
        else
        {
          L1_send_ISR_DEACT_CNF(FALSE, gas_id);
        }

        /* Make sure this boolean is reset to prevent un-necessary
         * sending of stop mode cnf messages.
         */
        l1_set_gsm_stop_req_in_queue(FALSE,gas_id);

        l1_tskisr_blk->command = L1_NULL_COMMAND;
      }
      else
      {
        MSG_GERAN_HIGH_0_G("MDSP is not ready for DEACT, wait for a frame");
      }
    }
    else
    {
      if( WTOG_BPLMN_SUSPENDED == l1_get_wtog_bplmn_task_substate(gas_id)  )
      {
        MSG_GERAN_MED_1_G(" DEACTIVATE CMD WTOG suspended %u ", GSTMR_GET_FN_GERAN( gas_id ));

        l1_change_state( L1_INACTIVE_MODE, gas_id );
        l1_tskisr_blk->main_command = L1_NULL_COMMAND;
        l1_set_wtog_bplmn_task_state(NO_HPLMN_SEARCH,gas_id);
        l1_set_bplmn_suspend_in_progress(FALSE,gas_id);
        if (l1_tskisr_blk->command == L1_DEEP_SLEEP_COMMAND)
        {
          MSG_GERAN_ERROR_0_G("Unexpected command in WTOG BPLMN: DEEP_SLEEP from RR");
          L1_send_ISR_DEACT_CNF(TRUE, gas_id);
        }
        else
        {
          L1_send_ISR_DEACT_CNF(FALSE, gas_id);
        }

        l1_tskisr_blk->command = L1_NULL_COMMAND;
      }
    }
  }
  else if (l1_tskisr_blk->main_command == L1_SUSPEND_COMMAND)
  {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if ( gl1_msg_get_multi_sim_mode() )
    {
      if ( WTOG_BPLMN_MULTI_SIM_ENDING != l1_get_wtog_bplmn_task_substate(gas_id))
      {
        l1_set_wtog_bplmn_task_substate( WTOG_BPLMN_MULTI_SIM_SUSPENDING,gas_id );
      }

      (void)l1_do_WTOG_BPLMN_search(gas_id);
    }
    else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    if (gl1_deactivate(gas_id))
    {
      l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
      l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_SUSPENDED,gas_id);  /* we are suspend-ING */
      /*
      ** CgC We are in a transition state, don't clear this flag until the ISR_SUSPEND_CNF has been processed.
      ** else if we receieve MPH_STOP_GSM_MODE_REQ at the same time, we skip a state change and GFW crashes
      */
      l1_set_bplmn_suspend_in_progress(TRUE,gas_id);

      l1_isr_watchdog_deactivate(gas_id);
      L1_send_ISR_SUSPEND_CNF( 0,gas_id );
    }
    else
    {
      MSG_GERAN_HIGH_0_G("MDSP is not ready for SUSPEND, wait for a frame");
    }
  }
  else if (l1_tskisr_blk->main_command == L1_RESUME_COMMAND)
  {
    l1_tskisr_blk->main_command = L1_NULL_COMMAND;
    l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_STARTING,gas_id);
    L1_send_MPH_RESUME_GSM_MODE_FOR_BPLMN_CNF(gas_id);
  }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (l1_tskisr_blk->main_command == L1_WTOG_BPLMN_TRM_REQUEST_COMMAND)
  {
    l1_tskisr_blk->main_command = L1_NULL_COMMAND;
    L1_send_MPH_RESUME_GSM_MODE_FOR_BPLMN_CNF(gas_id);
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
  else
  {
    if(l1_tskisr_blk->main_command != L1_NULL_COMMAND)
    {
      MSG_GERAN_ERROR_1_G("WtoG BPLMN running with wrong command:%d",l1_tskisr_blk->main_command);
    }
    (void)l1_do_WTOG_BPLMN_search(gas_id);
  }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( gl1_msg_get_multi_sim_mode() )
  {
    /*ensure that if this is dual sim mode of operation that the other sim doesn't put us to sleep*/
    gl1_drx_require_next_tick(gas_id);
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
  (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
}
#endif

/*===========================================================================

FUNCTION  l1_isr_null_mode

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_isr_null_mode( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   /* Set this signal for GPRS every tick to ensure L1 task knows
   * that ISR is running
   */
  (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);

   if (l1_tskisr_blk->main_command == L1_FIND_BCCH_COMMAND)
   {
      l1_isr_handle_find_bcch_command( FALSE, gas_id );
   }
   else if (l1_tskisr_blk->main_command == L1_GET_TIMING_COMMAND )
   {
      l1_change_state( L1_FIND_BCCH, gas_id );
      l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
      L1_transfer_new_data( gas_id );
      l1_tskisr_blk->sub_state     = L1_ACQUIRING;
      l1_tskisr_blk->sub_sub_state = L1_ACQ_INITIALISE_FOR_INTERRAT;
   }
   else if (l1_tskisr_blk->main_command == L1_CM_HANDOVER_COMMAND )
   {
      /* This is run in the ISR context */
      /* Enable frame number counting */
      gl1_begin_INTERRAT_Handover(gas_id);
      l1_tskisr_blk->sub_state = L1_DED_SUSPEND;
      l1_tskisr_blk->main_command = L1_NULL_COMMAND;
      l1_tskisr_blk->command = L1_NULL_COMMAND;
   }
   else if (l1_tskisr_blk->main_command == L1_DEDICATED_COMMAND )
   {
      /* This is run in the ISR context */
      gl1_begin_INTERRAT_Reconnect(gas_id);
   }
   else if (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND)
   {
      l1_change_state( L1_INACTIVE_MODE, gas_id );
      /* l1_tskisr_blk->main_command will be set to
       * L1_NULL_COMMAND in the next frame tick
      */
   }
   else if (l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND)
   {
      l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
      L1_transfer_new_data( gas_id );
      if (l1_tskisr_blk->current_params.L1_confirm_ID == L1_DECODE_BCCH_LIST_CNF)
      {
         l1_tskisr_blk->current_params.L1_confirm_ID = L1_NULL_CNF;
         l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;
         L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE,gas_id);
      }
   }
   else if (l1_tskisr_blk->main_command != L1_NULL_COMMAND)
   {
      MSG_GERAN_HIGH_1_G("Unexpected L1 main_command: %d",l1_tskisr_blk->main_command);
   }
}


/*===========================================================================

FUNCTION  l1_isr_find_bcch

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_isr_find_bcch( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   boolean abort_find_bcch = FALSE;

   (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
   
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
   /*Inform MCPM about critical scenario now if needed*/
   if((timeline_error_recovery_needed[gas_id] !=0) && (critical_scn_updated_to_mcpm[gas_id] == FALSE))
   {
     gl1_clkdata_speed(TRUE, L1_TIME_LINE_ERROR, 0,L1_TIME_LINE_DELAY, gas_id);
     critical_scn_updated_to_mcpm[gas_id] = TRUE;
   }
#endif

   switch (l1_tskisr_blk->sub_state)
   {
      case L1_SCANNING:
         L1_power_scan(gas_id);
         abort_find_bcch = TRUE;
         break;

      case L1_ACQUIRING:
         abort_find_bcch = L1_initial_acq(gas_id);
         break;

      case L1_ACQUIRING_DURING_RESELECTION:
         abort_find_bcch = L1_initial_acq(gas_id);
         break;

      case L1_GETTING_BCCH:
         {
           uint32   frames_until_active = 0;
           abort_find_bcch = L1_get_bcch(&frames_until_active, gas_id);
         }
         break;

      case L1_GETTING_BCCH_LIST:
         abort_find_bcch = L1_get_bcch_list_fcch_sch_bcch(gas_id);
#ifdef FEATURE_GL1_ASDIV_ACQ
         gl1_asdiv_acq_state_tick(gas_id);
         MSG_GERAN_HIGH_1_G("AsDiv: FT handler reg: %d", gl1_msgi_check_ft_handler_registered(gas_id ,GL1_MSG_FT_HDLR_PWR_MEAS));
#endif
         break;

#ifdef FEATURE_GL1_ASDIV_ACQ 
         /*New Sub State for AsDiv in Acquisition*/
      case L1_ASDIV_ACQ_SWITCH:
        gl1_asdiv_acq_state_tick(gas_id);
        abort_find_bcch = TRUE;
        break;
#endif /*FEATURE_GL1_ASDIV_ACQ*/



      default:
         MSG_GERAN_HIGH_1_G( "Bad L1 sub-state %d", l1_tskisr_blk->sub_state);
         break;
   }

   /******************************************************************************/
   if (l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND )
   {
      if ( abort_find_bcch )
      {
         l1_change_state( L1_NULL_MODE, gas_id );
         l1_tskisr_blk->main_command = L1_NULL_COMMAND;

         /*
            This should not affect anything since we're merely
            transitioning to NULL mode
          */
         L1_transfer_new_data( gas_id );

         /* Send MPH_DECODE_BCCH_LIST_CNF to RR if specified */
         if (l1_tskisr_blk->current_params.L1_confirm_ID == L1_DECODE_BCCH_LIST_CNF )
         {
            /* reset to a NULL_CNF so that we dont send the BCCH_LIST_CNF twice */
            l1_tskisr_blk->current_params.L1_confirm_ID = L1_NULL_CNF;
            l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;
            L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE,gas_id);
         }
         else if (l1_tskisr_blk->current_params.L1_confirm_ID == L1_POWER_SCAN_CNF )
         {
           /* reset to a NULL_CNF so that we dont send the MPH_POWER_SCAN_CNF twice */

           l1_tskisr_blk->current_params.L1_confirm_ID = L1_NULL_CNF;
           l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;

           L1_send_MPH_POWER_SCAN_CNF(FALSE, gas_id);

         }

      }
   }
   /******************************************************************************/
   else if (l1_tskisr_blk->main_command == L1_IDLE_COMMAND )
   {
      if ( abort_find_bcch )
      {
         idle_data_T  *idle_data_ptr;

         L1_transfer_new_data( gas_id );
         l1_change_state( L1_IDLE_MODE, gas_id );
         l1_tskisr_blk->sub_state = L1_IDLE_INIT;
         l1_tskisr_blk->main_command = L1_NULL_COMMAND;
         L1_send_MPH_START_IDLE_MODE_CNF(gas_id);

         /*set timeslot according to CCCH_GROUP*/
         idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

         L1_change_timeslot((uint8)(idle_data_ptr->paging_data.ccch_group << 1), gas_id);
      }
   }
   /******************************************************************************/
   else if (l1_tskisr_blk->main_command == L1_ACCESS_COMMAND )
   {
      if ( abort_find_bcch )
      {
         idle_data_T  *idle_data_ptr;
         /* Reset always to default L1_MAX_RANDOM_ACCESS_MODE_FRAMES */
         l1_max_rach_guard_time[gas_id] = L1_MAX_RANDOM_ACCESS_MODE_FRAMES;
         L1_transfer_new_data( gas_id );
         l1_change_state( L1_RANDOM_ACCESS_MODE, gas_id );
         l1_tskisr_blk->sub_state    = L1_ACCESS_INIT;
         l1_tskisr_blk->main_command = L1_NULL_COMMAND;

         /*set timeslot according to CCCH_GROUP*/
         idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
         L1_change_timeslot((uint8)(idle_data_ptr->paging_data.ccch_group << 1), gas_id);

         l1_set_rach_read_all_AGCH(FALSE, gas_id);
      }
   }
   /******************************************************************************/
   else if (l1_tskisr_blk->main_command == L1_FIND_BCCH_COMMAND )
   {
      if ( abort_find_bcch )
      {
         /* The way to FIND_BCCH is always through NULL MODE.  Got to
            NULL mode and branch based on the specifid command
         */
         l1_change_state( L1_NULL_MODE, gas_id );
      }
   }
   /******************************************************************************/
   else if (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
   {
      if ( abort_find_bcch )
      {
         l1_change_state( L1_INACTIVE_MODE, gas_id );
      }
   }
   /******************************************************************************/
   else if (l1_tskisr_blk->main_command != L1_NULL_COMMAND)
   {
      MSG_GERAN_HIGH_1_G("Unexpected L1 main_command: %d",l1_tskisr_blk->main_command);
   }
}

/*===========================================================================

FUNCTION  l1_isr_find_sch

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_isr_find_sch( gas_id_t gas_id )
{
   L1_get_sch(gas_id);
}

/*===========================================================================

FUNCTION  l1_isr_idle_mode

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_isr_idle_mode( gas_id_t gas_id )
{
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];

   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   idle_data_T      *idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

   /* Set this signal for GPRS every tick to let L1 task
    * know that ISR is running.  This is used to let
    * task know when ISR is running again after a rude
    * wake up.
    */
   (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);

#ifdef  FEATURE_GSM_TO_LTE
   l1_isr_G2L_idle_trigger(gas_id);
#endif
   /* If background HPLMN search is in progress, restore
      idle tskisr blk parameters.  These would have been overwritten by
      the HPLMN search handlers (acq,pscan etc )
    */
   if ( L1_background_HPLMN_search_running(gas_id) )
   {
      l1_tskisr_blk->sub_state = L1_get_last_idle_sub_state(gas_id);

      l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;

      L1_transfer_new_data( gas_id );
   }

   switch ( l1_tskisr_blk->main_command )
   {
      /******************************************************************************/
      case L1_NULL_COMMAND:

         /* We are falling back to idle, check the flag
          * and post a message if necessary.
          */
         if( FALSE != l1_isr_data_ptr->l1_access_isr_send_mac_sb_failure )
         {
           L1_send_L1_ISR_SINGLE_BLK_FAILURE_CNF(gas_id );

           /* Reset the boolean */
           l1_isr_data_ptr->l1_access_isr_send_mac_sb_failure = FALSE;
         }

          /*
             This flags is for test purposes only and is used for
             switching between idle and SDCCH to allow testing
             surround cell operation on the SDCCH for extended
             periods of time.  It is a run time flag.
          */
         if (jump_to_sdcch)
         {
            l1_isr_jump_to_sdcch(gas_id);
         }
         else
         {
            (void)L1_idle_mode_ISR( FALSE, gas_id );
         }
         if(TRUE == l1_tskisr_blk->time_slot_change_pending
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            && !l1_fm_any_activity_running(gas_id)
#endif
           )
         {
           /*set timeslot according to CCCH_GROUP*/
           L1_change_timeslot((uint8)(idle_data_ptr->paging_data.ccch_group << 1), gas_id);
           l1_tskisr_blk->time_slot_change_pending = FALSE;
         }
         break;

      /******************************************************************************/
      case L1_DEACTIVATE_COMMAND:
         if (L1_idle_mode_ISR( TRUE, gas_id ))
         {
            byte currentTS = 0;
            /* change mode */
            L1_transfer_new_data( gas_id );
            l1_change_state( L1_INACTIVE_MODE, gas_id );

            currentTS = l1_get_current_timeslot(gas_id);
            MSG_GERAN_HIGH_2_G("Check for G2X HO cmd[%d], then slam from TS[%d] to TS0", l1_tskisr_blk->command, currentTS);

            /* For G2X H/O slam to TS 0 */
            if( ( currentTS != 0 ) && ( l1_tskisr_blk->command == L1_RAT_CHANGE_COMMAND ) )
            {
               L1_change_timeslot(0, gas_id);
            }
            /* l1_tskisr_blk->main_command will be set to
             * L1_NULL_COMMAND in the next frame tick
             */
         }
         break;

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
      /******************************************************************************/
      case L1_SUSPEND_COMMAND:
         if (L1_idle_mode_ISR( TRUE, gas_id ))
         {
            /* change mode */
            L1_transfer_new_data( gas_id );
            l1_change_state( L1_SUSPEND_MODE, gas_id );
         }
         break;
#endif

      /******************************************************************************/
      case L1_ENTER_NULL_COMMAND:
         if (L1_idle_mode_ISR( TRUE, gas_id ))
         {
            /* change mode */
            L1_transfer_new_data( gas_id );
            l1_change_state( L1_NULL_MODE, gas_id );
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;
         }
         break;

      /******************************************************************************/
      case L1_IDLE_COMMAND:
         l1_change_state( L1_IDLE_MODE, gas_id );
         l1_tskisr_blk->main_command = L1_NULL_COMMAND;

         /*set timeslot according to CCCH_GROUP*/
         L1_change_timeslot((uint8)(idle_data_ptr->paging_data.ccch_group << 1), gas_id);

         if ((l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR) &&
             (l1_transfer_data[gas_id].release_pending != L1_NO_RELEASE))
         {
            MSG_GERAN_HIGH_0_G("rel confirm isr ");
            /* Indicate to MAC release procedure is complete */
            gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending, gas_id);

            /* reset relelase type and rel confirm status*/
            l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
            l1_transfer_data[gas_id].confirm_pending = SENT;
         }
         L1_send_MPH_START_IDLE_MODE_CNF(gas_id);

         if(!l1_sc_globals_ptr->active)
         {
            l1_sci_go(gas_id);
         }

         (void)L1_idle_mode_ISR( FALSE, gas_id );
         break;

      /******************************************************************************/
      case L1_ACCESS_COMMAND:
         /* Clean up idle SCE before RACHing */
         if (L1_idle_mode_ISR( TRUE, gas_id ))
         {
            /* Reset always to default L1_MAX_RANDOM_ACCESS_MODE_FRAMES */
            l1_max_rach_guard_time[gas_id] = L1_MAX_RANDOM_ACCESS_MODE_FRAMES;
            l1_change_state( L1_RANDOM_ACCESS_MODE, gas_id );
            l1_tskisr_blk->sub_state = L1_ACCESS_INIT;
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;
            l1_set_rach_read_all_AGCH(FALSE, gas_id);

            l1_isr_data_ptr->l1_access_isr_send_mac_sb_failure = FALSE;
         }
         break;

      case L1_PRACH_COMMAND:
         /* Clean up idle SCE before RACHing */
         if( L1_idle_mode_ISR( TRUE, gas_id ) )
         {
           l1_change_state( L1_PACKET_ACCESS_MODE, gas_id );

           l1_tskisr_blk->sub_state    = L1_INITIAL_PRACH;
           l1_tskisr_blk->main_command = L1_NULL_COMMAND;
           l1_set_rach_read_all_AGCH(FALSE, gas_id );
           L1_change_timeslot( 0, gas_id);
           l1_isr_data_ptr->l1_access_isr_send_mac_sb_failure = FALSE;
         }
         break;

      /******************************************************************************/
      case L1_DEDICATED_COMMAND:
         /* check that all Activity has finished */
         /* can this happen ??? */
         if (L1_idle_mode_ISR( TRUE, gas_id ))
         {
            /* change mode */
            L1_transfer_new_data( gas_id );
            l1_change_state( L1_DEDICATED_MODE, gas_id );
            l1_tskisr_blk->sub_state = L1_DED_INIT;
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;
         }
         break;


      /******************************************************************************/
      case L1_FIND_SCH_COMMAND:
         /* check that all Activity has finished */
         if (L1_idle_mode_ISR( TRUE, gas_id ))
         {
            l1_isr_handle_find_sch_command(  l1_idle_data->reselection_apply_freq_correction, gas_id );
         }
         break;

      /******************************************************************************/
      case L1_FIND_BCCH_COMMAND:
         /* check that all Activity has finished */

         if (L1_idle_mode_ISR( TRUE, gas_id ))
         {
            l1_isr_handle_find_bcch_command(  l1_idle_data->reselection_apply_freq_correction, gas_id );
         }
         break;


      /******************************************************************************/
      case L1_TRANSFER_COMMAND:
      {
         transfer_data_T *transfer_data_ptr     = l1_tskisr_blk->next_params.L1Data.pTransfer_data;

         /* First check if a starting time has been specified */
         if (transfer_data_ptr->starting_time_valid)
         {
            /* Is the starting time about to expire in 3 frames
             * assuming starting time is always aligned to block boundary
             */
            const uint32  current_fn = GSTMR_GET_FN_GERAN( gas_id);

            if( ( transfer_data_ptr->starting_time == ADD_FN( current_fn, (NPL1_FN_OFFSET + 2) ) ) ||
                ( FALSE != l1_isr_data_ptr->starting_time_elapsed ) )
            {
               /*  Ensure this condition is met until L1_idle_mode_ISR returns TRUE  */
               l1_isr_data_ptr->starting_time_elapsed = TRUE;

               if (L1_idle_mode_ISR( TRUE, gas_id ))
               {
                  L1_transfer_new_data( gas_id );
                  l1_change_state( L1_TRANSFER_MODE, gas_id );
                  l1_tskisr_blk->sub_state = L1_TRANSFER_INIT;
                  l1_tskisr_blk->sub_sub_state = NULL_STATE;
                  l1_tskisr_blk->main_command = L1_NULL_COMMAND;
                  /* Clear start time valid flag for next time */
                  idle_data_ptr->starting_time_valid = FALSE;

                  /* If there are outstanding Serving cell Sys Info read requests
                   * that have not been serviced by L1 then send a failure indication
                   * to Upper layers with cause: state change.
                   */
                  if(  ( RR_L1_SI_TYPE_NONE != idle_data_ptr->si_mask )
                     &&(  ( SERV_BCCH_MASK_NONE != idle_data_ptr->BCCH_mask )
                        ||( SERV_EXT_BCCH_MASK_NONE != idle_data_ptr->ext_BCCH_mask )
                       )
                    )
                  {
                    /* Send a sys info request failure indication */
                    L1_send_MPH_READ_SYS_INFO_FAILURE_IND( TRUE,       /* state change */
                                                           FALSE, gas_id );
                  }

                  /*  Ensure this condition is met until L1_idle_mode_ISR returns TRUE  */
                  l1_isr_data_ptr->starting_time_elapsed = FALSE;

               }
            }
            else
            {
               /* starting time set but too far away
                * continue with either 51 idle mode
                */
               (void)L1_idle_mode_ISR( FALSE, gas_id );

               gl1_drx_require_next_tick(gas_id);
            }
         }
         else // no starting time so change state to transfer asap
         {
            /*  There will always be a starting time at this point as the
                task will create one if one not present in the assisgnment  */
            MSG_GERAN_FATAL_1_G("TRANSFER COMMAND WITH NO ST %d",GSTMR_GET_FN_GERAN( gas_id));
         }

         } /* End of case L1_TRANSFER_COMMAND */
         break;

         default:
            MSG_GERAN_HIGH_1_G("Bad L1 main_command %d.", l1_tskisr_blk->main_command);
            break;

   } /* End of switch on main command */

   /* Run HPLMN search handlers if they have been activated.  This
      is controlled (suspend and resume ) from within l1_idle
    */
   L1_do_background_HPLMN_search(gas_id);
}

/*===========================================================================

FUNCTION  l1_isr_random_access_mode

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_isr_random_access_mode( gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];

   /* Set this signal for GPRS every tick to ensure L1 task is
    * not locked out permenantly
    */
   (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);

#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
   if(critical_scn_updated_to_mcpm[gas_id] == TRUE)
   {
     gl1_hw_update_tx_burst_success(gas_id);
   }
   /*Inform MCPM about critical scenario now if needed*/
   if((timeline_error_recovery_needed[gas_id] !=0) && (critical_scn_updated_to_mcpm[gas_id] == FALSE))
   {
     gl1_clkdata_speed(TRUE, L1_TIME_LINE_ERROR, 0,L1_TIME_LINE_DELAY, gas_id);
     critical_scn_updated_to_mcpm[gas_id] = TRUE;
   }
#endif

   switch ( l1_tskisr_blk->main_command )
   {
      /******************************************************************************/
      case L1_NULL_COMMAND:

         (void)L1_access_ISR(L1_MODE_RUN, gas_id);
         break;

      /******************************************************************************/
      case L1_DEACTIVATE_COMMAND:
         if (L1_access_ISR(L1_MODE_STOP, gas_id))
         {
            /* change mode */
            l1_change_state( L1_INACTIVE_MODE, gas_id );
            /* l1_tskisr_blk->main_command will be set to
             * L1_NULL_COMMAND in the next frame tick
             */
         }
         break;

      /******************************************************************************/
      case L1_ENTER_NULL_COMMAND:
         if (L1_access_ISR(L1_MODE_STOP, gas_id))
         {
           /* change mode */
           L1_transfer_new_data( gas_id );
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
           if ( gl1_msg_get_multi_sim_mode() )
           {
             l1_change_state( L1_MULTI_SIM_NULL, gas_id );
           }
           else
#endif
           {
           l1_change_state( L1_NULL_MODE, gas_id );
           }
           l1_tskisr_blk->main_command = L1_NULL_COMMAND;
         }
         break;

      /******************************************************************************/
      case L1_FIND_SCH_COMMAND:
         /* check that all Activity has finished */
         if (L1_access_ISR(L1_MODE_STOP, gas_id))
         {
            l1_isr_handle_find_sch_command(  l1_idle_data->reselection_apply_freq_correction, gas_id );
         }
         break;

      /******************************************************************************/
      case L1_FIND_BCCH_COMMAND:
         /* check that all Activity has finished */
         if (L1_access_ISR(L1_MODE_STOP, gas_id))
         {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            if ( gl1_msg_get_multi_sim_mode() )
            {
              l1_isr_handle_multi_sim_find_bcch_command(gas_id);
            }
            else
#endif
            {
              l1_isr_handle_find_bcch_command(  l1_idle_data->reselection_apply_freq_correction, gas_id );
            }
         }
         break;

      /******************************************************************************/
#ifdef FEATURE_DUAL_SIM
      case L1_MULTI_SIM_IDLE_COMMAND:
#endif
      case L1_IDLE_COMMAND:
      {
         if (L1_access_ISR(L1_MODE_STOP, gas_id))
         {
            idle_data_T  *idle_data_ptr;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            if ( gl1_msg_get_multi_sim_mode() )
            {
              l1_tskisr_blk->main_command = L1_WAIT_FOR_MDSP_TO_COMPLETE;

              l1_isr_data_ptr->mdsp_awake_panic = MAX_FRAMES_MDSP_AWAKE_BEFORE_PANIC;

              MSG_GERAN_HIGH_0_G("l1_isr_random_access_mode - move to L1_WAIT_FOR_MDSP_TO_COMPLETED");

              break;
            }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

          /* Delay sending any pending release confirm to MAC until afer Idle Init */
          if (l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR)
          {
            l1_transfer_data[gas_id].confirm_pending = SEND_AT_IDLE_INIT;
          }

          /* TODO this should be done in the task */
          l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;

          L1_transfer_new_data(gas_id);
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
          if ( gl1_msg_get_multi_sim_mode() )
          {
            l1_change_state( L1_MULTI_SIM_IDLE, gas_id );
          }
          else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
          {
            l1_change_state( L1_IDLE_MODE,gas_id );
          }

          l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          l1_tskisr_blk->sub_state    = L1_IDLE_INIT;

          /* Inform GRR that L1 has transitioned to Idle */
          L1_send_MPH_START_IDLE_MODE_CNF(gas_id);

          /*set timeslot according to CCCH_GROUP*/
          idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
          L1_change_timeslot((uint8)(idle_data_ptr->paging_data.ccch_group << 1),gas_id);
         }
      }
      break;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      /******************************************************************************/
      case L1_WAIT_FOR_MDSP_TO_COMPLETE:
        {
          if ( mdsp_ready_for_sleep(gas_id) || ( l1_isr_data_ptr->mdsp_awake_panic == 0 ) )
          {
            idle_data_T  *idle_data_ptr;

            gl1_msg_abort_pwr_meas(gas_id);

            l1_change_state( L1_MULTI_SIM_IDLE, gas_id );

            /* Perform the TRM releasing here for exit from access mode back to DS IDLE */
            call_in_task_grm_release( l1_tskisr_blk->client_id );

            /* Send Release confirm to MAC */
            if (l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR)
            {
               MSG_GERAN_MED_0_G("L1_WAIT_FOR_MDSP_TO_COMPLETE rel confirm isr ");
               /*  Indicate to MAC release procedure is complete */
               /* Delay sending any pending release confirm to MAC until afer Idle Init */
               l1_transfer_data[gas_id].confirm_pending = SEND_AT_IDLE_INIT;
            }

            L1_transfer_new_data( gas_id );
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;
            l1_tskisr_blk->sub_state = L1_IDLE_INIT;


            /* Inform GRR that L1 has transitioned to Idle */
            L1_send_MPH_START_IDLE_MODE_CNF(gas_id);

            /*set timeslot according to CCCH_GROUP*/
            idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
            L1_change_timeslot((uint8)(idle_data_ptr->paging_data.ccch_group << 1), gas_id);
         }
          else
          {
            if ( l1_isr_data_ptr->mdsp_awake_panic > 0 )
            {
              l1_isr_data_ptr->mdsp_awake_panic--;
            }

            MSG_GERAN_HIGH_1_G("MSIM mDSP not ready for deact from ACCESS %d",l1_isr_data_ptr->mdsp_awake_panic);
          }
        }
      break;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

      /******************************************************************************/
      case L1_ACCESS_COMMAND:

         /* TODO - this doesn't look right to me. */
            /* Reset always to default L1_MAX_RANDOM_ACCESS_MODE_FRAMES */
         l1_max_rach_guard_time[gas_id] = L1_MAX_RANDOM_ACCESS_MODE_FRAMES;
         l1_change_state( L1_RANDOM_ACCESS_MODE, gas_id );
         l1_tskisr_blk->sub_state    = L1_ACCESS_INIT;
         l1_tskisr_blk->main_command = L1_NULL_COMMAND;

         MSG_GERAN_ERROR_0_G("Access command in Access Mode.");

         (void)L1_access_ISR(L1_MODE_RUN, gas_id);
         break;

      /******************************************************************************/
      case L1_DEDICATED_COMMAND:
         /* check that all Activity has finished */

         if (L1_access_ISR(L1_MODE_STOP, gas_id))
         {
            /* change mode */
            L1_transfer_new_data( gas_id );
            l1_change_state( L1_DEDICATED_MODE, gas_id );
            l1_tskisr_blk->sub_state     = L1_DED_INIT;
            l1_tskisr_blk->sub_sub_state = NULL_STATE;
            l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
         }
         break;


      /******************************************************************************/
      case L1_TRANSFER_COMMAND:
      {
         /* Assign a pointer to the transfer data structure */
         transfer_data_T *transfer_data_ptr = l1_tskisr_blk->next_params.L1Data.pTransfer_data;

#ifdef FEATURE_GTA_DURING_PS_RACH
         gl1_trm_state_t trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
#endif
         /* First check if a starting time has been specified */
         if (transfer_data_ptr->starting_time_valid)
         {
            /* Is the starting time about to expire ?
             * Assumes starting time is always aligned to block boundary.
             * Use of ADD_FN macro assumes the constants added do not exceed
             * the hyperframe boundary value of 2715648.
             */
            const uint32  this_FN = GSTMR_GET_FN_GERAN( gas_id);
            uint32 offset_fn = NPL1_FN_OFFSET + L1_STATE_CHANGE_FN_OFFSET + L1_CAMP_TS_FN_OFFSET;


            if( ( transfer_data_ptr->starting_time == ADD_FN( this_FN,offset_fn))
              ||( FALSE != l1_isr_data_ptr->starting_time_elapsed )
              )
            {
               RESET_BITMAP(gas_id);

#ifdef FEATURE_GTA_DURING_PS_RACH
              /*we may not have TRM here because TDS might have retained it for longer*/
              if( (trm_status != GL1_TRM_GRANTED) &&
                 (trm_status != GL1_TRM_RETAINED_FOR_ACCESS))
              {
                MSG_GERAN_HIGH_1_G("GTA_RACH_PS: TRM not granted for TBF starting time, send DS ABORT trm_status %d",trm_status );

                /*need to abort here*/
                L1_send_MPH_L1_DS_ABORT_IND(gas_id,RR_DS_ABORT_TIMEDOUT,200);
                l1_tskisr_blk->main_command = L1_NULL_COMMAND;
              }
              else if( (l1_tskisr_blk->sub_state == L1_ACCESS_GTA_FW_LOAD_WAIT )||(l1_tskisr_blk->sub_state == L1_ACCESS_GTA_TRM_WAIT))
              {
              /* Its a race condition, where trm status is GL1_TRM_GRANTED and sub_state is L1_ACCESS_GTA_TRM_WAIT. In this case, FW is not loaded yet.
                 so the l1_tskisr_blk->sub_state == L1_ACCESS_GTA_TRM_WAIT check is added to avoid moving to transfer state without FW loaded */
                 
                l1_isr_data_ptr->starting_time_elapsed = TRUE;
                MSG_GERAN_HIGH_1_G("GTA_RACH_PS:l1_tskisr_blk->sub_state = %d",l1_tskisr_blk->sub_state);
               (void)L1_access_ISR(L1_MODE_RUN, gas_id);
              }
              else
              {
#endif /*FEATURE_GTA_DURING_PS_RACH*/

                 /*  Ensure this condition is met until L1_access_mode_ISR returns TRUE  */
                 l1_isr_data_ptr->starting_time_elapsed = TRUE;

                 /* Need to wait for RACH ISR to stop first */
                 if( L1_access_ISR( L1_MODE_STOP, gas_id ) )
                 {
                    l1_isr_data_ptr->starting_time_elapsed = FALSE;
                    L1_transfer_new_data( gas_id );
                    l1_change_state( L1_TRANSFER_MODE, gas_id );

                  l1_tskisr_blk->sub_state = L1_TRANSFER_INIT;
                  l1_tskisr_blk->sub_sub_state = NULL_STATE;
                  l1_tskisr_blk->main_command = L1_NULL_COMMAND;

                  /*  Reset command */
                  l1_tskisr_blk->command = L1_NULL_COMMAND;
               }
#ifdef FEATURE_GTA_DURING_PS_RACH
              }
#endif /*FEATURE_GTA_DURING_PS_RACH*/
            }
            else
            {

               /* keep the access ISR running until the starting time elapses */
               (void)L1_access_ISR(L1_MODE_RUN, gas_id);
            }
         }
         else /* no starting time so change state to transfer asap */
         {
            /* 51 MFRM -> TRANSFER */
            /* need to stop the access isr on 51 MFRM first */
            if (L1_access_ISR(L1_MODE_STOP, gas_id))
            {
               L1_transfer_new_data( gas_id );
               l1_change_state( L1_TRANSFER_MODE, gas_id );
               l1_tskisr_blk->sub_state = L1_TRANSFER_INIT;
               l1_tskisr_blk->sub_sub_state = NULL_STATE;
               l1_tskisr_blk->main_command = L1_NULL_COMMAND;

            }
         }
      } /* End of case  */
      break;

      /******************************************************************************/
      default:
         MSG_GERAN_HIGH_1_G("Bad L1 main_command %d.", l1_tskisr_blk->main_command);
         break;
   }
}

/*===========================================================================

FUNCTION  l1_isr_channel_transition

DESCRIPTION
  For ded channel assignment or channel modify, we execute the handler code twice in isr context.
  So increase CPU speed, use data setting to avoid isr overruns..

DEPENDENCIES
  None
RETURN VALUE
  None

NOTES

 This DOES NOT cause a state change, transition to increase clock speed only,


===========================================================================*/

static void l1_isr_channel_transition( boolean speedup, gas_id_t gas_id )
{
   if(TRUE==speedup)
   {
      switch ( l1_get_current_rr_l1_msg_id(gas_id) )
      {
        case MPH_CHANNEL_ASSIGN_REQ:
        case MPH_CHANNEL_MODE_MODIFY_REQ:
        {
          gl1_clkdata_speed(TRUE, L1_DED_SPEEDUP_CHAN_RECONFIG, L1_DED_DATA_NULL,L1_DED_CHAN_RECONFIG,gas_id);

        }
        break;

        default:          /* take no action */
        break;
      }
   }
   else
   {
     gl1_clkdata_speed(FALSE, L1_DED_SPEEDUP_CHAN_RECONFIG, L1_DED_DATA_NULL,L1_DED_CHAN_RECONFIG,gas_id);
   }
}

/*===========================================================================

FUNCTION  l1_isr_dedicated_mode

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_isr_dedicated_mode( gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T    *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  static uint16 l1_dedicated_guard_fn[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };

  l1_isr_G2L_trigger(gas_id);

#ifdef FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY
  if (gl1_hw_get_dedicated(gas_id)) 
  {
     gl1_hw_rf_recovery_cleanup(FALSE,gas_id);
  }
#endif /* FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY */

#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
  if(critical_scn_updated_to_mcpm[gas_id] == TRUE)
  {
    gl1_hw_update_tx_burst_success(gas_id);
  }
  /*Inform MCPM about critical scenario now if needed*/
  if((timeline_error_recovery_needed[gas_id] !=0) && (critical_scn_updated_to_mcpm[gas_id] == FALSE))
  {
    gl1_clkdata_speed(TRUE, L1_TIME_LINE_ERROR, 0,L1_TIME_LINE_DELAY, gas_id);
    critical_scn_updated_to_mcpm[gas_id] = TRUE;
  }
#endif

   switch ( l1_tskisr_blk->main_command )
   {
      case L1_NULL_COMMAND:
         if (jump_to_idle)
         {
            /* If flag set, stop SDCCH and jump back to idle */
            l1_tskisr_blk->main_command = L1_DEDICATED_STOP;
            l1_sc_stop(gas_id);
         }
         else
         {
            (void)L1_control_dedicated(gas_id);
         }
         break;

      case L1_ENTER_NULL_COMMAND:
         l1_change_state( L1_NULL_MODE, gas_id );
         l1_tskisr_blk->main_command = L1_NULL_COMMAND;
         L1_terminate_dedicated();
         L1isr_send_MPH_CHANNEL_RELEASE_CNF (NO_ERROR, gas_id);
         gl1_msg_disable_ciphering(gas_id);
         break;

      case L1_DEACTIVATE_COMMAND:

         /*if Deactivation has happened because of X2G Ho fail, adjust GSTMR to its previous value*/
         if(l1_tskisr_blk->x2g_inter_rat_ho_triggered  == TRUE)
         {
           L1_sync_to_current(&l1_dedicated_data_ptr->current_cell_details, gas_id);
           l1_tskisr_blk->x2g_inter_rat_ho_triggered  = FALSE;
         }

         /* For G2X H/O slam to TS 0 */
         if(l1_tskisr_blk->command == L1_RAT_CHANGE_COMMAND)
         {
            L1_change_timeslot(0, gas_id);
         }
		l1_change_state( L1_INACTIVE_MODE, gas_id );
         /* l1_tskisr_blk->main_command will be set to
          * L1_NULL_COMMAND in the next frame tick
          */
         L1_terminate_dedicated();
         gl1_msg_disable_ciphering(gas_id);
         break;

      case L1_FIND_BCCH_COMMAND:
         /*  We do not go back into the start of find BCCH , but the
             part that reads the BCCH  */
         if (l1_tskisr_blk->command == L1_DECODE_BCCH_COMMAND)
         {
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;
            l1_tskisr_blk->command = L1_NULL_COMMAND;
            L1_transfer_new_data( gas_id );
            l1_tskisr_blk->sub_state = L1_GETTING_BCCH;
            l1_tskisr_blk->sub_sub_state = L1_BCCH_START;
            l1_change_state( L1_FIND_BCCH, gas_id );
            L1isr_send_MPH_CHANNEL_RELEASE_CNF (NO_ERROR, gas_id);
            gl1_msg_disable_ciphering(gas_id);

            L1_change_timeslot ( 0, gas_id );
         }
         else
         {
            MSG_GERAN_HIGH_1_G("Unexpected L1 command: %d",l1_tskisr_blk->command);
         }
         break;

      case L1_DEDICATED_COMMAND:
         l1_change_state( L1_DEDICATED_MODE, gas_id );
         l1_tskisr_blk->sub_state    = L1_DED_INIT;
         l1_tskisr_blk->main_command = L1_NULL_COMMAND;

         /* Invoke this to process the state transition immediately */
         (void)L1_control_dedicated(gas_id);
         break;

      case L1_DEDICATED_STOP:

         /* GUARD timer expirey - we are stuck! */
         if (l1_dedicated_guard_fn[gas_id]++ >= L1_DED_GUARD_FN_MAX)
         {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_GERAN_FATAL_2_G("L1 DEDICATED guard time expirey %d [%d]",l1_dedicated_guard_fn[gas_id],GSTMR_GET_FN_GERAN( gas_id));
         }

         l1_tskisr_blk->command = L1_DEDICATED_STOP;

         l1_isr_channel_transition(TRUE, gas_id);      /* increase the CPU speed for transition */


         if (L1_control_dedicated(gas_id))
         {
           /* Reset GUARD timer */
           l1_dedicated_guard_fn[gas_id] = 0;

            /* If flag set, jump back to idle mode */
            if (jump_to_idle)
            {
               /* This path bypasses L1_handle_dedicated */
               jump_to_idle = 0;
               jump_to_sdcch = 0;

               /*
               ** restore voice speed setting before new state change
               */
               l1_isr_channel_transition(FALSE, gas_id);

               l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;
               l1_sc_start(L1SCModeIdle, gas_id);  /* can't do from task in this case */
               l1_change_state( L1_FIND_BCCH, gas_id );
               l1_tskisr_blk->main_command = L1_IDLE_COMMAND;
               l1_tskisr_blk->command = L1_DECODE_BCCH_COMMAND;
               l1_tskisr_blk->sub_state = L1_DED_SUSPEND;
               l1_tskisr_blk->sub_sub_state = L1_BCCH_START;
            }
            else
            {
               /* everything has finished, idle = true */
               l1_tskisr_blk->sub_state = L1_DED_SUSPEND;
               l1_tskisr_blk->main_command = L1_NULL_COMMAND;
               l1_tskisr_blk->command = L1_NULL_COMMAND;

               /* Complete new channel setup in ISR.  This was previously
                * done in the task.  This will speed up the process
                */
               gl1_setup_new_channel ( gas_id );

               if(l1_tskisr_blk->sub_sub_state == L1_DED_RUNNING)
               {
                  /*
                  ** restore voice speed setting after tch init completes on transition into ded running
                  */
                  l1_isr_channel_transition(FALSE, gas_id);
               }
            }
         }
         break;

#ifdef FEATURE_GSM_DTM
      case L1_DEDICATED_TO_DTM_COMMAND:
        {
          /* Perform the final tick of dedicated FSM in this state */
          (void)L1_control_dedicated(gas_id);

          /* Only when the SCE has stoped can the transition occur */
          if ( !l1_sci_is_op_in_progress(gas_id) )
          {
            /* Complete the transition to the DTM state */
            gl1_setup_new_channel(gas_id);
          }

        }
        break;
#endif /*FEATURE_GSM_DTM*/

      default:
         MSG_GERAN_ERROR_1_G("Unexpected main_command %d",l1_tskisr_blk->main_command);
   }

   /* Trigger CB tick */
   l1_tick_cbch( gas_id );
}


/*===========================================================================

FUNCTION  l1_isr_transfer_mode

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if defined (FEATURE_G2X_TUNEAWAY) || defined ( FEATURE_GPRS_GBTA)
void    l1_isr_transfer_mode( gas_id_t gas_id )
#else
static void l1_isr_transfer_mode( gas_id_t gas_id )
#endif
{
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
   l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];
   uint32 l1_dtm_temp_st =0;

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
   boolean  reset_dedicated_command = FALSE;
#endif

   /* Set this signal for GPRS every tick to ensure L1 task is
    * not locked out permenantly
    */
   (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);

#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
    if(critical_scn_updated_to_mcpm[gas_id] == TRUE)
    {
      gl1_hw_update_tx_burst_success(gas_id);
    }
    /*Inform MCPM about critical scenario now if needed*/
    if((timeline_error_recovery_needed[gas_id] !=0) && (critical_scn_updated_to_mcpm[gas_id] == FALSE))
    {
      gl1_clkdata_speed(TRUE, L1_TIME_LINE_ERROR, 0,L1_TIME_LINE_DELAY, gas_id);
      critical_scn_updated_to_mcpm[gas_id] = TRUE;
    }
#endif

#ifdef FEATURE_GSM_TDS
    if(l1_g2t_issue_tbf_rel)
     {
       boolean return_to_idle = gpl1_gprs_handle_tbf_release( (release_tbf_t)l1_g2t_delay_tbf_rel, gas_id );

       MSG_GERAN_HIGH_1_G("TDS: Issuing pending TBF release %d", l1_g2t_delay_tbf_rel);
       if( FALSE != return_to_idle )
       {
          gpl1_gprs_setup_start_idle((void *)0, gas_id);
       }
       l1_g2t_issue_tbf_rel = FALSE;
       l1_g2t_delay_tbf_rel = 0xff;
     }
#endif

   l1_isr_G2L_trigger(gas_id);

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
   /* @@OG : EDTM design */

   switch( l1_tskisr_blk->command )
   {
     case  L1_DEDICATED_COMMAND:
     /* Might need to do the comparison here based on 42432 frame counter. */

     /* warning: must ensure start time is aligned on a block boundary */
     if( l1_dedicated_data_ptr->starting_time <= ADD_MOD( frame_counters[gas_id].FNmod42432, NPL1_FN_OFFSET, 42432 ) )
     {
        const uint8  BLK_BOUNDARY_FN_OFFSET[ 13 ]=
        {
            /* 0  1  2  3  4  5  6  7  8  9  10 11 12 */
               0, 3, 2, 1, 0, 3, 2, 1, 0, 4, 3, 2, 1
        };
         transfer_data_T*  transfer_data_ptr = l1_tskisr_blk->next_params.L1Data.pTransfer_data;
 
         if(l1_sci_is_op_in_progress(gas_id) )
         {
           if( FALSE == transfer_data_ptr->starting_time_valid)
           {
              l1_dedicated_data_ptr->starting_time  =
                                     ADD_MOD( frame_counters[gas_id].FNmod42432, (NPL1_FN_OFFSET+1), 42432 );
              l1_dedicated_data_ptr->starting_time =
                                      ADD_MOD( l1_dedicated_data_ptr->starting_time,
                     BLK_BOUNDARY_FN_OFFSET[ (l1_dedicated_data_ptr->starting_time%13) ],
                                                     42432 );
            
           }
           else
           {
               l1_dtm_temp_st = ADD_FN(gl1_get_FN(gas_id),NPL1_FN_OFFSET*4);
               gpl1_gprs_chk_blk_bndry(&l1_dtm_temp_st);
               transfer_data_ptr->starting_time =l1_dtm_temp_st;
               if(transfer_data_ptr->ul_st != L1_GPRS_INVALID_FN)
               {
                 transfer_data_ptr->ul_st = l1_dtm_temp_st;
               }
               if(transfer_data_ptr->dl_st != L1_GPRS_INVALID_FN)
               {
                 transfer_data_ptr->dl_st = l1_dtm_temp_st;
               }
               l1_dedicated_data_ptr->starting_time  = l1_dtm_temp_st % 42432;
               MSG_GERAN_HIGH_3_G(" Ded ST= %d , UL ST= %d , DL ST  %d",
                                 l1_dedicated_data_ptr->starting_time,transfer_data_ptr->ul_st,transfer_data_ptr->dl_st);
               
           }
           break;
         }
         

         /* Set this to TRUE
          * who is and when is it going to reset to FALSE ?
          */
         l1_transfer_data[gas_id].disable_rx_tx = TRUE;

         /* A reconfiguration of packet resources is still required
          * even if dtm assignment was cs only. This is because the MS
          * timebase potentially changes through the current timeslot.
          */
#if 0
         /* I do not think this is necessary since
          * we are assuming no pkt assignment arrived between
          * receiving the dtm assignment and starting dtm
          * mode.
          */
         l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;
#endif

         /* Null the main command to avoid un-necessary pre-dtm mode transfer_reconfigs*/
         if(  ( FALSE != transfer_data_ptr->starting_time_valid )
            &&( l1_dedicated_data_ptr->starting_time == (transfer_data_ptr->starting_time%42432) )
           )
         {
           l1_tskisr_blk->main_command = L1_NULL_COMMAND;
         }

         /* talk with state machine to "translate info " */

         l1_isr_data_ptr->dtm_command_event               = L1_DTM_ACTIVATE;
         l1_isr_data_ptr->l1_dtm_mode_state               = L1_DTM_NULL;
         gpl1_dtm_data[gas_id].l1_dtm_init_mode  = L1_DTM_CS_PS;
         gpl1_dtm_data[gas_id].l1_dtm_abort_mode = L1_DTM_NONE;

         reset_dedicated_command = TRUE;
     }
     else
     {

       MSG_GERAN_MED_2_G(" ded st %u FNmod42432 %u ", l1_dedicated_data_ptr->starting_time, frame_counters[gas_id].FNmod42432);
     }
     break;

     default:
     break;
   }
#endif /* FEATURE_GSM_EDTM && FEATURE_GSM_DTM */

   switch (l1_tskisr_blk->main_command)
   {
      /* normal operation */
      case L1_NULL_COMMAND:
         (void)gpl1_gprs_control_transfer(L1_MODE_RUN, gas_id);
         break;

      case L1_DEACTIVATE_COMMAND:
         if (gpl1_gprs_control_transfer(L1_MODE_STOP, gas_id))
         {
            /* change mode */
            L1_transfer_new_data( gas_id );

            l1_change_state( L1_INACTIVE_MODE, gas_id );
            /* l1_tskisr_blk->main_command will be set to
             * L1_NULL_COMMAND in the next frame tick
             */
         }
         break;

      case L1_IDLE_COMMAND:
      {
         if (l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR)
         {
            /* Clear wait for pca vars */
            l1_transfer_data[gas_id].wait_for_pca_fn    = L1_GPRS_INVALID_FN;
            l1_transfer_data[gas_id].wait_for_pca_valid = FALSE;
         }

         if (gpl1_gprs_control_transfer(L1_MODE_STOP, gas_id) )
         {
            idle_data_T  *idle_data_ptr;

            /* this should be done in the task */
            l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;


          	l1_tskisr_blk->main_command = L1_WAIT_FOR_MDSP_TO_COMPLETE;
          	l1_isr_data_ptr->mdsp_awake_panic = MAX_FRAMES_MDSP_AWAKE_BEFORE_PANIC;
          	MSG_GERAN_HIGH_0_G("l1_isr_transfer_mode - move to L1_WAIT_FOR_MDSP_TO_COMPLETED");
         }
      }
      break;
      case L1_WAIT_FOR_MDSP_TO_COMPLETE:
        {
          if ( mdsp_ready_for_sleep(gas_id) || ( l1_isr_data_ptr->mdsp_awake_panic == 0 ) )
          {
            idle_data_T  *idle_data_ptr;

            gl1_msg_abort_pwr_meas(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

#ifdef FEATURE_GTA_2PHASE_ACCESS 
			gpl1_gprs_reset_G2T_2phase_state_machine(gas_id);
#endif

            /*Send Recovery_cnf for Recovery_req from GL1 .recovery_cnf_pending is set once Recovery_req is recieved*/
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY

            if(TRUE == gpl1_gprs_get_recovery_cnf_pending(gas_id))
            {
              MSG_GERAN_HIGH_1_G("Moving To IDLE Due to PTM Error Recovery Recovery_pending=%d",gpl1_gprs_get_recovery_cnf_pending(gas_id));

              /*Debug crash will be done only if recovery EFS is enabled EFS Path: "/nv/item_files/gsm/l1/gl1_err_rec_enabled"*/

              if(gl1_gprs_rec_efs_enabled(gas_id)) 
              {
#ifdef FEATURE_QSH_MDUMP
                QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
                ERR_GERAN_FATAL_0("REC:DEBUG_CRASH please report this to geran.l1.pkt.transfer group");
              }

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
              #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/

            }
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

			if ( gl1_msg_get_multi_sim_mode() )
          	{
            	l1_change_state( L1_MULTI_SIM_IDLE, gas_id );

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
                /*Exchange the trm back to gsm client if gprs client is holding the chain*/
                if((l1_tskisr_blk->client_id == GRM_GPRS1) ||(l1_tskisr_blk->client_id == GRM_GPRS2))
                {
                  gpl1_ccch_xfer_exchange_trm_clients(gas_id);
                }
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/

               if (is_hw_tx_on(gas_id) == FALSE)
               {
                 grm_release_tx_chain(grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id),gas_id);
                 
               }
                /* Avoid releasing chain from here */
              if(grm_get_trm_status(l1_tskisr_blk->client_id,gas_id) == GL1_TRM_RETAINED_FOR_ACCESS)
              {
                 grm_set_trm_status(GL1_TRM_GRANTED,l1_tskisr_blk->client_id,gas_id);
              }
        
          	}
			else
#endif				
			{
            	l1_change_state( L1_IDLE_MODE, gas_id );
			}	



          	if (TRUE==l1_transfer_data[gas_id].send_l1_mac_single_block_sent)
          	{
            	/* Inform MAC that UL Single block has been sent.
             	* This is required since PL1 releases the single block tbf without
             	* any interaction between MAC and PL1
            	 */
             	l1_transfer_data[gas_id].send_l1_mac_single_block_sent = FALSE;

             	gpl1_gprs_send_l1_mac_single_block_sent(gas_id);
          	}

            

            /* Only send release confirm to MAC when we receive a release_req */
            if (l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR)
            {
              	MSG_GERAN_MED_0_G("L1_WAIT_FOR_MDSP_TO_COMPLETE rel confirm isr IDLE CMD ");

	              /*  Indicate to MAC release procedure is complete */
	              /*  send confirm  */
	            MSG_GERAN_MED_0_G("rel confirm isr IDLE CMD ");
	            l1_transfer_data[gas_id].confirm_pending = SEND_AT_IDLE_INIT;
            }

            L1_transfer_new_data( gas_id );
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;
            l1_tskisr_blk->sub_state = L1_IDLE_INIT;

            /* Set up the non drx periods when leaving transfer */
            gpl1_gprs_setup_idle_non_drx(gas_id);

            /*set timeslot according to CCCH_GROUP*/
            idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
            L1_change_timeslot((uint8)(idle_data_ptr->paging_data.ccch_group << 1), gas_id);
          }
          else
          {
            if ( l1_isr_data_ptr->mdsp_awake_panic > 0 )
            {
              	l1_isr_data_ptr->mdsp_awake_panic--;
            }

            MSG_GERAN_HIGH_1_G("MSIM mDSP not ready for deact from PTM %d",l1_isr_data_ptr->mdsp_awake_panic);
          }
        }
        break;

      case L1_ACCESS_COMMAND:
         /* not supported in transfer, needs removing */
         MSG_GERAN_ERROR_1_G("wrong main command %d",l1_tskisr_blk->main_command);
         break;

      case L1_TRANSFER_COMMAND:
      {
         /* reset transfer mode */
         transfer_data_T *transfer_data_ptr = l1_tskisr_blk->next_params.L1Data.pTransfer_data;

         /* First check if a starting time has been specified */
         if (transfer_data_ptr->starting_time_valid)
         {
            /* Is the starting time about to expire in 3 frames
             * assuming starting time is always aligned to block boundary
             */
            const uint32  current_fn = GSTMR_GET_FN_GERAN( gas_id);

            /* Improve the reaction time of L1
             * Only account for the NPL1 offset and hence add 1 tdma frame.
             */
            if (l1_transfer_data[gas_id].l1_called_from_sngl_blk)
            {
              /*If yes, move the start time by 1 block..until we come out of TA gap*/
                if( gl1_msg_get_multi_sim_standby_mode() && 
                    ( transfer_data_ptr->starting_time == ADD_FN( current_fn, GL1_DEFS_FRAMES_IN_BLK_PERIOD ))&&
                    ( l1_tskisr_blk->firmware_loaded == FALSE ))
                {
                  uint32 prev_ST = transfer_data_ptr->starting_time;
                
                  /*move the starting time by 1 block*/
                  transfer_data_ptr->starting_time = ADD_FN(transfer_data_ptr->starting_time , GL1_DEFS_FRAMES_IN_BLK_PERIOD);
                
                  /*check block boundary for starting time*/
                  gpl1_gprs_chk_blk_bndry(&transfer_data_ptr->starting_time);
                
                  if((NULL != transfer_data_ptr->pending_dynamic_tbf_ptr) && 
                	 (transfer_data_ptr->pending_dynamic_tbf_ptr->starting_time == prev_ST))
                  {
                    MSG_GERAN_HIGH_1_G("Pending Dynamic tbf ptr in TA gap - ST %d", transfer_data_ptr->pending_dynamic_tbf_ptr->starting_time);
                    transfer_data_ptr->pending_dynamic_tbf_ptr->starting_time = transfer_data_ptr->starting_time;
                    l1_transfer_data[gas_id].ul_st = transfer_data_ptr->starting_time;
                  }
                  
                  MSG_GERAN_HIGH_1_G("Starttime is near but we are in TA gap...move startime by 1 block...new st %d",transfer_data_ptr->starting_time );
                }
                else if( transfer_data_ptr->starting_time == ADD_FN( current_fn, ( NPL1_FN_OFFSET + 2 ) ) )
                {

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
                  /*If any own PCH/BCCH activity was planned/Ongoing, we need to cancel/abort it as we will do a low ts change at transfer init*/
                  if( transfer_data_ptr->starting_time <= ADD_FN( current_fn, ( NPL1_FN_OFFSET + 3 )) )
                  {
                    gpl1_g2x_ta_priority_t pri;
                    l1_sc_globals_T *l1_sc_globals_ptr	= gl1_ms_switch_l1_sc_globals_store(gas_id);
                    
                    if ( l1_sc_globals_ptr->monscan_in_progress )
                    {
                      MSG_GERAN_MED_0_G("Suspending Mon for possible lowest_ts_change gap" );
                      l1_sc_monscan_abort_gprs(gas_id);
                    }
                    
                    if(TRUE == gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_SERV_PCH, gas_id))
                    {
                      gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_SERV_PCH,gas_id);
                    }
                    if(TRUE == gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_SERV_BCCH, gas_id))
                    {
                      gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_SERV_BCCH,gas_id);
                    }
                    if (TRUE == gpl1_g2x_ta_is_activty_running( &pri,gas_id)) 
                    {
                      if (pri == G2X_TA_PRI_SERV_PCH)
                      {
                        MSG_GERAN_HIGH_1_G("PCHXFER Missed PCH due to lowest tn change collision ..aborting c_fn %d ",gl1_get_FN(gas_id));
                        L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND(gas_id);
                      }
                      else if (pri ==   G2X_TA_PRI_SERV_BCCH)
                      {
                        MSG_GERAN_HIGH_1_G("PCHXFER Missed BCCH due to lowest tn change collision ..aborting c_fn %d ",gl1_get_FN(gas_id));
                      }
                      
                      gpl1_g2x_ta_deregister_seq(gas_id);
                      l1_sc_abort_receive(gas_id);
                      l1_sci_tick_receive(gas_id);
                      gpl1_start_sc_tick_trans(gas_id);
                    }
                    /*Abort if any Ncell Activity is in progress*/
                    if (l1_sc_globals_ptr->receive_in_progress)
                    {
                      l1_sc_abort_receive(gas_id);
                      l1_sci_tick_receive(gas_id);
                    }
                  }
#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

                  /* Abort all current GPRS NPL1 processing */
                  gl1_msg_abort_dl_pacch_pdtch(gas_id);
                  gl1_msg_abort_dl_ptcch(gas_id);
                  gl1_msg_abort_int_meas(gas_id);

                  /* no need to stop anything for the PRACH ISR */
                  L1_transfer_new_data( gas_id );
                  l1_tskisr_blk->sub_state = L1_TRANSFER_INIT;
                  l1_tskisr_blk->sub_sub_state = NULL_STATE;
                  if (l1_transfer_data[gas_id].pending_frequency_info_ptr)
                  {
                     l1_transfer_data[gas_id].frequency_info_1 = *l1_transfer_data[gas_id].pending_frequency_info_ptr;
                     l1_transfer_data[gas_id].pending_frequency_info_ptr=NULL;

                  }

                  l1_transfer_data[gas_id].l1_called_from_sngl_blk = FALSE;
                  l1_tskisr_blk->main_command = L1_NULL_COMMAND;
                 }
             }
             else
             {
#if (defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )) && defined (FEATURE_USE_ACTUAL_TRM_DUR_UNLOCK)

               /*the below check is for checking if 4 frames before starting time, if we are in in TA gap or not*/
               /*If yes, move the start time by 1 block..until we come out of TA gap*/
               if( gl1_msg_get_multi_sim_standby_mode() && 
                   ( transfer_data_ptr->starting_time == ADD_FN( current_fn, GL1_DEFS_FRAMES_IN_BLK_PERIOD ))&&
                   (( ONGOING == gpl1_g2x_ta_get_act_status(G2X_TA_PRI_TRM_UNLOCK, gas_id)) ||( l1_tskisr_blk->firmware_load_status != FW_LOADED)))
               {
                 uint32 prev_ST = transfer_data_ptr->starting_time;

                 /*move the starting time by 1 block*/
                 transfer_data_ptr->starting_time = ADD_FN(transfer_data_ptr->starting_time , GL1_DEFS_FRAMES_IN_BLK_PERIOD);

                 /*check block boundary for starting time*/
                 gpl1_gprs_chk_blk_bndry(&transfer_data_ptr->starting_time);

                 if((NULL != transfer_data_ptr->pending_dl_tbf_ptr ) && 
                    (transfer_data_ptr->pending_dl_tbf_ptr->tbf_start_time == prev_ST))
                 {
                   MSG_GERAN_HIGH_1_G("Pending DL tbf ptr in TA gap - ST %d", transfer_data_ptr->pending_dl_tbf_ptr->tbf_start_time);
                   transfer_data_ptr->pending_dl_tbf_ptr->tbf_start_time = transfer_data_ptr->starting_time;
                   l1_transfer_data[gas_id].dl_st = transfer_data_ptr->starting_time;
                 }
                 if((NULL != transfer_data_ptr->pending_dynamic_tbf_ptr) && 
                    (transfer_data_ptr->pending_dynamic_tbf_ptr->starting_time == prev_ST))
                 {
                   MSG_GERAN_HIGH_1_G("Pending Dynamic tbf ptr in TA gap - ST %d", transfer_data_ptr->pending_dynamic_tbf_ptr->starting_time);
                   transfer_data_ptr->pending_dynamic_tbf_ptr->starting_time = transfer_data_ptr->starting_time;
                   l1_transfer_data[gas_id].ul_st = transfer_data_ptr->starting_time;
                 }
                 
                 MSG_GERAN_HIGH_1_G("Starttime is near but we are in TA gap...move startime by 1 block...new st %d",transfer_data_ptr->starting_time );

                 /*if there was a lowest tn chnage scheduled for this reconfig, move that too*/
                 if(TRUE == gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_TBF_TS_CHANGE, gas_id))
                 {
                   gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];

                   /*check if this low ts change is scheduled for this ST*/
                   if(data_ptr->table[G2X_TA_PRI_TBF_TS_CHANGE].frame_number == prev_ST)
                   {
                     MSG_GERAN_HIGH_0_G("Lowest tn change pending in TA gap...move it by 1 block");
                     (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_TBF_TS_CHANGE, gl1_map_gas_id_to_client_id(gas_id),transfer_data_ptr->starting_time,2, gas_id);
                     gpl1_g2x_ta_scheduler(gas_id);
                   }
                 }
               }
               else
#endif
                if( transfer_data_ptr->starting_time == ADD_FN( current_fn, NPL1_FN_OFFSET ) )
                {
                 l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;
                 l1_tskisr_blk->main_command = L1_NULL_COMMAND;
                }
             }


            if ((l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR) &&
                (l1_transfer_data[gas_id].wait_for_pca_valid))
            {
               /*  Indicate to MAC release procedure is complete */
               /*  send confirm  */
               gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending, gas_id);

               /* reset relelase type and rel confirm status*/
               l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
               l1_transfer_data[gas_id].confirm_pending = SENT;

               /* Clear wait for pca vars */
               transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
               transfer_data_ptr->wait_for_pca_valid = FALSE;
            }

            (void)gpl1_gprs_control_transfer(L1_MODE_RUN, gas_id);
         }
         else // no starting time so change state to transfer asap
         {
             /*we might be here due to a MAC L1 REL inside TA gap*/
#if (defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )) && defined (FEATURE_USE_ACTUAL_TRM_DUR_UNLOCK)
             boolean reconfig = TRUE;		

             if( gl1_msg_get_multi_sim_standby_mode() && 
                (( ONGOING == gpl1_g2x_ta_get_act_status(G2X_TA_PRI_TRM_UNLOCK, gas_id)) ||( l1_tskisr_blk->firmware_load_status != FW_LOADED)))
             {
               reconfig = FALSE;
             }
#endif
             if (l1_transfer_data[gas_id].l1_called_from_sngl_blk)
             {
                /***
                 *** Shouldn't get here !!!
                 ***/
                if(geran_get_nv_recovery_restart_enabled(gas_id))
                {
                  MSG_GERAN_ERROR_0_G("L1 Wrong single block setup");
                  l1_call_panic_reset_in_task(L1_WRONG_SINGLE_BLK_SETUP, gas_id);
                }
                else
                {
#ifdef FEATURE_QSH_MDUMP
                  QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
                  ERR_GERAN_FATAL_0_G("L1 Wrong single block setup");
                }
             }
             else
             {
#if (defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )) && defined (FEATURE_USE_ACTUAL_TRM_DUR_UNLOCK)
                if(reconfig == FALSE)
                {
                  MSG_GERAN_MED_0_G("Do not do Transfer RECONFIG as we are in TA gap");
                }
                else
                {
#endif
                  l1_tskisr_blk->sub_state = L1_TRANSFER_RECONFIG;

#if (defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )) && defined (FEATURE_USE_ACTUAL_TRM_DUR_UNLOCK)
                }
#endif 
             }
              
             /*When in G2XTA,reconfiguration will be delayed. So Release confirm also needs 
             to be delayed. Other wise we might end up at state mismatch in some corner cases*/
             if ((l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR) &&
                 (l1_transfer_data[gas_id].wait_for_pca_valid) &&
                 (reconfig != FALSE) )
             {
                MSG_GERAN_MED_0_G("rel confirm isr TRANS NO STARTING TIME ");

                /*  Indicate to MAC release procedure is complete */
                /*  send confirm  */
                gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending, gas_id);

                /* reset relelase type and rel confirm status*/
                l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
                l1_transfer_data[gas_id].confirm_pending = SENT;

                /* Clear wait for pca vars */
                transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
                transfer_data_ptr->wait_for_pca_valid = FALSE;
             }

#if (defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )) && defined (FEATURE_USE_ACTUAL_TRM_DUR_UNLOCK)
             if(reconfig == FALSE)
             {
               MSG_GERAN_MED_0_G("Do not set main command to NULL as we are in TA gap");
             }
             else
             {
#endif
               l1_tskisr_blk->main_command = L1_NULL_COMMAND;

#if (defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )) && defined (FEATURE_USE_ACTUAL_TRM_DUR_UNLOCK)
             }
#endif 

             /* call transfer mode ISR */
             (void)gpl1_gprs_control_transfer(L1_MODE_RUN, gas_id);
         }
      }
      break;

#ifdef  FEATURE_GPRS_PS_HANDOVER

      case L1_PSHO_COMMAND:
      case L1_PSHO_REVERT_COMMAND:
      {
         transfer_data_T *transfer_data_ptr = l1_tskisr_blk->next_params.L1Data.pTransfer_data;

         /* reset transfer mode */

         /* First check if a starting time has been specified */
         if (transfer_data_ptr->starting_time_valid)
         {
            /* Is the starting time about to expire in 3 frames
             * assuming starting time is always aligned to block boundary
             */
            const uint32  current_fn = GSTMR_GET_FN_GERAN( gas_id);
            MSG_GERAN_MED_1_G("ITP PSHO COMMAND - start time present on %d",current_fn);

            /* Improve the reaction time of L1
             * Only account for the NPL1 offset and hence add 1 tdma frame.
             */
            if( transfer_data_ptr->starting_time == ADD_FN( current_fn, NPL1_FN_OFFSET ) )
            {
              if( L1_PSHO_REVERT_COMMAND != l1_tskisr_blk->main_command )
              {
                /* After handling signalling of PSHO signal, tell MAC Boys */
                gpl1_gprs_send_l1_mac_psho_cnf( PSHO_SUCCESS, gas_id );

                MSG_GERAN_MED_2_G("PL1: L1_MAC_PSHO_CNF [%d] [%d]",PSHO_SUCCESS,current_fn);
              }

              l1_tskisr_blk->sub_state    = L1_PSHO_RECONFIG;
              l1_tskisr_blk->main_command = L1_NULL_COMMAND;
            }

            (void)gpl1_gprs_control_transfer(L1_MODE_RUN, gas_id);
         }
         else // no starting time so start PSHO asap
         {
             MSG_GERAN_MED_0_G("ITP PSHO Reconfigure - No start time present ");

             if( L1_PSHO_REVERT_COMMAND != l1_tskisr_blk->main_command )
             {
               /* After handling signalling of PSHO signal, tell MAC Boys */
               gpl1_gprs_send_l1_mac_psho_cnf( PSHO_SUCCESS, gas_id );

               MSG_GERAN_MED_1_G("PL1: L1_MAC_PSHO_CNF [%d] ",PSHO_SUCCESS);
             }

             l1_tskisr_blk->sub_state    = L1_PSHO_RECONFIG;
             l1_tskisr_blk->main_command = L1_NULL_COMMAND;

             /* @@OG: What happened to the PSHO success cnf here ? */

             /* call transfer mode ISR */
             (void)gpl1_gprs_control_transfer(L1_MODE_RUN, gas_id);
         }
      }
      break;

#endif /* case L1_PSHO_COMMAND */

      default:
         MSG_GERAN_HIGH_1_G("Bad L1 main_command %d.", l1_tskisr_blk->main_command);
         break;
   }

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
   /* @@OG: EDTM design */

   if( FALSE != reset_dedicated_command )
   {
     /* Are there any pending sysinfo read requests ? */
     gpl1_transfer_service_pending_sysinfo_reqs( gas_id );

     /* Initialise suspension params since they are not needed
      * in DTM.
      * This function resets the disable rx tx boolean to FALSE.
      */
#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
    
     /* initialise TBF suspension variables */
     gpl1_gprs_init_tuneaway_and_ncell_data( gas_id);
    
#else
      /* initialise TBF suspension variables */
     gpl1_gprs_init_tbf_suspension(gas_id);
    
#endif /*#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

     /* Before this function call happens and change state occurs.
      * Need to ensure all the dedicated mode initialisation is done
      * correctly.
      * The current DTM code assumes dedicated cs was already running
      * and hence is able to reconfigure it but not initialise and start it.
      * I need to decide on where this initialisation needs to happen.
      */
     l1_change_state( L1_DTM_MODE, gas_id ); /* This just sets the state into the taskisr blk */

     /* Setup the dtm and dedicated information pointers */
     l1_tskisr_blk->next_params.L1Data.pDedi_data = l1_dedicated_data_ptr;
     l1_tskisr_blk->next_params.L1Data.pDtm_data  = &gpl1_dtm_data[gas_id];

     /* Swap the next params to be current */
     L1_transfer_new_data( gas_id );

     l1_tskisr_blk->command   = L1_NULL_COMMAND;
     l1_tskisr_blk->sub_state = L1_EDTM_INIT;

     /* Do we still have any pending assignments? i.e. anything that
      * was received after the DTM assignment.
      * If yes then DTM mode handlers would need to know about it.
      */
     if( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
     {

#if 0  /* Might need to specify a new event e.g. L1_EDTM_TRASNFER_RECONFIG */

      if( FALSE != l1_transfer_data[gas_id].starting_time_valid )
      {
        /* Specify the dtm command event */
        l1_isr_data_ptr->dtm_command_event = L1_DTM_TRANSFER_RECONFIG;
        l1_isr_data_ptr->dtm_command_event = L1_DTM_RECONFIG_BOTH_CS_PS;

        if( GPL1_INVALID != l1_transfer_data[gas_id].dl_st )
        {

        }

        if( GPL1_INVALID != l1_transfer_data[gas_id].ul_st )
        {

        }
      }

      if( L1_NO_RELEASE != l1_transfer_data[gas_id].release_pending )
      {

      }
#endif

     }

     /* @@WARNING: Might need to supply a new transfer_reconfig
      * handler which is used in the EDTM init state only and allows
      * DTM CHANNEL assignment implicit release rules to be followed.
      */
   }

#endif  /* FEATURE_GSM_EDTM && FEATURE_GSM_DTM */

   /* Trigger CB tick */
   l1_tick_cbch( gas_id );

/*Trigger Recovery tick to recovery check */
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
   gpl1_gprs_recovery_tick(gas_id);
#endif/* FEATURE_GERAN_PTM_ERR_RECOVERY */

}

/*===========================================================================

FUNCTION  l1_isr_handle_find_bcch_command

DESCRIPTION
   Handle the main command mentioned in the function name in various modes.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_isr_handle_find_bcch_command( boolean  apply_freq_correction, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   switch (l1_tskisr_blk->command)
   {
      case L1_DECODE_BCCH_COMMAND:
         /*set up to reselect new serving cell*/
         L1_transfer_new_data( gas_id );
         l1_change_state( L1_FIND_BCCH, gas_id );
         l1_tskisr_blk->sub_state     = L1_GETTING_BCCH;
         l1_tskisr_blk->sub_sub_state = L1_BCCH_START;
         l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
         l1_tskisr_blk->command       = L1_NULL_COMMAND;

         /* change frame number */
         (void)L1_change_FN(l1_tskisr_blk->current_params.frame_lag, gas_id);

         /* slam frame tick */
         gl1_msg_correct_timing((int32)l1_tskisr_blk->current_params.QB_lag,gas_id);

         if( apply_freq_correction )
         {
           l1_sc_reselection_apply_freq_correction(l1_tskisr_blk->current_params.ARFCN,gas_id);

           /* Apply the frequency correction */
           l1_idle_data->reselection_apply_freq_correction = FALSE;
         }

         /* Send a reselection cnf if necessary */
         if (l1_tskisr_blk->current_params.reselection_cnf_reqd )
         {
            L1_send_MPH_RESELECTION_CNF( TRUE, gas_id );
            l1_tskisr_blk->current_params.reselection_cnf_reqd = FALSE;
            l1_tskisr_blk->next_params.reselection_cnf_reqd = FALSE;
         }

         /* we will start out on timeslot 0 */
         l1_set_current_timeslot(0, gas_id);

         l1_sc_set_sync(l1_tskisr_blk->current_params.frame_lag, /*FN*/
                        l1_tskisr_blk->current_params.QB_lag,     /*qbit_offset*/
                        gas_id);

         break;

      case L1_ACQUIRE_COMMAND:
         /* change mode */
         L1_transfer_new_data( gas_id );
         l1_change_state( L1_FIND_BCCH, gas_id );
         l1_tskisr_blk->sub_state     = L1_ACQUIRING;
         l1_tskisr_blk->sub_sub_state = L1_ACQ_INITIALISE;
         l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
         break;

      case L1_ACQUIRE_BCCH_LIST_COMMAND:
         L1_transfer_new_data( gas_id );
         l1_change_state( L1_FIND_BCCH, gas_id );
         l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
         l1_tskisr_blk->sub_state     = L1_GETTING_BCCH_LIST;
         l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_INIT;
         break;

      case L1_SCAN_COMMAND:
         L1_transfer_new_data( gas_id );
         l1_change_state( L1_FIND_BCCH, gas_id );
         l1_tskisr_blk->sub_state     = L1_SCANNING;
         l1_tskisr_blk->pscan_state = L1_SCAN_START;
         l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
         l1_pscan_mcpm_change(TRUE,gas_id);
         break;

      case L1_ACQUIRE_DURING_RESELECTION_COMMAND:
         L1_transfer_new_data( gas_id );
         l1_change_state( L1_FIND_BCCH, gas_id );
         l1_tskisr_blk->sub_state     = L1_ACQUIRING_DURING_RESELECTION;
         l1_tskisr_blk->sub_sub_state = L1_ACQ_INITIALISE;
         l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
         break;

      default:
         MSG_GERAN_HIGH_1_G("Bad L1 command %d.", l1_tskisr_blk->command);
         break;
   }
}

/*===========================================================================

FUNCTION  l1_isr_handle_find_sch_command

DESCRIPTION
   Handle the main command mentioned in the function name in various modes.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_isr_handle_find_sch_command( boolean  apply_freq_correction, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];

   switch (l1_tskisr_blk->command)
   {
      case L1_DECODE_SCH_COMMAND:
         /*set up to reselect new serving cell*/

         l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
         l1_tskisr_blk->command       = L1_NULL_COMMAND;

         L1_transfer_new_data( gas_id );

         l1_tskisr_blk->sub_state     = L1_GETTING_SCH;
         l1_tskisr_blk->sub_sub_state = L1_SCH_START;
         l1_change_state( L1_FIND_SCH, gas_id );

         /* change frame number */
         (void)L1_change_FN(l1_tskisr_blk->current_params.frame_lag, gas_id);

         /* slam frame tick */
         gl1_msg_correct_timing((int32)l1_tskisr_blk->current_params.QB_lag,gas_id);

         if( apply_freq_correction )
         {
            l1_sc_reselection_apply_freq_correction(l1_tskisr_blk->current_params.ARFCN,gas_id);

            /* Apply the frequency correction */
            l1_idle_data->reselection_apply_freq_correction = FALSE;
         }

     /* we will start out on timeslot 0 */
         l1_set_current_timeslot(0, gas_id);

         l1_sc_set_sync(l1_tskisr_blk->current_params.frame_lag, /*FN*/
                        l1_tskisr_blk->current_params.QB_lag,     /*qbit_offset*/
                        gas_id);
         break;

      default:
         MSG_GERAN_HIGH_1_G("Bad L1 command %d.", l1_tskisr_blk->command);
         break;
   }
}

/*===========================================================================

FUNCTION  l1_isr_jump_to_sdcch

DESCRIPTION
  Cause L1 to jump to SDCCH state.  This is for debugging purposes only.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_isr_jump_to_sdcch( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

   /* If flag set, jump to SDCCH */
   l1_sc_stop(gas_id); /* can't call from task in this case */
   if (L1_idle_mode_ISR( TRUE, gas_id ))
    {
      l1_sc_start(L1SCModeDediSDCCh, gas_id);
      l1_change_state( L1_DEDICATED_MODE, gas_id );
      l1_tskisr_blk->sub_state    = L1_DED_INIT;
      l1_tskisr_blk->main_command = L1_NULL_COMMAND;
      l1_tskisr_blk->next_params.L1Data.pDedi_data = l1_dedicated_data_ptr;

      L1_transfer_new_data( gas_id );
      L1_set_to_sdcch_mode(gas_id);
   }
}

/*===========================================================================

FUNCTION  L1_TRANSFER_NEW_DATA

DESCRIPTION
  This function copies the parameters for the new command being issued
  by the L1 task to the L1 ISR.  This is a mechanism for queueing two
  commands to the ISR simultaneously.  This function should only be
  invoked when the ISR is ready to service a new command.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_transfer_new_data ( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_tskisr_blk->current_params               =  l1_tskisr_blk->next_params;
}

/*===========================================================================

FUNCTION  L1_INITIALISE_ISR

DESCRIPTION
  This function sets up the hardware on power up or reset.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_initialise_isr( gas_id_t gas_id )
{
   gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];


   l1_idle_data->reselection_pending = FALSE;
   gprs_l1_idle_data->start_time_valid = FALSE;
   l1_idle_data->reselection_apply_freq_correction = FALSE;

}

/*===========================================================================

FUNCTION  L1_restart_frames_in_random_access_mode

DESCRIPTION
  This function re-starts the frames_in_random_access_mode to value 0

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_restart_frames_in_random_access_mode( gas_id_t gas_id )
{
  l1_isr_data[gas_id].l1_frames_in_random_access_mode = 0;
}

/*===========================================================================

FUNCTION  GL1_DEACTIVATE

DESCRIPTION
  This function prepares L1 to go inactive.



DEPENDENCIES
  None

RETURN VALUE
  TRUE if going to deactivate, FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
static boolean gl1_deactivate( gas_id_t gas_id )
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  boolean success = gl1_hw_inact_isr(FALSE,gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( success )
  {
    l1_tskisr_blk->firmware_loaded = FALSE;
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  return success;
}


/*===========================================================================

FUNCTION  l1_state_log, l1_state_log_dump

DESCRIPTION
  Create a log of the L1 states and commands by saving these to a buffer
  each time they change.  Used for debugging.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_state_log( boolean start_of_isr, gas_id_t gas_id )
{
   l1_NVLog_struct_T *l1_nv_log_ptr=&l1_nv_log[gas_id];
   l1_state_log_T *l_current_value_ptr = &current_values[gas_id];
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];


   if ( (l_current_value_ptr->state        != l1_tskisr_blk->l1_state) ||
        (l_current_value_ptr->sub_state    != l1_tskisr_blk->sub_state) ||
        (l_current_value_ptr->main_command != l1_tskisr_blk->main_command) ||
        (l_current_value_ptr->command      != l1_tskisr_blk->command) )
   {
      l_current_value_ptr->state        = l1_tskisr_blk->l1_state;
      l_current_value_ptr->sub_state    = l1_tskisr_blk->sub_state;
      l_current_value_ptr->main_command = l1_tskisr_blk->main_command;
      l_current_value_ptr->command      = l1_tskisr_blk->command;
      l_current_value_ptr->FN           = gl1_get_FN( gas_id );
      l_current_value_ptr->start_of_isr = start_of_isr;
      #ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
      l_current_value_ptr->init_tbf_susp = trans[gas_id].tuneaway_and_ncell_data_initialized;
      #endif


      if (l1_nv_log_ptr->l1_state_log_buf_index >= L1_STATE_LOG_LEN)
      {
         l1_nv_log_ptr->l1_state_log_buf_index = 0;
      }

      l1_nv_log_ptr->l1_state_log_buf[l1_nv_log_ptr->l1_state_log_buf_index] = current_values[gas_id];
      l1_nv_log_ptr->l1_state_log_buf_index++;
   }
}

void l1_state_log_dump( gas_id_t gas_id )
{
   l1_NVLog_struct_T *l1_nv_log_ptr=&l1_nv_log[gas_id];
   uint32 ix;

   MSG_GERAN_ERROR_0_G("===============");
   for (ix=0; ix < L1_STATE_LOG_LEN; ix++)
   {
      MSG_GERAN_ERROR_3_G("l1_state=%d, FN=%d, soi=%d", l1_nv_log_ptr->l1_state_log_buf[ix].state,
                l1_nv_log_ptr->l1_state_log_buf[ix].FN,
                l1_nv_log_ptr->l1_state_log_buf[ix].start_of_isr);
      MSG_GERAN_ERROR_3_G("sub_st=%d, main_cmd=%d, cmd=%d", l1_nv_log_ptr->l1_state_log_buf[ix].sub_state,
                l1_nv_log_ptr->l1_state_log_buf[ix].main_command,
                l1_nv_log_ptr->l1_state_log_buf[ix].command);
      MSG_GERAN_ERROR_0_G("--");
   }
}

/*===========================================================================

FUNCTION  L1_do_background_HPLMN_search

DESCRIPTION
  This function runs the appropriate handler depending on which component
  of the background HPLMN search is active i,e POWER_SCAN, BCCH_LIST or
  ACQ_LIST



DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void L1_do_background_HPLMN_search(gas_id_t gas_id)
{
  l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];


   switch ( L1_get_current_background_HPLMN_search_activity(gas_id))
   {
      case POWER_SCAN:
            if (!L1_background_HPLMN_search_running(gas_id))
            {
              /* Get the minimum no. of frames available to the next Ncell activity or CCCH block */
              l1_isr_data_ptr->frames_required = (int32)(l1_fm_get_available_frames(FM_PRI_BACKGROUND_PLMN, gl1_get_FN( gas_id ), gas_id)-2);

              /* We will resume BPLMN only if there are more than 8 frames to the next Ncell activity
               * or CCCH block
               */
              if(l1_isr_data_ptr->frames_required > 2*FRAMES_PER_BLOCK)
              {
#ifdef BPLMN_DEBUG
                #error code not present
#endif
                l1_fm_set_required_frame_number(FM_PRI_BACKGROUND_PLMN, gl1_get_FN( gas_id ), (uint32)l1_isr_data_ptr->frames_required, gas_id);

                if (l1_fm_are_frames_available(FM_PRI_BACKGROUND_PLMN,(uint32)l1_isr_data_ptr->frames_required,
                                                                     L1_pscan_suspend, 0, gas_id))
                {
                    /* We have suspended PLMN search before. Since frames are available now,
                     * resume the search.
                     */
                    MSG_GERAN_MED_1_G("Resuming background HPLMN search FN=%d",gl1_get_FN( gas_id ));

                    /* We should suspend BPLMN atleast 4 frames before the next Ncell activity or
                     * CCCH block to allow for the abort to complete in time.
                     */
                    l1_isr_data_ptr->frames_required -= FRAMES_PER_BLOCK;
                    L1_pscan_resume(gas_id);
                    L1_power_scan(gas_id);
                }
              }
            }
            else
            {
              volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
              l1_isr_data_ptr->frames_required--;

#ifdef BPLMN_DEBUG
              #error code not present
#endif
              
              /* Time to suspend BPLMN if we have run out of frames unless we have received Suspend already */
              if((l1_isr_data_ptr->frames_required == 0) && (l1_tskisr_blk->next_params.L1_confirm_ID != L1_POWER_SCAN_CNF))              
              {
                 L1_pscan_suspend(0,gas_id);
              }
              L1_power_scan(gas_id);
            }

            /* Don't sleep when background PLMN search running */
            gl1_drx_require_next_tick(gas_id);
            break;

      case ACQ_BCCH_LIST:
            if (!L1_background_HPLMN_search_running(gas_id))
            {
              /* Get the minimum no. of frames available to the next Ncell activity or CCCH block */
              l1_isr_data_ptr->frames_required = (int32)(l1_fm_get_available_frames(FM_PRI_BACKGROUND_PLMN, gl1_get_FN( gas_id ), gas_id)-2);

              /* We will resume BPLMN only if there are more than 8 frames to the next Ncell activity
               * or CCCH block
               */
              if(l1_isr_data_ptr->frames_required > 2*FRAMES_PER_BLOCK)
              {
#ifdef BPLMN_DEBUG
                #error code not present
#endif
                l1_fm_set_required_frame_number(FM_PRI_BACKGROUND_PLMN, gl1_get_FN( gas_id ), (uint32)l1_isr_data_ptr->frames_required, gas_id);

                if (l1_fm_are_frames_available(FM_PRI_BACKGROUND_PLMN,(uint32)l1_isr_data_ptr->frames_required,
                                                          L1_bcch_list_acq_bcch_suspend, 0, gas_id))
                {
                    /* We have suspended PLMN search before. Since frames are available now,
                     * resume the search.
                     */
                    MSG_GERAN_MED_1_G("Resuming background HPLMN search FN=%d",gl1_get_FN( gas_id ));

                    /* We should suspend BPLMN atleast 4 frames before the next Ncell activity or
                     * CCCH block to allow for the abort to complete in time.
                     */
                    l1_isr_data_ptr->frames_required -= FRAMES_PER_BLOCK;
                    L1_bcch_list_acq_bcch_resume(gas_id);
                    (void)L1_get_bcch_list_fcch_sch_bcch(gas_id);
                }
              }
            }
            else
            {
              l1_isr_data_ptr->frames_required--;

#ifdef BPLMN_DEBUG
              #error code not present
#endif
              /* Time to suspend BPLMN if we have run out of frames. */
              if(l1_isr_data_ptr->frames_required == 0)
              {
                 L1_bcch_list_acq_bcch_suspend(0, gas_id);
              }
              (void)L1_get_bcch_list_fcch_sch_bcch(gas_id);
            }

            /* Don't sleep when background PLMN search running */
            gl1_drx_require_next_tick(gas_id);
            break;


      case NO_HPLMN_SEARCH:
      default:
            break;
   }
}

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
static boolean l1_do_WTOG_BPLMN_search( gas_id_t gas_id )
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  trm_resource_enum_t       resource;
#endif

  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];

    if (l1_get_wtog_bplmn_task_substate(gas_id) == WTOG_BPLMN_SUSPENDED)
    {
       /* We are still waiting on the hardware to initialize */
       return FALSE;
    }
    if ( l1_get_wtog_bplmn_task_substate(gas_id) == WTOG_BPLMN_STARTING )
    {
      /* In single sim mode, GL1 needs one frame to finish RF wakeup and FW's enter_mode.
         In rare case, a MPH_STOP_GSM_REQ comes right after MPH_RESUME_GSM_MODE_FOR_BPLMN_REQ.
         Without waiting here for one frame, GL1 may send rf_sleep before FW finishes
         enter_mode. RF would crash if it happens.
         Multisim mode is fine since GL1 has the states for TRM request, FW loading etc.
       */
      l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_ACTIVE,gas_id);
      return FALSE;
    }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if ( l1_get_wtog_bplmn_task_substate(gas_id) == WTOG_BPLMN_TRM_REQUEST )
    {
      uint32 drx_interval;
     /*
      * Now need to keep requesting time from TRM until its granted. Also need to keep
      * a check on the drx_interval so we can suspend in time to allow WCDMA back in the
      * picture.
      */
      l1_decrement_drx_interval(gas_id);
      drx_interval = l1_get_drx_interval(gas_id);

      if (( drx_interval < MIN_USEFUL_BPLMN_DURATION_FRAMES )||(L1_DEACTIVATE_COMMAND == l1_tskisr_blk->main_command))
      {
        /* SUSPEND search here - need to stop theres no more time left */
        l1_isr_data_ptr->l1_bplmn_suspend_in_progress = TRUE;

        if( L1_DEACTIVATE_COMMAND == l1_tskisr_blk->main_command )
        {
          l1_set_gsm_stop_req_in_queue(TRUE,gas_id);
          MSG_GERAN_MED_1_G(" Enforce the stop_mode_req FN %u ", GSTMR_GET_FN_GERAN( gas_id ));
        }

        l1_tskisr_blk->main_command = L1_SUSPEND_COMMAND;
        l1_isr_data_ptr->suspension_timer = 0;

        MSG_GERAN_MED_2_G("Suspending W2G BPLMN at %d drx interval %d min useful", l1_get_drx_interval(gas_id),MIN_USEFUL_BPLMN_DURATION_FRAMES);
      }
      else
      {
        trm_grant_return_enum_type trm_grant_status = TRM_RETURN_MAX;
        trm_reason_enum_t reason;

        if (l1_get_wtog_bplmn_task_state(gas_id) == POWER_SCAN)
        {
          reason = TRM_ACQUISITION;
        } else
        {
          reason = TRM_ACQUISITION_CELL_SELECT;
        }
        reason = grm_get_acquisition_reason(reason,gas_id);
        resource = gl1_ms_get_trm_resource( gas_id, reason );

        trm_grant_status =
          grm_request( l1_tskisr_blk->client_id,
                       timetick_cvt_to_sclk(MIN_USEFUL_BPLMN_DURATION_MS, T_MSEC),
                       reason,
                       SYS_PROC_TYPE_BPLMN_SEARCH,
                       resource,
                       GRM_DIVRX_TRM_REQ_INVALID);
        grm_set_trm_grant_status(trm_grant_status, l1_tskisr_blk->client_id, gas_id);

        if (trm_grant_status != TRM_DENIAL )
        {
          /* Now we have the minimum duration, keep the lock for as long as possible and just
          register the unlock callback with TRM. This should result in a value getting set within
          l1_tskisr_blk->frame_duration */
          grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);

          /* Initialise this to the longest time to start with */
          grm_set_frame_duration(l1_get_drx_interval(gas_id), l1_tskisr_blk->client_id, gas_id);

          grm_retain_lock(l1_tskisr_blk->client_id,
                           (grm_unlock_callback_t)&grm_normal_acq_unlock,
                           gas_id );

          /* Need to load the RF and FW now, and only move to runnning once ready, this will be
          indicated by the flag l1_tskisr_blk->firmware_loaded */
          gl1_multi_sim_fw_rf_load( l1_tskisr_blk->client_id, gas_id );
          l1_isr_data_ptr->rf_fw_warmup_timer = RF_FW_WARMUP_FRAME_DELAY;
          l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_RF_FW_LOADING,gas_id);

          l1_subtract_drx_interval( SUSPENSION_FRAME_DELAY, gas_id );
          l1_isr_data_ptr->leaving_bplmn = FALSE;
        }
        else if ( trm_grant_status == TRM_DENIAL )
        {
          grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;
          grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;

          grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;

            /* Ask and wait for a callback when granted. The duration is irrelevant here */
            /* as its unknown and is highest priority anyway. */
          grm_request_and_notify_enhanced_data_p->client_id = l1_tskisr_blk->client_id;
          grm_request_and_notify_enhanced_data_p->resource  = resource;
          grm_request_and_notify_enhanced_data_p->duration  = timetick_cvt_to_sclk(MIN_USEFUL_BPLMN_DURATION_MS, T_MSEC); 
          grm_request_and_notify_enhanced_data_p->reason    = reason;
          grm_request_and_notify_enhanced_data_p->sub_reason = SYS_PROC_TYPE_BPLMN_SEARCH;

          grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_ACQUISITION;

          grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, l1_tskisr_blk->client_id, gas_id);

          l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_TRM_WAIT,gas_id );

          call_in_task_grm_request_and_notify_enhanced( grm_request_and_notify_enhanced_data_p );
        }
      }
      return (l1_isr_data_ptr->leaving_bplmn);
    }
    else
    if ( l1_get_wtog_bplmn_task_substate(gas_id) == WTOG_BPLMN_TRM_WAIT )
    {
      uint32 drx_interval;
      gl1_trm_state_t gl1_trm_state;
     /*
      * Now need to keep requesting time from TRM until its granted. Also need to keep
      * a check on the drx_interval so we can suspend in time to allow WCDMA back in the
      * picture.
      */
      l1_decrement_drx_interval(gas_id);
      drx_interval = l1_get_drx_interval(gas_id);
      gl1_trm_state = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);

      if ((drx_interval < MIN_USEFUL_BPLMN_DURATION_FRAMES) || (L1_DEACTIVATE_COMMAND == l1_tskisr_blk->main_command)
          || (gl1_trm_state == GL1_TRM_ABORTING))
      {
        /* SUSPEND search here - need to stop theres no more time left */
        l1_isr_data_ptr->l1_bplmn_suspend_in_progress = TRUE;

        if( L1_DEACTIVATE_COMMAND == l1_tskisr_blk->main_command )
        {
          l1_set_gsm_stop_req_in_queue(TRUE,gas_id);
          MSG_GERAN_MED_1_G(" Enforce the stop_mode_req FN %u ", GSTMR_GET_FN_GERAN( gas_id ));

        }

        call_in_task_grm_release( l1_tskisr_blk->client_id );

        l1_tskisr_blk->main_command = L1_SUSPEND_COMMAND;
        l1_isr_data_ptr->suspension_timer = 0;

        MSG_GERAN_MED_3_G("Suspending W2G BPLMN at drx interval %d, main_command=%d, trm_state=%d", l1_get_drx_interval(gas_id),l1_tskisr_blk->main_command, gl1_trm_state);
      }
      else if ( (gl1_trm_state == GL1_TRM_GRANTED) || (gl1_trm_state == GL1_TRM_RETAINED_FOR_ACCESS) )
      {
          gl1_multi_sim_fw_rf_load( l1_tskisr_blk->client_id, gas_id );
          l1_isr_data_ptr->rf_fw_warmup_timer = RF_FW_WARMUP_FRAME_DELAY;
          l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_RF_FW_LOADING,gas_id);
          l1_isr_data_ptr->leaving_bplmn = FALSE;
      }

           return (l1_isr_data_ptr->leaving_bplmn);
    }
    else
    if (l1_get_wtog_bplmn_task_substate(gas_id) == WTOG_BPLMN_RF_FW_LOADING)
    {
      boolean ret = TRUE;
      /*
       * We need to clear this if still set as it means the normal deactivate
       * hasn't been actioned for some reason which leads to the BCCH reads
       * not being stopped when requested and the mdsp later crashes
       */
      l1_decrement_drx_interval(gas_id);
      (void)grm_get_post_decrement_frame_duration(l1_tskisr_blk->client_id, gas_id);


      if ((l1_get_drx_interval(gas_id) == 0) ||
          (grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id) == 0) ||
          (grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_ABORTING) ||
          (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
         )
      {
        /* SUSPEND search here and release TRM - need to stop theres no more time left */
        l1_isr_data_ptr->l1_bplmn_suspend_in_progress = TRUE;

        if( L1_DEACTIVATE_COMMAND == l1_tskisr_blk->main_command )
        {
          l1_set_gsm_stop_req_in_queue(TRUE,gas_id);
          MSG_GERAN_MED_1_G(" Enforce the stop_mode_req FN %u ", GSTMR_GET_FN_GERAN( gas_id ));
        }

        l1_tskisr_blk->main_command = L1_SUSPEND_COMMAND;

        call_in_task_grm_release( l1_tskisr_blk->client_id );

        /* If TRM is released then make suspension timer 0
        * FW can be accessed if suspension_timer is running
        */
        l1_isr_data_ptr->suspension_timer = 0;
      }
      else
      if ( l1_tskisr_blk->firmware_loaded == TRUE )
      {
        if ( l1_get_wtog_bplmn_task_state(gas_id) == POWER_SCAN )
        {
          L1_pscan_resume(gas_id);
        }
        else
        {
          L1_bcch_list_acq_bcch_resume(gas_id);
          ret = FALSE;
        }
  
        l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_RUNNING,gas_id);
      }
      else
      {
        MSG_GERAN_HIGH_1_G("WTOG_BPLMN_RF_FW_LOADING %d",
                            grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id));
      }

      if(ret)
      {
        return (l1_isr_data_ptr->leaving_bplmn);
      }
      /*else continue further*/
    }
    else
    if ( l1_get_wtog_bplmn_task_substate(gas_id) == WTOG_BPLMN_MULTI_SIM_SUSPENDING )
    {
      if ( l1_isr_data_ptr->suspension_timer )
      {
        l1_isr_data_ptr->suspension_timer--;
        /*
         Check if this the final time through that the drivers have actually finished.
         And if not perform another iteration of the suspension
         */
        if ( (l1_isr_data_ptr->suspension_timer == 0) &&
             (((gpl1_multi_sim_bplmn_decode_bcch_inactive(gas_id) == FALSE) && (l1_get_wtog_bplmn_task_state(gas_id) == ACQ_BCCH_LIST)) ||
             ((l1_get_wtog_bplmn_task_state(gas_id) == POWER_SCAN) && (TRUE == gl1_msg_get_meas_in_progress(gas_id)) && ( l1_tskisr_blk->main_command != L1_DEACTIVATE_COMMAND ))))
        {
          l1_isr_data_ptr->suspension_timer++;
        }
      }
      if( l1_isr_data_ptr->suspension_timer )
      {
        MSG_GERAN_HIGH_0_G("Dual SIM W2G Process suspension timer running");
        /* keep ticking the schedulers during this time */
        switch (l1_get_wtog_bplmn_task_state(gas_id))
        {
          case POWER_SCAN:
            L1_power_scan(gas_id);
            break;
          case ACQ_BCCH_LIST:
            {
              L1_get_bcch_list_fcch_sch_bcch(gas_id);

              }
            break;
          default: MSG_GERAN_ERROR_1_G("Multi-SIM W2G suspension in incorrect state %d", l1_get_wtog_bplmn_task_state(gas_id));
        }
      }
      else if ( l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
      {
        MSG_GERAN_MED_0_G("Multi-SIM Handle stop_mode suspension");
        l1_isr_data_ptr->leaving_bplmn = TRUE;
      }
      else
      {
       if (gl1_deactivate(gas_id))
       {
        gl1_trm_state_t trm_status = GL1_TRM_NULL;
        l1_tskisr_blk->main_command = L1_NULL_COMMAND;
        l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_SUSPENDED,gas_id);
        l1_isr_watchdog_deactivate(gas_id);
        L1_send_ISR_SUSPEND_CNF( 0,gas_id );

        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);

        if ( ( trm_status == GL1_TRM_GRANTED ) ||
             ( trm_status == GL1_TRM_ABORTING ) )
        {
          call_in_task_grm_release( l1_tskisr_blk->client_id );
        }
        MSG_GERAN_HIGH_1_G("Multi-SIM W2G Process terminated, trm_status=%d", trm_status);
       }
      }
      return (l1_isr_data_ptr->leaving_bplmn);
    }
    else if ( l1_get_wtog_bplmn_task_substate(gas_id) == WTOG_BPLMN_MULTI_SIM_ENDING )
    {
      if (l1_isr_data_ptr->suspension_timer)
      {
        l1_isr_data_ptr->suspension_timer--;
        l1_isr_data_ptr->leaving_bplmn = FALSE;
        MSG_GERAN_HIGH_0_G("Multi SIM W2G Ending timer running");
      }else
      {
        l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_ACTIVE, gas_id);
        l1_isr_data_ptr->leaving_bplmn = TRUE;
        call_in_task_grm_release( l1_tskisr_blk->client_id );
        MSG_GERAN_HIGH_0_G("Multi SIM W2G Ended");
      }
    }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

    switch (l1_get_wtog_bplmn_task_state(gas_id))
    {
      case POWER_SCAN:
         if (l1_get_wtog_bplmn_task_substate(gas_id) == WTOG_BPLMN_ACTIVE)
         {
            /* We have suspended PLMN search before. Since frames are available now,
             * resume the search.
             */
            MSG_GERAN_MED_1_G("PSCAN: Resuming WtoG background HPLMN search FN=%d",gl1_get_FN( gas_id ));

            /*
             * We need to clear this if still set as it means the normal deactivate
             * hasn't been actioned for some reason which leads to the BCCH reads
             * not being stopped when requested and the mdsp later crashes
             */
            if ( l1_isr_data_ptr->l1_bplmn_suspend_in_progress )
            {
              MSG_GERAN_ERROR_0_G( "Clearing l1_bplmn_suspend_in_progress");

              l1_isr_data_ptr->l1_bplmn_suspend_in_progress = FALSE;
            }

            l1_decrement_drx_interval(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            if ( gl1_msg_get_multi_sim_mode() )
            {
              /* if we are in dual sim mode then move to the state to request resoures
              from trm here */
              l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_TRM_REQUEST,gas_id);
              break;
            }
            else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
            {
              l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_RUNNING,gas_id);
              L1_pscan_resume(gas_id);
            }
         }
         else
         {
            l1_decrement_drx_interval(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            (void)grm_get_post_decrement_frame_duration(l1_tskisr_blk->client_id, gas_id);
            if( ( l1_get_drx_interval(gas_id) == 0 ) ||
                ( gl1_msg_get_multi_sim_mode() &&
                ((grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_ABORTING) ||
                 (grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id) == 0)))
              )
#else
            if(l1_get_drx_interval(gas_id) == 0)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      {
               l1_isr_data_ptr->l1_bplmn_suspend_in_progress = TRUE;

               if(  ( L1_DEACTIVATE_COMMAND == l1_tskisr_blk->main_command )
                  &&( WTOG_BPLMN_SUSPENDED  != l1_get_wtog_bplmn_task_substate(gas_id) )
                 )
               {
                 l1_set_gsm_stop_req_in_queue(TRUE,gas_id);

                 MSG_GERAN_MED_1_G(" Enforce the stop_mode_req FN %u ", GSTMR_GET_FN_GERAN( gas_id ));
               }

               L1_pscan_suspend(0,gas_id);
            }
         }

         L1_power_scan(gas_id);
         l1_isr_data_ptr->leaving_bplmn = TRUE;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         /*
         Ensure that if dual sim that the TRM lock is released correctly at the end of the pscan
         process, or if it has been suspended
         */
         if ( gl1_msg_get_multi_sim_mode() )
         {
           if ( L1_DEACTIVATE_COMMAND == l1_tskisr_blk->main_command )
           {
             /* L1_power_scan() will detect that the stop_gsm_mode_req has come in and
                terminate. Handle the suspension timer setup here. */
             l1_isr_data_ptr->suspension_timer = SUSPENSION_FRAME_DELAY;
             l1_set_wtog_bplmn_task_substate( WTOG_BPLMN_MULTI_SIM_SUSPENDING ,gas_id);

             MSG_GERAN_HIGH_0_G("Multi-SIM W2G PSCAN Process suspending for stop mode");

             l1_isr_data_ptr->leaving_bplmn = FALSE;
           }
           else
           {
             switch ( l1_tskisr_blk->pscan_state )
             {
             case L1_SCAN_END:
               {
                 l1_isr_data_ptr->suspension_timer = SUSPENSION_FRAME_DELAY;
                 MSG_GERAN_HIGH_0_G("Multi-SIM W2G PSCAN Ending");
                 l1_set_wtog_bplmn_task_substate( WTOG_BPLMN_MULTI_SIM_SUSPENDING, gas_id );
                 l1_isr_data_ptr->leaving_bplmn = FALSE;
               }
               break;

             case L1_SCAN_WAIT_TO_RESUME_MEAS_PERIOD:
               {
                 l1_isr_data_ptr->suspension_timer = SUSPENSION_FRAME_DELAY;
                 MSG_GERAN_HIGH_0_G("Multi-SIM W2G PSCAN Process starting suspension timer");
                 l1_set_wtog_bplmn_task_substate( WTOG_BPLMN_MULTI_SIM_SUSPENDING, gas_id );
                 l1_isr_data_ptr->leaving_bplmn = FALSE;
               }
               break;

             default: MSG_GERAN_LOW_0_G("no handler");
             }
           }
         }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
         break;

      case ACQ_BCCH_LIST:
         if (l1_get_wtog_bplmn_task_substate(gas_id) == WTOG_BPLMN_ACTIVE)
         {
            /* We have suspended PLMN search before. Since frames are available now,
             * resume the search.
             */
            MSG_GERAN_MED_1_G("ACQ_BCCH: Resuming WtoG background HPLMN search FN=%d",gl1_get_FN( gas_id ));

            /*
             * We need to clear this if still set as it means the normal deactivate
             * hasn't been actioned for some reason which leads to the BCCH reads
             * not being stopped when requested and the mdsp later crashes
             */
            if ( l1_isr_data_ptr->l1_bplmn_suspend_in_progress )
            {
              MSG_GERAN_ERROR_0_G( "Clearing l1_bplmn_suspend_in_progress");

              l1_isr_data_ptr->l1_bplmn_suspend_in_progress = FALSE;
            }

            l1_decrement_drx_interval(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            if ( gl1_msg_get_multi_sim_mode() )
            {
              /* if we are in dual sim mode then move to the state to request resoures
              from trm here */
              l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_TRM_REQUEST,gas_id);
              break;
            }
            else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
            {
              l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_RUNNING,gas_id);
              L1_bcch_list_acq_bcch_resume(gas_id);
            }
         }
         else
         {
            l1_decrement_drx_interval(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            grm_get_post_decrement_frame_duration(l1_tskisr_blk->client_id, gas_id);
            if( ( l1_get_drx_interval(gas_id) == 0 ) ||
                ( gl1_msg_get_multi_sim_mode() &&
                  ((grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_ABORTING) ||
                   (grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id) == 0))
                  )
              )
#else
            if(l1_get_drx_interval(gas_id) == 0)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      {
               l1_isr_data_ptr->l1_bplmn_suspend_in_progress = TRUE;

               if(  ( L1_DEACTIVATE_COMMAND == l1_tskisr_blk->main_command )
                  &&( WTOG_BPLMN_SUSPENDED  != l1_get_wtog_bplmn_task_substate(gas_id) )
                 )
               {
                 l1_set_gsm_stop_req_in_queue(TRUE,gas_id);

                 MSG_GERAN_MED_1_G(" Enforce the stop_mode_req FN %u ", GSTMR_GET_FN_GERAN( gas_id ));
               }

               L1_bcch_list_acq_bcch_suspend(0, gas_id);
            }
         }

         l1_isr_data_ptr->leaving_bplmn = L1_get_bcch_list_fcch_sch_bcch(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         /*
         Ensure that if dual sim that the TRM lock is released correctly at the end of the bcch acq
         process, or if it has been suspended.
         */
         if ( gl1_msg_get_multi_sim_mode() )
         {
           if ( l1_isr_data_ptr->leaving_bplmn && ( l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND) )
           {
             /* L1_get_bcch_list_fcch_sch_bcch will detect that the stop_gsm_mode_req has come in and
             terminate. Handle the suspension timer setup here. */
             l1_isr_data_ptr->suspension_timer = SUSPENSION_FRAME_DELAY;
             l1_set_wtog_bplmn_task_substate( WTOG_BPLMN_MULTI_SIM_SUSPENDING,gas_id );

             MSG_GERAN_HIGH_0_G("Multi-SIM W2G ACQ Process suspending for stop mode");

             l1_isr_data_ptr->leaving_bplmn = FALSE;
           }
           else
           if ( l1_isr_data_ptr->l1_bplmn_suspend_in_progress )
           {
             /* The search has been suspended start the suspension timer */
             l1_isr_data_ptr->suspension_timer = SUSPENSION_FRAME_DELAY;
             MSG_GERAN_HIGH_0_G("Dual SIM W2G ACQ Process starting suspension timer");
             l1_set_wtog_bplmn_task_substate( WTOG_BPLMN_MULTI_SIM_SUSPENDING,gas_id );
             l1_isr_data_ptr->leaving_bplmn = FALSE;
           }
           else
           if ( l1_get_wtog_bplmn_task_state(gas_id) == NO_HPLMN_SEARCH )
           {
             /* release the lock the BCCH list search is complete */
             call_in_task_grm_release( l1_tskisr_blk->client_id );

             MSG_GERAN_HIGH_0_G("Multi-SIM W2G ACQ Process completed");
           }
         }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

         break;

      case NO_HPLMN_SEARCH:
      default:
         if(l1_get_drx_interval(gas_id) > 0)
         {
           /* CR262337 - drx_interval is the number of GSM frames until the
            * next WCDMA page. When RRC sends RR_INTERRAT_PLMN_SRCH_ABORT_REQ
            * to RR if drx_interval is a 'large' value(greater than 100ms)
            * then RRC timer will expire - RRC will try to carry on and activate
            * WCDMA but G still hasn't aborted!
            * Should be safe to limit this value to 70ms - RRC have agreed to
            * remove this timer going forward(CR262947) but there is no dependancy
            */
           if (l1_get_drx_interval(gas_id) > L1_MAX_DRX_INTERVAL_VAL)
           {
             MSG_GERAN_HIGH_3_G("Changing drx_interval val %d-->%d l1_bplmn_sus_in_prog %d",
                                 l1_get_drx_interval(gas_id),L1_MAX_DRX_INTERVAL_VAL,l1_isr_data_ptr->l1_bplmn_suspend_in_progress);
             l1_set_drx_interval(L1_MAX_DRX_INTERVAL_VAL, gas_id);
           }

           l1_decrement_drx_interval(gas_id);
           if( l1_get_drx_interval(gas_id)== 0)
           {
                if( L1_DEACTIVATE_COMMAND == l1_tskisr_blk->main_command )
               {
                  l1_set_gsm_stop_req_in_queue(TRUE,gas_id);
                  MSG_GERAN_MED_1_G(" Enforce the stop_mode_req FN %u ", GSTMR_GET_FN_GERAN( gas_id ));
                }
             l1_tskisr_blk->main_command = L1_SUSPEND_COMMAND;
           }
         }
         l1_isr_data_ptr->leaving_bplmn = TRUE;
         break;
    }

    return (l1_isr_data_ptr->leaving_bplmn);
}
#endif

/*===========================================================================

FUNCTION l1_access_isr_set_mac_sb_failure

DESCRIPTION
  This function is called to tell the idle mode isr to send the sb failure
  message to MAC.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_access_isr_set_mac_sb_failure( gas_id_t gas_id )
{
  l1_isr_data_t *l1_isr_data_ptr = &l1_isr_data[gas_id];

  l1_isr_data_ptr->l1_access_isr_send_mac_sb_failure = TRUE;
}

/*===========================================================================

FUNCTION L1isr_get_jump_to_sdcch

DESCRIPTION
  This function returns the value of jump_to_sdcch

DEPENDENCIES
  none.

RETURN VALUE
  value of jump_to_sdcch

SIDE EFFECTS
  None
===========================================================================*/
uint8 L1isr_get_jump_to_sdcch( gas_id_t gas_id )
{
  return jump_to_sdcch;
}

/*===========================================================================

FUNCTION gl1_state_log

DESCRIPTION
  This function calls l1_state_log function

DEPENDENCIES
  none.

RETURN VALUE
  value of jump_to_sdcch

SIDE EFFECTS
  None
===========================================================================*/
void gl1_state_log( gas_id_t gas_id )
{
   l1_state_log(FALSE,gas_id);
}

/*===========================================================================

FUNCTION gl1_wait_for_dpc

DESCRIPTION
  This function will wait for the next isr->DPC to execute
  ensuring that any possible queued activity relying on this completing
  will run before any activity runs after this. Call drx wakeup to
  make sure it that GL1 is awake and executing the isr.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Will always wait for a frame ISR then DPC to execute
===========================================================================*/
void gl1_wait_for_dpc( gas_id_t gas_id )
{
#ifndef FEATURE_GL1_GPLT
  rex_tcb_type *active_task_tcb = l1_task_tcb_read( gas_id );

  if ( active_task_tcb )
  {
    /* Wait for real next frame tick to send pending TT update from DPC task */
    (void)rex_clr_sigs( active_task_tcb,
                        GL1_WAIT_FOR_DPC_TO_FINISH );

    /*
     * Make sure ISR is running - don't want to wait until next
     * paging block.
     */
    gl1_drx_require_next_tick( gas_id );

    /* Signal set once next GPS DPC task completes */
    (void)rex_wait( GL1_WAIT_FOR_DPC_TO_FINISH );

    /* Clear for next request */
    (void)rex_clr_sigs( active_task_tcb,
                        GL1_WAIT_FOR_DPC_TO_FINISH );
   }
#endif /* !FEATURE_GL1_GPLT */
}

/* END OF FILE */
