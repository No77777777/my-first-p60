/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

          L 1   D E D I C A T E D - F U L L R A T E  T R A F F I C

GENERAL DESCRIPTION
   This module contains the procedures to control the full rate traffic
   channel.  These procedure executes in the ISR context and
   signals the L1 task of any status.

EXTERNALIZED FUNCTIONS
  L1_control_full_rate
     This function controls the traffic channel,
  L1_handle_nt_downlink
     This function processes downlink data in Non- Transparent data mode
  L1_handle_nt_uplink
     This function processes uplink data in Non- Transparent data mode
  L1_set_to_tch_mode
     Test Procedure to set the L1 state machine to dedicated mode - traffic

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_tch.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
11/12/17   mk       CR2152940: Revert CR 2001095 and related CR changes
26/06/18   sid      CR2267640 Block FACCH transmissions while aborting current channel
03/05/18   rv       CR2233364 : Add feature flags for FR38904 changes - GERAN
14/11/16   mk       CR1082549 : FR 38904: Audio gap optimization during Handovers in Voice call - GERAN
21/02/17   bg       CR2007570 Use PRx power for DRx power instead of default gain (-70dBm) when RxD enabled
09/02/16   mk       CR2002074 Do not include offset for relative FN calculation for TxLev 
05/08/16   tsk      CR1048165 Reset GL1 and Gdrivers flag to remain in sync after RxD exit.
30/05/17   rv       CR2031381: L1 fix to address issues with CR2001095 causing GCF failures
06/03/17   sp       CR2014559: Ensure FACCH fill frame not sent during Loopbacks
02/02/17   hd       CR2001095 Not to send homing sequence when vocoder is ready, but not transmitting
08/12/16   mk       CR1097205: Fixing AGC initialisation when Rxd is getting enabled and enhancing serving cell measurement reporting when Rxd was partially enabled in a SACCH period
19/09/16   dg       CR1066652 Make MCVS request of 384 mhz 1 frame in advance before initiating G2L search in dedicated mode
30/06/16   mk       CR957925  Do not send DTX disable to vocoder blindly in HO scenario which impacts voice quality
16/05/16   mk       CR1010068 Add DTF events in dedicated mode
12/05/16   rv       CR998395: Excessive F3 logging in  GERAN L1
18/04/16   hd       CR1004583: Delay the vocoder stop and start till the point just before VFR is aligned to optimize 
                    the mute duration during the time of G2G handover
08/03/16   dg       CR975456 Send tx on to RF after frame adjustment has happened for X2G Handover case
18/02/16   hd       CR978516 No need to check for block boundary while sending PH_DATA_IND to L2 
01/02/16   db       CR965933 GCF TC 26.21.8-4 Handover failure
26/08/15   ws       CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
15/07/15   mc       CR861999 Failure to abort TCH mode when G+G COEX manager denies permission to FACCH Tx
04/06/15   jk       CR846952:RxLEV report should be based on maximum of PRx and DRx average RSSIs
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
09/04/15   cja      CR819956 G2T changes to support new RF API's  FEATURE_NEW_RF_API
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
07/04/15   shr      CR808932: COEX support for DTM
01/04/15   pjr      CR810462 Ensure Vocoder acquire/enable sequence is respected during srvcc handover
31/03/15   pa       CR815621:GRM restructuring for TH 2.0. 
30/03/15   mc       CR773788: COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
11/03/15   pjr      CR806250 Fix featurisation for changes submitted under CR787199
24/02/15   pjr      CR787199 Acquire vocoder immediately after IMS releases it during srvcc handover
17/11/14   dg       CR758006 IRAT improvements to ease debugging
20/02/15   js       CR693771: W+T removal. Resolving compilation errors 
16/12/14   jk       CR753423:LIF for Idle,voice and data
31/10/14   xz       CR748816 Extend dual data space for building RF script variable 
25/07/14   hd       CR699946 Pass correct channel type to the function l1_log_burst_metrics() for FR/HR
20/05/14   hd       CR668030: Meas Report is sent after processing the blocks from FW so that it contains 
                    the block info from the current Meas Period.
17/07/14   pjr      CR672464 Removed fake VFR realignment sequence
11/07/14   pjr      CR 692177 Added support for RFACCH 2.0
10/07/14   jk       CR688587:GL1 changes to support LIF feature
24/07/14   og       CR684296. Sanity check the gas id in dedicated isr and SCE.
02/07/14   mc       CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
29/04/14   pjr      CR600612 Dedicated WTR Hop initial code drop
08/05/14   cgc      CR642356 fix prototype for tch_rfacch_indicator() added to header.
06/05/14   npt      CR659704 Ensure VFR is assigned before trying to align it
10/04/14   am       CR643747 Consider all SACCH bursts for AFC update
08/04/14   jk       CR649527 VAMOS2 Fixes for Freqeucny hopping and DTX ON cases
13/03/14   mc       CR602368 COEX : Support for Frequency Hopping over large ARFCN lists
19/02/14   jk       CR608290 GERAN RxD ON/OFF adaptive algorithm
02/01/14   jk       CR595130 GL1 changes to support VAMOS-II
06/02/14   smd      CR548408 Remove sending TX state through MCS_CXM_STX messages. 
19/12/13   jk       CR503359 GL1 changes to support Rx diversity
12/12/13   pjr      CR588712 Call mn_has_nas_already_acquired_voc when FEATURE_DUAL_ACTIVE is enabled
10/12/13   npt      CR586981 Fix the featurisation for UL SACCH 
29/11/13   js       CR574402 COEX : VBATT : Accurate prediction of N+1 frame Tx power for CS (TCH and SDCCH)
02/10/13   pjr      CR552448 DDR Frequency plan implementation - l1_arbitration modification
04/11/13   mc       CR556742 G+G COEX : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
24/10/13   cja      CR566207 Generate filler frame for FR signalling only channel.
28/11/13   cja      CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
22/11/13   cja      CR455527 Remove featurisation from files in API directory
24/09/13   jk       CR549227:Shifted SACCH changes for VAMOS2
07/08/13   jk       CR490807:Support for shifted SACCH  
08/11/13   cja      CR555989 Do not add extra power monitors until first complete TCH multiframe.
05/11/13   cja      CR523987 Add FEATURE_GSM_DED_SCELL_MONITOR for extra SCell monitor in voice call
24/10/13   cja      CR566207 enerate filler frame for FR signalling only channel.
28/08/13   mko    CR532432:While running the TCH abort FTN processing is not needed. Check is added to fix it
20/08/13   npt      CR530340 - Added VSTMR VFR handling  
15/08/13   sk       CR524039 GBTA bringup changes
7/16/13    zc    	CR470197 Replace memcpy with memscpy
22/05/13   cs       TSTS Updates
16/05/13   pjr      CR487402: Changes to improve interaction with Voice Services
03/04/13   pjr      CR470940 Modification to calls to new VS interface
07/03/13   dv       CR461080: Introduction of new GSM VS interface
13/03/13   mc       CR459487 : COEX Support - GERAN arbitrator framework
04/03/13   ws       CR457871 - Crash in l1_acch_nv_flag_initialise() due to missing gas_id
07/12/12   jv       CR438196: Using RF Task API gl1_hw_rf_idle_frame_processing() for performing idle frame processing.
07/12/12   cja      CR429173 Remove Dime compiler warnings
31/05/12   ab       CR 415136 RX and TX Time Power averaging update using msg router intf.
08/06/12   pa       GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
22/06/12   pa       CR371580: Delay TBF release during G2T activity.
31/05/12   ab       CR 366880 RX and TX Time Power averaging Feature for DSDA as part of SAR Req.
25/04/12   pa       CR355881: G2T Dedicated TCH: TL1 crashed in RF API. Added support of RF_SCRIPTS_CMD.
13/03/12   ws       CR 330862 - Change dedicated mode F3 level's and add sacch burst
                    metrics and compact message metrics log packets
12/04/12   cja      CR351581 In FTM do not error fatal if stuck.
19/03/11   rc       CR:344697 Featurized code under FEATURE_MODEM_NO_VOICE_SUPPORT to support no voice builds
13/03/12   ws       CR335057 eSACCH buffer flush and restore after inter-cell handovers
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
10/01/12   cja      CR327967 Add request MCPM AEq on for FR
18/10/11   jj       CR310766 Support of GSM thermal Read in idle Frame
16/05/11   nf       CR281320 R-SACCH flags not correctly set up
19/04/11   cja      Add cipher clock gating for Nikel
18/03/11   ws       Further updates for MCPM
18/03/11   ws       Added Initial MCPM updates
22/11/10   ky       Panic reset mechanism is changed. For all errors gl1 first
                    sees an nv item set or not. if this nv item is set then does
                    error fatal immediately other wise gl1 sends signal to grr for
                    recovery
22/10/10   dv       CR261234. Add Homing Sequences for fusion products
15/06/10   cja      CR241658. Add catch for Tx sacch not active (for CS end in EDTM)
14/06/10   dv       GP042232 - correction in BEP operation for EMR
26/04/10   cja      CR235459. GCF14.5.2 failing as SAIC not re-enabled for TCH9.6.
10-04-06   tjw      Lint - correct indent
30/03/10   dv       Lint fix.
11/03/10   ws       Fixed compile warnings for MOB environment
05/03/10   dv       CR224044 GCF TC 26.7.5.2 is failing due to L1 forwarding
                    RFACCH blk to Lapdm
15/02/10   cja      Re-enable GSAC clock when dedicated started if cipher enabled.
21/01/10   cs       Fix mereg error from previous checkin
20/01/10   cs       CR221032 21.3.1 failure with SAIC enabled as AFC/TT not updating
05/01/10   cs       CR 218984 GCF 13.3.4.1 failure on high bands TX lev wrongly
                    reported
17/11/09   cs       CR218239 Use the correct ARFCN for priming AFC loop with band info
09-10-28   tjw      Clean up medium Lint errors
09/10/09   dv       CR188820 - change of name for NAS API
23/09/09   dv       Featurise fix for CR188820
14/09/09   dv       CR188820 - update fix as 1st check-in was not complete
14/09/09   cs       Fix Lint indentation problems
28/08/09   dv       CR188820 - Voice heard before MT call connecte
10/09/09   dv       CR193756 - GCF TC 29.3.3.3 failure
18/08/09   dv       CR 188663 - enable RFACCH F3 during Back to Back FACCH
24/07/09   ws       CR 180476 - Improve performance of Dedicated AGC
07/07/09   dv       Lint Fix
03/07/09   dv       CR188109 - GCF 26.12.4 is failing - L1 not forwarding UA frame to LAPDm
16/06/09   dv       CR185918 - GCF 26.7.5.2 R-FACCH fail
10/06/09   dv       CR185481 - RFACCH not enabled on AMR Channels
28/05/09   dv       CR171098 - Gobi2000 fails TS51.010 14.2.25 and 14.4.31
29/04/09   ws       Fixed Lint Warnings for MOB environment
04/03/09   dv       CR176933 - Call mute after ending Loopback C Test
06/04/09   dv       CR172437:Distortion at far end while answering MT call
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
23/02/09   dv       CR169424:Cannot make MO/MT calls
17/02/09   ap       As part of CR 172492 request to output "FACCH transmitted" as hex characters
22/01/08   dv       CR168236-Correction to previous checkin.
05/01/08   dv       CR168236-Incorrect handling of Channel Types in EMR
23/12/08   ws       CR 167008 Added missing call to gl1_msg_clear_esaic_iir_filter()
                    for all half rate channels.
02/12/08   cs       Added in new gl1_hw_clk_ctl.h include from gl1_hw.h
03/12/08   ws       Added FEATURE_GSM_DYNAMIC_SAIC
12/01/08   gj       CR165617 - mainline FEATURE_GSM_GPRS_ENHANCED_MEASUREMENT_REPORTING feature.
27/10/08   dv       CR160511:R-FACCH design should filter UI filler frames
27/10/08   cs       Fix more HIGH Lint errors
03/10/08   cs       Fix HIGH Lint errors
15/09/08   cs       CR130550 Add in the ability to request a higher/lower clock speed if req'd
                    under the FEATURE_GPRS_USES_HIGH_ARM9_CLOCK
12/09/09   dv       CR154518 - Fix for BAD CRC count going -ve.
26/08/08   dv       Introduced new HS for new TCHFS IF on 8k
26/08/08   hv       Fixed ALIGN usage to cater for non-L4/ARM targets
28/07/08   ws       Re-introduced Eng mode API
28/07/08   ws       Added FEATURE_GSM_8K_QDSP6_AUDIO for chanding Endian of homing sequence
28/07/08   ws       backed out Eng mode API
12/05/08   dv       Added FEATURE_GSM_AMR_WB - L1 component
11/04/08   ws       CR 143418 - Corrected scheduling of Uplink SID frames for
                    TCH half rate. Added gl1_include_in_rxlev() to exclude BCCH for
                    sub channel 0 Rxlev SUB averaging on TCH/H
27/03/08   nf       Added NV Item handling to R-SACCH and R-FACCH
20/03/08   agv      Fixed the LCU PMIC Band setup for a Channel Change.
27/02/08   cs       Use an access function in gl1_hw for the g2wIsrLockOut checks
14/02/08   cs       CR136944 Use the new g2wIsrLockOut to ensure no mdsp stall when
                    disabling the WSTMR in dedicated
05/02/08   og       Ensure ftick notification is called every frame in the ded
                    init state.
14/01/08   cs       Allow for separate IQ logging and saving
14/12/07   ws       merged LCU Bringup and demo changes from 6260 branch
05/12/07   dv       enable RFACCH block to be Rx'd straight after back-to-back FACCHs
27/11/07   nt       Removed compiler warnings
27/11/07   nt       CR111687 - Fix for ISR overrun problem.
09/11/07   nf       Added FEATURE_GSM_R_SACCH
26/10/07   cs       Decouple CHIPXN_DIV4 changes from l1_sc.h
21/09/07   cs/ip    CR117884 Added FEATURE_GSM_DISABLE_CHIPXN_DIV4 control
02/10/07   og       Clean up of unused code.
28/09/07   ws       Complete fix for CR 126612 Don't wait for block boundary if we have an ST. We
                    may have to send acces bursts in midle of block implemented on FR and HR.
25/09/07   ws       CR 128421 - Don't Transmit Meas report on SACCH if we are currently
                    sending Handover Access bursts
17/09/07   agv      Added I/Q logging to code base.
31/08/07   agv      CR127468 Fix for SAIC TT handling for a TCH.
14/09/07   ws       CR 126612 Don't wait for block boundary if we have an ST. We
                    may have to send acces bursts in midle of block
21/09/07   cs/ip    CR117884 Added FEATURE_GSM_DISABLE_CHIPXN_DIV4 control
08/07/07   ws       CR - 124046 Open loopback mode C when we need to send FACCH
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
12/07/07   agv      CR123023 - revert unsafe change.
06/07/07   agv      CR123023 - Update fix for AFC corruption.
27/06/07   agv      Fix for TT. Only use validated SACCH metrics for TT until
                    SID, and FACCH change ready.
04/06/07   dv       Removed compiler warning
23/05/07   nt       CR118485: Stops the SDCCH rx/tx whilst aborting, and alligns
                    new channels on block boundary.
01/05/07   nf       Additional RFACCH debug to reset counters at call init.
30/04/07   dv       Correct RFACCH bug when dealing with back/back FACCH blks
20/04/07   cs       CR117035 Report HR DTX correctly for SACCH Measurement reports
27/03/07   dv       Added FEATURE_GSM_R_FACCH
28/11/06   dv       Lint cleanup. Removed sacch51_tx_in_progress as redundant
27/11/06   og       Remove un-necessary arithmetic modulo operations.
30/10/06   og       EDTM initialisation support for half rate channels.
21/09/06   og       Reset only the cs part of lower layers when connecting DTM
                    assignment in packet transfer (EDTM route).
14/08/06   ws       Allow dedicated to be timeslot slammed to lowest received tn
                    and also to place the CS timeslot on TN 2
06/04/06   av       Feature SAIC MIMO elements added to the burst metrics.
03/20/06   ws       Removed conditional check for sending FACCH decode to L2.
02/17/06   ws       Added additional CRC TCH profiling for SAIC debugging
11/16/05   ws       More Lint Cleanup
09/10/05   dp       Fix to call l1_sci_monscan_next_period_ded() a frame
                    earlier so that measurements get shipped upto RR immediately
08/19/05   gfr      Use gl1_msg_init_frame_tick_info to init the ftn structure
08/19/05   ws       Added setting AFC_TT_MODE if DTM is enabled
08/16/05   dp       Lint cleanup
08/12/05   ws       Added initial FEATURE_GSM_DTM support
07/29/05   gfr      Support for NPL1 DTM interface changes
06/24/05   dp       Fix to exclude BCCH carrier from RXLEV average if its in
                    the hopping list and the PWRC bit is set
06/08/05   dp       Moved aborts after FTN so that we can abort as soon as
                    NPL1 is done aborting
05/25/05   dp       Renamed FNmod13 to not conflict with global.   Put checks
                    around access burst decrementing.  Fixed bug in voc init
                    in HR
04/22/05   gfr      Align tx data on a word boundary to optimize NPL1 processing.
04/21/05   sv       Add support for L1 "Panic Reset"
03/30/05   pp       Call l1_start_ftn_fr_hr() after aborts - fixes merge error
03/29/05   dp       InterRAT audio optimizations.  Fix for CSD issue where
                    csd_rx_rpt was NULL for first block
03/15/05   dp       Added support for enhanced measurement reporting
03/11/05   sv       Mainlined FEATURE_SCE.
01/24/05   pp       Restructured L1_control_full_rate and L1_control_half_rate using
                    l1_tch_amr.c as a reference. Changed calls to
                    l1_log_transmit_burst_metrics() to conform to new interface
01/05/05   dp       Merged dedicated timeline changes for audio optimizations
                    to mainline
11/01/04   yhong    Call gl1_msg_frame_tick_notification() after aborts
08/18/04   yhong    Merged tch abort calls
08/18/04   dp       Fixed merge error
08/16/04   dp       Fixed bug in HR uplink SID Processing
08/16/04   gsc      Adjust timing based on the SACCH bursts timing offset in
                    dedicated mode.
08/14/04   dp       Fixed bug in TAF setting for SID frames.   Fix to send out
                    complete SID block(8 frames ) on transition from non DTX to
                    DTX
08/12/04   gsc      Added support for Loopback I.
06/24/04   dp       Fixed half rate FACCH bug ( compute burst_fn mod 104 in
                    HR callback )
05/26/04   sv       Changed "l1_isrtsk_blk" structure parameters for better
                    code readability.
04/20/04   dp       Fix to align the VFR just once on a new channel
03/31/04   dp       Fix to not send null FACCH data pointer in CSD
03/31/04   dp       Fixed fluctuating TXLEV issue (introduced by linting)
03/22/04   gfr      Detect if too many SACCH decodes fail and RR does not react.
03/18/04   gsc      Lint Cleanup.
03/12/04   dp       Align VFR and enable MVS on first SACCH or idle frame
                    whichever comes earlier
02/17/04   gfr      AFC cleanup.
01/30/04   dp       Merged changes to inform SCE about which frame is the TCH
                    idle frame (for ncell SCH scheduling)
01/30/04   dp       Merged TT fix for 16.1(Leopard).  Apply TT correction every
                    SACCH block.
01/20/04   dp       Fix to not switch off and switch on the vocoder across
                    inter mode GSM handovers ( merely mvs_enable )
01/12/03   gsc      In L1_control_full_rate and L1_control_half_rate, return
                    a valid boolean value if the speech version is V3 (AMR).
11/24/03   gsc      Made first_rx_block global to drop first block received
                    on the D/L.
11/11/03   dp       Fix to disable vocoderon old channel during handover
                    only if channel mode changes
11/05/03   gfr      Support for quad-band.
11/05/03   gsc      Removed enabling/disabling DTX in L1_control_full_rate
                    and L1_control_half_rate.
10/22/03   gsc      Moved receiving of vocoder packet below a call to
                    l1_control_amr_<fr, hr>.
10/03/03   dp       If L1 is in control of the vocoder, disable the vocoder
                    on TCH tear down and re-enable after first aligning the
                    VFR on the idle frame (Audio fix for Samsung )
09/04/03   gsc      Updated rx_callbacks to drop the first block received on
                    the D/L.
08/15/03   dp       Moved decrementing of number of access bursts outside
                    L1_more_abs_to_send to fix issues with incorrect number
                    of access bursts.   Implemented new CSD RXQual algorithm
08/11/03   gsc      Migrated CR30848 fix "screech heard when vocoder not
                    initialized" to Half Rate.
07/25/03   dp       Installed ded monitor callback
07/11/03   gfr      Lint cleanup.
07/10/03   gsc      Enable/Disable DTX based on DTX_Indicator in channel
                    specification.
06/24/03   jc/bm    CR30848 fix screech heard when vocoder not initialized.
06/23/03   gfr      Compiler warning cleanup.
05/27/03   dp       Fix for mdsp errors during HR handover to subchannel 1
05/07/03   gsc      Added AMR support
05/02/03   gw       Merged in power transfer changes from branch.
04/14/03   dp       Added check to see if vocoder is initialized.   If not
                    we ignore the dtx flag and transmit anyways
04/02/03   gw       Changes to reflect modifications in monscan module.
03/25/03   gsc      Fix for Half Rate processing where we were RACHing
                    during incorrect TDMA frame.
03/24/03   gsc      Fix for Half Rate processing where a wrong offset and
                    increment were being passed to L1_get_ARFCNS().
02/25/03   dp       Added HR support featurized under
                    FEATURE_GSM_HALF_RATE and enabled by default
01/24/03   atm      Logging cleanup (Added TXLEV/TA Logging)
01/02/03   gw       Changed timing of sending serving cell and neighbor cell
                    measurements to RR.
11/22/02   mk        Added multiband AFC support
11/20/02   dp       Removed CSD workaround to not transmit a FACCH in the
                    first 6 blocks.  We now have a fix in gl1_msg_tch.c
10/21/02   dp       Fix to start transmitting SACCH bursts right after we
                    start the TCH even if they form an incomplete block
10/16/02   dp       Added support to update SACCH ARFCNs midway through a
                    block if frequency redefinition occurs
10/01/02   ATM      Moved central get and set FN routines to l1_utils
09/18/02   dp       Passing in frequency redef parameters to L1_get_ARFCNS
                    to guarantee a seamless transition between the before and
                    after channels in the case of the start time lying in the
                    middle of a block
09/05/02   dp       Put in CSD workaround for multiple SABM issue. This is
                    a hack and we need to revisit this issue
08/29/02   JC       Obsoleted call to L1_map_txlev_to_power_index(); buried
                    mapping in msg layer to support multi-band operation.
                    Also, modified usage of gl1_msg_tx_rach().
08/21/02   dp       Fix to not schedule monitors when aborting
07/26/02   dp       Support to avg measurements ovr 2 SACCH periods for
                    ranking cells
07/11/02   gw       Changed references to power_dBm_x16 in metrics reports to
                    pwr_dBm_x16.
07/03/02   dp       Fix to not abort the channel on a FACCH block boundary.
                    We send a "completing" TCH block to fill the FACCH
                    interleaver with 8 bursts
06/14/02   dp       Changes to meet the 20ms constraint between last burst of
                    last complete block on old channel and first burst on new
                    channel
06/10/02   dp       Support to stop sending measurement reports when commanded
                    by L2
06/10/02   dp       Passing in before and after freq parameters to L1_get_ARFCNS
                    to guarantee a seamless transition between the before and
                    after channels in the case of the start time lying in the
                    middle of a block.
06/06/02   mk       Support for aborts on all channels.
05/17/02   dp       Support to pass on TAF flag to vocoder. Also to transmit
                    SID on transition from DTX off to on.
05/20/02   JC       Mainlined DED_METRICS and added SID bursts into the
                    RXLEV_SUB calculations.  Also, added all TCH bursts in
                    RXLEV_FULL calcualtions.  Added sob check on RXQUAL for
                    mk.
05/14/02   dp       Fix to transmit a TCH block after every FACCH block to
                    ensure that there are 8 bursts for the FACCH interleaver
05/14/02   mk       Modified RXQUAL_FULL and _SUB averaging.
05/08/02   JC       Divied up metrics logging s.t. burst and message metrics
                    are logged separately for reliability.
04/30/02   mk       Added direct frequency correction support to TCH.
04/24/02   dp       Fix for TXLEV reporting on SACCH.  We now report the TXLEV
                    of the last burst of the previous reporting period
04/19/02   ATM      Adding support for LOG_GSM_RECEIVE_METRICS_C packet
04/19/02   dp       Pulled in Gabe's fix to prevent TCH abort from calling
                    gl1_msg_terminate_tch and sc_abort multiple times.  Fix
                    to not transmit handover access bursts on the SACCH
04/16/02   dp       Set channel type to GL1_MSG_DCCH for signaling only
03/29/02   dp       BFI changes.  Fix for sync rx seq num mismatch when transitioning
                    from TCH to idle
03/28/02   JC/dp    Temp put in l1_sci_do_monscan_ded till we consolidate with idle
03/25/02   dp       Fix to transmit handover access burst only on the SACCH
                    and FACCH.  Previously it was also being transmitted in
                    the idle frame
03/20/02   dp       Added support for power control in steps of 2dB on TCH.
                    TXLEV to power index changes.   Separated SACCH metrics
                    callbacks for 51 and 26 multiframes since direct freq
                    correction is done only on the TCH for now
03/18/02   jc       Added support for FEATURE_TCH_FIXED_PATTERN_TEST.
03/15/02   mk       Added support for direct frequency correction.
03/06/02   dp       Support to frequency hop access bursts
03/05/02   JC       Mainline sacch_metrics_callback for RXLEV reporting.
                    Some cleanup. Moved dedicated_report to metrics_callback.
02/28/02   mk       Added initial support for RXQUAL_FULL and RXQUAL_SUB
                    measurements.
02/20/02   dp       Added support for handovers
02/14/02   gw       Added calls to gl1_msg_reset() to support AGC.
                    Changed DED_SC to FEATURE_SCE.
02/13/02   jc       Add support for DTX.
01/31/02   gw       Improved stopping of SCE.
02/05/02   JC       Changes to support AGC.
02/01/02   mk       Updated to support latest afc/tt algorithms.
01/16/02   mk       Added support for afc/tt.
12/18/01   dp       Added support for aborting power measurements and SC receives
                    before leaving the TCH
12/12/01   jc       Added call to gl1_msg_terminate_tch();
12/11/01   gw       Changes to match new interface for getting debug (logging)
                    data.
12/07/01   mk       Introduced STOP/ABORT to tch, facch and sacch26.
11/27/01   mk       Disabled ncell monitoring and scell metrics, logging for now.
                    Added rx (tx) counters to sacch51 callbacks. Misc cleanup.
11/9/01    bk       Updated NPL1 FTN block info to conform to the interface change
11/02/01   mk       Featurized TIMETEST_PORT.
10/31/01   dp/gw    Incorporated changes due to new TCH_FACCH decode_rpt format.
                    Added support for Rx metrics, featurized with DED_METRICS
                    and support for logging featurized by DED_LOG.
10/24/01   dp       Implemented surround cell functionality and added support
                    for monitoring
10/10/01   gw       Added support for message/frame layer Ncell reception.
09/25/01   mk       Non-supported SCE code featurized using OLD_SCE.
09/20/01   jc       Converted to message/frame layer interface.
08/31/01   JC       Placed temporary COMMENT blocks around dsp_xxx vars and
                    Pu_xxx vars.  Also, removed KOVU compile flag & added
                    std QCT fcn headers.
08/24/01   jc       changed rx sig strength to -95 to accomodate changes in
                    hw_gsm.c
07/26/01   JC       Added support for SIGNALLING_ONLY mode.  Updated hw_
                    APIs to include message length information to
                    retrieve decoded data.  Bug fix on sending fill frame
                    to L2.
07/17/01   jc       Removed extraneous hw_tx_tch and hw_tx_sacch that were
                    incorrectly added.
06/04/01   jc       Add in changes to support facch,sacch.
05/28/01   thh      Pass in the TSC when we call hw_init_for_tch() as required
                    by the new interface.
05/13/01   JC       Commented out hw_pa_on calls since these functions
                    are doing nothing.
05/07/01   jc       Redid function hw_rx_tch().Correct problem with setting
                    to the wrong channel on the frame before idle.
04/24/01   jc       Initial version (Sina emulaton board)

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "memory.h"
#include "l1i.h"
#include "l1_ds.h"
#include "l1_ds_v.h"
#include "l1_ds_v_g.h"
#include "l1_isr.h"
#include "l1_utils.h"
#include "l1_amr.h"
#include "geran_msgs.h"
#include "l1_sc.h"
#include "l1_task.h"
#include "l1_log.h"
#include "l1_sacch.h"
#include <stringl/stringl.h>

#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_clk_ctl.h"
#include "gl1_hw_debug.h"
#include "gl1_voice_server.h"

#include "geran_eng_mode_info.h"

#include "ftm.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#include "gl1_msg_wtr_hop.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#include "geran_dual_sim_g.h"

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif

/* Enables SC functionality on the TCH */
#undef DED_LOG
#undef TCH_PROFILING
#define TCH_PROFILING

#undef RXQUAL_MSG

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
#define TCH_FS_DATA_WIDTH_OCTETS 34             /* (260/8) per GSM 5.03 */
#define TCH_HS_DATA_WIDTH_OCTETS 14

#define TCH_NUM_FRAMES_BETWEEN_BLOCKS_R_FACCH_MIN   8
#define TCH_NUM_FRAMES_BETWEEN_BLOCKS_R_FACCH_MAX   9
#define TCH_MAX_NUM_FRAMES_BETWEEN_BACK_TO_BACK_FACCH_BLKS 5

#ifdef FEATURE_GSM_COEX
#define MAX_COEX_DENIALS_FOR_FACCH 2
#endif

typedef struct
{
  cch_decode_rpt           sacch_rpt;       // decoded data goes here
  tch_facch_decode_rpt     tch_rpt;         // decoded data goes here
  // sacch and sdcch can share the same
  // rpt since they occur far apart

  gl1_msg_metrics_rpt      sacch_metrics_rpts[4];
  gl1_msg_metrics_rpt      tch_metrics_rpts[4];
  gl1_msg_rx_rpt           s_rpt;
  gl1_msg_rx_rpt           t_rpt;

#ifdef FEATURE_DATA_GCSD
  tch_facch_decode_rpt     *csd_rx_rpt;
  uint8                    *csd_tx_ptr;
  uint16                   csd_rx_block_count;
  uint8                    CSD_FACCH_Buffer;
#endif // FEATURE_DATA_GCSD

  /*---------------------------------------------------------------------------
   * Various other globals used for Full/Half Rate processing
   *-------------------------------------------------------------------------*/

  /* Globals used to store the the position of sacch, idle, tch etc frames
   * in the 26 frame multi-frame structure.
   */
  uint8                    sacch_minus_one_frame;
  uint8                    idle_minus_one_frame;
  uint8                    idle_frame;   /* idle frame (in FN mod 26) */
  uint8                    send_sacch;
  uint8                    reporting_period_end_minus_1;
  uint8                    afc_correction_frame;

  boolean                  facch_not_available;
  boolean                  dtx;
  boolean                  prev_dtx;
  boolean                  vfr_aligned;
  uint8                    packet_available;

  boolean                  prev_block_was_facch;
  int8                     prev_block_was_facch_counter;
  uint16                   num_sids_pending;
  boolean                  first_rx_block;

  gl1_msg_chan_type        channel_type;
  gl1_msg_voc_type         voc_type;
  boolean                  idle_status;

  /* Number of SC power measurements to schedule at a time */
  uint8                    num_pwr_meas_to_schedule;
  uint8                    num_pwr_meas_per_frame;

  ARFCN_T                  arfcns[4];

#if defined (__GNUC__)
  uint8                    tx_data[TCH_FS_DATA_WIDTH_OCTETS] __attribute__((aligned(16)));
  uint8                    voc_tx_data[TCH_FS_DATA_WIDTH_OCTETS] __attribute__((aligned(16)));
#else
  ALIGN16 uint8            tx_data[TCH_FS_DATA_WIDTH_OCTETS];
  ALIGN16 uint8            voc_tx_data[TCH_FS_DATA_WIDTH_OCTETS];
#endif

  uint16                   data_length;
  int16                    txlev[4];
  boolean                  block_start;
  boolean                  prepare_msg;
  sys_algo_agc_T           *agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
  sys_algo_agc_T           *agc_ptr_divrx;
#endif
  boolean                  access_burst_scheduled;
  boolean                  sacch_ul_block_start;
  boolean                  enable_loopback_after_facch_block;
  uint16                   facch_blk_count;

  boolean                  x2g_ho_indicator;

  boolean                  r_facch_support_detected;
  boolean                  r_facch_nv_flag_active;
  /* Array to store the fn (mod42432) of possible facch
   * repetitions */
  uint16                    possible_rfacch_fn[2];
  /* Index to the buffer used by firmware to store the 
   * soft bit of the failed facch */
  uint8                    facch_soft_buffer_index;
#ifdef DEBUG_FEATURE_GSM_R_FACCH
  #error code not present
#endif

  /*---------------------------------------------------------------------------
   * Full Rate processing specific globals
   *-------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_GCSD
  boolean                  data_services;
#endif /* FEATURE_DATA_GCSD */

  /*---------------------------------------------------------------------------
   * Half Rate processing specific globals
   *-------------------------------------------------------------------------*/
  boolean                  facch_tx_allowed;
  boolean                  facch_rx_expected;
  boolean                  middle_facch_block;
  int                      hr_facch_tx_in_progress;
  uint8                    half_rate_subchannel;
#ifdef FEATURE_VAMOS_II
  boolean                  middle_facch_block_vamos2;
#endif

  boolean                  l1_rsacch_nv_flags_advance_mode;
#ifdef DEBUG_FEATURE_GSM_R_SACCH
  #error code not present
#endif
  boolean                  tx_homing_sequence;
  eng_mode_chan_type_t     chan_mode;

  boolean                  sacch_tx_started_fr;
  uint8                    vfr_align_frame_fr;

  boolean                  sacch_tx_started_hr;
  uint8                    vfr_align_frame_hr;

#ifdef TCH_PROFILING
  uint16                   crc_cnt_fr;
  uint16                   crc_bad_fr;
  uint16                   bfi_cnt_fr;
  uint16                   total_crc_cnt_fr;
  uint16                   total_crc_bad_fr;
  uint16                   total_bfi_cnt_fr;

  uint16                   crc_cnt_hr;
  uint16                   crc_bad_hr;
  uint16                   bfi_cnt_hr;
  uint16                   total_crc_cnt_hr;
  uint16                   total_crc_bad_hr;
  uint16                   total_bfi_cnt_hr;

  uint16                   crc_cnt_rx_cb;
  uint16                   crc_bad_rx_cb;
#endif

  uint16                   bep[4];
  uint8                    bep_index;

  uint16                   previousFacchData_FN;
#ifdef DEBUG_FEATURE_GSM_R_FACCH
  #error code not present
#endif
  uint8                    previousFacchData[3];

  garb_request_tx_t        coex_request_tx;

  uint8                    coex_denials_for_facch;
} l1_tch_data_t;

