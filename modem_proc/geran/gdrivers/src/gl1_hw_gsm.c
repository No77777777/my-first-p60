/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      G L 1  F R A M E   L A Y E R

GENERAL DESCRIPTION
   This file contains the GSM/GPRS section of the GL1 frame layer.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2003 - 2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_gsm.c#6 $
$DateTime: 2020/02/10 05:49:45 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
06/02/20   nv       CR2617011 FR 54762: Framework for mitigating concurrency of transient scenarios across Q6 subsystems
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
23/01/20   rks      CR2608728 backing out the qsh change causing the compilation failure
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
25/04/18   rv       CR2225983 - GERAN F3 and log pkt Reduction TA.3.0
07/02/18   hd       CR2181437 Report the BCCH as found when BSIC doesn't match after SCH decode
23/01/18   br       CR2033217 GL1 to disable rxd during ACQ in FTM mode
10/08/16   cws      CR1044687 Use ARFCN from pwr sm directly for pwr monitor log packet
30/08/16    tsk     CR1058305: Update to 0x5096 RxD log packet with additional details.
15/11/16   bg       CR1089997 Fix for KW errors
28/02/17   og       CR2006578. FRs 39363 and 39364.
29/10/17   km       CR2133917 GL1 support for TA.3.0 L+L
17/08/17   br       CR2092505 NV control for logging GDET related F3s
21/04/17   br       CR1041084 FR 36219: GDET – GMSK Modulation Detector
14/02/17   mk       CR2004610 : FR 40251 (SeNS) - Skip reporting NCELL if SCH reconfirmation failed with good GDET score
03/11/15   ss       CR916273 FR 29852: AsDiv in Acquisition
27/05/16   snjv     CR1000909 pass ACI400 switchpoint detection stat from GFW to GRF via GL1
13/06/16   sk       CR1026984 Rework on CR1011063 (Data halt in DTM scenario and Call drop observed)
03/06/16   sk       CR1011063 Do not update average power if all the CCCH bursts have been yielded
19/10/15   br       CR926522 GL1 to Pass the frequency offset to rotator to correct in GBTA.
05/11/15   pg       CR935738 FTM GSM DRx IQ logging broken since Rx/Tx split 
03/11/15   sp       CR916957: Update CR909073, log with 0's instead of no logging when yield
22/10/15   zf       CR923858 Rework Sleep error recovery in QTA 
25/09/15   jk       CR915579:Enable LIF by default for ACQ in FTM Cal mode
17/09/15   jk       CR909073:Don't log MON metrics in MON log packet if MON is yielded
18/09/15   sd       CR905968: Set sleep commit mask for Rx Burst in ACQ
09/09/15   ab       CRxxxxxx Incorrectly setting SCH as YIELED when actually Proceed, 
                             when PDTCH RX/TX is YIELDED causes continous SCH attempts
08/09/15   pjr      CR900800 Allocate VFR based on subscription capability
07/09/15   npt      CR898493 GL1 NV control of T2G acquisition GFW error recovery
26/08/15   ws       CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
26/08/15   npt      CR892374 Generate monitor log when leaving GERAN
20/08/15   npt      CR878988 GL1 NV control of GFW recovery features
03/08/15   jj       CR879940  disable_power_opt15 for W2G
21/07/15   pg       CR876928 Fix compiler warnings due to CR871098 
15/07/15   pg       CR871098 Request VSTMR VFR allocation based on AS_ID, not multi-mode sub 
04/06/15   jk       CR846952:RxLEV report should be based on maximum of PRx and DRx average RSSIs
30/06/15   ws       CR852519 - Resolve GL1 enum conversion compile warnings
29/05/15   pa       CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD.
22/05/15   ws       CR790493 Check BSIC decoded in SCH matches requested BSIC from RR
20/05/15   dg       CR839000 In case of X2G, Reset partialtonedetect flag if SCH decode is SET
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
29/04/15   npt      CR804870 RF timeline error recovery mechanism
22/04/15   ab       CR822921 : Provide Rx ARFCN in SYNC RX / ASYNC RX for PCHXFER Scheduling in GFW and RXLEV info for IQ Scaling in ALL ASYNC RX. 
16/04/15   smd      CR822202 Dual space data for IRAT and QTA
07/04/15   og       CR814507. Further stack reduction in the GSM L1 ISR.
17/03/15   xz       CR801386 sovle the address word aligment issue for scheduled_status of SCH
24/03/15   pg       CR786023 Add support for FTM DRx IQ logging (FR22183) 
17/03/15   jk       CR805689:ARFCN not logged correctly  in GSM_NCELL_ASYNC_METRICS, 0x507D
17/03/15   cs       CR804196 Use new passthrough versioned FCCH/SCH log packet
05/03/15   cja      CR803591 Ignore stale SCH data for parallel FCCH/SCH
07/01/15   jj       CR777805 Add probe burst flag for diversity rx
02/03/15   ak       CR796460 Apex L1 Monitor Burst Metrics - RSSI does not match ARFCN after power monitor abort
28/02/15   cja      CR768169 Add RF Seq Number
23/02/15   jk       CR793098:LIF additional logging changes
20/02/14   pjr      CR745263 Configure VFR mux to select correct vstmr based VFR - TABASCO Only
18/02/15   am       CR775652 Mismatch in monitor reading
09/02/15   jj       CR 793229 making debug f3 for crash in the middle of the burst under NV control
17/11/14   jj       CR 752202 GL1 - bolt - Debug support for crashing in the
                    middle of rx burst for a given range of rssi value
15/02/15   sp       CR729492 Remove unimportant logging
04/02/15   cja      CR789095 Store X2G acquisition tags to use for SCH results
28/01/15   cja      CR767058 T2G using parallel FCCH/SCH
27/01/15   cja      CR780982 For CM SCH, get metrics for correct burst
27/01/15   npt      CR697236 Mainline FEATURE_GERAN_VSTMR
27/01/15   cs       Improve GL1 Parallel SCH F3
21/01/15   pa       CR758792:RxD support for multi-sim.
12/12/14   cs       CR754853 Add support for Parallel SCH detection handling
16/12/14   jk       CR753423:LIF for Idle,voice and data
03/12/14   pg       CR765847: Add support of Idle RxD for AGCH and NCELL SCH
28/11/14   am       CR760135 Enable SBD and EPD for page decode in transfer mode
07/11/14   pjr      CR752564 Calculate correct buffer index when retrieving AMR-PD metrics
                             Retrieve AMR-PD metrics in gl1_hw_get_sync_rx_data_amr
19/11/14   zf       CR752040: Do not use empty power measurement results on calculating RSSI measurement
07/11/14   cja      CR751049 Add cm FCCH and SCH results API (ready for parallel FCCH/SCH)
23/09/14   jj       CR 728792  Discrete Fourier transform (DFT) spur metric log packet
19/08/14   pjr      CR711506 Support for AMR Partial Decode and DTX detection FR19667
10/10/14   cos      CR736560 BO 2.x Idle RxD mirror combined exit/sleep requests according to the enter/wakeup modes
19/09/14   ss       CR722595 Do not set swidth to default for every SCH decode measuremen
06/06/14   jk       CR668058:Adding EPD Log pakcet
17/09/14   ws       CR 694807 Reduction in High occuring F3
15/09/14   am       CR723773 Desense channel check in X2G SCH decode
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
12/09/14   nm       CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
04/09/14   am       CR682760 Addressing KW GL1 P1 errors
06/08/14   jj       CR 705509 reducing scell meaurement when BA list is empty in dedicated mode
14/04/14   pjr      CR641577 Replaced FN with Seq Number in cxm log table
07/08/14   cja      CR643648 Desense for channel adjacent to spur
11/07/14   pjr      CR 692177 Added support for RFACCH 2.0
06/08/14   df       CR598015 Avoid silent frame DRX processing when pending firmware activity trigger is set
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
24/07/14   jk       CR699271:GL1 LIF changes for RxD
02/07/14   cos      CR 688739 - Residual BER in RxD mode when PRx is disconnected
15/05/14   jj       CR 664355 Memclr rx_chan_params to avoid junk values in rx params send to rf
30/05/14   jj       CR656854  Resolve Critical Klocwork errors in gdrivers code space
21/07/14   jk       CR660801:Support for logging VAMOS log packet 0x5099
11/06/14   npt      CR630396 Wake-up error recovery mechanism
30/05/14   pg       CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM
27/05/14   sp       CR670592: Update AGC_ptr with Rxlev reported from FCCH results
06/05/14   npt      CR659704 Ensure VFR is assigned before trying to align it
17/04/14   cos      CR636543 Changes for enabling Rx Diversity on Bolt - GL1/GRF intf updates
07/04/14   cja      CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
04/04/14   cah      CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
25/03/14   pjr      CR628670 Fix cxm log packets for async activities
24/03/14   nk       CR624721 Update arfcn in 0x507D GSM Ncell Metrics log packet
10/03/13   ka       CR628885 Remove unused functions
06/03/14   jk       CR627307 Fix compilation error for NARM build flavor
05/03/14   jk       CR626652 Fix compilation error
03/03/14   am       CR605057 Print SNR of SCH in combined ACQ
02/01/14   jk       CR595130 GL1 changes to support VAMOS-II
19/02/14   am       CR604674 Take spur into account for T2G acquisition
06/02/14   cgc      CR598061 Corrections to use of mdsp_intf_shared_var_read(), dereference pointer
22/01/14   jk       CR606383 GL1 changes to support RxDiversity
21/01/14   cgc      CR602010 TR2.0 merge fix,prevent WTOG BPLMN deactivate
19/12/13    jk      CR503359 - GL1 changes to support Rx diversity
20/12/13   cja/cs   CR588310 For SCH decoding on spur channels ensure XO correction on DPLL is 0
03/01/14   jk       CR572347 - GL1 FTM API changes to support Rx diversity
02/01/13   dv       CR592228 RxAGC flooring after few QTAs
10/12/13   ws       CR 587586 - Remove DUAL_SIM featurisation in API
15/11/13   js       CR549415 - Do not do reselection meas on yielded arfcns
28/11/13   cja      CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
22/11/13   cja      CR455527 Remove featurisation from files in API directory
11/10/13   pjr      CR555300 Changes to correctly register freqid during monitors  and acq
19/11/13   og       CR578795. Correct cxm featurization for gl1_hw_rx_async_burst().
11/18/13   pjr      CR563534 Fix incorrect channel type shown in CXM logs packets
21/10/13   mc       CR555528 : COEX : CXM Priority and Freq ID for Async Rx
03/10/13   pjr      CR553798: Added f3 message when setting VFR mux
05/11/13   js       CR555528 : COEX : CXM Priority and Freq ID for Async Rx
11/11/13   ws       CR 573757 Featurise GSMTR-1 registers and Voice services driver
31/10/13   cgc      CR569801  Add wrapper functions gl1_cm_hw_cleanup_async_rx()
11/10/13   cja      CR554146 Remove F3 to reduce processor load in T2G
24/09/13   sp       CR449651: Retrieve Burst data as per updated GFW-GL1 interface
30/07/13   jj       CR 520475 dime stand alone asdiv  GL1 - RF interface change based on new request
21/01/13   jj       CR 443405  modified implementation of dynamic tx-div disable
10/01/13   jj       CR 402579 fixing tx diversity merge errors
21/01/13   jj       CR 443405  modified implementation of dynamic tx-div disable
26/11/12   jj       CR 424199 Adding Scell measurement for tx-diversity when BA list is empty
17/09/13   pjr      CR538893 Changes to enable CXM logging
23/09/13   sk       2.0 syncup from 1.1
13/09/13   jk       CR413653:Set flag spurChanFlag for desense channel
12/09/13   js       CR534904 No Audio on SLOT 2 in G+G / W+G / C+G
21/08/13   dv       CR532560 Improve T2G SCH performance
03/09/13   npt      CR524770 - GL1 changes to support RFLM FED
20/08/13   npt      CR530340 - Added VSTMR VFR handling
20/08/13   ab       CR531098 GL1 Reduce Feature GBTA F3
15/08/13   sk       CR524039 GBTA bringup changes
02/08/13   pg       CR447578: Enable partial tone detection only during X2G
29/07/13   sk       CR519663 Partial QBTA bringup changes
7/16/13     zc       CR470197 Replace memcpy with memscpy
19/06/13   cs       Major Triton TSTS Syncup
30/05/13   pjr      CR493441 removed static qualifier from variables in gl1_hw_pwr_meas_cmd
29/05/13   pjr      CR487402 added support for second VFR module
20/05/13   pg       CR475487: Use GL1 RSSI to dBm API, not legacy RF API
17/05/13   cs       TSTS Updates
25/04/13   npt      CR472893 - CR411410 broken SCH decode
16/05/13   og       CR453445. QTA Ncell BCCH.
02/05/13   pjr      CR482811 Only gas_id1 can change vfr setting
29/04/13   ws       CR481359 enable GTA support
28/03/13   jk       CR452710: Changes to support continuous IQ logging and GSM XO calibration
07/03/13   dv       CR461080: Introduction of new GSM VS interface
06/02/13   jk       CR449735: Resolving KW errors
17/10/12   zf       CR411410: Report separate SCH decode and CRC results
05/12/12   pg       CR427361: GFW/RF shared memory interface cleanup
26/11/12   pg       CR424194: updates for AFC and AGC when using RF Task
26/10/12   cja      CR414421. Correct C over I sigma for AFS.
04/09/12   og       CR395649. T2G Acquisition and BSIC decode enhancements.
14/07/11   cja      CR296296 Correct conversion for WL1 freq report
29/06/12   jj       CR374950 GL1 changes for 38 symbol HMSC monitor
20/06/12   ky       Added support of using new CCS interface and passing new CCS
                    buffer pointers in RF task messages
08/06/12   pa       GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
25/05/12   pg       Add support for RF Task
01/05/12   ws       CR355391 - Deprecate arm.h, armasm.h and hw.h
10/04/12   cja      CR350203 Send loopback command in ISR
26/03/12   jj       CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
19/03/12   pg       Dime modem specific HWIO implementation
14/03/12   pg       Add support for local HWIO macros (msm.h deprecated)
13/03/12   ws       CR335057 eSACCH buffer flush and restore after inter-cell handovers
16/02/12   ws       CR 332030. Implement storage of last good modulation for
                    SRB loopback
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
27/01/12   jj       HMSC power monitor
11/11/11   og       CR318651. Change GL1_HW_CM_TONE_MIN_SNR_THRESH to 1dB.
10/10/11   cja      CR280738 Check for SAIC active in blocker detection
08/09/11   jj       CR302378  Added GL1 changes for RSB along with spectral inversion
14/07/11   cja      CR296296 Correct conversion for WL1 freq report
09/06/11   ws       Enable VFR for GSM on NIKEL target as hw system drivers are deprecated
21/06/11   cja      Featurise 32-bit FCCH freq interface to MDSP
14/06/11   cja      CR264690 Acquisition frequency capture range larger
28/04/11   pg       Fix compiler warnings
17/03/11   cja      CR279518 Add single slot FTM power store for GPRS/EDGE
20/04/11   cja      Add spectral inversion bit for Nikel
27/01/11   cja      Changes for RxLM and TxLM for NikeL
28/02/11   cja      CR276826 Change check for valid W2G freq report, to saturate
08/02/11   cja      CR264690 Acquisition frequency capture range larger
14/01/11   npt      CR264682 Add support for asynch dsp completion interrupt
09/12/10   cs       CR264682 Add support for asynch dsp completion interrupt
24/11/10   cja      Integrate HMSC
07/10/10   ky       254466:Removed fw unused shared variables from the sw code
24/09/10   cja      Fix KlocWork warnings
17/09/10   ws       CR 255850 - Implemented gl1_hw_ftm_get_rx_power_results()
                    for FTM verification of AGC
03/09/10   ws       Added FEATURE_GSM_MDSP_AEQ_SCH for QDSP6 targets
19/07/10   cs       CR246418 Add 2dB SNR theshold to CM tone detects
29/06/10   ky       Added the performace logging for rf driver apis
27/04/10   cs       CR236460 Latest AFC\XO handling updates
05/05/10   pg       Add AEQ metrics logging for QDSP6
20/04/10   cja      Use frequency offset from W. CR232174
04/02/10   ws       Removed leagcy Q4 MDSP services and image for Q6 targets
01/02/10   ky       Extracting Ncell SCH Burst metrics should happen from gfwSchRxBurstMetricsBuffer
                    instead of gfwAsyncMetricBuffers for Q6.
19/11/09   ws       Fixed MSG_LOW for QDSP6 defines for afc freq in
                    gl1_hw_gen_was_tone_detected()
09-10-22   tjw      include rfm_meas for fn prototypes
23/09/09   ps       Resolve Klocwork Buffer Overflow errors
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
16/06/09   ap       Fix Lint/compile warnings
26/05/09   cs       CR167848 fix for expanded afc_freq I/F to mDSP
29/05/09   ws       FEATURE_GSM_MDSP_AEQ phase 1 for all GMSK bursts
29/05/09   ws       Removed Featurisation from GL1 API for CMI compatability
20/03/09   cs       Fixes to the common 32-bit afc_freq mDSP I/F
13/03/09   og       FEATURE_XO updates.
12/03/09   og       FEATURE_XO support.
26/02/09   cs/hy    Initial XO changes for zeroing Rot when acq
08/12/08   cs       Add in the new split gl1_hw_debug.h from gl1_hw.h
30/10/08   ws       CR 162112 - ESAIC Only reset IIR filter on PCH and NCELL BCCH
09/10/08   cs       Allow the RX Sweep to schedule shorter monitor duration
01/10/08   cs       Lint HIGH fixes
28/08/08   cs       Implement the Fast Rx Sweep functionality for FTM
12/05/08   dv       Added FEATURE_GSM_AMR_WB - L1 component
19/05/08   cs       Fix the Klocwork around bound error
27/03/08   nf       Added NV Item handling to R-SACCH and R-FACCH
19/03/08   agv      LCU RF Spur Interface Support.
14/02/08   nf       Added feature enabling other Tech L1's to find if GSM is in a TX state
14/01/08   cs       Allow for separate IQ logging and saving
23/11/07   cs       Fix the radio block debug printing
09/11/07   nf       Added FEATURE_GSM_R_SACCH
30/10/07   og       Added support for FTM BER calculation in loopback A, B, C
                    and SRB.
22/10/07   ws       Add support for ESAIC NV 2508 item control
04/10/07   dv       CR129880:Fix for reading back RFACCH status flags.
31/08/07   agv      Fix for blocker algorithm.
07/09/07   og       Added support for FTM BER calculation in loopback A, B, C
                    and SRB.
13/08/07   ws       Corrected new RFCOM definition for previous checkin
                    RFCOM_HOT_DELTA
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
17/07/07   ws       CR123746 Add LNA_GAIN_RANGE to rfm_set_rx_gain() API
09/07/07   nt       Added extra debug to show the expeceted power and Gain Stage
                    for SCH FCCH acquisition.
12/06/07   og       Upgrade the FEATURE_GSM_L1_HFREQ_ERR_NCELL_SCH_DECODE to
                    include dedicated TCH NCELL SCH attempts.
11/05/07   npr      From Jai Fu - featurisation changes for 1X/GSM 7500-90
01/05/07   nf       Corrected RFACCH reading of MDSP status bits
27/03/07   dv       Added FEATURE_GSM_R_FACCH
07/02/07   cs       Fix SAIC on/off feature for loop-c
24/11/06   og       Correct compilation issues when rotator feature is switched
                    off.
06/14/06   ws       disable SAIC for AMR HR and TCH HR when doing loop mode C
06/04/06   agv      Feature SAIC MIMO elements added to the burst metrics.
03/29/06   gfr      Add frequency offset to ncell functions to support non
                    spec-compliant networks in WCDMA mode.
01/13/06   gfr      Support for new RFM API (FEATURE_RFM_INTERFACE)
12/20/05   gfr      Support for logging SAIC metrics
10/18/05   gfr      Trace the RF setup in the power measure function
10/17/05   gfr      Moved gsac code to gl1_hw_gsac.c
10/07/05   gfr      Add ACI detection metrics to the burst metrics
09/21/05   gfr      Remove reference to rf_pa_precharge_stop_time
09/12/05   kt       Added FEATURE_GSM_AFIVE12_BLK to support targets with separate
                    AFIVE1/2 encryption block
08/19/05   gfr      QLint cleanup
08/05/05   gfr      Change gl1_hw_cleanup_abort_async_rx to gl1_hw_cleanup_async_rx
07/29/05   gfr      DTM support
06/16/05   gfr      Added calls to gl1_hw_set_iq_samples_tn
05/18/05   gfr      No need to include mdsp_gprs.h
05/17/05   yh       Use channel filter stage2 as default.
05/05/05   gfr      Use gl1_hw_setup_tx_power_control
05/03/05   pp       Changes to support 14.10 test case
04/26/05   gfr      No need to unwind RF driver for missing power measures
04/21/05   gfr      No need to buffer AMR phase anymore
04/20/05   yh       Remove use of RF fifo
04/12/05   gfr      Consolidate GSM hw layer parameters
04/08/05   gfr      Use macros to parse the decoded data header
04/08/05   gfr      Combined GSM and GPRS channel types
04/08/05   gfr      Move logging code into gl1_hw_log.c
03/25/05   yh       Fix sid calculation in gl1_hw_get_sync_rx_data.
03/22/05   gfr      Lint cleanup and changed FEATURE_POLAR_RF to FEATURE_POLAR
03/09/05   yh       API change for gl1_hw_get_sync_rx_data(),
                    gl1_hw_get_sync_rx_data_amr() and
                    gl1_hw_get_async_rx_data().
02/11/05   gfr      Moved MDSP_RX_HDR_SIZE definition to mdsp driver
01/24/05   gfr      Use correct mDSP burst types
12/07/04   yhong    Lint cleanup
12/06/04   gfr      Featurize polar RF support.
11/30/04   gfr      Support for polar RF.
07/22/04   gfr      EGPRS support.
06/15/04   gfr      Removed SACCH specific rx functions, put rx RF code into
                    a common function for regular and AMR calls.
06/23/04   gfr      Lint cleanup.
06/10/04   gfr      Support for early CBCH decode.
05/20/04   gfr      Mainlined FEATURE_COMMON_PWR_HANDLER
05/06/04   gfr      Combine phase and freq value for acq results.
04/27/04   rv       Replaced T_RF_ZRF6250 with T_RF_GWZRF6250.
04/27/04   hg       Added FEATURE_MULTI_RF_CHAIN_API.
03/26/04   jtn      For GSM TX Use RF_NUM_BURSTS 1 for 6250, 2 for 6200.
03/25/04   jtn      Removed extra timeslot from TX tune time.
03/05/04    gfr     Lint cleanup.
02/23/04   gfr      Support for new rfgsm_set_pa_profile interface, and
                    changed timing offset to unsigned.
02/17/04   gfr      GSM rotators support.
02/11/04   gfr      Support for new HW scheduler.
01/30/04   gfr      Support for logging mdsp scaling factors
01/13/03   gsc      Use ARFC_T instead of uint16 in CBCH function.
12/10/03   gfr      Compiler fix.
12/05/03   bm       Added logging support for MDSPcommands issued,
                    and the corresponding pops
11/05/03   gfr      Support for quad-band.
10/21/03   gsc      Added SMS CB support.
08/29/03   jc       Mainlined RF_MULTIMODE, removed ypf functions no longer
                    needed by PLT.
08/21/03   gw       Added function gl1_hw_cleanup_abort_async_rx().
08/11/03   gsc      Added c_over_i handling to gl1_hw_get_syncrx_metrics for
                    AMR.
08/06/03   gw       Added function gl1_hw_abort_early_decode().
07/15/03   gsc      Included gl1_msg.h for AMR under FEATURE_GSM_AMR.
07/15/03   gsc      Included gl1_msg.h for AMR
07/01/03   gsc      Updated gl1_hw_get_syncrx_data_amr to remove a F3 message.
06/20/03   gfr      GSM power measures use rx alpha not mon alpha.
06/17/03   gfr      Different tune times for monitor versus rx bursts.
06/09/03   gfr      Set the async search width to 20 for ncell BCCH.
06/04/03   gfr      HW schedule routines no longer need the search width.
                    Lint cleanup.
06/01/03   dp       Fixed merge error
05/27/03   dp       mdsp async search width is no longer hardcoded to 100.
                    Its set to 100 for TCH and to 20 for non TCH channels
05/21/03   gw       Added support for early decode of paging block.
05/21/03   gfr      Save the rx schedule in AMR.
05/15/03   gfr      Common power handler support
05/15/03   thh      Set the Tx tune time to 2 slots as soon as we do a normal
                    Rx.
05/14/03   gfr      Save the start and stop acq schedule.
05/12/03   gfr      Save the rx and tx schedule.
05/12/03   gfr      Tx Alpha is now sent in the tx command.
05/12/02   gsc      Replaced amr_codec_mode_type with mdsp_amr_codec_mode_type.
05/07/03   gsc      Added GSM AMR support. Removed rfgsm_set_tx_setup_time from
                    gl1_hw_tx_normal_burst.
05/05/03   thh      Removed rfgsm_set_tx_setup_time() from gl1_hw_tx_rach_burst().
05/01/03   thh      Channel filter start/stop time is now handled by the RF
                    driver.
04/24/03   thh      Added calls to rfgsm_set_tx_setup_time().
03/28/03   gfr      Renamed gl1_log_monitor gl1_hw_log_monitor and made it not
                    static, it is now called by the GPRS monitor code as well.
03/05/03   dp       Put back call to gl1_log_monitor.   Corrected bug in
                    extracting voicing mode for half rate
02/28/03   dp       Commented out call to gl1_log_monitor for now to fix memory
                    corruption which was messing up ciphering
