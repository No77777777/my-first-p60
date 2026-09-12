/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              L 1   N U L L  M O D E   T A S K  P R O C E S S I N G

GENERAL DESCRIPTION
   This module contains the procedures to handle events while the L1
   task is in the NULL_MODE mode.  Events can originate from other
   layers (eg: RR) or from the L1 ISR.

EXTERNALIZED FUNCTIONS
  L1_handle_null_mode
    Procedure to handle external events while the L1 task is in the
    NULL_MODE mode.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_null_if.c#3 $
$DateTime: 2020/02/06 01:54:50 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
05/02/20   sal       CR2614195 In case of IRAT activity on, wait for one frame while sending SUSPEND_GSM_MODE_CNF to RR.
16/01/20   sc        CR2213654 Do G2X Deinit from inact_isr when GSM is deactivating
02/20/18   ks        CR2005881 : KW P1 errors on AT.2.3 fix
08/09/17   akm       CR2106414 P1 issue on AT.3.1 | L1_handle_WTOG_BPLMN_mode()
24/01/18   sp        CR2099959 Wait one more frame before processing StopModeReq in WTOG_BPLMN state if it was followed by MPH_RESUME_GSM_MODE_FOR_BPLMN_REQ.
20/05/16   zf        CR1016887: To update correct freq_info when handling DECODE_BCCH_LIST command
21/04/17   br        CR1041084 FR 36219: GDET – GMSK Modulation Detector
03/01/16   mn        CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
03/04/17   br        CR2027088 Band based ASDIV support during ACQ
27/12/16   sk        CR1092325 GL1 to report zero frequencies when sending MPH_POWER_SCAN_CNF for DS Abort       
15/06/16   mk        CR1028521 Improve AIT delay in X2G Handover by removing +/-10 QS Qbit margin check to avoid blind HO
18/5/16    sm        CR1017901 Added GL1 idle mode QSH events 
10/03/16   dg        CR986989 - Re-work CR798863 After X2G HO is successful, don't adjust G frame number when G2X Handover is triggered
08/03/16   dg        CR975456 Send tx on to RF after frame adjustment has happened for X2G Handover case
23/02/16   br        CR980003 Add ISR lock while handling stop mode req in suspended mode to prevent concurrent isr execution
28/01/16   sk	     CR967760 Added header file "smscb.h" to remove compilation warning
12/01/16   sk        CR960249 Added CB messages handling in Suspend mode too.
04/11/15   cah       CR934489 Update GRM freq_info with request bands for BCCH decode list.
17/11/15   sjw       CR938544 Ensure the DSDS handler for BPLMN subtracts the X2G frame overhead.
09/11/15   og        CR929332 Don't do immediate logging of power monitors when suspending GSM L1.
10/09/15   sk        CR899847 GERAN changes to trigger QSH events and handler for debugging
11/09/15   br        CR903740 Add mutex protection for rach abort,to prevent removal of handler while handler is executing
01/09/15   hd        CR899231 Remove the rach_tx_handler once MPH_RECONNECT_CHANNEL_REQ is received
26/08/15   ws        CR896163 Fix compile errors for undefining FEATURE_DUAL_SIM
24/07/15   smd       CR877996 Reset trm_inv flag when deactivating GL1
21/07/15   cjl       CR876903 Use system procedure types for TRM subreasons 
03/23/15   sjv       CR810208 update trm freq info for BPLMN only before TRM req
13/07/15   ab        CR832798 FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
25/06/15   dg        CR798863 Adjust GSTMR back to its previous value after X2G HO Fail
22/05/15   ws        CR790493 Check BSIC decoded in SCH matches requested BSIC from RR
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX 
08/04/15   pjr       CR818535 FR25951 Changes to support Single Voice Session 
31/03/15   cja       CR800650 Optimise irat state machine
31/03/15   pa        CR815621:GRM restructuring for TH 2.0. 
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic 
11/03/15   pjr       CR806250 Fix featurisation for changes submitted under CR787199
24/02/15   pjr       CR787199 Acquire vocoder immediately after IMS releases it during srvcc handover
27/02/15   am        CR646584 WLAN coex
07/01/15   ap        CR776194: RLC timer expiry due to GERAN L1 not responding to MPH_POWER_SCAN_ABORT_REQ message 
22/09/14   ap        CR720862: FR 20912: SGLTE+G DSDA with WTR asymmetric GSM support
08/09/14   sp        CR720676: add ISR lock before processing list decode req for w2g bplmn scenario.
07/09/14   js        CR711466:Reset the main command after drx off in W2G handover.
01/08/14   zf        CR703343 When aborting WTOG BPLMN search, dont inactive GL1 task again
                     if WTOG BPLMN search has already suspended
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
05/08/14   npt       CR694242 Make sure VFR has been released before exiting GSM 
25/06/14   cs        Remove FEATURE_ASYMMETRIC_GERAN_BANDS
08/05/14   sp        CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
17/03/14   pjr       CR629255 Protect calls to gl1_vs_release with critical section
04/04/14   sai       CR638388: Delay TBF re-establish in DSDS if G2W resel ongoing on other sub.
08/04/14   br        CR645100 Deallocate RXLM,TXLM buffres while deactivating bplmn, in case CM is already intailzed.
27/01/14   cgc       CR607316 add task mutex protection for race between gl1_hw_cm_gsm_init() and BPLMN deactivate
21/01/14   cgc       CR602010 TR2.0 merge fix,prevent WTOG BPLMN deactivate
31/12/13   xz        CR594706 record more status for compressed mode
12/12/13   pjr       CR588712 Call mn_has_nas_already_acquired_voc when FEATURE_DUAL_ACTIVE is enabled
06/12/13   nk        CR585331 Don't disable FW when handling STOP_MODE_REQ in WTOG_BPLMN mode if CM is initialized
27/12/13   dg        CR593754 Gl1 to set TRX state to ON ALWAYS during beginning of X2G IRAT HO
28/11/13   mko       CR580118 Disbale the geran app while processing stop mode request in W2GBPLMN
20/11/13   ka        CR579293 Incorrect cast of message type on reception of MPH_STOP_GSM_MODE_REQ
11/11/13   ws        CR 573757 Featurise GSMTR-1 registers and Voice services driver
23/10/13   pa        CR565614:Copied 128-bit ciphering key to internal data structure.
11/10/13   js        CR556573 Invaild releasing of VS buffers
10/10/13   js        CR520476 Always set the power scan frequency parameters to 0, when we receive power scan request
04/10/13   nm        CR552803 tighten power scan threshold from -107dBm to -102dBm for background power scan only
01/10/13   sk        CR552259 CR sync from 1.0/1.1
20/08/13   nn        CR518344 Use secapi_get_random() for generating random numbers.
18/09/13   sjw       CR543997 - do not allow power scan results to be shared if invalid
114/09/13  sp        CR460968 Ensure W2G BPLMN Power Scan works correctly [Propogation]
16/09/13   cs        CR503042 Updated XO RGS handling for Triton
08/05/13   cgc       CR453758 add FEATURE_CM_G2L search functionality FR-3009
24/07/13   jk        CR464982:Changes to support GEA5_4 ciphering
01/08/13   sk        CR521009 Ensure the global flag rr_send_stop_gsm_mode_req is data spaced
07/16/13   zc        CR470197 Replace memcpy with memscpy
25/06/13   cs        Major Triton TSTS Syncup
16/05/13   ab        CR487396 - Support EGPRS IR Reduced Window in GFW
24/05/13   ap        CR472264 DSDS: Fix the gas_id problems when the IQ logging is enabled
23/05/13   ss        CR490280 Clear IRAT SC list while terminating GSM
23/05/13   ss        CR490459 Execute L1_resume_req independent of dual sim mode
16/05/13   pjr       CR487402: close vocoder when gsm only sub gets deactivated
06/06/13   cs        TSTS Updates
20/12/12   am/ap     CR432888 Adjust BPLMN frames for acqusition of BCCH in DSDS mode
10/05/13   sai       CR 481625: Force G2X shutdown at GL1 task suspend.
22/04/13   cja       CR476910 When suspending or stopping W2G BPLMN exit RF
26/02/13   ip        DSDS CR453357, don't sleep until stop mode CNF is sent
18/04/13   ss        CR476304 Inactivate gl1 task on MPH_SUSPEND_GSM_MODE_REQ
10/04/13   pg        CR469228 Avoid schedule two tx band events
25/02/13   cja       CR390390 Slam to TS0 for G2W handover.
10/02/13   ip        CR448394
07/02/13   ap        CR449348 DSDS W/G+G IRAT MO Voice: HO from W2G requires attention
02/02/13   ip        CR444436
31/01/13   ip        CR443748
22/01/13   cja       CR443834 Extend mdsp_awake to indicate FW enable/disable
16/01/13   gj        CR415176: Add support for L2G SRVCC (CS handover)
12/01/13   xz        CR439646 Feedback MPH_POWER_SCAN_CNF directly when IRAT Measurement is handling.
07/01/13   cgc       CR438203 fix Dime merge error for CR398370, line omitted.
21/12/12   cja       CR423646 Store initial acq XO value for use by MPH_SELECT_SPECIFIC_BCCH_REQ
12/11/12   cgc       CR415620 Inhibit LTE initialisation in xfer mode in l1_sc_irat_check_active() TRUE
18/12/12   cgc       CR398370 Allow wtog_bplmn suspend to complete before actioning l1_get_gsm_stop_req_in_queue()
06/12/12   cja       CR383165 Remove compiler warnings
12/09/12   am        CR397448 Fix the regression caused by CR367081
19/06/12   jj        CR367081 Ignoring RR messages once received MPH_STOP_GSM_MODE_REQ
                     until next state change
07/11/12   cja       CR390390 Slam to TS0 for G2W handover.
22/10/12   og        CR412742: For T2G only, remove forced blind HO when qbit_lag>4990
14/09/12   ip        CR384421 Disable GSM sleep during CCO activity
12/09/12   gj        CR398910 Call l1_set_drx_interval to suspend BPLMN search.
05/09/12   cja       CR394799 Call rfm_power_clock_off when suspending.
04/09/12   og        CR395649. T2G Acquisition and BSIC decode enhancements.
13/07/12   gj        CR365359 Suspend and Resume of W2G BPLMN search
22/06/12   cja       CR357490 Correct FN lag for X2G HO with qbit lag = 0
22/06/12   ip        CR362050 - Set tx state to DRX_TX_ON_ALWAYS when comming from NULL
20/06/12   cja       Fix compiler warning
25/05/12   pg        Add support for RF Task
01/05/12   og        CR357437. Ensure the 625 QS is not subtracted from the cell
                     timing during T2G non-blind handover.
02/04/12   cgc       CR338501 From 9K: BPLMN l1_bplmn_suspend_in_progress and MPH_STOP_GSM_MODE_REQ transition error.
19/03/11   rc        CR:344697 Featurized code under FEATURE_MODEM_NO_VOICE_SUPPORT to support no voice builds
12/03/12   jj        CR341666 Gl1 to handle MPH_DECODE_BCCH_LIST_ABORT_REQ in NULL mode
24/02/12   pg        CR338211: Modification to CR331722 to account for variable MVS
                     execution timings
25/05/12   pg        Add support for RF Task
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
19/01/12   pg        CR331722 Ensure GL1 maintains vocoder control throughout HO
02/02/12   cja       CR329327 Ensure mdsp init complete before using command buffer
04/12/11   cgc       CR320281 check for sub_state == L1_IDLE_INIT transition during deactivate.
04/10/11   cja       CR310265 Disable WFW after suspend WFW
12/09/11   cja       CR305705  Remove FORCE_BLIND_HANDOVER definition
09/09/11   cja       CR303806  Ensure G2X disable on suspend and inactive
05/09/11   cja       CR303937  Disable FW and RxLM/TxLM for BPLMN suspend
31/08/11   og        Ensure the txlm configuration update is sent to GFW during
                     W2G handover and force blind handover.
30/08/11   cja       CR301101 In W2G BPLMN mode deallocate RxLM and disable GFW app if ISR deactivated
25/08/11   og        Disable the GFW APP when BPLMN is suspended. CR300303.
22/07/11   ap        CR297650 DSDS Develop the watchdog deactivate functionality for Dual Sim
25/07/11   ap        CR297559 DSDS: When SYNC sim is de-allocated in WtoG BPLMN, ASYNC sim is not promoted
20/07/11   dp        CR291166. Ensure the sleep command is sent to WFW when the
                     search is done.