static l1_tch_data_t l1_tch_data[NUM_GERAN_DATA_SPACES];


#ifdef FEATURE_DATA_GCSD
 static void cstch_rx_callback(gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
#endif // FEATURE_DATA_GCSD



/* Keep track of how many sequential SACCH failures we get to make sure
   RR is still paying attention */
#define L1_MAX_FAILED_SACCH_DECODES 360
//extern int l1_bad_sacch_block_cnt;

#ifdef FEATURE_QDSP6_ADSP_MDSP_RADIO_FUSION
/* On Fusion targets ADSP & MODEM are on Q6. Therefore
 * memcpy is used & IF1 re-ordering removed from MDSP */
static uint16 fr_homing_sequence[17] =
{
  0x0a82, 0x152e, 0x00a5, 0x9204, 0x2449, 0x4592, 0x0400, 0x4992,
  0x9224, 0x0045, 0x9204, 0x2449, 0x4592, 0x0400, 0x4992, 0x9224, 0x0040
};

static uint16 efr_homing_sequence[17] =
{
  0x5E08, 0x90B4, 0xADFA, 0x3E60, 0x183A, 0x7B60, 0x420C, 0x80C0,
  0x0548, 0x0058, 0x0000, 0x0000, 0xB036, 0x0000, 0x0000, 0x0000, 0x0000
};

static uint16 hr_homing_sequence[7] =
{
  0x7103, 0x61AF, 0xF2C8, 0x2580, 0xC031, 0x0000, 0x0000
};
#else
static uint16 fr_homing_sequence[17] =
{
  0x4820, 0xD617, 0x0284, 0x2480, 0x9249, 0x8924, 0x8002, 0x4924,
  0x2492, 0x0289, 0x2480, 0x9249, 0x8924, 0x8002, 0x4924, 0x2492, 0x0009
};

static uint16 efr_homing_sequence[17] =
{
  0x085E, 0xB490, 0xFAAD, 0x603E, 0x3A18, 0x607B, 0x0C42, 0xC080,
  0x4805, 0x5800, 0x0000, 0x0000, 0x36B0, 0x0000, 0x0000, 0x0000, 0x0000
};

static uint16 hr_homing_sequence[7] =
{
  0x0371, 0xaf61, 0xc8f2, 0x8025, 0x31c0, 0x0000, 0x0000
};
#endif /* FEATURE_QDSP6_ADSP_MDSP_RADIO_FUSION */

/*---------------------------------------------------------------------------
 * All of the following are common to Full and Half rate
 *-------------------------------------------------------------------------*/

static void l1_recompute_sacch_arfcns_fr_hr(dedicated_data_T       *ded_dtr_ptr, gas_id_t gas_id);
static boolean l1_process_sacch_fr_hr(dedicated_data_T       *ded_dtr_ptr,
                                   channel_information_T  *chnl1_info_ptr, gas_id_t gas_id);
static void l1_process_idle_fr_hr( gas_id_t gas_id );
static void l1_start_ftn_fr_hr( gas_id_t gas_id );
static void facch_tx_callback( gas_id_t gas_id );
static void tch_tx_callback( gas_id_t gas_id );
static void l1_schedule_monitors_fr_hr( gas_id_t gas_id );
static void l1_process_sce_pwr_data_fr_hr( gas_id_t gas_id );
extern void l1_sc_set_tch_idle_frame( uint8 idle_frame, gas_id_t gas_id  );
extern void l1_sc_check_if_next_cm_search_is_lte(gas_id_t gas_id);

extern dedicated_data_T l1_dedicated_data[];

void gl1_store_block_rx_power ( gl1_msg_metrics_rpt *rpt[], uint8 bursts_per_block, gas_id_t gas_id );
void gl1_update_emr_parameters (gl1_defs_rx_hdr_struct *rpt_hdr, boolean is_sid_fn, boolean compute_bep, gas_id_t gas_id );
boolean gl1_is_sid_fn ( uint16 fn, gas_id_t gas_id );

#ifdef DED_LOG
#error code not present
#endif
#ifdef FEATURE_GSM_AMR
extern void gl1_amr_setup_loopback_I(gas_id_t gas_id);
extern void gl1_amr_reset_loopback_I(gas_id_t gas_id);
#endif
extern void l1_call_panic_reset_in_task( l1_panic_reset_type_T error_code, gas_id_t gas_id );

extern boolean gl1_include_in_rxlev ( ARFCN_T arfcn, gas_id_t gas_id );


static  void    l1_rsacch_dl_process_block      (cch_decode_rpt *rpt, gas_id_t gas_id);
static  boolean l1_rsacch_nv_flags_active       (gas_id_t gas_id);
 #ifdef DEBUG_FEATURE_GSM_R_SACCH
#error code not present
#endif

#ifdef FEATURE_GSM_COEX_SW_CXM
static void l1_tch_get_arfcns(int16 start, uint16 increment, uint16 num_arfcns, ARFCN_T arfcns[], gas_id_t gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

/*---------------------------------------------------------------------------
 * All of the following are Full Rate specific
 *-------------------------------------------------------------------------*/
static void l1_setup_timeslot_params_fr(channel_information_T *chnl1_info_ptr, gas_id_t gas_id);

#ifdef FEATURE_VAMOS_II
static void l1_setup_timeslot_params_vamos2_fr(channel_information_T *chnl1_info_ptr, gas_id_t gas_id);
#endif /*FEATURE_VAMOS_II*/

static void l1_setup_message_layer_fr(dedicated_data_T       *ded_dtr_ptr,
                                      channel_information_T  *chnl1_info_ptr, gas_id_t gas_id);
static void l1_process_rach_fr(dedicated_data_T       *ded_dtr_ptr,
                               channel_information_T  *chnl1_info_ptr, gas_id_t gas_id);
static void l1_process_tch_fr( dedicated_data_T       *ded_dtr_ptr,
                               channel_information_T  *chnl1_info_ptr, gas_id_t gas_id );
static void l1_tch_abort_fr(gas_id_t gas_id);
static void tch_fr_rx_callback(gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
void tch_fr_metrics_callback( gl1_msg_metrics_rpt *rpt[], 
                              uint16 chan,
                              gas_id_t gas_id );

/*---------------------------------------------------------------------------
 * All of the following are Half Rate specific
 *-------------------------------------------------------------------------*/
static void l1_setup_timeslot_params_hr(channel_information_T *chnl1_info_ptr, gas_id_t gas_id);

#ifdef FEATURE_VAMOS_II
static void l1_setup_time_slot_params_vamos2_hr(channel_information_T *chnl1_info_ptr, gas_id_t gas_id);
#endif /*FEATURE_VAMOS_II*/

static void l1_setup_message_layer_hr(dedicated_data_T       *ded_dtr_ptr,
                                      channel_information_T  *chnl1_info_ptr, gas_id_t gas_id);
static void l1_process_rach_hr(dedicated_data_T       *ded_dtr_ptr,
                               channel_information_T  *chnl1_info_ptr, gas_id_t gas_id);
static void l1_process_tch_hr(dedicated_data_T       *ded_dtr_ptr,
                              channel_information_T  *chnl1_info_ptr, gas_id_t gas_id);
static void l1_tch_abort_hr(gas_id_t gas_id);
static void tch_hr_rx_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
void tch_hr_metrics_callback( gl1_msg_metrics_rpt *rpt[], 
                              uint16 chan,
                              gas_id_t gas_id );


/*===========================================================================

FUNCTION L1_tch_init_data_struct

DESCRIPTION
  This function initialiase the l1_tch_data structures

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  initialiase l1_tch_data

===========================================================================*/
void L1_tch_init_data_struct( gas_id_t gas_id )
{
  int j;

  // Initialiase s_rpt and t_rpt
  l1_tch_data[gas_id].s_rpt.data = (gl1_msg_decode_rpt *) &l1_tch_data[gas_id].sacch_rpt;
  for (j = 0; j < GL1_DEFS_FRAMES_IN_BLK_PERIOD; j++)
    l1_tch_data[gas_id].s_rpt.metrics[j] = &l1_tch_data[gas_id].sacch_metrics_rpts[j];

  l1_tch_data[gas_id].t_rpt.data = (gl1_msg_decode_rpt *) &l1_tch_data[gas_id].tch_rpt;
  for (j = 0; j < GL1_DEFS_FRAMES_IN_BLK_PERIOD; j++)
    l1_tch_data[gas_id].t_rpt.metrics[j] = &l1_tch_data[gas_id].tch_metrics_rpts[j];


#ifdef FEATURE_DATA_GCSD
  l1_tch_data[gas_id].csd_rx_rpt                           = NULL;
  l1_tch_data[gas_id].csd_tx_ptr                           = NULL;
  l1_tch_data[gas_id].CSD_FACCH_Buffer                     = 0;
#endif // FEATURE_DATA_GCSD

  /*---------------------------------------------------------------------------
   * Various other globals used for Full/Half Rate processing
   *-------------------------------------------------------------------------*/

  /* Globals used to store the the position of sacch, idle, tch etc frames
   * in the 26 frame multi-frame structure.
   */
  l1_tch_data[gas_id].dtx                                  = FALSE;
  l1_tch_data[gas_id].prev_dtx                             = FALSE;
  l1_tch_data[gas_id].vfr_aligned                          = FALSE;

  l1_tch_data[gas_id].prev_block_was_facch_counter         = 0;
  l1_tch_data[gas_id].num_sids_pending                     = 0;
  l1_tch_data[gas_id].first_rx_block                       = TRUE;

  /* Number of SC power measurements to schedule at a time */

  l1_tch_data[gas_id].enable_loopback_after_facch_block    = FALSE;
  l1_tch_data[gas_id].facch_blk_count                      = 0;

  l1_tch_data[gas_id].x2g_ho_indicator                        = FALSE;

  l1_tch_data[gas_id].r_facch_support_detected             = FALSE;
  l1_tch_data[gas_id].r_facch_nv_flag_active               = FALSE;
  l1_tch_data[gas_id].possible_rfacch_fn[0]                = 0xFFFF;
  l1_tch_data[gas_id].possible_rfacch_fn[1]                = 0xFFFF;
  l1_tch_data[gas_id].facch_soft_buffer_index              = 0;
#ifdef DEBUG_FEATURE_GSM_R_FACCH
  #error code not present
#endif

  /*---------------------------------------------------------------------------
   * Full Rate processing specific globals
   *-------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_GCSD
  l1_tch_data[gas_id].data_services                        = FALSE;
#endif // FEATURE_DATA_GCSD

  /*---------------------------------------------------------------------------
   * Half Rate processing specific globals
   *-------------------------------------------------------------------------*/
  l1_tch_data[gas_id].hr_facch_tx_in_progress              = 0;
  l1_tch_data[gas_id].half_rate_subchannel                 = 0;

#ifdef DEBUG_FEATURE_GSM_R_SACCH
  #error code not present
#endif
  l1_tch_data[gas_id].tx_homing_sequence                   = FALSE;
  l1_tch_data[gas_id].chan_mode                            = ENG_MODE_INVALID_CHAN_MODE;

  l1_tch_data[gas_id].sacch_tx_started_fr                  = FALSE;
  l1_tch_data[gas_id].sacch_tx_started_hr                  = FALSE;
  l1_tch_data[gas_id].bep_index                            = 0;

  l1_tch_data[gas_id].previousFacchData_FN                 = 0xFFFF;
  l1_tch_data[gas_id].previousFacchData[0]                 = 0xFE;
  l1_tch_data[gas_id].previousFacchData[1]                 = 0xFE;
  l1_tch_data[gas_id].previousFacchData[2]                 = 0xFE;

#ifdef FEATURE_GSM_COEX
  l1_tch_data[gas_id].coex_denials_for_facch               = 0;
#endif
}

/*===========================================================================

FUNCTION L1_CONTROL_FULL_RATE

DESCRIPTION
  This function controls the traffic channel,

DEPENDENCIES
  pointer to dedicated data
  pointer to channel 1 info
  pointer to channel 2 info

RETURN VALUE
  TRUE  - processing is complete and mode may be changed
  FALSE - this function should be called again on the next frame tick

SIDE EFFECTS
  None

===========================================================================*/
boolean  L1_control_full_rate ( dedicated_data_T       *ded_dtr_ptr,
                                channel_information_T  *chnl1_info_ptr,
                                boolean                 apply_freq_redef, gas_id_t gas_id
                              )
{
   boolean get_voc_data = FALSE;
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION
   static uint8 no_vocoder_change_after_handover;
#endif
   /*
   * Initialize global flags.
   */
   l1_tch_data[gas_id].idle_status            = FALSE;
   l1_tch_data[gas_id].block_start            = FALSE;
   l1_tch_data[gas_id].prepare_msg            = FALSE;
   l1_tch_data[gas_id].access_burst_scheduled = FALSE;
   l1_tch_data[gas_id].sacch_ul_block_start   = FALSE;

   if (!ded_dtr_ptr || !chnl1_info_ptr)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
               ded_dtr_ptr, chnl1_info_ptr);
      return FALSE;
   }

   l1_tch_data[gas_id].agc_ptr = &ded_dtr_ptr->c0_agc;

#ifdef FEATURE_GSM_RX_DIVERSITY
   l1_tch_data[gas_id].agc_ptr_divrx = &ded_dtr_ptr->agc_divrx;
#endif

   gas_id = check_gas_id(gas_id);

   switch ( l1_tskisr_blk->sub_sub_state )
   {
#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM

      case L1_EDTM_INIT:

#endif
#endif

      case L1_DED_INIT :
         l1_acch_nv_flag_initialise(gas_id);
         l1_tch_set_rfacch_fw_buffer_index(0, gas_id);
         clear_rfacch_decode_frame(0, gas_id);
         clear_rfacch_decode_frame(1, gas_id);

         l1_tch_data[gas_id].first_rx_block = TRUE;
         l1_tch_data[gas_id].voc_type = GL1_MSG_FS;

#ifdef FEATURE_GSM_MDSP_ESAIC
         if ( l1_dedicated_data_ptr->handover_info.gl1_inter_handover_in_progress == TRUE)
         {
            if(l1_dedicated_data_ptr->handover_info.gl1_inter_handover_fail == TRUE )
            {
               /* Handover failed*/
               gl1_msg_clear_esaic_iir_filter(TRUE,FALSE, gas_id);
               l1_dedicated_data_ptr->handover_info.gl1_inter_handover_fail = FALSE;
               l1_dedicated_data_ptr->handover_info.gl1_inter_handover_in_progress = FALSE;
            }
            else
            {
              /* Handover success*/
              gl1_msg_clear_esaic_iir_filter(TRUE, TRUE, gas_id);
            }
            l1_dedicated_data_ptr->handover_info.gl1_inter_handover_in_progress = FALSE;

         }
         else
         {
           /* If this is not an inter-vell handover then no need
             * to indicate to GFW
             */
           gl1_msg_clear_esaic_iir_filter(FALSE, FALSE, gas_id);
         }


#endif

/* Exclude as PMIC signaling is only needed for Solo. */


/* For Poseidon enable cipher clock for mDSP */
         if (gl1_msg_is_ciphering_enabled(gas_id))
         {
             gl1_hw_cipher_clock_required(TRUE, gas_id);
         }

         /*
          * Convert channel_type and channel_mode to CHT parameters for
          * the DSP
          */

         if ( chnl1_info_ptr->channel_type == FULL_RATE_TRAFFIC )
         {
            switch ( ded_dtr_ptr->channel_specification.channel_mode_1 )
            {
               case SPEECH_V1:
#ifdef FEATURE_VAMOS_II
               case SPEECH_V1_VAMOS2:
#endif
                  l1_tch_data[gas_id].channel_type = GL1_MSG_TCH;
                  MSG_GERAN_MED_0_G("Tch init: FR voice");
                  l1_tch_data[gas_id].voc_type = GL1_MSG_FS;
                  l1_tch_data[gas_id].chan_mode = ENG_MODE_FULL_RATE;
#ifdef FEATURE_DATA_GCSD
                  l1_tch_data[gas_id].data_services = FALSE;
#endif // FEATURE_DATA_GCSD
                  break;


               case SPEECH_V2:
#ifdef FEATURE_VAMOS_II
               case SPEECH_V2_VAMOS2:
#endif
                  l1_tch_data[gas_id].channel_type = GL1_MSG_TCH;
                  MSG_GERAN_MED_0_G("Tch init: EFR voice");
                  l1_tch_data[gas_id].voc_type = GL1_MSG_EFS;
                  l1_tch_data[gas_id].chan_mode = ENG_MODE_ENHANCED_FULL_RATE;
#ifdef FEATURE_DATA_GCSD
                  l1_tch_data[gas_id].data_services = FALSE;
#endif // FEATURE_DATA_GCSD
                  break;

#ifdef FEATURE_GSM_AMR
               case SPEECH_V3:
#ifdef FEATURE_VAMOS_II
               case SPEECH_V3_VAMOS2:
#endif
#ifdef FEATURE_GSM_AMR_WB
               case SPEECH_V5:
#ifdef FEATURE_VAMOS_II
               case SPEECH_V5_VAMOS2:
#endif
#endif
                  l1_tch_data[gas_id].chan_mode = ENG_MODE_AMR_FULL_RATE;
                  return L1_control_amr_fr( ded_dtr_ptr, chnl1_info_ptr,
                                            apply_freq_redef, gas_id );
#endif /* FEATURE_GSM_AMR */

               case SIGNALLING_ONLY:
                  l1_tch_data[gas_id].channel_type = GL1_MSG_DCCH;
#ifdef FEATURE_DATA_GCSD
                  l1_tch_data[gas_id].data_services = FALSE;
#endif // FEATURE_DATA_GCSD
                  break;

#ifdef FEATURE_DATA_GCSD
               case DATA_9600:
                  l1_tch_data[gas_id].data_services = TRUE;
                  l1_tch_data[gas_id].channel_type = GL1_MSG_TCH_F96;
                  break;

               case DATA_14400:
                  l1_tch_data[gas_id].data_services = TRUE;
                  l1_tch_data[gas_id].channel_type = GL1_MSG_TCH_F144;
                  break;
#endif // FEATURE_DATA_GCSD

               default:
                  MSG_GERAN_ERROR_1_G("Unsupported TCH channel mode %d",
                            ded_dtr_ptr->channel_specification.channel_mode_1);

                  break;
            }
            /* Log the eng_mode_info for CM stats. Nothing to do with AMR here*/
            {
              eng_mode_amr_info_t amr_info = {0};
              uint8 i = 0;

              amr_info.ul_chan_mode = (uint8)l1_tch_data[gas_id].chan_mode;
              amr_info.dl_chan_mode = (uint8)l1_tch_data[gas_id].chan_mode;

              for (i = 0; i < MAX_CODEC_MODES; i++)
              {
                amr_info.ul_acs[i] = (uint8)0xFF;
                amr_info.dl_acs[i] = (uint8)0xFF;
              }

              amr_info.dl_dtx = FALSE;
              amr_info.dl_c_over_i = (uint16)0xFFFF;
              geran_eng_mode_data_write(ENG_MODE_AMR_INFO, (void *)&amr_info, TRUE, gas_id);

            }
         }

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
         gl1_msg_set_iq_state( TRUE, gas_id );
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES || FEATURE_GSM_LOG_IQ_SAMPLES */

#ifdef FEATURE_DATA_GCSD
         if ( l1_tch_data[gas_id].data_services )
         {
            /*********** Add data services code *********************/
            l1_tch_data[gas_id].csd_rx_rpt = NULL;
            l1_tch_data[gas_id].csd_tx_ptr = NULL;
            l1_tch_data[gas_id].csd_rx_block_count = 0;
            l1_tch_data[gas_id].CSD_FACCH_Buffer = 0;

            /* Get a buffer from the DS layer.  The CSD buffers passed
             * into NPL1 are allocated by the DS layer
             */
            gl1_msg_cs_data_rx( NULL, &l1_tch_data[gas_id].csd_rx_rpt, (int)GCSD_NO_BUFFER, gas_id );
         }
#endif // FEATURE_DATA_GCSD
#ifdef FEATURE_VAMOS_II
         /* If RR indicates that UE has to operate is VAMOS 2 mode and if TSC set 2 is allocated,
          * the SACCH position is shifted as per 3GPP 45.002, section 7, Table 1a. 
          */
         if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3))
         {
            l1_setup_timeslot_params_vamos2_fr(chnl1_info_ptr, gas_id);
         }
         else
#endif /*FEATURE_VAMOS_II*/
         {
           l1_setup_timeslot_params_fr(chnl1_info_ptr, gas_id);
         }
         l1_tch_data[gas_id].prev_block_was_facch = FALSE;
         l1_tch_data[gas_id].prev_block_was_facch_counter =0;
         l1_tch_data[gas_id].num_sids_pending = 0;
         l1_tch_data[gas_id].vfr_aligned = FALSE;
         l1_tch_data[gas_id].sacch_tx_started_fr = FALSE;
         l1_tch_data[gas_id].enable_loopback_after_facch_block = FALSE;
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION		 
         no_vocoder_change_after_handover = FALSE;
#endif

#ifdef FEATURE_GSM_COEX_SW_CXM
         /* Notify the arbitrator of the channel assignment */
         garb_intf_notify_cs_assignment(
            (l1_tskisr_blk->l1_state == L1_DTM_MODE ? GL1_DEFS_TN_2 : GL1_DEFS_TN_0),
            chnl1_info_ptr->frequency_list,
            chnl1_info_ptr->subchannel,
            TRUE,
            FALSE,
            l1_tch_data[gas_id].sacch_minus_one_frame+1,
            l1_tch_data[gas_id].idle_frame,
            l1_tch_get_arfcns,
           &l1_tch_data[gas_id].coex_request_tx,
            l1_tskisr_blk->l1_state,
            gas_id
         );
#endif /* FEATURE_GSM_COEX_SW_CXM */

         /* Pass idle frame to SCE - for ncell scheduling */
         l1_sc_set_tch_idle_frame( l1_tch_data[gas_id].idle_frame, gas_id );

         l1_setup_message_layer_fr(ded_dtr_ptr, chnl1_info_ptr, gas_id);

         /* Install callback to trigger serving cell measurement reporting. */
         l1_sci_monscan_set_next_period_ded_cb( L1_send_dedicated_report, gas_id );


         MSG_GERAN_MED_1_G("Tch msg init FNmod42432 = %d", frame_counters[gas_id].FNmod42432);

         l1_schedule_monitors_fr_hr(gas_id);

         if ( frame_counters[gas_id].FNmod13 >= 9  ||
              frame_counters[gas_id].FNmod13 <= 0 )
         {
            /* VFR will be aligned to frame 4 and 20000 of 13M count*/
            l1_tch_data[gas_id].vfr_align_frame_fr = 3;
         }
         else if ( frame_counters[gas_id].FNmod13 >= 1  &&
                   frame_counters[gas_id].FNmod13 <= 4 )
         {
            /* VFR will be aligned to frame 8 and 40000 of 13M count*/
            l1_tch_data[gas_id].vfr_align_frame_fr = 7;
         }
         else
         {
            /* VFR will be aligned to frame 0 and 0 of 13M count*/
            l1_tch_data[gas_id].vfr_align_frame_fr = 12;
         }

         if (( l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_STARTED ) ||
              ( l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_READY ))
          {
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION
            /*If no change in vocoder id, do not stop the vocoder and align VFR in immediate possible frame */
            if(l1_vs_change_in_voc(l1_tch_data[gas_id].channel_type,l1_tch_data[gas_id].voc_type, gas_id) )
            {
              l1_vs_stop_client(gas_id);				
            }
            else
            {
              no_vocoder_change_after_handover = TRUE;

              if( frame_counters[gas_id].FNmod13 == 3   ||
                  frame_counters[gas_id].FNmod13 == 7   ||
                  frame_counters[gas_id].FNmod13 == 12  )
              {
                gl1_msg_align_vfr( TRUE, gas_id );
                l1_tch_data[gas_id].vfr_aligned = TRUE;
                MSG_GERAN_MED_1_G("Aligning Real VFR FNmod13=%d",frame_counters[gas_id].FNmod13);
              }
              else if ( frame_counters[gas_id].FNmod13 >= 12  ||
                        frame_counters[gas_id].FNmod13 <= 2 )
              {
                /* VFR will be aligned to frame 4 and 20000 of 13M count*/
                l1_tch_data[gas_id].vfr_align_frame_fr = 3;
              }
              else if ( frame_counters[gas_id].FNmod13 >= 3  &&
                        frame_counters[gas_id].FNmod13 <= 6 )
              {
                /* VFR will be aligned to frame 8 and 40000 of 13M count*/
                l1_tch_data[gas_id].vfr_align_frame_fr = 7;
              }
              else
              {
                /* VFR will be aligned to frame 0 and 0 of 13M count*/
                l1_tch_data[gas_id].vfr_align_frame_fr = 12;
              }   
            }
#else
              l1_vs_stop_client(gas_id);
#endif 
         }

         l1_tskisr_blk->sub_sub_state  = L1_DED_RUNNING;

#if  defined (DEBUG_FEATURE_GSM_R_FACCH)
         #error code not present
#endif
         /* End the Channel Init here so the DED_RUNNING is run on the next frame
           tick, otherwise this may result in possible ISR Overrun. */
         l1_start_ftn_fr_hr(gas_id);

         break;

      case L1_DED_RUNNING:

#ifdef FEATURE_GSM_AMR

         if ( ( ded_dtr_ptr->channel_specification.channel_mode_1 == SPEECH_V3 )
#ifdef FEATURE_VAMOS_II
              ||( ded_dtr_ptr->channel_specification.channel_mode_1 == SPEECH_V3_VAMOS2 )
#endif
#ifdef FEATURE_GSM_AMR_WB
              || ( ded_dtr_ptr->channel_specification.channel_mode_1 == SPEECH_V5 )
#ifdef FEATURE_VAMOS_II
              || ( ded_dtr_ptr->channel_specification.channel_mode_1 == SPEECH_V5_VAMOS2 )
#endif
#endif
            )
         {
            return L1_control_amr_fr( ded_dtr_ptr, chnl1_info_ptr,
                                      apply_freq_redef, gas_id );
         }
#endif

#ifdef FEATURE_VAMOS_II
         if((gl1_hw_get_vamos2_support(gas_id)) &&
            ((chnl1_info_ptr->TSC & 0x8) >> 3) && (l1_tch_data[gas_id].sacch_minus_one_frame == 12))
         {
           if((frame_counters[gas_id].FNmod13 == 3) || (frame_counters[gas_id].FNmod13 == 7)
               ||(frame_counters[gas_id].FNmod26 == 11) || (frame_counters[gas_id].FNmod26 == 25))
           {
             get_voc_data = TRUE;
           }
         }
         else
#endif /*FEATURE_VAMOS_II*/
         {
           if((frame_counters[gas_id].FNmod13 == 3) || (frame_counters[gas_id].FNmod13 == 7)
               ||(frame_counters[gas_id].FNmod13 == 12))
           {
             get_voc_data = TRUE;
           }
         }

         /* See if the vocoder has a packet ready */
         if (get_voc_data)
         {
           l1_tch_data[gas_id].prev_dtx = l1_tch_data[gas_id].dtx;
           l1_tch_data[gas_id].dtx = l1_vs_get_gsm_packet_from_client( l1_tch_data[gas_id].voc_tx_data, 
                                                                        sizeof(l1_tch_data[gas_id].voc_tx_data), 
                                                                        &l1_tch_data[gas_id].packet_available, gas_id );

           if (l1_tch_data[gas_id].dtx)
           {
             gl1_set_dtx_used_flag(gas_id);
           }

           if ( ( l1_tch_data[gas_id].dtx == TRUE ) && ( l1_tch_data[gas_id].prev_dtx == FALSE ) )
           {
             MSG_GERAN_MED_1_G("Entering DTX fn=%d",frame_counters[gas_id].FNmod104);
             l1_tch_data[gas_id].num_sids_pending = 2;
           }

           /* Transmit homing sequence if no packet available or if
            * the vocoder is not ready after a handover
            */
           if ( l1_tch_data[gas_id].packet_available == FALSE
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
               || ( !l1_vs_is_client_ready(gas_id) )
#endif
              )
           {
             switch ( l1_tch_data[gas_id].voc_type )
             {
               case GL1_MSG_EFS:
                 memscpy( l1_tch_data[gas_id].voc_tx_data, sizeof(l1_tch_data[gas_id].voc_tx_data), efr_homing_sequence, TCH_FS_DATA_WIDTH_OCTETS );
                 MSG_GERAN_MED_3_G("Tx EFR homing seq %x %x %x",l1_tch_data[gas_id].voc_tx_data[0],l1_tch_data[gas_id].voc_tx_data[1],l1_tch_data[gas_id].voc_tx_data[2]);
                 l1_tch_data[gas_id].tx_homing_sequence = TRUE;
                 break;

               case GL1_MSG_FS:
                 memscpy( l1_tch_data[gas_id].voc_tx_data, sizeof(l1_tch_data[gas_id].voc_tx_data), fr_homing_sequence, TCH_FS_DATA_WIDTH_OCTETS );
                 MSG_GERAN_MED_3_G("Tx FR homing seq %x %x %x",l1_tch_data[gas_id].voc_tx_data[0],l1_tch_data[gas_id].voc_tx_data[1],l1_tch_data[gas_id].voc_tx_data[2]);
                 l1_tch_data[gas_id].tx_homing_sequence = TRUE;
                 break;

               default:
                 MSG_GERAN_ERROR_1_G("Undefined voc type %d for full rate",l1_tch_data[gas_id].voc_type);
                 l1_tch_data[gas_id].tx_homing_sequence = FALSE;
                 break;
             }

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
             if(l1_tch_data[gas_id].tx_homing_sequence)
             {
                 if(!l1_vs_is_client_ready(gas_id))
                 {
                   gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_TX_HOMING_SEQ_VOC_NOT_READY);
                 }
                 else
                 {
                   gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_TX_HOMING_SEQ_VOC_READY);
                 }
             }
#endif

           }
           else
           {
             l1_tch_data[gas_id].tx_homing_sequence = FALSE;
           }
         }

         /*
           If frequency redefinition occurs midway through a SACCH block,
           we need to recompute the SACCH ARFCNs based on the new parameters
           and inform the msg layer.  We don't do this for the TCH because
           they are contiguous and this scenario is extremely unlikely.  We
           need to recompute because the ARFCNs are pre-computed before the
           block starts
         */
         if ( apply_freq_redef )
         {
            l1_recompute_sacch_arfcns_fr_hr(ded_dtr_ptr, gas_id);
         }

         /* Check up front to prevent Tx TCH from being scheduled in sacch/idle - 1 */
         if ( L1_more_abs_to_send(ded_dtr_ptr, FACCH_F, gas_id) )
         {
            l1_tch_data[gas_id].access_burst_scheduled = TRUE;
         }

         if ( frame_counters[gas_id].FNmod26 == l1_tch_data[gas_id].sacch_minus_one_frame )
         {
            if ( l1_tskisr_blk->main_command != L1_DEDICATED_STOP )
            {
               /* Transmit a sacch and encode if it is time and if data is ready */
               /* We normally start transmitting only on a SACCH block
                  boundary.  However to pass burst level frequency sampling
                  in 11.10 test cases, we need to transmit an incomplete
                  block after we get onto a new channel and haven't reached
                  the SACCH block start
               */
               if ( frame_counters[gas_id].FNmod104 == l1_tch_data[gas_id].send_sacch ||
                    !l1_tch_data[gas_id].sacch_tx_started_fr )
               {
                  /* Only setup sacch_tx flags if we have scheduled SACCH TX */
                  if(l1_process_sacch_fr_hr(ded_dtr_ptr, chnl1_info_ptr, gas_id) == TRUE)
                  {
                    /*
                       Once an incomplete or complete SACCH block is started,
                       sacch_tx_started is set to TRUE and is reset only
                       when we get onto another channel.  sacch_ul_block
                       start indicates the start of an incomplete or complete
                       block.  The old indicator was ticking the handler only
                       on a SACCH block boundary
                    */
                    l1_tch_data[gas_id].sacch_ul_block_start = TRUE;
                    l1_tch_data[gas_id].sacch_tx_started_fr = TRUE;
                  }
               }
            }
         }
         else if ( frame_counters[gas_id].FNmod26 != l1_tch_data[gas_id].idle_minus_one_frame )
         {
            /*
             * If it is not sacch - 1 frame and is not idle - 1 frame,
             * then see if have to process a RACH.
             */
            if ( l1_tskisr_blk->main_command != L1_DEDICATED_STOP )
            {
               l1_process_rach_fr(ded_dtr_ptr, chnl1_info_ptr, gas_id);
            }


            /* If it is not sacch - 1 frame and is not idle - 1 frame, check
             * to see if it is an idle frame.
             */
            if ( frame_counters[gas_id].FNmod26 == l1_tch_data[gas_id].idle_frame )
            {
               l1_process_idle_fr_hr(gas_id);
            }

            /*
             * If it is not sacch - 1 frame and is not idle - 1 frame, check
             * to see if it is an idle - 1 - 1 frame.
             */
            if ( frame_counters[gas_id].FNmod26 == (l1_tch_data[gas_id].idle_minus_one_frame - 1) )
            {
               //Tick the surrounding cell engine to schedule the rx
               l1_sci_tick_receive(gas_id);

            }

            /*Check if G2L search can happen on next idle frame*/
            if ( frame_counters[gas_id].FNmod26 == (l1_tch_data[gas_id].idle_minus_one_frame - 4) )
            {
              l1_sc_check_if_next_cm_search_is_lte(gas_id);
            }

            /*
             * If it is not sacch - 1 frame and is not idle - 1 frame, check
             * to see if it is an idle - 1 - 3 frame.
             */
            if ( frame_counters[gas_id].FNmod26 == (l1_tch_data[gas_id].idle_minus_one_frame - 3) )
            {
               l1_process_sce_pwr_data_fr_hr(gas_id);
            }

         }

         /* Check if the VFR needs aligning */
         if ( frame_counters[gas_id].FNmod13 == l1_tch_data[gas_id].vfr_align_frame_fr && 
              !l1_tch_data[gas_id].vfr_aligned )
         {
            /* Check vocoder is stopped before aligning VFR */
            if ( (l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_OPENED)
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION				
				|| (TRUE == no_vocoder_change_after_handover )
#endif
			    )
            {
               gl1_msg_align_vfr( TRUE, gas_id );
               l1_tch_data[gas_id].vfr_aligned = TRUE;
               MSG_GERAN_MED_1_G("Aligning Real VFR FNmod13=%d",frame_counters[gas_id].FNmod13);
            }
            else
            {
              MSG_GERAN_MED_1_G("Vocoder is not stopped, aligning VFR postponed FNmod13=%d",
                                                                         frame_counters[gas_id].FNmod13);
             /* If vocoder is not stopped, calculate when is the 
              * next oportunity to align the vocoder */
              switch (l1_tch_data[gas_id].vfr_align_frame_fr)
              {
                case 3:
                  l1_tch_data[gas_id].vfr_align_frame_fr  = 7;
                  break;

                case 7:
                  l1_tch_data[gas_id].vfr_align_frame_fr  = 12;
                  break;

                case 12:
                  l1_tch_data[gas_id].vfr_align_frame_fr  = 3;
              }
            }
         }

         /* Check if vocoder needs to be enabled */
         if ( l1_tch_data[gas_id].vfr_aligned &&
              l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_OPENED &&
              l1_tch_data[gas_id].channel_type == GL1_MSG_TCH )
         {
            MSG_GERAN_MED_1_G("Starting vocoder FNmod13=%d",frame_counters[gas_id].FNmod13);

            l1_vs_start_client(l1_tch_data[gas_id].channel_type,
                               l1_tch_data[gas_id].voc_type, gas_id);
         }

         /*
          * Now process the traffic frame.
          */
         l1_process_tch_fr(ded_dtr_ptr, chnl1_info_ptr, gas_id);

         /*******************************************************
          *
          *  Process the Surrounding Cell power data
          *  N.B. must be before l1_sci_monscan_tick() call!!!!
          ********************************************************/
         /*
           instead of calling immediately before l1_sci_monscan_tick(),
           l1_sci_monscan_next_period() is called one frame earlier, after
           l1_sci_monscan_tick() to reduce the runtime of the next frame
         */

         /* GW added an extra -1 because reporting_period_end_minus_1 is
          * actually set to reporting period end.
          * l1_sci_monscan_next_period() must be called before the end
          * of the reporting period otherwise the ncell measurement
          * report is sent to RR about 26 frames too late.
          */
         if ( frame_counters[gas_id].FNmod104 == l1_tch_data[gas_id].reporting_period_end_minus_1 - 1 )
         {
            l1_sci_monscan_next_period_ded(gas_id);
         }

         /*
          *   TT / AFC correction
          */
         if ( frame_counters[gas_id].FNmod104 == l1_tch_data[gas_id].afc_correction_frame )
         {
            gl1_msg_dch_tt_adjust(gas_id);
         }

         l1_tch_data[gas_id].idle_status = FALSE;

         /* Don't abort till the extra TCH block to complete the FACCH has been
            sent out
         */
         if ( l1_tskisr_blk->main_command == L1_DEDICATED_STOP )
         {
            l1_dedicated_data_ptr->no_of_ab_to_send = 0;
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
            // In case of GPLT and no RF, return default/canned result
            MSG_GERAN_HIGH_0_G("GPLT DBG:  calling l1_tch_abort_fr();");
            l1_tch_abort_fr(gas_id);
#else
            if ( !l1_tch_data[gas_id].prev_block_was_facch )
            {
               /* Abort after FTN will save us a frame during channel changes
                * since we can abort as soon as NPL1 is done aborting.
                * Important from an audio perspective
                */
               l1_tch_abort_fr(gas_id);
            }
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
         }

         /* If idle_status flag is set to ture then TCH will be aborted. While running Tch abort no need to process FTN*/
         if( l1_tch_data[gas_id].idle_status == FALSE )
         {
           /* FTN processing */
            l1_start_ftn_fr_hr(gas_id);
         }

         break;

      default:
         break;
   } /* of switch */


   return(l1_tch_data[gas_id].idle_status);

} /* end of function */



