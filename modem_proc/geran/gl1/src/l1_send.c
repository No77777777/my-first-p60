/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             l1_send.c

GENERAL DESCRIPTION
   This module contains functions common to L1 task and L1 ISR.

EXTERNALIZED FUNCTIONS

  L1_send_PH_DATA_IND
    Procedure to

  L1_SEND_CB_SCHED_EXPIRY_IND
    Procedure to

  L1_SEND_CB_MISSING_BLOCK_IND
    Procedure to

  L1_SEND_CB_SKIP_CNF
    Procedure to

  L1_send_PH_RANDOM_ACCESS_CNF
    Procedure to

  L1_send_MPH_BLOCK_QUALITY_IND
    Procedure to

  L1_send_MPH_SERVING_DED_MEAS_IND
    Procedure to

  L1isr_send_MPH_CHANNEL_ASSIGN_CONF
    Procedure to

  L1isr_send_PH_CONNECT_IND
    Procedure to

  L1isr_send_MPH_RECONNECT_CHANNEL_CNF
    Procedure to

  L1_send_ISR_SCAN_CNF
    Procedure to

  L1_send_ISR_HANDOVER_CNF
    Procedure to

  L1_send_ISR_FIRST_FCB_CNF
    Procedure to

  L1_send_ISR_SCH_CNF
    Procedure to

  L1isr_send_PH_READY_TO_SEND_IND
    Procedure to

  L1isr_send_MPH_IMMED_ASSIGN_CNF
    Procedure to

  L1_send_MPH_IMMED_ASSIGN_CNF
    Procedure to

  L1isr_send_MPH_CHANNEL_RELEASE_CNF
    Procedure to

  L1isr_send_MPH_SET_CIPHERING_CNF
    Procedure to

  L1isr_send_L1_REP_PERIOD_IND
    Procedure to

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_send.c#4 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $


when       who       what, where, why
--------   --------  ----------------------------------------------------------
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
11/04/17   rv        CR2031827 Excessive F3 log reduction for Oppo
21/08/17   sk        CR2093261 Dual space IMS meas data and report meas as state if TRM not available during previous DRX period
26/05/17   mk        CR2020690 Wait for 2 complete SACCH periods before scheduling NCell FCCH and SCH after channel transition in dedicated mode
20/03/17   mk        CR2020999 Prx Drx power difference needs to be calculated & applied to NCELLs before REP_PERIOD_IND to avoid delays
07/03/17   mk        CR2015800 Reset "l1_ded_prx_drx_pwr_diff" once they applied to NCELL Rx level
18/01/18   km        CR2172348 Addition of GTOL_SRCH_STATE_ABORT_CNF_WAIT in IRAT sm
11/08/17   rc        CR2138578 ML logging - GL1 Changes
09/01/18   km        CR2166879 G2L abort handling in QTA/non-QTA scenarios
12/07/17   ng        CR2074605 Do not leave idle mode until G2X init confirm is received
25/01/16   cws       CR962379 Disable/enable ASDIV in IDLE/RACH state by NV
29/10/17   km        CR2133917 GL1 support for L+L
16/02/17   cc        CR1010614 G2L idle time transfer and MEAS only support
21/04/17   br        CR1041084 FR 36219: GDET – GMSK Modulation Detector
27/07/16   zf        CR1046704: Using MSGR variant
08/12/16   mk        CR1097591 Enhancement for DRX: DRx and PRx difference will be applied to neighbor cell measurements, if DRx power level is greater than PRx
01/06/16   dg        CR1023017 Don't set init req variable when already target tech is initialized
09/05/16   dg        CR1012577 Set g2x init flag to TRUE on receiving a new irat list if target tech is not initialized
08/01/16   cws       CR949517 Force asdiv position when requested by RF at certain SAR stateNew
11/02.16   dg        CR975067 Send empty surround irat meas ind to higher layer if there is no valid measurement
22/01/16   dg        CR963952 Don't send surround irat meas ind with uarfcn as 0 to higher layers
07/12/15   cc        CR933097 Fix yield SCH flag
19/11/15   cc        CR939086 Remove unmeasured W cell from RR
10/11/15   cws       CR937708 reset num of meas period for asdiv type2 when entering connected mode
08/10/15   dg        CR920293 Set drx task as active while sending INIT_REQ to Xl1 on receiving DEINIT_CNF
10/09/15   sk        CR899847 GERAN changes to trigger QSH events and handler for debugging
15/08/15   zf        CR887132 Use variant for IRAT CM_GSM_INIT MSGs.
25/08/15   fan       CR893794 Page sharing not happening due to missing BLOCK_QUALITY indication
26/08/15   am        CR890694 Ensure GRR is provided with correct BSIC
25/08/15   sjv       CR896284 Regrsn from CR890466:Rectify Var name ref. for checking Idle State 
13/08/15   sjv       CR890466 GL1_AsDIv: Use SCell band in idle to make AsDiv decisions
14/08/14   ws        CR889273 Fix Static declarations of variables which are not Dual data spaced
12/08/15   cws       CR864883 FR27546+FR27544 Add MTPL/power imbalance factors into ASDIV algorithm
05/08/15   hd        CR884936: Handle QTA STARTUP and CLEANUP messages in MSGR IF
05/08/15   cws       CR881986 Use only gsm1/gsm2 client for trm asdiv request
05/08/15   cws       CR882575 Set default type2 ASDIV rxlev to negative value 
17/07/15   ab        CR 871981 Address Static Analysis Warning.
25/06/15   mko       CR829915: Populate version field in l1_g2t_irat_log_acq_req()
31/07/15   cja       CR881129 Fix deadlock in send G2L deinit
29/07/15   dp/dv     CR878618: Changes to wait for rf exit confirmation in gl1 for X2W srch
21/07/15   cws       CR873163 fix asdiv/pchxfer compile warnings
14/07/15   cja       CR871031 Remove forcing gas id 1 for T2G/G2T (required for W+W).
20/04/15   dg        CR825682 While committing log packet G2T_IRAT_MEAS_REQ_LOG_PACKET,don't log more than 8 TD cells for a particular UARFCN
07/04/15   dg        CR817505 After sending INIT_REQ to ML1, update G2l_init_sent variable only if ISR lock is granted
12/07/15   nm        CR861458 Fix the interworking issues between PCHXFER, DRDSDS, RX/TX split
07/07/15   cws       CR863352 Clear Type2 switch_required and switch_burst flags when asdiv mode is not in RXTX
15/06/15   am        CR854340 Adding simulation support through diag
04/06/15   jk        CR846952:RxLEV report should be based on maximum of PRx and DRx average RSSIs
06/07/15   sp        CR862778: Reduce F3 logging in Idle as its impacting QTA timeline
03/07/15   jj        CR 865562 Remove featurisation introduced for CR839611
30/06/15   zf        CR864313: Update freq band if changed in G2L.
17/06/15   ab        CR847473 : unify usage of gl1_map_gas_id_to_client_id for TX_ANT_DIV/DR_DSDS/RX_DIV/PCHXFER/AS_DIV
18/05/15   jj        CR 839611 New skip last measurement in T2G if command is late 
17/06/15   jj        CR 856570  fix the regression 853609 
12/06/15   cgc       CR853609 moved g2l_irat_x2l_meas_enter() to after api call lte_ml1_irat_setup_schedule()
11/06/15   zf        CR847878: Using PRE_SRCH_REQ and CNF for G2W
17/06/15   sv        CR856025 Revert CR853609
12/06/15   cgc       CR853609 moved g2l_irat_x2l_meas_enter() to after api call lte_ml1_irat_setup_schedule()
09/06/15   cws       CR849140 Update antenna_to_use_for_tx when RF switch completes
28/05/15   cws       CR843724 Fix compile error with ASDIV feature enable 
28/05/15   cws       CR842048 Do not abort during G2W idle reconf if abort comes after rf_enter
05/06/15   dp        CR833596 Making use of MSGR support of multi-instances of SUBs using MSGR variant    
11/05/15   cja       CR827065 Handle G2W initialisation requested when G2W shutdown in progress
23/03/15   jj        CR 811970 Mutex dead lock l1_sci_tick_wcdma 
                        L1_send_LTE_CPHY_IRAT_MEAS_G2L_DEINIT_REQ caused by merge  of CR  765859 
08/05/15   ws        CR834838 Correct as_id,gas_id calling fn parameter bugs
05/07/15   zc        CR826075 Band Specific ASDIV Enable
24/03/15   zc        CR806813 Fix ASDIV switch stuck
29/04/15   npt       CR804870 RF timeline error recovery mechanism
04/21/15   zc        CR811956 Fix ASDIV switch stuck
09/04/15   cgc       CR794901 deprecated MPH_SURROUND_WCDMA_DEDICATED_MEAS_IND
09/04/15   cgc       CR819956 Thor 2.x RF API change, G2W use MSGR
31/03/15   pa        CR815621:GRM restructuring for TH 2.0. 
17/03/15   xz        CR801386 solve the address word alignment issue for scheduled_status of SCH
06/03/15   cgc       CR794323 FR25799 support x8 LTE with fast idle search.
18/02/15   zc        CR795410: Support asdiv type2 during data
24/02/15   js        CR749012: GL1 changes for LTE removal
20/02/15   js        CR693771: W+T removal. Resolving compilation errors 
29/01/15   zc        CR788139 Asdiv QTA gap DR-DSDS mode using preferred instead of current antenna
18/02/15   dg        CR783813 Clear STOP_CMD pending flag if GL1 has to intialize TD again for G2T search
17/02/15   zc        CR778432 Asdiv to stay in crisis mode until switchback period in traffic is over
11/02/15   sjv       CR791268 G2L Abort support in case ML1 aborts
29/01/15   cws       CR788745 [ASDIV]clear antenna measurments when turning off ASDIV algorithm  
27/01/15   cgc       CR782109 remove obsolete LTE_GAP_ABORT
21/01/15   jj        CR784462 remove FEATURE_THOR_G2X_MSGR_REWORK 
10/12/14   dg        CR765859 Do not go to sleep if MCPM is not updated for G2X MEAS STOP activity
17/12/14   jj        CR772391 ASDIV switch request  cancellation if RF does not switch 
12/12/14   zc        CR769907: Traffic mode asdiv test mode crash
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
11/12/14   ak        CR739561 SCE opt top2/top6 mode, disable pwr msr of blacklisted cells
10/12/14   ws        CR768376 Q6 compile warning fixes
03/12/14   npt       CR759467 - Fix compiler warnings
03/12/14   cgc       CR715394 G2L code Idle/Dedicated rework FEATURE_THOR_G2L_REWORK
20/11/14   cws       CR760451 Set ASDIV crisis_mode_allowed flag to true
01/10/14   zc        CR707083: FR 22623: ASDiv (+tuner) for TDD-LTE/TDS/G+G DR-DSDS w/ TDS/G CSFB
31/10/14   zc        CR755621 Fix merge error caused by checking in 748647
31/10/14   xz        CR748816 Extend dual data space for building RF script variable 
10/11/14   zc        CR748647 ASDIV Type1 Switching Test Mode API
17/10/14   jj        CR741205 moving  G2X CNF from l1 task to MSGR 
29/10/14   ab        CR745429 - Featurization correction for PCHXFER for SS / CR merges.
29/10/14   zc        CR684294 Take into account GTA case for ASDIV on setting wakeup config
07/10/14   cja       CR730201 Send rf meas exit after cleanup rather than waiting inline for RF scripts
21/08/14   zc        CR712672: Reset wait counter when transition to voice/traffic asdiv state
07/10/14   pa        CR733749:Handling of MPH_DSC_THRESHOLD_IND in the RACH state. 
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
11/09/14   zc        CR723421: Remove setting default ASDIV config on wakeup
09/09/14   pa        CR719024: Implementation of Hybrid Tuneaway Fallback Algorithm  
13/06/14   sp        CR674999 FTM Mode - Set FTM_GSM_BER_CMD_Q_SIG while sending MPH_STOP_GSM_MODE_CNF
22/08/14   cgc       CR713634 obsoleted G2L code and featurisations FEATURE_GSM_G2L_TIMED_SRCH and FEATURE_CM_G2L removed
04/08/14   nm        CR675844 Clear decoding_cbch flags correctly on all CB data(good/bad) /missing indications
31/07/14   zc        CR702863 Fix compiler error when asdiv is enabled
31/07/14   cos       CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
30/05/14   cgc       CR631007 enhancement, force G2X measurement within the RR reselection period
19/05/14   ss        CR668474 X2G Init via task : Seperate message header for each tech
10/03/14   ka        CR628921 Use LTE earfcn type in gl1 to allow move to 32bit earfcn's
02/06/14   jj        CR 688565 re-name function get_gsm_client_from_gas_id to  
10/06/14   jj        CR 668965 merge ASDIV from DI.3.0 to BO 2.0 
13/06/14   og        CR671046. Remove the featurization to enable the gas id setting.
12/05/14   cgc       CR662650 fix update for CR659270
12/05/14   zc        CR660275: Do not set asdiv wakeup config unless granted TRM
06/05/14   cgc       CR659270 mode change request causes race condition between gl1_TDS_IRAT_INIT_CNF_pending and L1_send_TDS_IRAT_STOP_CMD
23/04/14   zf        CR653611: Reset reduced power monitor duty cycle count when missing a page
16/04/14   cgc       CR641709 Check TDSCDMA_IRAT_INIT_CNF response add l1_sc_reset_G2T_state()
14/04/14   ss        CR646261 FR GSM CM INIT via message in GL1 task context
11/04/14   jj        CR 647256  client ID correction in trm message to adapt for multisim
07/04/14   cja       CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF (and others already mainlined)
04/04/14   cah       CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
12/02/14   jj        CR 596556 Returning switch permission to LTE when GSM is in OOS in SGLTE mode
26/03/14    ab       CR639078 : COEX : CXM Priority and Freq ID for cm_power_meas
24/03/14   sd        CR630989 Pass ML1 RF  Device ID for G2L
24/01/14   dg        CR592932: Removed Prevent tool compiler warnings
13/03/14   jj        CR  629747 DSDS: register inactive mode  before GL1 goes inactive
23/03/14   sjw       CR637079:new interface for BPLMN failures
03/10/14   sc        CR618785 Inserting a break statement to avoid logging in a default switch case
20/02/14   jj        CR616632 gsm_ant_switch_cb register in set_mode and config for DSDA/DSDS ASDIV
19/02/14   cja       CR513250 G band in IRAT replace rf_card_band_type with rfcom_gsm_band_type
17/02/14   cja       CR605188 Add PN_offset to WCDMA best six structure
10/02/14   cja       CR612218 For G2L add meas enter and meas exit for LTE RF
14/01/14   og        CR589868. T+G DSDA support.
05/02/14   br        CR576666 Prevent the denit if we have already sent the G2L INIT
11/02/13   sp        CR488596 To indicate CB Missing indication in dedicated mode too
24/01/14  cah        CR598552: enable sleep for DSDS GSTMR+VSTMR
14/01/14   jj        CR600114 tx- diversity in dime SGLTE part 2
14/01/14   cja       CR599843 Pass GL1 RxLM buffer index to TL1 in G2T startup
10/01/14   cja       CR598696 Set all RF parameters in G2X
08/01/14    us       CR562238 Partial T2G gap usage for RSSI measurement
02/11/12   jj        CR402579 Support of Tx Diversity in GERAN.
25/11/13   pa        CR58117:Bolt: Support for GPLT G2W Dedicated Mode Measurement
22/10/13   pa        CR564235: Added GPLT Ciphering command.
10/10/13   cgc       CR556101 Change lte init/deinit req to msg HIGH
27/09/13   sp        CR551580: Add NV logging of outgoing L1 messages
27/09/13   cgc       CR523552 add support for FR 16939 L+DSDS LTA/QTA
16/09/13   js        CR540576 Missed pages not logged with the correct SCLK
05/08/13   cja       CR497487 New RF IRAT API to add RF ID in G2T.
07/16/13   zc        CR470197 Replace memcpy with memscpy
22/05/13   cgc       CR488995 G2L Idle abort overlaps with G2L CM start up.
08/05/13   cgc       CR453758 add FEATURE_CM_G2L search functionality FR-3009
26/06/13   cs        CR505156 Latest TSTS Interface updates
26/06/13   cs        TSTS Updates
13/05/13   ss        CR486978 Remove extra f3 messages while logging G2T log packets
10/04/13   xz        CR485349 ADD TDS Feature for L1_send_MPH_SURROUND_WCDMA_IDLE_MEAS_IND
03/04/13   cja       CR470662 Use msgr_init_hdr for L1_send_LTE_CPHY_IRAT_MEAS_G2L_INIT_REQ
08/04/13   xz        CR472202 Remove tds_state judgement when issue TDSCDMA_IRAT_INIT_CMD.
04/01/13   dv        CR433484 Wait for G2T abort to complete before exit idle
28/01/13   cja       CR445737 Use msgr_init_hdr to set LTE and TDSCDMA message headers
06/12/12   cja       CR429173 Fix Klockwork error (memcpy to TDS log packet)
06/12/12   cgc       CR429175  Klockwork fixes, replace MAX_WCDMA_FREQS with TDSIRAT_MAX_TDS_FREQ
28/11/12   br        CR383165 Removed unused variables to avoid compilation warnings
26/11/12   ss        CR 405129 : Add version in G2T Log Packets  and Change Log Packet Id for CR363991
21/10/12   dv        CR397922: Fix for 6.2.2.3a G2T TC
24/09/12   og        CR402775.T2G BSIC triple buffered.
10/09/12   ss        CR363991 : Enable FEATURE_G2T_SRCH_LOGGING for G2T meas and acq request
04/09/12   og        CR395649. T2G Acquisition and BSIC decode enhancements.
03/08/12   og        CR383632. Don't request a tuneback to GSM when the G2L startup request
                     is not sent to LTE L1.
02/08/12   pa        CR384676: Consider only JDS cells in G2T measurement.
24/07/12   og        CR379017. G2W reporting problems in idle mode.
17/07/12   cja       CR375594 Ensure dedicated W search is aborted
08/06/12   pa        Added support for GPLT.
16/05/12   dv        CR357448 G2T Abort mechanism
25/04/12   og        CR356465. T2G updates for TDS-1361 and 1379.
25/04/12   pa        CR355881: G2T Dedicated TCH: TL1 crashed in RF API. Added support of RF_SCRIPTS_CMD.
29/03/12   dv        CR347854:Enable error msg handling for T2G
30/11/11   pa        Updated ts0_position in G2T Meas Req.
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
06/12/11   cja       CR321466 In dedicated ID search send RR update if cell not found
11/10/11   og        CR310953. Report Ec/No x 2 in both idle and transfer.
28/07/11   og        Cancel the G2L search attempt if sending the startup message
                     fails. CR296627.
03/06/11   og        CR287418. Pend the G2L DEINIT REQ message on the cleanup cnf
                     when the search is active.
02/06/11   og        CR290078. Ensure stopping SCE and deactivating are done atomically.
18/04/11   og        CR283929. Race condition between the ISR sending the G2L
                     ABORT req and GSM L1 task sending the CLEANUP req.
03/02/11   og        Use legacy messages when sending RR IRAT measurements in
                     dedicated mode. Resolves CR274061.
02/01/11   og        Ensure the cleanup req is not sent if abort cnf is pending
                     CR273767.
28/01/11   og        CR273067. Fix the WCDMA dedicated report.
28/01/11   og        Stop rogue WCDMA reports from being processed by RR when
                     there are no WCDMA cells / UARFCNs stored in L1.
24/01/11   og        CR272096. Do not send a G2L ABORT REQ if a G2L CLEANUP REQ
                     was sent before.
21/01/10   og        CR270636. Ensure the UE doesn't go to sleep till the G2L CNF is
                     received from LTE L1.
17/12/10   og        Changed G2L INIT and DEINIT.
2/12/10    og        Send the gsm scell parameters to LTE L1 in the G2L startup request.
29/11/10   og        Adding G2L support.
15/08/11   og        CR287418. Pend the G2L DEINIT REQ message on the cleanup cnf
                     when the search is active.
28/07/11   og        Cancel the G2L search attempt if sending the startup message
                     fails. CR296627.
02/06/11   og        CR290078. Ensure stopping SCE and deactivating are done atomically.
18/04/11   og        CR283929. Race condition between the ISR sending the G2L
                     ABORT req and GSM L1 task sending the CLEANUP req.
28/04/11   cja       Fix compiler warnings
19/04/11   cja       Add RxLM buffer for LTE startup
03/02/11   og        Use legacy messages when sending RR IRAT measurements in
                     dedicated mode. Resolves CR274061.
02/01/11   og        Ensure the cleanup req is not sent if abort cnf is pending
                     CR273767.
28/01/11   og        CR273067. Fix the WCDMA dedicated report.
28/01/11   og        Stop rogue WCDMA reports from being processed by RR when
                     there are no WCDMA cells / UARFCNs stored in L1.
24/01/11   og        CR272096. Do not send a G2L ABORT REQ if a G2L CLEANUP REQ
                     was sent before.
21/01/10   og        CR270636. Ensure the UE doesn't go to sleep till the G2L CNF is
                     received from LTE L1.
17/12/10   og        Changed G2L INIT and DEINIT.
2/12/10    og        Send the gsm scell parameters to LTE L1 in the G2L startup request.
29/11/10   og        Adding G2L support.
13/07/10   cs        Updates for CB_DRX support
09/09/09   cs        Fix Klocwork bounds checking error
12/01/08   gj        CR165617 - mainline FEATURE_GSM_GPRS_ENHANCED_MEASUREMENT_REPORTING feature.
22/07/08   dv        CR150983 - Temp workaround for EMR reporting problem.
27/03/08   nf        Added NV Item handling to R-SACCH and R-FACCH
17/01/08   cs        Fix the external def of gl1_hw_get_vctcxo_pdm_val
21/11/07   og        Ensure L1 drops back to IDLE when the single block start
                     time elapses. Resolves CR115529.
09/11/07   nf        Added FEATURE_GSM_R_SACCH
06/06/07   ws        CR 118908 - Don't reset nc_mode in l1_sc_init() since this
                     is called when we reselect to PBCCH cell stopping NC2 reporting
11/16/05   ws        More Lint Cleanup
08/24/05   ws        Changed prototype for MPH_DTM_CHANNEL_RELEASE_IND to
                     include cs_released and ps_released flags
08/12/05   og        Merge change 228472 from saber 5x branch. Fixes CR71702.
08/12/05   ws        Added initial FEATURE_GSM_DTM support
03/15/05   dp        Added support for enhanced measurement reporting
03/11/05   sv        Mainlined FEATURE_GSM_CB
02/11/04   bm        Fix to send RESUME_GSM_MODE_CNF after resuming
02/11/04   bm        Support for Background WCDMA PLMN Scan
02/09/05   dmb       #define RACH in rr_l2_l1.h renamed L2_RACH
10/01/04   bm        Enhanced Parallel BCCH decode support
09/24/04   bm        Support for Split up the L1 deactivation into TASK and ISR
08/23/04   dp        Added support for "fast camping" for FTM BER measurements
08/09/04   gsc       Added support for sending CB_MISSING_BLOCK_IND and
                     CB_SKIP_CNF to CB task.
07/21/04   bk        Merged in changes to RACH to support EGPRS
05/26/04   sv        Lint Cleanup.
02/25/04   gw        Lint clean up.
02/19/04   gfr       Compile fixes for gsm rotators.
02/15/04   jc        New format for reporting WCDMA meas in
                     L1_send_MPH_SURROUND_WCDMA_XFER_MEAS_IND
02/04/04   ws        fixed bug with parameter order in L1_send_MPH_READ_SYS_INFO_FAILURE_IND
02/02/04   kf        Added fields to L1_send_MPH_READ_SYS_INFO_FAILURE_IND
01/30/04   jc        Clear out ded wcdma msg before filling it in.Aids debugging
01/28/04   gw        Changes to support reselection optimizations.
01/20/04   jc        Updated l1_send primitive for handling new dedicated mode
                     measurement reporting algorithm to RR.
01/09/04   gw        Added support for sending MPH_READ_SYS_INFO_FAILURE_IND.
11/05/03   gfr       Support for quad-band.
10/09/03   gsc       Added support for GSM Cell Broadcast under FEATURE_GSM_CB
01/10/03   kf        changed setting of valid flag in
                     L1_send_MPH_NC_MEASUREMENT_IND
08/18/03   sw        Added WCDMA measurements during transfer
08/15/03   dp        Added debug msgs
08/07/03   tb        Increase precision of EcNo value.
08/01/03   tb        Pass trk_lo_adj for GtoW interrat.
10/07/03   kf        added field to L1_send_MPH_SURROUND_UPDATE_FAILURE_IND
06/23/03   gfr       Compiler warning cleanup.
06/06/03   tb        Added MPH_SURROUND_WCDMA_DED_MEAS_IND support.
29/05/03   kf        ncell pbcch read changes
01/10/03   gw        Changed couple of SCE function calls to reflect renaming of
                     BA list functions done as part of SCE clean up.
12/03/02   mk        General GPRS featurization cleanup, fixed featurization in
                     l1_scdrv_mon_ind().
11/25/02   mk        Updated GPRS baseline.
11/15/02   mk        Updated GPRS baseline with release 2.0 functionality.
11/03/02   tb        Added support for INTERRAT_GTOW
10/08/02   kf        Added PL1 GPRS measurement code hooks under switch
                     FEATURE_GPRS_MEAS.
10/07/02   dp        Added support for fully parallel decodes
09/05/02   plm       Merged m10200.c, m10550.c, m10551.c and m10552.c. Also
                     removed declared local variable "i" that were not being used
                     in the L1_send_ISR_DECODE_BCCH_LIST_SCH_CNF() and
                     L1_send_ISR_DECODE_BCCH_LIST_BCCH_CNF() functions.


===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "rfgsm_msg.h"
#include "geran_variation.h"
#include "comdef.h"
#include "memory.h"
#include "gs.h"
#include "l1i.h"
#include "l1_sc.h"
#include "l1_sc_int.h"
#include "l1_utils.h"
#include "l1_task.h"
#include "l1_isr.h"

#include "l2_l1.h"
#include "l2_l1_g.h"
#include "ms.h"
#include "msg.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "sys_stru.h"
#include "cbl1if.h"
#include "l1_smscb.h"
#include <stringl/stringl.h>
#include "l1_log.h"
#include "gmacl1.h"
#include "l1_drx.h"
#include "geran_dual_sim_g.h"

#include "ftm.h"
#include "ftm_gsm_ber.h"

#ifdef FEATURE_GL1_GPLT
#include "gplt_cmdi.h"
#include "gplt_ctl.h"
#endif /*FEATURE_GL1_GPLT*/

#ifdef  FEATURE_GSM_TO_LTE
#include "gl1_sys_params.h"
#include "lte_as.h"
#include "lte_cphy_irat_meas_msg.h"
#include "lte_ml1_irat_ext_api.h"
#endif

#ifdef FEATURE_GSM_TDS
#include "tdsiratif.h"
#endif