30/06/11   sjw       CR288650 - ensure corrected FN is used for compressed mode handover in Single Standby
28/06/11   dv        CR290908 - Audio Mute after G->W->G HO
16/06/11   ap        CR292170 DSDS:Unallocate SIM when STOP_GSM_MODE_REQ received whilst W2G BPLMN is suspended
03/05/11   cja       Nikel - only send G2L init when L neighbours
28/04/11   cja       Fix compiler warning
29/03/11   og        CR280195. Ensure LTE L1 G2L is deinitialised when suspending.
08/03/11   cja       Initial IRAT for Nikel
27/01/11   cja       Changes for RxLM and TxLM for NikeL
04/02/11   cs        Fix compile issue introduced by 246818 changes and fic DSDS DEACT
27/01/11   dmb       The following RR<->L1 messages are removed (not used):
                     MPH_ADD_CHANNEL_ASSIGN_REQ/CNF,MPH_STOP_CAMPING_REQ,MPH_STOP_READ_SYS_INFO_REQ
24/01/11   cja       CR271249. Start SC on interrat HO, so NCells updated correctly.
24/01/11   cs        CR264682 DSDS Allow disabling of SAIC F/W processing in idle
21/01/11   cs        CR246418 DSDS Allow disabling of SAIC F/W processing in idle
2/12/10    og        Ensure LTE L1 is deinitialised when GSM is deactivated.
02/12/10   nf        9K changed the way clk delays and clk switching is handled
18/11/10   nf        9K added handling for clock speed based on efs value
03/11/10   cja       CR262197 In W2G handover set to dedicated mode before updating NCells (so old serving cell gets updated)
02/12/10   nf        9K changed the way clk delays and clk switching is handled
18/11/10   nf        9K added handling for clock speed based on efs value
03/11/10   cja       CR262197 In W2G handover set to dedicated mode before updating NCells (so old serving cell gets updated)
22/04/09   ws        CR 235527 - Don't process ISR_SCAN_CNF if L1_frequency_list ptr
                     is NULL
21/07/10   cs        CR246418 Allow disabling of SAIC F/W processing in idle
25/06/10   ap        CR244291 - Prevent frequency list memory freeing with FEATURE_EOOS
08/05/10   nt        CR221084 Introduction of FEATURE_GSM_FEMTO_SEARCH
15/04/10   dmb       Add the ability to force SI3-only parallel BCCH decodes
25/02/10   ip        CR227540 Add NULL pointer check for Mem Heap alloc/dealloc
08/02/10   cja       XO Update
09/10/09   dv        CR188820 - change of name for NAS API
23/09/09   dv        Featurise fix for CR188820
16/09/09   ps        Resolve Klocwork errors
14/09/09   cs        Lint indent fixes
28/08/09   dv        CR188820 - Voice heard before MT call connected after alerting
11/07/09   ws        Added FEATURE_MODEM_HEAP around GPRS_MEM_FREE
06/06/09   ab        Feature FEATURE_GSM_EGPRS_IR_OPT for LCU
29/05/09   ps        Zero Initialise memory relocation to Heap for selected data structure
30/03/09   ws        Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                     to replace FEATURE_GSM_GPRS_MUSTANG
03/03/09   agv       Mustang fix for BPLMN when in G2W GSM active RAT.
26/02/08   ws        CR 173554 - Ensure AGC is primed when either no irat measuremts are
                     present or the target cell can't be found in WL1 irat list
23/12/08   agv       CR165531 - Possible hole in iRAT Handover Tx Power setting.
08/12/08   og        Mustang G2W IRAT integration.
                     SCM mod to not call g2w_srch_sleep_wfw() in gsm only mode.
04/12/08   og        Mustang G2W IRAT integration.
                     SCM Call g2w_srch_sleep_wfw() when GSM stopped.
01/12/08   cs        Add new split gl1_hw_clk_ctl.h from gl1_hw.h
28/07/98   hv        Added KxMutex support
16/06/08   nt        CR147984:- Changes for TCXO reset.
20/03/08   agv       Fixed the LCU PMIC Band setup for a Channel Change.
19/02/08   og        Handle the case of power scan request containing a zero list.
14/01/08   cs        Allow for separate IQ logging and saving
06/11/07   sjw       CR115620 Removed use of FEATURE_INTER_RAT_HO_OPT around
                     gl1_msg_disable_l1_vocoder_control(), required for Worldphone targets
26/10/07   cs        Fix IQ logging include
17/09/07   agv       Added I/Q logging to code base.
17/09/07   agv       CR127820 - iRAT W2G handover AGC setting.
06/07/07   og        Allow W to inform G about the frequency offset of the G cell
                     in W2G non-blind handover scenarios. CR122524.
21/06/07   og        Allow initialisation of the target GSM ncell gain setting using
                     the W side interrat measurements. CR123141.
21/06/07   og        Allow the agc and rx power info of ncell to be primed with values
                     measured during dedic mode tch in G2G blind handovers. CR123136
04/07/07   dv        CR122359:Correct FN_lag error for W2G HO
11/06/07   cs        Fix the FTM sweep test TCXO non-release problem
16/05/07   ws        Added FEATURE_GSM_L1_CONTROL_SET_BAND CR 114613
10/05/07   cs        Aries GPS changes for frequency error update
02/03/07   cs        Add support for new CGPS timetag
18/10/06   av        Added TCXO MGR 3.0 for 7600.
12/10/06   og        Merge fix to CR101667 / SR860781. Ensure the correct freq err
                     is applied before attenpting to decode BCCH after pscan.
14/08/06   ws        Allow dedicated to be timeslot slammed to lowest received tn
                     and also to place the CS timeslot on TN 2
14/06/06   ws        Lock interrupts when handling stop gsm mode req. Resolves
                     CR95811 / SR833323.
13/06/06   nt        CR94517: Cleared out the starting time upon a reconnect reqest as the
                     code sometimes uses it even if the valid flag is false.
23/05/06   ws        Increase speed to 122 Mhz if we support DTM CR 91655
15/05/06   nt        CR91329 - Added flag to indicate if the previous starting time
                     on the old assignment had expired, upon returning to old channel
                     configuration with the reconnect_channel_req.
18/04/06   og        Initialise the Idle camped cell AGC information when performing
                     W2G inter-RAT handover. Partially addresses CR 91655.
01/17/06   sv        Remove the F3's that cause the delayed PSCAN_CNF to RR thus causing
                     a lockup in WtoG BPLMN search.
12/14/05   sv        Support the changed prototype for MPH_DECODE_BCCH_LIST_CNF.
12/12/05   sv        Check whether L1 is already suspending when processing
                     MPH_STOP_GSM_MODE_REQ; no need to suspend twice.
11/15/05   sv        Lint cleanup.
11/07/05   sv        Add support for FEATURE_WTOG_BACKGROUND_PLMN_SEARCH.
11/07/05   sv        Reset BPLMN state when deactivating GSM.
10/06/05   sv        Start the SCE when RR sends the MPH_RESUME_GSM_MODE_REQ.
09/08/05   dp        Moved enabling L1 vocoder control to just before we kick
                     off dedicated mode to allow time to disable vocoder
08/10/05   dp        Fix to set the TA to 0 for synchronized W2G HOs
08/04/05   sv        Fixed compile warning.
06/16/05   sv        Fixed lint error due to header file include.
05/25/05   gw        Added debug message when deactivating.
04/12/05   dp        Release GSM client as a safety precaution during an
                     inter RAT handover
03/29/05   dp        InterRAT audio optimizations
03/16/05   pp        Change to support modified prototype for gl1_load_current_cell_data()
02/25/05   pp        Added support for GSM to GSM Blind Handovers
02/09/05   bm        Power scan optimizations
02/11/04   bm        Support for Background WCDMA PLMN Scan
01/05/05   dp        Merged dedicated timeline changes for audio optimizations
                     to mainline
11/01/04   bm        Send MPH_RECONNECT_CHANNEL_CNF after starting SCE, to be able
                     to handle DED_WCDMA_UPDATE_LIST_REQ
06/12/04   gfr       Control slew rate limiting in PL1.
07/01/04   bm        Init surround cell data upon MPH_RECONNECT_CHANNEL_REQ
06/07/04   gw        Allow reselection pcan to do more than 1 meas per ARFCN.
06/01/04   bm        Check to see BSIC matches before retrieving interrat measurement
06/04/04   sv        Removed all references to XXX_mframe_power_XXX.
06/02/04   gw        Do fewer measurements for reselection powerscan.
05/26/04   sv        Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters for better
                     code readability.
04/05/04   dp        Fix to handle a null decode bcch list and just return
                     a decode bcch list cnf to RR
03/29/04   bm        Fix for Reconnect_Req failure during G2W HO
03/26/04   gfr       Time tagging support.
03/22/04   gfr       Support for ISR watchdog.
02/25/04   gw        Lint clean up.
02/09/04   dp        Added support for background HPLMN searching
11/10/03   gw        Added check for zero length AFRCN list in L1_setup_PSCAN().
11/05/03   gfr       Support for quad-band.
11/05/03   gsc       Added support for handling multirate IE in the handover
                     request.
10/19/03   jc        Remove hardcoded number 138, fix for T3124 expiration.
09/19/03   jc        Display BSIC as hex.
07/11/03   gfr       Lint cleanup.
06/06/03   tb        Support for dedicated mode gtow searching.
05/14/03   gfr       Common power handler support.
05/12/03   ATM       Fixed last 3% of failed CMHO with empirical fix to GFN
05/12/03   ATM       Moved Frame Number management entirely inside GTMRS
05/02/03   gw        Merged in power transfer changes from branch.
04/28/03   tb        Support for MPH_UE_MODE_CHANGE_REQ from RR.
04/04/03   gw        Added calls to l1_sc_init() to ensure ncell info is reset
                     on all transitions out of Null Mode.
03/07/03   tb        Modifications to handle GtoW Handover failure (reconnect).
02/27/03   jc        Move DEBUG_FORCE_BLIND_HANDOVER definition to cust file.
02/21/03   gw        Added function L1_setup_DEACTIVATE to support deep sleep.
02/20/03   dp        Moved fn_lag computation for compressed mode handovers
                     to ISR.  Handled corner case when qbit_lag < 625
01/17/02   dp        Moved gl1_msg_cfg_power_measure() for power scan to
                     l1_pscan to fix "Idle abort" errors
11/06/02   ATM       Added ciphering to interrat handover
                     also moved common handover code to l1_utils
11/06/02   ATM       Bug:Fixes to CM time calculation
11/04/02   ATM       Added more debug for CM handovers
11/03/02   tb        Changes to support INTERRAT_GTOW
10/22/02   ATM       Changed interrat_meas reads to match new RR struct
10/07/02   dp        Added support for fully parallel decodes
10/02/02   ATM       Added compressed mode interRAT handover functionality
10/01/02   ATM       Removed compiler warnings
09/17/02   plm       Moved assignment out of if statement in function
                     L1_setup_DECODE_BCCH_LIST() to eliminate compiler warning.
09/08/02   ATM       Bug:Changed interface for acq
08/27/02   ATM       Bug:Curren/Target cell details reversed
08/22/02   dp        Fix to ignore MPH_STOP_CAMPING_REQ in null mode
08/14/02   ATM       Copied l1_dedicated_data into -)p
08/13/02   dp        Support to handle MPH_DECODE_BCCH_LIST_REQ
08/12/02   gw        Changed RR-L1 i/f for pscan as part of cell selection /
                     re-selection optimizations.
07/10/02   ATM       Added blind-handover support.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include <string.h>         /* memcpy() */

#include "comdef.h"
#include "gprs_mem.h"
#include "gl1_sys_algo.h"
#include "gl1_sys_params.h"
#include "l1_task.h"
#include "l1_utils.h"
#include "ms.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "sys_stru.h"
#include "l1_log.h"
#include "l1_isr.h"
#include "msg.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_debug.h"
#include "l1i.h"
#include "l1_sc.h"
#include "l1_drx.h"
#include "gpl1_dual_sim.h"
#include "geran_dual_sim_g.h"
#include <stringl/stringl.h>
#include "geran_dual_sim.h"
#include "l1_sc_irat.h"
#include "l1_smscb.h"
#include "gl1_asdiv.h"

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
#include "gl1_msg_g.h"
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES || FEATURE_GSM_LOG_IQ_SAMPLES */


#include "tcxomgr.h"
#include "gl1_hw_tcxo_mgr3.h"

#include "gfw_sw_intf.h"
#include "fws.h"
#include "mdsp_intf.h"


#include "gl1_hw_clk_ctl.h"
#include "gl1_hw_clk_ctl_g.h"
#include "gl1_voice_server.h"

#ifdef FEATURE_WLAN_COEX_SW_CXM
#include "gl1_arbitrator_cxm.h"
#endif

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif 
#include "mcfg_nv_api.h"


extern gl1_hw_cm_status_t         gl1_hw_cm_status[];

extern byte bplmn_idle_band_bitmap[NUM_GERAN_DATA_SPACES];

void    L1_setup_blind_ho_ACQ( ARFCN_T  arfcn,
                               const sys_algo_agc_T*  agc_ptr, gas_id_t gas_id );

boolean gl1_is_interrat_handover_msg_valid (mph_handover_req_T *message_ptr);
boolean gl1_is_interrat_handover_blind     (mph_handover_req_T *message_ptr);
void    gl1_store_interrat_handover_details(mph_handover_req_T *message_ptr);
void    gl1_begin_INTERRAT_Handover        ( gas_id_t gas_id );

