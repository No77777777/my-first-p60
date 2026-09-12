#ifndef GL1_HW_G_H
#define GL1_HW_G_H
/*===========================================================================
                      GSM GPRS NPL1 FRAME LAYER HEADER FILE
DESCRIPTION
   This file defines the interfaces to the GSM/GPRS NPL1 FRAME LAYER.
   The GSM/GPRS NPL1 frame layer abstracts all hardware concepts. The frame
   layer functions typically need to be called periodically every frame.

   The implementation for the functions are distributed in several files.
   gl1_hw.c           - File handling common functionality.
   gl1_hw_gsm.c       - File handling GSM/GPRS frame layer functionality
   gl1_hw_gprs.c      - File handling GPRS functionality.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/inc/gl1_hw_g.h#7 $
$DateTime: 2020/07/28 11:01:41 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
24/07/20   sal      CR2736787 Added G2X cleanup done handler to issue rf_meas_exit immediately on cleanup confirmation.
05/06/20   sc       CR2703314 TDS Removal on Agatti- Moving G2T/T2G code under TDS feature
04/02/20   nv		CR2615257 Reverting the hack made to handle L1_TIME_LINE_DELAY scenario in HA as part of FR:51554
27/01/20    rks     CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
18/01/16    zf      CR961805: Set first burst flag in x2g_startup cmd
25/04/18   rv       CR2225983 - GERAN F3 and log pkt Reduction TA.3.0
06/04/18    akm     CR2216412 Moving G2X de-init in QTA cleanup
09/11/17    ng      CR2140861 Do not allow Non Irat activity in QTA gap opened for IRAT activity
11/08/16   sk       CR1048655  FW is ordered to read RF Alpha values from RF without which RSSI was 
                    getting floored leading to xCCH failure, if it's first QTA after Stack activation
10/08/16    cws     CR1044687 Use ARFCN from pwr sm directly for pwr monitor log packet
30/08/16    tsk     CR1058305: Update to 0x5096 RxD log packet with additional details.
25/01/16    cws     CR962379 Disable/enable ASDIV in IDLE/RACH state by NV
19/09/16    cws     CR1067531 Use turn tx off API which checks Tx status before sending Tx sleep req to RF
08/12/15    br      CR 955168 GL1 to disable TX during rach to find bcch state transition 
15/12/16   ksb      CR1041734 Error recovery handing at regular interval using NV control
15/12/17   km       CR1049586 Send G2X cleanup through ASYNC to avoid executing before G2X startup
15/12/17    ap      CR1103567 FR36133: Extended LTE Signaling (ELS) to manage tune-away gaps in DSDS 
28/02/17    og      CR2006578. FRs 39363 and 39364.
29/10/17    km      CR2133917 GL1 support for TA.3.0 L+L
06/07/17    mk      CR2071218 FR44121 -Synchronous Ncell Quality Monitor
02/02/17    br      CR1103497 FR 39735: Allowing TA during PAGE RE-ORG mode in GERAN
14/02/17   mk       CR2004610 : FR 40251 (SeNS) - Add NV item to enable SeNS feature
03/01/16   mn       CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
15/05/17   akm      CR2047050 Avoid GL1 going to sleep during rude wakeup if RF wakeup CNF is not received
08/12/16    mk      CR1097591 Enhancement for DRX: DRx and PRx difference will be applied to neighbor cell measurements, if DRx power level is greater than PRx
16/12/16   sk       CR1100657  Send GERAN_FW_RFM_ENTER_MODE_CMD only for PRX if wakeup Cnf has come only for it
23/08/16    bg      CR1045621 Updating Rx Div Ant parameters in Engineering mode
20/10/15    br      CR926522 GL1 to Pass the frequency offset to rotator to correct in GBTA.
12/05/16    rv      CR998395: Excessive F3 logging in  GERAN L1
07/03/15   mko      CR981074: Read the GCF NV first then try to register NPA callback based on GCF NV settings 
08/01/16    cws     CR949517 Force asdiv position when requested by RF at certain SAR stateNew 
08/02/16    mn      CR872002 GL1 changes for VSWR feature 
18/11/15    cja     CR938089 Use XtoG RF timing for RX in QTA gap
09/11/15    og      CR929332. Don't do immediate logging of power monitors
                    when suspending GSM L1.
11/05/15    cc      CR931979 Move EFS read to l1 task init
03/11/15    sp      CR916957: Update CR909073, log with 0's instead of no logging when yield
22/10/15    zf      CR923858 Rework Sleep error recovery in QTA 
02/11/15    SS      CR933903 Fix GL1 Compiler Warnings
06/10/15    cja     CR918431 If QTA startup after GSM start/resume initialise MDSP
29/09/15    zf      CR914601 Crash when L1 on sleep without unload firmware
28/09/15    aga     CR911414 During QTA RF init send a START GSM RF if needed 
31/08/15    mko     CR896740 sleep error recovery changes in QTA gap
03/09/15    kb      CR863908 Added new EFS file structure for sleep control
26/08/15    npt     CR892374 Generate monitor log when leaving GERAN  
19/08/15    mc      CR874662 CXM Freq ID support for X2G startup and X2G preload
12/08/15    jk      CR882186 Don't request DRx device if in warm up ISR,LMM and SAIC is disabled
03/08/15    jj      CR879940 disable_power_opt15 for W2G
14/08/14    ws      CR889273 Fix Static declarations of variables which are not Dual data spaced
12/08/15    cws     CR864883 FR27546+FR27544 Add MTPL/power imbalance factors into ASDIV algorithm
11/08/15    fan/akh CR871795 FR19329: Avoid cancelled CCCH booking after rude wakeup
12/05/15    jk      CR883632:SAWless support info needs to be handled in RF start GSM CNF message
20/07/15    shr     CR866607 Observing yield/proceed on both subs when in collision during data transfer on one sub
04/08/15    aga     CR880762 Remove QTA deprecated APIs code
29/07/15   dp/dv    CR878618: Changes to wait for rf exit confirmation in gl1 for X2W srch
30/07/15    fan/akh CR880067 FR19329: Disable support for non-CCCH activity cancellation
23/07/15    fan/akh CR865360 Use correct TRM reservation period when doing wakeup cancelation
23/07/15    akh     CR876576 Avoid TRM adjustment to include warmup when already included
21/07/15    pg      CR871012 Apply FTM diversity band support capabilities passed by GRF for cal
20/07/15    pa      CR875967:Disable Data Rxd on Multimode sub if other sub is non3pp capable.
15/07/15    fan/akh CR857332 FR19329: WMGR support for rescheduling non-PCH activities
18/05/15    dg      CR838227 Remove frame_slaming_flag as block level checks are already there
                    to ensure no G2X irat is triggered if TS adjustment is pending
14/07/15    cws     CR845909 Switch antenna when AGCH decode fails
04/06/15    jk      CR846952:RxLEV report should be based on maximum of PRx and DRx average RSSIs
12/05/15    mko     CR831541: G21x Tuneaway Optimization - during T3168/T3190 & PDAN G21x TA allowed with NV control
03/07/15    jj      CR  865562 Remove featurisation introduced for CR839611
29/06/15    shr     CR860767: Add XCCH Debug Capability in DR-DSDS using NV control
22/05/15    ws      CR790493 Check BSIC decoded in SCH matches requested BSIC from RR
01/06/15    js      CR814275 GL1 changes for ROTA interface during extended paging
18/05/15    jj      CR 839611 New skip last measurement in T2G if command is late 
17/06/15    aga     CR855583 Allocate RxLM buffer in the QTA start-up callback if invalid  
11/06/15    zf      CR847878: Using PRE_SRCH_REQ and CNF for G2W
12/06/15    js      CR850684 Change QBTA offset for X2G startup
11/06/15    pg      CR850142 Featurise dedicated parallel SCH clock bump to avoid unnecessary MCPM/NPA calls
02/06/15    cja     CR844096 RF meas gsm parameters to use xtoy param type
02/06/15    pg      CR846507 Allow non-signaling RF devices to be controlled by FTM 
29/05/15    pa      CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD.
21/05/15    cja     CR839948 For QTA gap build preload scripts in QTA startup, send to GFW in cm startup
22/05/15    pg      CR841558 In FTM, use DRx device passed by GRF (if valid), rather than via TRM request 
20/05/15    ar      CR841232: Follow-up for CR834975 Removing an extern variable that is no longer needed.
07/05/15    zf      CR832962: Issuing QTA_startup command at the frame QTA gap starts.
20/04/15    sjv     CR823566 Set number of TRM denials/Xcch failures count through debug NV 
15/04/15    pa      CR822841: Split Rx/Tx GL1 TRM changes for Single SIM.
08/05/15   ar       CR834975 Double the RF tuning parameters in the GL1 for W+W  
07/05/15   mc       CR830988: COEX Channel ID selection for X2G IRAT measurements does not support TRM_UMTS2
07/05/15   fan/akh  CR834237 FR19329 Full G wake-up cancellation + code cleanup
05/07/15    zc      CR826075 Band Specific ASDIV Enable
07/05/15    npt     CR821672 GERAN power logging update  
06/05/15   aga      CR833610 dual data space gl1_wcdma_rxlm_buf_idx and gl1_hw_g2w_rf_enter_called_last
24/04/15   cja      CR800657 QTA gap for G2W IRAT
23/04/15    cs      CR827583 Remove multiple debug control bits using same value
20/04/15    ip      CR811868 Dual Multisim mode code changes + debug overhaul
16/04/15   smd      CR822202 Dual space data for IRAT and QTA
09/04/15   jj/dp    CR819956 Thor 2.x RF API change, G2W use MSGR
08/04/15    pjr     CR818535 FR25951 Changes to support Single Voice Session 
31/03/15    pa      CR815621:GRM restructuring for TH 2.0. 
30/03/15    nm      CR765891 Allowing TA for TDS during PS RACH
23/03/15    pg      CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP  
17/03/15    cs      CR804196 Use new passthrough versioned FCCH/SCH log packet
12/03/15    pa      CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
11/03/15    pjr     CR806250 Fix featurisation for changes submitted under CR787199
11/03/15    cs/tw   CR805703 Fix QTF builds with included array definition
27/02/15    ab      CR750511: GSM Timeline reporting for WLAN COEX
24/02/15    pjr     CR787199 Acquire vocoder immediately after IMS releases it during srvcc handover
05/12/14    sp      CR767083: Override paging multiframe to 2, for PDT tests
27/02/15    cja     CR768169 Add RF Seq Number
18/02/15    zc      CR795410: Support asdiv type2 during data
20/02/15    js      CR693771: W+T removal. Resolving compilation errors
20/01/15    zf      CR772571: Build G2W RF scripts one frame earlier
18/02/15    am      CR775652 Mismatch in monitor reading
18/02/15    am      CR777094 Keep mon write and read logging-index aligned after mon is aborted
11/02/15    gk      CR710069 Make gl1_qta_wcdma_has_initialised dual data spaced
04/02/15    cja     CR789095 Store X2G acquisition tags to use for SCH results
04/02/15    sjv     CR790894: Disable DR->SR Fallback using mask set for NV 65770 (l1_debug)
26/01/15    pa      CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs.
28/01/15    cja     CR767058 T2G using parallel FCCH/SCH
26/01/15    cja     CR780982 For CM SCH, get metrics for correct burst
21/01/15    pa      CR758792:RxD support for multi-sim.
21/01/15    jk      CR782488:Use updated gain tags for PRx and DRx in DTM mode
07/01/15    jj      CR778334 Don't send set tx band when hw tx off
07/01/15    jj      CR 766904 reset the type 1 asdiv algorithm during transition from dedicated to packet
22/12/14    pa      CR773689: Set RF device to Invalid on RF exit.
12/12/14    cs      CR754853 Add support for Parallel SCH detection handling
16/12/14    jk      CR753423:LIF for Idle,voice and data
15/12/14    pg      CR630281 implementation of RxD support for data/MSRD
12/12/14    cja     CR752567 Always send set tx band after Tx enable cnf
12/12/14    pa      CR763859:Handling of RxD clients in GL1 TRM structures.
11/12/14    ak      CR739561 SCE opt top2/top6 mode, disable pwr msr of blacklisted cells
02/12/14    df      CR758358 Change EFS file bit to disable adaptive timeline
17/11/14    npt     CR642426 Added power reporting log packet
01/10/14    zc      CR707083: FR 22623: ASDiv (+tuner) for TDD-LTE/TDS/G+G DR-DSDS w/ TDS/G CSFB
13/11/14    pa      CR755557:Support for granted band/rf device in Idle mode RxD.
01/11/14    df      CR748602 Rework and enable adaptive timeline
07/11/14    cja     CR751049 Add cm FCCH and SCH results API (ready for parallel FCCH/SCH)
31/10/14    xz      CR748816 Extend dual data space for building RF script variable
10/11/14    zc      CR748647 ASDIV Type1 Switching Test Mode API
23/09/14    jj      CR 728792  Discrete Fourier transform (DFT) spur metric log packet
30/10/14    cs      CR701894 Enhanced Page Detection Support
21/10/14    fan     CR734490 Rework the wake up recovery mechanism
17/10/14    ssh     CR737668: Reset ISR overrun condition if leaving or entering idle after rude wake up
14/10/14    df      CR676084 Sleep debug structure improvements
10/10/14    cos     CR736560 BO 2.x Idle RxD mirror combined exit/sleep requests according to the enter/wakeup modes
07/10/14    sjv     CR734254  G2L rf devices retrieval for primary and diversity
03/10/14    aga     CR732455 Changes required to tune away framework for TH 1.0 single sim build
01/10/14    df      CR611845 SBD interface updates for missed wakeup
24/09/14    cja     CR726441 In QTA wait for RF startup scripts to built before sending X2G startup
22/09/14    cgc     CR727137 LTE api additions add gl1_hw_get_lte_irat_rf_device()
06/06/14    jk      CR668058:Adding EPD Log pakcet
09/09/14    jk      CR697308:GL1 changes to support HLin LLin sawless feature
12/09/14    nm      CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
09/09/14    cos     CR720965 adding support for DR-DSDS - rfdev 2 and 3 handling wtr to RxD and calibration
08/09/14    cja     CR715411 Add support of MPH_DEEP_SLEEP_IND
28/08/14    am      CR716841 enable device id in enter mode command as FW is supporting now
22/08/14    cgc     CR713634 obsoleted G2L code and featurisations FEATURE_GSM_G2L_TIMED_SRCH and FEATURE_CM_G2L removed
07/08/14    cja     CR643648 Desense for channel adjacent to spur
23/04/14    jj      CR 653796 function  cannot called inside ASSERT
11/02/14    jk      CR614254:GL1 changes to support IP2 calibration
06/08/14    ka      CR699964 Implement Idle RXD support in GL1
06/08/14    df      CR590772 - Use of NV items to control sleep and features
06/08/14    df      CR598015 Avoid silent frame DRX processing when pending firmware activity trigger is set
06/08/14    df      CR536059 Allow Asynchronous sleep for all idle activities
31/07/14    cos     CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
30/07/14    ssh     CR701245: Avoid running an adjoining activity after rude wake up if the FW/RF is being initialized in that frame
28/07/14    ssh     CR700687: Handle X2G start up from warm up ISR
24/07/14    jk      CR699271:GL1 LIF changes for RxD
28/07/14    cja     CR697957 For QTA startup run build scripts in DPC task
27/07/14    ap      CR678067: Race condition between APP mode as ACTIVE and Device mode change as DSDA leads to crash
21/07/14    jk      CR660801:Support for logging VAMOS log packet 0x5099
16/07/14    jk      CR695230:Reset enable_rxdiversity variable while exiting DRx mode
27/06/14    cos     CR686719: Cleaning up AEQPH4 GL1 control info at call exit
18/06/14    cos     CR682031 - GL1 control algorithm and changes for supporting AEQ PHASE 4
18/07/14    pg/pa   CR694601:GERAN L1 support for dedicated WTR hopping with FED architecture
10/07/14    js      CR593074- ROTA/QBTA Feature implementation
14/07/14    cja     CR692358 Ensure RxDiv RxLM buffer de-allocated.
10/03/14    ka      CR628921 Use LTE earfcn type in gl1 to allow move to 32bit earfcn's
02/07/14    cos     CR 688739 - Residual BER in RxD mode when PRx is disconnected
02/07/14    mc      CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
10/06/14    jj      CR 668965 merge ASDIV from DI.3.0 to BO 2.0
25/06/14    cs      Remove FEATURE_ASYMMETRIC_GERAN_BANDS
30/05/14    pg      CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM
18/06/14    ssh     CR598637: Missing Merge
19/06/14    cs/ch   CR651795 TRM Updated changes for DR-DSDS
12/06/14    ssh     CR671029: On G+G wake up collision defer wake up by 1 frame
30/05/14    ssh     CR673304: Allow QTA gap on multimode sub for SGLTE + G DSDA scenario
21/05/14    ssh     CR668643: gl1_hw_qta_gap_active() should return FALSE for multimode sub
14/05/14    cja     CR664278 For T2G share preloaded scripts between IRAT and QTA.
29/04/14    pjr     CR639777 Configure TX chain Id correctly after connected wtr hopping
29/04/14    pjr     CR633698 Dedicated WTR Hop Bring Up changes
29/04/14    pjr     CR600612 Dedicated WTR Hop initial code drop
09/06/14    cja     CR670351 For QTA empty gap wait for FW preload before aborting RF scripts
14/05/14    cja     CR664278 For T2G share preloaded scripts between IRAT and QTA.
12/05/14    cja     CR662836 Add support for qta_cleanup_pending to Bolt.
08/05/14    sp      CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
12/05/14    zc      CR660275: Do not set asdiv wakeup config unless granted TRM
02/05/14    zf      CR640262: Need to call RF enter mode after multi SIM init
28/04/14    cja     CR656165 Add external API for CM gap complete
17/04/14    cos     CR636543 - Changes for enabling Rx Diversity on Bolt - GL1/GRF intf updates
17/04/14    cos     CR646420 Changes for enabling Rx Diversity on Bolt - GL1/GFW intf updates
17/04/14    jk      CR649527:handling frequency hopping and DTX with Shifted SACCH for all cases
17/04/14    jj      CR 650872 send switch complete based on  call back from rf in pch crc fail
28/03/14    zc      CR640583: QTA ASDIV sleep/wakeup transition fix
11/04/14    cja     CR644441 Bolt: Preload script for QTA
09/04/14    jk      CR645719:use correct gas_id to retrieve rxlm buffer index while VregOff request
07/04/14    cja     CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
04/04/14    pg      CR643846 RFLM/FED architecture support for multi-SIM
12/02/14    jj      CR 596556 Returning switch permission to LTE when GSM is in OOS in SGLTE mode
01/04/14   xz       CR634903 Clear maintain_tds_ref_count timely if it is not forewakeup
19/03/14   cja      CR632037 Use RF Task for X2G
13/03/14    jj      CR  629747 DSDS: register inactive mode  before GL1 goes inactive
17/03/14    jk      CR629569:Incorrect gain values used in DTM
05/03/14    jj      CR 610751  dime 3.0 dsda Asdiv
25/02/14    cos     CR608951 GERAN RFLM FED: Pass subscription ID to GERAN RF in all message router payloads
19/02/14    sk      CR602282 Ensure Priority inversion applied after the deferred wake up
19/02/14    jk      CR608290:GERAN RxD ON/OFF adaptive algorithm
10/02/14    cja     CR612218 For G2L add meas enter and meas exit for LTE RF
04/02/14    cja     CR611621 Do complete CM terminate before gstmr init
02/01/14    jk      CR595130: GL1 changes to support VAMOS-II
09/01/14    zf      CR598637: DRX only updates TRM booking if DRX activity is matching with the TRM booking
07/02/14    nk      CR576899 GBTA feature is not working during DSDA to DSDS fallback
30/01/14    mc      CR609473 SGLTE COEX : GAS needs to register with COEX management
21/01/14   mko      CR596574: Reset HW schedule events when TBF is suspended for G2W TA
02/12/14    js      CR595142 :  T+G compilation errors removed
31/12/13    xz      CR594706 record more status for compressed mode
12/12/13    ssh     CR588188: ISR overrun recovery mechanism
07/12/13    js      CR586603 GSM has not called Enter mode
21/01/14   cgc      CR602010 TR2.0 merge fix,prevent WTOG BPLMN deactivate
20/01/14    jk      CR503359 - GL1 changes to support Rx diversity
19/01/14    dv      CR602224 QTA crash because TRM lock released after wakeup from sleep
14/01/14    ssh     CR597264:GSM uses RF API to choose to use RFM enter mode or VREG+WAKEUP.
12/01/14    jk      CR572347 - GL1 FTM API changes to support Rx diversity
08/01/14    us      CR562238 Partial T2G gap usage for RSSI measurement
02/01/13    dv      CR592228 RxAGC flooring after few QTAs
24/12/13    ap      CR592015: Check WTR IRAT capability before allowing IRAT measurements
04/12/13    zf      CR572394: Vote RFHAL bus without TRM lock
29/11/13    ssh     CR583144: On Rude wakeup make sure pending mcpm sleep req is done before initiating a mcpm wake up req.
29/11/13    sp      CR583330:Revert CR553516
26/11/13    dv      CR580909 Ensure of a 'empty' QTA gap the pending cleanup flag is set
22/11/13    sk      CR576246 Ensure Clenup scripts status not cleared while populating startup scripts inside the QTA gap
09/11/13    mc      CR572746 : COEX : DSDA G+G VBATT power back-offs for TxTx collisions are not correctly applied
19/11/13    ssh/ip  CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
15/11/13    nm      CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
09/24/13    sc      CR538584 T2G Statistical runtime histogram for performance evaluation
27/11/13    cja     CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
22/11/13    cja     CR455527 Remove featurisation from files in API directory
05/11/13    cja     CR570704 Add T2G RF pre-building startup/cleanup scripts
01/11/13    pa      CR569692: X2G via RF Task.
09/24/13    sc      CR538584 T2G Statistical runtime histogram for performance evaluation
02/11/12    jj      CR 402579 Support of Tx Diversity in GERAN.
14/10/13    pa      CR539505:Bolt: Added support for Dynamic Power Scheduling
08/10/13    ssh     CR554677 - XO shutdown broken in G for TR 1.1
07/10/13    gk      CR553516 Added non blocking wait for MCPM start
14/09/13    ap/aga  CR545423 - G2W Tune Away Feature
04/09/13    cs      CR532298 Integrate all wakeup-mgr changes for Multi-SIM
03/09/13    npt     CR524770 - GL1 changes to support RFLM FED
19/08/13    fan     CR530376 -  GL1 starts sleep before slpc OLS serviced, so that slpc can not configure sleep duration and GL1 will not wakeup
15/08/13    sk      CR524039 GBTA bringup changes
13/08/13    fan     CR521324 -  Do not use the OLS sleep controller callback to reprogram the terminal count
14/06/13    pjr     CR490812 added function gl1_hw_get_rf_gsm_warmup_time
23/07/13    ss      CR517408 Check stop_acq_pending while issuing X2G_STOP_ACQ in acq ft handler
05/08/13    cja     CR511042 Separate RF ID for active and CM.
05/08/13    cja     CR498800 Use TRM to get IRAT RF ID
05/08/13    cja     CR497487 New RF IRAT API to add RF ID in G2T.
02/08/13    cs      CR522369 Include residual XO drift handling in CR509747
29/07/13    sk      CR519663 Partial QBTA bringup changes
12/07/13    cs      CR509747 Apply frequency drift to TRM_DENIED SUB(s)
11/07/13    cja     CR511042 Separate RF ID for active and CM.
04/07/13    cgc     CR498800 Add gl1_hw_cm_trm_init()
20/06/13    cs      Major Triton TSTS Syncup
18/06/13    cja     CR498800 Use TRM to get IRAT RF ID
18/06/13    cja     CR497487 New RF IRAT API to add RF ID in G2T.
14/06/13    cja     CR490812 Remove rfgsm_get_rf_warmup_time(), use gl1_rf_gsm_timing_info.rf_warmup_time_qs
14/06/13    pjr     CR490812 added function gl1_hw_get_rf_gsm_warmup_time
28/05/13    ap      CR461268: DSDS XO management + async time drift problem
22/05/13    cs      TSTS Updates
21/02/13    dv      CR448609 Handle T2G HO GFW transition from MEAS mode to ACTIVE mode
24/05/13    kb/npt  CR459207 - Maintain TDSCDMA frame reference count through
                    GSM sleep when there are TDSCDMA neighbours
13/05/13    cja     CR486893 Support G2T gap offset in range (5000+FW margin) to (5000 + FTSM)
09/05/13    kb/zf   CR385548 - Decode PCH after sleep wakeup error recovery.
26/04/13    zf      CR477992: Fix G2W IRAT issue
02/05/13    pjr     CR482754 added gl1_hw_efs_read_debug prototype
25/04/13    pjr     CR480197 Updated function prototype gl1_hw_cm_get_status
19/04/13    dv      CR458558 T2G:Don't issue GCC from ISR
13/04/13    mc      CR471759 DSDS: QTA Support : Initial code changes & Triton Bring-Up
05/04/13    mc      CR470155 : COEX Support - GERAN arbitrator framework - PS integration
25/03/13    mc      CR464496 : COEX Support - GERAN arbitrator SW CXM integration
28/03/13    jk      CR467869:Changes to send AFC command to FW in FTM mode
14/03/13    cja     CR461030 Add RF CM Enter and Exit
07/03/13    dv      CR461080: Introduction of new GSM VS interface
13/03/13    mc      CR459487: COEX Support - GERAN arbitrator framework
11/03/13    cja     CR460319 Remove EDGE core reset for Dime (MCPM Geran stop does power collapse)
01/03/13    cja     CR450178 In T2G add call of rfm exit after long or delayed gaps.
06/02/13    jk      CR449735: Resolving KW errors
09/01/13    cja     CR438712 Set FN to match command FN/offset for mdsp immediate cmd
10/12/12    cja     CR411308 Dime T2G changes (msg router to GFW)
05/12/12    pg      CR427361: GFW/RF shared memory interface cleanup
23/10/12    pg      CR409233: use RF Task CCS buffer implementation for all X2G measurements
23/10/12    npt     CR413635 - Port sleep code for DIME modem
21/09/12    cja     CR401869 Make G2T cleanup margin bigger for GFW Rx Beta
03/08/12    og      CR383632. Don't request a tuneback to GSM when the G2L startup request
                    is not sent to LTE L1.
02/08/12    ky      Exporting the gl1 functions to FTM code
20/06/12    ky      Added support of using new CCS interface and passing new CCS
                    buffer pointers in RF task messages
11/06/12    ss      CR365989 stop acq and cleanup using second async interrupt in FT.
25/04/12    ws      CR 355838 FR 1604 - GERAN changes to cleanup WCDMA- GL1 API
19/04/12    cja     CR349237 Add API for FTM to reset edge core
12/04/12    pa      CR351597: G2T: UE goes into sleep mode during cleanup
02/04/12    cja     CR346018 RxFE recovery in FTM
29/03/12    dv      CR347854:Enable error msg handling for T2G
27/02/12    cja     CR338055 Recovery for Nikel RxFE Stuck
13/03/12    ws      CR335057 eSACCH buffer flush and restore after inter-cell handovers
29/02/12    dv      CR:340365 -  Idle mode T2G functionality with TS2 & ftick restriction removed
14/02/12    sjw     DSDS CR290462 - wakeup collision detection for DSDS
10/02/12    ws      CR 336194 Enable TD-SCDMA IRAT for GERAN
19/01/12    pg      CR331722 Ensure GL1 maintains vocoder control throughout HO
12/01/12    npt     CR323780 - Modify warmup time and OLS values when low monitor mode is active
12/11/12    npt     CR311544 - Added sleep debug structure FEATURE_GL1_SLEEP_DEBUG_STRUCT
11/01/12    npt     CR315272 - Remove usage of deprecated legacy sleep APIs
27/06/11    cs      CR289602 Clear pending RX at sleep for DSDS G+G mode
15/06/11    cja     CR264690 Correct error in merge
14/06/11    cja     CR264690 Acquisition frequency capture range larger
29/03/11    cs      DSDS CR281877 Disable the G2W init for C+G builds
17/03/11    cja     CR279518 Add single slot FTM power store for GPRS/EDGE
07/02/11    nf      CR273728 added gl1_hw_cm_is_cleanup_done()
20/12/10    nf      Added GFW awake api for WL1
09/12/10    cs      CR264682 Add support for asynch dsp completion interrupt
08/12/10    nf      Added GFW awake api for WL1
02/12/10    nf      9K changed the way clk delays and clk switching is handled
18/11/10    nf      9K added handling for clock speed based on efs value
02/11/10    og      Adding bandwidth to the cm info startup i/f.
14/10/10    ws      FEATURE_GSM_MDSP_AEQ_8PSK - support AEQ Ph2
17/09/10    ws      CR 255850 - Implemented gl1_hw_ftm_get_rx_power_results()
                    for FTM verification of AGC
03/09/10    ws      Added FEATURE_GSM_MDSP_AEQ_SCH for QDSP6 targets
19/08/10    cja     CR252063 Add guard period for CM Async Rx
11/08/10    cja     CR248460 Add guard period for CM Acq (already used for power measure)
23/07/10    ip      CR245474 Added sleep debug
07/06/10    cja     Add W measurment start/stop RF API for BB Rx clock gating.
04/06/10    ip      CR240251 - Updates to the FEE calculation
05/05/10    pg      Add AEQ metrics logging for QDSP6
27/04/10    cs      CR236460 Latest AFC\XO handling updates
16/04/10    cja     Add intial set-up of cycle swallower
10-04-01   tjw      Add customer.h and comdef.h to all SU API files
12/03/10    nf      SBD:Added burst pattern interface
01/03/10    og      g2x and x2g updates.
24/02/10    og      Add new RF API function calls.
19/02/10    og      Update the CM API to support the new RF API on 9k.
08/02/10    cja     Update XO
01/02/10    ky      Extracting Ncell SCH Burst metrics should happen from gfwSchRxBurstMetricsBuffer
                    instead of gfwAsyncMetricBuffers for Q6.
16/11/09    ws      Compressed mode updates for RTR6285
05/08/09    ws      Removed FEATURE_GSM_MDSP_DTM it's now mainlined
04/06/09    ws      Ensured FEATURE_GSM_TX_DPLL_SUPPORT is enabled for POSEIDON modem
26/06/09    sjw     Remove use of RR public store from L1 (for GCF NV flag)
26/05/09    cs      CR167848 fix for expanded afc_freq I/F to mDSP
29/05/09    ws      FEATURE_GSM_MDSP_AEQ phase 1 for all GMSK bursts
18/05/09    ap      Fix compile warnings
09/04/09    pm      DEBUG_GL1_CLK is moved to //source/qcom/qct/modem/geran/cust/main/latest/custgsmdrv.h#27
07/04/09    cs      Fix Lint errors
30/03/09    ws      Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
20/03/09    cs      Fixes to the common 32-bit afc_freq mDSP I/F
13/03/08    og      FEATURE_XO updates.
12/03/09    og      FEATURE_XO support.
26/02/09    cs/hy   Add initial XO support for handling XO and Rot
25/02/09    cs      Add support for FreqCommon AFC logging
06/02/09    cs      Add access function for SSRV0/1 selection in Mustang
02/02/09    cs      Fix the LCU build for previous updates
30/01/09    ip      CR 169036 SSRV0 clock switching for LCU Featurised
30/01/09    ip      CR 167135 Added sleep debug code
26/01/09    cs      Support for SSRV0/1 selection for Mustang
16/01/09    cs      Fix the prototype for gl1_hw_cm_gsm_init
15/01/09    agv     Mustang loading of the GSM mDSP via gl1_hw_mdsp_init
                    via call from gl1_hw_cm_gsm_init.
19/12/08    ip      CR167592 Added additional LCU clock switching
16/12/08    cs      Use common definition for the gsIsrProcDelay
09/12/08    agv     Changed setting of FEATURE_GSM_TX_DPLL_SUPPORT to stop
                    Mustang compile errors.
08/12/08    cs      New split gl1_hw.c with gl1_hw_debug.c and gl1_hw_clk_ctl.c
03/12/08    ws      Added FEATURE_GSM_DYNAMIC_SAIC
21/11/08    agv     CR164875-Resolved High Transmit Frequency Error in EGPRS
04/11/08    ip      Added LCU clock regime changes
30/10/08    ws      CR 162112 - ESAIC Only reset IIR filter on PCH and NCELL BCCH
09/10/08    cs      Allow the RX Sweep to schedule shorter monitor duration
28/08/08    cs      Implement the Fast Rx Sweep functionality for FTM
23/09/08    cs      CR149660 Add in LPM changes so we can support UMTS targets
15/09/08    cs      CR130550 Add in the ability to request a higher/lower clock speed if req'd
                    under the FEATURE_GPRS_USES_HIGH_ARM9_CLOCK
18/07/08    agv     Increased tune time for LCU Compressed Mode Measurements.
09/07/08    ws      CR 149981 - Use Devman to control MDSP clock for 7500 target
27/06/08    nt      CR147984 - REV2:- Final changes for TCXO reset
23/06/08    nt      CR147984:- added access function gl1_hw_shared2gsm
16/06/08    nt      CR147984:- Changes for TCXO reset.
12/05/08    dv      Added FEATURE_GSM_AMR_WB - L1 component
22/04/08    cs      CR141866 Add in a new special LPM for 7500 that doesn't touch mdsp, rf, trm etc. for GPS
21/04/08    ws      CR 144248 Enable and disable ESAIC in gl1_msg_set_egprs_mode() and
                    improve F3 for SAICS status
07/04/08    cs      Improvements to get effective QXDM IQ logging
14/03/08    cs      Fix a LINT prot error for gl1_hw_combined_tcxo_frozen
27/02/08    cs      Add access functions for g2wIsrLockOut manipulation
14/02/08    nf      Added feature enabling other Tech L1's to find if GSM is in a TX state
15/01/08    ws      Added suport for ESAIC logging and SAIC AFC in PTM
14/01/08    cs      Allow for separate IQ logging and saving
13/12/07    agv     CR 131982 Fix for EGPRS DM causing sleep nacks.
07/12/07    cs/ip   CR134215 Remove the problem with GL1/WCDMA sleep getting out of synch on
                    early wakeup
09/11/07    nf      Added FEATURE_GSM_R_SACCH
22/10/07    ws      Add support for ESAIC NV 2508 item control
25/10/07    cs      Remove dependency on l1_sc.h for gsm_only indication for CR117884
12/10/07    cs      CR129559 Add support for asserting sleep at task startup
21/09/07    cs/ip   Unified GL1 clock control
17/09/07    agv     Added I/Q Logging to code base.
17/09/07    agv     CR12855 - Changed search width for sync/async from 14 to
                    12 as requested by systems.
03/08/07    agv     CR123023 - Combined fix for AFC/TT/AGC.
01/08/07    cs      Add in featurisation support for EDGE only/1x targets
17/07/07    ws      CR123746 Add LNA_GAIN_RANGE to rfm_set_rx_gain() API
07/07/07    agv     CR123023 - Update fix for AFC corruption.
27/06/07    agv     CR123023 Fix for AGC corruption, and TT.
12/06/07    og      Upgrade the FEATURE_GSM_L1_HFREQ_ERR_NCELL_SCH_DECODE to
                    include dedicated TCH NCELL SCH attempts.
12/06/07    nt      CR118485: Reverted the previous checkin, for new fix
23/05/07    nt      CR118485: Stops the SDCCH rx/tx whilst aborting, and alligns
                    on block boundary.
23/05/07    agv     CR118309 - Large RSSI variation initial fix.
25/05/07    ws      Adding prototype gl1_hw_get_afc_rot_val_in_ppm()
04/05/07    cs      Updates for TCXO restriction support and rotator push
27/03/07    dv      Added FEATURE_GSM_R_FACCH
12/02/07    agv     Added TCXO Mgr 3.0 Rotator Push for GPS.
12/02/07    cs      F3 for SAIC enable/disable now tracks NV setting
19/12/06    cs      Extra fix for IQ logging, avoids disabling active channel
04/12/06    ws      Added Prototype for gl1_hw_combined_tcxo_frozen()
23/10/06    agv     Fixed a crash for TCXO Mgr 3.0 when release to WCDMA occurs.
18/10/06    agv     Added TRM, TCXO MGR 3.0, and new hw_set_system_mode for 7600.
13/10/06    cjs     CR101170 Fix the IQ logging for MDSP_SAIC and IQ_LOGGING
21/09/06    og      Reset only the cs part of lower layers when connecting DTM
                    assignment in packet transfer (EDTM route).
14/08/06    ws      Allow EDGE clock to be registered independently of EGPRS mode
31/07/06    agv     CR85810 NB AB TSC, and TA fix.
11/07/06    cs      CR104631 - Disable CHIPXN_DIV2 clk regime in GSM TCH mode on 7200
06/18/06    gfr     Added gl1_hw_init_wcdma_clocks
03/29/06    gfr     Add frequency offset to ncell functions to support non
                    spec-compliant networks in WCDMA mode.
16/03/06    og      Take timeslot gaps into account when deciding on Tta/Trb
                    or Ttb/Tra. Resolves CR 88935 and CR 88975.
09/03/06    gfr     Mainline FEATURE_GSM_TCXOMGR_SUPPORT
02/17/06    gfr     Added modulation for power backoff to RF Driver.
01/13/06    gfr     Support for new RFM API (FEATURE_RFM_INTERFACE)
01/13/06    ip      Add function prototype gl1_hw_sleep_gsm_confirm_OKTS
12/20/05    gfr     Added gl1_hw_log_saic_metrics prototype
12/08/05    gfr     Modify gl1_hw_save_iq_samples to work with L4
11/07/05    gfr     Fixed prototype for gl1_hw_start_acquisition
10/25/05    gfr     Renamed gl1_hw_edge_feature_disable_ir
09/30/05    gfr     Added gl1_hw_request_tx_clk
09/29/05    gfr     Removed gl1_hw_set_aci_detection
09/22/05    gfr     Removed FEATURE_SBI_MDSP_CONTROLLER
08/31/05    gfr     Add gl1_hw_enter/leave_idle functions to be called
                    independently of FEATURE_GSM_SLEEP_CONTROLLER
08/19/05    gfr     QLint cleanup
08/05/05    gfr     Change gl1_hw_cleanup_abort_async_rx to gl1_hw_cleanup_async_rx
07/29/05    gfr     DTM support
07/20/05    pp      Renamed gl1_hw_handle_mdsp_panic() to gl1_hw_handle_panic()
                    to allow NPL1/PL1 to call PANIC
06/27/05    gfr     Abort gl1_hw_turn_tx_off if we have a pending mdsp nonsbi
                    write to a register that depends on the tx clock.
06/21/05    gfr     Added gl1_hw_schedule_tt_ok
06/16/05    gfr     Added gl1_hw_set_iq_samples_tn
06/01/05    gfr     Added prototype for gl1_hw_calc_gsm_time_from_sclk
05/27/05    gw      Removed some obsolete sleep definitions.
05/25/05    gfr     Add channel-specific IQ sample saving enabling
05/05/05    gfr     Remove gl1_hw_set_pa_profile
05/03/05    pp      Changes to support 14.10 test case
04/22/05    gfr     Abort interference measures if a conflict is detected
04/21/05    gfr     Added data_tag to the rx_tags structure for getting data
04/21/05    yh      Support for panic reset
04/20/05    yh      Remove use of RF fifo
04/12/05    gfr     Consolidate GSM rx/tx parameters
04/08/05    gfr     Allow caller to specify whether to discard untxed data
04/08/05    gfr     Combined GSM and GPRS channel types
04/08/05    gfr     Move logging code into gl1_hw_log.c and put scheduler
                    prototypes in gl1_hwi.h
04/07/05    gfr     Treat PDM as signed for target independence
04/07/05    gfr     Change GPRS search width to 10.
03/22/05    gfr     Use tx tag to keep track of encoded data.
03/09/05    yh      API change for gl1_hw_get_sync_rx_data(),
                    gl1_hw_get_sync_rx_data_amr() and
                    gl1_hw_get_async_rx_data().
03/04/05    gfr     Decrease GPRS search width to 8 to improve performance.
02/02/05    gfr     Support for NV based disabling of IR and ACI detection.
01/28/05    yh      gl1_hw_init() takes 2 signals in the parameter and pass them
                    to mdsp_download_and_startup()
01/24/05    gfr     Added gl1_hw_set_aci_detection
12/01/04    gfr     Power index is unsigned
10/19/04    gfr     Add TSC to the debug buffer logging
10/08/04    yhong   Added conflict check for delay frame
09/24/04    bm      Support for L1 deactivation split into TASK and ISR
09/20/04    gfr     Support for logging the mdsp debug buffer.
08/26/04    gfr     Added first_burst parameter to gprs rx command.
08/10/04    yhong   Added new wcdma function prototypes from the gl1_hw_wcdma.c
07/22/04    gfr     EGPRS support.
06/15/04    gfr     Added early decode and look_for_facch flag to rx, removed
                    gl1_hw_measurement_T from async metrics call.
06/22/04    gfr     Added prototype for gl1_hw_afc_locked().
06/10/04    gfr     Support for early CBCH decode.
06/01/04    gfr     Support for the TCXO manager.
05/27/04    gfr     Added dedicated mode flag to SRM frequency request.
05/20/04    gfr     Mainlined FEATURE_COMMON_PWR_HANDLER
05/06/04    gfr     Combine phase and freq value for acq results.
04/08/04    gfr     Minor prototype fixes for rotators.
04/08/04    gfr     Wrap the sample_ram scheduler with G2W ifdefs.
02/25/04    gfr     Timing error is unsigned, GSM rotators updates.
02/17/04    gfr     GSM rotators support.
02/11/04    gfr     Support for wcdma reconfirm in GPRS and new HW scheduler.
02/03/04    gfr     Support for logging mdsp scale factors.
02/02/04    bm      Added a function prototype gl1_hw_set_async_rx_search_width
01/30/04    jc      pwr measures for cm should use mon tune times.
01/12/04    gfr     Support for 6 monitors per frame.
01/07/04    bm      Mainlined RF_MULTIMODE
01/06/04    jc      changed startup cmd duration to 125qs.
12/22/03    jc      Added few more defines missing for Compressed Mode.
12/05/03    bm      Added logging support for MDSPcommands issued,
                    and the corresponding pops
12/01/03    jc      Added defs for gsm measurements in WCDMA mode.
11/05/03    gfr     Support for quad-band.
10/22/03    jc      Fix for T3124 by adjusting PDM to last WCDMA value used.
10/21/03    gsc     Added SMS CB support
09/22/03    gfr     Add workaround for anite agc problem.
                    Save the TSC with the I/Q samples.
09/10/03    gfr     Support for saving I/Q samples to flash.
08/28/03    bm      Added prototype for gl1_hw_cm_gsm_init().
08/27/03    gfr     Support for FEATURE_GSM_HW_TASK_REQUEST to support dumping
                    mdsp memory to flash.
08/21/03    gw      Added prototype for gl1_hw_cleanup_abort_async_rx().
08/07/03    tb      New functions for gtow clock control.
08/06/03    gw      Added prototype for gl1_hw_abort_early_decode().
08/05/03    gfr     Changed BCCH number to not conflict with PTCCH.
07/09/03    bk      Got rid of multiple declarations for default async search width
07/08/03    bk      Changed the default ASYNC search width to 80 symbols to handle
                    partial tones
06/23/03    gfr     Compiler warning cleanup.
06/09/03    gfr     Set the async search width to 20 for ncell BCCH.
06/04/03    gfr     Fix the search width values.
06/03/03    gfr     Pass the last attempt parameter when looking for gprs data.
05/27/03    dp      Defined default mdsp async rx search widths for TCH and
                    non TCH channels
05/23/03    gw      Removed code added to support turning off aux components
                    via GPIO 65.
05/20/03    gw      Added support for turning off auxillary components on
                    GPIO 65 during sleep. Added support for early decode of
                    paging block.
05/21/03    gfr     Compress monitors into available space.
05/13/03    gfr     Common power handler support.
05/14/03    gfr     Added prototypes for processing the acq schedule.
05/12/03    gfr     Added prototypes for new GSM schedule saving functions.
05/12/03    gsc     Replaced amr_codec_mode_type with mdsp_amr_codec_mode_type
05/07/03    gsc     Added GSM AMR support
03/28/03    gfr     Added prototype for gl1_hw_log_monitor.
03/19/03    gw      Changesd GL1_HW_DEFAULT_SEARCH_WIDTH from 20 symbols to
                    13 symbols per request from systems group.
02/26/03    gfr     Added prototype for hw sched check code.
02/26/03    dp      Added half rate support featurized under
                    FEATURE_GSM_HALF_RATE
02/24/03    gfr     Lint cleanup.
02/25/03    gw      Moved prototypes for gl1_hw_turn_tx_{on|off}() here
                    from gl1_hwi.h.  These functions are needed outside the
                    hw layer.
02/21/03    gw      Changed prototype of gl1_hw_inact to support deep sleep.
02/12/03    gw      Changed prototypes to support FEE timing error
                    functionality.  Added prototype for gl1_hw_halt_up().
02/12/03    gw      Updated header comments.  Added prototype for function
                    gl1_hw_sched_check_async_rx.
02/04/03    mk      Added functions to allow only one initialization of the
                    WCDMA Searcher block.
02/04/03    jc/drl  Define VCTCXO_MID_RANG_VAL in this file.
01/02/03    gw      Added another argument to prototype for
                    gl1_hw_sleep_gsm_start_sleep().
12/06/02    gw      Added suppport for maintaining WCDMA reference counter
                    during GSM sleep.  Required to support GTOW measurements.
11/03/02    tb      Added support for INTERRAT_GTOW
10/28/02    gw      Changed prototype for gl1_hw_was_async_burst_decoded() to
                    support back-to-back receives for compressed mode.
10/16/02    jc      Add gl1_hw_cm_gsm_terminate().
10/10/02    ih      Moved all script-related data structure and functions into
                    the RF driver to resolve circular dependency between HW and
                    RF
10/11/02    gw      Added return value to gl1_hw_sleep_controller_start_sleep.
                    Added prototype for gl1_hw_pdown_up.
10/10/02    gr      Changed GPRS TX interface to accept TXLEV vs. power index.
10/03/02    JC      Renamed HW_RF_WR_GRFC to HW_RF_WR_NONSBI in
                    hw_rf_bus_enum_type and hw_rf_script_grfc_type to
                    hw_rf_script_nonsbi_type to address register writes
                    within a frame.
09/19/02    ATM     Added Combined ACQ logging
09/16/02    plm     Added void to prototype for gl1_hw_init_static_buffers()
                    to eliminate compiler warning.
09/12/02    sd      Added support for RF driver multimode API that includes
                    GSM drivers.
09/10/02    ih      Added prototype for gl1_hw_init_static_buffers.
09/09/02    gw/ih   Added STATIC types to hw_rf_buf_enum_type to support MDSP
                    static buffers.
09/06/02    gr      Added GPRS DL abort functionality.
09/06/02    jc      Fixed merge problem: put compressed mode code back in.
08/21/02    gw      Merged in GSM sleep code that had been on a branch.
08/23/02    dp      Added crc_pass parameter to gl1_hw_was_sch_decoded
08/21/02    JC      Added burst type to rfgsm_compute_rx_power() prototype.
07/22/02    ATM     Added FCCH/SCH decode logging.
06/28/02    thh     Changed definition of hw_rf_script_sbi_type.
                    New RF API:  all rf_*() functions are now rfgsm_*().
06/25/02    gw      Renamed gl1_hw_power_down_micro to gl1_hw_micro_halt.
06/10/02    gw      Added definitions to support GSM sleep.
06/04/02    bbd     Changed FEATURE_GPRS to FEATURE_GSM_GPRS
04/19/02    JC      Added rx_gain_range to gl1_hw_measurement_T for enhancing
                    logging.
04/16/02    bk      Updated GPRS metrics API and added med. acc. mode API
03/22/02    JC      Changed int16 to dBx16 in rx_async_bcch_burst prototype.
03/20/02    dp      Changed txlevs to tx power index
03/14/02    bk      Changed the signature of GPRS Intf function
03/08/02    gs      included rex.h to get gprs sim to compile
03/06/02    JC      Added optional param to hw_was_sch_decoded() for GPLT.
03/05/02    JC      Renamed gl1_hw_gsm_init() to gl1_hw_init().
02/20/02    dp      Changed gl1_hw_get_async_rx_metrics interface to return
                    metrics availability (true/false)
02/15/02    mk      Updated afc/tt.
02/14/02    gw      Added support for AGC to async rx functions.
02/13/02    jc      New prototype.
01/25/02    JC      Added gl1_hw_measurement_T for AGC support.
02/01/02    mk      Updated to support the latest afc/tt algorithms.
01/23/02    bk      Separated GPRS tx bursts into tx_bursts and tx_access_burst
01/16/02    mk      Added support for afc/tt.
01/15/02    sg      Removed a parameter for gl1_hw_issue_power_meas_cmd
12/18/01    dp      Added function gl1_hw_was_tone_detected and changed acquisition
                    offset type to int32 to handle negative offsets from the DSP
12/13/01    gw      Changes to support message/frame layer acquisition.
12/12/01    jc      Added definitions for sync rx search width size
12/12/01    gw      Moved prototype for gl1_msg_reset_sync_rx_seq_nums to
                    gl1_msg.h
12/10/01    thh     Merged in Brian RF support.
                    Remaining externs from hw_gsm.h are moved here.
                    Hw_gsm.h is obsoleted.
11/19/01    jc      Added gl1_hw_tch_loopback() prototype.
11/9/01     bk      Added GPRS support
10/31/01    gw/dp   Added support for logging debug info.  Added support for
                    returning metrics in dedicated mode. Changed interface to
                    accept the required number of data bytes when retrieving
                    sync and async decoded data from the DSP.
10/31/01    dp      Interface changes in gl1_hw_get_syncrx_data and combined
                    gl1_hw_was_async_bcch_decoded and gl1_hw_was_async_sch_decoded
                    into a single function.  Moved definition of gl1_hw_rx_hdr_struct
                    to gl1_defs.h
10/24/01    dp      Added support for surround cell tone detecttion with an
                    interface to pass in a time offset at which to start or
                    stop the tone detection.
10/23/01    gw      Updated prototype for gl1_hw_get_power_measure_results.
10/16/01    gw      Added comments to some function prototypes.
10/10/01    gw      Added support for power measurements and Ncell SCH and
                    BCCH reception.
09/25/01   gw/jc    Added syncrx metrics and RACH support.
08/31/01     jc     Initial version.
===========================================================================*/