#ifdef FEATURE_GSM_TDS
boolean l1_send_tds_irat_build_rfscripts[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
#endif /*FEATURE_GSM_TDS*/

#include "gtmrs_g.h"
#include "l1_sc_irat.h"
#include "l1_sc_lte_irat.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hwi.h"
#ifdef GL1_GMSK_MOD_DET
#include "gprs_mem.h"
#endif
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "gpl1_dual_sim.h"
#include "gl1_drdsds.h"
#include "geran_dual_sim_g.h"
#endif /*( FEATURE_DUAL_SIM ) |( FEATURE_TRIPLE_SIM )*/

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
#include "wl1x2wsrchapi.h"
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

#include "trm.h"
#include "l1_ds.h"
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

#define  GL1_MAX_REPORTED_WCDMA_UARFCNs  (3)

#define L1_EMR_MAX_BLKS_PER_MEAS_PERIOD  23

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#define GL1_QSH_PAG_DEC_FAIL_CNT_THRESHOLD 3
#define GL1_QSH_PAG_MISS_CNT_THRESHOLD 3
#endif


#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
static uint8 gl1_qsh_page_dec_fail_cnt[NUM_GERAN_DATA_SPACES];
static uint8 gl1_qsh_page_miss_cnt[NUM_GERAN_DATA_SPACES];
#endif

#ifdef GL1_GMSK_MOD_DET
extern uint16 *decStat[NUM_GERAN_DATA_SPACES];
#endif
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
extern int16 tx_diversity_threshold ;
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING*/

volatile boolean gl1_TDS_IRAT_STOP_MODE_pending = FALSE;

/*Flag indicates that STOP_MODE is issued to TD and GL1 is waiting for STOP_CNF*/
volatile boolean g2t_stop_mode_cnf_pending = FALSE;


#ifdef FEATURE_GSM_QSC_TX_DIV
#define GL1_ASDIV_MICROSEC_IN_FRAME 4600
#define GL1_ASDIV_NUM_MEAS_PERIODS 8
extern boolean  gl1_hw_rfa_rf_gsm_set_antenna_req_cmd( boolean antenna_pos,gas_id_t gas_id );
extern boolean    tx_blnk_cnt[NUM_GERAN_DATA_SPACES] ;
boolean passive_sub_switch_notification [NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };

gl1_trm_asdiv_test_mode_T gl1_trm_asdiv_test_mode_store[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE_S(0)};
boolean antenna_switch_burst [NUM_GERAN_DATA_SPACES] =  { INITIAL_VALUE( FALSE ) };
boolean qsc_got_switch_control[NUM_GERAN_DATA_SPACES] =  { INITIAL_VALUE( FALSE ) };
 boolean  antenna_switch_required_at_start_of_next_frame [NUM_GERAN_DATA_SPACES] =  { INITIAL_VALUE( 0 ) };

/*Static Global Variables*/
gl1_trm_asdiv_traffic_mode_T gl1_trm_asdiv_traffic_store[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE_S(0)};
/*structure to track Tx power, Tx/Rx imbalance and mtpl for ASDIV*/
gl1_hw_asdiv_mtpl_T gl1_hw_asdiv_mtpl_store[NUM_GERAN_DATA_SPACES]= {INITIAL_VALUE_S(0)};


#ifdef FEATURE_GERAN_DRDSDS
gl1_trm_asdiv_drds_mode_t gl1_trm_asdiv_drds_mode_var[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE_S( 0 ) };
#endif

/*EFS/NV controlled Items*/
extern int16  trafic_mdm_threshold [NUM_GERAN_DATA_SPACES],idle_mdm_threshold[NUM_GERAN_DATA_SPACES],gsm_trafi_sens[NUM_GERAN_DATA_SPACES],gsm_idle_sens[NUM_GERAN_DATA_SPACES];
extern int16  gsm_qsc_large_delta[NUM_GERAN_DATA_SPACES],gsm_qsc_small_delta[NUM_GERAN_DATA_SPACES],gsm_qsc_hysteriesis_time_l1[NUM_GERAN_DATA_SPACES];
extern uint8  gsm_qsc_idle_pingpong_mitigation_thre [NUM_GERAN_DATA_SPACES], gsm_qsc_traffic_crisis_mode_thre [NUM_GERAN_DATA_SPACES], gsm_qsc_idle_suspension_time[NUM_GERAN_DATA_SPACES];

/*Function Definitions*/
trm_ant_switch_div_callback_t gsm_ant_switch_cb (trm_ant_switch_cb_info_type  ant_cfg_info  );
trm_ant_switch_set_mode_callback_t gsm_set_mode_cb (trm_ant_switch_set_mode_type  set_mode_info);
trm_asd_set_mode_and_config_cb_t set_mode_and_config_cb (trm_asd_set_mode_and_config_type    set_mode_cfg_info );
void  gl1_rf_switch_complete_cb (rfa_rf_gsm_set_antenna_callback_s *cb_data_ptr);
void gl1_trm_ant_switch_div_set_config ( gas_id_t gas_id  );

/*New GL1 functions to handle interface updates for DR-DSDS*/
#ifdef FEATURE_GERAN_DRDSDS
static void gl1_trm_asdiv_clear_consecutive_thresh_cnt(gas_id_t gas_id, trm_client_mode_type mode_type);
boolean gl1_trm_asdiv_get_traffic_crisis_flag(gas_id_t gas_id);
static boolean gl1_trm_asdiv_increment_crisis_or_pingpong_mode_cnt(gas_id_t gas_id, trm_client_mode_type mode_type);
#endif

#endif /* FEATURE_GSM_QSC_TX_DIV */

extern boolean L1_ftm_mode_enabled ( void );
extern dedicated_data_T l1_dedicated_data[];
extern gpl1_serving_cell_meas_t gpl1_serving_cell_meas[];

extern uint32 gl1_hw_sleep_wmgr_missed_PCH_time[NUM_GERAN_DATA_SPACES];

extern uint32 get_RFN_from_sch_data( const uint8 *sch_data );

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
extern void gl1_hw_clear_antenna_meas (gas_id_t gas_id);
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
#ifdef FEATURE_G2T_SRCH_LOGGING
/*While Logging G2T commands. this stucture stores the latest values of gap info for G2T operation.
 * It is populated in the function  L1_send_TDS_IRAT_STARTUP_REQ and used for logging G2T meas and acq resp */
typedef struct{

    gsm_l1_tds_gap_info_type tds_gap_info;
    uint32 rxlm_rx_buffer;

} log_packets_tds_startup_req_info_type;

log_packets_tds_startup_req_info_type log_packets_tds_startup_req_info;

#endif /*  FEATURE_G2T_SRCH_LOGGING*/


/*===========================================================================

FUNCTION  L1_send_MPH_SURROUND_IRAT_MEAS_IND

DESCRIPTION
  This function sends the MPH_SURROUND_IRAT_MEAS_IND message for all G2X irat results.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void  L1_send_MPH_SURROUND_IRAT_MEAS_IND( rr_l1_irat_rat_e                  rat,
                                          const rr_l1_irat_measurements_u*  measurements_ptr,
                                          gas_id_t                          gas_id
                                         )
{
#ifdef FEATURE_GL1_GPLT
  /* 
  ** GPLT test mode 
  */
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }
    else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_SURROUND_IRAT_MEAS_IND;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->surrounding_irat_meas_ind.param.rat = rat;
      gplt_cmd_ptr->surrounding_irat_meas_ind.param.measurements = *measurements_ptr;

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_SURROUND_IRAT_MEAS_IND");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
    /*
    ** normal mode 
    */
    mph_surround_irat_meas_ind_t  mph_surround_irat_meas_ind;

    mph_surround_irat_meas_ind.message_header.message_set = MS_RR_L1;
    mph_surround_irat_meas_ind.message_header.message_id  = MPH_SURROUND_IRAT_MEAS_IND;

    PUT_IMH_LEN( sizeof ( mph_surround_irat_meas_ind ) - sizeof ( IMH_T ), &mph_surround_irat_meas_ind );

    mph_surround_irat_meas_ind.rat          = rat;
    mph_surround_irat_meas_ind.measurements = *measurements_ptr;

    mph_surround_irat_meas_ind.gas_id = gas_id;

    if( GS_SUCCESS != L1_isr_send_message( GS_QUEUE_RR, &mph_surround_irat_meas_ind, TRUE, gas_id) )
    {
      MSG_GERAN_ERROR_0_G(" Failed to send MPH_SURROUND_IRAT_MEAS_IND ");
    }
#endif /* FEATURE_GL1_GPLT*/
}




/*===========================================================================

FUNCTION  L1_SEND_MPH_SURROUND_WCDMA_XFER_MEAS_IND
      now L1_SEND_MPH_SURROUND_WCDMA_MEAS_IND

DESCRIPTION
  Common function for WCDMA cell measurement results
  This function sends the MPH_SURROUND_IRAT_MEAS_IND measurement results
  for WCDMA  in packet transfer/dedicated.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void L1_send_MPH_SURROUND_WCDMA_MEAS_IND( word num_cells, wcdma_dedicated_meas_T cells[], gas_id_t gas_id )
{

  rr_l1_irat_measurements_u  irat_measurements_per_UARFCN[GL1_MAX_REPORTED_WCDMA_UARFCNs];

  uint32  num_uarfcns = 0;
  uint32  cell_idx;
  uint32  i = 0;
  uint16  freq_idx;

  for( freq_idx = 0; freq_idx < GL1_MAX_REPORTED_WCDMA_UARFCNs; freq_idx++ )
  {
     irat_measurements_per_UARFCN[freq_idx].wcdma.uarfcn = 0;
  }


  for( cell_idx = 0; cell_idx  < num_cells; cell_idx++ )
  {
    uint16  uarfcn = cells[cell_idx].UARFCN;

    for( freq_idx = 0; freq_idx < num_uarfcns; freq_idx++ )
    {
      if( uarfcn == irat_measurements_per_UARFCN[freq_idx].wcdma.uarfcn )
      {
        break;
      }
    }

    if( freq_idx < GL1_MAX_REPORTED_WCDMA_UARFCNs )
    {
      uint16 curr_cell_cnt;

      /* check if this is a new frequency */
      if (freq_idx == num_uarfcns)
      {
        num_uarfcns++;

        irat_measurements_per_UARFCN[freq_idx].wcdma.uarfcn        = uarfcn;
        irat_measurements_per_UARFCN[freq_idx].wcdma.no_of_entries = 0;
      }
      curr_cell_cnt = irat_measurements_per_UARFCN[freq_idx].wcdma.no_of_entries;

      if (curr_cell_cnt < MAX_WCDMA_MEASUREMENTS_PER_FREQUENCY)
      {
        irat_measurements_per_UARFCN[freq_idx].wcdma.entries[curr_cell_cnt].scrambling_code = cells[cell_idx].Scrambling_Code;
        irat_measurements_per_UARFCN[freq_idx].wcdma.entries[curr_cell_cnt].diversity       = FALSE;
        irat_measurements_per_UARFCN[freq_idx].wcdma.entries[curr_cell_cnt].rscp            = cells[cell_idx].RSCP;
        irat_measurements_per_UARFCN[freq_idx].wcdma.entries[curr_cell_cnt].ecno            = cells[cell_idx].Ec_Nox2;
        irat_measurements_per_UARFCN[freq_idx].wcdma.entries[curr_cell_cnt].rssi            = cells[cell_idx].RSSI;
        irat_measurements_per_UARFCN[freq_idx].wcdma.entries[curr_cell_cnt].pn_offset       = cells[cell_idx].PN_Offset;

        irat_measurements_per_UARFCN[freq_idx].wcdma.no_of_entries++;
      }
    }   /* End of if (freq_idx < MAX_WCDMA_FREQS) */
  }     /* for( cell_idx = 0; cell_idx  < num_cells; cell_idx++ ) */

  while(  ( i < num_uarfcns )
        &&( i < GL1_MAX_REPORTED_WCDMA_UARFCNs )
       )
  {
      L1_send_MPH_SURROUND_IRAT_MEAS_IND( RR_L1_IRAT_RAT_WCDMA, &irat_measurements_per_UARFCN[i], gas_id );
      i++;
  }
}

/*===========================================================================

FUNCTION  L1_send_not_found_MPH_SURROUND_DED_ID_MEAS_IND

DESCRIPTION
  This function sends the MPH_SURROUND_IDLE_MEAS_IND message during
  dedicated if no cell found on a particular UARFCN/EARFCN.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_not_found_MPH_SURROUND_IRAT_MEAS_IND( rr_l1_irat_rat_e rat, lte_earfcn_t uearfcn,gas_id_t gas_id )
{
  rr_l1_irat_measurements_u  irat_measurements;
  boolean RatOk=TRUE;

  switch (rat)
  {
  case RR_L1_IRAT_RAT_WCDMA:
     irat_measurements.wcdma.uarfcn        = uearfcn;
     irat_measurements.wcdma.no_of_entries = 0;
     break;


#if defined(FEATURE_GSM_TO_LTE)
  case RR_L1_IRAT_RAT_LTE:
     irat_measurements.lte.earfcn        = uearfcn;
     irat_measurements.lte.no_of_entries = 0;
     break;
#endif

#if defined(FEATURE_GSM_TDS)
  case RR_L1_IRAT_RAT_UTRAN_TDD:
     irat_measurements.utran_tdd.uarfcn         = uearfcn;
     irat_measurements.utran_tdd.no_of_entries  = 0;
     break;
#endif

  default:
     RatOk=FALSE;
     break;
  }
  if(RatOk)
  {
    L1_send_MPH_SURROUND_IRAT_MEAS_IND( rat, &irat_measurements, gas_id);
  }
}

/*===========================================================================

FUNCTION  L1_SEND_MPH_SURROUND_WCDMA_IDLE_MEAS_IND

DESCRIPTION
  This function sends the MPH_SURROUND_WCDMA_IDLE_MEAS_IND message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_SURROUND_WCDMA_IDLE_MEAS_IND( gas_id_t gas_id )
{
#if defined(FEATURE_GSM_TO_LTE)|| defined(FEATURE_GSM_TDS)

  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  best_six_wcdma_info_T      best_six_wcdma_info;

  if( FALSE != l1_sc_wcdma_get_best_six( &best_six_wcdma_info, gas_id ) )
  {
    uint32  rr_params_freq_index = 0;

    MSG_4(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "rr_params_freq_index %d best_six_wcdma_info.no_of_entries %d RAT %d rr_params.num_freqs %d",
        rr_params_freq_index,
        best_six_wcdma_info.no_of_entries,
        l1_sc_wcdma_data_ptr->rr_params.freq_list[ rr_params_freq_index ].rat,
        l1_sc_wcdma_data_ptr->rr_params.num_freqs
        );

    if( best_six_wcdma_info.no_of_entries > MAX_NUMBER_OF_BEST_WCDMA_CELLS )
    {
      best_six_wcdma_info.no_of_entries = MAX_NUMBER_OF_BEST_WCDMA_CELLS;
    }

    while( rr_params_freq_index < l1_sc_wcdma_data_ptr->rr_params.num_freqs )
    {
      if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[ rr_params_freq_index ].rat  )
      {
        rr_l1_irat_measurements_u  irat_measurements;
        uint32                     index = 0;
        uint16                     uarfcn = l1_sc_wcdma_data_ptr->rr_params.freq_list[ rr_params_freq_index ].arfcn.UARFCN;

        /* Make sure this is set to zero */
        irat_measurements.wcdma.no_of_entries = 0;

        while( index < best_six_wcdma_info.no_of_entries )
        {
          MSG_GERAN_HIGH_3_G("uarfcn %d  best_six uarfcn %d rr_params_freq_index %d",
              uarfcn,
              best_six_wcdma_info.surrounding_wcdma_channel_info[index].UARFCN,
              rr_params_freq_index);

          if( uarfcn ==  best_six_wcdma_info.surrounding_wcdma_channel_info[index].UARFCN  )
          {
            irat_measurements.wcdma.uarfcn  = uarfcn;

            irat_measurements.wcdma.entries[irat_measurements.wcdma.no_of_entries].scrambling_code = best_six_wcdma_info.surrounding_wcdma_channel_info[index].Scrambling_code;
            irat_measurements.wcdma.entries[irat_measurements.wcdma.no_of_entries].diversity       = FALSE;
            irat_measurements.wcdma.entries[irat_measurements.wcdma.no_of_entries].rscp            = best_six_wcdma_info.surrounding_wcdma_channel_info[index].RSCP;
            irat_measurements.wcdma.entries[irat_measurements.wcdma.no_of_entries].ecno            = best_six_wcdma_info.surrounding_wcdma_channel_info[index].EcNo;
            irat_measurements.wcdma.entries[irat_measurements.wcdma.no_of_entries].rssi            = best_six_wcdma_info.surrounding_wcdma_channel_info[index].RSSI;
            irat_measurements.wcdma.entries[irat_measurements.wcdma.no_of_entries].pn_offset       = best_six_wcdma_info.surrounding_wcdma_channel_info[index].PN_Offset;

            MSG_GERAN_MED_5_G("irat_measurements: scrambling_code %d rscp %d, rssi %d pn_offset %d, no_of_entries %d",
                irat_measurements.wcdma.entries[irat_measurements.wcdma.no_of_entries].scrambling_code,
                irat_measurements.wcdma.entries[irat_measurements.wcdma.no_of_entries].rscp,
                irat_measurements.wcdma.entries[irat_measurements.wcdma.no_of_entries].rssi,
                irat_measurements.wcdma.entries[irat_measurements.wcdma.no_of_entries].pn_offset,
                irat_measurements.wcdma.no_of_entries );

            irat_measurements.wcdma.no_of_entries++;
          }

          index++;
        }

        if( irat_measurements.wcdma.no_of_entries != 0 )
        {
          L1_send_MPH_SURROUND_IRAT_MEAS_IND( RR_L1_IRAT_RAT_WCDMA, &irat_measurements, gas_id );
        }

      }  /* if( RR_L1_IRAT_RAT_WCDMA == rr_params.freq_list[ rr_params_freq_index ].rat  ) */
#if defined(FEATURE_GSM_TDS)
      else if ( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[ rr_params_freq_index ].rat  )
      {
        rr_l1_irat_measurements_u  irat_measurements;
        uint32                     index = 0;
        uint16                     uarfcn = l1_sc_wcdma_data_ptr->rr_params.freq_list[ rr_params_freq_index ].arfcn.UARFCN;

        /* Make sure this is set to zero */
        irat_measurements.utran_tdd.no_of_entries = 0;

        while( index < best_six_wcdma_info.no_of_entries )
        {
          MSG_GERAN_HIGH_3_G("TDS: uarfcn %d  best_six uarfcn %d rr_params_freq_index %d",
              uarfcn,
              best_six_wcdma_info.surrounding_wcdma_channel_info[index].UARFCN,
              rr_params_freq_index);

          if( uarfcn ==  best_six_wcdma_info.surrounding_wcdma_channel_info[index].UARFCN  )
          {
            irat_measurements.utran_tdd.uarfcn  = uarfcn;
            /* Why do we use best six struct rather than stored_meas struct? */
            irat_measurements.utran_tdd.entries[irat_measurements.utran_tdd.no_of_entries].cell_parameter_id =
              best_six_wcdma_info.surrounding_wcdma_channel_info[index].Scrambling_code;

            irat_measurements.utran_tdd.entries[irat_measurements.utran_tdd.no_of_entries].diversity = FALSE;

            irat_measurements.utran_tdd.entries[irat_measurements.utran_tdd.no_of_entries].rscp =
              best_six_wcdma_info.surrounding_wcdma_channel_info[index].RSCP;

            /* RSSI not really required by RR*/
            irat_measurements.utran_tdd.entries[irat_measurements.utran_tdd.no_of_entries].rssi =
              best_six_wcdma_info.surrounding_wcdma_channel_info[index].RSSI;



            MSG_4(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
                "TDS: irat_measurements: cpid %d rscp %d, rssi %d no_of_entries %d",
                irat_measurements.utran_tdd.entries[irat_measurements.utran_tdd.no_of_entries].cell_parameter_id,
                irat_measurements.utran_tdd.entries[irat_measurements.utran_tdd.no_of_entries].rscp,
                irat_measurements.utran_tdd.entries[irat_measurements.utran_tdd.no_of_entries].rssi,
                irat_measurements.utran_tdd.no_of_entries );

            irat_measurements.utran_tdd.no_of_entries++;
          }

          index++;
        }

        if( irat_measurements.utran_tdd.no_of_entries != 0 )
        {
          L1_send_MPH_SURROUND_IRAT_MEAS_IND( l1_sc_wcdma_data_ptr->rr_params.freq_list[ rr_params_freq_index ].rat,
                                              &irat_measurements, gas_id );
        }
      }
#endif
      rr_params_freq_index++;

    }  /* while( rr_params_freq_index < l1_sc_wcdma_data_ptr->rr_params.num_freqs ) */

  }  /* if( FALSE != l1_sc_wcdma_get_best_six( &best_six_wcdma_info ) ) */
  else
  {
    /*When no good cell is present , send an empty irat_ind to GRR */
    rr_l1_irat_measurements_u  irat_measurements;
    rr_l1_irat_rat_e rat;

    rat = RR_L1_IRAT_RAT_UNDEFINED;
    irat_measurements.wcdma.uarfcn        = 0;
    irat_measurements.wcdma.no_of_entries = 0;

    MSG_GERAN_HIGH_0_G("No UARFCN cell in rr_report");

    L1_send_MPH_SURROUND_IRAT_MEAS_IND( rat, &irat_measurements, gas_id);
  }
#else

  mph_surround_wcdma_idle_meas_ind_T msg;
  msg.gas_id = gas_id;
  msg.message_header.message_set = MS_RR_L1;
  msg.message_header.message_id = MPH_SURROUND_WCDMA_IDLE_MEAS_IND;  /*lint !e641 */
  PUT_IMH_LEN( sizeof ( msg ) - sizeof ( IMH_T ), &msg );

  l1_sc_wcdma_get_best_six( &msg.best_six_wcdma_info, gas_id);

  (void)L1_send_message( GS_QUEUE_RR, &msg, TRUE, gas_id );

#endif
} /* L1_send_MPH_SURROUND_WCDMA_IDLE_MEAS_IND */

/*===========================================================================

FUNCTION  L1_SCDRV_WCDMA_IDLE_MEAS_IND

DESCRIPTION
  See L1_send_MPH_SURROUND_WCDMA_IDLE_MEAS_IND

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_scdrv_wcdma_idle_meas_ind( gas_id_t gas_id )
{
   L1_send_MPH_SURROUND_WCDMA_IDLE_MEAS_IND(gas_id);
}

/*===========================================================================

FUNCTION  L1_SEND_MPH_SURROUND_MEAS_IND

DESCRIPTION
  This function sends the MPH_SURROUND_MEAS_IND message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_SURROUND_MEAS_IND( gas_id_t gas_id )
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */
  l1_sc_bs_T*             cells[ MAX_NUMBER_OF_SURROUND_CELLS ];
  l1_sc_bs_T*             cell;
  surrounding_channel_info_T  *chnl_info;
  int n = 0;

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_SURROUND_MEAS_IND;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
      /* top-2 mode status */
      l1_sc_reduced_ncell_list_status(
         &gplt_cmd_ptr->surrounding_meas_ind.param.best_six_info.reduced_ncell_list_stored_in_l1,
         &gplt_cmd_ptr->surrounding_meas_ind.param.best_six_info.reduced_ncell_list_mode_active,
         &gplt_cmd_ptr->surrounding_meas_ind.param.best_six_info.reduced_ncell_list_allowed,
         gas_id);
#endif

      gplt_cmd_ptr->surrounding_meas_ind.param.best_six_info.no_of_entries = l1_sc_balist_get_best_six( cells, gas_id );

      if ( gplt_cmd_ptr->surrounding_meas_ind.param.best_six_info.no_of_entries > ( sizeof( cells ) / sizeof( cells[0] ) ) )
     {
        MSG_GERAN_ERROR_2_G( "Too many best_six_info entries %d clamping to max %d",
                   gplt_cmd_ptr->surrounding_meas_ind.param.best_six_info.no_of_entries,
                   ( sizeof( cells ) / sizeof( cells[0] ) ));

        gplt_cmd_ptr->surrounding_meas_ind.param.best_six_info.no_of_entries = ( sizeof( cells ) / sizeof( cells[0] ) );
     }

     for ( n = 0; n < gplt_cmd_ptr->surrounding_meas_ind.param.best_six_info.no_of_entries; ++n )
     {
        cell = cells[ n ];
        chnl_info = &(gplt_cmd_ptr->surrounding_meas_ind.param.best_six_info.surrounding_channel_info[ n ]);
        chnl_info->valid_info = TRUE;
        chnl_info->BCCH_ARFCN = cell->arfcn;
        chnl_info->RXLEV_average  = L1_map_dBm_x16_to_rxlev(cell->rx_power);
        chnl_info->RXLEV_instant  = L1_map_dBm_x16_to_rxlev(cell->rx_power_instant);

        /* Reconstruct SCH data (BSIC & RFN).
         * SCH data is sent to RR in MPH_SURROUND_MEAS_IND.
         * RR does not use RFN, so only fill in BSIC.
         * BSIC is b2-b7 in byte 0 of SCH data.
         * Remaining bytes are for RFN.
         * SCH data will be refreshed on SCH reconf.
         */
        chnl_info->SCH_block[ 0 ] = cell->bsic << 2; /* BSIC */
        chnl_info->SCH_block[ 1 ] = 0;               /* RFN  */
        chnl_info->SCH_block[ 2 ] = 0;               /* RFN  */
        chnl_info->SCH_block[ 3 ] = 0;               /* RFN  */

        MSG_GERAN_LOW_2_G("NC_Ncell %d normal_rxlev_aver %d ",chnl_info->BCCH_ARFCN.num,cell->rx_power);
        if ( n == 0 )
        {
           MSG_GERAN_MED_3_G("Top Ncell %d pwr=%d RXLEV=%d", cell->arfcn.num,cell->rx_power,chnl_info->RXLEV_average);
        }
     }
     for ( ; n < MAX_NUMBER_OF_SURROUND_CELLS; ++n )
     {
        gplt_cmd_ptr->surrounding_meas_ind.param.best_six_info.surrounding_channel_info[ n ].valid_info = FALSE;
     }

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_SURROUND_MEAS_IND");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else

   mph_surround_meas_ind_T msg;
   surrounding_channel_info_T  *chnl_info;
   l1_sc_bs_T*             cells[ MAX_NUMBER_OF_SURROUND_CELLS ];
   l1_sc_bs_T*             cell;
   int                     n;
#ifdef FEATURE_GSM_RX_DIVERSITY
   uint8                   l1_ded_prx_drx_pwr_diff = 0;
#endif
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);


   msg.message_header.message_set = MS_RR_L1;
   msg.message_header.message_id = MPH_SURROUND_MEAS_IND;  /*lint !e641 */
   PUT_IMH_LEN( sizeof ( msg ) - sizeof ( IMH_T ), &msg );

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
   /* top-2 mode status */
   l1_sc_reduced_ncell_list_status(
      &msg.best_six_info.reduced_ncell_list_stored_in_l1,
      &msg.best_six_info.reduced_ncell_list_mode_active,
      &msg.best_six_info.reduced_ncell_list_allowed,
      gas_id);
#endif

   msg.best_six_info.no_of_entries = l1_sc_balist_get_best_six( cells, gas_id );

   if ( msg.best_six_info.no_of_entries > ( sizeof( cells ) / sizeof( cells[0] ) ) )
   {
      MSG_GERAN_ERROR_2_G( "Too many best_six_info entries %d clamping to max %d",
                 msg.best_six_info.no_of_entries,
                 ( sizeof( cells ) / sizeof( cells[0] ) ));

      msg.best_six_info.no_of_entries = ( sizeof( cells ) / sizeof( cells[0] ) );
   }

#ifdef FEATURE_GSM_RX_DIVERSITY
   if((L1_DEDICATED_MODE == l1_tsk_buffer[gas_id].l1_state) || (L1_DTM_MODE == l1_tsk_buffer[gas_id].l1_state))
   {
      l1_ded_prx_drx_pwr_diff = L1_get_prxdrx_pwrdiff(gas_id);
   }
#endif

   /* Do not report NCELLs for 150 frames in new channel after transition */
   if((L1_DEDICATED_MODE == l1_tsk_buffer[gas_id].l1_state) && (FALSE == l1_sc_globals_ptr->start_ncell_rep_n_acq_dedi))
   {
      msg.best_six_info.no_of_entries = 0;
   }

   
   for ( n = 0; n < msg.best_six_info.no_of_entries; ++n )
   {
      cell = cells[ n ];
      chnl_info = &(msg.best_six_info.surrounding_channel_info[ n ]);
      chnl_info->valid_info = TRUE;
      chnl_info->BCCH_ARFCN = cell->arfcn;
#ifdef FEATURE_GSM_RX_DIVERSITY
      chnl_info->RXLEV_average  = L1_map_dBm_x16_to_rxlev(cell->rx_power) + l1_ded_prx_drx_pwr_diff;
#else
      chnl_info->RXLEV_average  = L1_map_dBm_x16_to_rxlev(cell->rx_power);
#endif
      chnl_info->RXLEV_instant  = L1_map_dBm_x16_to_rxlev(cell->rx_power_instant);
#if defined (FEATURE_GSM_SIMULATE_GL1_RF) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
     #error code not present
#endif /* FEATURE_GSM_SIMULATE_GL1_RF */

     /* Reconstruct SCH data (BSIC & RFN).
      * SCH data is sent to RR in MPH_SURROUND_MEAS_IND.
      * RR does not use RFN, so only fill in BSIC.
      * BSIC is b2-b7 in byte 0 of SCH data.
      * Remaining bytes are for RFN.
      * SCH data will be refreshed on SCH reconf.
      */
      chnl_info->SCH_block[ 0 ] = cell->bsic << 2; /* BSIC */
      chnl_info->SCH_block[ 1 ] = 0;               /* RFN  */
      chnl_info->SCH_block[ 2 ] = 0;               /* RFN  */
      chnl_info->SCH_block[ 3 ] = 0;               /* RFN  */

      MSG_GERAN_LOW_2_G("NC_Ncell %d normal_rxlev_aver %d ",chnl_info->BCCH_ARFCN.num,cell->rx_power);
      if ( n == 0 )
      {
         MSG_GERAN_MED_3_G("Top Ncell %d pwr=%d RXLEV=%d", cell->arfcn.num,cell->rx_power,chnl_info->RXLEV_average);
      }
   }
   for ( ; n < MAX_NUMBER_OF_SURROUND_CELLS; ++n )
   {
      msg.best_six_info.surrounding_channel_info[ n ].valid_info = FALSE;
   }