/* from l1_ded.c */
extern void L1_sync_to_target(l1_sc_bs_T *current_cell,l1_sc_bs_T *target_cell, gas_id_t gas_id);

/* from l1_ded_if.c */
extern void gl1_load_current_cell_data( gas_id_t gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
sys_algo_agc_T* l1_ds_get_w2g_blind_handover_agc(gas_id_t gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

boolean rr_send_stop_gsm_mode_req[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };

extern dedicated_data_T l1_dedicated_data[];

typedef struct
{
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
boolean l1_gsm_stop_req_in_queue;
#endif
sys_algo_agc_T  l1_w2g_blind_handover_agc;
}l1_null_if_data_t;

/* BCCH carrier list for parallel decodes */
BCCH_List_T l1_BCCH_List[NUM_GERAN_DATA_SPACES];

static l1_null_if_data_t  l1_null_if_data[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
boolean l1_get_gsm_stop_req_in_queue( gas_id_t gas_id )
{
  l1_null_if_data_t *l1_null_if_data_ptr = &l1_null_if_data[gas_id];
  return l1_null_if_data_ptr->l1_gsm_stop_req_in_queue;
}
void l1_set_gsm_stop_req_in_queue(boolean l1_gsm_stop_req_in_queue, gas_id_t gas_id)
{
  l1_null_if_data_t *l1_null_if_data_ptr = &l1_null_if_data[gas_id];

  l1_null_if_data_ptr->l1_gsm_stop_req_in_queue=l1_gsm_stop_req_in_queue;
}
#endif

extern void l1_pscan_reset_freq_completed(gas_id_t gas_id);
extern boolean gl1_acq_asdiv_band_supported(ARFCN_T arfcn,gas_id_t gas_id);

/*===========================================================================

FUNCTION  L1_HANDLE_NULL_MODE

DESCRIPTION
  This function handles the L1 Task in null mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_handle_null_mode(IMH_T  *msg_header)

{
   gas_id_t gas_id;
   /* code start */
   if (msg_header->message_set == MS_RR_L1)
   {
     switch (msg_header->message_id)
     {

      /* Enable Sleep indication */
      case MPH_ENABLE_SLEEP_IND:
    {
    L1_handle_ENABLE_SLEEP_IND ( msg_header );
        break;
      }

     /********************************************************
      *                                                      *
      *                MPH_POWER_SCAN_REQ                    *
      *                                                      *
      ********************************************************/
      case MPH_POWER_SCAN_REQ:
      {
         mph_power_scan_req_T *message_ptr;
         message_ptr = (mph_power_scan_req_T *)msg_header;
         gas_id = check_gas_id(message_ptr->gas_id);
         if( FALSE == L1_setup_PSCAN( msg_header ) )
         {
           /* (Re)-Initialize the SCE */
           l1_sc_init(gas_id);

           GL1_ISR_LOCK(gas_id);
           /* Command the  the ISR to start measuring */
           L1_send_ISRTIM_SCAN_REQ(L1_FIND_BCCH_COMMAND,gas_id);
           GL1_ISR_UNLOCK(gas_id);
         }
         else
         {
           /* Something is not right about the power scan request.
            * Send back a confirmation to avoid any upper layer
            * timeouts.
            */
           L1_send_MPH_POWER_SCAN_CNF( FALSE,gas_id ); /* suspending boolean */
         }
         break;
      }

      /********************************************************
       *                                                      *
       *          MPH_SELECT_SPECIFIC_BCCH_REQ                *
       *                                                      *
       ********************************************************/
      case MPH_SELECT_SPECIFIC_BCCH_REQ:
      {
         mph_select_specific_bcch_req_T *message_ptr;
         volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
         uint8 bsic_requested;

         sys_band_mask_type             band_mask;
         /* align pointers */
         message_ptr = (mph_select_specific_bcch_req_T *)msg_header; /*lint !e740 unusual pointer cast */
         gas_id = check_gas_id(message_ptr->gas_id);
         l1_tskisr_blk = &l1_tsk_buffer[gas_id];

         bsic_requested = (message_ptr->bsic.PLMN_colour_code << 3) | message_ptr->bsic.BS_colour_code;

         /* If we are about to camp on a 1800 band cell ensure we are running at
          * low clock speed, otherwise set some other band to clock at high speed
          */
         if(message_ptr->specific_channel_no.band == SYS_BAND_DCS_1800)
         {
           band_mask = SYS_BAND_MASK_GSM_DCS_1800;
         }else
         {
           band_mask = SYS_BAND_MASK_GSM_850;
         }

         gl1_set_band_group(band_mask, gas_id);


         /* (Re)-Initialize the SCE */
         l1_sc_init(gas_id);

         GL1_ISR_LOCK(gas_id);
         l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
         L1_send_ISRTIM_ACQ_REQ(message_ptr->specific_channel_no, gas_id);
         l1_tskisr_blk->next_params.bsic_expected = bsic_requested;
         MSG_GERAN_MED_2_G("SpecificBCCH Req with ARFCN:%d, BSIC %2x",message_ptr->specific_channel_no.num, bsic_requested);
         GL1_ISR_UNLOCK(gas_id);

#ifdef FEATURE_GSM_COEX_SW_CXM
         garb_intf_notify_find_bcch_update_arfcn(message_ptr->specific_channel_no,gas_id);
         garb_intf_notify_find_bcch_update_rx_power(message_ptr->specific_channel_no,
              (int32)(l1_tskisr_blk->next_params.L1Data.pAgc_data->pwr_dBm_x16),gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

         break;
      }


      /********************************************************
       *                                                      *
       *            MPH_DECODE_BCCH_LIST_REQ
       *                                                      *
       ********************************************************/
       case MPH_DECODE_BCCH_LIST_REQ:
       {
          mph_decode_bcch_list_req_T  *message_ptr;
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
          BCCH_List_T *l1_BCCH_List_ptr;

          /* Align pointers */
          message_ptr = ( mph_decode_bcch_list_req_T *)msg_header; /*lint !e740 unusual pointer cast */
          gas_id = check_gas_id(message_ptr->gas_id);
          l1_tskisr_blk = &l1_tsk_buffer[gas_id];
          l1_BCCH_List_ptr = &l1_BCCH_List[gas_id];

          /* If RR sends a null list, just send back a cnf and do nothing */
          if ( message_ptr->frequency_list.num_frequencies == 0 )
          {
             MSG_GERAN_ERROR_0_G("RR->L1 BCCH decode list empty");
             /* Send CNF(ERROR) back to RR */
             L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE,gas_id);
          }
          else
          {
             L1_setup_DECODE_BCCH_LIST( msg_header );

             /* (Re)-Initialize the SCE */
             l1_sc_init(gas_id);

             GL1_ISR_LOCK(gas_id);
             l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
             l1_tskisr_blk->command = L1_ACQUIRE_BCCH_LIST_COMMAND;
             L1_send_ISRTIM_ACQ_LIST_REQ( l1_BCCH_List_ptr->current_sch_index,gas_id );
            GL1_ISR_UNLOCK(gas_id);
          }
          break;
       }

       /********************************************************
        *                                                      *
       *                MPH_HANDOVER_REQ                      *
       *                                                      *
       ********************************************************/
      case MPH_HANDOVER_REQ:
      {
         /* InterRAT handover */
         mph_handover_req_T  *message_ptr = (mph_handover_req_T *)msg_header; /*lint !e740 unusual pointer cast */
         volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
         l1_null_if_data_t *l1_null_if_data_ptr;

         ARFCN_T arfcn;

         gas_id = check_gas_id(message_ptr->gas_id);
         l1_tskisr_blk = &l1_tsk_buffer[gas_id];
         l1_null_if_data_ptr = &l1_null_if_data[gas_id];
		 
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
         gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_IRAT_HO);
#endif

#ifdef FEATURE_INTER_RAT_HO_OPT
         /* Notify Voice Agent that GSM will need acces to the vocoder */
         l1_vs_request_vocoder_ownership(gas_id);
#endif

         /* (Re)-Initialize the SCE */
         l1_sc_init(gas_id);

         arfcn =  message_ptr->BCCH_ARFCN;

         MSG_GERAN_HIGH_1_G("MPH_HANDOVER_REQ to GSM Cell %d ",arfcn.num);

         /*Check if handover is possible - validate timing advance */
         if ( ! gl1_is_interrat_handover_msg_valid(message_ptr) )
         {
             L1_send_MPH_HANDOVER_IND( HANDOVER_OUT_OF_RANGE, gas_id );
             break; // Stay here -- wait for GSM_STOP_REQ from RR
         }

         gl1_store_interrat_handover_details(message_ptr);
#ifdef FEATURE_WLAN_COEX_SW_CXM
         garb_cxm_high_priority_event_ind(GSTMR_GET_FN_GERAN(gas_id),GL1_DEFS_INVALID_FN,CXM_HIGH_PRIO_HO,gas_id);
#endif

         if( gl1_is_interrat_handover_blind(message_ptr) )
         {    /* BLIND CASE */
             MSG_GERAN_HIGH_0_G("Performing Blind Handover ");

             L1_setup_blind_ho_ACQ( arfcn,
                                    &(l1_null_if_data_ptr->l1_w2g_blind_handover_agc), gas_id ); /* list of one freq */

             GL1_ISR_LOCK(gas_id);
             l1_tskisr_blk->main_command = L1_GET_TIMING_COMMAND;
             L1_send_ISRTIM_ACQ_REQ(arfcn, gas_id);
             GL1_ISR_UNLOCK(gas_id);
         }
         else
         {   /* COMPRESSED MODE CASE */
             MSG_GERAN_HIGH_0_G("Performing Compressed Mode Handover ");
             GL1_ISR_LOCK(gas_id);
             l1_tskisr_blk->main_command = L1_CM_HANDOVER_COMMAND;
             GL1_ISR_UNLOCK(gas_id);
         }
         break;
      }

      /********************************************************
       *                                                         *
       *                MPH_RECONNECT_CHANNEL_REQ                *
       *                                                         *
       ********************************************************/
      case MPH_RECONNECT_CHANNEL_REQ:
      {
         /* GtoW handover failed */
         mph_reconnect_channel_req_T  *message_ptr;
         volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
         dedicated_data_T *l1_dedicated_data_ptr;

         message_ptr = (mph_reconnect_channel_req_T *)msg_header; /*lint !e740 unusual pointer cast */
         gas_id = check_gas_id(message_ptr->gas_id);
         l1_tskisr_blk = &l1_tsk_buffer[gas_id];
         l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

         MSG_GERAN_HIGH_0_G("MPH_RECONNECT_CHANNEL_REQ to GSM Cell");

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
         gl1_msg_set_iq_state( FALSE, gas_id );
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES || FEATURE_GSM_LOG_IQ_SAMPLES */

#ifdef FEATURE_INTER_RAT_HO_OPT
         /* Notify Voice Agent that GSM will need acces to the vocoder */
         l1_vs_request_vocoder_ownership(gas_id);
#endif

         l1_sc_init(gas_id);

         /* Update Dedicated params to that of target cell*/
         l1_dedicated_data_ptr->channel_specification
             = message_ptr->channel_spec;
         l1_dedicated_data_ptr->starting_time
             = (message_ptr->channel_spec.starting_time[0]<<8)
              + message_ptr->channel_spec.starting_time[1];

         /*
         If when previously on this cell the starting time had expired then
         invalidate this starting time here. The value of the starting time must also
         be cleared as this is sometimes used without the check for starting_time_valid
         in l1_ded.c
         */
         if ( l1_dedicated_data_ptr->previous_cell_on_after_channels == TRUE )
         {
           l1_dedicated_data_ptr->channel_specification.starting_time_valid = FALSE;
           l1_dedicated_data_ptr->starting_time = 0;
         }

         gl1_load_current_cell_data(gas_id);

         l1_dedicated_data_ptr->no_of_ab_to_send = 0;
         GL1_ISR_LOCK(gas_id);
         gl1_msg_abort_rach(gas_id);
         l1_tskisr_blk->main_command = L1_DEDICATED_COMMAND;
         GL1_ISR_UNLOCK(gas_id);
         break;
      }

      /********************************************************
       *                                                      *
       *                MPH_UE_MODE_CHANGE_REQ                *
       *                                                      *
       ********************************************************/

      case MPH_UE_MODE_CHANGE_REQ:
      {
         mph_ue_mode_change_req_T  *message_ptr;
         message_ptr = ( mph_ue_mode_change_req_T *) msg_header; /*lint !e740 unusual pointer cast */
         gas_id = check_gas_id(message_ptr->gas_id);

         l1_sc_wcdma_set_mode(message_ptr->gsm_only,gas_id);
         break;
      }

      /********************************************************
       *                                                      *
       *              MPH_STOP_GSM_MODE_REQ                   *
       *                                                      *
       ********************************************************/
      case MPH_STOP_GSM_MODE_REQ:
      {
         /* Stop L1 ISR */
         L1_setup_DEACTIVATE( msg_header );
         break;
      }

      case MPH_DECODE_BCCH_LIST_ABORT_REQ:
      {
        mph_decode_bcch_list_abort_req_T* message_ptr = ( mph_decode_bcch_list_abort_req_T* )msg_header;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        gas_id = check_gas_id(message_ptr->gas_id);

        GL1_ISR_LOCK(gas_id);

        l1_tskisr_blk = &l1_tsk_buffer[gas_id];
        l1_tskisr_blk->main_command = L1_NULL_COMMAND;
        /*
        As a decode bcch list cnf is to be sent here.
        Clear the confirm_ID
        */

        l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;

        GL1_ISR_UNLOCK(gas_id);
        L1_send_MPH_DECODE_BCCH_LIST_CNF(TRUE,gas_id);
        MSG_GERAN_HIGH_0_G( "Handles MPH_DECODE_BCCH_LIST_ABORT_REQ in null mode");
        break;
      }

      default:
      {
        MSG_GERAN_ERROR_1("RR->L1(NULL) message %d ignored.",
                   msg_header->message_id);
      }
     }/*end switch msgid */
   }/*end if MS_RR_L1*/
   else
   {
      MSG_GERAN_ERROR_2("L1(NULL) - Unknown message <%d,%d>.",
                msg_header->message_set,msg_header->message_id);
   }

}

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
/*===========================================================================

FUNCTION  L1_handle_suspend_mode

DESCRIPTION
  This function handles the L1 Task in suspended mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_handle_suspend_mode(IMH_T  *msg_header)

{
   gas_id_t gas_id;

   if (msg_header->message_set == MS_RR_L1)
   {
     switch (msg_header->message_id)
     {

      /* Enable Sleep indication */
    case MPH_ENABLE_SLEEP_IND:
    {
    L1_handle_ENABLE_SLEEP_IND ( msg_header );
        break;
      }

      case MPH_STOP_GSM_MODE_REQ:
      {
        mph_stop_gsm_mode_req_T* message_ptr = ( mph_stop_gsm_mode_req_T* )msg_header;
		volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        gas_id = check_gas_id(message_ptr->gas_id);
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        l1_tsk_buffer[gas_id].g2x_resel_in_progress = message_ptr->g2x_resel_in_progress;
        MSG_GERAN_HIGH_1_G("L1_handle_suspend_mode: MPH_STOP_MODE_REQ, g2x_resel_in_progress:%d",
                           l1_tsk_buffer[gas_id].g2x_resel_in_progress);
#endif
         /* ISR is not running in this case, so, change the state to INACTIVE
          * and SRAT_GSM_MODE_REQ will resume the action
          */
		 GL1_ISR_LOCK(gas_id);
		 if( l1_tskisr_blk->main_command == L1_IDLE_COMMAND)
		 {
		   l1_tskisr_blk->main_command = L1_NULL_COMMAND;  
		   l1_sc_stop(gas_id);
		   gl1_hw_inact_task(FALSE,FALSE,gas_id);	   		   
		 }
		 GL1_ISR_UNLOCK(gas_id);
         l1_change_state(L1_INACTIVE_MODE,gas_id);
         L1isr_send_MPH_STOP_GSM_MODE_CNF (gas_id);
 #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
         gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_G2W_BPLMN_SRCH_END);
#endif
         break;
      }

      case MPH_RESUME_GSM_MODE_REQ:
      {
         mph_resume_gsm_mode_req_T* message_ptr = ( mph_resume_gsm_mode_req_T* )msg_header;
         volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

         gas_id = check_gas_id(message_ptr->gas_id);
         l1_tskisr_blk = &l1_tsk_buffer[gas_id];

         MSG_GERAN_ERROR_0_G("MPH_RESUME_GSM_MODE_REQ to GSM Cell");
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
         gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_G2W_BPLMN_SRCH_RESUME);