02/26/03   dp       Added half rate support featurized under
                    FEATURE_GSM_HALF_RATE
02/12/03   mk/gw    Initial revision created by moving code from gl1_hw.c.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif

#include "memory.h"
#include <stringl/stringl.h>

#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_debug.h"
#include "gl1_hwi.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "mdsp_intf_shared_vars.h"
#include "mdsp_gprs.h"
#include "rfm.h"
#include "rfm_meas.h"
#include "geran_msgs.h"
#include "l1_sc.h"
#include "vtmrs_g.h"
#include "gpl1_dual_sim.h"


#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
#include "l1_task.h"
#endif /*FEATURE_GSM_TX_DIV_ANTNA_SWITCHING*/

#include "mcs_hwio.h"

#include "gl1_defs.h"
#include "gl1_defs_g.h"

#include "gl1_msg_g.h"

#include "gl1_msgi_tch.h"

#include "l1_utils.h"
#include "l1_log.h"
#include "l1_task.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#ifdef FEATURE_GL1_GPLT
#include "gplt_result.h"
#endif /* FEATURE_GL1_GPLT */
#include <stringl/stringl.h>

#ifdef FEATURE_GSM_COEX
#include "gl1_hw_arbitration.h"
#endif
#ifdef FEATURE_GSM_RX_DIVERSITY
#include "ftm.h"
#include "gl1_hw_rxdiversity.h"
#endif

#ifdef FEATURE_GL1_ASDIV_ACQ
#include "gl1_asdiv.h"
#endif
#ifdef GL1_GMSK_MOD_DET
extern boolean gl1_hw_is_gdet_debug_enabled(void);
#endif
#define RF_NUM_BURSTS 2

/* Interferer Blocker Algorithm defines. */
#define TWENTY_QS  20

#define DCI_Q_LIMIT   1100

#ifdef FEATURE_GSM_COEX
extern gl1_hw_arbitration_t gl1_hw_arbitration[];
#endif

/*===========================================================================

                     DEFINITIONS FOR MODULE

===========================================================================*/

#ifdef FEATURE_GSM_AMR
#define GSM_AMR_TCH_AFS_ADJ_FACTOR  2621
#define GSM_AMR_TCH_AHS_ADJ_FACTOR  3277
#endif /* FEATURE_GSM_AMR */

/* The defines for accessing the R-SACCH control bits from the mDSP interface
   > SACCH Soft Bits Stored - Control Word 0 bit 1
   > Recombined Decode Attempted - Control Word 0 bit 10
 */
#define GL1_HW_CTRL_WRD_0_R_SACCH_BIT_1_SOFT_BITS_STORED   0x0002
#define GL1_HW_CTRL_WRD_0_R_SACCH_BIT_10_COMB_DEC_ATTEMPT  0x0400
#define GL1_HW_BIT_1_SOFT_BITS_STORED_RIGHT_SHIFT          1
#define GL1_HW_BIT_10_COMB_DEC_ATTEMPT_RIGHT_SHIFT         10


/*===========================================================================

                             LOCAL MACROS

===========================================================================*/


/*===========================================================================

                             LOCAL STORAGE

===========================================================================*/

#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
#error code not present
#endif

/* Convert from gl1 to mdsp acq_afc types */
mdsp_acq_afc_type gl1_hw_acq_afc_to_mdsp_acq_afc[] =
{
  MDSP_ACQ_AFC_DISABLED,          /* GL1_HW_ACQ_AFC_DISABLED */
  MDSP_ACQ_AFC_ON_PDM,            /* GL1_HW_ACQ_AFC_ON_PDM */
  MDSP_ACQ_AFC_ON_ROTATOR,        /* GL1_HW_ACQ_AFC_ON_ROTATOR */
  MDSP_ACQ_GSM_XOCAL_AFC_DISABLED /* GL1_HW_ACQ_GSM_XOCAL */
};

/* Convert from HW to mDSP channel type */
#define MDSP_CHANNEL_TYPE_LUT_ERROR FORCEWORD_LOGICAL_CHANNEL_TYPE
const mdsp_channel_type mdsp_channel_type_lut[] =
{
   MDSP_TCH_FS,          /* GL1_HW_TCH_FS             */
   MDSP_TCH_EFS,         /* GL1_HW_TCH_EFS            */
   MDSP_TCH_F96,         /* GL1_HW_TCH_F96            */
   MDSP_TCH_F144,        /* GL1_HW_TCH_F144           */
   MDSP_SACCH,           /* GL1_HW_SACCH              */
   MDSP_SDCCH,           /* GL1_HW_SDCCH              */
   MDSP_BCCH,            /* GL1_HW_BCCH               */
   MDSP_PCH,             /* GL1_HW_PCH                */
   MDSP_FACCH,           /* GL1_HW_FACCH              */
   MDSP_SCH,             /* GL1_HW_SCH                */
   MDSP_RACH,            /* GL1_HW_RACH               */
   MDSP_F96_AND_FACCH,   /* GL1_HW_TCH_F96_AND_FACCH  */
   MDSP_F144_AND_FACCH,  /* GL1_HW_TCH_F144_AND_FACCH */
   MDSP_TCH_HS,          /* GL1_HW_TCH_HS             */
   MDSP_FACCH_HS,        /* GL1_HW_FACCH_HS           */
#ifdef FEATURE_GSM_AMR
   MDSP_TCH_AFS,         /* GL1_HW_TCH_AFS            */
   MDSP_TCH_AHS,         /* GL1_HW_TCH_AHS            */
   MDSP_FACCH_AFS,       /* GL1_HW_FACCH_AFS          */
   MDSP_FACCH_AHS,       /* GL1_HW_FACCH_AHS          */
#ifdef FEATURE_GSM_AMR_WB
   MDSP_TCH_WFS,         /* GL1_HW_TCH_WFS            */
   MDSP_FACCH_WFS,       /* GL1_HW_FACCH_WFS          */
#else
   MDSP_CHANNEL_TYPE_LUT_ERROR,       /* GL1_HW_TCH_WFS            */
   MDSP_CHANNEL_TYPE_LUT_ERROR,       /* GL1_HW_FACCH_WFS          */
#endif
#else
   MDSP_CHANNEL_TYPE_LUT_ERROR,       /* GL1_HW_TCH_AFS            */
   MDSP_CHANNEL_TYPE_LUT_ERROR,       /* GL1_HW_TCH_AHS            */
   MDSP_CHANNEL_TYPE_LUT_ERROR,       /* GL1_HW_FACCH_AFS          */
   MDSP_CHANNEL_TYPE_LUT_ERROR,       /* GL1_HW_FACCH_AHS          */
   MDSP_CHANNEL_TYPE_LUT_ERROR,       /* GL1_HW_TCH_WFS            */
   MDSP_CHANNEL_TYPE_LUT_ERROR,       /* GL1_HW_FACCH_WFS          */
#endif
   MDSP_PTCCHD,          /* GL1_HW_PTCCHD             */
   MDSP_PRACH,           /* GL1_HW_PRACH_ACC          */
   MDSP_PTCCHU,          /* GL1_HW_PTCCHU_ACC         */
   MDSP_PDTCHU_CS1,      /* GL1_HW_PDTCHU_CS1         */
   MDSP_PDTCHU_CS2,      /* GL1_HW_PDTCHU_CS2         */
   MDSP_PDTCHU_CS3,      /* GL1_HW_PDTCHU_CS3         */
   MDSP_PDTCHU_CS4,      /* GL1_HW_PDTCHU_CS4         */
   MDSP_PDTCHD,          /* GL1_HW_PDTCHD             */
   MDSP_PACCHU,          /* GL1_HW_PACCHU             */
   MDSP_PRACH_EXT,       /* GL1_HW_PRACH_EXT          */
   MDSP_PTCCHU_EXT,      /* GL1_HW_PTCCHU_EXT         */
   MDSP_PACCHU_EXT,      /* GL1_HW_PACCHU_EXT         */
   MDSP_PDTCHD,          /* GL1_HW_PBCCH              */
   MDSP_PDTCHD,          /* GL1_HW_PCCCH              */
#if  defined(FEATURE_GSM_MDSP_EGPRS)
   MDSP_PDTCHU_MCS1,     /* GL1_HW_PDTCHU_MCS1        */
   MDSP_PDTCHU_MCS2,     /* GL1_HW_PDTCHU_MCS2        */
   MDSP_PDTCHU_MCS3,     /* GL1_HW_PDTCHU_MCS3        */
   MDSP_PDTCHU_MCS4,     /* GL1_HW_PDTCHU_MCS4        */
   MDSP_PDTCHU_MCS5,     /* GL1_HW_PDTCHU_MCS5        */
   MDSP_PDTCHU_MCS6,     /* GL1_HW_PDTCHU_MCS6        */
   MDSP_PDTCHU_MCS7,     /* GL1_HW_PDTCHU_MCS7        */
   MDSP_PDTCHU_MCS8,     /* GL1_HW_PDTCHU_MCS8        */
   MDSP_PDTCHU_MCS9,     /* GL1_HW_PDTCHU_MCS9        */
#else
   MDSP_CHANNEL_TYPE_LUT_ERROR,     /* GL1_HW_PDTCHU_MCS1        */
   MDSP_CHANNEL_TYPE_LUT_ERROR,     /* GL1_HW_PDTCHU_MCS2        */
   MDSP_CHANNEL_TYPE_LUT_ERROR,     /* GL1_HW_PDTCHU_MCS3        */
   MDSP_CHANNEL_TYPE_LUT_ERROR,     /* GL1_HW_PDTCHU_MCS4        */
   MDSP_CHANNEL_TYPE_LUT_ERROR,     /* GL1_HW_PDTCHU_MCS5        */
   MDSP_CHANNEL_TYPE_LUT_ERROR,     /* GL1_HW_PDTCHU_MCS6        */
   MDSP_CHANNEL_TYPE_LUT_ERROR,     /* GL1_HW_PDTCHU_MCS7        */
   MDSP_CHANNEL_TYPE_LUT_ERROR,     /* GL1_HW_PDTCHU_MCS8        */
   MDSP_CHANNEL_TYPE_LUT_ERROR,     /* GL1_HW_PDTCHU_MCS9        */
#endif

   MDSP_CHANNEL_TYPE_LUT_ERROR,     /* GL1_HW_RRBP_HI            */
   MDSP_CHANNEL_TYPE_LUT_ERROR,     /* GL1_HW_RRBP_LO            */

   FORCEWORD_LOGICAL_CHANNEL_TYPE   /* GL1_HW_NUM_CHANNEL_TYPES */
};

#if  defined(FEATURE_GSM_MDSP_ACI_DETECTION_VOICE)
/* Lookup table for ACI filter */
extern const gl1_defs_aci_filter_type mdsp_aci_filter_lut[];
#endif

extern boolean dynamic_tx_div_enable_disable;
extern uint32  antenna_to_use[]; /*L1_SC_PWR_MAX_MEAS_PER_CALL = 100*/

/* Structure to store RSSI and RX power in dBm for FTM mode */
typedef struct
{
  boolean init;
  uint32 rssi;
  dBx16_T pwr_dbm;
  uint32 rssi_divrx;
  dBx16_T pwr_dbm_divrx;
}gl1_hw_ftm_power_store_T;

gl1_hw_ftm_power_store_T gl1_hw_ftm_power_store[NUM_GERAN_DATA_SPACES];


typedef struct
{
   boolean  pending;
   gl1_defs_loopback_type mode;
}gl1_hw_mdsp_loopback_T;

gl1_hw_mdsp_loopback_T gl1_hw_mdsp_loopback[NUM_GERAN_DATA_SPACES] =
                         { INITIAL_VALUE_S( 0 )};

extern const mdsp_burst_type mdsp_gprs_mon_metrics_burst_types[];

static uint32 geran_vfr_allocated[NUM_GERAN_DATA_SPACES] =
                {INITIAL_VALUE(GERAN_VFR_INVALID)};

typedef struct
{
  boolean            valid;
  GfwSchResultStruct data;
} gl1_sch_store_t;


/*===========================================================================

                           FUNCTION PROTOTYPES

===========================================================================*/
/* Needs removing when Half rate performance improves for SAIC */
gl1_hw_channel_type tch_chan_type[NUM_GERAN_DATA_SPACES] =
                      { INITIAL_VALUE( GL1_HW_TCH_FS ) };

gl1_hw_sch_decode_t gl1_hw_x2g_get_sch_decode_mode(
                      boolean sch_decode );

#ifdef FEATURE_GSM_GPRS_TX_INFO
static ARFCN_T  gl1_hw_stored_arfcn[NUM_GERAN_DATA_SPACES];
#endif

#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY
boolean  critical_scn_updated_to_mcpm[NUM_GERAN_DATA_SPACES]= { INITIAL_VALUE( FALSE ) };
uint32  timeline_error_recovery_needed[NUM_GERAN_DATA_SPACES]= { INITIAL_VALUE( 0 ) };

#define GL1_HW_MAX_BURST_ERR_REC_COUNT 1
#define GL1_HW_MAX_BURST_SUCCESS_COUNT 5

gl1_hw_burst_recovery_count_t gl1_hw_burst_recovery_count[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
gl1_hw_burst_success_count_t  gl1_hw_burst_success_count[NUM_GERAN_DATA_SPACES]= { INITIAL_VALUE( 0 ) };
#endif
/*===========================================================================

                                FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION    gl1_hw_print_radio_block

DESCRIPTION This prints the Hex contents of a Radio Block. Note
            it's structured so only the required octets are
            printed to F3 reducing the load.

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_print_radio_block( uint8 *msg,gas_id_t gas_id )
{
   uint32  i, rb_data[6];
   uint32 *rb_data_p = rb_data;

   if ( !msg )
   {
     MSG_GERAN_ERROR_0_G( "NULL msg ptr");

     return;
   }

   /* Order the Octets for printing. */
   for (i=0; i < 5; i++)
   {
     *rb_data_p  = ( *msg++ << 24 );
     *rb_data_p |= ( *msg++ << 16 );
     *rb_data_p |= ( *msg++ << 8  );
     *rb_data_p |= ( *msg );
     rb_data_p++;

     if ( (*msg++) == 0x2B )
     {
       break;
     }
   }

   /* Only print the octets up to the boundary 0x2B fill. */
   switch ( i )
   {
      case 0:
        MSG_3(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
              "gs%d:Radio Block = %08x FN:%d",
              (gas_id+1),rb_data[0],GSTMR_GET_FN_GERAN(gas_id));
      break;

      case 1:
        MSG_4(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
              "gs%d:Radio Block = %08x %08x FN:%d",
              (gas_id+1),rb_data[0], rb_data[1],GSTMR_GET_FN_GERAN(gas_id));
      break;

      case 2:
        MSG_5(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
              "gs%d:Radio Block= %08x %08x %08x FN:%d",
              (gas_id+1),rb_data[0], rb_data[1], rb_data[2],GSTMR_GET_FN_GERAN(gas_id));
      break;

      case 3:
        MSG_6(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
              "gs%d:Radio Block = %08x %08x %08x %08x FN:%d",
              (gas_id+1),rb_data[0], rb_data[1],
              rb_data[2], rb_data[3], GSTMR_GET_FN_GERAN(gas_id));
      break;

      case 4:
        MSG_7(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
              "gs%d:Radio Block = %08x %08x %08x %08x %08x FN:%d",
              (gas_id+1),rb_data[0], rb_data[1], rb_data[2],
              rb_data[3], rb_data[4], GSTMR_GET_FN_GERAN(gas_id));
      break;

      case 5:
        *rb_data_p  = ( *msg++ << 24 );
        *rb_data_p |= ( *msg++ << 16 );
        *rb_data_p |= ( *msg   << 8  );

        MSG_8(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
              "gs%d:Radio Block = %08x %08x %08x %08x %08x %08x FN:%d",
              (gas_id+1),rb_data[0], rb_data[1], rb_data[2],
              rb_data[3], rb_data[4], rb_data[5], GSTMR_GET_FN_GERAN(gas_id));
      break;

      default:
        MSG_3(MSG_SSID_DFLT, MSG_LEGACY_ERROR,
              "Badly formatted message %d",i,0,0);
      break;
   }
}

/*===========================================================================

FUNCTION  gl1_hw_start_acquisition

DESCRIPTION
  This function initiates a tone detection on the specified frequency
  at the specified offset into the frame and at the specified
  received signal level

  Now only used for W2G and L2G

DEPENDENCIES
  ARFCN              - Radio Channel Number on which to detect tone.
  rx_signal_strength - Anticipated Signal Strength at Antenna.
  offset             - Offset in quarter symbols at which to start tone
                       detection
  sch_decode         - TRUE:  decode SCH after tone is detected
                       FALSE: don't decode SCH
  AFC                - TRUE:  adjust TCXO based on tone results
                       FALSE: don't adjust TCXO
  tags               - Pointer to where the gain will be stored

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_start_acquisition(
       ARFCN_T                 ARFCN,
       dBx16_T                 rx_signal_strength,
       uint16                  timing_offset,
       int16                   frequency_offset,
       boolean                 sch_decode,
       gl1_hw_acq_afc_type     AFC,
       gl1_hw_rx_tags_type    *tags
#ifdef FEATURE_DUAL_SIM
     , sys_modem_as_id_e_type  as_id
#endif /* FEATURE_DUAL_SIM*/
)
{
   gl1_hw_rx_tags_type *tag_ptr;
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /* FEATURE_DUAL_SIM*/

   tag_ptr = tags;
   if (sch_decode)
   {
     gl1_hw_cm_store_acq_tag(tags);
     tag_ptr = gl1_hw_cm_get_acq_tag();
   }

   gl1_hw_start_acquisition_geran(ARFCN,rx_signal_strength,timing_offset,
                                   frequency_offset, gl1_hw_x2g_get_sch_decode_mode( sch_decode ),
                                   AFC, tag_ptr, gl1_hw_cm_read_w2g_active(geran_map_nas_id_to_gas_id( as_id )), as_id);

}

/*===========================================================================

FUNCTION  gl1_hw_start_acquisition_geran

DESCRIPTION
  This function initiates a tone detection on the specified frequency
  at the specified offset into the frame and at the specified
  received signal level

DEPENDENCIES
  ARFCN              - Radio Channel Number on which to detect tone.
  rx_signal_strength - Anticipated Signal Strength at Antenna.
  offset             - Offset in quarter symbols at which to start tone
                       detection
  sch_decode         - TRUE:  decode SCH after tone is detected
                       FALSE: don't decode SCH
  AFC                - TRUE:  adjust TCXO based on tone results
                       FALSE: don't adjust TCXO
  tags               - Pointer to where the gain will be stored

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_start_acquisition_geran(
   ARFCN_T                 ARFCN,
   dBx16_T                 rx_signal_strength,
   uint16                  timing_offset,
   int16                   frequency_offset,
   gl1_hw_sch_decode_t     sch_decode,
   gl1_hw_acq_afc_type     AFC,
   gl1_hw_rx_tags_type    *tags,
   boolean                 disable_power_opt15,
   sys_modem_as_id_e_type  as_id
)
{
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
   boolean part_tone_det = FALSE;
   int32 phase_increment;
   rfgsm_rx_chan_params_type rx_chan_params[1];
   uint32 rf_seq_num;

   uint16                    num_bursts = 1;
   int32                     frequency_offset_xo = 0;

   rfgsm_rx_diversity_chan_params_type  div_rx_chan_params[1];
   boolean enable_rxd = FALSE;

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_start_acquisition: Return immediately");
   return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   memset (rx_chan_params,0,( sizeof (rfgsm_rx_chan_params_type)* 1));

   /* Calculate the phase increment as a combination of band-compensated
      current rotator value and given frequency offset */
   phase_increment = gl1_hw_calc_phase_increment(ARFCN,gas_id);

   /* Setup the MON buffer with the data required to establish a certain gain */
   /* on the receiver.  This will take effect on the next command that uses  */
   /* MDSP RX buffer (example, acquisition, sync RX, ..)                     */

     /* package up Rx chan params - only 1 set of params required per frame for Rx Normal Burst */
     rx_chan_params[0].burst_type = RF_MON_BURST;

     rx_chan_params[0].band       = gl1_hw_arfcn_to_rfgsm_band(ARFCN);
     rx_chan_params[0].arfcn      = (uint16)ARFCN.num;
     rx_chan_params[0].rx_lvl_dbm = (dBx16_T)rx_signal_strength;
     rx_chan_params[0].lna_range  = NULL;
     /* gain will need to become a ptr to FW interface memory */
     rx_chan_params[0].gain_ptr   = &tags->gain;

#ifdef GERAN_L1_ENHANCED_RX
     rx_chan_params[0].enh_rx_params.enh_rx_enable_flag = FALSE;
     rx_chan_params[0].enh_rx_params.enh_rx_offset_freq = 0;

     /*Based on the request from GRF team, LIF needs to be enabled for ACQ in FTM Cal mode to arrive at 
           * better sensitivity measurements. */
     if((ftm_get_mode() == FTM_MODE)&& (gl1_hw_get_enable_lif_in_ftm_cal(gas_id) == TRUE))
     {
       rx_chan_params[0].enh_rx_params.enh_rx_enable_flag  = TRUE;
       rx_chan_params[0].enh_rx_params.enh_rx_offset_freq  = GL1_LIF_FREQ_OFFSET;
     }
#endif /*GERAN_L1_ENHANCED_RX*/

#ifdef GERAN_L1_HLLL_LNASTATE
     rx_chan_params[0].high_lin_mode_active = &(tags->sawless_lnastate);
#endif /*GERAN_L1_HLLL_LNASTATE*/

     if(AFC == GL1_HW_ACQ_GSM_XOCAL)
     {
       rx_chan_params[0].freq_err = 0;
     }
     else
     {
       rx_chan_params[0].freq_err   = gl1_hw_rf_get_freq_error(gas_id);
     }

     /* create array of burst types - only 1 required for Rx Normal Burst*/

     /* if ARFCN is not a desense channel then correct for NCell freq error via DPLL */
     if ( gl1_hw_is_arfcn_xo_desense( ARFCN, gas_id )  != DESENSE_CHAN )
     {
       frequency_offset_xo         = gl1_hw_freqtonormxofreq( ARFCN, frequency_offset, gas_id );
       rx_chan_params[0].freq_err += frequency_offset_xo;
     }
     else
     {
       /* restore local freq error and correct for NCell freq error via rotator */
       gl1_hw_rf_update_freq_error( gl1_hw_get_xo_acc_freq_err(gas_id), gas_id );
          phase_increment += gl1_hw_hztophase((int32)frequency_offset);
     }

     div_rx_chan_params[0].rx_lvl_dbm = 0;
     div_rx_chan_params[0].gain_ptr = NULL;
     div_rx_chan_params[0].lna_range = NULL;
     enable_rxd = FALSE;


     if((gl1_hw_sleep_get_in_warmup_check(gas_id)==TRUE)
         && (gl1_hw_is_forced_wakeup(gas_id)==FALSE)
#ifdef FEATURE_QTA
         && (gl1_hw_qta_gap_active(gas_id)==FALSE)
#endif
       )
     { 
       gl1_hw_sleep_set_commit_mask (RX_BURST_COMMIT, gas_id); 
     } 

     rx_chan_params[0].rf_sequence_number = gl1_hw_get_next_rf_seq_num();
     rf_seq_num = rx_chan_params[0].rf_sequence_number;

     /* issue RF RX Burst command */
     gl1_hw_rf_rx_burst( num_bursts, rx_chan_params,
                         div_rx_chan_params, enable_rxd,
                         ACQ_MODE, gas_id );

   MSG_GERAN_LOW_3_G( "Acquisition phase inc:%d freq_offset:%d frequency_offset_xo:%d",
                      phase_increment, frequency_offset, frequency_offset_xo );

   /* enable partial tone detection only if SCH decode is not SET*/
   if(!sch_decode)
   {
   part_tone_det = gl1_hw_cm_status[gas_id].initialised;
   }
   
   MSG_GERAN_LOW_1_G( "ACQ: partial tone detection: %d",
                      part_tone_det );

#ifdef FEATURE_GSM_COEX
   /* Perform arbitration of activities prior to sending mDSP commands */
   gl1_hw_arbitration[gas_id].acq.arfcn = ARFCN;

   gl1_hw_arbitration[gas_id].notify_event(ARBITRATE_ACQ, gas_id);
#endif
   /* Reset SCH pointers so stale data ignored, except for T2G where acquisition state machines overlap */
   if ( (sch_decode != T2G_CONVENTIONAL_SCH_DECODE) && (sch_decode != T2G_PARALLEL_SCH_DECODE) )
   {
      mdsp_reset_sch_results_ptr(gas_id);
   }

   /* --------------------------------------------------------------------
   **               Initiate Acquisition Command
   ** --------------------------------------------------------------------
   */
   mdsp_start_acquisition
    (
      timing_offset,
      sch_decode,
      gl1_hw_acq_afc_to_mdsp_acq_afc[AFC],
      part_tone_det,
      phase_increment,
#ifdef FEATURE_GSM_COEX
      gl1_hw_arbitration[gas_id].acq.priority,
      gl1_hw_arbitration[gas_id].acq.desense_id,
      gl1_hw_arbitration[gas_id].acq.enhanced_fcch_detection,
#endif
      ARFCN,
      disable_power_opt15,
      rf_seq_num,
#ifdef FEATURE_GSM_RX_DIVERSITY
      enable_rxd,
#endif
      gas_id
    );

   gl1_hw_schedule_start_acq(timing_offset,gas_id);

   gl1_hw_log_cell_cmds_to_diag (cmd_start_acquisition, FALSE,gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_stop_acquisition_part2

DESCRIPTION
  This function terminates the acquisition process.

DEPENDENCIES
  offset  - Offset in quarter symbols at which to stop the
            tone detection process

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_stop_acquisition_part2(void)
{
   return;
}
/*===========================================================================

FUNCTION  gl1_hw_stop_acquisition

DESCRIPTION
  This function terminates the acquisition process.

DEPENDENCIES
  offset  - Offset in quarter symbols at which to stop the
            tone detection process

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_stop_acquisition_part1( uint16 offset
#ifdef FEATURE_DUAL_SIM
                                    , sys_modem_as_id_e_type as_id
#endif /*FEATURE_DUAL_SIM*/
                                    )
{
   /* This function used to stop rf and mdsp along with the
      hw layer when stopping an acquisition.
      Since gl1_hw_stop_acquisition_part2 has been removed,
      the code for gl1_hw_stop_acquisition_part1 has been
      shifted to gl1_hw_stop_acquisition. This function name
      remains here for WCDMA's usage as their code is shared
      between Saber and Raven.
   */
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/
   gl1_hw_stop_acquisition_geran(offset,geran_map_nas_id_to_gas_id(as_id));
}