/*===========================================================================
FUNCTION l1_setup_time_slot_params_fr

DESCRIPTION
   This function sets up the time slot specific parameters for Full Rate
   traffic channel.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_setup_timeslot_params_fr ( channel_information_T  *chnl1_info_ptr,
                                          gas_id_t gas_id )
{
   if ( !chnl1_info_ptr )
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL chnl1_info_ptr");
      return;
   }

   switch ( (chnl1_info_ptr->TN) & 7 )
   {
      case 0:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 11;
         l1_tch_data[gas_id].idle_minus_one_frame         = 24;
         l1_tch_data[gas_id].idle_frame                   = 25;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 103;
         l1_tch_data[gas_id].send_sacch                   = 11;
         l1_tch_data[gas_id].afc_correction_frame         = 89;
         break;

      case 1:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 24;
         l1_tch_data[gas_id].idle_minus_one_frame         = 11;
         l1_tch_data[gas_id].idle_frame                   = 12;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 12;
         l1_tch_data[gas_id].send_sacch                   = 24;
         l1_tch_data[gas_id].afc_correction_frame         = 102;
         break;

      case 2:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 11;
         l1_tch_data[gas_id].idle_minus_one_frame         = 24;
         l1_tch_data[gas_id].idle_frame                   = 25;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 25;
         l1_tch_data[gas_id].send_sacch                   = 37;
         l1_tch_data[gas_id].afc_correction_frame         = 89;
         break;

      case 3:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 24;
         l1_tch_data[gas_id].idle_minus_one_frame         = 11;
         l1_tch_data[gas_id].idle_frame                   = 12;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 38;
         l1_tch_data[gas_id].send_sacch                   = 50;
         l1_tch_data[gas_id].afc_correction_frame         = 102;
         break;

      case 4:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 11;
         l1_tch_data[gas_id].idle_minus_one_frame         = 24;
         l1_tch_data[gas_id].idle_frame                   = 25;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 51;
         l1_tch_data[gas_id].send_sacch                   = 63;
         l1_tch_data[gas_id].afc_correction_frame         = 89;
         break;

      case 5:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 24;
         l1_tch_data[gas_id].idle_minus_one_frame         = 11;
         l1_tch_data[gas_id].idle_frame                   = 12;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 64;
         l1_tch_data[gas_id].send_sacch                   = 76;
         l1_tch_data[gas_id].afc_correction_frame         = 102;
         break;

      case 6:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 11;
         l1_tch_data[gas_id].idle_minus_one_frame         = 24;
         l1_tch_data[gas_id].idle_frame                   = 25;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 77;
         l1_tch_data[gas_id].send_sacch                   = 89;
         l1_tch_data[gas_id].afc_correction_frame         = 89;
         break;

      case 7:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 24;
         l1_tch_data[gas_id].idle_minus_one_frame         = 11;
         l1_tch_data[gas_id].idle_frame                   = 12;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 90;
         l1_tch_data[gas_id].send_sacch                   = 102;
         l1_tch_data[gas_id].afc_correction_frame         = 102;
         break;

      default:
         break;
   }

    L1_set_reporting_period(l1_tch_data[gas_id].reporting_period_end_minus_1, 104, gas_id);
}

#ifdef FEATURE_VAMOS_II
/*===========================================================================
FUNCTION l1_setup_timeslot_params_vamos2_fr

DESCRIPTION
   This function sets up the time slot specific parameters for Full Rate
   traffic channel in case of VAMOS 2, TSC set 2.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_setup_timeslot_params_vamos2_fr ( channel_information_T  *chnl1_info_ptr, gas_id_t gas_id )
{
   if ( !chnl1_info_ptr )
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL chnl1_info_ptr");
      return;
   }

   MSG_GERAN_HIGH_1_G("TN:%d",((chnl1_info_ptr->TN) & 7));

   switch ( (chnl1_info_ptr->TN) & 7 )
   {
      case 0:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 12;
         l1_tch_data[gas_id].idle_minus_one_frame         = 24;
         l1_tch_data[gas_id].idle_frame                   = 25;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 103;
         l1_tch_data[gas_id].send_sacch                   = 12;
         l1_tch_data[gas_id].afc_correction_frame         = 89;
         break;

      case 1:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 23;
         l1_tch_data[gas_id].idle_minus_one_frame         = 11;
         l1_tch_data[gas_id].idle_frame                   = 12;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 12;
         l1_tch_data[gas_id].send_sacch                   = 23;
         l1_tch_data[gas_id].afc_correction_frame         = 102;
         break;

      case 2:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 12;
         l1_tch_data[gas_id].idle_minus_one_frame         = 24;
         l1_tch_data[gas_id].idle_frame                   = 25;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 25;
         l1_tch_data[gas_id].send_sacch                   = 38;
         l1_tch_data[gas_id].afc_correction_frame         = 89;
         break;

      case 3:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 23;
         l1_tch_data[gas_id].idle_minus_one_frame         = 11;
         l1_tch_data[gas_id].idle_frame                   = 12;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 38;
         l1_tch_data[gas_id].send_sacch                   = 49;
         l1_tch_data[gas_id].afc_correction_frame         = 102;
         break;

      case 4:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 12;
         l1_tch_data[gas_id].idle_minus_one_frame         = 24;
         l1_tch_data[gas_id].idle_frame                   = 25;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 51;
         l1_tch_data[gas_id].send_sacch                   = 64;
         l1_tch_data[gas_id].afc_correction_frame         = 89;
         break;

      case 5:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 23;
         l1_tch_data[gas_id].idle_minus_one_frame         = 11;
         l1_tch_data[gas_id].idle_frame                   = 12;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 64;
         l1_tch_data[gas_id].send_sacch                   = 75;
         l1_tch_data[gas_id].afc_correction_frame         = 102;
         break;

      case 6:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 12;
         l1_tch_data[gas_id].idle_minus_one_frame         = 24;
         l1_tch_data[gas_id].idle_frame                   = 25;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 77;
         l1_tch_data[gas_id].send_sacch                   = 90;
         l1_tch_data[gas_id].afc_correction_frame         = 89;
         break;

      case 7:
         l1_tch_data[gas_id].sacch_minus_one_frame        = 23;
         l1_tch_data[gas_id].idle_minus_one_frame         = 11;
         l1_tch_data[gas_id].idle_frame                   = 12;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = 90;
         l1_tch_data[gas_id].send_sacch                   = 101;
         l1_tch_data[gas_id].afc_correction_frame         = 102;
         break;

      default:
         break;
   }
}
#endif /*FEATURE_VAMOS_II*/

/*===========================================================================
FUNCTION l1_setup_message_layer_fr

DESCRIPTION
   This function sets up the Message layer for Full Rate traffic channel.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_setup_message_layer_fr(dedicated_data_T       *ded_dtr_ptr,
                                      channel_information_T  *chnl1_info_ptr, gas_id_t gas_id
                                     )
{
   volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   gl1_msg_tx_cb_type     tx_cb;
   gl1_msg_rx_cb_type     rx_cb;

   if ( !ded_dtr_ptr || !chnl1_info_ptr )
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
                ded_dtr_ptr, chnl1_info_ptr);
      return;
   }

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM

   if( L1_EDTM_INIT == l1_tskisr_blk->sub_sub_state )
   {
     gl1_msg_reset_cs_only(gas_id);
   }
   else
#endif
#endif
   {
     gl1_msg_reset(gas_id);
   }

   gl1_msg_init_tch(l1_tch_data[gas_id].channel_type, l1_tch_data[gas_id].voc_type, gas_id);

   tx_cb.data_cb    = tch_tx_callback;
   tx_cb.metrics_cb = NULL;
   tx_cb.log_cb     = NULL;
   tx_cb.abort_cb   = NULL;
   rx_cb.data_cb    = tch_fr_rx_callback;
#ifdef FEATURE_DATA_GCSD
   if ( l1_tch_data[gas_id].data_services )
   {
      rx_cb.data_cb    = cstch_rx_callback;
   }
#endif // FEATURE_DATA_GCSD
   rx_cb.tch_metrics_cb = tch_fr_metrics_callback;
#ifdef DED_LOG
   #error code not present
#else
   rx_cb.log_cb     = NULL;
#endif
   rx_cb.abort_cb   = NULL;
   gl1_msg_cfg_ul_tch(chnl1_info_ptr->TSC, &tx_cb, gas_id );

   tx_cb.data_cb    = facch_tx_callback;
   gl1_msg_cfg_ul_facch(chnl1_info_ptr->TSC, &tx_cb, gas_id );

   gl1_msg_cfg_dl_tch_facch(chnl1_info_ptr->TSC, &rx_cb, gas_id );

   tx_cb.data_cb    = sacch_tx_callback;
   tx_cb.abort_cb   = NULL;
   rx_cb.data_cb    = sacch_rx_callback;
   rx_cb.metrics_cb = sacch26_metrics_callback;
#ifdef DED_LOG
   #error code not present
#else
   rx_cb.log_cb     = NULL;
#endif
   rx_cb.abort_cb   = NULL;

   gl1_msg_cfg_ul_sacch(chnl1_info_ptr->TSC, &tx_cb, gas_id );
   gl1_msg_cfg_dl_sacch(chnl1_info_ptr->TSC, &rx_cb, gas_id );

#ifdef FEATURE_GSM_DTM
   if(l1_tskisr_blk->l1_state != L1_DTM_MODE)
   {
     /* Only setup AFC_TT_MODE to DCH if we are not in DTM
      * the main DTM handler gpl1_dtm.c will set this instead
      * based on type of lowest allocated timeslot
      */
     gl1_msg_set_afc_tt_mode(AFC_TT_MODE_DCH, gas_id);
   }
#endif

   gl1_msg_dch_afc_tt_init( chnl1_info_ptr->frequency_list.channel[0], gas_id );

   l1_tch_data[gas_id].idle_status = FALSE;

   /* Precompute TXLEVS for first reporting period.  Transmit at the
      command power level immediately as soon as we jump onto the new
      channel. ms_power_level_to_use is initially set to ms_power_level_in_use
      ,so L1_precompute_txlev will not step up/step down the power
      The 2dB step algo kicks in only after the next power
      level change commanded by the base station
   */

   L1_precompute_txlev( ded_dtr_ptr->ms_power_level_to_use,
                        ded_dtr_ptr->ms_power_level_in_use,
                        104,
                        chnl1_info_ptr->frequency_list.channel[0],
                        gas_id
                      );
}

/*===========================================================================
FUNCTION l1_schedule_monitors_fr_hr

DESCRIPTION
   This function schedules the monitors for both Full and Half Rate.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_schedule_monitors_fr_hr(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /* When we first get onto the traffic channel
    * we schedule only as many monitors
    * as the number of frames upto the next "idle - 1"
    * starting with the current frame.  Otherwise we
    * schedule 24 monitors starting at "idle + 1".  If
    * we're on the "idle - 1" or "idle" frame wait till
    * the idle frame and schedule 24 monitors
    */

   if ( (frame_counters[gas_id].FNmod26 != l1_tch_data[gas_id].idle_minus_one_frame - 1) &&
        (frame_counters[gas_id].FNmod26 != l1_tch_data[gas_id].idle_minus_one_frame) &&
        (frame_counters[gas_id].FNmod26 != l1_tch_data[gas_id].idle_frame) )
   {
      /* Consider both cases - when idle is at the end of the 26
         multiframe and when its in the middle
      */
      if ( frame_counters[gas_id].FNmod26 < l1_tch_data[gas_id].idle_minus_one_frame )
      {
         /* # of monitors = # of frames upto and not including idle - 1
            Account for the fact that the power measure handler only starts
            a frame later (when we get into DED_RUNNING)
         */
         l1_tch_data[gas_id].num_pwr_meas_to_schedule =
            (l1_tch_data[gas_id].idle_minus_one_frame - frame_counters[gas_id].FNmod26 - 2);
      }
      else
      {
         /* Since we've excluded "idle - 1" and "idle", this condition
            applies only to the case when the idle falls in the middle
            of the 26 multiframe
         */
         l1_tch_data[gas_id].num_pwr_meas_to_schedule =
            L1_SC_DED_MAX_MEAS_PER_MF - (frame_counters[gas_id].FNmod26 - l1_tch_data[gas_id].idle_frame) - 1;
      }

      /* This will handle 0 and negative # of measurements if any */
      if ( l1_tch_data[gas_id].num_pwr_meas_to_schedule )
      {
         /* Schedule monitors immediately if we've just got onto the TCH */
         l1_tch_data[gas_id].num_pwr_meas_per_frame = 1;

         /* Don't schedule monitors if we're aborting */
         if ( l1_tskisr_blk->main_command != L1_DEDICATED_STOP )
         {
            l1_sci_monscan_do_ded(l1_tch_data[gas_id].num_pwr_meas_to_schedule, l1_tch_data[gas_id].num_pwr_meas_per_frame, TRUE, gas_id);
         }
      }
   }
}

/*===========================================================================
FUNCTION l1_recompute_sacch_arfcns_fr_hr

DESCRIPTION
   This function recomputes the sacch ARFCNS for the Full and Half rate.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_recompute_sacch_arfcns_fr_hr( dedicated_data_T       *ded_dtr_ptr, gas_id_t gas_id )
{
   int16 offset;

   if ( !ded_dtr_ptr )
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL ded_dtr_ptr");
      return;
   }

   /* Determine when the SACCH block started relative to curr FN */
   offset = ( l1_tch_data[gas_id].send_sacch + 1 - frame_counters[gas_id].FNmod104 - 104 ) % 104;

   /* Recompute ARFCNs */
   L1_get_ARFCNS ( offset, 26, 4, l1_tch_data[gas_id].arfcns ,
                   &ded_dtr_ptr->channel_specification.channel_info_1_before,
                   &ded_dtr_ptr->channel_specification.channel_info_1_after,
                   &ded_dtr_ptr->new_channel_info, gas_id);

   /* Update msg layer */
   gl1_msg_update_sacch_arfcns( l1_tch_data[gas_id].arfcns, gas_id );
}

/*===========================================================================
FUNCTION l1_process_sacch_fr_hr

DESCRIPTION
   This function processes the Full and Half Rate SACCH channel.

DEPENDENCIES
   None

RETURN VALUE
  Sacch_tx, if true SACCH has been scheduled, FALSE otherwise

SIDE EFFECTS
   None
===========================================================================*/
static boolean l1_process_sacch_fr_hr(dedicated_data_T       *ded_dtr_ptr,
                                   channel_information_T  *chnl1_info_ptr, gas_id_t gas_id
                                  )
{
   boolean sacch_tx = FALSE;
   dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

   if ( !ded_dtr_ptr || ! chnl1_info_ptr )
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
                ded_dtr_ptr, chnl1_info_ptr);
      return(FALSE);
   }

   /*
      Abort SACCH if there was a previous block in process.
      This would happen right after we get onto a new channel
      and have to transmit incomplete SACCH bursts till the
      first complete SACCH block occurs
   */
   /* Abort Tx SACCH */
   gl1_msg_abort_tx_sacch(gas_id);

   (void) L1_get_CCH_message ( TRUE, SACCH, ded_dtr_ptr, l1_tch_data[gas_id].tx_data, &l1_tch_data[gas_id].data_length, gas_id);

   MSG_GERAN_MED_2_G("Meas Rpt: Serv RXLEV=%d,Ncell RXLEV=%d",(l1_tch_data[gas_id].tx_data[7]&0x3F),(l1_tch_data[gas_id].tx_data[10]&0x3F));
   /* Get the next block of ARFCN's and signal strengths */
   /* Passing in before and after freq parameters to L1_get_ARFCNS
      to guarantee a seamless transition between the before and
      after channels in the case of the start time lying in the
      middle of a block.  Not very elegant but the least complicated
      solution for now
   */
   L1_get_ARFCNS ( 1, 26, 4, l1_tch_data[gas_id].arfcns ,
                   &ded_dtr_ptr->channel_specification.channel_info_1_before,
                   &ded_dtr_ptr->channel_specification.channel_info_1_after,
                   &ded_dtr_ptr->new_channel_info, gas_id);

   /*
      The start offset here is relative to the start of the reporting period -
      The first SACCH block is 13 frames from the start of the reporting period
   */

   /* Convert TXLEV to power index before passing to the msg layer */
   L1_get_txlev ( 12, 26, l1_tch_data[gas_id].txlev, gas_id );

   /* Update ded data ptr with most recent power level*/
   ded_dtr_ptr->ms_power_level_in_use = (byte)l1_tch_data[gas_id].txlev[3];

   /* Don't send SACCH if L2 has asked us to stop meas rpt or we are currently
    * transmitting Handover access bursts. We don't have a valid Timing Advance
    * so wait until next SACCH period
    */
   if ( (ded_dtr_ptr->stop_meas_rpt == FALSE) &&
        (l1_tch_data[gas_id].access_burst_scheduled == FALSE) )
   {
      gl1_msg_tx_sacch ( l1_tch_data[gas_id].arfcns, l1_tch_data[gas_id].txlev,
                         l1_tch_data[gas_id].tx_data, gas_id );

      /* Log tx burst metrics */
      l1_log_transmit_burst_metrics
      ( GL1_MSG_SACCH,
        1, 26, 4,
        l1_tch_data[gas_id].arfcns,
        l1_tch_data[gas_id].txlev,
        chnl1_info_ptr->TSC,
        ded_dtr_ptr->timing_advance_in_use, gas_id );

      sacch_tx = TRUE;

   }


   /* Receive only complete SACCH blocks */
   if ( frame_counters[gas_id].FNmod104 == l1_tch_data[gas_id].send_sacch )
   {
      gl1_msg_rx_sacch ( l1_tch_data[gas_id].arfcns, l1_tch_data[gas_id].agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                         l1_tch_data[gas_id].agc_ptr_divrx,
#endif                         
                         &l1_tch_data[gas_id].s_rpt, l1_dedicated_data_ptr->rsacch_ctrl.active,
                         gas_id );
   }

   /* Return schedule status of SACCH TX */
   return(sacch_tx);
}

/*===========================================================================
FUNCTION l1_process_rach_fr

DESCRIPTION
   This function processes the Full Rate RACH channel

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_process_rach_fr( dedicated_data_T       *ded_dtr_ptr,
                                channel_information_T  *chnl1_info_ptr, gas_id_t gas_id
                              )
{
   int i = 0;
   dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

   if ( !ded_dtr_ptr || !chnl1_info_ptr )
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
                ded_dtr_ptr, chnl1_info_ptr);
      return;
   }

   /*
      If there are handover access bursts to be transmitted,
      transmit them in consecutive frames on the FACCH
      (we exclude SACCH and idle frames)
   */
   if ( l1_tch_data[gas_id].access_burst_scheduled )
   {
      /* Decrement # of handover acccess bursts to send */
      if ( ded_dtr_ptr->no_of_ab_to_send > 0 )
      {
         ded_dtr_ptr->no_of_ab_to_send--;
      }
      else
      {
         MSG_GERAN_ERROR_1_G("# of access bursts incorrect %d", ded_dtr_ptr->no_of_ab_to_send);
      }

      /* Get the ARFCN in frequency hopping sequence*/
      /* Passing in before and after freq parameters to L1_get_ARFCNS
         to guarantee a seamless transition between the before and
         after channels in the case of the start time lying in the
         middle of a block.  Not very elegant but the least complicated
         solution for now
      */
      L1_get_ARFCNS ( 1, 1, 4, l1_tch_data[gas_id].arfcns ,
                      &ded_dtr_ptr->channel_specification.channel_info_1_before,
                      &ded_dtr_ptr->channel_specification.channel_info_1_after,

                      &ded_dtr_ptr->new_channel_info, gas_id);
      /*
      Transmit handover access bursts. Single burst, so just
      pass in the first ARFCN in the block
      */
      gl1_msg_tx_rach ( l1_tch_data[gas_id].arfcns[0], l1_dedicated_data_ptr->ms_power_level_to_use,
                        0, l1_dedicated_data_ptr->access_burst_content, gas_id );

      l1_tch_data[gas_id].access_burst_scheduled = TRUE;

      for ( i = 0 ; i < 4; i++ )
      {
         l1_tch_data[gas_id].txlev[i] = l1_dedicated_data_ptr->ms_power_level_to_use;
      }

      /* Log tx burst metrics */
      l1_log_transmit_burst_metrics
      ( GL1_MSG_RACH,
        1, 1, 1,
        l1_tch_data[gas_id].arfcns,
        l1_tch_data[gas_id].txlev,
        chnl1_info_ptr->TSC,
        0 , gas_id);

   }
}

/*===========================================================================
FUNCTION l1_process_idle_fr_hr

DESCRIPTION
   This function performs the Full Rate idle frame processing.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_process_idle_fr_hr(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /* Dedicated mode surround cell processing */
   /* SC monitors(power measurements) are to be done in every
      frame except "idle - 1" and "idle".  Thus at every
      idle, we schedule 24 power measurements
   */
   l1_tch_data[gas_id].num_pwr_meas_to_schedule = L1_SC_DED_MAX_MEAS_PER_MF;
   l1_tch_data[gas_id].num_pwr_meas_per_frame = 1;

#ifdef FEATURE_GSM_WTR_HOP
   if (gl1_msg_wtr_hop_get_status() == WTR_HOP_INACTIVE)
#endif
   {
      gl1_hw_rf_idle_frame_processing(gas_id);
   }

   /* Don't schedule monitors if we're aborting */
   if ( l1_tskisr_blk->main_command != L1_DEDICATED_STOP )
   {
      l1_sci_monscan_do_ded(l1_tch_data[gas_id].num_pwr_meas_to_schedule, l1_tch_data[gas_id].num_pwr_meas_per_frame,TRUE,gas_id);
   }
}

/*===========================================================================
FUNCTION l1_process_sce_pwr_data_fr_hr

DESCRIPTION
   This function processes the Full and Half Rate Surround Cell Power Data

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_process_sce_pwr_data_fr_hr(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /*
     Start in idle - 4 because this runs in the task
     and has to complete before idle - 2 when we tick
     the Ncell handler

     If we're stopping TCH or have access burst scheduled, don't schedule
     surround cell decodes
   */
   gas_id = check_gas_id(gas_id);

   if ( l1_tskisr_blk->main_command != L1_DEDICATED_STOP && !l1_tch_data[gas_id].access_burst_scheduled )
   {
      /*
      Trigger surrounding cell engine.  This kicks off the tone
      detect/SCH decode scheduling in the task.  Has to complete
      before "idle - 2" when we trigger the ncell handler
      */
      l1_sci_tick_dedi_tch(gas_id);
   }
}

/*===========================================================================
FUNCTION l1_process_tch_fr

DESCRIPTION
   The function processes the Full Rate Traffic Channel. It implements
   the TX side implementation to resolve conflict between FACCH and TCH
   frames giving priority to FACCH. This function is also responsible for
   scheduling the frames.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_process_tch_fr( dedicated_data_T       *ded_dtr_ptr,
                               channel_information_T  *chnl1_info_ptr, gas_id_t gas_id
                             )
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#ifdef FEATURE_VAMOS_II
  gl1_vamos2_tch_wait_type vamos2_tch_wait_state = NO_WAIT;
#endif 

   if ( !ded_dtr_ptr || ! chnl1_info_ptr )
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
                ded_dtr_ptr, chnl1_info_ptr);
      return;
   }

#ifdef FEATURE_VAMOS_II
   MSG_GERAN_LOW_3_G("FN:%d,TSC:%d,sacch_minus_one_frame:%d",
                       gl1_get_FN(gas_id),
                       chnl1_info_ptr->TSC,
                       l1_tch_data[gas_id].sacch_minus_one_frame);

   if((gl1_hw_get_vamos2_support(gas_id)) &&
      ((chnl1_info_ptr->TSC & 0x8) >> 3) && (l1_tch_data[gas_id].sacch_minus_one_frame == 12))
   {
     if( (frame_counters[gas_id].FNmod13 == 3)||
        (frame_counters[gas_id].FNmod13 == 7)||
        (frame_counters[gas_id].FNmod26 == 11)|| 
        (frame_counters[gas_id].FNmod26 == 25)
      )
        {
           l1_tch_data[gas_id].prepare_msg = TRUE;
           l1_tch_data[gas_id].block_start = TRUE;
        }
   }
   else
#endif /*FEATURE_VAMOS_II*/
   {
     switch ( frame_counters[gas_id].FNmod13 )
     {
        case 3:
        case 7:
        case 12:
           l1_tch_data[gas_id].prepare_msg = TRUE;
           l1_tch_data[gas_id].block_start = TRUE;
           break;
        default:
           break;
     }
   }

#ifdef FEATURE_VAMOS_II
   if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3))
   {
     if((frame_counters[gas_id].FNmod26 == 11) && (l1_tch_data[gas_id].sacch_minus_one_frame == 12))
     {
       vamos2_tch_wait_state = WAIT_AFTER_BLOCK_START;
     }   
     if((frame_counters[gas_id].FNmod26 == 20) && (l1_tch_data[gas_id].sacch_minus_one_frame == 23))
     {
       vamos2_tch_wait_state = WAIT_AFTER_BURST2;
     }
   }