#endif

         L1_resume_req(gas_id);

         l1_sc_start(L1SCModeIdle, gas_id);

         GL1_ISR_LOCK(gas_id);
         l1_tskisr_blk->main_command = L1_IDLE_COMMAND;
         GL1_ISR_UNLOCK(gas_id);

 #ifdef  FEATURE_GSM_TO_LTE
         if( FALSE == gl1_hw_get_gsm_only_mode(gas_id) )
         {
           l1_sc_irat_check_active(1, 1, FALSE, 1, FALSE, gas_id);
         }
#endif
         break;
      }

      default:
         MSG_GERAN_ERROR_1("RR message=%d ignored",msg_header->message_id);
         break;
     }
   }
   else if (msg_header->message_set == MS_L1_L1)
   {
       switch  (msg_header->message_id )
       {
          case ISR_SUSPEND_CNF:
          {
            isrtim_suspend_cnf_T *message_ptr;

             /* align pointers */
             message_ptr = (isrtim_suspend_cnf_T *)msg_header; /*lint !e740 */
             gas_id = check_gas_id(message_ptr->gas_id);

               MSG_GERAN_HIGH_0_G("Suspending L1 task");

             /* Force G2X shutdown */
             l1_sc_irat_check_active(0, 0, FALSE, 0, FALSE, gas_id);

              /* Check if the G2X acivity is on then wait for one frame before sedding suspend_cnf to GRR*/
             if ( l1_sc_check_irat_active(gas_id) )
             {
                MSG_GERAN_HIGH_0_G("G2X IRAT activiy is on, wait for one frame before sending suspend_cnf");
                gl1_drx_require_and_wait_for_next_tick(gas_id);
             }

             gl1_hw_inact_task( FALSE, TRUE, gas_id );

             L1_send_MPH_SUSPEND_GSM_MODE_CNF((message_ptr->timer_value), TRUE, gas_id);
          }
          break;

          default:
             break;
       }
   }
   else if ( msg_header->message_set == MS_CB_L1)
   {
    if (msg_header->message_id == CB_SCHEDULING_REQ) /*lint !e641 */
    {
      cb_scheduling_req_type *message_ptr;
      message_ptr = (cb_scheduling_req_type *) msg_header; /*lint !e740 */

      l1_smscb_process_scheduling_info
        ( message_ptr->channel_ind,
        message_ptr->scheduling_mode,
        message_ptr->scheduling_descriptor.scheduling_period_fn_list,
        message_ptr->scheduling_descriptor.scheduling_period_length,
        geran_map_nas_id_to_gas_id(message_ptr->as_id)
        );
    }
    else if (msg_header->message_id == CB_SKIP_REQ)  /*lint !e641 */
    {
      cb_skip_req_type *message_ptr;
      message_ptr = (cb_skip_req_type *) msg_header; /*lint !e740 */

      l1_smscb_process_skip_info
        ( message_ptr->channel_ind,
        message_ptr->scheduling_descriptor.scheduling_period_fn_list,
        message_ptr->scheduling_descriptor.scheduling_period_length,
        message_ptr->skip_ind,
        geran_map_nas_id_to_gas_id(message_ptr->as_id)
        );
    }
    else if (msg_header->message_id == CB_DEACTIVATE_REQ )  /*lint !e641 */
    {
      cb_deactivate_req_type *message_ptr;
      message_ptr = (cb_deactivate_req_type *) msg_header; /*lint !e740 */

      l1_smscb_process_deactivate_info( message_ptr->channel_ind, geran_map_nas_id_to_gas_id(message_ptr->as_id) );
    }
   }
   else
   {
       MSG_GERAN_ERROR_1("Messages from Layer=%d ignored",msg_header->message_set);
   }
}
#endif

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
/*===========================================================================

FUNCTION  L1_handle_WTOG_BPLMN_mode

DESCRIPTION
  This function handles the L1 Task in suspended mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_handle_WTOG_BPLMN_mode(IMH_T  *msg_header)
{
   gas_id_t gas_id;

   if (msg_header->message_set == MS_RR_L1)
   {
     switch (msg_header->message_id)
     {

      /* Enable Sleep indication */
    case MPH_ENABLE_SLEEP_IND:
    {
    L1_handle_ENABLE_SLEEP_IND ( msg_header );
        break;
      }

      case MPH_STOP_GSM_MODE_REQ:
      {
        boolean deep_sleep = FALSE;
        mph_stop_gsm_mode_req_T* message_ptr = ( mph_stop_gsm_mode_req_T* )msg_header;
        l1_null_if_data_t *l1_null_if_data_ptr;
        volatile ISRTIM_CMD_BLK *l1_tskisr_blk;

      if(message_ptr-> rr_l1_gsm_stop_reason == RR_L1_DEEP_SLEEP)
      {
          deep_sleep =TRUE;
      }

         /* Used for INTLOCK / INTFREE */

         gas_id = check_gas_id(message_ptr->gas_id);
         l1_null_if_data_ptr = &l1_null_if_data[gas_id];
         l1_tskisr_blk = &l1_tsk_buffer[gas_id];

         MSG_GERAN_HIGH_0_G("MPH_STOP_MODE_REQ in WTOG_BPLMN mode.");

         /*To avoid deadlock between task+isr lock below 
           && ISR+Task lock done in gl1_hw_resume_init
           For Single Sim mode, if a gl1_hw_resume_init is pending
           then wait for one more tick, as by this time TRM would be granted in ISR and init would be completed
           */
         if((gl1_msg_get_multi_sim_mode() == FALSE) && 
            l1_tskisr_blk->main_command == L1_RESUME_COMMAND)
         {
           gl1_drx_require_and_wait_for_next_tick_conditional(gas_id);
         }
         /*
         ** add task lock to prevent gl1_hw_cm_gsm_init() race condition, before ISR lock.
         */
         gl1_task_mutex_lock(gas_id);
         /* Lock interrupts to prevent TDMA ISR from interrupting these
          * checks.
          */
         GL1_ISR_SAVE_LOCK(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         l1_tsk_buffer[gas_id].g2x_resel_in_progress = message_ptr->g2x_resel_in_progress;
         MSG_GERAN_HIGH_1_G("L1_handle_WTOG_BPLMN_mode: MPH_STOP_MODE_REQ, g2x_resel_in_progress:%d",
                            l1_tsk_buffer[gas_id].g2x_resel_in_progress);
#endif
         /*
         ** CgC l1_bplmn_suspend_in_progress, ensure this means what it says.
         ** we have not completed the suspend until the ISR_SUSPEND_CNF has been processed.
         */
         if( (l1_get_bplmn_suspend_in_progress(gas_id) == FALSE)
           &&(l1_get_wtog_bplmn_task_substate(gas_id) == WTOG_BPLMN_SUSPENDED))
         {
           /* ISR is not running in this case, send STOP_GSM_MODE_CNF
            * change the state to INACTIVE.
            */
            l1_change_state(L1_INACTIVE_MODE,gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            /* process the deactivation of the DUAL SIM worlds */
            if ( gl1_msg_get_multi_sim_mode() )
            {
              l1_swap_multi_sim_sync_state_or_deactivate(gas_id);
            }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

            if(deep_sleep)
            {
              MSG_GERAN_HIGH_0_G( "MCPM change for GERAN Stop");
              gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_STOP_REQ, GL1_MCPM_GERAN_STOP,gas_id);
              gstmr_force_reinit(gas_id);
            }

            L1isr_send_MPH_STOP_GSM_MODE_CNF (gas_id);

            l1_isr_watchdog_deactivate(gas_id);
         }
         else
         {
            if(l1_get_bplmn_suspend_in_progress(gas_id))
            {
               l1_null_if_data_ptr->l1_gsm_stop_req_in_queue = TRUE;
            }
            else
            {
              L1_setup_DEACTIVATE( msg_header );
            }
         }

         /* Unlock them */
         GL1_ISR_SAVE_UNLOCK(gas_id);
         gl1_task_mutex_unlock(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
         gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_W2G_BPLMN_SRCH_END);
#endif
         break;
      }

      case MPH_RESUME_GSM_MODE_FOR_BPLMN_REQ:
      {
         L1_handle_MPH_RESUME_GSM_MODE_FOR_BPLMN_REQ( msg_header );
         break;
      }

      case MPH_POWER_SCAN_REQ:
      {
         mph_power_scan_req_T *message_ptr;
         volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

         message_ptr = (mph_power_scan_req_T *)msg_header;
         gas_id = check_gas_id(message_ptr->gas_id);
         l1_tskisr_blk = &l1_tsk_buffer[gas_id];

         MSG_GERAN_HIGH_0_G("MPH_POWER_SCAN_REQ to GSM Cell");

         if( FALSE == L1_setup_PSCAN( msg_header ) )
         {
           l1_set_wtog_bplmn_task_state(POWER_SCAN,gas_id);
           l1_tskisr_blk->pscan_state = L1_SCAN_START;
         }
         else
         {
           /* Something is not right about the power scan request.
            * Send back a confirmation to avoid any upper layer
            * timeouts.
            */
           L1_send_MPH_POWER_SCAN_CNF( FALSE,gas_id ); /* suspending boolean */
         }

         break;
      }

      case MPH_DECODE_BCCH_LIST_REQ:
      {
        mph_decode_bcch_list_req_T* message_ptr = ( mph_decode_bcch_list_req_T* )msg_header;
        gas_id = check_gas_id(message_ptr->gas_id);
         MSG_GERAN_HIGH_0_G("MPH_DECODE_BCCH_LIST_REQ to GSM Cell");
         GL1_ISR_LOCK(gas_id);
         L1_setup_DECODE_BCCH_LIST( msg_header );
         l1_set_wtog_bplmn_task_state(ACQ_BCCH_LIST,gas_id);
         GL1_ISR_UNLOCK(gas_id);
         break;
      }

      case MPH_SUSPEND_GSM_MODE_REQ:
      {
        mph_suspend_gsm_mode_req_T* message_ptr = ( mph_suspend_gsm_mode_req_T* )msg_header;
        gas_id = check_gas_id(message_ptr->gas_id);

        MSG_GERAN_HIGH_0_G("MPH_SUSPEND_GSM_MODE_REQ");
        GL1_ISR_LOCK(gas_id);
        l1_set_drx_interval(MIN_FRAMES_FOR_L1_TO_SUSPEND, gas_id);
        GL1_ISR_UNLOCK(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_W2G_BPLMN_SRCH_SUSPEND);
#endif	
         break;
      }

      default:
         MSG_GERAN_ERROR_1("RR message=%d ignored",msg_header->message_id);
         break;
     }
   }
   else if (msg_header->message_set == MS_L1_L1)
   {
      uint16 num_satisfy_threshold_criteria=0;
      uint16   i;

      switch (msg_header->message_id )
      {
         case ISR_SCAN_CNF:
         {
           isrtim_scan_cnf_T* message_ptr = ( isrtim_scan_cnf_T* )msg_header;
           gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
           volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
           /* Check L1_Frequency_List pointer is valid, since MPH_STOP_MODE_REQ
            * may have deallocated dynamic memory causing a potential page fault
            */
            Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];

            l1_tskisr_blk = &l1_tsk_buffer[gas_id];

            if(l1_Frequency_List == NULL)
            {
              MSG_GERAN_HIGH_0_G("l1_Frequency_List NULL!");
              break;
            }
            /* A set of measurements exist in the l1_Frequency_List. */

            /* Calculate the signal strength averages for the list ... */
            L1_normalise_signal_strengths(l1_Frequency_List->length,
                                          l1_Frequency_List->no_of_measurement,
                                          l1_Frequency_List->power_dBm_x16,
                                          l1_Frequency_List->agc
#ifdef GL1_GMSK_MOD_DET
                                          ,l1_Frequency_List->decstat
#endif                                    
                                         );
            /* ... sort it ... */
            num_satisfy_threshold_criteria = L1_sort_signal_strengths(gas_id);

            /* Bounds check to resolve Klocwork ABR errors */
            if ( num_satisfy_threshold_criteria >
                 ( sizeof( l1_Frequency_List->results->meas ) / sizeof( l1_Frequency_List->results->meas[0] ) ) )
            {
              MSG_ERROR( "Too many freq list elements requested %d clamp to max %d",
                   num_satisfy_threshold_criteria,
                   ( sizeof( l1_Frequency_List->results->meas ) / sizeof( l1_Frequency_List->results->meas[0] ) ), 0 );

             num_satisfy_threshold_criteria =
                   ( sizeof( l1_Frequency_List->results->meas ) / sizeof( l1_Frequency_List->results->meas[0] ) );
            }

            /* ... copy the results into RR's buffer ... */
            for (i=0; i<num_satisfy_threshold_criteria; i++)
            {
               l1_Frequency_List->results->meas[i].RXLEV_average =
                  L1_map_dBm_x16_to_rxlev(l1_Frequency_List->power_dBm_x16[l1_Frequency_List->sorted_array_indexes[i]]);

                  /* F3 the Power Scan Results
                  MSG_GERAN_MED_3_G("PScan Arfcn[%i]RxLev=%i dBm= %i", l1_Frequency_List.ARFCN[l1_Frequency_List.sorted_array_indexes[i]].num,
                                                             l1_Frequency_List.results->meas[i].RXLEV_average,
                                                             (l1_Frequency_List.results->meas[i].RXLEV_average-110));
                  */
               l1_Frequency_List->results->meas[i].arfcn = l1_Frequency_List->ARFCN[l1_Frequency_List->sorted_array_indexes[i]];
            }

            l1_Frequency_List->results->num_frequencies = num_satisfy_threshold_criteria;

            if (l1_get_drx_interval(gas_id) < MIN_FRAMES_FOR_L1_TO_SUSPEND)
            {
                /* ... and tell RR we're done. */
                L1_send_MPH_POWER_SCAN_CNF(TRUE,gas_id);
                l1_tskisr_blk->main_command = L1_SUSPEND_COMMAND;
            }
            else
            {
                /* ... and tell RR we're done. */
                L1_send_MPH_POWER_SCAN_CNF(FALSE,gas_id);
            }
         }
         break;

         case ISR_SUSPEND_CNF:
         {
            isrtim_suspend_cnf_T* message_ptr = ( isrtim_suspend_cnf_T* )msg_header;
            gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

            /* Force G2X shutdown */
            l1_sc_irat_check_active(0, 0, FALSE, 0, FALSE, gas_id);

            gl1_hw_inact_task(FALSE, TRUE, gas_id);

            L1_send_MPH_SUSPEND_GSM_MODE_IND(gas_id );
            l1_set_bplmn_suspend_in_progress(FALSE,gas_id);   /* CgC now we are suspended */

/*
** dual sim implementation is different, has suspending state  see l1_do_WTOG_BPLMN_search()
*/
            if(l1_get_gsm_stop_req_in_queue(gas_id))
            {
               l1_tskisr_blk->main_command = L1_NULL_COMMAND;
               l1_tskisr_blk->command      = L1_NULL_COMMAND;
               l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_SUSPENDED,gas_id);
               l1_set_wtog_bplmn_task_state(NO_HPLMN_SEARCH,gas_id);
               l1_set_gsm_stop_req_in_queue(FALSE,gas_id);
               l1_set_bplmn_suspend_in_progress(FALSE,gas_id);
               l1_change_state(L1_INACTIVE_MODE,gas_id);
               L1isr_send_MPH_STOP_GSM_MODE_CNF (gas_id);
               MSG_GERAN_HIGH_1_G("###WTOG queued stop req actioned FN=%d",gl1_get_FN(gas_id));
            }
   }
   break;

         case ISR_DEACT_CNF:
         {
            isrtim_deact_cnf_T* message_ptr = ( isrtim_deact_cnf_T* )msg_header;
            gas_id_t gas_id = check_gas_id(message_ptr->gas_id);

            /* Force G2X shutdown */
            l1_sc_irat_check_active(0, 0, TRUE, 0, FALSE, gas_id);

            gl1_hw_inact_task( message_ptr->deep_sleep, FALSE, gas_id );

            L1isr_send_MPH_STOP_GSM_MODE_CNF (gas_id);

        }
         break;

         default:
            MSG_GERAN_ERROR_1("L1->L1(WTOG_BPLMN mode) message %d ignored.", msg_header->message_id);
            break;
      }
   }
}
#endif