void gl1_hw_stop_acquisition( uint16 offset
#ifdef FEATURE_DUAL_SIM
                              , sys_modem_as_id_e_type as_id
#endif /*FEATURE_DUAL_SIM*/
                               )
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/
   gl1_hw_stop_acquisition_geran(offset,geran_map_nas_id_to_gas_id(as_id));
}

void gl1_hw_stop_acquisition_geran( uint16 offset, gas_id_t gas_id )
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
  // In case of GPLT and no RF, return default/canned result
  MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_stop_acquisition : Return immediately");
  return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

    mdsp_stop_acquisition( offset, gas_id );

    gl1_hw_schedule_stop_acq( offset, gas_id );

    gl1_hw_log_cell_cmds_to_diag( cmd_stop_acquisition, FALSE, gas_id );
}

/*===========================================================================

FUNCTION  gl1_hw_was_sch_decoded

DESCRIPTION
  This function polls the MDSP buffers to verify whether there are
  SCH results available.  The availability of results indicates that
  the MDSP has detected and decoded an SCH.  The CRC is a strong indicator
  that the SCH burst is decoded correctly.  The SCH burst contains
  a 10-bit CRC.

DEPENDENCIES
  ARFCN              - Radio Channel Number on which to acquire.
  rx_signal_strength - Anticipated Signal Strength at Antenna.

RETURN VALUE
  time_offset        - The time offset from the start of acquisition
                       in quarter symbol units.
  sch_data           - The decoded data from the received SCH burst
  mdsp_payload       - optional pointer parameter to retrieve metrics
                       associated with the SCH burst.

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_was_sch_decoded
(
   ARFCN_T arfcn,  /* Used for logging */
   uint16 *id,
   int32  *time_offset,
   uint8  *sch_data,
   uint16 *mdsp_payload,
   uint8  *crc_pass
#ifdef FEATURE_DUAL_SIM
   ,sys_modem_as_id_e_type as_id
#endif /* FEATURE_DUAL_SIM*/
)
{
  boolean                 sch_decoded;
  gl1_hw_sch_decode_data *gl1_hw_was_sch_decode_data_p;
  gl1_hw_sch_decode_data  gl1_hw_was_sch_decode_data = { 0 };

#ifndef FEATURE_DUAL_SIM
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id( GERAN_ACCESS_STRATUM_ID_1 );
#endif /*FEATURE_DUAL_SIM*/

  gl1_hw_was_sch_decode_data_p = &gl1_hw_was_sch_decode_data;

  sch_decoded =
    gl1_hw_was_sch_decoded_geran( arfcn, GL1_HW_INVALID_BSIC, gl1_hw_was_sch_decode_data_p, as_id );

  *id           = gl1_hw_was_sch_decode_data_p->id;
  *time_offset  = gl1_hw_was_sch_decode_data_p->time_offset;
  *crc_pass     = gl1_hw_was_sch_decode_data_p->crc_pass;

  COPY_SCH_DATA( sch_data,
                 &gl1_hw_was_sch_decode_data_p->sch_data );

  if (mdsp_payload != NULL)
  {
     *mdsp_payload = 0;
  }
  if(gl1_hw_was_sch_decode_data_p->schedule_error)
  {
    sch_decoded = FALSE;
  }
  return ( sch_decoded );
}

/*===========================================================================

FUNCTION  gl1_hw_copy_sch_data

DESCRIPTION
  This function copies SCH data from source to destination.
  Can operate on arbritary data types as uses safe byte copy.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_copy_sch_data(       void *destination_sch_data_p,
                           const void *source_sch_data_p )
{
  memscpy( destination_sch_data_p, MAX_SCH_OCTETS,
           source_sch_data_p, MAX_SCH_OCTETS );
}

/*===========================================================================

FUNCTION  gl1_hw_was_sch_decoded_geran

DESCRIPTION
  This function polls the MDSP buffers to verify whether there are
  SCH results available.  The availability of results indicates that
  the MDSP has detected and decoded an SCH.  The CRC is a strong indicator
  that the SCH burst is decoded correctly.  The SCH burst contains
  a 10-bit CRC.

DEPENDENCIES
  ARFCN              - Radio Channel Number on which to acquire.
  rx_signal_strength - Anticipated Signal Strength at Antenna.

RETURN VALUE
  time_offset        - The time offset from the start of acquisition
                       in quarter symbol units.
  sch_data           - The decoded data from the received SCH burst
  mdsp_payload       - optional pointer parameter to retrieve metrics
                       associated with the SCH burst.

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_was_sch_decoded_geran(
          ARFCN_T                 arfcn,
          uint8                   bsic_expected,
          gl1_hw_sch_decode_data *gl1_hw_sch_decode_data_p,
          sys_modem_as_id_e_type  as_id )
{
   boolean             results = FALSE;
   GfwSchResultStruct *sch_data_p = NULL;

   gl1_sch_store_t     temp_sch_store[MAX_SCH_RES];
   uint16               i;
     

   gas_id_t            gas_id   = geran_map_nas_id_to_gas_id( as_id );

   memset(temp_sch_store,NULL,sizeof(temp_sch_store));
   gl1_hw_sch_decode_data_p->schedule_error = FALSE;

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   if (gplt_fbsb_data.success)
   {
     MSG_GERAN_HIGH_1_G("GPLT DBG: gl1_hw_was_sch_decoded: Returning SCH result gplt_fbsb_data.success = %d",
                        gplt_fbsb_data.success);

     results = TRUE;
   }
   else
   {
     MSG_GERAN_HIGH_1_G("GPLT DBG: gl1_hw_was_sch_decoded: Returning SCH result gplt_fbsb_data.failure_sch = %d",
                        gplt_fbsb_data.failure_sch);

     results = ( gplt_fbsb_data.failure_sch ? FALSE : TRUE );
   }
#else
   for (i=0;i< MAX_SCH_RES; i++)
   {
     if (mdsp_get_SCH_results( &(temp_sch_store[i].data), gas_id ) )
     {
       temp_sch_store[i].valid = TRUE;
       MSG_GERAN_HIGH_1_G(GL1_ACQ_HDR" results found idx %d",i);
       results = TRUE;
     }
   }
#endif

   /* Nothing valid so just return early */
   if ( !results )
   {
     return ( results );
   }

   /* now check if we need to look for specific BSIC*/
   if (bsic_expected != GL1_HW_INVALID_BSIC)
   {
     i=0;

     while ( (!sch_data_p) && (i < MAX_SCH_RES) )
     {
       if (temp_sch_store[i].valid)
       {
         if ( (temp_sch_store[i].data.schCrc & MDSP_SCH_RESULTS_MASK_CRC) &&
              (bsic_expected == ((temp_sch_store[i].data.schData & 0xfc) >> 2) ) )
         {
           sch_data_p = &(temp_sch_store[i].data);
           MSG_GERAN_HIGH_2_G(GL1_ACQ_HDR" BSIC found %d idx %d",bsic_expected,i);
         }
       }
       i++;
     }
     if (!sch_data_p)
     {
       sch_data_p = &(temp_sch_store[0].data); 
     }

   }
   else
   {
     sch_data_p = &(temp_sch_store[0].data);
      
   }

   /* Log the result */
   gl1_hw_log_decoded_sch_data( arfcn, sch_data_p, gas_id );

   gl1_hw_sch_decode_data_p->id          = sch_data_p->schToneCount;
   gl1_hw_sch_decode_data_p->time_offset = (int32)sch_data_p->schStartPosition;
   gl1_hw_sch_decode_data_p->schedule_error = sch_data_p->scheduleTimeError;

   /* If the CRC passed, declare victory */
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_was_sch_decoded: making results & *crc_pass true");

   if ( gplt_fbsb_data.success )
   {
     MSG_GERAN_HIGH_0_G( "GPLT DBG: gl1_hw_was_sch_decoded: making results & *crc_pass true" );

     gl1_hw_sch_decode_data_p->crc_pass = TRUE;
   }
   else
   {
     MSG_GERAN_HIGH_1_G( "GPLT DBG: gl1_hw_was_sch_decoded: results & *crc_pass  gplt_fbsb_data.failure_sch = %d",
                         gplt_fbsb_data.failure_sch );

     gl1_hw_sch_decode_data_p->crc_pass = ( gplt_fbsb_data.failure_sch ? FALSE : TRUE );
   }

   /* Parallel SCH indication bit here */
   gl1_hw_sch_decode_data_p->parallel_sch = FALSE;
#else
   gl1_hw_sch_decode_data_p->crc_pass     =
     ( ( sch_data_p->schCrc & MDSP_SCH_RESULTS_MASK_CRC ) ? TRUE : FALSE );

   /* Parallel SCH indication bit here */
   gl1_hw_sch_decode_data_p->parallel_sch =
     ( ( sch_data_p->parSchScheduled != 0x0 ) ? TRUE : FALSE );
#endif /* FEATURE_GL1_GPLT& FEATURE_GL1_GPLT_CANNED_RSLT */

   /* check if BSIC matches our expected value, set to CRC fail if mismatched*/
   if ( (bsic_expected != GL1_HW_INVALID_BSIC) && 
        ( gl1_hw_sch_decode_data_p->crc_pass ) &&
        (bsic_expected != ((sch_data_p->schData & 0xfc) >> 2)) ) 
   {
      MSG_GERAN_ERROR_3_G( GL1_ACQ_HDR" BSIC doesn't match set CRC=FAIL exp %d rxd %d [%d]",bsic_expected,((sch_data_p->schData & 0xfc) >> 2),GSTMR_GET_FN_GERAN(gas_id));
   }

   if ( gl1_hw_sch_decode_data_p->crc_pass )
   {
     if ( gl1_hw_sch_decode_data_p->parallel_sch )
     {
       /* Hz total frequency error from start */
       gl1_hw_sch_decode_data_p->coarse_freq =
         (int32)sch_data_p->coarse_freq_sch;

       /* Grab RSSI/SNR data as no FCCH available */
       gl1_hw_sch_decode_data_p->rssi =
         sch_data_p->rssi;

       gl1_hw_sch_decode_data_p->sch_snr =
         sch_data_p->schCombSNR;
     }
     else
     {
       gl1_hw_sch_decode_data_p->sch_snr =
         sch_data_p->schSnr;

       gl1_hw_sch_decode_data_p->coarse_freq =
         (int32)sch_data_p->coarse_freq;
     }

     COPY_SCH_DATA( &gl1_hw_sch_decode_data_p->sch_data,
                    &sch_data_p->schData );

     MSG_GERAN_HIGH_3_G( GL1_ACQ_HDR"SCH decoded->SNR:%d coarse_freq:%d parallel_sch:%d",
                         gl1_hw_sch_decode_data_p->sch_snr,
                         gl1_hw_sch_decode_data_p->coarse_freq,
                         gl1_hw_sch_decode_data_p->parallel_sch );

     MSG_GERAN_LOW_5_G( GL1_ACQ_HDR"SCH decoded data 0x%x ([0]0x%x [1]0x%x [2]0x%x [3]0x%x)",
                        sch_data_p->schData,
                        gl1_hw_sch_decode_data_p->sch_data[0],
                        gl1_hw_sch_decode_data_p->sch_data[1],
                        gl1_hw_sch_decode_data_p->sch_data[2],
                        gl1_hw_sch_decode_data_p->sch_data[3]);
   }
   else
   {
     MSG_GERAN_ERROR_3_G( GL1_ACQ_HDR"SCH->crc error SNR:%d coarse_freq:%d parallel_sch:%d",
                          sch_data_p->schSnr, (int32)sch_data_p->coarse_freq,
                          gl1_hw_sch_decode_data_p->parallel_sch );
   }

   /* If parallel SCH active then log associated data */
   if ( gl1_hw_sch_decode_data_p->parallel_sch )
   {
     GfwPllelSchLogPckt *parallel_sch_log_data_p;

     GfwPllelSchLogPckt  parallel_sch_log_data = { 0 };

     parallel_sch_log_data_p = &parallel_sch_log_data;

     /* Grab single Parallel SCH log data from shared firmware buffer */
     if ( mdsp_get_Parallel_SCH_log_data( parallel_sch_log_data_p, gas_id ) )
     {
       /* Log the result */
       gl1_hw_log_parallel_sch_data( arfcn, parallel_sch_log_data_p, gas_id );
     }
   }

   /* -----------------------------------------------------------
   ** If there are SCH results but the CRC is failed, we haven't
   ** decoded.
   ** -----------------------------------------------------------
   */

   return ( results );
}

/*===========================================================================

FUNCTION  gl1_hw_was_parallel_sch_detected

DESCRIPTION
  This function determines if a tone was detected

DEPENDENCIES
  A tone detection must be started before invoked before invoking
  this function

RETURN VALUE
  PARALLEL_SCH_DECODED     - a parallel SCH has been detected
  CONVENTIONAL_SCH_DECODED - a normal SCH has been detected
  SCH_CRC_FAILED           - fake SCH found and CRC fails
  NO_CELL_FOUND            - no SCH has been detected

SIDE EFFECTS
  None

===========================================================================*/
gl1_cell_acq_decode_status_t gl1_hw_was_parallel_sch_detected(
                               ARFCN_T         arfcn,
                               uint8           bsic_expected,
                               acq_decode_rpt *acq_decode_data_p,
                               acq_decode_rpt *acq_decode_rpt_p,
                               gas_id_t        gas_id )
{
  gl1_hw_sch_decode_data      *gl1_hw_was_sch_decode_data_p;
  boolean                      sch_decoded;

  gl1_cell_acq_decode_status_t sch_decode_status          = NO_CELL_FOUND;
  gl1_hw_sch_decode_data       gl1_hw_was_sch_decode_data = { 0 };

  /* Always assume no parallel SCH until data read */
  acq_decode_rpt_p->was_sch_decoded = FALSE;

  gl1_hw_was_sch_decode_data_p = &gl1_hw_was_sch_decode_data;

  sch_decoded = gl1_hw_was_sch_decoded_geran(
                  arfcn,
                  bsic_expected,
                  gl1_hw_was_sch_decode_data_p,
                  geran_map_gas_id_to_nas_id( gas_id ) );

  /* SCH data read from firmware so indicate good cell otherwise FCCH attempt */
  if ( sch_decoded )
  {
    if ( gl1_hw_was_sch_decode_data_p->crc_pass )
    {
      /* Copy data over and new log packet */
      acq_decode_data_p->id          =
        gl1_hw_was_sch_decode_data_p->id;

      COPY_SCH_DATA( &acq_decode_data_p->sch_data,
                     &gl1_hw_was_sch_decode_data_p->sch_data );

      MSG_GERAN_LOW_4_G( GL1_ACQ_HDR"Parallel SCH decoded data [0]0x%x [1]0x%x [2]0x%x [3]0x%x",
                         acq_decode_data_p->sch_data[0],
                         acq_decode_data_p->sch_data[1],
                         acq_decode_data_p->sch_data[2],
                         acq_decode_data_p->sch_data[3] );

      acq_decode_data_p->time_offset =
        gl1_hw_was_sch_decode_data_p->time_offset;

      acq_decode_data_p->snr =
        gl1_hw_was_sch_decode_data_p->sch_snr;

      /* Parallel SCH frequency magic */
      if ( gl1_hw_was_sch_decode_data_p->parallel_sch )
      {
        int32 sch_total_freq_err_hz =
                gl1_hw_was_sch_decode_data_p->coarse_freq;

        acq_decode_rpt_p->coarse_freq =
          sch_total_freq_err_hz;

        /* Parallel SCH success so copy total coarse freq (Hz) to afc_freq (Rot) */
        acq_decode_rpt_p->afc_freq =
          gl1_hw_hztophase( sch_total_freq_err_hz );

        /* For AGC param tracking */
        acq_decode_rpt_p->rssi =
          gl1_hw_was_sch_decode_data_p->rssi;

        sch_decode_status = PARALLEL_SCH_DECODED;
      }
      else
      {
        sch_decode_status = CONVENTIONAL_SCH_DECODED;
      }
    }
    else
    {
      sch_decode_status = SCH_CRC_FAILED;
    }

    /* Parallel SCH indication bit here */
    acq_decode_rpt_p->was_sch_decoded =
      gl1_hw_was_sch_decode_data_p->parallel_sch;
    acq_decode_data_p->schedule_error = gl1_hw_was_sch_decode_data_p->schedule_error;
  }

   MSG_GERAN_HIGH_2_G( GL1_ACQ_HDR"gl1_hw_was_parallel_sch_detected sched_error %d sch_decode_status:%d",
                      acq_decode_data_p->schedule_error, sch_decode_status );

  return ( sch_decode_status );
}

/*===========================================================================

FUNCTION  gl1_hw_was_gsm_cell_detected

DESCRIPTION
  This function determines if a tone was detected

DEPENDENCIES
  A tone detection must be started before invoked before invoking
  this function

RETURN VALUE
  FCCH_FOUND               - a valid FCCH tone found but no SCH
  PARALLEL_SCH_DECODED     - a parallel SCH has been detected
  CONVENTIONAL_SCH_DECODED - a normal SCH has been detected
  SCH_CRC_FAILED           - fake SCH found and CRC fails
  NO_CELL_FOUND            - no SCH has been detected

SIDE EFFECTS
  None

===========================================================================*/
gl1_cell_acq_decode_status_t gl1_hw_was_gsm_cell_detected(
                               ARFCN_T         arfcn,
                               uint8           bsic_expected,
                               acq_decode_rpt *acq_decode_data_p,
                               acq_decode_rpt *acq_decode_rpt_p,
                               gas_id_t        gas_id )
{
  gl1_cell_acq_decode_status_t gsm_cell_found = NO_CELL_FOUND;

  /* Check to see if we have found a tone yet */
  /* or if we have used up all our frames.    */
  if ( gl1_hw_is_parallel_sch_enabled() ||
       gl1_hw_is_x2g_parallel_sch_enabled() )
  {
    gsm_cell_found =
      gl1_hw_was_parallel_sch_detected( arfcn, bsic_expected, acq_decode_data_p,
                                        acq_decode_rpt_p,gas_id );
  }

  /* The rare case when FCCH and Conventional SCH found on same frame ! */
  if ( gsm_cell_found == CONVENTIONAL_SCH_DECODED )
  {
     /* FCCH detection should have occurred so populate params
      * and if not available for whatever reason continue as if nothing found
      */
     if ( !gl1_hw_gen_was_tone_detected( arfcn, acq_decode_rpt_p, gas_id ) )
     {
       MSG_GERAN_HIGH_0_G( "Conventional SCH Decoded, but no tone data available" );
     }
  }
  /* Parallel and Conventional SCH failed so try and see if FCCH found so far */
  else if ( gsm_cell_found == NO_CELL_FOUND ||
            gsm_cell_found == SCH_CRC_FAILED )
  {
     /* FCCH detection */
     if ( gl1_hw_gen_was_tone_detected( arfcn, acq_decode_rpt_p, gas_id ) )
     {
       gsm_cell_found = FCCH_FOUND;
     }
  }

  MSG_GERAN_HIGH_1_G( GL1_ACQ_HDR"gl1_hw_was_gsm_cell_detected gsm_cell_found:%d",
                      gsm_cell_found );

  return ( gsm_cell_found );
}

#ifdef FEATURE_GSM_AMR
/*===========================================================================

FUNCTION  gl1_hw_set_dl_acs_amr

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_set_dl_acs_amr( mdsp_amr_codec_mode_type *dl_acs, gas_id_t gas_id )
{
  mdsp_dl_codec_set_configure_cmd ( dl_acs, gas_id );
}
#endif /* FEATURE_GSM_AMR */

/*===========================================================================

FUNCTION  gl1_hw_write_tx_data

DESCRIPTION
  This function writes data to the mdsp data buffers for transmission later.
  The data written will be transmitted by the mdsp when it receives the transmit
  burst command (see gl1_hw_tx_normal_burst() ).   This function can be called at
  the task level in the block before the data is to be transmitted.

DEPENDENCIES
  This function must be called and completed before the frame tick indicating
  start of a transmit block.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_write_tx_data
(
   gl1_hw_channel_type  chan,
   uint8               *primary_data,
   uint16               primary_data_length,
   uint8               *secondary_data,        /* Used in CSD */
   uint16               secondary_data_length, /* Used in CSD */
   gl1_defs_tn_type     tn,                    /* Used in DTM */
   gl1_hw_tx_tags_type *tags,                  /* Used in DTM */
   gas_id_t             gas_id
)
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
  // In case of GPLT and no RF, return default/canned result
  MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_write_tx_data: Return immediately");
  *((uint16 *)primary_data) = gplt_rach_data.random_access_content;
  return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   uint8 p_len, s_len;
   mdsp_dtm_tx_msg_type msg;


   /* Firmware does uint16 copies on the data pointers, so unaligned accesses
      are extra slow, warn user */
   MDSP_PTR_CHECK(primary_data);
   MDSP_PTR_CHECK(secondary_data);

   /* Secondary data is FACCH portion of CSD and FACCH */
   p_len = (uint8) (primary_data_length + 1) / 2;
   s_len = (uint8) (secondary_data_length + 1) / 2;

   msg.gsm.len[0]  = p_len;
   msg.gsm.data[0] = (uint16 *)primary_data;    /*lint !e826*/
   msg.gsm.len[1]  = s_len;
   msg.gsm.data[1] = (uint16 *)secondary_data;  /*lint !e826*/

   if (ARR_SIZE(mdsp_channel_type_lut) != GL1_HW_NUM_CHANNEL_TYPES+1)
   {
     MSG_GERAN_ERROR_0_G("mdsp_channel_type_lut error");
   }


   if (mdsp_channel_type_lut[chan] != MDSP_CHANNEL_TYPE_LUT_ERROR)
   {
   (void) mdsp_dtm_write_tx_data
   (
      mdsp_channel_type_lut[chan],
      &msg,
      MDSP_PDCH_SPECIFIC,
      (mdsp_timeslot_type)tn,
      TRUE, /* let driver clear data buffer automatically */
      &tags->data_tag
      ,gas_id
   );
   }
   else
   {
     MSG_GERAN_ERROR_1_G("Invalid channel number: %d", chan);
   }

}

#ifdef FEATURE_GSM_AMR
/*===========================================================================

FUNCTION  gl1_hw_write_tx_data_amr

DESCRIPTION
  This function writes data to the mdsp data buffers for transmission later.
  The data written will be transmitted by the mdsp when it receives the transmit
  burst command (see gl1_hw_tx_normal_burst() ).   This function can be called at
  the task level in the block before the data is to be transmitted.

DEPENDENCIES
  This function must be called and completed before the frame tick indicating
  start of a transmit block.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_write_tx_data_amr
(
   gl1_hw_channel_type  chan,
   uint8               *data,
   uint16               len,
   gl1_defs_tn_type     tn,    /* Used in DTM */
   gl1_hw_tx_tags_type *tags,  /* Used in DTM */
   gas_id_t             gas_id
)
{
   /* In DTM there is no difference between AMR and non-AMR tx data */
   gl1_hw_write_tx_data(chan, data, len, NULL, 0, tn, tags,gas_id);

}
#endif /* FEATURE_GSM_AMR */

/*===========================================================================

FUNCTION  gl1_hw_get_sync_rx_data

DESCRIPTION
  This function retrieves data from the mdsp buffers.

DEPENDENCIES
  This function must be called and completed before the frame tick indicating
  start of a transmit block.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_get_sync_rx_data
(
   gl1_hw_channel_type     chan,
   gl1_hw_rx_tags_type    *tags,
   gl1_defs_rx_hdr_struct *rpt_hdr,
   uint8                  *data,
   uint16                  data_len,
   gas_id_t                gas_id
)
{
   boolean results_avail;
   GfwDecodedDataStruct rx;

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
   GfwDtxAmrPdMetrics gfw_amr_pd_dtx_detect_metrics;
#endif

   uint16 *data_ptrs[GL1_DEFS_MSG_PER_SLOT];

   /* OK to cast data from uint8 ptr to uint16 ptr because the mdsp will
      take care of the alignment, but it is slower, so warn user */
   MDSP_PTR_CHECK(data);

   /* Only a single message per block for GSM */
   data_ptrs[0] = (uint16 *)data;  /*lint !e826*/
#ifdef FEATURE_GSM_MDSP_EGPRS
   data_ptrs[1] = NULL;