#include "geran_variation.h"
#ifdef CUST_H
#ifndef CUSTOMER_H
  #include "customer.h"
#endif

#endif

#ifndef COMDEF_H
  #include "comdef.h"
#endif

#include "sys_type.h"  /* dBx16_T */
#include "rex.h"

#include "rfcom.h"

#include "gl1_defs.h"
#include "gl1_defs_g.h"

#include "gl1_sys_algo.h"
#include "gl1_hw.h"
#ifdef GERAN_L1_ENHANCED_RX
#include "gl1_hw_lif.h"
#endif /*GERAN_L1_ENHANCED_RX*/

#include "gl1_hw_clk_ctl_g.h"
#include "mdsp_intf_g.h"
#include "mdsp_dtm.h"

#include "geran_dual_sim.h"
#include "grm_type.h"

#include "geran_test_mode_api.h"

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "wmgr.h"
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#include "tcxomgr.h"

#include "slpc.h"

#if defined( FEATURE_INTERRAT_HANDOVER_GTOW ) || defined( FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
#if defined FEATURE_WCDMA
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
#include "wl1x2wsrchapi.h"
#endif
#endif
#endif

#include "rfm_meas.h"
#include "gl1_msgrif_init.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
// Extra guard period in GSM symbols required at the end of SYNC_RX & ASYNC_RX bursts in DSDS
#define GL1_FW_EXTENDED_GUARD_PERIOD GFW_CXM_RX_EXTRA_GUARD_PERIOD