#endif

   /* Ordered TA and TXLEV is applied only at the start of the next reporting period.
      We precompute TXLEV for the entire reporting period and then retrieve it later
      for each transmitted block.  We do this in reporting_period - 3 because the
      block is prepared and scheduled a few frames before its actually scheduled to
      start
   */
   if ( frame_counters[gas_id].FNmod104 == l1_tch_data[gas_id].reporting_period_end_minus_1 - 2 )
   {
      /* Update the power level and TA to use with that commanded by the base
       * station
       */
      ded_dtr_ptr->ms_power_level_to_use =  ded_dtr_ptr->ordered_ms_power_level;
      ded_dtr_ptr->timing_advance_to_use =  ded_dtr_ptr->ordered_timing_advance;

      /* We need to report the power level of the last burst on the previous
       * SACCH multiframe.   100 would be the start of the last block.
       * 103 would be the idle frame but the specs say we have to consider
       * frames whether actual transmission takes place or not
       */
      L1_get_txlev( 100, 1, l1_tch_data[gas_id].txlev, gas_id );
      ded_dtr_ptr->ms_power_level_to_report = (byte)l1_tch_data[gas_id].txlev[3];

      L1_precompute_txlev( ded_dtr_ptr->ordered_ms_power_level,
                           ded_dtr_ptr->ms_power_level_in_use,
                           104,
                           chnl1_info_ptr->frequency_list.channel[0],
                           gas_id
                         );

      ded_dtr_ptr->timing_advance_in_use =
        L1_change_ta( ded_dtr_ptr->timing_advance_to_use,
                      ded_dtr_ptr->timing_advance_in_use,gas_id );

   }


   if ( l1_tch_data[gas_id].prepare_msg )
   {
      boolean    sending_FACCH  = FALSE;
      boolean    facch_filler_reqd = FALSE;

      /* Initialize tx_data */
      memset (l1_tch_data[gas_id].tx_data, NULL, TCH_FS_DATA_WIDTH_OCTETS);

      /* If previous block was FACCH, we need to send one more TCH block to
         ensure that the interleaver has 8 bursts, so override DEDICATED_STOP
         till the "completing" TCH block has been sent out
      */
      if ( l1_tskisr_blk->main_command != L1_DEDICATED_STOP || l1_tch_data[gas_id].prev_block_was_facch )
      {
         int16 offset = 1;
         uint8 FNrptperiod = 0;

         /* Did we break the loop to send FACCH in last block? If so re-enable
          * loopback */
         if(l1_tch_data[gas_id].enable_loopback_after_facch_block == TRUE)
         {
           /* Wait for interleaving, FACCH iis interleaved over 8 frames so we need to
            * wait for 2 blocks before closing the loop again
            */
           if(++l1_tch_data[gas_id].facch_blk_count == 2)
           {
            l1_tch_data[gas_id].enable_loopback_after_facch_block = FALSE;

            /* If we broke loopback to send a FACCH not related to Opening loopback
             * C, resume loopback
             */
            if(ded_dtr_ptr->l1_loopback_disabled == LOOPBACK_C_TEMP_OPEN)
            {
              ded_dtr_ptr->l1_loopback_disabled = LOOPBACK_C_CLOSED;
              gl1_msg_tch_loopback(GL1_DEFS_LOOPBACK_TYPE_C, gas_id);
            }

            l1_tch_data[gas_id].facch_blk_count = 0;
           }
         }

         /* Is there a facch to transmit */
         if ( ded_dtr_ptr->channel_specification.channel_mode_1 == SIGNALLING_ONLY)
         {
            facch_filler_reqd = TRUE;
         }

         /*
          * if there is new FACCH data or if it is a
          * SIGNALLING only channel then send DSP a tx facch command
          */
#ifdef FEATURE_GSM_COEX
         if( L1_get_CCH_message( facch_filler_reqd, FACCH_F, ded_dtr_ptr, l1_tch_data[gas_id].tx_data, &l1_tch_data[gas_id].data_length, gas_id) == FALSE )
         {
            if( (l1_tch_data[gas_id].coex_denials_for_facch <= MAX_COEX_DENIALS_FOR_FACCH) &&
                (l1_tch_data[gas_id].coex_request_tx != NULL) &&
                (l1_tch_data[gas_id].coex_request_tx(GL1_MSG_FACCH, gas_id) == FALSE) )
            {
               L1_get_CCH_filler_message( FACCH_F, l1_tch_data[gas_id].tx_data, &l1_tch_data[gas_id].data_length, gas_id );

               l1_tch_data[gas_id].facch_not_available = TRUE;

               l1_tch_data[gas_id].coex_denials_for_facch++;
            }
            else
            {
               L1_free_CCH_message(FACCH_F, ded_dtr_ptr, gas_id);

               l1_tch_data[gas_id].facch_not_available = FALSE;

               l1_tch_data[gas_id].coex_denials_for_facch = 0;
            }
         }
		 else
		 {
            l1_tch_data[gas_id].facch_not_available = TRUE;
		 }
#else
         l1_tch_data[gas_id].facch_not_available =
         L1_get_CCH_message( facch_filler_reqd, FACCH_F, ded_dtr_ptr, l1_tch_data[gas_id].tx_data, &l1_tch_data[gas_id].data_length, gas_id);
#endif

         if ( ( ded_dtr_ptr->channel_specification.channel_mode_1 == SIGNALLING_ONLY )
              || ( l1_tch_data[gas_id].facch_not_available == FALSE ) )
         {
            sending_FACCH = TRUE;
         }

         /* Get the next block of ARFCN's and signal strengths */

         /* Passing in before and after freq parameters to L1_get_ARFCNS
            to guarantee a seamless transition between the before and
            after channels in the case of the start time lying in the
            middle of a block.  Not very elegant but the least complicated
            solution for now
         */
         L1_get_ARFCNS ( offset, 1, 4, l1_tch_data[gas_id].arfcns,
                         &ded_dtr_ptr->channel_specification.channel_info_1_before,
                         &ded_dtr_ptr->channel_specification.channel_info_1_after,
                         &ded_dtr_ptr->new_channel_info, gas_id );
#ifdef FEATURE_VAMOS_II
		 if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3))
		   {
		   if(vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)
			 {	 
			 offset = 3;
			 L1_get_ARFCNS
			 ( offset,
			   1,
			   3,
			   l1_tch_data[gas_id].arfcns + 1 ,
			   &ded_dtr_ptr->channel_specification.channel_info_1_before,
			   &ded_dtr_ptr->channel_specification.channel_info_1_after,
			   &ded_dtr_ptr->new_channel_info, gas_id );
			 }
		   else if(vamos2_tch_wait_state == WAIT_AFTER_BURST2)
			 {				 
			 offset = 5;
			 L1_get_ARFCNS
			 ( offset,
			   1,
			   1,
			   l1_tch_data[gas_id].arfcns + 3 ,
			   &ded_dtr_ptr->channel_specification.channel_info_1_before,
			   &ded_dtr_ptr->channel_specification.channel_info_1_after,
			   &ded_dtr_ptr->new_channel_info, gas_id );
			 }
		   }
#endif


         /* FNrptperiod is the FN relative to the start of the reporting period
            This is used to index into the precomputed TXLEVs since they are computed
            with reference to the start of the reporting period
         */
         /* Below logic gives correct FN relative to the start of the reporting period
            adding offset is not needed as relative FN can go wrong when offset is different in VAMOS cases
            which causes Tx power fluctuation
         */
         FNrptperiod = SUB_MOD ( frame_counters[gas_id].FNmod104 , l1_tch_data[gas_id].reporting_period_end_minus_1 , 104 );

         /* Convert TXLEV to power index before passing to the msg layer */
         L1_get_txlev( FNrptperiod, 1, l1_tch_data[gas_id].txlev, gas_id );

         /* Update ded data ptr with most recent power level*/
         ded_dtr_ptr->ms_power_level_in_use = (byte)l1_tch_data[gas_id].txlev[3];


#ifdef FEATURE_DATA_GCSD
         if ( l1_tch_data[gas_id].data_services )
         {
            /* If transmitting access burst, don't tx anything else */
            if ( l1_tch_data[gas_id].access_burst_scheduled )
            {
            }
            /* Now tx a tch or facch message */
            else if ( sending_FACCH )
            {
               gl1_msg_cs_data_tx( &l1_tch_data[gas_id].csd_tx_ptr, gas_id);
               gl1_msg_tx_cstch(l1_tch_data[gas_id].arfcns, l1_tch_data[gas_id].txlev,
                                l1_tch_data[gas_id].csd_tx_ptr, l1_tch_data[gas_id].tx_data, TRUE, gas_id );
            }
            else
            {
               gl1_msg_cs_data_tx( &l1_tch_data[gas_id].csd_tx_ptr, gas_id );
               gl1_msg_tx_cstch(l1_tch_data[gas_id].arfcns, l1_tch_data[gas_id].txlev,
                                l1_tch_data[gas_id].csd_tx_ptr, l1_tch_data[gas_id].tx_data, FALSE, gas_id );
            }
            /* Go get an rx message */
            gl1_msg_rx_cstch_facch(l1_tch_data[gas_id].arfcns, l1_tch_data[gas_id].agc_ptr,            
#ifdef FEATURE_GSM_RX_DIVERSITY
                                   l1_tch_data[gas_id].agc_ptr_divrx,
#endif                         
                                   &l1_tch_data[gas_id].csd_rx_rpt,
                                   &l1_tch_data[gas_id].t_rpt,
                                   gas_id);
         }
         else
#endif // FEATURE_DATA_GCSD
         {
            /*
                Don't transmit anything else if transmitting handover access bursts
            */
            if ( l1_tch_data[gas_id].access_burst_scheduled )
            {
            }
            /* Now tx a tch or facch message */
            /* If the vocoder is not initialized or no packets available,
               transmit FACCH fill frames.  This prevents garbage packets
               from being transmitted before voc is initialized.   We need
               to transmit right away to meet 11.10
            */
            else if ( sending_FACCH )
            {
               MSG_GERAN_MED_3_G("FACCH transmitted 0x%x 0x%x 0x%x",l1_tch_data[gas_id].tx_data[0],
                                                          l1_tch_data[gas_id].tx_data[1],l1_tch_data[gas_id].tx_data[2]);

               gl1_msg_tx_facch_fr(l1_tch_data[gas_id].arfcns, l1_tch_data[gas_id].txlev, l1_tch_data[gas_id].tx_data,
#ifdef FEATURE_VAMOS_II
                                   vamos2_tch_wait_state,
#endif 
                                   gas_id );

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
               gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_FACCH_TX);
#endif

               l1_tch_data[gas_id].prev_block_was_facch = TRUE;
               /* This counter is used to reset the prev_block_was_facch if we are
                * in UL DTX mode - without the counter we will not be able to abort
                * the TCH in this scenario - CR169424
                */
               if(l1_tskisr_blk->main_command != L1_DEDICATED_STOP)
               {
                 l1_tch_data[gas_id].prev_block_was_facch_counter = L1_WAIT_FOUR_FR_BURSTS_BEFORE_ABORT;
               }

              /* Tx FACCH during TCH loopback, loopback was opened when we received PH_DATA_REQ in l1_ded_if.c
               * so re-enable once FACCH has been transmitted. Note this take 8 frames due to interleaving
               */
              l1_tch_data[gas_id].enable_loopback_after_facch_block = TRUE;

               /* Log tx burst metrics */
               l1_log_transmit_burst_metrics( GL1_MSG_FACCH, 1, 1, 4, l1_tch_data[gas_id].arfcns,
                                              l1_tch_data[gas_id].txlev, chnl1_info_ptr->TSC,
                                              ded_dtr_ptr->timing_advance_in_use, gas_id );

               /* Delay the SID by a block each time a FACCH block preempts the
                  SID block.  This is mandated in the specs
               */
               if ( frame_counters[gas_id].FNmod104 >=51 && frame_counters[gas_id].FNmod104 <=55 )
               {
                  MSG_GERAN_HIGH_1_G("SID block fn=%d pre-empted by FACCH",frame_counters[gas_id].FNmod104);
                  l1_tch_data[gas_id].num_sids_pending = 2;
               }
            }
            else
            {
               boolean sid_frame =
                  ( frame_counters[gas_id].FNmod104 >= 51 && frame_counters[gas_id].FNmod104 <= 55 ) ? TRUE : FALSE;

               /* No need to tx if silence is found by vocoder, However on frames  */
               /* 52->59, we must tx.  The msg layer is invoked a few frames prior */
               /* If previous block was FACCH, we need to transmit a TCH block so
                  that we have 8 bursts to complete the FACCH interleaver. Otherwise
                  the FACCH will not get sent out correctly.   prev_dtx != dtx checks
                  if there was a transition in the dtx state (from OFF to ON).  Need
                  to transmit a SID frame in this case too as the specs say.
                  sid_delay shifts the SID window if we're preempted by a FACCH
               */
               if ( (l1_tch_data[gas_id].dtx==FALSE) ||
                    ( sid_frame ) ||
                    (l1_tch_data[gas_id].prev_block_was_facch) ||
                    ( l1_tch_data[gas_id].tx_homing_sequence ) ||
                    ( l1_tch_data[gas_id].num_sids_pending ) )
               {
                  gl1_msg_tx_tch_fr(l1_tch_data[gas_id].arfcns, l1_tch_data[gas_id].txlev,
                                    l1_tch_data[gas_id].voc_tx_data,
#ifdef FEATURE_VAMOS_II
                                    vamos2_tch_wait_state,
#endif 
                                    gas_id);

                  /* Log tx burst metrics */
                  l1_log_transmit_burst_metrics
                  ( GL1_MSG_TCH,
                    1, 1, 4,
                    l1_tch_data[gas_id].arfcns,
                    l1_tch_data[gas_id].txlev,
                    chnl1_info_ptr->TSC,
                    ded_dtr_ptr->timing_advance_in_use, gas_id );

                  if ( l1_tch_data[gas_id].num_sids_pending )
                  {
                     MSG_GERAN_MED_2_G("Tx FR pending SID fn=%d,num_sid=%d",frame_counters[gas_id].FNmod104,l1_tch_data[gas_id].num_sids_pending);
                     l1_tch_data[gas_id].num_sids_pending-- ;
                  }

                  if ( sid_frame )
                  {
                     MSG_GERAN_MED_1_G("Tx FR SID at fn=%d",frame_counters[gas_id].FNmod104);
                  }

                  if ( l1_tch_data[gas_id].prev_dtx != l1_tch_data[gas_id].dtx )
                  {
                     MSG_GERAN_MED_3_G("DTX change %d->%d fn=%d",l1_tch_data[gas_id].prev_dtx,l1_tch_data[gas_id].dtx,frame_counters[gas_id].FNmod104);
                  }

                  if ( l1_tch_data[gas_id].tx_homing_sequence )
                  {
                     MSG_GERAN_MED_1_G("Homing seq transmitted fn=%d",frame_counters[gas_id].FNmod104);
                  }

               }
            }


            /* Go get an rx message */
            gl1_msg_rx_tch_facch_fr(l1_tch_data[gas_id].arfcns, l1_tch_data[gas_id].agc_ptr, 
#ifdef FEATURE_GSM_RX_DIVERSITY
                                    l1_tch_data[gas_id].agc_ptr_divrx,
#endif    
                                    &l1_tch_data[gas_id].t_rpt,
#ifdef FEATURE_VAMOS_II
                                    vamos2_tch_wait_state,
#endif 
                                    gas_id);
         }
      }
   }
   if (l1_tch_data[gas_id].prev_block_was_facch)
   {
     l1_tch_data[gas_id].prev_block_was_facch_counter--;
     if (l1_tch_data[gas_id].prev_block_was_facch_counter <= 0)
     {
       l1_tch_data[gas_id].prev_block_was_facch = FALSE;
     }
     if (l1_tch_data[gas_id].prev_block_was_facch_counter < 0)
     {
       MSG_GERAN_ERROR_1_G("FR:prev_block_was_facch_cnt =%d",l1_tch_data[gas_id].prev_block_was_facch_counter);
     }
   }
}

/*===========================================================================
FUNCTION l1_start_ftn_fr_hr

DESCRIPTION
   This function starts Frame Tick Notification for Full and Half Rate
   if the current state is not L1_DTM otherwise it stores the ftn
   structure away for processing in gpl1_dtm.c

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_start_ftn_fr_hr(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   gl1_msg_frame_tick_type    ftn_type;

   /*
    *  FTN processing
    */
   gl1_msg_init_frame_tick_info(&ftn_type);
   ftn_type.next_frame_is_dl_block_start             = l1_tch_data[gas_id].block_start;
   ftn_type.next_frame_is_ul_block_start             = l1_tch_data[gas_id].block_start || l1_tch_data[gas_id].access_burst_scheduled;
   ftn_type.next_frame_is_ul_ptcch_sacch_block_start = l1_tch_data[gas_id].sacch_ul_block_start;
   ftn_type.next_frame_is_dl_ptcch_sacch_block_start = (l1_tch_data[gas_id].send_sacch==frame_counters[gas_id].FNmod104) ? TRUE : FALSE;
   ftn_type.next_frame_is_acq_ncell_block_start      = gl1_is_ncell_block_start(gas_id);
   ftn_type.next_frame_is_pwr_meas_block_start       = gl1_is_pwr_meas_block_start(gas_id);

#ifdef FEATURE_GSM_DTM

   if(l1_tskisr_blk->l1_state != L1_DTM_MODE)
   {
     /* Pass FTN structure to NPL1 Message layer */
      gl1_msg_frame_tick_notification ( &ftn_type,gas_id );

   /* Clear block starts */
   gl1_set_pwr_meas_block_start(FALSE,gas_id);
   gl1_set_ncell_block_start(FALSE,gas_id);
   }else
   {
     /* Store FTN structure away until PS activities are scheduled in gpl1_dtm.c */
     l1_ded_store_ftn_info(ftn_type, gas_id);
   }

#else

   /* Pass FTN structure to NPL1 Message layer */
   gl1_msg_frame_tick_notification ( &ftn_type, gas_id );

   /* Clear block starts */
   gl1_set_pwr_meas_block_start(FALSE,gas_id);
   gl1_set_ncell_block_start(FALSE,gas_id);

#endif
}

/*===========================================================================
FUNCTION l1_tch_abort_fr

DESCRIPTION
   Function to abort the Full Rate Traffic Channel

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_tch_abort_fr( gas_id_t gas_id )
{
   /* Wait for SCE */
   l1_tch_data[gas_id].idle_status = !l1_sci_is_op_in_progress(gas_id);

   /*Check if GL1 is stuck in this state because of unfinished IRAT activity*/
   gl1_check_if_irat_state_stuck(gas_id);

   /* Abort Tx SACCH */
   gl1_msg_abort_tx_sacch(gas_id);

   /* Poll TCH and FACCH aborts.  These aborts return TRUE only after
      the tx block has been completed.  This is required to
      satisfy the 20 ms constraint - i.e the time interval between the
      last burst on the old channel and the first burst on the new channel
      should be <= 20ms.  The 20ms window applies to the last complete
      tx block which the network sees, so an incomplete block increases
      this window causing us to fail this 11.10 test case
   */
   /* Abort Tx TCH */
   l1_tch_data[gas_id].idle_status &= gl1_msg_abort_tx_tch_facch_fr(gas_id);

   /* Abort Rx SACCH */
   gl1_msg_abort_rx_sacch(gas_id);

   /* Wait for TCH rx to complete */
   l1_tch_data[gas_id].idle_status &= gl1_msg_abort_rx_tch_fr(gas_id);

   /* let SCE terminate - it already knows it should */
   l1_sci_tick_receive(gas_id);

   MSG_GERAN_MED_1_G("Tch FR abort, idle status %d", l1_tch_data[gas_id].idle_status);

}



/*===========================================================================

FUNCTION L1_CONTROL_HALF_RATE

DESCRIPTION
  This function controls the half traffic channel,

DEPENDENCIES
  pointer to dedicated data
  pointer to channel 1 info
  pointer to channel 2 info

RETURN VALUE
  TRUE  - processing is complete and mode may be changed
  FALSE - this function should be called again on the next frame tick

SIDE EFFECTS
  None

===========================================================================*/
boolean  L1_control_half_rate ( dedicated_data_T       *ded_dtr_ptr,
                                channel_information_T  *chnl1_info_ptr,
                                boolean                apply_freq_redef, gas_id_t gas_id
                              )
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  boolean get_voc_data = FALSE;
  uint16 gl1_hr_block_start = 0;
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION
  static uint8 no_vocoder_change_after_handover;
#endif
   /* Indicates if we have started transmitting on the SACCH */
   uint8  FNmod13_HR;

   /*
    * Initialize global flags.
    */
   l1_tch_data[gas_id].idle_status = FALSE;
   l1_tch_data[gas_id].block_start = FALSE;
   l1_tch_data[gas_id].prepare_msg = FALSE;
   l1_tch_data[gas_id].access_burst_scheduled = FALSE;
   l1_tch_data[gas_id].sacch_ul_block_start = FALSE;

   if ( !ded_dtr_ptr || !chnl1_info_ptr )
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
                ded_dtr_ptr, chnl1_info_ptr);
      return FALSE;
   }

   l1_tch_data[gas_id].agc_ptr = &ded_dtr_ptr->c0_agc;

#ifdef FEATURE_GSM_RX_DIVERSITY
   l1_tch_data[gas_id].agc_ptr_divrx = &ded_dtr_ptr->agc_divrx;
#endif  

   switch ( l1_tskisr_blk->sub_sub_state )
   {

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM

      case  L1_EDTM_INIT:

#endif
#endif

      case L1_DED_INIT :

         /* Overwrite running average to 2 bursts length
          * since half rate is being configured
          */
         sys_algo_agc_avg_init(TWO_BURST_AVG, gas_id);
         l1_ded_drx_agc_init[gas_id] = TRUE;

         l1_acch_nv_flag_initialise(gas_id);

/* For Poseidon enable cipher clock for mDSP */
         if (gl1_msg_is_ciphering_enabled(gas_id))
         {
            gl1_hw_cipher_clock_required(TRUE, gas_id);
         }

         l1_tch_data[gas_id].first_rx_block = TRUE;
         l1_tch_data[gas_id].voc_type = GL1_MSG_HS;

#ifdef FEATURE_GSM_MDSP_ESAIC
         if ( l1_dedicated_data_ptr->handover_info.gl1_inter_handover_in_progress == TRUE)
         {
            if(l1_dedicated_data_ptr->handover_info.gl1_inter_handover_fail == TRUE )
            {
               /* Handover failed*/
               gl1_msg_clear_esaic_iir_filter(TRUE,FALSE, gas_id);
               l1_dedicated_data_ptr->handover_info.gl1_inter_handover_fail = FALSE;

            }
            else
            {
              /* Handover success*/
              gl1_msg_clear_esaic_iir_filter(TRUE, TRUE, gas_id);

            }
            l1_dedicated_data_ptr->handover_info.gl1_inter_handover_in_progress = FALSE;

         }
         else
         {
           /* If this is not an inter-cell handover then no need
             * to indicate to GFW
             */
            gl1_msg_clear_esaic_iir_filter(FALSE, FALSE, gas_id);
         }

#endif
/* Exclude as PMIC signaling is only needed for Solo.
**
*/


         /*
          * Convert channel_type and channel_mode to CHT parameters for
          * the DSP
          */

         if ( chnl1_info_ptr->channel_type == HALF_RATE_TRAFFIC )
         {
            switch ( ded_dtr_ptr->channel_specification.channel_mode_1 )
            {
               case SPEECH_V1:
#ifdef FEATURE_VAMOS_II
               case SPEECH_V1_VAMOS2:
#endif
                  l1_tch_data[gas_id].channel_type = GL1_MSG_TCH;
                  MSG_GERAN_MED_0_G("Tch init: HR voice");
                  l1_tch_data[gas_id].voc_type = GL1_MSG_HS;
                  l1_tch_data[gas_id].chan_mode = ENG_MODE_HALF_RATE;
                  break;

               case SPEECH_V3:
#ifdef FEATURE_VAMOS_II
               case SPEECH_V3_VAMOS2:
#endif
                  l1_tch_data[gas_id].chan_mode = ENG_MODE_AMR_HALF_RATE;

                  return L1_control_amr_hr( ded_dtr_ptr, chnl1_info_ptr,
                                            apply_freq_redef, gas_id );

               case SIGNALLING_ONLY:
                  l1_tch_data[gas_id].channel_type = GL1_MSG_DCCH;
                  MSG_GERAN_MED_0_G("Tch init: HR signaling only");
                  break;

               default:
                  MSG_GERAN_ERROR_1_G("Unsupported TCH channel mode %d",
                            ded_dtr_ptr->channel_specification.channel_mode_1);

                  break;
            }
            /* Log the eng_mode_info for CM stats. Nothing to do with AMR here*/
            {
              eng_mode_amr_info_t amr_info = {0};
              uint8 i = 0;

              amr_info.ul_chan_mode = (uint8)l1_tch_data[gas_id].chan_mode;
              amr_info.dl_chan_mode = (uint8)l1_tch_data[gas_id].chan_mode;

              for (i = 0; i < MAX_CODEC_MODES; i++)
              {
                amr_info.ul_acs[i] = (uint8)0xFF;
                amr_info.dl_acs[i] = (uint8)0xFF;
              }

              amr_info.dl_dtx = FALSE;
              amr_info.dl_c_over_i = (uint16)0xFFFF;
              geran_eng_mode_data_write(ENG_MODE_AMR_INFO, (void *)&amr_info, TRUE, gas_id);

            }
         }


#ifdef FEATURE_VAMOS_II
        /* If RR indicates that UE has to operate is VAMOS 2 mode and if TSC set 2 is allocated,
         * the SACCH position is shifted as per 3GPP 45.002, section 7, Table 1a. 
         */
        if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3))
        {
          l1_setup_time_slot_params_vamos2_hr(chnl1_info_ptr, gas_id);
        }
        else
#endif /*FEATURE_VAMOS_II*/
        {
           /*
            * Set up parameters that depend on the timeslot used
            */
            l1_setup_timeslot_params_hr( chnl1_info_ptr, gas_id );
         }
         l1_tch_data[gas_id].half_rate_subchannel = chnl1_info_ptr->subchannel;
         l1_tch_data[gas_id].afc_correction_frame = chnl1_info_ptr->TN % 2  ? 102 : 89;

         l1_tch_data[gas_id].prev_block_was_facch = FALSE;
         l1_tch_data[gas_id].prev_block_was_facch_counter =0;
         l1_tch_data[gas_id].num_sids_pending = 0;
         l1_tch_data[gas_id].vfr_aligned = FALSE;
         l1_tch_data[gas_id].sacch_tx_started_hr = FALSE;
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION		 
         no_vocoder_change_after_handover = FALSE;
#endif

#ifdef FEATURE_GSM_COEX_SW_CXM
         /* Notify the arbitrator of the channel assignment */
         garb_intf_notify_cs_assignment(
            (l1_tskisr_blk->l1_state == L1_DTM_MODE ? GL1_DEFS_TN_2 : GL1_DEFS_TN_0),
            chnl1_info_ptr->frequency_list,
            chnl1_info_ptr->subchannel,
            FALSE,
            FALSE,
            l1_tch_data[gas_id].sacch_minus_one_frame+1,
            l1_tch_data[gas_id].idle_frame,
            l1_tch_get_arfcns,
           &l1_tch_data[gas_id].coex_request_tx,
            l1_tskisr_blk->l1_state,
            gas_id
         );
#endif /* FEATURE_GSM_COEX_SW_CXM */


         /* Pass idle frame to SCE - for ncell scheduling */
         l1_sc_set_tch_idle_frame( l1_tch_data[gas_id].idle_frame, gas_id  );

         /*
          * Setup message layer
          */
         l1_setup_message_layer_hr(ded_dtr_ptr, chnl1_info_ptr, gas_id);

         /* Install callback to trigger serving cell measurement reporting. */
         l1_sci_monscan_set_next_period_ded_cb( L1_send_dedicated_report, gas_id );

         MSG_GERAN_MED_1_G("Tch msg init FNmod42432 = %d", frame_counters[gas_id].FNmod42432);

         l1_schedule_monitors_fr_hr(gas_id);

         FNmod13_HR = ( frame_counters[gas_id].FNmod26 - chnl1_info_ptr->subchannel + 13 ) % 13 ;

         if ( FNmod13_HR >= 9  ||
              FNmod13_HR <= 0 )
         {
            /* VFR will be aligned to frame 4 and 20000 of 13M count*/
            l1_tch_data[gas_id].vfr_align_frame_hr = 3;
         }
         else if ( FNmod13_HR >= 1  &&
                   FNmod13_HR <= 4 )
         {
            /* VFR will be aligned to frame 8 and 40000 of 13M count*/
            l1_tch_data[gas_id].vfr_align_frame_hr = 7;
         }
         else
         {
            /* VFR will be aligned to frame 0 and 0 of 13M count*/
            l1_tch_data[gas_id].vfr_align_frame_hr = 12;
         }

         if (( l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_STARTED ) ||
              ( l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_READY ))
          {
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION		  
            /*If no change in vocoder id, do not stop the vocoder and align VFR in immediate possible frame */
            if(l1_vs_change_in_voc(l1_tch_data[gas_id].channel_type,l1_tch_data[gas_id].voc_type, gas_id) )
            {
            l1_vs_stop_client(gas_id);
            }
            else
            {
               no_vocoder_change_after_handover = TRUE;
 
               if(   FNmod13_HR == 3   ||
                     FNmod13_HR == 7   ||
                     FNmod13_HR == 12    )
               {
                 gl1_msg_align_vfr( TRUE, gas_id );
                 l1_tch_data[gas_id].vfr_aligned = TRUE;
                 MSG_GERAN_MED_1_G("Aligning Real VFR FNmod13=%d",frame_counters[gas_id].FNmod13);
               }
               else  if( FNmod13_HR >= 12  ||
                         FNmod13_HR <= 2 )
               {
                 /* VFR will be aligned to frame 4 and 20000 of 13M count*/
                 l1_tch_data[gas_id].vfr_align_frame_hr = 3;
               }
               else if ( FNmod13_HR >= 3  &&
                         FNmod13_HR <= 6 )
               {
                 /* VFR will be aligned to frame 8 and 40000 of 13M count*/
                 l1_tch_data[gas_id].vfr_align_frame_hr = 7;
               }
               else
               {
                 /* VFR will be aligned to frame 0 and 0 of 13M count*/
                 l1_tch_data[gas_id].vfr_align_frame_hr = 12;
               }
            }
#else
     
            l1_vs_stop_client(gas_id);
#endif			
        }

         l1_tskisr_blk->sub_sub_state  = L1_DED_RUNNING;


         /* End the Channel Init here so the DED_RUNNING is run on the next frame
           tick, otherwise this may result in possible ISR Overrun. */
         l1_start_ftn_fr_hr(gas_id);

         break;

      case L1_DED_RUNNING:

         if ( ( ded_dtr_ptr->channel_specification.channel_mode_1 == SPEECH_V3 )
#ifdef FEATURE_VAMOS_II
              ||( ded_dtr_ptr->channel_specification.channel_mode_1 == SPEECH_V3_VAMOS2 )
#endif
            )
         {
            return L1_control_amr_hr( ded_dtr_ptr, chnl1_info_ptr,
                                      apply_freq_redef, gas_id );
         }

         gl1_hr_block_start = ( frame_counters[gas_id].FNmod26 % 13 - chnl1_info_ptr->subchannel + 13 ) % 13;
         
#ifdef FEATURE_VAMOS_II
         if((gl1_hw_get_vamos2_support(gas_id)) &&
            ((chnl1_info_ptr->TSC & 0x8) >> 3) && (chnl1_info_ptr->subchannel == 0))
         {
           if ( (frame_counters[gas_id].FNmod13 == 3 ) || (frame_counters[gas_id].FNmod13 == 7) ||
                (frame_counters[gas_id].FNmod26 == 11) || (frame_counters[gas_id].FNmod26 == 25) )
           {
             get_voc_data = TRUE;
           }
         }
         else
#endif /*FEATURE_VAMOS_II*/
         {            
           if ((gl1_hr_block_start == 3) || (gl1_hr_block_start == 7) ||
               (gl1_hr_block_start == 12))
           {
             get_voc_data = TRUE;
           }
         }


         /* See if a vocoder packet is available */
         if(get_voc_data)
         {
           l1_tch_data[gas_id].prev_dtx = l1_tch_data[gas_id].dtx;

           l1_tch_data[gas_id].dtx = l1_vs_get_gsm_packet_from_client( l1_tch_data[gas_id].voc_tx_data,
                                                                       sizeof(l1_tch_data[gas_id].voc_tx_data),
                                                                       &l1_tch_data[gas_id].packet_available, gas_id);
           if (l1_tch_data[gas_id].dtx)
           {
             gl1_set_dtx_used_flag(gas_id);
           }

           if ( ( l1_tch_data[gas_id].dtx == TRUE ) && ( l1_tch_data[gas_id].prev_dtx == FALSE ) )
           {
             MSG_GERAN_MED_1_G("Entering DTX fn=%d",frame_counters[gas_id].FNmod104);
             l1_tch_data[gas_id].num_sids_pending = 2;
           }

           /* Transmit homing sequence if no packet available or if
            * the vocoder is not re-enabled after a handover
            */
           if ( l1_tch_data[gas_id].packet_available == FALSE
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
               || !l1_vs_is_client_ready(gas_id)
#endif
              )
           {
             memscpy( l1_tch_data[gas_id].voc_tx_data,sizeof(l1_tch_data[gas_id].voc_tx_data), hr_homing_sequence, TCH_HS_DATA_WIDTH_OCTETS );
             MSG_GERAN_MED_3_G("Tx HR homing seq %x %x %x",l1_tch_data[gas_id].voc_tx_data[0],l1_tch_data[gas_id].voc_tx_data[1],l1_tch_data[gas_id].voc_tx_data[2]);
             l1_tch_data[gas_id].tx_homing_sequence = TRUE;
			 
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
             if(!l1_vs_is_client_ready(gas_id))
             {
               gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_TX_HOMING_SEQ_VOC_NOT_READY);
             }
             else
             {
               gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_TX_HOMING_SEQ_VOC_READY);
             }