/*===========================================================================

FUNCTION  L1_setup_PSCAN

DESCRIPTION
  This function sets up the parameters needed to perform a power scan/
  acquisition based on MPH_POWER_SCAN_REQ from RR.

DEPENDENCIES
  None

RETURN VALUE
  error boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean  L1_setup_PSCAN( IMH_T *msg_header )
{
   /* align pointers */
   mph_power_scan_req_T * message_ptr = (mph_power_scan_req_T *)msg_header; /*lint !e740 unusual pointer cast */
   gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
   dBx16_T  pwr_threshold;  
   boolean  error = FALSE;
   Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];
   volatile ISRTIM_CMD_BLK    *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   grm_client_enum_t client_id = l1_tskisr_blk->client_id;
#ifdef FEATURE_DUAL_SIM
   idle_data_T      *l1_idle_data=&l1_idle_data_store[gas_id];
#endif
   if (RR_SEARCH_BACKGROUND == message_ptr->search_mode)
   {
     pwr_threshold = SYS_PARAM_MIN_ACCEPTABLE_PLMN_dBm_x16;
     MSG_GERAN_HIGH_0_G("Power scan threshold set to -102dBm");
   }
   else
   {
     pwr_threshold = SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16;
     MSG_GERAN_HIGH_0_G("Power scan threshold set to -107dBm");
   }

   /* store the threshold for use during sorting later */
   L1_pscan_set_threshold(pwr_threshold, gas_id);

   /* Get length */
   l1_Frequency_List->length = message_ptr->pscan_results_buffer_ptr->num_frequencies;
   
   if( l1_Frequency_List->length != 0 )
   {
     sys_algo_agc_T       *agc_ptr;
     uint16                i;

     if (l1_Frequency_List->length > RR_MAX_PSCAN_FREQ_SCAN)
     {
          /* This implies something is wrong with the build up of the arfcn buffer in
           * upper layers, print out a warning F3.
           */
          MSG_GERAN_ERROR_1_G(" pscan list is %d arfcns long ", l1_Frequency_List->length);

        l1_Frequency_List->length = RR_MAX_PSCAN_FREQ_SCAN;
     }

     /* Set how many measurements per ARFCN. */
     if ( message_ptr->search_mode == RR_SEARCH_RESELECTION )
     {
        l1_Frequency_List->meas_per_ARFCN  = SYS_PARAM_NUM_MEASUREMENTS_PER_CARRIER_RESELECTION;
        l1_Frequency_List->min_meas_period = 0;
     }
     else
     {
        if(mcfg_gcf_nv_get_status())
        {
           /* Do not use the optimizations if Anite GCF Flag is set */
           l1_Frequency_List->meas_per_ARFCN  = SYS_PARAM_NUM_MEASUREMENTS_PER_CARRIER;
           l1_Frequency_List->min_meas_period = SYS_PARAM_Tav_TICKS_MIN/SYS_PARAM_NUM_MEASUREMENTS_PER_CARRIER;
        }
        else
        {
            /* Use the optimised values */
            l1_Frequency_List->meas_per_ARFCN  = SYS_PARAM_NUM_MEASUREMENTS_PER_CARRIER_OPT;
            l1_Frequency_List->min_meas_period = SYS_PARAM_Tav_TICKS_MIN_OPT/SYS_PARAM_NUM_MEASUREMENTS_PER_CARRIER_OPT;
        }
     }

     /* RR provides buffers for the results. */
     l1_Frequency_List->results = message_ptr->pscan_results_buffer_ptr;
     l1_Frequency_List->results->num_frequencies = 0;
     /* Initialize the frequency list. */
     for ( i = 0, agc_ptr = l1_Frequency_List->agc;
           i < l1_Frequency_List->length;
           i++, agc_ptr++ )
     {
        /* Copy ARFCNs into the L1 array. */
        l1_Frequency_List->ARFCN[i] = message_ptr->pscan_results_buffer_ptr->meas[i].arfcn;

        /* Initialize average. */
        l1_Frequency_List->power_dBm_x16[i]     = pwr_threshold;
        l1_Frequency_List->no_of_measurement[i] = 0;

        /* Initialize the AGC data */
        sys_algo_agc_init( agc_ptr, SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16 );
        sys_algo_agc_disable_slew_rate_limiting(agc_ptr);
     }
   }
   else
   {
     MSG_GERAN_ERROR_0_G("No ARFCNs in pscan list.");

     error = TRUE;
   }

   /*check the bands requested in this PSCAN request by RR and select our band_to_request (for TRM resv/Req) accordingly*/
   if (!error) 
   {
#ifdef FEATURE_DUAL_SIM
     /*Bands to request to be updated only if Not for BPLMN in Idle mode*/
     if ( RR_SEARCH_BACKGROUND == message_ptr->search_mode  )
     {
        client_id = gl1_convert_gas_id_to_bplmn_client(gas_id);
        l1_idle_data->g_bplmn_running_state = BPLMN_ACTIVE;
        bplmn_idle_band_bitmap[gas_id] = message_ptr->requested_bands;
        MSG_GERAN_MED_3_G("WARNING: GRM_freq_info not updated as band_bitmap:%d search_mode:%d l1_state:%d",
                            bplmn_idle_band_bitmap[gas_id],message_ptr->search_mode, l1_tskisr_blk->l1_state );
     }
     else    
#endif
     {
       /*selected our band to request, it should not change till the next power scan*/
       gpl1_select_band_to_req_trm(message_ptr->requested_bands, client_id,  gas_id);
     }
     MSG_GERAN_HIGH_1_G("l1_Frequency_List->length = %d", l1_Frequency_List->length);
   }


   l1_pscan_reset_freq_completed(gas_id);

   return  error;
}