#endif
   results_avail = mdsp_dtm_get_rx_data(tags->data_tag, &rx, data_ptrs, TRUE ,gas_id);

   if (results_avail)
   {
      rpt_hdr->good_data     = rx.gfwDecodedCtrlWord2.crc1;
      rpt_hdr->firecode_used = rx.gfwDecodedCtrlWord2.Ir2WOrFireCode;
      rpt_hdr->len           = rx.len;

#ifdef FEATURE_VAMOS_II
      rpt_hdr->vamospaired   = rx.bVamosPaired;
#endif

      /* Determine if it is a FACCH */
      if (rx.gfwDecodedCtrlWord1.facch)
      {
         rpt_hdr->chan = (uint16)GL1_HW_FACCH;
      }
      else
      {
         rpt_hdr->chan = (uint16)chan;
      }

      rpt_hdr->rx_qual      = rx.hardRxQual;
      rpt_hdr->rx_qual_s    = rx.softRxQual;
      rpt_hdr->bfi          = FALSE;  /* bfi is calculated by ARM algorithm */
      rpt_hdr->sid          = data[0] | (data[1] << 8);
      rpt_hdr->voicing_mode = rx.gfwDecodedCtrlWord1.hrVoicingModeOrSidParams;

      rpt_hdr->epdLogPacket = rx.epdLogPacket;

#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
      #error code not present
#endif

      /* Extract from the mDSP interface whether rx'd FACCH blks Soft
         decision bits were stored (bit 8 of 1st control word) and
         whether mDSP carried out a combined Soft decision decode
         (bit 9 of 1st control word).
      */
      rpt_hdr->rfacchLogBuff = rx.rfacchLogBuff;
      rpt_hdr->rfacch_store_status  = rx.gfwDecodedCtrlWord1.rfacchStoreStatus;
      rpt_hdr->rfacch_comb_status   = rx.gfwDecodedCtrlWord1.rfacchCombStatus;

 #ifdef DEBUG_FEATURE_GSM_R_FACCH
      #error code not present
#endif
      /*Extract "SACCH Stored" bit 1 and "Combined Decode Attempted" bit 10 from
        Control Word 0 from the mDSP interface.*/
      rpt_hdr->rsacch_store_status  = rx.gfwDecodedCtrlWord1.rsacchStoreStatus;

      rpt_hdr->rsacch_comb_status   = rx.gfwDecodedCtrlWord1.rsacchCombStatus;
 #ifdef DEBUG_FEATURE_GSM_R_SACCH
      #error code not present
#endif

      /* Print good FACCH blocks for debug.*/
      if (rpt_hdr->good_data &&
          (rpt_hdr->chan  == (uint16)GL1_HW_FACCH ||
           rpt_hdr->chan == (uint16)GL1_HW_FACCH_HS))
      {
        gl1_hw_print_radio_block(data,gas_id);
      }
#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
      mdsp_gprs_get_dtx_amr_pd_metrics(tags->metrics_tag, &gfw_amr_pd_dtx_detect_metrics, gas_id);

      rpt_hdr->gfw_dtx_ind_dec        = gfw_amr_pd_dtx_detect_metrics.bDtxIndDec;
      rpt_hdr->gfw_amr_pd_ind_dec     = gfw_amr_pd_dtx_detect_metrics.bAmrPdIndDec;
      rpt_hdr->gfw_gl1_disable_amr_pd = gfw_amr_pd_dtx_detect_metrics.bFwToL1DisableAmrPd;

      gl1_msg_amr_pd_cancel_partial_decode(rpt_hdr->gfw_gl1_disable_amr_pd, gas_id);
#endif
   }
   else
   {
      rpt_hdr->good_data = FALSE;
      rpt_hdr->firecode_used = FALSE;
   }

#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
   #error code not present
#endif

}

#ifdef FEATURE_GSM_AMR
/*===========================================================================

FUNCTION  gl1_hw_get_sync_rx_data_amr

DESCRIPTION
  This function retrieves data from the mdsp buffers.

DEPENDENCIES
  This function must be called and completed before the frame tick indicating
  start of a transmit block.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_get_sync_rx_data_amr
(
   gl1_hw_channel_type     chan,
   gl1_hw_rx_tags_type    *tags,
   gl1_defs_rx_hdr_struct *rpt_hdr,
   uint8                  *data,
   uint16                  data_len,
   gas_id_t                gas_id
)
{
  boolean results_avail;
  GfwDecodedDataStruct rx;

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
   GfwDtxAmrPdMetrics gfw_amr_pd_dtx_detect_metrics;
#endif

  uint16 *data_ptrs[GL1_DEFS_MSG_PER_SLOT];

   /* OK to cast data from uint8 ptr to uint16 ptr because the mdsp will
     take care of the alignment, but it is slower, so warn user */
  MDSP_PTR_CHECK(data);

  /* Only a single message for GSM */
  data_ptrs[0] = (uint16 *)data;  /*lint !e826*/
#ifdef FEATURE_GSM_MDSP_EGPRS
  data_ptrs[1] = NULL;
#endif
  results_avail = mdsp_dtm_get_rx_data(tags->data_tag, &rx, data_ptrs, TRUE ,gas_id);


  if (results_avail)
  {
    rpt_hdr->good_data     = rx.gfwDecodedCtrlWord2.crc1;
    rpt_hdr->firecode_used = rx.gfwDecodedCtrlWord2.Ir2WOrFireCode;
    rpt_hdr->len           = rx.len;

#ifdef FEATURE_VAMOS_II
    rpt_hdr->vamospaired   = rx.bVamosPaired;
#endif

    /* There is a problem in the mDSP setting FACCH and SID_FIRST. */
    if (rx.gfwDecodedCtrlWord1.facch)
    {
      /* Is it FACCH? */
      if ( chan == GL1_HW_TCH_AFS )
      {
        /* Is it FACCH FS? */
        rpt_hdr->chan = (uint16)GL1_MSG_FACCH_AFS;
      }
#ifdef FEATURE_GSM_AMR_WB
      else if ( chan == GL1_HW_TCH_WFS )
      {
        rpt_hdr->chan = (uint16)GL1_MSG_FACCH_WFS;
      }
#endif
      else
      {
        /* Is it FACCH FS? */
        rpt_hdr->chan = (uint16)GL1_MSG_FACCH_AHS;
      }
      /* Extract from the mDSP interface whether rx'd FACCH blks Soft
         decision bits were stored (bit 8 of 1st control word) and
         whether mDSP carried out a combined Soft decision decode
         (bit 9 of 1st control word).
      */
      rpt_hdr->rfacchLogBuff = rx.rfacchLogBuff;
      rpt_hdr->rfacch_store_status  = rx.gfwDecodedCtrlWord1.rfacchStoreStatus;
      rpt_hdr->rfacch_comb_status   = rx.gfwDecodedCtrlWord1.rfacchCombStatus;
    }
    else if (rx.gfwDecodedCtrlWord1.ratScchDecode)
    {
      /* Is it RATSCCH? */
      if ( chan == GL1_HW_TCH_AFS )
      {
        /* Is it RATSCCH_FS? */
        rpt_hdr->chan = (uint16)GL1_MSG_RATSCCH_FS;
      }
#ifdef FEATURE_GSM_AMR_WB
      else if ( chan == GL1_HW_TCH_WFS )
      {
        rpt_hdr->chan = (uint16)GL1_MSG_RATSCCH_WFS;
      }
#endif
      else
      {
        /* Is it RATSCCH_HS? */
        rpt_hdr->chan = (uint16)GL1_MSG_RATSCCH_HS;
      }
    }
    else if (rx.gfwDecodedCtrlWord1.ratScchMarker)
    {
      /* Is it RATSCCH_MARKER? */
      rpt_hdr->chan = (uint16)GL1_MSG_RATSCCH_MARKER;
    }
    /* There is a problem in the mDSP setting FACCH and SID_FIRST. */
    else if (rx.gfwDecodedCtrlWord1.sidFirst)
    {
      /* Is is SID_FIRST? */
      if ( !rx.gfwDecodedCtrlWord1.inhibit)
      {
        /* If not INHIBIT */
#ifdef FEATURE_GSM_AMR_WB
        if (( chan == GL1_HW_TCH_AFS )||( chan == GL1_HW_TCH_WFS ))
#else
        if ( chan == GL1_HW_TCH_AFS )
#endif
        {
          /* Is it SID_FIRST_FS? */
          rpt_hdr->chan = (uint16)GL1_MSG_SID_FIRST_FS;
        }
        else
        {
          /* Is it SID_FIRST_HS? */
          rpt_hdr->chan = (uint16)GL1_MSG_SID_FIRST_HS;
        }
      }
      else
      {
        /* It is SID_FIRST_INH */
        rpt_hdr->chan = (uint16)GL1_MSG_SID_FIRST_INH;
      }
    }
    else if (rx.gfwDecodedCtrlWord1.hrVoicingModeOrSidParams & 0x1)
    {
      /* Is is SID_UPDATE? */
      if ( !rx.gfwDecodedCtrlWord1.inhibit )
      {
        /* If not INHIBIT */
#ifdef FEATURE_GSM_AMR_WB
        if (( chan == GL1_HW_TCH_AFS )||( chan == GL1_HW_TCH_WFS ))
#else
        if ( chan == GL1_HW_TCH_AFS )
#endif
        {
          /* Is it SID_UPDATE_FS? */
          rpt_hdr->chan = (uint16)GL1_MSG_SID_UPDATE_FS;
        }
        else
        {
          /* Is it SID_UPDATE_HS? */
          rpt_hdr->chan = (uint16)GL1_MSG_SID_UPDATE_HS;
        }
      }
      else
      {
        /* It is SID_UPDATE_INH */
        rpt_hdr->chan = (uint16)GL1_MSG_SID_UPDATE_INH;
      }
    }
    else
    {
      if ( chan == GL1_HW_TCH_AFS )
      {
        rpt_hdr->chan = (uint16)GL1_MSG_TCH_AFS;
      }
#ifdef FEATURE_GSM_AMR_WB
      else if ( chan == GL1_HW_TCH_WFS )
      {
        rpt_hdr->chan = (uint16)GL1_MSG_TCH_WFS;
      }
#endif
      else
      {
        rpt_hdr->chan = (uint16)GL1_MSG_TCH_AHS;
      }
    }

    rpt_hdr->rx_qual   = rx.hardRxQual;
    rpt_hdr->rx_qual_s = rx.softRxQual;
    rpt_hdr->onset     = ((rx.gfwDecodedCtrlWord1.hrVoicingModeOrSidParams & 0x2) >> 1) ? TRUE : FALSE;
    rpt_hdr->phase     = rx.gfwDecodedCtrlWord1.phase;
    rpt_hdr->cmc       = rx.gfwDecodedCtrlWord1.cmc;
    rpt_hdr->cmi       = rx.gfwDecodedCtrlWord1.cmi;

#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
    #error code not present
#endif

    /* Print good FACCH blocks for debug.*/
    if (rpt_hdr->good_data &&
        (rpt_hdr->chan == (uint16)GL1_MSG_FACCH_AFS ||
#ifdef FEATURE_GSM_AMR_WB
         rpt_hdr->chan == (uint16)GL1_MSG_FACCH_WFS ||
#endif
         rpt_hdr->chan == (uint16)GL1_MSG_FACCH_AHS))
    {
      gl1_hw_print_radio_block(data,gas_id);
    }
#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
    mdsp_gprs_get_dtx_amr_pd_metrics(tags->metrics_tag, &gfw_amr_pd_dtx_detect_metrics, gas_id);

    rpt_hdr->gfw_dtx_ind_dec        = gfw_amr_pd_dtx_detect_metrics.bDtxIndDec;
    rpt_hdr->gfw_amr_pd_ind_dec     = gfw_amr_pd_dtx_detect_metrics.bAmrPdIndDec;
    rpt_hdr->gfw_gl1_disable_amr_pd = gfw_amr_pd_dtx_detect_metrics.bFwToL1DisableAmrPd;

    gl1_msg_amr_pd_cancel_partial_decode(rpt_hdr->gfw_gl1_disable_amr_pd, gas_id);
#endif
  }
  else
  {
    rpt_hdr->good_data = FALSE;
    rpt_hdr->firecode_used = FALSE;
  }
}
#endif /* FEATURE_GSM_AMR */

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
void gl1_hw_init_tch( gl1_hw_channel_type chan, gas_id_t gas_id )
{
   vstmr_rtc_id           rtc_id = VSTMR_RTC_INVALID;
   uint32                 vfr_id = GERAN_VFR_INVALID;
   /*
    * Use different adjustment factor depending on AMR full or half rate.
    * NOTE: for WB_AMR, systems team need to evaluate a new value but will
    * use current NB value during WB_AMR bring-up
    */
   switch ( chan )
   {
#ifdef FEATURE_GSM_AMR
      case GL1_HW_TCH_AFS:
#ifdef FEATURE_GSM_AMR_WB
      case GL1_HW_TCH_WFS:
#endif
         mdsp_intf_shared_var_write(GFWSHAREDVARIABLES_EDGE_COVERISIGMA,(uint16)GSM_AMR_TCH_AFS_ADJ_FACTOR ,gas_id);
         mdsp_updated_shared_variable(GFW_EDGE_SHARED_VARIABLE,gas_id);
         break;

      case GL1_HW_TCH_AHS:
         mdsp_intf_shared_var_write(GFWSHAREDVARIABLES_EDGE_COVERISIGMA,(uint16)GSM_AMR_TCH_AHS_ADJ_FACTOR ,gas_id);
         mdsp_updated_shared_variable(GFW_EDGE_SHARED_VARIABLE,gas_id);
         break;
#endif /* FEATURE_GSM_AMR */

      default:
         break;
   }

   /* Needs Removing when performance on SAIC half rate improves */

   /* Store channel type */
   tch_chan_type[gas_id] = chan;


   gl1_hw_set_ftm_power_store_init(TRUE,gas_id);

   if ( geran_vfr_allocated[gas_id] == GERAN_VFR_INVALID )
   {
      if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
      {
         rtc_id = VSTMR_RTC_GERAN_SUB0;
      }
      else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
      {
         rtc_id = VSTMR_RTC_GERAN_SUB1;
      }
      else
      {
         MSG_GERAN_ERROR_1_G("VFR: unsupported gas_id (%d) for VFR allocation", gas_id);
      }


      switch(gl1_get_stored_vfrmap(gas_id))
      {
          case SYS_VFR_PRIMARY:

         MSG_GERAN_HIGH_0_G("Allocate Primary VFR for VSID 0x11C05000");
         vfr_id = GERAN_VFR_PRIMARY;
#ifdef FEATURE_CONFIG_VFR_REGISTER
         /* Set VFR select to vstmr_vfr_0 */
         HWIO_VFR_SEL_OUT(0x1);
#endif
             break;   

          case SYS_VFR_SECONDARY:

         MSG_GERAN_HIGH_0_G("Allocate Secondary VFR for VSID 0x11DC5000");
         vfr_id = GERAN_VFR_SECONDARY;
#ifdef FEATURE_CONFIG_VFR_REGISTER
              /* Set VFR select to vstmr_vfr_0 */
         HWIO_VFR_SEL_1_OUT(0x1);

#endif
              break;

          default:
              vfr_id = GERAN_VFR_INVALID;
      }  
  

      if ((rtc_id != VSTMR_RTC_INVALID) && (vfr_id != GERAN_VFR_INVALID))
      {
         geran_vfr_allocated[gas_id] = vstmr_geran_vfr_allocate( rtc_id,
                                                                 VSTMR_GERAN_GSTMR_VIEW,
                                                                 vfr_id );
      }

      MSG_GERAN_MED_1_G ("GERAN VSTMR VFR %d allocated", geran_vfr_allocated[gas_id]);
   }
   else
   {
     MSG_GERAN_ERROR_0_G("VFR has already been allocated, do not allocate again");
   }
}

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
void gl1_hw_terminate_tch( gas_id_t gas_id )
{
#ifdef FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY
   gl1_hw_rf_recovery_cleanup(TRUE,gas_id);
#endif

   gl1_hw_set_ftm_power_store_init(FALSE,gas_id);
   if ( geran_vfr_allocated[gas_id] != GERAN_VFR_INVALID )
   {
      if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
      {
         vstmr_geran_vfr_release( geran_vfr_allocated[gas_id],
                                  VSTMR_RTC_GERAN_SUB0 );
      }
      else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
      {
         vstmr_geran_vfr_release( geran_vfr_allocated[gas_id],
                                  VSTMR_RTC_GERAN_SUB1 );
      }
      else
      {
         MSG_GERAN_ERROR_1_G("VFR: unsupported gas_id (%d) for VFR release", gas_id);
      }

#ifdef FEATURE_CONFIG_VFR_REGISTER
      if (geran_vfr_allocated[gas_id] == GERAN_VFR_PRIMARY)
      {
         /* Set VFR select to gnd */
         HWIO_VFR_SEL_OUT(0);
      }
      else if (geran_vfr_allocated[gas_id] == GERAN_VFR_SECONDARY)
      {
         /* Set VFR select 1 to gnd */
         HWIO_VFR_SEL_1_OUT(0);
      }
#endif
     MSG_GERAN_MED_1_G ("VFR %d released", geran_vfr_allocated[gas_id]);
     geran_vfr_allocated[gas_id] = GERAN_VFR_INVALID;

   }
   else
   {
     MSG_GERAN_MED_0_G ("VFR has already been released, do not release again");
   }
}

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
void gl1_hw_align_vfr( gas_id_t gas_id )
{
  vstmr_gstmr_t  geran_vfr_sync_cnt;
  uint32 frame_num;
  int    as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  geran_vfr_sync_cnt = vstmr_geran_get_current_gstmr ( gtmrs_data[as_idx].geran_view );

  frame_num = geran_vfr_sync_cnt.frame_num;

  geran_vfr_sync_cnt.frame_num = (frame_num + 1)%GERAN_VSTMR_MAX_FN;

  if ( frame_counters[gas_id].FNmod13 == 3 )
  {
    geran_vfr_sync_cnt.raw_13M = VSTMR_GERAN_FRAME_RANGE/3;
  }
  else if ( frame_counters[gas_id].FNmod13 == 7 )
  {
    geran_vfr_sync_cnt.raw_13M = 2*(VSTMR_GERAN_FRAME_RANGE/3);
  }
  else if ( frame_counters[gas_id].FNmod13 == 12 )
  {
    geran_vfr_sync_cnt.raw_13M = 0*(VSTMR_GERAN_FRAME_RANGE/3);
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Attempting to align VFR on a wrong frame, FNmod13=%d", frame_counters[gas_id].FNmod13);
  }

  if ( geran_vfr_allocated[gas_id] != GERAN_VFR_INVALID )
  {
      vstmr_geran_vfr_sync( geran_vfr_allocated[gas_id],
                            geran_vfr_sync_cnt );

      MSG_GERAN_MED_1_G("VFR sucesfully aligned, FNmod13=%d", frame_counters[gas_id].FNmod13);
  }
  else
  {
      MSG_GERAN_MED_1_G("VFR do not align - already released, FNmod13=%d", frame_counters[gas_id].FNmod13);
  }
}
/*===========================================================================

FUNCTION gl1_hw_store_rx_power

DESCRIPTION
  This function stores RSSI and Gain for use by FTM mode


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void gl1_hw_ftm_store_rx_power( uint32 rssi, dBx16_T pwr_dbm,
                                uint32 rssi_divrx, dBx16_T pwr_dBm_divrx,
                                gas_id_t gas_id )
{
  gl1_hw_ftm_power_store[gas_id].rssi = rssi;
  gl1_hw_ftm_power_store[gas_id].pwr_dbm = pwr_dbm;
  gl1_hw_ftm_power_store[gas_id].rssi_divrx = rssi_divrx;
  gl1_hw_ftm_power_store[gas_id].pwr_dbm_divrx = pwr_dBm_divrx;
}

/*===========================================================================

FUNCTION gl1_hw_set_ftm_power_store_init

DESCRIPTION
  This function stores RSSI and Gain for use by FTM mode


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void gl1_hw_set_ftm_power_store_init(boolean ftm_power_store_init,gas_id_t gas_id)
{
  if (ftm_power_store_init)
  {
    /* Clear out any old data in the buffer when we init transfer */
    gl1_hw_ftm_power_store[gas_id].rssi = 0;
    gl1_hw_ftm_power_store[gas_id].pwr_dbm = 0;
    gl1_hw_ftm_power_store[gas_id].rssi_divrx = 0;
    gl1_hw_ftm_power_store[gas_id].pwr_dbm_divrx = 0;
  }

  gl1_hw_ftm_power_store[gas_id].init = ftm_power_store_init;
}



/*===========================================================================

FUNCTION gl1_hw_ftm_get_rx_power_results

DESCRIPTION
    This function retrieves Rx power results in TCH dedicated mode for FTM mode



DEPENDENCIES
  None

RETURN VALUE
  TRUE if results are valid, FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/

boolean gl1_hw_ftm_get_rx_power_results( uint32 *rssi, dBx16_T *pwr_dbm,
                                         uint32 *rssi_divrx, dBx16_T *pwr_dbm_divrx,
                                         sys_modem_as_id_e_type as_id )
{
  /* API needs update when we need to support non signalling on 2nd chain*/

  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  MSG_GERAN_HIGH_2_G("ftm_get_rx_power_results - gas_id = %d, as_id = %d", gas_id,as_id);

  if (gl1_hw_ftm_power_store[gas_id].init)
  {
    *rssi =  gl1_hw_ftm_power_store[gas_id].rssi;
    *pwr_dbm =  gl1_hw_ftm_power_store[gas_id].pwr_dbm;
#ifdef FEATURE_GSM_RX_DIVERSITY
    *rssi_divrx    =  gl1_hw_ftm_power_store[gas_id].rssi_divrx;
    *pwr_dbm_divrx =  gl1_hw_ftm_power_store[gas_id].pwr_dbm_divrx;
#else /*FEATURE_GSM_RX_DIVERSITY*/
    *rssi_divrx    =  0;
    *pwr_dbm_divrx =  0;
#endif /*FEATURE_GSM_RX_DIVERSITY*/
  }

  return (gl1_hw_ftm_power_store[gas_id].init);
}

/*===========================================================================

FUNCTION gl1_hw_get_sync_rx_metrics

DESCRIPTION
  This function retreives the metrics for the burst received in the previous
  frame.

DEPENDENCIES
  A burst receive command must have been issued two frames previously to
  cause a burst to be received in the previous frame.

RETURN VALUE
  blocker TRUE/FALSE

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_hw_get_sync_rx_metrics
(
   gl1_defs_metrics_rpt *rpt,
   gl1_hw_rx_tags_type  *tags,
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
   gl1_log_chan_type log_chan_type,
#endif
   gas_id_t             gas_id
)
{
   static GfwBurstMetricsStruct  sync_metrics[3];

#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_rxd_control_type gl1_rxd_ctl_flag;
#endif
#ifdef FEATURE_VAMOS_II
   boolean ShiftedSacchEn = FALSE;
#endif
#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
   GfwDtxAmrPdMetrics gfw_amr_pd_dtx_detect_metrics;
#endif

   /* Interferer Id. */
   boolean blocker = FALSE;

   /* Reset the metrics */
   memset( &sync_metrics[gas_id], 0, sizeof( GfwBurstMetricsStruct ) );

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))

   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_get_sync_rx_metrics: Not calling mdsp_dtm_get_rx_burst_metrics");

#else

   (void) mdsp_dtm_get_rx_burst_metrics
    (
      tags->metrics_tag,
      &sync_metrics[gas_id],
      (boolean) (loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_C)
     ,gas_id
    );

   gl1_log_gsm_dft_spur_metric( sync_metrics[gas_id].gfwDftSpurAciMetrics,
                                sync_metrics[gas_id].pdch,
                                gas_id
                              );
#endif

   rpt->rssi          = (sync_metrics[gas_id].rssiMsw << 16) + sync_metrics[gas_id].rssiLsw;

#ifdef GERAN_L1_ENHANCED_RX
   rpt->WBEE = (sync_metrics[gas_id].rxdLogPacket.rxdMisc.rxdSelStream == 2)?sync_metrics[gas_id].gfwLifBurstMetricsStruct.wbeeWbdcEstDRx.wbPwrEstRaw:sync_metrics[gas_id].gfwLifBurstMetricsStruct.wbeeWbdcEstPRx.wbPwrEstRaw;
   rpt->WBEE_DRx = sync_metrics[gas_id].gfwLifBurstMetricsStruct.wbeeWbdcEstDRx.wbPwrEstRaw;
   rpt->lif_offset_freq = sync_metrics[gas_id].gfwLifBurstMetricsStruct.nextLifFreqKhz;
   rpt->curLifFreqKhz  = sync_metrics[gas_id].gfwLifBurstMetricsStruct.curLifFreqKhz;
   rpt->aci400Detected = sync_metrics[gas_id].gfwLifBurstMetricsStruct.aci400Detected;
   rpt->accISqr        = sync_metrics[gas_id].gfwLifBurstMetricsStruct.rsbEstimation.accISqr;
   rpt->accQSqr        = sync_metrics[gas_id].gfwLifBurstMetricsStruct.rsbEstimation.accQSqr;
   rpt->accIQ          = sync_metrics[gas_id].gfwLifBurstMetricsStruct.rsbEstimation.accIQ;
   rpt->wbdcEstI_PRx    = sync_metrics[gas_id].gfwLifBurstMetricsStruct.wbeeWbdcEstPRx.wbdcEstI;
   rpt->wbdcEstQ_PRx    = sync_metrics[gas_id].gfwLifBurstMetricsStruct.wbeeWbdcEstPRx.wbdcEstQ;
   rpt->wbdcEstI_DRx    = sync_metrics[gas_id].gfwLifBurstMetricsStruct.wbeeWbdcEstDRx.wbdcEstI;
   rpt->wbdcEstQ_DRx    = sync_metrics[gas_id].gfwLifBurstMetricsStruct.wbeeWbdcEstDRx.wbdcEstQ;
   rpt->rsbImgAciMetric = sync_metrics[gas_id].gfwLifBurstMetricsStruct.rsbImgAciMetric;
#endif /*GERAN_L1_ENHANCED_RX*/

#ifdef FEATURE_GSM_RX_DIVERSITY
   /* Log Rx Diversity metrics only if Rx diversity NV is enabled*/
   gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);