#endif
           }
           else
           {
             l1_tch_data[gas_id].tx_homing_sequence = FALSE;
           }
         }

         /*
           If frequency redefinition occurs midway through a SACCH block,
           we need to recompute the SACCH ARFCNs based on the new parameters
           and inform the msg layer.  We don't do this for the TCH because
           they are contiguous and this scenario is extremely unlikely.  We
           need to recompute because the ARFCNs are pre-computed before the
           block starts
         */
         if ( apply_freq_redef )
         {
            l1_recompute_sacch_arfcns_fr_hr( ded_dtr_ptr, gas_id );
         }

         /* Check up front to prevent Tx TCH from being scheduled in sacch/idle - 1 */
         if ( L1_more_abs_to_send(ded_dtr_ptr, FACCH_H, gas_id) )
         {
            l1_tch_data[gas_id].access_burst_scheduled = TRUE;
         }

         /*
          * SACCH Processing
          */
         if ( frame_counters[gas_id].FNmod26 == l1_tch_data[gas_id].sacch_minus_one_frame )
         {
            if ( l1_tskisr_blk->main_command != L1_DEDICATED_STOP )
            {
               if ( frame_counters[gas_id].FNmod104 == l1_tch_data[gas_id].send_sacch || !l1_tch_data[gas_id].sacch_tx_started_hr )
               {
                  if( l1_process_sacch_fr_hr( ded_dtr_ptr, chnl1_info_ptr, gas_id ) == TRUE )
                  {
                    /*
                        Once an incomplete or complete SACCH block is started,
                        sacch_tx_started is set to TRUE and is reset only
                        when we get onto another channel.  sacch_ul_block
                        start indicates the start of an incomplete or complete
                        block.  The old indicator was ticking the handler only
                        on a SACCH block boundary
                    */
                    l1_tch_data[gas_id].sacch_ul_block_start = TRUE;
                    l1_tch_data[gas_id].sacch_tx_started_hr = TRUE;
                  }
               }
            }
         }
         else if ( frame_counters[gas_id].FNmod26 != l1_tch_data[gas_id].idle_minus_one_frame )
         {
            if ( l1_tskisr_blk->main_command != L1_DEDICATED_STOP )
            {
               l1_process_rach_hr( ded_dtr_ptr, chnl1_info_ptr, gas_id );
            }


            /*
             * If it is not sacch - 1 frame and is not idle - 1 frame, check
             * to see if it is an idle frame.
             */
            if ( frame_counters[gas_id].FNmod26 == l1_tch_data[gas_id].idle_frame )
            {
               l1_process_idle_fr_hr(gas_id);
            }

            /*
             * If it is not sacch - 1 frame and is not idle - 1 frame, check
             * to see if it is an idle - 1 - 1 frame.
             */
            if ( frame_counters[gas_id].FNmod26 == (l1_tch_data[gas_id].idle_minus_one_frame - 1) )
            {
               //Tick the surrounding cell engine to schedule the rx
               l1_sci_tick_receive(gas_id);

            }

            /*Check if G2L search can happen on next idle frame*/
            if ( frame_counters[gas_id].FNmod26 == (l1_tch_data[gas_id].idle_minus_one_frame - 4) )
            {
              l1_sc_check_if_next_cm_search_is_lte(gas_id);
            }

            /*
             * If it is not sacch - 1 frame and is not idle - 1 frame, check
             * to see if it is an idle - 1 - 3 frame.
             */
            if ( frame_counters[gas_id].FNmod26 == (l1_tch_data[gas_id].idle_minus_one_frame - 3) )
            {
               l1_process_sce_pwr_data_fr_hr(gas_id);
            }
         }

         /* Check if the VFR needs aligning */
         if ( frame_counters[gas_id].FNmod13 == l1_tch_data[gas_id].vfr_align_frame_hr &&
              !l1_tch_data[gas_id].vfr_aligned )
         {
             /* Check Vocoder is in the correct state before aligning VFR */
            if ( (l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_OPENED)
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION				
				|| ( TRUE == no_vocoder_change_after_handover)
#endif
			   )
            {
               gl1_msg_align_vfr( TRUE, gas_id );
               l1_tch_data[gas_id].vfr_aligned = TRUE;
               MSG_GERAN_MED_1_G("Aligning Real VFR FNmod13=%d",frame_counters[gas_id].FNmod13);
            }
            else
            {
            
              MSG_GERAN_MED_1_G("Vocoder is not stopped, aligning VFR postponed FNmod13=%d",
                                                                         frame_counters[gas_id].FNmod13);
              /* If vocoder was not stopped, calculate when is the 
               * next oportunity to align the vocoder */
              switch (l1_tch_data[gas_id].vfr_align_frame_hr)
              {
                case 3:
                  l1_tch_data[gas_id].vfr_align_frame_hr  = 7;
                  break;

                case 7:
                  l1_tch_data[gas_id].vfr_align_frame_hr  = 12;
                  break;

                case 12:
                  l1_tch_data[gas_id].vfr_align_frame_hr  = 3;
              }
            }
         }

         /* Check if vocoder needs to be enabled */
         if ( l1_tch_data[gas_id].vfr_aligned &&
              l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_OPENED &&
              l1_tch_data[gas_id].channel_type == GL1_MSG_TCH )
         {
            MSG_GERAN_MED_1_G("Starting vocoder FNmod13=%d",frame_counters[gas_id].FNmod13);

            l1_vs_start_client(l1_tch_data[gas_id].channel_type,
                               l1_tch_data[gas_id].voc_type, gas_id);
         }


         /*
          * Now process the traffic frame.
          */
         l1_process_tch_hr( ded_dtr_ptr, chnl1_info_ptr, gas_id );


         /*******************************************************
          *
          *  Process the Surrounding Cell power data
          *  N.B. must be before l1_sci_monscan_tick() call!!!!
          ********************************************************/
         /*
           instead of calling immediately before l1_sci_monscan_tick(),
           l1_sci_mon_scan_next_period() is called one frame earlier, after
           l1_sci_monscan_tick() to reduce the runtime of the next frame
         */
         /* Added an extra -1 because reporting_period_end_minus_1 is
          * actually set to reporting period end.
          * l1_sci_monscan_next_period() must be called before the end
          * of the reporting period otherwise the ncell measurement
          * report is sent to RR about 26 frames too late.
          */
         if ( frame_counters[gas_id].FNmod104 == l1_tch_data[gas_id].reporting_period_end_minus_1 - 1 )
         {
            l1_sci_monscan_next_period_ded(gas_id);
         }

         /*
          *   TT / AFC correction
          */
         if ( frame_counters[gas_id].FNmod104 == l1_tch_data[gas_id].afc_correction_frame )
         {
            gl1_msg_dch_tt_adjust(gas_id);
         }

         l1_tch_data[gas_id].idle_status = FALSE;


         /* Don't abort till the extra TCH block to complete the FACCH has been
            sent out
         */
         if ( l1_tskisr_blk->main_command == L1_DEDICATED_STOP )
         {
            l1_dedicated_data_ptr->no_of_ab_to_send = 0;

            if ( !l1_tch_data[gas_id].prev_block_was_facch )
            {
               /* Abort after FTN will save us a frame during channel changes
                * since we can abort as soon as NPL1 is done aborting.
                * Important from an audio perspective
                */
               l1_tch_abort_hr(gas_id);
            }
         }

         /* If idle_status flag is set to ture then TCH will be aborted. While running Tch abort no need to process FTN*/
         if( l1_tch_data[gas_id].idle_status == FALSE )
         {
           /* FTN processing */
            l1_start_ftn_fr_hr(gas_id);
         }
         break;

      default:
         break;
   } /* of switch */


   return(l1_tch_data[gas_id].idle_status);

} /* end of function */



/*===========================================================================
FUNCTION l1_setup_time_slot_params_hr

DESCRIPTION
   This function sets up the time slot specific parameters for Half Rate
   traffic channel.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_setup_timeslot_params_hr( channel_information_T  *chnl1_info_ptr,
                                         gas_id_t gas_id )
{
   if ( !chnl1_info_ptr )
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL chnl1_info_ptr");
      return;
   }

   /*****************************************************************
    *
    *    Set up parameters that depend on the timeslot and subchannel
    *    used.  See 05.02 Clause 7 Table 1 and 05.08 Sec 8.4
    *
    *****************************************************************/
   switch ( (chnl1_info_ptr->TN) & 7 )
   {
      case 0:
      case 1:
         l1_tch_data[gas_id].sacch_minus_one_frame = ( 11 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].idle_minus_one_frame = ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].idle_frame = ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = ( 103 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
         l1_tch_data[gas_id].send_sacch= ( 11 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
         break;

      case 2:
      case 3:
         l1_tch_data[gas_id].sacch_minus_one_frame = ( 11 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].idle_minus_one_frame = ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].idle_frame = ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
         l1_tch_data[gas_id].send_sacch= ( 37 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
         break;

      case 4:
      case 5:
         l1_tch_data[gas_id].sacch_minus_one_frame = ( 11 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].idle_minus_one_frame = ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].idle_frame = ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = ( 51 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
         l1_tch_data[gas_id].send_sacch= ( 63 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
         break;

      case 6:
      case 7:
         l1_tch_data[gas_id].sacch_minus_one_frame = ( 11 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].idle_minus_one_frame = ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].idle_frame = ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = ( 77 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
         l1_tch_data[gas_id].send_sacch= ( 89 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
         break;

      default:
         break;
   }

   L1_set_reporting_period(l1_tch_data[gas_id].reporting_period_end_minus_1, 104, gas_id);
}

#ifdef FEATURE_VAMOS_II
/*===========================================================================
FUNCTION l1_setup_time_slot_params_vamos2_hr

DESCRIPTION
   This function sets up the time slot specific parameters for Half Rate
   traffic channel in case of VAMOS 2, TSC set 2.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_setup_time_slot_params_vamos2_hr( channel_information_T  *chnl1_info_ptr, gas_id_t gas_id )
{
   if ( !chnl1_info_ptr )
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL chnl1_info_ptr");
      return;
   }

   /*****************************************************************
    *
    *    Set up parameters that depend on the timeslot and subchannel
    *    used.  See 05.02 Clause 7 Table 1a and 05.08 Sec 8.4
    *
    *****************************************************************/
   switch ( (chnl1_info_ptr->TN) & 7 )
   {
      case 0:
      case 1:
         l1_tch_data[gas_id].sacch_minus_one_frame = ( 12 + ( chnl1_info_ptr->subchannel * 11) ) % 26;
         l1_tch_data[gas_id].idle_minus_one_frame = ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].idle_frame = ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = ( 103 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
         l1_tch_data[gas_id].send_sacch= ( 12 + ( chnl1_info_ptr->subchannel * 11) ) % 104;
         break;

      case 2:
      case 3:
         l1_tch_data[gas_id].sacch_minus_one_frame = ( 12 + ( chnl1_info_ptr->subchannel * 11) ) % 26;
         l1_tch_data[gas_id].idle_minus_one_frame = ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].idle_frame = ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
         l1_tch_data[gas_id].send_sacch= ( 38 + ( chnl1_info_ptr->subchannel * 11) ) % 104;
         break;

      case 4:
      case 5:
         l1_tch_data[gas_id].sacch_minus_one_frame = ( 12 + ( chnl1_info_ptr->subchannel * 11) ) % 26;
         l1_tch_data[gas_id].idle_minus_one_frame = ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].idle_frame = ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = ( 51 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
         l1_tch_data[gas_id].send_sacch= ( 64 + ( chnl1_info_ptr->subchannel * 11) ) % 104;
         break;

      case 6:
      case 7:
         l1_tch_data[gas_id].sacch_minus_one_frame = ( 12 + ( chnl1_info_ptr->subchannel * 11) ) % 26;
         l1_tch_data[gas_id].idle_minus_one_frame = ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].idle_frame = ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
         l1_tch_data[gas_id].reporting_period_end_minus_1 = ( 77 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
         l1_tch_data[gas_id].send_sacch= ( 90 + ( chnl1_info_ptr->subchannel * 11) ) % 104;
         break;

      default:
         break;
   }
}
#endif  /*FEATURE_VAMOS_II*/

/*===========================================================================
FUNCTION l1_setup_message_layer_hr

DESCRIPTION
   This function sets up the Message layer for Half Rate.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_setup_message_layer_hr(dedicated_data_T       *ded_dtr_ptr,
                                      channel_information_T  *chnl1_info_ptr, gas_id_t gas_id
                                     )
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   gl1_msg_tx_cb_type     tx_cb;
   gl1_msg_rx_cb_type     rx_cb;

   if ( !ded_dtr_ptr || ! chnl1_info_ptr )
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
                ded_dtr_ptr, chnl1_info_ptr);
      return;
   }

#if defined (FEATURE_GSM_DTM) && defined (FEATURE_GSM_EDTM)

   if( L1_EDTM_INIT == l1_tskisr_blk->sub_sub_state )
   {
      gl1_msg_reset_cs_only(gas_id);
   }
   else
   {
      gl1_msg_reset(gas_id);
   }
#else
   gl1_msg_reset(gas_id);
#endif

   gl1_msg_init_tch(l1_tch_data[gas_id].channel_type, l1_tch_data[gas_id].voc_type, gas_id);


   tx_cb.data_cb    = tch_tx_callback;
   tx_cb.metrics_cb = NULL;
   tx_cb.log_cb     = NULL;
   tx_cb.abort_cb   = NULL;
   rx_cb.data_cb    = tch_hr_rx_callback;
   rx_cb.tch_metrics_cb = tch_hr_metrics_callback;
#ifdef DED_LOG
   #error code not present
#else
   rx_cb.log_cb     = NULL;
#endif
   rx_cb.abort_cb   = NULL;
   gl1_msg_cfg_ul_tch(chnl1_info_ptr->TSC, &tx_cb, gas_id );

   tx_cb.data_cb    = facch_tx_callback;
   gl1_msg_cfg_ul_facch(chnl1_info_ptr->TSC, &tx_cb, gas_id );

   gl1_msg_cfg_dl_tch_facch(chnl1_info_ptr->TSC, &rx_cb, gas_id );

   tx_cb.data_cb    = sacch_tx_callback;
   tx_cb.abort_cb   = NULL;
   rx_cb.data_cb    = sacch_rx_callback;
   rx_cb.metrics_cb = sacch26_metrics_callback;
#ifdef DED_LOG
   #error code not present
#else
   rx_cb.log_cb     = NULL;
#endif
   rx_cb.abort_cb   = NULL;

   gl1_msg_cfg_ul_sacch(chnl1_info_ptr->TSC, &tx_cb, gas_id );
   gl1_msg_cfg_dl_sacch(chnl1_info_ptr->TSC, &rx_cb, gas_id );


#ifdef FEATURE_GSM_DTM
   if(l1_tskisr_blk->l1_state != L1_DTM_MODE)
   {
     /* Only setup AFC_TT_MODE to DCH if we are not in DTM
      * the main DTM handler gpl1_dtm.c will set this instead
      * based on type of lowest allocated timeslot
      */
     gl1_msg_set_afc_tt_mode(AFC_TT_MODE_DCH, gas_id);
   }
#endif

   gl1_msg_dch_afc_tt_init( chnl1_info_ptr->frequency_list.channel[0], gas_id );

   l1_tch_data[gas_id].hr_facch_tx_in_progress = 0;

   l1_tch_data[gas_id].idle_status = FALSE;

   /* Precompute TXLEVS for first reporting period.  Transmit at the
      command power level immediately as soon as we jump onto the new
      channel. ms_power_level_to_use is initially set to ms_power_level_in_use
      ,so L1_precompute_txlev will not step up/step down the power
      The 2dB step algo kicks in only after the next power
      level change commanded by the base station
   */

   L1_precompute_txlev( ded_dtr_ptr->ms_power_level_to_use,
                        ded_dtr_ptr->ms_power_level_in_use,
                        104,
                        chnl1_info_ptr->frequency_list.channel[0],
                        gas_id
                      );
}

/*===========================================================================
FUNCTION l1_process_rach_hr

DESCRIPTION
   This function processes the Half Rate RACH channel

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_process_rach_hr( dedicated_data_T       *ded_dtr_ptr,
                                channel_information_T  *chnl1_info_ptr, gas_id_t gas_id
                              )
{
   int i = 0;
   dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

   if ( !ded_dtr_ptr || !chnl1_info_ptr )
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
                ded_dtr_ptr, chnl1_info_ptr);
      return;
   }

   /*
      If there are handover access bursts to be transmitted,
      transmit them in consecutive frames on the FACCH
      (we exclude SACCH and idle frames)
   */
   if ( l1_tch_data[gas_id].access_burst_scheduled )
   {
      /* Transmit only on the assigned subchannel.  Remember that
        the actual burst gets sent out in the next frame
       */

      /* Do not send a rach if the FNmod26 % 13 is 11 for subchannel
         0 and 12 for subchannel 1
       */

      if ( ( chnl1_info_ptr->subchannel == 0 &&
             (( frame_counters[gas_id].FNmod13 )%2 == 1 || (frame_counters[gas_id].FNmod13) == 12) &&
             (frame_counters[gas_id].FNmod13) != 11
           ) ||
           ( chnl1_info_ptr->subchannel == 1 &&
             (( frame_counters[gas_id].FNmod13 )%2 == 0 || (frame_counters[gas_id].FNmod13) == 0)  &&
             (frame_counters[gas_id].FNmod13) != 12
           )
         )
      {
         /* Decrement # of handover acccess bursts to send */
         if ( ded_dtr_ptr->no_of_ab_to_send > 0 )
         {
            ded_dtr_ptr->no_of_ab_to_send--;
         }
         else
         {
            MSG_GERAN_ERROR_1_G("# of access bursts incorrect %d", ded_dtr_ptr->no_of_ab_to_send);
         }

         /* Get the ARFCN in frequency hopping sequence*/
         /* Passing in before and after freq parameters to L1_get_ARFCNS
            to guarantee a seamless transition between the before and
            after channels in the case of the start time lying in the
            middle of a block.  Not very elegant but the least complicated
            solution for now
         */
         L1_get_ARFCNS ( 1, 2, 4, l1_tch_data[gas_id].arfcns ,
                         &ded_dtr_ptr->channel_specification.channel_info_1_before,
                         &ded_dtr_ptr->channel_specification.channel_info_1_after,
                         &ded_dtr_ptr->new_channel_info, gas_id);
         /*
          Transmit handover access bursts. Single burst, so just
          pass in the first ARFCN in the block
         */
         gl1_msg_tx_rach(l1_tch_data[gas_id].arfcns[0], l1_dedicated_data_ptr->ms_power_level_to_use,
                         0, l1_dedicated_data_ptr->access_burst_content, gas_id);

         for ( i = 0 ; i < 4; i++ )
         {
            l1_tch_data[gas_id].txlev[i] =  l1_dedicated_data_ptr->ms_power_level_to_use;
         }

         /* Log tx burst metrics */
         l1_log_transmit_burst_metrics
         ( GL1_MSG_RACH,
           1, 1, 1,
           l1_tch_data[gas_id].arfcns,
           l1_tch_data[gas_id].txlev,
           chnl1_info_ptr->TSC,
           0, gas_id );


      }
   }
}

/*===========================================================================
FUNCTION l1_process_tch_hr

DESCRIPTION
   The function processes the Half Rate Traffic Channel. It implements
   the TX side implementation to resolve conflict between FACCH and TCH
   frames giving priority to FACCH. This function is also responsible for
   scheduling the frames.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_process_tch_hr( dedicated_data_T      *ded_dtr_ptr,
                               channel_information_T *chnl1_info_ptr, gas_id_t gas_id
                             )
{
#ifdef FEATURE_VAMOS_II
  gl1_vamos2_tch_wait_type vamos2_tch_wait_state = NO_WAIT;
#endif 
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   if ( !ded_dtr_ptr || !chnl1_info_ptr )
   {
      MSG_GERAN_ERROR_2_G( "Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
                 ded_dtr_ptr, chnl1_info_ptr);

      return;
   }

#ifdef FEATURE_VAMOS_II
   MSG_GERAN_HIGH_3_G("FN:%d,TSC:%d,sacch_minus_one_frame:%d",
                       gl1_get_FN(gas_id),
                       chnl1_info_ptr->TSC,
                       l1_tch_data[gas_id].sacch_minus_one_frame);

   if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3) && (chnl1_info_ptr->subchannel == 0))
	 {
	 if( (frame_counters[gas_id].FNmod13 == 3)||
         (frame_counters[gas_id].FNmod13 == 7)||
         (frame_counters[gas_id].FNmod26 == 11)||
         (frame_counters[gas_id].FNmod26 == 25)
       )
	   {		   
             l1_tch_data[gas_id].prepare_msg = TRUE;
             l1_tch_data[gas_id].block_start = TRUE;
	   }
	 }
   else
#endif /*FEATURE_VAMOS_II*/
    {
   /*
       HR subchannel 1 is essentially HR subchannel 0 shifted by 1
       frame, so we use the subchannel number to shift everything by
       a frame.  Only TCH/H is symmetric w.r.t FNmod13.  FACCH needs
       separate processing
   */
   switch ( ( frame_counters[gas_id].FNmod26 % 13 - chnl1_info_ptr->subchannel + 13 ) % 13 )
   {
      case 3:
      case 7:
      case 12:
         l1_tch_data[gas_id].prepare_msg = TRUE;
         l1_tch_data[gas_id].block_start = TRUE;
         break;
      default:
         break;
   }
   }

#ifdef FEATURE_VAMOS_II
   if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3))
   {
     if((frame_counters[gas_id].FNmod26 == 11) && (l1_tch_data[gas_id].sacch_minus_one_frame == 12) && (chnl1_info_ptr->subchannel == 0))
     {
       vamos2_tch_wait_state = WAIT_AFTER_BLOCK_START;
     }   
     if((frame_counters[gas_id].FNmod26 == 21) && (l1_tch_data[gas_id].sacch_minus_one_frame == 23) && (chnl1_info_ptr->subchannel == 1))
     {
       vamos2_tch_wait_state = WAIT_AFTER_BLOCK_START;
     }
  }
#endif

   l1_tch_data[gas_id].facch_tx_allowed = FALSE;
   l1_tch_data[gas_id].facch_rx_expected = FALSE;
   l1_tch_data[gas_id].middle_facch_block = FALSE;
#ifdef FEATURE_VAMOS_II
   l1_tch_data[gas_id].middle_facch_block_vamos2 = FALSE;
#endif

   /* FACCH HR needs special handling since its assymetric */
   /*
      HR 0 FACCH tx allowed on 0,8,17: FACCH rx on 6,15,23
      HR 1 FACCH tx allowed on 1,9,18: FACCH rx on 7,16,24

      Again its just offset by a frame, so we use the subchannel
      number to determine if it should be shifted or not

      The middle FACCH block (8,10,13,15) is assymetric because
      of the intervening SACCH burst, so we need to set a flag
      and pass it down to the msg layer to tell it to leave
      a 2 frame separation between bursts 2 & 3

      We also need to tell the msg layer when the FACCH rx is
      expected so that it can tell the mdsp to look for FACCH.
      We set this at the beginning of the block where the FACCH
      is expected.  The msg layer will pass it down to the mdsp
      only on the last burst when the decode is issued
   */
#ifdef FEATURE_VAMOS_II
   if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3) && (chnl1_info_ptr->subchannel == 0))
     {
   switch ( ( frame_counters[gas_id].FNmod26 % 26 - chnl1_info_ptr->subchannel + 26 ) % 26 )
   {
      case 7:
         l1_tch_data[gas_id].middle_facch_block = TRUE;
          l1_tch_data[gas_id].middle_facch_block_vamos2 = TRUE;
         /* Fall through */ /*lint !e616*/
      case 16:
      case 25:
         l1_tch_data[gas_id].facch_tx_allowed = TRUE;
         break;

      case 3:
        case 11:
        case 20:
          l1_tch_data[gas_id].facch_rx_expected = TRUE;
          break;
   
        default:
          break;
       }
     }
   else if ((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3) && (chnl1_info_ptr->subchannel == 1))
     {
     switch ( ( frame_counters[gas_id].FNmod26 % 26 - chnl1_info_ptr->subchannel + 26 ) % 26 )
      {   
        case 7:
          l1_tch_data[gas_id].middle_facch_block = TRUE;
          l1_tch_data[gas_id].facch_tx_allowed = TRUE;
          break;
        case 16:
          l1_tch_data[gas_id].middle_facch_block_vamos2 = TRUE;
          l1_tch_data[gas_id].facch_tx_allowed = TRUE;
          /* Fall through */ /*lint !e616*/
        case 25:
          l1_tch_data[gas_id].facch_tx_allowed = TRUE;
          break;
      case 3:
      case 12:
      case 20:
         l1_tch_data[gas_id].facch_rx_expected = TRUE;
         break;

      default:
         break;
   }
     }
   else
#endif /*FEATURE_VAMOS_II*/
   {
   switch ( ( frame_counters[gas_id].FNmod26 % 26 - chnl1_info_ptr->subchannel + 26 ) % 26 )
   {
      case 7:
         l1_tch_data[gas_id].middle_facch_block = TRUE;
         /* Fall through */ /*lint !e616*/
      case 16:
      case 25:
         l1_tch_data[gas_id].facch_tx_allowed = TRUE;
         break;

      case 3:
      case 12:
      case 20:
         l1_tch_data[gas_id].facch_rx_expected = TRUE;
         break;

      default:
         break;
   }
  }

   /* Ordered TA and TXLEV is applied only at the start of the next reporting period.
      We precompute TXLEV for the entire reporting period and then retrieve it later
      for each transmitted block.  We do this in reporting_period - 3 because the
      block is prepared and scheduled a few frames before its actually scheduled to
      start
   */
   if ( frame_counters[gas_id].FNmod104 == l1_tch_data[gas_id].reporting_period_end_minus_1 - 2 )
   {
      /*Update the power level and TA to use with that commanded by the base station */
      ded_dtr_ptr->ms_power_level_to_use =  ded_dtr_ptr->ordered_ms_power_level;
      ded_dtr_ptr->timing_advance_to_use =  ded_dtr_ptr->ordered_timing_advance;
      /* We need to report the power level of the last burst on the previous
         SACCH multiframe.   100 would be the start of the last block.
         103 would be the idle frame but the specs say we have to consider
         frames whether actual transmission takes place or not
      */
      L1_get_txlev( 100, 1, l1_tch_data[gas_id].txlev, gas_id );
      ded_dtr_ptr->ms_power_level_to_report = (byte)l1_tch_data[gas_id].txlev[3];

      L1_precompute_txlev( ded_dtr_ptr->ordered_ms_power_level,
                           ded_dtr_ptr->ms_power_level_in_use,
                           104,
                           chnl1_info_ptr->frequency_list.channel[0],
                           gas_id
                          );

      ded_dtr_ptr->timing_advance_in_use =
        L1_change_ta( ded_dtr_ptr->timing_advance_to_use,
                      ded_dtr_ptr->timing_advance_in_use,gas_id );

   }


   if ( l1_tch_data[gas_id].prepare_msg )
   {
      boolean    sending_FACCH  = FALSE;
      boolean    facch_filler_reqd = FALSE;

      /* Initialize tx_data */
      memset (l1_tch_data[gas_id].tx_data, NULL, TCH_FS_DATA_WIDTH_OCTETS);

      /* If previous block was FACCH, we need to send one more TCH block to
         ensure that the interleaver has 8 bursts, so override DEDICATED_STOP
         till the "completing" TCH block has been sent out
      */
      if ( l1_tskisr_blk->main_command != L1_DEDICATED_STOP || l1_tch_data[gas_id].prev_block_was_facch )
      {
         int16 offset = 1;
         uint8 FNrptperiod = 0;

         /* For half rate, FACCH is allowed only on 8 frame boundaries */
         if ( l1_tch_data[gas_id].facch_tx_allowed )
         {
           /* Did we break the loop to send FACCH in last block? If so re-enable
            * loopback */
           if(l1_tch_data[gas_id].enable_loopback_after_facch_block == TRUE)
           {
             /* Wait for interleaving, FACCH iis interleaved over 8 frames so we need to
              * wait for 2 blocks before closing the loop again
              */
             if(++l1_tch_data[gas_id].facch_blk_count == 2)
             {
               l1_tch_data[gas_id].enable_loopback_after_facch_block = FALSE;

               /* If we broke loopback to send a FACCH not related to Opening loopback
                * C, resume loopback
                */
               if(ded_dtr_ptr->l1_loopback_disabled == LOOPBACK_C_TEMP_OPEN)
               {
                 ded_dtr_ptr->l1_loopback_disabled = LOOPBACK_C_CLOSED;
                 gl1_msg_tch_loopback(GL1_DEFS_LOOPBACK_TYPE_C, gas_id);
               }
               l1_tch_data[gas_id].facch_blk_count = 0;
             }
           }

           /* Is there a facch to transmit */
           if ( ded_dtr_ptr->channel_specification.channel_mode_1 == SIGNALLING_ONLY)
           {
              facch_filler_reqd = TRUE;
           }

           /*
            * if there is new FACCH data or if it is a
            * SIGNALLING only channel or if the vocoder is not
            * initialized or a voc packet is not ready, tx valid FACCH or
            *  transmit FACCH fill frames.  This prevents garbage packets
            *  from being transmitted before voc is initialized.   We need
            *  to transmit right away to meet 11.10
            */
#ifdef FEATURE_GSM_COEX
            if( L1_get_CCH_message( facch_filler_reqd, FACCH_H, ded_dtr_ptr, l1_tch_data[gas_id].tx_data, &l1_tch_data[gas_id].data_length, gas_id) == FALSE )
            {
              if( (l1_tch_data[gas_id].coex_denials_for_facch <= MAX_COEX_DENIALS_FOR_FACCH) &&
                  (l1_tch_data[gas_id].coex_request_tx != NULL) &&
                  (l1_tch_data[gas_id].coex_request_tx(GL1_MSG_FACCH_HS, gas_id) == FALSE) )
              {
                L1_get_CCH_filler_message( FACCH_H, l1_tch_data[gas_id].tx_data, &l1_tch_data[gas_id].data_length, gas_id );

                l1_tch_data[gas_id].facch_not_available = TRUE;

                l1_tch_data[gas_id].coex_denials_for_facch++;
              }
              else
              {
                L1_free_CCH_message(FACCH_H, ded_dtr_ptr, gas_id);

                l1_tch_data[gas_id].facch_not_available = FALSE;

                l1_tch_data[gas_id].coex_denials_for_facch = 0;
              }
            }
			else
			{
			   l1_tch_data[gas_id].facch_not_available = TRUE;
			}
#else
            l1_tch_data[gas_id].facch_not_available =
            L1_get_CCH_message( facch_filler_reqd, FACCH_H, ded_dtr_ptr, l1_tch_data[gas_id].tx_data, &l1_tch_data[gas_id].data_length, gas_id);
#endif

            if ( ded_dtr_ptr->channel_specification.channel_mode_1 == SIGNALLING_ONLY
                 || l1_tch_data[gas_id].facch_not_available == FALSE )
            {
               sending_FACCH = TRUE;
            }
         }

         /* Get the next block of ARFCN's and signal strengths */

         /*
          * For subchannel 1 we prepare message in the 11th and 24th frame of the
          * 26 frame multi-frame structure and not in the 10th and 23rd frame. Thus,
          * if the (FNmod26 % 13) is (10 + chnl1_info_ptr->subchannel), we want to
          * account for the SACCH frame in middle and the offset should be 3.
          */

         /* Passing in before and after freq parameters to L1_get_ARFCNS
            to guarantee a seamless transition between the before and
            after channels in the case of the start time lying in the
            middle of a block.  Not very elegant but the least complicated
            solution for now
         */
         L1_get_ARFCNS ( offset, 2, 4, l1_tch_data[gas_id].arfcns ,
                         &ded_dtr_ptr->channel_specification.channel_info_1_before,
                         &ded_dtr_ptr->channel_specification.channel_info_1_after,
                         &ded_dtr_ptr->new_channel_info, gas_id );

#ifdef FEATURE_VAMOS_II
		if ((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3))
		{
		   if((chnl1_info_ptr->subchannel == 0) && (vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START))
		   {		   
		   offset = 4;
		   L1_get_ARFCNS ( offset, 2, 3, l1_tch_data[gas_id].arfcns + 1 ,
						   &ded_dtr_ptr->channel_specification.channel_info_1_before,
						   &ded_dtr_ptr->channel_specification.channel_info_1_after,
						   &ded_dtr_ptr->new_channel_info,gas_id );
		   }
		   else if( (chnl1_info_ptr->subchannel == 0) && (l1_tch_data[gas_id].middle_facch_block_vamos2) )
		   {
		   offset = 8;
		   L1_get_ARFCNS ( offset, 2, 1, l1_tch_data[gas_id].arfcns + 3 ,
						   &ded_dtr_ptr->channel_specification.channel_info_1_before,
						   &ded_dtr_ptr->channel_specification.channel_info_1_after,
						   &ded_dtr_ptr->new_channel_info,gas_id );
		   }		   
		   else if((chnl1_info_ptr->subchannel == 1) && (vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START))
		   {		   
		   offset = 4;
		   L1_get_ARFCNS ( offset, 2, 3, l1_tch_data[gas_id].arfcns + 1 ,
						   &ded_dtr_ptr->channel_specification.channel_info_1_before,
						   &ded_dtr_ptr->channel_specification.channel_info_1_after,
						   &ded_dtr_ptr->new_channel_info,gas_id );
		   }
		   else if((chnl1_info_ptr->subchannel == 1) && (l1_tch_data[gas_id].middle_facch_block))
		   {			   
		   offset = 6;
		   L1_get_ARFCNS
		   (offset,
			2,
			2, l1_tch_data[gas_id].arfcns + 2 ,
			&ded_dtr_ptr->channel_specification.channel_info_1_before,
			&ded_dtr_ptr->channel_specification.channel_info_1_after,
			&ded_dtr_ptr->new_channel_info, gas_id );
		   }
		   else if((chnl1_info_ptr->subchannel == 1) && (l1_tch_data[gas_id].middle_facch_block_vamos2))
		   {			   
		   offset = 8;
		   L1_get_ARFCNS ( offset, 2, 1, l1_tch_data[gas_id].arfcns + 3 ,
						   &ded_dtr_ptr->channel_specification.channel_info_1_before,
						   &ded_dtr_ptr->channel_specification.channel_info_1_after,
						   &ded_dtr_ptr->new_channel_info,gas_id );
		   }			   
	   }
	else
#endif
       {
         /*
            Middle FACCH block is assymetric because the SACCH comes
            in between, recompute ARFCNs for last two bursts
         */
         if ( l1_tch_data[gas_id].middle_facch_block )
         {
            offset = 6;
            L1_get_ARFCNS ( offset, 2, 2, l1_tch_data[gas_id].arfcns + 2 ,
                            &ded_dtr_ptr->channel_specification.channel_info_1_before,
                            &ded_dtr_ptr->channel_specification.channel_info_1_after,
                            &ded_dtr_ptr->new_channel_info, gas_id );
          }
        }

         /* FNrptperiod is the FN relative to the start of the reporting period
            This is used to index into the precomputed TXLEVs since they are computed
            with reference to the start of the reporting period
         */
         /* As below logic gives correct FN relative to the start of the reporting period
            adding offset is not needed as relative FN can go wrong when offset is different in VAMOS cases
            which may cause Tx power fluctuation
          */

         FNrptperiod = SUB_MOD ( frame_counters[gas_id].FNmod104 , l1_tch_data[gas_id].reporting_period_end_minus_1 , 104 );

         /* Convert TXLEV to power index before passing to the msg layer */
         L1_get_txlev( FNrptperiod, 1, l1_tch_data[gas_id].txlev, gas_id );

         /* Update ded data ptr with most recent power level*/
         ded_dtr_ptr->ms_power_level_in_use = (byte)l1_tch_data[gas_id].txlev[3];


         /*
             Don't transmit anything else if transmitting handover access bursts
         */
         if ( l1_tch_data[gas_id].access_burst_scheduled )
         {
         }
         /* Now tx a tch or facch message */
         else if ( sending_FACCH )
         {
            MSG_GERAN_MED_3_G("FACCH transmitted 0x%x 0x%x 0x%x",l1_tch_data[gas_id].tx_data[0],l1_tch_data[gas_id].tx_data[1],l1_tch_data[gas_id].tx_data[2]);
            gl1_msg_tx_facch_hr(l1_tch_data[gas_id].arfcns, l1_tch_data[gas_id].txlev, l1_tch_data[gas_id].tx_data, l1_tch_data[gas_id].middle_facch_block,
#ifdef FEATURE_VAMOS_II
                                l1_tch_data[gas_id].middle_facch_block_vamos2,
                                vamos2_tch_wait_state,
#endif 
                                gas_id );

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_FACCH_TX);
#endif

            l1_tch_data[gas_id].hr_facch_tx_in_progress++;
            l1_tch_data[gas_id].prev_block_was_facch = TRUE;
            /* This counter is used to reset the prev_block_was_facch if we are
             * in UL DTX mode - without the counter we will not be able to abort
             * the TCH in this scenario - CR169424
             */
            if(l1_tskisr_blk->main_command != L1_DEDICATED_STOP)
            {
              l1_tch_data[gas_id].prev_block_was_facch_counter = L1_WAIT_FOUR_HR_BURSTS_BEFORE_ABORT;
            }

            /* On UL B0 is 11 frames, B1, B2 are 12 frames wide. We need
             * to account for this if we wish to delay prev_blk_was_facch
             * flag being reset until 4 bursts have been TX'd
             */
            if ((( frame_counters[gas_id].FNmod26 % 26 - chnl1_info_ptr->subchannel + 26 ) % 26 ) != 25)
            {
                l1_tch_data[gas_id].prev_block_was_facch_counter++;
            }

            /* Tx FACCH during TCH loopback, loopback was opened when we received PH_DATA_REQ in l1_ded-if.c
             * so re-enable once FACCH has been transmitted. Note this take 8 frames due to interleaving
             */
            l1_tch_data[gas_id].enable_loopback_after_facch_block = TRUE;

            /* Log tx burst metrics */
            l1_log_transmit_burst_metrics
            ( GL1_MSG_FACCH_HS,
              1, 2, 4,
              l1_tch_data[gas_id].arfcns,
              l1_tch_data[gas_id].txlev,
              chnl1_info_ptr->TSC,
              ded_dtr_ptr->timing_advance_in_use, gas_id );

            /* Delay the SID by a block each time a FACCH block preempts the
               SID block.  This is mandated in the specs.  SID frames are
               different for each subchannel
            */
            if ( chnl1_info_ptr->subchannel == 0 )
            {
              /* TCH/HS Sub 0 the SID bursts are 0,2,4,6 and 52,54,56,58. We schedule one frame
               * in advance of the burst, and the frame later handler sends two bursts for one
               * schedule here, so FnMod104 switch works like this:
               * PL1 FnMod104 NPL1 (hr_tx_handler)
               * 103 0,2
               *   3 4,6
               *  51 52,54
               *  55 56,58
               */

              switch(frame_counters[gas_id].FNmod104)
              {
                case 103:
                case 3:
                case 51:
                case 55:
                {
                  if(l1_tch_data[gas_id].facch_not_available == FALSE)
                  {
                    l1_tch_data[gas_id].num_sids_pending = 2;
                  }
                  break;
                }

                default:
                  break;
              }
            }
            else
            {
              switch(frame_counters[gas_id].FNmod104)
              {
              /* TCH/HS Sub 0 the SID bursts are 0,2,4,6 and 52,54,56,58. We schedule one frame
               * in advance of the burst, and the frame later handler sends two bursts for one
               * schedule here, so FnMod104 switch works like this:
               * PL1 FnMod104 NPL1 (hr_tx_handler)
               * 103 0,2
               *   3 4,6
               *  51 52,54
               *  55 56,58
               */

                case 13:
                case 17:
                case 65:
                case 69:
                {
                  if(l1_tch_data[gas_id].facch_not_available == FALSE)
                  {
                    l1_tch_data[gas_id].num_sids_pending = 2;
                  }
                  break;
                }

                default:
                  break;
              }
            }
         }
         else
         {
            boolean sid_frame = FALSE;

            /* SID frames are different for each subchannel.
               See 05.08 Sec 8.3
            */
            if ( chnl1_info_ptr->subchannel == 0 )
            {
              /* TCH/HS Sub 0 the SID bursts are 0,2,4,6 and 52,54,56,58. We schedule one frame
               * in advance of the burst, and the frame later handler sends two bursts for one
               * schedule here, so FnMod104 switch works like this:
               * PL1 FnMod104 NPL1 (hr_tx_handler)
               * 103 0,2
               *   3 4,6
               *  51 52,54
               *  55 56,58
               */

              switch(frame_counters[gas_id].FNmod104)
              {
                case 103:
                case 3:
                case 51:
                case 55:
                {
                  sid_frame = TRUE;
                  break;
                }

                default:
                  break;
              }
            }
            else
            {
              /* TCH/HS Sub 0 the SID bursts are 0,2,4,6 and 52,54,56,58. We schedule one frame
               * in advance of the burst, and the frame later handler sends two bursts for one
               * schedule here, so FnMod104 switch works like this:
               * PL1 FnMod104 NPL1 (hr_tx_handler)
               * 103 0,2
               *   3 4,6
               *  51 52,54
               *  55 56,58
               */

              switch(frame_counters[gas_id].FNmod104)
              {
                case 13:
                case 17:
                case 65:
                case 69:
                {
                  sid_frame = TRUE;
                  break;
                }

                default:
                  break;
              }
            }

            /* No need to tx if silence is found by vocoder, However on frames  */
            /* 52->59, we must tx.  The msg layer is invoked a few frames prior */
            /* If previous block was FACCH, we need to transmit a TCH block so
               that we have 8 bursts to complete the FACCH interleaver. Otherwise
               the FACCH will not get sent out correctly.   prev_dtx != dtx checks
               if there was a transition in the dtx state (from OFF to ON).  Need
               to transmit a SID frame in this case too as the specs say.
               sid_delay shifts the SID window if we're preempted by a FACCH
               Also if a FACCH has been scheduled, don't tranmit a TCH till it
               completes because it spans 4 bursts
            */
            if ( ( (l1_tch_data[gas_id].dtx==FALSE) ||
                   (sid_frame) ||
                   (l1_tch_data[gas_id].prev_block_was_facch) ||
                   ( l1_tch_data[gas_id].tx_homing_sequence ) ||
                   ( l1_tch_data[gas_id].num_sids_pending ) ) &&
                 ( l1_tch_data[gas_id].hr_facch_tx_in_progress == FALSE ) )
            {
               gl1_msg_tx_tch_hr(l1_tch_data[gas_id].arfcns, l1_tch_data[gas_id].txlev, l1_tch_data[gas_id].voc_tx_data,
#ifdef FEATURE_VAMOS_II
                                 vamos2_tch_wait_state,
#endif
                                 gas_id );

               /* Log tx burst metrics */
               l1_log_transmit_burst_metrics
               ( GL1_MSG_TCH_HS,
                 1, 2, 2,
                 l1_tch_data[gas_id].arfcns,
                 l1_tch_data[gas_id].txlev,
                 chnl1_info_ptr->TSC,
                 ded_dtr_ptr->timing_advance_in_use, gas_id );

               if ( l1_tch_data[gas_id].num_sids_pending )
               {
                  MSG_GERAN_MED_2_G("Tx HR pending SID fn=%d,num_sid=%d",frame_counters[gas_id].FNmod104,l1_tch_data[gas_id].num_sids_pending);
                  l1_tch_data[gas_id].num_sids_pending--;
               }

               if ( sid_frame )
               {
                  MSG_GERAN_MED_1_G("Tx HR SID at fn=%d",frame_counters[gas_id].FNmod104);
               }

               if ( l1_tch_data[gas_id].prev_dtx != l1_tch_data[gas_id].dtx )
               {
                  MSG_GERAN_MED_3_G("DTX change %d->%d fn=%d",l1_tch_data[gas_id].prev_dtx,l1_tch_data[gas_id].dtx,frame_counters[gas_id].FNmod104);
               }

               if ( l1_tch_data[gas_id].tx_homing_sequence )
               {
                  MSG_GERAN_MED_1_G("Homing seq transmitted fn=%d",frame_counters[gas_id].FNmod104);
               }

            }
         }

         /* Signal the subchannel for DL DTX detection. */
         gl1_msg_rx_tch_hr_sub_chan(chnl1_info_ptr->subchannel, gas_id);

         /* Go get an rx message */
         gl1_msg_rx_tch_facch_hr(l1_tch_data[gas_id].arfcns, l1_tch_data[gas_id].agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                                 l1_tch_data[gas_id].agc_ptr_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/   
                                 &l1_tch_data[gas_id].t_rpt, l1_tch_data[gas_id].facch_rx_expected,
#ifdef FEATURE_VAMOS_II
                                 vamos2_tch_wait_state,
#endif
                                 gas_id);

      }

   }
   if (l1_tch_data[gas_id].prev_block_was_facch)
   {
     l1_tch_data[gas_id].prev_block_was_facch_counter--;
     if (l1_tch_data[gas_id].prev_block_was_facch_counter <= 0)
     {
       l1_tch_data[gas_id].prev_block_was_facch = FALSE;
     }
     if (l1_tch_data[gas_id].prev_block_was_facch_counter < 0)
     {
       MSG_GERAN_ERROR_1_G("HR:prev_block_was_facch_cnt =%d",l1_tch_data[gas_id].prev_block_was_facch_counter);
     }
   }
}