#if defined (FEATURE_GSM_SIMULATE_GL1_RF) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
   #error code not present
#endif /* FEATURE_GSM_SIMULATE_GL1_RF */

   msg.gas_id = gas_id;
   (void)L1_send_message( GS_QUEUE_RR, &msg, TRUE, gas_id );
#endif /* FEATURE_GL1_GPLT*/

   MSG_GERAN_LOW_0_G("***NORMAL_MEAS***");

}

/*===========================================================================

FUNCTION  L1_SCDRV_MON_IND

DESCRIPTION
  See L1_send_MPH_SURROUND_MEAS_IND

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_scdrv_mon_ind( gas_id_t gas_id )
{
    gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr =
              gl1_ms_switch_meas_mode_data(gas_id);
    if (gpl1_gprs_meas_mode_ptr->nc_mode == TRUE)
    {
       L1_send_MPH_NC_MEASUREMENT_IND(gas_id);
    }
    else if (gpl1_gprs_meas_mode_ptr->nc_mode == FALSE)
    {
       L1_send_MPH_SURROUND_MEAS_IND(gas_id);
    }
}


/*===========================================================================

FUNCTION  L1_send_MPH_SURROUND_UPDATE_FAILURE_IND

DESCRIPTION
  Sends MPH_SURROUND_UPDATE_FAILURE_IND primitive to RR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_SURROUND_UPDATE_FAILURE_IND( ARFCN_T ARFCN, boolean state_swap, gas_id_t gas_id )
{
   mph_surround_update_failure_ind_T    msg;
   int                                  message_length;

   /* Set up the message header */
   msg.message_header.message_set = MS_RR_L1;
   msg.message_header.message_id  = MPH_SURROUND_UPDATE_FAILURE_IND;   /*lint !e641 */
   message_length = sizeof(mph_surround_update_failure_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length, &msg)

   /* Set up the data in the message body */
   msg.BCCH_ARFCN = ARFCN;
   msg.state_changed  = state_swap;
   msg.gas_id = gas_id;
   /* Send the message */
   (void)L1_send_message(GS_QUEUE_RR, &msg,TRUE,gas_id);
}


/*===========================================================================

FUNCTION  L1_SEND_PH_CONNECT_IND

DESCRIPTION
  This function sends the PH_CONNECT_IND message to L2

DEPENDENCIES
  L2 Channel type (SACCH or DCCH)

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_PH_CONNECT_IND(l2_channel_type_T chnl_type, gas_id_t gas_id)
{

   ph_connect_ind_T              message_buffer,*message;
   int                           message_length;

   /* Align the pointers */

   message = &message_buffer;

      /* Set up the message header */

   message->message_header.message_set = MS_L2_L1;
   message->message_header.message_id = PH_CONNECT_IND;
   message_length = sizeof(ph_connect_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

          /* Set up the data in the message body */
   message->gas_id =  gas_id;
   message->connected_channel = chnl_type;
      /* Send the message */

   (void)L1_send_message(GS_QUEUE_LAYER_2,message,TRUE, gas_id);
}

/*===========================================================================

FUNCTION  L1_SEND_PH_READY_TO_SEND_IND

DESCRIPTION
  This function sends the PH_READY_TO_SEND_IND message to L2

DEPENDENCIES
  L2 Channel type (SACCH or DCCH)

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_PH_READY_TO_SEND_IND(l2_channel_type_T chnl_type, gas_id_t gas_id)
{

   ph_ready_to_send_ind_T        message_buffer,*message;
   int                           message_length;

   /* Align the pointers */

   message = &message_buffer;

      /* Set up the message header */

   message->message_header.message_set = MS_L2_L1;
   message->message_header.message_id = PH_READY_TO_SEND_IND;
   message_length = sizeof(ph_ready_to_send_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

          /* Set up the data in the message body */
   message->gas_id =  gas_id;
   message->l2_channel_type = chnl_type;
      /* Send the message */

   (void)L1_send_message(GS_QUEUE_LAYER_2,message,TRUE, gas_id);
}


/*===========================================================================

FUNCTION  L1_SEND_MESSAGE

DESCRIPTION
  Function which calls gs_send_message without changing any of the
  parameters. It is used in a debug situation to cause the message which
  is to be sent to be dumped to the screen.

DEPENDENCIES
  queue_id : IN - Id of the queue where the message is to be sent
  message_p : IN - pointer to message to be sent
  wait : IN - flag to indicated if call should block on transmission or not
  gas_id : added for multi-SIM support (currently not used)

RETURN VALUE
  status - indication of success or otherwise of the gs_send_message call

SIDE EFFECTS
  None

===========================================================================*/
gs_status_T L1_send_message(gs_queue_id_T queue_id, void *message_p, boolean wait, gas_id_t gas_id)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /*To extract msg_header*/
  ph_connect_ind_T *msg;
  msg = message_p;

  // reroute the message to a different queue if in FTM mode.
  if (L1_ftm_mode_enabled() && ((queue_id == GS_QUEUE_RR) || (queue_id == GS_QUEUE_LAYER_2)))
  {
    queue_id = GS_QUEUE_FTM_GSM;
  }


  /*for logging messages other than to L1*/
  if (msg->message_header.message_set != MS_L1_L1)
  {
#ifdef L1_MSG_TRAFFIC_LOGGING
     l1_log_msg_traffic(msg->message_header.message_set, msg->message_header.message_id, GSTMR_GET_FN_GERAN(gas_id), FALSE, gas_id);
#endif
  }

  (void)gas_id;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  return gs_send_message(queue_id, message_p, wait);
}


/*===========================================================================

FUNCTION  L1_ISR_SEND_MESSAGE

DESCRIPTION
  Function which calls gs_send_message without changing any of the parameters.

DEPENDENCIES
  queue_id : IN - Id of the queue where the message is to be sent
  message_p : IN - pointer to message to be sent
  wait : IN - flag to indicated if call should block on transmission or not
  gas_id : added for multi-SIM support (currently not used)

RETURN VALUE
  status - indication of success or otherwise of the gs_send_message call

SIDE EFFECTS
  None

===========================================================================*/
gs_status_T L1_isr_send_message(gs_queue_id_T queue_id, void *message_p, boolean wait, gas_id_t gas_id)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /*To extract msg_header*/
  ph_connect_ind_T *msg;
  msg = message_p;

  // reroute the message to a different queue if in FTM mode.
  if (L1_ftm_mode_enabled() && ((queue_id == GS_QUEUE_RR) || (queue_id == GS_QUEUE_LAYER_2)))
  {
    queue_id = GS_QUEUE_FTM_GSM;
    if (msg->message_header.message_id == MPH_STOP_GSM_MODE_CNF)
    {
      /*RF task is blocked till this signal is set*/
      ftm_gsm_set_exit_cmd_sig();
    }
  }

  /*for logging messages other than to L1*/
  if (msg->message_header.message_set != MS_L1_L1)
  {
#ifdef L1_MSG_TRAFFIC_LOGGING
     l1_log_msg_traffic(msg->message_header.message_set, msg->message_header.message_id, GSTMR_GET_FN_GERAN(gas_id), FALSE, gas_id);
#endif
  }

  (void)gas_id;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  return gs_send_message ( queue_id, message_p, wait);
}

/*===========================================================================

FUNCTION  L1_SEND_MPH_NC_MEASUREMENT_IND

DESCRIPTION
  This function sends the GLL1_GPRS_SEND_MPH_NC_MEASUREMENT_IND message.
  NC Measurements reported to RR sest six
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_NC_MEASUREMENT_IND(gas_id_t gas_id)
{
   volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   mph_nc_measurement_ind_t msg;
   surrounding_channel_info_T  *chnl_info;
   l1_sc_bs_T*             cells[ 32 ];
   l1_sc_bs_T*             cell;
   int                     n;

   msg.message_header.message_set = MS_RR_L1;
   msg.message_header.message_id = MPH_NC_MEASUREMENT_IND;   /*lint !e641 */
   PUT_IMH_LEN( sizeof ( msg ) - sizeof ( IMH_T ), &msg );
   msg.gas_id = gas_id;
   msg.no_of_entries = l1_sc_balist_get_best_thirtytwo( cells, gas_id );

   for ( n = 0; n < msg.no_of_entries; ++n )
   {
      cell = cells[ n ];
      chnl_info = &(msg.surrounding_channel_info[ n ]);

      /* Initialise the validity flag to FALSE */
      chnl_info->valid_info = FALSE;

      /* Enforce the 5 measurements rla_p rule for the
       * GPRS 52 idle only.
       */
      if( L1_GPRS_IDLE_MODE == l1_tskisr_blk->l1_state )
      {
          /* Make sure the 5 seconds average has been met and
           * the bsic has been decoded.
           */
          if(  ( FALSE != l1_running_avg_is_full( &(cell->ra) ) )
             &&( FALSE != cell->bsic_known )
            )
          {
              /* Report the cell */
              chnl_info->valid_info = TRUE;
          }
      }
      else
      {
          if( FALSE != cell->bsic_known )
          {
              /* Report the cell */
              chnl_info->valid_info = TRUE;
          }
      }

      chnl_info->BCCH_ARFCN = cell->arfcn;
      chnl_info->RXLEV_average  = L1_map_dBm_x16_to_rxlev(cell->rx_power);

      /* Reconstruct SCH data (BSIC & RFN).
       * SCH data is sent to RR in MPH_SURROUND_MEAS_IND.
       * RR does not use RFN, so only fill in BSIC.
       * BSIC is b2-b7 in byte 0 of SCH data.
       * Remaining bytes are for RFN.
       * SCH data will be refreshed on SCH reconf.
       */
      chnl_info->SCH_block[ 0 ] = cell->bsic << 2; /* BSIC */
      chnl_info->SCH_block[ 1 ] = 0;               /* RFN  */
      chnl_info->SCH_block[ 2 ] = 0;               /* RFN  */
      chnl_info->SCH_block[ 3 ] = 0;               /* RFN  */

      MSG_GERAN_LOW_2_G("NC_Ncell %d NC_rxlev_aver %d ",chnl_info->BCCH_ARFCN.num,cell->rx_power);
      MSG_GERAN_LOW_0_G("Ncell INFO");
   }

   for ( ; n < MAX_NUMBER_OF_BEST_CELLS; ++n )
   {
      msg.surrounding_channel_info[ n ].valid_info = FALSE;
   }

   (void)L1_send_message( GS_QUEUE_RR, &msg, TRUE, gas_id );
   MSG_GERAN_LOW_0_G("***NC_MEAS***");
}

/*===========================================================================

FUNCTION  L1_SEND_PH_DATA_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_L2
  queue.

DEPENDENCIES
  good_data, channel type,  data length
  own_paging_block
  channel ARFCN
  input data pointer,
  data length

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_PH_DATA_IND( boolean  good_data,
                          uint16     channel_type,
                          boolean  own_paging_block,
                          ARFCN_T  ARFCN,
                          byte    *data_ptr,
                          byte     data_length, gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
    gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_PH_DATA_IND;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->ph_data_ind.param.channel_number = ARFCN;
      gplt_cmd_ptr->ph_data_ind.param.own_paging_block = own_paging_block;

      gplt_cmd_ptr->ph_data_ind.param.l2_channel_type = (byte)channel_type;
      gplt_cmd_ptr->ph_data_ind.param.tc = 0xFF;

      if (GL1_MSG_CBCH == channel_type)
      {
        /* logical CBCH channel was either SDCCH_8 or SDCCH_4 so just use SDCCH for Layer 2 */
        gplt_cmd_ptr->ph_data_ind.param.l2_channel_type = SDCCH;
        gplt_cmd_ptr->ph_data_ind.param.cbch_frame = l1_smscb_get_cbch_fn(gas_id);
      }
      else
      {
        gplt_cmd_ptr->ph_data_ind.param.l2_channel_type = (byte)channel_type;
        gplt_cmd_ptr->ph_data_ind.param.cbch_frame = 0;
      }

      if ( good_data )
      {
        if(data_ptr)
        {
          memscpy(gplt_cmd_ptr->ph_data_ind.param.layer2_frame,
                  sizeof(gplt_cmd_ptr->ph_data_ind.param.layer2_frame),
                  data_ptr,
                  data_length);
        }
      }

   if((l1_dedicated_data_ptr->rsacch_ctrl.nv_active==TRUE)&&(channel_type==SACCH))
   {
       /*Add SRO Sacch Repeat Order flag : based on r_sacch_sro_flag*/
       gplt_cmd_ptr->ph_data_ind.param.l2_repetition_order = l1_dedicated_data_ptr->rsacch_ctrl.sro_flag;
       MSG_GERAN_HIGH_2_G("R-SACCH: DL PH_DATA_IND (SACCH) SRO %d, good_data %d",gplt_cmd_ptr->ph_data_ind.param.l2_repetition_order,good_data);
   }
   else
   {
      gplt_cmd_ptr->ph_data_ind.param.l2_repetition_order = 0;
   }

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_PH_DATA_IND");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else

   /* Locals */

   ph_data_ind_T                 message_buffer,*message;
   int                           message_length = 0;
   dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_L2_L1;
   message->message_header.message_id = PH_DATA_IND;

   message->own_paging_block = own_paging_block;
   message->channel_number = ARFCN;
   
   if (GL1_MSG_CBCH == channel_type)
   {
     /* logical CBCH channel was either SDCCH_8 or SDCCH_4 so just use SDCCH for Layer 2 */
     message->l2_channel_type = SDCCH;
     message->cbch_frame = l1_smscb_get_cbch_fn(gas_id);
   }
   else
   {
     message->l2_channel_type = (byte)channel_type;
     message->cbch_frame = 0;
   }
   message->tc = 0xFF;
   message->gas_id = gas_id;
   message->dual_sim_mode_enabled = gl1_msg_get_multi_sim_mode();

   if ( good_data )
   {
      if(data_ptr)
      {
        memscpy(message->layer2_frame,sizeof(message->layer2_frame),data_ptr,data_length);
        message_length = sizeof(ph_data_ind_T) - sizeof(IMH_T);
      }
      else
      {
        MSG_GERAN_ERROR_0_G("data_ptr NULL !!");
      }
   }
   else
   {
      message_length = sizeof(ph_data_ind_T) - sizeof(IMH_T) - N201_MAX;
   }

   if((l1_dedicated_data_ptr->rsacch_ctrl.nv_active==TRUE)&&(channel_type==SACCH))
   {
       /*Add SRO Sacch Repeat Order flag : based on r_sacch_sro_flag*/
       message->l2_repetition_order = l1_dedicated_data_ptr->rsacch_ctrl.sro_flag;
       MSG_GERAN_HIGH_2_G("R-SACCH: DL PH_DATA_IND (SACCH) SRO %d, good_data %d",message->l2_repetition_order,good_data);
   }
   else
   {
       message->l2_repetition_order = 0;
   }

   PUT_IMH_LEN(message_length,message);

   /* Send the message */

   (void)L1_isr_send_message(GS_QUEUE_LAYER_2,message,TRUE, gas_id);
#endif /*FEATURE_GL1_GPLT*/
}

#ifdef FEATURE_GSM_CB_DRX_SUPPORTED
/*===========================================================================

FUNCTION  L1_SEND_CB_SCHED_EXPIRY_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_CB
  queue.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_CB_SCHED_EXPIRY_IND( cb_channel_ind_type chan, gas_id_t gas_id )
{
  /* Locals */
  l1_cb_sched_expiry_ind_type message_buffer,*message;
  int                         message_length;

  /* Align the pointers */
  message = &message_buffer;

  /* Set up the message header */
  message->message_header.message_set = MS_CB_L1;
  message->message_header.message_id  = L1_CB_SCHED_EXPIRY_IND;  /*lint !e641 */
  message->channel_ind                = chan;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  message->as_id                      =
    geran_map_gas_id_to_nas_id( gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  message_length = ( sizeof(l1_cb_sched_expiry_ind_type) - sizeof(IMH_T) );

  PUT_IMH_LEN( message_length, message );

  /* Send the message */
  (void)L1_isr_send_message( GS_QUEUE_CB, message, TRUE, gas_id );
}

/*===========================================================================

FUNCTION  L1_SEND_CB_SCHEDULING_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_CB
  queue.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_CB_SCHEDULING_CNF( uint32 *fn_list, gas_id_t gas_id )
{
  /* Locals */
  l1_cb_scheduling_cnf_type message_buffer, *message;
  int                       message_length;

  /* Align the pointers */
  message = &message_buffer;

  /* Set up the message header */
  message->message_header.message_set = MS_CB_L1;
  message->message_header.message_id  = L1_CB_SCHEDULING_CNF;  /*lint !e641 */
  message->scheduling_period_fn_list  = fn_list;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  message->as_id                      =
    geran_map_gas_id_to_nas_id( gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  message_length = ( sizeof(l1_cb_scheduling_cnf_type) - sizeof(IMH_T) );

  PUT_IMH_LEN( message_length, message );

  /* Send the message */
  (void)L1_isr_send_message( GS_QUEUE_CB, message, TRUE, gas_id );
}
#endif /* FEATURE_GSM_CB_DRX_SUPPORTED */

/*===========================================================================

FUNCTION  L1_SEND_CB_MISSING_BLOCK_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_CB
  queue.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_CB_MISSING_BLOCK_IND( uint32 cbch_fn, gas_id_t gas_id )
{
  /* Locals */
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_cb_missing_block_ind_type message_buffer, *message;
  int                          message_length;

  /* Align the pointers */
  message = &message_buffer;

  /* Set up the message header */
  message->message_header.message_set = MS_CB_L1;
  message->message_header.message_id  = L1_CB_MISSING_BLOCK_IND; /*lint !e641 */
  message->fn                         = cbch_fn;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  message->as_id                      =
    geran_map_gas_id_to_nas_id( gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  if (l1_tskisr_blk->l1_state == L1_DEDICATED_MODE || l1_tskisr_blk->l1_state == L1_TRANSFER_MODE
#ifdef FEATURE_GSM_DTM
        || l1_tskisr_blk->l1_state == L1_DTM_MODE
#endif
      )
  {
    message->is_l1state_idle            = 0;
  }
  else
  {
    message->is_l1state_idle            = 1;
  }

  message_length = ( sizeof(l1_cb_missing_block_ind_type) - sizeof(IMH_T) );

  PUT_IMH_LEN( message_length, message );

  /*reset the decoding_cbch flags as we have sent the CBCH missing indication above*/
  l1_smscb_set_decoding_basic_cbch (FALSE, gas_id );
  l1_smscb_set_decoding_extended_cbch(FALSE, gas_id );

  /* Send the message */
  (void)L1_isr_send_message( GS_QUEUE_CB, message, TRUE, gas_id );
}

/*===========================================================================

FUNCTION  L1_SEND_CB_SKIP_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_CB
  queue.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_CB_SKIP_CNF( uint32 *fn_list, gas_id_t gas_id )
{
  /* Locals */
  l1_cb_skip_cnf_type message_buffer, *message;
  int                 message_length;

  /* Align the pointers */
  message = &message_buffer;

  /* Set up the message header */
  message->message_header.message_set = MS_CB_L1;
  message->message_header.message_id  = L1_CB_SKIP_CNF;  /*lint !e641 */
  message->scheduling_period_fn_list  = fn_list;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  message->as_id                      =
    geran_map_gas_id_to_nas_id( gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  message_length = ( sizeof(l1_cb_skip_cnf_type) - sizeof(IMH_T) );

  PUT_IMH_LEN( message_length, message );

  /* Send the message */
  (void)L1_isr_send_message( GS_QUEUE_CB, message, TRUE, gas_id );
}

/*===========================================================================

FUNCTION  L1_SEND_SYSINFO_PH_DATA_IND

DESCRIPTION
  Same as L1_send_PH_DATA_IND except that this function allows TC to be
  specified.  TC is only relevant for sysinfo messages.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_sysinfo_PH_DATA_IND( boolean  good_data,
                                  uint16     channel_type,
                                  boolean  own_paging_block,
                                  ARFCN_T  ARFCN,
                                  uint8    TC,
                                  byte    *data_ptr,
                                  byte     data_length, gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_PH_DATA_IND;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->ph_data_ind.param.channel_number = ARFCN;
      gplt_cmd_ptr->ph_data_ind.param.own_paging_block = own_paging_block;

      gplt_cmd_ptr->ph_data_ind.param.l2_channel_type = (byte)channel_type;
      gplt_cmd_ptr->ph_data_ind.param.tc = TC;

      if ( good_data )
      {
        if(data_ptr)
        {
          memscpy(gplt_cmd_ptr->ph_data_ind.param.layer2_frame,
                  sizeof(gplt_cmd_ptr->ph_data_ind.param.layer2_frame),
                  data_ptr,
                  data_length);
        }
      }
      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_PH_DATA_IND");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   /* Locals */
   ph_data_ind_T                 message_buffer,*message;
   int                           message_length = 0;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_L2_L1;
   message->message_header.message_id = PH_DATA_IND;

   message->own_paging_block = own_paging_block;
   message->channel_number = ARFCN;
   message->l2_channel_type = (byte)channel_type;
   message->tc = TC;
   message->gas_id = gas_id;
   message->dual_sim_mode_enabled = gl1_msg_get_multi_sim_mode();

   if ( good_data )
   {
      if(data_ptr)
      {
        memscpy(message->layer2_frame,sizeof(message->layer2_frame),data_ptr,data_length);
        message_length = sizeof(ph_data_ind_T) - sizeof(IMH_T);
      }
      else
      {
        MSG_GERAN_ERROR_0_G("data_ptr NULL!!");
      }
   }
   else
   {
      message_length = sizeof(ph_data_ind_T) - sizeof(IMH_T) - N201_MAX;
   }

   PUT_IMH_LEN(message_length,message)

      /* Send the message */

   (void)L1_isr_send_message(GS_QUEUE_LAYER_2,message,TRUE, gas_id);
#endif /*FEATURE_GL1_GPLT*/
}

/*===========================================================================

FUNCTION  L1_SEND_PH_RANDOM_ACCESS_CNF

DESCRIPTION
  This function sends the PH_RANDOM_ACCESS_CNF to L2.

DEPENDENCIES
  Frame number mod 42432, random access content

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_PH_RANDOM_ACCESS_CNF(uint16 ra_content, word sent_FNmod42432, gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_PH_RANDOM_ACCESS_CNF;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->random_access_cnf.param.frame_number.lsb = (byte) (sent_FNmod42432 % 256);
      gplt_cmd_ptr->random_access_cnf.param.frame_number.msb = (byte) (sent_FNmod42432 / 256);;

      gplt_cmd_ptr->random_access_cnf.param.l2_channel_type = L2_RACH;
      gplt_cmd_ptr->random_access_cnf.param.random_access_content = ra_content;


      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_PH_RANDOM_ACCESS_CNF");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   ph_random_access_cnf_T        message_buffer,*message;
   int                           message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_L2_L1;
   message->message_header.message_id  = PH_RANDOM_ACCESS_CNF;
   message_length = sizeof(ph_random_access_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* Set up the data in the message body */

   message->random_access_content = ra_content;
   message->l2_channel_type       = L2_RACH;
   message->frame_number.lsb      = (byte) (sent_FNmod42432 % 256);
   message->frame_number.msb      = (byte) (sent_FNmod42432 / 256);
   message->gas_id                = gas_id;
   message->result                = GERAN_ACCESS_GRANTED;

   /* Send the message */

   (void)L1_isr_send_message(GS_QUEUE_LAYER_2,message,TRUE, gas_id);
#endif
}

/*===========================================================================

FUNCTION  L1_SEND_MPH_BLOCK_QUALITY_IND

DESCRIPTION
  This function sends the MPH_BLOCK_QUALITY_IND to L2.

DEPENDENCIES
  boolean (Good/Bad)

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_BLOCK_QUALITY_IND(byte chnl_type, gl1_block_quality_t   good_frame, gas_id_t gas_id)
{
#if defined (FEATURE_DUAL_SIM) || defined (FEATURE_TRIPLE_SIM)
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_BLOCK_QUALITY_IND;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->block_quality_ind.param.block_quality = good_frame;

      gplt_cmd_ptr->block_quality_ind.param.l2_channel_type = (byte)chnl_type;
      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_BLOCK_QUALITY_IND");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else

   mph_block_quality_ind_T        message_buffer,*message;
   word                           message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_BLOCK_QUALITY_IND;   /*lint !e641 */
   message_length = sizeof(mph_block_quality_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* Set up the data in the message body */
   message->gas_id = gas_id;
   message->l2_channel_type = chnl_type;
   message->block_quality = good_frame;
   message->block_timestamp = timetick_get_safe();
   message->bplmn_fail_count = gl1_retrieve_bplmn_fail_count(TRUE, gas_id);
   gpl1_serving_cell_meas[gas_id].stale_meas = FALSE;   

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
   if (chnl_type == CCCH)
   {
     if (good_frame == GL1_BLOCK_QUALITY_BAD)
     {
       gl1_qsh_page_dec_fail_cnt[gas_id]++;

       if (gl1_qsh_page_dec_fail_cnt[gas_id] >= GL1_QSH_PAG_DEC_FAIL_CNT_THRESHOLD)
       {
         gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_PAGE_DEC_FAIL);
         gl1_qsh_page_dec_fail_cnt[gas_id] = 0;
       }
     }
     else if (good_frame == GL1_BLOCK_QUALITY_GOOD)
     {
       gl1_qsh_page_dec_fail_cnt[gas_id] = 0;
     }
     
     gl1_qsh_page_miss_cnt[gas_id] = 0;
   }
#endif

#if defined (FEATURE_GSM_SIMULATE_GL1_RF) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
   #error code not present
#endif /* FEATURE_GSM_SIMULATE_GL1_RF */
#if defined (FEATURE_DUAL_SIM) || defined (FEATURE_TRIPLE_SIM)
   /* Trigger Hybrid Tuneaway State machine*/
   /* Don't trigger hybrid tuneaway state machine on decoding AGCH block in the RACH state*/
   if (CCCH == chnl_type && 
       (l1_tskisr_blk->l1_state != L1_TRANSFER_MODE))
   {
     gl1_drdsds_hybrid_tuneaway_trigger(DRDSDS_MPH_BLOCK_QUALITY_IND_EVENT, 
                                        (gl1_drdsds_hybrid_tuneaway_msg *)message, 
                                        l1_tskisr_blk->client_id,
                                        gas_id); 
   }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#endif /* FEATURE_GL1_GPLT */
}


/*===========================================================================

FUNCTION  L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND

DESCRIPTION
  This function sends the MPH_BLOCK_QUALITY_IND to L2 whenevr the TRM lock
  was denied and thererfore the block was not read.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND( gas_id_t gas_id )
{
   mph_block_quality_ind_T        message_buffer,*message;
   word                           message_length;
   idle_data_T                   *l1_idle_data_ptr=&l1_idle_data_store[gas_id];

   /* Align the pointers */
   message = &message_buffer;

   /* Set up the message header */
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_BLOCK_QUALITY_IND;   /*lint !e641 */
   message_length = sizeof(mph_block_quality_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* Set up the data in the message body */
   message->l2_channel_type = CCCH;
   message->block_quality = GL1_BLOCK_QUALITY_UNKNOWN;
   message->gas_id = gas_id;

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
   gl1_qsh_page_dec_fail_cnt[gas_id] = 0;

   gl1_qsh_page_miss_cnt[gas_id]++;
   if (gl1_qsh_page_miss_cnt[gas_id] >= GL1_QSH_PAG_MISS_CNT_THRESHOLD)
   {
     gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_PAGE_MISS);
     gl1_qsh_page_miss_cnt[gas_id] = 0;
   }
#endif
   
   if( gl1_hw_sleep_wmgr_cancel_in_prog[gas_id])
   {
     message->block_timestamp = gl1_hw_sleep_wmgr_missed_PCH_time[gas_id];
   }
   else
   { 
   message->block_timestamp = timetick_get_safe();
   }
   message->bplmn_fail_count = gl1_retrieve_bplmn_fail_count(TRUE, gas_id);
   gpl1_serving_cell_meas[gas_id].stale_meas = TRUE;

   /* Send the message */
   (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);

   l1_idle_data_ptr->idle_rx_missed_page = TRUE;
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