/*===========================================================================

FUNCTION  L1_setup_PSCAN_RESCAN

DESCRIPTION
  This function sets up the parameters needed to perform a power scan/
  acquisition based on MPH_POWER_SCAN_REQ from RR.

DEPENDENCIES
  None

RETURN VALUE
  error boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean  L1_setup_PSCAN_RESCAN( gas_id_t gas_id)
{

   dBx16_T  pwr_threshold;
   boolean  error = FALSE;

   Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];
   volatile ISRTIM_CMD_BLK    *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   grm_client_enum_t client_id = l1_tskisr_blk->client_id;
#ifdef FEATURE_DUAL_SIM
   idle_data_T      *l1_idle_data=&l1_idle_data_store[gas_id];
#endif

     pwr_threshold = SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16;
     MSG_GERAN_HIGH_0_G("Power scan threshold set to -107dBm");

   /* store the threshold for use during sorting later */
   L1_pscan_set_threshold(pwr_threshold, gas_id);

   if( l1_Frequency_List->length != 0 )
   {
     sys_algo_agc_T       *agc_ptr;
     uint16                i,j;

     /* Initialize the frequency list. */
     for ( i = 0,j =0;
           i < l1_Frequency_List->length;
           i++)
     {
      if(gl1_acq_asdiv_band_supported(l1_Frequency_List->ARFCN[i],gas_id)== TRUE)
      {
        
        agc_ptr = l1_Frequency_List->agc;
		
        /* Copy ARFCNs into the L1 array. */
        l1_Frequency_List->ARFCN[j] = l1_Frequency_List->ARFCN[i] ;

        /* Initialize average. */
        l1_Frequency_List->power_dBm_x16[j]     = pwr_threshold;
        l1_Frequency_List->no_of_measurement[j] = 0;

        /* Initialize the AGC data */
        sys_algo_agc_init( agc_ptr, SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16 );
        sys_algo_agc_disable_slew_rate_limiting(agc_ptr);
		agc_ptr++;
		j++;
      	}
     }
	 l1_Frequency_List->length = j;   
   }
  if(!(l1_Frequency_List->length))
   {
     MSG_GERAN_ERROR_0_G("No ARFCNs in pscan list.");
	 error = TRUE;

   }
   l1_pscan_reset_freq_completed(gas_id);
   return  error;
}

/*===========================================================================

FUNCTION  L1_setup_DECODE_BCCH_LIST

DESCRIPTION
  This function sets up the parameters needed to perform a parallel
  BCCH decode based on the freq list in the MPH_DECODE_BCCH_LIST_REQ
  from RR

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_setup_DECODE_BCCH_LIST( IMH_T *msg_header )
{
  gas_id_t gas_id;
  int i;
  mph_decode_bcch_list_req_T  *message_ptr;
  BCCH_List_T *l1_BCCH_List_ptr;
  volatile ISRTIM_CMD_BLK    *l1_tskisr_blk;
  grm_client_enum_t client_id;
#ifdef FEATURE_DUAL_SIM
   idle_data_T      *l1_idle_data;
#endif

  /* Align pointers */
  message_ptr = ( mph_decode_bcch_list_req_T *)msg_header; /*lint !e740 unusual pointer cast */
  gas_id = check_gas_id(message_ptr->gas_id);
  l1_BCCH_List_ptr = &l1_BCCH_List[gas_id];
  l1_tskisr_blk  = &l1_tsk_buffer[gas_id];
#ifdef FEATURE_DUAL_SIM
  l1_idle_data=&l1_idle_data_store[gas_id];
#endif
  client_id = l1_tskisr_blk->client_id;

  /*Tell the msg layer about the setting of the value in the decode BCCH list req*/
  gl1_msg_check_list_req_pass_num((boolean)message_ptr->retry_with_reset,gas_id);

  l1_BCCH_List_ptr->sch_found = FALSE;

  l1_BCCH_List_ptr->length = message_ptr->frequency_list.num_frequencies;

  /* Initialize L1 BCCH list and populate with RR list */
  l1_BCCH_List_ptr->all_arfcns_done = FALSE;
  l1_BCCH_List_ptr->current_sch_index = 0;
  l1_BCCH_List_ptr->current_bcch_index = 0;
  l1_BCCH_List_ptr->num_valid_carriers = 0;

  for ( i = 0; i < RR_MAX_CANDIDATE_FREQ_LIST; i++)
  {
    l1_BCCH_List_ptr->next_bcch_fn[i] = MAX_GSM_FRAME_NUMBER + 1;
    l1_BCCH_List_ptr->prev_bcch_fn[i] = MAX_GSM_FRAME_NUMBER + 1;
  }

  for ( i = 0; i < l1_BCCH_List_ptr->length; i++)
  {
     l1_BCCH_List_ptr->arfcn[i] = message_ptr->frequency_list.arfcn[i];

     l1_BCCH_List_ptr->sch_decoded[i] = FALSE;
     l1_BCCH_List_ptr->bcch_decoded[i] = FALSE;
     l1_BCCH_List_ptr->bcch_failed_cnt[i] = 0;
     l1_BCCH_List_ptr->bcch_blocked_cnt[i] = 0;

     l1_BCCH_List_ptr->agc_ptrs[i] = l1_get_agc_ptr ( l1_BCCH_List_ptr->arfcn[i], gas_id );

     if (l1_BCCH_List_ptr->agc_ptrs[i] == NULL)
     {
        /*
           If BCCH decode list freq is not in power scanlist, initialize
           AGC data with default values - this can happen if DSDS result sharing is active
        */
#if !defined ( FEATURE_DUAL_SIM ) && !defined ( FEATURE_TRIPLE_SIM )
        MSG_GERAN_ERROR_1_G("ARFCN %d not in Pscan list!",l1_BCCH_List_ptr->arfcn[i].num);
#endif /* !FEATURE_DUAL_SIM & !FEATURE_TRIPLE_SIM */

        sys_algo_agc_init(&l1_BCCH_List_ptr->agc[i], SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16);
        sys_algo_agc_disable_slew_rate_limiting(&l1_BCCH_List_ptr->agc[i]);
        l1_BCCH_List_ptr->agc_ptrs[i] = &l1_BCCH_List_ptr->agc[i];
     }

     /* power for purpose of coex reproting */ 
     l1_BCCH_List_ptr->rx_power_instant[i] = l1_BCCH_List_ptr->agc_ptrs[i]->pwr_dBm_x16;
     
     /* Initialise the AFC information */
     l1_BCCH_List_ptr->init_acq_afc[i].valid = FALSE;
     l1_BCCH_List_ptr->init_acq_afc[i].word1 = 0;
  }

#ifdef  FEATURE_GSM_COEX_SW_CXM
  if (l1_BCCH_List_ptr->length)
  {
    garb_intf_notify_find_bcch_setup_bcch_list(l1_BCCH_List_ptr->length,
                                               &l1_BCCH_List_ptr->arfcn[0],
                                               &l1_BCCH_List_ptr->rx_power_instant[0],
                                               &l1_BCCH_List_ptr->bcch_decoded[0],
                                               (message_ptr->search_mode == RR_SEARCH_BACKGROUND)?TRUE:FALSE,
                                               gas_id);
    if(message_ptr->requested_bands)
    {
#ifdef FEATURE_DUAL_SIM
     /*Bands to request to be updated only if Not for BPLMN in Idle mode*/
     if ( RR_SEARCH_BACKGROUND == message_ptr->search_mode  )
     {
        l1_idle_data->g_bplmn_running_state = BPLMN_ACTIVE;
        bplmn_idle_band_bitmap[gas_id] = message_ptr->requested_bands;
        MSG_GERAN_MED_3_G("WARNING: GRM_freq_info not updated as band_bitmap:%d search_mode:%d l1_state:%d",
                            bplmn_idle_band_bitmap[gas_id],message_ptr->search_mode, l1_tskisr_blk->l1_state );
     }
     else
#endif
     {
      /*selected the bands to request, it should not change till the next acquisition*/
      gpl1_select_band_to_req_trm(message_ptr->requested_bands, client_id,  gas_id);
    }
  }
  }
#endif /* FEATURE_GSM_COEX_SW_CXM */

  l1_BCCH_List_ptr->si3_only = message_ptr->si3_only;
}

/*===========================================================================

FUNCTION  L1_setup_blind_ho_ACQ

DESCRIPTION
  This function sets up the parameters needed to perform an acquisition
  based on MPH_HANDOVER_REQ from RR (needed for blind handovers)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_setup_blind_ho_ACQ( ARFCN_T                arfcn,
                            const sys_algo_agc_T*  agc_ptr, gas_id_t gas_id )
{
   Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];

   /* Fill up the frequency list */
   l1_Frequency_List->length = 1;

   l1_Frequency_List->ARFCN[0]             = arfcn;
   l1_Frequency_List->no_of_measurement[0] = 0;

   /*debug - we seem to miss the case where agc_ptr = 0!! This is when there is no
   previous info on cell in GSM database.*/
   if( (NULL == agc_ptr) || (agc_ptr->pwr_dBm_x16 == 0) )
   {
     sys_algo_agc_T  *local_agc_ptr;

     l1_Frequency_List->power_dBm_x16[0]     =
       SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16;

     /* Initialize the AGC data */
     local_agc_ptr = l1_Frequency_List->agc_ptrs[0];

     sys_algo_agc_init( local_agc_ptr, SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16 );

     sys_algo_agc_disable_slew_rate_limiting( local_agc_ptr );

     MSG_GERAN_ERROR_1_G("arfcn %d NULL agc ptr",
               arfcn.num );
   }
   else
   {
     *(l1_Frequency_List->agc_ptrs[0])   = *agc_ptr;

     l1_Frequency_List->power_dBm_x16[0] = agc_ptr->pwr_dBm_x16;

     MSG_GERAN_ERROR_2_G("arfcn %d pwr %d dBm16",
               arfcn.num,
               l1_Frequency_List->power_dBm_x16[0] );
   }
}

/*===========================================================================
FUNCTION gl1_is_interrat_handover_msg_valid

DESCRIPTION
  Determines if message has all the required parameters.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - if the requested handover is valid.
  FALSE - otherwise.

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_is_interrat_handover_msg_valid( mph_handover_req_T *message_ptr )
{
    /* First check this is an interrat handover */
    if( RR_L1_WCDMA_GSM == message_ptr->handover_type )
    {
        return TRUE;
    }
#ifdef FEATURE_GSM_TDS
    else if (RR_L1_TDS_GSM == message_ptr->handover_type )
    {
        return TRUE;
    }
#endif
#ifdef FEATURE_LTE_TO_GSM
    else if (RR_L1_LTE_GSM == message_ptr->handover_type )
    {
        return TRUE;
    }

#endif /* FEATURE_LTE_TO_GSM */
    else
    {
        return FALSE;
    }
}

