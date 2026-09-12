
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        INTERRAT (WCDMA) SURROUND CELL

GENERAL DESCRIPTION
   This module contains routines specific to performing LTE/TDS/WCDMA interRAT
   measurements while in GSM.

EXTERNALIZED FUNCTIONS
   
   l1_sc_wcdma_update_list
   l1_sc_ded_wcdma_update_list
   l1_sc_wcdma_get_best_six
   l1_sc_wcdma_set_paging_multiframes
   l1_sc_wcdma_ignore_neighbors
   update_stored_measurements
   update_stored_measurements_from_ded_reconf
   update_stored_measurements_for_top_8
   get_stored_meas_cell_entry
   remove_stored_meas_cell_entry
   update_stored_meas_cell_entry
   l1_sc_idle_tds_acq_results
   l1_sc_idle_tds_meas_results
   l1_sc_ded_tds_acq_results
   l1_sc_ded_tds_meas_results
   l1_set_serving_RLA_C
   l1_get_serving_RLA_C
   l1_sc_specific_irat_freq_update
   l1_sc_forced_meas_expiry
   l1_sc_forced_meas_event
   l1_sc_find_freq_index
   

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sc_irat_meas.c#3 $
$DateTime: 2022/09/08 22:34:43 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
09/09/22   sal       CR3013737 Ensure IRAT frequency fetched into int32 while storing.
17/09/19   sc        CR2439599 FR 56377 Elevator Sensor Ind for G2L
13/06/18   km        CR2252685 Forced interval changed from 3 to 2 for DRX cycle 4
06/04/18   akm       CR2216412 No need to wait for G2L search to finish
20/02/18   shm       Change Request 2192580 : KW P1 errors on TA.3.0 fix
01/02/18   km        CR2111389 Increase ID cycle and reconf cycle if total freq are more than TDS reconf cycle
20/09/17   ksb       CR2110372 Set abort for Fast search soon after the update list is received and before the freq list is updated.
05/10/16   ng        CR1093500 Irat Init/Deinit Code Change
29/11/16   cc        CR1091632 Remove F3 for 96 cells
22/07/16   ng        CR1045258 Add W and L freqs correctly to search list in dedicated mode
15/11/16   zf/ng     CR1081607: Stop forced measurement when update cell list
15/11/16   bg        CR1089997 Fix for KW errors
25/07/17   ng        CR2081244 Restart only ID search if reconfirm attempt fails for a forced search 
13/07/17   ng        CR2075022 Reset G2W ID search scheduling cycle only for a particular freq when reconf attempt on cell belonging to that freq fails
15/11/17   ng        CR1021532 Make G2L Null2X (and changes for all G2X as Null2X)
16/02/17   cc        CR1010614 G2L idle time transfer and MEAS only support
17/11/16   akm       CR1090724 JO.3.0 P1 compilation issues due to new KW version
22/04/16   smd       CR1005957 don't process the new WCDMA_LIST until fast search is done
09/03/16   sk        CR986058 Klockwork error removal
16/11/15   dg        CR936934 Wait for FN tick for timer calculation when received MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ for G2X forced search
17/11/15   hd        CR940246 Update the stored_meas structure through mutex lock.
09/09/15   dv        CR905027 Abandon scheduled IRAT search if update list occurs
16/04/15   xz        CR821958 don't re-do acq if we have synchronized when we do specific force measurement.
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
17/08/15   cja       CR890396 In G2T dedicated abort do cleanup before setting state to NULL
13/05/15   dg        CR836846 Print SCR instead of UARFCN on receiving IRAT neighbors from GRR
29/07/15   br        CR869828 Back out CR805033, to start g2l force measurements during last 5sec of tresel timer in PTM mode
18/05/15   dg        CR838096 Check no. of freqs while logging packet 0x525A and enable TD-SCDMA cell logging for the same
08/07/15   df        CR815683 Avoid error recovery when G2W scripts are built during wakeup
16/06/15   dg        CR854712 Ensure feature FEATURE_GSM_SEARCH_ALL_WCELL runs during every iteration of G2W ID search
01/07/15   cgc       CR853993 fix reprune totals error, reset num_detected_cells
30/06/15   zf        CR864108: Reset curr_id_in_tbf_freq_offset after updated frequency list
26/03/15   cgc       CR814333 Fix linker errors for non FEATURE_GSM_TO_LTE build
13/05/15   cgc       CR833607 update_stored_measurements() exclude legacy code, prevents LTE cell update from stored_meas.
03/03/15   zc        CR815093: Cancel reservation for W ncell activity if list is empty
21/04/15   cgc       CR826504 Reset ForcedMeas, move expiry code braces
04/16/15   zc        CR805033 Keep forced measurement complete flag set in transfer
03/02/15   cjl       CR790433 Create separate TRM activity for WCDMA reconfirm when needed
14/04/15   aga       CR799530 Reserve W/TD/L IRAT ID search with TRM in advance
14/04/15   cgc       CR794901 added L1_send_not_found_MPH_SURROUND_IRAT_MEAS_IND for common xfer/dedicated results
09/04/15  dp/cgc     CR819956 Thor 2.x RF API change, G2W use MSGR
30/03/15   cja       CR800650 Optimise irat state machine
11/03/15   cgc       CR798584 G2L dedicated, clear unused lte_cell parameters.
09/03/15   br        CR802926:While handling plmn search request, make sure clear all irat related fm activities
26/02/15   ak        CR800311 SS: RR cell update list causes crash during ongoing G2W measurement
30/01/15   cgc       CR794323 FR25799 support x8 LTE with fast idle search.
24/01/15   jj        CR 799441 Active irat should not reset until id state becomes NULL
27/02/15   br        CR800652 Clear fm activity and Wsrch_in_progress,if TRM denied  for G2T reconfirm search
17/11/14   dg        CR758006 IRAT improvements to ease debugging
12/02/15   br        CR793233 G2T:Don't request TRM for G2T reconfirm search continously, if other sub is in data call
13/02/15   br        CR795586 Reset wsrch_in_progress while aborting g2t reconfirm search in GTOW_SRCH_STATE_MULTI_SIM_FW_WAIT state
24/02/15   js        CR749012: GL1 changes for LTE removal
20/02/15   cgc       CR776811 re-enter idle from idle to check for id_state GTOW_SRCH_STATE_WAIT.
20/02/15   js        CR693771: W+T removal. Resolving compilation errors
18/02/15   dg        CR786825 On entering dedicated mode, configure G2W dedicated mode meas. only if no ded srch is in progress
18/02/15   dg        CR780330 Call require next tick while decrementing trm request countdown while waiting to start G2W ID search
18/02/15   cgc       CR774999 l1_sc_irat_check_active() to check for pending init_req before issuing l1_sc_g2l_shutdown().
13/02/15   pa        CR792105: Allow either GBTA or IRAT.
13/02/15   pa        CR756156:G2X RF device handling using TRM apis.
09/02/15   sjv       CR793538 On Panic Reset GL1 will wait for G2L to complete
21/01/15   xz        CR783911 Make sure we have chance to shutdown G2L/G2T when G2W is inactive
20/01/15   xz        CR783697 Re-work CR650965 for DSDS/DSDA design
11/02/15   xz        CR650965 Clear tds_update_list_irat_active in shutdown to guarantee stop IRAT really.
04/02/15   dp        CR791452 Move crit sect init function to a safe place (crashes in G+T)
27/01/15   cgc       CR782109 G2L add missing abort for leaving xfer mode
15/12/14   xz        CR770750 Make strict check valid RSRP when make LTE report to GRR
21/01/15   jj        CR 784462 remove FEATURE_THOR_G2X_MSGR_REWORK
12/01/15   sjv       CR756154 Enhance Band Registration for Activities 
09/01/15   xz        CR775244 Make sure the acq failure report to GRR timely in idle
10/12/14   dg        CR765859 Do not go to sleep if MCPM is not updated for G2X MEAS STOP activity
15/12/14   ws        CR771244 GL1: Compile warnings for LTE,1X,GERAN build flavour
08/01/15   sjw       CR758083 Deprecate use of geran_get_multi_mode_gas_id()
11/12/14   jj        CR 769775 release the  ISR_LOCK  hold by l1_sc_ded_tds_meas_results
17/12/14   dg        CR772242 While aborting G2X search in MULTI FW WAIT , use index based on search type
15/05/13   nn        CR487943 Enable FEATURE_GSM_SEARCH_ALL_WCELL
12/12/14   cgc       CR769137 l1_g2l_cm_abort() FEATURE_THOR_G2L_REWORK correction for CR763539.
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
11/12/14   jj        CR769775 add missing FEATURE_MUTEX_CHANGE for GL1_ISR_LOCK in l1_sc_ded_tds_meas_results 
11/12/14   jj        CR 769775 release the  ISR_LOCK  hold by l1_sc_ded_tds_meas_results 
03/12/14   npt       CR759467 Fix compiler warnings
03/12/14   cgc       CR715394 G2L code Idle/Dedicated rework FEATURE_THOR_G2L_REWORK
01/12/14   dg        CR763539 Abort G2L CM gap instead of clearing its values while leaving idle mode
26/11/14   cgc       CR762480 Ensure l1_sc_mode_cnf_pending() pending_flags are only set on gsm_only 0->1 transition
20/11/14   hd        CR760078 Add byte padding value to size parameter while calling log_alloc()
13/11/14   cgc       CR755634 use tds_update_list_irat_active when setting ID_cycle in gsm_l1_set_wcdma_search_cycles()
21/11/14   dp        CR700052 Move g2w drivers into W filetree
30/10/14   dg        CR746862  Break the code flow on the first index which meets G2W search criteria during G2W PTM ID search
18/11/14   dg        CR758569 Clear ID search related flags if GL1 receives new cell list when id search is either in WAIT or in FW_WAIT state
17/11/14   sjv       CR758331 for g2l, avoid doing band_release immediately after starting/scheduling it in CM
03/11/14   sjv       CR750056 IRAT activities need to be aborted/cleared when an update_list is received
16/10/14   xz        CR738656 if the frequency has been deleted when rsp is back from TDS, then will not send report to RR
09/10/14   cgc       CR734430 G2L pruning algorithm to consider detection only scenarios, +legacy code fixes for LTE
13/11/14   jj        CR754831 fix unpaired mutex unlock 
07/11/14   jj        CR752569 Fix thor bring up issues 
28/10/14   fm        CR750872 Let GL1 still call multi-SIM TRM manager if NCell activity is scheduled
21/10/14   cgc       CR740777 G2L race condition on back to back mode change requests set bALL_RATS_INACTIVE
31/10/14   xz        CR748816 Extend dual data space for building RF script variable 
28/10/14   xz        CR746778 Clear FM booking on G2X abort when in GTOW_SRCH_STATE_MULTI_SIM_FW_WAIT state for re-confirm
31/10/14   xz        CR748254 Check if is there some IRAT acitvity is pending before go to sleep
03/11/14   cgc       CR746617 Fix regression due to CR720774, bounds check bckup_cell[] array copy
16/07/14   xz        CR695052 Remove id_in_tbf statement when shutdown G2L/G2T
30/10/14   pa        CR748275 In DRDSDS (Transfer Mode): allow IRAT if band registration is successful.
17/10/14   jj        CR741205 moving  G2X CNF from l1 task to MSGR 
06/10/14   hd        CR733056 Race condition to handle TDSCDMA_IRAT_MEAS_RSP after the WCDMA list update
14/10/14   cgc       CR737141 GL1 prevent updating of forced measurement timing if cell update received
29/10/14   dv        CR731946 Ensure that G2X scheduling not delayed if in non-drx mode
28/10/14   dp        CR733893 Prevent W searches from starting when W-Segment not loaded
10/10/14   nm        CR735664 Reset start_wcdma_receive after TDS srch done
23/10/14   pa        CR744906: Allow one IRAT DRDSDS activity at a time.
26/09/14   hd        CR724976 Clear the FM activity when reconfirm state is MULTI_SIM_FW_WAIT in reconfirm_aborted()
08/08/14   nk        CR705945 Check for GERAN_ACCESS_STRATUM_ID_UNDEFINED to avoid memory read crash
07/08/14   dg        CR703831 Clear number of L frequencies inside function l1_sc_wcdma_init
21/10/14   pa        CR743016: Clear IRAT(W,T) band registration on aborting from scheduled state.
21/08/14   br        CR629582 GL1 not  shutdown the G2w driver while entering to Idle mode after TBF release
13/10/14   xz        CR732386 Don't update ded tick when update cell list  to avoid back to back G2X activity.
22/09/14   xz        CR727830 Corrent the supported maximum LTE EARFCNs number
14/10/14   cgc       CR738236 G2L idle measurement finishes early, add GTOW_SRCH_STATE_LTE_CLEANUP_WAIT to prevent sleep.
08/09/14   jj        CR720774 correct stored cell energy index before filter treatment
07/10/14   sjv       CR734254  G2L rf devices retrieval for primary and diversity
06/10/14   dg        CR730851 Reset IRAT related filters and stored meas. when UE mode changes from 0 to 1(gsm_only)
07/10/14   sjv       CR734254  G2L rf devices retrieval for primary and diversity
09/10/14   dv        CR735413 Ensure G2W search not kicked off with undefined RAT
07/10/14   sjv       CR734254 G2L rf devices retrieval for primary and diversity
03/10/14   smd       CR732883 Allow TDS IRAT abort when IRAT reconfirm search state is GTOW_SRCH_STATE_MULTI_SIM_FW_WAIT
03/10/14   aga       CR732455 Changes required to tune away framework for TH 1.0 single sim build
16/09/14   hd        CR724348 When re-enter from idle to idle and reconfirm search already started, don't clear states
30/09/14   br        CR721433 Skip G2W IRAT search, for PTM time slot reconfiguration
25/09/14   cgc       CR728533 cell list change causes G2L stuck in GTOW_SRCH_STATE_SCHEDULE_LTE
24/09/14   cah       CR729408: Updates to reduce number of TRM API calls in Gl1. 
16/09/14   cgc       CR723214 Avoid forced stale irat measurements when sub1 is blocked by sub2 MO/MT.
08/09/14   hd        CR714227 Don't set reconfirm state to ACTIVE for LTE during the start of the paging block
04/09/14   jj        CR 719553  GSM  IRAT  F3 Clean up
17/09/14   ws        CR 694807 Reduction in High occuring F3
15/09/14   ws        CR 724529 Resolve remaining P1 Klocwork issues
12/09/14   jj        CR 723592 G2T crash caused by regression of CR514967
11/09/14   am        CR677361 Addressing KW GL1 P1 errors
09/09/14   cgc       CR721718 FEATURE_GSM_TO_LTE changes to fix compile errors for non-lte build
04/09/14   cja       CR717966 G2L stuck in SCHEDULE LTE as update list had made index WCDMA
13/08/14   xz        CR708896 Remove unuseful aborting treatment for TDS
03/09/14   cja       CR717826 Prevent G2X and WTR hop at the same time
28/08/14   br        CR715552 During TDS abort don't decrement idle monitors in progress if srch state is GTOW_SRCH_STATE_WAIT
03/09/14   cws       CR714049 Schedule G2T irat measuement in DTM
02/09/14   dg        CR717969 Clear flag gl1_set_tx_band_cmd_iss_to_gfw before enabling RF for TX
28/08/14   cgc       CR717278 G2L dedicated scheduling changes,refinement of CR700929
31/07/14   xz        CR697883 Not allow GBTA if current idle frame is doing TDS IRAT in xfer mode
26/08/14   pa        CR715657 Band deregistration on finishing IRAT activity.
20/06/14   jj        CR656872 fix critical gl1 kw error
29/08/14   cah       CR716092: Handling of TRM_AYSNC_DR_INFO_REQ and removal of DR check before badn release
23/08/14   sv        CR714251 regression fix for CR706923
22/08/14   cgc       CR713634 obsoleted G2L code and featurisations FEATURE_GSM_G2L_TIMED_SRCH and FEATURE_CM_G2L removed
12/08/14   dg        CR708414 Limit the no. of for loop index search to 6 while clearing rr_report for those TD UARFCN's
                              whose no. of stored cells are greater than 6
20/08/14   mko       CR702442 use correct freq_offset for reconfirm (in l1_sci_tick_sched_reconfim)
18/08/14   cgc       CR706923 regression fix for CR631007, l1_sc_wcdma_ded_ready()
13/08/14   pa        CR709108 Clear FM activity after G2T Reconfirmation abort.
15/08/14   pa        CR709772 Clear receive_in_progress flag after G2T Abort.
13/08/14   pa        CR709108 Clear FM activity after G2T Reconfirmation abort.
04/08/14   xz        CR703797 make freq_idx can be updated and set correctly in l1_sc_wcdma_receive
09/08/14   dv        CR707309 Ensure if in Schedule_wait state that in abort FM is cleared down
11/08/14   cgc       CR707165 Regression fix for CR631007, remove "else" clause in l1_sc_wcdma_ded_ready
08/08/14   cja       CR706878 Do not set G2L CM inactive if exit transfer at search state not NULL
29/05/14   xz        CR670706 Make sure we sent out stop cmd to TL1
08/08/14   cgc       CR706185 fix KW errors as part of this CR
08/08/14   cgc       CR682787 Hopping conflict fix for G2X on SDCCH propagation from DI2.3
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
05/08/14   cgc       CR704038 Missing var in merge causing G2L/ncell clash
04/08/14   cja       CR703742 Check IRAT state machine is NULL before isr inact is complete
10/07/14   cgc       CR631007 update fix KW error in l1_sc_specific_irat_freq_update()
04/08/14   nm        CR670867 Clear frames from FM for WCDMA ID srch in wcdma enter idle if prev state was fw load wait
01/08/14   pa        CR697586: Fixed Idle mon abort. Increment idle_monitor_in_progress before scheduling W reconf.
31/07/14   cja       CR693851 Set CM inactive when exit dedicated or transfer
28/07/14   am        CR700644 Check if sce rx is already in progress before scheduling iRAT in packet transfer state
25/07/14   am        CR698669 Transfer mode SCE should not be triggered until all g2l_timed_searchs are completed
28/07/14   zc        CR696897 Correctly clean up G2L idle state machine
25/07/14   dp        CR700060 Device_2 is OK in DR-DSDS scenarios
10/07/14   cgc       CR631007 FR21768 enhancement force G2X measurement MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ
16/06/14   br        CR678656 GL1 should check for RF device  compatibility before scheduling reconf search on IRAT
04/07/14   dg        CR687873 Re-work on CR684528 fix to clear wcdma srch in progress flag when trm is denied for reconfirm search
24/06/14   dv/cah    CR697365 ensure wcdma_srch_in_progress flag is reset
08/07/14   dg        CR690007 WHile moving from IDLE to CM mode, don't skip the current freq. offset
15/07/14   dg        CR694542 GL1 to send INIT_REQ to LTE only when it has received LIST REQ from RR with valid LTE neighbors
09/07/14   dg        CR682537 Send INIT_REQ to ML1 only when MCPM is configured for G2L Meas
18/06/14   br        CR671074 Regression on CR665231, handle reconfirm and idle search speraterely during leave idle
24/05/14   br        CR670494 Reset the Wcdma search variables.
19/05/14   br        CR665231 Race condition between handling of RACH in task and is state moving to GTOW_SRCH_STATE_WAIT state
22/07/14   cah       CR697586  trm_client.cpp:4539   Assertion  search_activity_q.cnt == 0 failed
17/12/13   jj        CR 590806 correcting pointer overflows
24/04/13   jj        CR 477224  NV control the enabling of feature FEATURE_GSM_SEARCH_ALL_WCELL
17/05/14   xz        CR661995  G2L 3-tap filter side effect
25/02/14   xz        CR611918  G2W/G2L 3-tap filter implementation
15/07/14   dp        CR692373 monitors++ only when G2W reconf is actually scheduled
14/07/14   zc        CR692630: Reset G2L State variables when leaving idle mode
11/07/14   dp        CR674070 - Use new G2W drivers interface (phase 1)
10/07/14   ka        CR628921 Use LTE earfcn type in gl1 to allow move to 32bit earfcn's
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
07/07/14   cgc       CR689692 l1_sc_wcdma_enter_xfer() do not reset freq counts.
02/07/14   aga       CR675214 G2T TA feature and GTA/G2W TA re-worked optimized solution
30/05/14   ws        CR657674 Data + MMS Phase 1
05/06/14   cgc       CR674175 Ensure gsm_only mode change clears irat num_freq list count.
20/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
06/06/14   cja       CR676197 Prevent timed G2L getting stuck in GTOW_SRCH_STATE_LTE_CLEANUP
29/05/14   cgc       CR669540 Add l1_sc_start_G2L_timed_search() to GTOW_SRCH_STATE_MULTI_SIM_FW_WAIT
22/05/14   hd        CR668193 Call l1_sc_g2x_pending_shutdown when TDS search is aborted only when 
                     the id and reconfirm states become NULL
14/02/14   cgc       CR639886 Changes for FR19574 add priority search for dedicated FEATURE_SGLTE_G2X_CONN_MODE_RESEL.
12/05/14   br        CR663243 Decrement idle_monitor_in_progress while restting the reconfirm state to NULL.
12/05/14   cgc       CR662650 fix update for CR659270
08/05/14   cgc       CR660909 ID search not started, add l1_sc_check_priority_threshold() in l1_sc_wcdma_receive_id_in_tbf()
01/05/14   cgc       CR646598 Remove frequent resetting of ded_tick, move to l1_sc_wcdma_init() and xx_wcdma_update_list
06/05/14   xz        CR659107 Make G2T info pring more correctly when id state is GTOW_SRCH_STATE_ACTIVE
09/05/14   dv        CR655393 enable G2W TA for G2W ID search on MM sub when Gsub in xfer
28/04/14   og        CR646838. Mutex protect the wcdma sample ram parameters.
17/04/14   gk        CR650694 for GP041540 scenario add check qSearch_C=NEVER
28/04/14   dv        CR655739 ensure GFW does not sleep before receiving STOP CNF from G2X RATS
17/04/14   cgc       CR650694 for GP041540 scenario add check qSearch_C=NEVER
16/04/14   cgc       CR641709 propagation Check TDSCDMA_IRAT_INIT_CNF response for failure and reset TDS irat state
15/04/14   cgc       CR639867 G2L mode transition gets aborted before LTE_GAP_DEINIT_CNF recieved
14/04/14   cgc       CR647270 Ensure lte_cm_irat_active is resetfor g2l_scheduled_state_cleanup()
07/04/14   cja       CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
02/04/14   cgc       CR642257 fix regression on CR635441, add TDS reconfirm_state == GTOW_SRCH_STATE_WAIT check
13/01/13   xz        CR596858 Guarantee to do G2T activity after init cnf is back
27/03/14   cja       CR638302 For G2L timed search in idle ensure state is NULL after search.
27/03/14   cja       CR624068 Add extra frame on TDS reconfirm before NULL state to prevent sleep too early
26/03/14   cgc       CR635441 clear FM booking on G2X abort when in GTOW_SRCH_STATE_MULTI_SIM_FW_WAIT state
24/03/14   sd        CR630989 Pass ML1 RF  Device ID for G2L
17/01/14   xz        CR617692 when do not schdule IRAT scenario, don't clear stored num of cells.
06/11/13   xz        CR569088 make optimization for TDS rscp 3-tap fitler.
19/03/14   ws        CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
25/02/14   dg        CR614999 GL1 to set active_irat to WCDMA on initiating G2W IRAT search in packet transfer mode
19/03/14   ws        CR635101 Resolve P1 Critical Klocwork errors in GL1 code space
04/03/14   cgc       CR626771 G2L mode change enter idle race condition waiting for deinit_cnf
04/03/14   ap        CR626016 UE is not doing GERAN to LTE dedicated mode measurement in during Voice call
04/03/13   xz        CR590653 Abort current IRAT Activity when the state is also GTOW_SRCH_STATE_MULTI_SIM_FW_WAIT
03/03/14   cja       CR624650 Report idle G2W on_pos as cx1 (not cx8)
28/02/14   br        CR624189 Call function l1_sc_wcdma_ded_ready during packet transfer mode when UE is in DSDA
28/02/14   cgc       CR624632 Rework CR621594  G2L abort, add  g2l_scheduled_state_cleanup()
25/02/14   dg        CR614999 GL1 to set active_irat to WCDMA on initiating G2W IRAT search in packet transfer mode
24/02/14   og        CR606561. Ignore the G2T ACQ and MEAS results when desense occurs.
27/02/14   cgc       CR623840 wait for TDS abort cnf before state change allowed
26/02/14   xz        CR622667 when start to schdule G2T activity, set wcdma_id_srch_in_progress for MULTI SIME mode
24/02/14   ss        CR621594 Set id_state to GTOW_SRCH_STATE_LTE_CLEANUP when aborting G2L in GTOW_SRCH_STATE_SCHEDULE_LTE state
24/01/14   dg        CR605600 Call function l1_sc_wcdma_ded_ready during packet transfer mode when all other subs are in inactive mode
17/02/14   cja       CR605188 Add PN_offset to WCDMA best six structure
10/02/14   cja       CR612218 For G2L add meas enter and meas exit for LTE RF
28/01/13   cja       CR606841 When enter xfer if W freq only list then set PN_pos as cx1.
02/12/13   xz        CR574811 Change the wrong index to correct index to get TDS report
09/12/13   xz        CR586053 Did not schdule measurement once jds cell is zero when it is TDS Cell.
26/09/13   xz        CR549601 Keep the stop acq list info from ded/xfer to idle
05/02/14   br        CR576666 Prevent the denit if we have already sent the G2L INIT
07/01/13   br        CR594569 The crash during G2L IRAT measurement, GL1 sending G2L_STARTUP_CMD with earfcn 0.
04/02/14   cgc       CR611004 l1_sc_stop_cm_g2l() for cell update change between G2L gap searches
31/01/14   cgc       CR609820 Prevent sleep on MEAS_G2L_DEINIT_REQ in l1_sc_g2l_shutdown()
28/01/14   cja       CR606841 When enter xfer if W freq only list then set PN_pos as cx1.
06/01/14   dg        CR595677 GL1 to continue with TDS re-confirm search based on Active RAT Info
28/01/14   cgc       CR608402 Add l1_fm_clear_required_frame id_report_cb()
27/01/14   dv        CR607497 Crash During G2L PTM with Sub2 in idle camped
24/01/14   PS        CR607076 In dedicated mode IRAT measurements supported on RF device ID 0
23/01/14   cgc       CR606502 rework CR589523 to include TS reconfigure due to MAC_RELEASE
17/01/14   cgc       CR600002 propagate missing code from CR504088, support for SGLTE
20/01/13   ap        CR599098: check for compatable RF device for IRAT
15/01/14   cja       CR599114 For G2T abort in dedicated mode do not set tds cleanup if startup not yet sent.
14/01/14   cgc       CR598082 add wcdma_id_srch_in_progress to g2l_scheduled_state_cleanup()
20/12/13   js        CR592583 : Removing compilation errors : MPSS.2.0.1 CRM for T+G (W removed)
09/12/13   sk        CR586591 Ensure GBTA status is not reset when GBTA is active in TSTS mode
16/12/13   dv        CR569122 High missed page rate due to W ID search
16/12/13   dv        CR580986 Re-introduce CR557546 plus regression fixes
09/01/14   cgc       CR565747 gsm_l1_set_wcdma_search_cycles() interval based solely on number of W freqs.
12/12/13   ssh       CR588188: ISR overrun recovery mechanism
07/01/14   cgc       CR516981 propagation from 2.x, add l1_sc_wcdma_set_mode_from_init()
24/12/13   ap        CR592015: Check WTR IRAT capability before allowing IRAT measurements
02/12/13   dg        CR583241 Mutex lock added in function l1_sc_wcdma_abort_ded_search while aborting dedicated search
13/12/13   cja       CR587963 Allow G2T pending shutdown in packet transfer
13/12/13   ka        CR584923 EV_RACH_FOR_PS_FAILURE no response to rach
10/12/13   ws        CR 587586 - Remove DUAL_SIM featurisation in API
09/12/13   cgc       CR587019 regression on CR562746, re-instate g2l_configure_for_cm()
03/12/13   cgc       CR580986 backout CR557546 G2W broken.
02/12/13   jk        CR481418: Do not send abort command to WFW when in G2W_ABORT_STATE
08/05/13   dg        CR479252: Call wcdma_ded_srch_abort_cb in function l1_sc_wcdma_abort_ded_search
25/11/13   pa        CR58117:Bolt: Support for GPLT G2W Dedicated Mode Measurement
22/11/13   cja       CR455527 Remove featurisation from files in API directory
22/11/13   cja       CR579119 Only clear g2x cleanup pending at boot up, not sc init
21/11/13   cgc       CR579032 Add gas_id check to L1_G2X_terminate()
11/11/13   pa        CR573743:Bolt G2W: Report call back registration and clear FM activity after receiving the report.
11/20/13   xz        CR557546 optimize stop list management for idle/ded mode
11/18/13   nk        CR570193 Hold off sleep for one more tick when loading FW for WCDMA search
18/10/13   nn        CR563624 Add the correct LNA State to G2T IRAT Acquisition Response/G2T IRAT Measurement Response Log Packets
18/10/13   cgc       CR563485 KW fixes
18/10/13   cgc       CR562746 ensure num_freqs sub totals are cleared
15/10/13   dv        CR560959 G2X Idle crash due to cleanup being issued too soon.
15/10/13   cgc       CR560979 correction to numerical errors in l1_sc_ded_wcdma_update_list
11/11/13   ka        CR572452 Verify that GFW is loaded before commencing a G2L search
10/10/13   cgc       CR556101 prevent G2L init_req transition when LTE shutdown is pending
07/10/13   nn        CR552215 After CSFB call end, UE didn't pick up the first IRAT priority for IRAT reselection.
02/10/13   cja       CR552127 When G2L connected mode aborts call l1_sc_g2x_pending_shutdown
26/09/13   xz        CR550600 - reduce time between G2X ACQ searchs to ~20s on top of CR498686
23/09/13   cgc       CR550626 L+W limit exceeded when RR total >9 and is odd.
10/12/12   jj        CR430392 G2W driver not configured of dedicated mode when  re-enter transfer mode
30/08/13   xz        CR534056 init RSCPLAST And RSCPLAST2 as MIN_TDD_CELL_RSCP_VALUE.
24/07/13   ss        CR515896  Set wcdma_list_update_inprogress to TRUE in msg_handler routine
02/07/13   sai       CR505290: Complete pending G2W reconfirm and id srch state cleanup at G stack inact.
17/06/13   ap        CR500653 no W measurement started, UE was not able to do G2W Ho and G2W reselection
01/07/13   dv        CR506299 Regression of CR492158 where false G2T reselections are made
27/05/13   dv        CR492158: GL1 should update rr report everytime it receives Acq data from TD
05/09/13   sp        CR538279 -Add F3s to debug rcv_in_progress flag.
25/09/13   nm        CR539361 Fix for issue where abort WCDMA srch done when already in clean up state
24/09/13   gk        CR549082:Sync from 1.1 and 1.0 to 2.0
12/06/13   ap        CR498947 Check ID Search State before clearing in Reconf Mode
19/09/13   dv        CR545770 MS FM not run in Idle when IRAT ncells present
13/09/13   dv        CR533758 G2T log packet crash and num_jds_cells>0 crash
18/07/13   gk        CR513938:Mon counter for WCDMA reconf should not be incremented, if a WCDMA reconf is already in progress
14/08/13   xz        CR525010 Issue Cleanup command to GFW when abort conf back in dedication mode.
15/08/13   ss        CR530036: Do not clear wcdma_srch.state after every G2T gap in dedicated mode
21/08/13   cja       CR531211 Prevent mutex deadlock between GL1 ISR Lock and g2w srch drv lock
23/09/13   cgc       CR548006 Ensure G2L CM starts when in DTM mode.
16/09/13   cgc       CR547939 reduce gap by 1 frame for G2L measurements in SDCCH
16/09/13   cgc       CR545752 Prevent back to back G2L measurements in SDCCH
12/09/13   cgc       CR531210 remove W legacy wcdma_start_of_sacch_indication in dedicated
11/09/13   cgc       CR540158 Synchronise G2L CM gaps to move next freq status from ML1
06/09/13   cgc       CR537714 G2L connected mode trigger mechanism was missing for DTM mode
16/08/13   cgc       CR530370 add SDCCH support for G2L measurements in dedicated mode.
26/09/13   pg        CR550447 update to L2G gap timing API
21/08/13   npt       CR531633 - Remove FEATURE_DIME_MODEM dependencies
14/08/13   cgc       CR528421 fix regression due to CR520526, add G2L_RETURN_TO_IDLE
09/08/13   cgc       CR509276 add id_cycle_lte, make G2L idle search interval dynamic based on pg_mfrms
07/08/13   cgc       CR523423 add g2l_scheduled_state_cleanup() if cell update changes the scheduled index
04/08/13   cgc       CR520526 add g2l_set_cm_inactive() to ensure CM mode is cleared down.
02/08/13   cgc       CR520611 prevent invalid abort state transition for G2L CM
29/07/13   cgc       CR516981 fix CR504088 regression, prevent sending unsolicited mode cnf from start_req init()
12/06/13   dv        CR498686 - reduce time between G2X ACQ searchs to ~30s
14/08/13   dv        CR529270 Regression of CR514967
15/08/13   sk        CR524039 GBTA bringup changes
08/13/13   sbr       CR527070 fix TDS related compile errors
08/08/13   dv        CR525765 Changes for DSDS T/G IRAT bringup
26/07/13   cja       CR518025 If G2L aborted in G2L schedule state do not cleanup until statrup/cleanup scripts built.
08/05/13   cgc       CR453758 add FEATURE_CM_G2L search functionality FR-3009
08/05/13   cgc       CR453758 remove all obsolete #if 0's prior to adding G2L functionality for FR-3009
24/07/13   dv        CR514967 Ensure build scripts for G2T sent if changing modes xfer->Idle
25/06/13   sai       CR504290: Workaround G2W abort overrunning G frame tick at G stack inact.
20/06/13   sai       CR500680: Ensure G2W driver shutdown at abort immediately followed by
                               GL1 task deact at G next frame tick.
12/07/13   cja       CR501032 Mutex for G2W shutdown
10/07/13   sai/ws    CR502959: Ensure G2W search state machine not executed for non-MM sub
26/05/13   cs        Major Triton TSTS Syncup
05/06/13   dv        CR496269 Ensure no G2X active in Idle before running l1_dual_sim_trm_manager
05/06/13   cja       CR496237 Use wcdma_update_list_irat_active to turn of G2W
31/05/13   zf        CR494151: Check dual_sim mode for firmware load.
28/05/13   cja       CR492493 Only initialise G2W driver when W neighbour in list
21/05/13   xz        CR483951  Copy the cell info when GRR update cell list if current uarfcn has been acq.
29/01/13   xz        CR441401 GL1 implement the new 3-tap RSCP Smooth algorithms.
04/03/13   og        CR443397. Limit the G2T search period in idle mode.
28/05/13   ss        CR487097 Only log valid freq during L1_SC_DED_WCDMA_UPDATE_LIST
29/05/13   dv        CR492212 Introduce SM to handle G2W abort
13/03/13   xz        CR461618 For G2T, clear the ded_state when received acqusition result in corner case
07/03/13   xz        CR460045 FM Abort callback function to handle abort state branch.
14/03/13   dg        CR460161 Since the flag wcdma_srch_in_progress remains TRUE, L1 remains stuck in Idle mode
                     and is not able to send any reply to RR
06/03/13   xz        CR459659 For G2T, just report jds cell to RR.
09/01/13   dv        CR438637 G2X cleanup issued 1 frame too late
23/05/13   ap        CR491318: Reverted GFW state transitions regression 485912
16/05/13   dv        CR488755 G2T crash when leaving Idle for xfer and SYNCH_RX before cleanup
21/05/13   dv        CR489950 gas_id is not passed in l1_sc_get_tch_idle_frame
20/05/13   npt       CR417466 - Ensure that opt3 is not kicking in
                     if change to GSM only mode is being processed
20/05/13   ap        CR459492 DSDS two SYNC_RX commands whilst doing GTA
10/05/13   cja       CR486815 Mutex lock for W neighbour inactive
06/02/13   ss        CR448945: Clear required frames from the frame manager in G2W abort
27/03/13   br        CR443827 Abort the g2w search if WFW is active bfeore calling l1_sc_g2x_pending_shutdown
16/05/13   og        CR453577.
02/02/13   ap        DSDS CR448118 W/G + G IDLE : FM / TRM is getting stuck in
                      possession of SUB2 after an ungraceful WCDMA ID search
08/05/13   cja       CR481603 Do not shutdown G2L on DEINIT conf if INIT conf already sent.
06/05/13   dv        CR482086 Ensure when leaving Idle that wcdma_id_handler runs to completion
03/05/13   sjw       CR380127 Dedicated abort should return indication of failure
26/04/13   zf        CR477992: Fix G2W IRAT issue
28/04/13   ss        CR480389 memset sc wcdma parameters to 0 when num of cell = 0 (dedicated mode)
20/04/13   cja       CR478053 For G2T reconfirm abort just issue cleanup and set to cleanup state
28/03/13   dv        CR469297 Don't reset stop_cell_id list when entering transfer
11/04/13   cja       CR471731 Do not clear srch_in_progress flag if idle reconfirm srch active
04/04/13   ss        CR471326 Pass gas_id to the function l1_sc_start_wcdma_receive
04/04/13   sv        Do not schedule GTW search when GTA ongoing.
26/03/13   ws        CR466973 - DSDS BU - Added missing gas_id to a number of function calls
28/02/13   og        CR432171. Ensure the wcdma_srch_in_progress is reset when aborting
                     an inactive reconfirm search.
26/02/13   cja       CR455032 Changes for build without TD feature
25/02/13   jj        CR424195 GL1 is not sending abort if W dedicated search timed out
23/01/13   dv        CR444151 Regression from CR438637
22/01/13   xz        CR443458 If G2T ncell list updated between decision to schedule and action, abort.
31/01/13   xz        CR447437 Regression from CR433484
04/01/13   og        CR427574. Make sure the PDTCH handler is active before scheduling
                     G2T searches.
12/01/12   xz        CR422758 For PFR, Report ACQ Result even if it failed or did not measure.
04/01/13   dv        CR433484 Wait for G2T abort to complete before exit idle
01/02/12   cgc       CR432045 Move CASE GTOW_SRCH_STATE_LTE_CLEANUP fall thru to avoid errors
12/12/12   cgc       CR431006 add mutex and l1_sc_frames_available(), prevent concurrent task update
04/01/13   xz        CR434376  Clear History rscp value when redo acq
30/11/12   xz        CR423287 For G2T, Wait one more frame for stop cnf.
27/11/12   dv        CR423866 For G2T, if abort occurs make sure states are reset
09/01/13   dv        CR438637 G2X cleanup issued 1 frame too late
29/11/12   jj        CR425555 GL1 is stuck in Idle mode due to incomplete
                     abort of WCDMA reconfirmation.
12/11/12   cgc       CR415620 Inhibit LTE initialisation in xfer mode in l1_sc_irat_check_active() TRUE
17/12/12   og        CR400355. Slow G2W searches during fast transitions in and out of
                     idle mode.
06/12/12   cgc       CR429175  TDS Klockwork fixes
26/11/12   ss        CR 405129 : Add version in G2T Log Packets  and Change Log Packet Id for CR363991
22/11/12   cja       CR414098 For Dime only need W clks if W neighbours
06/11/12   cja       CR417929 Force abort in W schedule state.
21/10/12   xz        CR413865: numjdscells =0 crash fix
12/10/12   dv        CR408573: G2T fix to copy TD data into new cell list with same UARFCN
21/10/12   dv        CR397922: Fix for 6.2.2.3a G2T TC
16/10/12   cja       CR406119 Allow W clocks off in TCH if no W neighbours
15/10/12   cja       CR406110 Prevent ISR when clearing g2x shutdown pending.
10/10/12   ws        CR 401931 - Further LTE featured off compile errors
10/02/12   ss        CR 405129 : Add version in G2T Log Packets  and Change Log Packet Id for CR363991
02/10/12   ky        Fixed compilation issue
21/09/12   cja       CR401869 Make G2T cleanup margin bigger as FW RX Beta has increased
21/09/12   ws        CR 400052 fixed bug where strong W PSC is not added to stored_meas
                     structure even when FEATURE_GSM_SEARCH_ALL_WCELL is enabled
19/09/12   rc       CR401496 - Back out fix for CR382976,398725 & 400225
18/09/12  cja/og     CR400225 Avoid clash GFW task Q programming between T gap cleanup and Rx
14/09/12   cja       CR398725 Ensure minimum T gap is 6754qs
13/09/12   ss        CR363991 : Enable FEATURE_G2T_SRCH_LOGGING for G2T meas and acq response
12/09/12   cja       CR395615 Do not send WFW to sleep until abort is complete
10/09/12   cja       CR382976 Correct TD timing for G2T dedicated gap
03/09/12   cgc       CR369309 W dedicated search fix in l1_sc_wcdma_ded_pending
30/08/12   cgc       CR357522 G2X measuresments based on priority + tdd + fixes.
15/08/12   pa        CR386883: Don't schedule G2T dedicated until receive prev RSP from TL1.
03/08/12   og        CR383632. Don't request a tuneback to GSM when the G2L startup request
                     is not sent to LTE L1
20/09/12   ws        CR 401931 - removed references to assign_search_params()
13/09/12   ws        CR 399455 Fix compile errors when FEATURE_LTE_TO_GSM is disabled
02/08/12   pa        CR384676: Consider only JDS cells in G2T measurement.
30/07/12   pa        CR383547: Don't schedule G2T if TBF starts during BLk21-24
26/07/12   og        CR379017. G2W reporting problems in idle mode.
26/07/12   pa        CR382647: Don't consider Femto cells while scheduling G2T.
20/07/12   pa        CR380400: CMCC Req. GL1 shall measure upto 9 T freqs.
17/07/12   cja       CR375594 Ensure dedicated W search is aborted
20/09/12   ws        CR 401931 - removed references to assign_search_params()
13/09/12   ws        CR 399455 Fix compile errors when FEATURE_LTE_TO_GSM is disabled
08/06/12   pa        GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
02/07/12   pa        CR374196: Fix compilation error.
27/06/12   pa        CR374916: G2T: Make wcdma_srch_in_progress TRUE in SCHEDULE_TDS state and
                     Send Abort only in the gap.
27/06/12   jj        CR369758 if wfw active ,delay the shut down till wfw inactive
10/07/12   pa        CR377523: G2T Abort: Avoid BCCH reading during MEAS
09/07/12   pa        CR376965: G2T: Align FM G2T activity with GAP position.
06/07/12   pa        CR376751: G2T GL1 stuck in Idle abort
03/07/12   pa        CR371273: Don't schedule G2T if TS reconfigure before idle frame.
08/06/12   pa        GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
02/07/12   pa        CR374196: Fix compilation error.
27/06/12   pa        CR374916: G2T: Make wcdma_srch_in_progress TRUE in SCHEDULE_TDS state and
                     Send Abort only in the gap.
27/06/12   jj        CR369758 if wfw active ,delay the shut down till wfw inactive
25/06/12   pa        CR371580: Ensure GL1 sends TDSCDMA_IRAT_STOP after 0 TDD cells.
25/06/12   pa        CR373257: G2T Idle: : Clean up issued one frame after the GAP
22/06/12   pa        CR371580: Delay TBF release during G2T activity.
22/06/12   pa        CR372464: Disable tds_irat_active after stopping IRAT. Handling of Abort
                     on receiving modified WCDMA_UPDATE_LIST.
18/06/12   pa        CR371123: G2T SDCCH: Handling of SACCH tx in gap calc.
18/06/12   pa        CR371123: In G2T sdcch, allocate max 15G frames and correct gap calc.
18/06/12   dv        CR371317: Avoid double issuance of cleanup command in Idle when aborting
14/06/12   cja       CR359280 Turn off W/L/T clocks after WFW/LFW/TDFW disable
12/06/12   pa        CR369174: Ensure GL1 sends TDSCDMA_IRAT_STOP after reselection.
08/06/12   pa        CR367120: Send MPH_READ_SYS_INFO_FAILURE_IND to RR during G2T idle meas.
31/05/12   cja       CR366518 Correct g2t startup/cleanup time decrement
29/05/12   pa        CR362054: Execute already scheduled G2T cleanup if FM calls abort.
25/05/12   cgc       CR340815 limit G2X searches to 3W+3L frequencies
28/05/12   pa        CR360742: Call TDS_IRAT_BUILD_RF_SCRIPT immediately if TBF is suspended
11/05/12   jj        CR354241 Send G2L clean up in case of invalid lte search result
23/05/12   dv        CR364279 G2T: Pick up frequency only list for Pseudo Fast return
18/05/12   pa        CR361192,CR362129,CR362562: Ensure UE doesn't sleep during G2T MEAS
17/05/12   cja       CR Clear srch_in_progress if abort in reconfirm wait state
16/05/12   dv        CR357448 G2T Abort mechanism
16/05/12   pa        CR361944: UE uses WCDMA Q_searchI value instead of TDD specific, leading to cell reselection failure.
16/05/12   pa        CR361192: Ensure UE doesn't go into sleep mode during TDS MEAS. Checks to prevent
                     both id_state and reconfirm_state running at same time.
15/05/12   pa        CR359646 - Assertion (numJdsCells > 0) failed
10/05/12   dv        CR360134 Issue G2T Clean up immediately from mode change
10/05/12   cja       CR356836 If abort search in tbf, clear wcdma_id_srch_in_progress
07/05/12   dv        CR358920 Additional fix for CR357454
01/05/12   cja       CR357454 Fix stuck in idle abort due G2T measurements active
30/04/12   dv        CR348530: RR-L1 interface change to support PFR
25/04/12   pa        CR355881: G2T Dedicated TCH: TL1 crashed in RF API. Added support of RF_SCRIPTS_CMD.
23/04/12   pa        CR355008: G2T Dedicated Mode: Incorrect gap value passed to TL1
23/04/12   ws        CR354304 G2W Dedicated mode measurements are not being scheduled.
20/04/12   pa        CR353888:G2T dedicated: Reinitialise dedicated search id when RSCP fluctuation is high
19/04/12   pa        CR349239: G2T handling of idle mode IRAT measurement using FM
19/04/12   cja       CR351224 If abort but not id_srch_in_progress set GTOW state to NULL
17/04/12   pa        CR352537:G2T Passing incorrect number of cells in measurement report to RR
14/04/12   og        CR352194. Resolve G2T GSTMR ISR delay.
12/04/12   pa        CR351579: G2T: Fix for workaround: Stop scheduling IRAT when BCCH block to read
10/04/12   pa        CR350696: G2T Incorrect number of TDD cells passed to TL1 in MEAS_REQ
05/04/12   ws        CR349582 Ensure correct number of WCDMA cells is passed to l1_sc_irat_check_active()
04/04/12   pa        CR349266: G2T: Workaround for not to do IRAT when there is a BCCH block to read
02/04/12   jj        CR314295 UE should search on all possible PSCs if no suitable cells are
                     found in the 3G neighbour cell list in system information.
28/03/12   pa        CR347851 Correct check on RSCP value and ignore junk rscp values in ACQ-RSP
19/12/11   og        Ensure the WCDMA ID search periodicity does not exceed 30*3 seconds
                     when more than 3 W frequencies are provided by the NW. CR322915.
05/01/11   og        CR326225. Double cleanup request sent to LTE L1.
28/03/12   pa        CR346648 : G2T JDS implementation.
28/03/12   pa        CR 346062 Workaround for G2T Abort.
28/03/12   pa        CR347573 Correct start gap pos in G2T Pkt Txfr Measurement.
27/03/12   pa        CR347127 Generic protoype of update_stored_meas_cell_entry.
26/03/12   pa        CR347049 Added support of dropping TX in GAP. Calculate gap length based on RX and TX slot.
24/03/12   ws        CR329564 - Fix compile issues with l1_sc_global_ptr
23/03/12   ws        CR342566 - Fixed featurisation bug in LTE measurement code
24/02/12   og        CR339002 Handle failure to abort W ID search
10/01/12   og        CR329564. Ensure the WCDMA srch flag is reset correctly when no
                     search attempt occurs.
22/03/12   ws        CR345988 - Featurization FEATURE_GSM_TDS for further compilation errors
21/03/12   pm        CR345586. Fix for compilation error on Non-TDS builds. Removed FEATURE_GSM_TDS
24/02/12   og        CR334293. Fix the copying of LTE and WCDMA frequencies from the
                     wcdma_update_list message.
13/03/12   cja       CR341564 Clear wcdma_id_srch_in_progress and recieve_in_progress when id abort
09/03/12   pa        CR 342232 - Fixes for G2T measurement in Packet Transfer.
09/03/12   pa        CR 342226 - Fixes for G2T measurement in SDCCH
08/03/12   pa        CR 342196 - Abort G2T Idle measurement when GL1 is not actually measuring T frequency
02/03/12   pa        CR 340670 G2T: Incorrect value of ts0_position stored/passed by GL1
02/03/12   pa        CR 340669 G2T Dedicated: Scheduling of TDD cell breaks L and W scheduling in multi-RAT(LWT).
01/03/12   cgc       CR318816  Fix LTE state transition error l1_sci_wcdma_aborted
01/03/12   pa        CR 340471 Ignore junk values of RSCP. Report to RR when RSCP is valid.
                     Configure wcdma_srch state machine after issuing request to TL1.
27/01/12   pa        Added handling of invalid rscp value retured by TL1. In G2T dedicated measurement, disable toggling
                     functionality to do measurement. Fixed stop_cell_id_srch in the dedicated mode.
25/01/12   pa        Fixed gap number values for G2T dedicated measurement. Added debug info.
                     Fixed problem with stop_id_srch in G2T. Fixed problem with reporting to RR (not to be sent)
                     in case of ACQ failure.
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
19/01/12   cja       CR331892 Ensure do not turn off WSTMR if W becoming active
22/12/11   cja       CR316946 Call sleepctl_error_feedback for W drift.
21/12/11   cja       CR326938 In abort cb, shutdown g2x if pending
09/12/11   cja       CR321466 In dedicated ID search send RR update if cell not found
08/12/11   cgc       Port 9K Cr305475, (most was included on 30/11/11)
02/12/11   og        CR322973. Ensure the LTE SCHEDULE state is aborted correctly
                     when the frame manager decides there no available frames.
30/11/11   cgc       Port 9K fixes
                     CR 319595  Abort ded search on transition into gsm only mode
                     CR 317538  Initiate abort if in state GTOW_SRCH_STATE_SCHEDULE_LTE when deactivating
                     CR 319382  Ensure LTE is restarted after gsm_only mode change transition
30/11/11   cja       This version #36 is same as #32
29/11/11   cja       This version #35 is the same as #30 with added fix for CR321086
24/11/11   pm        This version #34 is same as #32
24/11/11   pm        This version #33 is same as #28 with added fix for CR321086. Back out some of the klocwork changes.
23/11/11   og        CR321086. Back out some of the klocwork changes.
17/11/11   cja       CR318115 Turn off MCPM W nbr during TCH/TRANS if no W
14/11/11   cja       CR318816 Call g2X shutdown in panic reset
09/11/11   ws        CR317823 - Use correct dedicated Qsearch param. Fix bug in CR 309807
11/11/11   cja       CR317094 Check for pending G2X shutdown in dedicated and when enter idle
10/11/11   cja       CR314495 Fix Klocwork errors
02/11/11   nt        CR316567 Idle id preemption needs to reset the wcdma_id_srch_in_progress flag
04/11/11   og        Ensure the correct index is reset in the fix for CR306187.
01/11/11   og        CR316087. Disable G2L scheduling in TBF suspension.
27/10/11   og        CR306187. Correct the G2W search results averaging.
03/10/11   dv        CR309807 don't do IRAT search if qsearch is 15. Fix bug in CR241049
17/10/11   kb        CR308921 - set wcdma_srch.id_in_tbf_state to Abort only if ID searches
                     is in progress else set the search state to NULL
12/10/11   og        CR302751. Mutex protect the set gsm mode function.
04/10/11   cja       CR310265 Disable WFW after suspend WFW
26/09/11   cja       CR308292 Prevent W schedule before WFW enabled.
23/09/11   og        CR308714. Ensure the srch state is set to NULL when aborting.
19/09/11   og        CR307585. Don't shutdown G2W when reconfirm is pending.
18/08/11   ws        CR300736 - Only reference serving BCCH Rxlev from SCE if BCCH
                     ARFCN is included in BA list otherwise it's out of date
07/09/11   cja       CR303937  Disable FW and RxLM for BPLMN suspend
07/09/11   cja       CR305152  Correct WCDMA list update freq num passed to IRAT check
02/09/11   cja       CR304800  Remove workaround which disabled G2W in dedicated
01/09/11   cja       CR304538  Check active RAT for list update in dedicated mode
31/08/11   cja       CR303797  Send G2L_DEINT when entering gsm only mode
20/08/11   cja       CR301307. Temporarily disable G2W dedicated for BU
15/08/11   og        CR287418. Pend the G2L DEINIT REQ message on the cleanup cnf
                     when the search is active.
15/08/11   og        CR287906. Ensure a G2L DEINIT REQ is sent when the mode changes
                     to gsm only.
28/07/11   og        Cancel the G2L search attempt if sending the startup message
                     fails. CR296627.
14/06/11   cja       Nikel G2W changes
07/06/11   ws        Cleaned up compile warnings and commented out unused functions
28/04/11   og        CR285621. Ensure G2L search attemps occur after BPLMN.
18/04/11   og        CR283929. Race condition between the ISR sending the G2L
                     ABORT req and GSM L1 task sending the CLEANUP req.
05/04/11   cja       L1 I/F change for Nikel IRAT
19/03/11   cja       Initial Nikel - RxLM for IRAT
18/03/11   og        Complete the fix for CR270499.
18/02/11   og        CR274956. Remove the cast from a union and buffer the wcdma
                     params instead.
18/02/11   cs        CR264682 Add support for asynch dsp completion interrupt
02/01/11   og        Disable LTE search during TBF suspension. CR273766
                     Ensure the cleanup req is not sent if abort cnf is pending
                     CR273767.
31/01/11   og        Ensure the number of LTE cells is reported correctly
                     Resolves CR273325.
28/01/11   og        Stop rogue WCDMA reports from being processed by RR when
                     there are no WCDMA cells / UARFCNs stored in L1.
27/01/10   og        Speed up LTE search to occur every 10 paging cycles i.e.
                     5 seconds instead of 23.
25/01/11   og        G2L search and measurement updates.
21/01/11   og        Ensure LTE threshold search 15 is handled correctly. CR270840.
19/01/11   og        Do not send rogue WCDMA cell reports to RR. CR270499.
18/01/11   og        Ensure the G2L cleanup is sent when no cells are detected
                     in the search procedure. CR271049.
17/01/11   og        G2L updates to resolve CRs: 271029, 271051, 271049, 270251,
                     267848, 269942 and 270674.
11/01/11   og        Schedule both low and high priority LTE cells when the GSM
                     serving cell is lower than the threshold. CR270684
14/12/10   og        Ensure the g2x startup is only issued for WCDMA cells on
                     the paging block boundary.
6/12/10    og        Changes to l1_sc_wcdma_ded_pending( ).
29/11/10   og        Adding G2L support.
08/12/10   ws        CR 267264 - Merge NPA clients for CPU resource to 1 client to avoid
                     mutiple npa_cancel_request() which can take 1-2 mSecs to return
29/11/10   nt        CR264717 L1 gets stuck after TBF suspension
15/10/10   cja       CR259899 When exiting GSM only mode ensure CHIPXN_DIV4_CLK is enabled.
24/09/10   cja       Fix Klocwork and Lint warnings
15/09/10   ip        CR248185 Increase MARM speed to 384MHz
16/08/10   ap        CR251318:GL1 doesn't add all the cells in wcdma srch to storage_cell_db
07/07/10   cja       CR244769 Prevent race condition of W Search start and idle to GPRS transfer transition
05/07/10   tjw       uint32 isave required for GL1_ISR_SAVE_LOCK
01/07/10   og        CR243083. Lock the mutex when running l1_sc_wcdma_receive( ).
11/06/10   dv        Fix compile error in last check in
10/06/10   dv        GP-041540:Clarifies use of Qsearch for 3g ncell search
07/06/10   pg        CR241107 Added PACKED_POST to resolve packed struct alignment
                     issues with logging
08/05/10   nt        CR221084 Introduction of FEATURE_GSM_FEMTO_SEARCH
07/05/10   ip        CR235204 DCVS settings added for Poseidon 2
17/03/10   og        Removal of unused debug features.
10-03-16   tjw       Renamed UNUSED to __UNUSED_ARG to avoid name clash.
22/01/10   dv        CR224235 3G ncell measurements affected by bursty TBFs
16/09/09   ps        Resolve Klocwork errors
30/03/09   ws        Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                     to replace FEATURE_GSM_GPRS_MUSTANG
27/02/09   nt        CR172122 - Remove feature FEATURE_GTOW_DONT_PASS_PSC_INFO.
12/18/08   scm       Enhance debug in l1_sc_process_wcdma_id_srch().
01/09/08   cs        Ensure we always set gsm_only indication whenever set mode is called
20/05/08   cs        Fix for Klocwork array bounds checking
15/04/08   sjw       More featurisation cleanup for Worldphone targets.
14/04/08   nt        Featurisation cleanup for World phone target.
28/03/08   nt        CR140522 - Second fix to halt an active IDLE frame search in transfer
                     mode, when a TBF suspend for ID is iminent.
18/03/08   nt        CR140522 - Fix to prevent dedicated ID/reconfirms when there is a TBF
                     suspension for cell ID about to occur.
02/11/07   cs        Fix the previous change to actually track gsm_only when transitioning
                     from manual to automatic mode, otherwise WSTMR off during W search
25/10/07   cs        Remove dependency on l1_sc.h for gsm_only indication
16/09/07   cs/ip     Missed change for 117884 update
31/08/07   ws        Fixed Klocwork warnings
20/07/07   nt        CR107476 sync up the wcdma scheduling and the start of the
                     SACCH periods via addition of flag wcdma_start_of_sacch_indication.
21/05/07   nt        Taken in to Account QSearch_P value when scheduling WCDMA search in
                     transfer mode function l1_sc_wcdma_tick_idle_id_in_tbf.
30/04/07   ws        Changes to Feturisation for first pass GSM Only build
23/01/07   og        Resolve RVCT2.1 compiler warnings, merge from the
                     gpl1_hummer.10.00.43.01 branch.
30/10/06   dv        CR104421 - EcNo, RSSI & RSCP averages set to currently measured
                     value if uninitialised.
26/10/06   cs        Fix the LINT reported problems
19/10/06   nt        CR103708: - Cleared flag ded_srch_aborted that can abort IDLE
                     searches if set upon returning to IDLE.
03/10/06   cs        CR99697 When performing a WCDMA search after as an ID in TBF search
                     we need to reset the id_tick to use the idle setting and not GPRS one
25/09/06   nt        CR101231: - Fixed problem where id_in_tbf_state can get stuck when
                     releasing transfer and aborting a tbf suspend for cell ID. Also added
                     code to unlock the state is it has got stuck.
04/05/06   nt        CR66476 and CR92528 - Cell ID tick is now updated in transfer
                     mode using the id_tick that gets ticked on the IDLE frames of the
                     PDTCH. Function l1_sc_wcdma_tick_idle_id_in_tbf() is called to do this
                     The TBF suspend for ID is also scheduled off the idle ID Tick
                     to keep IDLE and TRANSFER mode cell ID's correctly spaced at 25secs.
24/04/06   nt        CR:91964, added code to expand optimisation so where measurements
                     are aborted in the GTOW_SRCH_STATE_WAIT state they get fasttracked
                     next time in idle.
07/04/06   nt        ID/REC timing hanges for required host test support
05/04/06   nt        Corrected the RSCP_avg calculation to use the EcNo_avg / 2
30/03/06   nt        CR87844 - Changed code that processes the IDLE mode cell ID report.
                     This will make sure that a measurement report to RR will only contain
                     cells previously recofirmed.
                     CR89144 - It will also clear out the stored_meas database of cells
                     not contained in the id report.
                     CR89245 - Changed the way the RSCP_average gets calculated. By adding
                     the RSSI_avg and the EcNo_AVG
17/02/06   nt        CR:87354 - Fix in l1_sc_wcdma_enter_idle() to make sure that
                     cell ID's that are stopped in idle can only remain that way if the
                     dedicated search also has them stopped.
                     CR:87280 - ID searches that get aborted will be fast tracked
                     upon returning to IDLE mode.
02/03/06   ws        Merged RVDS fixes for log type casts
01/31/06   nt        CR77995:Stopped empty cached_xfer_cells from overwriting good
                     stored_meas cells for reconfirmation after single block Xfer.
01/27/06   nt        2 x Fix in l1_sci_tick_wcdma() to make sure that if
                     FEATURE_GTOW_DONT_PASS_PSC_INFO and the gcf flag is set then
                     the cell_ptr is correctly populated with the cell list from RR.
01/25/06   nt        CR:80991 - Fix to update EcNo_avg to strongest instance in the
                     cell ID report.
01/04/06   nt        Added code to run the normal course of events under the
                     FEATURE_GTOW_DONT_PASS_PSC_INFO if the gcf flag isn't set.
12/23/05   nt        Reversed the logic of the previous checking
12/22/05   nt        Added the gl1_is_anite_gcf_flag_set() check within the
                     FEATURE_GTOW_DONT_PASS_PSC_INFO.
12/21/05   nt        Added call to gl1_wcdma_ecio_logarithmic_average() to
                     accurately calculate EcNo_avg.
11/18/05   sv        Fix the bug where aborting during W searches in dedicated\packet
                     transfer mode was causing forced mdsp sleep situation.
11/18/05   nt        CR81279: Additional supression of debug during paging block tick
11/16/05   nt        CR81279: Supression of F3 debug when reconfirmation has occured.
11/15/05   sv        Lint cleanup.
10/10/05   sv        Fix bug where incorrect PN position is listed for PSCs in SACCH
                     measurement report during dedicated 3G searches.
09/16/05   sv        Fix problem with dedicated searches getting continuously aborted
                     if number of cells to ID is greater than 4.
09/08/05   sv        Fix PANIC RESET due to getting stuck in WCDMA SCE while aborting idle mode.
09/07/05   sv        Added support to not pass WCDMA PSC info to searcher driver
                     in Idle mode.
08/29/05   sv        Change prototype for assign_search_params.
08/03/05   sv        Allow Cell ID searches to schedule after higher priority
                     neighbor activities are done.
07/27/05   sv        Send only the latest Cell ID search result to RR.
07/27/05   sv        Send the best peak Ec\No for a particular SCR to RR.
02/06/05   og        Removal of header-header includes.
20/04/05   og        Addition of filtered bep params to the msg metrics log packet.
03/29/05   bm        Merged in fix for L1 Idle Abort
03/15/05   bm        Do not carry 3G Dedicated mode list to TBF
02/03/05   bm        Support for ID searches in TBF.
02/03/05   bm        Added support to log WCDMA SCE activities
01/25/05   bm        Abort any stuck WCDMA DED searches
01/27/05   bm        Fixes for indexing issue in mesrurement report, not restarting
                     dedicated mode searches in TBF
01/26/05   sv        Add support for Frame Manager.
11/29/04   bm        New averaging mechanism for EcIo, i.e. EcIo_Avg = 0.9*new + 0.1*old
11/18/04   bm        Do not carry 3G list in dedicated mode to IDLE mode
10/25/04   bm        - Fix for L1 Idle abort, in a race condition resulted in SCE STRAT, STOP
                     couple of times in same frame
                     - Fix to wait for all dedicated call backs before witching state
09/23/04   bm        Abort the dedicated search at time out, to avoid TASK lockup
08/20/04   bm        Remove call to l1_sc_receive_done, causing ncell cb getting called twice
                     Start Cell IDsearch after TBF only if necessary
                     Do not send cells with low EcNo (-63db) to RR in dedicated/TBF mode
07/26/04   bk        Include gpl1_gprs_serving_cell.h to accomodate a new i/f.
07/01/04   bm        Remove the bad cells in stored_meas after reconfirmation
                     only if the cells are requested in reconfirmation search
06/22/04   dp        Lint cleanup
06/21/04   gsc       Updated to include API changes to l1_sci_set_required_frames.
06/07/04   bm        Remove the check on FDD_Qmin before sending the report to RR
                     Restart cell id searches when we enter Xfer mode
05/26/04   sv        Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters for better
                     code readability.
05/25/04   bm        Fix not to proceed further if there are no cells to search, reconf
05/11/04   bm        Support for Stop and resume dedicated cell id searches
05/05/04   bm        Cell ID search enhancements, Stop/Resume Cell ID search
                     report cell id search results to RR, and increased the
                     cell Id, cell reconf search periodicity
04/08/04   bm        Support for Packet Measurement Report in IDLE mode
04/01/04   jc        Fix for xfer mode getting aborted creating error msgs and
                     also fix to eliminate 'no suitable cells' as we enter and
                     exit xfer mode quickly.
03/26/04   gj        Removed call to l1_sc_wcdma_reset_xfer_instant_average()
                     which wipes RSCP_last, preventing cell reconf.
03/19/04   gw        Removed duplicate declaration of using_this_frame from
                     l1_sc_wcdma_ded_ready().
03/16/04   gw        Hack to make wcdma searching work with redesigned SCE
                     receive module.  Needs to be revisited.
02/29/04   tb        Partial fix for CR39869, srch abort bug.
02/25/04   gw        Fixed some lint errors.
02/17/04   kf        Support for wcdma measurements in transfer
02/15/04   jc        Add in xfer mode code, implemented code review changes.
                     Now keep only top 8 per uarfcn when entering ded/xfer mode.
02/11/04   gfr       Support for reconfirm in GPRS mode.
01/30/04   jc        Fixed some merge problem w sending gtow meas report and
                     Min power.
01/20/04   jc        Updated gtow nonblind HO measurement algorithm.
01/09/04   gw        Replaced reference to l1_idle_data.read_BCCH with
                     l1_idle_data.BCCH_mask.
12/11/03   tb        Enhancements to improve (shorten) time to reselect to
                     a WCDMA cell. Main change is to use PN offset info from
                     previous idle mode searches.
11/14/03   tb        Change parsing of FDD_Qmin per standards CRA375.
11/05/03   gfr       Support for quad-band.
11/05/03   tb        Stop reconfirming cells when 2 consecutive EcNo values
                     are <= -31dB.
10/16/03   tb        Fix for problems 1) where measurements stop during packet
                     transfer, 2) where srch driver stays in dedicated mode
                     when call fades. Also cleanup some commented out code.
01/10/03   kf        Added use of gprs_page_mode.
09/18/03   tb        Updates to gprs xfer code from code review.
09/12/03   tb        Change to log packets from task context, and fix for
                     problem with measurements from sdcch.
09/05/03   sw        Fixed Cell Reconfirmation in non-DRX, so that empty
                     measurement reports are not sent to GRR if valid
                     WCDMA cells are avaialble.
09/03/03   sw        Enhancements to transfer implementation
08/27/03   tb        Fixed display of MSG_ERROR when empty id search result.
08/19/03   sw        Added 3G SEARCH PRIO processing for transfer.
08/18/03   sw        Added WCDMA measurements during transfer
08/07/03   tb        Support for sdcch, and lint cleanup.
08/01/03   tb        Changes from code review.
07/14/03   gw        Removed ASSERTs.
07/11/03   gfr       Lint cleanup.
07/11/03   sw        Fixed clash with WCDMA ID and PCCCH/Monitors
07/11/03   sw        Fixed ID and Reconf cycles on PCCCH with more than 1
                     UARFCN in the WCDMA neighbour list.
07/02/03   sw        Fixed clash of WCDMA list search and PBCCH
06/25/03   tb        Fix to schedule measurements based current paging_mframe.
06/23/03   tb        Change to not send report to RR if in gsm_only mode.
06/06/03   tb        Added support for dedicated mode gtow.
05/14/03   tb        Removed initialization of gsm_only from l1_sc_wcdam_init
04/28/03   tb        Added l1_sc_wcdma_set_mode() and support for handling
                     gsm_only mode notification from RR.
04/16/03   sw        Added initial changes for WCDMA neighbour cell search
                     during packet idle on PCCCH
04/15/03   tb        Added use of l1_sc_globals_ptr->wcdma_list_srch_in_progress
                     to fix problems with aborting idle mode.
03/20/03   tb        Modifications for GPRS problems.
02/13/03   tb        Modifications for aborting measurements.
01/28/03   tb        Modifications to support ignore list.
01/09/03   tb        Modifications for logging known list.
11/13/02   tb        Modification to support latest design.
11/03/02   tb        Initial version.

===========================================================================*/


#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "l1_sc_irat.h"

#ifdef  FEATURE_GSM_TO_LTE
#include "l1_sc_lte_irat.h"
#include "lte_as.h"
#include "lte_l1_types.h"
#include "lte_cphy_irat_meas_msg.h"
#include "lte_ml1_irat_ext_api.h"
#endif  /* FEATURE_GSM_TO_LTE  */


#include "msgr.h"
#include "msgr_geran.h"

#include "rr_l1.h"

#include "l1_sc.h"
#include "l1i.h"
#include "l1_utils.h"
#include "gl1_sys_params.h"
#include "geran_msgs.h"
#include "gl1_msg_wcdma_meas.h"
#include "l1_drx.h"
#include "l1_fm.h"

#include "log.h"            /* For log_commit() and log_alloc() and log codes */
#include "log_codes_gsm.h"  /* For WCDMA log codes */
#include "log_codes_gsm_v.h"
#include "l1_task.h"

#include "l1_isr.h"
#include "gpl1_gprs_isr.h"  /* For GPRS_NORMAL_PAGING enum */
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_task.h"
#include "gpl1_dual_sim.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#include "gpl1_grm_intf.h"

#include "l1_log.h"
#include "wl1x2wsrchapi.h"

#include "gl1_hw_clk_ctl_g.h"
#include "mcpm_api.h"
#include "gl1_hw_sleep_ctl.h"
#include "gl1_hwi.h"

#ifdef FEATURE_GSM_TDS
#include "tdsiratif.h"
#include "tds_l1_api.h"
#endif

#ifdef FEATURE_GL1_GPLT
#include "gplt_result.h"
#endif /* FEATURE_GL1_GPLT*/

#include "gl1_hw_g.h"
#include "geran_dual_sim_g.h"


#include "gl1_msg_wtr_hop.h"

#ifdef FEATURE_SEGMENT_LOADING
#include "mcfg_seg_load.h"
/* from gtmrs.c */
extern interface_t *pi_gl1_wcdma;
#endif

#include "gl1_msgrif_init.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
/* upper limit 11 applied */
#define   notG2L_ALLOW_MORETHAN_8
#define   REDUCE_PERIDICITY    (0)
/************************** enums / structures ***************************************/

/************************** local data ***************************************/

/* Value of I, from HLD (table on page 15) */
/* Following values are choosen to perform Cell ID searches every 25 seconds*/

/* These need are updated because the number of frequencies is increased
 * take into account LTE.
 */
static const uint16  num_pgcycles_per_id[MAX_WCDMA_FREQS][MAX_PG_MFRMS-1] =
{
#ifdef FEATURE_INCREASED_PERIOD_FOR_CELL_ID_CELL_RECONF_SRCHS
  {27, 18, 13, 11, 9, 8, 7, 6},
  {58, 38, 29, 23, 19, 16, 14, 12},
  {60, 40, 30, 24, 20, 17, 15, 13}

#ifdef FEATURE_GSM_TO_LTE
// CgC these need to be check, differences with Dime+
  /* GERAN LTE
   * Added three new entries for LTE frequencies.
   */
  ,{60, 40, 30, 24, 20, 17, 15, 13},
  {60, 40, 30, 24, 20, 17, 15, 13},
  {60, 40, 30, 24, 20, 17, 15, 13},
  {60, 40, 30, 24, 20, 17, 15, 13},
  {60, 40, 30, 24, 20, 17, 15, 13},
  {60, 40, 30, 24, 20, 17, 15, 13}
#endif

#ifdef FEATURE_GSM_TDS
  /* GERAN TDS
   * Added 9 new entries for TDS frequencies.
   */
  ,{60, 40, 30, 24, 20, 17, 15, 13},
  {60, 40, 30, 24, 20, 17, 15, 13},
  {60, 40, 30, 24, 20, 17, 15, 13},
  {60, 40, 30, 24, 20, 17, 15, 13},
  {60, 40, 30, 24, 20, 17, 15, 13},
  {60, 40, 30, 24, 20, 17, 15, 13}
#endif /*FEATURE_GSM_TDS*/

#else /*FEATURE_INCREASED_PERIOD_FOR_CELL_ID_CELL_RECONF_SRCHS*/

  {40, 26, 19, 14, 13, 10, 8, 7},    /* 19 seconds */
  {52, 34, 24, 18, 17, 14, 12, 10},
  {56, 35, 27, 21, 17, 14, 12, 11}

#ifdef FEATURE_GSM_TO_LTE
  /* GERAN LTE
   * Added three new entries for LTE frequencies.
   */
  ,{56, 35, 27, 21, 17, 14, 12, 11},
  {56, 35, 27, 21, 17, 14, 12, 11},
  {56, 35, 27, 21, 17, 14, 12, 11}
#endif

#ifdef FEATURE_GSM_TDS
  ,{56, 35, 27, 21, 17, 14, 12, 11},
  {56, 35, 27, 21, 17, 14, 12, 11},
  {56, 35, 27, 21, 17, 14, 12, 11},
  {56, 35, 27, 21, 17, 14, 12, 11},
  {56, 35, 27, 21, 17, 14, 12, 11},
  {56, 35, 27, 21, 17, 14, 12, 11}

#endif /*FEATURE_GSM_TDS*/

#endif
};

/* Value of I, from HLD (table on page 12) */
/* Following values are choosen to perform Cell Reconfirm searches every 5 seconds*/

/* These need to be updated because the number of frequencies is increased
 * take into account LTE.
 */
static const uint16  num_pgcycles_per_reconfirm[MAX_WCDMA_FREQS][MAX_PG_MFRMS-1] =
{
#ifdef FEATURE_INCREASED_PERIOD_FOR_CELL_ID_CELL_RECONF_SRCHS
  {10, 7, 5, 4, 3, 3, 2, 2},
  {5, 3, 2, 2, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1}

  /* GERAN LTE
   * Added three new entries for LTE frequencies.
   */
#ifdef FEATURE_GSM_TO_LTE
  ,{3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1}
#endif
#ifdef FEATURE_GSM_TDS
  ,{3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1}
#endif

#else /*FEATURE_INCREASED_PERIOD_FOR_CELL_ID_CELL_RECONF_SRCHS*/
  {6, 4, 3, 3, 2, 2, 2, 2},   /* 3 seconds */
  {3, 2, 2, 2, 1, 1, 1, 1},
  {2, 2, 1, 1, 1, 1, 1, 1}

  /* GERAN LTE
   * Added three new entries for LTE frequencies.
   */
#ifdef FEATURE_GSM_TO_LTE
  ,{2, 2, 1, 1, 1, 1, 1, 1},
  {2, 2, 1, 1, 1, 1, 1, 1},
  {2, 2, 1, 1, 1, 1, 1, 1}
#endif
#ifdef FEATURE_GSM_TDS
  ,{3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1},
  {3, 2, 1, 1, 1, 1, 1, 1}
#endif
#endif
};



/* definition of the FEMTO timer and timeout values */
static rex_timer_type l1_isr_femto_timer;

femto_search_state_enum femto_search_state = FEMTO_SEARCH_NULL;
static void l1_isr_femto_timer_searching_start (void);
void l1_isr_femto_timer_stop (void);

/* Additional I values for GPRS Packet Idle on PCCCH
 * with split_pg_cycle fixed at 10.
 * The implementation will need reviewing if split_pg_cycle
 * changes at some stage in the future.
 */
static const uint16  num_pgcycles_per_id_pccch[MAX_WCDMA_FREQS] =
{ 12, 15, 15 };

static const uint16  num_pgcycles_per_reconfirm_pccch[MAX_WCDMA_FREQS] =
{  2,  1,  1 };


const dBx16_T  gsm_l1_priority_search_threshold[ GSM_L1_NUM_PRIORITY_SEARCH_THRESHOLDS ] =
{
   -98*16,  /* 0 */
   -95*16,
   -92*16,
   -89*16,
   -86*16,
   -83*16,
   -80*16,
   -77*16,
   -74*16,
   -71*16,
   -68*16,  /* 10 */
   -65*16,
   -62*16,
   -59*16,
   -56*16,
   -10*16   /* 15 GSM Rxlev cannot be better than this */
};

extern dedicated_data_T l1_dedicated_data[];


/************************** local function definitions ***************************************/

/* needs moving to header !! used everywhere */
extern dedicated_data_T l1_dedicated_data[];

/************************** local function definitions ***************************************/

#ifdef FEATURE_GSM_TDS
/* function declaration to retrieve stored frequency information */
static boolean  get_stored_meas_freq_entry( uint32                                    UARFCN,
                                            uint8                                     rat,
                                            gl1_wcdma_cell_meas_rpt*                  freq_params_ptr,
                                            gas_id_t                                  gas_id
                                          );


static void  l1_sc_send_tds_cell_search_results_to_rr( void* freq_index, gas_id_t gas_id );


/* function declaration to update exisiting stored frequency information */
static void  update_stored_meas_freq_entry( uint32                           UARFCN,
                                            const  gl1_wcdma_cell_meas_rpt*   freq_params_ptr,
                                            uint8                            rat,
                                            gas_id_t                         gas_id
                                          );

static void  update_stored_meas_jds_cells( uint32                           UARFCN,
                                           uint8                            num_jds_cells,
                                           const tdsirat_cell_type*         cell_params_ptr,
                                           gas_id_t                         gas_id
                                          );
static uint16  l1_sc_add_tds_freq_list   ( t_rr_params *rr_params_ptr, utran_tdd_cell_list_T     *pCellList,  uint16 msg_num_cells , uint16 new_num_freqs );

#endif /* FEATURE_GSM_TDS*/
static void  gsm_l1_set_wcdma_search_cycles( uint16  num_freqs,
                                             uint16  pg_mfrms,
                                             gas_id_t gas_id
                                           );

static void  gsm_l1_set_lte_search_cycles (uint16 pg_mfrms, gas_id_t gas_id);


#ifdef FEATURE_GSM_TO_LTE
static void                  l1_sc_sort_priority_list_index( rr_l1_irat_frequency_priority_list_t* pList , uint16 *pSorted_list, gas_id_t gas_id);
static rr_l1_eutran_type_e   l1_sc_get_lte_type(rr_l1_eutran_frequency_t* pFreq, uint16 num_entries, lte_earfcn_t earfcn );
static uint16                l1_sc_get_lte_tdd_count(rr_l1_eutran_frequency_t *pFreq, uint16 num_entries);
static void                  l1_sc_add_lte_bw_freq_list( t_rr_params *rr_params_ptr, rr_l1_eutran_frequency_t  *pEutran,    uint16 msg_num_freqs , uint16 new_num_freqs, gas_id_t gas_id );

#endif


#ifdef FEATURE_WCDMA
static uint16  l1_sc_add_wcdma_freq_list ( t_rr_params *rr_params_ptr, wcdma_cell_list_T         *pCellList,  uint16 msg_num_cells , uint16 new_num_freqs );
#endif

#ifdef FEATURE_SGLTE_G2X_CONN_MODE_RESEL
static uint16  get_msg_rat_count      ( rr_l1_irat_frequency_priority_list_t *pList, rr_l1_irat_rat_e rat_type, gas_id_t gas_id);
#endif
#ifdef  FEATURE_GSM_TO_LTE
static uint16  l1_sc_add_priority_freq( rr_l1_irat_frequency_priority_list_t* pList, rr_l1_eutran_frequency_t *pFreq, uint16 freqCount, gas_id_t gas_id );
#endif
static void    l1_sc_cap_L_plus_W_totals(boolean tds_freqs_present,gas_id_t gas_id);


extern boolean nv_for_search_all_w_cell ;

static void    gsm_l1_set_ecio_rssi_levels( uint16 rat_idx, uint16 qsearch, uint16 ecode, gas_id_t gas_id );

static void    l1_sc_stop_cm_g2l(gas_id_t gas_id);

/*
** This only needs to be less than the actual timeout by an adequate margin
** to ensure RR gets response before RR timer expiry.
*/
#define G2X_FRAMES_PER_SEC             (216)
#define G2X_FRAMES_FOR_2SEC            (440)   /* includes +30ms margin */
#define G2X_FRAMES_FOR_5SEC           (1390)   /* includes +30ms margin */
#define G2X_FORCED_FN_MARGIN            (20)
#define G2X_FORCED_RESEL_5_SECS          (5)

static uint16  g2x_forced_update_tick_offset_with_speedup(uint16 pg_mfrms );
static uint16  g2x_forced_update_tick_offset(uint16 pg_mfrms );

/************************** external function definitions ***************************************/


/*===========================================================================

FUNCTION l1_sc_restart_cell_id_srch

DESCRIPTION
  This function is called to initialize stop_cell_id_srch, i.e.
  This will restart the Cell Id searches again.
DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_restart_cell_id_srch(gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    uint16 idx;

#ifdef FEATURE_GSM_TDS_DEBUG
       MSG_GERAN_HIGH_0_G("Calling l1_sc_restart_cell_id_srch ");
#endif

   /*
   ** if force measure running do not change timers 
   */
   if(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_NULL )
   {
    for (idx=0; idx<MAX_WCDMA_FREQS; idx++)
    {
        /* Start the Id search, and Reconf search as soon as possible */
      l1_sc_wcdma_data_ptr->stop_cell_id_srch[idx] = 0;
        l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[idx] = 0;
        l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[idx]        = 0;
  
      l1_sc_wcdma_data_ptr->wcdma_srch.last_idle_id_aborted[idx] = FALSE;
    }
   }
}

/*===========================================================================

FUNCTION l1_sc_restart_specific_cell_id_srch

DESCRIPTION
  This function is called to initialize stop_cell_id_srch, i.e.
  This will restart the Cell Id search on a particular freq again.
DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_restart_specific_cell_id_srch(uint16 freq_idx, gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

#ifdef FEATURE_GSM_TDS_DEBUG
       MSG_GERAN_HIGH_0_G("Calling l1_sc_restart_cell_id_srch ");
#endif

   /* if forced measure is not running, we are good to restart both ID and reconf searches */
   if(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_NULL )
   {
      /* Start the Id search, and Reconf search as soon as possible for this freq index */
      l1_sc_wcdma_data_ptr->stop_cell_id_srch[freq_idx] = 0;
      l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[freq_idx] = 0;
      l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx]        = 0;
      l1_sc_wcdma_data_ptr->wcdma_srch.last_idle_id_aborted[freq_idx] = FALSE;
   }
   
   /* If forced search is going on, and reconf attempt fails, then restart only ID search
      let reconf scheduling be as per forced search timelines*/
   else
   {
      l1_sc_wcdma_data_ptr->stop_cell_id_srch[freq_idx] = 0;
      l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = 0;
      l1_sc_wcdma_data_ptr->wcdma_srch.last_idle_id_aborted[freq_idx] = FALSE;
   }
}

/*===========================================================================

FUNCTION GSM_L1_UPDATE_RR_REPORT

DESCRIPTION
  This function is called from the L1 task to update the
  rr_report structure with the measurement results.

  It does not send the measurement report to RR.

  Only the 6 strongest cells are taken into account (RSCP).

  Uses the following structures:

  stored_meas

  rr_ignore_list

  rr_report

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gsm_l1_update_rr_report( const t_stored_meas        stored_meas[MAX_WCDMA_FREQS],
                                     const t_rr_ignore_list*    rr_ignore_list_ptr,
                                     const t_rr_params*         rr_params_ptr,
                                     t_rr_report*               rr_report_ptr,
                                     gas_id_t                   gas_id
                                   )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16  meas_idx;
  uint16  freq_idx;

  uint16  ignore_num_cells = rr_ignore_list_ptr->num_cells;
  uint16  num_freqs        = rr_params_ptr->num_freqs;

  int16 rscp_threshold = -1024;

  /* KW fix */
  if(ignore_num_cells > MAX_IGNORE_CELLS)
  {  ignore_num_cells = MAX_IGNORE_CELLS;}

  if(num_freqs > MAX_WCDMA_FREQS)
  {  num_freqs = MAX_WCDMA_FREQS;}

  /* Update our report structure with MAX_NUMBER_OF_BEST_WCDMA_CELLS = 6 strongest cells */
  rr_report_ptr->num_cells = 0;

  for( freq_idx = 0; freq_idx < num_freqs; freq_idx++ )
  {
    if( RR_L1_IRAT_RAT_WCDMA == rr_params_ptr->freq_list[ freq_idx ].rat )
    {
      for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
      {
        int16 idx;
        uint16  uarfcn    = rr_params_ptr->freq_list[freq_idx].arfcn.UARFCN;


        /* A separate function is used for LTE cells */
        uint16  scr_code  = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].wcdma_cell.scr_code;
        int16 srxlevmin = rscp_threshold;

        /* assign new thresholds if this cell is on ignore list */
        for (idx=0; idx < ignore_num_cells; idx++)
        {
          if ((uarfcn   == rr_ignore_list_ptr->cell_list[idx].UARFCN) &&
              (scr_code == rr_ignore_list_ptr->cell_list[idx].scr_code))
          {
            if (rr_ignore_list_ptr->cell_list[idx].threshold_valid)
            {
              srxlevmin = rr_ignore_list_ptr->cell_list[idx].s_rxlevmin;

              MSG_GERAN_MED_2_G("Ignore threshold uarfcn:%d scr_code%d", uarfcn, scr_code);
            }
            else
            {
              /* no threshold, so ignore regardless of energy */
              srxlevmin = 0;

              MSG_GERAN_MED_2_G("Ignoring uarfcn:%d scr_code%d", uarfcn, scr_code);
            }
            break;
          }
        } /* End of for (idx=0; idx < rr_ignore_list_ptr->num_cells; idx++) */

        if ((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg != 0) &&
            (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg > srxlevmin))
        {
          /* add this to our report list */
          int16  rscp = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg;
          int16  rssi = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg;
          int16  ecno = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_avg;

          if (++rr_report_ptr->num_cells > MAX_NUMBER_OF_BEST_WCDMA_CELLS)
          {
            rr_report_ptr->num_cells = MAX_NUMBER_OF_BEST_WCDMA_CELLS;
          }

          /* Place the new cell in the correct ranking on rscp basis */

          /* @@TODO: This is a problem because the rr report uses unions
           * and hence includes both WCDMA and LTE cells.
           *
           * Need two separate rr report structures for WCDMA and LTE.
           * and this would require keeping a copy of uarfn in the cell
           * structure to remove any dependency on l1_sc_wcdma_data_ptr->rr_params.freq_list[]
           *
           */
          for (idx = (int16) rr_report_ptr->num_cells-1; idx>0; idx--)
          {
            if (rscp <= rr_report_ptr->wcdma_cell[idx-1].rscp)
            {
              break;
            }

            /* @@TODO: What if this cell we are copying to is an LTE cell
             *
             * Split the rr report structure to a WCDMA one and an LTE one.
             */
            rr_report_ptr->wcdma_cell[idx] = rr_report_ptr->wcdma_cell[idx-1];
          }

          rr_report_ptr->wcdma_cell[idx].freq     = uarfcn;
          rr_report_ptr->wcdma_cell[idx].scr_code = scr_code;

          rr_report_ptr->wcdma_cell[idx].rscp     = rscp;
          rr_report_ptr->wcdma_cell[idx].rssi     = rssi;
          rr_report_ptr->wcdma_cell[idx].ecno     = ecno;
          /* Report pn_pos as chips, so convert from chipx8 */
          rr_report_ptr->wcdma_cell[idx].pn_pos   = (uint16)(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].wcdma_cell.PN_pos >>  3);

          if (rr_report_ptr->num_cells == MAX_NUMBER_OF_BEST_WCDMA_CELLS)
          {
            rscp_threshold = rr_report_ptr->wcdma_cell[MAX_NUMBER_OF_BEST_WCDMA_CELLS-1].rscp;
          }
        }
      } /* End of for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++) */
    }   /* if( RR_L1_IRAT_RAT_WCDMA == rr_params_ptr->freq_list[ freq_idx ].rat ) */
#ifdef FEATURE_GSM_TDS
    else if( RR_L1_IRAT_RAT_UTRAN_TDD == rr_params_ptr->freq_list[ freq_idx ].rat )
    {
      /*Only jds cell is reliable cell which do reseelct to it, others maybe fake cell, no need to report*/
      MSG_GERAN_HIGH_3_G("Update RR report: stored_meas: freq_idx %d, num_jds_cells %d, uarfcn %d",
               freq_idx,
               l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.num_of_jds_cells,
               l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN);
      for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.num_of_jds_cells; meas_idx++)
      {
        int16 idx;
        uint16  uarfcn    = rr_params_ptr->freq_list[freq_idx].arfcn.UARFCN;


        /* A separate function is used for LTE cells */
        uint16 cell_id   = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].tds_cell.cell_parameter_id;
        int16  srxlevmin = rscp_threshold;

        /* assign new thresholds if this cell is on ignore list */
        for (idx=0; idx < ignore_num_cells; idx++)
        {
          if ((uarfcn == rr_ignore_list_ptr->cell_list[idx].UARFCN) &&
              (cell_id == rr_ignore_list_ptr->cell_list[idx].cell_parameter_id))
          {
            if (rr_ignore_list_ptr->cell_list[idx].threshold_valid)
            {
              srxlevmin = rr_ignore_list_ptr->cell_list[idx].s_rxlevmin;

              MSG_GERAN_MED_2_G("Ignore TDS threshold uarfcn:%d cell_param_id %d", uarfcn, cell_id);
            }
            else
            {
              /* no threshold, so ignore regardless of energy */
              srxlevmin = MIN_TDD_CELL_RSCP_VALUE;

              MSG_GERAN_MED_2_G("Ignoring TDS uarfcn:%d cell_param_id %d", uarfcn, cell_id);
            }
            break;
          }
        } /* End of for (idx=0; idx < rr_ignore_list_ptr->num_cells; idx++) */

        MSG_4(MSG_SSID_GSM_L1, MSG_LEGACY_HIGH,
              "TD update RR report: RSCP_avg %d srxlevmin %d freq_idx %d meas_idx %d",
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg,
              srxlevmin,
              freq_idx,
              meas_idx);

        /*TDS needs to report RSCP so we can use the existing WCDMA structures*/
        if ((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg != 0) &&
            (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg > srxlevmin))
        {
          /* add this to our report list */
          int16  rscp = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg;

          if (++rr_report_ptr->num_cells > MAX_NUMBER_OF_BEST_WCDMA_CELLS)
          {
            rr_report_ptr->num_cells = MAX_NUMBER_OF_BEST_WCDMA_CELLS;
          }

          for (idx = (int16) rr_report_ptr->num_cells-1; idx>0; idx--)
          {
            if (rscp <= rr_report_ptr->wcdma_cell[idx-1].rscp)
            {
              break;
            }
            rr_report_ptr->wcdma_cell[idx] = rr_report_ptr->wcdma_cell[idx-1];
          }

          rr_report_ptr->wcdma_cell[idx].freq     = uarfcn;

          rr_report_ptr->wcdma_cell[idx].cell_parameter_id = cell_id;

          rr_report_ptr->wcdma_cell[idx].rscp     = rscp;

          if (rr_report_ptr->num_cells == MAX_NUMBER_OF_BEST_WCDMA_CELLS)
          {
            rscp_threshold = rr_report_ptr->wcdma_cell[MAX_NUMBER_OF_BEST_WCDMA_CELLS-1].rscp;
          }
        }
      } /* End of for (meas_idx=0; meas_idx<stored_meas[freq_idx].num_cells; meas_idx++) */
       //If the no. of reported cells are 0, still update the rr_report
      if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.num_of_jds_cells == 0)
      {
           uint16 meas_idx =0,idx =0;
           uint16 cell_id   = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].tds_cell.cell_parameter_id; //Take cell id from stored meas
           MSG_GERAN_HIGH_3_G(" jds_cells are 0: uarfcn %d reset rr_rep: num_cells %d stored_meas_num_cells %d",
                                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN,
                                (rr_report_ptr->num_cells ),
                                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells);

           /*Loop for the no. of cells present in stored_meas or 6 cells whichever is less*/
           for (idx=0; ((idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells)&& (idx< MAX_NUMBER_OF_BEST_WCDMA_CELLS)); idx++)
            {
              if(cell_id == rr_report_ptr->wcdma_cell[idx].cell_parameter_id)//If cell id is present in RR report
                {
                 rr_report_ptr->wcdma_cell[idx].rscp = MIN_TDD_CELL_RSCP_VALUE; //Set all its parameters to 0 in that case
                 rr_report_ptr->wcdma_cell[idx].cell_parameter_id = 0;
                   rr_report_ptr->wcdma_cell[idx].freq = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN;
                 rr_report_ptr->wcdma_cell[idx].scr_code = 0;
                   rr_report_ptr->wcdma_cell[idx].rssi = MIN_LVL_dBm_x16;
                 rr_report_ptr->wcdma_cell[idx].ecno = 0;

                 if(rr_report_ptr->num_cells > 0)
                  {
                    rr_report_ptr->num_cells--;             //Decrease the no. of cells
                  }
                }
            }
        }

    }
#endif /*FEATURE_GSM_TDS*/
  }  /* End of for (freq_idx=0; freq_idx<rr_params.num_freqs; freq_idx++) */
} /* gsm_l1_update_rr_report */


#ifdef FEATURE_GSM_TO_LTE
static void  gsm_l1_update_rr_lte_report( const t_stored_meas        stored_meas[MAX_WCDMA_FREQS],
                                          const t_rr_ignore_list*    rr_ignore_list_ptr,
                                          const t_rr_params*         rr_params_ptr,
                                          t_rr_report*               rr_report_ptr,
                                          gas_id_t                   gas_id
                                        )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16  meas_idx;
  uint16  freq_idx;

  /* Update our report structure with MAX_NUMBER_OF_BEST_WCDMA_CELLS = 6 strongest cells */
  rr_report_ptr->num_cells = 0;

  for( freq_idx=0; freq_idx < rr_params_ptr->num_freqs; freq_idx++ )
  {
    /* The rr_report structure can be fully populated with LTE measurements
   * without the need to add WCDMA checks.
   * Overwriting WCDMA measurements or cells is not an issue in this
   * case.
   */
    if( RR_L1_IRAT_RAT_LTE == rr_params_ptr->freq_list[ freq_idx ].rat )
    {
      for( meas_idx = 0; meas_idx < l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++ )
      {
        /* WCDMA cells use a separate function */

        lte_earfcn_t  euarfcn = rr_params_ptr->freq_list[freq_idx].arfcn.euarfcn;

        uint16  cell_id = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].lte_cell.cell_id;

        int16  rsrp = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_avg;
        int16  rsrq = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrq_avg;

        if(rr_report_ptr->num_cells >= MAX_LTE_MEASUREMENTS_PER_FREQUENCY)
        {
          rr_report_ptr->num_cells = MAX_LTE_MEASUREMENTS_PER_FREQUENCY - 1;

          /* this should break rather than overwrite the last cell */

          break;  /* G2L added */
        }

        if (( rsrp > l1_sc_wcdma_data_ptr->rr_params.gl1_lte_cell_rsrp_threshold ) && (rsrp < 0))
        {
          rr_report_ptr->lte_cell[rr_report_ptr->num_cells].freq    = euarfcn;
          rr_report_ptr->lte_cell[rr_report_ptr->num_cells].cell_id = cell_id;

          rr_report_ptr->lte_cell[rr_report_ptr->num_cells].rsrp    = rsrp;
          rr_report_ptr->lte_cell[rr_report_ptr->num_cells].rsrq    = rsrq;

          rr_report_ptr->num_cells++;
        }
      } /* End of for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++) */
    }   /* if( RR_L1_IRAT_RAT_LTE == rr_params_ptr->freq_list[ freq_idx ].rat ) */

  }     /* End of for (freq_idx=0; freq_idx<l1_sc_wcdma_data_ptr->rr_params.num_freqs; freq_idx++) */

  //rr_report_ptr->num_cells = l1_sc_wcdma_data_ptr->stored_meas[0].num_cells;


} /* gsm_l1_update_rr_lte_report */
#endif /* FEATURE_GSM_TO_LTE */
/*===========================================================================

FUNCTION L1_SC_WCDMA_SEND_RR_REPORT

DESCRIPTION
  This function is called from the L1 task to send the
  wcdma measurement report to RR.

  l1_sc_wcdma_data_ptr->wcdma_srch.send_report is used to ensure the report is sent only in idle
  mode and when the gsm_only flag is FALSE.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_send_rr_report(void* unused, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  if (l1_sc_wcdma_data_ptr->wcdma_srch.send_report)
  {
    /* Send report to RR */
    l1_scdrv_wcdma_idle_meas_ind(gas_id);   /* L1_send_MPH_SURROUND_WCDMA_IDLE_MEAS_IND */
  }

/*lint -esym(715,unused)*/ /* unused is not used */
}

#ifdef FEATURE_INCREASED_PERIOD_FOR_CELL_ID_CELL_RECONF_SRCHS
/*===========================================================================

FUNCTION L1SC_SEND_CELL_ID_SRCH_TO_RR

DESCRIPTION
  This function is called from the L1 task to send the cell id search results to RR
  so that RR can start the 5second timer, without waiting for cell reconf search.

  1- It scans through the stored measurements and marks the all cells for
     deletion except the strongest 8 (RSCP).

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_send_cell_id_srch_to_rr(void* freq_index, gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    uint16 meas_idx, freq_idx;

    uint16 new_idx = 0;
    uint16 min_idx = 0;
    uint16 num_valid = 0;

    int16 min_avg = 0;

  freq_idx = (uint16) (uint32) (void *) freq_index;

#ifdef FEATURE_GSM_TDS
  if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat || RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat )
#else
  if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat)
#endif
  {
    /*Of the cells with valid averages, mark for deletion all
       but the strongest 8 */
    for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
    {
      int16 curr_avg = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg;

      if (curr_avg != 0)
      {
        if (num_valid < 8)
        {
          num_valid++;

          if (curr_avg < min_avg)
          {
            min_avg = curr_avg;
            min_idx = meas_idx;
          }
        }
        else
        {
          if (curr_avg < min_avg)
          {
            /* mark this cell for deletion (set RSCP_list = 0) */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = 0;

            /* remove this cell from further consideration */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg = 0;
          }
          else
          {
            uint16 tmp_idx;

            /* mark the min cell for deletion (set RSCP_list = 0) */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_last = 0;

            /* remove min cell from further consideration */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_avg = 0;

            /* go find new min cell */
            min_avg = 0;

            for (tmp_idx = 0; tmp_idx <= meas_idx; tmp_idx++)
            {
              int16 tmp_avg = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[tmp_idx].wcdma_cell.RSCP_avg;
              if ((tmp_avg != 0) && (tmp_avg < min_avg))
              {
                min_avg = tmp_avg;
                min_idx = tmp_idx;
              }
            }
          }
        }
      }        /* if (curr_avg != 0) */
    }          /* for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++) */

    /*Delete any items in our stored_meas struct marked for deletion */
    for (meas_idx=0, new_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
    {
      if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last != 0)
      {
        if (new_idx != meas_idx)
        {
          /* @@TODO: No problems here with LTE and WCDMA cell indices clashing */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx] =
                  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx];

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx] =
                  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx];
        }

        new_idx++;
      }
    }
    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = new_idx;

    if (new_idx != meas_idx)
    {
      /* set these 2 params to zero (to cleanup, not necessary) */
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_last = 0;

      if(RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat)
      {
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx].wcdma_cell.scr_code = 0;
      }
#ifdef FEATURE_GSM_TDS
      else if (RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat)
      {
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx].tds_cell.cell_parameter_id = 0;
      }
#endif
    }

    /* Generate report for RR */
    gsm_l1_update_rr_report( &( l1_sc_wcdma_data_ptr->stored_meas[0] ),
                             &l1_sc_wcdma_data_ptr->rr_ignore_list,
                             &l1_sc_wcdma_data_ptr->rr_params,
                             &l1_sc_wcdma_data_ptr->rr_report,
                             gas_id
                           );

    /* Send report to RR */

    l1_sc_wcdma_send_rr_report(NULL, gas_id);

  }
  else
  {
    MSG_GERAN_ERROR_2_G(" Cannot send WCDMA/TDS search results for an LTE (%d) cached frequency %d ",
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat,l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN);
  }
} /* l1_sc_send_cell_id_srch_to_rr */

#endif

/*===========================================================================

FUNCTION l1_sc_send_lte_cell_search_results_to_rr

DESCRIPTION
  This function is called from the L1 task to send the lte search results to RR

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#ifdef FEATURE_GSM_TO_LTE
void  l1_sc_send_lte_cell_search_results_to_rr( uint16 freq_index, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint16  freq_idx      = freq_index;

  if( RR_L1_IRAT_RAT_LTE == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat )
  {
    rr_l1_irat_measurements_u  irat_measurements;

    uint16  meas_idx  = 0;
    uint16  store_idx = 0;
    /* Send report to RR */
    irat_measurements.lte.earfcn = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn;

    while( meas_idx < l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells )
    {
      if((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_avg < 0) &&
        (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_avg > l1_sc_wcdma_data_ptr->rr_params.gl1_lte_cell_rsrp_threshold))
      {
        irat_measurements.lte.entries[store_idx].pcid = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].lte_cell.cell_id;
        irat_measurements.lte.entries[store_idx].rsrp = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_avg;
        irat_measurements.lte.entries[store_idx].rsrq = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrq_avg;
        store_idx++;
      }
      meas_idx++;
    }

    /* record number of cells added to message above threshold */
    irat_measurements.lte.no_of_entries = store_idx;


    L1_send_MPH_SURROUND_IRAT_MEAS_IND( RR_L1_IRAT_RAT_LTE, &irat_measurements, gas_id );
    
    if(store_idx)
    {
       MSG_GERAN_HIGH_5_G( "G2L:rr_meas_ind earfcn %d pcid[0]=%d rsrp[0]=%d no_of_entries=%d num_cells=%d" ,
                          irat_measurements.lte.earfcn,
                          irat_measurements.lte.entries[0].pcid,
                          irat_measurements.lte.entries[0].rsrp,
                          irat_measurements.lte.no_of_entries,
                          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells
                      );
    }

    l1_sc_forced_meas_event( freq_idx, FORCE_MEAS_COMPLETED, gas_id);

  }  /* if( RR_L1_IRAT_RAT_LTE == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat ) */
  else
  {
    MSG_GERAN_ERROR_2(" Cannot send LTE search results for a WCDMA frequency idx %d rat %d",freq_idx, l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat );
  }

} /* l1_sc_send_lte_cell_search_results_to_rr */

#endif /* #ifdef FEATURE_GSM_TO_LTE */


/*===========================================================================

FUNCTION L1_SC_WCDMA_LOG_KNOWN_LIST

DESCRIPTION
  This function is called from the L1 task to generate the
  log the current known cell list.

  It is called after the reconfirmation search results are
  processed.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_log_known_list(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  LOG_GSM_WCDMA_KNOWN_LIST_C_type  *log_ptr;    /* pointer to log buffer */

  uint16  meas_idx, freq_idx;
  uint16  log_size;

  uint16  num_cells = 0;

  for( freq_idx = 0; freq_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs; freq_idx++ )
  {
    if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat )
    {
      num_cells += l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;
    }
  }

  if (num_cells > LOG_MAX_KNOWN_CELL_LIST_SIZE)
  {   num_cells = LOG_MAX_KNOWN_CELL_LIST_SIZE;}

  log_size = (uint16) (sizeof(LOG_GSM_WCDMA_KNOWN_LIST_C_type) -
             ((LOG_MAX_KNOWN_CELL_LIST_SIZE - num_cells)*sizeof(l1_log_wcdma_known_list_info_T)));

  /* Try to allocate memory for the log packet */
  if ( NULL != (log_ptr = (LOG_GSM_WCDMA_KNOWN_LIST_C_type  *)log_alloc( LOG_GSM_WCDMA_KNOWN_LIST_C, log_size )) )
  {
    /* Fill packet and commit it */
    log_ptr->Cell_Count  =  (uint8) num_cells;

    /* Loop through all frequencies and cells */
    num_cells = 0;

    for( freq_idx=0; freq_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs; freq_idx++ )
    {
      uint16 stored_num_cells = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;
       /* KW fix */
      if(stored_num_cells > MAX_WCDMA_CELLS_PER_UARFCN )
      {  stored_num_cells = MAX_WCDMA_CELLS_PER_UARFCN;}

      if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
      {
        for( meas_idx = 0; meas_idx < stored_num_cells; meas_idx++ )
        {
          uint16 idx;

          uint16 uarfcn = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN;

          uint16 scr_code = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].wcdma_cell.scr_code;

          if (num_cells >= LOG_MAX_KNOWN_CELL_LIST_SIZE)
          {
            break;
          }

          log_ptr->Cell[num_cells].UARFCN   = uarfcn;
          log_ptr->Cell[num_cells].scr_code = scr_code;

          /* Shift PN position from chipx8 to chipx1 */

          log_ptr->Cell[num_cells].pn_pos =
               (uint16)(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].wcdma_cell.PN_pos >> 3);

          if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].wcdma_cell.diversity == DIVERSITY_ON)
          {
            log_ptr->Cell[num_cells].diversity = TRUE;
          }
          else
          {
            log_ptr->Cell[num_cells].diversity = FALSE;
          }

          log_ptr->Cell[num_cells].ignore           = 0;
          log_ptr->Cell[num_cells].thresholds_valid = 0;
          log_ptr->Cell[num_cells].sQual            = 0;
          log_ptr->Cell[num_cells].sRxLev           = 0;

          log_ptr->Cell[num_cells].RSCP =
               (int8) l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg;

          /* was (int8) l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].EcNo_avg; */
          log_ptr->Cell[num_cells].EcNo =
               (int8) l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_avg/2;

          /* set ignore fields if this cell is on ignore list */
          for (idx=0; idx < l1_sc_wcdma_data_ptr->rr_ignore_list.num_cells; idx++)
          {
            if ((uarfcn == l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].UARFCN) &&
                (scr_code == l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].scr_code))
            {
              log_ptr->Cell[num_cells].ignore = 1;

              if (l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].threshold_valid)
              {
                log_ptr->Cell[num_cells].thresholds_valid = 1;
                log_ptr->Cell[num_cells].sQual = (int8)
                           l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].s_qualmin;
                log_ptr->Cell[num_cells].sRxLev = (int8)
                           l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].s_rxlevmin;
              }
            }
          }
          num_cells++;

        } /* for( meas_idx = 0; meas_idx < l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++ ) */
      }   /* if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat ) */
    }     /* for( freq_idx=0; freq_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs; freq_idx++ ) */

    log_commit((PACKED void *)log_ptr);

  }
  else
  {
    MSG_GERAN_ERROR_0_G(" NULL LOG_GSM_WCDMA_KNOWN_LIST_C pointer ");
  }
}


/*===========================================================================

FUNCTION GSM_L1_SET_WCDMA_SEARCH_CYCLES

DESCRIPTION
  This function is called when either paging multiframes or
  number of frequencies to search has changed. The cycle counts
  of when to issue searches is adjusted.

  The cycles are set for both id and reconfirm searches.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gsm_l1_set_wcdma_search_cycles( uint16  num_freqs,
                                            uint16  pg_mfrms,gas_id_t gas_id
                                          )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
  /* Called in Task context */
  MSG_GERAN_LOW_0_G("gsm_l1_set_wcdma_search_cycles");

  /* Ensure the ID search cycle in idle mode does not exceed
   * MAX_SEARCH_CYCLE_WCDMA_FREQS*30 seconds.
   * The spec requires 3 frequencies but the UE can potentially receive more.
   * L1 will store more than 3 when interRAT search to technologies other than WCDMA
   * is enabled.
   */

#ifdef FEATURE_GSM_TDS
  /* 
  ** CMCC requirement: GL1 shall measure upto 9 TDD frequencies as fast as possible.
  ** use tds_update_list_irat_active as this is set immediately, rather than waiting for confirm.
  ** wrong interval is set otherwise
  */
  if(l1_sc_wcdma_data_ptr->tds_update_list_irat_active)
  {
    /* 
    ** this reduces the scheduling interval for TDS to the fastest, always 1 freq, irrespective of the number of frequencies 
    */  
    num_freqs = (num_freqs > MAX_FREQ_FASTEST_ACQ_TIME) ? MAX_FREQ_FASTEST_ACQ_TIME : num_freqs;
  }
  else
#endif
  {
    num_freqs = (num_freqs > MAX_SEARCH_CYCLE_WCDMA_FREQS) ? MAX_SEARCH_CYCLE_WCDMA_FREQS : num_freqs;
  }

  MSG_GERAN_MED_2_G("G2W search time for num_W_freq=%d with pg_mfrms=%d ",num_freqs, pg_mfrms );

  if( l1_sc_globals_ptr->mode == L1SCModeGprsIdle)
  {
    if( (num_freqs > 0) && (num_freqs <= MAX_WCDMA_FREQS) )
    {
      l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle = (uint16)
        (num_pgcycles_per_id_pccch[num_freqs - 1] * num_freqs);

      l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_cycle = (uint16)
        (num_pgcycles_per_reconfirm_pccch[num_freqs - 1] * num_freqs);
    }
    else
    {
      MSG_GERAN_ERROR_1_G(" invalid num freqs %d ", num_freqs);
    }
  }
  else if (((pg_mfrms > 1) && (pg_mfrms  <= MAX_PG_MFRMS)) &&
          ((num_freqs > 0) && (num_freqs <= MAX_WCDMA_FREQS)))
  {

    /* from HLD Table on page 15 (N*I) */
    l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle = (uint16)
       (num_pgcycles_per_id[num_freqs-1][pg_mfrms-2] * num_freqs);

    /* from HLD Table on page 12 (N*I) */
    l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_cycle = (uint16)
       (num_pgcycles_per_reconfirm[num_freqs-1][pg_mfrms-2] * num_freqs);

#ifdef FEATURE_GSM_TDS

    /* If total num of freq (L+T) exceed reconf cycle of TDS then ID search may be blocked */
    if(l1_sc_wcdma_data_ptr->tds_update_list_irat_active)
    {
        if(l1_sc_wcdma_data_ptr->rr_params.num_freqs >= l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_cycle)
        {
            num_freqs = l1_sc_wcdma_data_ptr->rr_params.num_freqs;

            /* Add 5 and 3 to id_cycle and reconf_cycle to accomodate G ncells */
            l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle        = 2*num_freqs + 5;
            l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_cycle =   num_freqs + 3;

        }
    }

#endif /* FEATURE_GSM_TDS */

  }
  else
  {
    MSG_GERAN_ERROR_3_G("G2X: sc mode %d num freqs %d pg mfrms %d", l1_sc_globals_ptr->mode, num_freqs, pg_mfrms );
  }
} /* gsm_l1_set_wcdma_search_cycles */

/*===========================================================================

FUNCTION gsm_l1_set_lte_search_cycles

DESCRIPTION

   Set the LTE search interval in paging ticks, nearest to give 5 secs.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gsm_l1_set_lte_search_cycles (uint16 pg_mfrms, gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
    uint16 lte_interval;

    /*
    ** for the paging multiframe, set the number of ticks to give nearest to a 5sec interval.
    */
    switch(pg_mfrms)    /* 2..9 */
    {
        case 0:
        case 1:
        case 2:
        default:
          lte_interval = 10;     /* 4.7 secs */
        break;

        case 3:
          lte_interval =  7;     /* 4.9  secs */
        break;

        case 4:
          lte_interval =  5;     /* 4.7 secs */
        break;

        case 5:
          lte_interval =  4;     /* 4.7  secs */
        break;

        case 6:
        case 7:
           lte_interval = 3;     /* 4.9 secs */
        break;

        case 8:
        case 9:
           lte_interval = 2;     /* 4.20 secs */
        break;

    }

    l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte = lte_interval;
    
    if(l1_sc_wcdma_data_ptr->elevator_flag)
    {
      l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte = (lte_interval+REDUCE_PERIDICITY) >> REDUCE_PERIDICITY;
    }

} /* gsm_l1_set_lte_search_cycles */

/*===========================================================================

FUNCTION L1_SC_WCDMA_SET_PAGING_MULTIFRAMES

DESCRIPTION
  This function is called to let the WCDMA search code know that
  the paging multiframes has changed. It is used to adjust the
  the counters used to initiate id and reconfirmation searches.

  This applies to 51 and 52 idle.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_set_paging_multiframes(uint16 pg_mfrms, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  /* Called in Task context */
  l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms = pg_mfrms;

  MSG_GERAN_LOW_1_G("l1_sc_wcdma_set_paging_multiframes %d", pg_mfrms);

  if ((pg_mfrms > MAX_PG_MFRMS) || (pg_mfrms < 2))
  {
    MSG_GERAN_ERROR_1_G("invalid number of multiframes %d", pg_mfrms);

    /* set to some valid value number and continue */
    if (pg_mfrms < 2)
    {
      l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms = 2;
    }
    else
    {
      l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms = MAX_PG_MFRMS;
    }
  }

  gsm_l1_set_wcdma_search_cycles( l1_sc_wcdma_data_ptr->rr_params.num_W_freqs, l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms , gas_id);

  gsm_l1_set_lte_search_cycles(l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms, gas_id);
}

/*===========================================================================

FUNCTION UPDATE_STORED_MEASUREMENTS

DESCRIPTION
  This local function is called when L1 receives a new
  MPH_WCDMA_CELL_UPDATE_LIST or MPH_DEDICATED_WCDMA_CELL_UPDATE_LIST
  message from RR, or when leaving
  dedicated mode where cell list could have been updated.

  This function compares the current and new lists:

  If the new and current lists contain the same UARFCN then
  measurements and cell parameters for that UARFCN are saved.


DEPENDENCIES
  The rr_params structure should be updated using the RR messages before
  calling this function.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  update_stored_measurements( uint8          num_new_freqs,
                                  t_stored_meas  stored_meas[MAX_WCDMA_FREQS],
                                  t_rr_params*   rr_params_ptr,
                                  gas_id_t       gas_id
                                )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16  cell_idx;
  uint16  freq_idx;

  uint16  m_freq_idx;
  uint16  m_cell_idx;

  uint16 meas_num_cells[MAX_WCDMA_FREQS];

  #ifdef FEATURE_GSM_TDS
  gsm_l1_tdd_frequency_params_type tds_para_backup[MAX_WCDMA_FREQS];

  memset(tds_para_backup, 0, sizeof(tds_para_backup));
  #endif

  gl1_irat_mutex_lock(gas_id);

  /* save off old num cells and clear the new one */

  for (freq_idx=0; freq_idx<MAX_WCDMA_FREQS; freq_idx++)
  {
    /* Save off stored_meas data to be possibly used on the new
     * cell list if there are duplicates.
     */
    meas_num_cells[freq_idx] = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;

    if (meas_num_cells[freq_idx] > MAX_SAVED_MEAS)
    {
      /* limit the amount of cells per frequency that we will save
       * old measurements data for to 16. */
      meas_num_cells[freq_idx] = MAX_SAVED_MEAS;
    }

    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = 0;
  }


  /* Ensure no array overflow for Klocwork*/
  if (l1_sc_wcdma_data_ptr->rr_params.cached_idle_freqs > MAX_WCDMA_FREQS)
  {
     l1_sc_wcdma_data_ptr->rr_params.cached_idle_freqs = MAX_WCDMA_FREQS;
  }

  /* We want to save off all the old measurement data that still applies
   * to the new list we are receiving. In order to limit the amount of
   * memory needed to do this, we limit the number of stored cell data
   * per frequency to 16 (could be as high as 32).
   */

   /*
   ** We start by copying the old cell measurement data to the last unused 16 locations in the stored_meas structure.
   ** this is to prevent cell data being overwritten if the arfcn freq_index has changed between rr_params and stored_meas
   */

  for( m_freq_idx = 0; m_freq_idx < rr_params_ptr->cached_idle_freqs; m_freq_idx++ )
  {
    for(m_cell_idx=0; m_cell_idx<meas_num_cells[m_freq_idx]; m_cell_idx++)
    {
      l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_params[m_cell_idx+MAX_SAVED_MEAS] =
        l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_params[m_cell_idx];

      l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_energy[m_cell_idx+MAX_SAVED_MEAS] =
                                              l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_energy[m_cell_idx];

      /* cell_det is only used by LTE */     
      l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_det[m_cell_idx+MAX_SAVED_MEAS]   =
                                               l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_det[m_cell_idx];
    }

#ifdef FEATURE_GSM_TDS
    tds_para_backup[m_freq_idx].last_acq_state   = stored_meas[m_freq_idx].tdd_freq_params.last_acq_state;
    tds_para_backup[m_freq_idx].last_lna_state   = stored_meas[m_freq_idx].tdd_freq_params.last_lna_state;
    tds_para_backup[m_freq_idx].measuredFreq     = stored_meas[m_freq_idx].tdd_freq_params.measuredFreq;
    tds_para_backup[m_freq_idx].num_of_jds_cells = stored_meas[m_freq_idx].tdd_freq_params.num_of_jds_cells;
    tds_para_backup[m_freq_idx].ts0_position     = stored_meas[m_freq_idx].tdd_freq_params.ts0_position;
    tds_para_backup[m_freq_idx].rssi             = stored_meas[m_freq_idx].tdd_freq_params.rssi;
#endif

  }
  MSG_GERAN_HIGH_1_G("cached_idle_freqs %d",rr_params_ptr->cached_idle_freqs);

  if(num_new_freqs > MAX_WCDMA_FREQS)   /* KW */
  {  num_new_freqs = MAX_WCDMA_FREQS;}

  /* cycle through all the old stored_meas frequencies */
  for( m_freq_idx = 0; m_freq_idx < rr_params_ptr->cached_idle_freqs; m_freq_idx++ )
  {
    /* cycle through the new list frequencies */
    for( freq_idx=0; freq_idx < num_new_freqs; freq_idx++ )
    {
      /*
       * try to find frequencies and technologies that match
       */

      if (l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].rat == rr_params_ptr->freq_list[freq_idx].rat &&
          l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cached_UARFCN == rr_params_ptr->freq_list[freq_idx].arfcn.UARFCN)
      {
        uint16 num_cells = 0;

        /* update to indicate previously measured freq */
        rr_params_ptr->freq_list[freq_idx].freq_status = G2X_FS_PREV_MEASURED;

#ifdef FEATURE_GSM_TDS
        if(RR_L1_IRAT_RAT_UTRAN_TDD == stored_meas[m_freq_idx].rat)
        {
           stored_meas[freq_idx].tdd_freq_params.last_acq_state   = tds_para_backup[m_freq_idx].last_acq_state;
           stored_meas[freq_idx].tdd_freq_params.last_lna_state   = tds_para_backup[m_freq_idx].last_lna_state;
           stored_meas[freq_idx].tdd_freq_params.measuredFreq     = tds_para_backup[m_freq_idx].measuredFreq;
           stored_meas[freq_idx].tdd_freq_params.num_of_jds_cells = tds_para_backup[m_freq_idx].num_of_jds_cells;
           stored_meas[freq_idx].tdd_freq_params.ts0_position     = tds_para_backup[m_freq_idx].ts0_position;
           stored_meas[freq_idx].tdd_freq_params.rssi             = tds_para_backup[m_freq_idx].rssi;

           MSG_GERAN_MED_3_G("l1_sc_update_list: new freq idx %d copying tdd_freq_params from old idx %d, numofjds %d",
                    freq_idx,m_freq_idx,stored_meas[freq_idx].tdd_freq_params.num_of_jds_cells);
        }
#endif
        if(rr_params_ptr->freq_list[freq_idx].rat != RR_L1_IRAT_RAT_LTE)
        {
            /*
            ** Exclude for LTE 
            ** on entry rr_params_ptr->freq_list[freq_idx].num_cells can only be non-zero for WCDMA or TDS 
            */

            if ((rr_params_ptr->freq_list[freq_idx].num_cells == 0) ||
                (rr_params_ptr->freq_list[freq_idx].rat == RR_L1_IRAT_RAT_UTRAN_TDD))
            {
              /* this is frequency only list, so save all previous measurement
                 data for this list */
              num_cells = meas_num_cells[m_freq_idx];

              for(m_cell_idx=0; m_cell_idx<num_cells; m_cell_idx++)
              {
                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[m_cell_idx] =
                                       l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_params[m_cell_idx+MAX_SAVED_MEAS];

                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[m_cell_idx] =
                                       l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_energy[m_cell_idx+MAX_SAVED_MEAS];
              }

              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = num_cells;

              /* Assumes no duplicate frequencies exist in the rr_params_ptr->freq_list[] */
              break;
            }
        } /* Exclude for LTE  */

        switch( l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].rat )
        {
        case  RR_L1_IRAT_RAT_WCDMA:

        /* check each stored_meas cell to make sure it is still in the
           new list. If it is save it, if it is not skip it. */
        for(m_cell_idx=0; m_cell_idx<meas_num_cells[m_freq_idx]; m_cell_idx++)
        {
          uint16 scr_code = l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_params[m_cell_idx+MAX_SAVED_MEAS].wcdma_cell.scr_code;

          for(cell_idx=0; cell_idx < rr_params_ptr->freq_list[freq_idx].num_cells; cell_idx++)
          {
            if( scr_code == rr_params_ptr->freq_list[freq_idx].cell_list[cell_idx].wcdma_cell.scr_code )
            {
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[num_cells] =
                                    l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_params[m_cell_idx+MAX_SAVED_MEAS];

              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[num_cells] =
                                    l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_energy[m_cell_idx+MAX_SAVED_MEAS];

              num_cells++;
              break;
            }
          }

            if (cell_idx >= rr_params_ptr->freq_list[freq_idx].num_cells)
          {
            MSG_GERAN_HIGH_2_G("Dropping WCDMA UARFCN %d scr_code %d from stored list",
                 l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cached_UARFCN ,scr_code);
          }

        } /* for( m_cell_idx = 0; m_cell_idx < meas_num_cells[m_freq_idx]; m_cell_idx++ ) */

        break;

#ifdef FEATURE_GSM_TO_LTE


        case RR_L1_IRAT_RAT_LTE:
          {
          /* 
          ** arrive here with matching rat/arfcn
          **        l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx]
          **        rr_params_ptr->freq_list[freq_idx]
          ** 
          ** unlike WCDMA where RR provides a cell list to search legacy behaviour is not
          ** approriate here, copy over any previously measured LTE cells for the same arfcn.
          ** 
          */
          num_cells =0;

          for( m_cell_idx = 0; m_cell_idx < meas_num_cells[m_freq_idx]; m_cell_idx++ )
          {
             l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[num_cells] =
                                   l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_params[m_cell_idx+MAX_SAVED_MEAS];
             /* for LTE copy to rr_params also */
             rr_params_ptr->freq_list[freq_idx].cell_list[num_cells]=
                                   l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_params[m_cell_idx+MAX_SAVED_MEAS];

             l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[num_cells] =
                                   l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_energy[m_cell_idx+MAX_SAVED_MEAS];

             l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[num_cells] =
                               l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_det[m_cell_idx+MAX_SAVED_MEAS];
             num_cells++;
          }

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = num_cells;
          rr_params_ptr->freq_list[freq_idx].num_cells          = num_cells;

          if(num_cells)
          {
             rr_params_ptr->freq_list[freq_idx].freq_status  = G2X_FS_PREV_MEASURED;
          }
          else
          {
             rr_params_ptr->freq_list[freq_idx].freq_status  = G2X_FS_NOT_MEASURED;
          }

          MSG_GERAN_HIGH_3_G("update LTE stored_meas copied %d cells for uarfcn=%d idx=%d", num_cells, 
                                                                                            rr_params_ptr->freq_list[freq_idx].arfcn.euarfcn,
                                                                                            freq_idx );
          } /* RR_L1_IRAT_RAT_LTE */
          break;
#endif /*FEATURE_GSM_TO_LTE*/


#ifdef FEATURE_GSM_TDS
        case  RR_L1_IRAT_RAT_UTRAN_TDD:

        /* check each stored_meas cell to make sure it is still in the
           new list. If it is save it, if it is not skip it. */
        for(m_cell_idx=0; m_cell_idx<meas_num_cells[m_freq_idx]; m_cell_idx++)
        {
          uint16 cell_id = l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_params[m_cell_idx+MAX_SAVED_MEAS].tds_cell.cell_parameter_id;

          for(cell_idx=0;
                cell_idx < rr_params_ptr->freq_list[freq_idx].num_cells;
              cell_idx++)
          {

              if( cell_id == rr_params_ptr->freq_list[freq_idx].cell_list[cell_idx].tds_cell.cell_parameter_id )
            {
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[num_cells] =
                l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_params[m_cell_idx+MAX_SAVED_MEAS];

              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[num_cells] =
                l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cell_energy[m_cell_idx+MAX_SAVED_MEAS];

              num_cells++;
              break;
            }
          }

            if (cell_idx >= rr_params_ptr->freq_list[freq_idx].num_cells)
          {
            MSG_GERAN_HIGH_2_G("Dropping TDS UARFCN %d cell_param_id %d from stored list",
                 l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cached_UARFCN ,cell_id);
          }

          } /* for( m_cell_idx = 0; m_cell_idx < meas_num_cells[m_freq_idx]; m_cell_idx++ ) */

        break;

#endif /*FEATURE_GSM_TDS*/
        default:

          MSG_GERAN_ERROR_1_G(" Uknown RAT %d ", l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].rat);

          break;

        } /* switch( l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].rat ) */

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = num_cells;
        break;

      } /* End of if (l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cached_UARFCN == rr_params_ptr->freq_list[freq_idx].UARFCN) */

    } /* for (freq_idx=0; freq_idx < num_new_freqs; freq_idx++) */

    if (freq_idx >= num_new_freqs)
        {
           MSG_GERAN_HIGH_2_G("Dropping UARFCN %d from stored list m_idx=%d",
                 l1_sc_wcdma_data_ptr->stored_meas[m_freq_idx].cached_UARFCN ,m_freq_idx);
        }
  } /* for (m_freq_idx = 0; m_freq_idx < rr_params_ptr->cached_idle_freqs; m_freq_idx++) */

  /* now update cached values */
  rr_params_ptr->cached_idle_freqs = num_new_freqs;

  for (freq_idx=0; freq_idx<num_new_freqs; freq_idx++)
  {
    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat           = rr_params_ptr->freq_list[freq_idx].rat;

#ifdef FEATURE_GSM_TO_LTE
    if (rr_params_ptr->freq_list[freq_idx].rat == RR_L1_IRAT_RAT_LTE)
    {
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN = rr_params_ptr->freq_list[freq_idx].arfcn.euarfcn;
      /*
      ** clear unused cell params
      */
      if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells)
      {
        MSG_GERAN_HIGH_2_G("G2L stored list freq_index=%d has existing cells num_cells=%d ", freq_idx, l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells );
      }
      else
      {
         for (cell_idx=0; cell_idx < G2L_MEAS_DET_MAX_CELLS; cell_idx++)
         {
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[cell_idx].lte_cell.cell_id = 0;
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[cell_idx].lte_cell.cp_type = 0;
         }
      }
    }
    else
#endif
    {
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN = rr_params_ptr->freq_list[freq_idx].arfcn.UARFCN;
    }
  }

  gl1_irat_mutex_unlock(gas_id);

} /* update_stored_measurements */

/*===========================================================================

FUNCTION GSM_L1_SET_ECIO_RSSI_LEVELS

DESCRIPTION
  This function is called to let the WCDMA/LTE search code set
  the search measurements filters used to determine if
  WCDMA searching should take place, and minimum EcIo levels
  to report to RR.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void  gsm_l1_set_ecio_rssi_levels( uint16 rat_idx,
                                          uint16 qsearch,
                                   uint16   ecode,
                                   gas_id_t gas_id
                                 )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  int16 offset = 0;

  /* Called in Task context */
  MSG_GERAN_LOW_2_G("gsm_l1_set_ecio_rssi_levels %d %d", qsearch, ecode);

  switch (qsearch)
  {
    /* Search will be performed if:
     *   l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16 <= serving_RLA_C <= l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16 */
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[rat_idx] = MIN_LVL_dBm_x16;
      l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[rat_idx] = (dBx16_T) ((-98 + (4 * qsearch)) * 16);
      break;

    case 7:
      /* 7 is always, so set variables accordingly */
      l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[rat_idx] = MIN_LVL_dBm_x16;
      l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[rat_idx] = 0;
      break;

    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
#ifdef FEATURE_GSM_TDS
      /**
       * Req 45: When the Qsearch_I is between 8 to 14(8 and 14 are
       * included), the signal value corresponding to Qsearch_I should
       * add -12dB( E.g. when Qsearch_I is 8, the corresponding
       * threshold value to perform Inter-RAT cell measurement is
       * -78dBm in old mechanism. But the threshold value in new
       *  mechanism is -78dBm+(-12dB)=-90dBm).
       */
      offset = l1_sc_wcdma_data_ptr->tds_update_list_irat_active ? -12 : 0;
#endif
      l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[rat_idx] = (dBx16_T) ((-78 + offset + (4 * (qsearch-8))) * 16);
      l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[rat_idx] = 0;
      break;

    case 15:
      /* 15 is default, which is never, so set variables accordingly */
      l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[rat_idx] = 0;
      l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[rat_idx] = MIN_LVL_dBm_x16;
      break;

    default:
      /* set to default, which is never, so set variables accordingly */
      l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[rat_idx] = 0;
      l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[rat_idx] = MIN_LVL_dBm_x16;
      MSG_GERAN_ERROR_3_G("invalid QSearch assignment %d", qsearch,0,0);
      break;
  }

  /* Mainlined FEATURE_NEW_FDDQMIN_CRA375 */

  /* New mapping from 05.08 CR A375 is:
   *   0=-20dB, 1=-6dB, 2=-18dB, 3=-8dB, 4=-16dB,
   *   5=-10dB, 6=-14dB, 7=-12dB. (-12dB is default).
   */
  if (ecode >= 8)
  {
    /* default is -12dB */
    MSG_GERAN_ERROR_1_G("invalid EcIo_threshold_code assignment %d", ecode);
    ecode = 7;
  }

  if (ecode & 0x01)
  {
    /* ecode is odd (1,3,5, or 7) */
    l1_sc_wcdma_data_ptr->wcdma_srch.min_wcdma_ecio = (int16) (-5 - ecode);
  }
  else
  {
    /* ecode is even (0,2,4, or 6) */
    l1_sc_wcdma_data_ptr->wcdma_srch.min_wcdma_ecio = (int16) (-20 + ecode);
  }

}

#ifdef  FEATURE_GSM_TO_LTE

/*===========================================================================

FUNCTION l1_sc_sort_priority_list_index

DESCRIPTION
  This function sorts the priority list for MPH_WCDMA_CELL_UPDATE_LIST,
  Sort the list is order of priority and generate a sorted list of indices.
  This is then used to populate the rr_params.freq_list in priority ordeer.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_sort_priority_list_index( rr_l1_irat_frequency_priority_list_t* pList , uint16 *pSorted_list, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint32 i,j,k,idx_cnt,num_prio_entries;

  uint16 curr_prio,num_msg_entries,gPriority,gPriority_cnt;

  uint16 pri_array[MAX_L1_PRIORITY_ENTRIES] = {0};       /* temp storage for sorting only */


  idx_cnt         = 0;
  num_prio_entries= 0;

  gPriority       = l1_sc_wcdma_data_ptr->rr_params.geran_priority;
  gPriority_cnt   = 0;

  num_msg_entries = pList->count;

  for(i=0 ; i < num_msg_entries; i++)
  {
     curr_prio = pList->entries[i].priority;

     if(curr_prio > gPriority)
     {
        gPriority_cnt++;   /* count the number of Freq with higher priority that geran */
     }
     /*
     ** find idx of entry of > priority in list, move rest down then add
     */
     for(j=0 ; j < num_prio_entries; j++)
     {
       if(curr_prio > pri_array[j])
       {
         break;  /* j==index of entry of lower priority entry */
       }
     }
     num_prio_entries++; /* we are adding a new entry */

     if(curr_prio > pri_array[j])
     {
        /*
        ** move down existing sorted list, start from last entry
        ** and work back so as not to overwrite wanted entry
        */

        for( k = num_prio_entries; k > j ; k--)
        {
           pSorted_list[k] = pSorted_list[k-1];
           pri_array[k]    = pri_array[k-1];
        }
        idx_cnt= j;   /* sorted index location for new entry so far */
     }
     else
     {
       idx_cnt= num_prio_entries-1;   /* no higher priority found in list so far, so add to end of list */
     }

     pSorted_list[idx_cnt]= i;            /* the index of the msg entry to sorted location */
     pri_array[idx_cnt]   = curr_prio;    /* the priority of this entry to sorted location */
  }
  MSG_GERAN_MED_3_G("G2X: Sorted Priority W+L num=%d Nbf=%d > %d geran priority",num_prio_entries, gPriority_cnt, gPriority);
}

/*===========================================================================

FUNCTION l1_sc_get_lte_tdd_count

DESCRIPTION
  returns the number of LTE tdd freqs in the eutran_frequencies list from rr

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static uint16 l1_sc_get_lte_tdd_count(rr_l1_eutran_frequency_t* pFreq, uint16 num_entries)
{
   uint32 i;
   uint16 num_tdd_entries = 0;

   if( num_entries > MAX_RR_L1_EUTRAN_FREQUENCIES )
   {   num_entries = MAX_RR_L1_EUTRAN_FREQUENCIES;}

   for( i=0; i < num_entries;i++)
   {
      if(pFreq->type == RR_L1_EUTRAN_TYPE_TDD )
      {
         num_tdd_entries++;
      }
      pFreq++;
   }
   return num_tdd_entries;
}


/*===========================================================================

FUNCTION l1_sc_get_lte_type

DESCRIPTION
  returns the LTE type for the given earfcn, FDD or TDD

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static rr_l1_eutran_type_e l1_sc_get_lte_type(rr_l1_eutran_frequency_t* pFreq, uint16 num_entries, lte_earfcn_t earfcn )
{
   uint32 i;
   rr_l1_eutran_type_e type = RR_L1_EUTRAN_TYPE_FDD;

   if( num_entries > MAX_RR_L1_EUTRAN_FREQUENCIES )
   {   num_entries = MAX_RR_L1_EUTRAN_FREQUENCIES;}

   for( i=0; i < num_entries;i++)
   {
      if(pFreq->earfcn == earfcn )
      {
         type = pFreq->type;
         break;
      }
      pFreq++;
   }
   return type;
}

#endif

#ifdef  FEATURE_GSM_TO_LTE

/*===========================================================================

FUNCTION l1_sc_add_priority_freq

DESCRIPTION
  adds the new freq rats in priority order to rr_params.freq_list, capping any excess as required

DEPENDENCIES
  none

RETURN VALUE
  Number of freqs added

SIDE EFFECTS
  None

===========================================================================*/
static uint16 l1_sc_add_priority_freq( rr_l1_irat_frequency_priority_list_t* pList, rr_l1_eutran_frequency_t *pFreq, uint16 freqCount, gas_id_t gas_id )
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    uint16  msg_num_freqs;
    uint16  msg_freq_idx;
    uint16  freq_idx = 0;
    boolean is_allowed;
    uint16  sorted_idx,W_rat_count,LFdd_rat_count,LTdd_rat_count;

    uint16  prioIdx[MAX_L1_PRIORITY_ENTRIES] = {0};

    rr_l1_irat_rat_e rat;

    freq_idx = W_rat_count = LFdd_rat_count = LTdd_rat_count = 0;

    /*
    ** priority sort the list first so only lower priorities get discarded if capped
    */
    if( pList->count > MAX_L1_PRIORITY_ENTRIES)
    {   pList->count = MAX_L1_PRIORITY_ENTRIES;  }
    /*
    ** generate an array of indices sorted in priority and use this as subsequent index into msg data
    */
    l1_sc_sort_priority_list_index( pList, prioIdx, gas_id);

    msg_num_freqs = pList->count;

    for (msg_freq_idx=0; msg_freq_idx < msg_num_freqs; msg_freq_idx++)
    {
      uint32 uarfcn;
      uint16 priority;
      /*
      ** Copy all the priority information in sorted order, checking rat count cap for each
      */
      sorted_idx = prioIdx[msg_freq_idx];
      is_allowed = FALSE;

      uarfcn     = pList->entries[ sorted_idx ].arfcn;
      rat        = pList->entries[ sorted_idx ].rat;
      priority   = pList->entries[ sorted_idx ].priority;

      /*
      ** rats WCDMA and TDS are mutually exclusive, only one type ever present per update.
      */
      if(rat==RR_L1_IRAT_RAT_WCDMA)
      {
         if(W_rat_count < l1_sc_wcdma_data_ptr->rr_params.num_W_freqs)
         {
           W_rat_count++;
           is_allowed=TRUE;
           MSG_GERAN_HIGH_3_G("G2W: UARFCN %d WCDMA prio=%d added idx=%d " , uarfcn, priority, freq_idx );
         }
      }
      else if(rat==RR_L1_IRAT_RAT_UTRAN_TDD)
      {
         if(W_rat_count < l1_sc_wcdma_data_ptr->rr_params.num_W_freqs)
         {
           W_rat_count++;
           is_allowed=TRUE;
           l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_only_data = TRUE;

           MSG_GERAN_HIGH_3_G("G2T: UARFCN %d TDS   prio=%d added idx=%d " , uarfcn, priority, freq_idx );
         }
      }
      else if (rat==RR_L1_IRAT_RAT_LTE )
      {
        /* combined total - number of Tdd present, the number of Fdd we are adding */
        uint16  limitLFdd_rat_count = l1_sc_wcdma_data_ptr->rr_params.num_L_freqs - l1_sc_wcdma_data_ptr->rr_params.num_L_tdd_freqs;

        /* we can have both fdd and tdd LTE types */
        if( RR_L1_EUTRAN_TYPE_TDD == l1_sc_get_lte_type( pFreq, freqCount, uarfcn))
        {
          if( LTdd_rat_count < l1_sc_wcdma_data_ptr->rr_params.num_L_tdd_freqs )
          {
              LTdd_rat_count++;
              is_allowed=TRUE;

              l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqList |=  (1<<freq_idx);   /* set bit for each LTE freq */

              MSG_GERAN_HIGH_3_G("G2L: EARFCN %d LTE tdd prio=%d added idx=%d " , uarfcn, priority, freq_idx );
          }
        }
        else
        {
          if( LFdd_rat_count < limitLFdd_rat_count)
          {
            LFdd_rat_count++;
            is_allowed = TRUE;
            l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqList |=  (1<<freq_idx);   /* set bit for each LTE freq */

            MSG_GERAN_HIGH_3_G("G2L: EARFCN %d LTE fdd prio=%d added idx=%d " , uarfcn, priority, freq_idx );
          }
        }
      }

      if((is_allowed) && (freq_idx < MAX_WCDMA_FREQS))   /* then add to freq list */
      {
         l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat                = rat;
         l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN       = uarfcn;
         l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].priority           = priority;
         l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells          = 0;
         l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_detected_cells = 0;
         l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age           = 0;
         
         freq_idx++;
         /* This is a fix for KW error. This for loop can run MAX_RR_L1_PRIORITY_ENTRIES(25) number
          * of times but the freq_list array has size of MAX_WCDMA_FREQS(upto 17). So if the array 
          * index reaches to MAX_WCDMA_FREQS, exit this for loop immediately. */
         if (freq_idx == MAX_WCDMA_FREQS)
         {
           break;
         }
      }
    }

    return freq_idx;   /* total number added for all rats */
} /* l1_sc_add_priority_freq */
#endif
/*===========================================================================

FUNCTION l1_sc_cap_L_plus_W_totals

DESCRIPTION
  If both L + W are present cap the totals to 6

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_cap_L_plus_W_totals(boolean tds_freqs_present,gas_id_t gas_id)
{
  uint16  msg_cap_L_freqs = 0;
  uint16  msg_cap_W_freqs = 0;
  uint8   num_tdd,num_fdd;
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

#ifdef FEATURE_GSM_TO_LTE
  if(l1_sc_wcdma_data_ptr->rr_params.num_L_freqs > MAX_RR_L1_EUTRAN_FREQUENCIES)            /* upper limit of msg input array size 25 */
  {  l1_sc_wcdma_data_ptr->rr_params.num_L_freqs = MAX_RR_L1_EUTRAN_FREQUENCIES; }

#endif

#ifdef FEATURE_GSM_TDS
  if(TRUE==tds_freqs_present)
  {
    if(l1_sc_wcdma_data_ptr->rr_params.num_W_freqs > TOTAL_TDS_FREQS)    /* upper limit of msg input array size 9 */
    {  l1_sc_wcdma_data_ptr->rr_params.num_W_freqs = TOTAL_TDS_FREQS; }

#if defined(G2L_ALLOW_MORETHAN_8)

     /* if TDS < 9  do we want to measure > 8 lte ? */
    
     if(l1_sc_wcdma_data_ptr->rr_params.num_W_freqs < TOTAL_TDS_FREQS)
     {
        msg_cap_L_freqs = TOTAL_LTE_FREQS + (TOTAL_TDS_FREQS - l1_sc_wcdma_data_ptr->rr_params.num_W_freqs);

        if(msg_cap_L_freqs > LIMIT_LTE_FREQS )          /* upper limit of num LTE to be measured, 11 */
        {  msg_cap_L_freqs = LIMIT_LTE_FREQS;}
     }
     else
#endif
     {
        msg_cap_L_freqs = TOTAL_LTE_FREQS;    /* is now 8 */
     }
     msg_cap_W_freqs = TOTAL_TDS_FREQS;       /* 9 when is TDS */
  }
  else
#endif
  {
    if(l1_sc_wcdma_data_ptr->rr_params.num_W_freqs > MAX_RR_MPH_FREQ_UPDATE_LIST)           /* upper limit of input array size 8 */
    {  l1_sc_wcdma_data_ptr->rr_params.num_W_freqs = MAX_RR_MPH_FREQ_UPDATE_LIST; }

    if( l1_sc_wcdma_data_ptr->rr_params.num_L_freqs)  /* if L is present, cap value based on num of W */
    {
      /*
      ** total W+L freqs
      */
#if defined(G2L_ALLOW_MORETHAN_8)
      if(l1_sc_wcdma_data_ptr->rr_params.num_W_freqs < TOTAL_WCDMA_FREQS )             /*  <3 */
      {
         msg_cap_L_freqs = TOTAL_LTE_FREQS + (TOTAL_WCDMA_FREQS - l1_sc_wcdma_data_ptr->rr_params.num_W_freqs);
      }
      else
#endif
      {
         msg_cap_L_freqs = TOTAL_LTE_FREQS;                      /* if we have >=3 W's , the max L is 4Fdd+4Tdd */
      }
      msg_cap_W_freqs  = TOTAL_WCDMA_FREQS;   /* with L present allow only 3 W */
    }
    else
    {
       msg_cap_W_freqs = LIMIT_WCDMA_FREQS;   /* if no L allow more W upto 6, n/a to TDS */
    }
  }

  if( l1_sc_wcdma_data_ptr->rr_params.num_L_freqs)  /* if L is present and this is > 8 , cap value based on num of W */
  {
    /*
    ** must consider both fdd and tdd counts for LTE
    */
    num_tdd =  l1_sc_wcdma_data_ptr->rr_params.num_L_tdd_freqs;
    num_fdd =  l1_sc_wcdma_data_ptr->rr_params.num_L_freqs - num_tdd;

    /*
    ** we have more than the capped value so apply limit
    */
    if(l1_sc_wcdma_data_ptr->rr_params.num_L_freqs > msg_cap_L_freqs)
    {
       while((num_fdd + num_tdd) > msg_cap_L_freqs)
       {
          if(num_tdd > TOTAL_LTE_TDD_FREQS)
          {
            num_tdd--;
          }
          if((num_fdd + num_tdd) <= msg_cap_L_freqs)
          {
             break;   /* if odd number */
          }
          if(num_fdd > TOTAL_LTE_FDD_FREQS)
          {
             num_fdd--;
          }
       } /* while */

       l1_sc_wcdma_data_ptr->rr_params.num_L_freqs     = msg_cap_L_freqs;
       l1_sc_wcdma_data_ptr->rr_params.num_L_tdd_freqs = num_tdd;        /* update as may have changed */
    }
    /* adjust W cap */
    if(FALSE == tds_freqs_present)
    {
       /* re-adjust W cap for actual number of L present */
       /* allow more W to be searched upto the W limit */
       msg_cap_W_freqs =   (TOTAL_LTE_FREQS+TOTAL_WCDMA_FREQS) - l1_sc_wcdma_data_ptr->rr_params.num_L_freqs; /* 11-n */

       if(msg_cap_W_freqs > LIMIT_WCDMA_FREQS)
       {
          msg_cap_W_freqs = LIMIT_WCDMA_FREQS;   /* upper limit for W cap is 6 */
       }
    }
  }

  if(l1_sc_wcdma_data_ptr->rr_params.num_W_freqs > msg_cap_W_freqs)   /* we have more than the capped value so limit */
  {  l1_sc_wcdma_data_ptr->rr_params.num_W_freqs = msg_cap_W_freqs; }

} /* l1_sc_cap_L_plus_W_totals */


#ifdef FEATURE_GSM_TO_LTE
/*
**  LTE_MEAS_BANDWIDTH LUT
*/

static const lte_bandwidth_e  GL1_RR_2_LTE_MEAS_BANDWIDTH[ 6 ] =
{
  /*! 1.4MHz bandwidth */
  LTE_BW_NRB_6,

  /*! 3MHz bandwidth */
  LTE_BW_NRB_15,

  /*! 5MHz bandwidth */
  LTE_BW_NRB_25,

  /*! 10MHz bandwidth */
  LTE_BW_NRB_50,

  /*! 15MHz bandwidth */
  LTE_BW_NRB_75,

  /*! 20MHz bandwidth */
  LTE_BW_NRB_100
};
#endif

/*===========================================================================

FUNCTION L1_SC_GET_WCDMA_LIST_NUM_CACHED_IDLE_FREQS

DESCRIPTION
  Return number of cached idle freqs after a list update

DEPENDENCIES
  none

RETURN VALUE
  num_cached_idle_freqs

SIDE EFFECTS
  None

===========================================================================*/
uint8 l1_sc_get_wcdma_list_num_cached_idle_freqs(gas_id_t gas_id)
{
    l1_sc_wcdma_data_t * l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
    return l1_sc_wcdma_data_ptr->rr_params.cached_idle_freqs;
}

/*===========================================================================

FUNCTION L1_SC_GET_WCDMA_LIST_UPDATE_INPROGRESS

DESCRIPTION
  This function informs if gl1 is currently updating the SC WCDMA List on receiving MPH_WCDMA_CELL_UPDATE_LIST_REQ message from RR

  Prevent any new searches from starting on this frame tick until update list has completed

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_get_wcdma_list_update_inprogress(gas_id_t gas_id)
{
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

   return l1_sc_wcdma_data_ptr->wcdma_list_update_inprogress ;

}

/*===========================================================================

FUNCTION L1_SC_SET_WCDMA_LIST_UPDATE_INPROGRESS

DESCRIPTION
  This function Set the flag to inform if gl1 is currently updating the SC WCDMA List on receiving MPH_WCDMA_CELL_UPDATE_LIST_REQ message from RR

  Prevent any new searches from starting on this frame tick until update list has completed

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_set_wcdma_list_update_inprogress(boolean update_inprogress  , gas_id_t gas_id )
{
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /*variable to track num frames to wait before processing update list in Idle DR Mode*/
   uint16 l1_sc_num_frm_wait_updt_lst = 0;

   /*
   ** holdoff update list if in GTOW_SRCH_STATE_SCHEDULE_LTE state, so wait upto 2 frames
   ** Also, wait for certain other states in DR Mode, as freq info in rr_params is used for band updates
   ** in DR Mode 
   */
   if (update_inprogress == TRUE)
   {
      GL1_ISR_LOCK(gas_id);
      l1_sc_wcdma_data_ptr->wcdma_list_update_inprogress = update_inprogress;
      GL1_ISR_UNLOCK(gas_id);

      if(l1_tskisr_blk->l1_state == L1_IDLE_MODE
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
      || (gl1_msg_get_multi_sim_mode() && (l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE))
#endif
      )
      {

        MSG_GERAN_HIGH_1_G( "update_list: received in Idle DR Mode - wcdma_srch.srch_state:%d ",
                            l1_sc_wcdma_data_ptr->wcdma_srch.srch_state);

        switch ( l1_sc_wcdma_data_ptr->wcdma_srch.srch_state )
        {
          case GTOW_SRCH_STATE_WAIT:
          case GTOW_SRCH_STATE_SCHEDULE_WCDMA:
#ifdef FEATURE_GSM_TDS
          case GTOW_SRCH_STATE_SCHEDULE_TDS:
#endif
#ifdef FEATURE_GSM_TO_LTE
          case GTOW_SRCH_STATE_SCHEDULE_LTE:
#endif
             /*Wait for 2 frames, Activity will Abort, as check for update_list 
               present in l1_sc_wcdma_tick*/          
             l1_sc_num_frm_wait_updt_lst = 2;
             while(l1_sc_num_frm_wait_updt_lst>0)
             { 
                MSG_GERAN_MED_1_G("update_list:Wait for WCDMA ID State change before updating cell list wait=%d" 
                     ,l1_sc_num_frm_wait_updt_lst );
                gl1_drx_require_and_wait_for_next_tick(gas_id);
                --l1_sc_num_frm_wait_updt_lst;
             }
             break;

          default:
            break;
        }/*switch (id state)*/
     }/*if l1_tskisr_blk and dr enabled*/ 
   }
   else
   {
      l1_sc_wcdma_data_ptr->wcdma_list_update_inprogress = update_inprogress;
   }
}

/*===========================================================================

FUNCTION L1_SC_WCDMA_UPDATE_LIST

DESCRIPTION
  This function is called when L1 receives a MPH_WCDMA_CELL_UPDATE_LIST
  message from RR.

  It is used in idle, access and packet transfer modes.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_update_list( mph_wcdma_cell_update_list_req_T* msg, gas_id_t gas_id)
{

   l1_sc_globals_T    *l1_sc_globals_ptr   = gl1_ms_switch_l1_sc_globals_store(gas_id);
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint16  msg_num_freqs;
  uint16  msg_num_cells;
  uint16  msg_freq_idx;

  uint8 new_num_freqs;

  boolean use_priority;
  boolean tds_freqs_present;

  GL1_ISR_LOCK(gas_id);

  /*
  ** clear list under mutex lock
  */
  if (l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE)
  {
    
    if ((l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_SCHEDULE_LTE) ||
        (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_ACTIVE))
    {
      l1_sc_wcdma_data_ptr->wcdma_srch.abort = TRUE;
    }
    
  }
  /* Clear this flag, so that "l1_sc_irat_check_active" is not called because of just received previous list */
  check_irat_active[gas_id] &= ~(1 << UPDATE_LIST_REQ_RECEIVED);

  new_num_freqs = l1_sc_wcdma_data_ptr->rr_params.num_freqs = msg_num_cells = 0;

  l1_sc_wcdma_data_ptr->rr_params.num_L_freqs  = 0;
  l1_sc_wcdma_data_ptr->rr_params.num_W_freqs  = 0;

  l1_sc_wcdma_data_ptr->id_search_fn = L1_GPRS_INVALID_FN;
  l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent = FALSE;
  /*
  ** stop any existing cm G2L and restart with new list
  */
  l1_sc_stop_cm_g2l(gas_id);

  /* Abort any ongoing forced measurement. */
  l1_sc_forced_meas_expiry(TRUE, gas_id);

  /*Check if id_state is WAIT or SIM_FW_WAIT*/
  if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_WAIT)
  {
    MSG_GERAN_HIGH_1_G("l1_sc_wcdma_update_list srch_state=%d",l1_sc_wcdma_data_ptr->wcdma_srch.srch_state);

    /*Clear id_state and receive_in_progress flag*/
    l1_sc_set_gtow_srch_state_null(gas_id);

    /*ALso clear FM request for all the G2X activities*/
#ifdef  FEATURE_GSM_TO_LTE
    l1_fm_clear_required_frames( FM_PRI_LTE_IDSEARCH, gas_id );
#endif
#ifdef FEATURE_GSM_TDS
    l1_fm_clear_required_frames( FM_PRI_TDS_ACQ, gas_id );
#endif
    l1_fm_clear_required_frames( FM_PRI_WCDMA_IDSEARCH, gas_id );
    l1_fm_clear_required_frames( FM_PRI_WCDMA_RECONF, gas_id );
  }
  

  for (msg_freq_idx = 0; msg_freq_idx < MAX_WCDMA_FREQS; msg_freq_idx++)
  {
    l1_sc_wcdma_data_ptr->rr_params.freq_list[msg_freq_idx].arfcn.UARFCN       = 0;
    l1_sc_wcdma_data_ptr->rr_params.freq_list[msg_freq_idx].num_cells          = 0;
    l1_sc_wcdma_data_ptr->rr_params.freq_list[msg_freq_idx].num_detected_cells = 0;
    l1_sc_wcdma_data_ptr->rr_params.freq_list[msg_freq_idx].rat                = RR_L1_IRAT_RAT_UNDEFINED;
    l1_sc_wcdma_data_ptr->rr_params.freq_list[msg_freq_idx].priority           = 0;
    l1_sc_wcdma_data_ptr->rr_params.freq_list[msg_freq_idx].freq_status        = G2X_FS_NOT_MEASURED;   /* FS */
    l1_sc_wcdma_data_ptr->rr_params.freq_list[msg_freq_idx].freq_age           = 0;
  }

#ifdef FEATURE_GSM_SEARCH_ALL_WCELL
  if ( nv_for_search_all_w_cell )
  {
    l1_sc_wcdma_data_ptr->rr_listed_wcdma_cell_not_found = FALSE;
    l1_sc_wcdma_data_ptr->number_of_expanded_wcdma_cell_scan = -1 ;
  }


#endif /*FEATURE_GSM_SEARCH_ALL_WCELL */
  /* and clear report and ignore list */
  l1_sc_wcdma_data_ptr->rr_report.num_cells       = 0;
  l1_sc_wcdma_data_ptr->rr_ignore_list.num_cells  = 0;

  if(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_NULL )
  {
   /* xfer uses ded_tick */
    l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick = 0;
    l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick       = 1;  /* start new measurement on next tick */
  }
  GL1_ISR_UNLOCK(gas_id);

  /* In packet transfer RR fills Qsearch_I with the value of
   * Qsearch_P.
   */
  l1_sc_wcdma_data_ptr->rr_params.wcdma_QSearch             = msg->WCDMA_QSearch_I;  /* 0..15,  7=Always 15=Never */
  l1_sc_wcdma_data_ptr->rr_params.wcdma_EcIo_threshold_code = msg->FDD_Qmin;

#ifdef FEATURE_GSM_TO_LTE
  l1_sc_wcdma_data_ptr->rr_params.num_L_freqs     = msg->eutran_frequency_count;   /* total FDD+TDD  */
  l1_sc_wcdma_data_ptr->rr_params.num_L_tdd_freqs = l1_sc_get_lte_tdd_count(&msg->eutran_frequencies[0],msg->eutran_frequency_count);  /* number of TDD  */

  use_priority              = msg->use_priority_reselection_algo;
  l1_sc_wcdma_data_ptr->rr_params.geran_priority  = msg->geran_priority;

#else
  l1_sc_wcdma_data_ptr->rr_params.num_L_freqs     = 0;
  l1_sc_wcdma_data_ptr->rr_params.num_L_tdd_freqs = 0;

  use_priority              = FALSE;
  l1_sc_wcdma_data_ptr->rr_params.geran_priority  = 0;

#endif
  l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqList = 0;

  /*
  ** All W freqs are now known at the outset from wcdma_reselection_freq_list
  */
  l1_sc_wcdma_data_ptr->rr_params.num_W_freqs = msg->wcdma_reselection_freq_list.num_of_frequencies;

  tds_freqs_present = FALSE;

#ifdef FEATURE_GSM_TDS
  if(l1_sc_wcdma_data_ptr->rr_params.num_W_freqs == 0)
  {
     /*
     ** only one or the other can be present , W or TDS
     */
     l1_sc_wcdma_data_ptr->rr_params.num_W_freqs = msg->utran_tdd_reselection_freq_list.num_of_frequencies;

     if(l1_sc_wcdma_data_ptr->rr_params.num_W_freqs)
     {
        tds_freqs_present= TRUE;
     }
  }
#endif


  l1_sc_wcdma_data_ptr->elevator_flag = msg->prioritize_lte_meas;


/*
** The 2 networks are mutually exclusive, in that we only ever search on W or TDS
*/

  MSG_GERAN_HIGH_4_G("G2X: update_list with freq W=%d L=%d present use priority=%d elevator_flag = %d",  l1_sc_wcdma_data_ptr->rr_params.num_W_freqs, l1_sc_wcdma_data_ptr->rr_params.num_L_freqs, use_priority,l1_sc_wcdma_data_ptr->elevator_flag);
  /*
  ** LTE TDD count now added to l1_sc_cap_L_plus_W_totals()
   */
  l1_sc_cap_L_plus_W_totals(tds_freqs_present,gas_id);

  /*
  ** CgC step 1) Add all e|uarfcns to freq_list in priority order
  **
  */

#ifdef FEATURE_GSM_TO_LTE

  if(use_priority == TRUE)
  {
     new_num_freqs = l1_sc_add_priority_freq( &msg->frequency_priority_list, msg->eutran_frequencies, msg->eutran_frequency_count, gas_id );
  }
  else  /* non priority ==> NO LTE present */
#endif
  {
     /*
     ** CgC step 1) for non-priority case all W (or T) uarfcn freq's are NOW present in wcdma_reselection_freq_list
      */
     msg_num_freqs = l1_sc_wcdma_data_ptr->rr_params.num_W_freqs;   /* use limit checked value */

#ifdef FEATURE_WCDMA

     if(!tds_freqs_present && msg_num_freqs)
     {
        uint16  freq_idx;
        uint16  uarfcn;

       MSG_GERAN_MED_1_G("G2W: WCDMA update with NO Priority, num W freqs=%d", msg_num_freqs);

       for (freq_idx=0; freq_idx < msg_num_freqs; freq_idx++)
       {
             uarfcn = msg->wcdma_reselection_freq_list.uarfcn[freq_idx];

             l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN = uarfcn;
             l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells    = 0;

             l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat          = RR_L1_IRAT_RAT_WCDMA;
             l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].priority     = 0;

             MSG_GERAN_HIGH_2_G(" UARFCN %d WCDMA added GL1 idx=%d", uarfcn,freq_idx);

       }  /* for (msg_freq_idx=0; msg_freq_idx<msg_num_freqs; msg_freq_idx++) */

       new_num_freqs = freq_idx;

     } /*  if msg_num_freqs */
     else
     {
        MSG_GERAN_MED_0_G("G2X: update list is empty");
     }
#endif

/*
** As of 25/05/12 TDS and WCDMA are mutually exclusive rats
*/
#ifdef FEATURE_GSM_TDS

  if(tds_freqs_present && msg_num_freqs)
  {
      uint16  freq_idx;
      uint16  uarfcn;

      MSG_GERAN_MED_1_G("G2X: TDS update with NO Priority, num W freqs=%d", msg_num_freqs);

      for (freq_idx=0; freq_idx < msg_num_freqs; freq_idx++)
      {
           uarfcn = msg->utran_tdd_reselection_freq_list.uarfcn[freq_idx];

           l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN = uarfcn;
           /*Since we are searching through the new re-seelction list there is no cell info*/
           l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells      = 0;
           l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat            = RR_L1_IRAT_RAT_UTRAN_TDD;
           l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_only_data = TRUE;
           l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_status    = G2X_FS_NOT_MEASURED;


           MSG_GERAN_HIGH_2_G("G2X: UARFCN %d TDS resel added GL1 idx=%d", uarfcn,freq_idx);
      }
      new_num_freqs = freq_idx;
  }
  else
  {
     MSG_GERAN_MED_0_G("G2X: TDS update list is empty");
  }

#endif

  } /* else NON priority */
  /*
  ** CgC step 2) update the LTE freqs added in step 1 in priority order with bandwidth details
  **
  */
  if(l1_sc_wcdma_data_ptr->rr_params.num_L_freqs)
  {
#ifdef FEATURE_GSM_TO_LTE
     l1_sc_add_lte_bw_freq_list( &l1_sc_wcdma_data_ptr->rr_params, &msg->eutran_frequencies[0], msg->eutran_frequency_count, new_num_freqs , gas_id );
#endif
  } /* if num_L_freqs */

/*
** CgC step 5) add the W freq cell info to only those uarfcn's already in the list
*/

#ifdef FEATURE_WCDMA

  msg_num_cells = msg->wcdma_cell_list.num_of_cells;    /* is the total for all freqs */

  if(msg_num_cells > MAX_RR_MPH_CELL_UPDATE_LIST)      /* bounds check input array size */
  {  msg_num_cells = MAX_RR_MPH_CELL_UPDATE_LIST; }

  if( !tds_freqs_present && msg_num_cells )
  {
     msg_num_cells = l1_sc_add_wcdma_freq_list( &l1_sc_wcdma_data_ptr->rr_params, &msg->wcdma_cell_list, msg_num_cells, new_num_freqs );
  } /* cell list */

#endif

#ifdef FEATURE_GSM_TDS

  msg_num_cells = msg->utran_tdd_cell_list.num_of_cells; /* is the total for all freqs */

  if(msg_num_cells > MAX_RR_MPH_CELL_UPDATE_LIST)
  {  msg_num_cells = MAX_RR_MPH_CELL_UPDATE_LIST;}

  if( tds_freqs_present && msg_num_cells )
  {
     msg_num_cells = l1_sc_add_tds_freq_list( &l1_sc_wcdma_data_ptr->rr_params, &msg->utran_tdd_cell_list, msg_num_cells, new_num_freqs );
  }

#endif

  l1_sc_wcdma_data_ptr->rr_params.num_freqs = new_num_freqs;

  /*
  ** reset to last location, so next new search is at index 0 the highest priority.
  */
  if (new_num_freqs)
  {
      l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset        = new_num_freqs-1;
      l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_in_tbf_freq_offset = new_num_freqs-1;
      l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset = new_num_freqs-1;
      l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset  = new_num_freqs-1;
  }

  /*
  ** Note THRESH_priority_search has opposite meaning to QSearch_I,
  **            15=Always                                15=never
  */
#ifdef FEATURE_GSM_TO_LTE
  if( msg->THRESH_priority_search < GSM_L1_NUM_PRIORITY_SEARCH_THRESHOLDS )
  {
     l1_sc_wcdma_data_ptr->rr_params.THRESH_priority_search     = msg->THRESH_priority_search;  /* msg enum 0..15 */
     l1_sc_wcdma_data_ptr->wcdma_srch.priority_search_threshold = gsm_l1_priority_search_threshold[ msg->THRESH_priority_search ];  /* as power in dBm's */
  }
  else
 #endif  /* FEATURE_GSM_TO_LTE */

  {
    l1_sc_wcdma_data_ptr->rr_params.THRESH_priority_search     = THRESH_ALWAYS;
    l1_sc_wcdma_data_ptr->wcdma_srch.priority_search_threshold = gsm_l1_priority_search_threshold[ THRESH_ALWAYS ];
  }

  update_stored_measurements( new_num_freqs,
                              &( l1_sc_wcdma_data_ptr->stored_meas[0] ),
                              &l1_sc_wcdma_data_ptr->rr_params,
                              msg->gas_id
                            );
  /*
   * As the transfer implementation only gets a new WCDMA
   * neighbour list from GRR when it actually changes in the P/SI
   * (cell reselection, PSI refresh, etc.) then we need to cache
   * the state of the original list on idle to transfer transitions.
   */
  l1_sc_wcdma_data_ptr->rr_params.cached_idle_freqs = new_num_freqs;

  {

    if(TRUE == tds_freqs_present)
    {
      l1_sc_wcdma_data_ptr->sc_tds_count = l1_sc_wcdma_data_ptr->rr_params.num_W_freqs;
      l1_sc_wcdma_data_ptr->sc_wcdma_count = 0;
    }
    else
    {
       l1_sc_wcdma_data_ptr->sc_wcdma_count = l1_sc_wcdma_data_ptr->rr_params.num_W_freqs;
       l1_sc_wcdma_data_ptr->sc_tds_count   = 0;
    }

    /* Set the flag to show that update list message had come and we need to activate other RATs as needed */
    check_irat_active[gas_id] |= (1 << UPDATE_LIST_REQ_RECEIVED);

    MSG_GERAN_HIGH_4_G("check_irat_active 0x%x, w_freq %d, t_freq %d, L_freq %d",check_irat_active[gas_id],
                           l1_sc_wcdma_data_ptr->sc_wcdma_count,
                           l1_sc_wcdma_data_ptr->sc_tds_count,
                           l1_sc_wcdma_data_ptr->rr_params.num_L_freqs
                         );
  }

  /*
  ** set for Idle, set both rat thresholds the same using W parameters, L threshold is not used in Idle.
  */
  l1_sc_gsm_l1_set_ecio_rssi_levels( l1_sc_wcdma_data_ptr->rr_params.wcdma_QSearch,
                                     l1_sc_wcdma_data_ptr->rr_params.wcdma_QSearch,
                                     l1_sc_wcdma_data_ptr->rr_params.wcdma_EcIo_threshold_code,
                                     gas_id);

  if(l1_sc_wcdma_data_ptr->rr_params.num_W_freqs)
  {
     gsm_l1_set_wcdma_search_cycles( l1_sc_wcdma_data_ptr->rr_params.num_W_freqs, l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms, gas_id );
  }
  gsm_l1_set_lte_search_cycles(l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms, gas_id);

  if( TRUE == use_priority)
  {
      l1_sc_wcdma_data_ptr->wcdma_srch.use_priority_reselection_algo = G2X_IDLE_PRIORITY;
  }
  else
  {
     l1_sc_wcdma_data_ptr->wcdma_srch.use_priority_reselection_algo = G2X_PRIORITY_FALSE;
  }


  for (msg_freq_idx=0; msg_freq_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs; msg_freq_idx++)
  {
      l1_sc_wcdma_data_ptr->rr_params.freq_list[msg_freq_idx].num_scr_codes = l1_sc_wcdma_data_ptr->rr_params.freq_list[msg_freq_idx].num_cells;
  }

   /* Update the rr_report structure for both WCDMA & TDS*/
   gsm_l1_update_rr_report( &( l1_sc_wcdma_data_ptr->stored_meas[0] ),
                            &l1_sc_wcdma_data_ptr->rr_ignore_list,
                            &l1_sc_wcdma_data_ptr->rr_params,
                            &l1_sc_wcdma_data_ptr->rr_report,
                            msg->gas_id
                          );
#ifdef FEATURE_GSM_TO_LTE
   gsm_l1_update_rr_lte_report( &( l1_sc_wcdma_data_ptr->stored_meas[0] ),
                                &l1_sc_wcdma_data_ptr->rr_ignore_list,
                                &l1_sc_wcdma_data_ptr->rr_params,
                                &l1_sc_wcdma_data_ptr->rr_report,
                                msg->gas_id
                              );
 
  l1_sc_g2l_fast_search_start( gas_id);          /* FR25799 */
                             
#endif

  /* Cell Id searches should be started again */

  /* @@TODO:
   *
   * This restarts both WCDMA and LTE search counters.
   *
   * This means LTE cell search will be influenced by WCDMA
   * measurement results.
   */
  l1_sc_restart_cell_id_srch( gas_id);

  l1_sc_restart_ded_cell_id_srch(gas_id);

    /* start the Femto search timer here */
  l1_isr_femto_timer_start();

  MSG_GERAN_HIGH_4_G("G2X:IRAT num freqs=%d  W=%d L=%d FS=0x%X", new_num_freqs,
                                                         l1_sc_wcdma_data_ptr->rr_params.num_W_freqs,
                                                         l1_sc_wcdma_data_ptr->rr_params.num_L_freqs,
                                                         l1_sc_wcdma_data_ptr->rr_params.fastSearchState );

} /* l1_sc_wcdma_update_list */

/*===========================================================================

FUNCTION l1_sc_add_lte_bw_freq_list

DESCRIPTION
  Add the LTE bandwidth details to freq list entry for rr_params.freq_list[]

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_GSM_TO_LTE

static void  l1_sc_add_lte_bw_freq_list( t_rr_params *rr_params_ptr, rr_l1_eutran_frequency_t  *pEutran, uint16 msg_num_freqs , uint16 new_num_freqs, gas_id_t gas_id )
{
     uint16 freq_idx,msg_freq_idx = 0;
  
     if( new_num_freqs > MAX_WCDMA_FREQS)
     {
       MSG_GERAN_ERROR_1("Invalid new_num_freqs %d", new_num_freqs);
       new_num_freqs = MAX_WCDMA_FREQS;
     }

     if(msg_num_freqs > MAX_RR_L1_EUTRAN_FREQUENCIES)           /* rr_l1.h currently 16 */
     {  msg_num_freqs = MAX_RR_L1_EUTRAN_FREQUENCIES; }
     /*
     ** Note the total allowed, rr_params.num_L_freqs, may be capped, but the number in list maybe more
     ** and the location of the priority entries is not known, so all must be searched.
     */
     for ( msg_freq_idx = 0; msg_freq_idx < msg_num_freqs; msg_freq_idx++ )   /* CgC the loop must now use un-capped */
     {
       /*
       ** Now the earfcn is already in the l1_sc_wcdma_data_ptr->rr_params.freq_list,
       ** find its location and add the other LTE bandwidth data from msg
       */
       for (freq_idx=0; freq_idx<new_num_freqs; freq_idx++)
       {
         /*
         ** The list contains both L & W in priority order, find the L freq
         */
         if(  ( pEutran->earfcn      == rr_params_ptr->freq_list[freq_idx].arfcn.euarfcn )
            &&( RR_L1_IRAT_RAT_LTE   == rr_params_ptr->freq_list[freq_idx].rat ))
         {
            break;
         }
       }

       if(freq_idx >= new_num_freqs )
       {
          /*
          ** this is valid scenario as we may have lower priority entries capped
          */
          MSG_GERAN_MED_2("G2X:earfcn %d not found in freq_list with %d entries" , pEutran->earfcn, new_num_freqs);
          continue;
       }

       /* Copy only the bandwidth information
        * The priority list is checked before eutran and hence priority per earfcn is already filled in.
        */
       if( pEutran->measurement_bandwidth < 6  )
       {
          rr_params_ptr->freq_list[freq_idx].dl_bandwidth = GL1_RR_2_LTE_MEAS_BANDWIDTH[ pEutran->measurement_bandwidth ];
       }
       else
       {
          MSG_GERAN_ERROR_1(" invalid dl_bandwidth %d ",  pEutran->measurement_bandwidth );

          rr_params_ptr->freq_list[freq_idx].dl_bandwidth = LTE_BW_NRB_6;
       }

       rr_params_ptr->freq_list[freq_idx].lte_type = pEutran->type;

       pEutran++;  /* next msg entry */

     }  /* for ( msg_freq_idx < msg_num_L_freqs ) */

} /* add LTE freq */

#endif

/*===========================================================================

FUNCTION l1_sc_add_wcdma_freq_list

DESCRIPTION
  Add the WCDMA cell list details to the rr_params.freq_list[]

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_WCDMA

static uint16  l1_sc_add_wcdma_freq_list( t_rr_params *rr_params_ptr, wcdma_cell_list_T  *pCellList, uint16 msg_num_cells , uint16 new_num_freqs )
{

   uint16  msg_cells_added = 0;
   uint16  cell_idx,curr_cell_cnt;

  if( new_num_freqs > MAX_WCDMA_FREQS)
  {
    MSG_GERAN_ERROR_1("Invalid new_num_freqs %d", new_num_freqs);
    new_num_freqs = MAX_WCDMA_FREQS;
  }

/*
** Update the UARFCNs list in L1 with the scrambling code and frequency data included in the message.
*/
  for (cell_idx=0; cell_idx<msg_num_cells; cell_idx++)
  {
    uint16 freq_idx;
    uint16 uarfcn = pCellList->cell_list[cell_idx].uarfcn;

    /*
     * Only add to those uarfcn's in the list already, priority sorted and capped.
     */
     for (freq_idx=0; freq_idx < new_num_freqs; freq_idx++)
     {
      /* The list in the message contains only WCDMA cells so there is no need
       * to add a similar check for LTE here.
       */
       if( (               uarfcn == rr_params_ptr->freq_list[freq_idx].arfcn.UARFCN)
         &&( RR_L1_IRAT_RAT_WCDMA == rr_params_ptr->freq_list[freq_idx].rat )
         )
       {
         break;
       }
     }

     if (freq_idx >= new_num_freqs)
     {
      /*
      ** this is valid scenario as we may have low priority entries capped
      */
        MSG_GERAN_MED_2("G2W: UARFCN %d not found in freq_list with %d entries", uarfcn, new_num_freqs);

        continue;
     }

    /* At this point we are guaranteed that the freq_idx we are dealing with
     * is that of a WCDMA frequency.
     */
     curr_cell_cnt = rr_params_ptr->freq_list[freq_idx].num_cells;

     if (curr_cell_cnt < MAX_WCDMA_CELLS_PER_FREQ)
     {
        rr_params_ptr->freq_list[freq_idx].cell_list[curr_cell_cnt].wcdma_cell.scr_code =
                                pCellList->cell_list[cell_idx].scrambling_code;

        rr_params_ptr->freq_list[freq_idx].cell_list[curr_cell_cnt].wcdma_cell.diversity =
                   (diversity_T)pCellList->cell_list[cell_idx].diversity;

        rr_params_ptr->freq_list[freq_idx].cell_list[curr_cell_cnt].wcdma_cell.PN_pos = 0;
        rr_params_ptr->freq_list[freq_idx].num_cells++;

        msg_cells_added++;

     }   /* End of if curr_cell_cnt < MAX_WCDMA_CELLS_PER_FREQ */
  }   /* End of for (cell_idx=0; cell_idx < msg_num_cells; cell_idx++) */

  MSG_GERAN_MED_3("WCDMA cells in=%d cells added=%d for %d freqs", msg_num_cells, msg_cells_added, new_num_freqs );

  return msg_cells_added;

}
#endif

/*===========================================================================

FUNCTION l1_sc_add_tds_freq_list

DESCRIPTION
    Add the TDS cell list details to the rr_params.freq_list[]

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_GSM_TDS
static uint16  l1_sc_add_tds_freq_list( t_rr_params *rr_params_ptr, utran_tdd_cell_list_T  *pCellList,uint16 msg_num_cells ,uint16 new_num_freqs )
{

  uint16  msg_cells_added = 0;
  uint16  cell_idx,curr_cell_cnt;

  if( new_num_freqs > MAX_WCDMA_FREQS)
  {
    MSG_GERAN_ERROR_1("Invalid new_num_freqs %d", new_num_freqs);
    new_num_freqs = MAX_WCDMA_FREQS;
  }

  for (cell_idx=0; cell_idx < msg_num_cells; cell_idx++)
  {
     uint16  freq_idx;
     uint16  uarfcn = pCellList->cell_list[cell_idx].uarfcn;

   /* This is a complete replacement of the current frequency list with
    * whatever the message contains.
    *
    * No TDD frequencies, present before the message was received,
    * will be retained.
    */
    for (freq_idx=0; freq_idx<new_num_freqs; freq_idx++)
    {
      /* The list in the message contains only WCDMA cells so there is no need
       * to add a similar check for LTE here.
        */
      if(  ( uarfcn                   == rr_params_ptr->freq_list[freq_idx].arfcn.UARFCN )
         &&( RR_L1_IRAT_RAT_UTRAN_TDD == rr_params_ptr->freq_list[freq_idx].rat )
        )
      {
        break;
      }
    }

    if (freq_idx >= new_num_freqs)
    {
      /*
      ** this is valid scenario as we may have low priority entries capped
      */
      MSG_GERAN_MED_2("G2X: UARFCN %d not found in freq_list with %d entries", uarfcn, new_num_freqs);

      continue;
    }

    curr_cell_cnt = rr_params_ptr->freq_list[freq_idx].num_cells;

    if (curr_cell_cnt < MAX_WCDMA_CELLS_PER_FREQ)
    {
        rr_params_ptr->freq_list[freq_idx].cell_list[curr_cell_cnt].tds_cell.cell_parameter_id =
                                pCellList->cell_list[cell_idx].cell_parameter_id;

        rr_params_ptr->freq_list[freq_idx].cell_list[curr_cell_cnt].tds_cell.diversity =
                                pCellList->cell_list[cell_idx].diversity;

        rr_params_ptr->freq_list[freq_idx].cell_list[curr_cell_cnt].tds_cell.sync_case =
                                pCellList->cell_list[cell_idx].sync_case;

        rr_params_ptr->freq_list[freq_idx].num_cells++;

        msg_cells_added++;
     }
  }   /* End of for (cell_idx=0; cell_idx < msg_num_cells; cell_idx++) */


  MSG_GERAN_MED_3("G2X:TDS cells in=%d cells added=%d for %d freqs", msg_num_cells, msg_cells_added, new_num_freqs );

  return msg_cells_added;

} /* l1_sc_add_tds_freq_list */
#endif


/*===========================================================================

FUNCTION L1_SC_WCDMA_IGNORE_NEIGHBORS

DESCRIPTION
  This function is called when L1 receives a MPH_IGNORE_WCDMA_NEIGHBORS_REQ
  message from RR.

  GERAN - LTE reselection

  This message needs to be updated to include LTE cells.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_ignore_neighbors( mph_ignore_wcdma_neighbors_req_T* msg )
{

  uint16 idx;
  gas_id_t gas_id = check_gas_id(msg->gas_id);
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];


  /* Called in Task context */
  MSG_GERAN_LOW_0_G("l1_sc_wcdma_ignore_neighbors");

  if (msg->ignore_flag)
  {
    /* Add to the ignore list */

    /* Check to see if WCDM/TDS cell already on list */
    for (idx=0; idx < l1_sc_wcdma_data_ptr->rr_ignore_list.num_cells; idx++)
    {
      if ( (l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].UARFCN == msg->uarfcn) &&
           ( (l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].scr_code == msg->scrambling_code)
#ifdef FEATURE_GSM_TDS
            ||(l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].cell_parameter_id == msg->cell_parameter_id)
           ) )
#else
           ) )
#endif
      {
#ifdef FEATURE_GSM_TDS
        MSG_GERAN_HIGH_3_G("Cell already on ignore list uarfcn %d: scramble_code %d  cell_param_id %d",
                  msg->uarfcn, msg->scrambling_code,msg->cell_parameter_id);
#else
        MSG_GERAN_HIGH_2_G("Cell already on ignore list %d:%d",
                  msg->uarfcn, msg->scrambling_code);
#endif
        break;
      }
    }

    if (idx >= MAX_IGNORE_CELLS)
    {
      MSG_GERAN_HIGH_1_G("Too many Cells on ignore %d", idx);
      idx = MAX_IGNORE_CELLS - 1;
    }

    l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].UARFCN = msg->uarfcn;
    l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].scr_code = msg->scrambling_code;
    l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].threshold_valid = msg->threshold_valid;
    l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].s_qualmin = (int16) msg->s_qualmin;
    l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].s_rxlevmin = (int16) msg->s_rxlevmin;
#ifdef FEATURE_GSM_TDS
    l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].cell_parameter_id = msg->cell_parameter_id;
#endif

    l1_sc_wcdma_data_ptr->rr_ignore_list.num_cells = idx + 1;
  }
  else
  {
    /* Remove from the ignore list */
    boolean found = FALSE;

    for (idx=0; idx < l1_sc_wcdma_data_ptr->rr_ignore_list.num_cells; idx++)
    {
      if (!found)
      {
        if ( (l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].UARFCN == msg->uarfcn) &&
             ( (l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].scr_code == msg->scrambling_code)
#ifdef FEATURE_GSM_TDS
            ||(l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx].cell_parameter_id == msg->cell_parameter_id)
           ) )
#else
           ) )
#endif
        {
          found = TRUE;
        }
      }

      if (found && (idx < (MAX_IGNORE_CELLS-1)))
      {
        /* shift others to front of list */
        l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx] = l1_sc_wcdma_data_ptr->rr_ignore_list.cell_list[idx+1];
      }
    }

    if ( !found)
    {
#ifdef FEATURE_GSM_TDS
      MSG_GERAN_MED_3_G("Cell not on ignore list uarfcn %d: scramble_code %d  cell_param_id %d",
                  msg->uarfcn, msg->scrambling_code,msg->cell_parameter_id);
#else
      MSG_GERAN_MED_2_G("Cell not on ignore list %d:%d",
                  msg->uarfcn, msg->scrambling_code);
#endif
    }
    else
    {
      l1_sc_wcdma_data_ptr->rr_ignore_list.num_cells--;
    }
  }  /* End of else msg->ignore_flag is FALSE */

  /* Update rr_report */
  gsm_l1_update_rr_report( &( l1_sc_wcdma_data_ptr->stored_meas[0] ),
                           &l1_sc_wcdma_data_ptr->rr_ignore_list,
                           &l1_sc_wcdma_data_ptr->rr_params,
                           &l1_sc_wcdma_data_ptr->rr_report,
                           gas_id
                         );

  /* How does it get included in the rr_params_list ? */

  /* If the ignore list is always checked before scheduling then
   * there is no need.
   */
}


/*===========================================================================

FUNCTION L1_SC_WCDMA_GET_BEST_SIX

DESCRIPTION
  This function is called when L1 is sending MPH_SURROUND_WCDMA_IDLE_MEAS_IND
  message to RR.

  This fills the message in idle mode only.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean  l1_sc_wcdma_get_best_six( best_six_wcdma_info_T*  best_six_msg_ptr, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16  rr_report_cnt = 0;
  uint16  rr_report_idx = 0;

  boolean  wcdma_cell_available = FALSE;

  best_six_msg_ptr->no_of_entries = 0;

  /* Called in Task context */

  if( ( 0 == l1_sc_wcdma_data_ptr->rr_report.num_cells )
    &&( 0 == l1_sc_wcdma_data_ptr->rr_params.num_freqs )
    )
  {
    MSG_GERAN_MED_0_G("l1_sc_wcdma_get_best_six: NO cells");

    return  FALSE;
  }

  MSG_GERAN_MED_3_G("l1_sc_wcdma_get_best_six: report uarfcn=%d num_freqs=%d num cells=%d ", l1_sc_wcdma_data_ptr->rr_report.wcdma_cell[0].freq, l1_sc_wcdma_data_ptr->rr_params.num_freqs, l1_sc_wcdma_data_ptr->rr_report.num_cells);

  while( rr_report_cnt < l1_sc_wcdma_data_ptr->rr_report.num_cells )
  {
    /* @@TODO:
     * Temporary change till the rr reporting structure is split to separate
     * WCDMA and LTE sections.
     */
    surrounding_wcdma_channel_info_T*  info_ptr = &(best_six_msg_ptr->surrounding_wcdma_channel_info[ rr_report_idx ]);

    /* Find this frequency and make sure its a wcdma UARFCN */
    uint32  i = 0;

    /* Make sure this cell is WCDMA cell */
    while( i < l1_sc_wcdma_data_ptr->rr_params.num_freqs )
    {
        if( ( l1_sc_wcdma_data_ptr->rr_report.wcdma_cell[rr_report_idx].freq == l1_sc_wcdma_data_ptr->rr_params.freq_list[i].arfcn.UARFCN ) &&
            (   ( RR_L1_IRAT_RAT_WCDMA     == l1_sc_wcdma_data_ptr->rr_params.freq_list[i].rat  )

#if defined(FEATURE_GSM_TDS)
          ||( ( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[i].rat  ) &&
              (l1_sc_wcdma_data_ptr->stored_meas[i].tdd_freq_params.num_of_jds_cells >0)
            )
#endif
            )
         )
        {
          wcdma_cell_available = TRUE;
          break;
        }
        i++;
    }

    if( wcdma_cell_available && (i < l1_sc_wcdma_data_ptr->rr_params.num_freqs) )
    {
      info_ptr->UARFCN          = l1_sc_wcdma_data_ptr->rr_report.wcdma_cell[rr_report_idx].freq;
      info_ptr->Scrambling_code = l1_sc_wcdma_data_ptr->rr_report.wcdma_cell[rr_report_idx].scr_code;

#if defined(FEATURE_GSM_TDS)
      if ( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[i].rat  )
      {
        /* use this field to represent cpid for TDS*/
        info_ptr->Scrambling_code = (word) l1_sc_wcdma_data_ptr->rr_report.wcdma_cell[rr_report_idx].cell_parameter_id;
      }
#endif
      info_ptr->RSCP = l1_sc_wcdma_data_ptr->rr_report.wcdma_cell[rr_report_idx].rscp;
      info_ptr->RSSI = l1_sc_wcdma_data_ptr->rr_report.wcdma_cell[rr_report_idx].rssi;
      info_ptr->EcNo = l1_sc_wcdma_data_ptr->rr_report.wcdma_cell[rr_report_idx].ecno;
      info_ptr->PN_Offset = l1_sc_wcdma_data_ptr->rr_report.wcdma_cell[rr_report_idx].pn_pos;

      best_six_msg_ptr->no_of_entries++;
      MSG_GERAN_HIGH_5_G("get best six infor_ptr: uarfcn %d scr_code %d rscp %d, rssi %d rr_report_idx %d",
                          info_ptr->UARFCN, info_ptr->Scrambling_code, info_ptr->RSCP, info_ptr->RSSI,rr_report_idx );

      rr_report_idx++;

      wcdma_cell_available = FALSE;   /* prevent infinite loop, check rat each iteration */
    }
    rr_report_cnt++;                  /* prevent infinite loop, count regardless of rat type */
  }  /* while( rr_report_cnt < rr_report.num_cells ) */
  /* Check that num_cells is not larger than surrounding_wcdma_channel_info[ ] */
  return  (rr_report_idx !=0);

} /* l1_sc_wcdma_get_best_six */

/*===========================================================================

FUNCTION L1_SC_DED_WCDMA_UPDATE_LIST

DESCRIPTION
  This function is called when L1 receives a
  MPH_DEDICATED_WCDMA_CELL_UPDATE_LIST message from RR.
  The list in the message should replace the L1 list - anything
  new is added, anything that L1 already has is updated and anything
  not in the list is erased from the L1 list
  @@TODO: Check with Gareth and Mungal:

  Is this list a complete replacement for the idle list including the LTE
  frequencies?

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  l1_sc_ded_wcdma_update_list( mph_dedicated_wcdma_cell_update_list_req_T* msg, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[msg->gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

#if defined(FEATURE_WCDMA) || defined(FEATURE_GSM_TDS)
  uint16  cell_idx;
  uint16  msg_num_cells;
#endif
  uint16 cell_count =0;
  uint16 freq_idx;
  uint8  num_freq_count=0;
  uint8  w_freq_count =0;
  uint8   t_freq_count   =0;
  uint8   qsearch;

  uint16  freq_limit;

  LOG_GSM_WCDMA_DED_CELL_LIST_C_type*  log_ptr;
  l1_log_wcdma_freq_info_T *freq_info_ptr;


  /* Called in Task context */
  qsearch = msg->wcdma_QSearch_C;
  l1_sc_wcdma_data_ptr->rr_params.qSearch_C[0] = qsearch;            /* set default as for W */
  l1_sc_wcdma_data_ptr->rr_params.qSearch_C[1] = qsearch;

  num_freq_count = 0;

  GL1_ISR_LOCK(gas_id);
  
  /* Clear this flag, so that "l1_sc_irat_check_active" is not called because of just received previous list */
  check_irat_active[gas_id] &= ~(1 << UPDATE_LIST_REQ_RECEIVED);

  /* set num_freqs and cell_offset fields to zero to prevent a new
   * search from being scheduled while we are updating the list */
  l1_sc_wcdma_data_ptr->rr_params.num_freqs = 0;
  l1_sc_wcdma_data_ptr->rr_params.num_W_freqs = 0;
  l1_sc_wcdma_data_ptr->rr_params.num_L_freqs = 0;

  /*
  ** stop any existing cm G2L and restart with new list
  */
  l1_sc_stop_cm_g2l(gas_id);

  for (freq_idx=0; freq_idx< MAX_WCDMA_FREQS; freq_idx++)
  {
    l1_sc_wcdma_data_ptr->wcdma_srch.next_ded_srch_cell_offset[freq_idx] = 0;
  }
  if(  (l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_UNDEFINED)
     &&(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_NULL))
  {
    l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick = 0;
    l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick       = 1;  /* start new measurement on next tick */
  }
  GL1_ISR_UNLOCK(gas_id);

  freq_idx=0;       /* reset */

#ifdef FEATURE_SGLTE_G2X_CONN_MODE_RESEL
/*
** this feature uses the same priority search functions re-worked from idle to be compatible
** with both msg types from RR for cell_update_list ded/idle
*/
  if(msg->use_priority_reselection_algo)
  {
    boolean tds_freqs_present;

    /* new feature being used */
    l1_sc_wcdma_data_ptr->rr_params.geran_priority                 = msg->geran_priority;
    l1_sc_wcdma_data_ptr->wcdma_srch.use_priority_reselection_algo = G2X_DEDICATED_PRIORITY;

    l1_sc_wcdma_data_ptr->rr_params.num_L_freqs     = msg->eutran_frequency_count;   /* total FDD+TDD  */
    l1_sc_wcdma_data_ptr->rr_params.num_L_tdd_freqs = l1_sc_get_lte_tdd_count(&msg->eutran_frequencies[0],msg->eutran_frequency_count);  /* number of TDD  */
    /*
    ** All W freqs must be known at the outset, extract count from frequency_priority_list
    */
    w_freq_count = get_msg_rat_count(&msg->frequency_priority_list, RR_L1_IRAT_RAT_WCDMA, gas_id );

    l1_sc_wcdma_data_ptr->rr_params.num_W_freqs = w_freq_count;

    tds_freqs_present = FALSE;

#ifdef FEATURE_GSM_TDS
    if(w_freq_count == 0)
    {
       /*
       ** only one or the other can be present , W or TDS
       */
       t_freq_count = get_msg_rat_count(&msg->frequency_priority_list, RR_L1_IRAT_RAT_UTRAN_TDD, gas_id );

       l1_sc_wcdma_data_ptr->rr_params.num_W_freqs = t_freq_count;

       if(t_freq_count)
       {
         tds_freqs_present= TRUE;
       }
    }
#endif

    MSG_GERAN_HIGH_2_G("G2X: ded_update_list with freq W=%d L=%d present with priority",  l1_sc_wcdma_data_ptr->rr_params.num_W_freqs, l1_sc_wcdma_data_ptr->rr_params.num_L_freqs);
    /*
    ** LTE TDD count now added to l1_sc_cap_L_plus_W_totals()
    */
    l1_sc_cap_L_plus_W_totals(tds_freqs_present,gas_id);
    /*
    ** Add all e|uarfcns to freq_list in priority order
    */
#ifdef  FEATURE_GSM_TO_LTE
    num_freq_count = l1_sc_add_priority_freq( &msg->frequency_priority_list, msg->eutran_frequencies, msg->eutran_frequency_count, gas_id );
#endif
    /*
    ** update the LTE freqs added in step 1 in priority order with bandwidth details
    */
    if(l1_sc_wcdma_data_ptr->rr_params.num_L_freqs)
    {
       l1_sc_add_lte_bw_freq_list( &l1_sc_wcdma_data_ptr->rr_params, &msg->eutran_frequencies[0], msg->eutran_frequency_count, num_freq_count );
    } /* if num_L_freqs */
/*
** add the W freq cell info to only those uarfcn's already in the list
*/
#ifdef FEATURE_WCDMA
    msg_num_cells = msg->wcdma_cell_list.num_of_cells;    /* is the total for all freqs */

    if(msg_num_cells > MAX_RR_MPH_CELL_UPDATE_LIST)      /* bounds check input array size */
    {  msg_num_cells = MAX_RR_MPH_CELL_UPDATE_LIST; }

    if( !tds_freqs_present && msg_num_cells )
    {
       msg_num_cells = l1_sc_add_wcdma_freq_list( &l1_sc_wcdma_data_ptr->rr_params, &msg->wcdma_cell_list, msg_num_cells, num_freq_count );
    } /* cell list */
#endif

#ifdef FEATURE_GSM_TDS
    msg_num_cells = msg->utran_tdd_cell_list.num_of_cells; /* is the total for all freqs */

    if(msg_num_cells > MAX_RR_MPH_CELL_UPDATE_LIST)
    {  msg_num_cells = MAX_RR_MPH_CELL_UPDATE_LIST;}

    if( tds_freqs_present && msg_num_cells )
    {
       msg_num_cells = l1_sc_add_tds_freq_list( &l1_sc_wcdma_data_ptr->rr_params, &msg->utran_tdd_cell_list, msg_num_cells, num_freq_count );
    }
#endif
  }
  else
#endif  /* FEATURE_SGLTE_G2X_CONN_MODE_RESEL  */
{
/*
** skip existing dedicated code for update lists for priority case
*/
#ifdef  FEATURE_GSM_TO_LTE
/* G2L add LTE freqs first so as then searched first, higher priority  */
{
 /*
 ** NOT using priority search
 */
  uint16 msg_num_freqs = msg->eutran_frequency_count;

  l1_sc_wcdma_data_ptr->rr_params.num_L_tdd_freqs = l1_sc_get_lte_tdd_count(&msg->eutran_frequencies[0], msg_num_freqs );  /* number of TDD type in list */

  MSG_GERAN_HIGH_3_G("G2L: ded update_list LTE num freqs=%d num_tdd=%d  qsearch_c_e=%d", msg_num_freqs,l1_sc_wcdma_data_ptr->rr_params.num_L_tdd_freqs, msg->qsearch_c_eutran );

  if(msg_num_freqs > MAX_RR_L1_EUTRAN_FREQUENCIES)           /* rr_l1.h currently 25 */
  {  msg_num_freqs = MAX_RR_L1_EUTRAN_FREQUENCIES; }

  if (msg_num_freqs)
  {
    uint16 msg_freq_idx =0;
    uint16 num_fdd_freq =0, num_tdd_freq=0;
    uint16 limitLFdd_rat_count,limitLTdd_rat_count;

    l1_sc_wcdma_data_ptr->rr_params.num_L_freqs = msg_num_freqs;

    l1_sc_cap_L_plus_W_totals(FALSE,gas_id);   /* cap totals use common function */

    /* combined total - number of Tdd present, the number of Fdd we are adding */
    limitLFdd_rat_count = l1_sc_wcdma_data_ptr->rr_params.num_L_freqs - l1_sc_wcdma_data_ptr->rr_params.num_L_tdd_freqs;
    limitLTdd_rat_count = l1_sc_wcdma_data_ptr->rr_params.num_L_tdd_freqs;

    l1_sc_wcdma_data_ptr->rr_params.qSearch_C[1]  = msg->qsearch_c_eutran;  /* 0..15,  7=Always 15=Never */

    l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqList = 0;

    for (msg_freq_idx = 0; msg_freq_idx < msg_num_freqs ;msg_freq_idx++)
    {
      if (freq_idx < TOTAL_LTE_FREQS)
      {
        lte_earfcn_t         earfcn;
        rr_l1_eutran_type_e  lte_type;
        /*
        ** we need type field now for G2L TDD mode
        */
        lte_type = msg->eutran_frequencies[ msg_freq_idx ].type;

        earfcn = msg->eutran_frequencies[ msg_freq_idx ].earfcn;

        if(lte_type == RR_L1_EUTRAN_TYPE_TDD )
        {
              if(num_tdd_freq < limitLTdd_rat_count)
              {
                num_tdd_freq++;
              }
              else
              {
                  MSG_GERAN_MED_3_G("G2L: Lte Tdd Freq limit reached not adding euarfcn=%d idx=%d", earfcn ,freq_idx,0);
                  continue;        /* add no more as limit reached */
              }
        }
        else
        {
            if(num_fdd_freq < limitLFdd_rat_count)
            {
              num_fdd_freq++;
            }
            else
            {
                MSG_GERAN_MED_3_G("G2L: Lte Fdd Freq limit reached not adding euarfcn=%d idx=%d", earfcn, freq_idx,0);
                continue;        /* add no more as limit reached */
            }
        }
        /*
        ** must come after limit checks
        */
        num_freq_count++;          /* not found, so add new freq */

        l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].lte_type           = lte_type;
        l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN       = earfcn;
        l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells          = 0;
        l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_detected_cells = 0;
        l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age           = 0;
        l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat          = RR_L1_IRAT_RAT_LTE;
        l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].priority     = 7;

        l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqList |=  (1<<freq_idx);   /* set bit for each LTE freq */

        if( msg->eutran_frequencies[ msg_freq_idx ].measurement_bandwidth < 6  )
        {
           l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].dl_bandwidth = GL1_RR_2_LTE_MEAS_BANDWIDTH[ msg->eutran_frequencies[ msg_freq_idx ].measurement_bandwidth ];
        }
        else
        {
           MSG_GERAN_ERROR_3_G(" invalid dl_bandwidth %d ",  msg->eutran_frequencies[ msg_freq_idx ].measurement_bandwidth,0,0 );
           l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].dl_bandwidth = LTE_BW_NRB_6;
        }
        l1_sc_wcdma_data_ptr->stored_meas[ freq_idx].rat                 = RR_L1_IRAT_RAT_LTE;

        MSG_GERAN_MED_3_G("G2L: new lte freq adding euarfcn=%d type=%d idx=%d", earfcn ,l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].lte_type ,freq_idx);

        freq_idx++;   /* for next entry */
      }
      else  /* if (freq_idx < TOTAL_LTE_FREQS) */
      {
          MSG_GERAN_ERROR_1_G("G2L: freq_idx %d > TOTAL_LTE_FREQS" ,freq_idx);
      }
    }
    l1_sc_wcdma_data_ptr->rr_params.num_L_tdd_freqs = num_tdd_freq;
    l1_sc_wcdma_data_ptr->rr_params.num_L_freqs     = num_freq_count;   /* total LTE FDD+TDD  */
  }
}
#endif /* FEATURE_GSM_TO_LTE      */

  l1_sc_wcdma_data_ptr->rr_params.geran_priority                 = 0;
  l1_sc_wcdma_data_ptr->wcdma_srch.use_priority_reselection_algo = G2X_PRIORITY_FALSE;
 
  /* allow upto max 6W freqs if L <=5; or else total 11(W + L) 
  ** num_freq_count represents no. of LTE frequencies added to list */
  freq_limit = num_freq_count + LIMIT_WCDMA_FREQS;

  if (freq_limit > LIMIT_LTE_FREQS)
  {freq_limit = LIMIT_LTE_FREQS;}

#ifdef FEATURE_WCDMA
  /*In order to remove the above feature def and when W is not active but TDD/L are
   * RR needs to ensure that num_of_cells is explicity 0
   */

  msg_num_cells = msg->wcdma_cell_list.num_of_cells;

  MSG_GERAN_HIGH_3_G("G2X: ded update_list numCells=%d qSearchC=%d,%d", msg_num_cells, l1_sc_wcdma_data_ptr->rr_params.qSearch_C[0],l1_sc_wcdma_data_ptr->rr_params.qSearch_C[1]);

  if(msg_num_cells > MAX_RR_MPH_CELL_UPDATE_LIST)      /* bounds check input array size */
  {  msg_num_cells = MAX_RR_MPH_CELL_UPDATE_LIST; }

      for (cell_idx=0; cell_idx<msg_num_cells; cell_idx++)
      {
        uint16 uarfcn = msg->wcdma_cell_list.cell_list[cell_idx].uarfcn;

        if(num_freq_count > MAX_WCDMA_FREQS)       /* KW */
        {  num_freq_count = MAX_WCDMA_FREQS;}

        for (freq_idx=0; freq_idx < num_freq_count; freq_idx++)
        {
          if( ( uarfcn               == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN )
            &&( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
            )
          {
            /* G2W:UARFCN %d is already in the list */
            break;
          }
        }

        /*
        ** allow existing if found or new if count not exceeded
        */
        if ((freq_idx < num_freq_count) || (num_freq_count < freq_limit))   /* prevent more than +3 W freq being added */
        {
          /* check if this is a new frequency */
          if (freq_idx == num_freq_count)
          {
            num_freq_count++;         /* this is global count */

            w_freq_count++;           /* w only count */

            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN = uarfcn;
            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells = 0;
            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat      = RR_L1_IRAT_RAT_WCDMA;
            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].priority = 0;

             MSG_GERAN_MED_3_G("G2W:UARFCN %d WCDMA freq added GL1 idx=%d cnt=%d", uarfcn, freq_idx, num_freq_count );
          }

          cell_count = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells;

          if (cell_count < MAX_WCDMA_CELLS_PER_FREQ)
          {
            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_count].wcdma_cell.scr_code =
                msg->wcdma_cell_list.cell_list[cell_idx].scrambling_code;

                /* msg->wcdma_cell_list.cell_list[cell_idx].scrambling_code >> 4; */

            if (msg->wcdma_cell_list.cell_list[cell_idx].diversity)
            {
               l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_count].wcdma_cell.diversity = DIVERSITY_ON;
            }
            else
            {
               l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_count].wcdma_cell.diversity = DIVERSITY_OFF;
            }

            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_count].wcdma_cell.PN_pos = 0;
            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells++;

            /* G2W:cell WCDMA cell added to GL1 */

          }
        } /* if (freq_idx < MAX_WCDMA_FREQS) */
        else
        {
          MSG_GERAN_HIGH_3_G("G2X: WCDMA limit reached not adding freq=%d idx=%d" ,uarfcn,freq_idx,0);
        }
      }   /* for (cell_idx=0; cell_idx < msg_num_cells; cell_idx++) */

  cell_count = 0;

  if(num_freq_count > MAX_WCDMA_FREQS)       /* KW */
  {  num_freq_count = MAX_WCDMA_FREQS;}


  l1_sc_wcdma_data_ptr->rr_params.num_W_freqs = w_freq_count;

#endif /*FEATURE_WCDMA*/

#ifdef FEATURE_GSM_TDS
  /*
  ** set limit for TDS
  */
  if(num_freq_count > MAX_WCDMA_FREQS)       /* KW */
  {  num_freq_count = MAX_WCDMA_FREQS;}

  freq_limit  = num_freq_count + TOTAL_TDS_FREQS;    /* num_freq_count has L count if present */

  if(freq_limit > MAX_WCDMA_FREQS)       /* KW */
  {  freq_limit = MAX_WCDMA_FREQS;}

  /*now check to see if there are any TDS cells in the list */
  msg_num_cells = msg->utran_tdd_cell_list.num_of_cells;

  if(msg_num_cells > MAX_RR_MPH_CELL_UPDATE_LIST)      /* bounds check input array size */
  {  msg_num_cells = MAX_RR_MPH_CELL_UPDATE_LIST; }

  if (!msg_num_cells)
  {
    MSG_GERAN_MED_1_G("G2X:No cells in TDD cell list %d ",msg_num_cells);
  }

  for (cell_idx=0; cell_idx<msg_num_cells; cell_idx++)
  {
    uint16 uarfcn = msg->utran_tdd_cell_list.cell_list[cell_idx].uarfcn;

    /*go through entire freq list & check to see if uarfcn can be found*/
    for (freq_idx=0; ( freq_idx < num_freq_count ) && (freq_idx < MAX_WCDMA_FREQS); freq_idx++)
    {
      if( ( uarfcn               == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN ) &&
          ( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat ) )
      {
        break;
      }
    }

    if (num_freq_count < freq_limit)   /* prevent more than +9 TDS freq being added */
    {
      if(freq_idx < MAX_WCDMA_FREQS)
      {
        /* check if this is a new frequency */
        if (freq_idx == num_freq_count)
        {
          t_freq_count++;

          if(num_freq_count < MAX_WCDMA_FREQS)       /* KW */
          {
             num_freq_count++;      /* not found, so add new freq */
          }
          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN = uarfcn;
          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells = 0;
          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat      = RR_L1_IRAT_RAT_UTRAN_TDD;
          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].priority = 0;
        }

        cell_count = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells;

        if (cell_count < MAX_WCDMA_CELLS_PER_FREQ)
        {
          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_count].tds_cell.cell_parameter_id =
          msg->utran_tdd_cell_list.cell_list[cell_idx].cell_parameter_id;


          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_count].tds_cell.diversity =
          msg->utran_tdd_cell_list.cell_list[cell_idx].diversity;

          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_count].tds_cell.sync_case =
          msg->utran_tdd_cell_list.cell_list[cell_idx].sync_case;

          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells++;
        }
      } /* if (freq_idx < MAX_WCDMA_FREQS) */
      else
      {
        MSG_GERAN_ERROR_1_G("Invalid freq_idx %d. Should be less than MAX_WCDMA_FREQS", freq_idx);
      }
    } /* if (num_freq_count < freq_limit) */
    else
    {
       MSG_GERAN_HIGH_3_G("G2X: TDS limit reached not adding freq=%d idx=%d" ,uarfcn,freq_idx,0);
    }
  }   /* for (cell_idx=0; cell_idx < msg_num_cells; cell_idx++) */

  if(t_freq_count)
  {
    l1_sc_wcdma_data_ptr->rr_params.num_W_freqs = t_freq_count;         /* either W or T but never both */
  }

#ifdef FEATURE_GSM_TDS_PSEUDO_FAST_RETURN
{
  uint16 msg_num_freqs = msg->utran_tdd_freq_list.num_of_frequencies;

  if(msg_num_freqs > TOTAL_TDS_FREQS)           /* check bounds */
  {  msg_num_freqs = TOTAL_TDS_FREQS; }

  /*We only expect a Freq list OR a cell list - not both CgC but no check ??*/
  if (msg_num_freqs)
  {
    uint16 num_of_freq =0;

    if(num_freq_count > MAX_WCDMA_FREQS)       /* KW */
    {  num_freq_count = MAX_WCDMA_FREQS;}

   MSG_GERAN_MED_3_G("G2T:Freq only list present - num of freq: %d uarfcn[0] %d, uarfcn[1] %d",
            msg->utran_tdd_freq_list.num_of_frequencies,
            msg->utran_tdd_freq_list.uarfcn[0],
            msg->utran_tdd_freq_list.uarfcn[1]);

    for (num_of_freq = 0; num_of_freq < msg_num_freqs ;num_of_freq++)
    {
      uint16 uarfcn = msg->utran_tdd_freq_list.uarfcn[num_of_freq];

      for (freq_idx=0; (freq_idx < num_freq_count) && (freq_idx < MAX_WCDMA_FREQS); freq_idx++)
      {
        if( ( uarfcn               == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN ) &&
            ( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat ) )
        {
          break;
        }
      }

      if (num_freq_count < freq_limit)                  /* prevent more than +6 T freq being added */
      {
        /* check if this is a new frequency */
        if (freq_idx == num_freq_count)
        {
          num_freq_count++; /* not found, so add new freq */
          t_freq_count++;

          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN = uarfcn;
          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells = 0;
          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat      = RR_L1_IRAT_RAT_UTRAN_TDD;
          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].priority = 0;
        }

      } /* if (freq_idx < MAX_WCDMA_FREQS) */
      else
      {
         MSG_GERAN_HIGH_3_G("G2X: TDS limit reached not adding freq=%d idx=%d" ,uarfcn,freq_idx,0);
      }
    }
    l1_sc_wcdma_data_ptr->rr_params.num_W_freqs = t_freq_count;         /* either W or T but never both */
  }
}
#endif /*FEATURE_GSM_TDS_PSEUDO_FAST_RETURN*/
#endif /*FEATURE_GSM_TDS*/

 /*Count total no of TDSCDMA/WCDMA cells*/
  for (freq_idx=0; freq_idx<num_freq_count; freq_idx++)
  {
    if(( RR_L1_IRAT_RAT_LTE != l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat ) &&
        ( RR_L1_IRAT_RAT_UNDEFINED != l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat ))
    {
      cell_count += l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells;
    }
  }

  MSG_GERAN_HIGH_3_G("G2X log packet freq_count %d cell_count %d" ,num_freq_count,cell_count,0);
  
  /*Commit log packet 0x525A for WCDMA/TD-SCDMA cells*/
  if ( NULL != (log_ptr = (LOG_GSM_WCDMA_DED_CELL_LIST_C_type *)log_alloc( LOG_GSM_WCDMA_DED_CELL_LIST_C,
        LOG_GSM_WCDMA_DED_CELL_LIST_C_LEN(num_freq_count, cell_count) )) )
  {
    /* Fill packet and commit it */
    log_ptr->qSearch_C   = qsearch;
    log_ptr->search_Prio = TRUE;
    log_ptr->num_freqs = num_freq_count;

    /*Don't access freq info ptr if freq count is 0*/
    if(num_freq_count)
    {
      freq_info_ptr = (l1_log_wcdma_freq_info_T *) &(log_ptr->freq_info[0]);
    }

    /*log both WCDMA and TD-SCDMA cells*/
    for( freq_idx = 0; freq_idx < num_freq_count; freq_idx++ )
    {

#ifdef FEATURE_WCDMA
      if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
      {
      uint8 *byte_ptr;

      cell_count = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells;

      freq_info_ptr->UARFCN    = (uint16)l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN,
      freq_info_ptr->num_cells = (uint8) cell_count;

      for (cell_idx=0; cell_idx<cell_count; cell_idx++)
      {
        freq_info_ptr->cell[cell_idx].scr_code =
            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_idx].wcdma_cell.scr_code;

        freq_info_ptr->cell[cell_idx].diversity =
            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_idx].wcdma_cell.diversity;   /*lint !e641 */
      }

      byte_ptr = (uint8 *) &(freq_info_ptr->cell[cell_count]);
      freq_info_ptr = (l1_log_wcdma_freq_info_T *) byte_ptr;

      } /* if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat ) */
#endif
      
#ifdef FEATURE_GSM_TDS
      if( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
      {
        uint8 *byte_ptr;

        cell_count = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells;

        freq_info_ptr->UARFCN  = (uint16)l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN,
        freq_info_ptr->num_cells = (uint8) cell_count;

        for (cell_idx=0; cell_idx<cell_count; cell_idx++)
        {
          freq_info_ptr->cell[cell_idx].scr_code =
           l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_idx].tds_cell.cell_parameter_id;

          freq_info_ptr->cell[cell_idx].diversity =
           l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_idx].tds_cell.diversity; /*lint !e641 */
        }

        byte_ptr = (uint8 *) &(freq_info_ptr->cell[cell_count]);
        freq_info_ptr = (l1_log_wcdma_freq_info_T *) byte_ptr;

      } /* if( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat ) */
#endif

    }

    log_commit((PACKED void *)log_ptr);
  }

} /*  FEATURE_SGLTE_G2X_CONN_MODE_RESEL else NOT using priority */

  if(!(l1_sc_globals_ptr->wcdma_srch_in_progress) && num_freq_count)
  {
      /*
      ** This ensures the first ded search is the first in new list, LTE being highest priority
      */
      l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset = num_freq_count-1;
  }

  l1_sc_wcdma_data_ptr->rr_params.num_freqs = num_freq_count;   /* Now L + T | W only one or the other, never both W & T */

  MSG_GERAN_HIGH_3_G("G2X:IRAT Dedicated  num freqs=%d  W=%d L=%d", num_freq_count, l1_sc_wcdma_data_ptr->rr_params.num_W_freqs, l1_sc_wcdma_data_ptr->rr_params.num_L_freqs );

  update_stored_measurements( l1_sc_wcdma_data_ptr->rr_params.num_freqs,
                              &( l1_sc_wcdma_data_ptr->stored_meas[0] ),
                              &l1_sc_wcdma_data_ptr->rr_params,
                              gas_id
                            );
  
  if(t_freq_count)
  {
    l1_sc_wcdma_data_ptr->sc_tds_count = t_freq_count;
    l1_sc_wcdma_data_ptr->sc_wcdma_count = 0;
  }
  else if (w_freq_count)
  {
    l1_sc_wcdma_data_ptr->sc_wcdma_count = w_freq_count;
    l1_sc_wcdma_data_ptr->sc_tds_count   = 0;
  }
  /* Set the flag to show that update list message had come and we need to activate other RATs as needed */
  check_irat_active[gas_id] |= (1 << UPDATE_LIST_REQ_RECEIVED);
  
  MSG_GERAN_HIGH_4_G("check_irat_active 0x%x, w_freq %d, t_freq %d, L_freq %d",check_irat_active[gas_id],
                           l1_sc_wcdma_data_ptr->sc_wcdma_count,
                           l1_sc_wcdma_data_ptr->sc_tds_count,
                           l1_sc_wcdma_data_ptr->rr_params.num_L_freqs
                         );	  

  /* minimum Ec/Io is constant in dedicated mode */
  /* EcIo_threshold_code = 0; -20 dB in dedicated */
  /*
  ** For CM this is now rat dependant as we have different qsearch_C values for both W and L freqs
  */
  l1_sc_gsm_l1_set_ecio_rssi_levels( l1_sc_wcdma_data_ptr->rr_params.qSearch_C[0], l1_sc_wcdma_data_ptr->rr_params.qSearch_C[1], 0, msg->gas_id);

  for (freq_idx=0; freq_idx<l1_sc_wcdma_data_ptr->rr_params.num_freqs; freq_idx++)
  {
      l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_scr_codes = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells;
  }

  /* Cell Id searches should be started again */
  l1_sc_restart_ded_cell_id_srch(msg->gas_id);

#ifdef  FEATURE_GSM_TO_LTE
  l1_sc_g2l_fast_search_start( gas_id);          /* FR25799 update fast search */
#endif

} /* l1_sc_ded_wcdma_update_list */


/*===========================================================================

FUNCTION update_stored_measurements_from_ded_reconf

DESCRIPTION
  This local function is called after receiving reconfirmed cell info
  from the driver while in dedicated mode.  This cell info is the latest
  info available so the stored_meas database should be updated .
  
  Only used for WCDMA results.
   
DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void update_stored_measurements_from_ded_reconf( gl1_wcdma_cell_ded_reconf_rpt *rpt,
                                                        uint16                    freq_idx,
                                                        gas_id_t                    gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16 cell_idx,i;
  uint16 num_cells,num_rpt_cells;

  /*int16  rscp_last;*/
  int16  ecno_last;
  int16  rscp_new,  ecno_new;

  uint16 new_idx   = 0;
  uint16 min_idx   = 0;
  uint16 num_valid = 0;
  int16  min_avg   = 0;


  num_rpt_cells = rpt->num_cell_measurements;

  if( ( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN != rpt->UARFCN )    ||
      ( RR_L1_IRAT_RAT_LTE == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
#ifdef FEATURE_GSM_TDS
    ||(RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
#endif
    )
  {
    /* wcdma cell list must have changed, so just ignore this result */
    MSG_GERAN_HIGH_1_G("Unknown WCDMA meas rpt: ch:%d!", rpt->UARFCN);

    return;
  }

  if( ( l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN != rpt->UARFCN )
    ||(  RR_L1_IRAT_RAT_LTE == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat )
#ifdef FEATURE_GSM_TDS
    ||(RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
#endif
    )
  {
    /* see if UARFCN is in local db */
    MSG_GERAN_ERROR_1_G("Reconfirmed UARFCN %d not in local db.", rpt->UARFCN);

    return;
  }

  if ( num_rpt_cells > MAX_WCDMA_CELLS_PER_DED_RECONF )
  {
    MSG_GERAN_ERROR_2_G( "Too many WCDMA cells in rpt:%d clamping to max %d!",
               num_rpt_cells, MAX_WCDMA_CELLS_PER_DED_RECONF);

    num_rpt_cells = MAX_WCDMA_CELLS_PER_DED_RECONF;
  }

  /*----- first step: add report data to db -------*/

  /* number of cells in stored db for this freq */
  num_cells = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;

  /* now locate the particular cell */
  for (cell_idx=0; cell_idx<num_cells; cell_idx++)
  {
    boolean found = FALSE;

    for (i=0; i<num_rpt_cells; i++)
    {
      if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[cell_idx].wcdma_cell.scr_code == rpt->meas[i].scr_code)
      {
        /* indicate that a cell is found in db */
        found = TRUE;

        /* update the db with new pn position */
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[cell_idx].wcdma_cell.PN_pos = rpt->meas[i].PN_pos;

        /* update the db with new rscp values */
        /*rscp_last = stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_last*/
        rscp_new  = rpt->meas[i].RSCP;

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_last = rscp_new;

        /* update the db with new ecno values */
        ecno_last = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.EcNo_last;
        ecno_new  = rpt->meas[i].EcNo;

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.EcNo_last = ecno_new;

        if (!ecno_last)
        {
          ecno_last = ecno_new;   /* copy new if 0 */
        }

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.EcNo_avg = 
                                                        l1_sc_g2w_get_ecio_logarithmic_average( ecno_new, ecno_last );

       /* update the avg regardless */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_avg =
            ( rpt->meas[i].rssi + (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.EcNo_avg/2) );

        /* Update RSSI */
        l1_sc_g2x_filter_treatment(NULL, rpt->meas[i].rssi, freq_idx, cell_idx, RR_L1_IRAT_RAT_WCDMA, gas_id);


        if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.EcNo_avg <= MIN_EcNo_TO_BE_VALID_CELL)
        {
          /* Need to stop making searches on cells that no longer
           * are relevant (due to extremely low power), otherwise we
           * never stop searching. Mark for deletion by setting found
           * to FALSE */
           MSG_GERAN_HIGH_2_G("Stop reconfirming wcdma cell:low EcNo UARFCN=%d, scr_code=%d, Restart ID search ",
                                                   l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN,
                                                   l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[cell_idx].wcdma_cell.scr_code );

          l1_sc_restart_ded_specific_cell_id_srch(freq_idx, gas_id);

          found = FALSE;
        }
      }
    } /* end for */

    if (!found)
    {
      /****************************
       This is the problem:
       say Id search returned 5 cells, and after that we did reconfirmation search and it resutned 3 cells
       then two cells from Id search will be deleted
       This can ahppen when we did ID search on new cells and reconf search on already indentified cells
      ********************************/

      /*And check to see if we requested this in reconfirmation, if so then only mark for deletion*/
      uint16 count_i=0;
      if(l1_sc_wcdma_data_ptr->requested_reconf_meas.cached_UARFCN == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN)
      {
          for(count_i=0; count_i<l1_sc_wcdma_data_ptr->requested_reconf_meas.num_cells; count_i++)
          {
             if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[cell_idx].wcdma_cell.scr_code == l1_sc_wcdma_data_ptr->requested_reconf_meas.scr_code[count_i])
             {
                 /* No current measurement, so mark for deletion */
                 l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_last = 0;
                 /* set this deleted cell RSCP average to zero, so its not included
                    in the strongest 8 filter that follows */
                 l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_avg = 0;

                 /* Avoid partial measurement filling */
                 l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.EcNo_avg  = 0;
                 l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.EcNo_last = 0;

                 l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSSI_avg  = 0;
                 l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSSI_last = 0;
             }
          }
      }
    }
  }


  /*----- Second step: Of the cells with valid avgs, mark for deletion all but top 8 ------*/

  for (cell_idx=0; cell_idx<num_cells; cell_idx++)
  {
    int16  curr_avg = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_avg;

    if (curr_avg != 0)
    {
      if (num_valid < 8)
      {
        num_valid++;

        if (curr_avg < min_avg)
        {
          min_avg = curr_avg;
          min_idx = cell_idx;
        }
      }
      else
      {
        if (curr_avg < min_avg)
        {
          /* mark this cell for deletion (set RSCP_last = 0) */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_last = 0;

          /* remove this cell from further consideration */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_avg = 0;

          /* Avoid partial measurement filling */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.EcNo_avg  = 0;
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.EcNo_last = 0;

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSSI_avg  = 0;
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSSI_last = 0;

        }
        else
        {
          uint16 tmp_idx;

          /* mark the min cell for deletion (set RSCP_last = 0) */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_last = 0;

          /* remove min cell from further consideration */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_avg = 0;

          /* Avoid partial measurement filling */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.EcNo_avg  = 0;
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.EcNo_last = 0;

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSSI_avg  = 0;
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSSI_last = 0;

          /* go find new min cell */
          min_avg = 0;

          for (tmp_idx = 0; tmp_idx <= cell_idx; tmp_idx++)
          {
            int16 tmp_avg = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[tmp_idx].wcdma_cell.RSCP_avg;

            if ((tmp_avg != 0) && (tmp_avg < min_avg))
            {
              min_avg = tmp_avg;
              min_idx = tmp_idx;
            }
          } /* end for */
        }
      }
    }
  } /* end for */

  /*---- Third step: Delete any items in our db marked for deletion -----*/

  for (cell_idx=0, new_idx=0; cell_idx<num_cells; cell_idx++)
  {
    if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_last != 0)
    {
      if (new_idx != cell_idx)
      {
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx] =
                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[cell_idx];

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx] =
                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx];
      }
      new_idx++;
    }
  }

  /* update the new qty in the db */
  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = new_idx;

  if (new_idx != cell_idx)
  {
    /* set these 2 params to zero (to cleanup, not necessary) */
    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_last = 0;
    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx].wcdma_cell.scr_code  = 0;
  }

} /* update_stored_measurements_from_ded_reconf */

/*===========================================================================

FUNCTION update_stored_measurements_for_top_8

DESCRIPTION
  This local function is called to clean the stored_measurement db and keep
  only the top 8 strongest cells. All others are deleted.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void update_stored_measurements_for_top_8( uint16 freq_idx, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16 cell_idx;
  uint16 num_cells;

  uint16 new_idx   = 0;
  uint16 min_idx   = 0;
  uint16 num_valid = 0;


  /* Ensure we don't over index stored_meas buffer */
  if(freq_idx >=MAX_WCDMA_FREQS)
  {
    freq_idx = (MAX_WCDMA_FREQS -1);

    MSG_GERAN_ERROR_1_G("warning freq_idx larger than buffer!! %d",freq_idx);
  }

  /* number of cells in stored db for this freq */
  num_cells = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;

  if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
  {
    int16   min_rscp  = 0;

  /*----- First step: Of the cells with valid measurements, mark for deletion all but top 8 ------*/
  for (cell_idx=0; cell_idx<num_cells; cell_idx++)
  {
    int16  curr_rscp;

      curr_rscp = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_last;

    if (curr_rscp != 0)
    {
      if (num_valid < 8)
      {
        num_valid++;
        if (curr_rscp < min_rscp)
        {
          min_rscp = curr_rscp;
          min_idx = cell_idx;
        }
      }
      else
      {
        if (curr_rscp < min_rscp)
        {
          /* mark this cell for deletion (set RSCP_last = 0) */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_last = 0;

          /* remove this cell from further consideration */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_avg = 0;
        }
        else
        {
          uint16 tmp_idx;

          /* mark the min cell for deletion (set RSCP_last = 0) */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_last = 0;

          /* remove min cell from further consideration */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_avg = 0;

          /* go find new min cell */
          min_rscp = 0;

          for (tmp_idx = 0; tmp_idx <= cell_idx; tmp_idx++)
          {
              int16 tmp_rscp = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[tmp_idx].wcdma_cell.RSCP_last;

            if ((tmp_rscp != 0) && (tmp_rscp < min_rscp))
            {
              min_rscp = tmp_rscp;
              min_idx = tmp_idx;
            }
          } /* end for */
        }
      }
    }
    } /* End of for( cell_idx = 0; cell_idx < num_cells; cell_idx++ ) */

  /*---- Second step: Shift all the good cells to the front of the db  -----*/

  for (cell_idx=0, new_idx=0; cell_idx<num_cells; cell_idx++)
  {
      if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_last != 0)
    {
      if (new_idx != cell_idx)
      {
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx] =
                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[cell_idx];

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx] =
                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx];
      }
      new_idx++;
    }
  }
  /* update the new qty in the db */
  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = new_idx;

  if (new_idx != cell_idx)
  {
    /* set these 2 params to zero (to cleanup, not necessary) */
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_last = 0;

      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx].wcdma_cell.scr_code = 0;
    }

  } /* End of if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat ) */
#ifdef FEATURE_GSM_TDS
  else if( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
  {
    int16   min_rscp  = MIN_TDD_CELL_RSCP_VALUE;
    gsm_l1_lte_wcdma_cell_params_type       temp_cell_params = {{0, 0, 0}};
    t_gsm_l1_lte_wcdma_cell_energy_details  temp_cell_energy = {{0, 0}};

  /*----- First step: Of the cells with valid measurements, mark for deletion all but top 8 ------*/
  for (cell_idx=0; cell_idx<num_cells; cell_idx++)
  {
    int16  curr_rscp;

      curr_rscp = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_last;

    if (curr_rscp != MIN_TDD_CELL_RSCP_VALUE)
    {
      if (num_valid < 8)
      {
        num_valid++;
        if (curr_rscp < min_rscp)
        {
          min_rscp = curr_rscp;
          min_idx = cell_idx;
        }
      }
      else
      {
        if (curr_rscp < min_rscp)
        {
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_last2= MIN_TDD_CELL_RSCP_VALUE;

          /* remove this cell from further consideration */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;
        }
        else
        {
          uint16 tmp_idx;

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_last2= MIN_TDD_CELL_RSCP_VALUE;

          /* remove min cell from further consideration */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;

          /* go find new min cell */
          min_rscp = curr_rscp;

          for (tmp_idx = 0; tmp_idx <= cell_idx; tmp_idx++)
          {
              int16 tmp_rscp = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[tmp_idx].wcdma_cell.RSCP_last;

            if ((tmp_rscp != MIN_TDD_CELL_RSCP_VALUE) && (tmp_rscp < min_rscp))
            {
              min_rscp = tmp_rscp;
              min_idx = tmp_idx;
            }
          } /* end for */
        }
      }
    }
    } /* End of for( cell_idx = 0; cell_idx < num_cells; cell_idx++ ) */

  /*---- Second step: Shift all the good cells to the front of the db  -----*/

  for (cell_idx=0, new_idx=0; cell_idx<num_cells; cell_idx++)
  {
    if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].wcdma_cell.RSCP_last >= MIN_TDD_CELL_RSCP_VALUE)
    {
      if (new_idx != cell_idx)
      {
        temp_cell_params = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx];

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx] =
                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[cell_idx];

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[cell_idx] = temp_cell_params;

        temp_cell_energy = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx];

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx] =
                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx];

    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx] = temp_cell_energy;

      }
      new_idx++;
    }
  }

  if (new_idx != cell_idx)
  {
    while(new_idx < num_cells)
    {
      /* set these 2 params to zero (to cleanup, not necessary) */
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_last2= MIN_TDD_CELL_RSCP_VALUE;
      new_idx++;
    }
  }
  } /* End of if( RR_L1_IRAT_RAT_UTRAN_TDD == rr_params.freq_list[freq_idx].rat ) */
#endif /*FEATURE_GSM_TDS*/
#ifdef  FEATURE_GSM_TO_LTE

/* G2L add explicit rat check */
  else if( RR_L1_IRAT_RAT_LTE == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
  {
    int16  min_rsrp  = 0;

    /* This is an LTE arfcn */
    /*----- First step: Of the cells with valid measurements, mark for deletion all but top 8 ------*/
    for( cell_idx = 0; cell_idx < num_cells; cell_idx++ )
    {
      int16  curr_rsrp = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].lte_cell.rsrp_last;

      if (curr_rsrp != 0)
      {
        if (num_valid < 8)
        {
          num_valid++;

          if (curr_rsrp < min_rsrp)
          {
            min_rsrp = curr_rsrp;
            min_idx = cell_idx;
          }
        }
        else
        {
          if (curr_rsrp < min_rsrp)
          {
            /* mark this cell for deletion (set RSCP_last = 0) */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].lte_cell.rsrp_last = 0;

            /* remove this cell from further consideration */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].lte_cell.rsrp_avg = 0;
          }
          else
          {
            uint16 tmp_idx;

            /* mark the min cell for deletion (set RSCP_last = 0) */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].lte_cell.rsrp_last = 0;

            /* remove min cell from further consideration */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].lte_cell.rsrp_avg = 0;

            /* go find new min cell */
            min_rsrp = 0;

            for (tmp_idx = 0; tmp_idx <= cell_idx; tmp_idx++)
            {
              int16 tmp_rsrp = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[tmp_idx].lte_cell.rsrp_last;

              if ((tmp_rsrp != 0) && (tmp_rsrp < min_rsrp))
              {
                min_rsrp = tmp_rsrp;
                min_idx = tmp_idx;
              }
            } /* end for */
          }
  }
      }
    } /* End of for( cell_idx = 0; cell_idx < num_cells; cell_idx++ ) */

    /*---- Second step: Shift all the good cells to the front of the db  -----*/

    for (cell_idx=0, new_idx=0; cell_idx<num_cells; cell_idx++)
    {
      if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx].lte_cell.rsrp_last != 0)
      {
        if (new_idx != cell_idx)
        {
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx] = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[cell_idx];
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx] = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[cell_idx];
        }
        new_idx++;
      }
    }
    /* update the new qty in the db */
    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = new_idx;

    if (new_idx != cell_idx)
    {
      /* set these 2 params to zero (to cleanup, not necessary) */
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].lte_cell.rsrp_last = 0;
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx].lte_cell.cell_id   = 0;
    }
  } /* End of else this is an LTE arfcn */
#endif /* FEATURE_GSM_TO_LTE */

} /* update_stored_measurements_for_top_8 */

/*===========================================================================

FUNCTION get_stored_meas_cell_entry

DESCRIPTION
  Returns the stored measurement databases cell_params and cell_energy
  that correspond to the UARFCN and SCR code passed in. If no entry is
  found to match then the boolean returns FALSE, otherwise TRUE.
  Data for the entry is passed back using the pointers cell_params
  and cell_energy, providing they are not NULL

DEPENDENCIES
  none

RETURN VALUE
  boolean - if cell was found TRUE else FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean get_stored_meas_cell_entry(uint32 UARFCN,
                                   uint16 scr_code,
                                   uint8  rat,
                                   gsm_l1_lte_wcdma_cell_params_type**       cell_params,
                                   t_gsm_l1_lte_wcdma_cell_energy_details**  cell_energy,
                                   gas_id_t gas_id
                                  )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  boolean entry_found = FALSE;
  boolean freq_found = FALSE;
  uint16 meas_idx, arfcn_idx;

  for ( arfcn_idx=0;arfcn_idx<MAX_WCDMA_FREQS;arfcn_idx++)
  {
    if(  ( l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cached_UARFCN == UARFCN )
       &&( rat == l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].rat )
      )
    {
      freq_found = TRUE;
      break;
    }
  }

  /* Keep LINT happy */
  if (arfcn_idx >= MAX_WCDMA_FREQS)
  {
    arfcn_idx = ( MAX_WCDMA_FREQS - 1 );
  }

  if ( !freq_found )
  {
    MSG_GERAN_ERROR_2_G("Cannot find stored meas freq UARFCN %d RAT %d",UARFCN,rat);
    return entry_found;
  }


  for ( meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].num_cells; meas_idx++ )
  {
    if (rat == RR_L1_IRAT_RAT_WCDMA)
    {
      if(  ( scr_code == l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].wcdma_cell.scr_code )
         &&( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].rat )
        )
      {
        if ( cell_params != NULL )
        {
          *cell_params = &l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx];
        }

        if ( cell_energy != NULL )
        {
          *cell_energy = &l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx];
        }

        entry_found = TRUE;
        break;
      }
    }
#ifdef FEATURE_GSM_TDS
    else if (rat == RR_L1_IRAT_RAT_UTRAN_TDD)
    {
      if(  ( scr_code == l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].tds_cell.cell_parameter_id )
         &&( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].rat )
        )
      {
        if ( cell_params != NULL )
        {
          *cell_params = &l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx];
        }

        if ( cell_energy != NULL )
        {
          *cell_energy = &l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx];
        }

        entry_found = TRUE;
        break;
      }
    }
#endif
  }

  return entry_found;
}

/*===========================================================================

FUNCTION remove_stored_meas_cell_entry

DESCRIPTION
  Deletes the entry in the stored meas database that matches the UARFCN
  and the SCR code passed in.
    This function is also used for doing the same thing for TDS cells

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void remove_stored_meas_cell_entry( uint32   UARFCN,
                                    uint16   scr_code,
                                    uint8    rat,
                                    gas_id_t gas_id
                                  )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16 meas_idx, arfcn_idx;

  boolean freq_found = FALSE;
  boolean cell_found = FALSE;

  for ( arfcn_idx=0;arfcn_idx<MAX_WCDMA_FREQS;arfcn_idx++)
  {
    if (rat == (uint8) l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].rat)
    {
      if (rat == RR_L1_IRAT_RAT_LTE)
      {
        if (l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cached_UARFCN == UARFCN)
        {
          freq_found = TRUE;
          break;
        }
      }
      else
      {
        if ((uint32)l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cached_UARFCN == UARFCN)
        {
          freq_found = TRUE;
          break;
        }
      }
    }
  }

  /* Keep LINT happy */
  if (arfcn_idx >= MAX_WCDMA_FREQS)
  {
    arfcn_idx = ( MAX_WCDMA_FREQS - 1 );
  }

  if ( !freq_found )
  {
    MSG_GERAN_ERROR_2_G("Cannot remove from stored meas UARFCN%d RAT %d",UARFCN,rat);
    return;
  }


  for ( meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].num_cells; meas_idx++ )
  {
    if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].rat )
    {
      if ( !cell_found )
      {
          if ( scr_code == l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].wcdma_cell.scr_code)
          {
            cell_found = TRUE;
          }
      }

      if ( cell_found && (meas_idx < (MAX_WCDMA_CELLS_PER_UARFCN-1)) )
      {
        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx] = l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx+1];
        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx] = l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx+1];
      }

    }
#ifdef FEATURE_GSM_TDS
    else if( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].rat )
    {
      if ( !cell_found )
      {
        /* cell_param_id would be passed into this function  - same as scr_code */
        if ( scr_code == l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].tds_cell.cell_parameter_id)
        {
          cell_found = TRUE;
        }
      }

      if ( cell_found && (meas_idx < (MAX_WCDMA_CELLS_PER_UARFCN-1)) )
      {
        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx] = l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx+1];
        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx] = l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx+1];
      }
    }
#endif
    else
    {
      l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx] = l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx+1];
      l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx] = l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx+1];
    }
  }

  if ( cell_found )
  {
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].num_cells--;
    MSG_GERAN_HIGH_3_G("Removed PSC:%d on UARFCN %d cells %d from stored_meas",scr_code,UARFCN,l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].num_cells);
  }
  else
  {
    MSG_GERAN_HIGH_3_G("Cannot remove PSC:%d on UARFCN %d RAT %d from stored_meas",scr_code,UARFCN,rat);
  }
} /* remove_stored_meas_cell_entry */
 
/*===========================================================================

FUNCTION update_stored_meas_cell_entry

DESCRIPTION
  Updates the data for the entry in the stored_meas database that matches
  the UARFCN and SCR code passed in.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void update_stored_meas_cell_entry( uint32                           UARFCN,
                                    const gl1_wcdma_cell_meas_type*  cell_data_ptr,
                                    uint8                            rat,
                                    boolean                          rscp_valid,
                                    gas_id_t                         gas_id
                                  )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16 meas_idx, arfcn_idx;
  boolean freq_found = FALSE;

  for ( arfcn_idx=0;arfcn_idx<MAX_WCDMA_FREQS;arfcn_idx++)
  {
    if ( (uint32)l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cached_UARFCN == UARFCN )
    {
      freq_found = TRUE;
      break;
    }
  }

  /* Keep LINT happy */
  if (arfcn_idx >= MAX_WCDMA_FREQS)
  {
    arfcn_idx = ( MAX_WCDMA_FREQS - 1 );
  }

  if ( !freq_found )
  {
    MSG_GERAN_ERROR_1_G("Cannot update stored meas UARFCN%d",UARFCN);
    return;
  }

  for ( meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].num_cells; meas_idx++ )
  {
    if (rat == (uint8) RR_L1_IRAT_RAT_WCDMA)
    {
      if ( cell_data_ptr->cell.scr_code == l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].wcdma_cell.scr_code)
      {

        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.EcNo_avg =
                   l1_sc_g2w_get_ecio_logarithmic_average( cell_data_ptr->EcNo,
                                                           l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.EcNo_last
                                                         );

        l1_sc_g2x_filter_treatment(NULL, cell_data_ptr->RSSI, arfcn_idx, meas_idx, RR_L1_IRAT_RAT_WCDMA, gas_id);


        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg =
                ( l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg +
                 (l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.EcNo_avg/2) );

        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].wcdma_cell.PN_pos    = cell_data_ptr->cell.PN_pos;
        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].wcdma_cell.diversity = cell_data_ptr->cell.diversity;

        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = cell_data_ptr->RSCP;
        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.EcNo_last = cell_data_ptr->EcNo;
        break;
      }
    }
#ifdef FEATURE_GSM_TDS
    else if ( rat == RR_L1_IRAT_RAT_UTRAN_TDD )
    {
      if (cell_data_ptr->cell.scr_code == l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].tds_cell.cell_parameter_id)
      {

        /*EcNo measurements do not aplpy for TDS so no need to populate that field*/

        /*Need to confirm whether RSSI will be returned to us.*/
        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg =
                ( l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last +
                  cell_data_ptr->RSSI) / 2;

        /*Since TDS L1 supplies RSCP we should be able to trust their values*/
        /* Store RSCP_last and RSCP_avg when RSCP value is valid */
        if(rscp_valid)
        {
           l1_sc_g2x_filter_treatment(NULL, cell_data_ptr->RSCP, arfcn_idx, meas_idx, RR_L1_IRAT_RAT_UTRAN_TDD, gas_id);
        }
        else
        {
          MSG_GERAN_HIGH_3_G("invalid history rscp value = %d, arfcnidex = %d, measidx = %d",l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last, arfcn_idx, meas_idx);
          l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg   = MIN_TDD_CELL_RSCP_VALUE;
          l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last  = MIN_TDD_CELL_RSCP_VALUE;
          l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2 = MIN_TDD_CELL_RSCP_VALUE;
        }

        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].tds_cell.diversity  = cell_data_ptr->cell.diversity;

        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last = cell_data_ptr->RSSI;

        // frequency parameters are updated via different function
        l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].tds_cell.cell_position = cell_data_ptr->cell.PN_pos;
      break;
    }
    }
#endif
  }
} /* update_stored_meas_cell_entry */

/*===========================================================================

FUNCTION add_stored_meas_cell_entry

DESCRIPTION
  Adds an entry to the stored measurement database using the data supplied
  in the gl1_wcdma_cell_meas_type

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void add_stored_meas_cell_entry( uint32                           UARFCN,
                                 const gl1_wcdma_cell_meas_type*  cell_data_ptr,
                                 uint8                            rat,
                                 boolean                          rscp_valid,
                                 gas_id_t                         gas_id
                                 )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16 meas_idx, arfcn_idx;
  boolean freq_found = FALSE;

  for ( arfcn_idx=0;arfcn_idx<MAX_WCDMA_FREQS;arfcn_idx++)
  {
    if ( (uint32)l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cached_UARFCN == UARFCN )
    {
      freq_found = TRUE;
      break;
    }
  }

  /* Keep LINT happy */
  if (arfcn_idx >= MAX_WCDMA_FREQS)
  {
    arfcn_idx = ( MAX_WCDMA_FREQS - 1 );
  }

  if ( !freq_found )
  {
    MSG_GERAN_ERROR_1_G("Cannot add to stored meas UARFCN%d",UARFCN);
    return;
  }

  /* if there are already max cells in the known list */
  if ( l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].num_cells >= MAX_WCDMA_CELLS_PER_UARFCN )
  {
    /* delete the most recent one added */
    meas_idx = MAX_WCDMA_CELLS_PER_UARFCN - 1;
  }
  else
  {
    meas_idx = l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].num_cells;
  }


  MSG_GERAN_HIGH_3_G("Added new cell arfcn = %d arfcn_idx= %d meas_indx = %d",
           UARFCN, arfcn_idx, meas_idx);

  if (rat == (uint8)RR_L1_IRAT_RAT_WCDMA )
  {
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].wcdma_cell.scr_code  = cell_data_ptr->cell.scr_code;
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].wcdma_cell.PN_pos    = cell_data_ptr->cell.PN_pos;
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].wcdma_cell.diversity = cell_data_ptr->cell.diversity;
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg = cell_data_ptr->RSCP;
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = cell_data_ptr->RSCP;
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.EcNo_avg = cell_data_ptr->EcNo;
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.EcNo_last = cell_data_ptr->EcNo;

    l1_sc_g2x_filter_treatment(NULL, cell_data_ptr->RSSI, arfcn_idx, meas_idx, RR_L1_IRAT_RAT_WCDMA, gas_id);
  }
#ifdef FEATURE_GSM_TDS
  else if (rat == (uint8)RR_L1_IRAT_RAT_UTRAN_TDD )
  {
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].tds_cell.cell_parameter_id = cell_data_ptr->cell.scr_code;
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].tds_cell.diversity = cell_data_ptr->cell.diversity;

    // frequency parameters are updated  via different function
    // l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.last_lna_state = cell_data_ptr->cell.last_lna_state;
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_params[meas_idx].tds_cell.cell_position = cell_data_ptr->cell.PN_pos;
    /*Don't think I need these last 2 as done below.*/
    // l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.last_acq_state = cell_data_ptr->cell.last_acq_state;

    /* Store RSCP_avg and RSCP_last when RSCP value is valid*/
    if(rscp_valid)
    {
        l1_sc_g2x_filter_treatment(NULL, cell_data_ptr->RSCP, arfcn_idx, meas_idx, RR_L1_IRAT_RAT_UTRAN_TDD, gas_id);
    }
  else
  {
    MSG_GERAN_HIGH_3_G("Reported RSCP is out of range: last %d, last2 %d, avg %d",l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last,l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2,l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg);
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg   = MIN_TDD_CELL_RSCP_VALUE;
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2 = MIN_TDD_CELL_RSCP_VALUE;
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last  = MIN_TDD_CELL_RSCP_VALUE;
    }
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg = cell_data_ptr->RSSI;
    l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last = cell_data_ptr->RSSI;
  }
#endif


  l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].num_cells++;

  MSG_GERAN_HIGH_2_G("rscp valid: cpid: %d, num_cells: %d", cell_data_ptr->cell.scr_code,
                                                            l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].num_cells);

} /* add_stored_meas_cell_entry */


#ifdef FEATURE_GSM_TDS
/*===========================================================================

FUNCTION l1_sc_send_tds_cell_search_results_to_rr

DESCRIPTION
  This function is called to from the L1 task to process the
  results of the most recent TDS id search. It uses as a template
  l1_sc_process_wcdma_id_srch_results().

  1- Duplicate cells (scrambling codes) are removed from the id report.

  2- Remove any cells in the stored measurements structure that are not
     in the id report.

  3- Update the stored measurement structure with the results.

  The id report contains:

  Scrambling code (0..511, or all codes
  cell diversity
  PN Position (19 bit value, or PN_POSITION_UNKNOWN)

  RSCP     Received signal strength (in dBm)
  EcNo     Energy value returned from firmware
  RSSI

  The rr_report structure is updated and MPH_SURROUND_WCDMA_IDLE_MEAS_IND is
  sent to RR.

  This function handles  TDS cells.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void  l1_sc_send_tds_cell_search_results_to_rr( void* freq_index, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint16  freq_idx      = (uint16) (uint32) (void *) freq_index;

  if( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat )
  {
    rr_l1_irat_measurements_u  irat_measurements;

    uint16  meas_idx = 0;

    /* Send report to RR */
    irat_measurements.utran_tdd.uarfcn = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN;

    /* rr_report.num_cells includes both WCDMA and LTE cells so find
     * the number of LTE cells only.
     */

    irat_measurements.utran_tdd.no_of_entries = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.num_of_jds_cells;

    /* CR352537: Don't pass more than 32(MAX_UTRAN_TDD_MEASUREMENTS_PER_FREQUENCY) cells to RR */
    while( (meas_idx < l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.num_of_jds_cells) && (meas_idx < MAX_UTRAN_TDD_MEASUREMENTS_PER_FREQUENCY) )
    {
      irat_measurements.utran_tdd.entries[meas_idx].cell_parameter_id =
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].tds_cell.cell_parameter_id;

      irat_measurements.utran_tdd.entries[meas_idx].diversity =
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].tds_cell.diversity;

      /*This value is not reliable - see how we generate this!!*/
      irat_measurements.utran_tdd.entries[meas_idx].rscp =
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg;

      irat_measurements.utran_tdd.entries[meas_idx].rssi =
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg;

      meas_idx++;
    }

    irat_measurements.utran_tdd.ts0_pos = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.ts0_position;

    L1_send_MPH_SURROUND_IRAT_MEAS_IND( RR_L1_IRAT_RAT_UTRAN_TDD,
                                        &irat_measurements, gas_id
                                      );

    l1_sc_forced_meas_event( freq_idx, FORCE_MEAS_COMPLETED, gas_id);


    MSG_GERAN_MED_3_G( " TDS uarfcn %u num cells %u %u ",
             irat_measurements.utran_tdd.uarfcn,
             irat_measurements.utran_tdd.no_of_entries,
             l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells
           );

  }  /* if( RR_L1_IRAT_RAT_LTE == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat ) */
  else
  {
    MSG_GERAN_ERROR_3_G(" Cannot send RAT % search results for frequency %d in TDS meas report ",
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat,
              freq_idx,
              l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN );
  }
} /* l1_sc_send_tds_cell_search_results_to_rr */

/*===========================================================================

FUNCTION l1_sc_send_tds_failure_acq_results_to_rr

DESCRIPTION
  This function is called to from the L1 task to process the
  results of the most recent TDS id search.

  The id report contains:
     Only UARFCN is valid.


  This function handles  TDS ACQ failure case.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void  l1_sc_send_tds_failure_acq_results_to_rr( void* freq_index, gas_id_t gas_id)
{
  uint16  freq_idx      = (uint16) (uint32) (void *) freq_index;
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  if( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat )
  {
    rr_l1_irat_measurements_u  irat_measurements;

    irat_measurements.utran_tdd.uarfcn = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN;

    irat_measurements.utran_tdd.no_of_entries = 0;

    irat_measurements.utran_tdd.ts0_pos = 0xFFFF;

    L1_send_MPH_SURROUND_IRAT_MEAS_IND( RR_L1_IRAT_RAT_UTRAN_TDD,&irat_measurements, gas_id);

    MSG_GERAN_HIGH_1_G("Send Failure ACQ result, uarfcn = %d",irat_measurements.utran_tdd.uarfcn);
  }
  else
  {
    MSG_GERAN_ERROR_3_G("Cannot send Failure ACQ result, RAT % uarfcn = %d",
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat,
              l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN,0);
  }
}
#endif /*FEATURE_GSM_TDS*/

/*===========================================================================
FUNCTION l1_isr_femto_timeout_handler

DESCRIPTION
  These functions handle the femto search timer. This is run to ensure that
  if no searches have taken place in the past 5 mins, than one is started.
  This may have happened as the MS is in a good signal strength area and the
  Qsearch is not set to always, but is dependant upon the serving cell signal
  strength.

===========================================================================*/
static void l1_isr_femto_timeout_handler_callback (const uint32 timer_id)
{
  switch ( femto_search_state )
  {
  case FEMTO_SEARCH_NULL:
    break;

  case FEMTO_SEARCH_TIMER_RUNNING:
    l1_isr_femto_timer_searching_start();
    MSG_GERAN_HIGH_0("FEMTO search timeout");
    /* Start a cell ID off */
    break;

  case FEMTO_SEARCH_IN_PROGRESS:
    femto_search_state = FEMTO_SEARCH_NULL;
    l1_isr_femto_timer_start();
    MSG_GERAN_HIGH_0("FEMTO search running timeout");
    break;

  default:
    MSG_GERAN_HIGH_0("FEMTO state unknown");
  }
}

/*===========================================================================
FUNCTION l1_isr_femto_timer_start

DESCRIPTION
  Starts the FEMTO search timer. When/if this expires it will flag the need for
  a WCDMA cell search to take place. The timer can only be started if the FEMTO
  search state is FEMTO_SEARCH_NULL. This ensures that if the timer has already
  expired and the MS is performing a 20sec femto search, this is not abandoned.
===========================================================================*/
void l1_isr_femto_timer_start (void)
{
   if ( femto_search_state == FEMTO_SEARCH_NULL )
   {
     (void) rex_set_timer(&l1_isr_femto_timer, L1_ISR_FEMTO_TIMEOUT);

     femto_search_state = FEMTO_SEARCH_TIMER_RUNNING;

     MSG_GERAN_HIGH_0("l1_isr_femto_timer_start");
   }
   else
   {
     MSG_GERAN_HIGH_1("l1_isr_femto_timer_start - not actioned %d",femto_search_state);
   }
}

/*===========================================================================
FUNCTION l1_isr_femto_timer_searching_start

DESCRIPTION
  Starts the FEMTO searching timer. This runs to allow searches to occur for
  the duration of its period.
===========================================================================*/
static void l1_isr_femto_timer_searching_start (void)
{
   (void) rex_clr_timer(&l1_isr_femto_timer);
   (void) rex_set_timer(&l1_isr_femto_timer, L1_ISR_FEMTO_RUNNING);

   femto_search_state = FEMTO_SEARCH_IN_PROGRESS;

   MSG_GERAN_HIGH_0("l1_isr_femto_timer_searching_start");
}

/*===========================================================================
FUNCTION l1_isr_femto_timer_stop

DESCRIPTION
  Stops any FEMTO searching timer.The timer can only be stopped if the FEMTO
  search state is FEMTO_SEARCH_NULL. This ensures that if the timer has already
  expired and the MS is performing a 20sec femto search, this is not abandoned.
===========================================================================*/
void l1_isr_femto_timer_stop (void)
{
   /* Disable the timer and clear any pending signal but not if in FEMTO_SEARCH_RUNNING state */
  if ( femto_search_state == FEMTO_SEARCH_TIMER_RUNNING )
  {
    (void) rex_clr_timer(&l1_isr_femto_timer);

    femto_search_state = FEMTO_SEARCH_NULL;

    MSG_GERAN_HIGH_0("l1_isr_femto_timer_stop");
  }
  else
  {
    MSG_GERAN_HIGH_1("l1_isr_femto_timer_stop - not actioned %d",femto_search_state);
  }
}

/*===========================================================================
FUNCTION l1_sc_init_femto_timer

DESCRIPTION
  defines the FEMTO timer using the rex_def_timer_ex() which allows the addition
  of a callback to be processed upon the specified timer expiry.
===========================================================================*/
void l1_sc_init_femto_timer (void)
{
  rex_def_timer_ex(&l1_isr_femto_timer, l1_isr_femto_timeout_handler_callback, 0 );
}

#ifdef FEATURE_GSM_TDS
/*===========================================================================

FUNCTION get_stored_meas_freq_entry

DESCRIPTION
  Returns the stored measurement databases frequency parameters
  that correspond to the UARFCN and RAT passed in. If no entry is
  found to match then the boolean returns FALSE, otherwise TRUE.
  Data for the entry is passed back using the pointers freq_params_ptr,
  providing they are not NULL

DEPENDENCIES
  none

RETURN VALUE
  boolean - if frequency was found TRUE else FALSE

SIDE EFFECTS
  None

===========================================================================*/
static boolean get_stored_meas_freq_entry(uint32 UARFCN,
                                          uint8  rat,
                                          gl1_wcdma_cell_meas_rpt* freq_params_ptr,
                                          gas_id_t gas_id
                                         )
{

  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  boolean freq_found = FALSE;
  uint16 arfcn_idx;

  for ( arfcn_idx=0;arfcn_idx<MAX_WCDMA_FREQS;arfcn_idx++)
  {
    if(  ( (uint32)l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cached_UARFCN == UARFCN )
       &&( rat == l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].rat )
      )
    {
      freq_found = TRUE;
      break;
    }
  }

  /* Keep LINT happy */
  if (arfcn_idx >= MAX_WCDMA_FREQS)
  {
    arfcn_idx = ( MAX_WCDMA_FREQS - 1 );
  }

  if ( !freq_found )
  {
    MSG_GERAN_ERROR_2_G("Cannot find stored meas freq UARFCN %d RAT %d",UARFCN,rat);
    return !freq_found;
  }
  else
  {
    if (freq_params_ptr)
    {
      freq_params_ptr->last_acq_state = l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.last_acq_state;
      freq_params_ptr->last_lna_state = l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.last_lna_state;
      freq_params_ptr->measuredFreq = l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.measuredFreq;
      freq_params_ptr->ts0_position = l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.ts0_position;
      freq_params_ptr->num_of_jds_cells = l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.num_of_jds_cells;
      freq_params_ptr->rssi = l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.rssi;
    }
  }
  return freq_found;
}

/*===========================================================================

FUNCTION update_stored_meas_freq_entry

DESCRIPTION
  Updates the data for the entry in the stored_meas database that matches
  the UARFCN and RAT passed in.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void update_stored_meas_freq_entry( uint32                           UARFCN,
                                           const gl1_wcdma_cell_meas_rpt*  freq_params_ptr,
                                           uint8                            rat,
                                           gas_id_t                         gas_id
                                         )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint16 arfcn_idx;
  boolean freq_found = FALSE;

  for ( arfcn_idx=0;arfcn_idx<MAX_WCDMA_FREQS;arfcn_idx++)
  {
    if ( (uint32)l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].cached_UARFCN == UARFCN )
    {
      freq_found = TRUE;
      break;
    }
  }

  /* Keep LINT happy */
  if (arfcn_idx >= MAX_WCDMA_FREQS)
  {
    arfcn_idx = ( MAX_WCDMA_FREQS - 1 );
  }

  if ( !freq_found )
  {
    MSG_GERAN_ERROR_1_G("Cannot update stored meas UARFCN%d",UARFCN);
    return;
  }

  l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.last_acq_state = freq_params_ptr->last_acq_state;
  l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.last_lna_state = freq_params_ptr->last_lna_state;
  l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.measuredFreq = freq_params_ptr->measuredFreq;
  l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.ts0_position = freq_params_ptr->ts0_position;
  l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.num_of_jds_cells = freq_params_ptr->num_of_jds_cells;
  l1_sc_wcdma_data_ptr->stored_meas[arfcn_idx].tdd_freq_params.rssi = freq_params_ptr->rssi;

}

/*===========================================================================

FUNCTION l1_sc_idle_tds_acq_results

DESCRIPTION
  This function is called to from the L1 task to process the
  results of the most recent TDS id search. It uses as a template
  l1_sc_process_wcdma_id_srch_results().

  1- Duplicate cells (scrambling codes) are removed from the id report.

  2- Remove any cells in the stored measurements structure that are not
     in the id report.

  3- Update the stored measurement structure with the results.

  The id report contains:

  Scrambling code (0..511, or all codes
  cell diversity
  PN Position (19 bit value, or PN_POSITION_UNKNOWN)

  RSCP     Received signal strength (in dBm)
  EcNo     Energy value returned from firmware
  RSSI

  The rr_report structure is updated and MPH_SURROUND_WCDMA_IDLE_MEAS_IND is
  sent to RR.

  This function handles both WCDMA, LTE and TDS cells.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  l1_sc_idle_tds_acq_results( const tdsirat_freq_list_type*  acq_rsp_ptr, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16 meas_idx = 0;
  uint16 tmp_scr_code = 0;
  uint16 new_idx = 0;
  uint16  rpt_idx = 0;
  uint32  freq_idx = 0;

  uint16 num_rpt_cells = 0;
  uint8  num_of_freq = 0;
  gsm_l1_lte_wcdma_cell_params_type     temp_cell_params = {{0, 0, 0}};
  t_gsm_l1_lte_wcdma_cell_energy_details  temp_cell_energy = {{0, 0}};

  //tds_irat_meas_list_struct  stripped_id_rpt = {0};
  /* In TDD, ACQ_RSP may or may not contain valid RSCP depend upon the acq state */
  boolean rscp_valid = TRUE;

  if(l1_sc_wcdma_data_ptr->tds_state != TDS_ACQ_RSP)
  {
    MSG_GERAN_ERROR_1_G(" TDS_CPHY_IRAT_ACQ_RSP received in wrong state: TDS State %d",
              l1_sc_wcdma_data_ptr->tds_state);
    return;
  }

  MSG_GERAN_HIGH_3_G("TDS ACQ FREQ Info: uarfcn %d rssi %d jds_cells %d", acq_rsp_ptr->freq[0].uarfcn,acq_rsp_ptr->freq[0].rssi, acq_rsp_ptr->freq[0].num_of_jds_cells);
  MSG_GERAN_HIGH_3_G("TDS ACQ FREQ Info: acq_rsp_ptr->freq[0].acq_state %d, acq_rsp_ptr->freq[0].lna_state %d, acq_rsp_ptr->freq[0].ts0_position %d",
               acq_rsp_ptr->freq[0].acq_state,acq_rsp_ptr->freq[0].lna_state,acq_rsp_ptr->freq[0].ts0_position);
  for(num_rpt_cells = 0; num_rpt_cells < acq_rsp_ptr->freq[0].num_of_cells; num_rpt_cells++)
  {
     MSG_GERAN_HIGH_3_G("TDS ACQ cell Info: cell_position %d cpid %d rscp %d",
              acq_rsp_ptr->freq[0].cell[num_rpt_cells].cell_position,
              acq_rsp_ptr->freq[0].cell[num_rpt_cells].cpid,
              acq_rsp_ptr->freq[0].cell[num_rpt_cells].rscp);
  }
  num_rpt_cells = 0;


  for (num_of_freq=0;num_of_freq < acq_rsp_ptr->num_of_freq;num_of_freq++)
  {
    /*find the freq_idx that the 1st reported freq relates to*/
    for (freq_idx=0;freq_idx <l1_sc_wcdma_data_ptr->rr_params.num_freqs;freq_idx++)
    {
      if ( (RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat) &&
           (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN == acq_rsp_ptr->freq[num_of_freq].uarfcn) )
      {
        /*found the correct freq_idx*/
        num_rpt_cells = acq_rsp_ptr->freq[num_of_freq].num_of_cells;
        break;
      }

      if (freq_idx == ( l1_sc_wcdma_data_ptr->rr_params.num_freqs -1))
      {
        MSG_GERAN_HIGH_1_G("Unknown TDS meas rpt: ch:%d!", acq_rsp_ptr->freq[num_of_freq].uarfcn);

        return;
      }
    }

  MSG_GERAN_HIGH_3_G("idle acq_rsp_ptr->num_of_freq: %d; number cell = %d, store cellnum =%d", acq_rsp_ptr->num_of_freq,acq_rsp_ptr->freq[0].num_of_cells,l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells);

#ifdef FEATURE_GSM_TDS
    {
  gl1_wcdma_cell_meas_rpt* tdd_freq_info_ptr = &l1_sc_wcdma_data_ptr->id_rpt;

    // find reported frequency information from stored value
    if ( get_stored_meas_freq_entry( acq_rsp_ptr->freq[num_of_freq].uarfcn,
                                       (uint8) RR_L1_IRAT_RAT_UTRAN_TDD,
                                       NULL,
                                       gas_id
                                    )
          )
    {
      // if frequency infor exists in the stored meas, then update with the latest information

      tdd_freq_info_ptr->last_acq_state = acq_rsp_ptr->freq[num_of_freq].acq_state;
      tdd_freq_info_ptr->last_lna_state = acq_rsp_ptr->freq[num_of_freq].lna_state;
      tdd_freq_info_ptr->measuredFreq = acq_rsp_ptr->freq[num_of_freq].measuredFreq;
      tdd_freq_info_ptr->ts0_position = acq_rsp_ptr->freq[num_of_freq].ts0_position;
      tdd_freq_info_ptr->num_of_jds_cells = acq_rsp_ptr->freq[num_of_freq].num_of_jds_cells;
      tdd_freq_info_ptr->rssi = acq_rsp_ptr->freq[num_of_freq].rssi;
      update_stored_meas_freq_entry(acq_rsp_ptr->freq[num_of_freq].uarfcn, tdd_freq_info_ptr, (uint8) RR_L1_IRAT_RAT_UTRAN_TDD, gas_id );
    }
    else
    {
      MSG_GERAN_ERROR_1_G(" l1_sc_tds_process_acq_results : No frequency entry in stored_meas %d",
              acq_rsp_ptr->freq[num_of_freq].uarfcn);
    }

    /* CR352537 Delete cells which are not common in ACQ_RSP and stored_meas structure*/
    for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
    {
      boolean found = FALSE;

      tmp_scr_code = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].tds_cell.cell_parameter_id;

      for (rpt_idx=0; rpt_idx<num_rpt_cells; rpt_idx++)
      {
        if ((acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].cpid == tmp_scr_code) && (acq_rsp_ptr->freq[0].num_of_cells != 0))
        {
          found = TRUE;
          break;
        }
      }
      if (!found)
      {
#ifdef FEATURE_GSM_TDS_DEBUG
        MSG_GERAN_HIGH_1_G("JDS: Marking tmp_scr_code %d for deletion", tmp_scr_code);
#endif
        /* No current measurement, so mark for deletion */
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2= MIN_TDD_CELL_RSCP_VALUE;

        /* set this deleted cell RSCP average to zero, so its not included in the strongest 8 filter that follows */
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;
      }
    }
    for (meas_idx=0, new_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
    {
      if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last != MIN_TDD_CELL_RSCP_VALUE)
      {
        if (new_idx != meas_idx)
        {
          temp_cell_params = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx];

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx] =
                  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx];

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx] = temp_cell_params;

          temp_cell_energy = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx];

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx] =
                  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx];

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx] = temp_cell_energy;
        }
        new_idx++;
      }
    }

    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = new_idx;
    MSG_GERAN_HIGH_3_G("num_cells = %d, new_idx = %d, meas_idx = %d", l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells, new_idx, meas_idx);
  }

#endif /*FEATURE_GSM_TDS*/
    /**********************************************************/
    /* Loop through all the entries returned in the ID report */
    /* and store the data values in the stored meas database  */
    /**********************************************************/
    for (rpt_idx=0; rpt_idx< acq_rsp_ptr->freq[num_of_freq].num_of_cells; rpt_idx++)
    {
        // memory is not allocated
        //gl1_wcdma_cell_meas_type*  id_rpt_entry_ptr;//just need this to pass down values = &l1_sc_wcdma_data_ptr->id_rpt.meas[rpt_idx];
        gl1_wcdma_cell_meas_type*  id_rpt_entry_ptr = &l1_sc_wcdma_data_ptr->id_rpt.meas[rpt_idx];

        /* scr_code & cell_param_id have he same type and neither
         * will change going forward so OK to use gl1_wcdma_cell_meas_type
         * to convey TDS information
         */
        id_rpt_entry_ptr->cell.scr_code =
          acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].cpid;

        /*
        Update RSCP value when acq state is TDSIRAT_MEAS_DONE, otherwise TL1 returns snr value in the rscp field.
        */
        if((acq_rsp_ptr->freq[num_of_freq].acq_state == TDSIRAT_MEAS_DONE) &&
           (acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].rscp <= 0) &&
           (acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].rscp >= MIN_TDD_CELL_RSCP_VALUE)
          )
        {
           rscp_valid = TRUE;
           id_rpt_entry_ptr->RSCP = acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].rscp;
        }
        else
        {
#ifdef FEATURE_GSM_TDS_DEBUG
          MSG_GERAN_HIGH_1_G("TDS Ignoring Junk RSCP %d",acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].rscp);
#endif /*FEATURE_GSM_TDS_DEBUG*/
          rscp_valid = FALSE;
          id_rpt_entry_ptr->RSCP = MIN_TDD_CELL_RSCP_VALUE;
        }

        id_rpt_entry_ptr->cell.PN_pos =
          acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].cell_position;
      /**************************************************************/
      /* See if there is already a matching entry for this PSC      */
      /* in the stored_meas database. If there is update the values */
      /* otherwise add the entry.                                   */
      /**************************************************************/
      if ( get_stored_meas_cell_entry( acq_rsp_ptr->freq[num_of_freq].uarfcn,
                                       id_rpt_entry_ptr->cell.scr_code,
                                       (uint8) RR_L1_IRAT_RAT_UTRAN_TDD,
                                       NULL,
                                       NULL,
                                       gas_id )
          )
      {

       /* No need to qualify results with a ECNO check as needed with WCDMA
        * results as TDS side will pre-filter or us
        */

        /***********************************************************/
        /* Update all the relevent parameters for the entry in the */
        /* stored meas database. These will get reported to RR in  */
        /* the next measurement report.                            */
        /* Use gl1_wcdma_cell_meas_type so that we can use same    */
        /* function - it has the correct types/information to be   */
        /* reused for TDS                                          */
        /***********************************************************/

        update_stored_meas_cell_entry(acq_rsp_ptr->freq[num_of_freq].uarfcn, id_rpt_entry_ptr, (uint8) RR_L1_IRAT_RAT_UTRAN_TDD, rscp_valid, gas_id );
      }
      else
      {

        /* Fill in the diversity value as defaulting to ON */
        id_rpt_entry_ptr->cell.diversity = DIVERSITY_ON;

        add_stored_meas_cell_entry( acq_rsp_ptr->freq[num_of_freq].uarfcn, id_rpt_entry_ptr, (uint8) RR_L1_IRAT_RAT_UTRAN_TDD, rscp_valid, gas_id );

      } /* End of else the cell is not found in the stored measurements */
    }   /* for( rpt_idx = 0; rpt_idx < l1_sc_wcdma_data_ptr->id_rpt.num_cell_measurements; rpt_idx++ ) */

    if (acq_rsp_ptr->freq[num_of_freq].num_of_jds_cells)
    {

      /**
       * CR347851 JDS: If ACQ_RSP contains number of cells, then stop
       * acquisition irrespective whether number of cells matches with
       * NW info or not
       * l1_sc_update_stopping_cell_id_srch((uint16)freq_idx, gas_id)
       */
      l1_sc_wcdma_data_ptr->stop_cell_id_srch[freq_idx] = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN;
      #ifdef FEATURE_GL1_GPLT
        gplt_acq_meas_result[gas_id] = FALSE;
      #endif /* FEATURE_GL1_GPLT*/
    }
    else
    {
      for(meas_idx = 0; meas_idx < MAX_WCDMA_CELLS_PER_UARFCN; meas_idx++)
      {
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg  = MIN_TDD_CELL_RSCP_VALUE;
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2= MIN_TDD_CELL_RSCP_VALUE;
     }

     MSG_GERAN_HIGH_3_G("reported cells = %d, stored cells = %d",acq_rsp_ptr->freq[num_of_freq].num_of_cells, l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells, 0);
    }

    if (acq_rsp_ptr->freq[num_of_freq].acq_state == TDSIRAT_MEAS_DONE)
    {
      update_stored_meas_jds_cells(acq_rsp_ptr->freq[num_of_freq].uarfcn,
                                   acq_rsp_ptr->freq[num_of_freq].num_of_jds_cells, acq_rsp_ptr->freq[num_of_freq].cell,gas_id);
    }
  }


   /* Generate report for RR irrespective of Acq results*/
   gsm_l1_update_rr_report( &( l1_sc_wcdma_data_ptr->stored_meas[0] ),
                            &l1_sc_wcdma_data_ptr->rr_ignore_list,
                            &l1_sc_wcdma_data_ptr->rr_params,
                            &l1_sc_wcdma_data_ptr->rr_report,
                            gas_id);

   if ( (acq_rsp_ptr->freq[0].acq_state == TDSIRAT_MEAS_DONE))
   {

     MSG_GERAN_HIGH_1_G("Sending ACQ Result to RR freq_idx %d", freq_idx);
     l1_sc_send_tds_cell_search_results_to_rr( (void *) freq_idx, gas_id );
   }
   else
   {
     MSG_GERAN_HIGH_1_G("Sending Failure ACQ Result to RR freq_idx %d", freq_idx);
     l1_sc_send_tds_failure_acq_results_to_rr((void *) freq_idx, gas_id);
   }
   // ======================GOT TO HERE=====================

    /* start the Femto search timer here */
  l1_isr_femto_timer_start();

} /* l1_sc_idle_tds_acq_results */


/*===========================================================================

FUNCTION l1_sc_ded_tds_acq_results

DESCRIPTION
  This function is called to from the L1 task to process the
  results of the most recent TDS id search. It uses as a template
  l1_sc_process_wcdma_id_srch_results().

  1- Duplicate cells (scrambling codes) are removed from the id report.

  2- Remove any cells in the stored measurements structure that are not
     in the id report.

  3- Update the stored measurement structure with the results.

  The id report contains:

  Scrambling code (0..511, or all codes
  cell diversity
  PN Position (19 bit value, or PN_POSITION_UNKNOWN)

  RSCP     Received signal strength (in dBm)
  EcNo     Energy value returned from firmware
  RSSI

  The rr_report structure is updated and MPH_SURROUND_WCDMA_IDLE_MEAS_IND is
  sent to RR.

  This function handles both WCDMA, LTE and TDS cells.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  l1_sc_ded_tds_acq_results( const tdsirat_freq_list_type*  acq_rsp_ptr, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint16 meas_idx = 0;
  uint16 tmp_scr_code = 0;
  uint16 new_idx = 0;
  uint16  rpt_idx = 0;
  uint32  freq_idx = 0;
  uint16 num_rpt_cells = 0;
  uint8  num_of_freq = 0;
  gsm_l1_lte_wcdma_cell_params_type     temp_cell_params = {{0, 0, 0}};
  t_gsm_l1_lte_wcdma_cell_energy_details  temp_cell_energy = {{0, 0}};

  //tds_irat_meas_list_struct  stripped_id_rpt = {0};

  /* In TDD, ACQ_RSP may or may not contain valid RSCP depend upon the acq state */
  boolean rscp_valid = TRUE;
  // index of frequency on which acq performed
  freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset;


  MSG_GERAN_LOW_0_G("TDSCDMA ACQ in Dedicated Mode successful!");


  if(l1_sc_wcdma_data_ptr->tds_state != TDS_ACQ_RSP)
  {
    MSG_GERAN_ERROR_1_G(" TDS_CPHY_IRAT_ACQ_RSP received in wrong state: TDS State %d",
              l1_sc_wcdma_data_ptr->tds_state);
    return;
  }

  MSG_GERAN_HIGH_3_G("TDS ACQ FREQ Info: uarfcn %d rssi %d jds_cells %d", acq_rsp_ptr->freq[0].uarfcn,acq_rsp_ptr->freq[0].rssi, acq_rsp_ptr->freq[0].num_of_jds_cells);
  MSG_GERAN_HIGH_3_G("TDS ACQ FREQ Info: acq_rsp_ptr->freq[0].acq_state %d, acq_rsp_ptr->freq[0].lna_state %d, acq_rsp_ptr->freq[0].ts0_position %d",
               acq_rsp_ptr->freq[0].acq_state,acq_rsp_ptr->freq[0].lna_state,acq_rsp_ptr->freq[0].ts0_position);
  for(num_rpt_cells = 0; num_rpt_cells < acq_rsp_ptr->freq[0].num_of_cells; num_rpt_cells++)
  {
     MSG_GERAN_HIGH_3_G("TDS ACQ cell Info: cell_position %d cpid %d rscp %d",
              acq_rsp_ptr->freq[0].cell[num_rpt_cells].cell_position,
              acq_rsp_ptr->freq[0].cell[num_rpt_cells].cpid,
              acq_rsp_ptr->freq[0].cell[num_rpt_cells].rscp);
  }
  num_rpt_cells = 0;

  for (num_of_freq=0;num_of_freq < acq_rsp_ptr->num_of_freq;num_of_freq++)
  {
    /*find the freq_idx that the 1st reported freq relates to*/
    for (freq_idx=0;freq_idx <l1_sc_wcdma_data_ptr->rr_params.num_freqs;freq_idx++)
    {
      if ( (RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat) &&
           (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN == acq_rsp_ptr->freq[num_of_freq].uarfcn) )
      {
        /*found the correct freq_idx*/
        num_rpt_cells = acq_rsp_ptr->freq[num_of_freq].num_of_cells;

        break;
      }

      if (freq_idx == ( l1_sc_wcdma_data_ptr->rr_params.num_freqs -1))
      {
        MSG_GERAN_HIGH_1_G("Unknown TDS meas rpt: ch:%d!", acq_rsp_ptr->freq[num_of_freq].uarfcn);
        l1_g2t_gap_end(gas_id);
        return;
      }
    }
  MSG_GERAN_HIGH_3_G("acq_rsp_ptr->num_of_freq: %d; number cell = %d, store cellnum =%d", acq_rsp_ptr->num_of_freq,acq_rsp_ptr->freq[0].num_of_cells,l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells);


#ifdef FEATURE_GSM_TDS
    {
    gl1_wcdma_cell_meas_rpt* tdd_freq_info_ptr = &l1_sc_wcdma_data_ptr->id_rpt;

    // find reported frequency information from stored value
    if ( get_stored_meas_freq_entry( acq_rsp_ptr->freq[num_of_freq].uarfcn,
                                       (uint8) RR_L1_IRAT_RAT_UTRAN_TDD,
                                       NULL,
                                       gas_id
                                    )
          )
    {
      // if frequency infor exists in the stored meas, then update with the latest information
      tdd_freq_info_ptr->last_acq_state = acq_rsp_ptr->freq[num_of_freq].acq_state;
      tdd_freq_info_ptr->last_lna_state = acq_rsp_ptr->freq[num_of_freq].lna_state;
      tdd_freq_info_ptr->measuredFreq = acq_rsp_ptr->freq[num_of_freq].measuredFreq;
      tdd_freq_info_ptr->ts0_position = acq_rsp_ptr->freq[num_of_freq].ts0_position;
      tdd_freq_info_ptr->num_of_jds_cells = acq_rsp_ptr->freq[num_of_freq].num_of_jds_cells;
      tdd_freq_info_ptr->rssi = acq_rsp_ptr->freq[num_of_freq].rssi;
      update_stored_meas_freq_entry(acq_rsp_ptr->freq[num_of_freq].uarfcn, tdd_freq_info_ptr, (uint8) RR_L1_IRAT_RAT_UTRAN_TDD, gas_id );
    }
    else
    {
      MSG_GERAN_ERROR_1_G(" l1_sc_tds_process_acq_results : No frequency entry in stored_meas %d",
              acq_rsp_ptr->freq[num_of_freq].uarfcn);
    }

    /* CR352537 Delete cells which are not common in ACQ_RSP and stored_meas structure*/
    for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
    {
      boolean found = FALSE;

      tmp_scr_code = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].tds_cell.cell_parameter_id;

      for (rpt_idx=0; rpt_idx<num_rpt_cells; rpt_idx++)
      {
        if ((acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].cpid == tmp_scr_code) && (acq_rsp_ptr->freq[num_of_freq].num_of_cells != 0))
        {
          found = TRUE;
          break;
        }
      }
      if (!found)
      {
#ifdef FEATURE_GSM_TDS_DEBUG
        MSG_GERAN_HIGH_1_G("JDS: Marking tmp_scr_code %d for deletion", tmp_scr_code);
#endif
        /* No current measurement, so mark for deletion */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2= MIN_TDD_CELL_RSCP_VALUE;

        /* set this deleted cell RSCP average to zero, so its not included in the strongest 8 filter that follows */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg  = MIN_TDD_CELL_RSCP_VALUE;
      }
    }
    for (meas_idx=0, new_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
    {
      if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last != MIN_TDD_CELL_RSCP_VALUE)
      {
        if (new_idx != meas_idx)
        {
          temp_cell_params = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx];

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx] =
                  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx];

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx] = temp_cell_params;

          temp_cell_energy = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx];

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx] =
                  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx];

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx] = temp_cell_energy;
        }
        new_idx++;
      }
    }

    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = new_idx;

    if (new_idx != meas_idx)
    {
      /* set these 2 params to zero (to cleanup, not necessary) */
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_last2 = MIN_TDD_CELL_RSCP_VALUE;

      MSG_GERAN_HIGH_3_G("Removing old jds cell: uarfcn %d, rscp_avg %d num_cells %d",l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN,
                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_avg,l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells);

      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx].tds_cell.cell_parameter_id = 0;
    }
  }
#endif /*FEATURE_GSM_TDS*/
    /**********************************************************/
    /* Loop through all the entries returned in the ID report */
    /* and store the data values in the stored meas database  */
    /**********************************************************/
    for (rpt_idx=0; rpt_idx< acq_rsp_ptr->freq[num_of_freq].num_of_cells; rpt_idx++)
    {
        // memory is not allocated
        //gl1_wcdma_cell_meas_type*  id_rpt_entry_ptr;//just need this to pass down values = &l1_sc_wcdma_data_ptr->id_rpt.meas[rpt_idx];
        gl1_wcdma_cell_meas_type*  id_rpt_entry_ptr = &l1_sc_wcdma_data_ptr->id_rpt.meas[rpt_idx];

        /* scr_code & cell_param_id have he same type and neither
         * will change going forward so OK to use gl1_wcdma_cell_meas_type
         * to convey TDS information
         */
        id_rpt_entry_ptr->cell.scr_code =
          acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].cpid;

        /*
        Update RSCP value when acq state is TDSIRAT_MEAS_DONE, otherwise TL1 returns snr value in the rscp field.
        */
        if((acq_rsp_ptr->freq[num_of_freq].acq_state == TDSIRAT_MEAS_DONE) &&
           (acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].rscp <= 0) &&
           (acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].rscp >= MIN_TDD_CELL_RSCP_VALUE)
          )
        {
           rscp_valid = TRUE;
           id_rpt_entry_ptr->RSCP = acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].rscp;
        }
        else
        {
#ifdef FEATURE_GSM_TDS_DEBUG
          MSG_GERAN_HIGH_1_G("TDS Ignoring Junk RSCP %d",acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].rscp);
#endif /*FEATURE_GSM_TDS_DEBUG*/
          rscp_valid = FALSE;
          id_rpt_entry_ptr->RSCP = MIN_TDD_CELL_RSCP_VALUE;
        }

        id_rpt_entry_ptr->cell.PN_pos =
          acq_rsp_ptr->freq[num_of_freq].cell[rpt_idx].cell_position;


      /**************************************************************/
      /* See if there is already a matching entry for this PSC      */
      /* in the stored_meas database. If there is update the values */
      /* otherwise add the entry.                                   */
      /**************************************************************/
      if ( get_stored_meas_cell_entry( acq_rsp_ptr->freq[num_of_freq].uarfcn,
                                       id_rpt_entry_ptr->cell.scr_code,
                                       (uint8) RR_L1_IRAT_RAT_UTRAN_TDD,
                                       NULL,
                                       NULL,
                                       gas_id)
          )
      {

       /* No need to qualify results with a ECNO check as needed with WCDMA
        * results as TDS side will pre-filter or us
        */

        /***********************************************************/
        /* Update all the relevent parameters for the entry in the */
        /* stored meas database. These will get reported to RR in  */
        /* the next measurement report.                            */
        /* Use gl1_wcdma_cell_meas_type so that we can use same    */
        /* function - it has the correct types/information to be   */
        /* reused for TDS                                          */
        /***********************************************************/

        update_stored_meas_cell_entry(acq_rsp_ptr->freq[num_of_freq].uarfcn, id_rpt_entry_ptr, (uint8) RR_L1_IRAT_RAT_UTRAN_TDD, rscp_valid, gas_id );
      }
      else
      {
        /* Fill in the diversity value as defaulting to ON */
        id_rpt_entry_ptr->cell.diversity = DIVERSITY_ON;

        add_stored_meas_cell_entry( acq_rsp_ptr->freq[num_of_freq].uarfcn, id_rpt_entry_ptr, (uint8) RR_L1_IRAT_RAT_UTRAN_TDD, rscp_valid, gas_id );

      } /* End of else the cell is not found in the stored measurements */
    }   /* for( rpt_idx = 0; rpt_idx < l1_sc_wcdma_data_ptr->id_rpt.num_cell_measurements; rpt_idx++ ) */

    /**
     * CR347851 JDS: If ACQ contains number of cells, stop cell if
     * search.
     */
    if((acq_rsp_ptr->freq[num_of_freq].num_of_cells) &&
       ((acq_rsp_ptr->freq[num_of_freq].acq_state == TDSIRAT_MEAS_DONE) ||
        (acq_rsp_ptr->freq[num_of_freq].acq_state == TDSIRAT_ACQ_SUCCESS)) )
    {
        /* We have found all the cells in stored list, so, don't do anymore cell id srchs */
        l1_sc_wcdma_data_ptr->stop_ded_cell_id_srch[freq_idx] = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN;
    }

    if(acq_rsp_ptr->freq[num_of_freq].acq_state == TDSIRAT_MEAS_DONE)
    {
      /* now keep only the top 8 cells */
      update_stored_measurements_for_top_8(num_of_freq, gas_id);
      update_stored_meas_jds_cells(acq_rsp_ptr->freq[num_of_freq].uarfcn, acq_rsp_ptr->freq[num_of_freq].num_of_jds_cells,
                                   acq_rsp_ptr->freq[num_of_freq].cell, gas_id);

    }

    if (acq_rsp_ptr->freq[num_of_freq].num_of_cells)
    {
      #ifdef FEATURE_GL1_GPLT
        gplt_acq_meas_result[gas_id] = FALSE;
      #endif /* FEATURE_GL1_GPLT*/
    }
    else
    {
      for(meas_idx = 0; meas_idx < MAX_WCDMA_CELLS_PER_UARFCN; meas_idx++)
      {
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg  = MIN_TDD_CELL_RSCP_VALUE;
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2= MIN_TDD_CELL_RSCP_VALUE;
      }

      MSG_GERAN_HIGH_2_G("reported cells = %d, stored cells = %d",acq_rsp_ptr->freq[num_of_freq].num_of_cells, l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells);
    }
  }

  l1_g2t_gap_end(gas_id);

  /* Generate report for RR */
  gsm_l1_update_rr_report( &( l1_sc_wcdma_data_ptr->stored_meas[0] ),
                           &l1_sc_wcdma_data_ptr->rr_ignore_list,
                           &l1_sc_wcdma_data_ptr->rr_params,
                           &l1_sc_wcdma_data_ptr->rr_report,
                           gas_id);

  if ( (acq_rsp_ptr->freq[0].acq_state == TDSIRAT_MEAS_DONE))
  {

    MSG_GERAN_HIGH_1_G("Sending ACQ Result to RR freq_idx %d", freq_idx);
    l1_sc_send_tds_cell_search_results_to_rr( (void *) freq_idx, gas_id );
  }
  else
  {
    MSG_GERAN_HIGH_1_G("Sending Failure ACQ Result to RR freq_idx %d", freq_idx);
    l1_sc_send_tds_failure_acq_results_to_rr((void *) freq_idx, gas_id);
  }

    /* start the Femto search timer here */
  l1_isr_femto_timer_start();
} /* l1_sc_ded_tds_acq_results */

/*===========================================================================

FUNCTION l1_sc_idle_tds_meas_results

DESCRIPTION
  This function is called to from the L1 task to process the
  results of the most recent TDS reconfirmation search.
  This function is based on l1_sc_process_wcdma_recon_srch_results()

  1- The results are read from the recon_rpt structure.

  2- If reported EcNo is below the minimum for the cell to be valid then
     cell id search is restarted.

  3- If the RSCP has fluctuated compared to the last measurement then cell
     id search is restarted.

  4- All cells are marked for deletion except the strongest 8.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  l1_sc_idle_tds_meas_results( const tdsirat_freq_list_type*  meas_rsp_ptr, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16 rpt_idx, meas_idx;

  uint16 tmp_scr_code;
  uint16 num_rpt_cells;

  uint16 new_idx = 0;
  uint16 min_idx = 0;
  uint16 num_valid = 0;
  int16 min_avg = MIN_TDD_CELL_RSCP_VALUE;

  uint32 freq_idx = 0;
  uint8 meas_list_num_freq = meas_rsp_ptr->num_of_freq;
  uint8 meas_list_idx = 0;
  gsm_l1_lte_wcdma_cell_params_type       temp_cell_params = {{0, 0, 0}};
  t_gsm_l1_lte_wcdma_cell_energy_details  temp_cell_energy = {{0, 0}};

  boolean uarfcn_found = FALSE;

    /* CR347851*/
  /* In TDD, to avoid junk rscp values */
  boolean rscp_valid = TRUE;
  t_gsm_l1_lte_wcdma_cell_energy_details  cell_energy, *cell_energy_ptr = NULL, **cell_energy_ptr_to_ptr = NULL;

  MSG_GERAN_HIGH_3_G("TDS MEAS FREQ Info: uarfcn %d rssi %d jds_cells %d", meas_rsp_ptr->freq[0].uarfcn,meas_rsp_ptr->freq[0].rssi, meas_rsp_ptr->freq[0].num_of_jds_cells);
  MSG_GERAN_HIGH_3_G("TDS MEAS  Info: meas_rsp_ptr->freq[0].acq_state %d, meas_rsp_ptr->freq[0].lna_state %d, meas_rsp_ptr->freq[0].ts0_position %d",
               meas_rsp_ptr->freq[0].acq_state,meas_rsp_ptr->freq[0].lna_state,meas_rsp_ptr->freq[0].ts0_position);
  for(num_rpt_cells = 0; num_rpt_cells < meas_rsp_ptr->freq[0].num_of_cells; num_rpt_cells++)
  {
     MSG_GERAN_HIGH_3_G("TDS MEAS cell Info: cell_position %d cpid %d rscp %d",
              meas_rsp_ptr->freq[0].cell[num_rpt_cells].cell_position,
              meas_rsp_ptr->freq[0].cell[num_rpt_cells].cpid,
              meas_rsp_ptr->freq[0].cell[num_rpt_cells].rscp);
  }
  num_rpt_cells = 0;

  //gl1_hw_g2t_cleanup( );

  /*only valid to process if there are frequencies!!*/
  if (meas_list_num_freq > 0)
  {
    /*find the returned frequency in the rr_params list*/
    for (meas_list_idx=0;meas_list_idx < meas_list_num_freq;meas_list_idx++)
    {

      for (freq_idx = 0; freq_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs; freq_idx++)
      {
        if( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
        {
          if (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN == meas_rsp_ptr->freq[meas_list_idx].uarfcn)
          {
            uarfcn_found = TRUE;
            /* CR 340670: Update tdd frequency related parameters*/
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.ts0_position = meas_rsp_ptr->freq[meas_list_idx].ts0_position;
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.last_lna_state = meas_rsp_ptr->freq[meas_list_idx].lna_state;
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.last_acq_state = meas_rsp_ptr->freq[meas_list_idx].acq_state;
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.num_of_jds_cells = meas_rsp_ptr->freq[meas_list_idx].num_of_jds_cells;
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.rssi = meas_rsp_ptr->freq[meas_list_idx].rssi;
            break;
          }
        }
      }
      if (!uarfcn_found )
      {
        continue;
      }

      num_rpt_cells = meas_rsp_ptr->freq[meas_list_idx].num_of_cells;

      if (num_rpt_cells >  TDSIRAT_MAX_TDS_CELLS_PER_FREQ)
      {
        num_rpt_cells =  TDSIRAT_MAX_TDS_CELLS_PER_FREQ;

        MSG_GERAN_ERROR_1_G("Too many TDS cells in rpt:%d!", num_rpt_cells);
      }
      /*CR353888 if num of cells are zero, then process them and perfrom ACQ
      next time*/

      /* CR347851*/

      /**
       * JDS: MEAS_RSP may not contain cells which are reported
       * before mark to delete them
       * */
      for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
      {
        boolean found = FALSE;

        tmp_scr_code = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].tds_cell.cell_parameter_id;

        for (rpt_idx=0; rpt_idx<num_rpt_cells; rpt_idx++)
        {
          if (meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].cpid == tmp_scr_code)
          {
            found = TRUE;
            break;
          }
        }
        if (!found)
        {
#ifdef FEATURE_GSM_TDS_DEBUG
          MSG_GERAN_HIGH_2_G("JDS: Marking tmp_scr_code %d,  for deletion on UARFCN %d", tmp_scr_code, l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN);
#endif
          /* No current measurement, so mark for deletion */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2= MIN_TDD_CELL_RSCP_VALUE;


          /* set this deleted cell RSCP average to zero, so its not included in the strongest 8 filter that follows */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;
        }
      }

      /**
       * JDS: Delete marked cells which are in stored list but not in
       * MESA_RSP
       * */
      for (meas_idx=0, new_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
      {
        if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last != MIN_TDD_CELL_RSCP_VALUE)
        {
          if (new_idx != meas_idx)
          {
            temp_cell_params = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx] =
                    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx] = temp_cell_params;

            temp_cell_energy = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx] =
                    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx] = temp_cell_energy;
          }
          new_idx++;
        }
      }

      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = new_idx;

      if (new_idx != meas_idx)
      {
        /* set these 2 params to zero (to cleanup, not necessary) */
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_last2 = MIN_TDD_CELL_RSCP_VALUE;

        MSG_GERAN_HIGH_3_G("Removing old jds cell: uarfcn %d, rscp_avg %d num_cells %d",l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN,
                 l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_avg,l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells);

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;
      }

      /**
       * JDS: MEAS_RSP can return more number of TDD cells Add
       * newly reported cells and update already exisiting cell
       * info */
      /* First step: Add report data to our stored_meas struct */
      for (rpt_idx=0; rpt_idx < num_rpt_cells; rpt_idx++)
      {
        gl1_wcdma_cell_meas_type*  id_rpt_entry_ptr = &l1_sc_wcdma_data_ptr->id_rpt.meas[rpt_idx];

        cell_energy_ptr = &cell_energy;
        cell_energy_ptr_to_ptr = &cell_energy_ptr;

        /* scr_code & cell_param_id have he same type and neither
         * will change going forward so OK to use gl1_wcdma_cell_meas_type
         * to convey TDS information
         */
        id_rpt_entry_ptr->cell.scr_code =
          meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].cpid;

        /*RSCP should not be valid if we have not detected any cells*/
        if((meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].rscp <= 0) &&
            (meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].rscp >= MIN_TDD_CELL_RSCP_VALUE) &&
            (meas_rsp_ptr->freq[meas_list_idx].num_of_jds_cells > 0))
        {
           rscp_valid = TRUE;
           id_rpt_entry_ptr->RSCP = meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].rscp;
        }
        else
        {
#ifdef FEATURE_GSM_TDS_DEBUG
            MSG_GERAN_HIGH_1_G("TDS Ignoring Junk RSCP %d",meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].rscp);
#endif /*FEATURE_GSM_TDS_DEBUG*/


          rscp_valid = FALSE;
          id_rpt_entry_ptr->RSCP = MIN_TDD_CELL_RSCP_VALUE;
        }

        id_rpt_entry_ptr->cell.PN_pos =
           meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].cell_position;


        /**************************************************************/
        /* See if there is already a matching entry for this PSC      */
        /* in the stored_meas database. If there is update the values */
        /* otherwise add the entry.                                   */
        /**************************************************************/
        if ( get_stored_meas_cell_entry(  meas_rsp_ptr->freq[meas_list_idx].uarfcn,
                                         id_rpt_entry_ptr->cell.scr_code,
                                         (uint8) RR_L1_IRAT_RAT_UTRAN_TDD,
                                         NULL,
                                         cell_energy_ptr_to_ptr,
                                         gas_id)
            )
        {

          MSG_GERAN_HIGH_2_G("JDS: Cell exists id_rpt_entry_ptr->cell.scr_code %d rscp_valid %d", id_rpt_entry_ptr->cell.scr_code, rscp_valid);

          if(rscp_valid)
          {
             l1_sc_g2x_filter_treatment(cell_energy_ptr_to_ptr, meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].rscp, meas_list_idx, rpt_idx, RR_L1_IRAT_RAT_UTRAN_TDD, gas_id);

          }
          else
          {
            MSG_GERAN_HIGH_3_G("Received Junk rscp values in tdds meas rpt:%d!", meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].rscp,0,0);
          }
        }
        else
        {
          /* Fill in the diversity value as defaulting to ON */
          id_rpt_entry_ptr->cell.diversity = DIVERSITY_ON;

          add_stored_meas_cell_entry( meas_rsp_ptr->freq[meas_list_idx].uarfcn, id_rpt_entry_ptr, (uint8) RR_L1_IRAT_RAT_UTRAN_TDD, rscp_valid , gas_id);
        } /* End of else the cell is not found in the stored measurements */
      }   /* for( rpt_idx = 0; rpt_idx < l1_sc_wcdma_data_ptr->id_rpt.num_cell_measurements; rpt_idx++ ) */

      /* Second step: Of the cells with valid averages, mark for deletion all
         but the strongest 8 */
      for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
      {
        int16 curr_avg = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg;

        if (curr_avg != MIN_TDD_CELL_RSCP_VALUE)
        {
          if (num_valid < 8)
          {
            num_valid++;

            if (curr_avg < min_avg)
            {
              min_avg = curr_avg;
              min_idx = meas_idx;
            }
          }
          else
          {
            if (curr_avg <= min_avg)
            {
              /* mark this cell for deletion (set RSCP_list = 0) */
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2= MIN_TDD_CELL_RSCP_VALUE;

              /* remove this cell from further consideration */
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;
            }
            else
            {
              uint16 tmp_idx;

              /* mark the min cell for deletion (set RSCP_list = 0) */
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_last2= MIN_TDD_CELL_RSCP_VALUE;

              /* remove min cell from further consideration */
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;

              /* go find new min cell */
              min_avg = curr_avg;

              for (tmp_idx = 0; tmp_idx <= meas_idx; tmp_idx++)
              {
                int16 tmp_avg = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[tmp_idx].wcdma_cell.RSCP_avg;

                if ((tmp_avg != MIN_TDD_CELL_RSCP_VALUE) && (tmp_avg < min_avg))
                {
                  min_avg = tmp_avg;
                  min_idx = tmp_idx;
                }
              }
            }
          }
        }    /* if (curr_avg != 0) */
      }      /* for( meas_idx=0; meas_idx < l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++ ) */

      /* Third step: Delete any items in our stored_meas struct marked for deletion */
      for (meas_idx=0, new_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
      {
        if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last != MIN_TDD_CELL_RSCP_VALUE)
        {
          if (new_idx != meas_idx)
          {
            temp_cell_params = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx] =
                    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx];

             l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx] = temp_cell_params;

            temp_cell_energy = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx] =
                    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx] = temp_cell_energy;
          }
          new_idx++;
        }
      }

      update_stored_meas_jds_cells(meas_rsp_ptr->freq[meas_list_idx].uarfcn, meas_rsp_ptr->freq[meas_list_idx].num_of_jds_cells,
                                   meas_rsp_ptr->freq[meas_list_idx].cell, gas_id);

    }

    /*CR353888: zero number of cells do ACQ again*/
    if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells == 0)
    {
      l1_sc_wcdma_data_ptr->stop_cell_id_srch[freq_idx] = 0;
    }
#ifdef FEATURE_GL1_GPLT
    gplt_acq_meas_result[gas_id] = TRUE;
#endif /* FEATURE_GL1_GPLT*/



  }
  else
  {
    MSG_GERAN_ERROR_1_G("G2T: returned G->T measurement list contained no results %d "
             ,meas_rsp_ptr->num_of_freq);
  }

  if (!uarfcn_found )
  {
    MSG_GERAN_ERROR_3_G("TDS:frequency %d from returned meas list NOT in RR Params -Skipping: num_freq %d, loop %d.",
                         meas_rsp_ptr->freq[meas_list_idx].uarfcn,
                         meas_list_num_freq,
                         meas_list_idx);

    return;
  }

  if ((meas_list_num_freq > 0) && (uarfcn_found))
  {
    /* Generate report for RR */
     gsm_l1_update_rr_report(  &(l1_sc_wcdma_data_ptr->stored_meas[0]),
                                &l1_sc_wcdma_data_ptr->rr_ignore_list,
                                &l1_sc_wcdma_data_ptr->rr_params,
                                &l1_sc_wcdma_data_ptr->rr_report,
                                gas_id
                            );
    MSG_GERAN_HIGH_1_G("Sending Measurement Result to RR freq_idx %d", freq_idx);
    l1_sc_send_tds_cell_search_results_to_rr( (void *) freq_idx, gas_id );
    /* Send report to RR */
    //l1_sc_wcdma_send_rr_report(NULL);

    /* generate log */
    l1_sc_wcdma_log_known_list(gas_id);
  }
} /* l1_sc_idle_tds_meas_results */

/*===========================================================================

FUNCTION l1_sc_ded_tds_meas_results

DESCRIPTION
  This function is called to from the L1 task to process the
  results of the most recent TDS reconfirmation search.
  This function is based on l1_sc_process_wcdma_recon_srch_results()

  1- The results are read from the recon_rpt structure.

  2- If reported EcNo is below the minimum for the cell to be valid then
     cell id search is restarted.

  3- If the RSCP has fluctuated compared to the last measurement then cell
     id search is restarted.

  4- All cells are marked for deletion except the strongest 8.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  l1_sc_ded_tds_meas_results( const tdsirat_freq_list_type*  meas_rsp_ptr, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16 rpt_idx, meas_idx;

  uint16 tmp_scr_code;
  uint16 num_rpt_cells;

  uint16 new_idx = 0;
  uint16 min_idx = 0;
  uint16 num_valid = 0;
  int16 min_avg = MIN_TDD_CELL_RSCP_VALUE;

  uint32 freq_idx = 0;
  uint8 meas_list_num_freq = meas_rsp_ptr->num_of_freq;
  uint8 meas_list_idx;
  gsm_l1_lte_wcdma_cell_params_type       temp_cell_params = {{0, 0, 0}};
  t_gsm_l1_lte_wcdma_cell_energy_details  temp_cell_energy = {{0, 0}};
  boolean uarfcn_found = FALSE;

  /* CR347851*/
  /* In TDD, to avoid junk rscp values */
  boolean rscp_valid = TRUE;
  t_gsm_l1_lte_wcdma_cell_energy_details  cell_energy, *cell_energy_ptr = NULL, **cell_energy_ptr_to_ptr = NULL;

  MSG_GERAN_HIGH_3_G("TDS MEAS FREQ Info: uarfcn %d rssi %d jds_cells %d", meas_rsp_ptr->freq[0].uarfcn,meas_rsp_ptr->freq[0].rssi,
           meas_rsp_ptr->freq[0].num_of_jds_cells);
  MSG_GERAN_HIGH_3_G("TDS MEAS  Info: meas_rsp_ptr->freq[0].acq_state %d, meas_rsp_ptr->freq[0].lna_state %d, meas_rsp_ptr->freq[0].ts0_position %d",
               meas_rsp_ptr->freq[0].acq_state,meas_rsp_ptr->freq[0].lna_state,meas_rsp_ptr->freq[0].ts0_position);
  for(num_rpt_cells = 0; num_rpt_cells < meas_rsp_ptr->freq[0].num_of_cells; num_rpt_cells++)
  {
     MSG_GERAN_HIGH_3_G("TDS MEAS cell Info: cell_position %d cpid %d rscp %d",
              meas_rsp_ptr->freq[0].cell[num_rpt_cells].cell_position,
              meas_rsp_ptr->freq[0].cell[num_rpt_cells].cpid,
              meas_rsp_ptr->freq[0].cell[num_rpt_cells].rscp);
  }
  num_rpt_cells = 0;
 
 
  /*only valid to process if there are frequencies!!*/
  if (meas_list_num_freq > 0)
  {
    /*find the returned frequency in the rr_params list*/
    for (meas_list_idx=0;meas_list_idx < meas_list_num_freq;meas_list_idx++)
    {

      for (freq_idx = 0; freq_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs; freq_idx++)
      {

        if( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
        {
          if (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN == meas_rsp_ptr->freq[meas_list_idx].uarfcn)
          {
            uarfcn_found = TRUE;
            /* CR 340670: Update tdd frequency related parameters*/
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.ts0_position = meas_rsp_ptr->freq[meas_list_idx].ts0_position;
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.last_lna_state = meas_rsp_ptr->freq[meas_list_idx].lna_state;
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.last_acq_state = meas_rsp_ptr->freq[meas_list_idx].acq_state;
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.num_of_jds_cells = meas_rsp_ptr->freq[meas_list_idx].num_of_jds_cells;
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.rssi = meas_rsp_ptr->freq[meas_list_idx].rssi;
            break;
          }
        }
        if (freq_idx == (l1_sc_wcdma_data_ptr->rr_params.num_freqs -1) )
        {
          if (!uarfcn_found )
          {
            MSG_GERAN_ERROR_3_G("G2T:frequency %d from returned meas list NOT in RR Params -Skipping: num_freq %d, loop %d.",
                   meas_rsp_ptr->freq[meas_list_idx].uarfcn,
                   meas_list_num_freq,
                   meas_list_idx);

            continue;
          }
        }
      }

      num_rpt_cells = meas_rsp_ptr->freq[meas_list_idx].num_of_cells;

      if (num_rpt_cells >  TDSIRAT_MAX_TDS_CELLS_PER_FREQ)
      {
        num_rpt_cells =  TDSIRAT_MAX_TDS_CELLS_PER_FREQ;

        MSG_GERAN_ERROR_1_G("Too many TDS cells in rpt:%d!", num_rpt_cells);
      }
      /*CR353888 if num of cells are zero, then process them and perfrom ACQ
      next time*/

      /* CR347851*/
      /**
       * JDS: MEAS_RSP may not contain cells which are reported
       * before do mark to delete them*/
      for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
      {
        boolean found = FALSE;

        tmp_scr_code = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].tds_cell.cell_parameter_id;

        for (rpt_idx=0; rpt_idx<num_rpt_cells; rpt_idx++)
        {
          if ((meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].cpid == tmp_scr_code) && (meas_rsp_ptr->freq[meas_list_idx].num_of_cells != 0))
          {
            found = TRUE;
            break;
          }
        }
        if (!found)
        {

          MSG_GERAN_HIGH_1_G("JDS: Marking tmp_scr_code %d for deletion", tmp_scr_code);

          /* No current measurement, so mark for deletion */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2= MIN_TDD_CELL_RSCP_VALUE;

          /* set this deleted cell RSCP average to zero, so its not included in the strongest 8 filter that follows */
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;
        }
      }

      /**
       * JDS: Delete marked cells which are in stored list but not in
       * MESA_RSP
       * */
      for (meas_idx=0, new_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
      {
        if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last != MIN_TDD_CELL_RSCP_VALUE)
        {
          if (new_idx != meas_idx)
          {
             temp_cell_params = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx] =
                    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx] = temp_cell_params;

            temp_cell_energy = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx] =
                    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx] = temp_cell_energy;
          }
          new_idx++;
        }
      }

      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = new_idx;

      if (new_idx != meas_idx)
      {
        /* set these 2 params to zero (to cleanup, not necessary) */
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_last2 = MIN_TDD_CELL_RSCP_VALUE;

        MSG_GERAN_HIGH_3_G("Removing old jds cell: uarfcn %d, rscp_avg %d num_cells %d",l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN,
                 l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_avg,l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells);

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx].tds_cell.cell_parameter_id = 0;

      }

      /**
       * JDS: MEAS_RSP can return more number of TDD cells Add
       * newly reported cells and update already exisiting cell
       * info */
      /* First step: Add report data to our stored_meas struct */
      for (rpt_idx=0; rpt_idx < num_rpt_cells; rpt_idx++)
      {
        gl1_wcdma_cell_meas_type*  id_rpt_entry_ptr = &l1_sc_wcdma_data_ptr->id_rpt.meas[rpt_idx];

        cell_energy_ptr = &cell_energy;
        cell_energy_ptr_to_ptr = &cell_energy_ptr;

        /* scr_code & cell_param_id have he same type and neither
         * will change going forward so OK to use gl1_wcdma_cell_meas_type
         * to convey TDS information
         */
        id_rpt_entry_ptr->cell.scr_code =
          meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].cpid;

        if((meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].rscp <= 0) &&
            (meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].rscp >= MIN_TDD_CELL_RSCP_VALUE) &&
            (meas_rsp_ptr->freq[meas_list_idx].num_of_jds_cells > 0))
        {
           rscp_valid = TRUE;
           id_rpt_entry_ptr->RSCP = meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].rscp;
        }
        else
        {

#ifdef FEATURE_GSM_TDS_DEBUG
            MSG_GERAN_HIGH_1_G("TDS Ignoring Junk RSCP %d",meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].rscp);
#endif /*FEATURE_GSM_TDS_DEBUG*/

          rscp_valid = FALSE;
          id_rpt_entry_ptr->RSCP = MIN_TDD_CELL_RSCP_VALUE;
        }

        id_rpt_entry_ptr->cell.PN_pos =
           meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].cell_position;

        /**************************************************************/
        /* See if there is already a matching entry for this PSC      */
        /* in the stored_meas database. If there is update the values */
        /* otherwise add the entry.                                   */
        /**************************************************************/
        if ( get_stored_meas_cell_entry(  meas_rsp_ptr->freq[meas_list_idx].uarfcn,
                                         id_rpt_entry_ptr->cell.scr_code,
                                         (uint8) RR_L1_IRAT_RAT_UTRAN_TDD,
                                         NULL,
                                         cell_energy_ptr_to_ptr,
                                         gas_id)
            )
        {

          MSG_GERAN_HIGH_1_G("JDS: Cell exists id_rpt_entry_ptr->cell.scr_code %d ", id_rpt_entry_ptr->cell.scr_code);

          if(rscp_valid)
          {
             l1_sc_g2x_filter_treatment(cell_energy_ptr_to_ptr, meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].rscp, meas_list_idx, rpt_idx, RR_L1_IRAT_RAT_UTRAN_TDD, gas_id);

          }
          else
          {
            MSG_GERAN_ERROR_1_G("Received Junk rscp values in tdds meas rpt:%d!", meas_rsp_ptr->freq[meas_list_idx].cell[rpt_idx].rscp);
          }
        }
        else
        {
          /* Fill in the diversity value as defaulting to ON */
          id_rpt_entry_ptr->cell.diversity = DIVERSITY_ON;

          add_stored_meas_cell_entry( meas_rsp_ptr->freq[meas_list_idx].uarfcn, id_rpt_entry_ptr, (uint8) RR_L1_IRAT_RAT_UTRAN_TDD, rscp_valid, gas_id );
        } /* End of else the cell is not found in the stored measurements */
      }   /* for( rpt_idx = 0; rpt_idx < l1_sc_wcdma_data_ptr->id_rpt.num_cell_measurements; rpt_idx++ ) */
}

      min_idx = MAX_WCDMA_CELLS_PER_UARFCN - 1;

      /* Second step: Of the cells with valid averages, mark for deletion all
         but the strongest 8 */
      for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
      {
        int16 curr_avg = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg;

        if (curr_avg != MIN_TDD_CELL_RSCP_VALUE)
        {
          if (num_valid < 8)
          {
            num_valid++;

            if (curr_avg < min_avg)
            {
              min_avg = curr_avg;
              min_idx = meas_idx;
            }
          }
          else
          {
            if (curr_avg < min_avg)
            {
              /* mark this cell for deletion (set RSCP_list = 0) */
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2= MIN_TDD_CELL_RSCP_VALUE;

              /* remove this cell from further consideration */
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;
            }
            else
            {
              uint16 tmp_idx;

              /* mark the min cell for deletion (set RSCP_list = 0) */
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_last = MIN_TDD_CELL_RSCP_VALUE;
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_last2= MIN_TDD_CELL_RSCP_VALUE;

              /* remove min cell from further consideration */
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;

              /* go find new min cell */
              min_avg = curr_avg;

              for (tmp_idx = 0; tmp_idx <= meas_idx; tmp_idx++)
              {
                int16 tmp_avg = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[tmp_idx].wcdma_cell.RSCP_avg;

                if ((tmp_avg != MIN_TDD_CELL_RSCP_VALUE) && (tmp_avg < min_avg))
                {
                  min_avg = tmp_avg;
                  min_idx = tmp_idx;
                }
              }
            }
          }
        }    /* if (curr_avg != 0) */
      }      /* for( meas_idx=0; meas_idx < l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++ ) */

      /* Third step: Delete any items in our stored_meas struct marked for deletion */
      for (meas_idx=0, new_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
      {
        if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last != MIN_TDD_CELL_RSCP_VALUE)
        {
          if (new_idx != meas_idx)
          {
            temp_cell_params = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx] =
                    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx] = temp_cell_params;

            temp_cell_energy = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx] =
                    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx];

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx] = temp_cell_energy;
          }
          new_idx++;
        }
      }


      MSG_GERAN_ERROR_3_G("numofcells %d, newidx %d, measidx %d", l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells, new_idx, meas_idx);

      update_stored_meas_jds_cells(meas_rsp_ptr->freq[meas_list_idx].uarfcn, meas_rsp_ptr->freq[meas_list_idx].num_of_jds_cells,
                                   meas_rsp_ptr->freq[meas_list_idx].cell, gas_id);

    /*CR353888 zero num of cells, do ACQ again*/
    if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells == 0)
    {
      l1_sc_wcdma_data_ptr->stop_ded_cell_id_srch[freq_idx] = 0;
    }
#ifdef FEATURE_GL1_GPLT
  gplt_acq_meas_result[gas_id] = TRUE;
#endif /* FEATURE_GL1_GPLT*/

  }
  else
  {
    MSG_GERAN_ERROR_1_G("G2T: returned G->T measurement list contained no results %d "
             ,meas_rsp_ptr->num_of_freq);
  }
  
  l1_g2t_gap_end(gas_id);

  if ((meas_list_num_freq > 0) && (uarfcn_found))
  {
    /* Generate report for RR */
    gsm_l1_update_rr_report( &( l1_sc_wcdma_data_ptr->stored_meas[0] ),
                                &l1_sc_wcdma_data_ptr->rr_ignore_list,
                                &l1_sc_wcdma_data_ptr->rr_params,
                                &l1_sc_wcdma_data_ptr->rr_report,
                                gas_id
                            );

    MSG_GERAN_HIGH_1_G("Sending Measurement Result to RR freq_idx %d", freq_idx);
    l1_sc_send_tds_cell_search_results_to_rr( (void *) freq_idx, gas_id );
    /* Send report to RR */
    //l1_sc_wcdma_send_rr_report(NULL);

    /* generate log */
    l1_sc_wcdma_log_known_list(gas_id);
  }
} /* l1_sc_ded_tds_meas_results */

#endif /* FEATURE_GSM_TDS*/

/*===========================================================================

FUNCTION l1_set_serving_RLA_C

DESCRIPTION
  Used to setup RLA_C value
DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void l1_set_serving_RLA_C(dBx16_T local_serving_RLA_C, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  l1_sc_wcdma_data_ptr->serving_RLA_C = local_serving_RLA_C;
}

/*===========================================================================

FUNCTION l1_get_serving_RLA_C

DESCRIPTION
  Used to get RLA_C value
DEPENDENCIES
  none

RETURN VALUE
  dBx16 RLA_C value

SIDE EFFECTS
  none

===========================================================================*/
dBx16_T l1_get_serving_RLA_C(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  return(l1_sc_wcdma_data_ptr->serving_RLA_C);
}

#ifdef FEATURE_GSM_TDS

/*===========================================================================

FUNCTION update_stored_meas_jds_cells

DESCRIPTION
  In JDS, TL1 provides number of JDS cells in ACQ_RSP/MEAS_RSP which GL1 should
  report to RR. In the current implementation, GL1 stores TDD cell information
  received in every ACQ_RSP/MEAS_RSP and reports to RR. It can be possible that
  first number of jds cells in stored meas are not actually reported in ACQ_RSP/MEAS_RSP.
  For example: stored_meas { 1, 2, 3, 4, 5}  MEAS_RSP {4,5, 3, 2, 1}  and
  num_jds_Cells = 2. In current implementation, GL1 will report {1, 2} to RR.

  This function reshuffles stored meas to bring jds cells information in the
  start of stored_mes. After this function call, stored_meas will become
  {4,5, 1,2,3} and GL1 will report {4,5} to RR.


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void  update_stored_meas_jds_cells( uint32                           UARFCN,
                                           uint8                            num_jds_cells,
                                           const tdsirat_cell_type*         cell_params_ptr,
                                           gas_id_t                         gas_id
                                          )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint8 cell_idx = 0, stored_cell_idx = 0, freq_idx = 0, stored_num_cells = 0;
  uint8 i = 0;
  gsm_l1_lte_wcdma_cell_params_type *stored_cell_param_ptr = NULL, temp_cell_param = {{0, 0, 0}};
  t_gsm_l1_lte_wcdma_cell_energy_details  *stored_cell_energy_ptr = NULL, temp_cell_energy_param = {{0, 0}};

  /* Find freq idx in stored_meas matches to received ACQ_RSP/MEAS_RSP*/
  for(freq_idx = 0; freq_idx < MAX_WCDMA_FREQS; freq_idx ++)
  {
    if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN == UARFCN)
    {
      /* Store number of cells, cell parameter, energy parameter from
         stored_meas*/
      stored_num_cells       = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;
      stored_cell_param_ptr  = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params;
      stored_cell_energy_ptr = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy;
      break;
    }
  }

  if((freq_idx == MAX_WCDMA_FREQS) || (stored_num_cells == 0))
  {
    MSG_GERAN_ERROR_2_G("UARFCN %d or num_cells %d not found in stored_meas", UARFCN, stored_num_cells );
    return;
  }

  if(num_jds_cells > TDSIRAT_MAX_TDS_CELLS_PER_FREQ)
  {  num_jds_cells = TDSIRAT_MAX_TDS_CELLS_PER_FREQ; }

  /* For each JDS cell in ACQ_RSP/MEAS_RSP, find index in stored meas based upon CPID*/
  for (cell_idx = 0; (cell_idx < num_jds_cells)&&(cell_idx < TDSIRAT_MAX_TDS_CELLS_PER_FREQ); cell_idx++)
  {
    for(i = stored_cell_idx;  i < stored_num_cells; i++)
    {
      if(stored_cell_param_ptr[i].tds_cell.cell_parameter_id == cell_params_ptr[cell_idx].cpid)
      {
        /* stored meas idx doesn't match with ACQ_RSP/MEAS_RSP cell idx
           then swap cell and energy info*/
        if(stored_cell_idx != i)
        {
          temp_cell_param = stored_cell_param_ptr[i];
          stored_cell_param_ptr[i] = stored_cell_param_ptr[stored_cell_idx];
          stored_cell_param_ptr[stored_cell_idx] = temp_cell_param;

          temp_cell_energy_param = stored_cell_energy_ptr[i];
          stored_cell_energy_ptr[i] = stored_cell_energy_ptr[stored_cell_idx];
          stored_cell_energy_ptr[stored_cell_idx] = temp_cell_energy_param;

        }
        stored_cell_idx++;
        break;
      }
    }
  }
  return;
}

#endif /*FEATURE_GSM_TDS*/


/*
** With the addition of DED LTE searches x2 Qsearch_C values now exist, 1 for each rat
*/
void l1_sc_gsm_l1_set_ecio_rssi_levels( uint16 qsearch_W, uint16 qsearch_L, uint16 ecode_both, gas_id_t gas_id )
{
  gsm_l1_set_ecio_rssi_levels( SRCH_WCDMA_IDX, qsearch_W, ecode_both, gas_id);
  gsm_l1_set_ecio_rssi_levels( SRCH_LTE_IDX,   qsearch_L, ecode_both, gas_id);

}

/*===========================================================================

FUNCTION l1_sc_stop_cm_g2l

DESCRIPTION
   When cell update occurs, stop any existing cm g2l measurement.
   Restarts with new list, freq list indices may have changed between gaps.
   freq data maybe nolonger valid.


DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void l1_sc_stop_cm_g2l(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req  = 0;
  l1_sc_wcdma_data_ptr->rr_params.cntTimed_Cnf  = 0;
  l1_sc_wcdma_data_ptr->rr_params.use_next_freq = FALSE;
#ifdef  FEATURE_GSM_TO_LTE
  l1_sc_wcdma_data_ptr->g2l_inhibit_next_update = FALSE;
#endif
}

/*===========================================================================

FUNCTION get_msg_rat_count

DESCRIPTION
   count the number of rat_type arfcns in the priority list from RR

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

#ifdef FEATURE_SGLTE_G2X_CONN_MODE_RESEL

static uint16 get_msg_rat_count( rr_l1_irat_frequency_priority_list_t *pList, rr_l1_irat_rat_e rat_type , gas_id_t gas_id )
{
   uint32 i;
   uint16 num_entries;
   uint16 num_present = 0;

   num_entries= pList->count;

   if( num_entries > MAX_L1_PRIORITY_ENTRIES )
   {   num_entries = MAX_L1_PRIORITY_ENTRIES;}

   for( i=0; i < num_entries;i++)
   {
      if(pList->entries[i].rat == rat_type )
      {
         num_present++;
      }
   }

   MSG_GERAN_HIGH_2_G("DED G2X: for rat type %d rat_freq_count=%d" , rat_type, num_present );

   return num_present;
}

#endif

/*===========================================================================

FUNCTION l1_sc_specific_irat_freq_update

DESCRIPTION
   Handle message from RR MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ to force a G2X measurement to be the next measurement
   on specific uarfcn+rat prior to the reselection timer expiry, nominally 5 secs.
   Additionally if timer_resel == 5 secs modifies the cycle ticks to force extra measurements
   to occur within that period.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_sc_specific_irat_freq_update(mph_specific_irat_freq_update_req_T *pMsg, gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    uint16  ForcedFreqIdx;

    /* we already know all the details from update list, so only need rat and arfcn */

    rr_l1_irat_rat_e reselRat    = pMsg->rat;
    uint32 ForcedArfcn           = pMsg->arfcn;  /* uint32 */

    /* Wait for the next frame tick */
    gl1_drx_require_and_wait_for_next_tick_conditional(gas_id);

    ForcedFreqIdx = l1_sc_find_freq_index(ForcedArfcn, reselRat, FALSE , gas_id);

    if((INVALID_IDX != ForcedFreqIdx) && (ForcedFreqIdx < MAX_WCDMA_FREQS))  /* KW fix */
    {

      uint32  tempFrame;
      uint16  forced_offset;
      /*
      ** check FG2X not already running
      */
      if(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_NULL)
      {

        l1_sc_wcdma_data_ptr->rr_params.ForcedMeas    = FORCE_MEAS_ACTIVE;
        l1_sc_wcdma_data_ptr->rr_params.ForcedFreqIdx = ForcedFreqIdx;
        l1_sc_wcdma_data_ptr->rr_params.ForcedArfcn   = ForcedArfcn;

        tempFrame         = (pMsg->resel_timer * G2X_FRAMES_PER_SEC) - G2X_FORCED_FN_MARGIN;   /* ensure measurement within the next N frames. */

        l1_sc_wcdma_data_ptr->rr_params.ForcedExpiryFrame =  ADD_FN(gl1_get_FN( gas_id), tempFrame );

        l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index = 0;   /* reset on first */
 
        MSG_GERAN_HIGH_4_G("FG2X: Reselect measurement requested for uarfcn=%d idx=%d resel_timer=%d before FN=%d" ,
                              ForcedArfcn,
                              ForcedFreqIdx ,
                              pMsg->resel_timer,
                              l1_sc_wcdma_data_ptr->rr_params.ForcedExpiryFrame
                          );

        /*
        ** reduce cycle interval by 1/2 for forced measurement for LTE id cycle and W reconfirm_cycle when 5 sec timer is used.
        */
        if(pMsg->resel_timer <= G2X_FORCED_RESEL_5_SECS)   /* if > 5secs then existing cycle_tick is sufficient */
        {
           l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_cycle  = (l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_cycle+1) >> 1;
           l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte     = (l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte+1) >> 1;

           forced_offset = g2x_forced_update_tick_offset_with_speedup(l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms);
        }
        else
        {
           forced_offset = g2x_forced_update_tick_offset(l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms);

           if( ( l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_WAIT)
             &&( l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset == ForcedFreqIdx)
             &&(!l1_sc_wcdma_data_ptr->wcdma_srch.abort)
             )
           {
             /* 
             ** if the forced arfcn is already waiting,revert and delay using updated cycle_tick with forced_offset 
             */
             l1_sc_set_gtow_srch_state_null (gas_id);
           }
        }
       /*
        * start forced idle measurement on tick+4, so next measurement is at +1.8secs and +4.2secs
        * the aim here is to:
        *     1) fully populate the 3-tap filter that is now used.
        *     2) do the last measurement at ~1sec before timer expiry.
        */
           l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[ForcedFreqIdx]        = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + forced_offset;
           l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[ForcedFreqIdx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + forced_offset;
       }
       else 
       if(l1_sc_wcdma_data_ptr->rr_params.ForcedFreqIdx != ForcedFreqIdx)
       {
         /* 
         ** for 2nd and any subsequent requests to a different arfcn, 
         ** update the cycle tick with same parameters.
         */
         if(pMsg->resel_timer <= G2X_FORCED_RESEL_5_SECS)   /* if > 5secs then existing cycle_tick is sufficient */
         {
            forced_offset = g2x_forced_update_tick_offset_with_speedup(l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms);
         }
         else
         {
            forced_offset = g2x_forced_update_tick_offset(l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms);
         }
         
         l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[ForcedFreqIdx]        = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + forced_offset;;
         l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[ForcedFreqIdx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + forced_offset;;
       }
    
      /*
      ** record each freq idx that's being forced, the first modifies the cycle_tick above
      */
      l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index |= (1<<ForcedFreqIdx);
      /* copy so that it can be reset */
      l1_sc_wcdma_data_ptr->rr_params.bForcedReq2Index = l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index;

    }
    else
    {
      MSG_GERAN_ERROR_1_G("FG2X: Reselect measurement request for uarfcn=%d not found in current cell list" , ForcedArfcn);
    }
} /* l1_sc_specific_irat_freq_update */


/*===========================================================================

FUNCTION l1_sc_find_freq_index

DESCRIPTION
   Find the freq index in cell list for requested forced measurement uarfcn.

   If Arfcn is not found and anyIndexForRat is true find any index for the same rat.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

uint16 l1_sc_find_freq_index( uint32 findArfcn , rr_l1_irat_rat_e findRat , boolean anyIndexForRat , gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
    uint16 freq_idx;

    for (freq_idx=0; freq_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs; freq_idx++)
    {
        /* euarfcn is currently unit16 but will be uint32 very soon */
        if(  (findArfcn ==(uint32)l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN)
           &&(findRat          == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat)
          )
        {
           break;
        }
    }

    if(freq_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs)
    {
        return freq_idx;
    }

    if(FALSE==anyIndexForRat)
    {
        return INVALID_IDX;
    }

    /* find first for this rat, for LTE recovery */
    for (freq_idx=0; freq_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs; freq_idx++)
    {
        if( findRat == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat)
        {
           break;
        }
    }

    if(freq_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs)
    {
        return freq_idx;
    }

    return INVALID_IDX;

} /* l1_sc_find_freq_index */

/*===========================================================================

FUNCTION l1_sc_forced_meas_expiry

DESCRIPTION
   If the forced measurement does not occur on the specified uarfcn within the specified timer_resel period.
   Then report 0 cells to RR to stop the reselection process.
   On expiry restore the nominal cycle ticks to their original values for the given pg_mfrms

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void l1_sc_forced_meas_expiry(boolean abort, gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    uint32  i;
    uint32  frame_now = gl1_get_FN(gas_id);

    /*
    ** check expiry in any state and send response if required
    */
    if( (l1_sc_wcdma_data_ptr->rr_params.ForcedMeas & FORCE_MEAS_ACTIVE )
      &&(IS_FRAME_NUM_LATER_OR_EQUAL(frame_now, l1_sc_wcdma_data_ptr->rr_params.ForcedExpiryFrame) || abort))
    {
        MSG_GERAN_MED_4_G("FG2X: Forced period expired restore normal measurement period FN=%d ForcedMeas=0x%X bMeas=0x%X abort=%d",
                                                      frame_now , 
                                                      l1_sc_wcdma_data_ptr->rr_params.ForcedMeas, 
                                                      l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index,
                                                      abort);

        if(!(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas & FORCE_MEAS_COMPLETED))
        {
            /*
            ** we did not do a measurement within the reselect period.
            */
            uint16 freq_index = l1_sc_wcdma_data_ptr->rr_params.ForcedFreqIdx;

            /*
            ** Send response indicating 0 cells measured to stop RR reselection
            ** due to lack of a new measurement
            **
            */
            MSG_GERAN_HIGH_3_G("FG2X: Forced measurement expiry no meas on Arfcn=%d, rat=%d FN=%d",
                                 l1_sc_wcdma_data_ptr->rr_params.ForcedArfcn,
                                 l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_index].rat,
                                 frame_now);

            L1_send_not_found_MPH_SURROUND_IRAT_MEAS_IND( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_index].rat,
                                                          l1_sc_wcdma_data_ptr->rr_params.ForcedArfcn,
                                                          gas_id
                                                        );
            l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index &= ~(1<<freq_index);   /* reset and check if any others exist */

            for( i=0 ; i < l1_sc_wcdma_data_ptr->rr_params.num_freqs; i++ )
            {
               if(  (l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index & ( 1<< i))  
                  &&(l1_sc_wcdma_data_ptr->rr_params.freq_list[i].rat != RR_L1_IRAT_RAT_UNDEFINED))
               {
                   L1_send_not_found_MPH_SURROUND_IRAT_MEAS_IND( l1_sc_wcdma_data_ptr->rr_params.freq_list[i].rat,
                                                                 l1_sc_wcdma_data_ptr->rr_params.freq_list[i].arfcn.UARFCN,
                                                                 gas_id );
               }
            }
        }
        l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index = 0;
        l1_sc_wcdma_data_ptr->rr_params.ForcedMeas        = FORCE_MEAS_NULL;  /* finished after expiry */

        /*
        ** restore original cycle times
        */
        if(l1_sc_wcdma_data_ptr->rr_params.num_W_freqs)
        {
           gsm_l1_set_wcdma_search_cycles( l1_sc_wcdma_data_ptr->rr_params.num_W_freqs, l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms , gas_id);
        }
        gsm_l1_set_lte_search_cycles(l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms, gas_id);
    }
} /* l1_sc_forced_meas_expiry */


/*===========================================================================

FUNCTION l1_sc_forced_meas_event

DESCRIPTION
    Responds to the forced measurement FORCE_MEAS_COMPLETED event
   After 2 measurements restores the nominal cycle ticks to their original values for the given pg_mfrms

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_sc_forced_meas_event(uint16 freq_idx , g2x_forced_meas_e measEvent, gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
    
 
    if(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas & FORCE_MEAS_ACTIVE)
    {
        /*
        ** for multiple FG2X requests requests, clear bit for this freq_idx
        */
        l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index &= ~(1<<freq_idx);
 
        if(freq_idx == l1_sc_wcdma_data_ptr->rr_params.ForcedFreqIdx)
        {
          l1_sc_wcdma_data_ptr->rr_params.ForcedMeas |= measEvent;

          if( measEvent == FORCE_MEAS_COMPLETED)
          {
             uint32 frames_to_expiry;

             frames_to_expiry = SUB_FN(l1_sc_wcdma_data_ptr->rr_params.ForcedExpiryFrame , gl1_get_FN( gas_id));
             
             l1_sc_wcdma_data_ptr->rr_params.ForcedMeas++;
             /*
             ** 2 extra G2X measurements are attempted within the 5 secs
             */
             if( (l1_sc_wcdma_data_ptr->rr_params.ForcedMeas & FORCE_MEAS_COUNT_MASK) >= 2)
             {
                 /*
                 ** restore original cycle tick
                 */
                 if(l1_sc_wcdma_data_ptr->rr_params.num_W_freqs)
                 {
                    gsm_l1_set_wcdma_search_cycles( l1_sc_wcdma_data_ptr->rr_params.num_W_freqs, l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms , gas_id);
                 }
                 gsm_l1_set_lte_search_cycles(l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms, gas_id);


                 l1_sc_wcdma_data_ptr->rr_params.ForcedMeas=FORCE_MEAS_NULL;  /* finished after expiry */
             }

             MSG_GERAN_HIGH_3_G("FG2X: Forced measurement for Arfcn=%d ForcedMeas=0x%X bMeas=0x%X" ,
                                                 l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN, 
                                                 l1_sc_wcdma_data_ptr->rr_params.ForcedMeas,
                                                 l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index );


             if( frames_to_expiry > G2X_FRAMES_FOR_5SEC) 
             {
                 /*
                 ** Stale measurements: when sub1 measurements are blocked by sub2 MO/MT call
                 ** if expiry is still > 5secs away restart FORCE_MEAS_ACTIVE
                 ** to ensure that on expiry a zero cell response is sent to rr
                 ** when no new measurement has been made in the last 5 secs.
                 */
                 l1_sc_wcdma_data_ptr->rr_params.ForcedMeas = FORCE_MEAS_ACTIVE;  /* restart */
                 /*
                 ** reset bmask for those requested
                 */
                 l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index = l1_sc_wcdma_data_ptr->rr_params.bForcedReq2Index;

                 MSG_GERAN_MED_1_G("FG2X: Reset requested bForcedMeas2Index=0x%X" ,l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index);
             }
          }
       }
    }
} /* l1_sc_forced_meas_event */

/*===========================================================================

FUNCTION g2x_forced_update_tick_offset

DESCRIPTION

   Set the forced tick offset for the start the next measurement.

   When reselect_timer > 5secs, the existing cycle tick is approx 5 secs.
   Start tick offset is adjusted here to take into account the pg_mfrms being used.
   The aim is to ensure the last measurement tick occurs ~1sec ( as best we can)
   before the timer expiry, (10,15,20secs). So this just skews the initial tick offset
   to align the measurement to the expiry time by stretching the current cycle.
   At the higher pg_mfrms intervals we have limited resolution to achieve this.

   e.g Reselect_Timer expiry         +20secs
       Last measurement scheduled at +19secs from now.

    cycle tick          < 5secs >
    ___|_______________|__________|__________|__________|__________|

      time now             +20secs                       expiry
       ___|________________________________________________|________

    initial skew offset

              > 5secs
       ___|____________|_


DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

static uint16 g2x_forced_update_tick_offset(uint16 pg_mfrms )
{
    uint16 forced_interval;

    switch(pg_mfrms)    /* 2..9 */
    {
        case 0:
        case 1:
        case 2:
          forced_interval =  12;
        break;

        case 3:
          forced_interval =  8;
        break;

        case 4:
          forced_interval =  6;
        break;

        case 5:
          forced_interval =  5;
        break;

        case 6:
        case 7:
           forced_interval = 4;
        break;

        case 8:
        case 9:
           forced_interval = 3;
        break;
        default:
          forced_interval =  12;
        break;

    }

    return forced_interval;

}
/*===========================================================================

FUNCTION g2x_forced_update_tick_offset_with_speedup

DESCRIPTION
   set the forced tick offset for the start the next measurement.
   When relect timer is 5 secs the cycle tick is reduced by 1/2 to allow
   more measurements cycle ticks to occur.
   Aim is to schedule 2 measurements before 5sec timer expiry.
   1st after 2 sec approx,
   2nd after 4 sec approx

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static uint16 g2x_forced_update_tick_offset_with_speedup(uint16 pg_mfrms )
{
    uint16 forced_interval = 1;

    switch(pg_mfrms)    /* 2..9 */
    {
        case 0:
        case 1:
        case 2:
          forced_interval =  4;     /* +1.88 secs */
        break;

        case 3:
          forced_interval =  3;
        break;

        case 4:
          forced_interval =  2;
        break;

        case 5:
          forced_interval =  2;
        break;

        case 6:
        case 7:
           forced_interval = 1;
        break;

        case 8:
        case 9:
           forced_interval = 1;
        break;
        default:
        break;

    }

    return forced_interval;

}

/*===========================================================================

FUNCTION l1_sc_is_reconf_next_pch_possible

DESCRIPTION
  Returns TRUE if there are W neighbour cells requiring a G2W reconfirmation on
  the next PCH. 

  If a reconf occurs then additional processing (an RF ENTER MODE etc) will occur 
  during the next warmup ISR. Therefore, some extra time should be allowed for 
  the next warmup.

  The final decision to perform the W reconf is made during the warmup ISR. It 
  is possible for this function to return TRUE and for subsequently the 
  reconfirmation not to be performed.

DEPENDENCIES
  None

RETURN VALUE
  Boolean - TRUE if there are W neighbour cells requiring a G2W reconfirmation on
  the next PCH. 

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_sc_is_reconf_next_pch_possible(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint32             next_page_cycle_tick  = 0;
  boolean            reconf_possible       = FALSE;
  int                i                     = 0;

  gl1_irat_mutex_lock( gas_id);

  next_page_cycle_tick = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + 1;

  for (i=0; i<l1_sc_wcdma_data_ptr->rr_params.num_freqs; i++)
  {
    if(l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[i] <= next_page_cycle_tick &&
       l1_sc_wcdma_data_ptr->stored_meas[i].num_cells > 0)
    {
      /* Found at least one neighbour cell requiring a W reconf */
      reconf_possible = TRUE;
      break;
    }
  }

  gl1_irat_mutex_unlock( gas_id);

  return reconf_possible;
}

/* EOF */