/*===========================================================================

FUNCTION  L1_send_PH_RANDOM_ACCESS_CNF_with_result

DESCRIPTION
  This function sends the PH_RANDOM_ACCESS_CNF to L2.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_PH_RANDOM_ACCESS_CNF_with_result(const gas_id_t gas_id, const geran_result_t rach_result)
{
   ph_random_access_cnf_T        message_buffer,*message;
   int                           message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* Set up the message header */
   message->message_header.message_set = MS_L2_L1;
   message->message_header.message_id  = PH_RANDOM_ACCESS_CNF;
   message_length = sizeof(ph_random_access_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* Set up the data in the message body */
   message->gas_id = gas_id;
   message->result = rach_result;

   /* Send the message */

   (void)L1_isr_send_message(GS_QUEUE_LAYER_2,message,TRUE, gas_id);
}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */



#ifdef FEATURE_GSM_QSC_TX_DIV

#ifdef FEATURE_GERAN_DRDSDS



/*
FUNCTION  gl1_trm_asdiv_traffic_denial_thresh_check

PARAMS:
    new_config_success: Whether the current set antenna request was denied or granted
    
DESCRIPTION
    This function checks to see if the new antenna switch position for the traffic sub has been 
    consecutively denied gsm_qsc_traffic_crisis_mode_thre times.
    
    gsm_qsc_traffic_crisis_mode_thre is set in antenna_swth_get_nv_item_qsc()
    
    If this is true, we set the crisis mode flag in the GL1/TRM interface the next time GL1 attempts to set an antena position

RETURN VALUE
  Void
 
*/

void gl1_trm_asdiv_traffic_denial_thresh_check( gas_id_t gas_id,
                                              trm_set_ant_switch_return_type new_config_success)
{
    gl1_trm_asdiv_drds_mode_t * gl1_trm_asdiv_drds_mode_data = &gl1_trm_asdiv_drds_mode_var[gas_id];
    if(new_config_success == TRM_ANT_SWITCH_DIV_SET_CFG_DENIED)
    {
        MSG_GERAN_HIGH_0_G("Asdiv Crisis Mode: Asdiv RxTx Denial Increment");
        gl1_trm_asdiv_drds_mode_data->is_crisis_mode_traffic = gl1_trm_asdiv_increment_crisis_or_pingpong_mode_cnt(gas_id, TRM_CLIENT_MODE_RXTX);
    }
    else
    {
        MSG_GERAN_HIGH_0_G("Asdiv Crisis Mode: Asdiv RxTx Denial Clear");
        gl1_trm_asdiv_clear_consecutive_thresh_cnt(gas_id, TRM_CLIENT_MODE_RXTX);
    }
}



/*
FUNCTION  gl1_trm_asdiv_idle_pingpong_thresh_check

PARAMS:
    new_idle_config: The antenna that is currently being used on this wakeup cycle
    
DESCRIPTION
    In idle, if there are gsm_qsc_idle_pingpong_mitigation_thre consecutive switches of the antenna, 
    where the new switch position is different than the previous position, 
    then we stay on current config for idle_pingpong_suspension_count
    cycles.
    gsm_qsc_idle_pingpong_mitigation_thre and idle_pingpong_suspension_count
    are set in the function: antenna_swth_get_nv_item_qsc

RETURN VALUE
  In Idle Mode, returns the config to set. 
 
*/

trm_ant_switch_div_config_type gl1_trm_asdiv_idle_pingpong_thresh_check(gas_id_t gas_id, 
                                                                     int32 new_idle_config)
{
    /*Return Value for Idle Mode*/
    trm_ant_switch_div_config_type asdiv_config_to_return = TRM_ANT_SWITCH_DIV_CONFIG_CURRENT;
    gl1_trm_asdiv_drds_mode_t * gl1_trm_asdiv_drds_mode_data = &gl1_trm_asdiv_drds_mode_var[gas_id];
    
    /*Suspend Idle Switch Activities*/
    if(gl1_trm_asdiv_drds_mode_data->idle_pingpong_suspension_count > 0)
    {
        MSG_GERAN_HIGH_2_G("Idle Switch Mitigation: Suspension State count = %d. Stay on Current Antenna = %d", 
                            gl1_trm_asdiv_drds_mode_data->idle_pingpong_suspension_count,
                            gl1_trm_asdiv_drds_mode_data->last_type2_alg_antenna_to_use);
        if(gl1_trm_asdiv_drds_mode_data->idle_pingpong_suspension_count == 0)
        {
            MSG_GERAN_HIGH_0_G("Idle Switch Mitigation: Leaving Idle Switch Suspension State");
        }
        else
        {
            gl1_trm_asdiv_drds_mode_data->idle_pingpong_suspension_count--;
        }
    }
    /*Check the Idle Switch States*/
    else 
    {
        /*Different State*/ 
        if(gl1_trm_asdiv_drds_mode_data->last_type2_alg_antenna_to_use != new_idle_config)
        {
            MSG_GERAN_HIGH_2_G("Idle Switch Mitigation: Change State. Last Config = %d, New Config = %d", 
                                gl1_trm_asdiv_drds_mode_data->last_type2_alg_antenna_to_use,
                                new_idle_config);
            gl1_trm_asdiv_drds_mode_data->last_type2_alg_antenna_to_use = new_idle_config;
            asdiv_config_to_return = TRM_ANT_SWITCH_DIV_CONFIG_PREFERRED;
        }
        /*Clear the counter*/
        else
        {
            MSG_GERAN_HIGH_2_G("Idle Switch Mitigation: Same State. Last Config = %d, New Config = %d", 
                                gl1_trm_asdiv_drds_mode_data->last_type2_alg_antenna_to_use,
                                new_idle_config);
            gl1_trm_asdiv_clear_consecutive_thresh_cnt(gas_id, TRM_CLIENT_MODE_IDLE_WAKEUP);
            asdiv_config_to_return = TRM_ANT_SWITCH_DIV_CONFIG_PREFERRED;
        }

        /*Idle state consecutive switch check. Check for next wakeup cycle to suspend switching*/
        if(gl1_trm_asdiv_increment_crisis_or_pingpong_mode_cnt(gas_id, TRM_CLIENT_MODE_IDLE_WAKEUP))
        {
            gl1_trm_asdiv_drds_mode_data->idle_pingpong_suspension_count = gsm_qsc_idle_suspension_time[gas_id];
        }
    }
    
    return asdiv_config_to_return;

}



/*
FUNCTION  gl1_trm_asdiv_get_traffic_crisis_flag

DESCRIPTION
    Gets the traffic crisis flag

RETURN VALUE
  Returns whethers the traffic crisis flag was set to true
*/


boolean gl1_trm_asdiv_get_traffic_crisis_flag(gas_id_t gas_id)
{
  boolean is_crisis_mode = gl1_trm_asdiv_drds_mode_var[gas_id].is_crisis_mode_traffic;
  return is_crisis_mode;
}


/*
FUNCTION  gl1_trm_asdiv_set_traffic_crisis_flag

DESCRIPTION
    Gets the traffic crisis flag

RETURN VALUE
  Returns whethers the traffic crisis flag was set to true
*/


void gl1_trm_asdiv_set_traffic_crisis_flag(gas_id_t gas_id, boolean flag)
{
          gl1_trm_asdiv_drds_mode_var[gas_id].is_crisis_mode_traffic = flag;
}

/*
FUNCTION  gl1_trm_asdiv_clear_consecutive_thresh_cnt

DESCRIPTION
    Clears the counter for either
    a.) The number of consecutive preferred config denials
    b.) The number of consecutive ping-pong switches in idle (ping-pong meaning each switch results in
        a new switch position
  
RETURN VALUE
*/
static void gl1_trm_asdiv_clear_consecutive_thresh_cnt(gas_id_t gas_id, trm_client_mode_type mode_type)
{
    gl1_trm_asdiv_drds_mode_t * gl1_trm_asdiv_drds_mode_data = &gl1_trm_asdiv_drds_mode_var[gas_id];

    MSG_GERAN_HIGH_0_G("Clearing consecutive thresh cnt flag!");
    switch(mode_type)
    {
        case TRM_CLIENT_MODE_IDLE_WAKEUP:
            gl1_trm_asdiv_drds_mode_data->idle_mode_consecutive_preferred_count = 0;
            break;
            
        case TRM_CLIENT_MODE_RXTX:
            gl1_trm_asdiv_drds_mode_data->traffic_mode_consecutive_denied_count = 0;
            break;
            
        default:
            MSG_GERAN_ERROR_0_G("Invalid Mode Type, clearing both counts");
            gl1_trm_asdiv_drds_mode_data->idle_mode_consecutive_preferred_count = 0;
            gl1_trm_asdiv_drds_mode_data->traffic_mode_consecutive_denied_count = 0;
    }
}


/*
FUNCTION  gl1_trm_asdiv_increment_crisis_or_pingpong_mode_cnt

DESCRIPTION
  Increments the threshold counter for
    a.) Traffic. If traffic mode is denied its switch request
    b.) Idle. If IDLE mode has consecutively changed its antenna over the past wakeup cycles
  
  Sets the crisis mode flag, or the ping-pong suspension flag, if threshold has been reached
  
RETURN VALUE
 
*/
static boolean gl1_trm_asdiv_increment_crisis_or_pingpong_mode_cnt(gas_id_t gas_id, trm_client_mode_type mode_type)
{
    uint8 * thresh_count;
    uint8 thresh = 0;
    boolean is_over_threshold = FALSE;
    gl1_trm_asdiv_drds_mode_t * gl1_trm_asdiv_drds_mode_data = &gl1_trm_asdiv_drds_mode_var[gas_id];
    /*Get Idle/Traffic Crisis mode count Parameters*/
    switch(mode_type)
    {
        case TRM_CLIENT_MODE_IDLE_WAKEUP:
            thresh_count = &gl1_trm_asdiv_drds_mode_data->idle_mode_consecutive_preferred_count;
            thresh = gsm_qsc_idle_pingpong_mitigation_thre[gas_id];
            break;
            
        case TRM_CLIENT_MODE_RXTX:
            thresh_count = &gl1_trm_asdiv_drds_mode_data->traffic_mode_consecutive_denied_count;
            thresh = gsm_qsc_traffic_crisis_mode_thre[gas_id];
            break;
            
        default:
            MSG_GERAN_ERROR_0_G("Invalid Mode Type, defaulting to IDLE count");
            thresh_count = &gl1_trm_asdiv_drds_mode_data->idle_mode_consecutive_preferred_count;
            thresh = gsm_qsc_idle_pingpong_mitigation_thre[gas_id];
    }

    if(thresh == 0)
    {
        MSG_GERAN_ERROR_1_G("Threshold for Mode = %d is not enabled, returning!", mode_type);
        return FALSE;
    }
    
    /*Increment Crisis Count*/
    (*thresh_count)++;

    is_over_threshold = (*thresh_count >= thresh);
    
    /*Reset Crisis Mode Count and Set Crisis Mode to TRUE*/
    if(is_over_threshold)
    {
        gl1_trm_asdiv_clear_consecutive_thresh_cnt(gas_id, mode_type);
    }

    return is_over_threshold;
    

}




#endif /*FEATURE_GERAN_DRDSDS*/

static void l1_asdiv_ded_clear_meas(gas_id_t gas_id)
{
    gl1_trm_asdiv_traffic_mode_T * gl1_trm_asdiv_traffic_var = &gl1_trm_asdiv_traffic_store[gas_id];
    gl1_trm_asdiv_traffic_var->type2_algorithm_start = 0;
    gl1_trm_asdiv_traffic_var->previous_rxlev_sub = -1;
    gl1_trm_asdiv_traffic_var->new_rxlev_backup = -1;
    gl1_trm_asdiv_traffic_var->old_rxlev_average = -1;
	gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_accum = 0;
	gl1_hw_asdiv_mtpl_store[gas_id].number_of_tx_pwr_avg = 0;
	gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_avg_over_sacch = 0;

}

static void l1_asdiv_reset_wait_counter(gas_id_t gas_id)
{
    gl1_trm_asdiv_traffic_store[gas_id].wait_counter = -1;
}

/*===========================================================================

FUNCTION  l1_asdiv_ded_meas_qsc_alg

DESCRIPTION
  The antenna switching algorithm. Please make mention of the below comments indicating switch state:

 POST_SWITCH
  - We enter this state after a switch
  - This state is responsible for determining whether we stay on the current antenna or
  switch back, immediately.
  - In both circumstances, we enter GL1_ASDIV_WAIT_MEASURE
  - Min wait in this state of 1 meas period (480ms) Max wait of 2 meas periods (960ms)

 WAIT_PERIOD:
  - This state is the waiting state. No measurements are taken in this state
  - The duration of this state is set via the ASDIV_HYSTERESIS_TIME NV*480ms
  - At the end of state we move to GL1_ASDIV_PRE_SWITCH_WAIT_MEASURE

 PRE_SWITCH:
  - To determine whether we switch, we must sample the antenna for two periods
  - If the average RSSI after two periods is above the threshold, now switch
  - If below, we switch

DEPENDENCIES
  Rxlev sub


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static void l1_asdiv_ded_meas_qsc_alg(gas_id_t gas_id, int32 Rxlev_sub)
{
  gl1_trm_asdiv_traffic_mode_T * gl1_trm_asdiv_traffic_var = &gl1_trm_asdiv_traffic_store[gas_id];
  int16 threshold;
 #ifdef FEATURE_GERAN_SAR 
  if(gl1_hw_check_rf_switch_disable(gas_id))
  {
    l1_asdiv_ded_clear_meas(gas_id);
	return;
  }
  #endif
  if (gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE2))
  {
    gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_avg_over_sacch = 
	gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_accum/gl1_hw_asdiv_mtpl_store[gas_id].number_of_tx_pwr_avg;

	MSG_GERAN_HIGH_5_G("wait_counter = %d, previous_rxlev_sub = %d, tx_pwr_avg_over_sacch = %d, mtpl_ant_1 = %d, mtpl_ant_2 = %d",
                        gl1_trm_asdiv_traffic_var->wait_counter,
                        gl1_trm_asdiv_traffic_var->previous_rxlev_sub,
                        gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_avg_over_sacch,
	                    gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_1,
		                gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2);
	
	/*if mtpl is not a legit value, set it to 40 dBm so the Tx condition will be bypass in the algorithm*/
    if(gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_1 <= 250 )
    {
      gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_1 = 400;
    }
	if(gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2 <= 250 )
    {
      gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2 = 400;
    }  

    /*PRE and POST SWITCH Cases*/
      if (gl1_trm_asdiv_traffic_var->wait_counter< 0 )
      {
        if (gl1_trm_asdiv_traffic_var->previous_rxlev_sub >= 0 )
        {
          MSG_GERAN_HIGH_4_G( "type2_algorithm_start = %d, old_rxlev_average = %d, Rxlev_sub = %d, new_rxlev_backup = %d",
		                      gl1_trm_asdiv_traffic_var->type2_algorithm_start,
							  gl1_trm_asdiv_traffic_var->old_rxlev_average,
							  Rxlev_sub,
							  gl1_trm_asdiv_traffic_var->new_rxlev_backup);

          /*POST_SWITCH, determine whether we switch back*/
          if (gl1_trm_asdiv_traffic_var->type2_algorithm_start )
          {
              if(type2_alg_antenna_to_use[gas_id] == 1 )
              {
                threshold = gsm_qsc_large_delta[gas_id] - gl1_hw_asdiv_mtpl_store[gas_id].UL_delta;
              }
			  else
			  {
			    threshold = gsm_qsc_large_delta[gas_id] + gl1_hw_asdiv_mtpl_store[gas_id].UL_delta;
			  }
			  MSG_GERAN_HIGH_3_G("threshold:%d, large_delta:%d, UL_delta=%d",threshold,gsm_qsc_large_delta[gas_id],gl1_hw_asdiv_mtpl_store[gas_id].UL_delta );
			  if(((gl1_trm_asdiv_traffic_var->old_rxlev_average - Rxlev_sub )>= threshold))
              {
                MSG_GERAN_HIGH_0_G( "switch back antenna and wait for 2*N + 2 periods");
                antenna_switch_required_at_start_of_next_frame[gas_id] = TRUE;
                gl1_trm_asdiv_traffic_var->wait_counter = 2*gsm_qsc_hysteriesis_time_l1[gas_id];
                l1_asdiv_ded_clear_meas(gas_id);
              }
              else
              {
                  if (gl1_trm_asdiv_traffic_var->new_rxlev_backup >= 0)
                  {
					if(type2_alg_antenna_to_use[gas_id] == 1 )
                    {
                      threshold = gsm_qsc_small_delta[gas_id] - gl1_hw_asdiv_mtpl_store[gas_id].UL_delta;
                    }
			        else
			        {
			          threshold = gsm_qsc_small_delta[gas_id] + gl1_hw_asdiv_mtpl_store[gas_id].UL_delta;
			        }
					MSG_GERAN_HIGH_3_G("threshold:%d, small_delta:%d, UL_delta=%d",threshold,gsm_qsc_small_delta[gas_id],gl1_hw_asdiv_mtpl_store[gas_id].UL_delta );
                    if (((gl1_trm_asdiv_traffic_var->old_rxlev_average - ((Rxlev_sub + gl1_trm_asdiv_traffic_var->new_rxlev_backup)/2 ))>= threshold))
                    {
                        MSG_GERAN_HIGH_1_G("switch back antenna and wait for N + 2 periods, small hysteresis = %d",gsm_qsc_small_delta[gas_id]);
                        antenna_switch_required_at_start_of_next_frame[gas_id] = TRUE;
                        gl1_trm_asdiv_traffic_var->wait_counter= gsm_qsc_hysteriesis_time_l1[gas_id];
                                l1_asdiv_ded_clear_meas(gas_id);
                    }
                    else
                    {
                        MSG_GERAN_HIGH_1_G("no switch  wait for 2*N + 2 periods, small hysteresis = %d",gsm_qsc_small_delta[gas_id]);
                        gl1_trm_asdiv_traffic_var->wait_counter = 2*gsm_qsc_hysteriesis_time_l1[gas_id];
                                l1_asdiv_ded_clear_meas(gas_id);
#ifdef FEATURE_GERAN_DRDSDS
                                                gl1_trm_asdiv_set_traffic_crisis_flag(gas_id, FALSE);
#endif                                              
                    }

                  }

            if(gl1_trm_asdiv_traffic_var->type2_algorithm_start != 0)
            {
              gl1_trm_asdiv_traffic_var->new_rxlev_backup = Rxlev_sub ;
            }
          }

          }

        /*PRE SWITCH this is the entry point into the algorithm when UE enters dedicated mode for the first time*/
          else
          {
          gl1_trm_asdiv_traffic_var->rxlev_average = (Rxlev_sub + gl1_trm_asdiv_traffic_var->previous_rxlev_sub)/2 ;
          MSG_GERAN_HIGH_2_G("rxlev_average =%d Rxlev_sub =%d  ",gl1_trm_asdiv_traffic_var->rxlev_average,Rxlev_sub);
          if (gl1_trm_asdiv_traffic_var->rxlev_average >= trafic_mdm_threshold[gas_id] )
          {
            MSG_GERAN_HIGH_0_G(" set MDM_switch_permission true ");
            gl1_trm_ant_switch_set_control( TRM_ANT_SWITCH_CTRL_RELEASE,gas_id );
          }
          else if(gl1_trm_asdiv_traffic_var->rxlev_average< gsm_trafi_sens[gas_id] )
          {
            MSG_GERAN_HIGH_0_G(" set MDM_switch_permission false ");
            gl1_trm_ant_switch_set_control( TRM_ANT_SWITCH_CTRL_REQUEST ,gas_id);
          }

          if(type2_alg_antenna_to_use[gas_id] == 1)
          {
            threshold = gsm_trafi_sens[gas_id] - gl1_hw_asdiv_mtpl_store[gas_id].UL_delta;
            if((gl1_trm_asdiv_traffic_var->rxlev_average < threshold)&&
			   ((gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_avg_over_sacch < gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2)||
                (gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_1 <= gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2))
			  )
            {
              if (qsc_got_switch_control[gas_id])
              {
                MSG_GERAN_HIGH_0_G("qsc sends switch request to trm");
                antenna_switch_required_at_start_of_next_frame [gas_id]= TRUE;
              }
              gl1_trm_asdiv_traffic_var->type2_algorithm_start = 1 ;
              gl1_trm_asdiv_traffic_var->old_rxlev_average = gl1_trm_asdiv_traffic_var->rxlev_average;
            }
          }
		  if(type2_alg_antenna_to_use[gas_id] == 2)
          {
            threshold = gsm_trafi_sens[gas_id] + gl1_hw_asdiv_mtpl_store[gas_id].UL_delta;
            if((gl1_trm_asdiv_traffic_var->rxlev_average < threshold)||
			   ((gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_avg_over_sacch > (gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2 -20))&&
                (gl1_trm_asdiv_traffic_var->rxlev_average < (threshold + gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_1 - gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2)))
			  )
            {
              if (qsc_got_switch_control[gas_id])
              {
                MSG_GERAN_HIGH_0_G("qsc sends switch request to trm");
                antenna_switch_required_at_start_of_next_frame [gas_id]= TRUE;
              }
              gl1_trm_asdiv_traffic_var->type2_algorithm_start = 1 ;
              gl1_trm_asdiv_traffic_var->old_rxlev_average = gl1_trm_asdiv_traffic_var->rxlev_average;
            }
          }
          }
        }
      }

    /*WAIT PERIOD: Don't use measurements to determine switch, since we wait for RSSI to settle*/
      else
      {
        gl1_trm_asdiv_traffic_var->wait_counter--;
      }
	  
      gl1_trm_asdiv_traffic_var->previous_rxlev_sub = Rxlev_sub;

  }
}
#endif




/*===========================================================================

FUNCTION  L1_SEND_MPH_SERVING_DED_MEAS_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  Rxlev full
  Rxlev sub
  Rxqual full
  Rxqual sub
  DTX used

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_SERVING_DED_MEAS_IND(byte    Rxlev_full,
                                      byte    Rxlev_sub,
                                      byte    Rxlev_val,
                                      byte    Mean_BEP,
                                      byte    CV_BEP,
                                      byte    Num_Rcvd_Blocks,
                                      byte    Rxqual_full,
                                      byte    Rxqual_sub,
                                      boolean dtx_used,
                                      byte    PRxlev_sub,
                                      gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_SERVING_DED_MEAS_IND;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_SERVING_DED_MEAS_IND");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   /* Locals */
   mph_serving_ded_meas_ind_T   message_buffer,*message;
   int                          message_length;

   /* Align the pointers */

   message = &message_buffer;

      /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_SERVING_DED_MEAS_IND;  /*lint !e641 */
   message_length = sizeof(mph_serving_ded_meas_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)
/* fill the data part of the message */
   message->RXLEV_FULL_average = Rxlev_full;
   message->RXLEV_SUB_average = Rxlev_sub;
   message->RXLEV_VAL = Rxlev_val;
   message->MEAN_BEP = Mean_BEP;
   message->CV_BEP = CV_BEP;
   /*
    * CR150983 - This is a temp workaround until
    * CR152211 has been completed
    */
   MSG_HIGH ( "L1_send_MPH_SERVING_DED_MEAS_IND ",0,0,0);

   if (Num_Rcvd_Blocks == L1_EMR_MAX_BLKS_PER_MEAS_PERIOD)
   {
     /* We are only interested in incrementing the cnt
      * for the error case where we have all TCH blocks in
      * a FR channel during a MR period
      */
     Num_Rcvd_Blocks++;
   }
   message->NBR_RCVD_BLOCKS = Num_Rcvd_Blocks;
   message->RXQUAL_FULL_average = Rxqual_full;
   message->RXQUAL_SUB_average = Rxqual_sub;
   message->DTX_used = dtx_used;
#ifdef FEATURE_GSM_QSC_TX_DIV
   /*Use PRxlev_sub as input to AsDiv Algorithm*/
   l1_asdiv_ded_meas_qsc_alg(gas_id, (int32)PRxlev_sub);
#endif /* FEATURE_GSM_QSC_TX_DIV */

#if defined (FEATURE_GSM_SIMULATE_GL1_RF) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
   #error code not present
#endif /* FEATURE_GSM_SIMULATE_GL1_RF */

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
    if (gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE1))
    {
        if(gl1_trm_asdiv_debug_get_test_mode(gas_id))
        {
            gl1_trm_asdiv_debug_test_mode_switch_sm(gas_id, TRM_CLIENT_MODE_RXTX);
        }
        else
        {
            if ( PRxlev_sub < tx_diversity_threshold )
            {
                gl1_hw_asdiv_type1.start_alg = TRUE;
                start_tx_div_antenna_alg ( FALSE,gas_id);
            }
            else
            {
                gl1_hw_asdiv_type1.start_alg = FALSE;
                gl1_hw_clear_antenna_meas (gas_id);
                MSG_GERAN_HIGH_2_G("start asdiv type1 algorithm = %d Rxlev_sub =%d ",gl1_hw_asdiv_type1.start_alg,Rxlev_sub);
            }
        }
    }

#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */
   MSG_GERAN_MED_5_G("L1_send_MPH_SERVING_DED_MEAS_IND : Serving RXLEV_FULL=%d RXLEV_SUB=%d RXQUAL: FULL=%d SUB=%d DTX=%d",
   Rxlev_full, Rxlev_sub, Rxqual_full, Rxqual_sub, dtx_used);

   message->trk_lo_adj = gl1_hw_get_vctcxo_pdm_val( gas_id );
   MSG_GERAN_MED_5_G("Serving RXLEV_VAL=%d Num_Blks=%d Current trk_lo_adj=%d Mean_BEP=%d CV_BEP=%d", Rxlev_val,Num_Rcvd_Blocks, message->trk_lo_adj,Mean_BEP,CV_BEP);


   
   message->gas_id = gas_id;

   /* Send the message */
   (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#endif
}


/*===========================================================================

FUNCTION  L1ISR_SEND_MPH_CHANNEL_ASSIGN_CONF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error_code value

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1isr_send_MPH_CHANNEL_ASSIGN_CONF(byte    error_code, gas_id_t gas_id)
{
   /* Locals */
   mph_channel_assign_cnf_T   message_buffer,*message;
   int                           message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_CHANNEL_ASSIGN_CNF;  /*lint !e641 */
   message_length = sizeof(mph_channel_assign_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* fill the data part of the message */
   message->error_code = error_code;
   message->gas_id = gas_id;

      /* Send the message */

   (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
}


/*===========================================================================

FUNCTION  L1ISR_SEND_PH_CONNECT_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_L2
  queue.

DEPENDENCIES
  L2 Channel type (SACCH or DCCH)

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1isr_send_PH_CONNECT_IND(channel_type_T chnl_type, gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_PH_CONNECT_IND;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->ph_connect_ind.param.connected_channel = chnl_type;

      MSG_GERAN_HIGH_1_G( "GPLT DBG: Sending GPLT_PH_CONNECT_IND chnl_type %d ", chnl_type);
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   ph_connect_ind_T              message_buffer,*message;
   int                           message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_L2_L1;
   message->message_header.message_id = PH_CONNECT_IND;
   message_length = sizeof(ph_connect_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

          /* Set up the data in the message body */

   message->connected_channel = chnl_type;
   message->gas_id = gas_id;

      /* Send the message */

   (void)L1_isr_send_message(GS_QUEUE_LAYER_2,message,TRUE, gas_id);
#endif /* FEATURE_GL1_GPLT */
}




/*
 * Function name :  L1isr_send_MPH_RECONNECT_CHANNEL_CNF
 * -------------
 *
 * Description :  Creates the above named message
 * -----------    message and sends to the GS_QUEUE_RR
 *                queue.
 *
 *
 * Uses :
 * ----
 *
 *
 * Parameters:  none
 * ----------
 *
 *
 *
 * Returns:
 * --------
 *
 */

/*===========================================================================

FUNCTION  L1ISR_SEND_MPH_RECONNECT_CHANNEL_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1isr_send_MPH_RECONNECT_CHANNEL_CNF( gas_id_t gas_id )
{
   /* Locals */
   mph_reconnect_channel_cnf_T   message_buffer,*message;
   int                           message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */
   message->gas_id = gas_id;

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_RECONNECT_CHANNEL_CNF;   /*lint !e641 */
   message_length = sizeof(mph_reconnect_channel_cnf_T) - sizeof(IMH_T); /*lint !e778 expression = 0 */
   PUT_IMH_LEN(message_length,message)

   /* Send the message */

   (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
}
/*===========================================================================

FUNCTION  L1_SEND_ISR_SCAN_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_L1
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_ISR_SCAN_CNF ( gas_id_t gas_id )
{
   /* Locals */
   isrtim_scan_cnf_T   message_buffer,*message;
   int                           message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_L1_L1;
   message->message_header.message_id = ISR_SCAN_CNF;
   message->gas_id = gas_id;
   message_length = sizeof(isrtim_scan_cnf_T) - sizeof(IMH_T); /*lint !e778 expression = 0 */
   PUT_IMH_LEN(message_length,message)

   /* Send the message */

   (void)L1_isr_send_message(l1_queues[gas_id],message,TRUE, gas_id);
}