#endif

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_get_sync_rx_metrics: Not calling rfm_compute_rx_power");
   rpt->pwr_dBm_x16 = gplt_scan_power_data.pwr_dBm_x16;
#else
#ifdef FEATURE_GSM_RX_DIVERSITY
   /*In DRX only mode use DRX gain values to calculate PRX power level in dBm*/
   if((gl1_rxd_ctl_flag.enableRxd) && (gl1_rxd_ctl_flag.enableDrx)
      && (!(gl1_rxd_ctl_flag.enablePrx)))
     {
   rpt->powerStatus   = gl1_msg_compute_rx_power(rpt->rssi,
                                                 &rpt->pwr_dBm_x16,
                                                 &rpt->rx_gain_range,
                                                    &tags->gain_divrx);
     }
   else
#endif
     {
   rpt->powerStatus   = gl1_msg_compute_rx_power(rpt->rssi,
                           &rpt->pwr_dBm_x16,
                           &rpt->rx_gain_range,
                                                 &tags->gain);
     }

#ifdef FEATURE_GSM_RX_DIVERSITY
     rpt->dc_offset_i_divrx   = (int16)sync_metrics[gas_id].rxdLogPacket.dcIDRx;
     rpt->dc_offset_q_divrx   = (int16)sync_metrics[gas_id].rxdLogPacket.dcQDRx;

     rpt->rssi_divrx = sync_metrics[gas_id].rxdLogPacket.rssiDRx;
     if(rpt->rssi_divrx != 0)
      {
       rpt->powerStatus_divrx = gl1_msg_compute_rx_power(rpt->rssi_divrx,
                                                         &rpt->pwr_dBm_x16_divrx,
                                                         &rpt->rx_gain_range_divrx,
                                                         &tags->gain_divrx);
      }
     else
      {
        rpt->pwr_dBm_x16_divrx   = 0;
        rpt->rx_gain_range_divrx = 0;
        rpt->powerStatus_divrx   = RF_NOISY;
      }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

#ifdef FEATURE_GSM_RX_DIVERSITY

   if((gl1_rxd_ctl_flag.enableRxd) && (gl1_rxd_ctl_flag.enableDrx)
      && (!(gl1_rxd_ctl_flag.enablePrx)))
   {
       gl1_hw_ftm_store_rx_power( 0,
                                  0,
                                  rpt->rssi_divrx,
                                  rpt->pwr_dBm_x16_divrx,
                                  gas_id
                                );
   }
   else
   {
      gl1_hw_ftm_store_rx_power(rpt->rssi,rpt->pwr_dBm_x16,
                                rpt->rssi_divrx,rpt->pwr_dBm_x16_divrx,gas_id);
   }

#else

  gl1_hw_ftm_store_rx_power(rpt->rssi,rpt->pwr_dBm_x16,
                            0,0,gas_id);

#endif /*FEATURE_GSM_RX_DIVERSITY*/

   rpt->dc_offset_i   = (int16)sync_metrics[gas_id].dcI;
   rpt->dc_offset_q   = (int16)sync_metrics[gas_id].dcQ;
   rpt->freq_offset   = (int16)sync_metrics[gas_id].freqOffset;
   rpt->timing_offset = (int16)sync_metrics[gas_id].timingOffset;
   rpt->snr           = sync_metrics[gas_id].snr;
#ifdef FEATURE_GSM_AMR
   rpt->c_over_i      = sync_metrics[gas_id].cOverI;
#endif

   /* Fill in ACI fields */
#ifdef FEATURE_GSM_MDSP_ACI_DETECTION_VOICE
   rpt->aci_pwr_low     = sync_metrics[gas_id].aci_pwr_n200kHz;
   rpt->aci_pwr_center  = sync_metrics[gas_id].aci_pwr_0Hz;
   rpt->aci_pwr_high    = sync_metrics[gas_id].aci_pwr_p200kHz;
   rpt->aci_filter_used = (gl1_defs_aci_filter_type)sync_metrics[gas_id].aciIndex;
#else
   rpt->aci_pwr_low     = 0;
   rpt->aci_pwr_center  = 0;
   rpt->aci_pwr_high    = 0;
   rpt->aci_filter_used = GL1_DEFS_ACI_FILTER_CENTER;
#endif
   rpt->reliabFactorQ16 = sync_metrics[gas_id].reliabFactorQ16;
   rpt->brstCancel = sync_metrics[gas_id].brstCancel;

   rpt->aqpsk_detected = sync_metrics[gas_id].vamosLogPacket.vamosMetWord0.vamosAqpskDetected;

   rpt->pre_mimo_trained_complete_snr  = sync_metrics[gas_id].pre_mimo_trained_complete_snr;
   rpt->post_mimo_trained_complete_snr = sync_metrics[gas_id].post_mimo_trained_complete_snr;
   rpt->misc_flags                     = sync_metrics[gas_id].misc_flags;

#ifdef GERAN_L1_HLLL_LNASTATE
   rpt->jdet_value = sync_metrics[gas_id].jdetReading;
   rpt->SawlessHLLLState = tags->sawless_lnastate;
#endif /*GERAN_L1_HLLL_LNASTATE*/

   /* Copy the 8 of the hard decisions metrics word16s
    * Maintain word16 alignment, do not use memcpy.
    */
   if( GL1_DEFS_LOOPBACK_TYPE_C == loopback_type[gas_id] )
   {
     rpt->hard_decisions[ 0 ] = sync_metrics[gas_id].hard_decisions[ 0 ];
     rpt->hard_decisions[ 1 ] = sync_metrics[gas_id].hard_decisions[ 1 ];
     rpt->hard_decisions[ 2 ] = sync_metrics[gas_id].hard_decisions[ 2 ];
     rpt->hard_decisions[ 3 ] = sync_metrics[gas_id].hard_decisions[ 3 ];
     rpt->hard_decisions[ 4 ] = sync_metrics[gas_id].hard_decisions[ 4 ];
     rpt->hard_decisions[ 5 ] = sync_metrics[gas_id].hard_decisions[ 5 ];
     rpt->hard_decisions[ 6 ] = sync_metrics[gas_id].hard_decisions[ 6 ];
     rpt->hard_decisions[ 7 ] = sync_metrics[gas_id].hard_decisions[ 7 ];
   }

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
   mdsp_gprs_get_dtx_amr_pd_metrics(tags->metrics_tag, &gfw_amr_pd_dtx_detect_metrics, gas_id);

   rpt->esaic_metrics    = gfw_amr_pd_dtx_detect_metrics.dtxEsaicMetric;
   rpt->dtx_cxm_reg      = gfw_amr_pd_dtx_detect_metrics.bDtxCxmReg;
   rpt->amr_pd_cxm_reg   = gfw_amr_pd_dtx_detect_metrics.bAmrPdCxmReg;
   rpt->dtx_rx_cancel    = gfw_amr_pd_dtx_detect_metrics.bDtxRxCancel;
   rpt->amr_pd_rx_cancel = gfw_amr_pd_dtx_detect_metrics.bAmrPdRxCancel;
   rpt->amr_pd_ind       = gfw_amr_pd_dtx_detect_metrics.bAmrPdInd;
   rpt->amr_pd_desense   = gfw_amr_pd_dtx_detect_metrics.bAmrPdDesense;
#endif

     /* Log AEQ metrics*/
     gl1_hw_log_aeq_metrics (0, tags->metrics_tag,FALSE,gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
   /* Log Rx Diversity metrics only if Rx diversity NV is enabled*/
/*
  Run a continuous average on stream selection to conclude later whether we can turn RXD off only based on the RXQUAL level.
*/
 if(sync_metrics[gas_id].rxdLogPacket.rxdMisc.rxdSelStream == 2)
  {
    gl1_run_divrx_stream_sel_avg (TRUE,gas_id);
  }
 else
  {
    gl1_run_divrx_stream_sel_avg (FALSE,gas_id);
  }
  /* Log Rx Diversity metrics only if Rx diversity NV is enabled and RxD is enabled for the burst*/
 if( ( gl1_rxd_ctl_flag.enableRxd )
#ifdef FEATURE_GERAN_LOG_OPT
     && ( ( sync_metrics[gas_id].rxdLogPacket.rxdMisc.rxdOn != 0 ) || ( gl1_hw_disable_conditional_logging() != FALSE ) ) 
#endif
   )
  {
    if( (gl1_rxd_ctl_flag.enableDrx) && !(gl1_rxd_ctl_flag.enablePrx) )
    {
      gl1_hw_log_rxd_metrics( 0, rpt->pwr_dBm_x16_divrx, 0, rpt->rx_gain_range_divrx, rpt->powerStatus_divrx, &(sync_metrics[gas_id].rxdLogPacket),
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
        log_chan_type,
#else
        LOG_TCH,
#endif
        gas_id);
    }
    else
    {
      gl1_hw_log_rxd_metrics(rpt->pwr_dBm_x16, rpt->pwr_dBm_x16_divrx, rpt->rx_gain_range, rpt->rx_gain_range_divrx, rpt->powerStatus_divrx, &(sync_metrics[gas_id].rxdLogPacket),
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
        log_chan_type,
#else
        LOG_TCH,
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */
        gas_id);
    }
  }
#endif

#ifdef FEATURE_VAMOS_II
   /*log VAMOS2 metrics, log only if VAMOS is active when conditional logging enabled*/
#ifdef FEATURE_GERAN_LOG_OPT
  if ( ( sync_metrics[gas_id].vamosLogPacket.vamosMetWord0.paired ) || ( gl1_hw_disable_conditional_logging()!= FALSE ))
#endif	  
  {
   ShiftedSacchEn = (gl1_hw_get_vamos2_support(gas_id) && ((sync_metrics[gas_id].vamosLogPacket.vamosMetWord0.vamosTscIdx & 0x8) >> 3));
   gl1_hw_log_vamos2_metrics(gl1_hw_get_vamos_nv(gas_id),ShiftedSacchEn,&(sync_metrics[gas_id].vamosLogPacket),gas_id);
  }
#endif /*FEATURE_VAMOS_II*/

   /*Sync Metrics will always have only one Slot active*/
   /*Update Global Struct reg. ACI Status*/
   gl1_hw_set_aci_fr_arfcn(rpt->arfcn, sync_metrics[gas_id].gfwDftSpurAciMetrics.aciJammer, gas_id); 

   return (blocker);

}

/*=============================================================================
FUNCTION  gl1_hw_pwr_meas_cmd

DESCRIPTION
   Commands the DSP to take measurements at the specified frequencies.
   The results will be available in the subsequent frame

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
)
{
   uint32   i;
   uint32   num_freq;
   boolean  mdsp_scheduled;
   boolean  hmsc_enable = FALSE;

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
   idle_data_T               *l1_idle_data= &l1_idle_data_store[gas_id];
#endif

   static uint16                               pwr_meas_offsets[3][MAX_MEAS_PER_FRAME];
   static uint32                               pwr_meas_rf_seq_num[3][MAX_MEAS_PER_FRAME];
   static rfgsm_rx_chan_params_type            pwr_meas_rx_chan_params[3][MAX_MEAS_PER_FRAME];
   static rfgsm_rx_diversity_chan_params_type  pwr_meas_div_rx_chan_params[3][MAX_MEAS_PER_FRAME];
   static mdsp_gprs_monitor_coex_params        pwr_meas_coex_params[3][MAX_MEAS_PER_FRAME];

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))

   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_pwr_meas_cmd: Returning num of freq");

   return gplt_scan_power_data.num_pwr_meas;
#endif

   memset( &pwr_meas_offsets[gas_id][0],            0, sizeof(uint16)*MAX_MEAS_PER_FRAME );
   memset( &pwr_meas_rf_seq_num[gas_id][0],         0, sizeof(uint32)*MAX_MEAS_PER_FRAME );
   memset( &pwr_meas_coex_params[gas_id][0],        0, sizeof(mdsp_gprs_monitor_coex_params)*MAX_MEAS_PER_FRAME );
   memset( &pwr_meas_rx_chan_params[gas_id][0],     0, sizeof(rfgsm_rx_chan_params_type)*MAX_MEAS_PER_FRAME );
   memset( &pwr_meas_div_rx_chan_params[gas_id][0], 0, sizeof(rfgsm_rx_diversity_chan_params_type)*MAX_MEAS_PER_FRAME );


   /* Determine the schedule and inform the scheduler */
   num_freq = gl1_hw_schedule_pwr_meas( arfcns,
                                        max_num_freq,
                                        &pwr_meas_offsets[gas_id][0],
                                        &pwr_meas_coex_params[gas_id][0],
                                        &mdsp_scheduled,
                                        &hmsc_enable,
                                        gas_id
                                      );

   if (num_freq == 0)
   {
       MSG_ERROR(" no pwr monitors scheduled ",0,0,0 );
       return  0;
   }

#ifdef FEATURE_GSM_DED_SCELL_MONITOR
   else if ( num_freq == DUMMY_ARFCN_FOR_SC_MEAS)
   {
       return 1 ;
   }
#endif

   /* sanity check */
   if( num_freq > MAX_MEAS_PER_FRAME )
   {
     MSG_GERAN_ERROR_2_G("Invalid number of monitors %d, max allowed is %d", num_freq,MAX_MEAS_PER_FRAME);
     num_freq = MAX_MEAS_PER_FRAME;
   }

   /* sanity check */
   if( num_freq > max_num_freq )
   {
     MSG_GERAN_ERROR_2_G("no pwr monitors scheduled as num_freq %d > max_num_freq %d",num_freq, max_num_freq);
     return  0;
   }

   GL1_HANDLER_PERFORMANCE_LOGGING(START_PWR_MEAS_RF,gas_id);

   for (i = 0; i < num_freq; i++)
   {
       pwr_meas_rx_chan_params[gas_id][i].burst_type = RF_MON_BURST;
       pwr_meas_rx_chan_params[gas_id][i].band       = gl1_hw_arfcn_to_rfgsm_band(arfcns[i]);
       pwr_meas_rx_chan_params[gas_id][i].arfcn      = (uint16)arfcns[i].num;
       pwr_meas_rx_chan_params[gas_id][i].rx_lvl_dbm = (dBx16_T)agc_ptrs[i]->pwr_dBm_x16;
       pwr_meas_rx_chan_params[gas_id][i].lna_range  = NULL;  // not applicable for interference level measurements
       pwr_meas_rx_chan_params[gas_id][i].gain_ptr   = &tags[i].gain;
       pwr_meas_rx_chan_params[gas_id][i].freq_err   = gl1_hw_rf_get_freq_error(gas_id);

#ifdef GERAN_L1_HLLL_LNASTATE
       pwr_meas_rx_chan_params[gas_id][i].high_lin_mode_active = &(tags[i].sawless_lnastate);
#endif

#ifdef GERAN_L1_ENHANCED_RX
       pwr_meas_rx_chan_params[gas_id][i].enh_rx_params.enh_rx_enable_flag = FALSE;
       pwr_meas_rx_chan_params[gas_id][i].enh_rx_params.enh_rx_offset_freq = 0;
#endif

       pwr_meas_div_rx_chan_params[gas_id][i].rx_lvl_dbm = 0;
       pwr_meas_div_rx_chan_params[gas_id][i].lna_range = NULL;

#ifdef FEATURE_GSM_RX_DIVERSITY
       pwr_meas_div_rx_chan_params[gas_id][i].gain_ptr = &tags[i].gain;
#else
       pwr_meas_div_rx_chan_params[gas_id][i].gain_ptr = NULL;
#endif

#ifdef FEATURE_GSM_RF_DEBUG_NV
    if ( mdsp_rf_debug_NV_enable_g [gas_id] )
    {

      MSG_GERAN_HIGH_3_G("   agc_ptrs[i]->pwr_dBm_x16  = %d rx_chan_params[i].band  = %d  arfcns[i].num =%d",agc_ptrs[i]->pwr_dBm_x16, pwr_meas_rx_chan_params[gas_id][i].band ,arfcns[i].num);
      MSG_GERAN_HIGH_3_G(" gl1_hw_pwr_meas_cmd  mdsp_rf_debug_NV_enable_g[0] = %d mdsp_rf_debug_upper_power_limit_g = %d mdsp_rf_debug_lower_power_limit_g = %d",
                                 mdsp_rf_debug_NV_enable_g[gas_id],mdsp_rf_debug_upper_power_limit_g[gas_id] ,mdsp_rf_debug_lower_power_limit_g[gas_id]);
      MSG_GERAN_HIGH_3_G(" gl1_hw_pwr_meas_cmd  mdsp_rf_debug_band_g[0] = %d mdsp_rf_debug_arfcn_g = %d  i=%d",mdsp_rf_debug_band_g [gas_id],mdsp_rf_debug_arfcn_g[gas_id],i);
      if ( (mdsp_rf_debug_NV_enable_g [gas_id])&&
            (((dBx16_T)agc_ptrs[i]->pwr_dBm_x16/16)<= mdsp_rf_debug_upper_power_limit_g [gas_id])&&
            (((dBx16_T)agc_ptrs[i]->pwr_dBm_x16/16)>= mdsp_rf_debug_lower_power_limit_g [gas_id])&&
            ( pwr_meas_rx_chan_params[gas_id][i].band == mdsp_rf_debug_band_g [gas_id])&&
            (arfcns[i].num == mdsp_rf_debug_arfcn_g[gas_id] )
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


#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
     if (
          (
            ( arfcns[i].num == l1_idle_data->campedon_cell_ARFCN.num ) 
#ifdef FEATURE_GL1_ASDIV_ACQ
            || (!gl1_asdiv_acq_check_rescan(gas_id)      /*Not for Re-Scans as Antenna is already switched*/
                 &&  gl1_asdiv_acq_is_mon_active(gas_id))/*To Allow for AsDiv during BCCH Decodes*/
#endif
          )/*camped on cell and acq asdiv check*/
          &&
          (antenna_to_use[arfcn_index]==1) 
          && 
           antenna_swith_tx_div_nv_enable
        )
     {
       pwr_meas_rx_chan_params[gas_id][i].probe_burst     = RFGSM_RX_PROBE_BURST;
       pwr_meas_div_rx_chan_params[gas_id][i].probe_burst = RFGSM_RX_PROBE_BURST;
       MSG_GERAN_HIGH_0_G ("switch antenna for probing");
     }
     else
     {
       pwr_meas_rx_chan_params[gas_id][i].probe_burst     = RFGSM_RX_SYNC_BURST;
       pwr_meas_div_rx_chan_params[gas_id][i].probe_burst = RFGSM_RX_SYNC_BURST;
     }
     arfcn_index++;
#endif /*FEATURE_GSM_TX_DIV_ANTNA_SWITCHING  */

     pwr_meas_rx_chan_params[gas_id][i].rf_sequence_number = gl1_hw_get_next_rf_seq_num();
     pwr_meas_rf_seq_num[gas_id][i]                        = pwr_meas_rx_chan_params[gas_id][i].rf_sequence_number;

   }
   if (num_freq <= MAX_MEAS_PER_FRAME)
   {
     gl1_hw_rf_rx_burst((uint16)num_freq,
                        &pwr_meas_rx_chan_params[gas_id][0]
                        ,&pwr_meas_div_rx_chan_params[gas_id][0]
                        ,FALSE
                        ,MON_MODE
                       ,gas_id
                        );
    }
    else
    {
#ifdef FEATURE_QSH_MDUMP
     QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
     ERR_GERAN_FATAL_1_G( "rx_chan_params buffer overflow  num_freq %d ", num_freq);
    }


   if( 0 != num_freq )
   {
     /* Issue the mdsp command */
     if (mdsp_scheduled)
     {
       mdsp_gprs_power_measure_auto((uint16)num_freq, pwr_meas_offsets[gas_id][0], &pwr_meas_coex_params[gas_id][0], arfcns, &pwr_meas_rf_seq_num[gas_id][0], gas_id);
     }
     else
     {
       mdsp_gprs_power_measure((uint16)num_freq, &pwr_meas_offsets[gas_id][0], &pwr_meas_coex_params[gas_id][0], hmsc_enable, arfcns, &pwr_meas_rf_seq_num[gas_id][0], gas_id);
     }
   }

   return num_freq;
}


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
)
{
   static uint32   pwr_meas_monitors[3][MAX_MEAS_PER_FRAME];
   static boolean  pwr_meas_yield_status[3][MAX_MEAS_PER_FRAME];
   static boolean  pwr_meas_sched_error[3][MAX_CM_RSSI_MEASUREMENTS];

#ifdef GERAN_L1_HLLL_LNASTATE
   static uint16  pwr_meas_jdetvalues[3][MAX_MEAS_PER_FRAME];
#endif
#ifdef GL1_GMSK_MOD_DET
   uint8  failedEngyTest[3][MAX_MEAS_PER_FRAME];
   uint16 decstat [3][MAX_MEAS_PER_FRAME];
   uint16 decstat2 [3][MAX_MEAS_PER_FRAME];
#endif
   uint32  measured_len;
   uint32  i;

   boolean  results_avail;
   gl1_concman_config_params_type gl1_concman_config_params;
   cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);

   memset( &pwr_meas_monitors[gas_id][0],     0, sizeof(uint32)*MAX_MEAS_PER_FRAME );
   memset( &pwr_meas_yield_status[gas_id][0], 0, sizeof(boolean)*MAX_MEAS_PER_FRAME );
   memset( &pwr_meas_sched_error[gas_id][0], 0, sizeof(boolean)*MAX_MEAS_PER_FRAME );
#ifdef GERAN_L1_HLLL_LNASTATE
   memset( &pwr_meas_jdetvalues[gas_id][0],   0, sizeof(uint16)*MAX_MEAS_PER_FRAME );
#endif
#ifdef GL1_GMSK_MOD_DET
   memset( &failedEngyTest[gas_id][0],     0, sizeof(uint8)*MAX_MEAS_PER_FRAME );
   memset( &decstat[gas_id][0], 0, sizeof(uint16)*MAX_MEAS_PER_FRAME );
   memset( &decstat2[gas_id][0], 0, sizeof(uint16)*MAX_MEAS_PER_FRAME );
#endif  
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_pwr_meas_results: Returning power result");

   measurements->dBm_x16 = gplt_scan_power_data.pwr_dBm_x16;
   *retrieved_data_len =  gplt_scan_power_data.num_pwr_meas;

   return TRUE;
#endif 

   /* Grab the measurements from the mdsp */
   results_avail = mdsp_gprs_get_power_measure_results
                    (
                      &measured_len,
                      &pwr_meas_monitors[gas_id][0],
                      &pwr_meas_yield_status[gas_id][0],
                      &pwr_meas_sched_error[gas_id][0],
                      last_attempt
#ifdef GERAN_L1_HLLL_LNASTATE
                      ,&pwr_meas_jdetvalues[gas_id][0]
#endif /*GERAN_L1_HLLL_LNASTATE*/
#ifdef GL1_GMSK_MOD_DET
                       ,&failedEngyTest[gas_id][0]
					   ,&decstat[gas_id][0]
					   ,&decstat2[gas_id][0]
#endif /*GERAN_L1_HLLL_LNASTATE*/
                      ,gas_id
                    );

   if (!results_avail)
   {
      if (!last_attempt)
      {
         return FALSE;
      }
      else
      {
         measured_len = 0;
      }
   }

   /* Validate the measurement size */
   if ( meas_data_len != measured_len )
   {
      MSG_GERAN_HIGH_2_G("Power meas mismatch #exp=%d,#got=%d",meas_data_len,measured_len);

      /* Compute power using at most the expected results */
      if (measured_len > meas_data_len)
      {
         measured_len = meas_data_len;
      }
   }

   /* Calculate rf power for the actually retrieved data */
   for (i = 0; i < measured_len; i++)
   {
      measurements[i].status = gl1_msg_compute_rx_power(pwr_meas_monitors[gas_id][i],
                                                        &measurements[i].dBm_x16,
                                                        &measurements[i].rx_gain_range,
                                                        &tags[i].gain);
      measurements[i].rssi = pwr_meas_monitors[gas_id][i];
#ifdef GERAN_L1_HLLL_LNASTATE
      measurements[i].jdet_value       = pwr_meas_jdetvalues[gas_id][i];
      measurements[i].SawlessHLLLState = tags[i].sawless_lnastate;
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GL1_GMSK_MOD_DET
      measurements[i].energytest_flag = failedEngyTest[gas_id][i];
	  measurements[i].decstat = decstat[gas_id][i];
	  measurements[i].decstat2 = decstat2[gas_id][i];
	  if(gl1_hw_is_gdet_debug_enabled())
	  {
	   MSG_GERAN_HIGH_4_G("Power meas energy rssi %d flag %d, decstat %d, decstat2 %d",pwr_meas_monitors[gas_id][i],measurements[i].energytest_flag,measurements[i].decstat,measurements[i].decstat2);
	   MSG_GERAN_HIGH_3_G("gain range =%d,gain offset %d pwr_dbm %d",tags[i].gain.range,tags[i].gain.valx16,measurements[i].dBm_x16);
	  }
	 
#endif

      measurements[i].yield = pwr_meas_yield_status[gas_id][i];
      measurements[i].ScheduleError = pwr_meas_sched_error[gas_id][i];
   }

   /* Clean up any left over data not grabbed from dsp to sync to log fifo */
   for (i = measured_len; i < meas_data_len; i++)
   {
      measurements[i].status = RF_NOISY;
      measurements[i].yield = TRUE;
   }

   *retrieved_data_len = measured_len;

   /* inform MCPM that power meas scenario is now over (FR54762)*/
    gl1_concman_config_params.type = CMGR_STEADY_END;
    gl1_concman_config_params.scenario = CMGR_GERAN_PSCAN;
    gl1_concman_config_params.update_scenario = 0;
    gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_ACQUISITION);
    gl1_concman_config_params.start_timestamp = 0;
    gl1_concman_config_params.duration_msec = 0;
    gl1_concman_config_params.flow_ctrl_time_msec = 0;
    gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);


   return results_avail;

}