/* Defines invalid BSIC, used in searching for
 * specific BSIC during SCH decoding
 */
#define GL1_HW_INVALID_BSIC      (255)


/* Search width in symbols for the syncrx cmd */
/* The DSP uses this constant and does a search */
/* indexed from 0 to SEARCH_WIDTH,  which means */
/* that mDSP uses 13 in Idle and Dedicated mode */
#define GL1_HW_DEFAULT_SEARCH_WIDTH       (12)

/* Default async search width */
/* The DSP uses this constant and does a search */
/* indexed from 0 to SEARCH_WIDTH,  which means */
/* that mDSP uses 13 in Idle and Dedicated mode */
#define GL1_HW_ASYNC_SEARCH_WIDTH_DEFAULT (12)

/*
 * This is the minimum threshold that we will gate the FCCH results by
 * to decide if the tone is worth performing an SCH on.
 * This is called from the tone detect used by WCDMA so for CM activity.
 * Previoulsy no SNR thresholding was inplace leading to poor HO cells
 * being used and the HO failing. This linear value corresponds to 1dB
 * and is calculated as ( 256 * 10 ^ ( 1dB / 10 ) ) ) = 322
 */
#define GL1_HW_CM_TONE_MIN_SNR_THRESH  (322)

/* Definition for unused/invalid RxLM Buffer */
#define GL1_RXLM_BUF_IDX_NOT_USED MDSP_RXLM_BUF_IDX_INVALID

#define  GL1_EXTRA_LTE_STARTUP_MARGIN  0x13A

#define  GL1_EXTRA_G2T_STARTUP_MARGIN  150


#define GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS          200  // This should match GFW Rx Beta + small margin
#define GL1_HW_G2L_CLEANUP_CMD_PROCESS_QS          200  // This should match GFW Rx Beta + small margin
#define GL1_MAX_TDS_SUBFRAME_COUNT                 8192 // After this value subframe value 0 is returned by TDS_WALLTIME_COUNTER

#define GL1_HW_MCPM_START_STOP_DELAY_LOOP   50

#ifdef FEATURE_GSM_DYNMC_PWR_MEAS
/* Frames to complete pwr measurment. After scheduling one set of pwr measurement,
   GL1 shoud receive pwr measurement results within GL1_HW_PWR_MEAS_FRAMES */
#define GL1_HW_MAX_PWR_MEAS_FRAMES        3
/* State machines to schedules power measurements*/
#define GL1_HW_PWR_MEAS_STATE_MACHINES    2
/* MON_FREQ_LIST_SIZE  (MAX_MEAS_PER_FRAME * GL1_HW_PWR_MEAS_FRAMES * GL1_HW_PWR_MEAS_STATE_MACHINES)
   MON_FREQ_LIST_SIZE can be 12 * 3 * 2 = 72. In mon_read_write_idx_bitmap, each bit represent
   one frequency, so therefore three 32 bits variables are need to keep 72 bits bitmap */
#define GL1_HW_MAX_MON_BITMAP_IDX     3
#endif /*FEATURE_GSM_DYNMC_PWR_MEAS*/

#if defined FEATURE_QBTA || defined (FEATURE_QTA)
#define QBTA_STARTUP_MARGIN            400        // Could this be 0 ?
#define GL1_FW_QBTA_CM_START_UP_OFFSET 50
#define GL1_FW_QBTA_CM_CLEAN_UP_OFFSET 1350

#define QTA_START_OFFSET_SLKS   139
#define QTA_CLEANUP_AFTER_FIRST_FRAME_SLKS 40

#endif


#ifdef GERAN_L1_HLLL_LNASTATE
#define MAX_NUMBER_OF_CHANNELS   12
#define INVALID_SNR              0xFFFF
#endif /* GERAN_L1_HLLL_LNASTATE */

#define GL1_EXPECTED_RX_PWR_AVG_LEN    (2)

/* us = (QS * 3784 / 4096) */
#define CONVERT_QS_TO_US(qs)            ((qs) * 3781 >> 12)

typedef struct
{
sys_modem_as_id_e_type gl1_qta_cm_sys_modem_as_id;
boolean                gl1_qta_wcdma_has_initialised;
boolean                gl1_qta_fw_rf_initialised;
boolean                gl1_empty_qta_gap;
uint8                  gl1_hw_cm_cleanup_wait_frames;
boolean                gl1_hw_cm_gap_complete;
boolean                gl1_hw_cm_startup_preloaded;
boolean                gl1_hw_cm_preload_sent;
boolean                gl1_hw_first_gap_after_preload;
uint16                 gl1_hw_preload_qs;
uint32                 gl1_hw_prebuilt_fn;
rfcom_mode_enum_type   gl1_hw_cm_serving_rat_mode;
rfcom_mode_enum_type   gl1_hw_cm_cxm_source_rat;

boolean                qta_gap_for_g2x_irat;
boolean                qta_cleanup_pending;
boolean                gl1_qta_early_cleanup;
uint32                 gl1_hw_qta_start_slcks_w ;
uint32                 gl1_hw_qta_stop_slcks_w;

uint32                 gl1_hw_cm_nbr_pretune_offset_time;

rfm_meas_common_param_type *gl1_hw_rfm_meas_common_ptr ;

/* for TRM */
rfm_device_enum_type   gl1_hw_cm_trm_rf_device;

#if defined ( FEATURE_QTA ) && defined ( FEATURE_CXM_QTA )

boolean               gl1_hw_rx_flag_in_qta;
int8                  gl1_hw_rx_count_in_qta;

#endif /* defined ( FEATURE_QTA ) && defined ( FEATURE_CXM_QTA )*/

mdsp_x2g_coex_params_type gl1_hw_cm_x2g_coex_params;
} gl1_hw_x2g_data_type;


/* -----------------------------------------------------------------------
**                             GSM Declarations
** ----------------------------------------------------------------------- */
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
extern rfm_meas_xtoy_irat_info_param_type  rf_meas_x2g_irat_info_param[NUM_GERAN_DATA_SPACES];

#if defined( FEATURE_GSM_TX_DIV_ANTNA_SWITCHING) ||defined( FEATURE_GSM_QSC_TX_DIV)

extern boolean antenna_swith_tx_div_nv_enable ;
extern boolean tx_div_type2_enable [];
extern int  antenna_to_use_for_tx [];
void gl1_hw_reset_typ1_asdiv_switch_algorithm (gas_id_t gas_id);
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */


/* General messgae header for sleep controller debug */
#define GL1_SLEEP_HDR "GL1_SLEEP_CTL: "

#ifdef FEATURE_GSM_QSC_TX_DIV

#ifdef FEATURE_GERAN_DRDSDS
/*Static Global Variables used for crisis mode*/
typedef struct
{
  boolean is_crisis_mode_traffic;
  uint8 traffic_mode_consecutive_denied_count;
  uint8 idle_mode_consecutive_preferred_count;
  uint8 idle_pingpong_suspension_count;
  int32 last_type2_alg_antenna_to_use;
} gl1_trm_asdiv_drds_mode_t;

trm_ant_switch_div_config_type gl1_trm_asdiv_idle_pingpong_thresh_check(gas_id_t gas_id,
                                   int32 new_idle_config);
void gl1_trm_asdiv_traffic_denial_thresh_check( gas_id_t gas_id,
                        trm_set_ant_switch_return_type new_config_success);


boolean gl1_trm_asdiv_get_traffic_crisis_flag(gas_id_t gas_id);

#endif

extern boolean  passive_sub_switch_notification [];
extern boolean antenna_switch_burst [] ;
extern boolean  antenna_switch_required_at_start_of_next_frame [];
extern int32 type2_alg_antenna_to_use [];
extern boolean pending_asdiv_switch_request[] ;
extern trm_switch_complete_input_type gl1_switch_complete_input[] ;
void gl1_trm_ant_switch_div_set_mode (trm_client_mode_type mode_setings ,gas_id_t gas_id);
void gl1_trm_ant_switch_div_set_config ( gas_id_t gas_id  );
void gl1_trm_asd_set_mode_and_config (trm_client_mode_type    mode , trm_ant_switch_div_config_type  config ,gas_id_t gas_id );
void gl1_hw_set_asdiv_mode_config(gas_id_t gas_id, trm_client_mode_type mode);
void gl1_hw_set_asdiv_sleep_config(gas_id_t gas_id);
void set_pending_asdiv_switch_request( boolean antenna_pos , gas_id_t gas_id );
void clear_pending_asdiv_switch_request( gas_id_t gas_id );
boolean read_pending_asdiv_switch_request( gas_id_t gas_id );
void gl1_hw_init_asdiv_preferred_antenna(gas_id_t gas_id, trm_ant_switch_get_input_type ant_config_type);

typedef struct
{
  int * antenna_to_use_for_tx;
  uint32 num_elapsed_periods;
  boolean ant_switch_position;
} gl1_trm_asdiv_test_mode_T;

typedef struct
{
/*Variables shared between Data Traffic Type2 switching and Voice Traffic Type 2 switching*/
  int32 rxlev_average;
  int32 previous_rxlev_sub;
  int32 type2_algorithm_start;
  int32 wait_counter;
  int32 new_rxlev_backup;
  int32 old_rxlev_average;

/*Traffic mode variables*/
  int32 rx_pwr_dbmx16_trans_sum;
  uint8 num_meas_data_traffic;
  uint8 num_meas_periods;

} gl1_trm_asdiv_traffic_mode_T;

/*structure for ASDIV type1 in connected mode to track Rx power on each antenna and counter*/
typedef struct
{
  int32 ant_1_rssi;                 /*Current Rx power on antenna 1*/
  int32 ant_2_rssi;                 /*Current Rx power on antenna 2*/
  uint16 number_of_meas_ant_1;      
  uint16 number_of_meas_ant_2;      
  int32 ant_1_avg_rssi;             /*Rx avg power on antenna 1 over decision making period*/
  int32 ant_2_avg_rssi;             /*Rx avg power on antenna 2 over decision making period*/
  int32 wait_count;                 /*Track the waiting time after a switch is made*/
  int32 attempt_with_lower_delta;   /*Track how many times the algorithm has been run without switch*/ 
  boolean start_alg;                /*Flag to indicate if type1 algorithm should start*/
}gl1_hw_asdiv_type1_T;

extern gl1_hw_asdiv_type1_T gl1_hw_asdiv_type1;


/*structure defined for FR27546 and FR27544, it's used to track Tx power, Tx/Rx imbalance and mtpl*/
typedef struct
{

  int32 tx_pwr_accum;            /*accumulated Tx power in dBm*10*/
  int32 tx_pwr_avg_over_sacch;   /*average Tx power in dBm*10*/
  uint16 number_of_tx_pwr_avg;
  int16 DL_delta;                /*the default Rx performance delta (dBm) between antennas (ant1-ant2)*/
  int16 UL_delta;                /*the default Tx performance delta (dBm) between antennas (ant1-ant2)*/
  int16 imb_ULDL;                /*UL_delta - DL_delta (dBmx16)*/
  int32 mtpl_antenna_1;
  int32 mtpl_antenna_2;
#ifdef FEATURE_GERAN_SAR 
  boolean RF_disable_switch;
#endif /*  FEATURE_GERAN_SAR  */ 
} gl1_hw_asdiv_mtpl_T;

extern gl1_hw_asdiv_mtpl_T gl1_hw_asdiv_mtpl_store[];

typedef struct
{
  int16 EGSM_900_UL_delta;
  int16 PGSM_900_UL_delta;
  int16 PCS_1900_UL_delta;
  int16 DCS_1800_UL_delta;
  int16 CELL_850_UL_delta;
  int16 EGSM_900_DL_delta;
  int16 PGSM_900_DL_delta;
  int16 PCS_1900_DL_delta;
  int16 DCS_1800_DL_delta;
  int16 CELL_850_DL_delta;
} antenna_switch_UL_DL_delta_table_T;

extern antenna_switch_UL_DL_delta_table_T asdiv_UL_DL_delta_table_store;
/*ENUM used to distinguish between type1 and type2 switching*/
typedef enum
{
  GL1_TRM_ASDIV_TYPE1,
  GL1_TRM_ASDIV_TYPE2,
	GL1_TRM_ASDIV_INVALID
} gl1_trm_asdiv_switch_type_T;

/*bit mask for idle/rach switch control*/
extern uint16 gl1_antenna_switch_control_mask;

#define DISABLE_IDLE_ANT_SWITCH    0x01
#define DISABLE_ACCESS_ANT_SWITCH    0x02
/*===========================================================================

FUNCTION gl1_hw_get_rxd_enable_status

DESCRIPTION
  

DEPENDENCIES
  None

RETURN VALUE
  boolean         

SIDE EFFECTS
  None
===========================================================================*/

boolean gl1_hw_get_rxd_enable_status(gas_id_t gas_id);

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

boolean gl1_trm_asdiv_enable(gas_id_t gas_id, gl1_trm_asdiv_switch_type_T switch_type);

/*===========================================================================

FUNCTION gl1_asdiv_set_UL_DL_delta
  Set the UL/DL delta base on the current band 

Parameters
  gas_id: gas_id
  curr_band: the current band

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void gl1_asdiv_set_UL_DL_delta(gas_id_t gas_id, sys_band_T curr_band);

/*===========================================================================


===========================================================================*/

void gl1_trm_asdiv_type2_trans_store_rx_pwr(gas_id_t gas_id, dBx16_T dbm_x16_val);

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

void gl1_trm_asdiv_type2_trans_alg(gas_id_t gas_id);

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


void gl1_trm_asdiv_reset_traffic_state_vars(gas_id_t gas_id);


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

void gl1_trm_asdiv_debug_test_mode_init_sm(gas_id_t gas_id);


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

boolean gl1_trm_asdiv_debug_get_test_mode(gas_id_t gas_id);

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


void gl1_trm_asdiv_debug_test_mode_switch_sm(gas_id_t gas_id, trm_client_mode_type mode);
#ifdef FEATURE_GERAN_SAR 
/*===========================================================================

FUNCTION  gl1_hw_check_rf_switch_disable

DESCRIPTION
  RF can disable the switching algorithm due SAR state change. GL1 needs to check with RF and force the switch to
  a certain fix position

===========================================================================*/
boolean gl1_hw_check_rf_switch_disable(gas_id_t gas_id);
#endif /*  FEATURE_GERAN_SAR  */

#endif /*  FEATURE_GSM_QSC_TX_DIV */

typedef enum
{
    cmd_gsm_inact=0,
    cmd_start_acquisition,
    cmd_stop_acquisition,
    cmd_stop_acquisition_part1,
    cmd_stop_acquisition_part2,
    cmd_rx_async_sch_burst,
    cmd_get_asyncrx_metrics,
    cmd_cm_power_measure,
    cmd_cm_get_power_measure_results,
    cmd_cm_startup,
    cmd_cm_cleanup,
    cmd_cm_gsm_init,
    cmd_cm_gsm_terminate
} gl1_hw_cmd_types;

/* Lookup table for converting to mdsp channel type */
extern const mdsp_channel_type mdsp_channel_type_lut[];

/* Function callback type to handle mdsp panic scenario */
typedef void (* gl1_hw_panic_cb_type)(gas_id_t gas_id);

#define GL1_AFC_HDR "GL1_XO: "

extern boolean gl1_is_fw_sleep_disabled( gas_id_t gas_id );
extern boolean gl1_asynch_int_dbg( gas_id_t gas_id );
extern boolean gl1_is_asynch_sleep_enabled( gas_id_t gas_id );
extern boolean gl1_is_rf_sleep_disabled( gas_id_t gas_id );
extern boolean gl1_is_fw_sleep_disabled( gas_id_t gas_id );
extern boolean gl1_is_sleep_opt3_enabled( gas_id_t gas_id );
extern boolean gl1_is_sbd_enabled( gas_id_t gas_id );
extern boolean gl1_is_epd_allowed( gas_id_t gas_id );
extern boolean gl1_is_early_decode_disabled( gas_id_t gas_id );
extern boolean gl1_is_drx_debug_enabled( gas_id_t gas_id );

#define GL1_HW_EFS_DEBUG_ENABLED( dEBUG_mASK ) \
  ( ( gl1_hw_efs_get_debug( ( gas_id ) ) & ( dEBUG_mASK ) ) == \
    ( dEBUG_mASK ) )

/* -----------------------------------------------------------------------
**                             GSM/GPRS Declarations
** ----------------------------------------------------------------------- */
typedef struct
{
  uint16     time_meas;
  uint16     time_bsic;
} gl1_x2g_timing_info_t;

extern boolean gl1_hw_debug_gsm_afc;
extern boolean gl1_hw_async_rx_saic[];

/* LCU needs 128 Qs for DMA normal targets uses only 4 */
#define Gl1_HW_MDSP_GS_ISR_PROCDELAY_QS (128)


/* Flag indicating if we are in gsm/gprs mode or not */
extern boolean gl1_hw_mode_gsm[];


typedef struct
{
   ARFCN_T          arfcn;
   int16            expected_power;
   gl1_defs_tn_type tn;
} gl1_hw_meas_signal_type;

/* lookup table for mapping msg len from coding scheme */
extern const uint8 gl1_hw_cs_to_len_lut[];

extern boolean first_awake_frame_after_sleep[];

/* Variable to track if there is pending sleep req to MCPM through DPC */
extern boolean mcpm_sleep_req_pending[];

typedef struct
{
  int32     freq_error_hz_q6;
  uint32    inv_f_dl_lo;
  boolean   update;
  boolean   valid;
} gl1_hw_mdsp_freq_t;


#ifdef GERAN_L1_HLLL_LNASTATE
typedef struct
{
ARFCN_T   arfcn;
dBx16_T   rx_lvl_dbm;
uint16    snr;
uint16    jdet_value;
}gl1_hw_gsm_metrics;

typedef struct
{
uint8              NoOfChannels;
gl1_hw_gsm_metrics gsm_metrics[MAX_NUMBER_OF_CHANNELS];
}gl1_defs_metrics_lnastate;

typedef struct
{
boolean                        updated_cs_metrics;
uint32                         fn_cs_metrics;
gl1_defs_metrics_lnastate      cs_metrics;
}gl1_hw_cs_metrics_t;

typedef struct
{
boolean                        updated_ps_metrics;
uint32                         fn_ps_metrics;
gl1_defs_metrics_lnastate      ps_metrics;
}gl1_hw_ps_metrics_t;

typedef struct
{
gl1_hw_cs_metrics_t   gl1_hw_cs_metrics;
gl1_hw_ps_metrics_t   gl1_hw_ps_metrics;
}gl1_defs_dtm_metrics_t;

typedef enum {
  UPDATE_CS_METRICS,
  UPDATE_PS_METRICS
}gl1_hw_update_metrics_t;

#endif /* GERAN_L1_HLLL_LNASTATE */

#ifdef FEATURE_GSM_TDS


#define NO_OF_BINS_PER_PHASE 3
#define STARTUP_EARLY_MARGIN_QS (900U)
#define STARTUP_LATE_MARGIN_QS  (700U)

#define PWR_MEAS_1_EARLY_MARGIN_QS  (500U)
#define PWR_MEAS_1_LATE_MARGIN_QS   (400U)

#define PWR_MEAS_2_EARLY_MARGIN_QS  (510U)
#define PWR_MEAS_2_LATE_MARGIN_QS   (400U)

#define PWR_MEAS_3_EARLY_MARGIN_QS  (515U)
#define PWR_MEAS_3_LATE_MARGIN_QS   (400U)

#define PWR_MEAS_4_EARLY_MARGIN_QS  (520U)
#define PWR_MEAS_4_LATE_MARGIN_QS   (400U)

#define PWR_MEAS_5_EARLY_MARGIN_QS  (525U)
#define PWR_MEAS_5_LATE_MARGIN_QS   (400U)

#define PWR_MEAS_6_EARLY_MARGIN_QS  (530U)
#define PWR_MEAS_6_LATE_MARGIN_QS   (400U)

#define PWR_MEAS_7_EARLY_MARGIN_QS  (535U)
#define PWR_MEAS_7_LATE_MARGIN_QS   (400U)

#define PWR_MEAS_8_EARLY_MARGIN_QS  (540U)
#define PWR_MEAS_8_LATE_MARGIN_QS   (400U)

#define SCH_DETECT_EARLY_MARGIN_QS  (500U)
#define SCH_DETECT_LATE_MARGIN_QS   (400U)

#define TONE_DETECT_EARLY_MARGIN_QS (500U)
#define TONE_DETECT_LATE_MARGIN_QS  (400U)

#define CLEANUP_EARLY_MARGIN_QS  (375U)
#define CLEANUP_LATE_MARGIN_QS   (300U)

#define LIMIT_MARGIN_QS_BIN_COUNTER 250
#define X2G_TRANSITION_PREPARATORY_PHASES 12

extern boolean  gl1_hw_schedule_x2g_gap_startup( uint16  timing_offset,gas_id_t gas_id  );
extern boolean  gl1_hw_schedule_x2g_gap_cleanup( uint16  timing_offset, boolean  pre_tick ,gas_id_t gas_id );
extern void     gl1_hw_schedule_remove_x2g_startup( gas_id_t gas_id );