/*===========================================================================

FUNCTION  L1_SEND_ISR_HANDOVER_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_L1
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_ISR_HANDOVER_CNF ( gas_id_t gas_id )

{
   /* Locals */
   isrtim_handover_cnf_T   message_buffer,*message;
   int                           message_length;


   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_L1_L1;
   message->message_header.message_id = ISR_HANDOVER_CNF;
   message_length = sizeof(isrtim_handover_cnf_T) - sizeof(IMH_T); /*lint !e778 expression = 0 */
   PUT_IMH_LEN(message_length,message)

   message->gas_id = gas_id;
   /* Send the message */

   (void)L1_isr_send_message(l1_queues[gas_id], message,TRUE, gas_id);
}

/*===========================================================================

FUNCTION  L1_SEND_ISR_FIRST_FCB_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_L1
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_ISR_FIRST_FCB_CNF (boolean  success, gas_id_t gas_id)

{
   /* Locals */
   isrtim_first_fcb_cnf_T   message_buffer,*message;
   int                           message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_L1_L1;
   message->message_header.message_id = ISR_FIRST_FCB_CNF;
   message_length = sizeof(isrtim_first_fcb_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* fill the data part of the message */
   message->success = success;
   message->gas_id = gas_id;
   /* Send the message */

   (void)L1_isr_send_message(l1_queues[gas_id], message,TRUE, gas_id);
}


/*===========================================================================

FUNCTION  L1_SEND_ISR_SCH_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_L1
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_ISR_SCH_CNF(boolean  success, byte *sch_dta_ptr, gas_id_t gas_id)
{
   /* Locals */
   l1_sch_cnf_T   message_buffer,*message;
   int                message_length,i;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */
   message->gas_id = gas_id;
   message->message_header.message_set = MS_L1_L1;
   message->message_header.message_id = ISR_SCH_CNF;
   message_length = sizeof(l1_sch_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* fill the data part of the message */
   message->success = success;
   if (success == TRUE)
   {
      for (i = 0; i< SCH_BLOCK_SIZE ; i++)
      {
          message->sch_block[ i ] = *(sch_dta_ptr + i );
      }
   }
   message->gas_id = gas_id;
   /* Send the message */

   (void)L1_isr_send_message(l1_queues[gas_id], message,TRUE, gas_id);
}



/*===========================================================================

FUNCTION  L1_send_ISR_DEACT_CNF

DESCRIPTION
  Sends indication to TASK, so that task can do final cleanup

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_ISR_DEACT_CNF ( boolean deep_sleep, gas_id_t gas_id )
{
   /* Locals */
   isrtim_deact_cnf_T   message_buffer,*message;
   int                           message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_L1_L1;
   message->message_header.message_id = ISR_DEACT_CNF;
   message->deep_sleep                = deep_sleep;
   message_length = sizeof(isrtim_deact_cnf_T) - sizeof(IMH_T); /*lint !e778 expression = 0 */
   PUT_IMH_LEN(message_length,message)
   message->gas_id = gas_id;
   /* Send the message */

   (void)L1_isr_send_message(l1_queues[gas_id], message,TRUE, gas_id);
}

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
/*===========================================================================

FUNCTION  L1_send_ISR_SUSPEND_CNF

DESCRIPTION
  Sends indication to TASK, so that task can do final cleanup
  This is called when suspending L1

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_ISR_SUSPEND_CNF ( uint32 timer_value, gas_id_t gas_id )
{
   /* Locals */
   isrtim_suspend_cnf_T   message_buffer,*message;
   int                           message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_L1_L1;
   message->message_header.message_id  = ISR_SUSPEND_CNF;
   message->timer_value                = timer_value;
   message_length = sizeof(isrtim_suspend_cnf_T) - sizeof(IMH_T); /*lint !e778 expression = 0 */
   PUT_IMH_LEN(message_length,message)

   message->gas_id = gas_id;

   /* Send the message */

   (void)L1_isr_send_message(l1_queues[gas_id], message,TRUE, gas_id);
}
#endif

/*===========================================================================

FUNCTION  L1ISR_SEND_PH_READY_TO_SEND_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_L2
  queue.

DEPENDENCIES
  Channel type (SACCH or DCCH)

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1isr_send_PH_READY_TO_SEND_IND( byte channel_type, gas_id_t gas_id )
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_PH_READY_TO_SEND_IND;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->ph_ready_to_send_ind.param.l2_channel_type = channel_type;

      MSG_GERAN_HIGH_1_G( "GPLT DBG: Sending GPLT_PH_READY_TO_SEND_IND chnl_type %d ", channel_type);
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   /* Locals */
   ph_ready_to_send_ind_T   message_buffer,*message;
   word                message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_L2_L1;
   message->message_header.message_id = PH_READY_TO_SEND_IND;
   message_length = sizeof(ph_ready_to_send_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* fill the data part of the message */
   message->l2_channel_type = channel_type;
   message->gas_id =  gas_id;

   /* Send the message */

   (void)L1_isr_send_message(GS_QUEUE_LAYER_2,message,TRUE, gas_id);
#endif
}


/*===========================================================================

FUNCTION  L1ISR_SEND_MPH_IMMED_ASSIGN_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1isr_send_MPH_IMMED_ASSIGN_CNF(byte error, gas_id_t gas_id)
{
   /* Locals */
   mph_immed_assign_cnf_T   message_buffer,*message;
   word                message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_IMMED_ASSIGN_CNF; /*lint !e641 */
   message_length = sizeof(mph_immed_assign_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* fill the data part of the message */
   message->error_code = error;
   message->gas_id = gas_id;
   /* Send the message */

   (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
}


/*===========================================================================

FUNCTION  L1_SEND_MPH_IMMED_ASSIGN_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_IMMED_ASSIGN_CNF (byte error, gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_IMMED_ASSIGN_CNF;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->immed_assign_cnf.param.error_code = error;


      MSG_GERAN_HIGH_1_G( "GPLT DBG: Sending GPLT_MPH_IMMED_ASSIGN_CNF error %d", error);
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   /* Locals */
   mph_immed_assign_cnf_T   message_buffer,*message;
   word                message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_IMMED_ASSIGN_CNF;  /*lint !e641 */
   message_length = sizeof(mph_immed_assign_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* fill the data part of the message */
   message->error_code = error;
   message->gas_id = gas_id;
   /* Send the message */

   (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#endif /* FEATURE_GL1_GPLT */
}


/*===========================================================================

FUNCTION  L1_send_MPH_UE_MODE_CHANGE_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_UE_MODE_CHANGE_CNF (gas_id_t gas_id)
{
#ifdef FEATURE_SGLTE

   mph_ue_mode_change_cnf_T mph_ue_mode_change_cnf;

   mph_ue_mode_change_cnf.message_header.message_set = MS_RR_L1;
   mph_ue_mode_change_cnf.message_header.message_id  = MPH_UE_MODE_CHANGE_CNF;

   mph_ue_mode_change_cnf.gas_id = gas_id;

   PUT_IMH_LEN(sizeof(mph_ue_mode_change_cnf) - sizeof(IMH_T), &mph_ue_mode_change_cnf.message_header);

   L1_isr_send_message(GS_QUEUE_RR, &mph_ue_mode_change_cnf.message_header, TRUE,gas_id);

#endif /* FEATURE_SGLTE */

   return;
}


/*===========================================================================

FUNCTION  L1ISR_SEND_MPH_CHANNEL_RELEASE_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1isr_send_MPH_CHANNEL_RELEASE_CNF (byte error, gas_id_t gas_id)
{
   /* Locals */
   mph_channel_release_cnf_T   message_buffer,*message;
   word                message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_CHANNEL_RELEASE_CNF;  /*lint !e641 */
   message_length = sizeof(mph_channel_release_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* fill the data part of the message */
   message->error_code = error;
   message->gas_id = gas_id;
   /* Send the message */

   (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
}

#ifdef FEATURE_GSM_DTM
/*===========================================================================

FUNCTION  L1isr_send_MPH_DTM_CHANNEL_RELEASE_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1isr_send_MPH_DTM_CHANNEL_RELEASE_IND (byte error, boolean ps_release, boolean cs_release, gas_id_t gas_id)
{
   /* Locals */
   mph_dtm_channel_release_ind_t    message_buffer,*message;
   word                             message_length;

   /* Align the pointers */

   message = &message_buffer;

   /* Set up the message header */

   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_DTM_CHANNEL_RELEASE_IND;  /*lint !e641 */
   message_length = (sizeof(mph_dtm_channel_release_ind_t) - sizeof(IMH_T));
   PUT_IMH_LEN(message_length,message)

   /* fill the data part of the message */
   message->error_code = error;
   message->ps_release = ps_release;
   message->cs_release = cs_release;
   message->gas_id = gas_id;
   /* Send the message */

   (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
}

/*===========================================================================

FUNCTION  L1isr_send_MPH_DTM_CHANNEL_ASSIGN_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1isr_send_MPH_DTM_CHANNEL_ASSIGN_CNF (gas_id_t gas_id)
{
   /* Locals */
   mph_dtm_channel_assign_cnf_t    message_buffer,*message;
   word                             message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* Set up the message header */
   message->gas_id = gas_id;
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_DTM_CHANNEL_ASSIGN_CNF;  /*lint !e641 */
   message_length = (sizeof(mph_dtm_channel_assign_cnf_t) - sizeof(IMH_T));
   PUT_IMH_LEN(message_length,message)

   /* Send the message */
   (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
}



#endif /*FEATURE_GSM_DTM*/

/*===========================================================================

FUNCTION  L1ISR_SEND_MPH_SET_CIPHERING_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1isr_send_MPH_SET_CIPHERING_CNF (byte error, gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MPH_SET_CIPHERING_CNF;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */
      gplt_cmd_ptr->set_ciphering_cnf.param.error_code = error;
    gplt_cmd_ptr->set_ciphering_cnf.param.gas_id = gas_id;

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MPH_SET_CIPHERING_CNF");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
   /* Locals */
   mph_set_ciphering_cnf_T   message_buffer,*message;
   word                message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* Set up the message header */
   message->message_header.message_set = MS_RR_L1;
   message->message_header.message_id = MPH_SET_CIPHERING_CNF;  /*lint !e641 */
   message_length = sizeof(mph_set_ciphering_cnf_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* fill the data part of the message */
   message->error_code = error;
   message->gas_id = gas_id;
   /* Send the message */
   (void)L1_isr_send_message(GS_QUEUE_RR,message,TRUE,gas_id);
#endif /*FEATURE_GL1_GPLT*/
}

/*===========================================================================

FUNCTION  L1ISR_SEND_L1_REP_PERIOD_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_L1
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void   L1isr_send_L1_REP_PERIOD_IND( gas_id_t gas_id )
{
   /* Locals */
   l1_rep_period_ind_T   message_buffer,*message;
   word                message_length;

   /* Align the pointers */
   message = &message_buffer;

   /* Set up the message header */
   message->message_header.message_set = MS_L1_L1;
   message->message_header.message_id = L1_REP_PERIOD_IND;
   message_length = sizeof(l1_rep_period_ind_T) - sizeof(IMH_T); /*lint !e778 expression = 0 */
   PUT_IMH_LEN(message_length,message)
   message->gas_id = gas_id;
   /* Send the message */
   (void)L1_isr_send_message(l1_queues[gas_id], message,TRUE, gas_id);
}

/*===========================================================================

FUNCTION  L1_send_MPH_READ_SYS_INFO_FAILURE_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void L1_send_MPH_READ_SYS_INFO_FAILURE_IND(boolean state_swap, boolean transfer, gas_id_t gas_id )
{
   mph_read_sys_info_failure_ind_T msg;

   msg.message_header.message_set = MS_RR_L1;
   msg.message_header.message_id  = MPH_READ_SYS_INFO_FAILURE_IND; /*lint !e641 */

   /* fill the data part of the message */
   msg.in_transfer   = transfer;
   msg.state_changed = state_swap;
   msg.gas_id = gas_id;
   PUT_IMH_LEN( sizeof ( msg ) - sizeof ( IMH_T ), &msg );

   (void)L1_isr_send_message( GS_QUEUE_RR, &msg, TRUE, gas_id);
}

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
/*===========================================================================

FUNCTION  L1_send_MPH_INTERRAT_LIST_SRCH_TIME_IND

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void L1_send_MPH_INTERRAT_LIST_SRCH_TIME_IND(uint32 timer_value, gas_id_t gas_id)
{
   mph_interrat_list_srch_time_ind_T msg;

   msg.message_header.message_set = MS_RR_L1;
   msg.message_header.message_id  = MPH_INTERRAT_LIST_SRCH_TIME_IND; /*lint !e641 */

   /* fill the data part of the message */
   msg.timer_value   = timer_value;
   msg.gas_id = gas_id;
   PUT_IMH_LEN( sizeof ( msg ) - sizeof ( IMH_T ), &msg );

   (void)L1_isr_send_message( GS_QUEUE_RR, &msg, TRUE, gas_id);
}

/*===========================================================================

FUNCTION  L1_send_MPH_SUSPEND_GSM_MODE_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void L1_send_MPH_SUSPEND_GSM_MODE_CNF(uint32 timer_value, boolean suspended, gas_id_t gas_id)
{
   mph_suspend_gsm_mode_cnf_T msg;

   msg.message_header.message_set = MS_RR_L1;
   msg.message_header.message_id  = MPH_SUSPEND_GSM_MODE_CNF; /*lint !e641 */

   /* fill the data part of the message */
   msg.timer_value   = timer_value;
   msg.suspended     = suspended;
   msg.gas_id = gas_id;
   PUT_IMH_LEN( sizeof ( msg ) - sizeof ( IMH_T ), &msg );

   (void)L1_isr_send_message( GS_QUEUE_RR, &msg, TRUE, gas_id);
}

/*===========================================================================

FUNCTION  L1_send_MPH_RESUME_GSM_MODE_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_RR
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_MPH_RESUME_GSM_MODE_CNF(gas_id_t gas_id)
{
   mph_resume_gsm_mode_cnf_T msg;
   int message_length;

   msg.message_header.message_set = MS_RR_L1;
   msg.message_header.message_id  = MPH_RESUME_GSM_MODE_CNF; /*lint !e641 */
   msg.gas_id = gas_id;
   message_length = sizeof(mph_resume_gsm_mode_cnf_T) - sizeof(IMH_T);  /*lint !e778 const expression = 0 */
   PUT_IMH_LEN(message_length,&msg);

   (void)L1_isr_send_message( GS_QUEUE_RR, &msg, TRUE, gas_id);
}
#endif

/*===========================================================================

FUNCTION  L1_send_L1_ISR_SINGLE_BLK_FAILURE_CNF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_L1
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_L1_ISR_SINGLE_BLK_FAILURE_CNF( gas_id_t gas_id )
{
  /* Message buffer and the status of sending the message */
  t_l1_isr_single_blk_failure_cnf  message_buffer;
  gs_status_T                      status;

  /* Set up the message header */
  message_buffer.header.message_set = MS_L1_L1;
  message_buffer.header.message_id  = L1_ISR_SINGLE_BLK_FAILURE_CNF;
  message_buffer.gas_id = gas_id;

  /* Set the length */
  PUT_IMH_LEN( sizeof( message_buffer ) - sizeof( message_buffer.header ), &message_buffer );

  /* Send the message to the L1 task */
  status = L1_isr_send_message( l1_queues[gas_id],
                                &message_buffer,
                                TRUE, gas_id
                              );

  /* Was the message sent correctly ? */
  if( GS_SUCCESS != status )
  {
    /* This is not good, print an error message */
    MSG_GERAN_ERROR_1_G(" isr msg send status %d ", status);
  }

}

/* $X2G_CM_INT_VIA_MSG : Send response to other tech here */
void L1_send_X2G_CM_GSM_INIT_RSP( gas_id_t gas_id , rfcom_mode_enum_type  serving_rat )
{
  errno_enum_type  msgr_error;
  x2g_irat_cm_gsm_init_rsp_type   x2g_irat_cm_gsm_init_rsp;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  msgr_init_hdr_variant(&x2g_irat_cm_gsm_init_rsp.hdr,
                        MSGR_GERAN_GL1,
                        GERAN_GL1_X2G_IRAT_CM_GSM_INIT_RSP,
                        (uint8)as_id);

  x2g_irat_cm_gsm_init_rsp.status = TRUE ;
  x2g_irat_cm_gsm_init_rsp.serving_rat = serving_rat;

  msgr_error = msgr_send( (msgr_hdr_struct_type *) &x2g_irat_cm_gsm_init_rsp , sizeof(x2g_irat_cm_gsm_init_rsp_type) );
  ASSERT(msgr_error == E_SUCCESS);
  MSG_GERAN_HIGH_1_G("GERAN_GL1_X2G_IRAT_CM_GSM_INIT_RSP : %d  sent", serving_rat );

}

#ifdef FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY
void L1_send_PH_CS_ABORT_IND(geran_abort_cause_t abort_cause, l2_channel_type_T channel_type, gas_id_t gas_id)
{
   ph_cs_abort_ind_T message_buffer,*message;
   int               message_length;

   /* Set up pointer to local message buffer */
   message = &message_buffer;

   /* Initialise the buffer */
   memset(message,0,sizeof(ph_cs_abort_ind_T));

   /* Set up the message header */
   message->message_header.message_set = MS_L2_L1;
   message->message_header.message_id  = (byte)PH_CS_ABORT_IND;

   message_length = sizeof(ph_cs_abort_ind_T) - sizeof(IMH_T);
   PUT_IMH_LEN(message_length,message)

   /* Complete the payload */
   message->l1_abort_cause  = abort_cause;
   message->l2_channel_type = channel_type;   
   message->gas_id          = gas_id;

   MSG_GERAN_ERROR_0_G("RF timeline error recovery: call drop"); 

   gl1_hw_rf_recovery_cleanup(TRUE,gas_id);

   /* Send the message */
   (void)L1_send_message(GS_QUEUE_LAYER_2, message, TRUE, gas_id);

}
#endif /* FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY */

#ifdef  FEATURE_GSM_TO_LTE

static uint16     G2L_INIT_sent[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0) };
static boolean    g2l_abort_cnf_pending[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };

/*===========================================================================

FUNCTION L1_send_LTE_CPHY_IRAT_MEAS_G2L_INIT_REQ

DESCRIPTION
    Starts up ML1 in preparation for G2L measurements.
    Sent only once per session
    
DEPENDENCIES
  none

RETURN VALUE
   
   ML1 responds with cnf message

SIDE EFFECTS
  None

===========================================================================*/

void  L1_send_LTE_CPHY_IRAT_MEAS_G2L_INIT_REQ( boolean  CM_mode, gas_id_t gas_id )
{
  lte_cphy_irat_meas_init_req_s  init_req_msg;
  errno_enum_type                msgr_error;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  /*Increment active task count so no sleep until deinit complete */
  gl1_drx_task_active(gas_id);

  if(G2L_INIT_sent[gas_id] & LTE_G2L_INIT_SENT)
  {
    MSG_GERAN_ERROR_1_G("G2L:LTE_CPHY_IRAT_MEAS_G2L_INIT_REQ already sent init=0x%x" , G2L_INIT_sent[gas_id]);

    /*decrement active task count*/
    gl1_drx_task_inactive(gas_id);

    return;
  }

  /* initialise msgr header */
  msgr_init_hdr_variant(&init_req_msg.hdr,
                        MSGR_GERAN_GL1,
                        LTE_CPHY_IRAT_MEAS_G2L_INIT_REQ,
                        (uint8)as_id);

  init_req_msg.rat              = LTE_CPHY_IMEAS_RAT_GSM;
  init_req_msg.maintain_timing  = TRUE;                 /* always true for timed search mode */
  init_req_msg.snapshot_present = FALSE;

  msgr_error = msgr_send( (msgr_hdr_struct_type *) &init_req_msg,
                          sizeof(lte_cphy_irat_meas_init_req_s)
                        );
  ASSERT(msgr_error == E_SUCCESS);

  gl1_irat_mutex_lock( gas_id);
  
  G2L_INIT_sent[gas_id] = LTE_G2L_INIT_SENT;

  gl1_irat_mutex_unlock( gas_id);

  MSG_GERAN_HIGH_1_G("G2L:LTE_CPHY_IRAT_MEAS_G2L_INIT_REQ init=0x%x" , G2L_INIT_sent[gas_id]);


} /* L1_send_LTE_CPHY_IRAT_MEAS_G2L_INIT_REQ */


/*===========================================================================

FUNCTION L1_send_LTE_CPHY_IRAT_MEAS_G2L_DEINIT_REQ

DESCRIPTION

    Stops ML1 for current G2L measurement session
    Must be paired with previous L1_send_LTE_CPHY_IRAT_MEAS_G2L_INIT_REQ
    Can only be sent once to end session.
   
DEPENDENCIES
  none

RETURN VALUE
   
   ML1 responds with cnf message

SIDE EFFECTS
  None

===========================================================================*/

void  L1_send_LTE_CPHY_IRAT_MEAS_G2L_DEINIT_REQ( gas_id_t gas_id )
{

  if( !(G2L_INIT_sent[gas_id] & LTE_G2L_INIT_SENT))
  {
    MSG_GERAN_ERROR_1_G("G2L: not sending LTE_CPHY_IRAT_MEAS_G2L_DEINIT_REQ  init=0x%x" , G2L_INIT_sent[gas_id]);
    return;
  }

  {
    lte_cphy_irat_meas_init_req_s  deinit_req_msg;
    errno_enum_type                msgr_error;
    sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

    /*Increment active task count so no sleep until deinit complete */
    gl1_drx_task_active(gas_id);

    gl1_irat_mutex_lock( gas_id);

    /* initialise msgr header */
    msgr_init_hdr_variant(&deinit_req_msg.hdr,
                          MSGR_GERAN_GL1,
                          LTE_CPHY_IRAT_MEAS_G2L_DEINIT_REQ,
                          (uint8)as_id);

    msgr_error = msgr_send( (msgr_hdr_struct_type *) &deinit_req_msg,
                            sizeof(lte_cphy_irat_meas_init_req_s)
                          );

    ASSERT(msgr_error == E_SUCCESS);

    MSG_GERAN_MED_0_G("G2L:LTE_CPHY_IRAT_MEAS_G2L_DEINIT_REQ ");

    G2L_INIT_sent[gas_id] = LTE_G2L_DEINIT_SENT;

    gl1_irat_mutex_unlock( gas_id);

  }
}  /* L1_send_LTE_CPHY_IRAT_MEAS_G2L_DEINIT_REQ */

/*===========================================================================

FUNCTION L1_send_LTE_CPHY_IRAT_MEAS_BUILD_RF_SCRIPT_REQ

DESCRIPTION
  Function build G2L script for measurement, used for G2L IRAT in QTA only.

DEPENDENCIES
  none

RETURN VALUE
  boolean     TRUE if message was sent

SIDE EFFECTS
  None

===========================================================================*/

void L1_send_LTE_CPHY_IRAT_MEAS_BUILD_RF_SCRIPT_REQ( gas_id_t gas_id )
{
  lte_cphy_irat_meas_build_rf_script_req_s build_rf_script_req;

  errno_enum_type msgr_error;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  /* only if we cannot guarantee all fields are not populated ML1 RECOMMENDS WE DO THIS  */
  memset( &build_rf_script_req, 0, sizeof(lte_cphy_irat_meas_build_rf_script_req_s));

  // Variant
  /* initialise msg header */
  msgr_init_hdr_variant(&build_rf_script_req.hdr,MSGR_GERAN_GL1,LTE_CPHY_IRAT_MEAS_G2L_BUILD_RF_SCRIPT_REQ, (uint8)as_id);

  g2l_irat_populate_rf_param (&build_rf_script_req.rf_param, gas_id);

  msgr_error = msgr_send((msgr_hdr_struct_type *)&build_rf_script_req,
                         sizeof(lte_cphy_irat_meas_build_rf_script_req_s));

  ASSERT(msgr_error == E_SUCCESS);
}


/*===========================================================================

FUNCTION L1_send_LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_REQ

DESCRIPTION
  Function Timed gap measurement for G2L, both idle and dedicated modes

DEPENDENCIES
  none

RETURN VALUE
  boolean     TRUE if message was sent

SIDE EFFECTS
  None

===========================================================================*/

boolean L1_send_LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_REQ( uint16 freq_index, boolean ml1_gap_init, lte_earfcn_t nextFreq, gas_id_t gas_id, boolean gap_start_asap)
{

  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  /*
  ** new api for G2L ML1 maintains ml1_gap_state and decides each action per gap and on cnf also
  ** with some helper functions, det/meas are pipelined across 2 gaps.
  */
  static  lte_ml1_irat_gap_scheduled_info_s         gap_sched_info[NUM_GERAN_DATA_SPACES];
  static  lte_cphy_irat_meas_timed_srch_meas_req_s  srch_meas_req[NUM_GERAN_DATA_SPACES];

  errno_enum_type                                   msgr_error;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  #ifdef FEATURE_G2L_TIME_TRANSFER
  lte_ml1_irat_gap_schedule_hint_e schedule_hint = LTE_ML1_IRAT_GAP_SCHEDULE_HINT_AUTO;
  #else
  lte_ml1_irat_gap_schedule_hint_e schedule_hint = LTE_ML1_IRAT_GAP_SCHEDULE_HINT_SEARCH_AND_MEAS;
  #endif

  /* Initialize the g2l_abort flag to FALSE here */
  l1_sc_wcdma_data_ptr->g2x_aborted          = FALSE;

  /*
  ** Can now add state check instead
  */
  if((G2L_INIT_sent[gas_id] & LTE_G2L_MASK) != LTE_G2L_READY)
  {
     MSG_GERAN_ERROR_1_G("G2L: not sending LTE_CPHY_IRAT_MEAS_TIMED_SRCH_MEAS_REQ init=0x%x abort=%d " ,G2L_INIT_sent[gas_id] );

     return FALSE;
  }

  if(ml1_gap_init)
  {
      /*
      ** reset the LTE state machine for CM gap
      ** only needs to be done once per session or after an abort
      */
      lte_ml1_irat_gap_init(geran_map_gas_id_to_nas_id(gas_id), &ml1_gap_state[gas_id]);
  }

  /* only if we cannot guarantee all fields are not populated ML1 RECOMMENDS WE DO THIS  */
  memset( &srch_meas_req[gas_id], 0, sizeof(lte_cphy_irat_meas_timed_srch_meas_req_s));

  /* initialise msg header */
  msgr_init_hdr_variant(&srch_meas_req[gas_id].hdr,
                        MSGR_GERAN_GL1,
                        LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_REQ,
                        (uint8)as_id);

  /*
  ** populate the msg detail for the next measurement gap
  */
  /* Valid for idle mode. renamed from l1_sc_cm_populate_g2l*/
  l1_sc_populate_g2l( freq_index, &srch_meas_req[gas_id], &schedule_hint, gap_start_asap, gas_id);

  /*
  ** ML1 checks the msg and decides what the next gap will do
  ** det/meas are pipelined with previous gap.
  */
  if(LTE_ML1_IRAT_GAP_SUCCESS==lte_ml1_irat_setup_schedule( geran_map_gas_id_to_nas_id(gas_id),
                                                            schedule_hint,
                                                            &srch_meas_req[gas_id],
                                                            &ml1_gap_state[gas_id],
                                                            &gap_sched_info[gas_id],
                                                            nextFreq))
  {
    /* If freq has been changed, it needs to register new IRAT band. */
    if(!gap_start_asap && (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_index].arfcn.euarfcn != srch_meas_req[gas_id].freq))
    {
      sys_sband_lte_e_type lte_band;
      if( lte_ml1_common_band_get_band_from_dl_earfcn( srch_meas_req[gas_id].freq, &lte_band ))
      {
        /* check and register band */
        if(!grm_register_additional_irat_bands((uint32)lte_band, MAX_LTE_SEARCH_FRAMES, gas_id))
        {
          lte_ml1_irat_gap_abort_handling(geran_map_gas_id_to_nas_id(gas_id), &ml1_gap_state[gas_id]);

          return FALSE;
        }
      }
    }
  
    /* For QTA case, G2L RF ENTER/Build script is done in QTA startup, no need to call ENTER again */
    if (!gl1_hw_qta_gap_active(gas_id))
    {
    /*
    ** Call rf meas enter before gap but after api call that updates freq when move_to_next_freq is set.
    ** ensure build scripts use the correct value.
    */
      g2l_irat_x2l_meas_enter( srch_meas_req[gas_id].freq , gas_id);
    }
    g2l_irat_populate_rf_param (&srch_meas_req[gas_id].rf_param, gas_id);
   
    msgr_error = msgr_send((msgr_hdr_struct_type *)&srch_meas_req[gas_id],
                            sizeof(lte_cphy_irat_meas_timed_srch_meas_req_s)
                          );

    ASSERT(msgr_error == E_SUCCESS);

    l1_sc_wcdma_data_ptr->rr_params.use_next_freq = gap_sched_info[gas_id].move_to_next_freq;   /* next gap switch to next freq */

    return TRUE;

  }

  return FALSE;

} /* L1_send_LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_REQ */