/*===========================================================================
FUNCTION l1_tch_abort_hr

DESCRIPTION
   Function to abort the Half Rate Traffic Channel

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_tch_abort_hr( gas_id_t gas_id )
{
   /* Wait for SCE */
   l1_tch_data[gas_id].idle_status = !l1_sci_is_op_in_progress(gas_id);

   /*Check if GL1 is stuck in this state because of unfinished IRAT activity*/
   gl1_check_if_irat_state_stuck(gas_id);

   /* Abort Tx SACCH */
   gl1_msg_abort_tx_sacch(gas_id);

   /* Poll TCH and FACCH aborts.  These aborts return TRUE only after
   the tx block has been completed.  This is required to
   satisfy the 20 ms constraint - i.e the time interval between the
   last burst on the old channel and the first burst on the new channel
   should be <= 20ms.  The 20ms window applies to the last complete
   tx block which the network sees, so an incomplete block increases
   this window causing us to fail this 11.10 test case
  */
   /* Abort Tx TCH */
   l1_tch_data[gas_id].idle_status &= gl1_msg_abort_tx_tch_facch_hr(gas_id);

   /* Abort Rx SACCH */
   gl1_msg_abort_rx_sacch(gas_id);

   /* Wait for TCH rx to complete */
   l1_tch_data[gas_id].idle_status &= gl1_msg_abort_rx_tch_hr(gas_id);

   /* let SCE terminate - it already knows it should */
   l1_sci_tick_receive(gas_id);

   MSG_GERAN_MED_1_G("Tch HR abort, idle status %d", l1_tch_data[gas_id].idle_status);

}

/******************************************************************************
 *
 *  Function name:  L1_set_to_tch_mode
 *  ------------------------------------
 *
 *  Description:
 *  ------------
 *  This function sets up the parameters to allow a jump to TCH from the ISR
 *  context.
 *
 *
 *  Parameters:
 *  -----------
 *
 *
 *  Returns:
 *  --------
 *
 *
 *****************************************************************************/
#ifdef JUMP_TO_TCH
#error code not present
#endif



/*****************************************************

 CALLBACK FUCNTIONS

******************************************************/


/*===========================================================================

FUNCTION tch_tx_callback

DESCRIPTION
  This function is called in the first frame of a TCH block (ie 4 frames).
  Currently no activity is needed here.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tch_tx_callback( gas_id_t gas_id )
{

   /* This callback is invoked in the frame the first burst is scheduled.
      i.e FNmod13 = 4,8,12.  We set this flag to FALSE here because if we
      set it when we prepare the TCH msg (FnMod13 = 2,6 wtc), the abort
      (which checks for prev_block_was_facch == FALSE)
      kicks in immediately even before the completing TCH block is scheduled.
      Thus we need to set this after the first burst of the completing TCH
      block has been scheduled
   */
   l1_tch_data[gas_id].prev_block_was_facch = FALSE;
   /* reset counter */
   l1_tch_data[gas_id].prev_block_was_facch_counter = 0;
}

/*===========================================================================

FUNCTION facch_tx_callback

DESCRIPTION
  This function is called in the first frame of a FACCH block (ie 4 frames).
  Currently no activity is needed here.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void facch_tx_callback( gas_id_t gas_id )
{
   /* nothing to do */

   /*
      Needed for HR. Otherwise we won't complete the interleaver
      after the FACCH
   */
   if ( l1_tch_data[gas_id].hr_facch_tx_in_progress )
   {
      l1_tch_data[gas_id].hr_facch_tx_in_progress--;
   }

   /* This is for a signaling only channel.   facch_not_available in this case
      indicates whether the FACCH block for which this callback is invoked is
      a filler frame or not.  If its a filler frame, its basically a completion
      block for the 8 bursts, so we can set prev_block_was_facch = FALSE
   */
   if ( l1_tch_data[gas_id].facch_not_available )
   {
      l1_tch_data[gas_id].prev_block_was_facch = FALSE;
      /* reset counter */
      l1_tch_data[gas_id].prev_block_was_facch_counter = 0;
   }
}




/*===========================================================================

FUNCTION tch_fr_rx_callback

DESCRIPTION
  This function is called when the data from the TCH has been decoded. The
  data is then passed to the vocoder or up to L2 if it is a FACCH message.
  Receiver Ready (layer 2) are filtered and not passed on up to reduce
  overhead.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tch_fr_rx_callback(gl1_msg_decode_rpt *rpt_i, gas_id_t gas_id )
{
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
//#ifdef TCH_PROFILING
//   static uint16    crc_cnt=0;
//   static uint16    crc_bad=0;
//   static uint16    bfi_cnt=0;
//   static uint16    total_crc_cnt=0;
//   static uint16    total_crc_bad=0;
//   static uint16    total_bfi_cnt=0;
//#endif
   uint16 burst_fn=0;

   boolean taf = FALSE;
   tch_facch_decode_rpt * rpt = (tch_facch_decode_rpt*)rpt_i;
#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_rxd_control_type gl1_rxd_ctl_flag;
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   

   if ( !rpt )
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
      return;
   }

   burst_fn = SUB_MOD ( frame_counters[gas_id].FNmod104, rpt->offset_to_sob, 104);

   /* Do RXLEV_VAL processing */
   gl1_update_emr_parameters ( &(rpt->hdr), gl1_is_sid_fn (burst_fn, gas_id), TRUE, gas_id );

   /* Mark the first block as always bad because we need 8 burst to get
    * good data. Throw away data but store the metrics
    */
   if ( l1_tch_data[gas_id].first_rx_block )
   {
      rpt->hdr.good_data = 0;
      rpt->hdr.bfi = 1;

#ifdef TCH_PROFILING
      /* Reset counters */
      l1_tch_data[gas_id].total_crc_cnt_fr = l1_tch_data[gas_id].total_crc_bad_fr = l1_tch_data[gas_id].total_bfi_cnt_fr = 0;
      l1_tch_data[gas_id].crc_cnt_fr = l1_tch_data[gas_id].crc_bad_fr = l1_tch_data[gas_id].bfi_cnt_fr = 0;
#endif

      /* LOG MESSAGE METRICS */
      l1_log_message_metrics ( (gl1_msg_chan_type)rpt->hdr.chan, (gl1_msg_decode_rpt*)rpt, gas_id );
      l1_log_message_metrics_compact ( (gl1_msg_chan_type)rpt->hdr.chan, (gl1_msg_decode_rpt*)rpt, gas_id );
      l1_tch_data[gas_id].first_rx_block = FALSE;
      return;
   }

   /* LOG MESSAGE METRICS */
   l1_log_message_metrics ( (gl1_msg_chan_type)rpt->hdr.chan, (gl1_msg_decode_rpt*)rpt, gas_id );
   l1_log_message_metrics_compact ( (gl1_msg_chan_type)rpt->hdr.chan, (gl1_msg_decode_rpt*)rpt, gas_id );

   if ( (gl1_msg_chan_type)rpt->hdr.chan== GL1_MSG_FACCH )
   {

      /* BFI always set to 1 for FACCH */
      /* DSP supposed to handle this but this is just to be sure */
      rpt->hdr.bfi = 1;
    
      l1_log_rfacch_metrics((gl1_msg_decode_rpt*)rpt, gas_id);

      /*check to see if R-FACCH support is active*/
      if (l1_handle_rfacch(rpt, GL1_MSG_FACCH, gas_id))

      {
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_VALID_FACCH_RX);
#endif
        MSG_GERAN_MED_0_G("Receiving FACCH!");
        L1_send_PH_DATA_IND (TRUE,
                             DCCH,
                             FALSE, /* no paging involved */
                             NULL_ARFCN,   /*ARFCN only in idle mode*/
                             rpt->data,
                             N201_MAX, gas_id);
      }
   }

   if (!rpt->hdr.good_data)
   {
#ifdef TCH_PROFILING
      l1_tch_data[gas_id].crc_bad_fr++;
      l1_tch_data[gas_id].total_crc_bad_fr++;
#endif
   }

   /* burst_fn is mod 104 since its used to determine the SID frames */
   burst_fn = SUB_MOD(frame_counters[gas_id].FNmod104, rpt->offset_to_sob, 104);


   /* Use only 2nd SID block for RXQUAL_SUB */
   if ( burst_fn == 56 )
   {
      gl1_msg_rxqual_sub_update( (uint16)rpt->hdr.rx_qual, gas_id );
#ifdef RXQUAL_MSG
      MSG_GERAN_MED_1_G("Update: rxqual_sub tch/f(dtx) = %d",rpt->hdr.rx_qual);
#endif

      /*
      Also set TAF for the 2nd SID frame i.e starting on block 56 (aligned with
      SACCH multiframe as defined in 05.08
      */
      taf = TRUE;
   }

   /* Don't pass FACCH down to vocoder */
   if ( (gl1_msg_chan_type)rpt->hdr.chan != GL1_MSG_FACCH )
   {
      l1_vs_send_gsm_packet_to_client(rpt->data,
                                      (uint8)rpt->hdr.bfi,
                                      FALSE, FALSE,
                                      taf, gas_id); 
   }
   else
   {
      MSG_GERAN_MED_2_G("Chan type %d crc=%d not passing to voc",rpt->hdr.chan,rpt->hdr.good_data);
   }

  #ifdef FEATURE_GSM_DYNAMIC_SAIC
   /* If SAIC NV is disabled then don't run SAIC disable code */
   if(gl1_hw_feature_disabled_saic(gas_id) != TRUE)
   {
     MSG_GERAN_LOW_3_G(" RXQUAL %d saic d/e %d state %d",rpt->hdr.rx_qual,gl1_msg_get_saic_state(gas_id),l1_dedicated_data_ptr->l1_saic_dis_ena_state);

     /* If We receive a block with less than perfect RXQUAL when SAIC is OFF then enable SAIC */
     if( (rpt->hdr.rx_qual < L1_PERFECT_RXQUAL) &&
         (gl1_msg_get_saic_state(gas_id) == FALSE) &&
         (l1_dedicated_data_ptr->l1_saic_dis_ena_state == FALSE) )
     {
       /* Turn SAIC ON */
       gl1_hw_cfg_saic(TRUE, gas_id);
       gl1_hw_mcpm_amr_aeq_update(TRUE, gas_id);

       /* Set SAIC DISABLE/ENABLE STATE to stop Flip-Flopping of SAIC state */
       l1_dedicated_data_ptr->l1_saic_dis_ena_state = TRUE;
     }

   }
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);
   if(gl1_rxd_ctl_flag.enableRxd) 
     {
     MSG_GERAN_HIGH_3_G("TCH FR Hard RxQUAL:%d,FN:%d rxd_l1_en_flag: %d",rpt->hdr.rx_qual,gl1_get_FN(gas_id), l1_dedicated_data_ptr->l1_enable_rxdiversity);
     if((rpt->hdr.rx_qual < RXD_HARD_RXQUAL_THRESHOLD) &&
        (l1_dedicated_data_ptr->l1_enable_rxdiversity == FALSE) )
       {
       l1_dedicated_data_ptr->l1_enable_rxdiversity = TRUE;
       gl1_set_enable_rxdiversity(TRUE,gas_id);
       l1_ded_drx_agc_init[gas_id] = TRUE;
       }
     }
#endif


   /* Use all blocks for RXQUAL_FULL */
   gl1_msg_rxqual_full_update( (uint16)rpt->hdr.rx_qual, gas_id );
#ifdef RXQUAL_MSG
   MSG_GERAN_MED_1_G("Update: rxqual_full tch/f = %d",rpt->hdr.rx_qual);
#endif

#ifdef TCH_PROFILING
   if (rpt->hdr.bfi && (gl1_msg_chan_type)rpt->hdr.chan != GL1_MSG_FACCH)
   {
      l1_tch_data[gas_id].total_bfi_cnt_fr++;
      l1_tch_data[gas_id].bfi_cnt_fr++;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
      gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_BFI_RCVD);
#endif
   }

   l1_tch_data[gas_id].total_crc_cnt_fr++;
   if ( ++l1_tch_data[gas_id].crc_cnt_fr >= 100 )
   {
      MSG_6(MSG_SSID_DFLT, MSG_LEGACY_MED,
            "CRC errors: %d/100, %d/%d  (BFI: %d/100, %d/%d)",
            l1_tch_data[gas_id].crc_bad_fr, l1_tch_data[gas_id].total_crc_bad_fr, l1_tch_data[gas_id].total_crc_cnt_fr, l1_tch_data[gas_id].bfi_cnt_fr, l1_tch_data[gas_id].total_bfi_cnt_fr,
            l1_tch_data[gas_id].total_crc_cnt_fr);
      l1_tch_data[gas_id].crc_cnt_fr=0;
      l1_tch_data[gas_id].crc_bad_fr=0;
      l1_tch_data[gas_id].bfi_cnt_fr=0;
   }
#endif

}


/*===========================================================================

FUNCTION tch_hr_rx_callback

DESCRIPTION
  This function is called when the data from the TCH has been decoded. The
  data is then passed to the vocoder or up to L2 if it is a FACCH message.
  Receiver Ready (layer 2) are filtered and not passed on up to reduce
  overhead.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tch_hr_rx_callback(gl1_msg_decode_rpt *rpt_i, gas_id_t gas_id )
{
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
//#ifdef TCH_PROFILING
//   static uint16    crc_cnt=0;
//   static uint16    crc_bad=0;
//   static uint16    bfi_cnt=0;
//   static uint16    total_crc_cnt=0;
//   static uint16    total_crc_bad=0;
//   static uint16    total_bfi_cnt=0;
//#endif
//
//   static uint16 bep[4];
//   static uint8 bep_index = 0;
   uint8 i = 0;
   boolean compute_bep = FALSE;

   uint16 burst_fn=0;

   boolean taf = FALSE;
   tch_facch_decode_rpt *rpt = (tch_facch_decode_rpt *)rpt_i;
#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_rxd_control_type gl1_rxd_ctl_flag;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

   if ( !rpt )
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
      return;
   }

   burst_fn = SUB_MOD( frame_counters[gas_id].FNmod104, rpt->offset_to_sob, 104);

   if ( l1_tch_data[gas_id].first_rx_block )
   {
      l1_tch_data[gas_id].bep_index = 0;
   }

   /* Accumulate 4BEPs from 2 HR blocks */
   for ( i = 0; i < 2; i++ )
   {
      l1_tch_data[gas_id].bep[l1_tch_data[gas_id].bep_index++] = rpt->hdr.bep[i];
   }

   /* After accumulating 4 bursts, copy it back to rpt hdr to pass
    * down to gl1_update_emr_parameters
    */
   if ( l1_tch_data[gas_id].bep_index == 4 )
   {
      for ( i = 0; i < l1_tch_data[gas_id].bep_index; i++ )
      {
         rpt->hdr.bep[i] = l1_tch_data[gas_id].bep[i];
#ifdef DEBUG_GSM_GPRS_ENHANCED_MEASUREMENT_REPORTING
         #error code not present
#endif
      }
      compute_bep = TRUE;
      l1_tch_data[gas_id].bep_index = 0;
   }

   /* Do RXLEV_VAL processing */
   gl1_update_emr_parameters ( &(rpt->hdr), gl1_is_sid_fn (burst_fn, gas_id), compute_bep, gas_id );

   /* Mark the first block as always bad because we need 8 burst to get
    * good data. Throw it away but log the metrics
    */
   if ( l1_tch_data[gas_id].first_rx_block )
   {
      rpt->hdr.good_data = 0;
      rpt->hdr.bfi = 1;

#ifdef TCH_PROFILING
      /* Reset counters */
      l1_tch_data[gas_id].total_crc_cnt_hr = l1_tch_data[gas_id].total_crc_bad_hr = l1_tch_data[gas_id].total_bfi_cnt_hr = 0;
      l1_tch_data[gas_id].crc_cnt_hr = l1_tch_data[gas_id].crc_bad_hr = l1_tch_data[gas_id].bfi_cnt_hr = 0;
#endif

      /* LOG MESSAGE METRICS */
      l1_log_message_metrics ( (gl1_msg_chan_type)rpt->hdr.chan, (gl1_msg_decode_rpt*)rpt, gas_id );
      l1_log_message_metrics_compact ( (gl1_msg_chan_type)rpt->hdr.chan, (gl1_msg_decode_rpt*)rpt, gas_id );

      l1_tch_data[gas_id].first_rx_block = FALSE;
      return;
   }

   /* LOG MESSAGE METRICS */
   l1_log_message_metrics ( (gl1_msg_chan_type)rpt->hdr.chan, (gl1_msg_decode_rpt*)rpt, gas_id );
   l1_log_message_metrics_compact ( (gl1_msg_chan_type)rpt->hdr.chan, (gl1_msg_decode_rpt*)rpt, gas_id );

   if ( (gl1_msg_chan_type)rpt->hdr.chan== GL1_MSG_FACCH )
   {
      /* BFI always set to 1 for FACCH */
      /* DSP supposed to handle this but this is just to be sure */
      rpt->hdr.bfi = 1;

/*check to see if R-FACCH support is active*/
      l1_log_rfacch_metrics((gl1_msg_decode_rpt*)rpt, gas_id);

      if (l1_handle_rfacch(rpt, GL1_MSG_FACCH_HS, gas_id))
      {
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
         gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_VALID_FACCH_RX);
#endif
         MSG_GERAN_MED_0_G("Receiving HR FACCH!");
         L1_send_PH_DATA_IND (TRUE,
                              DCCH,
                              FALSE, /* no paging involved */
                              NULL_ARFCN,   /*ARFCN only in idle mode*/
                              &rpt->data[0],
                              N201_MAX, gas_id);
      }
   }

   if (!rpt->hdr.good_data)
   {
#ifdef TCH_PROFILING
      l1_tch_data[gas_id].crc_bad_hr++;
      l1_tch_data[gas_id].total_crc_bad_hr++;
#endif
   }

   /* Use only 2nd SID block for RXQUAL_SUB */
   if ( l1_tch_data[gas_id].half_rate_subchannel == 0 )
   {
      if ( burst_fn == 4 || burst_fn == 56 )
      {
         gl1_msg_rxqual_sub_update( (uint16)rpt->hdr.rx_qual, gas_id );
#ifdef RXQUAL_MSG
         MSG_GERAN_MED_2_G("Update: rxqual_sub tch/h(dtx)=%d,fn=%d",rpt->hdr.rx_qual,burst_fn);
#endif
      }
      /* Use only 2nd SID block for TAF alignment */
      if ( burst_fn == 56 )
      {
         /*
         Also set TAF for the 2nd SID frame i.e starting on block 56 (aligned with
         SACCH multiframe as defined in 05.08
         */
         taf = TRUE;
      }
   }
   else
   {
      if ( burst_fn == 18 || burst_fn == 70 )
      {
         gl1_msg_rxqual_sub_update( (uint16)rpt->hdr.rx_qual, gas_id );
#ifdef RXQUAL_MSG
         MSG_GERAN_MED_2_G("Update: rxqual_sub tch/h(dtx)=%d,fn=%d",rpt->hdr.rx_qual,burst_fn);
#endif
      }
      /* Use only 2nd SID block for TAF alignment */
      if ( burst_fn == 70 )
      {
         /*
          Also set TAF for the 2nd SID frame i.e starting on block 56 (aligned with
          SACCH multiframe as defined in 05.08
         */
         taf = TRUE;
      }
   }

   /* Don't pass FACCH to vocoder */
   if ( (gl1_msg_chan_type)rpt->hdr.chan != GL1_MSG_FACCH )
   {
      /* Pass BFI,UFI,TAF and SID flag to vocoder */
      l1_vs_send_gsm_packet_to_client (&rpt->data[2],
                                       (uint8)rpt->hdr.bfi,
                                       (uint8)rpt->hdr.ufi, 
                                       (uint8)rpt->hdr.sid,
                                       taf, gas_id);
   }
   else
   {
      MSG_GERAN_MED_2_G("Chan type %d crc=%d not passing to voc",rpt->hdr.chan,rpt->hdr.good_data);
   }

#ifdef FEATURE_GSM_DYNAMIC_SAIC
   if(gl1_hw_feature_disabled_saic(gas_id) != TRUE)
   {
     MSG_GERAN_LOW_3_G(" RXQUAL %d saic d/e %d state %d",rpt->hdr.rx_qual,gl1_msg_get_saic_state(gas_id),l1_dedicated_data_ptr->l1_saic_dis_ena_state);

     /* If We receive a block with less than perfect RXQUAL when SAIC is OFF then enable SAIC */
     if( (rpt->hdr.rx_qual < L1_PERFECT_RXQUAL) &&
         (gl1_msg_get_saic_state(gas_id) == FALSE) &&
         (l1_dedicated_data_ptr->l1_saic_dis_ena_state == FALSE) )
     {
       /* Turn SAIC ON */
       gl1_hw_cfg_saic(TRUE, gas_id);
       gl1_hw_mcpm_amr_aeq_update(TRUE, gas_id);

       /* Set SAIC DISABLE/ENABLE STATE to stop Flip-Flopping of SAIC state */
       l1_dedicated_data_ptr->l1_saic_dis_ena_state = TRUE;
     }

   }
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);
   if(gl1_rxd_ctl_flag.enableRxd) 
     {
     MSG_GERAN_HIGH_2_G("TCH HR Hard RxQUAL:%d,FN:%d",rpt->hdr.rx_qual,gl1_get_FN(gas_id));
     if((rpt->hdr.rx_qual < RXD_HARD_RXQUAL_THRESHOLD) &&
        (l1_dedicated_data_ptr->l1_enable_rxdiversity == FALSE) )
       {
       l1_dedicated_data_ptr->l1_enable_rxdiversity = TRUE;
       gl1_set_enable_rxdiversity(TRUE,gas_id);
       l1_ded_drx_agc_init[gas_id] = TRUE;
       }
     }
#endif


   /* Use all blocks for RXQUAL_FULL */
   gl1_msg_rxqual_full_update( (uint16)rpt->hdr.rx_qual, gas_id );
#ifdef RXQUAL_MSG
   MSG_GERAN_MED_2_G("Update: rxqual_full tch/h=%d,fn=%d",rpt->hdr.rx_qual,burst_fn);
#endif

#ifdef TCH_PROFILING
   if (rpt->hdr.bfi && (gl1_msg_chan_type)rpt->hdr.chan != GL1_MSG_FACCH)
   {
      l1_tch_data[gas_id].total_bfi_cnt_hr++;
      l1_tch_data[gas_id].bfi_cnt_hr++;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
      gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_BFI_RCVD);
#endif
   }

   l1_tch_data[gas_id].total_crc_cnt_hr++;
   if ( ++l1_tch_data[gas_id].crc_cnt_hr >= 100 )
   {
      MSG_6(MSG_SSID_DFLT, MSG_LEGACY_MED,
            "CRC errors: %d/100, %d/%d  (BFI: %d/100, %d/%d)",
            l1_tch_data[gas_id].crc_bad_hr, l1_tch_data[gas_id].total_crc_bad_hr, l1_tch_data[gas_id].total_crc_cnt_hr, l1_tch_data[gas_id].bfi_cnt_hr, l1_tch_data[gas_id].total_bfi_cnt_hr,
            l1_tch_data[gas_id].total_crc_cnt_hr);
      l1_tch_data[gas_id].crc_cnt_hr=0;
      l1_tch_data[gas_id].crc_bad_hr=0;
      l1_tch_data[gas_id].bfi_cnt_hr=0;
   }