/*===========================================================================
FUNCTION gl1_is_interrat_handover_blind

DESCRIPTION
  Determines if handover is blind or not.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - HANDOVER is BLIND.
  FALSE - HANDOVER is NON-BLIND (a COMPRESSED MODE handover).

SIDE EFFECTS
  If target_cell is found in the measurement data, the target_cell details
  will be populated.
===========================================================================*/
boolean gl1_is_interrat_handover_blind( mph_handover_req_T *message_ptr )
{
     gas_id_t           gas_id;
     idle_data_T       *l1_idle_data;
     l1_null_if_data_t *l1_null_if_data_ptr;
     dedicated_data_T  *l1_dedicated_data_ptr;

     int     x;
     ARFCN_T target_arfcn;
     boolean irat_agc_primed = FALSE;

     if( !message_ptr )
     {   /* CHECK FOR NULL POINTER */
         return TRUE;
     }

     gas_id = check_gas_id(message_ptr->gas_id);
     l1_idle_data=&l1_idle_data_store[gas_id];
     l1_null_if_data_ptr = &l1_null_if_data[gas_id];
     l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
     target_arfcn = message_ptr->BCCH_ARFCN;

     MSG_GERAN_HIGH_3_G("HO command ARFCN=%d, BCC=%d, NCC=%d",target_arfcn.num, message_ptr->BSIC.BS_colour_code, message_ptr->BSIC.PLMN_colour_code);

     /* Check if desired ARFCN is contained in list */
     for( x=0; x < message_ptr->interrat_measurement_data.number_of_cells &&
               x < RR_L1_MAX_INTERRAT_MEASUREMENTS                         ; ++x )
     {
         if( ARFCNS_EQUAL(message_ptr->interrat_measurement_data.interrat_measurements[x].bcch_arfcn, target_arfcn))
         {
             MSG_GERAN_HIGH_1_G("ARFCN %u found", target_arfcn.num);
             if((message_ptr->interrat_measurement_data.interrat_measurements[x].bsic_bcc == message_ptr->BSIC.BS_colour_code)
             &&(message_ptr->interrat_measurement_data.interrat_measurements[x].bsic_ncc == message_ptr->BSIC.PLMN_colour_code) )
             {
                 /* Cell Measurement found -- NON-BLIND */
                 uint8   local_bsic    = 0;
                 uint32  local_fn_lag  = 0;
                 int16   local_qs_lag  = 0;

                 MSG_GERAN_HIGH_2_G("BSIC found, BCC=%d, NCC=%d",message_ptr->BSIC.BS_colour_code, message_ptr->BSIC.PLMN_colour_code);

                 local_bsic = (uint8)(message_ptr->interrat_measurement_data.interrat_measurements[x].bsic_ncc<<3)+
                              (message_ptr->interrat_measurement_data.interrat_measurements[x].bsic_bcc);

                 local_fn_lag = message_ptr->interrat_measurement_data.interrat_measurements[x].fn_offset;


                 /* CALCULATE QS_LAG as (qbit_offset - GL1_HW_CM_DELAY_VALUE = 1 slot = 625 qs) */
                 local_qs_lag = (int16)message_ptr->interrat_measurement_data.interrat_measurements[x].qbit_offset;

                 /* ================================================================ *
                  *   The QBIT_LAG passed always FRAME_TICK_SETUP_MARGIN_QS (625 qs) *
                  *   too high due to the implementation on wcdma side.              *
                  * ================================================================ */

                   if ( 0 <= local_qs_lag && local_qs_lag < FRAME_TICK_SETUP_MARGIN_QS )
                   {
                     local_fn_lag = ADD_FN ( local_fn_lag , 1 );
                     MSG_GERAN_HIGH_2_G("qbit_lag %d (< %d) FN++",local_qs_lag,FRAME_TICK_SETUP_MARGIN_QS);
                   }
                   local_qs_lag = SUB_MOD( local_qs_lag, FRAME_TICK_SETUP_MARGIN_QS, 5000);


    #ifdef  DEBUG_FORCE_BLIND_HANDOVER
                 #error code not present
#endif
                 /* Populate target_cell_details */
                 l1_dedicated_data_ptr->target_cell_details.arfcn      = target_arfcn;
                 l1_dedicated_data_ptr->target_cell_details.bsic_known = TRUE;
                 l1_dedicated_data_ptr->target_cell_details.bsic       = local_bsic;
                 /* NOTE: WCDMA stores this as "DIFF" - convert to "LAG" */
                 l1_dedicated_data_ptr->target_cell_details.fn_lag     = local_fn_lag;
                 l1_dedicated_data_ptr->target_cell_details.qbit_lag   = local_qs_lag;
                 /* NOTE: rx_power is N/A -- I'll use a dummy value of the middle value */
                 l1_dedicated_data_ptr->target_cell_details.rx_power
                     = (SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16 + SYS_PARAM_MAX_RX_LVL_dBm_x16) / 2 ;

                 /* fine freq */
                 l1_dedicated_data_ptr->target_cell_details.frequency_offset =
                   message_ptr->interrat_measurement_data.interrat_measurements[x].frequency_offset;

                 /* Populate camped on cell details */
                 l1_idle_data->campedon_cell_ARFCN = message_ptr->BCCH_ARFCN;

#ifdef  FEATURE_GSM_DTM
                 /* Check the expected receive level on the camped on cell */
                 if( 0 == l1_idle_data->campedon_cell_agc.pwr_dBm_x16 )
                 {
                   /* When it is zero then initialise using the value in the message
                    * This for the benefit of the packet channels in DTM mode.
                    */
                   l1_idle_data->campedon_cell_agc.pwr_dBm_x16 =
                      message_ptr->interrat_measurement_data.interrat_measurements[x].rssi_dBm_x16;
                 }
#endif
                 /* Initialize AGC with rssi_dBm_x16 value from CM measurements */
                 /* Load the ARFCN, else the AGC will not be applied correctly. */
                 l1_dedicated_data_ptr->c0_agc.arfcn = message_ptr->BCCH_ARFCN;

                 sys_algo_agc_init( &l1_dedicated_data_ptr->c0_agc,
                                     message_ptr->interrat_measurement_data.
                                     interrat_measurements[x].rssi_dBm_x16);
                 sys_algo_agc_enable_slew_rate_limiting(&l1_dedicated_data_ptr->c0_agc);

                 /* Pass the iRAT Handover AGC data to be applied. */
                 l1_interrat_ho_agc(l1_dedicated_data_ptr->c0_agc, gas_id);

                 /* Indicate AGC has been primed */
                 irat_agc_primed = TRUE;

                 return FALSE;

             }
             else
             {
               /* Use the measured power to initialise the AGC */
               /* Load the ARFCN, else the AGC will not be applied correctly. */
               l1_null_if_data_ptr->l1_w2g_blind_handover_agc.arfcn = message_ptr->BCCH_ARFCN;

               sys_algo_agc_init( &(l1_null_if_data_ptr->l1_w2g_blind_handover_agc),
                                  message_ptr->interrat_measurement_data.
                                  interrat_measurements[x].rssi_dBm_x16 );

               /* Pass the iRAT Handover AGC data to be applied. */
               l1_interrat_ho_agc(l1_null_if_data_ptr->l1_w2g_blind_handover_agc, gas_id);

               /* Indicate AGC has been primed */
               irat_agc_primed = TRUE;

               MSG_GERAN_ERROR_2_G(" init w2g bho %d dBm16 arfcn %d ",
                         message_ptr->interrat_measurement_data.
                         interrat_measurements[x].rssi_dBm_x16/16,
                         target_arfcn.num);
             }
         }
     }

     /* If we still haven't primed AGC then do it now as either
      * we haven't found the target cell or there are no interrat measurements
      */
     if(irat_agc_primed == FALSE)
     {
       /* Use mid-range power (-70 dBm) to initialise the AGC */
       /* Load the ARFCN, else the AGC will not be applied correctly. */
       l1_null_if_data_ptr->l1_w2g_blind_handover_agc.arfcn = message_ptr->BCCH_ARFCN;

       sys_algo_agc_init( &(l1_null_if_data_ptr->l1_w2g_blind_handover_agc),SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16 );

       /* Pass the iRAT Handover AGC data to be applied. */
       l1_interrat_ho_agc(l1_null_if_data_ptr->l1_w2g_blind_handover_agc, gas_id);

       MSG_GERAN_ERROR_2_G(" init w2g bho %d dBm16 arfcn %d ",
                 l1_null_if_data_ptr->l1_w2g_blind_handover_agc.pwr_dBm_x16/16,
                 target_arfcn.num);
     }

     MSG_GERAN_HIGH_1_G("ARFCN %u not found, going blind...", target_arfcn.num);

     /* If arfcn is not in the list, perform a blind handover */
     return TRUE;
}

/*===========================================================================
FUNCTION GL1_STORE_INTERRAT_HANDOVER_DETAILS

DESCRIPTION
  This function stores parameters from the handover message required for
  inter-RAT handover.  This is because in the blind case, some calculations
  such as handover_ta can't be done until qbit_lag is known.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_store_interrat_handover_details( mph_handover_req_T *message_ptr )
{
    gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
    idle_data_T      *l1_idle_data=&l1_idle_data_store[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    /* Store params common to GSM handover */
    gl1_store_handover_details ( message_ptr );

    /* Update Idle ARFCN to that of target cell*/
    l1_idle_data->campedon_cell_ARFCN = message_ptr->BCCH_ARFCN;

    /* Update Dedicated params to that of target cell*/
    l1_dedicated_data_ptr->channel_specification
        = message_ptr->channel_spec;

#ifdef FEATURE_GSM_AMR
    /* Now fill in the multirate configuration data */
    memscpy ( &l1_dedicated_data_ptr->multirate_configuration,
             sizeof(l1_dedicated_data_ptr->multirate_configuration),
             &message_ptr->multirate_config,
             sizeof(multirate_config_T) );
#endif

    l1_dedicated_data_ptr->starting_time
        = (message_ptr->channel_spec.starting_time[0]<<8)
         + message_ptr->channel_spec.starting_time[1];
    /* Added l1_dedicated_data.channel_specification.power_level */
    /* so that for iRAT Handover the power level is transferred. */
    l1_dedicated_data_ptr->ms_power_level_in_use
        = l1_dedicated_data_ptr->channel_specification.power_level
        = l1_dedicated_data_ptr->ms_power_level_to_use
        = l1_dedicated_data_ptr->ms_power_level_to_report
        = l1_dedicated_data_ptr->ordered_ms_power_level
        = message_ptr->channel_spec.power_level;

    MSG_GERAN_HIGH_1_G("WtG iRAT Handover Tx Power Lvl: %d",
              l1_dedicated_data_ptr->ms_power_level_in_use);

    /* Transfer the cipher key */
    memscpy(l1_dedicated_data_ptr->cipher_key, sizeof(l1_dedicated_data_ptr->cipher_key),message_ptr->cipher_key, CIPHER_KEY_LENGTH);
    memscpy(l1_dedicated_data_ptr->cipher_key_128,sizeof(l1_dedicated_data_ptr->cipher_key_128), message_ptr->cipher_key_128, CIPHER_KEY_128_LENGTH);

#ifdef DEBUG_USE_FAKE_CIPHER_KEY
    #error code not present
#endif /*DEBUG_USE_FAKE_CIPHER_KEY*/

    MSG_GERAN_HIGH_3_G("Cipher Flag:%u Key:%#x %#x",
             l1_dedicated_data_ptr->channel_specification.cipher_flag,
             l1_dedicated_data_ptr->cipher_key[0],
             l1_dedicated_data_ptr->cipher_key[1]);
    MSG_GERAN_HIGH_3_G("%#x %#x %#x",
             l1_dedicated_data_ptr->cipher_key[2],
             l1_dedicated_data_ptr->cipher_key[3],
             l1_dedicated_data_ptr->cipher_key[4]);
    MSG_GERAN_HIGH_3_G("%#x %#x %#x",
             l1_dedicated_data_ptr->cipher_key[5],
             l1_dedicated_data_ptr->cipher_key[6],
             l1_dedicated_data_ptr->cipher_key[7]);

    l1_dedicated_data_ptr->L1_confirm_msg = PH_RANDOM_ACCESS_CNF;

    /* Set the number of handocer access bursts to be sent */
    if( message_ptr->handover_synchronisation == NON_SYNCHRONISED )
    {
        l1_dedicated_data_ptr->no_of_ab_to_send = 0xff;
    }
    else
    {
        l1_dedicated_data_ptr->no_of_ab_to_send = 4 + 1;
    }

    /* Initialize these to zero -- if non-zero then we will need to unslam */
    l1_dedicated_data_ptr->current_cell_details.fn_lag   = 0; /* Frame  Lag */
    l1_dedicated_data_ptr->current_cell_details.qbit_lag = 0; /* QtrBit Lag */
    l1_dedicated_data_ptr->current_cell_details.TN       = 0; /* Timeslot#  */

}

/*===========================================================================

FUNCTION gl1_begin_INTERRAT_Reconnect

DESCRIPTION
  This function calculates the necessary fields to attempt a reconnect
  to the GSM dedicated channel that was in use before the failed
  GtoW handover, and transitions to DEDICATED_SUSPEND.

DEPENDENCIES
  l1_dedicated_data.target_cell_details must be populated either by:
    measurement info (NON-BLIND case)
    completion of GET_TIMING_INFO (BLIND case)

RETURN VALUE
  None

SIDE EFFECTS
  MPH_RECONNECT_CHANNEL_IND()
===========================================================================*/
void gl1_begin_INTERRAT_Reconnect(gas_id_t gas_id)
{
    idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    uint32 gstmr_FN = GSTMR_GET_FN_GERAN( gas_id);
    uint32 gl1_FN   = gl1_get_FN(gas_id);

    /* Update camped on cell BSIC */
    l1_idle_data->campedon_cell_BSIC =
            l1_dedicated_data_ptr->current_cell_details.bsic;


    /* TEMP until GSTMR & L1_FN are the same */
    /* Adjust fn_lag to compensate for the missed frame ticks */
    (void)L1_change_FN( SUB_FN(gl1_FN, gstmr_FN), gas_id );

    /* GSM L1 controls vocoder across an inter-RAT HO */
    /* Now we depend on NAS access function to tell us when
     * we  acquire vocoder
     */

  /* From null, DRX manager will have the TRX state as off */
    gl1_set_tx_on_always (gas_id);
    gl1_hw_turn_tx_on(gas_id);

    /* Transition to dedicated suspend mode */
    l1_change_state( L1_DEDICATED_MODE, gas_id );
    l1_tskisr_blk->sub_state    = L1_DED_SUSPEND;
    l1_tskisr_blk->main_command = L1_DEDICATED_COMMAND;
    l1_tskisr_blk->command      = L1_SET_HANDVR_CIPH_COMMAND;
    /* Set config */
    l1_tskisr_blk->next_params.L1Data.pDedi_data = l1_dedicated_data_ptr;
    l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_RECONNECT_CHANNEL_CNF;
    L1_transfer_new_data( gas_id );

    //MPH_RECONNECT_CHANNEL_CNF will be sent to RR, from L1_control_dedicated after starting SCE
    //L1isr_send_MPH_RECONNECT_CHANNEL_CNF( );

    /* Tell CGPS we are leaving this cell */
    gl1_timetag_cgps(L1_DEDICATED_MODE, CGPS_TIMETAG_POST_CELL_CHANGE,gas_id);
}