/*===========================================================================

FUNCTION L1_send_LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ

DESCRIPTION
  Not yet supported by ML1 for timed requests, TBA

DEPENDENCIES
  none

RETURN VALUE
   

SIDE EFFECTS
  None

===========================================================================*/

void  L1_send_LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ( gas_id_t gas_id )
{
  lte_cphy_irat_meas_abort_req_s  meas_abort_req_msg;
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  errno_enum_type  msgr_error;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  if(l1_sc_wcdma_data_ptr->lte_cm_irat_active & G2L_CM_ACTIVE)
  {
     l1_g2l_cm_abort(gas_id);

     MSG_GERAN_HIGH_0_G("G2L: CM active: NOT sending LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ as n/a in CM mode");

     return;
  }

  if( !(G2L_INIT_sent[gas_id] & LTE_G2L_INIT_SENT ))
  {
    MSG_GERAN_ERROR_1_G("G2L:not sending LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ  init 0x%x", G2L_INIT_sent[gas_id]);
    return;
  }

  /* initialise msgr header */
  msgr_init_hdr_variant(&meas_abort_req_msg.hdr,
                        MSGR_GERAN_GL1,
                        LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ,
                        (uint8)as_id);

  msgr_error = msgr_send( (msgr_hdr_struct_type *) &meas_abort_req_msg,
                           sizeof(lte_cphy_irat_meas_abort_req_s)
                        );

  ASSERT(msgr_error == E_SUCCESS);

  MSG_GERAN_HIGH_0_G("G2L:LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ ");

  G2L_INIT_sent[gas_id] |=LTE_G2L_CNF_PENDING;
  g2l_set_abort_cnf_pending_status(TRUE, gas_id);

} /* L1_send_LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ */


/* LTE helper functions */

lte_l1_gsm_band_e gsmband2lte(sys_band_T gsmband)
{

  lte_l1_gsm_band_e band;

  switch( gsmband )
  {
  case SYS_BAND_EGSM_900:
    band = LTE_L1_GSM_BAND_EGSM_900;
  break;

  case SYS_BAND_PGSM_900:
    band = LTE_L1_GSM_BAND_PGSM_900;
  break;

  case SYS_BAND_PCS_1900:
    band = LTE_L1_GSM_BAND_PCS1900;
  break;

  case SYS_BAND_DCS_1800:
    band = LTE_L1_GSM_BAND_DCS1800;
  break;

  case SYS_BAND_CELL_850:
    band = LTE_L1_GSM_BAND_CELL_850;
  break;

  default:
    band = LTE_L1_GSM_BAND_EGSM_900;
  break;
  }

  return band;

} /* gsmband2lte */

uint16 g2l_get_Init_Sent_status(gas_id_t gas_id)
{
  return G2L_INIT_sent[gas_id];
}

void g2l_set_Init_Sent_status( uint16 g2l_status_bit, gas_id_t gas_id)
{
  G2L_INIT_sent[gas_id] |= g2l_status_bit;
}

void g2l_clr_Init_Sent_status( uint16 g2l_status_bit, gas_id_t gas_id)
{
     G2L_INIT_sent[gas_id] &= ~g2l_status_bit;
}

boolean g2l_get_abort_cnf_pending_status(gas_id_t gas_id)
{
     return g2l_abort_cnf_pending[gas_id];
}

void g2l_set_abort_cnf_pending_status( boolean value, gas_id_t gas_id)
{
     g2l_abort_cnf_pending[gas_id] = value;
}

#endif  /* LTE  */

#ifdef FEATURE_GSM_TDS

static boolean G2TDS_INIT_sent = FALSE;

volatile boolean gl1_TDS_IRAT_INIT_CNF_pending = FALSE;

void  L1_send_TDS_IRAT_INIT_CMD (gas_id_t gas_id)
{
  tdsirat_init_cmd_type   init_cmd_msg;
  errno_enum_type           msgr_error;
  byte                    tds_state_local;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  /*Increment active task count*/
  gl1_drx_task_active(gas_id);

  if( FALSE != G2TDS_INIT_sent)
  {
    gl1_TDS_IRAT_STOP_MODE_pending = FALSE;

    gl1_drx_task_inactive(gas_id);
    MSG_GERAN_ERROR_0("L1_send_TDS_CPHY_IRAT_INIT_CMD already sent ");
    return;
  }

 gl1_irat_mutex_lock( gas_id );

  /* initialise msgr header */
  msgr_init_hdr_variant(&init_cmd_msg.hdr,
                        MSGR_GERAN_GL1,
                        TDSCDMA_IRAT_INIT_CMD,
                        (uint8)as_id);

  msgr_error = msgr_send( (msgr_hdr_struct_type *) &init_cmd_msg,
                          sizeof(tdsirat_init_cmd_type)
                        );

  ASSERT(msgr_error == E_SUCCESS);

  MSG_GERAN_MED_0("TDSCDMA_IRAT_INIT_CMD sent");

  G2TDS_INIT_sent = TRUE;

  gl1_TDS_IRAT_INIT_CNF_pending = TRUE;

  tds_state_local = l1_sc_get_tds_state(gas_id);
  l1_sc_set_tds_state(++tds_state_local,gas_id);

 gl1_irat_mutex_unlock(gas_id);

}

#ifdef FEATURE_G2T_SRCH_LOGGING
/* Logs all the parameters sent to Tds by G  for ACQ request during G2T*/
void l1_g2t_irat_log_acq_req(const tdsirat_acq_list_type * acq_list_ptr, gas_id_t gas_id)
{

    G2T_IRAT_ACQ_REQ_LOG_PACKET_type   *acq_log_ptr;
    uint32 i;
    uint8 log_packets_num_of_freq;
    static uint32 previous_fn = 0, current_fn = 0 ;  /* static_dual_spaced_ignore */
    static uint16 previous_qs = 0, current_qs = 0;   /* static_dual_spaced_ignore */

    log_packets_num_of_freq = acq_list_ptr->num_of_freq;
    acq_log_ptr = (G2T_IRAT_ACQ_REQ_LOG_PACKET_type *)log_alloc(G2T_IRAT_ACQ_REQ_LOG_PACKET, G2T_IRAT_ACQ_REQ_PACKET_LEN(log_packets_num_of_freq) );

    if (  acq_log_ptr != NULL)
    {
        acq_log_ptr->version  = 1;
        current_fn = GSTMR_GET_FN_GERAN(gas_id);
        current_qs = gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id) );

        acq_log_ptr->rxlm_rx_buffer = log_packets_tds_startup_req_info.rxlm_rx_buffer;
        acq_log_ptr->num_of_freq = acq_list_ptr->num_of_freq;

        memscpy( &(acq_log_ptr->tds_gap_info), sizeof(acq_log_ptr->tds_gap_info),&log_packets_tds_startup_req_info.tds_gap_info, sizeof(gsm_l1_tds_gap_info_type));

        for (i = 0 ; i < log_packets_num_of_freq ; i++) {

             acq_log_ptr->freq[i].uarfcn = acq_list_ptr->freq[i].uarfcn;
             acq_log_ptr->freq[i].ts0_position = acq_list_ptr->freq[i].ts0_position;
             acq_log_ptr->freq[i].last_lna_state = acq_list_ptr->freq[i].last_lna_state;
             acq_log_ptr->freq[i].last_acq_state = acq_list_ptr->freq[i].last_acq_state;

        }

       acq_log_ptr->time_since_last_measurement_ms     =     (( (( current_fn - previous_fn) * QS_PER_FRAME ) + (current_qs - previous_qs) ) * 0.000923 );
       
       log_commit(acq_log_ptr);

       previous_fn = current_fn;
       previous_qs = current_qs;

    }
}

/* Logs all the parameters sent to Tds by G  for Meas request during G2T*/
void l1_g2t_irat_log_meas_req(const tdsirat_freq_list_type * meas_list_ptr, gas_id_t gas_id)
{

    G2T_IRAT_MEAS_REQ_LOG_PACKET_type   *meas_log_ptr;
    uint32 i,j;
    uint8 log_packets_num_of_freq, log_packets_num_of_cells;

    static uint32 previous_fn = 0, current_fn = 0 ;  /* static_dual_spaced_ignore */
    static uint16 previous_qs = 0, current_qs = 0;   /* static_dual_spaced_ignore */

    log_packets_num_of_freq = meas_list_ptr->num_of_freq;
    meas_log_ptr = (G2T_IRAT_MEAS_REQ_LOG_PACKET_type *)log_alloc(G2T_IRAT_MEAS_REQ_LOG_PACKET, G2T_IRAT_MEAS_REQ_PACKET_LEN(log_packets_num_of_freq) );
    
    if (  meas_log_ptr != NULL)
    {
        meas_log_ptr->version  = 1;

        current_fn = GSTMR_GET_FN_GERAN(gas_id);
        current_qs = gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id));

        meas_log_ptr->rxlm_rx_buffer = log_packets_tds_startup_req_info.rxlm_rx_buffer;
        meas_log_ptr->num_of_freq  =  meas_list_ptr->num_of_freq;
        memscpy( &(meas_log_ptr->tds_gap_info),sizeof(meas_log_ptr->tds_gap_info), &log_packets_tds_startup_req_info.tds_gap_info, sizeof(gsm_l1_tds_gap_info_type));

        for (i = 0 ; i < log_packets_num_of_freq ; i++) {
             meas_log_ptr->freq[i].uarfcn = meas_list_ptr->freq[i].uarfcn;
             meas_log_ptr->freq[i].measuredFreq= meas_list_ptr->freq[i].measuredFreq;
             meas_log_ptr->freq[i].num_of_cells = meas_list_ptr->freq[i].num_of_cells;
             meas_log_ptr->freq[i].ts0_position= meas_list_ptr->freq[i].ts0_position;
             meas_log_ptr->freq[i].acq_state = meas_list_ptr->freq[i].acq_state;

             log_packets_num_of_cells = meas_list_ptr->freq[i].num_of_cells ;

             if(log_packets_num_of_cells > TDSIRAT_MAX_TDS_CELLS_PER_FREQ)
             {
              MSG_GERAN_HIGH_1_G("G2T Logging: Cells clamped num of cells = %d",log_packets_num_of_cells);
              log_packets_num_of_cells = TDSIRAT_MAX_TDS_CELLS_PER_FREQ;
             }

             for (j = 0 ; j< log_packets_num_of_cells ; j++) {

                  meas_log_ptr->freq[i].cell[j].cpid = meas_list_ptr->freq[i].cell[j].cpid;
                  meas_log_ptr->freq[i].cell[j].rscp = meas_list_ptr->freq[i].cell[j].rscp;
                  meas_log_ptr->freq[i].cell[j].cell_position = meas_list_ptr->freq[i].cell[j].cell_position;
                  
             }

          }
       meas_log_ptr->time_since_last_measurement_ms     =     (( (( current_fn - previous_fn) * QS_PER_FRAME ) + (current_qs - previous_qs) ) * 0.000923 );

       log_commit(meas_log_ptr);

       previous_fn = current_fn;
       previous_qs = current_qs;
    }
}

#endif /*FEATURE_G2T_SRCH_LOGGING*/

void  L1_send_TDS_IRAT_ACQ_REQ (const tdsirat_acq_list_type * acq_list_ptr, gas_id_t gas_id)
{
  tdsirat_acq_req_type     acq_req_msg;
  errno_enum_type          msgr_error;
  uint8                    freq_index = 0;
  byte                     tds_state_local;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  if( FALSE == G2TDS_INIT_sent /*|| (tds_state != TDS_STARTUP_REQ)*/)
  {
    MSG_GERAN_ERROR_2(" TDS_ACQ_REQ in wrong state: G2TDS_INIT_sent %d TDS State %d",
              G2TDS_INIT_sent,l1_sc_get_tds_state(gas_id));
    return;
  }

  if ( (acq_list_ptr->num_of_freq > TDSIRAT_MAX_TDS_FREQ) ||
       (acq_list_ptr->num_of_freq== 0)
     )
  {
    MSG_GERAN_ERROR_1("TDSCDMA_IRAT_ACQ_REQ: freq_list size incorrect: size %d ",
              acq_list_ptr->num_of_freq);
    return;
  }

  /* initialise msgr header */
  msgr_init_hdr_variant(&acq_req_msg.hdr,
                        MSGR_GERAN_GL1,
                        TDSCDMA_IRAT_ACQ_REQ,
                        (uint8)as_id);

  /*values given in TDS time units*/
  acq_req_msg.acq_list.num_of_freq = acq_list_ptr->num_of_freq;

  while ( freq_index < acq_list_ptr->num_of_freq )
  {
      acq_req_msg.acq_list.freq[freq_index].uarfcn         = acq_list_ptr->freq[freq_index].uarfcn;
      acq_req_msg.acq_list.freq[freq_index].ts0_position   = acq_list_ptr->freq[freq_index].ts0_position;
      acq_req_msg.acq_list.freq[freq_index].last_lna_state = acq_list_ptr->freq[freq_index].last_lna_state;
      acq_req_msg.acq_list.freq[freq_index].last_acq_state = acq_list_ptr->freq[freq_index].last_acq_state;
      freq_index++;
  }

  g2t_populate_startup_params(&acq_req_msg.startup_params, gas_id);

  msgr_error = msgr_send( (msgr_hdr_struct_type *) &acq_req_msg, sizeof(tdsirat_acq_req_type));

  ASSERT(msgr_error == E_SUCCESS);
  MSG_GERAN_MED_1("TDS_CPHY_IRAT_ACQ_REQ sent TDS State %d", l1_sc_get_tds_state(gas_id));

 #ifdef FEATURE_G2T_SRCH_LOGGING
  l1_g2t_irat_log_acq_req(acq_list_ptr, gas_id);
#endif /* FEATURE_G2T_SRCH_LOGGING */

  l1_send_tds_irat_build_rfscripts[gas_id] = FALSE;
  // In ACQ_RSP, there is a check on tds_state.
  tds_state_local = TDS_ACQ_REQ;
  l1_sc_set_tds_state(++tds_state_local,gas_id);
}


void  L1_send_TDS_IRAT_MEAS_REQ (const tdsirat_freq_list_type * meas_list_ptr,gas_id_t gas_id)
{
  tdsirat_meas_req_type  meas_req_msg;
  errno_enum_type          msgr_error;
  uint8                    freq_index = 0;
  uint16                   i;
  byte                   tds_state_local;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  if( FALSE == G2TDS_INIT_sent/* || (tds_state != TDS_ACQ_RSP)*/)
  {
    MSG_GERAN_ERROR_2(" TDSCDMA_IRAT_MEAS_REQ in wrong state: G2TDS_INIT_sent %d TDS State %d",
                         G2TDS_INIT_sent,l1_sc_get_tds_state(gas_id));
    return;
  }

  if ( meas_list_ptr->num_of_freq > TDSIRAT_MAX_TDS_FREQ ||
       meas_list_ptr->num_of_freq == 0
     )
  {
    MSG_GERAN_ERROR_2("TDSCDMA_IRAT_MEAS_REQ: freq_list size incorrect: size %d Max %d",
              meas_list_ptr->num_of_freq,TDSIRAT_MAX_TDS_FREQ);
    return;
  }

  /* initialise msgr header */
  msgr_init_hdr_variant(&meas_req_msg.hdr,
                        MSGR_GERAN_GL1,
                        TDSCDMA_IRAT_MEAS_REQ,
                        (uint8)as_id);

  /*values given in TDS time units*/
  meas_req_msg.freq_list.num_of_freq = meas_list_ptr->num_of_freq;

  while ( freq_index < meas_list_ptr->num_of_freq )
  {
      meas_req_msg.freq_list.freq[freq_index].uarfcn           = meas_list_ptr->freq[freq_index].uarfcn;
      meas_req_msg.freq_list.freq[freq_index].num_of_cells     = meas_list_ptr->freq[freq_index].num_of_cells;
      meas_req_msg.freq_list.freq[freq_index].num_of_jds_cells = meas_list_ptr->freq[freq_index].num_of_jds_cells;
      meas_req_msg.freq_list.freq[freq_index].rssi             = meas_list_ptr->freq[freq_index].rssi;
      meas_req_msg.freq_list.freq[freq_index].lna_state        = meas_list_ptr->freq[freq_index].lna_state;
      meas_req_msg.freq_list.freq[freq_index].acq_state        = meas_list_ptr->freq[freq_index].acq_state;
      meas_req_msg.freq_list.freq[freq_index].ts0_position     = meas_list_ptr->freq[freq_index].ts0_position;

      for ( i = 0; i < meas_list_ptr->freq[freq_index].num_of_cells; i++ )
      {
        meas_req_msg.freq_list.freq[freq_index].cell[i].cpid =  meas_list_ptr->freq[freq_index].cell[i].cpid;
        meas_req_msg.freq_list.freq[freq_index].cell[i].rscp =  meas_list_ptr->freq[freq_index].cell[i].rscp;
        meas_req_msg.freq_list.freq[freq_index].cell[i].cell_position =  meas_list_ptr->freq[freq_index].cell[i].cell_position;
      }

      freq_index++;
  }

  g2t_populate_startup_params(&meas_req_msg.startup_params, gas_id);

  msgr_error = msgr_send( (msgr_hdr_struct_type *) &meas_req_msg, sizeof(tdsirat_meas_req_type));

  ASSERT(msgr_error == E_SUCCESS);
  MSG_GERAN_MED_1("TDS_CPHY_IRAT_MEAS_REQ sent TDS State %d", l1_sc_get_tds_state(gas_id));

  #ifdef FEATURE_G2T_SRCH_LOGGING
  l1_g2t_irat_log_meas_req(meas_list_ptr, gas_id);
  #endif /* FEATURE_G2T_SRCH_LOGGING */
  l1_send_tds_irat_build_rfscripts[gas_id] = FALSE;
  tds_state_local = l1_sc_get_tds_state(gas_id);
  l1_sc_set_tds_state(++tds_state_local,gas_id);
}

/*There is no STOP_CNF response for this CMD*/

void  L1_send_TDS_IRAT_STOP_CMD (gas_id_t gas_id)
{
  tdsirat_stop_cmd_type   stop_cmd_msg;
  errno_enum_type           msgr_error;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  /*Increment active task count*/
  gl1_drx_task_active(gas_id);

  gl1_irat_mutex_lock( gas_id);

  /*
  ** if TDSCDMA_IRAT_INIT_CNF failed must not send stop
  */
  if(( FALSE == G2TDS_INIT_sent) || (TRUE == gl1_TDS_IRAT_INIT_CNF_pending))
  {
    if(gl1_TDS_IRAT_INIT_CNF_pending == TRUE)
    {
      gl1_TDS_IRAT_STOP_MODE_pending = TRUE;
      MSG_GERAN_HIGH_0_G( " gl1_TDS_IRAT_INIT_CNF_pending is  TRUE");
    }
    else
    {
      MSG_GERAN_ERROR_0("G2TDS_INIT_sent is false not sending TDS_IRAT_STOP_CMD");
    }

    gl1_irat_mutex_unlock( gas_id);

    /*decrement active task count*/
    gl1_drx_task_inactive(gas_id);
    return;
  }

  /* initialise msgr header */
  msgr_init_hdr_variant(&stop_cmd_msg.hdr,
                        MSGR_GERAN_GL1,
                        TDSCDMA_IRAT_STOP_CMD,
                        (uint8)as_id);

  msgr_error = msgr_send( (msgr_hdr_struct_type *) &stop_cmd_msg,
                          sizeof(tdsirat_stop_cmd_type)
                        );

  ASSERT(msgr_error == E_SUCCESS);

  MSG_GERAN_MED_1("TDSCDMA_IRAT_STOP_CMD sent TDS State %d reset to TDS_NULL", l1_sc_get_tds_state(gas_id));

  l1_sc_set_tds_state(TDS_NULL,gas_id);
  G2TDS_INIT_sent = FALSE;
  gl1_TDS_IRAT_STOP_MODE_pending = FALSE;
  g2t_stop_mode_cnf_pending = TRUE;
  
  gl1_irat_mutex_unlock( gas_id);

}

/*===========================================================================

FUNCTION l1_sc_reset_G2T_state

DESCRIPTION
  If TDSCDMA_IRAT_INIT_CMD fails need to be able to clear TDS irat state

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_reset_G2T_state(gas_id_t gas_id)
{
    /*
    ** Reset the TDS state to prevent further G2T IRAT activity.
    */
    G2TDS_INIT_sent = FALSE;

    gl1_TDS_IRAT_STOP_MODE_pending = FALSE;

    l1_sc_set_tds_state(TDS_NULL, gas_id);
    l1_sc_set_tds_irat_active(FALSE, gas_id);
    /* this sends ue_mode_change_cnf */
    l1_sc_mode_cnf_pending(FALSE, bTDS_RAT_INACTIVE, gas_id);
}

/* Following messages are responses to X2G activity */

/*===========================================================================

FUNCTION L1_send_TDS_RSSI_MEAS_RSP
DESCRIPTION
  Sends RSSI Meas response to TL1

===========================================================================*/
void L1_send_TDS_RSSI_MEAS_RSP( gl1_hw_x2g_rssi_meas_ctl_t *ptr,
                                sys_modem_as_id_e_type      as_id )
{

  gsm_irat_meas_search_rsp_t    rssi_meas_msg;
  errno_enum_type                 msgr_error;
  uint8                           i,num_rpt;

  if( (ptr->meas.num == 0 ) && ( ptr->error_flag == FALSE ) )
  {
    /* do not send the message since this is just one of the state machines cleaning up */
    MSG_GERAN_ERROR_0(" not sending RSSI MEAS RSP ");

  return;
  }

  memset(&rssi_meas_msg, 0, sizeof(gsm_irat_meas_search_rsp_t));

  /* initialise msgr header */
  msgr_init_hdr_variant(&rssi_meas_msg.hdr,
                        MSGR_GERAN_GL1,
                        GERAN_GL1_MEAS_RSP,
                        (uint8)as_id);

  rssi_meas_msg.message_id = ptr->seq_num;
  if( (TRUE==ptr->gl1_skip_last_burst) && ( ptr->meas.num != 0))
  {
     ptr->meas.num -= 1;
  }

  num_rpt = 0;
  if (ptr->meas.num != 0)
  {
    for ( i=0; i<ptr->meas.num;i++ )
    {
       if (ptr->meas.samples[i].yield == FALSE)
       {
          rssi_meas_msg.arfcn[num_rpt].num             = ptr->meas.ARFCN[i].num  ;
          rssi_meas_msg.arfcn[num_rpt].band            = ptr->meas.ARFCN[i].band  ;
          rssi_meas_msg.samples[num_rpt]               = ptr->meas.samples[i];
          num_rpt++;
       }
#ifdef FEATURE_GSM_TDS_DEBUG2
       MSG_GERAN_HIGH_2_G(" RSSI send out to TL1 = %d arfcn =%d",rssi_meas_msg.samples[i].dBm_x16,   rssi_meas_msg.arfcn[i].num);
#endif
    }
  }

  rssi_meas_msg.num_meas = num_rpt;

  /* If all bursts yielded, no results so set error flag */
  if(num_rpt == 0)
  {
     rssi_meas_msg.error_flag = TRUE;
  }
  else
  {
     rssi_meas_msg.error_flag = ptr->error_flag;
  }


  msgr_error = msgr_send( (msgr_hdr_struct_type *) &rssi_meas_msg,
                          sizeof(gsm_irat_meas_search_rsp_t)
                        );

  ASSERT(msgr_error == E_SUCCESS);

  MSG_GERAN_MED_2("GERAN_X2G_RSSI_MEAS_RSP sent, num %d, error %d ", num_rpt, rssi_meas_msg.error_flag);
}


void L1_send_TDS_SCH_MEAS_RSP( gsm_irat_sch_burst_rsp_t*  sch_store,
                               gl1_defs_rx_hdr_struct*    sch_hdr_ptr,
                               uint8*                     sch_data_ptr,
                               gl1_defs_metrics_rpt*      sch_metrics_ptr,
                               sys_modem_as_id_e_type     as_id)
{

  gsm_irat_sch_burst_rsp_t      sch_meas_msg;
  errno_enum_type               msgr_error;
  uint8                         i;

  /*Initialize msg_sch_data to 0*/
  for ( i=0; i<MAX_SCH_OCTETS;i++ )
  {
    sch_meas_msg.sch_data[i] = 0;
  }

  /* initialise msgr header */
  msgr_init_hdr_variant(&sch_meas_msg.hdr,
                        MSGR_GERAN_GL1,
                        GERAN_GL1_SCH_RSP,
                        (uint8)as_id);

  sch_meas_msg.arfcn.band       = sch_store->arfcn.band;
  sch_meas_msg.arfcn.num        = sch_store->arfcn.num;
  sch_meas_msg.crc_pass         = (sch_hdr_ptr->good_data == 2 ? TRUE: FALSE );
  sch_meas_msg.time_offset      = sch_metrics_ptr->timing_offset;
  sch_meas_msg.message_id       = sch_store->message_id;
  sch_meas_msg.error_flag       = sch_store->error_flag;
  sch_meas_msg.schedule_error    = sch_store->schedule_error;
  
  sch_meas_msg.sch_frame        = sch_store->sch_frame;
  sch_meas_msg.reconfirm        = sch_store->reconfirm;
  sch_meas_msg.gsm_cell_ptr     = sch_store->gsm_cell_ptr;

#ifdef FEATURE_GPRS_GBTA

#ifdef FEATURE_GSM_COEX
  sch_meas_msg.yield            = ((sch_hdr_ptr->scheduled_status & GPL1_SCH_SCHDULE_SUCCESS) == 0);
#else
  sch_meas_msg.yield            = FALSE;
#endif /* FEATURE_GSM_COEX */

#else
  sch_meas_msg.yield            = FALSE;
#endif /* FEATURE_GPRS_GBTA */

  if ( sch_meas_msg.crc_pass )
  {
      for ( i=0; i<MAX_SCH_OCTETS;i++ )
      {
        sch_meas_msg.sch_data[i] = *sch_data_ptr;
        sch_data_ptr++;
      }
  }

  MSG_GERAN_ERROR_1("RFN %d", get_RFN_from_sch_data( sch_meas_msg.sch_data ));


  msgr_error = msgr_send( (msgr_hdr_struct_type *) &sch_meas_msg,
                          sizeof(gsm_irat_sch_burst_rsp_t)
                        );

  ASSERT(msgr_error == E_SUCCESS);

  MSG_GERAN_HIGH_2("GERAN_X2G_SCH_MEAS_RSP sent schedule_error %d SCH crc %d",sch_meas_msg.schedule_error,sch_meas_msg.crc_pass);
}


void L1_send_TDS_ACQ_RSP( gsm_irat_acquisition_rsp_t *ptr,
                          sys_modem_as_id_e_type      as_id )
{

  gsm_irat_acquisition_rsp_t      acq_msg;
  errno_enum_type                 msgr_error;

  acq_msg        = *ptr;
  /* initialise msgr header */
  msgr_init_hdr_variant(&acq_msg.hdr,
                        MSGR_GERAN_GL1,
                        GERAN_GL1_ACQ_RSP,
                        (uint8)as_id);

  msgr_error = msgr_send( (msgr_hdr_struct_type *) &acq_msg,
                          sizeof(gsm_irat_acquisition_rsp_t)
                        );

  ASSERT(msgr_error == E_SUCCESS);

  MSG_GERAN_HIGH_4("GERAN_X2G_ACQ_RSP sent Tone detect %d, SCH crc %d snr %d dBm, Schedule Err %d",
          acq_msg.tone_detect,acq_msg.sch_crc_pass,acq_msg.snr, acq_msg.schedule_error);
}