extern void gl1_hw_x2g_pwr_meas_ft_handler(sys_modem_as_id_e_type      as_id);
extern void gl1_hw_x2g_acq_ft_handler(sys_modem_as_id_e_type      as_id);
extern void gl1_hw_x2g_sch_ft_handler(sys_modem_as_id_e_type      as_id);
extern void gl1_hw_x2g_send_commands(gas_id_t gas_id);

typedef enum {
  _STARTUP,
  PWR_MEAS_1,
  PWR_MEAS_2,
  PWR_MEAS_3,
  PWR_MEAS_4,
  PWR_MEAS_5,
  PWR_MEAS_6,
  PWR_MEAS_7,
  PWR_MEAS_8,
  _SCH_DETECT,
  _TONE_DETECT,
  _CLEANUP
}  gl1_gfw_cmd_action_margin_qs_bin_type_t;
//margin_qs_bin_index_t;

typedef enum {
  EARLY_BIN,
  MIDDLE_BIN,
  LATE_BIN
} gl1_gfw_cmd_action_margin_qs_bin_index_t;

typedef struct
{
  uint32 g_fn;
  uint16 offset;              /* offset at which startup is to take effect*/
  uint16 offset_at_api_call;  /* qbit offset when the API was called */
  boolean pending;
  boolean async_int_executed;
} gl1_hw_x2g_startup_info_t;

typedef struct
{
  uint32 offset;
  uint32 g_frame_number;
  boolean pending;
} gl1_hw_x2g_cleanup_info_t;

typedef struct
{
  uint32 startup_g_fn;
  uint32 g_fn;
  uint32 cleanup_g_fn;
  uint16 startup_offset;              /* offset at which startup is to take effect*/
  uint16 offset;
  uint16 cleanup_offset;
  uint16 offset_at_api_call;  /* qbit offset when the API was called */
  uint16 stop_acq_offset;
  uint32 stop_acq_g_fn;
  boolean stop_acq_pending;
  boolean cleanup_pending;
  boolean stop_acq_offset_valid;
  boolean cleanup_async_int_used;

  boolean seperate_cleanup_async_required;  /*Flag to keep track if a seperate async is required for cleanup in the next frame tick*/
} gl1_hw_x2g_acq_info_t;

typedef struct
{
  uint32 startup_g_fn;
  uint32 g_fn;
  uint32 cleanup_g_fn;
  uint16 startup_offset;              /* offset at which startup is to take effect*/
  uint16 offset[MAX_CM_RSSI_MEASUREMENTS_REQ];
  uint16 cleanup_offset;
  uint16 offset_at_api_call;  /* qbit offset when the API was called */
  boolean cleanup_pending;
  uint16  startup_offset2;
  uint32  startup_g_fn2;
} gl1_hw_x2g_pwr_meas_info_t;

typedef struct
{
  ARFCN_T                     ARFCN[MAX_CM_RSSI_MEASUREMENTS];               /* Absolute Radio Frequency Channels  */
  dBx16_T                     rx_signal_strength[MAX_CM_RSSI_MEASUREMENTS];  /* Signal Strengths at Antenna        */
  uint8                       num;                                           /* Number of measurements to make     */
  uint16                      offset[MAX_CM_RSSI_MEASUREMENTS];              /* When to make a measurement in qs   */
  gl1_hw_burst_position_type  position[MAX_CM_RSSI_MEASUREMENTS];            /* position of burst within a gap     */
  gl1_hw_rx_tags_type         tags[MAX_CM_RSSI_MEASUREMENTS];
} gl1_hw_x2g_power_meas_store_t;

typedef struct
{
  uint32 startup_g_fn;
  uint32 startup_offset;
} gl1_hw_x2g_info_t;

/* struct for holding gap info in cx8 format for startup_req mesasges to TDL1*/

typedef struct
{
  /*GSM values in qsymbols*/
  uint32 gap_start_pos_qs;
  uint32 gap_length_qs;

  /*gap position info in TDS Cx8*/
  uint16 gap_start_subfn;
  uint16 gap_start_pos;

  uint16 gap_end_subfn;
  uint16 gap_end_pos;

} gsm_l1_tds_gap_info_type;

#ifdef FEATURE_GSM_TDS_DEBUG
typedef struct
{
  /*GSM values in qsymbols*/
  uint32 current_FN;
  uint32 gap_start_pos_qs;
  uint32 gap_length_qs;

  uint32 clean_pos_qs;
  /*gap position info in TDS Cx8*/
  uint16 gap_start_subfn;
  uint16 gap_start_pos;

  uint16 gap_end_subfn;
  uint16 gap_end_pos;

} gap_info_debug_type;
#endif /*FEATURE_GSM_TDS_DEBUG*/

#define GSM_IRAT_MAX_NUM_FRAMES_FOR_PWR_MEAS_CMDS 2

typedef struct
{
  gl1_hw_measurement_T  samples[MAX_CM_RSSI_MEASUREMENTS_REQ];
  uint8                 num;              /* Number of results to process  */
  gl1_hw_rx_tags_type   tags[MAX_CM_RSSI_MEASUREMENTS_REQ];
  ARFCN_T               ARFCN[MAX_CM_RSSI_MEASUREMENTS_REQ];
} gl1_hw_x2g_rssi_meas_result_t;

typedef struct
{
  gl1_hw_x2g_rssi_meas_result_t meas;
  uint16                seq_num;          /* ID tag associated with request*/
  uint8                 num_cmd;          /* Num of pwr meas cmds issued to MDSP dper frame */
  uint8                 num_cmd_idx;      /* Num of frames over which pwr meas cmds issued */
  uint8                 result_idx;       /* Num of times results are read back per single request */
  uint8                 cmd_issued;       /* Num of commands issued over multiple frames per API call*/
  boolean               struct_in_use;
  uint32                cmd_fn;
  boolean               error_flag;
  boolean               gl1_skip_last_burst;
	
} gl1_hw_x2g_rssi_meas_ctl_t;

#endif /* FEATURE_GSM_TDS */

typedef enum
{
  NoneAI=0,
  Initializing,
  Terminating,
  Initialized,
} gl1_hw_cm_action_types;


typedef struct
{
  gl1_hw_cm_action_types ActionValue;
  boolean   initialised;
  uint32    TimestampOn;
  uint32    TimestampOff;
} gl1_hw_cm_status_t;

typedef enum
{
  NONE_GL1,
  STARTUP,
  PWR_MEAS,
  TONE_DETECT,
  STOP_TONE_DETECT,
  SCH_DETECT,
  CLEANUP

} gl1_hw_x2g_api_type;

#define RF_API_PROFILING_NUM   10
#define MAX_PROFILING_LOG   10


typedef enum{
  SATRTUP_PRE,
  SATRTUP_SSBI_BUILD,
  STARTUP_GRFC_BUILD,
  SATRTUP_SSBI_POP,
  STARTUP_GRFC_POP,
  SATRTUP_AFTER,
  T2G_2G_BURST,
  CLEANUP_SSBI,
  CLEANUP_GRFC,
  CLEANUP_COPY

}t2g_rf_api_type;

typedef struct
{
  uint32 fn;
  uint32 qs;
}gsm_time;

typedef struct
{
  gl1_hw_x2g_api_type api_type;
  gsm_time start_time;
  gsm_time end_time;
  uint32 runtime;
}t2g_api_time;

typedef struct
{
  t2g_rf_api_type rf_type;
  gsm_time start_time;
  gsm_time end_time;
  uint32 runtime;
}t2g_rf_api_profiling_type;



typedef struct
{
  t2g_api_time startup_api_time;
  t2g_api_time cmd_api_time;
  t2g_api_time cleanup_api_time;
  gsm_time gap_time_info;
  uint32 api_runtime;
}t2g_api_profiling_type;

#ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
#define  ACQ_MEAS_NUM_SM           9
#else
#define  ACQ_MEAS_NUM_SM           5
#endif

typedef struct
{
  uint16                      seq_num;
  uint8                       acq_meas_state;
  boolean                     sch_decode;

  uint16                      id;
  uint8                       uinstall_acq_handler_dec_frame;

  boolean                     tone_detect_result;
  boolean                     sch_decode_result;

  boolean                     gl1_hw_x2g_acq_rsp_sent;

  gsm_irat_acquisition_rsp_t  gl1_hw_x2g_acq_store;
#ifdef FEATURE_GSM_TDS  
  gl1_hw_x2g_acq_info_t       gl1_hw_x2g_acq_info;
#endif
  gl1_hw_rx_tags_type*        tags;

} cm_acq_state_machine_type;

#define  POWER_MEAS_NUM_SM             GFW_X2G_PWR_MON_BUFFERING

typedef struct
{
    /* vars for state processing */
   uint8                       power_meas_state;
#ifdef FEATURE_GSM_TDS
   gl1_hw_x2g_rssi_meas_ctl_t  rssi_meas_result;
#endif

} cm_pwr_meas_state_machine_type;

typedef enum
{
  FREE,
  ABORTING,
  PENDING,
  USING,
  USED
} gl1_hw_x2g_command_state;


typedef struct gl1_x2g_command_info_T
{
   gl1_hw_x2g_api_type   cmd_id;
   uint16                message_id;
   uint8                 num_uarfcns;
   ARFCN_T               UARFCN[MAX_CM_RSSI_MEASUREMENTS_REQ];
   gl1_hw_x2g_command_state    state;
} gl1_x2g_command_info_t;

#define  GL1_HW_X2G_CMD_MAX   (20U)


#define Gl1_HW_CM_NUM_SCH_METRICS 3

typedef struct
{
   ARFCN_T arfcn;
   uint16  seq_num;
   uint8   metrics_index;
} gl1_hw_cm_sch_tags_type;



#ifdef QTA_EVENT_DATA_LOGGING
typedef enum
{
  GL1_HW_QTA_RESET,
  GL1_HW_QTA_STARTUP,
  GL1_HW_QTA_CLEANUP
} gl1_hw_qta_event_t;


/* Info struct to capture a QTA even and the serving RAT info
    Intention is to log QTA_STARTUP and QTA_CLEANUP events,
    such that we can track if there is mismatch.
  */
typedef struct
{
  sys_modem_as_id_e_type  as_id_of_srv_tech;  /* serving tech as_id */
  gas_id_t                gas_id;
  gl1_hw_cm_rat_t         srv_tech;           /* serving tech W/T/L */
  uint32                  frame_num;
  gl1_hw_qta_event_t      qta_event;
  boolean                 gl1_qta_has_initialised;  /* essential snapshot of gl1_qta_wcdma_has_initialised */
} gl1_hw_qta_event_data_t;

#define GL1_HW_QTA_EVENT_DATA_LOG_SIZE     (5*2)  /* Store last 5 QTA startup/cleanup pairs */
#endif

extern gl1_hw_cm_status_t         gl1_hw_cm_status[];

#ifdef FEATURE_QTA
/*===========================================================================

FUNCTION  gl1_hw_qta_startup

DESCRIPTION
  This function initializes the RF for QTA operation, this is called by
  WCDMA prior to releasing TRM for a QTA gap.

===========================================================================*/
void gl1_hw_qta_startup( const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr,
                                 geran_gl1_qta_msg_type*     qta_data_p,
                                 sys_modem_as_id_e_type      as_id );

/*===========================================================================

FUNCTION  gl1_hw_qta_cleanup

DESCRIPTION
  This function clears up the RF init after a QTA gap, this is called by
  WCDMA after it has re-acquired TRM at the end of the gap.

===========================================================================*/
void gl1_hw_qta_cleanup( sys_modem_as_id_e_type as_id );

boolean gl1_hw_qta_empty_gap(gas_id_t gas_id);
#endif


#ifdef FEATURE_GSM_RX_DIVERSITY
typedef enum
{
  LOG_TCH,
  LOG_NSCH,
  LOG_PDTCH
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  ,LOG_SACCH
  ,LOG_SDCCH
  ,LOG_PCH
  ,LOG_BCCH
  ,LOG_CCCH_OTHER
  ,LOG_INVALID_OR_FTM = 255
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE */
} gl1_log_chan_type;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
typedef enum
{
   NUM_ACTIVE_DL_SLOTS,
   NUM_ACTIVE_UL_SLOTS,
   AEQ_STATUS,
   NUM_POWER_MONITORS,
   UPLINK_POWER,
   RXD_STATUS,
   POWER_INFO_INVALID =255
}l1_power_info_data;

typedef struct
{
  uint64     ustmr;               /* Time when first sample in packet was collected */
  uint8      num_active_dl_slots; /* Number of active DL slots over the past 20 frames.*/
  uint8      num_active_ul_slots; /* Number of active UL slots over the past 20 frames.*/
  uint8      aeq_status;          /* TRUE if AEQ is used during the 20-frame duration.*/
  uint8      power_monitor;       /* Count of power monitor measurements made during the past 20 frames.*/
  uint8      uplink_power;        /* Latest non-zero Tx power used, in 0.5dBm steps.*/
  uint8      rxd;                 /* TRUE if RxD is enabled any point over the past 20 frames.*/
  boolean    reset;
}l1_power_info_T;

void l1_physical_layer_power_data( l1_power_info_data param, uint8 value, gas_id_t gas_id );

void l1_log_physical_layer_power_info( boolean immediate, gas_id_t gas_id );
boolean gl1_check_no_dl_rx_slots(gas_id_t gas_id);

#define GERAN_POWER_MAX_UL_SLOTS 5
#define GERAN_POWER_UNUSED_UL_SLOT_MASK 0x7F7F7F7F
#endif /* FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO */

typedef enum
{
  GL1_PRX_SLEEP_MODE,
  GL1_DRX_SLEEP_MODE,
  GL1_PRX_DRX_SLEEP_MODE,
  GL1_INVALID_SLEEP_MODE
} gl1_hw_sleep_mode_type;

typedef enum
{
  GL1_PRX_WAKEUP_MODE,
  GL1_DRX_WAKEUP_MODE,
  GL1_PRX_DRX_WAKEUP_MODE,
  GL1_INVALID_WAKEUP_MODE
} gl1_hw_wakeup_mode_type;

/****************************************************************************
                            GSM/GPRS Common FUNCTION PROTOTYPES
 ***************************************************************************/

extern int32 gl1_hw_tcxo_freq_read( gas_id_t gas_id );


/* Are we in the reduced monitor state */
boolean gl1_hw_ncell_pch_low_monitor_active( gas_id_t gas_id );

extern mcpm_tech_type gl1_hw_map_gas_id_to_mcpm_tech( gas_id_t gas_id );

boolean gl1_hw_fws_check_any_x_tech_enabled(void);
uint32 gl1_hw_fws_app_enabled(void);

extern void gl1_hw_copy_sch_data(       void *destination_sch_data_p,
                                  const void *source_sch_data_p );

#define COPY_SCH_DATA( sch_data_dest, sch_data_src )           \
          gl1_hw_copy_sch_data(       (void *)(sch_data_dest), \
                                (const void *)(sch_data_src) )

/*===========================================================================
FUNCTION GL1_HW_INIT

DESCRIPTION
  This function initializes the GSM hardware devices.  It also registers
  a TDMA frame processing handler.

DEPENDENCIES

  task_wait_handler - Needed by the RF component of hardware to implement
  a polling mechanism on the calling tasks context.

  task_wait_sig_1   - Signal used to notify the calling task that the
  RF driver is ready to proceed.

  task_wait_sig_2   - Signal used for mdsp timeout download.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_hw_init
(
  void               (*tdma_isr_ptr)(sys_modem_as_id_e_type as_id),
  rex_tcb_type       *task_ptr,
  void               (*task_wait_handler)(rex_sigs_type, gas_id_t),
  rex_sigs_type      task_wait_sig_1,
  rex_sigs_type      task_wait_sig_2,
  gl1_init_rflm_type gsm_init_rflm_state,
  gas_id_t           gas_id
);

/*===========================================================================

FUNCTION gl1_hw_dm_complete

DESCRIPTION
  This function reads the exported value of dm_complete which tells
  the Host that Data Mover (DM) is complete.

DEPENDENCIES
  None

RETURN VALUE
  boolean         DM finished or not.

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_hw_dm_complete (gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_hw_set_ho_pwr

DESCRIPTION
   This signals the Handover AGC power to use.

===========================================================================*/
void gl1_hw_set_ho_pwr( int16 agc_pwr,gas_id_t gas_id );

/*===========================================================================

FUNCTION gl1_hw_get_idle_mode

DESCRIPTION
  Used to return IDLE mode (TRUE or FALSE)

===========================================================================*/