#endif

}


#ifdef FEATURE_DATA_GCSD
// =========================================================================
// entries of  CSdata_RxQual_LUT[32]  in decimal
static uint32 CSdata_RxQual_LUT[32] =
{
8192,
10291,
10377,
13991,
10377,
13991,
14150,
21845,
9882,
13107,
13247,
19765,
13247,
19765,
20084,
40167,
8769,
11218,
11320,
15762,
11320,
15762,
15964,
26493,
10734,
14649,
14824,
23494,
14824,
23494,
23946,
59294
};

/*===========================================================================

FUNCTION cstch_rx_callback

DESCRIPTION
  This function is called when the data from the TCH has been decoded. The
  data is then passed to the vocoder or up to L2 if it is a FACCH message.
  Receiver Ready (layer 2) are filtered and not passed on up to reduce
  overhead.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void cstch_rx_callback(gl1_msg_decode_rpt *rpt_i, gas_id_t gas_id )
{

   uint8 FACCH_Status;
   uint32 hrxqual_L1; /* adjusted value of hard RxQual stored in L1 */
   tch_facch_decode_rpt *rpt = (tch_facch_decode_rpt *)rpt_i;

   if ( rpt == NULL )
   {
      // no data received, but may need buffer.

      l1_tch_data[gas_id].csd_rx_block_count++;
      if ( l1_tch_data[gas_id].csd_rx_rpt == NULL )
      {
         // we need a buffer.
         gl1_msg_cs_data_rx( NULL, &l1_tch_data[gas_id].csd_rx_rpt, (int)GCSD_NO_BUFFER, gas_id );
      }
      else
      {
         MSG_GERAN_MED_0_G("cstch_rx_callback: No Data");
         // just notify that no message.
         gl1_msg_cs_data_rx( NULL, &l1_tch_data[gas_id].csd_rx_rpt , (int)GCSD_UNKNOWN, gas_id);
      }

   }
   else
   {
      /* Do RXLEV_VAL processing */
      gl1_update_emr_parameters ( &(rpt->hdr), FALSE, TRUE, gas_id );

      if ( (gl1_msg_chan_type)rpt->hdr.chan== GL1_MSG_FACCH )
      {
         /* Did CRC pass for FACCH ? */
         FACCH_Status = ( rpt->hdr.good_data > 0 ) ? 1 : 0;

         /* Store history of FACCH decode status in FIFO buffer */
         l1_tch_data[gas_id].CSD_FACCH_Buffer = (uint8)(( l1_tch_data[gas_id].CSD_FACCH_Buffer << 1 ) | FACCH_Status );

         /*We only care about the 5 LSB */
         l1_tch_data[gas_id].CSD_FACCH_Buffer &= 0x1F;

         /* adding R-FACCH functionality */
         if ( l1_handle_rfacch(rpt, GL1_MSG_FACCH, gas_id))

         {

           MSG_GERAN_MED_0_G("Receiving FACCH!");
           L1_send_PH_DATA_IND (TRUE,
                                DCCH,
                                FALSE, /* no paging involved */
                                NULL_ARFCN,   /*ARFCN only in idle mode*/
                                rpt->data,
                                N201_MAX, gas_id);

         }
      }
      else if ( (gl1_msg_chan_type)rpt->hdr.chan== GL1_MSG_TCH )
      { /* must be data */
         if ( l1_tch_data[gas_id].csd_rx_block_count < 6 )
         {
            l1_tch_data[gas_id].csd_rx_block_count++;
            gl1_msg_cs_data_rx( rpt, &l1_tch_data[gas_id].csd_rx_rpt, (int)GCSD_BAD_DATA, gas_id );
         }
         else
         {

            hrxqual_L1 = (uint32) (( rpt->hdr.rx_qual * CSdata_RxQual_LUT[l1_tch_data[gas_id].CSD_FACCH_Buffer]));

            if ( hrxqual_L1 > 268435455 )
            {
               hrxqual_L1 = 268435455;
            }

            hrxqual_L1 >>= 13;

            gl1_msg_cs_data_rx( rpt, &l1_tch_data[gas_id].csd_rx_rpt, (int)GCSD_RX_NORMAL, gas_id );
            gl1_msg_rxqual_full_update( (uint16)hrxqual_L1, gas_id );

            rpt->hdr.rx_qual = (uint16)hrxqual_L1;
         }
      }
      /* LOG MESSAGE METRICS */
      l1_log_message_metrics ( (gl1_msg_chan_type)rpt->hdr.chan, (gl1_msg_decode_rpt*)rpt, gas_id );
      l1_log_message_metrics_compact ( (gl1_msg_chan_type)rpt->hdr.chan, (gl1_msg_decode_rpt*)rpt, gas_id );
   }

}
#endif // FEATURE_DATA_GCSD