/*=============================================================================
FUNCTION gl1_hw_pwr_meas_results_available

DESCRIPTION
   Checks if FW has updated the pwr results.

============================================================================= */
boolean gl1_hw_pwr_meas_results_available(gas_id_t gas_id)
{
  return ( mdsp_gprs_is_power_measure_results_available( gas_id ) );
}

/*=============================================================================
FUNCTION gl1_hw_pwr_meas_abort

DESCRIPTION
   Aborts ongoing power and interference measures and cleans-up.

============================================================================= */
void gl1_hw_pwr_meas_abort (gas_id_t gas_id)
{
   /* Abort and clean up mDSP driver */
   mdsp_gprs_abort_power_measure(gas_id);

}

/*===========================================================================

FUNCTION  gl1_hw_rx_async_burst

DESCRIPTION
  This function initiates an asynchronous burst receive process for the given
  channel type.

DEPENDENCIES
  This function can be executed at any time but will obviously result
  in failed received bursts if issued at the wrong time.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_rx_async_burst
(
   const gl1_hw_rx_signal_type   *signal_info,
   const gl1_hw_rx_burst_type    *burst_info,
   uint16                        timing_offset,
   int32                         frequency_offset,
   gl1_hw_rx_tags_type           *tags,
   mdsp_burst_mode               mdsp_burst_type,
   gas_id_t                      gas_id
)
{
   mdsp_async_params_type   params;
   rfgsm_rx_chan_params_type rx_chan_params[1];
   uint16                    num_bursts = 1;
   uint32                    frequency_offset_xo = 0;
   rfgsm_rx_diversity_chan_params_type  div_rx_chan_params[1];
   uint32                    rf_seq_num;

   int32 phase_increment;
   /* Calculate the phase increment as a combination of band-compensated
   current rotator value and given frequency offset */
   memset (rx_chan_params,0,( sizeof (rfgsm_rx_chan_params_type)*1));
   phase_increment = gl1_hw_calc_phase_increment(burst_info->arfcn,gas_id);

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_rx_async_burst: Immediately return");
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

     /* package up Rx chan params - only 1 set of params required per frame for Rx Normal Burst */
     rx_chan_params[0].burst_type = RF_PWR_BURST;
     rx_chan_params[0].band       = gl1_hw_arfcn_to_rfgsm_band(burst_info->arfcn);
     rx_chan_params[0].arfcn      = (uint16)burst_info->arfcn.num;
     rx_chan_params[0].rx_lvl_dbm = (dBx16_T)signal_info->rx_signal_strength;
     rx_chan_params[0].lna_range  = NULL;
     /* gain will need to become a ptr to FW interface memory */
     rx_chan_params[0].gain_ptr   = &tags->gain;
     rx_chan_params[0].freq_err   = gl1_hw_rf_get_freq_error(gas_id);
#ifdef GERAN_L1_HLLL_LNASTATE
     rx_chan_params[0].high_lin_mode_active = &(tags->sawless_lnastate);
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GERAN_L1_ENHANCED_RX
     rx_chan_params[0].enh_rx_params.enh_rx_enable_flag = FALSE;
     rx_chan_params[0].enh_rx_params.enh_rx_offset_freq = 0;
#endif /*GERAN_L1_ENHANCED_RX*/

     div_rx_chan_params[0].rx_lvl_dbm = 0;
     div_rx_chan_params[0].lna_range = NULL;
     div_rx_chan_params[0].probe_burst = NULL;
#ifdef FEATURE_GSM_RX_DIVERSITY
     div_rx_chan_params[0].gain_ptr = &tags->gain;
#else
     div_rx_chan_params[0].gain_ptr = NULL;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

     params.DesenseArfcn = gl1_hw_is_arfcn_xo_desense( burst_info->arfcn, gas_id );

     /* if ARFCN is not a desense channel then correct for NCell freq error via DPLL */
     if ( params.DesenseArfcn != DESENSE_CHAN )
     {
       frequency_offset_xo         = gl1_hw_freqtonormxofreq( burst_info->arfcn, 0,gas_id );
       rx_chan_params[0].freq_err += frequency_offset_xo;	   
	   phase_increment  +=  frequency_offset ;
     }
     else
     {
       /* restore local freq error and correct for NCell freq error via rotator */
       gl1_hw_rf_update_freq_error( gl1_hw_get_xo_acc_freq_err(gas_id),gas_id );
          phase_increment += frequency_offset;
     }

     rx_chan_params[0].rf_sequence_number = gl1_hw_get_next_rf_seq_num();
     rf_seq_num = rx_chan_params[0].rf_sequence_number;

     gl1_hw_rf_rx_burst(num_bursts,
                        rx_chan_params
                        ,div_rx_chan_params
                        ,FALSE
                        ,mdsp_burst_type
                       ,gas_id
                        );

   MSG_GERAN_HIGH_3_G( "Async Rx phase inc:%d, frequency_offset:%d, frequency_offset_xo:%d",
                      phase_increment, frequency_offset, frequency_offset_xo );

   /* Load up the data we need for SAIC. */
   params.rf_gain.range  = tags->gain.range;
   params.rf_gain.valx16 = tags->gain.valx16;
   params.saic   = gl1_hw_async_rx_saic[gas_id];
   params.idle   = gl1_hw_get_idle_mode(gas_id);
   params.dedicated = gl1_hw_get_dedicated(gas_id);
   params.rx_lev_dBm  = signal_info->rx_signal_strength;

   if (mdsp_channel_type_lut[burst_info->channel_type] != MDSP_CHANNEL_TYPE_LUT_ERROR)
   {
     /* Perform arbitration of activities prior to sending mDSP commands */
#ifdef FEATURE_GSM_COEX
     gl1_hw_arbitration[gas_id].async_rx.arfcn = burst_info->arfcn;
     gl1_hw_arbitration[gas_id].async_rx.timing_offset = timing_offset;

     gl1_hw_arbitration[gas_id].notify_event(ARBITRATE_ASYNCRX, gas_id);
#endif

#ifdef FEATURE_GSM_LOG_COEX
     /* Log async rx burst in cxm_log_table */
     l1_log_cxm_new_entry(mdsp_get_current_seq_num(gas_id), burst_info->arfcn.num,
                          burst_info->channel_type, timing_offset + (QS_PER_SLOT / 2),
                          CXM_LOG_UNKNOWN_VALUE, 0, 0, 0, CXM_LOG_ACTIVITY_RX, gas_id);
#endif
   /* Issue the command to the dsp */
   mdsp_async_rx
    (
      &params,
      mdsp_channel_type_lut[burst_info->channel_type],
      burst_info->tsc,
      burst_info->decode_flag,
      burst_info->early_decoding,
      burst_info->sbd_enabled,
      burst_info->epd_allowed,
      burst_info->burst_num,
      timing_offset,
      phase_increment,
      mdsp_burst_type,
      rf_seq_num,
      burst_info->arfcn.num,
#ifdef FEATURE_GSM_LOG_COEX
      gl1_hw_arbitration[gas_id].async_rx.priority,
      gl1_hw_arbitration[gas_id].async_rx.desense_id,
#endif
      gas_id
    );
    }
#if defined ( FEATURE_QTA ) && defined ( FEATURE_CXM_QTA )
   gl1_hw_set_rx_cmd_issued_in_qta(TRUE, gas_id);
#endif

   gl1_hw_schedule_async_rx(burst_info->channel_type, timing_offset,gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_x2g_rx_async_sch_burst_on_rotator

DESCRIPTION
  This function initiates an asynchronous burst receive process for a SCH
  burst on the rotator.

DEPENDENCIES
  This function can be executed at any time but will obviously result
  in failed received bursts if issued at the wrong time.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_x2g_rx_async_sch_burst_on_rotator
(
   ARFCN_T               ARFCN,
   int16                 signal_strength,
   uint16                timing_offset,
   int16                 frequency_offset,
   gl1_hw_rx_tags_type*  tags,
   uint8*                sch_metrics_buffer_idx_ptr,
   uint16*               sch_metrics_seq_num_ptr,
   uint8*                sch_data_buffer_idx_ptr,
   sys_modem_as_id_e_type as_id
)
{
   int32 phase_increment;
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
   rfgsm_rx_chan_params_type rx_chan_params[1];
   uint16                    num_bursts = 1;
   uint32                    frequency_offset_xo = 0;
   uint32                    rf_seq_num;
   rfgsm_rx_diversity_chan_params_type  div_rx_chan_params[1];

   memset (rx_chan_params,0,( sizeof (rfgsm_rx_chan_params_type)* 1));
   phase_increment = gl1_hw_hztophase((int32)frequency_offset);

   if ( gl1_msg_is_arfcn_xo_desense( ARFCN,gas_id ) == DESENSE_CHAN )
   {
     phase_increment += gl1_hw_convert_normcurrentxofreq_to_rot( ARFCN,gas_id );
   }

   /* Calculate the phase increment as a combination of band-compensated
    current rotator value and given frequency offset */
   (void)gl1_hw_calc_phase_increment(ARFCN,gas_id);

     /* package up Rx chan params - only 1 set of params required per frame for Rx Normal Burst */
     rx_chan_params[0].burst_type = RF_PWR_BURST;
     rx_chan_params[0].band       = gl1_hw_arfcn_to_rfgsm_band(ARFCN);
     rx_chan_params[0].arfcn      = (uint16)ARFCN.num;
     rx_chan_params[0].rx_lvl_dbm = (dBx16_T)signal_strength;
     rx_chan_params[0].lna_range  = NULL;
     /* gain will need to become a ptr to FW interface memory */
     rx_chan_params[0].gain_ptr   = &tags->gain;
     rx_chan_params[0].freq_err   = gl1_hw_rf_get_freq_error(gas_id);
#ifdef GERAN_L1_HLLL_LNASTATE
     rx_chan_params[0].high_lin_mode_active = &(tags->sawless_lnastate);
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GERAN_L1_ENHANCED_RX
     rx_chan_params[0].enh_rx_params.enh_rx_enable_flag = FALSE;
     rx_chan_params[0].enh_rx_params.enh_rx_offset_freq = 0;
#endif /*GERAN_L1_ENHANCED_RX*/

     div_rx_chan_params[0].rx_lvl_dbm = 0;
     div_rx_chan_params[0].lna_range = NULL;
     div_rx_chan_params[0].probe_burst =NULL;
#ifdef FEATURE_GSM_RX_DIVERSITY
     div_rx_chan_params[0].gain_ptr = &tags->gain;
#else
     div_rx_chan_params[0].gain_ptr =   NULL;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

     rx_chan_params[0].rf_sequence_number = gl1_hw_get_next_rf_seq_num();
     rf_seq_num = rx_chan_params[0].rf_sequence_number;

     gl1_hw_rf_rx_burst(num_bursts,
                        rx_chan_params
                        ,div_rx_chan_params
                        ,FALSE
                        ,ACQ_MODE
                        ,gas_id
                        );


   MSG_LOW( "Async SCH phase inc:%d frequency_offset:%d frequency_offset_xo:%d",
            phase_increment, frequency_offset, frequency_offset_xo );

#ifdef FEATURE_GSM_LOG_COEX
   /* Log SCH burst in cxm_log_table */
   l1_log_cxm_new_entry(mdsp_get_current_seq_num(gas_id), ARFCN.num,
                        GL1_HW_SCH, timing_offset + (QS_PER_SLOT / 2),
                        CXM_LOG_UNKNOWN_VALUE, 0, 0, 0, CXM_LOG_ACTIVITY_RX, gas_id);
#endif

   /* Issue the command to the dsp */
   mdsp_x2g_async_rx_sch
    (
      timing_offset,
      phase_increment,
      sch_metrics_buffer_idx_ptr,
      sch_metrics_seq_num_ptr,
      sch_data_buffer_idx_ptr,
      ARFCN,
      rf_seq_num,
      gas_id
    );

   gl1_hw_schedule_async_rx(GL1_HW_SCH, timing_offset,gas_id);

   gl1_hw_log_cell_cmds_to_diag (cmd_rx_async_sch_burst, FALSE,gas_id);
}

/*===========================================================================

FUNCTION  gl1_hw_rx_async_sch_burst

DESCRIPTION
  This function initiates an asynchronous burst receive process for a SCH
  burst.

DEPENDENCIES
  This function can be executed at any time but will obviously result
  in failed received bursts if issued at the wrong time.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_hw_x2g_rx_async_sch_burst
(
   ARFCN_T               ARFCN,
   int16                 signal_strength,
   uint16                timing_offset,
   int16                 frequency_offset,
   gl1_hw_rx_tags_type*  tags,
   uint8*                sch_metrics_buffer_idx_ptr,
   uint16*               sch_metrics_seq_num_ptr,
   uint8*                sch_data_buffer_idx_ptr,
   sys_modem_as_id_e_type as_id

)
{
   int32 phase_increment;
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
   rfgsm_rx_chan_params_type rx_chan_params[1];
   uint16                    num_bursts = 1;
   uint32                    frequency_offset_xo = 0;
   uint32                    rf_seq_num;
   rfgsm_rx_diversity_chan_params_type  div_rx_chan_params[1];
   /* Calculate the phase increment as a combination of band-compensated
    current rotator value and given frequency offset */
   phase_increment = gl1_hw_calc_phase_increment(ARFCN,gas_id);
   memset (rx_chan_params,0,( sizeof (rfgsm_rx_chan_params_type)* 1));
     /* package up Rx chan params - only 1 set of params required per frame for Rx Normal Burst */
     rx_chan_params[0].burst_type = RF_PWR_BURST;
     rx_chan_params[0].band       = gl1_hw_arfcn_to_rfgsm_band(ARFCN);
     rx_chan_params[0].arfcn      = (uint16)ARFCN.num;
     rx_chan_params[0].rx_lvl_dbm = (dBx16_T)signal_strength;
     rx_chan_params[0].lna_range  = NULL;
     /* gain will need to become a ptr to FW interface memory */
     rx_chan_params[0].gain_ptr   = &tags->gain;
     rx_chan_params[0].freq_err   = gl1_hw_rf_get_freq_error(gas_id);
#ifdef GERAN_L1_HLLL_LNASTATE
     rx_chan_params[0].high_lin_mode_active = &(tags->sawless_lnastate);
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GERAN_L1_ENHANCED_RX
     rx_chan_params[0].enh_rx_params.enh_rx_enable_flag = FALSE;
     rx_chan_params[0].enh_rx_params.enh_rx_offset_freq = 0;
#endif /*GERAN_L1_ENHANCED_RX*/

     div_rx_chan_params[0].rx_lvl_dbm = 0;
     div_rx_chan_params[0].lna_range = NULL;
     div_rx_chan_params[0].probe_burst =NULL;
#ifdef FEATURE_GSM_RX_DIVERSITY
     div_rx_chan_params[0].gain_ptr = &tags->gain;
#else
     div_rx_chan_params[0].gain_ptr = NULL;
#endif /*FEATURE_GSM_RX_DIVERSITY*/
     /* if ARFCN is not a desense channel then correct for NCell freq error via DPLL */
     if ( gl1_hw_is_arfcn_xo_desense( ARFCN,gas_id ) != DESENSE_CHAN )
     {
       frequency_offset_xo         = gl1_hw_freqtonormxofreq( ARFCN, frequency_offset,gas_id );
       rx_chan_params[0].freq_err += frequency_offset_xo;
     }
     else
     {
       /* restore local freq error and correct for NCell freq error via rotator */
       gl1_hw_rf_update_freq_error( gl1_hw_get_xo_acc_freq_err(gas_id),gas_id );
           phase_increment += gl1_hw_hztophase((int32)frequency_offset);
     }

     rx_chan_params[0].rf_sequence_number = gl1_hw_get_next_rf_seq_num();
     rf_seq_num = rx_chan_params[0].rf_sequence_number;

     gl1_hw_rf_rx_burst(num_bursts,
                        rx_chan_params
                        ,div_rx_chan_params
                        ,FALSE
                        ,ACQ_MODE
                        ,gas_id
                        );

   MSG_LOW( "Async SCH phase inc:%d frequency_offset:%d frequency_offset_xo:%d",
            phase_increment, frequency_offset, frequency_offset_xo );
#ifdef FEATURE_GSM_LOG_COEX
   /* Log SCH burst in cxm_log_table */
   l1_log_cxm_new_entry(mdsp_get_current_seq_num(gas_id), ARFCN.num,
                        GL1_HW_SCH, timing_offset + (QS_PER_SLOT / 2),
                        CXM_LOG_UNKNOWN_VALUE, 0, 0, 0, CXM_LOG_ACTIVITY_RX, gas_id);
#endif
   /* Issue the command to the dsp */
   mdsp_x2g_async_rx_sch
    (
      timing_offset,
      phase_increment,
      sch_metrics_buffer_idx_ptr,
      sch_metrics_seq_num_ptr,
      sch_data_buffer_idx_ptr,
      ARFCN,
      rf_seq_num,
      gas_id
    );

   gl1_hw_schedule_async_rx(GL1_HW_SCH, timing_offset,gas_id);

   gl1_hw_log_cell_cmds_to_diag (cmd_rx_async_sch_burst, FALSE,gas_id);
}


void gl1_hw_rx_async_sch_burst
(
   ARFCN_T    ARFCN,
   int16      signal_strength,
   uint16     timing_offset,
   int16      frequency_offset,
   gl1_hw_rx_tags_type *tags
#ifdef FEATURE_DUAL_SIM
   ,sys_modem_as_id_e_type as_id
#endif /*FEATURE_DUAL_SIM*/
)
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id;
   gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;

   as_id = geran_map_gas_id_to_nas_id(gas_id);
#else
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#endif /* FEATURE_DUAL_SIM*/

   gl1_hw_rx_async_sch_burst_geran(ARFCN, signal_strength, timing_offset,
                                   frequency_offset, tags, as_id);

   gl1_hw_cm_set_sch_metrics_tags(gas_id, ARFCN, mdsp_current_buffer_mod3(gas_id), mdsp_get_current_seq_num(gas_id));

   return;
}
void gl1_hw_rx_async_sch_burst_geran
(
   ARFCN_T    ARFCN,
   int16      signal_strength,
   uint16     timing_offset,
   int16      frequency_offset,
   gl1_hw_rx_tags_type *tags,
   sys_modem_as_id_e_type as_id
)
{

   int32 phase_increment;
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

   rfgsm_rx_chan_params_type rx_chan_params[1];
   uint16                    num_bursts = 1;
   uint32                    frequency_offset_xo = 0;
   uint32                    rf_seq_num;
   rfgsm_rx_diversity_chan_params_type  div_rx_chan_params[1];
   memset (rx_chan_params,0,( sizeof (rfgsm_rx_chan_params_type)* 1));
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_rx_async_sch_burst: Return immediately");
   return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   /* Calculate the phase increment as a combination of band-compensated
    current rotator value and given frequency offset */
   phase_increment = gl1_hw_calc_phase_increment(ARFCN,gas_id);

     /* package up Rx chan params - only 1 set of params required per frame for Rx Normal Burst */
     rx_chan_params[0].burst_type = RF_PWR_BURST;
     rx_chan_params[0].band       = gl1_hw_arfcn_to_rfgsm_band(ARFCN);
     rx_chan_params[0].arfcn      = (uint16)ARFCN.num;
     rx_chan_params[0].rx_lvl_dbm = (dBx16_T)signal_strength;
     rx_chan_params[0].lna_range  = NULL;
     /* gain will need to become a ptr to FW interface memory */
     rx_chan_params[0].gain_ptr   = &tags->gain;
     rx_chan_params[0].freq_err   = gl1_hw_rf_get_freq_error(gas_id);
#ifdef GERAN_L1_HLLL_LNASTATE
     rx_chan_params[0].high_lin_mode_active = &(tags->sawless_lnastate);
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GERAN_L1_ENHANCED_RX
     rx_chan_params[0].enh_rx_params.enh_rx_enable_flag = FALSE;
     rx_chan_params[0].enh_rx_params.enh_rx_offset_freq = 0;
#endif /*GERAN_L1_ENHANCED_RX*/

     div_rx_chan_params[0].rx_lvl_dbm = 0;
     div_rx_chan_params[0].lna_range = NULL;
     div_rx_chan_params[0].probe_burst =NULL;
#ifdef FEATURE_GSM_RX_DIVERSITY
     div_rx_chan_params[0].gain_ptr = &tags->gain;
#else
     div_rx_chan_params[0].gain_ptr = NULL;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

     /* if ARFCN is not a desense channel then correct for NCell freq error via DPLL */
     if ( gl1_hw_is_arfcn_xo_desense( ARFCN ,gas_id) != DESENSE_CHAN )
     {
       frequency_offset_xo         = gl1_hw_freqtonormxofreq( ARFCN, frequency_offset,gas_id );
       rx_chan_params[0].freq_err += frequency_offset_xo;
     }
     else
     {
       /* restore local freq error and correct for NCell freq error via rotator */
       gl1_hw_rf_update_freq_error( gl1_hw_get_xo_acc_freq_err(gas_id),gas_id );
           phase_increment += gl1_hw_hztophase((int32)frequency_offset);
     }

     if((gl1_hw_sleep_get_in_warmup_check(gas_id)==TRUE)
         && (gl1_hw_is_forced_wakeup(gas_id)==FALSE)
#ifdef FEATURE_QTA
         && (gl1_hw_qta_gap_active(gas_id)==FALSE)
#endif
       )
     {
       gl1_hw_sleep_set_commit_mask(RX_BURST_COMMIT, gas_id);
     }

     rx_chan_params[0].rf_sequence_number = gl1_hw_get_next_rf_seq_num();
     rf_seq_num = rx_chan_params[0].rf_sequence_number;

     gl1_hw_rf_rx_burst(num_bursts,
                        rx_chan_params
                        ,div_rx_chan_params
                        ,FALSE
                        ,ACQ_MODE
                       ,gas_id
                        );


   MSG_GERAN_LOW_2_G( "Async SCH phase inc:%d ARFCN:%d",
            phase_increment, ARFCN.num);

#ifdef FEATURE_GSM_COEX
   /* Perform arbitration of activities prior to sending mDSP commands */
   gl1_hw_arbitration[gas_id].sch.arfcn = ARFCN;
   gl1_hw_arbitration[gas_id].sch.timing_offset = timing_offset;

   gl1_hw_arbitration[gas_id].notify_event(ARBITRATE_SCH, gas_id);

#ifdef FEATURE_GSM_LOG_COEX
   /* Log SCH burst in cxm_log_table */
   l1_log_cxm_new_entry(mdsp_get_current_seq_num(gas_id), ARFCN.num,
                        GL1_HW_SCH, timing_offset + (QS_PER_SLOT / 2),
                        CXM_LOG_UNKNOWN_VALUE, 0, 0, 0, CXM_LOG_ACTIVITY_RX, gas_id);
#endif
#endif

   /* Issue the command to the dsp */
   mdsp_async_rx_sch
    (
      timing_offset,
      phase_increment,
      rf_seq_num,
#ifdef FEATURE_GSM_COEX
      gl1_hw_arbitration[gas_id].sch.priority,
      gl1_hw_arbitration[gas_id].sch.desense_id,
#endif
      ARFCN,
#ifdef FEATURE_GSM_RX_DIVERSITY
      FALSE,
#endif
      gas_id
    );

#if defined ( FEATURE_QTA ) && defined ( FEATURE_CXM_QTA )
   /*Call function twice to trigger cleanup as this rx cmd for SCH is only issued once.*/
   gl1_hw_set_rx_cmd_issued_in_qta(TRUE, gas_id);
   gl1_hw_set_rx_cmd_issued_in_qta(TRUE, gas_id);
#endif

   gl1_hw_schedule_async_rx(GL1_HW_SCH, timing_offset,gas_id);

   gl1_hw_log_cell_cmds_to_diag (cmd_rx_async_sch_burst, FALSE,gas_id);
}