boolean gl1_hw_get_idle_mode(gas_id_t gas_id);
/*===========================================================================

FUNCTION gl1_hw_set_idle_mode

DESCRIPTION
  Used to set IDLE mode to TRUE or FALSE

===========================================================================*/
void gl1_hw_set_idle_mode(boolean state,gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_hw_set_dedicated

DESCRIPTION
   This signals if the MS is in Dedicated.

===========================================================================*/
void gl1_hw_set_dedicated( boolean dedicated,gas_id_t gas_id );

/*===========================================================================

FUNCTION gl1_hw_get_dedicated

DESCRIPTION
   This checks if the MS is in Dedicated.

===========================================================================*/
boolean gl1_hw_get_dedicated( gas_id_t gas_id );

/*===========================================================================

FUNCTION  gl1_hw_shared2gsm

DESCRIPTION
   This function returns converted int16 to int32 bit vco value for GL1 PDM.

RETURN

   int32  PDM value for GL1 to use.

===========================================================================*/
int32 gl1_hw_shared2gsm(int16 vco_value);

/*===========================================================================

FUNCTION  gl1_hw_shared2gsm

DESCRIPTION
   This function returns converted int32 to int32 value for GL1 PDM.

RETURN

   int32  PDM value for GL1 to use.

===========================================================================*/
int32 gl1_hw_gsm2shared(int32 pdm_value);

/*===========================================================================

FUNCTION  gl1_hw_combined_tcxo_frozen

DESCRIPTION
   This function will returen the frozen state of TCXO Mgr 3 or SRM.

RETURN

   boolean state of TCXO Manager or SRM

===========================================================================*/
boolean gl1_hw_combined_tcxo_frozen( void );


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
tcxomgr_vco_type gl1_hw_get_afc_pdm_val( gas_id_t gas_id );

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
int32 gl1_hw_max_rotator_correction_in_hz( gas_id_t gas_id );


/*===========================================================================

FUNCTION gl1_hw_rf_driver_ho_req

DESCRIPTION
  This function allows MSG Layer to tell RF RF driver a Handover
  is about to occur.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_rf_driver_ho_req( void );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
extern wmgr_client_enum_t gl1_hw_map_gas_id_to_wmgr_client( gas_id_t gas_id );

extern gas_id_t gl1_hw_map_wmgr_client_to_gas_id( wmgr_client_enum_t wmgr_client );

extern boolean gl1_hw_check_granted_device_is_irat_compatabile(uint16 W_chn, gas_id_t gas_id);
extern boolean gl1_hw_check_granted_device_is_LTE_irat_compatabile(lte_earfcn_t lte_dl_earfcn, gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_multi_sim_init

DESCRIPTION
  This function initializes the firmware and RF for DUal sim operation, this must
  only be called once the lock has been granted.

  task_wait_handler - Needed by the RF component of hardware to implement
  a polling mechanism on the calling tasks context.

  task_wait_sig_1   - Signal used to notify the calling task that the
  RF driver is ready to proceed.

  task_wait_sig_2   - Signal used for mdsp timeout download.

===========================================================================*/
void gl1_hw_multi_sim_init
(
  void          (*tdma_isr_ptr)(sys_modem_as_id_e_type as_id),
  rex_tcb_type  *task_ptr,
  void          (*task_wait_handler)(rex_sigs_type, gas_id_t),
  rex_sigs_type task_wait_sig_1,
  rex_sigs_type task_wait_sig_2,
  gas_id_t      gas_id
);

/*===========================================================================

FUNCTION  gl1_hw_multi_sim_fw_rf_init

DESCRIPTION
  This function initializes the firmware and RF for Dual sim operation, this must
  only be called once the lock has been granted.

  task_wait_handler - Needed by the RF component of hardware to implement
  a polling mechanism on the calling tasks context.

  task_wait_sig_1   - Signal used to notify the calling task that the
  RF driver is ready to proceed.

  task_wait_sig_2   - Signal used for mdsp timeout download.

===========================================================================*/
void gl1_hw_multi_sim_fw_rf_init(
  boolean       debug_flag,
  rex_tcb_type  *task_ptr,
  void (*task_wait_handler)(rex_sigs_type, gas_id_t),
  rex_sigs_type task_wait_sig_1,
  rex_sigs_type task_wait_sig_2,
  gl1_init_rflm_type gsm_init_rflm_state,
  grm_client_enum_t  trm_client,
  boolean  wakeup,
  gas_id_t gas_id
);

/*===========================================================================

FUNCTION  gl1_hw_set_multi_sim_sleep

DESCRIPTION
  Allows in a DSDS configuration that we sleep and wakeup as normal. This
  allows us to missout the FW image download every time.

===========================================================================*/
void gl1_hw_set_multi_sim_sleep( boolean allowed,gas_id_t gas_id );

/*===========================================================================

FUNCTION  gl1_hw_set_multi_sim_gonly_deact

DESCRIPTION
  any cleanup required in G + G sleep mode for dual sim needs to take place
  here

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_set_multi_sim_gonly_deact( gas_id_t gas_id  );

/*===========================================================================

FUNCTION  gl1_hw_multi_sim_deact

DESCRIPTION
  This function performs any cleanup necessary when releaseing the lock
  in dual sim operations.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_multi_sim_deact( gas_id_t gas_id );
#ifdef FEATURE_G2X_TUNEAWAY
void gl1_hw_multi_sim_deact_g2x( gas_id_t gas_id );
#endif
/* ----------------------------------------------------------------------- */
/* State indicating if we are running in dual sim mode, and FW status      */
/* ----------------------------------------------------------------------- */
extern GfwAppModeType gl1_hw_multi_sim_mode[];
extern boolean gl1_hw_multi_sim_gonly_mode;
#ifdef FEATURE_WCDMA
boolean gl1_hw_gsm_client_requires_resource(gas_id_t gas_id);
void    gl1_hw_sleep_wcdma_active(const boolean is_active, gas_id_t gas_id);
#endif /* FEATURE_WCDMA */

/*===========================================================================

FUNCTION  gl1_hw_multi_sim_tcm_fw_init

DESCRIPTION
  This function initializes TCM and FW for. This moves FW into
  the measurement active state. This must be called by GL1 after the TRM lock
  has been granted.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_multi_sim_tcm_fw_init(GfwAppModeType state);

void gl1_set_gfw_app_mode_status(GfwAppModeType gfw_app_mode, gas_id_t gas_id);
GfwAppModeType gl1_get_gfw_app_mode(gas_id_t gas_id);

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION gl1_hw_sleep_reset_mdsp_intf

DESCRIPTION
  Clears out the mdsp interface when we enter sleep to ensure no pending
  commands remain when in DSDS mode. Avoids any issues with incomplete
  command requests being actioned by the firmware when we wakeup for the
  next G activity (could be the other sub).

  This is mainly seen when executing the sleep activity in DSDS after the
  asynch interrupt processing, but caould occur anytime we perform the
  asynch int handling.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void gl1_hw_sleep_reset_mdsp_intf( gas_id_t gas_id );

/*===========================================================================

FUNCTION  gl1_hw_inact_task

DESCRIPTION
  This function performs any cleanup necessary when leaving GSM mode from task.

  If deep_sleep is TRUE then the Layer 1 hardware will be powered
  down.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_inact_task( boolean   deep_sleep,
                        boolean   suspend,
                        gas_id_t  gas_id
                      );

/*===========================================================================

FUNCTION  gl1_hw_inact_isr

DESCRIPTION
  This function performs any cleanup necessary when leaving GSM mode from ISR.

DEPENDENCIES
  None

RETURN VALUE
  None TRUE if going to deactivate, FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_inact_isr(boolean force_mdsp_inact, gas_id_t gas_id);


/*===========================================================================

FUNCTION  gl1_hw_inact

DESCRIPTION
  This function performs any cleanup necessary when leaving GSM mode.

  If deep_sleep is TRUE then the Layer 1 hardware will be powered
  down.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_inact( boolean deep_sleep,gas_id_t gas_id );

/*===========================================================================

FUNCTION  gl1_hw_sleep_gsm_panic_inact

DESCRIPTION
   Immediately end the sleep timeline and turn all the clocks back on.  The
   RF may or may not be on after calling this function.  If this function is
   called during a sleep cycle then the GSMR will be restarted at a random
   point.  It is assumed that this will be followed by a full reset of L1.

   This function should only perform actions that are required to allow a
   reset of L1 to proceed without a crash.  Anything that can wait until
   L1 is brought up again should be done then (i.e. in gl1_hw_init() or
   gl1_hw_sleep_gsm_init() etc).

===========================================================================*/
void gl1_hw_sleep_gsm_panic_inact( gas_id_t gas_id );

/*============================================================================
FUNCTION  gl1_hw_panic_inact

DESCRIPTION
  This function is used to shut down all devices that NPL1 depends on. This is
  a force shut down due to a panic event, either an upper layer exception or a
  mdsp halt. Similar to gl1_hw_inact, but we don't wait for mdsp tick since
  ISR may be dead.

===========================================================================*/
void gl1_hw_panic_inact(gas_id_t gas_id);

/*=============================================================================
FUNCTION gl1_hw_terminate

DESCRIPTION
   Terminates all frame layer processing.


DEPENDENCIES
   The frame layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_hw_terminate( void );

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
void gl1_hw_sleep_gsm_init( gas_id_t gas_id );
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
void gl1_hw_sleep_register_npa_cb( gas_id_t gas_id );


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
void gl1_hw_sleep_gsm_shutdown( gas_id_t gas_id );


/****************************************************************************
                            GSM FUNCTION PROTOTYPES
 ***************************************************************************/

/*===========================================================================

FUNCTION gl1_hw_config_sample_server

DESCRIPTION
 The function sets the chosen Sample Server to the F/W if it
 supports setting else defaults to SSRV0 which is the normal
 configuration

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None
===========================================================================*/
extern void gl1_hw_config_sample_server( void );

/*===========================================================================

FUNCTION gl1_hw_is_sample_server1_active

DESCRIPTION
  Return the status of the active sample server.

DEPENDENCIES
  None

RETURN
  TRUE if SS1 active

SIDE EFFECTS
  None

===========================================================================*/
extern boolean gl1_hw_is_sample_server1_active( void );

/*===========================================================================

FUNCTION  gl1_hw_tone_detected_total_freq_offset

DESCRIPTION
  This function calculates the total freq offset of the detected tone

===========================================================================*/
int32 gl1_hw_tone_detected_total_freq_offset(ARFCN_T arfcn, int32 afc_freq, gas_id_t gas_id);


/*===========================================================================

FUNCTION gl1_hw_tx_normal_burst

DESCRIPTION
  This function initiates a NORMAL format burst transmit process.  Normal
  transmit bursts apply to SACCH, BCCH, PCH, AGCH, SDCCH, TCH.

DEPENDENCIES
  This function can be executed at any time but will obviously result
  in failed received bursts if issued at the wrong time.  It assumes
  that the slot alignment is correct.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void gl1_hw_tx_normal_burst
(
   const gl1_hw_tx_signal_type   *signal_info,
   const gl1_hw_tx_burst_type    *burst_info
);

#ifdef FEATURE_GSM_AMR
extern void gl1_hw_set_dl_acs_amr ( mdsp_amr_codec_mode_type *dl_acs,gas_id_t gas_id);
#endif /* FEATURE_GSM_AMR */

/*===========================================================================

FUNCTION  gl1_hw_rx_normal_burst

DESCRIPTION
  This function initiates a NORMAL format burst receive process.  Normal
  receive bursts apply to SACCH, BCCH, PCH, AGCH, SDCCH, TCH.

DEPENDENCIES
  This function can be executed at any time but will obviously result
  in failed received bursts if issued at the wrong time.  It assumes
  that the slot alignment is correct.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void gl1_hw_rx_normal_burst
(
   const gl1_hw_rx_signal_type *signal_info,
   const gl1_hw_rx_burst_type  *burst_info,
   gl1_hw_rx_tags_type         *tags
);

#ifdef FEATURE_GSM_AMR
extern void gl1_hw_rx_normal_burst_amr
(
   const gl1_hw_rx_signal_type  *signal_info,
   const gl1_hw_rx_burst_type   *burst_info,
   gl1_hw_rx_tags_type          *tags
);
#endif

/*===========================================================================

FUNCTION  gl1_hw_rx_async_burst

DESCRIPTION
  This function initiates an asynchronous burst receive process for the given
  channel type.
  The burst_num parameter should be set to -1 if this is not part of an early
  decode receive.

DEPENDENCIES
  This function can be executed at any time but will obviously result
  in failed received bursts if issued at the wrong time.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void gl1_hw_rx_async_burst
(
   const gl1_hw_rx_signal_type  *signal_info,
   const gl1_hw_rx_burst_type   *burst_info,
   uint16                       timing_offset,
   int32                        frequency_offset,
   gl1_hw_rx_tags_type          *tags,
   mdsp_burst_mode              mdsp_burst_type,
   gas_id_t                     gas_id
);


extern void gl1_hw_rx_async_sch_burst2
(
   ARFCN_T  ARFCN,
   int16    signal_strength,
#ifdef FEATURE_GSM_RX_DIVERSITY
   int16    signal_strength_divrx,
#endif
   uint16   timing_offset,
   int32    afc_freq,
   gl1_hw_rx_tags_type *tags,
   gas_id_t gas_id
);


/*===========================================================================

FUNCTION  gl1_hw_write_tx_data

DESCRIPTION
  This function writes data into the mDSP buffer.

===========================================================================*/
extern void gl1_hw_write_tx_data
(
   gl1_hw_channel_type  chan,
   uint8                *primary_data,
   uint16               primary_data_length,
   uint8                *secondary_data,
   uint16               secondary_data_length,
   gl1_defs_tn_type     tn,
   gl1_hw_tx_tags_type  *tags,
   gas_id_t gas_id
);


#ifdef FEATURE_GSM_AMR
extern void gl1_hw_write_tx_data_amr
(
   gl1_hw_channel_type  chan,
   uint8                *data,
   uint16               length,
   gl1_defs_tn_type     tn,
   gl1_hw_tx_tags_type  *tags,
   gas_id_t gas_id
);
#endif

/*===========================================================================

FUNCTION  gl1_hw_get_sync_rx_data

DESCRIPTION
  This function retrieves rx data from the mDSP.

DEPENDENCIES
  This function can be executed at any time but will obviously result
  in failed received bursts if issued at the wrong time.

===========================================================================*/
extern void gl1_hw_get_sync_rx_data
(
   gl1_hw_channel_type     chan,
   gl1_hw_rx_tags_type     *tags,
   gl1_defs_rx_hdr_struct  *rpt_hdr,
   uint8                   *data,
   uint16                  data_len,
   gas_id_t                gas_id
);


#ifdef FEATURE_GSM_AMR
extern void gl1_hw_get_sync_rx_data_amr
(
   gl1_hw_channel_type     chan,
   gl1_hw_rx_tags_type     *tags,
   gl1_defs_rx_hdr_struct  *rpt_hdr,
   uint8                   *data,
   uint16                  data_len,
   gas_id_t                gas_id
);
#endif /* FEATURE_GSM_AMR */

/*===========================================================================

FUNCTION  gl1_hw_enable_ciphering,   gl1_hw_disable_ciphering

DESCRIPTION
  This function enables/disables ciphering on both the uplink and downlink
  by writing to the GSAC_CONFIG_x_RW registers and then telling the mdsp
  to turn on/off ciphering.

  key - pointer to cipher key

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void gl1_hw_enable_ciphering ( uint8 *key, gl1_defs_cipher_mode mode,gas_id_t gas_id );

extern void gl1_hw_disable_ciphering ( gas_id_t gas_id );

extern void gl1_hw_update_ciphering_fn ( gas_id_t gas_id );

/*===========================================================================

FUNCTION  gl1_hw_init_for_tch

DESCRIPTION
  This function should be called to perform any initialization that need to
  be done when entering the traffic channel from idle, or rach states.
  The search window size is modified as is the burst size.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void gl1_hw_init_tch ( gl1_hw_channel_type chan,gas_id_t gas_id );

/*===========================================================================

FUNCTION gl1_hw_align_vfr

DESCRIPTION
  This function sets the hardware such that on the next frame tick, the
  Vocoder Frame Alignment should occur.  This function should be called
  every time the gstmr is reprogrammed to realign the VFR again.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void gl1_hw_align_vfr( gas_id_t gas_id );

/*===========================================================================

FUNCTION gl1_hw_get_sync_rx_metrics

DESCRIPTION
  This function retreives the metrics for the burst received in the previous
  frame.

DEPENDENCIES
  A burst receive command must have been issued two frames previously to
  cause a burst to be received in the previous frame.

RETURN VALUE
  True/False depending on if a Interferer Blocker is seen.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean gl1_hw_get_sync_rx_metrics
(
   gl1_defs_metrics_rpt *rpt,
   gl1_hw_rx_tags_type  *tags,
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
   gl1_log_chan_type log_chan_type,
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */
   gas_id_t             gas_id
);

/*===========================================================================

FUNCTION gl1_hw_tx_rach_burst

DESCRIPTION
  This function initiates a RACH format burst transmit process.

===========================================================================*/
extern void gl1_hw_tx_rach_burst
(
   gl1_hw_tx_signal_type   *signal_info,
   gl1_hw_tx_burst_type    *burst_info,
   uint16                  data
);

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
void gl1_hw_reset( gas_id_t gas_id );

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
#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM

void gl1_hw_reset_cs_only( gas_id_t gas_id );

#endif
#endif

/*===========================================================================

FUNCTION gl1_hw_tch_loopback

DESCRIPTION
  This function tells the mdsp to put the TCH into loopback A,B or C or no
  loopback.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_tch_loopback( gl1_defs_loopback_type loopback,gas_id_t gas_id );

/****************************************************************************
                            GPRS FUNCTION PROTOTYPES
 ***************************************************************************/

/*===========================================================================

FUNCTION gl1_hw_gprs_reset

DESCRIPTION
  Resets mDSP GPRS seq nums.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_gprs_reset( gas_id_t gas_id );

/*=============================================================================
FUNCTION gl1_hw_rx_normal_bursts

DESCRIPTION
   Commands the mDSP to receive data on any of the 4 specified timeslots.  This
   function is to be invoked every frame of a block.


DEPENDENCIES
   The frame layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_hw_rx_normal_bursts
(
   const gl1_hw_rx_signal_type      signal_info[ GL1_DEFS_MAX_ASSIGNED_DL_TS ],
   uint8                            num_tn,
   const gl1_hw_rx_pkt_burst_type   *burst_info,
   gl1_hw_rx_tags_type              *tags
);

/*=============================================================================
FUNCTION  gl1_hw_get_rx_metrics

DESCRIPTION
   Retrieves metrics information. This function is to be called every frame ISR.
   If there is no data or metrics to be returned then the corresponding buffer
   pointer will be set to 'NULL'.

DEPENDENCIES

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_hw_get_rx_metrics
(
   gl1_defs_rx_pkt_metrics_type *metrics_info[ GL1_DEFS_MAX_DL_MSGS ],
   uint8                        num_msgs,
   boolean                      get_usf_info,
   boolean                      get_hard_decisions,
   gl1_hw_rx_tags_type          *tags,
#ifdef GERAN_L1_ENHANCED_RX
   gl1_msg_lif_chan_type        chan_type,
#endif /*GERAN_L1_ENHANCED_RX*/
   gas_id_t                     gas_id
);

/*=============================================================================
FUNCTION gl1_hw_get_rx_data

DESCRIPTION
   Retrieves data information. This function is to be called every frame ISR. If
   there is no data or metrics to be returned then the corresponding buffer
   pointer will be set to 'NULL'.

DEPENDENCIES
   The frame layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
boolean gl1_hw_get_rx_data
(
   boolean                   ptcch_type,
   gl1_defs_tn_type          tn,
   gl1_hw_rx_tags_type       *tag,
   boolean                   last_attempt,
   gl1_defs_rx_pkt_data_type *msg_info,
   gas_id_t                  gas_id
);

/*=============================================================================
FUNCTION gl1_hw_abort_rx_decode

DESCRIPTION
   Aborts a receive in progress.  Only needs to be called if a receive with
   the decode flag was issued, otherwise it is not necessary to inform the
   frame layer of the abort.

============================================================================= */
void gl1_hw_abort_rx_decode (gl1_hw_rx_tags_type *tags,gas_id_t gas_id);

/*=============================================================================
FUNCTION gl1_hw_set_usf_list

DESCRIPTION
   Update the USF Table with USF values for a given PDCH. The DSP will use this
   table in dynamic or extended dynamic allocation to determine if any Uplink
   resources have been allocated

DEPENDENCIES
   The frame layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_hw_set_usf_list
(
  const gl1_defs_rx_pkt_alloc_type usf_info[ GL1_DEFS_MAX_ASSIGNED_DL_TS ],
  uint8                            num_alloc_tn,
  gas_id_t                         gas_id
);

/*=============================================================================
FUNCTION gl1_hw_tx_access_burst

DESCRIPTION
   Commands the mDSP to encode and transmit an access burst on the specified
   frequency using the specified power level.

DEPENDENCIES
   The frame layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_hw_tx_access_burst
(
   const gl1_hw_tx_signal_type       *tx_signal_info,
   const gl1_hw_tx_pkt_burst_type    *burst_info
);

/*=============================================================================
FUNCTION gl1_hw_tx_bursts

DESCRIPTION
   Commands the mDSP to transmit the encoded data on the specified frequencies
   using the specified power levels. The function is to be invoked every frame
   in a block.

   In case of dynamic and extended dynamic allocation this function is invoked
   every frame until all messages in the mDSP buffers are transmitted.


DEPENDENCIES
   The frame layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_hw_tx_bursts
(
   gl1_hw_tx_signal_type            tx_signal_info[ GL1_DEFS_MAX_ASSIGNED_UL_TS ],
   uint8                            num_tn,
   const gl1_hw_tx_pkt_burst_type   *burst_info
);

/*=============================================================================
FUNCTION  gl1_hw_write_tx_pkt_data

DESCRIPTION
   Writes a block/frame of data to the DSP memory. If the message buffer in the
   mDSP is full then one of the messages in the mDSP buffer will be discarded
   to make space for the submitted message (if the submitted message is a higher
   priority message).
   Returns TRUE if the message was successfully written to the mDSP.
============================================================================= */
boolean gl1_hw_write_tx_pkt_data
(
   gl1_defs_tx_pkt_data_type *msg_ptr,
   uint8                     len,
   gl1_defs_tn_type          tn,
   gl1_hw_pkt_msg_type       msg_type,
   gl1_hw_channel_type       chan_type,
   gl1_hw_tx_tags_type       *tags,
   gas_id_t                  gas_id
);

/*=============================================================================
FUNCTION gl1_hw_abort_tx_bursts

DESCRIPTION
   This function may be invoked to clear the mDSP memory of the submitted
   payloads and return the pointer to the payload.

   The channel type decides the payload to be cleared. To abort prach access
   bursts transmissions. Channel type PRACH  may be passed in.
   To abort pacch/pdtch access or normal bursts. Channel type PDTCH_CS_1
   may be passed in.

   Transmission of may resume by resubmitting the data via
   gl1_hw_write_pkt_data() and then invoking gl1_hw_tx_access_burst() or
   gl1_hw_tx_bursts()

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
boolean gl1_hw_abort_tx_bursts
(
   gl1_hw_tx_tags_type *tags,
   gas_id_t             gas_id
);

/*=============================================================================
FUNCTION  gl1_hw_get_tx_scheduled_result

DESCRIPTION
   Returns if the message associated with the given tag was transmitted.
   If discard_untxed is set then untransmitted messages are cleared.

============================================================================= */
boolean gl1_hw_get_tx_scheduled_result
(
   gl1_hw_tx_tags_type *tags,
   boolean             discard_untxed,
   gas_id_t            gas_id
);

/*=============================================================================
FUNCTION gl1_hw_convert_usf_to_pdch

DESCRIPTION
   This function converts all un-transmitted USF messages to PDCH specific
   messages. The PDCH specific messages are transmitted on the specified
   timeslots.

DEPENDENCIES
   The frame layer must be initialized.

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================= */
void gl1_hw_convert_usf_to_pdch
(
   gl1_hw_tx_tags_type *tags,
   gl1_defs_tn_type    tn,
   gas_id_t            gas_id
);

/*=============================================================================
FUNCTION gl1_hw_set_medium_access_mode

DESCRIPTION
   Set the medium access mode to fixed, dynamic or extended dynamic.

============================================================================= */
void gl1_hw_set_medium_access_mode
(
   gl1_defs_access_mode_type mode,
   gas_id_t                  gas_id
);

/*=============================================================================
FUNCTION gl1_hw_set_egprs_mode

DESCRIPTION
   Enables or disables EGPRS mode and Registers (conditionally)
   the EDGE CLK.

============================================================================= */
void gl1_hw_set_egprs_mode
(
   boolean  enable,
   boolean  reg_clk,
   gas_id_t gas_id
);

/*===========================================================================

FUNCTION gl1_hw_feature_disabled_ir

DESCRIPTION
  Returns if incremental redundancy is manually disabled or not.

===========================================================================*/
boolean gl1_hw_feature_disabled_ir (gas_id_t gas_id);

/*=============================================================================
FUNCTION gl1_hw_schedule_tt_ok

DESCRIPTION
  Checks if a give frame delay value would conflict with a scheduled event.

============================================================================= */
boolean gl1_hw_schedule_tt_ok (int16 delay, gas_id_t gas_id);

/*=============================================================================
FUNCTION gl1_hw_schedule_init

DESCRIPTION
   Reset the schedule check store data

============================================================================= */
void gl1_hw_schedule_init (gas_id_t gas_id);

/*=============================================================================
FUNCTION gl1_hw_schedule_reset_events

DESCRIPTION
   Reset hw schedule events

============================================================================= */
void gl1_hw_schedule_reset_events (gas_id_t gas_id);


/*=============================================================================
FUNCTION gl1_hw_data_init

DESCRIPTION
   Reset the gl1_hw.c static store data

============================================================================= */
void gl1_hw_data_init(gas_id_t gas_id);

/*=============================================================================
FUNCTION  gl1_hw_pwr_meas_cmd

DESCRIPTION
   Commands the DSP to take measurements at the specified frequencies and
   timeslots (if force_specified_timeslots is TRUE).
   If force_specified_timeslots if FALSE then the first timeslot is used
   as a starting window for the dsp to schedule the measurements.

DEPENDENCIES
   The frame layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
uint32 gl1_hw_pwr_meas_cmd
(
   ARFCN_T                 *arfcns,
   sys_algo_agc_T          **agc_ptrs,
   uint32                  max_num_freq,
   gl1_hw_rx_tags_type     *tags,
   gas_id_t                gas_id,
   uint32                  arfcn_index
);

/*=============================================================================
FUNCTION  gl1_hw_int_meas_cmd

DESCRIPTION
   Commands the DSP to take interference measurements at the specified
   frequencies and timeslots. The results will be available in the subsequent
   frame.

DEPENDENCIES
   The frame layer must be initialized.

RETURN VALUE
   Returns FALSE if the measurements could not be scheduled.

SIDE EFFECTS
   None
============================================================================= */
boolean gl1_hw_int_meas_cmd
(
   gl1_hw_meas_signal_type signal_info[ GL1_DEFS_MAX_ASSIGNED_DL_TS ],
   uint32                  num_freq,
   gl1_hw_rx_tags_type     tags[ GL1_DEFS_MAX_ASSIGNED_DL_TS ],
   gas_id_t                gas_id
);

/*=============================================================================
FUNCTION gl1_hw_pwr_meas_results

DESCRIPTION
   Get the measurement results of the previous measurements command.

DEPENDENCIES
   The frame layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
boolean gl1_hw_pwr_meas_results
(
  gl1_hw_measurement_T *measurements,
  uint32               meas_data_len,
  uint32               *retrieved_data_len,
  boolean              last_attempt,
  gl1_hw_rx_tags_type  tags[],
  gas_id_t             gas_id
);

/*=============================================================================
FUNCTION gl1_hw_pwr_meas_results_available

DESCRIPTION
   Checks if monitor result is available from FW.

============================================================================= */
boolean gl1_hw_pwr_meas_results_available(gas_id_t gas_id);