void tch_fr_metrics_callback( gl1_msg_metrics_rpt *rpt[], uint16 chan, gas_id_t gas_id )
{
  l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

   int i;
   uint16 burst_fn=0;

   if ( !rpt )
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
      return;
   }

   /* Store rx power over the current blocks for RXLEV_VAL computation later */
   gl1_store_block_rx_power ( rpt, 4, gas_id  );

   if ( l1_get_serv_pwr_meas_permitted(gas_id) )
   {
      /* Assuming a FULL RATE channel type here */
      for ( i=0; i<4; i++ )
      {

         /* burst_fn is mod 104 since its used to determine the SID frames */
         burst_fn = SUB_MOD(frame_counters[gas_id].FNmod104, rpt[i]->offset_to_sob, 104);

         /* accumulate frames in which there is a guaranteed burst */
         if ( burst_fn > 51 && burst_fn < 60 )
         {
            /* If carrier is on the hopping sequence, exclude it from RXLEV */
            if ( gl1_include_in_rxlev ( rpt[i]->arfcn, gas_id ) )
            {
               /* Measurement reporting on TCH: RXLEV_SUB */
               l1_serving_cell_meas_ptr->rx_power_sub += rpt[i]->pwr_dBm_x16;
               l1_serving_cell_meas_ptr->no_of_measurements_sub++;
#ifdef FEATURE_GSM_RX_DIVERSITY
               if(gl1_get_rxd_ctl_nv(gas_id) == TRUE)
                 {
                 /*If RxD is off at burst level, then use PRx RSSI for maintaining DRx RSSI*/
                 if(rpt[i]->pwr_dBm_x16_divrx == 0)
                  {
                  l1_serving_cell_meas_ptr->divrx_power_sub += rpt[i]->pwr_dBm_x16;
                  }
                 else
                  {
                  l1_serving_cell_meas_ptr->divrx_power_sub += rpt[i]->pwr_dBm_x16_divrx;
                  }
                 }
               else
                 {
                 l1_serving_cell_meas_ptr->divrx_power_sub = 0;
                 }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
            }
            else
            {
#ifdef FEATURE_GSM_RX_DIVERSITY
               MSG_GERAN_LOW_3_G("Excluding ARFCN %d RSSI %d from PRx_RXLEV %d from DRx_RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4,rpt[i]->pwr_dBm_x16_divrx>>4);
#else
               MSG_GERAN_LOW_2_G("Excluding ARFCN %d RSSI %d from RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4);
#endif
            }
         }

         /* If carrier is on the hopping sequence, exclude it from RXLEV */
         if ( gl1_include_in_rxlev ( rpt[i]->arfcn, gas_id ) )
         {
            /* All bursts get accumulated here for RXLEV */
            l1_serving_cell_meas_ptr->rx_power     += rpt[i]->pwr_dBm_x16;
            l1_serving_cell_meas_ptr->no_of_measurements++;
#ifdef FEATURE_GSM_RX_DIVERSITY
            if(gl1_get_rxd_ctl_nv(gas_id) == TRUE)
              {
              /*If RxD is off at burst level, then use PRx RSSI for maintaining DRx RSSI*/
              if(rpt[i]->pwr_dBm_x16_divrx == 0)
                {
                l1_serving_cell_meas_ptr->divrx_power += rpt[i]->pwr_dBm_x16;
                }
              else
                {
                l1_serving_cell_meas_ptr->divrx_power += rpt[i]->pwr_dBm_x16_divrx;
                }
              }
            else
              {
              l1_serving_cell_meas_ptr->divrx_power  = 0;
              }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
         }
         else
         {
#ifdef FEATURE_GSM_RX_DIVERSITY
            MSG_GERAN_LOW_3_G("Excluding ARFCN %d RSSI %d from PRx_RXLEV %d from DRx_RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4,rpt[i]->pwr_dBm_x16_divrx>>4);
#else
            MSG_GERAN_LOW_2_G("Excluding ARFCN %d RSSI %d from RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4);
#endif
         }
      }
   }

   /* Update AFC/TT */
   for ( i = 0; i < 4; i++ )
   {

      /* If SAIC is used in a burst the mDSP sets bit 14 of */
      /* the misc_flag true. Additionally, for AFC tracking */
      /* the flag selects the specific GMSK SAIC threshold. */
      if ( gl1_msg_saic_in_burst( rpt[i]->misc_flags ) )
      {
         gl1_msg_dch_tt_update ( rpt[i]->timing_offset,
                                 gl1_msg_saic_tt_threshold_snr( rpt[i] ), TRUE, gas_id );

         gl1_msg_dch_afc_update( rpt[i]->freq_offset,
                                 gl1_msg_saic_afc_threshold_snr( rpt[i] ), TRUE, gas_id );
      }
      else
      {
         gl1_msg_dch_tt_update ( rpt[i]->timing_offset, rpt[i]->snr, FALSE, gas_id );

         gl1_msg_dch_afc_update( rpt[i]->freq_offset, rpt[i]->snr, FALSE ,gas_id);
      }
   }

   gl1_msg_dch_afc_adjust(gas_id);

   /* LOG BURST METRICS */
   l1_log_burst_metrics ( (gl1_msg_chan_type)chan, rpt, gas_id  );

#if  defined (DEBUG_FEATURE_GSM_R_FACCH)
   #error code not present
#endif

}

void tch_hr_metrics_callback( gl1_msg_metrics_rpt *rpt[],  uint16 chan, gas_id_t gas_id )
{
  l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

   int i;
   uint16 burst_fn=0;

   if ( !rpt )
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
      return;
   }

   /* Store rx power over the current blocks for RXLEV_VAL computation later */
   gl1_store_block_rx_power ( rpt, 2, gas_id  );

   if ( l1_get_serv_pwr_meas_permitted(gas_id) )
   {
      /* Assuming a HALF RATE channel (2 bursts) type here */
      for ( i=0; i<2; i++ )
      {

         /* burst_fn is mod 104 since its used to determine the SID frames */
         burst_fn = SUB_MOD(frame_counters[gas_id].FNmod104, rpt[i]->offset_to_sob, 104);

         /* accumulate frames in which there is a guaranteed burst */

         if ( l1_tch_data[gas_id].half_rate_subchannel == 0 )
         {
            if ( ( burst_fn < 6 ) ||
                 ( burst_fn >= 52 && burst_fn < 58 ) )
            {
               /* If carrier is on the hopping sequence, exclude it from RXLEV */
               if ( gl1_include_in_rxlev ( rpt[i]->arfcn, gas_id ) )
               {
                  /* Measurement reporting on TCH: RXLEV_SUB */
                  l1_serving_cell_meas_ptr->rx_power_sub += rpt[i]->pwr_dBm_x16;
                  l1_serving_cell_meas_ptr->no_of_measurements_sub++;
#ifdef FEATURE_GSM_RX_DIVERSITY
               if(gl1_get_rxd_ctl_nv(gas_id) == TRUE)
                 {
                 /*If RxD is off at burst level, then use PRx RSSI for maintaining DRx RSSI*/
                 if(rpt[i]->pwr_dBm_x16_divrx == 0)
                  {
                  l1_serving_cell_meas_ptr->divrx_power_sub += rpt[i]->pwr_dBm_x16;
                  }
                 else
                  {
                  l1_serving_cell_meas_ptr->divrx_power_sub += rpt[i]->pwr_dBm_x16_divrx;
                  }
                 }
               else
                 {
                 l1_serving_cell_meas_ptr->divrx_power_sub = 0;
                 }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
               }
               else
               {
#ifdef FEATURE_GSM_RX_DIVERSITY
                  MSG_GERAN_LOW_3_G("Excluding ARFCN %d RSSI %d from PRx RXLEV DRx RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4,rpt[i]->pwr_dBm_x16_divrx>>4);
#else
                  MSG_GERAN_LOW_2_G("Excluding ARFCN %d RSSI %d from RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4);
#endif
               }

            }
         }
         else
         {
            if ( ( burst_fn >= 14 && burst_fn < 20 ) ||
                 ( burst_fn >= 66 && burst_fn < 72 ) )
            {
               /* If carrier is on the hopping sequence, exclude it from RXLEV */
               if ( gl1_include_in_rxlev ( rpt[i]->arfcn, gas_id ) )
               {
                  /* Measurement reporting on TCH: RXLEV_SUB */
                  l1_serving_cell_meas_ptr->rx_power_sub += rpt[i]->pwr_dBm_x16;
                  l1_serving_cell_meas_ptr->no_of_measurements_sub++;
#ifdef FEATURE_GSM_RX_DIVERSITY
                  if(gl1_get_rxd_ctl_nv(gas_id) == TRUE)
                    {
                    /*If RxD is off at burst level, then use PRx RSSI for maintaining DRx RSSI*/
                    if(rpt[i]->pwr_dBm_x16_divrx == 0)
                      {
                      l1_serving_cell_meas_ptr->divrx_power_sub += rpt[i]->pwr_dBm_x16;
                      }
                    else
                      {
                      l1_serving_cell_meas_ptr->divrx_power_sub += rpt[i]->pwr_dBm_x16_divrx;
                      }
                    }
                  else
                    {
                    l1_serving_cell_meas_ptr->divrx_power_sub = 0;
                    }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
               }
               else
               {
#ifdef FEATURE_GSM_RX_DIVERSITY
                  MSG_GERAN_LOW_3_G("Excluding ARFCN %d RSSI %d from PRx RXLEV DRx RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4,rpt[i]->pwr_dBm_x16_divrx>>4);
#else
                  MSG_GERAN_LOW_2_G("Excluding ARFCN %d RSSI %d from RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4);
#endif
               }
            }
         }

         /* If carrier is on the hopping sequence, exclude it from RXLEV */
         if ( gl1_include_in_rxlev ( rpt[i]->arfcn, gas_id ) )
         {
            /* All bursts get accumulated here for RXLEV */
            l1_serving_cell_meas_ptr->rx_power     += rpt[i]->pwr_dBm_x16;
            l1_serving_cell_meas_ptr->no_of_measurements++;
#ifdef FEATURE_GSM_RX_DIVERSITY
            if(gl1_get_rxd_ctl_nv(gas_id) == TRUE)
              {
              /*If RxD is off at burst level, then use PRx RSSI for maintaining DRx RSSI*/
              if(rpt[i]->pwr_dBm_x16_divrx == 0)
                {
                l1_serving_cell_meas_ptr->divrx_power += rpt[i]->pwr_dBm_x16;
                }
              else
                {
                l1_serving_cell_meas_ptr->divrx_power += rpt[i]->pwr_dBm_x16_divrx;
                }
              }
            else
              {
              l1_serving_cell_meas_ptr->divrx_power = 0;
              }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
         }
         else
         {
#ifdef FEATURE_GSM_RX_DIVERSITY
            MSG_GERAN_LOW_3_G("Excluding ARFCN %d RSSI %d from PRx_RXLEV %d from DRx_RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4,rpt[i]->pwr_dBm_x16_divrx>>4);
#else
            MSG_GERAN_LOW_2_G("Excluding ARFCN %d RSSI %d from RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4);
#endif
         }
      }
   }


   /* Update AFC/TT */
   for ( i = 0; i < 2; i++ )
   {
      /* If SAIC is used in a burst the mDSP sets bit 14 of */
      /* the misc_flag true. Additionally, for AFC tracking */
      /* the flag selects the specific GMSK SAIC threshold. */
      if ( gl1_msg_saic_in_burst( rpt[i]->misc_flags ) )
      {
         gl1_msg_dch_tt_update ( rpt[i]->timing_offset,
                                 gl1_msg_saic_tt_threshold_snr( rpt[i] ), TRUE, gas_id );

         gl1_msg_dch_afc_update( rpt[i]->freq_offset,
                                 gl1_msg_saic_afc_threshold_snr( rpt[i] ), TRUE, gas_id );
      }
      else
      {
         gl1_msg_dch_tt_update ( rpt[i]->timing_offset, rpt[i]->snr, FALSE, gas_id );

         gl1_msg_dch_afc_update( rpt[i]->freq_offset, rpt[i]->snr, FALSE, gas_id );
      }
   }

   gl1_msg_dch_afc_adjust(gas_id);

   /* LOG BURST METRICS */
   l1_log_burst_metrics ( (gl1_msg_chan_type)chan, rpt, gas_id  );

#if  defined (DEBUG_FEATURE_GSM_R_FACCH)
   #error code not present
#endif

}

#ifdef DED_LOG
#error code not present
#endif


/*===========================================================================

FUNCTION sacch_tx_callback

DESCRIPTION
  This function is called in the first frame of a SACCH block (ie 104 frames).
  Currently no activity is needed here.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void sacch_tx_callback( gas_id_t gas_id )
{
   /* nothing to do */
}



/*===========================================================================

FUNCTION sacch_rx_callback

DESCRIPTION
  This function is called when the data from the SACCH has been decoded. The
  data is then passed to L2.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void sacch_rx_callback(gl1_msg_decode_rpt *rpt_i, gas_id_t gas_id )
{
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
//#ifdef TCH_PROFILING
//   static uint16    crc_cnt=0;
//   static uint16    crc_bad=0;
//#endif
   byte new_ta, new_txlev;
   cch_decode_rpt *rpt =  (cch_decode_rpt *)rpt_i;
#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_rxd_control_type gl1_rxd_ctl_flag;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

   if ( !rpt )
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
      return;
   }

   /* Do RXLEV_VAL processing */
   gl1_update_emr_parameters ( &(rpt->hdr), FALSE, TRUE, gas_id );

   /* LOG MESSAGE METRICS */
   l1_log_message_metrics ( (gl1_msg_chan_type)rpt->hdr.chan, (gl1_msg_decode_rpt*)rpt, gas_id );
   l1_log_message_metrics_compact ( (gl1_msg_chan_type)rpt->hdr.chan, (gl1_msg_decode_rpt*)rpt, gas_id );

   if(l1_dedicated_data_ptr->rsacch_ctrl.nv_active == TRUE)
   {
      l1_rsacch_dl_process_block (rpt, gas_id);
   }

   if ( rpt->hdr.good_data )
   {
      new_txlev = rpt->data[0] & 31;

      /* See GSM 04.04 section 6.1 */
      /* 127 indicates that the msg does not contain a valid TA */
      if ( (rpt->data[1] & 127) != 127 )
      {
         new_ta = rpt->data[1] & 63;
      }
      else
      {
         /* Just set new_ta = old_ta */
         new_ta = (uint8)gl1_msg_retrieve_ta(gas_id);
      }
      geran_eng_mode_data_write(ENG_MODE_TIMING_ADVANCE,&new_ta,TRUE, gas_id);
      l1_log_tx_lev_and_ta( new_txlev, new_ta, gas_id );

      /*
         This just stores the ordered TA and TXLEV values.
         These are only to be applied at the start of the next
         reporting period
      */
      L1_store_ta_txlev(l1_dedicated_data_ptr, new_ta, new_txlev, gas_id);

      L1_send_PH_DATA_IND(TRUE,
                          SACCH,
                          FALSE,
                          NULL_ARFCN, /* ARFCN only used in idle mode */
                          &(rpt->data[2]),
                          SACCH_MESSAGE_UNIT, gas_id);

      L1_send_MPH_BLOCK_QUALITY_IND(SACCH, GL1_BLOCK_QUALITY_GOOD, gas_id);

      l1_ded_store_bad_sacch_block_cnt(0, gas_id);
   }
   else
   {
#ifdef TCH_PROFILING
     l1_tch_data[gas_id].crc_bad_rx_cb++;
#endif
      L1_send_PH_DATA_IND(FALSE,
                          SACCH,
                          FALSE,
                          NULL_ARFCN,
                          NULL,
                          0, gas_id);

      L1_send_MPH_BLOCK_QUALITY_IND(SACCH, GL1_BLOCK_QUALITY_BAD, gas_id);

      l1_ded_store_bad_sacch_block_cnt(l1_ded_get_bad_sacch_block_cnt(gas_id) + 1, gas_id);
      if ( l1_ded_get_bad_sacch_block_cnt(gas_id) > L1_MAX_FAILED_SACCH_DECODES )
      {
        if ( ftm_get_mode() == FTM_MODE )
        {
         MSG_GERAN_HIGH_0_G("Stuck on bad cell in FTM mode");
        }
        else
        {
         if(geran_get_nv_recovery_restart_enabled(gas_id))
         {
         MSG_GERAN_ERROR_0_G("Stuck on bad cell");
         l1_call_panic_reset_in_task(L1_STUCK_ON_BAD_TCH, gas_id);
         }
         else
         {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_GERAN_FATAL_0_G("Stuck on bad cell");
         }
        }
      }

   }

#ifdef FEATURE_GSM_DYNAMIC_SAIC
   if(gl1_hw_feature_disabled_saic(gas_id) != TRUE)
   {
     MSG_GERAN_LOW_3_G(" RXQUAL %d saic d/e %d state %d",rpt->hdr.rx_qual,gl1_msg_get_saic_state(gas_id),l1_dedicated_data_ptr->l1_saic_dis_ena_state);

     /* If We receive a block with less than perfect RXQUAL when SAIC is OFF then enable SAIC */
     if( (rpt->hdr.rx_qual < L1_PERFECT_RXQUAL) &&
         (gl1_msg_get_saic_state(gas_id) == FALSE) &&
         (l1_dedicated_data_ptr->l1_saic_dis_ena_state == FALSE) )
     {
       /* Set SAIC DISABLE/ENABLE STATE to stop Flip-Flopping of SAIC state */
       l1_dedicated_data_ptr->l1_saic_dis_ena_state = TRUE;

       /* Turn SAIC ON */
       gl1_hw_cfg_saic(TRUE, gas_id);
       gl1_hw_mcpm_amr_aeq_update(TRUE, gas_id);
     }
   }
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);
   if(gl1_rxd_ctl_flag.enableRxd) 
     {
     MSG_GERAN_HIGH_3_G("TCH SACCH Hard RxQUAL:%d,FN:%d rxd_l1_en_flag: %d",rpt->hdr.rx_qual,gl1_get_FN(gas_id), l1_dedicated_data_ptr->l1_enable_rxdiversity);
     if((rpt->hdr.rx_qual < RXD_HARD_RXQUAL_THRESHOLD) &&
        (l1_dedicated_data_ptr->l1_enable_rxdiversity == FALSE) )
       {
       l1_dedicated_data_ptr->l1_enable_rxdiversity = TRUE;
       gl1_set_enable_rxdiversity(TRUE,gas_id);
       l1_ded_drx_agc_init[gas_id] = TRUE;
       }
     }
#endif

   gl1_msg_rxqual_full_update( (uint16)rpt->hdr.rx_qual, gas_id );
   gl1_msg_rxqual_sub_update( (uint16)rpt->hdr.rx_qual, gas_id );

#ifdef RXQUAL_MSG
   MSG_GERAN_MED_1_G("Update: rxqual_full_sub sacch = %d",rpt->hdr.rx_qual);
#endif

#ifdef TCH_PROFILING
   /* Display CRC results of packets received */
   if ( ++l1_tch_data[gas_id].crc_cnt_rx_cb == 30 )
   {
      MSG_GERAN_MED_1_G("SACCH CRC's error rate %d/30",l1_tch_data[gas_id].crc_bad_rx_cb );

      l1_tch_data[gas_id].crc_cnt_rx_cb = 0;
      l1_tch_data[gas_id].crc_bad_rx_cb = 0;
   }
#endif
}

/*===========================================================================

FUNCTION SACCH26_METRICS_CALLBACK

DESCRIPTION
  This function is called for each SACCH burst in the 26-frame TCH
  multiframe.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void sacch26_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id )
{
  l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

   uint8 i;

   if ( !rpt )
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
      return;
   }

   /* Store rx power over the current blocks for RXLEV_VAL computation later */
   gl1_store_block_rx_power ( rpt, 4, gas_id  );

#ifdef TT_DEBUG
   #error code not present
#endif

   for ( i=0; i<4; i++ )
   {
      /* If SAIC is used in a burst the mDSP sets bit 14 of */
      /* the misc_flag true. Additionally,  AFC/TT tracking */
      /* the flag selects the specific GMSK SAIC threshold. */
      if ( gl1_msg_saic_in_burst( rpt[i]->misc_flags ) )
      {
         gl1_msg_dch_tt_update ( rpt[i]->timing_offset,
                                 gl1_msg_saic_tt_threshold_snr( rpt[i] ), TRUE, gas_id );

         gl1_msg_dch_afc_update( rpt[i]->freq_offset,
                                 gl1_msg_saic_afc_threshold_snr( rpt[i] ), TRUE,gas_id );
      }
      else
      {
         gl1_msg_dch_tt_update ( rpt[i]->timing_offset,rpt[i]->snr, FALSE, gas_id );

         gl1_msg_dch_afc_update( rpt[i]->freq_offset, rpt[i]->snr, FALSE,gas_id );
      }
   }

   gl1_msg_dch_afc_adjust(gas_id);

   if ( l1_get_serv_pwr_meas_permitted(gas_id))
   {
      for ( i=0; i<4; i++ )
      {
         /* If carrier is on the hopping sequence, exclude it from RXLEV */
         if ( gl1_include_in_rxlev ( rpt[i]->arfcn, gas_id ) )
         {
            /* Measurement reporting on TCH: RXLEV_SUB */
            l1_serving_cell_meas_ptr->rx_power_sub += rpt[i]->pwr_dBm_x16;
            l1_serving_cell_meas_ptr->no_of_measurements_sub++;

            /* All bursts get accumulated here for RXLEV */
            l1_serving_cell_meas_ptr->rx_power     += rpt[i]->pwr_dBm_x16;
            l1_serving_cell_meas_ptr->no_of_measurements++;
#ifdef FEATURE_GSM_RX_DIVERSITY
            if(gl1_get_rxd_ctl_nv(gas_id) == TRUE)
              {
              /*If RxD is off at burst level, then use PRx RSSI for maintaining DRx RSSI*/
              if(rpt[i]->pwr_dBm_x16_divrx == 0)
                {
                l1_serving_cell_meas_ptr->divrx_power += rpt[i]->pwr_dBm_x16;
                l1_serving_cell_meas_ptr->divrx_power_sub += rpt[i]->pwr_dBm_x16;
                }
              else
                {
                l1_serving_cell_meas_ptr->divrx_power += rpt[i]->pwr_dBm_x16_divrx;
                l1_serving_cell_meas_ptr->divrx_power_sub += rpt[i]->pwr_dBm_x16_divrx;
                }
              }
            else
              {
              l1_serving_cell_meas_ptr->divrx_power = 0;
              l1_serving_cell_meas_ptr->divrx_power_sub = 0;
              }
#endif

         }
         else
         {
#ifdef FEATURE_GSM_RX_DIVERSITY
            MSG_GERAN_LOW_3_G("Excluding ARFCN %d RSSI %d from PRx_RXLEV %d from DRx_RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4,rpt[i]->pwr_dBm_x16_divrx>>4);
#else
            MSG_GERAN_LOW_2_G("Excluding ARFCN %d RSSI %d from RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4);
#endif
         }
      }
   }

   /* LOG BURST METRICS */
   l1_log_burst_metrics ( GL1_MSG_SACCH, rpt, gas_id  );
   l1_log_sacch_burst_metrics(rpt, gas_id);
}

/*===========================================================================

FUNCTION SACCH51_METRICS_CALLBACK

DESCRIPTION
  This function is called for each SACCH burst in the 51
  multiframe.  AFC/TT not done on SDCCH right now

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void sacch51_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id )
{
  l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

   uint8 i;

   if ( !rpt )
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
      return;
   }

   /* Store rx power over the current blocks for RXLEV_VAL computation later */
   gl1_store_block_rx_power ( rpt, 4, gas_id  );

#ifdef TT_DEBUG
   #error code not present
#endif

   for ( i=0; i<4; i++ )
   {
      /* If carrier is on the hopping sequence, exclude it from RXLEV */
      if ( gl1_include_in_rxlev ( rpt[i]->arfcn, gas_id ) )
      {
         /* Measurement reporting on TCH: RXLEV_SUB */
         l1_serving_cell_meas_ptr->rx_power_sub += rpt[i]->pwr_dBm_x16;
         l1_serving_cell_meas_ptr->no_of_measurements_sub++;

         /* All bursts get accumulated here for RXLEV */
         l1_serving_cell_meas_ptr->rx_power     += rpt[i]->pwr_dBm_x16;
         l1_serving_cell_meas_ptr->no_of_measurements++;

#ifdef FEATURE_GSM_RX_DIVERSITY
         if(gl1_get_rxd_ctl_nv(gas_id) == TRUE)
           {
            /*If RxD is off at burst level, then use PRx RSSI for maintaining DRx RSSI*/
            if(rpt[i]->pwr_dBm_x16_divrx == 0)
              {
              l1_serving_cell_meas_ptr->divrx_power += rpt[i]->pwr_dBm_x16;
              l1_serving_cell_meas_ptr->divrx_power_sub += rpt[i]->pwr_dBm_x16;
              }
            else
              {
              l1_serving_cell_meas_ptr->divrx_power  += rpt[i]->pwr_dBm_x16_divrx;
              l1_serving_cell_meas_ptr->divrx_power_sub += rpt[i]->pwr_dBm_x16_divrx;
              }
            }
          else
            {
            l1_serving_cell_meas_ptr->divrx_power = 0;
            l1_serving_cell_meas_ptr->divrx_power_sub = 0;
            }
#endif
      }
      else
      {
#ifdef FEATURE_GSM_RX_DIVERSITY
         MSG_GERAN_LOW_3_G("Excluding ARFCN %d RSSI %d from PRx_RXLEV and %d from DRx_RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4,rpt[i]->pwr_dBm_x16_divrx>>4);
#else
         MSG_GERAN_LOW_2_G("Excluding ARFCN %d RSSI %d from RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4);
#endif
      }

   }

   /* TT and AFC Update was missing for SDCCH SACCH */
   for ( i=0; i<4; i++ )
   {
     /* If SAIC is used in a burst the mDSP sets bit 14 of */
     /* the misc_flag true. Additionally, for AFC/TT track */
     /* the flag selects the specific GMSK SAIC threshold. */
     if ( gl1_msg_saic_in_burst( rpt[i]->misc_flags ) )
     {
       gl1_msg_cch_tt_update ( rpt[i]->timing_offset,
                               gl1_msg_saic_tt_threshold_snr( rpt[i] ), TRUE, gas_id );

       gl1_msg_cch_afc_update( rpt[i]->freq_offset,
                               gl1_msg_saic_afc_threshold_snr( rpt[i] ), TRUE, gas_id );
     }
     else
     {
       gl1_msg_cch_tt_update ( rpt[i]->timing_offset, rpt[i]->snr, FALSE,gas_id );

       gl1_msg_cch_afc_update( rpt[i]->freq_offset, rpt[i]->snr, FALSE, gas_id );
     }
   }

   /* LOG BURST METRICS */
   l1_log_burst_metrics ( GL1_MSG_SACCH, rpt, gas_id  );
   l1_log_sacch_burst_metrics(rpt, gas_id);
}

#ifdef DED_LOG
#error code not present
#endif

/*===========================================================================

FUNCTION  gl1_tch_loopback

DESCRIPTION
  This function is used to open and close a TCH loopback.

DEPENDENCIES
  None

RETURN VALUE
  current FN

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_tch_loopback( gl1_defs_loopback_type loopback, gas_id_t gas_id )
{
   boolean result = TRUE;

   if (loopback == GL1_DEFS_LOOPBACK_TYPE_I)
   {
      if (gl1_msg_get_current_voc_type(gas_id) == GL1_MSG_AMR)
      {
         gl1_amr_setup_loopback_I(gas_id);
      }
      else
      {
         result = FALSE;
         loopback = GL1_DEFS_NO_LOOPBACK;
      }
   }
   else
   {
      gl1_amr_reset_loopback_I(gas_id);
   }

   gl1_msg_tch_loopback(loopback, gas_id);

   return result;
}

/*===========================================================================

FUNCTION  gl1_store_block_rx_power

DESCRIPTION
  This function accumulates the rx power in dBm for a single block

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  Block rx power totals are updated
===========================================================================*/
void gl1_store_block_rx_power ( gl1_msg_metrics_rpt *rpt[], uint8 bursts_per_block, gas_id_t gas_id )
{
  l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

  int i;

  /* Reset block rx power.  Accumulate only per block */
  l1_serving_cell_meas_ptr->rx_power_block = 0;
  l1_serving_cell_meas_ptr->divrx_power_block = 0;
  l1_serving_cell_meas_ptr->no_of_measurements_block = 0;

  for (i=0; i<bursts_per_block; i++)
  {
     /* If carrier is on the hopping sequence, exclude it from RXLEV */
     if ( gl1_include_in_rxlev ( rpt[i]->arfcn, gas_id ) )
     {
        /* Accumulate rx power for this block */
        l1_serving_cell_meas_ptr->rx_power_block     += rpt[i]->pwr_dBm_x16;
        l1_serving_cell_meas_ptr->no_of_measurements_block++;

#ifdef FEATURE_GSM_RX_DIVERSITY
        if(gl1_get_rxd_ctl_nv(gas_id) == TRUE)
          {
          /*If RxD is off at burst level, then use PRx RSSI for maintaining DRx RSSI*/
          if(rpt[i]->pwr_dBm_x16_divrx == 0)
            {
            l1_serving_cell_meas_ptr->divrx_power_block  += rpt[i]->pwr_dBm_x16;
            l1_serving_cell_meas_ptr->is_divrx_disabled_in_sacch_period = TRUE;
            }
          else
            {
            l1_serving_cell_meas_ptr->divrx_power_block  += rpt[i]->pwr_dBm_x16_divrx;
            }
          }
         else
          {
          l1_serving_cell_meas_ptr->divrx_power_block = 0;
          }
#endif
     }
     else
     {
#ifdef FEATURE_GSM_RX_DIVERSITY
        MSG_GERAN_LOW_3_G("Excluding ARFCN %d RSSI %d from PRx_RXLEV %d from DRx_RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4,rpt[i]->pwr_dBm_x16_divrx>>4);
#else
        MSG_GERAN_LOW_2_G("Excluding ARFCN %d RSSI %d from RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4);
#endif
     }
  }
}

/*===========================================================================

FUNCTION  gl1_update_emr_parameters

DESCRIPTION
  This function adds the rx power of the block just received to the RXLEV_VAL
  cumulative total, if it is correctly decoded and is not a SACCH or
  SID.  It then invokes NPL1 functions to compute the mean and CV BEPs for
  the block we just received and adds it to the totals.  The averages are
  computed at the end of the reporting period.

DEPENDENCIES
  None

RETURN VALUE
  current FN

SIDE EFFECTS
  None
===========================================================================*/
void gl1_update_emr_parameters (gl1_defs_rx_hdr_struct *rpt_hdr, boolean is_sid_fn, boolean compute_bep, gas_id_t gas_id  )
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  l1_serving_cell_meas_T  *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

   boolean correctly_decoded;
   uint32 mean_bep_block, cv_bep_block;
   channel_type_T physical_chan_type = FULL_RATE_TRAFFIC;
   boolean include_sacch_meas = FALSE;

   /* Use BFI for speech channels and  CRC for signaling channels to
    * indicate a correctly decoded block
    */
   switch( (gl1_msg_chan_type)rpt_hdr->chan )
   {
     case GL1_MSG_TCH:
     case GL1_MSG_TCH_HS:
     case GL1_MSG_TCH_AFS:
     case GL1_MSG_TCH_AHS:
#ifdef FEATURE_GSM_AMR_WB
     case GL1_MSG_TCH_WFS:
#endif
        correctly_decoded = ( rpt_hdr->bfi == 0 ) ? TRUE : FALSE;
        break;

     case GL1_MSG_SACCH:
     case GL1_MSG_SID_UPDATE_FS:
     case GL1_MSG_SID_UPDATE_HS:
     case GL1_MSG_RATSCCH_FS:
     case GL1_MSG_RATSCCH_HS:
     case GL1_MSG_SDCCH:
     case GL1_MSG_FACCH:
     case GL1_MSG_FACCH_HS:
     case GL1_MSG_FACCH_AFS:
     case GL1_MSG_FACCH_AHS:
#ifdef FEATURE_GSM_AMR_WB
     case GL1_MSG_FACCH_WFS:
     case GL1_MSG_RATSCCH_WFS:
#endif

     /*NOTE:mDSP by default passes CRC good to L1 for CSD.L1 does not
       generate a BFI value for CSD.A Enhancement would be to CRC set
       according to the rxqual threshold in cstch_rx_callback().
       This would ensure the correctness of EMR reporting.
      */
#ifdef FEATURE_DATA_GCSD
     case GL1_MSG_TCH_F144:
     case GL1_MSG_TCH_F96:
#endif
        correctly_decoded = rpt_hdr->good_data;
        break;

     /* These Channel types don't have parity bits.*/
     case GL1_MSG_ONSET:
     case GL1_MSG_SID_FIRST_FS:
     case GL1_MSG_SID_FIRST_HS:
     case GL1_MSG_SID_FIRST_INH:
     case GL1_MSG_SID_UPDATE_INH:
     case GL1_MSG_RATSCCH_MARKER:
        correctly_decoded = FALSE;
        break;


     default:
        correctly_decoded = FALSE;
        MSG_GERAN_ERROR_1_G("Incorrect chan type (%d) used for EMR report",
                  (gl1_msg_chan_type)rpt_hdr->chan);
        break;

   }
   /* Need to know if we are on SDCCH so that we can include SACCH
    * measurements or not - GP042232.
    */

   if ( l1_tskisr_blk->sub_state == L1_DED_BEFORE )
   {
      physical_chan_type = l1_dedicated_data_ptr->channel_specification.channel_info_1_before.channel_type;
   }
   else if( l1_tskisr_blk->sub_state == L1_DED_AFTER )
   {
      physical_chan_type = l1_dedicated_data_ptr->channel_specification.channel_info_1_before.channel_type;
   }

   if ( ((physical_chan_type == SDCCH_4) || (physical_chan_type == SDCCH_8)) &&
        ((gl1_msg_chan_type)rpt_hdr->chan == GL1_MSG_SACCH) )
   {
      include_sacch_meas = TRUE;
   }

   /* If correctly decoded, accumulate rx power from current block */
   if ( correctly_decoded )
   {

#ifdef DEBUG_GSM_GPRS_ENHANCED_MEASUREMENT_REPORTING
       #error code not present
#endif
      l1_serving_cell_meas_ptr->rx_power_val += l1_serving_cell_meas_ptr->rx_power_block;
      l1_serving_cell_meas_ptr->no_of_measurements_val += l1_serving_cell_meas_ptr->no_of_measurements_block;

#ifdef FEATURE_GSM_RX_DIVERSITY
      if(gl1_get_rxd_ctl_nv(gas_id) == TRUE)
        {
        /*If RxD is off at burst level, then use PRx RSSI for maintaining DRx RSSI*/
        if(l1_serving_cell_meas_ptr->divrx_power_block == 0)
          {
          l1_serving_cell_meas_ptr->divrx_power_val  += l1_serving_cell_meas_ptr->rx_power_block;
          }
        else
          {
          l1_serving_cell_meas_ptr->divrx_power_val  += l1_serving_cell_meas_ptr->divrx_power_block;
          }
        }
      else
        {
        l1_serving_cell_meas_ptr->divrx_power_val = 0;
        }
#endif

      /* Exclude SACCH, FACCH (unless its signaling only) blocks */
      if ( (gl1_msg_chan_type)rpt_hdr->chan == GL1_MSG_FACCH ||
           (gl1_msg_chan_type)rpt_hdr->chan == GL1_MSG_FACCH_AFS  ||
           (gl1_msg_chan_type)rpt_hdr->chan == GL1_MSG_FACCH_AHS
#ifdef FEATURE_GSM_AMR_WB
         ||(gl1_msg_chan_type)rpt_hdr->chan == GL1_MSG_FACCH_WFS
#endif
         )
      {
        {
           l1_serving_cell_meas_ptr->num_received_blocks++;
        }
      }
      /* Exclude SID, SACCH and RATSCCH frames */
      else if ( !((gl1_msg_chan_type)rpt_hdr->chan == GL1_MSG_SID_UPDATE_FS ||
                 (gl1_msg_chan_type)rpt_hdr->chan == GL1_MSG_SID_UPDATE_HS ||
                 (gl1_msg_chan_type)rpt_hdr->chan == GL1_MSG_RATSCCH_FS
#ifdef FEATURE_GSM_AMR_WB
                 || (gl1_msg_chan_type)rpt_hdr->chan == GL1_MSG_RATSCCH_WFS
#endif
                 ||(gl1_msg_chan_type)rpt_hdr->chan == GL1_MSG_RATSCCH_HS ) && !is_sid_fn )

      {
         if ( include_sacch_meas || !((gl1_msg_chan_type)rpt_hdr->chan == GL1_MSG_SACCH) )
         {
         l1_serving_cell_meas_ptr->num_received_blocks++;
         }
      }

      if ( compute_bep )
      {
         gl1_msg_process_bep ( rpt_hdr->bep, &mean_bep_block, &cv_bep_block );
         gl1_msg_mean_bep_update ( mean_bep_block, gas_id );

         /* SACCH/T blocks not included in CV_BEP measurements - SACCH/S are included.GP042232 */
         if ( include_sacch_meas || ((gl1_msg_chan_type)rpt_hdr->chan != GL1_MSG_SACCH ) )
         {
            gl1_msg_cv_bep_update ( cv_bep_block, gas_id );
         }
      }
   }

}

/*===========================================================================

FUNCTION  gl1_is_sid_fn

DESCRIPTION
  This function determines if the specified frame number is a SID frame for the
  current channel type

DEPENDENCIES
  None

RETURN VALUE
  current FN

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_is_sid_fn ( uint16 fn, gas_id_t gas_id )
{
  boolean is_sid_fn;
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

  /* SID applies only to speech channel modes */
  if ( l1_dedicated_data_ptr->channel_specification.channel_info_1_after.channel_type== FULL_RATE_TRAFFIC &&
       ( (l1_dedicated_data_ptr->channel_specification.channel_mode_1== SPEECH_V1) ||
         (l1_dedicated_data_ptr->channel_specification.channel_mode_1== SPEECH_V2) 
#ifdef FEATURE_VAMOS_II
          ||(l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V1_VAMOS2)
          ||(l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V2_VAMOS2)
#endif
     ))
  {
     if ( fn == 56 )
     {
        is_sid_fn = TRUE;
     }
     else
     {
        is_sid_fn = FALSE;
     }
  }
  else if ( l1_dedicated_data_ptr->channel_specification.channel_info_1_after.channel_type== HALF_RATE_TRAFFIC  &&
       ( (l1_dedicated_data_ptr->channel_specification.channel_mode_1== SPEECH_V1) ||
         (l1_dedicated_data_ptr->channel_specification.channel_mode_1== SPEECH_V2) 
#ifdef FEATURE_VAMOS_II
          ||(l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V1_VAMOS2)
          ||(l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V2_VAMOS2)
#endif
         ) )
  {

     if ( l1_tch_data[gas_id].half_rate_subchannel == 0 )
     {
        if ( fn == 4 || fn == 56 )
        {
           is_sid_fn = TRUE;
        }
        else
        {
           is_sid_fn = FALSE;
        }
     }
     else
     {
        if ( fn == 18 || fn == 70 )
        {
           is_sid_fn = TRUE;
        }
        else
        {
           is_sid_fn = FALSE;
        }
     }
  }
  else
  {
     is_sid_fn = FALSE;
  }

  return ( is_sid_fn );
}

/*===========================================================================

FUNCTION  l1_rfacch_handle_nv_flag

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_rfacch_handle_nv_flag (uint16 nv_flag, gas_id_t gas_id)
{
    l1_tch_data[gas_id].r_facch_nv_flag_active = FALSE;

    if( ((nv_flag &  L1_GSM_ACCH_NV_FLAG_ENABLED)
                  == L1_GSM_ACCH_NV_FLAG_ENABLED) ||
        ((nv_flag &  L1_GSM_ACCH_NV_FLAG_R_FACCH_ONLY)
                  == L1_GSM_ACCH_NV_FLAG_R_FACCH_ONLY) )
    {
        l1_tch_data[gas_id].r_facch_nv_flag_active = TRUE;
    }

}

/*===========================================================================

FUNCTION  l1_handle_rfacch

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  boolean, whether to send the FACCH message to L2

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_handle_rfacch(tch_facch_decode_rpt *rpt, gl1_msg_chan_type chan, gas_id_t gas_id)
{
  int32   burst_fn;
  uint16  possible_next_rfacch_fn;
  int32   facch_delta;
  boolean send_L2_ph_ind_msg = TRUE;
  boolean previousFacchData_valid = FALSE;

#ifdef DEBUG_FEATURE_GSM_R_FACCH
  #error code not present
#endif

  const  uint8 fillerfacchdata[3]   = {0x03,0x03,0x01};

  /*Before doing anything, ensure that CRC is good - if not then bug out*/
  if (!rpt->hdr.good_data)
  {
    send_L2_ph_ind_msg = FALSE;
  }
  /*check to see if we have rx'd a filler frame*/
  else if ( (fillerfacchdata[0] == rpt->data[0]) &&
    (fillerfacchdata[1] == rpt->data[1]) &&
    (fillerfacchdata[2] == rpt->data[2]) )
  {
    /* This is a filler frame. Pass this up to L2 but stop processing */
    /* here as we don't want to disable RFACCH mechanism as next blk */
    /* could be 2nd blk of a RFACCH pair*/
    MSG_GERAN_MED_0_G("FACCH filler frame rx'd");
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_FACCH_FILLER_RXED);
#endif

    return (send_L2_ph_ind_msg = TRUE);

  }

  if(l1_tch_data[gas_id].r_facch_nv_flag_active == TRUE)
  {

    burst_fn = SUB_MOD(frame_counters[gas_id].FNmod42432 , rpt->offset_to_sob, 42432 );


    /* Now we check (retrospectively) to see whether R-FACCH had been
    enabled by the BTS */

    facch_delta = SUB_MOD(burst_fn,l1_tch_data[gas_id].previousFacchData_FN,42432);

    if ( SUB_MOD(burst_fn,l1_tch_data[gas_id].previousFacchData_FN,42432) <= TCH_NUM_FRAMES_BETWEEN_BLOCKS_R_FACCH_MAX )
    {
      previousFacchData_valid = TRUE;
    }
    else
    {
      previousFacchData_valid = FALSE;
    }



    if ( facch_delta < TCH_NUM_FRAMES_BETWEEN_BLOCKS_R_FACCH_MIN )
    {
      /* we are getting a stream of back to back FACCHs.
      * we assume that RFACCH won't occur here but need to
      * check as some TCs do this i.e. 26.7.5
      */
      l1_tch_data[gas_id].r_facch_support_detected = FALSE;

      if(rpt->hdr.good_data && ((l1_tch_data[gas_id].previousFacchData[0]==rpt->data[0]) &&
        (l1_tch_data[gas_id].previousFacchData[1]==rpt->data[1]) &&
        (l1_tch_data[gas_id].previousFacchData[2]==rpt->data[2])) &&
        previousFacchData_valid )
      {
        send_L2_ph_ind_msg = FALSE;
        l1_tch_data[gas_id].r_facch_support_detected = TRUE;
#ifdef DEBUG_FEATURE_GSM_R_FACCH
        #error code not present
#endif
      }
    }
    else if ( facch_delta > TCH_NUM_FRAMES_BETWEEN_BLOCKS_R_FACCH_MAX)
    {
      /* No r-facch support */
      l1_tch_data[gas_id].r_facch_support_detected = FALSE;
    }
    else
    {
      /* r-facch support*/
      l1_tch_data[gas_id].r_facch_support_detected = TRUE;

      /* only let l1_tch_send a ph_data_ind if 1st facch in rfacch blk
      was not correctly decoded */
      if (previousFacchData_valid)
      {
        if((l1_tch_data[gas_id].previousFacchData[0]==rpt->data[0]) &&
          (l1_tch_data[gas_id].previousFacchData[1]==rpt->data[1]) &&
          (l1_tch_data[gas_id].previousFacchData[2]==rpt->data[2]))
        {
          send_L2_ph_ind_msg = FALSE;
#ifdef DEBUG_FEATURE_GSM_R_FACCH
          #error code not present
#endif
        }
        else
        {
          if (rpt->hdr.good_data==TRUE)
          {
            send_L2_ph_ind_msg       = TRUE;
            l1_tch_data[gas_id].r_facch_support_detected = FALSE;
#ifdef DEBUG_FEATURE_GSM_R_FACCH
            #error code not present
#endif
          }
        }
      }
    }
    /* Only copy Header if CRC good otherwise will copy junk into array */
    if (rpt->hdr.good_data==TRUE)
    {
      l1_tch_data[gas_id].previousFacchData[0] = rpt->data[0];
      l1_tch_data[gas_id].previousFacchData[1] = rpt->data[1];
      l1_tch_data[gas_id].previousFacchData[2] = rpt->data[2];
      l1_tch_data[gas_id].previousFacchData_FN = (uint16)burst_fn;
    }


    if ( (chan == GL1_MSG_FACCH_HS) ||(chan == GL1_MSG_FACCH_AHS) )
    {
        /* We are not told by the BTS if R-FACCH is supported.
        Also BTS may well dynamically enable this feature (44.006 10.1).
        We need to tell MDSP that there MAY be a R-FACCH at a certain
        FN
        */
      possible_next_rfacch_fn = (uint16)ADD_MOD(burst_fn, TCH_NUM_FRAMES_BETWEEN_BLOCKS_R_FACCH_MIN + FRAMES_PER_BLOCK - 2, 42432);

      MSG_GERAN_HIGH_2_G("RFACCH: HS processing, possible next rfacch: %d, CRC: %d", possible_next_rfacch_fn, rpt->hdr.good_data);

        if (chan == GL1_MSG_FACCH_AHS)
        {
          l1_tch_data[gas_id].half_rate_subchannel = l1_amr_get_hr_subchannel(gas_id);
        }

        if (l1_tch_data[gas_id].half_rate_subchannel == 1)
        {
          /*normalise burst_fn when on sub-chan 1*/
          burst_fn--;
          MSG_GERAN_HIGH_3_G("R-FACCH: chan = %d, l1_tch_data[gas_id].half_rate_subchannel = %d, burst_fn = %d",
            chan, l1_tch_data[gas_id].half_rate_subchannel, burst_fn);
          if (burst_fn < 0)
          {
            MSG_GERAN_HIGH_2_G("HR FACCH sub_chan = 1 on wrong FN.  Fnmod26 = %d, FN = %d",
                     frame_counters[gas_id].FNmod26,frame_counters[gas_id].FNmod42432);
          }
        }

        if (burst_fn%13 != 0)
        {
          /*if a SACCH/IDLE block is going to occur between the R-FACCH blks
          we need to increment by 1 */
          possible_next_rfacch_fn++;
        }
        /* Only indicate to mDSP to look for r_facch if this facch blk has
        bad CRC.
        Turn this mechanism off if the FACCH blocks are back to back.
        */
        if ( (!rpt->hdr.good_data) )
        {
          /* Inform driver layer so that it can signal this to the MDSP*/
        if ((chan == GL1_MSG_FACCH_HS) ||
            (rpt->hdr.chan == (int32)GL1_MSG_FACCH_AHS))
          {
            l1_tch_data[gas_id].possible_rfacch_fn[l1_tch_data[gas_id].facch_soft_buffer_index] = possible_next_rfacch_fn;

            MSG_GERAN_HIGH_2_G("Decode RFACCH at FN: %d (mod 42432), fw facch buffer index: %d", 
              possible_next_rfacch_fn, l1_tch_data[gas_id].facch_soft_buffer_index);

            l1_tch_data[gas_id].facch_soft_buffer_index = !l1_tch_data[gas_id].facch_soft_buffer_index;
          }
          else
          {
            MSG_GERAN_ERROR_2_G("Unable to handle Channel type %d %d in RFACCH handler",
              chan,rpt->hdr.chan);
          }
        }
      }
    else
    {


      /* We are not told by the BTS if R-FACCH is supported.
      Also BTS may well dynamically enable this feature (44.006 10.1).
      We need to tell MDSP that there MAY be a R-FACCH at a certain
      FN */

      possible_next_rfacch_fn = (uint16)ADD_MOD(burst_fn, TCH_NUM_FRAMES_BETWEEN_BLOCKS_R_FACCH_MIN + FRAMES_PER_BLOCK - 1, 42432);

      if (burst_fn%13 != 0)
      {
        /* if a SACCH/IDLE block is going to occur between the R-FACCH blks
        we need to increment by 1 */
        possible_next_rfacch_fn++;
      }

      /* Only indicate to mDSP to look for r_facch if this facch blk has
      bad CRC.
      Turn this mechanism off if the FACCH blocks are back to back.
      */
      if ( (!rpt->hdr.good_data) )
      {
        /* Inform driver layer so that it can signal this to the MDSP*/
        if ((chan == GL1_MSG_FACCH) || 
            (rpt->hdr.chan == (int32)GL1_MSG_FACCH_AFS) || 
          (rpt->hdr.chan == (int32)GL1_MSG_FACCH_WFS)  )
        {
          l1_tch_data[gas_id].possible_rfacch_fn[l1_tch_data[gas_id].facch_soft_buffer_index] = possible_next_rfacch_fn;

          MSG_GERAN_HIGH_3_G("Decode RFACCH at FN: %d (mod 42432), fw facch buffer index: %d, burst fn %d", 
              possible_next_rfacch_fn, l1_tch_data[gas_id].facch_soft_buffer_index, burst_fn);

          l1_tch_data[gas_id].facch_soft_buffer_index = !l1_tch_data[gas_id].facch_soft_buffer_index;
        }
        else
        {
          MSG_GERAN_ERROR_2_G("Unable to handle Channel type %d %d in RFACCH handler",
            chan,rpt->hdr.chan);
        }
      }
    }

    /* Now we check (retrospectively) to see whether R-FACCH had been
    enabled by the BTS */

#ifdef DEBUG_FEATURE_GSM_R_FACCH
    #error code not present
#else
   if(l1_tch_data[gas_id].r_facch_support_detected==TRUE)
   {
     MSG_GERAN_MED_0_G("R-FACCH: Support Detected");
   }
#endif

  }/*r_facch_nv_flag_active == TRUE*/
  else
  {
    MSG_GERAN_HIGH_0_G("RFACCH NV Item NOT set");
  }
  return (send_L2_ph_ind_msg);
}

/*===========================================================================

FUNCTION  l1_tch_set_rfacch_fw_buffer_index

DESCRIPTION
  Set the rfacch buffer index. This buffer in firmware is used to store
  the soft bits of failed FACCH to try to recombine with RFACCH

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_tch_set_rfacch_fw_buffer_index(uint8 index, gas_id_t gas_id)
{
  l1_tch_data[gas_id].facch_soft_buffer_index = index ? 1 : 0;
}

/*===========================================================================

FUNCTION  l1_tch_get_rfacch_fw_buffer_index

DESCRIPTION
  Returns the current value of rfacch buffer index.

DEPENDENCIES
  None

RETURN VALUE
  Returns the current value of rfacch buffer index

SIDE EFFECTS
  None
===========================================================================*/
uint8 l1_tch_get_rfacch_fw_buffer_index(gas_id_t gas_id)
{
  return l1_tch_data[gas_id].facch_soft_buffer_index;
}

/*===========================================================================

FUNCTION  l1_tch_is_rfacch_decode_frame

DESCRIPTION
  Check if it is necessary to decode an rfacch at frame fn_mod42432

DEPENDENCIES
  None

RETURN VALUE
  Return TRUE if RFACCH needs to be decoded and sets rfacch_buffer_index
  to the index value of the associated buffer

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_tch_is_rfacch_decode_frame(uint16 fn_mod42432, uint8* rfacch_buffer_index, gas_id_t gas_id)
{
  MSG_GERAN_DEBUG_OPT_3_G("Is RFACCH decode, fn_mod42432 %d, possible rfacch[0] %d, possible rfacch[1] %d", 
      fn_mod42432, l1_tch_data[gas_id].possible_rfacch_fn[0], l1_tch_data[gas_id].possible_rfacch_fn[1]);

  if (l1_tch_data[gas_id].possible_rfacch_fn[0] == fn_mod42432)
  {
    *rfacch_buffer_index = 0;
    return TRUE;
  }

  if (l1_tch_data[gas_id].possible_rfacch_fn[1] == fn_mod42432)
  {
    *rfacch_buffer_index = 1;
    return TRUE;
  }

  *rfacch_buffer_index = 0;
  return FALSE;
}

/*===========================================================================

FUNCTION  clear_rfacch_decode_frame

DESCRIPTION
  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void clear_rfacch_decode_frame(uint8 rfacch_buffer_index, gas_id_t gas_id)
{      
  l1_tch_data[gas_id].possible_rfacch_fn[rfacch_buffer_index] = 0xFFFF;
}

 #ifdef DEBUG_FEATURE_GSM_R_FACCH
#error code not present
#endif /*DEBUG_FEATURE_GSM_R_FACCH*/

/*===========================================================================

FUNCTION  l1_rsacch_dl_process_block

DESCRIPTION
  This function extracts the SRO flag from the SACCH header and sets the SRR
  flag if required.

DEPENDENCIES
  None

RETURN VALUE
  boolean rsacch_use_ta_pwr

SIDE EFFECTS
  None
===========================================================================*/
static void l1_rsacch_dl_process_block (cch_decode_rpt *rpt, gas_id_t gas_id)
{
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    l1_dedicated_data_ptr->rsacch_ctrl.srr_flag = FALSE;
    l1_dedicated_data_ptr->rsacch_ctrl.sro_flag = FALSE;

    if (rpt->hdr.good_data==TRUE)
    {
       /*Extract SRO flag*/
       l1_dedicated_data_ptr->rsacch_ctrl.sro_flag =
                    (boolean)((rpt->data[0] & L1_SACCH_HDR_SRO_BIT)>>L1_SACCH_SRO_BIT_RIGHT_SHIFT);

       if (l1_dedicated_data_ptr->rsacch_ctrl.sro_flag==FALSE)
       {
           /*Clear RSACCH buffer if SRR=0, repeat tx not required*/
           l1_dedicated_data_ptr->rsacch_ctrl.buffer_valid = FALSE;
       }
       else if (l1_tch_data[gas_id].l1_rsacch_nv_flags_advance_mode == TRUE)
       {
#ifdef DEBUG_FEATURE_GSM_R_SACCH
           #error code not present
#endif
           l1_dedicated_data_ptr->rsacch_ctrl.network_initiated = TRUE;
       }

       if(rpt->hdr.rsacch_comb_status==TRUE)
       {
           /*Must inform the network of a successful Recomb, set SRR flag*/
           l1_dedicated_data_ptr->rsacch_ctrl.srr_flag     = TRUE;
           /*Repeated SACCH block SRO not valid*/
           l1_dedicated_data_ptr->rsacch_ctrl.sro_flag     = FALSE;
           l1_dedicated_data_ptr->rsacch_ctrl.buffer_valid = FALSE;
#ifdef DEBUG_FEATURE_GSM_R_SACCH
           #error code not present
#endif
       }
       MSG_GERAN_HIGH_3_G("R-SACCH: DL CRC_OK SRO flag %d, Hdr 0x%x, Comb Status %d" ,
                    l1_dedicated_data_ptr->rsacch_ctrl.sro_flag,
                    rpt->data[0],
                    rpt->hdr.rsacch_comb_status);

#ifdef DEBUG_FEATURE_GSM_R_SACCH
       #error code not present
#endif
    }
    else
    {
       if(l1_tch_data[gas_id].l1_rsacch_nv_flags_advance_mode == TRUE)
       {
           if(l1_dedicated_data_ptr->rsacch_ctrl.network_initiated==TRUE)
           {
               /*From CRC set SRR flag*/
               l1_dedicated_data_ptr->rsacch_ctrl.srr_flag = TRUE;
           }
#ifdef DEBUG_FEATURE_GSM_R_SACCH
           #error code not present
#endif
       }
       else
       {
           /*From CRC set SRR flag*/
           l1_dedicated_data_ptr->rsacch_ctrl.srr_flag = TRUE;
       }

       /*Clear RSACCH buffer as no of knowing what the SRO was*/
       l1_dedicated_data_ptr->rsacch_ctrl.buffer_valid = FALSE;

       MSG_GERAN_HIGH_2_G("R-SACCH: DL CRC_BAD Comb Status %d, SoftBits Stored %d",
                    rpt->hdr.rsacch_comb_status,rpt->hdr.rsacch_store_status);
#ifdef DEBUG_FEATURE_GSM_R_SACCH
       #error code not present
#endif
    }
    /*Save CRC*/
    l1_dedicated_data_ptr->rsacch_ctrl.prev_crc = rpt->hdr.good_data;

    MSG_GERAN_HIGH_3_G("R-SACCH: DL SRR %d, SRO %d, BufV %d",
                l1_dedicated_data_ptr->rsacch_ctrl.srr_flag,
                l1_dedicated_data_ptr->rsacch_ctrl.sro_flag,
                l1_dedicated_data_ptr->rsacch_ctrl.buffer_valid);
#ifdef DEBUG_FEATURE_GSM_R_SACCH
    #error code not present
#endif
}

/*===========================================================================

FUNCTION  l1_rsacch_init_params

DESCRIPTION
  This function initialises the RSACCH flags on a TCH channel modification.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_rsacch_init_params (boolean ded_active, gas_id_t gas_id)
{
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    if(l1_dedicated_data_ptr->rsacch_ctrl.nv_active == TRUE)
    {
        /*Reset L1 R-SACCH params*/
        l1_dedicated_data_ptr->rsacch_ctrl.l2_repetition_info = L2_NOT_REPETITION_CANDIDATE;
        l1_dedicated_data_ptr->rsacch_ctrl.srr_flag           = FALSE;
        l1_dedicated_data_ptr->rsacch_ctrl.sro_flag           = FALSE;
        l1_dedicated_data_ptr->rsacch_ctrl.buffer_valid       = FALSE;
        /*Prompt the DSP R-SACCH functionality*/
        l1_dedicated_data_ptr->rsacch_ctrl.active             = ded_active;
        l1_dedicated_data_ptr->rsacch_ctrl.prev_crc           = FALSE;
        /*wait on the SRO being set before using SRR*/
        l1_dedicated_data_ptr->rsacch_ctrl.network_initiated  = FALSE;
#ifdef DEBUG_FEATURE_GSM_R_SACCH
        #error code not present
#endif
        MSG_GERAN_HIGH_1_G("R-SACCH: Params Reset, DSP Active %d",ded_active);
    }
}

/*===========================================================================

FUNCTION  l1_rsacch_nv_flags_active

DESCRIPTION
  This function returns the NV settings for ACCH.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None
===========================================================================*/
static boolean l1_rsacch_nv_flags_active ( gas_id_t gas_id )
{
    boolean active=FALSE;
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    /*Check on of the R-SACCH flags is set*/
    if((l1_dedicated_data_ptr->rsacch_ctrl.nv_flag &  L1_GSM_ACCH_NV_FLAG_ENABLED)
                                              == L1_GSM_ACCH_NV_FLAG_ENABLED)
    {
        active = TRUE;
        l1_tch_data[gas_id].l1_rsacch_nv_flags_advance_mode = FALSE;
    }

    return active;
}

 #ifdef DEBUG_FEATURE_GSM_R_SACCH
 #error code not present
#endif

/*===========================================================================

FUNCTION  l1_acch_nv_flag_initialise

DESCRIPTION
  This function reads the ACCH NV Flag.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_acch_nv_flag_initialise ( gas_id_t gas_id )
{
    uint16 nv_flag = gl1_msg_acch_get_nv_flag(gas_id);
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    l1_dedicated_data_ptr->rsacch_ctrl.nv_flag   = (boolean)nv_flag;
    l1_dedicated_data_ptr->rsacch_ctrl.nv_active = l1_rsacch_nv_flags_active(gas_id);
    l1_rsacch_init_params(TRUE, gas_id);
    l1_rfacch_handle_nv_flag(nv_flag, gas_id);
#if defined (DEBUG_FEATURE_GSM_R_SACCH) || defined (DEBUG_FEATURE_GSM_R_FACCH)
    #error code not present
#endif
}

/*===========================================================================

FUNCTION  l1_indicate_x2g_ho

DESCRIPTION
  This function sets the x2g ho indicator flag

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void l1_indicate_x2g_ho( boolean x2g_flag, gas_id_t gas_id )
{
    l1_tch_data[gas_id].x2g_ho_indicator = x2g_flag;
}
/*===========================================================================

FUNCTION  l1_check_for_x2g_ho

DESCRIPTION
  This function reads x2g ho indicator flag

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

boolean l1_check_for_x2g_ho(gas_id_t gas_id)
{

  if (l1_tch_data[gas_id].x2g_ho_indicator)
  {
    return (TRUE);
  }
  /* since this is not a g2w HO OR voc acq.
   * already complete set to FALSE
   */
  return FALSE;
}

/*===========================================================================

FUNCTION  l1_get_tch_chan_mode

DESCRIPTION
  This function returns tch data channel mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
eng_mode_chan_type_t l1_get_tch_chan_mode(gas_id_t gas_id)
{
  return(l1_tch_data[gas_id].chan_mode);
}

/*===========================================================================

FUNCTION  l1_get_tch_dtx_state

DESCRIPTION
  This function returns if the tch state machine is currently in dtx mode

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_get_tch_dtx_state(gas_id_t gas_id)
{
   return l1_tch_data[gas_id].dtx;
}

#ifdef FEATURE_GSM_COEX_SW_CXM
/*===========================================================================

FUNCTION  l1_tch_get_arfcns

DESCRIPTION
  This function determines the ARFCNs to be used on the specified frames 
  according to the defined frequency hopping parameters.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void l1_tch_get_arfcns(int16 start, uint16 increment, uint16 num_arfcns, ARFCN_T arfcns[], gas_id_t gas_id)
{
  dedicated_data_T *ded_dtr_ptr = &l1_dedicated_data[gas_id];

  L1_get_ARFCNS ( start, increment, num_arfcns, arfcns,
                  &ded_dtr_ptr->channel_specification.channel_info_1_before,
                  &ded_dtr_ptr->channel_specification.channel_info_1_after,
                  &ded_dtr_ptr->new_channel_info, gas_id );
}
#endif /* FEATURE_GSM_COEX_SW_CXM */