/*===========================================================================

FUNCTION gl1_begin_INTERRAT_Handover

DESCRIPTION
  This function calculates the necessary fields to perform handover and
  transitions to DEDICATED_SUSPEND on success, or NULL on failure.

DEPENDENCIES
  l1_dedicated_data.target_cell_details must be populated either by:
    measurement info (NON-BLIND case)
    completion of GET_TIMING_INFO (BLIND case)

RETURN VALUE
  None

SIDE EFFECTS
  MPH_HANDOVER_IND( NO_ERROR )     sent on success
  MPH_HANDOVER_IND( OUT_OF_RANGE ) sent on failure
===========================================================================*/
void gl1_begin_INTERRAT_Handover(gas_id_t gas_id)
{
    idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    boolean interrat_handover_is_possible = FALSE;
    byte    ho_ta = 0; /* Initializing to stop lint from complaining */

    /* Update camped on cell BSIC */
    if( l1_dedicated_data_ptr->target_cell_details.bsic_known )
    {
        l1_idle_data->campedon_cell_BSIC =
            l1_dedicated_data_ptr->target_cell_details.bsic;

        if( l1_dedicated_data_ptr->ho_details.BSIC == l1_dedicated_data_ptr->target_cell_details.bsic )
        {
           /* Synchronized handovers are not applicable in the W2G case since
            * we don't have the old TA.  So don't go through the computation of
            * new TA since it will be incorrect.  Just set it to 0
            */
            if ( l1_dedicated_data_ptr->ho_details.timing_advance_valid )
            {
               ho_ta = l1_dedicated_data_ptr->ho_details.timing_advance;
            }
            else
            {
               ho_ta = 0;
            }
            MSG_GERAN_HIGH_1_G("W2G HO TA = %d",ho_ta);
            interrat_handover_is_possible = TRUE;
        }
    }

    if ( interrat_handover_is_possible )
    {
        /* If its a compressed mode handover, use passed in fn_lag */
        if ( l1_tskisr_blk->main_command == L1_CM_HANDOVER_COMMAND
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
             ||
             l1_tskisr_blk->main_command == L1_CM_HANDOVER_COMMAND_WAIT_FOR_TRM
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
             )
        {
            /* CALCULATE FN_LAG as (-FN_OFFSET) */
            l1_dedicated_data_ptr->target_cell_details.fn_lag =
            NEG_FN( l1_dedicated_data_ptr->target_cell_details.fn_lag );

            MSG_GERAN_HIGH_3_G("Tgt cell: BSIC=0x%x Qlag=%u FNlag=%u",
                   l1_dedicated_data_ptr->target_cell_details.bsic,
                   l1_dedicated_data_ptr->target_cell_details.qbit_lag,
                   l1_dedicated_data_ptr->target_cell_details.fn_lag
                  );
         }

        /* Apply timing advance calculated in above function and save old TA */
        gl1_msg_store_ta( ho_ta,gas_id );
        l1_dedicated_data_ptr->timing_advance_in_use  = ho_ta;
        l1_dedicated_data_ptr->timing_advance_to_use  = ho_ta;
        l1_dedicated_data_ptr->ordered_timing_advance = ho_ta;

        /*Imp!!!  Change TSC to that of the target cell */
        /* Need to do this everythime we switch to another cell */

        /* Copy over channel specs first because L1_sync_to_target uses
         * pDedi_data
         */
        l1_tskisr_blk->next_params.L1Data.pDedi_data = l1_dedicated_data_ptr;
        L1_transfer_new_data( gas_id );

        /* fine freq */
        /* Initialise the band and the pdm value */
        gl1_msg_acq_afc_init( l1_dedicated_data_ptr->target_cell_details.arfcn,gas_id );

        /* fine freq Now set the stored frequency offset
         * Use a quick hack to convert the Hz error to pdm value.
         */
        gl1_msg_reset_pdm( l1_dedicated_data_ptr->target_cell_details.frequency_offset,
                           l1_dedicated_data_ptr->target_cell_details.arfcn,gas_id );

        /* Transition to dedicated suspend mode */
        /* (waiting for PH_RANDOM_ACCESS_REQ)   */
        l1_change_state( L1_DEDICATED_MODE, gas_id );

        /* Set SC for Dedi so NCell updated correctly */
        l1_sc_start( L1SCModeDediTCh, gas_id );

        /* Transition to dedicated suspend mode */
        /* (waiting for PH_RANDOM_ACCESS_REQ)   */
        l1_change_state( L1_DEDICATED_MODE, gas_id );

#ifdef FEATURE_GSM_DTM
        /* Setup slam mode for dedicated mode to lowest tn */
        l1_dedicated_data_ptr->l1_dtm_slam_mode = L1_CS_TN_SLAM_LOWEST;
#endif

        /*Set X2G HO flag*/
        l1_indicate_x2g_ho(TRUE,gas_id);

        /* SLAM TIMING TO TIMESLOT 0 of TARGET CELL
           NOTE: the inverse adjustments are written to current_cell_details */
        L1_sync_to_target( &l1_dedicated_data_ptr->current_cell_details,
                           &l1_dedicated_data_ptr->target_cell_details, gas_id  );


        /* GSM L1 controls vocoder across an inter-RAT HO */
        /* Now we depend on NAS access function to tell us when
         * we  acquire vocoder
         */

        L1_send_MPH_HANDOVER_IND(NO_ERROR, gas_id);

        /* Tell CGPS we are leaving this cell */
        gl1_timetag_cgps(L1_DEDICATED_MODE, CGPS_TIMETAG_POST_CELL_CHANGE,gas_id);

        MSG_GERAN_HIGH_0_G("Interrat Handover success");
    }
    else
    {
        /* Reset the BHO flag */
        l1_tskisr_blk->acq_for_blind_ho = FALSE;
        /* Wait in NULL mode for STOP_GSM_MODE command */
        l1_change_state( L1_NULL_MODE, gas_id );

        L1_send_MPH_HANDOVER_IND( HANDOVER_OUT_OF_RANGE, gas_id );
        MSG_GERAN_HIGH_0_G("Interrat Handover failed");
    }

    /* Prime For Blind and Compressed iRAT Handovers */
    l1_prime_agc_for_handover(l1_dedicated_data_ptr->target_cell_details.arfcn,gas_id);

}

/*===========================================================================

FUNCTION L1_setup_DEACTIVATE

DESCRIPTION
  Prepares Layer 1 for the MPH_STOP_GSM_MODE_REQ sent by GRR
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void L1_setup_DEACTIVATE( IMH_T *msg_header )
{
   mph_stop_gsm_mode_req_T *message_ptr;
   gas_id_t gas_id;
   volatile ISRTIM_CMD_BLK *l1_tskisr_blk;
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr;

   /* align pointers */
   message_ptr = (mph_stop_gsm_mode_req_T *)msg_header; /*lint !e740 unusual pointer cast */
   gas_id = check_gas_id(message_ptr->gas_id);
   l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   l1_tsk_buffer[gas_id].g2x_resel_in_progress = message_ptr->g2x_resel_in_progress;
   MSG_GERAN_HIGH_1_G("L1_setup_deactivate: g2x_resel_in_progress:%d",
                      l1_tsk_buffer[gas_id].g2x_resel_in_progress);
#endif
   if( l1_tskisr_blk->l1_state != L1_INACTIVE_MODE )
   {
     rr_send_stop_gsm_mode_req[gas_id] = TRUE ;
   }
   else
   {
     rr_send_stop_gsm_mode_req[gas_id] = FALSE;
   }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* reset inversion flag */
   gpl1_set_invert_sys_info_trm_priority(FALSE, gas_id);
#endif

   /* release VFR*/
   gl1_hw_terminate_tch(gas_id);

   /* Only stop idle SAIC disable timer on deactivation */
   l1_idle_saic_disable_timer_deactivate(gas_id);

   if (message_ptr->rr_l1_gsm_stop_reason == RR_L1_DEEP_SLEEP) {
      MSG_GERAN_MED_0_G("Deactivating GL1 for Deep Sleep.");
   }
   else if (message_ptr->rr_l1_gsm_stop_reason == RR_L1_RAT_CHANGE) {
      MSG_GERAN_MED_0_G("Deactivating GL1 for RAT Change.");
   }

  /* When we are in idle mode we need to tell the DRX manager
    * when we send a callback to the task so it knows that the
    * task is active.  This call is ignore when we are not in
    * Idle Mode because the DRX manager is off.
    */
   MSG_GERAN_MED_0_G("Task active, stop sleep until cnf sent");

   gl1_drx_task_active(gas_id);

   GL1_ISR_SAVE_LOCK(gas_id);

   if (message_ptr->rr_l1_gsm_stop_reason == RR_L1_DEEP_SLEEP)
   {
      l1_tskisr_blk->command = L1_DEEP_SLEEP_COMMAND;
   }
   else if (message_ptr->rr_l1_gsm_stop_reason == RR_L1_RAT_CHANGE)
   {
      l1_tskisr_blk->command = L1_RAT_CHANGE_COMMAND;
   }
   else
   {
      MSG_GERAN_ERROR_0_G("Invalid stop GSM reason.");
      l1_tskisr_blk->command = L1_RAT_CHANGE_COMMAND;
   }

   /* Tell CGPS we are leaving this cell */
   gl1_timetag_cgps(l1_tskisr_blk->l1_state, CGPS_TIMETAG_PRE_CELL_CHANGE,gas_id);

   /* Only stop idle SAIC disable timer on deactivation */
   l1_idle_saic_disable_timer_deactivate(gas_id);

   /* Final release of TCXO */
   gl1_hw_tcxo_release( gas_id );

   /* free up resources used for the frequency list if possible */
   l1_frequency_list_mem_free(gas_id);
   l1_sc_wcdma_data_ptr->issue_g2x_deinit = TRUE;
   /* Command the ISR to deactivate */
   l1_tskisr_blk->main_command = L1_DEACTIVATE_COMMAND;

   /*
   ** CgC DEACTIVATE can occur during a transition into IDLE before the isr has run.
   ** if sub_state == L1_IDLE_INIT, will cause a restart of gsm during deactivate
   ** and can lead to a bus hang. so skip over the init state in the L1_idle_mode_ISR()
   */
   if( l1_tskisr_blk->sub_state == L1_IDLE_INIT)
   {
       l1_tskisr_blk->sub_state =  L1_IDLE_WAITING;  /* set to a benign state */
   }

   /* Reset the BPLMN search state if deactivating GSM */
   l1_tskisr_blk->current_params.L1_confirm_ID = l1_tskisr_blk->next_params.L1_confirm_ID = L1_DEACTIVATE_BPLMN_CNF;

   /* Clear any RR priority inversion */
   grm_set_acquisition_priority(GERAN_PRIORITY_NOMINAL, gas_id);
   grm_set_resource_lock_state(FALSE, gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* Clear pre-idle mode RR indications */
   l1_tskisr_blk->x2g_resel_in_progress = FALSE;
   l1_tskisr_blk->x2g_cco_in_progress = FALSE;
#endif
   /* Deactivate the ISR watchdog */
   l1_isr_watchdog_deactivate(gas_id);

   l1_isr_femto_timer_stop();

   GL1_ISR_SAVE_UNLOCK(gas_id);
}


#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
void L1_setup_SUSPEND( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   GL1_ISR_SAVE_LOCK(gas_id);

   l1_tskisr_blk->command = L1_SUSPEND_COMMAND;

   /* Tell CGPS we are leaving this cell */
   gl1_timetag_cgps(l1_tskisr_blk->l1_state, CGPS_TIMETAG_PRE_CELL_CHANGE,gas_id);

   /* Command the ISR to deactivate */
   l1_tskisr_blk->main_command = L1_SUSPEND_COMMAND;

   /* Deactivate the ISR watchdog */
   l1_isr_watchdog_deactivate(gas_id);

   GL1_ISR_SAVE_UNLOCK(gas_id);
   MSG_GERAN_HIGH_0_G("L1_setup_SUSPEND");


}
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION l1_ds_get_w2g_blind_handover_agc

DESCRIPTION
  This function returns the address of l1_w2g_blind_handover_agc
  required by the Dual Sim null if
DEPENDENCIES

SIDE EFFECTS

===========================================================================*/
sys_algo_agc_T* l1_ds_get_w2g_blind_handover_agc(gas_id_t gas_id)
{
  l1_null_if_data_t *l1_null_if_data_ptr = &l1_null_if_data[gas_id];
  return &(l1_null_if_data_ptr->l1_w2g_blind_handover_agc) ;
}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/* EOF */