void L1_send_TDS_G2T_ABORT_REQ( gas_id_t gas_id  )
{
  tdsirat_init_cmd_type           abort_msg;
  errno_enum_type                 msgr_error;
  l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  /* initialise msgr header */
  msgr_init_hdr_variant(&abort_msg.hdr,
                        MSGR_GERAN_GL1,
                        TDSCDMA_IRAT_ABORT_CMD,
                        (uint8)as_id);

  msgr_error = msgr_send( (msgr_hdr_struct_type *) &abort_msg,
                          sizeof(tdsirat_init_cmd_type)
                        );

  ASSERT(msgr_error == E_SUCCESS);

  l1_sc_wcdma_data_ptr->l1_g2t_abort_cmd_sent = TRUE;
  MSG_GERAN_HIGH_0_G("GERAN_G2X_ABORT_CMD sent");
}

#endif /* FEATURE_GSM_TDS */

/*===========================================================================

FUNCTION g2x_irat_cnf_pending

DESCRIPTION
  return TRUE if any irat confirm is still pending.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean g2x_irat_cnf_pending(gas_id_t gas_id)
{
    boolean  g2x_cnf_pending = FALSE;
        
    g2x_cnf_pending = g2x_cnf_pending
      #ifdef FEATURE_GSM_TDS
                       || gl1_TDS_IRAT_INIT_CNF_pending || gl1_TDS_IRAT_STOP_MODE_pending
      #endif
      #ifdef FEATURE_GSM_TO_LTE
                       || ((G2L_INIT_sent[gas_id] & LTE_G2L_READY)  ==  LTE_G2L_INIT_SENT)      /* G2L init_cnf pending */
                       || ((G2L_INIT_sent[gas_id] & LTE_G2L_DEINIT) ==  LTE_G2L_DEINIT_SENT)    /* G2L deinit_cnf pending */
      #endif
                       || gl1_check_if_g2w_init_deinit_pend(gas_id);                            /* G2W init/deinit_cnf pending */

 
     return g2x_cnf_pending;            

}

void L1_send_gprs_gmac_message(l1_mac_sig_t *common_msg_ptr, uint8 *pacch_data,uint32 FN, gas_id_t gas_id)
{

#ifdef L1_MSG_TRAFFIC_LOGGING
   l1_log_msg_traffic(common_msg_ptr->task_id, common_msg_ptr->sig_id, FN, FALSE, gas_id);
#endif
   gprs_mac_l1_acc_func (common_msg_ptr,pacch_data, gas_id);
}

/***************************************************************************************
     gl1 trm interface for antenna diversity

***************************************************************************************/



/***************************************************************************************/
#ifdef FEATURE_GSM_QSC_TX_DIV

/*===========================================================================

FUNCTION gl1_trm_asdiv_enable
  Check whether Type1 is enabled by band.

  Band Bit Mask:
  1 << 0 = SYS_BAND_EGSM_900
  1 << 1 = SYS_BAND_PGSM_900
  1 << 2 = SYS_BAND_PCS_1900
  1 << 3 = SYS_BAND_DCS_1800
  1 << 4 = SYS_BAND_CELL_850

Parameters
  gas_id: gas_id
  
RETURN VALUE
  TRUE if ASDIV is enabled for the current band
	FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/

boolean gl1_trm_asdiv_enable(gas_id_t gas_id, gl1_trm_asdiv_switch_type_T switch_type)
{
  
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  boolean asdiv_enable = FALSE;

  //Get the serving Cell Arfcn
  ARFCN_T serving_cell = gl1_get_serving_cell_arfcn(gas_id);

  //Get Current band from DTM Params 
  //which is programmed when RF receive is attempted in Dedi. or Idle mode
  sys_band_T curr_band = (sys_band_T)gl1_hw_get_dtm_band(gas_id);

  //If in idle mode use serving cell band instead
  if ((l1_tskisr_blk->l1_state == L1_IDLE_MODE)||
      (l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE))
  {
     curr_band = serving_cell.band;
  }

  gl1_asdiv_set_UL_DL_delta(gas_id,curr_band);
  if((curr_band != INVALID_BAND) && (curr_band != SYS_BAND_NONE))
  {
  	uint8 band_mask = 0;
    if(switch_type == GL1_TRM_ASDIV_TYPE1)
    {
    	band_mask = antenna_swith_tx_div_nv_enable;	
    }
    else if(switch_type == GL1_TRM_ASDIV_TYPE2)
    {
    	band_mask = tx_div_type2_enable[gas_id];
    }
		
		
    asdiv_enable = (((1 << curr_band) & band_mask) != 0);
    if (asdiv_enable)
    {
      MSG_GERAN_HIGH_3_G("ASDIV Type%d: curr_band = %d, band_mask = %d", switch_type+1, curr_band, band_mask);
    }
  }
  return asdiv_enable;
}

void gl1_asdiv_set_UL_DL_delta(gas_id_t gas_id, sys_band_T curr_band)
{
  switch (curr_band)
  {
     case SYS_BAND_EGSM_900:
	 	gl1_hw_asdiv_mtpl_store[gas_id].UL_delta = asdiv_UL_DL_delta_table_store.EGSM_900_UL_delta;
		gl1_hw_asdiv_mtpl_store[gas_id].DL_delta = asdiv_UL_DL_delta_table_store.EGSM_900_DL_delta;
	 break;

	 case SYS_BAND_PGSM_900:
	 	gl1_hw_asdiv_mtpl_store[gas_id].UL_delta = asdiv_UL_DL_delta_table_store.PGSM_900_UL_delta;
		gl1_hw_asdiv_mtpl_store[gas_id].DL_delta = asdiv_UL_DL_delta_table_store.PGSM_900_DL_delta;
	 break;

	 case SYS_BAND_PCS_1900:
        gl1_hw_asdiv_mtpl_store[gas_id].UL_delta = asdiv_UL_DL_delta_table_store.PCS_1900_UL_delta;
		gl1_hw_asdiv_mtpl_store[gas_id].DL_delta = asdiv_UL_DL_delta_table_store.PCS_1900_DL_delta;       
     break;
	 
     case SYS_BAND_DCS_1800:
        gl1_hw_asdiv_mtpl_store[gas_id].UL_delta = asdiv_UL_DL_delta_table_store.DCS_1800_UL_delta;
		gl1_hw_asdiv_mtpl_store[gas_id].DL_delta = asdiv_UL_DL_delta_table_store.DCS_1800_DL_delta;
	 break;
	 
     case SYS_BAND_CELL_850:
		gl1_hw_asdiv_mtpl_store[gas_id].UL_delta = asdiv_UL_DL_delta_table_store.CELL_850_UL_delta;
		gl1_hw_asdiv_mtpl_store[gas_id].DL_delta = asdiv_UL_DL_delta_table_store.CELL_850_DL_delta;
     break;

	 default:
	 	gl1_hw_asdiv_mtpl_store[gas_id].UL_delta = 0;
	    gl1_hw_asdiv_mtpl_store[gas_id].DL_delta = 0;
	 break;

   }
   gl1_hw_asdiv_mtpl_store[gas_id].imb_ULDL = 16*(gl1_hw_asdiv_mtpl_store[gas_id].UL_delta - 
   	                                          gl1_hw_asdiv_mtpl_store[gas_id].DL_delta);
}




/*===========================================================================

FUNCTION gl1_trm_asdiv_type2_trans_store_rx_pwr
  Function stores serving cell RSSI

Parameters
  gas_id: gas_id
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_trm_asdiv_type2_trans_store_rx_pwr(gas_id_t gas_id, dBx16_T dbm_x16_val)
{
    gl1_trm_asdiv_traffic_mode_T * gl1_trm_asdiv_traffic_var = &gl1_trm_asdiv_traffic_store[gas_id];
    gl1_trm_asdiv_traffic_var->rx_pwr_dbmx16_trans_sum += (int32)dbm_x16_val;
    gl1_trm_asdiv_traffic_var->num_meas_data_traffic++;
    MSG_GERAN_HIGH_2_G("num_meas_data_traffic = %d, rx_pwr stored = %d", gl1_trm_asdiv_traffic_var->num_meas_data_traffic, (int32)dbm_x16_val);
}

/*===========================================================================

FUNCTION gl1_trm_asdiv_qsc_trans_alg
  Function is the entry point to the asdiv type2 switching algorithm for data traffic

Parameters
  gas_id: gas_id
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_trm_asdiv_type2_trans_alg(gas_id_t gas_id)
{
    gl1_trm_asdiv_traffic_mode_T * gl1_trm_asdiv_traffic_var = &gl1_trm_asdiv_traffic_store[gas_id];


    if((gl1_trm_asdiv_traffic_var->num_meas_periods == GL1_ASDIV_NUM_MEAS_PERIODS)&&
	   (gl1_trm_asdiv_traffic_var->num_meas_data_traffic != 0))
    {
        dBx16_T rx_pwr_dbmx16_ave = (dBx16_T)(gl1_trm_asdiv_traffic_var->rx_pwr_dbmx16_trans_sum/gl1_trm_asdiv_traffic_var->num_meas_data_traffic);
        MSG_GERAN_HIGH_2_G("num_meas_data_traffic = %d, average_rxpwr_dbmx16 = %d", gl1_trm_asdiv_traffic_var->num_meas_data_traffic, rx_pwr_dbmx16_ave);
        l1_asdiv_ded_meas_qsc_alg(gas_id, L1_map_dBm_x16_to_rxlev(rx_pwr_dbmx16_ave));

        /*Reset Data Traffic Variables*/
        gl1_trm_asdiv_traffic_var->rx_pwr_dbmx16_trans_sum = 0;
        gl1_trm_asdiv_traffic_var->num_meas_data_traffic = 0;
        gl1_trm_asdiv_traffic_var->num_meas_periods = 0;
    }

    /*Increment every 13 frames (scell meas done every 13 frames in traffic)*/
    gl1_trm_asdiv_traffic_var->num_meas_periods++;
}

/*===========================================================================

FUNCTION gl1_trm_asdiv_reset_traffic_state_vars
  Resets Asdiv type2 traffic mode variables

Parameters
  gas_id: gas_id
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/


void gl1_trm_asdiv_reset_traffic_state_vars(gas_id_t gas_id)
{
/*Variables shared between type2 voice traffic and data traffic*/
    gl1_trm_asdiv_traffic_mode_T * gl1_trm_asdiv_traffic_var = &gl1_trm_asdiv_traffic_store[gas_id];
    gl1_trm_asdiv_traffic_var->type2_algorithm_start = 0;
    gl1_trm_asdiv_traffic_var->previous_rxlev_sub = -1;
    gl1_trm_asdiv_traffic_var->new_rxlev_backup = -1;
    gl1_trm_asdiv_traffic_var->old_rxlev_average = -1;
    gl1_trm_asdiv_traffic_var->rxlev_average = -1;
    gl1_trm_asdiv_traffic_var->wait_counter = -1;
	gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_accum = 0;
	gl1_hw_asdiv_mtpl_store[gas_id].number_of_tx_pwr_avg = 0;
	gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_avg_over_sacch = 0;

/*Variables used by data traffic only*/
    gl1_trm_asdiv_traffic_var->rx_pwr_dbmx16_trans_sum = 0;
    gl1_trm_asdiv_traffic_var->num_meas_data_traffic = 0;
	gl1_trm_asdiv_traffic_var->num_meas_periods = 0;
}


/*===========================================================================

FUNCTION gl1_trm_asdiv_debug_get_test_mode_min_switch_period
  Function gets the minimum asdiv switch period in MS

Parameters
  gas_id: gas_id
  mode: The asdiv mode passed in
  
RETURN VALUE
  uint32 min_switch_period for GL1 in micro seconds

SIDE EFFECTS
  None

===========================================================================*/

static uint32 gl1_trm_asdiv_debug_get_test_mode_min_switch_period(gas_id_t gas_id, trm_client_mode_type mode)
{
    idle_data_T * l1_idle_data = &l1_idle_data_store[gas_id];
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    
    uint32 min_switch_period;
    if(mode == TRM_CLIENT_MODE_IDLE_WAKEUP)
    {
        /*Idle Mode Paging Cycle*/
        min_switch_period = GL1_ASDIV_MICROSEC_IN_FRAME*l1_idle_data->campedon_cell_data.BS_PA_MFRMS*MULTIFRAME_51;
    }
    else if((mode == TRM_CLIENT_MODE_RXTX) && (l1_tskisr_blk->l1_state == L1_DEDICATED_MODE))
    {
        /*Traffic/Dedicated Mode Reporting Period to RR*/
        min_switch_period = GL1_ASDIV_MICROSEC_IN_FRAME*MULTIFRAME_26*4;
    }
    else if((mode == TRM_CLIENT_MODE_RXTX) && (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE))
    {
        /*Traffic Data Mode Reporting Period to RR*/
        min_switch_period = GL1_ASDIV_MICROSEC_IN_FRAME*MULTIFRAME_26;
    }
    else
    {
        min_switch_period = GL1_ASDIV_MICROSEC_IN_FRAME;
    }
    return min_switch_period;
}


/*===========================================================================

FUNCTION gl1_trm_asdiv_debug_test_mode_init_sm
  Function initializes the test_mode State Machine variables

Parameters
  gas_id: gas_id
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_trm_asdiv_debug_test_mode_init_sm(gas_id_t gas_id)
{
    MSG_GERAN_HIGH_0_G("ASDIV_TEST_MODE: Initializing G ASDIV Testmode Vars");
    gl1_trm_asdiv_test_mode_store[gas_id].antenna_to_use_for_tx = &antenna_to_use_for_tx[gas_id];
    gl1_trm_asdiv_test_mode_store[gas_id].ant_switch_position = FALSE;
    gl1_trm_asdiv_test_mode_store[gas_id].num_elapsed_periods = 0;
}

/*===========================================================================

FUNCTION gl1_trm_asdiv_debug_get_test_mode

DESCRIPTION
  Function gets whether MCS debug test mode is enabled

Parameters
  gas_id: gas id

RETURN VALUE
  boolean     TRUE if debug mode is true

SIDE EFFECTS
  None

===========================================================================*/

boolean gl1_trm_asdiv_debug_get_test_mode(gas_id_t gas_id)
{
    
    if(trm_ant_switch_div_get_test_mode_ctrl() == ALTERNATING_OFF_FORCED_SWITCH)
    {
        MSG_GERAN_HIGH_0_G("ASDIV_TEST_MODE: trm_asdiv test mode enabled");
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



/*===========================================================================

FUNCTION gl1_trm_asdiv_debug_test_mode_switch_sm

DESCRIPTION
  Function is used when trm_ant_switch_div_get_test_mode_ctrl returns true.
  It periodically switches the antenna, dependent on the MCS value obtained from trm_get_ant_switch_div_dwelling_time()

Parameters
  gas_id: gas_id
  mode: The asdiv mode passed in
  
RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/


void gl1_trm_asdiv_debug_test_mode_switch_sm(gas_id_t gas_id, trm_client_mode_type mode)
{
    /*Minimum GL1 Switch Period in MS (depends on the mode either IDLE or Wakeup*/
    uint32 min_switch_period;
    trm_set_ant_switch_return_type trm_retrn = TRM_ANT_SWITCH_DIV_SET_CFG_DENIED;
    uint64 time_elapsed_micro_seconds;
    uint64 dwell_time_micro_seconds;
    trm_ant_switch_set_input_type ant_switch_set_input;
    trm_switch_complete_input_type trm_switch_complete;
	grm_client_enum_t grm_client_id;
	gl1_trm_asdiv_test_mode_T * gl1_trm_asdiv_test_mode_data = NULL;
	
	gas_id = check_gas_id(gas_id);
	gl1_trm_asdiv_test_mode_data = &gl1_trm_asdiv_test_mode_store[gas_id];
    min_switch_period = gl1_trm_asdiv_debug_get_test_mode_min_switch_period(gas_id, mode);
    
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    grm_client_id = gl1_map_gas_id_to_client_id(gas_id);
    grm_client_id = check_asdiv_grm_client(grm_client_id);
    ant_switch_set_input.client = trm_switch_complete.client = grm_map_grm_client_id_to_trm_client_id(grm_client_id);
#else
    ant_switch_set_input.client = trm_switch_complete.client = TRM_GSM1;
#endif /* ( FEATURE_DUAL_SIM ) ||  ( FEATURE_TRIPLE_SIM ) */   

    /*Time Elapsed in units of microseconds*/
    time_elapsed_micro_seconds = (uint64)(gl1_trm_asdiv_test_mode_data->num_elapsed_periods*min_switch_period);

    /*convert from centiseconds to us*/
    dwell_time_micro_seconds = trm_get_ant_switch_div_dwelling_time()*10000; 

    MSG_GERAN_HIGH_4_G("ASDIV_TEST_MODE: min_switch_period = %d, num_elapsed_periods = %d, dwell_time = %d, time_elapsed=%d",
                        min_switch_period,
                        gl1_trm_asdiv_test_mode_data->num_elapsed_periods,
                        dwell_time_micro_seconds,
                        time_elapsed_micro_seconds);

    /*trm_ant_switch_div_dwelling_time_type returns time in milliseconds, min Switch Period is in micro seconds */
    /*If time elapsed is > than the switch time, we switch and restart the switch count*/
    if(time_elapsed_micro_seconds > dwell_time_micro_seconds )
    {
        MSG_GERAN_HIGH_0_G("ASDIV_TEST_MODE: Switch Period Elapsed, Test_Mode Switch Starting");
        /*make sure to update GL1 states*/
        if( (*gl1_trm_asdiv_test_mode_data->antenna_to_use_for_tx) == 1)
        {
            *gl1_trm_asdiv_test_mode_data->antenna_to_use_for_tx = 2;
            gl1_trm_asdiv_test_mode_data->ant_switch_position = TRUE;
            ant_switch_set_input.config = TRM_ANT_SWITCH_DIV_CONFIG_1;
        }
        else if( (*gl1_trm_asdiv_test_mode_data->antenna_to_use_for_tx) == 2)
        {
            *gl1_trm_asdiv_test_mode_data->antenna_to_use_for_tx = 1;
            gl1_trm_asdiv_test_mode_data->ant_switch_position = FALSE;
            ant_switch_set_input.config = TRM_ANT_SWITCH_DIV_CONFIG_0;
        }
        else
        {
            MSG_GERAN_ERROR_1_G("ASDIV_TEST_MODE: Invalid antenna = %d", (*gl1_trm_asdiv_test_mode_data->antenna_to_use_for_tx));
        }
        trm_retrn = trm_ant_switch_div_set_config(ant_switch_set_input);
        gl1_trm_asdiv_test_mode_data->num_elapsed_periods = 0;
    }
    else
    {
        gl1_trm_asdiv_test_mode_data->num_elapsed_periods++;
    }


    /*Check whether asdiv switch resources were available on the MCS side. If they were available, then we switch RF*/
    if (trm_retrn == TRM_ANT_SWITCH_DIV_SET_SUCCESS)
    {   
        if (gl1_hw_rfa_rf_gsm_set_antenna_req_cmd (gl1_trm_asdiv_test_mode_data->ant_switch_position,gas_id )) 
        {
            MSG_GERAN_HIGH_3_G ("ASDIV_TEST_MODE: RF set antenna %d  sucessfully, ant_switch_position = %d ",*gl1_trm_asdiv_test_mode_data->antenna_to_use_for_tx,gl1_trm_asdiv_test_mode_data->ant_switch_position ,0);
            trm_switch_complete.config = ant_switch_set_input.config ;
            trm_ant_switch_div_switch_complete (trm_switch_complete );
            MSG_GERAN_HIGH_3_G ("ASDIV_TEST_MODE: trm_ant_switch_div_set_config sucess for antenna = %d ",*gl1_trm_asdiv_test_mode_data->antenna_to_use_for_tx,0,0);
        } 
        else
        {
            MSG_GERAN_HIGH_3_G ("ASDIV_TEST_MODE: RF set antenna %d wrong ",*gl1_trm_asdiv_test_mode_data->antenna_to_use_for_tx,0,0);
        }           
    } 
    else
    {
        MSG_GERAN_ERROR_3_G ("trm_ant_switch_div_set_config fail trm_retrn = %d ",trm_retrn,0,0);
    }

}


/*===========================================================================

FUNCTION gl1_hw_get_asdiv_preferred_antenna
  Sets the asdiv preferred antenna when G goes to sleep
  Gets the asdiv preferred antenna when G wakes up

Parameters
  gas_id: gas_id
  mode: TRM client mode (either idle_wakeup or sleep)
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/



trm_ant_switch_div_config_type gl1_hw_get_asdiv_preferred_antenna(gas_id_t gas_id, trm_client_mode_type mode)
{
  trm_ant_switch_div_config_type preferred_antenna = TRM_ANT_SWITCH_DIV_CONFIG_DEFAULT;

  if(mode == TRM_CLIENT_MODE_SLEEP)
  {
    if (type2_alg_antenna_to_use [gas_id] ==1)
    {
      preferred_antenna = TRM_ANT_SWITCH_DIV_CONFIG_0;
    }
    else if (type2_alg_antenna_to_use [gas_id]==2)
    {
      preferred_antenna = TRM_ANT_SWITCH_DIV_CONFIG_1;
    }
    else
    {
      preferred_antenna =TRM_ANT_SWITCH_DIV_CONFIG_DEFAULT;
    }
  }

  else if(mode == TRM_CLIENT_MODE_IDLE_WAKEUP)
  {
    if ( gl1_hw_qta_gap_active(gas_id) || ( gl1_is_any_sub_in_transfer() && gl1_msg_get_multi_sim_standby_mode() ))
    {
      preferred_antenna = TRM_ANT_SWITCH_DIV_CONFIG_CURRENT ;
    }
    else
    {
      if (( type2_alg_antenna_to_use [gas_id] == 1) || (type2_alg_antenna_to_use [gas_id]== 2))
      {
#ifdef FEATURE_GERAN_DRDSDS
                if(gl1_drdsds_enabled(gas_id))
                {
                    preferred_antenna = gl1_trm_asdiv_idle_pingpong_thresh_check(gas_id, type2_alg_antenna_to_use [gas_id]);
                }
                else
                {
                    preferred_antenna = TRM_ANT_SWITCH_DIV_CONFIG_PREFERRED;
                }
#else
                preferred_antenna = TRM_ANT_SWITCH_DIV_CONFIG_PREFERRED;
#endif
      }
      else
      {
          preferred_antenna =TRM_ANT_SWITCH_DIV_CONFIG_DEFAULT;
      }
    }
  }
  
  else
  {
    preferred_antenna = TRM_ANT_SWITCH_DIV_CONFIG_DEFAULT;
  }

  return preferred_antenna;
}

/*===========================================================================

FUNCTION gl1_hw_set_asdiv_mode_config

DESCRIPTION
  - Sets the mode (which will be used by MCS to determine the priority of switching)
  - Sets the config (which antenna GSM Tx path should use,
  either Prx0 or Prx1 for type2 algorithm, or Prx or Drx for type1 alg

Parameters
  gas_id: Sub identity to get the TRM client
  mode: Used by MCS to determine the priority of switching

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_hw_set_asdiv_mode_config(gas_id_t gas_id, trm_client_mode_type mode)
{
    if(gl1_trm_asdiv_debug_get_test_mode(gas_id) == FALSE)
    {
        trm_ant_switch_div_config_type preferred_antenna = TRM_ANT_SWITCH_DIV_CONFIG_CURRENT;
        
        /*Use the value stored in NV70219 if type2/type1 algs not enabled*/
        if(gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE1) || gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE2))
        {
            preferred_antenna = gl1_hw_get_asdiv_preferred_antenna(gas_id, mode);
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            gl1_trm_asd_set_mode_and_config ( mode, preferred_antenna, gas_id );
#else
            gl1_trm_ant_switch_div_set_mode ( mode, gas_id);
#endif
        }
        
    }
    else
    {
        if(mode == TRM_CLIENT_MODE_IDLE_WAKEUP)
        {
            gl1_trm_asdiv_debug_test_mode_switch_sm(gas_id, mode);
        }
    }

}



void gl1_trm_ant_switch_div_set_mode (trm_client_mode_type mode_setings ,gas_id_t gas_id)
{
  trm_set_mode_input_type set_mode_data;
  grm_client_enum_t grm_client_id;

  set_mode_data.mode = mode_setings;
  gas_id = check_gas_id(gas_id);
  GL1_ISR_LOCK(gas_id);
  if(mode_setings == TRM_CLIENT_MODE_RXTX)
  {
    l1_asdiv_reset_wait_counter(gas_id);
  }
  else
  {
    antenna_switch_required_at_start_of_next_frame[gas_id] = FALSE;
	antenna_switch_burst[gas_id] = FALSE;
  }
  GL1_ISR_UNLOCK(gas_id);
  
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )  
  grm_client_id = gl1_map_gas_id_to_client_id(gas_id);
  grm_client_id = check_asdiv_grm_client(grm_client_id);
  set_mode_data.client = grm_map_grm_client_id_to_trm_client_id(grm_client_id);
#else
    set_mode_data.client = TRM_GSM1;
#endif /* ( FEATURE_DUAL_SIM ) ||  ( FEATURE_TRIPLE_SIM ) */

  set_mode_data.set_mode_cb = (trm_ant_switch_set_mode_callback_t) gsm_set_mode_cb;
  set_mode_data.ant_switch_cb = (trm_ant_switch_div_callback_t) gsm_ant_switch_cb;
  set_mode_data.inform_switch_change = TRUE ;

  trm_ant_switch_div_set_mode (set_mode_data );

}

trm_ant_switch_div_callback_t gsm_ant_switch_cb (trm_ant_switch_cb_info_type  ant_cfg_info  )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr;
  gas_id_t gas_id = gl1_map_client_id_to_gas_id(grm_map_trm_client_id_to_grm_client_id(ant_cfg_info.client));

  gas_id = check_gas_id(gas_id);
  l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  switch (ant_cfg_info.action)
  {
    case TRM_ANT_SWITCH_SET_CONFIG_STARTED :
      passive_sub_switch_notification[gas_id] = TRUE ;
      if (l1_sc_wcdma_data_ptr->wcdma_srch.state == GtowStateDedicated)
      {
        l1_sc_wcdma_abort_ded_search(gas_id);    /* CgC ded search is active */
      }
      break ;

    case TRM_ANT_SWITCH_SET_CONFIG_DONE:
      MSG_GERAN_HIGH_0_G ("switching done, stop tx blanking");
      break ;

    case  TRM_ANT_SWITCH_INITIATE_SET_CONFIG:
      {
          boolean ant_pos = FALSE;
          passive_sub_switch_notification[gas_id] = TRUE ;
        
      MSG_GERAN_HIGH_0_G ("Send Switch Message to RF Router");
        
      if (ant_cfg_info.config ==TRM_ANT_SWITCH_DIV_CONFIG_0 )
      {
        ant_pos = FALSE;
      }
      else if (ant_cfg_info.config ==TRM_ANT_SWITCH_DIV_CONFIG_1 )
      {
        ant_pos = TRUE;
      }
      else
      {
        MSG_GERAN_HIGH_1_G("ant_switch_cb invalid config = %d, defaulting to 0", ant_cfg_info.config);
      }

          /*Try to set antenna*/
      if (gl1_hw_rfa_rf_gsm_set_antenna_req_cmd (ant_pos ,gas_id))
      {
        MSG_GERAN_HIGH_1_G (" RF set antenna %d sucessfully ", ant_pos);
      }
      else
      {
        MSG_GERAN_HIGH_1_G (" RF set antenna %d wrong ", ant_pos);
      }
    }
    break ;

  default:
    break;
  }

  MSG_HIGH (" gsm_ant_switch_cb  ",0,0,0);
  return (trm_ant_switch_div_callback_t)NULL;
}

trm_ant_switch_set_mode_callback_t gsm_set_mode_cb (trm_ant_switch_set_mode_type  set_mode_info)
{
  if (set_mode_info.set_mode_grant == TRM_SET_MODE_GRANTED)
  {
    MSG_HIGH (" TRM_SET_MODE_GRANTED  ",0,0,0);
  }
  else if ( set_mode_info.set_mode_grant == TRM_SET_MODE_DENIED)
  {
    MSG_HIGH (" TRM_SET_MODE_DENIED  ",0,0,0);
  }
  else
  {
    MSG_HIGH (" invalid mode set   ",0,0,0);
  }

  return (trm_ant_switch_set_mode_callback_t)NULL;
}