void gl1_hw_rx_async_sch_burst2
(
   ARFCN_T    ARFCN,
   int16      signal_strength,
#ifdef FEATURE_GSM_RX_DIVERSITY
   int16      signal_strength_divrx,
#endif
   uint16     timing_offset,
   int32      afc_freq,
   gl1_hw_rx_tags_type *tags,
   gas_id_t   gas_id
)
{
   rfgsm_rx_chan_params_type rx_chan_params[1];
   uint16                    num_bursts = 1;
   uint32                    rf_seq_num;
   rfgsm_rx_diversity_chan_params_type  div_rx_chan_params[1];
   boolean enable_rxdiversity = FALSE;
#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_rxd_control_type gl1_rxd_ctl_flag;
#endif
   memset (rx_chan_params,0,( sizeof (rfgsm_rx_chan_params_type)* 1));
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_hw_rx_async_sch_burst2: Return immediately");
   return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_rxd_ctl_flag  = gl1_get_rxd_control_flags(gas_id);

   if((gl1_rxd_ctl_flag.enableRxd) &&(gl1_rxd_ctl_flag.enableDrx)
      && (GL1_GFW_PRX_DIVRX_ENTER == gl1_hw_get_gfw_enter_status(gas_id))
      &&(gl1_hw_gsm_band_supported(ARFCN.band,gas_id)))
   {
     enable_rxdiversity = TRUE;
   }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   /* This will reset the XO for RF to 0 for spur but ignore any
      returned rotator correction as already included in passed in afc_freq */
   afc_freq += gl1_hw_calc_phase_increment(ARFCN,gas_id);

     /* package up Rx chan params - only 1 set of params required per frame for Rx Normal Burst */
     rx_chan_params[0].burst_type = RF_PWR_BURST;
     rx_chan_params[0].band       = gl1_hw_arfcn_to_rfgsm_band(ARFCN);
     rx_chan_params[0].arfcn      = (uint16)ARFCN.num;
     rx_chan_params[0].rx_lvl_dbm = (dBx16_T)signal_strength;
     rx_chan_params[0].lna_range  = NULL;
     /* gain will need to become a ptr to FW interface memory */
     rx_chan_params[0].gain_ptr   = &tags->gain;
     rx_chan_params[0].freq_err   = gl1_hw_rf_get_freq_error(gas_id);

#ifdef GERAN_L1_HLLL_LNASTATE
     rx_chan_params[0].high_lin_mode_active = &(tags->sawless_lnastate);
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GERAN_L1_ENHANCED_RX
     rx_chan_params[0].enh_rx_params.enh_rx_enable_flag = FALSE;
     rx_chan_params[0].enh_rx_params.enh_rx_offset_freq = 0;
#endif /*GERAN_L1_ENHANCED_RX*/
     div_rx_chan_params[0].probe_burst =NULL;
#ifdef FEATURE_GSM_RX_DIVERSITY
     div_rx_chan_params[0].rx_lvl_dbm = (dBx16_T)signal_strength_divrx;
     div_rx_chan_params[0].gain_ptr = &tags->gain_divrx;
#else
     div_rx_chan_params[0].rx_lvl_dbm = 0;
     div_rx_chan_params[0].gain_ptr = NULL;
#endif /*FEATURE_GSM_RX_DIVERSITY*/
     div_rx_chan_params[0].lna_range = NULL;

#ifdef FEATURE_GSM_RX_DIVERSITY
    MSG_GERAN_HIGH_3_G("signal_strength:%d,signal_strength_divrx:%d,enable_rxdiversity:%d",signal_strength,
                        signal_strength_divrx,enable_rxdiversity);
#endif
     /* if ARFCN is desense channel then restore local freq error  */
     if ( gl1_hw_is_arfcn_xo_desense( ARFCN, gas_id ) == DESENSE_CHAN )
     {
       gl1_hw_rf_update_freq_error( gl1_hw_get_xo_acc_freq_err(gas_id), gas_id );
     }

    if((gl1_hw_sleep_get_in_warmup_check(gas_id)==TRUE)
        && (gl1_hw_is_forced_wakeup(gas_id)==FALSE)
#ifdef FEATURE_QTA
        && (gl1_hw_qta_gap_active(gas_id)==FALSE)
#endif
      )
    {
       gl1_hw_sleep_set_commit_mask(RX_BURST_COMMIT, gas_id);
    }

     rx_chan_params[0].rf_sequence_number = gl1_hw_get_next_rf_seq_num();
     rf_seq_num = rx_chan_params[0].rf_sequence_number;

     gl1_hw_rf_rx_burst(num_bursts,
                        rx_chan_params
                        ,div_rx_chan_params
                        ,enable_rxdiversity
                        ,ACQ_MODE
                       ,gas_id
                       );
     MSG_GERAN_MED_2_G( "Async RF-ACQ=%d GS=unknown ARFCN=%d",
              ( signal_strength / 16 ), ARFCN.num);

   MSG_GERAN_LOW_1_G( "Async SCH2 phase inc:%d",
            afc_freq);

   /* Set the search width */
   mdsp_set_async_rx_search_width(GL1_HW_SCH_SEARCH_WIDTH_DEFAULT ,gas_id);

#ifdef FEATURE_GSM_COEX
   /* Perform arbitration of activities prior to sending mDSP commands */
   gl1_hw_arbitration[gas_id].sch.arfcn = ARFCN;
   gl1_hw_arbitration[gas_id].sch.timing_offset = timing_offset;

   gl1_hw_arbitration[gas_id].notify_event(ARBITRATE_SCH, gas_id);

#ifdef FEATURE_GSM_LOG_COEX
   /* Log SCH burst in cxm_log_table */
   l1_log_cxm_new_entry(mdsp_get_current_seq_num(gas_id), ARFCN.num,
                        GL1_HW_SCH, timing_offset + (QS_PER_SLOT / 2),
                        CXM_LOG_UNKNOWN_VALUE, 0, 0, 0, CXM_LOG_ACTIVITY_RX, gas_id);
#endif
#endif

   mdsp_async_rx_sch
     (
       timing_offset,
       afc_freq,
       rf_seq_num,
#ifdef FEATURE_GSM_COEX
       gl1_hw_arbitration[gas_id].sch.priority,
       gl1_hw_arbitration[gas_id].sch.desense_id,
#endif
       ARFCN,

#ifdef FEATURE_GSM_RX_DIVERSITY
       enable_rxdiversity,
#endif
       gas_id
     );

#if defined ( FEATURE_QTA ) && defined ( FEATURE_CXM_QTA )
   /*Call function twice to trigger cleanup as this rx cmd for SCH is only issued once.*/
   gl1_hw_set_rx_cmd_issued_in_qta(TRUE, gas_id);
   gl1_hw_set_rx_cmd_issued_in_qta(TRUE, gas_id);
#endif

   gl1_hw_schedule_async_rx(GL1_HW_SCH, timing_offset,gas_id);

   gl1_hw_log_cell_cmds_to_diag (cmd_rx_async_sch_burst, FALSE,gas_id);
}


/*===========================================================================

FUNCTION  gl1_hw_get_async_rx_data

DESCRIPTION
  This function attempts to retrieve data from the mdsp buffers for an
  async burst.

  This function may be called repeatedly to poll for results being
  available.  The parameter last_time is provided to support this
  fuctionality.  If this functions returns TRUE or last_time is TRUE
  then the next call to this function will expect that another block
  has been decoded (buffer pointers are updated).  Unless this
  function returns TRUE or last_time is TRUE this function will not
  move on to the next block. I.e. on the last attempt to get the
  data for a particular block last_time must be set to TRUE.

DEPENDENCIES
  An async block or burst must have been received -
  via 4 gl1_hw_rx_async_bcch_burst commands or a single SCH burst -
  prior to calling this function.  There is a delay between the bursts being
  received and the data being available.

RETURN VALUE
  TRUE   - a block has been dedoded (the CRC may have passed or failed)
  FALSE  - no block has been decoded

SIDE EFFECTS
  If last_time is TRUE or this functions returns TRUE then the data for
  this block cannot be retrieved again.

===========================================================================*/

boolean  gl1_hw_x2g_get_async_rx_sch_data
(
   gl1_defs_rx_hdr_struct*  rpt_hdr_ptr,
   uint8                    data[MAX_SCH_OCTETS],
   uint16                   data_len,
   boolean                  last_time,
   uint8                    sch_data_buffer_idx,
   uint16                   sch_data_seq_num,
   sys_modem_as_id_e_type   as_id
)
{
   boolean                 results_avail = FALSE;
   GfwSchDecodeDataStruct  sch_rx;
   sch_rx.scheduleTimeError = FALSE;
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);


   /* OK to cast data from uint8 ptr to uint16 ptr because the mdsp will
    * take care of the alignment, but it is slower */
   MDSP_PTR_CHECK(data);

   /* Retrieve the results */
   results_avail = mdsp_x2g_get_ncell_sch_decoded_data
                      (
                        &sch_rx,
                        (uint16 *)data,
                        (data_len/2),        /* NUM octets to NUM uint16 */
                        last_time,
                        sch_data_buffer_idx,
                        sch_data_seq_num,
                        gas_id
                      );

   if( results_avail )
   {
        rpt_hdr_ptr->good_data     = sch_rx.control[1];
        rpt_hdr_ptr->len           = sch_rx.len;
        rpt_hdr_ptr->chan          = (uint16)GL1_HW_SCH;

        rpt_hdr_ptr->rx_qual       = sch_rx.rx_qual_hard;  /* Hard RXQUAL */
        rpt_hdr_ptr->rx_qual_s     = sch_rx.rx_qual_soft;  /* Soft RXQUAL */
        rpt_hdr_ptr->schedule_error = sch_rx.scheduleTimeError;
        rpt_hdr_ptr->bfi       = FALSE;  /* bfi is calculated by ARM algorithm */

#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
        #error code not present
#endif

   }
   else
   {
        rpt_hdr_ptr->good_data     = FALSE;
        rpt_hdr_ptr->firecode_used = FALSE;
   }

#ifdef FEATURE_GPRS_GBTA
     if( (sch_rx.reason & SCH_SCHEDULED_BIT) == 0 )
     {
       rpt_hdr_ptr->scheduled_status = GPL1_SCH_SCHDULE_FAIL;
     }
     else
     {
       rpt_hdr_ptr->scheduled_status = GPL1_SCH_SCHDULE_SUCCESS;
     }
     MSG_GERAN_MED_2_G("GL1 SCH Decode status  %d, scheduled satus %d",sch_rx.reason, rpt_hdr_ptr->scheduled_status);
#endif /* FEATURE_GPRS_GBTA */

  return  results_avail;
}



boolean gl1_hw_get_async_rx_data
(
   gl1_hw_channel_type     chan,
   gl1_defs_rx_hdr_struct  *rpt_hdr,
   uint8                   *data,
   uint16                  data_len,
   boolean                 last_time
#ifdef FEATURE_DUAL_SIM
   ,sys_modem_as_id_e_type  as_id
#endif /*FEATURE_DUAL_SIM*/
)
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/
   return gl1_hw_get_async_rx_data_geran(chan,rpt_hdr,data,data_len,last_time,as_id);
}

boolean gl1_hw_get_async_rx_data_geran
(
   gl1_hw_channel_type     chan,
   gl1_defs_rx_hdr_struct  *rpt_hdr,
   uint8                   *data,
   uint16                  data_len,
   boolean                 last_time,
   sys_modem_as_id_e_type  as_id
)
{
   boolean results_avail;
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

   GfwDecodedDataStruct rx;
   GfwSchDecodeDataStruct sch_rx;
   sch_rx.scheduleTimeError = FALSE;

   /* OK to cast data from uint8 ptr to uint16 ptr because the mdsp will
    * take care of the alignment, but it is slower */
   MDSP_PTR_CHECK(data);

   if(chan == GL1_HW_SCH)
   {
     /* Retrieve the results */
     results_avail = mdsp_get_ncell_sch_decoded_data
                      (
                        &sch_rx,
                        (uint16 *)data,
                        data_len,
                        last_time
                        ,gas_id
                      );  /*lint !e826*/
     if (results_avail)
     {
        rpt_hdr->good_data     = sch_rx.control[1];
        rpt_hdr->len           = sch_rx.len;
        rpt_hdr->chan          = (uint16)chan;

        rpt_hdr->rx_qual       = sch_rx.rx_qual_hard;  /* Hard RXQUAL */
        rpt_hdr->rx_qual_s     = sch_rx.rx_qual_soft;  /* Soft RXQUAL */
        rpt_hdr->schedule_error = sch_rx.scheduleTimeError;
        rpt_hdr->bfi       = FALSE;  /* bfi is calculated by ARM algorithm */

#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
        #error code not present
#endif
     }
     else
     {
        rpt_hdr->good_data = FALSE;
        rpt_hdr->firecode_used = FALSE;
     }

     if(sch_rx.reason & ENTER_MODE_LATE_CANCEL_BURST) 
     {
        gl1_hw_enter_mode_late_cancel_burst (gas_id);
     }

#ifdef FEATURE_GPRS_GBTA
   if( (sch_rx.reason & SCH_SCHEDULED_BIT) == 0 || (sch_rx.scheduleTimeError == TRUE))
     {
       rpt_hdr->scheduled_status = GPL1_SCH_SCHDULE_FAIL;
     }
     else
     {
       rpt_hdr->scheduled_status = GPL1_SCH_SCHDULE_SUCCESS;
     }
     MSG_GERAN_MED_2_G("GL1 SCH Decode status  %d, scheduled satus %d",sch_rx.reason, rpt_hdr->scheduled_status);
#endif /* FEATURE_GPRS_GBTA */
   }
   else
   {


   /* Retrieve the results */
   results_avail = mdsp_get_async_decoded_data
                    (
                      &rx,
                      (uint16 *)data,
                      data_len,
                      last_time
                      ,gas_id
                    );  /*lint !e826*/

    if (results_avail)
    {

        rpt_hdr->good_data     = rx.gfwDecodedCtrlWord2.crc1;
        rpt_hdr->firecode_used = rx.gfwDecodedCtrlWord2.Ir2WOrFireCode;
        rpt_hdr->len           = rx.len;
        rpt_hdr->chan          = (uint16)chan;

        rpt_hdr->rx_qual       = rx.hardRxQual;  /* Hard RXQUAL */
        rpt_hdr->rx_qual_s     = rx.softRxQual;  /* Soft RXQUAL */


      rpt_hdr->bfi       = FALSE;  /* bfi is calculated by ARM algorithm */

#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
      #error code not present
#endif
    }
    else
    {
      rpt_hdr->good_data = FALSE;
      rpt_hdr->firecode_used = FALSE;
    }
  }

  #ifdef FEATURE_GSM_RX_DIVERSITY
  if (results_avail)
  {
    gl1_hw_set_HardRxQual(rpt_hdr->rx_qual, gas_id);
  }
  #endif /* FEATURE_GSM_RX_DIVERSITY */

  return results_avail;
}
/*===========================================================================

FUNCTION  gl1_hw_tone_detected_total_freq_offset

DESCRIPTION
  This function calculates the total freq offset of the detected tone

===========================================================================*/
int32 gl1_hw_tone_detected_total_freq_offset(ARFCN_T arfcn, int32 afc_freq, gas_id_t gas_id)
{
  int32 carr_temp = gl1_hw_phasetohz( afc_freq );

  if ( gl1_msg_is_arfcn_xo_desense( arfcn, gas_id ) == DESENSE_CHAN )
  {
   carr_temp = gl1_hw_freqtonormxofreq( arfcn, carr_temp, gas_id );
   carr_temp -= gl1_hw_get_xo_acc_freq_err( gas_id );
   carr_temp = gl1_hw_normxofreqtofreq( arfcn, carr_temp, gas_id );
  }

  return(carr_temp);
}

/*===========================================================================

FUNCTION  gl1_hw_was_tone_detected

DESCRIPTION
  This function determines if a tone was detected

DEPENDENCIES
  A tone detection must be started before invoked before invoking
  this function

RETURN VALUE
  TRUE   - a tone has been detected
  FALSE  - no tone has been detected

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_was_tone_detected
(
   ARFCN_T arfcn,
   uint16 *id,
   int32  *tone_offset,
   uint16 *coarse_freq,
   uint16 *fine_freq,
   uint16 *afc_freq,
   uint16 *snr
#ifdef FEATURE_DUAL_SIM
   ,sys_modem_as_id_e_type as_id
#endif /*FEATURE_DUAL_SIM*/
)
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /* FEATURE_DUAL_SIM*/

   return gl1_hw_was_tone_detected_geran(arfcn,id,tone_offset,coarse_freq,fine_freq,afc_freq,snr,as_id);
}

/*===========================================================================

FUNCTION  gl1_hw_was_tone_detected_geran

DESCRIPTION
  This function determines if a tone was detected

DEPENDENCIES
  A tone detection must be started before invoked before invoking
  this function

RETURN VALUE
  TRUE   - a tone has been detected
  FALSE  - no tone has been detected

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_was_tone_detected_geran
(
   ARFCN_T arfcn,
   uint16 *id,
   int32  *tone_offset,
   uint16 *coarse_freq,
   uint16 *fine_freq,
   uint16 *afc_freq,
   uint16 *snr,
   sys_modem_as_id_e_type as_id
)
{
  acq_decode_rpt local_fcch_rpt = { 0 };
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id );

  boolean results_available = gl1_hw_gen_was_tone_detected(
                                arfcn, &local_fcch_rpt, gas_id);

  /* Fine freq return value is the total frequency error (in Hz), so calculate here */
  local_fcch_rpt.fine_freq =
    gl1_hw_tone_detected_total_freq_offset( arfcn, local_fcch_rpt.afc_freq, gas_id );

  /* Cheat for Wl1 to use old I/F avoids W changes */
  if (local_fcch_rpt.afc_freq > MAX_INT16)
  {
     *afc_freq = MAX_INT16;
  }
  else
  {
     if(local_fcch_rpt.afc_freq < (0 - MAX_INT16))
     {
        *afc_freq = MAX_NEG_UINT16;
     }
     else
     {
        *afc_freq = (uint16)local_fcch_rpt.afc_freq;
     }
  }

  if (local_fcch_rpt.fine_freq > MAX_INT16)
  {
     *fine_freq = MAX_INT16;
  }
  else
  {
     if(local_fcch_rpt.fine_freq < (0 - MAX_INT16))
     {
        *fine_freq = MAX_NEG_UINT16;
     }
     else
     {
        *fine_freq = (uint16)local_fcch_rpt.fine_freq;
     }
  }

  *coarse_freq = (uint16)local_fcch_rpt.coarse_freq;

  *tone_offset = local_fcch_rpt.time_offset;
  *id = local_fcch_rpt.id;
  *snr = local_fcch_rpt.snr;

  /* Put an SNR thresh on the CM tone detect to ensure good detect */
  if ( *snr <= GL1_HW_CM_TONE_MIN_SNR_THRESH )
  {
    results_available = FALSE;
  }

  /*This function is called only by WCDMA- so we can pass result as FALSE if burst was not scheduled*/
  return((!local_fcch_rpt.schedule_error)&& (results_available));
}

/*===========================================================================

FUNCTION  gl1_hw_gen_was_tone_detected

DESCRIPTION
  This function determines if a tone was detected

DEPENDENCIES
  A tone detection must be started before invoked before invoking
  this function

RETURN VALUE
  TRUE   - a tone has been detected
  FALSE  - no tone has been detected

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_hw_gen_was_tone_detected(
         ARFCN_T         arfcn,
         acq_decode_rpt *fcch_rpt_p,
         gas_id_t        gas_id )
{
  GfwFcchResultStruct *fcch_data_p;

  boolean              results_available = FALSE;
  GfwFcchResultStruct  fcch_data         = { 0 };

  fcch_data_p = &fcch_data;

  if ( !fcch_rpt_p )
  {
    return ( results_available );
  }

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
  // In case of GPLT and no RF, return default/canned result
  if (gplt_fbsb_data.success)
  {
    MSG_GERAN_HIGH_1_G("GPLT DBG: gl1_hw_gen_was_tone_detected: Returning FCCH result gplt_fbsb_data.success = %d", gplt_fbsb_data.success);
    results_available = TRUE;
  }
  else
  {
    MSG_GERAN_HIGH_1_G("GPLT DBG: gl1_hw_gen_was_tone_detected: Returning FCCH result gplt_fbsb_data.failure_fcch = %d", gplt_fbsb_data.failure_fcch);
    results_available = gplt_fbsb_data.failure_fcch ? FALSE : TRUE;
  }

#else
  results_available = mdsp_get_FCCH_results( fcch_data_p ,gas_id );
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

  if ( results_available )
  {
    fcch_rpt_p->id          = fcch_data_p->toneCount;
    fcch_rpt_p->time_offset = (int32)fcch_data_p->toneStartPosition;
    fcch_rpt_p->coarse_freq = fcch_data_p->coarseFreqEstimate;
    fcch_rpt_p->fine_freq   = fcch_data_p->fineFreqEstimate;
    fcch_rpt_p->snr         = fcch_data_p->snrEst;
    fcch_rpt_p->schedule_error = fcch_data_p->scheduleTimeError;

    /* Full 32-bit length rotator now the F/W sign extends */
    fcch_rpt_p->afc_freq    = fcch_data_p->afcFreqEst;
    fcch_rpt_p->rssi        = fcch_data_p->rssi;
    fcch_rpt_p->arfcn       = arfcn;

#ifdef GERAN_L1_HLLL_LNASTATE
    fcch_rpt_p->jdetReading = 0;
#endif /*GERAN_L1_HLLL_LNASTATE*/

    /* Send a log packet */
    gl1_hw_log_detected_fcch( arfcn, fcch_data_p, gas_id );
  }

  return ( results_available );
}


boolean gl1_hw_x2g_was_tone_detected
(
   ARFCN_T          arfcn,
   acq_decode_rpt*  fcch_rpt,
   uint16           seq_num,
   gas_id_t         gas_id

)
{
  boolean  results_available;
  GfwFcchResultStruct  data;

    fcch_rpt->schedule_error = 0;

  results_available = mdsp_x2g_get_FCCH_results( &data, seq_num, gas_id );

  if ( results_available )
  {
    fcch_rpt->id   = data.toneCount;
    fcch_rpt->time_offset = (int32) data.toneStartPosition;
    fcch_rpt->coarse_freq = data.coarseFreqEstimate;
    fcch_rpt->fine_freq   = data.fineFreqEstimate;
    fcch_rpt->snr         = data.snrEst;

    /* Full 32-bit length rotator now the F/W sign extends */
    fcch_rpt->afc_freq    = data.afcFreqEst;
    fcch_rpt->schedule_error = data.scheduleTimeError;

    /* Send a log packet */
    gl1_hw_log_detected_fcch( arfcn, &data, gas_id );
  }

  return results_available;
}

/*===========================================================================

FUNCTION gl1_hw_get_async_rx_metrics

DESCRIPTION
  This function retreives the metrics for the async burst received in the
  previous frame.
  Sch flag is true for the ncell sch and it will false for the ccch decoding

  This function must be called when metrics a guaranteed to be availalbe
  i.e. do not poll this function.

DEPENDENCIES
  An async receive command must have been issued two frames previously to
  cause an async burst to be received in the previous frame.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_hw_x2g_get_async_sch_rx_metrics
(
   gl1_defs_metrics_rpt*  rpt,
   boolean                last_time,
   gl1_hw_rx_tags_type*   tags,
   uint8                  metrics_buffer_idx,
   uint16                 seq_num,
   ARFCN_T                arfcn,
   sys_modem_as_id_e_type  as_id

)
{
   boolean metrics_available = FALSE;
   GfwBurstMetricsStruct metrics;
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);


   metrics_available = mdsp_x2g_get_async_sch_rx_metrics( &metrics, last_time, metrics_buffer_idx, seq_num,gas_id );

   if (metrics_available)
   {
      rpt->arfcn         = arfcn;
      rpt->rssi          = (metrics.rssiMsw << 16) + metrics.rssiLsw;
      rpt->dc_offset_i   = (int16)metrics.dcI;
      rpt->dc_offset_q   = (int16)metrics.dcQ;
      rpt->freq_offset   = (int16)metrics.freqOffset;
      rpt->timing_offset = (int16)metrics.timingOffset;

      rpt->snr           = metrics.snr;
      rpt->powerStatus   = gl1_msg_compute_rx_power(rpt->rssi,
                                                    &rpt->pwr_dBm_x16,
                                                    &rpt->rx_gain_range,
                                                    &tags->gain);

#ifdef GERAN_L1_HLLL_LNASTATE
      rpt->jdet_value = metrics.jdetReading;
      rpt->SawlessHLLLState = tags->sawless_lnastate;
#endif

     /* Now fill in a log packet to help RF debug */
     gl1_hw_log_async_rx_data( rpt,gas_id );

#ifdef FEATURE_GSM_MDSP_AEQ_SCH

     {
       gl1_hw_log_aeq_sch_metrics(&metrics,gas_id);
     }
#endif
   }

   gl1_hw_log_cell_cmds_to_diag (cmd_get_asyncrx_metrics, FALSE,gas_id);

   return metrics_available;
}


boolean gl1_hw_get_async_rx_metrics
(
   gl1_defs_metrics_rpt *rpt,
   boolean              last_time,
   gl1_hw_rx_tags_type  *tags
   ,boolean sch_flag
#ifdef FEATURE_DUAL_SIM
   ,sys_modem_as_id_e_type  as_id
#endif
)
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/

   return gl1_hw_get_async_rx_metrics_geran(rpt,last_time,tags,sch_flag,as_id);
}