/*=============================================================================
FUNCTION gl1_hw_pwr_meas_abort

DESCRIPTION
   Aborts any ongoing power measures.  Note this also aborts interference
   measures.

============================================================================= */
void gl1_hw_pwr_meas_abort (gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_terminate_tch

DESCRIPTION
  This function should be called to perform any cleanup necessary when
  leaving the TCH. The search window size is returned to it's default value.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_terminate_tch ( gas_id_t gas_id );

/*=============================================================================
FUNCTION gl1_hw_schedule_frame_tick

DESCRIPTION
   Ticks the scheduler.

============================================================================= */
void gl1_hw_schedule_frame_tick (uint32 timestamp,gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_hw_dtm_frame_tick

DESCRIPTION
   Called every frame tick after the rx and tx handlers have been executed
   to issue any scheduled receives or transmits.

===========================================================================*/
void gl1_hw_dtm_frame_tick (gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_hw_delay_frame_tick

DESCRIPTION
  This function delays the GSM frame tick by the given offset

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_delay_frame_tick(int16 offset,gas_id_t gas_id);

/*=============================================================================
FUNCTION gl1_hw_setup_tx_power_control

DESCRIPTION
   Configures the RF and mDSP transmit power control parameters.

============================================================================= */
void gl1_hw_setup_tx_power_control
(
   uint8  num_pdch,
   uint16                   power_index[GL1_DEFS_MAX_ASSIGNED_UL_TS],
   /* Modulation needed for power backoff. */
   gl1_defs_modulation_type modulation[GL1_DEFS_MAX_ASSIGNED_UL_TS],
   gas_id_t                 gas_id
);

/*=============================================================================
FUNCTION gl1_hw_set_num_assigned_tx_slots

DESCRIPTION
   Sets the number of assigned uplink slots.  This is used for determining
   the power backoff for multi-slot uplink.  Note that "assigned" is used
   meaning the number of slots in the PUA, whereas "allocated" refers to
   the number of USFs allocated in dynamic allocation.

============================================================================= */
void gl1_hw_set_num_assigned_tx_slots (uint8 num_assigned_tx_slots,gas_id_t gas_id);

/****************************************************************************
                            DTM FUNCTION PROTOTYPES
 ***************************************************************************/

/*===========================================================================
FUNCTION gl1_hw_dtm_init

DESCRIPTION
   Initialized DTM related parameters.

===========================================================================*/
void gl1_hw_dtm_init (gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_hw_dtm_terminate

DESCRIPTION
   Terminate DTM processing

===========================================================================*/
void gl1_hw_dtm_terminate (void);

/*===========================================================================

FUNCTION gl1_hw_rx_dtm_gprs

DESCRIPTION
  Schedule a GPRS receive.

===========================================================================*/
void gl1_hw_rx_dtm_gprs
(
   const gl1_hw_rx_signal_type      signal_info[GL1_DEFS_MAX_ASSIGNED_DL_TS],
   uint8                            num_tn,
   const gl1_hw_rx_pkt_burst_type   *burst_info,
   gl1_hw_rx_tags_type              tags[GL1_DEFS_MAX_ASSIGNED_DL_TS],
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
   boolean                          block_start,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   gas_id_t                         gas_id
);

/*===========================================================================

FUNCTION gl1_hw_rx_dtm_gsm

DESCRIPTION
  Schedule a GSM receive.

===========================================================================*/
void gl1_hw_rx_dtm_gsm
(
   const gl1_hw_rx_signal_type   *signal_info,
   const gl1_hw_rx_burst_type    *burst_info,
   gl1_hw_rx_tags_type           *tags,
#ifdef FEATURE_GSM_RX_DIVERSITY
   boolean                        block_start,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   gas_id_t                       gas_id
);

/*===========================================================================

FUNCTION gl1_hw_tx_dtm_gprs

DESCRIPTION
  Schedule a GPRS transmit.

===========================================================================*/
void gl1_hw_tx_dtm_gprs
(
   const gl1_hw_tx_signal_type       signal_info[GL1_DEFS_MAX_ASSIGNED_UL_TS],
   uint8                             num_tn,
   const gl1_hw_tx_pkt_burst_type    *burst_info,
   gl1_hw_tx_tags_type               tags[GL1_DEFS_MAX_ASSIGNED_UL_TS],
   uint8                             num_msgs,
   uint8                             num_ul_timeslots,
   gas_id_t                          gas_id
);

/*===========================================================================

FUNCTION gl1_hw_tx_dtm_gsm

DESCRIPTION
  Schedule a GSM transmit.

===========================================================================*/
void gl1_hw_tx_dtm_gsm
(
   const gl1_hw_tx_signal_type  signal_info[GL1_DEFS_MAX_ASSIGNED_UL_TS],
   const gl1_hw_tx_burst_type   *burst_info,
   gl1_hw_tx_tags_type          *tags,
   gas_id_t                     gas_id
);

#ifdef FEATURE_GSM_DYNAMIC_SAIC
/*===========================================================================

FUNCTION gl1_hw_get_saic_status

DESCRIPTION
   returns state of SAIC ON,OFF

===========================================================================*/
boolean gl1_hw_get_saic_state(gas_id_t gas_id);
#endif

/*===========================================================================

FUNCTION gl1_hw_cfg_saic

DESCRIPTION
   This allows SAIC to be only active for dedicated mode.

===========================================================================*/
void gl1_hw_cfg_saic( boolean state,gas_id_t gas_id);

#ifdef FEATURE_GSM_MDSP_ESAIC
/*===========================================================================

FUNCTION gl1_hw_flush_esaic_iir_filter

DESCRIPTION
  Tells the mDSP to Clear it's IIR filter for use in applying the ESAIC on
  Downlink channels.

  Currently needs to be called For Downlink DTX and Handover scenarios
===========================================================================*/
void gl1_hw_flush_esaic_iir_filter (boolean flush_esacch, boolean flush_ersacch,gas_id_t gas_id);

#endif /* FEATURE_GSM_MDSP_ESAIC */

/*===========================================================================
               FUNCTIONS TO SUPPORT AFC
===========================================================================*/
  /* Max amount to move from the rotator to the pdm at a time in Hz, we
     want some small increment to allow for error in the PDM slope. */
  #define GL1_HW_MAX_ROTATOR_TO_PDM     (50)
  /* The maximum correction we allow on the rotators in Hz */
  #define GL1_HW_MAX_ROTATOR_CORRECTION (500)

/* The maximum distance (Hz) from the default PDM value we can be at and
   still decode all possible tones.  The pull-in range for acquisition
   is about 11 kHz, so use half that for safety */
#define GL1_HW_MAX_SAFE_FREQ_OFFSET     (5500)

void   gl1_hw_acq_afc_init(gl1_defs_acq_afc_type *data,gas_id_t);
void   gl1_hw_acq_afc_on_vctcxo(boolean active,gas_id_t gas_id);

void gl1_hw_set_vctcxo_pdm_val( int32            pdm_val,
                                chipxn_control_t TurnClockOff,gas_id_t gas_id  );

int32 gl1_hw_gen_set_vctcxo_pdm_val( int32            pdm_val,
                                    chipxn_control_t  TurnClockOff,
                                    gas_id_t          gas_id );

int32 gl1_hw_get_vctcxo_pdm_val( gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_hw_get_tcxomgr_pdm_default_val

DESCRIPTION
  Returns the default PDM value
===========================================================================*/
int32 gl1_hw_convert_tcxomgr_pdm_val(tcxomgr_vco_type vco_value);
tcxomgr_rotator_type gl1_hw_get_afc_rot_val( void );

int32 gl1_hw_calc_phase_increment(ARFCN_T arfcn,gas_id_t gas_id);
void  gl1_hw_set_afc_band(sys_band_T band, boolean dedicated_mode,gas_id_t gas_id);

void  gl1_hw_afc_locked(boolean is_idle,gas_id_t gas_id);

#ifdef FEATURE_WCDMA
int32 gl1_hw_calc_phase_increment_wcdma(void);

extern int32 gl1_hw_calc_ppm_w2g( void );
extern void  gl1_hw_g2w_set_frontend_rot( uint16 W_chn );
#endif

int32 gl1_hw_get_rotator_val(gas_id_t gas_id);
void  gl1_hw_set_rotator_val(int32 rot_val,gas_id_t gas_id);
void  gl1_hw_store_rotator_val(int32 new_rot_val,gas_id_t gas_id);


extern mdsp_rf_desense_chan_type gl1_hw_is_arfcn_xo_desense( ARFCN_T arfcn, gas_id_t gas_id );

extern void  gl1_hw_set_xo_acc_freq_err( int32 xo_value,gas_id_t gas_id );
extern int32 gl1_hw_get_xo_acc_freq_err_in_ppm( gas_id_t gas_id );
extern int32 gl1_hw_get_xo_acc_freq_err( gas_id_t gas_id );
extern void  gl1_hw_update_xo_acc_freq_err( int32 xo_freq_err,gas_id_t gas_id );
extern void gl1_msg_set_acq_xo_value( int32 xo_value,gas_id_t gas_id );

extern int32 gl1_hw_normxofreqtofreq( ARFCN_T arfcn, int32 norm_xo_freq, gas_id_t gas_id );
extern int32 gl1_hw_freqtonormxofreq( ARFCN_T arfcn, int32 fine_freq, gas_id_t gas_id );

extern int32 gl1_hw_convert_normxofreq_to_rot( ARFCN_T arfcn, int32 xo_val,gas_id_t gas_id );
extern int32 gl1_hw_convert_normcurrentxofreq_to_rot( ARFCN_T arfcn,gas_id_t gas_id );

extern void  gl1_hw_set_active_cycle_swallower_adj( ARFCN_T arfcn,gas_id_t gas_id );
extern void  gl1_hw_gfw_config_cycle_swallower( gas_id_t gas_id );
void gl1_hw_set_gfw_config_cycle_valid_state(gas_id_t gas_id, boolean isValid);
extern boolean gl1_hw_get_mdsp_freq_config_data ( gl1_hw_mdsp_freq_t *freq_config_data,gas_id_t gas_id );

extern void gl1_hw_set_xo_rx_config( ARFCN_T arfcn ,gas_id_t gas_id);
extern void gl1_hw_set_afc_mode(gl1_hw_acq_afc_type AFC);

/* Conversion routines that use PDM slope and band frequency */
int32 gl1_hw_phasetohz(int32 phase);
int32 gl1_hw_hztophase(int32 hz);
int32 gl1_hw_pdmtohz(int32 pdm,gas_id_t gas_id);
int32 gl1_hw_hztopdm(int32 hz,gas_id_t gas_id);

int32 gl1_hw_get_round_val( int32 input_val, int32 multiplier_val,
                            int32 divisor_val );

/*===========================================================================
               FUNCTIONS TO SUPPORT LOGGING
===========================================================================*/

/*===========================================================================
FUNCTION gl1_log_afc_adjust

DESCRIPTION
  This function is called to log any adjustments made through AFC.
===========================================================================*/
typedef enum
{
  LOG_AFC_TRACKING,
  LOG_AFC_ROTATOR_TO_PDM,
  LOG_AFC_ACQ_RESET,
  LOG_AFC_ACQ_ON_PDM,
  LOG_AFC_ACQ_RESULTS,
  LOG_AFC_BAND_CHANGE,
  LOG_AFC_CM_STARTUP,
  LOG_AFC_ACQ_ON_XO
} gl1_log_afc_adjust_type;

void gl1_log_afc_adjust
(
  int32 pdm_adjust,
  int32 rot_adjust,
  gl1_log_afc_adjust_type type,
  gas_id_t gas_id
);

void gl1_hw_log_monitor
 (
   dBx16_T          rx_power,
   uint32           rssi_value,
   rf_gain_range_T  rx_gain_range,
   ARFCN_T          arfcn,
   boolean          rx_yield,
   boolean          immediate_log,
   gas_id_t         gas_id
 );

extern uint16 arfcn_to_word( ARFCN_T arfcn );

extern void gl1_hw_log_detected_fcch(
             ARFCN_T              arfcn,
             GfwFcchResultStruct *fcch_data_p,
             gas_id_t             gas_id );

extern void gl1_hw_log_decoded_sch_data(
             ARFCN_T             arfcn,
             GfwSchResultStruct *sch_data_p,
             gas_id_t            gas_id );

extern void gl1_hw_log_parallel_sch_data(
             ARFCN_T             arfcn,
             GfwPllelSchLogPckt *par_sch_data_p,
             gas_id_t            gas_id );

extern gl1_hw_sch_decode_t gl1_hw_get_sch_decode_mode(
                             boolean sch_decode );
extern gl1_hw_sch_decode_t gl1_hw_x2g_get_sch_decode_mode(
                             boolean sch_decode );

/*===========================================================================

FUNCTION  gl1_hw_log_async_rx_data

DESCRIPTION
  Logs an async rx metrics packet.

===========================================================================*/
extern void gl1_hw_log_async_rx_data( gl1_defs_metrics_rpt *rpt,gas_id_t gas_id );

#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
#error code not present
#endif

void gl1_hw_log_mdsp_debug_buffer (uint8 TSC,gas_id_t gas_id);

void gl1_hw_log_saic_metrics (gas_id_t gas_id);
boolean gl1_hw_feature_disabled_saic (gas_id_t gas_id);

#ifdef FEATURE_GSM_MDSP_ESAIC
void gl1_hw_log_gprs_esaic_metrics (uint16 timeslot,gas_id_t gas_id);
boolean gl1_hw_feature_disabled_esaic (gas_id_t gas_id);
#endif

void gl1_hw_log_aeq_metrics (uint16 timeslot, uint16 tag,boolean packet_mode,gas_id_t gas_id);

#ifdef FEATURE_GSM_MDSP_AEQ_SCH
void gl1_hw_log_aeq_sch_metrics (GfwBurstMetricsStruct *metrics,gas_id_t gas_id);
#endif


#ifdef FEATURE_AEQ_PHASE4
void gl1_hw_log_aeq4_metrics (GfwAeq4LogPacket *datarcvd,gas_id_t gas_id);
#endif

boolean gl1_hw_feature_disabled_aeq (gas_id_t gas_id);

#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
boolean gl1_hw_feature_disabled_aeq_8psk (gas_id_t gas_id);
#endif

#ifdef FEATURE_GSM_MDSP_AEQ_SCH
boolean gl1_hw_feature_disabled_aeq_sch (gas_id_t gas_id);
#endif



void gl1_hw_log_cell_cmds_to_diag (gl1_hw_cmd_types cmd_type, boolean dump,gas_id_t gas_id);

extern void gl1_hw_log_gfn_adjust( uint32 old_FN, uint32 FN_diff );

/*===========================================================================
FUNCTION gl1_hw_log_comb_acq

DESCRIPTION
  This function should be called when combined acquisition is done.
  It will send a log packet.
===========================================================================*/
enum
{
   GL1_HW_LOG_ACQ_START,
   GL1_HW_LOG_ACQ_SUCCESS,
   GL1_HW_LOG_ACQ_FAIL_FCCH,
   GL1_HW_LOG_ACQ_FAIL_SCH,
   GL1_HW_LOG_ACQ_RESERVED,
   GL1_HW_LOG_ACQ_ABORTED
};

void gl1_hw_log_comb_acq
 (
   uint8 mode,
   ARFCN_T arfcn,
   dBx16_T rx_power,
   gas_id_t gas_id
 );

/*===========================================================================

FUNCTION gl1_hw_cleanup_early_decode

DESCRIPTION
  Clean up drivers after succesful early decode.  This is necessay because
  an extra rx burst command will have been issued but not acted on.

DEPENDENCIES
  An early decode must have just been completed.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_cleanup_early_decode( gl1_hw_rx_tags_type *tags,gas_id_t gas_id );


/*===========================================================================
               DECLARATIONS TO SUPPORT DRX / SLEEP
===========================================================================*/

void gl1_hw_enter_idle (gas_id_t gas_id);
void gl1_hw_leave_idle (gas_id_t gas_id);


boolean gl1_hw_turn_tx_off(boolean rf_disable_tx,gas_id_t gas_id);

typedef void (*gl1_hw_sleep_gsm_cb_type)( gas_id_t gas_id );

void gl1_hw_sleep_gsm_enter_idle( gas_id_t gas_id );
void gl1_hw_sleep_gsm_leave_idle( gas_id_t gas_id );
boolean gl1_hw_sleep_gsm_start_sleep( uint32 frames_to_sleep, uint32 frames_awake, boolean check_too_early, uint32 wakeup_fn, gas_id_t gas_id );
uint32 gl1_hw_sleep_gsm_force_wakeup( gas_id_t gas_id );
void gl1_hw_sleep_gsm_register_start_cb( gl1_hw_sleep_gsm_cb_type cb, gas_id_t gas_id );
void gl1_hw_sleep_gsm_register_warmup_cb( gl1_hw_sleep_gsm_cb_type cb, gas_id_t gas_id );
void gl1_hw_sleep_set_terminal_count(gas_id_t gas_id);
void gl1_hw_turn_tx_on( gas_id_t gas_id );
void gl1_hw_sleep_start_from_drx( gas_id_t gas_id );
boolean gl1_hw_wakeup_complete( gas_id_t gas_id );
void gl1_hw_sleep_set_missed_frames( gas_id_t gas_id, uint8 num_frames );
slpc_id_type gl1_hw_slpc_from_gas_id( gas_id_t gas_id );
slpc_id_type gl1_hw_wcdma_slpc_from_gas_id( gas_id_t gas_id );
uint8 gl1_hw_sleep_get_missed_frames( gas_id_t gas_id );
void  gl1_hw_sleep_clear_missed_frames( gas_id_t gas_id  );
boolean gl1_hw_sleep_get_forcedwakeup(gas_id_t gas_id);
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
void gl1_hw_set_rf_in_deep_sleep_whilst_forced_wakeup(gas_id_t gas_id, boolean flagVal);
boolean gl1_hw_get_rf_in_deep_sleep_whilst_forced_wakeup(gas_id_t gas_id);
#endif
uint32 gl1_get_time_next_vsmtr_us(gas_id_t gas_id);
boolean gl1_hw_sleep_get_entered_recovery(gas_id_t gas_id);
void gl1_hw_sleep_set_entered_recovery(gas_id_t gas_id, boolean enteredRecovery);
boolean gl1_hw_calc_gsm_time_from_sclk (uint32 *fn, uint32 *qsym, gas_id_t gas_id);
int32 gl1_hw_sleep_warmup_dur_in_sclks(gas_id_t gas_id);
trm_time_t gl1_hw_sleep_wmgr_calc_grm_when_time(uint32 requested_FN, gas_id_t gas_id);

#ifdef FEATURE_SLPC_TRIPLE_SIM
/* Nbr timeline to be maintained while in sleep */
typedef enum {
   WCDMA,
   TDS
} gl1_hw_alt_id_T;
#endif /* FEATURE_SLPC_TRIPLE_SIM */


#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
#ifdef FEATURE_SLPC_TRIPLE_SIM
void gl1_hw_sleep_maintain_ref_count(  gl1_hw_alt_id_T alt_id, boolean maintain_ref_count, gas_id_t gas_id );
#else
void gl1_hw_sleep_maintain_ref_count( boolean maintain_ref_count, gas_id_t gas_id );
#endif /* FEATURE_SLPC_TRIPLE_SIM */
#endif

void gl1_hw_sleep_set_rf_cnf_pending( boolean rf_cnf_pending, gas_id_t gas_id );
boolean gl1_hw_sleep_get_rf_cnf_pending( gas_id_t gas_id );

#ifdef FEATURE_QTA
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
                              uint32 offset,
                              boolean in_qbta,
                              ARFCN_T arfcn,
                              boolean interband_monitors,
                              boolean is_first_burst,
                              gas_id_t gas_id);


#if defined ( FEATURE_QTA) && defined ( FEATURE_CXM_QTA)
void gl1_hw_set_rx_cmd_issued_in_qta(boolean flag, gas_id_t gas_id);
#endif

#if defined(FEATURE_QBTA)
/*===========================================================================

FUNCTION  gl1_hw_check_in_QBTA

DESCRIPTION
  This function whether QTA or QBTA will be done.

===========================================================================*/
boolean gl1_hw_check_in_QBTA(gas_id_t gas_id);

/*===========================================================================
FUNCTION  gl1_hw_set_QBTA_flag

DESCRIPTION
  This function sets  QBTA flag.
===========================================================================*/
void gl1_hw_set_QBTA_flag(gas_id_t gas_id, boolean val);

/*===========================================================================

FUNCTION  gl1_hw_fill_next_qta_gap_params

DESCRIPTION
  This function is called from GL1 to set the G idle sub reserve time in SCLKs .

INPUT PARAMETERS
  start time and duartion in SLCKS
===========================================================================*/
void gl1_hw_fill_next_qta_gap_params( uint32 when,gas_id_t gas_id);
#endif /*FEATURE_QBTA*/

/*===========================================================================

FUNCTION  gl1_hw_qta_fw_rf_deinit

DESCRIPTION
  This function deinitializes the firmware and RF after QTA operation, this
  must called before releasing TRM.

===========================================================================*/
void gl1_hw_qta_fw_rf_deinit(gas_id_t gas_id, uint32 offset, boolean in_qbta);

boolean gl1_hw_get_qta_cm_fw_rf_init_status (gas_id_t gas_id);

void gl1_hw_set_qta_cm_fw_rf_init_status ( gas_id_t gas_id, boolean status );

#if defined FEATURE_QBTA
boolean gl1_hw_get_qbta_scripts_status(gas_id_t gas_id);
void gl1_hw_set_qbta_scripts_status(gas_id_t gas_id, boolean status);
#endif

boolean gl1_hw_cm_get_qta_wcdma_has_initialised(gas_id_t gas_id);
void gl1_hw_cm_set_qta_wcdma_has_initialised(boolean val, gas_id_t gas_id);
void gl1_hw_cm_set_qta_gap_for_g2x_irat(boolean val, gas_id_t gas_id);
void gl1_hw_reset_qta_flags_skipping_gap(gas_id_t gas_id);

#endif /* FEATURE_QTA */

/*===========================================================================

FUNCTION  gl1_hw_qta_gap_active

DESCRIPTION
  This function returns an indication of whether a QTA gap is currently open on that particular sub

===========================================================================*/
boolean gl1_hw_qta_gap_active(gas_id_t gas_id);

/*===========================================================================
FUNCTION  gl1_hw_get_qta_gap_for_g2x_irat

DESCRIPTION
  This function returns an indication of whether a QTA gap is open g2x irat on that particular sub

===========================================================================*/
boolean gl1_hw_get_qta_gap_for_g2x_irat(gas_id_t gas_id);

/*===========================================================================
FUNCTION  gl1_hw_get_gsm_timing_info
DESCRIPTION
  Returns GSM timing information (not RF timing).

INPUT PARAMETERS
  Pointer to structure for results
===========================================================================*/
void gl1_hw_get_gsm_timing_info( gl1_x2g_timing_info_t  *gl1_x2g_info );


#if defined( FEATURE_INTERRAT_HANDOVER_GTOW ) || defined( FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
#if defined FEATURE_WCDMA

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
extern void gl1_hw_wcdma_save_g2w_timing_params(wl1_x2w_rf_timing_params_struct_type *timing_params,
                                                gas_id_t gas_id);

extern void gl1_hw_g2w_send_rf_enter_ind(uint16 search_uarfcn,
                                         gas_id_t gas_id);

extern void gl1_hw_g2w_send_rf_exit_req(boolean abort,
                                        gas_id_t gas_id);

extern boolean gl1_hw_g2w_check_if_using_rf(gas_id_t gas_id);
extern void gl1_hw_g2w_set_done_with_rf(gas_id_t gas_id);

extern void gl1_hw_g2x_mdsp_start_wcdma_rf_cmd(uint16 start_time,
                                               gas_id_t gas_id);

extern void gl1_hw_g2x_mdsp_rf_sample_ram_cmd(uint16 start_time,
                                              gas_id_t gas_id);

extern void gl1_hw_g2w_send_pre_search_req(gas_id_t gas_id);

extern void gl1_hw_g2w_search(
  uint16                        search_uarfcn,
  uint8                         start_time_frame_offset,
  uint16                        start_time_qs,
  wl1_x2w_mode_enum_type        search_mode,
  wl1_x2w_srch_input_union_type *cell_search_params,
  gas_id_t                      gas_id);

#else /* FEATURE_GSM_TO_WCDMA_MSGR */

void gl1_hw_start_wcdma_rf_cmd( uint16 W_chn, uint16 start_time,gas_id_t gas_id );
void gl1_hw_rf_sample_ram_cmd( uint16 W_chn, uint16 start_time,gas_id_t gas_id);
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
#endif
void gl1_hw_setup_wcdma_rx(void);
void gl1_hw_reset_wcdma_rx(gas_id_t gas_id);
void gl1_hw_get_rf_off_script_offsets(uint16 *sbi_addr, uint16 *non_sbi_addr,gas_id_t gas_id);
#ifndef FEATURE_GSM_TO_WCDMA_MSGR
void gl1_hw_set_wcdma_srch_on( boolean wsrch_on );
boolean gl1_hw_is_wcdma_srch_on( void );
#endif /* !FEATURE_GSM_TO_WCDMA_MSGR */

#if defined(FEATURE_GSM_TO_LTE) || defined(FEATURE_GSM_TDS)
void  gl1_hw_g2x_startup( uint16   time_offset,gas_id_t gas_id );
#endif
void  gl1_hw_g2x_cleanup( boolean  tuneback_to_gsm,boolean immed_cleanup, gas_id_t gas_id );


void    gl1_hw_g2x_cleanup_done_handler( gas_id_t gas_id );

boolean gl1_hw_get_g2x_rf_enter_called_last(gas_id_t gas_id);
void    gl1_hw_set_g2x_rf_enter_called_last(boolean value, gas_id_t gas_id);

#ifdef FEATURE_GSM_TDS
void  gl1_hw_g2t_startup( uint32   time_offset,gas_id_t gas_id );

void gl1_hw_g2t_startup_decrement( gas_id_t gas_id );
void gl1_hw_g2t_cleanup_decrement( gas_id_t gas_id );

void  gl1_hw_g2t_cleanup(uint32   time_offset,gas_id_t gas_id );

void gl1_hw_calc_gap_values( gsm_l1_tds_gap_info_type *gap_info_ptr,gas_id_t gas_id );

#endif /*FEATURE_GSM_TDS*/

#endif /* ..INTERRAT.. */

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
void gl1_hw_register_panic_cb(gl1_hw_panic_cb_type cb,gas_id_t gas_id);

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
void gl1_hw_handle_panic(gas_id_t gas_id);

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
void gl1_hw_handle_sleep_panic(gas_id_t gas_id);



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
void    gl1_hw_set_gsm_only_mode( boolean gl1_gsm_only_mode,gas_id_t gas_id );
boolean gl1_hw_get_gsm_only_mode( gas_id_t gas_id );

#ifdef FEATURE_SGLTE
/*===========================================================================

FUNCTION gl1_hw_set/get_sglte_mode

DESCRIPTION
  Store/Return the status of the local sglte status.

DEPENDENCIES
  None

RETURN
  None/Current sglte value

SIDE EFFECTS
  None
===========================================================================*/
void    gl1_hw_set_sglte_mode( boolean gl1_sglte_mode,gas_id_t gas_id );
boolean gl1_hw_get_sglte_mode( gas_id_t gas_id );
boolean gl1_hw_get_sglte_plus_g_mode(gas_id_t gas_id);
#endif

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
uint16 gl1_hw_acch_get_nv_flag (gas_id_t gas_id);

/* Value to use for undefined table entries */
#define GL1_VCO_UNDEFINED             -32768

/* Value to set the GL1 default PDM to if no valid values are available */
#define GL1_VCO_UNDEFINED_SETTING     -1

#ifdef FEATURE_GSM_GPRS_TX_INFO
/*===========================================================================

FUNCTION gl1_hw_save_tx_arfcn

DESCRIPTION
  ARFCN_T *arfcn.

DEPENDENCIES
  None

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_save_tx_arfcn (ARFCN_T *arfcn,gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_hw_get_current_tx_arfcn

DESCRIPTION
  ARFCN_T *arfcn.

DEPENDENCIES
  None

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_get_current_tx_arfcn (ARFCN_T *arfcn,gas_id_t gas_id);

#endif

/*===========================================================================
FUNCTION gl1_hw_get_current_rgs_val

DESCRIPTION
  Returns the current rgs PDM value
===========================================================================*/
int32 gl1_hw_get_current_rgs_val (void);

/*===========================================================================

FUNCTION  gl1_msg_set_tcxo_reset_pending

DESCRIPTION
  This function sets the tcxo_reset_pending flag

DEPENDENCIES

PARAMETERS
  boolean tcxo_reset_pending_value - value to set
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
extern void gl1_msg_set_tcxo_reset_pending( boolean tcxo_reset_pending_value ,gas_id_t gas_id);

/*===========================================================================
FUNCTION gl1_hw_acq_tcxo_and_rotator_init

DESCRIPTION
  This function initializes the vctcxo parameters.
===========================================================================*/
void gl1_hw_acq_tcxo_and_rotator_init (gas_id_t gas_id);
extern int16 gl1_hw_vco_default;

void gl1_hw_pass_xmsi_to_mdsp( boolean  imsi_valid,  uint8 *imsi_ptr,
                               boolean  tmsi_valid,  uint8 *tmsi_ptr,
                               boolean  ptmsi_valid, uint8 *ptmsi_ptr,
                               gas_id_t gas_id );

uint16 gl1_hw_sbd_get_null_pattern (gas_id_t gas_id);

/*===========================================================================
FUNCTION  gl1_hw_enable_bbrx_clock_gating

DESCRIPTION
  This function enables or disables BB Rx clock gating
===========================================================================*/
void gl1_hw_enable_bbrx_clock_gating ( boolean enable_bbrx_clk_gating );

/*===========================================================================
FUNCTION gl1_hw_store_rx_power
DESCRIPTION
  This function stores RSSI and Gain for use by FTM mode

===========================================================================*/
void gl1_hw_ftm_store_rx_power(uint32 rssi, dBx16_T pwr_dbm,
                               uint32 rssi_divrx,dBx16_T pwr_dBm_x16_divrx,
                               gas_id_t gas_id);

/*===========================================================================
FUNCTION gl1_hw_set_ftm_power_store_init

DESCRIPTION
  This function stores RSSI and Gain for use by FTM mode
===========================================================================*/
void gl1_hw_set_ftm_power_store_init(boolean ftm_power_store_init,gas_id_t gas_id);

/*===========================================================================

  Format of the EFS file is a not clear so for a 32-bit hex value in format
  0xabcdefgh is reads in as 0xghefcdab.

  Use top bits (gh) for gas_id distinction, so will enable
  the same debug options for any enabled gas_id's.

  As an example a hex value of 0xD0000060 is interpreted as, and therefore
  enables selected options for GAS_ID_1/2:

    0x60 (gh) =>
    ( ( 0x1 << GERAN_ACCESS_STRATUM_ID_1 ) |
      ( 0x1 << GERAN_ACCESS_STRATUM_ID_2 ) )
    << ( 31 - 2 )

    0xD0 (ab)=>
    ( GL1_EFS_SLP_DISABLE_ASYNCH_DEBUG |
      GL1_EFS_SLP_DISABLE_ASYNCH_SLEEP |
      GL1_EFS_SLP_DISABLE_SBD )

===========================================================================*/

#define GL1_EFS_DEBUG_GAS_ID_SHFT           ( 31 - GERAN_ACCESS_STRATUM_ID_3 )
#define GL1_EFS_DEBUG_GAS_ID_N( gAS_iD )    ( ( 0x1 << ( gAS_iD ) ) \
                                               << ( GL1_EFS_DEBUG_GAS_ID_SHFT ) )

/*         *** BITMASK FOR 'l1_debug' EFS FILE *** */
/* Functionality controlled by each bit should be unique.
 * Each bit should control the same functionality across all targets.
 * Always keep this in order to avoid duplicate settings when mergin etc
 * and be aware that the top couple of bits are gas_id.
 * Don't use FEATURES around these to avoid problems.
 */
#define GL1_EFS_DEBUG_XCCH                      0x00000008
#define GL1_EFS_DEBUG_COEX_MODE_B0              0x00000200
#define GL1_EFS_DEBUG_COEX_MODE_B1              0x00000400
#define GL1_EFS_DEBUG_COEX_MODE_B2              0x00000800
#define GL1_EFS_DEBUG_XCCH_IN_XFER              0x00001000
#define GL1_EFS_DEBUG_DISABLE_CCCH_IN_XFER      0x00002000
#define GL1_EFS_DEBUG_FORCE_ENABLE_CCCH_IN_XFER 0x00004000
//To disable Fallback to SR from DR
#define GL1_EFS_DEBUG_DISABLE_HYBRID_FB_FRM_DR  0x00008000
#define GL1_EFS_DEBUG_BXCCH_IN_XFER             0x00010000
#define GL1_EFS_DEBUG_DISABLE_DRX_DEBUG_CRASH   0x00020000
#define GL1_EFS_DEBUG_DISABLE_REDUCED_NCELL     0x00040000
#define GL1_EFS_DEBUG_ENABLE_GTA_PS_RACH        0x00080000
#define GL1_EFS_DEBUG_DISABLE_G21X_TA_RRBP      0x00100000
#define GL1_EFS_DEBUG_REDUCE_DRX_CYCLE          0x00800000
//To track count of TRm denials/Xcch failures to trigger fallback on
#define GL1_EFS_DEBUG_TRMDEN_FAIL_CNT_BIT0      0x00200000
#define GL1_EFS_DEBUG_TRMDEN_FAIL_CNT_BIT1      0x00400000
#define GL1_EFS_DEBUG_XCCH_IN_DRDSDS            0x02000000

#define GL1_EFS_DEBUG_DISABLE_FR                0x04000000

//To reduce excessive F3
#define GL1_EFS_DEBUG_OPTIONAL                  0x08000000
/*===========================================================================
FUNCTION  gl1_hw_efs_get_debug
===========================================================================*/
uint32 gl1_hw_efs_get_debug( gas_id_t gas_id );

/*===========================================================================
FUNCTION  gl1_hw_efs_read_debug
===========================================================================*/
void gl1_hw_efs_read_debug(void);

/*===========================================================================
FUNCTION  gl1_hw_efs_get_slp
===========================================================================*/
uint32 gl1_hw_efs_get_slp (void);

/*===========================================================================
FUNCTION  gl1_hw_efs_read_slp
===========================================================================*/
void gl1_hw_efs_read_slp(void);
/*===========================================================================
FUNCTION  gl1_hw_efs_get_slp_error_rec
===========================================================================*/
uint32 gl1_hw_efs_get_slp_error_rec ( void );


#ifdef FEATURE_VAMOS_II
boolean gl1_hw_get_vamos2_support(gas_id_t gas_id);
uint8 gl1_hw_get_vamos_nv(gas_id_t gas_id);
void gl1_hw_log_vamos2_metrics(uint8 gl1_vamos_support,boolean ShiftedSacchEn,
                                      GfwVamosLogType *VamosLogPacket,gas_id_t gas_id);
#endif

/* Parallel SCH decode enabled for X2G and/or GL1 */
extern boolean gl1_hw_is_parallel_sch_enabled( void );
extern boolean gl1_hw_is_x2g_parallel_sch_enabled( void );

/* Ta is allowed or not in NON DRX or REORG mode */
extern boolean gl1_hw_is_g2x_reorg_ta_enabled( void );
extern boolean gl1_hw_is_g2x_non_drx_ta_enabled( void );

#ifdef FEATURE_GERAN_LOG_OPT
/*===========================================================================

FUNCTION
  gl1_hw_disable_conditional_logging

DESCRIPTION
  Get the status of NV setting to disable conditional logging

RETURN VALUE
   disbale/enable conditional logging
SIDE EFFECTS
  
===========================================================================*/
extern uint8 gl1_hw_disable_conditional_logging( void );
#endif

/*===========================================================================
FUNCTION  gl1_hw_efs_get_q6_clocks
===========================================================================*/
uint32 gl1_hw_efs_get_q6_clocks (void);

/*===========================================================================
FUNCTION  gl1_hw_non_blocking_delay
===========================================================================*/
void gl1_hw_delay (uint16 delay,gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_hw_set/get_mdsp_asynch_int

DESCRIPTION
  Returns the asynch PCH mdsp int trigger.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void    gl1_hw_set_mdsp_asynch_int( boolean asynch_int_enable, gas_id_t gas_id );
extern boolean gl1_hw_get_mdsp_asynch_int( gas_id_t gas_id );

/*===========================================================================

FUNCTION gl1_hw_is_asynch_int_possible

DESCRIPTION
  Returns the decision if the asynch PCH mdsp int should even be enabled.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern boolean gl1_hw_is_asynch_int_possible( gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_hw_get_gsm_rxlm_buf_idx
DESCRIPTION
  Returns gl1_gsm_rxlm_buf_idx
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
uint32 gl1_hw_get_gsm_rxlm_buf_idx( gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_hw_set_gsm_rxlm_buf_idx
DESCRIPTION
  Set gl1_hw_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx 
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_set_gsm_rxlm_buf_idx( uint32 gl1_gsm_rxlm_buf_idx ,gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_hw_get_lte_rxlm_buf0_idx
DESCRIPTION
  Returns gl1_gsm_rxlm_buf0_idx
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
uint32 gl1_hw_get_lte_rxlm_buf0_idx( gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_hw_get_lte_rxlm_buf1_idx
DESCRIPTION
  Returns gl1_lte_rxlm_buf1_idx
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
uint32 gl1_hw_get_lte_rxlm_buf1_idx( gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_hw_deallocate_rxlm_buf_idx
DESCRIPTION
  Returns none
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_deallocate_rxlm_buf_idx( gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_hw_deallocate_txlm_buf_idx
DESCRIPTION
  Returns None
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_deallocate_txlm_buf_idx(gas_id_t gas_id);

/*===========================================================================
FUNCTION gl1_hw_allocate_wcdma_rxlm_buf
DESCRIPTION
  Dealloactes the WCDMA RxLM buffer
  Returns none
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_allocate_wcdma_rxlm_buf( gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_hw_deallocate_wcdma_rxlm_buf
DESCRIPTION
  Returns none
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_deallocate_wcdma_rxlm_buf( gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_hw_allocate_lte_rxlm_buf
DESCRIPTION
  Dealloactes the WCDMA RxLM buffer
  Returns none
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_allocate_lte_rxlm_buf( gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_hw_deallocate_lte_rxlm_buf
DESCRIPTION
  Returns none
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_deallocate_lte_rxlm_buf( gas_id_t gas_id );

#ifdef FEATURE_GSM_TDS
/*===========================================================================
FUNCTION gl1_hw_allocate_tds_rxlm_buf
DESCRIPTION
  Dealloactes the WCDMA RxLM buffer
  Returns none
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_allocate_tds_rxlm_buf( void );

/*===========================================================================
FUNCTION gl1_hw_deallocate_tds_rxlm_buf
DESCRIPTION
  Returns none
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_deallocate_tds_rxlm_buf( void );

/*===========================================================================
FUNCTION gl1_hw_get_tds_rxlm_buf0_idx
DESCRIPTION
  Returns gl1_tds_rxlm_buf0_idx
DEPENDENCIES
  None
SIDE EFFECTS
  None
===========================================================================*/
uint32 gl1_hw_get_tds_rxlm_buf0_idx( void );

#endif

/*===========================================================================

FUNCTION gl1_hw_get_dtm_expected_power

DESCRIPTION
   returns current expected Rx power level

===========================================================================*/
int32 gl1_hw_get_dtm_expected_power(gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_hw_get_dtm_band

DESCRIPTION
   returns current band

===========================================================================*/
uint8 gl1_hw_get_dtm_band(gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_hw_dtm_get_tx_channel_type

DESCRIPTION
  This the callback used by the arbitration interface to get the tx channel
  type of the arbitrated frame.

===========================================================================*/
gl1_hw_channel_type gl1_hw_dtm_get_tx_channel_type(uint8 tn,gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_hw_dtm_get_rx_channel_type

DESCRIPTION
  This the callback used by the arbitration interface to get the rx channel
  type of the arbitrated frame.

===========================================================================*/
gl1_hw_channel_type gl1_hw_dtm_get_rx_channel_type(uint8 tn,gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_hw_get_dtm_arfcn

DESCRIPTION
   returns current ARFCN

===========================================================================*/
uint16 gl1_hw_get_dtm_arfcn(gas_id_t gas_id);

/*===========================================================================
FUNCTION gl1_hw_set_rf_gsm_rx_times
DESCRIPTION
  Sets RF Rx timing, used for QTA/IRAT
===========================================================================*/
void gl1_hw_set_rf_gsm_rx_time(int32 rx_setup_time, int32 idle_rx_setup_time, gas_id_t gas_id);
/*===========================================================================
FUNCTION gl1_hw_get_rf_gsm_rx_setup_time
DESCRIPTION
  Returns Rx Rf set-up time
===========================================================================*/
int32 gl1_hw_get_rf_gsm_rx_setup_time( gas_id_t gas_id );
/*===========================================================================
FUNCTION gl1_hw_get_rf_gsm_mon_setup_time
DESCRIPTION
  Returns Monitor Rf set-up time
===========================================================================*/
int32 gl1_hw_get_rf_gsm_mon_setup_time( gas_id_t gas_id );

void gl1_hw_inact_deep_sleep( boolean rat_change_ind, gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_hw_get_rf_gsm_tx_setup_time
DESCRIPTION
  Returns Tx Rf set-up time
===========================================================================*/
int32 gl1_hw_get_rf_gsm_tx_setup_time( gas_id_t gas_id );
#ifdef FEATURE_VSWR
int32 gl1_hw_get_rf_gsm_tx_vswr_setup_time( gas_id_t gas_id );
#endif /*FEATURE_VSWR*/
uint16 gl1_hw_get_rf_info_rf_warmup_time( gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_hw_get_rf_gsm_warmup_time
DESCRIPTION
  Returns Tx Rf warm up time
===========================================================================*/
uint32 gl1_hw_get_rf_gsm_warmup_time( gas_id_t gas_id );

/*===========================================================================
FUNCTION  gl1_hw_rf_rx_burst
DESCRIPTION
  Send MSGR message to RF Task for Rx burst processing
===========================================================================*/
void gl1_hw_rf_rx_burst(  uint16                  num_rx_bursts,
                          rfgsm_rx_chan_params_type rx_chan_params[],
                          rfgsm_rx_diversity_chan_params_type div_rx_chan_params[],
                          boolean                enableRxDiv,
                          mdsp_burst_mode        mdsp_burst_mode,
                          gas_id_t               gas_id );

/*===========================================================================
FUNCTION  gl1_hw_rf_update_freq_error
DESCRIPTION
  Keeps a store of the current frequency error to be applied via DPLL
===========================================================================*/
void gl1_hw_rf_update_freq_error(int32 freq_err,gas_id_t gas_id);

/*===========================================================================
FUNCTION  gl1_hw_g2w_wait_rf_buffer_completion
DESCRIPTION
  Called to ensure that all pending GSM RF CCS buffers have been populated in RF Task context
  before calling RF inline to populate G2W startup CCS buffer
===========================================================================*/
void gl1_hw_g2x_wait_rf_buffer_completion(gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_stop_acquisition

DESCRIPTION
  This function stops the tone detection at the specified offset into the
  frame. The offset does not include the DSP's frame tick setup margin

DEPENDENCIES
  This function should only be called after starting a tone detection
  by invoking gl1_hw_start_acquisition

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void gl1_hw_stop_acquisition_geran(uint16 offset,gas_id_t gas_id);

/*=============================================================================
FUNCTION gl1_hw_setup_tx_power_control

DESCRIPTION
   Configures the RF and mDSP transmit power control parameters.

============================================================================= */
void gl1_hw_setup_tx_power_control
(
   uint8                    num_pdch,
   uint16                   *power_index,
   /* Modulation needed for power backoff. */
   gl1_defs_modulation_type *modulation,
   gas_id_t                 gas_id
);

void gl1_hw_turn_tx_on( gas_id_t gas_id);
void gl1_task_mutex_lock( gas_id_t gas_id );
void gl1_task_mutex_unlock( gas_id_t gas_id );

boolean gl1_hw_has_reset_occurred ( gas_id_t gas_id );
void gl1_ftm_reset_edge_core( gas_id_t gas_id  );

/*===========================================================================

FUNCTION gl1_hw_cleanup_async_rx

DESCRIPTION
  Clean up drivers after an async rx.  Used to make aborting and early
  decoding cleanup simpler for the message layer.

===========================================================================*/
void gl1_hw_cleanup_async_rx_geran (gas_id_t gas_id);


void gl1_hw_rf_task_exit_mode(  gas_id_t gas_id,
                rfgsm_exit_mode_type exit_mode,
                rfm_device_enum_type gsm_rf_id);

/*===========================================================================
FUNCTION gl1_hw_get_gsm_rf_id
DESCRIPTION
  Returns the RF resource ID for GSM (currently all bands the same ID)
===========================================================================*/
rfm_device_enum_type gl1_hw_get_gsm_rf_id(grm_client_enum_t grm_client_id,
                                          gas_id_t          gas_id );

#ifdef FEATURE_GSM_RX_DIVERSITY
/*===========================================================================
FUNCTION gl1_hw_deallocate_rxlm_buf_idx_DivRx
DESCRIPTION
  De-allocates the RxDiv RxLM buffer
===========================================================================*/
void gl1_hw_deallocate_rxlm_buf_idx_DivRx( gas_id_t gas_id );

#endif

/*===========================================================================
FUNCTION gl1_hw_set_gsm_rf_id
DESCRIPTION
  Set the rf device to be used by the sub
===========================================================================*/
void gl1_hw_set_gsm_rf_id(rfm_device_enum_type rfm_device,
                          grm_client_enum_t grm_client_id,
                          boolean force_set_rfm_device,
                          gas_id_t gas_id);

/*===========================================================================
FUNCTION gl1_hw_reset_gsm_rf_ids
DESCRIPTION
  Reset rf devices (Rx, TX, DRX) to RFM_INVALID_DEVICE used by SUB.
===========================================================================*/
void gl1_hw_reset_gsm_rf_ids(grm_client_enum_t grm_client_id,
                             gas_id_t gas_id);

#ifdef FEATURE_WCDMA
/*===========================================================================
FUNCTION gl1_hw_get_wcdma_rf_id
DESCRIPTION
  Returns the RF resource ID for WCDMA Neighbour
===========================================================================*/
rfm_device_enum_type  gl1_hw_get_wcdma_rf_id(uint16 W_band, gas_id_t gas_id );
#endif
/*===========================================================================

FUNCTION gl1_hw_cm_get_trm_rf_device

DESCRIPTION
  Returns CM mode RF device ID
===========================================================================*/
rfm_device_enum_type gl1_hw_cm_get_trm_rf_device( gas_id_t gas_id );

errno_enum_type gl1_hw_rf_init_rfa_task_msgr_mailbox(gas_id_t gas_id);
/*===========================================================================
FUNCTION  gl1_hw_rf_idle_frame_procesing
DESCRIPTION
  Send MSGR message to RF Task to run any RF idle frame processing
===========================================================================*/
void gl1_hw_rf_idle_frame_processing(gas_id_t gas_id);

/*===========================================================================
FUNCTION  gl1_hw_get_next_rf_seq_num
DESCRIPTION
  Returns next number in sequence for RF seq num
===========================================================================*/
uint32 gl1_hw_get_next_rf_seq_num(void);

/*===========================================================================
FUNCTION  gl1_hw_rf_tx_burst
DESCRIPTION
  Send MSGR message to RF Task for Tx Burst processing
===========================================================================*/
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
                        gas_id_t                 gas_id
);
/*===========================================================================
FUNCTION  gl1_hw_rf_get_freq_error
DESCRIPTION
  Gets a copy of the current stored frequency error to be applied via DPLL
===========================================================================*/
int32 gl1_hw_rf_get_freq_error(gas_id_t gas_id);

/*===========================================================================
FUNCTION  gl1_hw_rf_set_tx_band
DESCRIPTION
  Send MSGR message to RF Task for Set Tx Band
===========================================================================*/
void gl1_hw_rf_set_tx_band(rfgsm_band_type band, gas_id_t gas_id);

/*======================================================================
FUNCTION  gl1_hw_rf_cm_enter
DESCRIPTION
  Send MSGR message to RF Task to run GSM CM enter.
===========================================================================*/
void gl1_hw_rf_cm_enter(gas_id_t gas_id);

/*===========================================================================
FUNCTION  gl1_hw_rf_cm_exit
DESCRIPTION
  Send MSGR message to RF Task to run GSM CM enter.
===========================================================================*/
void gl1_hw_rf_cm_exit(gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_rf_check_active_buffers

DESCRIPTION
  Provides ability to check if there are any outstanding RF Task activities i.e.
  whether all Rx and Tx burst requests have been completed by RF Task.
RETURN
  TRUE  - RF task still has active RF buffers to populate
  FALSE - RF Task has completed all oustanding RF buffer population
===========================================================================*/
boolean gl1_hw_rf_check_active_buffers(gas_id_t gas_id);

boolean gl1_hw_cm_get_status(gas_id_t gas_id);

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
extern void gl1_hw_complete_cm_terminate( gas_id_t gas_id );

/*===========================================================================

FUNCTION  gl1_hw_cm_set_sch_metrics_tags
DESCRIPTION
  In W2G/L2G SCH receive set the index and seq number for retrieving the result metrics

===========================================================================*/
void gl1_hw_cm_set_sch_metrics_tags(gas_id_t gas_id, ARFCN_T arfcn, uint8 mdsp_index, uint16 mdsp_seq_num );

/*===========================================================================

FUNCTION  gl1_hw_cm_store_acq_tag
DESCRIPTION
  In W2G/L2G acquisition stores the gain tags to be used for SCH results if parallel FCCH/SCH
===========================================================================*/
void gl1_hw_cm_store_acq_tag(gl1_hw_rx_tags_type *tags);
gl1_hw_rx_tags_type *gl1_hw_cm_get_acq_tag (void);

void gl1_hw_x2g_startup_geran( uint16                      offset,
                                const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr,
                                uint32                      g_frame_number,
                                boolean                     x2g_rf_rxlm_update,
                                sys_modem_as_id_e_type      as_id);
boolean gl1_hw_x2g_cleanup_geran (uint32 offset,
                                  uint32 g_frame_number,
                                  sys_modem_as_id_e_type      as_id );

void gl1_hw_cm_power_measure_geran
(
   ARFCN_T                     ARFCN[MAX_CM_RSSI_MEASUREMENTS_ARRAY_SIZE],               /* Absolute Radio Frequency Channels     */
   dBx16_T                     rx_signal_strength[MAX_CM_RSSI_MEASUREMENTS_ARRAY_SIZE],  /* Signal Strengths at Antenna           */
   uint8                       num,                                           /* Number of measurements to make        */
   uint16                      offset[MAX_CM_RSSI_MEASUREMENTS_ARRAY_SIZE],              /* When to make a measurement in qs      */
   gl1_hw_burst_position_type  position[MAX_CM_RSSI_MEASUREMENTS_ARRAY_SIZE],            /* position of burst within a gap  */
   gl1_hw_rx_tags_type         tags[MAX_CM_RSSI_MEASUREMENTS_ARRAY_SIZE],
   sys_modem_as_id_e_type      as_id
);

uint16 gl1_hw_x2g_power_measure_geran
(
   ARFCN_T                     ARFCN[MAX_CM_RSSI_MEASUREMENTS_REQ],               /* Absolute Radio Frequency Channels  */
   dBx16_T                     rx_signal_strength[MAX_CM_RSSI_MEASUREMENTS_REQ],  /* Signal Strengths at Antenna        */
   uint8                       num,                                               /* Number of measurements to make     */
   uint16                      offset[MAX_CM_RSSI_MEASUREMENTS_REQ],              /* When to make a measurement in qs   */
   gl1_hw_burst_position_type  position[MAX_CM_RSSI_MEASUREMENTS_REQ],            /* position of burst within a gap     */
   gl1_hw_rx_tags_type         tags[MAX_CM_RSSI_MEASUREMENTS_REQ],
   uint32                      g_frame_number[MAX_CM_RSSI_MEASUREMENTS_REQ],
   uint16                      offset2,
   uint32                      g_frame_number2,
   sys_modem_as_id_e_type      as_id
);

uint16 gl1_hw_x2g_start_acquisition_geran(ARFCN_T ARFCN,
                                  dBx16_T rx_signal_strength,
                                  uint16  timing_offset,
                                  int16   frequency_offset,
                                  boolean sch_decode,
                                  gl1_hw_rx_tags_type*  tags_ptr,
                                  uint32                g_frame_number,
                                  void*                 gsm_cell_ptr,
                                    sys_modem_as_id_e_type      as_id);

uint16 gl1_hw_x2g_sch_burst_geran(ARFCN_T    ARFCN,
                                 int16      signal_strength,
                                 uint16     timing_offset,
                                 int16      frequency_offset,
                                 gl1_hw_rx_tags_type*  tags_ptr,
                                 uint32                g_frame_number,
                                 boolean               reconfirm,
                                 void*                 gsm_cell_ptr,
                                 sys_modem_as_id_e_type      as_id);
extern boolean l1_sc_sub_in_packet_trans(gas_id_t gas_id);

sys_modem_as_id_e_type gl1_hw_qta_return_cm_as_id(gas_id_t gas_id);
extern void set_qta_cleanup_pending(boolean status, gas_id_t gas_id);
extern boolean get_qta_cleanup_pending(gas_id_t gas_id);

void gl1_hw_cm_build_startup_if_not_prebuilt (const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr, gas_id_t gas_id);
void gl1_hw_cm_build_startup_dpc(const gl1_hw_cm_rf_info_t*  cm_rf_info_ptr, gas_id_t gas_id);

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
void gl1_hw_switch_antenna_tx_div_ccch_fail(boolean is_pch, gas_id_t gas_id);
#endif /*  FEATURE_GSM_TX_DIV_ANTNA_SWITCHING*/

/*===========================================================================

FUNCTION gl1_hw_async_intf_dev_mode_cmd

DESCRIPTION
  Send dev mode indication command to GFW

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void gl1_hw_async_intf_dev_mode_cmd(sys_modem_device_mode_e_type devMode, gas_id_t gas_id);

void gl1_hw_update_gain_tags(rf_gain_T *gain,
#ifdef FEATURE_GSM_RX_DIVERSITY
                             rf_gain_T *gain_divrx,
#endif
#ifdef GERAN_L1_HLLL_LNASTATE
                                   uint16 *sawless_lnastate,
#endif
                                   gas_id_t gas_id);


uint32 gl1_hw_get_rxlm_buff_index(gas_id_t gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
void gl1_hw_rxdiv_data_init(gas_id_t gas_id);

 /* Log Rx Diversity metrics*/
void gl1_hw_log_rxd_metrics(dBx16_T pwr_dBm_x16_prx,dBx16_T pwr_dBm_x16_rxd,
                                 rf_gain_range_T rx_gain_range_prx,rf_gain_range_T rx_gain_range_rxd,
                                 rf_measurement_T powerstatus_rxd, GfwRxdLogPacket *gfwRxdLogPacket,
                                 gl1_log_chan_type log_chan_type, gas_id_t gas_id);

void gl1_enter_diversity_mode( gl1_init_rflm_type gsm_init_rflm_state,gas_id_t gas_id );
boolean gl1_hw_rf_task_enter_mode_DivRx( uint32 rxlm_buf_idx, rfcom_device_enum_type device, gas_id_t gas_id );
#ifndef FEATURE_GSM_RX_TX_SPLIT
void gl1_hw_async_intf_enter_mode_cmd_DivRx(GfwAppModeType app_mode, uint32 rxlm_buff_idx, GfwEnterModeType mode, sys_modem_as_id_e_type as_id);
#endif
void gl1_exit_diversity_mode(boolean force_exit, gas_id_t gas_id);

boolean gl1_hw_gsm_band_supported(sys_band_T sys_band,gas_id_t gas_id);

void gl1_set_rxd_control_flags(gl1_rxd_control_type gl1_rxd_control,gas_id_t gas_id);
gl1_rxd_control_type gl1_get_rxd_control_flags(gas_id_t gas_id);
void gl1_reset_rxd_control_flags(gas_id_t gas_id);

void gl1_start_rxdiv_agc(boolean is_agc_rxdiv,gas_id_t gas_id);
boolean gl1_get_rxdiv_agc_start(gas_id_t gas_id);

int32 gl1_hw_get_dtm_expected_power_divrx(gas_id_t gas_id);
void gl1_hw_get_rxd_efs_item(gas_id_t gas_id);

void gl1_set_enable_rxdiversity(boolean enable_rxdiversity,gas_id_t gas_id);
boolean gl1_get_enable_rxdiversity(gas_id_t gas_id);
void gl1_run_divrx_stream_sel_avg (boolean divrx_stream_selected, gas_id_t gas_id);
boolean gl1_get_divrx_stream_sel_rate (gas_id_t gas_id);
void gl1_reset_divrx_sel_avg (boolean clean_last_avg,gas_id_t gas_id);

void gl1_hw_set_rxqual_full(uint8 rxqual_full, gas_id_t gas_id);
void gl1_hw_set_HardRxQual(uint16 HardRxQual, gas_id_t gas_id);
void gl1_hw_set_fwakeup_rxd_log(boolean forcedwakeup, gas_id_t gas_id);
void gl1_hw_set_mean_bep(uint32 mean_bep, gas_id_t gas_id);

rfm_device_enum_type gl1_get_prx_ftm_dev(gas_id_t gas_id);
#ifdef FEATURE_GSM_RX_TX_SPLIT
#ifdef FEATURE_GSM_RX_DIVERSITY
rfm_device_enum_type gl1_get_drx_ftm_dev(gas_id_t gas_id);
uint16 gl1_get_drx_ftm_band_support(gas_id_t gas_id, int16 band);
#endif
rfm_device_enum_type gl1_get_tx_ftm_dev(gas_id_t gas_id);
#endif
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
boolean gl1_get_IMRD_ctl_nv(gas_id_t gas_id);
boolean gl1_get_rxd_ctl_nv(gas_id_t gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
void gl1_set_enable_rxdiversity_data(boolean enable_rxdiversity_data, gas_id_t gas_id);
boolean gl1_get_enable_rxdiversity_data(gas_id_t gas_id);
void gl1_set_enable_rxdiversity_data_dynamic(boolean enable_rxdiversity_data_dynamic, gas_id_t gas_id);
boolean gl1_get_enable_rxdiversity_data_dynamic(gas_id_t gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/

boolean gl1_get_MSRD_ctl_nv(gas_id_t gas_id);

#endif /*FEATURE_GSM_RX_DIVERSITY*/
#ifdef FEATURE_GSM_RX_TX_SPLIT

uint8 gl1_get_Rxd_threshold(gas_id_t gas_id);

uint8 gl1_hw_get_gfw_entermode_type(uint8 wakeup_mode, gas_id_t gas_id);
#else
GfwEnterModeType gl1_hw_get_gfw_entermode_type(uint8 wakeup_mode, gas_id_t gas_id);
#endif

#ifdef FEATURE_AEQ_PHASE4
void gl1_hw_efs_read_set_aeq4(gas_id_t gas_id);
gl1_hw_dtm_rx_aeq4_type gl1_get_aeq4_info(gas_id_t gas_id);
void gl1_set_aeq4_info (gl1_hw_dtm_rx_aeq4_type mdsp_aeq4_ctl_info, uint8 set_mask, gas_id_t gas_id);
boolean gl1_get_enableAeq4_status(gas_id_t gas_id);
void gl1_reset_aeq4_ctl (gas_id_t gas_id);
#endif  /* FEATURE_AEQ_PHASE4 */


void gl1_hw_async_intf_enter_mode_cmd(GfwAppModeType app_mode,
                                      uint32 rxlm_buff_idx,
#ifdef FEATURE_GSM_RX_TX_SPLIT
                                      uint32 txlm_buff_idx,
                                      uint8 entermode,
#else
                                      GfwEnterModeType entermode,
#endif
                                      rfm_device_enum_type device,
                                      sys_modem_as_id_e_type as_id);
#ifdef FEATURE_GSM_RX_TX_SPLIT
void gl1_hw_async_intf_tx_exit_cmd(gas_id_t gas_id);
#endif

#ifdef FEATURE_GSM_RX_TX_SPLIT
/*===========================================================================

FUNCTION  gl1_hw_rf_tx_wakeup

DESCRIPTION
  Send MSGR message to RF Task to run RF Wakeup for Tx.
  Use MSGR non-blocking wait to receive wakeup completion confimation in
  gl1_msgr_if task context.

===========================================================================*/
void gl1_hw_rf_tx_wakeup(uint32 txlm_buf_idx,
                         rfm_device_enum_type device,
                         gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_rf_tx_sleep

DESCRIPTION
  Send MSGR message to RF Task to run RF Sleep for Tx. Use MSGR blocking wait
  to receive wakeup completion confimation from RF Task before continuing

===========================================================================*/
void gl1_hw_rf_tx_sleep(uint32 txlm_buf_idx,
                        rfm_device_enum_type device,
                        gas_id_t gas_id);
/*===========================================================================

FUNCTION  gl1_hw_rf_set_ftm_devices

DESCRIPTION
  Sets PRx, DRx & Tx RFM devices for FTM.

===========================================================================*/
void gl1_hw_rf_set_ftm_devices(geran_test_mode_api_device_type devices, gas_id_t gas_id);

void gl1_hw_rf_set_ftm_devices_chain_select(geran_test_mode_api_device_type devices, gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_rf_reset_ftm_devices

DESCRIPTION
  Resets PRx, DRx & Tx RFM devices after FTM completes.

===========================================================================*/
void gl1_hw_rf_reset_ftm_devices(gas_id_t gas_id);
void gl1_hw_disable_tx( gas_id_t gas_id);
#else
/*===========================================================================

FUNCTION gl1_hw_rf_enable_tx_device

DESCRIPTION
  Send MSGR message to RF Task to run GSM Tx enable. Use MSGR blocking wait
  to receive enter mode completion confimation from RF Task before continuing.
  Take rf device as a parameter

===========================================================================*/
void gl1_hw_rf_enable_tx_device(uint32 txlm_buf_idx, rfm_device_enum_type device, gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_hw_rf_disable_tx

DESCRIPTION
  Send MSGR message to RF Task to run GSM Tx disable. Use MSGR blocking wait
  to receive enter mode completion confimation from RF Task before continuing

===========================================================================*/
void gl1_hw_rf_disable_tx(uint32 txlm_buf_idx, rfm_device_enum_type device, gas_id_t gas_id);
#endif
/*===========================================================================

FUNCTION  gl1_hw_rf_set_tx_band_device

DESCRIPTION
  Send MSGR message to RF Task for Set Tx Band. Take the RF device as a
  parameter and does not check if the band was changed

===========================================================================*/
void gl1_hw_rf_set_tx_band_device(rfgsm_band_type band, rfcom_device_enum_type device, gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_rf_task_enter_mode_req

DESCRIPTION
  Send MSGR message to RF Task to run GSM enter mode. This function does
  not block to wait the confirmation from RF

===========================================================================*/
void gl1_hw_rf_task_enter_mode_req(uint32 rxlm_buf_idx, rfm_device_enum_type  device, gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_rf_task_enter_mode_cnf

DESCRIPTION
  Wait for the response from RF Task to GSM enter mode. Use MSGR blocking wait
  to receive enter mode completion confimation from RF Task before continuing

===========================================================================*/
void gl1_hw_rf_task_enter_mode_cnf(gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_set_rxlm_buff_index

DESCRIPTION
  Updates the value of the RXLM buffer index

===========================================================================*/
void gl1_hw_set_rxlm_buff_index(uint32 idx, gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_set_txlm_buff_index

DESCRIPTION
  Updates the value of the TXLM buffer index

===========================================================================*/
void gl1_hw_set_txlm_buff_index(uint32 idx, gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_get_txlm_buff_index

DESCRIPTION
  Returns the TXLM buffer index

===========================================================================*/
uint32 gl1_hw_get_txlm_buff_index(gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_get_rf_gsm_band

DESCRIPTION
  Returns the current RF band

===========================================================================*/
int8 gl1_hw_get_rf_gsm_band(gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_set_invalid_rf_gsm_band

DESCRIPTION
  Sets gsm RF band to none, so force set tx band

===========================================================================*/
void gl1_hw_set_invalid_rf_gsm_band(gas_id_t gas_id);

#ifdef FEATURE_WLAN_COEX_SW_CXM
boolean gl1_hw_async_intf_cxm_policy_cmd( cxm_active_config_v01_s policy, gas_id_t gas_id );
#endif


/*=============================================================================
FUNCTION gl1_hw_pwr_meas_req_time

DESCRIPTION
  Returns the rf_gsm_mon_setup_time by a power measure including GL1_HW_SCHEDULE_GUARD_PERIOD

============================================================================= */
int32 gl1_mon_set_up_with_sched_guard_period( gas_id_t gas_id );

  /*=============================================================================
  FUNCTION gl1_hw_pwr_meas_req_time

  DESCRIPTION
    Returns the total time required by a power measure including tuning and BB Rx turn off

  ============================================================================= */
int32 gl1_hw_pwr_meas_req_time( gas_id_t gas_id );

/*===========================================================================

FUNCTION gl1_hw_cm_efs_read

DESCRIPTION
  Reads EFS needed in CM.

===========================================================================*/
void gl1_hw_cm_efs_read(gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_hw_cm_efs_update

DESCRIPTION
  Update EFS to mdsp shared variable needed in CM.

===========================================================================*/
void  gl1_hw_cm_efs_update(gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_hw_async_intf_wtr_cfg_cmd

DESCRIPTION
  Send MSGR message to GFW to update the RXLM index during WTR hopping.
  Use MSGR blocking wait to receive rsp from GFW before continuing

===========================================================================*/
#ifdef FEATURE_GSM_WTR_HOP
void gl1_hw_async_intf_wtr_cfg_cmd(uint32 buffer_index, rfm_device_enum_type device,
                                   sys_modem_as_id_e_type as_id);

/*===========================================================================

FUNCTION  gl1_hw_wtr_hop_config_tx

DESCRIPTION
  Configure FW for tx during wtr hopping

===========================================================================*/
void gl1_hw_wtr_hop_config_tx(uint32 buffer_index, rfm_device_enum_type device, gas_id_t gas_id);
#endif
#ifdef FEATURE_GSM_PARALLEL_SCH_DEDICATED
/*===========================================================================

FUNCTION  l1_par_sch_mcpm_change
DESCRIPTION
  This function requeste MCPM change at start/end of power scan so that
  clock speed is increased during power scan for RF processing load.

===========================================================================*/
void gl1_par_sch_mcpm_change(
              boolean par_acq_start,
              gas_id_t    gas_id );
#endif /* FEATURE_GSM_PARALLEL_SCH_DEDICATED */
#ifdef FEATURE_GSM_QSC_TX_DIV
void gl1_trm_ant_switch_set_control ( trm_ant_switch_set_ctrl_enum control_req_type ,gas_id_t gas_id);
#endif /* FEATURE_GSM_QSC_TX_DIV */

#ifdef GERAN_L1_HLLL_LNASTATE
void gl1_hw_send_dtm_burst_metrics(uint32 fn,gas_id_t gas_id);
boolean gl1_hw_get_sawless_HLLL_support(sys_band_T band,gas_id_t gas_id);
void gl1_hw_rf_burst_metrics(gl1_defs_metrics_lnastate *gl1_metrics_lnastate,gas_id_t gas_id);
void gl1_hw_log_HLin_LLin_metrics( gl1_defs_metrics_lnastate *gl1_metrics_lnastate,uint8 *SawlessHLLLState, gas_id_t gas_id);
void gl1_hw_update_dtm_metrics(gl1_defs_metrics_lnastate *gl1_metrics_lnastate,uint32 fn, gl1_hw_update_metrics_t update_metrics,
                                     gas_id_t gas_id);
void gl1_hw_reset_sawless_support_flag(gas_id_t gas_id);
#endif /*GERAN_L1_HLLL_LNASTATE*/

void gl1_log_gsm_dft_spur_metric (  GfwDftSpurAciMetricsStruct SpurAciMetrics,uint16 time_slot,gas_id_t gas_id );


void gl1_log_epd_metrics(GfwEpdLogPacket *gfwEPDLogPacket,gas_id_t gas_id);

#ifdef GERAN_L1_IP2CAL
void gl1_hw_send_ip2cal_cmd(uint8 num_of_ip2cal_steps,uint32 ip2Cal_StepSizeQS,gas_id_t gas_id);
void gl1_hw_rf_ip2_cal(gas_id_t gas_id);
void gl1_hw_deallocate_ip2cal_rxlm_buf_idx( gas_id_t gas_id );
void gl1_hw_deallocate_ip2cal_txlm_buf_idx( gas_id_t gas_id );
boolean gl1_hw_get_ip2cal_support(gas_id_t gas_id);
uint32 gl1_hw_get_ip2cal_rxlm_buff_index(gas_id_t gas_id);
uint32 gl1_hw_get_ip2cal_txlm_buff_index(gas_id_t gas_id);
int32 gl1_hw_get_rf_gsm_ip2cal_setup_time( gas_id_t gas_id );
#endif /*GERAN_L1_IP2CAL*/

rfm_device_enum_type gl1_hw_get_lte_rf_id(grm_client_enum_t lte_client, sys_sband_lte_e_type lte_band, gas_id_t gas_id);

boolean gl1_hw_rf_wakeup_req_msg_check(gas_id_t gas_id );
void gl1_hw_set_rf_wakeup_req_msg(gas_id_t gas_id, boolean rfWakeUpSent);
boolean is_hw_tx_on (gas_id_t gas_id );
void gl1_hw_rf_sleep(uint32 rxlm_buf_idx, gl1_hw_sleep_mode_type sleep_mode, gas_id_t gas_id);
void gl1_hw_rf_wakeup(uint32 rxlm_buf_idx,
                      gl1_hw_wakeup_mode_type wakeup_mode,
                      gas_id_t gas_id);

boolean gl1_hw_cm_read_t2g_active(gas_id_t gas_id);
boolean gl1_hw_cm_read_w2g_active(gas_id_t gas_id);

rfm_device_enum_type gl1_hw_cm_get_rf_device(gas_id_t gas_id);
rfcom_mode_enum_type gl1_hw_cm_get_cm_rat_type(gas_id_t gas_id);

void gl1_hw_start_gsm_qta(gas_id_t gas_id);
boolean gl1_hw_get_if_start_gsm_qta_sent(gas_id_t gas_id);
void gl1_hw_set_if_start_gsm_qta_sent(boolean value, gas_id_t gas_id);

boolean gl1_hw_is_g2x_reorg_ta_enabled( void );
boolean gl1_hw_is_g2x_non_drx_ta_enabled( void );

extern boolean            gl1_hw_sleep_wmgr_cancel_in_prog[NUM_GERAN_DATA_SPACES];
extern uint32             sleep_duration_gsm_clks[NUM_GERAN_DATA_SPACES];
extern boolean            gl1_hw_sleep_wmgr_cancel_after_prog[NUM_GERAN_DATA_SPACES]; 
extern trm_reason_enum_t  gl1_hw_sleep_wmgr_cancel_trm_reason[NUM_GERAN_DATA_SPACES];
extern boolean            gl1_hw_sleep_wmgr_pch_cancel_allowed[NUM_GERAN_DATA_SPACES];
extern uint32             gl1_hw_sleep_wmgr_trm_when_cur[NUM_GERAN_DATA_SPACES];
extern uint32             gl1_hw_sleep_wmgr_trm_when_at_gts[NUM_GERAN_DATA_SPACES];
extern uint32             gl1_hw_sleep_wmgr_trm_res_fn_at_gts[NUM_GERAN_DATA_SPACES];
extern uint32             gl1_hw_sleep_wmgr_warmup_incl_in_trm_fn[NUM_GERAN_DATA_SPACES];
extern boolean            gl1_hw_sleep_wmgr_trm_when_at_gts_valid[NUM_GERAN_DATA_SPACES];
extern uint32             gl1_hw_sleep_wmgr_result[NUM_GERAN_DATA_SPACES];
extern uint32             gl1_hw_sleep_next_allowed_ccch_fn[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
extern uint32             gl1_hw_sleep_wmgr_els_warmup_dur_in_sclk[NUM_GERAN_DATA_SPACES];
#endif

#define GL1_HW_SLEEP_WMGR_RES_CAN_ACT_MASK            0xF0000000  /* b31-b28 */
#define GL1_HW_SLEEP_WMGR_RES_BK_ACT_MASK             0x0F000000  /* b27-b24 */
#define GL1_HW_SLEEP_WMGR_RES_CAN_PRI_MASK            0x00F00000  /* b23-b20 */
#define GL1_HW_SLEEP_WMGR_RES_BK_PRI_MASK             0x000F0000  /* b19-b16 */
#define GL1_HW_SLEEP_WMGR_RES_ERR_MASK                0x00000FFF  /* b00-b12 */

/* GL1_HW_SLEEP_WMGR_RES_CAN_ACT_MASK */
#define GL1_HW_SLEEP_WMGR_RES_ACT_CAN_PCH             0x80000000
#define GL1_HW_SLEEP_WMGR_RES_ACT_CAN_BCCH            0x40000000
#define GL1_HW_SLEEP_WMGR_RES_ACT_CAN_OTHER           0x20000000

/* GL1_HW_SLEEP_WMGR_RES_BK_ACT_MASK */
#define GL1_HW_SLEEP_WMGR_RES_ACT_BK_PCH              0x08000000
#define GL1_HW_SLEEP_WMGR_RES_ACT_BK_BCCH             0x04000000
#define GL1_HW_SLEEP_WMGR_RES_ACT_BK_OTHER            0x02000000

/* GL1_HW_SLEEP_WMGR_RES_CAN_PRI_MASK */
#define GL1_HW_SLEEP_WMGR_RES_CAN_PRI(x)              ((x) << 20)

/* GL1_HW_SLEEP_WMGR_RES_BK_PRI_MASK */
#define GL1_HW_SLEEP_WMGR_RES_BK_PRI(x)               ((x) << 16)

/* GL1_HW_SLEEP_WMGR_RES_ERR_MASK */
#define GL1_HW_SLEEP_WMGR_RES_NO_ERROR                0x00000000
#define GL1_HW_SLEEP_WMGR_RES_ERR_TRM_REASON_CHNGD    0x00000001
#define GL1_HW_SLEEP_WMGR_RES_ERR_SLPC_INSUF_GAP      0x00000002
#define GL1_HW_SLEEP_WMGR_RES_ERR_INSUF_SLEEP_DUR     0x00000004
#define GL1_HW_SLEEP_WMGR_RES_ERR_NOT_IN_ASLEEP       0x00000008
#define GL1_HW_SLEEP_WMGR_RES_ERR_ACTIVITY_INSUF_GAP  0x00000010
#define GL1_HW_SLEEP_WMGR_RES_ERR_PCH_CAN_NOT_ALLOWED 0x00000020
#define GL1_HW_SLEEP_WMGR_RES_ERR_NO_OTHER_ACTIVITY   0x00000040
#define GL1_HW_SLEEP_WMGR_RES_ERR_SLPC_REPROG_FAILED  0x00000080
#define GL1_HW_SLEEP_WMGR_RES_ERR_UNSUPPORTED_ACT_CAN 0x00000100

boolean gl1_hw_sub_non3gpp_capable(gas_id_t gas_id);

boolean gl1_hw_is_saic_dis_in_lmm_mode(gas_id_t gas_id);

int32 gl1_hw_get_rf_gsm_tx_vswr_setup_time( gas_id_t gas_id );

sys_vfr_e_type gl1_get_stored_vfrmap(gas_id_t gas_id);

#ifdef FEATURE_GSM_SENS_SUPPORT
uint8 gl1_hw_get_sens_fr_ctrl(void);
#endif

#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY
typedef struct
{
  uint8 mon_recovery_count;
  uint8 rx_recovery_count;
  uint8 tx_recovery_count;
  uint8 rx_blanking_count;
  uint8 acq_recovery_count;
  uint8 x2g_acq_recovery_count;
}gl1_hw_burst_recovery_count_t;

typedef struct
{
  uint8 mon_success_count;
  uint8 rx_success_count;
  uint8 tx_success_count;
}gl1_hw_burst_success_count_t;

extern boolean           critical_scn_updated_to_mcpm[NUM_GERAN_DATA_SPACES];
extern uint32            timeline_error_recovery_needed[NUM_GERAN_DATA_SPACES];

void gl1_hw_reset_burst_recovery_counters (gas_id_t gas_id);
void gl1_hw_reset_burst_success_counters (gas_id_t gas_id);
void gl1_hw_update_tx_burst_success( gas_id_t gas_id);
#endif

#ifdef FEATURE_GSM_SYNQ_SUPPORT
uint8 gl1_hw_get_synq_fr_enabled(void);
uint16 gl1_hw_get_synq_fr_gdet_threshold(uint8 * bad_gdet_threshold, uint8 * gdet_recovering_threshold);
#endif

#endif /* GL1_HW_H */
/* EOF */