/*===========================================================================

FUNCTION gl1_hw_init_asdiv_preferred_antenna

DESCRIPTION
  Sets the initial antenna switch configuration. The value that we use to set can either be 0 or 1.

  0:
    GSM Tx/Rx----> Prx0
  1:
    GSM Tx/Rx----> Prx1

Parameters
  ant_config_type:
    - Can be "TRM_ANT_SWITCH_CONFIG_DEFAULT" (which will take the value stored in NV 70219)
  - Can be "TRM_ANT_SWITCH_CONFIG_CURRENT" (which will take the value stored in MCS global var)

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_hw_init_asdiv_preferred_antenna(gas_id_t gas_id, trm_ant_switch_get_input_type ant_config_type)
{
  switch ( trm_ant_switch_div_get_config ( ant_config_type))
    {
      case TRM_ANT_SWITCH_DIV_CONFIG_0 :
        antenna_to_use_for_tx [gas_id ] = 1 ;
        type2_alg_antenna_to_use [gas_id] = 1;
            if (gl1_hw_rfa_rf_gsm_set_antenna_req_cmd (FALSE , gas_id))
            {
              MSG_GERAN_HIGH_3_G (" RF set antenna 1  sucessfully ",0,0,0);
            }
            else
            {
              MSG_GERAN_HIGH_3_G (" RF set antenna 1 wrong ",0,0,0);
            }

           break;

        case TRM_ANT_SWITCH_DIV_CONFIG_1 :
            antenna_to_use_for_tx [gas_id ] = 2 ;
      type2_alg_antenna_to_use [gas_id] = 2;
        if (gl1_hw_rfa_rf_gsm_set_antenna_req_cmd (TRUE, gas_id))
            {
              MSG_GERAN_HIGH_3_G ("  RF set antenna 2  sucessfully ",0,0,0);
            }
            else
            {
              MSG_GERAN_HIGH_3_G (" RF set antenna 2  wrong ",0,0,0);
            }
            break;

        default :
               MSG_GERAN_LOW_3_G( " Invalid tx div antenna config =%d",0,0,0);
               break;
  }
    MSG_GERAN_LOW_3_G( " Initial antenna antenna_to_use_for_tx [gas_id =%d] =%d",gas_id,antenna_to_use_for_tx [gas_id ],0);

}


void gl1_trm_ant_switch_div_set_config ( gas_id_t gas_id)
{
  trm_ant_switch_set_input_type new_settings ;
  trm_set_ant_switch_return_type ant_switch_grant;
  grm_client_enum_t grm_client_id;

  gas_id = check_gas_id(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  grm_client_id = gl1_map_gas_id_to_client_id(gas_id);
  grm_client_id = check_asdiv_grm_client(grm_client_id);
  new_settings.client = grm_map_grm_client_id_to_trm_client_id(grm_client_id);
  
#ifdef FEATURE_GERAN_DRDSDS
  new_settings.is_crisis_mode = gl1_trm_asdiv_get_traffic_crisis_flag(gas_id);
#endif
 
#else
  new_settings.client = TRM_GSM1 ;
#endif /* ( FEATURE_DUAL_SIM ) ||  ( FEATURE_TRIPLE_SIM ) */

  if(antenna_switch_burst[gas_id] == TRUE)
  {
    MSG_GERAN_ERROR_1_G("antenna_switch_burst is TRUE before set config at FN=%d!",GSTMR_GET_FN_GERAN(gas_id));
  }
  antenna_switch_burst[gas_id] = FALSE;
  
  switch (trm_ant_switch_div_get_config (TRM_ANT_SWITCH_CONFIG_CURRENT ) )
  {
        case TRM_ANT_SWITCH_DIV_CONFIG_0:
            new_settings.config = TRM_ANT_SWITCH_DIV_CONFIG_1 ;
            ant_switch_grant = trm_ant_switch_div_set_config (new_settings );
    #ifdef FEATURE_GERAN_DRDSDS
                        if(!gl1_trm_asdiv_get_traffic_crisis_flag(gas_id))
                        {
                gl1_trm_asdiv_traffic_denial_thresh_check(gas_id, ant_switch_grant);
                        }
    #endif
            switch ( ant_switch_grant )
            {
                case TRM_ANT_SWITCH_DIV_SET_SUCCESS:
                    MSG_GERAN_HIGH_0_G(" do immediate switching single tx case");
                    if (gl1_hw_rfa_rf_gsm_set_antenna_req_cmd (TRUE,gas_id ))
                    {
                        MSG_GERAN_HIGH_0_G(" RF set antenna 2 sucessfully ");
                        antenna_switch_burst[gas_id] = TRUE ;
                    }
                    else
                    {
                        MSG_GERAN_HIGH_0_G(" RF set antenna 2 wrong ");
                    }
                    break;

                default :
                    MSG_GERAN_HIGH_0_G(" trm_ant_switch_div_set_config fail ");
                    break;
            }
            break ;

        case TRM_ANT_SWITCH_DIV_CONFIG_1 :
            new_settings.config = TRM_ANT_SWITCH_DIV_CONFIG_0 ;
            ant_switch_grant = trm_ant_switch_div_set_config (new_settings );
#ifdef FEATURE_GERAN_DRDSDS
                        if(!gl1_trm_asdiv_get_traffic_crisis_flag(gas_id))
                        {
                gl1_trm_asdiv_traffic_denial_thresh_check(gas_id, ant_switch_grant);
                        }
#endif
            switch (ant_switch_grant)
            {
                case TRM_ANT_SWITCH_DIV_SET_SUCCESS:
                    MSG_GERAN_HIGH_0_G (" trm_ant_switch_div_set_config succees ");
                    if (gl1_hw_rfa_rf_gsm_set_antenna_req_cmd (FALSE ,gas_id))
                    {
            MSG_GERAN_HIGH_0_G (" RF set antenna 1  sucessfully ");
                        antenna_switch_burst[gas_id] = TRUE ;
                    }
                    else
                    {
            MSG_GERAN_HIGH_0_G (" RF set antenna 1 wrong ");
                    }
                    break;

            default :
              MSG_GERAN_LOW_0_G (" trm_ant_switch_div_set_config fail ");
              break;
          }
        break ;

      default :

        MSG_HIGH (" trm returns invalid current config  ",0,0,0);
        break;
      }
}

void gl1_trm_ant_switch_set_control ( trm_ant_switch_set_ctrl_enum control_req_type, gas_id_t gas_id )
{
  trm_ant_switch_set_ctrl_type ctrl_req_type ;
  grm_client_enum_t grm_client_id;

  gas_id = check_gas_id(gas_id);
  if (control_req_type == TRM_ANT_SWITCH_CTRL_RELEASE )
  {
   qsc_got_switch_control[gas_id] = FALSE ;
  }
  else if (control_req_type == TRM_ANT_SWITCH_CTRL_REQUEST )
  {
    qsc_got_switch_control[gas_id] = TRUE ;
  }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  grm_client_id = gl1_map_gas_id_to_client_id(gas_id);
  grm_client_id = check_asdiv_grm_client(grm_client_id);
  ctrl_req_type.client = grm_map_grm_client_id_to_trm_client_id(grm_client_id);

#ifdef FEATURE_GERAN_DRDSDS
    ctrl_req_type.allow_crisis_mode_switching = TRUE;
#endif
    
#else
ctrl_req_type.client = TRM_GSM1;
#endif /* ( FEATURE_DUAL_SIM ) ||  ( FEATURE_TRIPLE_SIM ) */
  ctrl_req_type.req_type = control_req_type ;
  trm_ant_switch_set_control (ctrl_req_type );
}


void gl1_trm_asd_set_mode_and_config (trm_client_mode_type    mode , trm_ant_switch_div_config_type  config ,gas_id_t gas_id )
{
  trm_set_mode_and_config_input_t  input_data ;
  grm_client_enum_t grm_client_id;

  gas_id = check_gas_id(gas_id);
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )  
  grm_client_id = gl1_map_gas_id_to_client_id(gas_id);
  grm_client_id = check_asdiv_grm_client(grm_client_id);
  input_data.client = grm_map_grm_client_id_to_trm_client_id(grm_client_id);
#else
  input_data.client = TRM_GSM1;
#endif /* ( FEATURE_DUAL_SIM ) ||  ( FEATURE_TRIPLE_SIM ) */
  input_data.mode   = mode ;
  input_data.config = config;
  /*if disable idle switch bit it set, always use the default position in idle mode*/
  if(gl1_antenna_switch_control_mask & DISABLE_IDLE_ANT_SWITCH)
  {
    input_data.config = TRM_ANT_SWITCH_DIV_CONFIG_DEFAULT;
  }
  input_data.set_mode_cb = (trm_asd_set_mode_and_config_cb_t)set_mode_and_config_cb ;

  input_data.ant_switch_cb = (trm_ant_switch_div_callback_t) gsm_ant_switch_cb;
  input_data.inform_switch_change = TRUE ;

  input_data.tag = ( trm_request_tag_t )gas_id ;
  trm_asd_set_mode_and_config ( &input_data );
}

trm_asd_set_mode_and_config_cb_t set_mode_and_config_cb (trm_asd_set_mode_and_config_type    set_mode_cfg_info )
{
  switch ( set_mode_cfg_info.set_mode_grant )
  {
  case TRM_SET_MODE_GRANTED :
    MSG_HIGH (" trm returns TRM_SET_MODE_GRANTED   ",0,0,0);
    break ;
  case TRM_SET_SWITCH_AND_CONTINUE:
    MSG_HIGH (" trm returns TRM_SET_SWITCH_AND_CONTINUE  ",0,0,0);
    break ;
  case TRM_SET_MODE_DENIED :
    MSG_HIGH (" trm returns TRM_SET_MODE_DENIED   ",0,0,0);
    break ;
  default :
    MSG_HIGH (" trm returns invalid  mode set   ",0,0,0);
    break ;
  }

  switch ( set_mode_cfg_info.set_config_grant )
  {
  case TRM_ANT_SWITCH_DIV_SET_SUCCESS :
    switch (set_mode_cfg_info.config )
    {
    case TRM_ANT_SWITCH_DIV_CONFIG_0:
      if (gl1_hw_rfa_rf_gsm_set_antenna_req_cmd (FALSE ,(gas_id_t)set_mode_cfg_info.tag ))
      {
        MSG_HIGH (" RF set antenna 1 sucessfully ",0,0,0);
      }
      else
      {
        MSG_HIGH (" RF set antenna 1 fail ",0,0,0);
      }
      break;

    case TRM_ANT_SWITCH_DIV_CONFIG_1:
      if (gl1_hw_rfa_rf_gsm_set_antenna_req_cmd (TRUE ,(gas_id_t)set_mode_cfg_info.tag ))
      {
        MSG_HIGH (" RF set antenna 2 sucessfully ",0,0,0);
      }
      else
      {
        MSG_HIGH (" RF set antenna 2 fail ",0,0,0);
      }
      break;

    case TRM_ANT_SWITCH_DIV_CONFIG_DEFAULT :
      if (gl1_hw_rfa_rf_gsm_set_antenna_req_cmd (FALSE ,(gas_id_t)set_mode_cfg_info.tag ))
      {
        MSG_HIGH (" RF set antenna 1 sucessfully ",0,0,0);
      }
      else
      {
        MSG_HIGH (" RF set antenna 1 fail ",0,0,0);
      }
      break;

    default :
      MSG_HIGH (" TRM return invalid config  ",0,0,0);
      break;
    }
    break;

  default :
    MSG_HIGH (" TRM return invalid set_config_grant  ",0,0,0);
    break;
  }
  return (trm_asd_set_mode_and_config_cb_t)NULL;
}
#endif /* FEATURE_GSM_QSC_TX_DIV */

#if defined( FEATURE_GSM_TX_DIV_ANTNA_SWITCHING) ||defined( FEATURE_GSM_QSC_TX_DIV)

void  gl1_rf_switch_complete_cb (rfa_rf_gsm_set_antenna_callback_s  *cb_data_ptr)
{
  trm_switch_complete_input_type trm_switch_complete ;
  grm_client_enum_t grm_client_id;
  
  gas_id_t gas_id = check_gas_id((gas_id_t)cb_data_ptr->sub_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )  
    grm_client_id = gl1_map_gas_id_to_client_id(gas_id);
    grm_client_id = check_asdiv_grm_client(grm_client_id);
    trm_switch_complete.client = grm_map_grm_client_id_to_trm_client_id(grm_client_id);
#else
    trm_switch_complete.client = TRM_GSM1;
#endif /* ( FEATURE_DUAL_SIM ) ||  ( FEATURE_TRIPLE_SIM ) */
    if (cb_data_ptr->ant_pos == 0)
    {
      trm_switch_complete.config = TRM_ANT_SWITCH_DIV_CONFIG_0;
      type2_alg_antenna_to_use [gas_id] = 1;
      antenna_to_use_for_tx [gas_id] = 1;
    }
    else
    {
      trm_switch_complete.config = TRM_ANT_SWITCH_DIV_CONFIG_1;
      type2_alg_antenna_to_use [gas_id] = 2;
      antenna_to_use_for_tx [gas_id] = 2;
    }
    
  if (cb_data_ptr-> req_result == RFA_RF_GSM_SUCCESS )
  { 
    trm_ant_switch_div_switch_complete (trm_switch_complete );
    passive_sub_switch_notification [gas_id] = FALSE ;
    clear_pending_asdiv_switch_request(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_ASDIV_SWITCH);
#endif
  }
  else if ( cb_data_ptr-> req_result == RFA_RF_GSM_EXECUTION_ERROR )
  {
    trm_ant_switch_div_switch_cancel (trm_switch_complete );
    passive_sub_switch_notification [gas_id] = FALSE ;
    clear_pending_asdiv_switch_request(gas_id);      
    MSG_HIGH (" antenna switch cancelled   in rf ",0,0,0);  
  }
  else
  {
    MSG_HIGH (" antenna switch failed  in rf ",0,0,0);
  }
  return ;
}

#endif  /*( FEATURE_GSM_TX_DIV_ANTNA_SWITCHING) ||defined( FEATURE_GSM_QSC_TX_DIV)*/

#if defined FEATURE_GSM_TO_WCDMA_MSGR

void l1_send_IRAT_G2W_INIT_REQ(
  sys_modem_device_mode_e_type  multi_sim_mode,
  gas_id_t                      gas_id)
{
  wl1_irat_x2w_init_req_struct_type g2w_init;
  errno_enum_type msgr_error;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  gl1_drx_task_active(gas_id);
  msgr_init_hdr_variant(&g2w_init.hdr,
                        MSGR_GERAN_GL1,
                        WCDMA_IRAT_G2W_INIT_REQ,
                        (uint8)as_id);

  g2w_init.multi_sim_mode = multi_sim_mode;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  msgr_error = msgr_send((msgr_hdr_struct_type *) &g2w_init,
                         sizeof(wl1_irat_x2w_init_req_struct_type));

  ASSERT(msgr_error == E_SUCCESS);

  MSG_GERAN_HIGH_0_G("G2W: Sent G2W_INIT_REQ");
}

void l1_send_IRAT_G2W_DEINIT_REQ(gas_id_t gas_id)
{
  wl1_irat_x2w_deinit_req_struct_type g2w_deinit;
  errno_enum_type msgr_error;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);
  
  gl1_drx_task_active(gas_id);
  msgr_init_hdr_variant(&g2w_deinit.hdr,
                        MSGR_GERAN_GL1,
                        WCDMA_IRAT_G2W_DEINIT_REQ,
                        (uint8)as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  msgr_error = msgr_send((msgr_hdr_struct_type *) &g2w_deinit,
                         sizeof(wl1_irat_x2w_deinit_req_struct_type));
  ASSERT(msgr_error == E_SUCCESS);
  MSG_GERAN_HIGH_0_G("G2W: Sent G2W_SRCH_DEINIT_REQ");
}

void l1_send_IRAT_G2W_RF_ENTER_IND(
  rfm_meas_common_param_type *rf_params,
  uint16                     w_freq,
  lm_handle_type             w_rxlm_buf_idx,
  rfm_device_enum_type       w_rf_device,
  gas_id_t                   gas_id)
{
  wl1_irat_x2w_rf_enter_ind_struct_type rf_enter_ind;
  errno_enum_type msgr_error;

  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);
  
  msgr_init_hdr_variant(&rf_enter_ind.hdr,
                        MSGR_GERAN_GL1,
                        WCDMA_IRAT_G2W_RF_ENTER_IND,
                        (uint8)as_id);

  rf_enter_ind.rf_params      = rf_params;
  rf_enter_ind.w_freq         = w_freq;
  rf_enter_ind.w_rxlm_buf_idx = w_rxlm_buf_idx;
  rf_enter_ind.w_rf_device    = w_rf_device;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  msgr_error = msgr_send((msgr_hdr_struct_type *) &rf_enter_ind,
                         sizeof(wl1_irat_x2w_rf_enter_ind_struct_type));

  ASSERT(msgr_error == E_SUCCESS);

  MSG_GERAN_HIGH_0_G("G2W: Sent G2W_RF_ENTER_IND");
}

void l1_send_IRAT_G2W_RF_EXIT_REQ(
  rfm_meas_common_param_type *rf_params,
  boolean                    gap_aborted,
  gas_id_t                   gas_id)
{
  wl1_irat_x2w_rf_exit_req_struct_type rf_exit_ind;
  errno_enum_type msgr_error;

  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);
  
  msgr_init_hdr_variant(&rf_exit_ind.hdr,
                        MSGR_GERAN_GL1,
                        WCDMA_IRAT_G2W_RF_EXIT_REQ,
                        (uint8)as_id);

  rf_exit_ind.rf_params   = rf_params;
  rf_exit_ind.gap_aborted = gap_aborted;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  msgr_error = msgr_send((msgr_hdr_struct_type *) &rf_exit_ind,
                         sizeof(wl1_irat_x2w_rf_exit_req_struct_type));

  ASSERT(msgr_error == E_SUCCESS);

  MSG_GERAN_HIGH_0_G("G2W: Sent G2W_RF_EXIT_REQ");
}

void l1_send_IRAT_G2W_PRE_SRCH_REQ(
  rfm_meas_common_param_type *rf_params,
  gas_id_t                   gas_id)
{
  wl1_irat_x2w_pre_srch_req_struct_type pre_srch_req;
  errno_enum_type msgr_error;

  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);
  
  msgr_init_hdr_variant(&pre_srch_req.hdr,
                        MSGR_GERAN_GL1,
                        WCDMA_IRAT_G2W_PRE_SRCH_REQ,
                        (uint8)as_id);
  
  pre_srch_req.in_qta_gap = gl1_hw_qta_gap_active(gas_id);
  pre_srch_req.rf_params = rf_params;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  msgr_error = msgr_send((msgr_hdr_struct_type *) &pre_srch_req,
                         sizeof(wl1_irat_x2w_pre_srch_req_struct_type));

  ASSERT(msgr_error == E_SUCCESS);

  MSG_GERAN_HIGH_0_G("G2W: Sent G2W_PRE_SRCH_REQ");
}

void L1_send_IRAT_G2W_SRCH_REQ(
  wl1_x2w_mode_enum_type             mode,
  uint16                             uarfcn,
  wl1_x2w_srch_input_union_type      *search,
  wl1_x2w_startup_params_struct_type *startup_params,
  rfm_meas_common_param_type         *rf_params,
  gas_id_t                           gas_id)
{
  errno_enum_type       msgr_error;
  wl1_irat_x2w_srch_req_struct_type  g2w_srch;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  /* check input pointers.  "search" can be NULL with id_search since id
     search doesn't take any cell parameters */
  if(!startup_params ||
     (!search && mode != WL1_X2W_IDLE_ID_SRCH))
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_FATAL("passed in null pointer parameters", 0, 0, 0);
  }
  
  msgr_init_hdr_variant(&g2w_srch.hdr,
                        MSGR_GERAN_GL1,
                        WCDMA_IRAT_G2W_SRCH_REQ,
                        (uint8)as_id);
  
  g2w_srch.mode = mode;
  g2w_srch.uarfcn = uarfcn;
  g2w_srch.rf_params = rf_params;

  if(mode != WL1_X2W_IDLE_ID_SRCH)
  {
    memscpy(&g2w_srch.srch, 
            sizeof(g2w_srch.srch), 
            search, 
            sizeof(wl1_x2w_srch_input_union_type));
  }

  memscpy(&g2w_srch.startup_params,
          sizeof(g2w_srch.startup_params),
          startup_params,
          sizeof(wl1_x2w_startup_params_struct_type));
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  msgr_error = msgr_send((msgr_hdr_struct_type *) &g2w_srch,
                         sizeof(wl1_irat_x2w_srch_req_struct_type));
  
  ASSERT(msgr_error == E_SUCCESS);
  
  MSG_GERAN_HIGH_0_G("G2W: Sent G2W_SRCH_REQ");
}


void L1_send_IRAT_G2W_ABORT_REQ(gas_id_t gas_id)
{
  wl1_irat_x2w_abort_req_struct_type g2w_srch_abort;
  errno_enum_type msgr_error;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  msgr_init_hdr_variant(&g2w_srch_abort.hdr,
                        MSGR_GERAN_GL1,
                        WCDMA_IRAT_G2W_ABORT_REQ,
                        (uint8)as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER  
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  msgr_error = msgr_send((msgr_hdr_struct_type *) &g2w_srch_abort,
                        sizeof(wl1_irat_x2w_abort_req_struct_type));
  
  ASSERT(msgr_error == E_SUCCESS);
  
  MSG_GERAN_HIGH_0_G("G2W: sent G2W_SRCH_ABORT_REQ");
}

#endif /*FEATURE_GSM_TO_WCDMA_MSGR*/

/*===========================================================================

FUNCTION l1_send_qta_startup_or_cleanup_msg

DESCRIPTION

  This function sends GERAN_GL1_QTA_STARTUP_REQ or GERAN_GL1_QTA_CLEANUP_REQ to msgr 
  interface to handle QTA gaps
  
Parameters
  qta_info_p:
    pointer to QTA event callback

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void l1_send_qta_startup_or_cleanup_msg(trm_qta_event_callback_data *qta_info_p, gas_id_t gas_id)
{
  uint8       i;
  msgr_umid_type geran_gl1_qta_msg_umid = NULL;
  errno_enum_type  msgr_error;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);
  
  switch(qta_info_p->operation)
  {
    case TRM_START_STATE:
      {
        geran_gl1_qta_msg_umid = GERAN_GL1_QTA_STARTUP_REQ;

        break;
      }
    case TRM_END_STATE:
      {
         /* For en empty qta gap for G2L search */
         if(l1_sc_g2l_empty_qta_gap[gas_id])
         {
            uint8 abort_resp_cnt = 0;
            l1_sc_g2l_empty_qta_gap[gas_id] = FALSE;
            L1_send_LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ(gas_id);
            
            while (g2l_get_abort_cnf_pending_status(gas_id) || l1_sc_g2l_build_script_pending[gas_id])
            {
               gl1_hw_delay(100, gas_id);
               if(abort_resp_cnt > 50)
               {
                 /* 50*100 us = 5 msec ~= 1 frame */
#ifdef FEATURE_QSH_MDUMP
                 QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
                 MSG_GERAN_FATAL_1_G("NO reponse of G2L abort cnf in 1 frame =%d",abort_resp_cnt);
               }
               else
               {
                 abort_resp_cnt++;
               }
            }
         }   
         geran_gl1_qta_msg_umid = GERAN_GL1_QTA_CLEANUP_REQ;

        break;
      }
    default:
      {
        MSG_GERAN_ERROR_1_G("l1_send_qta_msg: no valid operation %d",
                                      qta_info_p->operation);
        break;
      }
  }

  if((TRM_START_STATE == qta_info_p->operation) || (TRM_END_STATE == qta_info_p->operation))
  {
    geran_gl1_qta_msg_type geran_gl1_qta_startup_or_cleanup;

    msgr_init_hdr_variant(&geran_gl1_qta_startup_or_cleanup.hdr,
                          MSGR_GERAN_GL1,
                          geran_gl1_qta_msg_umid,
                          (uint8)as_id);

    geran_gl1_qta_startup_or_cleanup.as_id = as_id;

    geran_gl1_qta_startup_or_cleanup.operation      = qta_info_p->operation;
    geran_gl1_qta_startup_or_cleanup.source_client  = qta_info_p->source_client;
    geran_gl1_qta_startup_or_cleanup.rf_params      = qta_info_p->rf_params;
    geran_gl1_qta_startup_or_cleanup.qta_result     = qta_info_p->qta_result;

    for(i=0;i<TRM_NUM_PRI_DIV_CLIENTS_PER_TECH;i++)
    {
      geran_gl1_qta_startup_or_cleanup.dev_allocated[i] = qta_info_p->dev_allocated[i];
    }

    msgr_error = msgr_send( (msgr_hdr_struct_type *) &geran_gl1_qta_startup_or_cleanup , sizeof(geran_gl1_qta_msg_type) );

    ASSERT(msgr_error == E_SUCCESS);

    MSG_GERAN_HIGH_2_G("l1_send_qta_msg: operation: %d um_id: %x", qta_info_p->operation , geran_gl1_qta_msg_umid );

  }
}

/*===========================================================================

FUNCTION gl1_check_if_g2t_init_pend

DESCRIPTION

Check if no init req has been sent to TD and also TD IRAT is not active
  
Parameters
NA

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#ifdef  FEATURE_GSM_TDS
boolean gl1_check_if_g2t_init_pend(gas_id_t gas_id)
{
 l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

 MSG_GERAN_HIGH_2_G("Check pend init irat %d init_sent %d",l1_sc_wcdma_data_ptr->tds_irat_active,G2TDS_INIT_sent);
 
 /*If no Init is issued to TD, it means that MCPM is still processing G2T MEAS START req*/
 if((G2TDS_INIT_sent == FALSE) && (l1_sc_wcdma_data_ptr->tds_irat_active == FALSE) && (g2t_stop_mode_cnf_pending == FALSE))
 {
   return TRUE;
 }
 else
 {
   return FALSE;
 }
}
#endif

/*===========================================================================

FUNCTION gl1_check_if_g2l_init_pend

DESCRIPTION

Check if no init req has been sent to LTE and also LTE IRAT is not active
  
Parameters
NA

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#ifdef  FEATURE_GSM_TO_LTE
boolean gl1_check_if_g2l_init_pend(gas_id_t gas_id)
{

 MSG_GERAN_HIGH_1_G("Check pend init_sent %d",G2L_INIT_sent[gas_id]);
 
 /*If G2L init sent is not init_Sent or ready or Deinit ongoing, it means, GL1 is still processing G2L mcpm start req*/
 if(((G2L_INIT_sent[gas_id] & LTE_G2L_READY)  !=  LTE_G2L_INIT_SENT) && ((G2L_INIT_sent[gas_id] & LTE_G2L_MASK) != LTE_G2L_READY)
    &&((G2L_INIT_sent[gas_id] & LTE_G2L_DEINIT_SENT)  !=  LTE_G2L_DEINIT_SENT))
 {
   return TRUE;
 }
 else
 {
   return FALSE;
 }
}

/*===========================================================================

FUNCTION l1_send_coll_msg_to_rr

DESCRIPTION

  This function sends MPH_IDLE_COLL_FALLBACK_IND  RR.
  
Parameters
  idle_coll_event_info:
    pointer to persistent collision event callback

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void l1_send_coll_msg_to_rr(
        trm_idle_coll_event_data  *idle_coll_event_info,
        gas_id_t gas_id)
{
  mph_idle_coll_fallback_ind_T  mph_idle_coll_fallback_ind;
  memset( &mph_idle_coll_fallback_ind, 0, sizeof(mph_idle_coll_fallback_ind_T));
  
  PUT_IMH_LEN( sizeof ( mph_idle_coll_fallback_ind_T ) - sizeof ( IMH_T ), &mph_idle_coll_fallback_ind );

  mph_idle_coll_fallback_ind.message_header.message_set = MS_RR_L1;
  mph_idle_coll_fallback_ind.message_header.message_id  = MPH_IDLE_COLL_FALLBACK_IND;

  mph_idle_coll_fallback_ind.gas_id = gas_id;
  mph_idle_coll_fallback_ind.action =  idle_coll_event_info->action_info.action;
  mph_idle_coll_fallback_ind.fallback_pref = idle_coll_event_info->data.idle_fallback_data.fallback_type;
  mph_idle_coll_fallback_ind.least_priority_rat = idle_coll_event_info->data.idle_fallback_data.least_priority_rat;

  MSG_GERAN_HIGH_3_G("action = %d fallback_pref = %d least_priority_rat = %d",
    mph_idle_coll_fallback_ind.action,
    mph_idle_coll_fallback_ind.fallback_pref,
    mph_idle_coll_fallback_ind.least_priority_rat
  );
  
  if( GS_SUCCESS != L1_isr_send_message( GS_QUEUE_RR, &mph_idle_coll_fallback_ind, TRUE, gas_id) )
  {
    MSG_GERAN_ERROR_0_G(" Failed to send MPH_IDLE_COLL_FALLBACK_IND ");
  }
}

#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

/* EoF */