boolean gl1_hw_get_async_rx_metrics_geran
(
   gl1_defs_metrics_rpt *rpt,
   boolean              last_time,
   gl1_hw_rx_tags_type  *tags
   ,boolean sch_flag
   ,sys_modem_as_id_e_type   as_id
)
{
   boolean metrics_available = FALSE;
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
   GfwBurstMetricsStruct metrics;
#ifdef FEATURE_GSM_SENS_SUPPORT
   GfwGdetStruct gfwGdetResults;
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
  gl1_rxd_control_type gl1_rxd_ctl_flag;
  /* Log Rx Diversity metrics only if Rx diversity NV is enabled*/
  gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef FEATURE_GSM_SENS_SUPPORT
   metrics_available = mdsp_get_async_rx_metrics( &metrics, last_time, sch_flag, &gfwGdetResults ,gas_id );
#else
   metrics_available = mdsp_get_async_rx_metrics( &metrics, last_time, sch_flag ,gas_id );
#endif

   if (metrics_available)
   {
      rpt->rssi          = (metrics.rssiMsw << 16) + metrics.rssiLsw;
      rpt->dc_offset_i   = (int16)metrics.dcI;
      rpt->dc_offset_q   = (int16)metrics.dcQ;
      rpt->freq_offset   = (int16)metrics.freqOffset;
      rpt->timing_offset = (int16)metrics.timingOffset;
      rpt->snr           = metrics.snr;
#ifdef FEATURE_GSM_SENS_SUPPORT
	  if(sch_flag && gl1_hw_get_sens_fr_ctrl())
	  {
        rpt->gfwGdetResults = gfwGdetResults;
	  }
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
      /*In DRX only mode use DRX gain values to calculate PRX power level in dBm*/
      if((gl1_rxd_ctl_flag.enableRxd) && (gl1_rxd_ctl_flag.enableDrx)
         && (!(gl1_rxd_ctl_flag.enablePrx)))
       {
      rpt->powerStatus   = gl1_msg_compute_rx_power(rpt->rssi,
                                                    &rpt->pwr_dBm_x16,
                                                    &rpt->rx_gain_range,
                                                   &tags->gain_divrx);
       }
      else
#endif
       {
      rpt->powerStatus   = gl1_msg_compute_rx_power(rpt->rssi,
                              &rpt->pwr_dBm_x16,
                              &rpt->rx_gain_range,
                                                    &tags->gain);
       }

#ifdef FEATURE_GSM_RX_DIVERSITY
      rpt->dc_offset_i_divrx     = (int16)metrics.rxdLogPacket.dcIDRx;
      rpt->dc_offset_q_divrx     = (int16)metrics.rxdLogPacket.dcQDRx;
      rpt->rssi_divrx = metrics.rxdLogPacket.rssiDRx;
      if(rpt->rssi_divrx != 0)
       {
       rpt->powerStatus_divrx = gl1_msg_compute_rx_power(rpt->rssi_divrx,
                                                         &rpt->pwr_dBm_x16_divrx,
                                                         &rpt->rx_gain_range_divrx,
                                                         &tags->gain_divrx);
       }
     else
       {
       rpt->pwr_dBm_x16_divrx   = 0;
       rpt->rx_gain_range_divrx = 0;
       rpt->powerStatus_divrx   = RF_NOISY;
       }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef GERAN_L1_HLLL_LNASTATE
     rpt->jdet_value = metrics.jdetReading;
     rpt->SawlessHLLLState = tags->sawless_lnastate;
#endif /*GERAN_L1_HLLL_LNASTATE*/

     /* Now fill in a log packet to help RF debug */
     gl1_hw_log_async_rx_data( rpt ,gas_id);

#ifdef FEATURE_GSM_MDSP_AEQ_SCH
     if(sch_flag)
     {
       gl1_hw_log_aeq_sch_metrics(&metrics,gas_id);
     }
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
    /* Log Rx Diversity metrics only if Rx diversity NV is enabled*/
    if(gl1_rxd_ctl_flag.enableRxd)
     {
      gl1_hw_log_rxd_metrics(rpt->pwr_dBm_x16,rpt->pwr_dBm_x16_divrx,
                             rpt->rx_gain_range,rpt->rx_gain_range_divrx,
                             rpt->powerStatus_divrx,&(metrics.rxdLogPacket),LOG_NSCH,gas_id);
   }
#endif
   }

   gl1_hw_log_cell_cmds_to_diag (cmd_get_asyncrx_metrics, FALSE,gas_id);

   return metrics_available;
}

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
void gl1_hw_tch_loopback( gl1_defs_loopback_type loopback,gas_id_t gas_id )
{
    gl1_hw_mdsp_loopback[gas_id].mode = loopback;
    gl1_hw_mdsp_loopback[gas_id].pending = TRUE;

    /* This is a workaround to enable test case 21.3.4 to pass, Half rate
     *  with SAIC enabled cause the test case to fail.
     *
     * This needs to be removed when firmware resolve this issue
     */

    if ( !gl1_hw_feature_disabled_saic(gas_id) )
    {
      /* SAIC is enabled */
      if( ((tch_chan_type[gas_id] == GL1_HW_TCH_AHS) || (tch_chan_type[gas_id] == GL1_HW_TCH_HS))
          && (loopback == GL1_DEFS_LOOPBACK_TYPE_C) )
      {
        /* disable SAIC for half rate channels when we are doing loop C*/
        mdsp_enable_saic(FALSE ,gas_id);
        MSG_GERAN_HIGH_2_G("HR Loop-C SAIC Fix for channel %d loopback mode %d",tch_chan_type[gas_id],loopback);
        gl1_hw_cfg_saic( FALSE,gas_id );
      }

      else if( ((tch_chan_type[gas_id] == GL1_HW_TCH_AHS) || (tch_chan_type[gas_id] == GL1_HW_TCH_HS))
               && (loopback == GL1_DEFS_NO_LOOPBACK) )
      {
        /* Turn SAIC back on again when turning off loopback */
        mdsp_enable_saic(TRUE ,gas_id);
        MSG_GERAN_HIGH_2_G("HR Loop-C SAIC Fix for channel %d loopback mode %d",tch_chan_type[gas_id],loopback);
        gl1_hw_cfg_saic( TRUE,gas_id );

      }

    }
}
/*===========================================================================

FUNCTION gl1_hw_issue_mdsp_loopback_cmd

DESCRIPTION
  This function send the loopback command to the mdsp to put the TCH into
  loopback A,B or C or no loopback.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_issue_mdsp_loopback_cmd(gas_id_t gas_id)
{
   if (gl1_hw_mdsp_loopback[gas_id].pending == TRUE)
   {
      gl1_hw_mdsp_loopback[gas_id].pending = FALSE;
      mdsp_set_loopback( (mdsp_loopback_type) gl1_hw_mdsp_loopback[gas_id].mode ,gas_id);
   }
}

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
void gl1_hw_cleanup_early_decode( gl1_hw_rx_tags_type *tags,gas_id_t gas_id )
{
   /* Clear the asynch int receive flag as we have completed SBD */
   if ( gl1_hw_get_mdsp_asynch_int(gas_id) )
   {
     gl1_hw_set_mdsp_asynch_int( FALSE,gas_id );
   }

   mdsp_dtm_abort_rx(tags->data_tag, gas_id);
}

/*===========================================================================

FUNCTION gl1_hw_cleanup_async_rx

DESCRIPTION
  Clean up drivers after an async rx.  Used to make aborting and early
  decoding cleanup simpler for the message layer.

===========================================================================*/
void gl1_cm_hw_cleanup_async_rx (sys_modem_as_id_e_type as_id)
{
   gas_id_t       gas_id =  geran_map_nas_id_to_gas_id(as_id);
   gl1_hw_cleanup_async_rx_geran(gas_id);
  }

void gl1_hw_cleanup_async_rx (void)
{
   gl1_hw_cleanup_async_rx_geran(GERAN_ACCESS_STRATUM_ID_1);
}


/*===========================================================================

FUNCTION gl1_hw_cleanup_async_rx_geran

DESCRIPTION
  Clean up drivers after an async rx.  Used to make aborting and early
  decoding cleanup simpler for the message layer.

===========================================================================*/
void gl1_hw_cleanup_async_rx_geran (gas_id_t gas_id)
{
  /* Clear the asynch int receive flag as we have completed asynch rx */
  if ( gl1_hw_get_mdsp_asynch_int( gas_id ) )
  {
    gl1_hw_set_mdsp_asynch_int( FALSE, gas_id );
  }

  /* Make mDSP happy */
  mdsp_reset_async_rx_ptrs( gas_id );
}

/*===========================================================================

FUNCTION gl1_hw_get_sch_decode_mode

DESCRIPTION
  Set SCH decode mode for X2G

===========================================================================*/
gl1_hw_sch_decode_t gl1_hw_get_sch_decode_mode(
                      boolean sch_decode )
{
  /* Default to SCH disabled */
  gl1_hw_sch_decode_t sch_decode_mode = GL1_NO_SCH_DECODE;

  /* Now if sch decode required ascertain if Parallel SCH enabled */
  if ( sch_decode )
  {
    if ( gl1_hw_is_parallel_sch_enabled() )
    {
      sch_decode_mode = GL1_PARALLEL_SCH_DECODE;
    }
    else
    {
      sch_decode_mode = GL1_CONVENTIONAL_SCH_DECODE;
    }
  }

  return ( sch_decode_mode );
}

/*===========================================================================

FUNCTION gl1_hw_x2g_get_sch_decode_mode

DESCRIPTION
  Set SCH decode mode for X2G

===========================================================================*/
gl1_hw_sch_decode_t gl1_hw_x2g_get_sch_decode_mode(
                             boolean sch_decode )
{
  /* Default to SCH disabled */
  gl1_hw_sch_decode_t x2g_sch_decode_mode = GL1_NO_SCH_DECODE;

  /* Now if sch decode required ascertain if Parallel SCH enabled */
  if ( sch_decode )
  {
    if ( gl1_hw_is_x2g_parallel_sch_enabled() )
    {
      x2g_sch_decode_mode = X2G_PARALLEL_SCH_DECODE;
    }
    else
    {
      x2g_sch_decode_mode = X2G_CONVENTIONAL_SCH_DECODE;
    }
  }

  return ( x2g_sch_decode_mode );
}


#ifdef FEATURE_GSM_MDSP_ESAIC
/*===========================================================================

FUNCTION gl1_hw_clear_esaic_iir_filter

DESCRIPTION
  Tells the mDSP to Clear it's IIR filter for use in applying the ESAIC on
  Downlink channels.

  Currently needs to be called For Downlink DTX and Handover scenarios

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_flush_esaic_iir_filter (boolean flush_esacch, boolean restore_ersacch,gas_id_t gas_id)
{
  /* issue Command to mdsp Driver toFlush IIR filter */
  mdsp_flush_esaic_iir_filter(flush_esacch, restore_ersacch ,gas_id);
}

#endif /* FEATURE_GSM_MDSP_ESAIC */

#ifdef FEATURE_GSM_GPRS_TX_INFO
/*===========================================================================

FUNCTION  gl1_hw_save_tx_arfcn

DESCRIPTION
  This function will save the current active TX arfcn and Band.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_hw_save_tx_arfcn (ARFCN_T *arfcn,gas_id_t gas_id)
{
    gl1_hw_stored_arfcn[gas_id].num  = arfcn->num;
    gl1_hw_stored_arfcn[gas_id].band = arfcn->band;
}

/*===========================================================================

FUNCTION  gl1_hw_get_current_tx_arfcn

DESCRIPTION
  This function will save the current active TX arfcn and Band.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_hw_get_current_tx_arfcn (ARFCN_T *arfcn,gas_id_t gas_id)
{
    arfcn->num  = gl1_hw_stored_arfcn[gas_id].num;
    arfcn->band = gl1_hw_stored_arfcn[gas_id].band;
}
#endif // FEATURE_GSM_GPRS_TX_INFO

#ifdef FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY
/*===========================================================================

FUNCTION gl1_hw_handle_timeline_error

DESCRIPTION
  Handle RF timeline error 

===========================================================================*/
void gl1_hw_handle_timeline_error(gas_id_t gas_id)
{
  gtmrs_data_t *gtmrs_data_p;
  int      as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  gtmrs_data_p = &gtmrs_data[as_idx];

  switch (gl1_gfw_nv_recovery) 
  {
  case GL1_GFW_RECOVER_AND_MOVE_ON:
     break;
  case GL1_GFW_CRASH_IN_FW:
     break;
  case GL1_GFW_CRASH_IN_SW:
#ifdef FEATURE_QSH_MDUMP
       QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
     ERR_FATAL("RF timeline error recovery: err fatal",0,0,0);
     break;
  case GL1_GFW_CALL_DROP:
     L1_send_PH_CS_ABORT_IND(GL1_CS_ABORT_FOR_COEXISTENCE, SACCH, gas_id);
     break;
  default:
     break;
  }
}

/*===========================================================================

FUNCTION gl1_hw_rf_recovery_cleanup

DESCRIPTION
  Reset variables for RF timeline error recovery mechanism

===========================================================================*/
void gl1_hw_rf_recovery_cleanup(boolean leave_ded,gas_id_t gas_id)
{
  gtmrs_data_t *gtmrs_data_p;
  int      as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  gtmrs_data_p = &gtmrs_data[as_idx];

  if ( gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE ) 
  {
     if (leave_ded)
     {
        gtmrs_data_p->geran_rf_timeline_recovery.number_of_dropped_tx = 0;
        gtmrs_data_p->geran_rf_timeline_recovery.number_of_success_pow_mon = 0;
        gtmrs_data_p->geran_rf_timeline_recovery.number_of_success_pow_mon_old = 0;

     }
     else
     {
        if ( get_vstmr_FN_modMaxFN(gas_id)%26==0 ) 
        {
           gtmrs_data_p->geran_rf_timeline_recovery.number_of_success_pow_mon_old = gtmrs_data_p->geran_rf_timeline_recovery.number_of_success_pow_mon;
           gtmrs_data_p->geran_rf_timeline_recovery.number_of_dropped_tx = 0;
           gtmrs_data_p->geran_rf_timeline_recovery.number_of_success_pow_mon = 0;
        }
     }
  }
}
/*===========================================================================

FUNCTION gl1_hw_tx_drop

DESCRIPTION
  Handling of skipped Tx burst

===========================================================================*/
void gl1_hw_tx_drop(gas_id_t gas_id)
{
  gtmrs_data_t *gtmrs_data_p;
  int      as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  gtmrs_data_p = &gtmrs_data[as_idx];

  if ((gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE) && gl1_hw_get_dedicated(gas_id))
  {
    gtmrs_data_p->geran_rf_timeline_recovery.number_of_dropped_tx++;

    if (gl1_msg_is_sacch_plus_one(gas_id) || (gtmrs_data_p->geran_rf_timeline_recovery.number_of_dropped_tx >=2)) 
    {
       gl1_hw_handle_timeline_error(gas_id);
    } 
  }
}

/*===========================================================================

FUNCTION gl1_hw_pow_mon_drop

DESCRIPTION
  Call on HO request. Handling of skipped power monitors.
===========================================================================*/
void gl1_hw_pow_mon_drop(gas_id_t gas_id)
{
   gtmrs_data_t *gtmrs_data_p;
   int      as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
   gtmrs_data_p = &gtmrs_data[as_idx];

   if ( gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE ) 
   {
     if (gtmrs_data_p->geran_rf_timeline_recovery.number_of_success_pow_mon_old <= 3)
     {
        gl1_hw_handle_timeline_error(gas_id); 
     }
   }
}
#endif /* FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY */
/*===========================================================================

FUNCTION gl1_hw_acq_late

DESCRIPTION
  Handling of late acquisition
===========================================================================*/
void gl1_hw_acq_late(gas_id_t gas_id)
{
   if ((gl1_gfw_nv_recovery & GL1_GFW_ACQ_DEL_RECOVERY_NV_MASK) == GL1_GFW_ACQ_DELAYED_CRASH_IN_SW)
   {
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
	 if ( ++gl1_hw_burst_recovery_count[gas_id].acq_recovery_count == GL1_HW_MAX_BURST_ERR_REC_COUNT )
#endif
	 {
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_FATAL("RF timeline error recovery: ACQ DELAYED GFW crash",0,0,0);
   }
	  MSG_GERAN_ERROR_0_G("RF timeline error recovery: ACQ DELAYED");
   }
   else
   {
      MSG_GERAN_ERROR_0_G("RF timeline error recovery: ACQ DELAYED");
   }
}

/*===========================================================================

FUNCTION gl1_hw_x2g_acq_cancelled

DESCRIPTION
  Handling of cancelled x2g acquisition
===========================================================================*/
void gl1_hw_x2g_acq_cancelled(gas_id_t gas_id)
{
   if ((gl1_gfw_nv_recovery & GL1_GFW_X2G_ACQ_CANCEL_RECOVERY_NV_MASK) == GL1_GFW_X2G_ACQ_CANCEL_CRASH_IN_SW)
   {
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
	if ( ++gl1_hw_burst_recovery_count[gas_id].x2g_acq_recovery_count == GL1_HW_MAX_BURST_ERR_REC_COUNT )
#endif 
	{
#ifdef FEATURE_QSH_MDUMP
			QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif

      ERR_FATAL("RF timeline error recovery: X2G ACQ CANCELLED GFW crash",0,0,0);
   }
   	MSG_GERAN_ERROR_0_G("RF timeline error recovery: X2G ACQ CANCELLED");
   }
   else
   {
      MSG_GERAN_ERROR_0_G("RF timeline error recovery: X2G ACQ CANCELLED");
   }
}
/*===========================================================================

FUNCTION gl1_hw_enter_mode_late_cancel_burst

DESCRIPTION
  Handling of RX blanking
===========================================================================*/
void gl1_hw_enter_mode_late_cancel_burst (gas_id_t gas_id)
{
   if ((gl1_gfw_nv_recovery & GL1_GFW_RX_BLANKING_RECOVERY_NV_MASK) == GL1_GFW_RX_BLANKING_CRASH_IN_SW)
   {
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
	if ( ++gl1_hw_burst_recovery_count[gas_id].rx_blanking_count == GL1_HW_MAX_BURST_ERR_REC_COUNT )
#endif
	{
#ifdef FEATURE_QSH_MDUMP
			QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif

      ERR_FATAL("RF timeline error recovery: RX BLANKING GFW crash",0,0,0);
   }
	MSG_GERAN_ERROR_0_G("RF timeline error recovery: RX BLANKING, GFW recovery");     
   }
   else
   {
      MSG_GERAN_ERROR_0_G("RF timeline error recovery: RX BLANKING, GFW recovery");
   }
}

/*===========================================================================

FUNCTION gl1_hw_handle_seq_num_error

DESCRIPTION
  Handling of sequence number timeline errors
===========================================================================*/
void gl1_hw_handle_seq_num_error (gl1_gfw_recovery_burst_type burst, boolean sched_err, gas_id_t gas_id)
{
  switch (burst) 
  {
  case GL1_GFW_RECOVERY_MON:
     if ((gl1_gfw_nv_recovery & GL1_GFW_SEQ_NUM_REC_MON_NV_MASK) == GL1_GFW_SEQ_NUM_REC_MON_CRASH_IN_SW)
     {
       if(sched_err == TRUE)
       {
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
         if ( ++gl1_hw_burst_recovery_count[gas_id].mon_recovery_count == GL1_HW_MAX_BURST_ERR_REC_COUNT )
#endif
         {
#ifdef FEATURE_QSH_MDUMP
     QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_FATAL("RF timeline error: MON drop",0,0,0);
         }
         gl1_hw_burst_success_count[gas_id].mon_success_count = 0;
         timeline_error_recovery_needed[gas_id] |= GL1_GFW_SEQ_NUM_REC_MON_CRASH_IN_SW;
         MSG_GERAN_ERROR_1_G("RF timeline error recovery: MON drop count %d",gl1_hw_burst_recovery_count[gas_id].mon_recovery_count);
       }
       else if((sched_err == FALSE) && ((timeline_error_recovery_needed[gas_id] & GL1_GFW_SEQ_NUM_REC_MON_CRASH_IN_SW) != 0))
       {
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
         if ( ++gl1_hw_burst_success_count[gas_id].mon_success_count == GL1_HW_MAX_BURST_SUCCESS_COUNT )
#endif
         {
           timeline_error_recovery_needed[gas_id] &= ~GL1_GFW_SEQ_NUM_REC_MON_CRASH_IN_SW;
           gl1_hw_burst_success_count[gas_id].mon_success_count = 0;
           /*Inform MCPM about critical scenario OFF now if needed*/
           if(critical_scn_updated_to_mcpm[gas_id] == TRUE)
           {
             gl1_clkdata_speed(FALSE, L1_TIME_LINE_ERROR, 0, L1_TIME_LINE_DELAY,gas_id);
             critical_scn_updated_to_mcpm[gas_id] = FALSE;
           }
         }
         MSG_GERAN_HIGH_1_G("RF timeline error recovery:MON success count %d",gl1_hw_burst_success_count[gas_id].mon_success_count);
       }
     }
     break;

  case GL1_GFW_RECOVERY_RX:
     if ((gl1_gfw_nv_recovery & GL1_GFW_SEQ_NUM_REC_RX_NV_MASK) == GL1_GFW_SEQ_NUM_REC_RX_CRASH_IN_SW)
     {
       if(sched_err == TRUE)
       {
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
         if ( ++gl1_hw_burst_recovery_count[gas_id].rx_recovery_count == GL1_HW_MAX_BURST_ERR_REC_COUNT )
#endif
         {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif

           ERR_FATAL("RF timeline error: Rx Burst drop",0,0,0);
         }
         gl1_hw_burst_success_count[gas_id].rx_success_count = 0;
         timeline_error_recovery_needed[gas_id] |= GL1_GFW_SEQ_NUM_REC_RX_CRASH_IN_SW;
         MSG_GERAN_ERROR_1_G("RF timeline error recovery: Rx Burst drop count %d ",gl1_hw_burst_recovery_count[gas_id].rx_recovery_count);
       }
       else if((sched_err == FALSE) && ((timeline_error_recovery_needed[gas_id] & GL1_GFW_SEQ_NUM_REC_RX_CRASH_IN_SW) != 0))
       {
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
         if ( ++gl1_hw_burst_success_count[gas_id].rx_success_count == GL1_HW_MAX_BURST_SUCCESS_COUNT )
#endif
         {
           timeline_error_recovery_needed[gas_id] &= ~GL1_GFW_SEQ_NUM_REC_RX_CRASH_IN_SW;
           gl1_hw_burst_success_count[gas_id].rx_success_count = 0;
           /*Inform MCPM about critical scenario OFF now if needed*/
           if(critical_scn_updated_to_mcpm[gas_id] == TRUE)
           {
             gl1_clkdata_speed(FALSE, L1_TIME_LINE_ERROR, 0,L1_TIME_LINE_DELAY, gas_id);
             critical_scn_updated_to_mcpm[gas_id] = FALSE;
           }
         }
         MSG_GERAN_HIGH_1_G("RF timeline error recovery:Rx success count %d",gl1_hw_burst_success_count[gas_id].rx_success_count);
       }
     }
     break;

  case GL1_GFW_RECOVERY_TX:
     if ((gl1_gfw_nv_recovery & GL1_GFW_SEQ_NUM_REC_TX_NV_MASK) == GL1_GFW_SEQ_NUM_REC_TX_CRASH_IN_SW)
     {
       if(sched_err == TRUE)
       {
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY     
         if ( ++gl1_hw_burst_recovery_count[gas_id].tx_recovery_count == GL1_HW_MAX_BURST_ERR_REC_COUNT )
#endif
         {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_FATAL("RF timeline error: Tx Burst drop",0,0,0);
         }
         gl1_hw_burst_success_count[gas_id].tx_success_count = 0;
         timeline_error_recovery_needed[gas_id] |= GL1_GFW_SEQ_NUM_REC_TX_CRASH_IN_SW;
         MSG_GERAN_ERROR_1_G("RF timeline error recovery: Tx Burst drop count %d ",gl1_hw_burst_recovery_count[gas_id].tx_recovery_count);
       }
       else if((sched_err == FALSE) && ((timeline_error_recovery_needed[gas_id] & GL1_GFW_SEQ_NUM_REC_TX_CRASH_IN_SW) != 0))
       {
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
         if ( ++gl1_hw_burst_success_count[gas_id].tx_success_count == GL1_HW_MAX_BURST_SUCCESS_COUNT )
#endif
         {
           timeline_error_recovery_needed[gas_id] &= ~GL1_GFW_SEQ_NUM_REC_TX_CRASH_IN_SW;
           gl1_hw_burst_success_count[gas_id].tx_success_count = 0;
           /*Inform MCPM about critical scenario OFF now if needed*/
           if(critical_scn_updated_to_mcpm[gas_id] == TRUE)
           {
             gl1_clkdata_speed(FALSE, L1_TIME_LINE_ERROR, 0,L1_TIME_LINE_DELAY, gas_id);
             critical_scn_updated_to_mcpm[gas_id] = FALSE;
           }
         }
         MSG_GERAN_HIGH_1_G("RF timeline error recovery: Tx success count %d",gl1_hw_burst_success_count[gas_id].tx_success_count);
       }
     }
     break;

  case GL1_GFW_RECOVERY_T2G_MON:
     break;
  default:
     MSG_GERAN_ERROR_0_G("RF timeline error recovery: unknown type");
     break;
   }
}


/*=======================================================================================
FUNCTION gl1_hw_update_tx_burst_success

DESCRIPTION
  If Tx burst error has happened then gl1 will inform MCPM about critical scenario
  But GFW does not inform Gl1 about Tx burst success, but gl1 need to remove mcpm vote
  So, tx error will be removed after 5 G frames and clock update will be removed,
  if error keeps on coming, Gl1 will crash after 5 such Tx errors

  Intention is to keep higher mcpm clocks for 5 G frames and then see if error persists
========================================================================================*/
void gl1_hw_update_tx_burst_success(gas_id_t gas_id)
{
  if((timeline_error_recovery_needed[gas_id] & GL1_GFW_SEQ_NUM_REC_TX_CRASH_IN_SW) != 0)
  {
    MSG_GERAN_HIGH_0_G("Debug: gl1_hw_update_tx_burst_success");
    gl1_hw_handle_seq_num_error (GL1_GFW_RECOVERY_TX, FALSE,gas_id);
  }
}

#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY
void gl1_hw_reset_burst_recovery_counters(gas_id_t gas_id)
{
  gl1_hw_burst_recovery_count[gas_id].mon_recovery_count = 0;
  gl1_hw_burst_recovery_count[gas_id].rx_recovery_count  = 0;
  gl1_hw_burst_recovery_count[gas_id].tx_recovery_count  = 0;
  gl1_hw_burst_recovery_count[gas_id].rx_blanking_count  = 0;
  gl1_hw_burst_recovery_count[gas_id].acq_recovery_count = 0;
  gl1_hw_burst_recovery_count[gas_id].x2g_acq_recovery_count = 0;

  timeline_error_recovery_needed[gas_id] = 0;
  
  /*Inform MCPM about critical scenario OFF now if needed*/
  if(critical_scn_updated_to_mcpm[gas_id] == TRUE)
  {
    gl1_clkdata_speed(FALSE, L1_TIME_LINE_ERROR, 0,L1_TIME_LINE_DELAY, gas_id);
    critical_scn_updated_to_mcpm[gas_id] = FALSE;
  }
}
#endif

void gl1_hw_reset_burst_success_counters(gas_id_t gas_id)
{
  gl1_hw_burst_success_count[gas_id].mon_success_count = 0;
  gl1_hw_burst_success_count[gas_id].rx_success_count  = 0;
  gl1_hw_burst_success_count[gas_id].tx_success_count  = 0;
}

