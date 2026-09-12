/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

L 1   D E D I C A T E D - A M R   T R A F F I C

GENERAL DESCRIPTION
This module contains the procedures to control the AMR full and half rate
traffic channel.  These procedure executes in the ISR context and
signals the L1 task of any status.

EXTERNALIZED FUNCTIONS
L1_control_amr_fr
This function controls the AMR full rate traffic channel
L1_control_amr_hr
This function controls the AMR half rate traffic channel

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.
$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_tch_amr.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
11/12/17   mk       CR2152940: Revert CR 2001095 and related CR changes
26/06/18   sid      CR2267640 Block FACCH transmissions while aborting current channel
03/05/18   rv       CR2233364 : Add feature flags for FR38904 changes - GERAN
14/11/16   mk       CR1082549 : FR 38904: Audio gap optimization during Handovers in Voice call - GERAN
09/02/16   mk       CR2002074 Do not include offset for relative FN calculation for TxLev 
03/01/16   mn       CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
11/05/17   mk       CR2045486: Revert the CMC optimizing enhancement CR 1025288 which degrades MOS score as vocoder function is not in line with GL1 CMC update
30/05/17   rv       CR2031381: L1 fix to address issues with CR2001095 causing GCF failure
10/03/17   hd       CR2011309 Drop a speech frame if vocoder is sending it with a codec mode out of ACS  
06/03/17   sp       CR2014559: Ensure FACCH fill frame not sent during Loopbacks
07/04/17   mk       CR2026575 Always send SID_FIRST followed by FACCH or RATSCCH in case U/L is in DTx to avoid interleaving issues
02/02/17   hd       CR2001095 Not to send homing sequence when vocoder is ready, but not transmitting
08/12/16   mk       CR1097205: Fixing AGC initialisation when Rxd is getting enabled and enhancing serving cell measurement reporting when Rxd was partially enabled in a SACCH period
19/09/16   dg       CR1066652 Make MCVS request of 384 mhz 1 frame in advance before initiating G2L search in dedicated mode
30/06/16   mk       CR957925  Do not send DTX disable to vocoder blindly in HO scenario which impacts voice quality
07/06/16   mk       CR1025288 Fixing one frame delay in applying CMC in AMR ODD PHASE
16/05/16   mk       CR1010068 Add DTF events in dedicated mode
12/05/16   rv       CR998395: Excessive F3 logging in  GERAN L1
18/04/16   hd       CR1004583: Delay the vocoder stop and start till the point just before VFR is aligned 
                    to optimize the mute duration during the time of G2G handover
23/02/16   sk       CR973190 Excessive F3 logging reduction
18/02/16   hd       CR978516 No need to check for block boundary while sending PH_DATA_IND to L2 
01/02/16   db       CR965933 GCF TC 26.21.8-4 Handover failure
26/08/15   ws       CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
14/08/15   pjr      CR890777 Fix logic to delay RATSCCH messages
15/07/15   mc       CR861999 Failure to abort TCH mode when G+G COEX manager denies permission to FACCH Tx
04/06/15   jk       CR846952:RxLEV report should be based on maximum of PRx and DRx average RSSIs
27/05/15   msa      CR844425 Incorporate new diag services API for logging events with subscription id
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
09/04/15   cja      CR819956 G2T changes to support new RF API's  FEATURE_NEW_RF_API
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
07/04/15   shr      CR808932: COEX support for DTM
01/04/15   pjr      CR810462 Ensure Vocoder acquire/enable sequence is respected during srvcc handover
30/03/15   mc       CR773788: COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
11/03/15   pjr      CR806250 Fix featurisation for changes submitted under CR787199
24/02/15   pjr      CR787199 Acquire vocoder immediately after IMS releases it during srvcc handover
17/11/14   dg       CR758006 IRAT improvements to ease debugging
20/02/15   js       CR693771: W+T removal. Resolving compilation errors 
16/12/14   jk       CR753423:LIF for Idle,voice and data
31/10/14   xz       CR748816 Extend dual data space for building RF script variable 
19/08/14   pjr      CR711506 Support for AMR Partial Decode and DTX detection FR19667
20/05/14   hd       CR668030: Meas Report is sent after processing the blocks from FW so that it contains 
                    the block info from the current Meas Period.
17/07/14   pjr      CR672464 Removed fake VFR realignment sequence
11/07/14   pjr      CR 692177 Added support for RFACCH 2.0
10/07/14   jk       CR688587:GL1 changes to support LIF feature
02/07/14   mc       CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
29/04/14   pjr      CR600612 Dedicated WTR Hop initial code drop
08/05/14   cgc      CR642356 fix prototype for tch_rfacch_indicator() added to header.
06/05/14   npt      CR659704 Ensure VFR is assigned before trying to align it
08/04/14   jk       CR649527 VAMOS2 Fixes for Freqeucny hopping and DTX ON cases
19/03/14   ws       CR635101 Resolve P1 Critical Klocwork errors in GL1 code space
13/03/14   mc       CR602368 COEX : Support for Frequency Hopping over large ARFCN lists
19/02/14   jk       CR608290 GERAN RxD ON/OFF adaptive algorithm
02/01/14   jk       CR595130 GL1 changes to support VAMOS-II
19/12/13   jk       CR503359 GL1 changes to support Rx diversity
12/12/13   pjr      CR588712 Call mn_has_nas_already_acquired_voc when FEATURE_DUAL_ACTIVE is enabled
10/12/13   npt      CR586981 Fix the featurisation for UL SACCH 
29/11/13   js       CR574402 COEX : VBATT : Accurate prediction of N+1 frame Tx power for CS (TCH and SDCCH)
02/10/13   pjr      CR552448 DDR Frequency plan implementation - l1_arbitration modification
04/11/13   mc       CR556742 G+G COEX : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
28/11/13   cja      CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
22/11/13   cja      CR455527 Remove featurisation from files in API directory
24/09/13   jk       CR549227:Shifted SACCH changes for VAMOS2
07/08/13   jk       CR490807:Support for shifted SACCH  
11/11/13   ws       CR 573757 Featurise GSMTR-1 registers and Voice services driver
08/11/13   cja      CR555989 Do not add extra power monitors until first complete TCH multiframe.
05/11/13   cja      CR523987 Add FEATURE_GSM_DED_SCELL_MONITOR for extra SCell monitor in voice call
24/10/13   cja      CR566207 Generate filler frame for FR signalling only channel.
17/10/13   ws       CR548645 - Compile errors with no Voice support flavour
10/09/13   og       CR517185. Ensure the ghdi_mvs AMR API is not called for the 
                    GSM only subscription.
28/08/13   mko    CR532432:While running the TCH abort FTN processing is not needed. Check is added to fix it
15/08/13   sk       CR524039 GBTA bringup changes
7/16/13    zc    	CR470197 Replace memcpy with memscpy
25/06/13   cs       Major Triton TSTS Syncup
24/05/13   ap       CR472264 DSDS: Fix the gas_id problems when the IQ logging is enabled
16/05/13   pjr      CR487402: Changes to improve interaction with Voice Services
03/04/13   pjr      CR470940 Modification to calls to new VS interface
07/03/13   dv       CR461080: Introduction of new GSM VS interface
13/03/13   mc       CR459487 : COEX Support - GERAN arbitrator framework
04/03/13   ws       CR457871 - Crash in l1_acch_nv_flag_initialise() due to missing gas_id
25/10/12   ws       CR410929 Move out of DL DTX mode if we receive SID_UPDATE_INH
07/12/12   jv       CR438196: Using RF Task API gl1_hw_rf_idle_frame_processing() for performing idle frame processing.
07/12/12   cja      CR429173 Remove Dime compiler warnings
22/06/12   pa       CR371580: Delay TBF release during G2T activity.
25/04/12   pa       CR355881: G2T Dedicated TCH: TL1 crashed in RF API. Added support of RF_SCRIPTS_CMD.
13/03/12   ws       CR 330862 - Change dedicated mode F3 level's and add sacch burst
                    metrics and compact message metrics log packets.
19/03/11   rc       CR:344697 Featurized code under FEATURE_MODEM_NO_VOICE_SUPPORT to support no voice builds
10/02/12   pg       CR334798: Adjust ARFCNs for 3rd and 4th bursts for AHS SID_UPDATE
                    spanning idle frame
07/11/11   cja      CR311154 Add request MCPM AEq on for AMR
18/10/11   jj       CR310766 Support of GSM thermal Read in idle Frame
16/05/11   nf       CR281320 R-SACCH flags not correctly set up
19/04/11   cja      Add cipher clock gating for Nikel
01/04/11   pg       CR280752 - Only transition to new ACS conveyed in RATSCCH after
                    sending previous audio packet to MVS
18/03/11   ws       Added Initial MCPM updates
11/03/11   cja      CR278492. Add WB-AMR for ghdi on even phase, missed on CR262902
24/12/10   cja      CR262902. Add WB-AMR calls for ghdi
21/12/10   cja      CR266729 Correct frame for U/L mode change.
22/10/10   dv       CR261234 - Add Homing Sequences for fusion products
21/10/10   dv       CR259486 - Dereference before parameter null-check
17/05/10   cja      CR237837 - if dynamic SAIC and SAIC disabled then force C/I to max
13/05/10   cja      CR237836 - check CRC of SID update before including in C/I calculation
15/04/10   dv       Correct miss-spelling of FEATURE_GSM_AMR_WB
06/04/10   dv       CR228666 AHS codec rate fluctuations in link adaptation TCs@Nortel IOT
26/02/10   dv       CR214481 amr-wb 8k gsm: buzzing noise on far-end during MT
                    call 'wait for answer'
15/02/10   cja      Re-enable GSAC clock when dedicated started if cipher enabled.
20/01/10   cs       CR221032 21.3.1 failure with SAIC enabled as AFC/TT not updating
05/01/10   cs       CR 218984 GCF 13.3.4.1 failure on high bands TX lev wrongly
                    reported
17/11/09   cs       CR218239 Use the correct ARFCN for priming AFC loop with band info
09-10-28   tjw      Clean up medium Lint errors
09/10/09   dv       CR188820 - change of name for NAS API
23/09/09   dv       Featurise fix for CR188820
14/09/09   dv       CR188820 - update fix as 1st check-in was not complete
14/09/09   cs       Fix Lint indent failures
28/08/09   dv       CR188820 - Voice heard before MT call connected after alerting
11/09/09   ps       Resolve Klocwork errors
04/08/09   dv       CR191821 - 8K AMR-WB L1 Modem Changes for GSM
08/07/09   nt       CR:161980 - Further modification to ensure no changes if the
                    threshold is not set.
05/05/09   nt       CR:161980 - Unequal C/I normalization values for AMR TC 14.10.1
                    and 14.10.2.
29/04/09   ws       Fixed Lint Warnings for MOB environment
24/03/09   dv       CR176933 - Call mute after ending Loopback C Test
06/04/09   dv       CR172437:Distortion at far end while answering MT call
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
23/02/09   dv       CR169424:Cannot make MO/MT calls
13/02/09   ap       Lint fixes for high Warning
03/12/08   ws       Added FEATURE_GSM_DYNAMIC_SAIC
12/01/08   gj       CR165617 - mainline FEATURE_GSM_GPRS_ENHANCED_MEASUREMENT_REPORTING feature.
27/11/08   ws       Added FEATURE_MVS_UNDER_GHDI to remove all direct MVS calls.
25/11/08   cs       Fix AMR_WB Tch init F3
23/10/08   dv       CR159576:glitch in audio when CRC falsely passes in DTX
03/10/08   cs       Fix HIGH Lint errors
02/09/08   dv       CR:155182 Buzz on UL after WB_AMR Codec Change
02/09/08   dv       CR:150818 AMR-WB fix for frame classification
26/08/08   hv       Fixed ALIGN usage to cater for non-L4/ARM targets
22/08/08   nt       Removed compiler warnings
10/07/08   dv       CR151169:Fix ACS values displayed in 0x5079 log pkt for WB_AMR
3/06/08    ws       Swap Endian for 8K targets that support QDSP6 Vocoder
12/05/08   dv       Added FEATURE_GSM_AMR_WB - L1 component
20/03/08   agv      Fixed the LCU PMIC Band setup for a Channel Change.
15/02/08   ws       Removed l1_msg_clear_esaic_iir_filter() for ESAIC in SID_FIRST case
                    since this imapct SACCH performance
16/01/08   ws       Added gl1_msg_clear_esaic_iir_filter() for ESAIC in SID_FIRST case
14/01/08   cs       Allow for separate IQ logging and saving
14/12/07   ws       merged LCU Bringup and demo changes from 6260 branch
27/11/07   nt       CR111687 - Fix for ISR overrun problem.
09/11/07   nf       Added FEATURE_GSM_R_SACCH
02/10/07   og       Clean up of unused code.
17/09/07   agv      Added I/Q logging to code base.
31/08/07   agv      CR127468 Fix for SAIC TT handling for TCH AMR.
08/07/07   ws       CR - 124046 Open loopback mode C when we need to send FACCH
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
12/07/07   agv      CR123023 - revert unsafe change.
06/07/07   agv      CR123023 - Update fix for AFC corruption.
27/06/07   agv      CR 123023 Fix for TT. Only use validated SACCH metrics for TT until
                    SID, and FACCH change ready.
20/06/07   ws       Backed out CR 119738, performance gets worse with this change
31/05/07   ws       CR 120246 - don't send SPEECH_GOOD after FACCH TX when in DTX
                    we can immediately send SID_FIRST followed by SID_UPDATE
24/05/07   ws       CR 119738 - Manage RXQUAL_SUB average for weak signal areas where DTX in DL
                    is not used
23/05/07   ws       CR 119507 - removed FEATURE_GSM_AMR_USE_SID_FOR_RXQUAL_SUB,
                    use SID_UPDATE frames for RXQUAL_SUB measurements
23/05/07   ws       CR 117197 - Fix for calculating CMR on SID_UPDATE Tx frames.
                    Previously CMR value calculated before DTX was being used
23/05/07   ws       Enabled GL1_TCH_AMR_DEBUG to aid debugging AMR link adaption issues
28/11/06   dv       Lint cleanup
27/11/06   og       Remove un-necessary arithmetic modulo operations.
30/10/06   og       EDTM initialisation support for half rate channels.
30/10/06   og       EDTM fixes to establishment in TCH AMR.
14/08/06   ws       Allow dedicated to be timeslot slammed to lowest received tn
                    and also to place the CS timeslot on TN 2
07/08/06   ws       CR 95398 - Optimization of C/I calculation for frequency hopping
06/04/06   av       Feature SAIC MIMO elements added to the burst metrics.
03/20/06   ws       Removed conditional check for sending FACCH decode to L2.
02/17/06   ws       Added additional CRC TCH profiling for SAIC debugging
23/01/06   og       Removal of QLint high errors.
01/11/06   ws       CR 85789 - Update CMR when CMI coincides with SID_UPDATE frames
09/10/05   dp       Fix to call l1_sci_monscan_next_period_ded() a frame
                    earlier so that measurements get shipped upto RR immediately
08/19/05   gfr      Use gl1_msg_init_frame_tick_info to init the ftn structure
08/19/05   ws       Added setting AFC_TT_MODE if DTM is enabled
08/16/05   dp       Lint cleanup
08/12/05   gfr      Consolidate tx aborts into gl1_msg_abort_tx_tch_amr_hr/fr
08/12/05   ws       Added initial FEATURE_GSM_DTM support
07/29/05   gfr      Support for NPL1 DTM interface changes
07/20/05   pp       Don't use SID_FIRST C/I for CMR generation in full rate
07/15/05   pp       Fix for 26.16.9.11 test case.
06/24/05   dp       Fix to exclude BCCH carrier from RXLEV average if its in
                    the hopping list and the PWRC bit is set
06/08/05   dp       Moved aborts after FTN so that we can abort as soon as
                    NPL1 is done aborting
05/25/05   dp       Renamed FNmod13 to not conflict with global.   Put checks
                    around access burst decrementing.  Fixed bug in voc init
                    in HR
05/25/05   dp       Fix to not recompute FNmod13
05/06/05   dp       Temp workaround to compute RXQUAL_SUB only on SACCH to
                    reduce number of handovers
05/03/05   pp       Fix for 14.10 test case. Fix to change vocoder codec mode
                    stepwise in alternate speech frames for CMC jumps of more
                    than one step. Optimization to only issue codec mode change
                    command to vocoder if CMC changes.
04/21/05   gfr      Align tx data on a word boundary to optimize NPL1 processing.
03/30/05   pp       Call l1_amr_start_ftn_fr_hr() after aborts - fixes merge error
03/29/05   dp       InterRAT audio optimizations
03/24/05   pp       Fix to only send onset before first speech frame after coming
                    out of DTX
03/15/05   dp       Added support for enhanced measurement reporting
03/11/05   sv       Mainlined FEATURE_SCE.
01/24/05   pp       Changes to support AMR RATSCCH response event logging and
                    GL1_MSG_ONSET channel type logging. Changed calls to
                    l1_log_transmit_burst_metrics() to conform to new interface
01/05/05   dp       Merged dedicated timeline changes for audio optimizations
                    to mainline
11/01/04   yhong    Call gl1_msg_frame_tick_notification() after aborts
08/18/04   yhong    Merged tch abort calls
08/12/04   gsc      Added support for Loopback I.
06/24/04   dp       Fixed half rate FACCH bug ( compute burst_fn mod 104 in
                    HR callback )
05/26/04   sv       Lint Cleanup.
04/20/04   dp       Fix to align the VFR just once after new channel setup
03/18/04   gsc      Lint Cleanup
03/12/04   dp       Align VFR and enable MVS on first SACCH or idle frame
                    whichever comes earlier
02/19/04   gfr      AFC cleanup.
02/18/04   gsc      Start ACK Activation counter in RATSCCH rx callback function.
                    Update REQ Activation and ACK Activation counter at the
                    end of rx_callback after the received block has been completely
                    processed. Get CMC or CMI from the received packet even when
                    CRC failed (SPEECH_BAD frames).
01/30/04   dp       TT fix for 16.1(Leopard).  Apply TT correction every
                    SACCH block.  Merged this to Saber mainline
01/20/04   dp       Fix to not switch off and switch on the vocoder across
                    inter mode GSM handovers ( merely mvs_enable )
12/30/03   gsc      Removed GL1_USE_NEW_TX_STATE_MACHINE and code under feature
                    definition. Force TX state to DTX if SID_UPDATE is received
                    in TCH state.
12/19/03   gsc      Fixes to handle back to back SID_FIRST and SID_UPDATE in
                    Half Rate.
12/10/03   gsc      Reverted back to use the previous CMI/CMC in case we receive
                    invalid values. Pass idle frame to SCE for ncell scheduling
12/10/03   gsc      Reverted back the change to repeat codec modes in remaining
                    ACS entries if less than 4 modes are received in multirate
                    IE. There has been a fix in F/W to prevent out of bounds
                    CMI and CMC.
12/04/03   gsc      Repeat the codec modes in the active code set if less than
                    4 rates are recieved in multirate IE.
12/04/03   gsc      Send dummy speech frames in case the vocoder is intialized
                    but no packets are received from the vocoder.
11/25/03   gsc      Changed the way in which invalid CMI/CMC rcvd on the D/L
                    was being handled. We now use the the highest rate if we
                    receive CMI/CMC out of bounds wrt to the ACS.
11/24/03   gsc      Fix to send speech frames with homing sequence until the
                    vocoder is initialized to pass Cetecom test cases. Fix to
                    disable vocoder on old channel during handover only if
                    channel mode changes.
11/05/03   gsc      Merged changes from the main line into 5.0 branch. Removed
                    enabling/disabling of DTX based DTX indicator.
10/28/03   gsc      Fixed a problem that was causing incorrect scheduling of
                    SID_FIRST and SID_UPDATE in Half Rate.
10/22/03   gsc      AMR IOT fixes. Updates to TX state machine. Delaying of
                    SID_FIRST/SID_UPDATE in case of collision with FACCH/
                    RATSCCH. Controling vocoder enabling/disabling from layer1.
10/03/03   gsc      U/L codec mode adaptation changes
09/04/03   gsc      In the rx_callbacks, drop the very first block received
                    on the D/L
08/15/03   dp       Moved decrementing of number of access bursts outside
                    L1_more_abs_to_send to fix issues with incorrect number
                    of access bursts
08/11/03   gsc      Synced up with l1_tch.c#56. C/I filter related
                    changes. Changed most of the MSG_MED to MSG_LOW.
07/14/03   gsc      Cosmetic changes
07/14/03   gsc      Fixes to the RX side state machine. Fixes based on Ericsson
                    IOT
07/01/03   gsc      Some minor fixes. Support for loopbacks. Cleaned up F3
                    messages.
05/07/03   gsc      First revision. Used l1_tch.c(49) as the base line.

===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"

#ifdef CUST_H
    #include "customer.h"
#endif

#ifdef FEATURE_GSM_AMR

    #include "gl1_msg_g.h"
    #include "gl1_voice_server.h"
    #include "l1i.h"
    #include "l1_ds.h"
    #include "l1_ds_v.h"
    #include "l1_ds_v_g.h"
    #include "l1_isr.h"
    #include "l1_utils.h"
    #include "msg.h"
    #include "l1_sc.h"
    #include "l1_task.h"
    #include "l1_log.h"
    #include "l1_amr.h"
    #include "log.h"
    #include "event.h"
    #include "string.h"     /* For memset */
    #include <stringl/stringl.h>

    #include "geran_eng_mode_info.h"
    /* Enables SC functionality on the TCH */
    #undef DED_LOG
    #define TCH_PROFILING

    #undef RXQUAL_MSG
#ifdef FEATURE_GERAN_REDUCED_F3
#define GL1_TCH_AMR_DEBUG
#endif

#include "l1_sacch.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#include "gl1_msg_wtr_hop.h"
#include "mcfg_nv_api.h"

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif

/*===========================================================================

DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
    #define AMR_TCH_FS_DATA_WIDTH_OCTETS            33 /* (260/8) per GSM 5.03 */
    #define AMR_CHNLS_FS_DATA_WIDTH_OCTETS          38
    #define AMR_MAX_NSYNC_CNT                       12
    #define AMR_MAX_C_OVER_I                     32767

#ifdef FEATURE_GSM_COEX
    #define MAX_AMR_COEX_DENIELS_FOR_RATSCCH         2
    #define MAX_AMR_COEX_DENIALS_FOR_FACCH           2
#endif

/*---------------------------------------------------------------------------
 *                       LOCAL TYPES
 *-------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 * Various Channel Types used in Full and Half Rate state machines for the
 * transmit side only. The inhibits are handled by looking at couple of
 * variables - amr_hr_sid_first_tx_in_progress and
 * amr_hr_sid_update_tx_in_progress. If we have a FACCH, RATSCCH or SPEECH
 * to transmit and if sid_first or sid_update tx is in progress, we send
 * out a corrresponding inhibit.
 *-------------------------------------------------------------------------*/
typedef enum
{
    AMR_FACCH,
    AMR_RATSCCH,
    AMR_SPEECH_GOOD,
    AMR_SID_FIRST,
    AMR_SID_UPDATE,
    AMR_NO_DATA,
    AMR_MAX_CHNLS_FS
}amr_chnl_type;

/*---------------------------------------------------------------------------
 * AMR States
 *-------------------------------------------------------------------------*/
typedef enum
{
    AMR_INIT,
    AMR_DTX,
    AMR_TCH,
    AMR_MAX_STATES_FS
}amr_state_type;

/*---------------------------------------------------------------------------
 * AMR State Table for Full and Half Rate for TX side only.
 *-------------------------------------------------------------------------*/

amr_state_type
amr_state_table [AMR_MAX_STATES_FS][AMR_MAX_CHNLS_FS] =
{
    { AMR_TCH,  AMR_TCH,  AMR_TCH,  AMR_INIT, AMR_INIT, AMR_INIT},
    { AMR_TCH,  AMR_TCH,  AMR_TCH,  AMR_DTX,  AMR_DTX,  AMR_DTX},
    { AMR_TCH,  AMR_TCH,  AMR_TCH,  AMR_DTX,  AMR_TCH,  AMR_TCH}
};


/*---------------------------------------------------------------------------
 * This type is used to monitor the CMC turnaround time. We count the
 * the number of TDMA frames, at the ARM level, it took for the CMC received
 * on the D/L to to arrive as the mode used on the U/L by the vocoder
 *-------------------------------------------------------------------------*/
typedef struct
{
    cmi_phase_type phase;
    uint8 cmc;
    uint8 frame_count;
    boolean valid;
}amr_debug_data_type;



typedef struct
{

  amr_debug_data_type               amr_voc_turnaround_time_data;

  /*---------------------------------------------------------------------------
   * Current and new TX and RX AMR states
   *-------------------------------------------------------------------------*/
  amr_state_type                    amr_curr_tx_state;
  amr_state_type                    amr_new_tx_state;
  amr_state_type                    amr_rx_state;


  /*---------------------------------------------------------------------------
   * Booleans needed to remember if there was a sid_update collision with
   * the FACCH/RATSCCH/SPEECH frames
   *-------------------------------------------------------------------------*/
  boolean                           amr_sid_update_pending;
  boolean                           amr_sid_first_pending;

  /*---------------------------------------------------------------------------
   * Count used to stay in the INIT state for 11 frames after which we
   * transition to the DTX state or TCH state.
   *-------------------------------------------------------------------------*/
  uint8                             amr_nsync_cnt;

  /*---------------------------------------------------------------------------
   * Globals used to store the the position of sacch, idle, tch etc frames
   * in the 26 frame multi-frame structure.
   *-------------------------------------------------------------------------*/
  uint8                             amr_sacch_minus_one_frame;
  uint8                             amr_idle_minus_one_frame;
  uint8                             amr_idle_frame;
  uint8                             amr_send_sacch;
  uint8                             amr_reporting_period_end_minus_1;
  uint8                             amr_afc_correction_frame;
  gl1_msg_chan_type                 amr_channel_type;

  gl1_msg_voc_type                  amr_voc_type;

  /* Number of SC power measurements to schedule at a time */
  uint8                             amr_num_pwr_meas_to_schedule;
  uint8                             amr_num_pwr_meas_per_frame;

  /*---------------------------------------------------------------------------
   * Buffers to hold the payload for various channels, aligned to 16 bit
   * boundary for NPL1 processing optimization
   *-------------------------------------------------------------------------*/
#if defined(__GNUC__)
  uint8                             amr_tx_data[AMR_TCH_FS_DATA_WIDTH_OCTETS] __attribute__((aligned(16)));
  uint8                             amr_voc_data[AMR_CHNLS_FS_DATA_WIDTH_OCTETS] __attribute__((aligned(16)));
  uint8                             amr_facch_data[AMR_TCH_FS_DATA_WIDTH_OCTETS] __attribute__((aligned(16)));
#else
  ALIGN16 uint8                     amr_tx_data[AMR_TCH_FS_DATA_WIDTH_OCTETS];
  ALIGN16 uint8                     amr_voc_data[AMR_CHNLS_FS_DATA_WIDTH_OCTETS];
  ALIGN16 uint8                     amr_facch_data[AMR_TCH_FS_DATA_WIDTH_OCTETS];
#endif

  /*---------------------------------------------------------------------------
   * Various other globals used Full/Half Rate processing
   *-------------------------------------------------------------------------*/
  ARFCN_T                           amr_arfcns[4];
  uint16                            amr_data_length;
  int16                             amr_txlev[4];
  sys_algo_agc_T                    *amr_agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
  sys_algo_agc_T                    *amr_agc_ptr_divrx;
#endif
  boolean                           amr_block_start;
  boolean                           amr_prepare_msg;
  boolean                           amr_access_burst_scheduled;
  boolean                           amr_idle_status;
  boolean                           amr_sacch_ul_block_start;
  uint16                            amr_saved_c_over_i[4];
  boolean                           amr_cmc_rcvd;
  boolean                           amr_first_rx_block;
  boolean                           amr_cmc_changed;
  uint8                             amr_rx_phase;

#ifdef GL1_TCH_AMR_DEBUG
  uint8                             amr_no_data_cnt;
#endif

  /* sacch and sdcch can share the same rpt since they occur far apart */
  /*  decoded data goes here */
  cch_decode_rpt                    amr_sacch_rpt;
  tch_facch_decode_rpt              amr_tch_rpt;
  boolean                           amr_facch_not_available;
  boolean                           amr_vfr_aligned;
  gl1_msg_metrics_rpt               amr_sacch_metrics_rpts[4];
  gl1_msg_metrics_rpt               amr_tch_metrics_rpts[4];
  boolean                           amr_prev_block_was_facch;
  int8                              amr_prev_block_was_facch_counter;
  boolean                           amr_loopback_I_in_progress;
  cmi_phase_type                    amr_loopback_I_prev_phase;
  boolean                           amr_need_to_expire_ack_act_timer;

  boolean                           amr_enable_loopback_after_facch_block;
  uint16                            amr_facch_blk_count;
#ifdef FEATURE_GSM_AMR_WB
  boolean                           wb_amr_flag;
#endif
  /*---------------------------------------------------------------------------
   * Half Rate processing specific globals
   *-------------------------------------------------------------------------*/
  uint8                             amr_hr_subchannel;
  int                               amr_hr_facch_tx_in_progress;
  int                               amr_hr_ratscch_tx_in_progress;
  int                               amr_hr_sid_first_tx_in_progress;
  int                               amr_hr_sid_update_tx_in_progress;
  boolean                           amr_sacch_in_middle;
  boolean                           amr_middle_facch_block;
  boolean                           amr_facch_tx_allowed;
  boolean                           amr_facch_rx_expected;
#ifdef FEATURE_VAMOS_II
  boolean                           amr_middle_facch_block_vamos2;
#endif


  gl1_msg_rx_rpt                    amr_s_rpt;
  gl1_msg_rx_rpt                    amr_t_rpt;
  byte                              amr_current_TSC;

  boolean                           sacch_tx_started_fr;
  uint8                             amr_vfr_align_frame_fr;

  boolean                           sacch_tx_started_hr;
  uint8                             amr_vfr_align_frame_hr;

#ifdef TCH_PROFILING
  uint16                            crc_cnt_fr;
  uint16                            bfi_cnt_fr;
  uint16                            total_crc_cnt_fr;
  uint16                            total_bfi_cnt_fr;

  uint16                            crc_cnt_hr;
  uint16                            bfi_cnt_hr;
  uint16                            total_crc_cnt_hr;
  uint16                            total_bfi_cnt_hr;
#endif
  uint16                            crc_bad_fr;
  uint16                            total_crc_bad_fr;

  uint16                            crc_bad_hr;
  uint16                            total_crc_bad_hr;

  uint16                            bep[4];
  uint8                             bep_index;

  garb_request_tx_t                 amr_coex_request_tx;

  uint8                             amr_coex_denials_for_facch;
  uint8                             amr_coex_denials_for_ratscch;

} l1_tch_amr_data_t;

#ifdef FEATURE_VAMOS_II
static gl1_vamos2_tch_wait_type vamos2_tch_wait_state[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( NO_WAIT ) };
#endif 

static l1_tch_amr_data_t l1_tch_amr_data[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_GSM_AMR_WB
/*The last 3 values are just made up and for devlopment use - need to be refined!*/
static uint16 amr_tch_fr_thresholds[GL1_MSG_AMR_MODE_MAX] =
{22500, 26000, 27000, 27500, 27500, 27500, 28200, 29200, 20000, 20000, 20000};
#else
static uint16 amr_tch_fr_thresholds[8] =
{22500, 26000, 27000, 27500, 27500, 27500, 28200, 29200};
#endif

#ifdef FEATURE_GSM_AMR_WB
uint16 amr_homing_seq[GL1_MSG_AMR_MODE_MAX][17] =
#else
uint16 amr_homing_seq[8][17] =
#endif
{
#ifdef FEATURE_QDSP6_ADSP_MDSP_RADIO_FUSION
    { 0x9df8, 0xf167, 0x0916, 0xbd35, 0x99e1, 0x4068, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0xb91f, 0xf767, 0xfdf1, 0x47f5, 0x2ebf, 0xc061, 0x0060, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0x59e9, 0x5ff3, 0xe5df, 0x66e9, 0xfb7f, 0x88c0, 0x8081, 0x0088, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0xb9f0, 0xbfe6, 0x0068, 0x0972, 0xd3c3, 0xc8cb, 0x4808, 0x55a1, 0x008c,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0x71f8, 0x48cf, 0x1e80, 0x27c4, 0xfcf0, 0x733f, 0x8918, 0x2286, 0x2206,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0xb342, 0x0832, 0x1318, 0xe9d7, 0xe716, 0x5eaa, 0xd780, 0xe8fd, 0xb812,
        0x80c0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0xc71f, 0xc722, 0x0388, 0xa928, 0x80c2, 0x0b03, 0x75c9, 0x3e5c, 0x19f5,
        0x00f8, 0x2900, 0x2353, 0x00e0, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0x5508, 0x946d, 0x714c, 0xa0a1, 0xe781, 0xd2ea, 0x2404, 0x8044, 0x0e00,
        0x82cd, 0x11b8, 0x0018, 0x9700, 0x79c4, 0x774e, 0x0040, 0x0000}
#ifdef FEATURE_GSM_AMR_WB
    ,{ 0x3100, 0x3800, 0x109c, 0x0130, 0x07f2, 0xfa22, 0xeb89, 0x8adb, 0x00d0,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},       /*6.6kbps*/
    { 0x0044, 0x000f, 0x550a, 0x5df1, 0x0f22, 0xd794, 0xfa01, 0x85a4, 0x44a7,
        0xc546, 0xe5e6, 0x0080, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },      /*8.85kbps*/
    { 0x4650, 0x7700, 0xdeff, 0xf105, 0x675b, 0x8c8f, 0x70f7, 0xda07, 0xca82,
        0x5ad1, 0xde42, 0xc05a, 0xee44, 0x5ad3, 0x44e6, 0xd8d1, 0x0000 }       /*12.65kbps*/
#endif
#else
    { 0xF89D, 0x38CC, 0x0328, 0xF70F, 0xB182, 0x3D36, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0xF89D, 0x38CC, 0x03DF, 0xC062, 0xFB7F, 0x7F47, 0xBE00, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0xF871, 0x8BEF, 0x401E, 0xFE01, 0xCF60, 0x7CFB, 0xF803, 0xDC00, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0xF871, 0x8BEF, 0x4017, 0x01E2, 0x63E1, 0x60B8, 0xBC07, 0xB18E, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0xF871, 0x8BEF, 0x400D, 0xE036, 0x208F, 0xC4C1, 0xBA6F, 0x01B0, 0x0378,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0x6138, 0xC5F7, 0xA006, 0xFA07, 0x3C08, 0x7A5B, 0x1C69, 0xBC41, 0xCA68,
        0x3C82, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0xF871, 0x8BD1, 0x4000, 0x0000, 0x00DA, 0xE4C6, 0x77EA, 0x2C40, 0xAD6B,
        0x3D80, 0x6C17, 0xC855, 0xC300, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0x0854, 0xDB96, 0xAAAD, 0x6000, 0x0000, 0x001B, 0x587F, 0x6683, 0x7940,
        0x9004, 0x1585, 0x4F10, 0xF6B0, 0x2403, 0xC7EA, 0x0000, 0x0000}
#ifdef FEATURE_GSM_AMR_WB
    ,{ 0x18C1, 0xD40B, 0x90EB, 0xEF90, 0x080D, 0x203B, 0xD040, 0x2E94, 0x8000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0x18C1, 0xEEC5, 0x36BA, 0x3A37, 0x9DE4, 0x41A7, 0x8C08, 0x0085, 0x21B2,
        0x2E0D, 0x8364, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    { 0x18C1, 0xEEC5, 0x36BA, 0x3A81, 0xC7E7, 0xF07C, 0x696D, 0x1693, 0x80C0,
        0x7C0F, 0x8041, 0x21FB, 0xB3F7, 0xB20E, 0xEDA5, 0x6CF8, 0x0000}
#endif
#endif /*FEATURE_QDSP6_ADSP_MDSP_RADIO_FUSION*/
};

static uint16 amr_tch_hr_thresholds[6] =
{30800, 30900, 31000, 31000, 31000, 31000};

/* ========================================================================*
 *  EXTERNAL DATA
 * ========================================================================*/
extern uint8 ratscch_rsp[NUM_GERAN_DATA_SPACES][MAX_RATSCCH_OCTETS];
extern boolean ratscch_rsp_pending[];
extern gl1_msg_amr_mode_type curr_dl_acs[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES];
extern gl1_msg_amr_mode_type curr_ul_acs[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES];
extern uint8 curr_dl_icm[];
extern uint8 curr_ul_icm[];
extern uint8 new_ul_icm[];
extern uint8 curr_dl_num_acm[];
extern uint8 curr_ul_num_acm[];
extern uint8 curr_dl_cmi[];
extern cmi_phase_type curr_phase[];
extern uint8 curr_cmr[];
extern uint8 computed_cmr[];
extern uint8 curr_ul_cmi[];
extern uint8 curr_cmc[];
extern uint8 new_cmc[];
extern uint8 curr_thresh_tbl[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES - 1];
extern uint8 curr_hyst_tbl[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES - 1];
extern channel_type_T amr_channel[];
extern uint32 curr_norm_c_over_i[];
extern int32 prev_c_over_i_filter_output[];
extern boolean amr_thresh_hyst_defined[];

extern dedicated_data_T l1_dedicated_data[];

/* ========================================================================*
 * Local Functions
 * ========================================================================*/

/*---------------------------------------------------------------------------
 * All of the following are common to AMR Full and Half rate
 *-------------------------------------------------------------------------*/
static void l1_amr_store_cmc( uint8 rcvd_cmc, gas_id_t gas_id );
static boolean l1_amr_update_cmc( gas_id_t gas_id );
static void l1_amr_update_cmr( uint8 phase, boolean ratscch_sid_update_tx, gas_id_t gas_id );
static void amr_tch_tx_callback_fr_hr( gas_id_t gas_id );
static void amr_facch_tx_callback_fr_hr( gas_id_t gas_id );
static void amr_ratscch_tx_callback_fr_hr( gas_id_t gas_id );
static void amr_sid_first_tx_callback_fr_hr( gas_id_t gas_id );
static void amr_sid_update_tx_callback_fr_hr( gas_id_t gas_id );
static void  l1_amr_process_multirate_ie( dedicated_data_T  *ded_dtr_ptr, gas_id_t gas_id);
extern void l1_sc_set_tch_idle_frame( uint8 idle_frame, gas_id_t gas_id  );
extern boolean gl1_include_in_rxlev ( ARFCN_T arfcn, gas_id_t gas_id );
extern void l1_sc_check_if_next_cm_search_is_lte(gas_id_t gas_id);

static void l1_amr_recompute_sacch_arfcns_fr_hr
( dedicated_data_T  *ded_dtr_ptr, gas_id_t gas_id );

static boolean l1_amr_process_sacch_fr_hr
(dedicated_data_T  *ded_dtr_ptr, gas_id_t gas_id);

static void l1_amr_process_idle_fr_hr (gas_id_t gas_id);
static void  l1_amr_process_sce_pwr_data_fr_hr ( gas_id_t gas_id );
static void  l1_amr_start_ftn_fr_hr (gas_id_t gas_id);
static void l1_amr_schedule_monitors_fr_hr(gas_id_t gas_id);

#ifdef FEATURE_GSM_COEX_SW_CXM
static void l1_tch_amr_get_arfcns(int16 start, uint16 increment, uint16 num_arfcns, ARFCN_T arfcns[], gas_id_t gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

/*---------------------------------------------------------------------------
 * All of the following are AMR Full Rate specific
 *-------------------------------------------------------------------------*/
static void amr_tch_rx_callback_fr(gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
void amr_tch_metrics_callback_fr( gl1_msg_metrics_rpt *rpt[], uint16 chan, gas_id_t gas_id );
static void l1_amr_setup_time_slot_params_fr ( channel_information_T  *chnl1_info_ptr, gas_id_t gas_id );

#ifdef FEATURE_VAMOS_II
static void l1_amr_setup_time_slot_params_vamos2_fr
( channel_information_T  *chnl1_info_ptr, gas_id_t gas_id );
#endif /*FEATURE_VAMOS_II*/

static void l1_amr_setup_message_layer_fr
( dedicated_data_T       *ded_dtr_ptr,
  channel_information_T  *chnl1_info_ptr, gas_id_t gas_id );
static void l1_amr_process_rach_fr (dedicated_data_T *ded_dtr_ptr, gas_id_t gas_id);
static void l1_amr_process_tch_fr( dedicated_data_T      *ded_dtr_ptr,
                                   channel_information_T *chnl1_info_ptr, gas_id_t gas_id );
static void l1_amr_tch_abort_fr ( gas_id_t gas_id );
static void l1_amr_tx_facch_fr( gl1_msg_amr_frame_info_type frame_info,
                                dedicated_data_T *ded_dtr_ptr, gas_id_t gas_id );
static void l1_amr_tx_ratscch_fr( gl1_msg_amr_frame_info_type frame_info, gas_id_t gas_id );
static void l1_amr_tx_tch_fr( gl1_msg_amr_frame_info_type frame_info, gas_id_t gas_id );

void gl1_amr_setup_loopback_I( gas_id_t gas_id );
void gl1_amr_reset_loopback_I( gas_id_t gas_id );


#ifdef DED_LOG
#error code not present
#endif


/*---------------------------------------------------------------------------
 * All of the following are Half Full Rate specific
 *-------------------------------------------------------------------------*/
static void amr_tch_rx_callback_hr(gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
void amr_tch_metrics_callback_hr( gl1_msg_metrics_rpt *rpt[], uint16 chan, gas_id_t gas_id );
static void l1_amr_setup_time_slot_params_hr
( channel_information_T  *chnl1_info_ptr, gas_id_t gas_id );

#ifdef FEATURE_VAMOS_II
static void l1_amr_setup_time_slot_params_vamos2_hr
( channel_information_T  *chnl1_info_ptr, gas_id_t gas_id );
#endif /*FEATURE_VAMOS_II*/

static void l1_amr_setup_message_layer_hr
( dedicated_data_T       *ded_dtr_ptr,
  channel_information_T  *chnl1_info_ptr, gas_id_t gas_id );
static void l1_amr_process_rach_hr(dedicated_data_T *ded_dtr_ptr,
                                   channel_information_T  *chnl1_info_ptr, gas_id_t gas_id );
static void l1_amr_process_tch_hr( dedicated_data_T      *ded_dtr_ptr,
                                   channel_information_T *chnl1_info_ptr, gas_id_t gas_id );
static void  l1_amr_tch_abort_hr ( gas_id_t gas_id );
static void l1_amr_tx_facch_hr( gl1_msg_amr_frame_info_type frame_info,
                                uint8 subchannel,
                                dedicated_data_T *ded_dtr_ptr, gas_id_t gas_id);
static void l1_amr_tx_ratscch_hr( gl1_msg_amr_frame_info_type frame_info,
                                  uint8 subchannel, gas_id_t gas_id);
static void  l1_amr_tx_tch_hr( gl1_msg_amr_frame_info_type frame_info,
                               uint8 subchannel, gas_id_t gas_id );

extern void ratscch_expire_ack_activation_timer( gas_id_t gas_id );
void ratscch_set_ul_voc_amr_mode_using_new_icm( gas_id_t gas_id );

extern void gl1_store_block_rx_power ( gl1_msg_metrics_rpt *rpt[], uint8 bursts_per_block, gas_id_t gas_id );
extern void gl1_update_emr_parameters ( gl1_defs_rx_hdr_struct *rpt_hdr, boolean is_sid_fn, boolean compute_bep, gas_id_t gas_id );


/*===========================================================================

FUNCTION l1_tch_amr_init_data

DESCRIPTION
  This function initialiase the l1_tch_amr_data structures

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  initialiase l1_tch_amr_data

===========================================================================*/
void l1_tch_amr_init_data( gas_id_t gas_id )
{
  int j;

  // Initialiase s_rpt and t_rpt
  l1_tch_amr_data[gas_id].amr_s_rpt.data = (gl1_msg_decode_rpt *) &l1_tch_amr_data[gas_id].amr_sacch_rpt;
  for (j = 0; j < GL1_DEFS_FRAMES_IN_BLK_PERIOD; j++)
    l1_tch_amr_data[gas_id].amr_s_rpt.metrics[j] = &l1_tch_amr_data[gas_id].amr_sacch_metrics_rpts[j];

  l1_tch_amr_data[gas_id].amr_t_rpt.data = (gl1_msg_decode_rpt *) &l1_tch_amr_data[gas_id].amr_tch_rpt;
  for (j = 0; j < GL1_DEFS_FRAMES_IN_BLK_PERIOD; j++)
    l1_tch_amr_data[gas_id].amr_t_rpt.metrics[j] = &l1_tch_amr_data[gas_id].amr_tch_metrics_rpts[j];


  l1_tch_amr_data[gas_id].amr_curr_tx_state                      = AMR_INIT;
  l1_tch_amr_data[gas_id].amr_new_tx_state                       = AMR_INIT;
  l1_tch_amr_data[gas_id].amr_rx_state                           = AMR_INIT;
  l1_tch_amr_data[gas_id].amr_sid_update_pending                 = FALSE;
  l1_tch_amr_data[gas_id].amr_sid_first_pending                  = FALSE;
  l1_tch_amr_data[gas_id].amr_nsync_cnt                          = 0;
  l1_tch_amr_data[gas_id].amr_voc_type                           = GL1_MSG_FS;
  l1_tch_amr_data[gas_id].amr_cmc_rcvd                           = FALSE;
  l1_tch_amr_data[gas_id].amr_first_rx_block                     = TRUE;
  l1_tch_amr_data[gas_id].amr_cmc_changed                        = FALSE;
  l1_tch_amr_data[gas_id].amr_rx_phase                           = 0;
#ifdef GL1_TCH_AMR_DEBUG
  l1_tch_amr_data[gas_id].amr_no_data_cnt                        = 0;
#endif
  l1_tch_amr_data[gas_id].amr_vfr_aligned                        = FALSE;
  l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter       = 0;
  l1_tch_amr_data[gas_id].amr_loopback_I_in_progress             = FALSE;
  l1_tch_amr_data[gas_id].amr_loopback_I_prev_phase              = CMI_PHASE_ODD;
  l1_tch_amr_data[gas_id].amr_need_to_expire_ack_act_timer       = FALSE;
  l1_tch_amr_data[gas_id].amr_enable_loopback_after_facch_block  = FALSE;
  l1_tch_amr_data[gas_id].amr_facch_blk_count                    = 0;
#ifdef FEATURE_GSM_AMR_WB
  l1_tch_amr_data[gas_id].wb_amr_flag                            = FALSE;
#endif
  l1_tch_amr_data[gas_id].amr_hr_subchannel                      = 0;
  l1_tch_amr_data[gas_id].amr_hr_facch_tx_in_progress            = 0;
  l1_tch_amr_data[gas_id].amr_hr_ratscch_tx_in_progress          = 0;
  l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress        = 0;
  l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress       = 0;
  l1_tch_amr_data[gas_id].amr_sacch_in_middle                    = FALSE;
  l1_tch_amr_data[gas_id].amr_middle_facch_block                 = FALSE;
  l1_tch_amr_data[gas_id].amr_facch_tx_allowed                   = FALSE;
  l1_tch_amr_data[gas_id].amr_facch_rx_expected                  = FALSE;

  l1_tch_amr_data[gas_id].sacch_tx_started_fr                    = FALSE;
  l1_tch_amr_data[gas_id].sacch_tx_started_hr                    = FALSE;

#ifdef TCH_PROFILING
  l1_tch_amr_data[gas_id].crc_cnt_fr                             = 0;
  l1_tch_amr_data[gas_id].bfi_cnt_fr                             = 0;
  l1_tch_amr_data[gas_id].total_crc_cnt_fr                       = 0;
  l1_tch_amr_data[gas_id].total_bfi_cnt_fr                       = 0;

  l1_tch_amr_data[gas_id].crc_cnt_hr                             = 0;
  l1_tch_amr_data[gas_id].bfi_cnt_hr                             = 0;
  l1_tch_amr_data[gas_id].total_crc_cnt_hr                       = 0;
  l1_tch_amr_data[gas_id].total_bfi_cnt_hr                       = 0;
#endif
  l1_tch_amr_data[gas_id].crc_bad_fr                             = 0;
  l1_tch_amr_data[gas_id].total_crc_bad_fr                       = 0;

  l1_tch_amr_data[gas_id].crc_bad_hr                             = 0;
  l1_tch_amr_data[gas_id].total_crc_bad_hr                       = 0;

  l1_tch_amr_data[gas_id].bep_index                              = 0;

#ifdef FEATURE_GSM_COEX
  l1_tch_amr_data[gas_id].amr_coex_denials_for_facch             = 0;
  l1_tch_amr_data[gas_id].amr_coex_denials_for_ratscch           = 0;
#endif

}


/*===========================================================================
FUNCTION L1_CONTROL_AMR_FR

DESCRIPTION
   This function controls the AMR Full Rate Rraffic channel.

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
boolean  L1_control_amr_fr ( dedicated_data_T       *ded_dtr_ptr,
                             channel_information_T  *chnl1_info_ptr,
                             boolean                 apply_freq_redef, gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION
    static uint8 no_vocoder_change_after_handover;
#endif

    /*
     * Initialize global flags.
     */
    l1_tch_amr_data[gas_id].amr_block_start = FALSE;
    l1_tch_amr_data[gas_id].amr_prepare_msg = FALSE;
    l1_tch_amr_data[gas_id].amr_access_burst_scheduled = FALSE;
    l1_tch_amr_data[gas_id].amr_idle_status  = FALSE;
    l1_tch_amr_data[gas_id].amr_sacch_ul_block_start = FALSE;

    amr_channel[gas_id] = FULL_RATE_TRAFFIC;

    if( !ded_dtr_ptr || !chnl1_info_ptr )
    {
        MSG_GERAN_ERROR_2_G("Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
                  ded_dtr_ptr, chnl1_info_ptr);
        return FALSE;
    }

    /*
     *  agc pointer
     */
    l1_tch_amr_data[gas_id].amr_agc_ptr = &ded_dtr_ptr->c0_agc;
#ifdef FEATURE_GSM_RX_DIVERSITY
    l1_tch_amr_data[gas_id].amr_agc_ptr_divrx = &ded_dtr_ptr->agc_divrx;
#endif  

    switch( l1_tskisr_blk->sub_sub_state )
    {

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM

    case L1_EDTM_INIT:

#endif
#endif

    case L1_DED_INIT :
        l1_acch_nv_flag_initialise(gas_id);
        /*
         * Reset these global variables in this state. Once
         * intialized this variable will maintain it's
         * state as long as we are in RUNNING state.
         */
        l1_tch_amr_data[gas_id].amr_prev_block_was_facch = FALSE;
        l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter = 0;
        l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.valid = FALSE;
        l1_tch_amr_data[gas_id].amr_sid_first_pending = l1_tch_amr_data[gas_id].amr_sid_update_pending = FALSE;
        l1_tch_amr_data[gas_id].amr_cmc_rcvd = FALSE;
        l1_tch_amr_data[gas_id].amr_vfr_aligned = FALSE;
        l1_tch_amr_data[gas_id].amr_cmc_changed = FALSE;
        l1_tch_amr_data[gas_id].amr_first_rx_block = TRUE;
        l1_tch_amr_data[gas_id].amr_loopback_I_in_progress = FALSE;
        l1_tch_amr_data[gas_id].amr_need_to_expire_ack_act_timer = FALSE;
#ifdef GL1_TCH_AMR_DEBUG
        l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
#endif

#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION
        no_vocoder_change_after_handover = FALSE;
#endif
        /*
         * Re-initialize this variable to FALSE if we are in DED_INIT.
         * It's a static variable and maintains the previous state if
         * we are not in DED_INIT but in DED_RUNNING state.
         */
        l1_tch_amr_data[gas_id].sacch_tx_started_fr = FALSE;

        /* Save TSC for logging */
        l1_tch_amr_data[gas_id].amr_current_TSC = chnl1_info_ptr->TSC;

        /*
         * Convert channel_type and channel_mode to CHT parameters for
         * the DSP
         */
#ifdef FEATURE_GSM_AMR_WB
        /* Check for WB AMR codecs being used */
        if ((ded_dtr_ptr->channel_specification.channel_mode_1 == SPEECH_V5)
#ifdef FEATURE_VAMOS_II
            ||(ded_dtr_ptr->channel_specification.channel_mode_1 == SPEECH_V5_VAMOS2)
#endif
           )
        {
           l1_tch_amr_data[gas_id].amr_channel_type = GL1_MSG_TCH_WFS;
           l1_tch_amr_data[gas_id].wb_amr_flag = TRUE;
        }
        else
        {
           l1_tch_amr_data[gas_id].amr_channel_type = GL1_MSG_TCH_AFS;
           l1_tch_amr_data[gas_id].wb_amr_flag = FALSE;
        }

        MSG_GERAN_HIGH_1_G("Tch init: (WB=%d) AMR FR voice",l1_tch_amr_data[gas_id].wb_amr_flag);
#else
        {
           l1_tch_amr_data[gas_id].amr_channel_type = GL1_MSG_TCH_AFS;
           MSG_GERAN_HIGH_0_G("Tch init: AMR FR voice");
        }
#endif


#ifdef FEATURE_GSM_AMR_WB
        /* setup WB_AMR mode on UL & DL */
        gl1_msg_ul_set_wb_amr_mode(l1_tch_amr_data[gas_id].wb_amr_flag,gas_id);
        gl1_msg_dl_set_wb_amr_mode(l1_tch_amr_data[gas_id].wb_amr_flag,gas_id);
#endif
        l1_tch_amr_data[gas_id].amr_voc_type = GL1_MSG_AMR;

        l1_tch_amr_data[gas_id].amr_enable_loopback_after_facch_block = FALSE;

        /*
         * Initialize the AMR rx and tx state machines
         */
        l1_tch_amr_data[gas_id].amr_rx_state = l1_tch_amr_data[gas_id].amr_curr_tx_state = l1_tch_amr_data[gas_id].amr_new_tx_state = AMR_INIT;

        /*
         * Init the AMR phase to ODD
         */
        curr_phase[gas_id] = CMI_PHASE_ODD;

        /*
         * Initialize all the codec related variables
         */
        curr_cmc[gas_id] = new_cmc[gas_id] = curr_cmr[gas_id] = computed_cmr[gas_id] = curr_ul_cmi[gas_id] = curr_dl_cmi[gas_id] = 0;
        curr_norm_c_over_i[gas_id] = 0;
        prev_c_over_i_filter_output[gas_id] = 0;

        /*
         * Look at the multirate IE and set up the initial_code_mode
         * acs etc.
         */
        l1_amr_process_multirate_ie(ded_dtr_ptr, gas_id);

#ifdef FEATURE_VAMOS_II
        /* If RR indicates that UE has to operate is VAMOS 2 mode and if TSC set 2 is allocated,
         * the SACCH position is shifted as per 3GPP 45.002, section 7, Table 1a. 
         */
        if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3))
        {
          l1_amr_setup_time_slot_params_vamos2_fr(chnl1_info_ptr, gas_id);
        }
        else
#endif /*FEATURE_VAMOS_II*/
        {
          /*
           * Set up parameters that depend on the timeslot used
           */
          l1_amr_setup_time_slot_params_fr(chnl1_info_ptr, gas_id);
        }

#ifdef FEATURE_GSM_COEX_SW_CXM
        /* Notify the arbitrator of the channel assignment */
        garb_intf_notify_cs_assignment(
           (l1_tskisr_blk->l1_state == L1_DTM_MODE ? GL1_DEFS_TN_2 : GL1_DEFS_TN_0),
           chnl1_info_ptr->frequency_list,
           chnl1_info_ptr->subchannel,
           TRUE,
           TRUE,
           l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame+1,
           l1_tch_amr_data[gas_id].amr_idle_frame,
           l1_tch_amr_get_arfcns,
          &l1_tch_amr_data[gas_id].amr_coex_request_tx,
           l1_tskisr_blk->l1_state,
           gas_id
        );
#endif /* FEATURE_GSM_COEX_SW_CXM */


        /*
         * Pass idle frame to SCE -- for ncell scheduling
         */
        l1_sc_set_tch_idle_frame (l1_tch_amr_data[gas_id].amr_idle_frame, gas_id );

        /*
         * Setup message layer
         */
        l1_amr_setup_message_layer_fr(ded_dtr_ptr, chnl1_info_ptr, gas_id );

        /* Install callback to trigger serving cell measurement reporting. */
        l1_sci_monscan_set_next_period_ded_cb( L1_send_dedicated_report, gas_id );

        MSG_GERAN_MED_1_G("Tch msg init FNmod42432 = %d", frame_counters[gas_id].FNmod42432);

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
        gl1_msg_set_iq_state( TRUE, gas_id );
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES || FEATURE_GSM_LOG_IQ_SAMPLES */

        l1_amr_schedule_monitors_fr_hr(gas_id);

         if ( frame_counters[gas_id].FNmod13 >= 9  ||
              frame_counters[gas_id].FNmod13 <= 0 )
         {
             /* VFR will be aligned to frame 4 and 20000 of 13M count*/
             l1_tch_amr_data[gas_id].amr_vfr_align_frame_fr = 3;
         }
         else if ( frame_counters[gas_id].FNmod13 >= 1  &&
                   frame_counters[gas_id].FNmod13 <= 4 )
         {
             /* VFR will be aligned to frame 8 and 40000 of 13M count*/
             l1_tch_amr_data[gas_id].amr_vfr_align_frame_fr = 7;
         }
         else
         {
             /* VFR will be aligned to frame 0 and 0 of 13M count*/
             l1_tch_amr_data[gas_id].amr_vfr_align_frame_fr = 12;
         }

        if (( l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_STARTED ) ||
             ( l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_READY ))
         {
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION			 
            /*If no change in vocoder id, do not stop the vocoder and align VFR in immediate possible frame */
            if(l1_vs_change_in_voc(l1_tch_amr_data[gas_id].amr_channel_type,l1_tch_amr_data[gas_id].amr_voc_type, gas_id) )
            { 
               l1_vs_stop_client(gas_id);
            }
            else
            {
              no_vocoder_change_after_handover = TRUE;

              if(   frame_counters[gas_id].FNmod13 == 3   ||
                    frame_counters[gas_id].FNmod13 == 7   ||
                    frame_counters[gas_id].FNmod13 == 12   )
              {
                 gl1_msg_align_vfr( TRUE, gas_id );
                 l1_tch_amr_data[gas_id].amr_vfr_aligned = TRUE;
                 MSG_GERAN_MED_1_G("Aligning Real VFR FNmod13=%d",frame_counters[gas_id].FNmod13);
              }
              else if ( frame_counters[gas_id].FNmod13 >= 12  ||
                        frame_counters[gas_id].FNmod13 <= 2 )
              {
                 /* VFR will be aligned to frame 4 and 20000 of 13M count*/
                 l1_tch_amr_data[gas_id].amr_vfr_align_frame_fr = 3;
              }
              else if ( frame_counters[gas_id].FNmod13 >= 3  &&
                        frame_counters[gas_id].FNmod13 <= 6 )
              {
                 /* VFR will be aligned to frame 8 and 40000 of 13M count*/
                 l1_tch_amr_data[gas_id].amr_vfr_align_frame_fr = 7;
              }
              else
              {
                 /* VFR will be aligned to frame 0 and 0 of 13M count*/
                 l1_tch_amr_data[gas_id].amr_vfr_align_frame_fr = 12;
              }
            }
#else
           l1_vs_stop_client(gas_id);
#endif
         }

        l1_tskisr_blk->sub_sub_state  = L1_DED_RUNNING;

/* Exclude as PMIC signaling is only needed for Solo.
** Mustang uses RTR6285 and diff PMIC
*/


/* For Poseidon enable cipher clock for mDSP */
        if (gl1_msg_is_ciphering_enabled(gas_id))
        {
             MSG_GERAN_HIGH_0_G("DG: AFR: cipher required.");
            gl1_hw_cipher_clock_required(TRUE, gas_id);
        }

    /* End the Channel Init here so the DED_RUNNING is run on the next frame
    tick, otherwise this may result in possible ISR Overrun. */
        l1_amr_start_ftn_fr_hr(gas_id);

        break;

    case L1_DED_RUNNING:

        /*
         * If frequency redefinition occurs midway through a SACCH block,
         * we need to recompute the SACCH ARFCNs based on the new parameters
         * and inform the msg layer.  We don't do this for the TCH because
         * they are contiguous and this scenario is extremely unlikely.  We
         * need to recompute because the ARFCNs are pre-computed before the
         * block starts
         */
        if( apply_freq_redef )
        {
            l1_amr_recompute_sacch_arfcns_fr_hr( ded_dtr_ptr , gas_id);
        }

        /* Check up front to prevent Tx TCH from being scheduled in sacch - 1 */
        if( L1_more_abs_to_send(ded_dtr_ptr, FACCH_F, gas_id) )
        {
            l1_tch_amr_data[gas_id].amr_access_burst_scheduled = TRUE;
        }

        /*
         * SACCH Processing
         */
        if(  frame_counters[gas_id].FNmod26 == l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame )
        {
            if( l1_tskisr_blk->main_command != L1_DEDICATED_STOP )
            {
                if( ( frame_counters[gas_id].FNmod104 == l1_tch_amr_data[gas_id].amr_send_sacch) || ( !l1_tch_amr_data[gas_id].sacch_tx_started_fr ) )
                {
                    if(l1_amr_process_sacch_fr_hr(ded_dtr_ptr, gas_id) == TRUE )
                    {
                      /*
                          Once an incomplete or complete SACCH block is started,
                          sacch_tx_started is set to TRUE and is reset only
                          when we get onto another channel.  sacch_ul_block
                          start indicates the start of an incomplete or complete
                          block.  The old indicator was ticking the handler only
                          on a SACCH block boundary
                      */
                      l1_tch_amr_data[gas_id].amr_sacch_ul_block_start = TRUE;
                      l1_tch_amr_data[gas_id].sacch_tx_started_fr = TRUE;
                    }
                }
            }
        }
        else if( frame_counters[gas_id].FNmod26 != l1_tch_amr_data[gas_id].amr_idle_minus_one_frame )
        {
            /*
             * If it is not sacch - 1 frame and is not idle - 1 frame,
             * then see if have to process a RACH.
             */
            if( l1_tskisr_blk->main_command != L1_DEDICATED_STOP )
            {
                l1_amr_process_rach_fr(ded_dtr_ptr, gas_id);
            }



            /* If it is not sacch - 1 frame and is not idle - 1 frame, check
            * to see if it is an idle frame.
            */
            if( frame_counters[gas_id].FNmod26 == l1_tch_amr_data[gas_id].amr_idle_frame )
            {
                l1_amr_process_idle_fr_hr(gas_id);
            }

            /*
             * If it is not sacch - 1 frame and is not idle - 1 frame, check
             * to see if it is an idle - 1 - 1 frame.
             */
            if( frame_counters[gas_id].FNmod26 == (l1_tch_amr_data[gas_id].amr_idle_minus_one_frame - 1) )
            {
                l1_sci_tick_receive(gas_id);
            }

            /*
             * If it is not sacch - 1 frame and is not idle - 1 frame, check
             * to see if it is an idle - 1 - 3 frame.
             */
            if( frame_counters[gas_id].FNmod26 == ( l1_tch_amr_data[gas_id].amr_idle_minus_one_frame - 3) )
            {
                l1_amr_process_sce_pwr_data_fr_hr(gas_id);
            }
        }

        /* Check if the VFR needs aligning */
        if( frame_counters[gas_id].FNmod13 == l1_tch_amr_data[gas_id].amr_vfr_align_frame_fr &&
            !l1_tch_amr_data[gas_id].amr_vfr_aligned )
        {
          /* Check Vocoder is in the correct state before changing VFR */
          if( (l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_OPENED)
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION			  
			  || ( TRUE == no_vocoder_change_after_handover)
#endif
			)
          {
            gl1_msg_align_vfr( TRUE, gas_id );
            l1_tch_amr_data[gas_id].amr_vfr_aligned = TRUE;
            MSG_GERAN_MED_1_G("Aligning Real VFR FNmod13=%d",frame_counters[gas_id].FNmod13);
          }
          else
          {
          
            MSG_GERAN_MED_1_G("Vocoder is not stopped, aligning VFR postponed FNmod13=%d",
                                                                       frame_counters[gas_id].FNmod13);
            /* If vocoder was not stopped, determine when is the 
             * next oportunity to align the vocoder */
            switch (l1_tch_amr_data[gas_id].amr_vfr_align_frame_fr)
            {
              case 3:
                l1_tch_amr_data[gas_id].amr_vfr_align_frame_fr  = 7;
                break;

              case 7:
                l1_tch_amr_data[gas_id].amr_vfr_align_frame_fr  = 12;
                break;

              case 12:
                l1_tch_amr_data[gas_id].amr_vfr_align_frame_fr  = 3;
            }
          }
        }

        /* Check if vocoder needs to be enabled */
        if( l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_OPENED &&
            l1_tch_amr_data[gas_id].amr_vfr_aligned  )
        {
          MSG_GERAN_MED_1_G("Enabling vocoder FNmod13=%d",frame_counters[gas_id].FNmod13);

          l1_vs_start_client(l1_tch_amr_data[gas_id].amr_channel_type,
                             l1_tch_amr_data[gas_id].amr_voc_type, gas_id);
        }

        /*
         * Now process the traffic frame.
         */
        l1_amr_process_tch_fr( ded_dtr_ptr, chnl1_info_ptr, gas_id );


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
        if( frame_counters[gas_id].FNmod104 == l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 - 1 )
        {
            l1_sci_monscan_next_period_ded(gas_id);
        }
        /*
         *   TT / AFC correction
         */
        if( frame_counters[gas_id].FNmod104 == l1_tch_amr_data[gas_id].amr_afc_correction_frame )
        {
            gl1_msg_dch_tt_adjust(gas_id);
        }

        /* Don't abort till the extra TCH block to complete the FACCH has been
         * sent out
         */
        if( l1_tskisr_blk->main_command == L1_DEDICATED_STOP )
        {
            l1_dedicated_data_ptr->no_of_ab_to_send = 0;

            if( !l1_tch_amr_data[gas_id].amr_prev_block_was_facch )
            {
               /* Abort after FTN will save us a frame during channel changes
                * since we can abort as soon as NPL1 is done aborting.
                * Important from an audio perspective
                */
                l1_amr_tch_abort_fr(gas_id);
            }
        }

         /* If amr_idle_status flag is set to ture then TCH will be aborted. While running Tch abort no need to process FTN*/
         if( l1_tch_amr_data[gas_id].amr_idle_status == FALSE )
         {
           /* FTN processing  */
           l1_amr_start_ftn_fr_hr(gas_id);
         }

        break;

    default:
        break;
    }  /* of switch */


    return(l1_tch_amr_data[gas_id].amr_idle_status);

}  /* end of function */



/*===========================================================================
FUNCTION l1_amr_setup_time_slot_params_fr

DESCRIPTION
   This function sets up the time slot specific parameters for AMR Full Rate
   traffic channel.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_setup_time_slot_params_fr ( channel_information_T  *chnl1_info_ptr,
                                               gas_id_t gas_id )
{
    if(!chnl1_info_ptr )
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL chnl1_info_ptr");
        return;
    }

    switch( (chnl1_info_ptr->TN) & 7 )
    {
    case 0:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 11;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 24;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 25;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 103;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 11;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 89;
        break;

    case 1:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 24;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 11;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 12;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 12;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 24;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 102;
        break;

    case 2:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 11;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 24;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 25;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 25;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 37;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 89;
        break;

    case 3:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 24;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 11;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 12;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 38;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 50;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 102;
        break;

    case 4:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 11;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 24;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 25;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 51;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 63;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 89;
        break;

    case 5:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 24;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 11;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 12;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 64;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 76;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 102;
        break;

    case 6:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 11;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 24;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 25;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 77;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 89;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 89;
        break;

    case 7:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 24;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 11;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 12;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 90;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 102;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 102;
        break;

    default:
        break;
    }
	L1_set_reporting_period(l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1, 104, gas_id);
}


#ifdef FEATURE_VAMOS_II
/*===========================================================================
FUNCTION l1_amr_setup_time_slot_params_vamos2_fr

DESCRIPTION
   This function sets up the time slot specific parameters for AMR Full Rate
   traffic channel.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_setup_time_slot_params_vamos2_fr ( channel_information_T  *chnl1_info_ptr, gas_id_t gas_id )
{
    if(!chnl1_info_ptr )
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL chnl1_info_ptr");
        return;
    }

    switch( (chnl1_info_ptr->TN) & 7 )
    {
    case 0:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 12;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 24;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 25;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 103;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 12;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 89;
        break;

    case 1:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 23;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 11;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 12;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 12;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 23;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 102;
        break;

    case 2:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 12;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 24;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 25;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 25;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 38;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 89;
        break;

    case 3:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 23;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 11;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 12;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 38;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 49;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 102;
        break;

    case 4:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 12;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 24;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 25;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 51;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 64;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 89;
        break;

    case 5:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 23;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 11;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 12;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 64;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 75;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 102;
        break;

    case 6:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 12;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 24;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 25;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 77;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 90;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 89;
        break;

    case 7:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame        = 23;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame         = 11;
        l1_tch_amr_data[gas_id].amr_idle_frame                   = 12;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 = 90;
        l1_tch_amr_data[gas_id].amr_send_sacch                   = 101;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame         = 102;
        break;

    default:
        break;
    }
}
#endif /*FEATURE_VAMOS_II*/


/*===========================================================================
FUNCTION l1_amr_setup_message_layer_fr

DESCRIPTION
   This function sets up the Message layer for AMR Full Rate.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_setup_message_layer_fr ( dedicated_data_T       *ded_dtr_ptr,
                                            channel_information_T  *chnl1_info_ptr, gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    gl1_msg_tx_cb_type     tx_cb;
    gl1_msg_rx_cb_type     rx_cb;

    if( !ded_dtr_ptr || !chnl1_info_ptr)
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
    gl1_msg_init_tch_amr((gl1_msg_chan_type)l1_tch_amr_data[gas_id].amr_channel_type, l1_tch_amr_data[gas_id].amr_voc_type,gas_id);

    tx_cb.data_cb    = amr_tch_tx_callback_fr_hr;
    tx_cb.metrics_cb = NULL;
    tx_cb.log_cb     = NULL;
    tx_cb.abort_cb   = NULL;
    rx_cb.metrics_cb = NULL;
    rx_cb.data_cb    = amr_tch_rx_callback_fr;
    rx_cb.tch_metrics_cb = amr_tch_metrics_callback_fr;
#ifdef DED_LOG
    #error code not present
#else
    rx_cb.log_cb     = NULL;
#endif
    rx_cb.abort_cb   = NULL;
    gl1_msg_cfg_ul_tch_amr_fr_hr(chnl1_info_ptr->TSC, &tx_cb,gas_id );

    /*
       FACCH callback...
    */
    tx_cb.data_cb    = amr_facch_tx_callback_fr_hr;
    gl1_msg_cfg_ul_facch_amr_fr_hr(chnl1_info_ptr->TSC, &tx_cb,gas_id );
    gl1_msg_cfg_dl_tch_facch_amr_fr_hr(chnl1_info_ptr->TSC, &rx_cb,gas_id );

    /*
       RATSCCH callback...
    */
    tx_cb.data_cb    = amr_ratscch_tx_callback_fr_hr;
    gl1_msg_cfg_ul_ratscch_amr_fr_hr(chnl1_info_ptr->TSC, &tx_cb,gas_id );

    /*
       SID_FIRST callback...
    */
    tx_cb.data_cb    = amr_sid_first_tx_callback_fr_hr;
    gl1_msg_cfg_ul_sid_first_amr_fr_hr(chnl1_info_ptr->TSC, &tx_cb ,gas_id);

    /*
       SID_UPDATE callback...
    */
    tx_cb.data_cb    = amr_sid_update_tx_callback_fr_hr;
    gl1_msg_cfg_ul_sid_update_amr_fr_hr(chnl1_info_ptr->TSC, &tx_cb ,gas_id);

    /*
       SACCH callback....
    */
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

    gl1_msg_cfg_ul_sacch(chnl1_info_ptr->TSC, &tx_cb, gas_id);
    gl1_msg_cfg_dl_sacch(chnl1_info_ptr->TSC, &rx_cb, gas_id);

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

    gl1_msg_dch_afc_tt_init( chnl1_info_ptr->frequency_list.channel[0] ,gas_id);

    /* Precompute TXLEVS for first reporting period.  Transmit at the
        command power level immediately as soon as we jump onto the new
        channel. ms_power_level_to_use is initially set to ms_power_level_in_use
        ,so L1_precompute_txlev will not step up/step down the power
        The 2dB step algo kicks in only after the next power
        level change commanded by the base station
    */

    L1_precompute_txlev ( ded_dtr_ptr->ms_power_level_to_use,
                          ded_dtr_ptr->ms_power_level_in_use,
                          104,
                          chnl1_info_ptr->frequency_list.channel[0],
                          gas_id );
}

/*===========================================================================
FUNCTION l1_amr_schedule_monitors_fr_hr

DESCRIPTION
   This function schedules the monitors for both AMR Full and Half Rate.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_schedule_monitors_fr_hr(gas_id_t gas_id)
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    /*
     * When we first get onto the traffic channel
     * we schedule only as many monitors
     * as the number of frames upto the next "idle - 1"
     * starting with the current frame.  Otherwise we
     * schedule 24 monitors starting at "idle + 1".  If
     * we're on the "idle - 1" or "idle" frame wait till
     * the idle frame and schedule 24 monitors
     */

    if( (frame_counters[gas_id].FNmod26 != l1_tch_amr_data[gas_id].amr_idle_minus_one_frame - 1) &&
        (frame_counters[gas_id].FNmod26 != l1_tch_amr_data[gas_id].amr_idle_minus_one_frame) &&
        (frame_counters[gas_id].FNmod26 != l1_tch_amr_data[gas_id].amr_idle_frame) )
    {
        /* Consider both cases - when idle is at the end of the 26
   multiframe and when its in the middle
        */
        if( frame_counters[gas_id].FNmod26 < l1_tch_amr_data[gas_id].amr_idle_minus_one_frame )
        {
            /* # of monitors = # of frames upto and not including idle - 1
                Account for the fact that the power measure handler only starts
                a frame later (when we get into DED_RUNNING)
            */
            l1_tch_amr_data[gas_id].amr_num_pwr_meas_to_schedule = (l1_tch_amr_data[gas_id].amr_idle_minus_one_frame -
                                                                    frame_counters[gas_id].FNmod26 - 2);
        }
        else
        {
            /* Since we've excluded "idle - 1" and "idle", this condition
                applies only to the case when the idle falls in the middle
                of the 26 multiframe
            */
            l1_tch_amr_data[gas_id].amr_num_pwr_meas_to_schedule = L1_SC_DED_MAX_MEAS_PER_MF - (frame_counters[gas_id].FNmod26 -
                                                                         l1_tch_amr_data[gas_id].amr_idle_frame) - 1;
        }

        /* This will handle 0 and negative # of measurements if any */
        if( l1_tch_amr_data[gas_id].amr_num_pwr_meas_to_schedule )
        {
            /* Schedule monitors immediately if we've just got onto the TCH */
            l1_tch_amr_data[gas_id].amr_num_pwr_meas_per_frame = 1;

            /* Don't schedule monitors if we're aborting */
            if( l1_tskisr_blk->main_command != L1_DEDICATED_STOP )
            {
                l1_sci_monscan_do_ded(l1_tch_amr_data[gas_id].amr_num_pwr_meas_to_schedule,
                                      l1_tch_amr_data[gas_id].amr_num_pwr_meas_per_frame, TRUE, gas_id);
            }
        }
    }
}

/*===========================================================================
FUNCTION l1_amr_recompute_sacch_arfcns_fr_hr

DESCRIPTION
   This function recomputes the sacch ARFCNS for the AMR Full and Half rate.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_recompute_sacch_arfcns_fr_hr( dedicated_data_T  *ded_dtr_ptr, gas_id_t gas_id )
{
    int16 offset;

    if(!ded_dtr_ptr)
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL ded_dtr_ptr");
        return;
    }

    /* Determine when the SACCH block started relative to curr FN */
    offset = ( l1_tch_amr_data[gas_id].amr_send_sacch + 1 - frame_counters[gas_id].FNmod104 - 104 ) % 104;

    /* Recompute ARFCNs */
    L1_get_ARFCNS
    ( offset, 26, 4, l1_tch_amr_data[gas_id].amr_arfcns ,
      &ded_dtr_ptr->channel_specification.channel_info_1_before,
      &ded_dtr_ptr->channel_specification.channel_info_1_after,
      &ded_dtr_ptr->new_channel_info, gas_id);

    /* Update msg layer */
    gl1_msg_update_sacch_arfcns( l1_tch_amr_data[gas_id].amr_arfcns, gas_id );
}


/*===========================================================================
FUNCTION l1_amr_process_sacch_fr_hr

DESCRIPTION
   This function processes the AMR Full and Half Rate SACCH channel.

DEPENDENCIES
   None

RETURN VALUE
   amr_sacch_tx True if Sacch Tx is scheduled, false otherwise

SIDE EFFECTS
   None
===========================================================================*/
static boolean l1_amr_process_sacch_fr_hr (dedicated_data_T  *ded_dtr_ptr, gas_id_t gas_id)
{
    boolean amr_sacch_tx = FALSE;
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    if(!ded_dtr_ptr)
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL ded_dtr_ptr");
        return(FALSE);
    }

    /*
        Abort SACCH if there was a previous block in process.
        This would happen right after we get onto a new channel
        and have to transmit incomplete SACCH bursts till the
        first complete SACCH block occurs
    */
    gl1_msg_abort_tx_sacch(gas_id);

    (void) L1_get_CCH_message ( TRUE, SACCH, ded_dtr_ptr,
                                l1_tch_amr_data[gas_id].amr_tx_data, &l1_tch_amr_data[gas_id].amr_data_length, gas_id);

    /* Get the next block of ARFCN's and signal strengths.
        Passing in before and after freq parameters to L1_get_ARFCNS
        to guarantee a seamless transition between the before and
        after channels in the case of the start time lying in the
        middle of a block.  Not very elegant but the least complicated
        solution for now
    */
    L1_get_ARFCNS ( 1, 26, 4, l1_tch_amr_data[gas_id].amr_arfcns,
                    &ded_dtr_ptr->channel_specification.channel_info_1_before,
                    &ded_dtr_ptr->channel_specification.channel_info_1_after,
                    &ded_dtr_ptr->new_channel_info, gas_id);

    /*
       The start offset here is relative to the start of the reporting period -
       The first SACCH block is 13 frames from the start of the reporting period
    */

    /* Convert TXLEV to power index before passing to the msg layer */
    L1_get_txlev ( 12, 26, l1_tch_amr_data[gas_id].amr_txlev, gas_id );

    /* Update ded data ptr with most recent power level*/
    ded_dtr_ptr->ms_power_level_in_use = (uint8)l1_tch_amr_data[gas_id].amr_txlev[3];

    /* Don't send SACCH if L2 has asked us to stop meas rpt or we are currently
     * scheduling Handover access bursts since we have no TA
     */
    if( (ded_dtr_ptr->stop_meas_rpt == FALSE) &&
        (l1_tch_amr_data[gas_id].amr_access_burst_scheduled == FALSE) )
    {
        gl1_msg_tx_sacch ( l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                     l1_tch_amr_data[gas_id].amr_tx_data, gas_id );

        /* Log tx burst metrics */
        l1_log_transmit_burst_metrics
        ( GL1_MSG_SACCH,
          1, 26, 4,
          l1_tch_amr_data[gas_id].amr_arfcns,
          l1_tch_amr_data[gas_id].amr_txlev,
          l1_tch_amr_data[gas_id].amr_current_TSC,
          ded_dtr_ptr->timing_advance_in_use, gas_id );

        amr_sacch_tx = TRUE;
    }

    /* Receive only complete SACCH blocks */
    if( frame_counters[gas_id].FNmod104 == l1_tch_amr_data[gas_id].amr_send_sacch )
    {
        gl1_msg_rx_sacch ( l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                           l1_tch_amr_data[gas_id].amr_agc_ptr_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/  
                           &l1_tch_amr_data[gas_id].amr_s_rpt, l1_dedicated_data_ptr->rsacch_ctrl.active,
                           gas_id );

    }

    /* return sacch Tx schedule status */
    return (amr_sacch_tx);
}

/*===========================================================================
FUNCTION l1_amr_process_rach_fr

DESCRIPTION
   This function process the AMR Full Rate RACH channel

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_process_rach_fr (dedicated_data_T *ded_dtr_ptr, gas_id_t gas_id)
{
    int i = 0;
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    if(!ded_dtr_ptr)
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL ded_dtr_ptr");
        return;
    }

    /*
       If there are handover access bursts to be transmitted,
       transmit them in consecutive frames on the FACCH
       (we exclude SACCH and idle frames)
    */
    if( l1_tch_amr_data[gas_id].amr_access_burst_scheduled )
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
        L1_get_ARFCNS ( 1, 1, 4, l1_tch_amr_data[gas_id].amr_arfcns,
                        &ded_dtr_ptr->channel_specification.channel_info_1_before,
                        &ded_dtr_ptr->channel_specification.channel_info_1_after,
                        &ded_dtr_ptr->new_channel_info, gas_id);
        /*
          Transmit handover access bursts. Single burst, so just
          pass in the first ARFCN in the block
        */
        gl1_msg_tx_rach(  l1_tch_amr_data[gas_id].amr_arfcns[0],
                          l1_dedicated_data_ptr->ms_power_level_to_use,
                          0,
                          l1_dedicated_data_ptr->access_burst_content,
                          gas_id
                       );

        for( i = 0; i < 4; i++ )
        {
            l1_tch_amr_data[gas_id].amr_txlev[i] = l1_dedicated_data_ptr->ms_power_level_to_use;
        }

        /* Log tx burst metrics */
        l1_log_transmit_burst_metrics
        ( GL1_MSG_RACH,
          1, 1, 1,
          l1_tch_amr_data[gas_id].amr_arfcns,
          l1_tch_amr_data[gas_id].amr_txlev,
          l1_tch_amr_data[gas_id].amr_current_TSC,
          0, gas_id );

        l1_tch_amr_data[gas_id].amr_access_burst_scheduled = TRUE;
    }
}

/*===========================================================================
FUNCTION l1_amr_process_idle_fr_hr

DESCRIPTION
   This function perform the AMR Full Rate idle frame processing.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_process_idle_fr_hr (gas_id_t gas_id)
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    /* Dedicated mode surround cell processing */
    /* SC monitors(power measurements) are to be done in every
        frame except "idle - 1" and "idle".  Thus at every
        idle, we schedule 24 power measurements
    */
    l1_tch_amr_data[gas_id].amr_num_pwr_meas_to_schedule = L1_SC_DED_MAX_MEAS_PER_MF;
    l1_tch_amr_data[gas_id].amr_num_pwr_meas_per_frame = 1;

#ifdef FEATURE_GSM_WTR_HOP
    if (gl1_msg_wtr_hop_get_status() == WTR_HOP_INACTIVE)
#endif
    {
        gl1_hw_rf_idle_frame_processing(gas_id);
    }

    /* Don't schedule monitors if we're aborting */
    if( l1_tskisr_blk->main_command != L1_DEDICATED_STOP )
    {
        l1_sci_monscan_do_ded(l1_tch_amr_data[gas_id].amr_num_pwr_meas_to_schedule,
                              l1_tch_amr_data[gas_id].amr_num_pwr_meas_per_frame, TRUE, gas_id);
    }
}

/*===========================================================================
FUNCTION l1_amr_process_sce_pwr_data_fr_hr

DESCRIPTION
   This function process the AMR Full and Half Rate Surround Cell Power Data

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void  l1_amr_process_sce_pwr_data_fr_hr ( gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    /*Check if G2L search can happen on next idle frame*/
    if( ( frame_counters[gas_id].FNmod26 == (l1_tch_amr_data[gas_id].amr_idle_minus_one_frame - 4) ) &&
        ( l1_tskisr_blk->main_command != L1_DEDICATED_STOP ) &&
        !l1_tch_amr_data[gas_id].amr_access_burst_scheduled )
    {

        l1_sc_check_if_next_cm_search_is_lte(gas_id);
    }

    /*
       Start in idle - 4 because this runs in the task
       and has to complete before idle - 2 when we tick
       the Ncell handler

       If we're stopping TCH or have access burst scheduled, son't schedule
       surround cell decodes
    */
    if( ( frame_counters[gas_id].FNmod26 == (l1_tch_amr_data[gas_id].amr_idle_minus_one_frame - 3) ) &&
        ( l1_tskisr_blk->main_command != L1_DEDICATED_STOP ) &&
        !l1_tch_amr_data[gas_id].amr_access_burst_scheduled )
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
FUNCTION l1_amr_process_tch_fr

DESCRIPTION
   The function processes the AMR Full Rate Traffic Channel. It implements
   the TX side state machine to resolve conflict between FACCH, RATSCCH and
   TCH frames giving priority to FACCH, RATSCCH and TCH is decreasing order.
   This function is also responsible for scheduling the frames.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_process_tch_fr( dedicated_data_T      *ded_dtr_ptr,
                                   channel_information_T *chnl1_info_ptr, gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    if ( !ded_dtr_ptr || !chnl1_info_ptr )
    {
        MSG_GERAN_ERROR_2_G( "Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
                   ded_dtr_ptr, chnl1_info_ptr);

        return;
    }

#ifdef FEATURE_VAMOS_II
    MSG_GERAN_HIGH_3_G("FN:%d,TSC:%d,amr_sacch_minus_one_frame:%d",
                        gl1_get_FN(gas_id),
                        chnl1_info_ptr->TSC,
                        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame);

    if((gl1_hw_get_vamos2_support(gas_id)) &&
       ((chnl1_info_ptr->TSC & 0x8) >> 3) && (l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame == 12))
       {
        if((frame_counters[gas_id].FNmod13 == 3)||
           (frame_counters[gas_id].FNmod13 == 7)||
           (frame_counters[gas_id].FNmod26 == 11) ||
           (frame_counters[gas_id].FNmod26 == 25))
          {
           l1_tch_amr_data[gas_id].amr_block_start = TRUE;
           l1_tch_amr_data[gas_id].amr_prepare_msg = TRUE;
          }
       }
    else
#endif /*FEATURE_VAMOS_II*/
      {
       if( ( frame_counters[gas_id].FNmod13 == 3 ) ||
           ( frame_counters[gas_id].FNmod13 == 7 ) ||
           ( frame_counters[gas_id].FNmod13 == 12 ) )
        {
         l1_tch_amr_data[gas_id].amr_block_start = TRUE;
         l1_tch_amr_data[gas_id].amr_prepare_msg = TRUE;
        }
      }

#ifdef FEATURE_VAMOS_II
   vamos2_tch_wait_state[gas_id] = NO_WAIT;

   if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3))
   {
     if((frame_counters[gas_id].FNmod26 == 11) && (l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame == 12))
     {
       vamos2_tch_wait_state[gas_id] = WAIT_AFTER_BLOCK_START;
     }   
     if((frame_counters[gas_id].FNmod26 == 20) && (l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame == 23))
     {
       vamos2_tch_wait_state[gas_id] = WAIT_AFTER_BURST2;
     }
   }
#endif


    /*
     * Increament the frame count. This frame count is used to see how
     * quickly the vocoder applies the new codec mode.
     */
    if(l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.valid)
    {
        l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.frame_count++;
    }

    /*
     * Ordered TA and TXLEV is applied only at the start of the next reporting
     * period. We precompute TXLEV for the entire reporting period and then
     * retrieve it later for each transmitted block.  We do this in
     * reporting_period - 3 because the block is prepared and scheduled a few
     * frames before its actually scheduled to start
     */
    if( frame_counters[gas_id].FNmod104 == l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 - 2)
    {
        /*
         * Update the power level and TA to use with that commanded by the base
         * station
         */
        ded_dtr_ptr->ms_power_level_to_use =
        ded_dtr_ptr->ordered_ms_power_level;
        ded_dtr_ptr->timing_advance_to_use =
        ded_dtr_ptr->ordered_timing_advance;

        /*
         * We need to report the power level of the last burst on the previous
         * SACCH multiframe.   100 would be the start of the last block.
         * 103 would be the idle frame but the specs say we have to consider
         * frames whether actual transmission takes place or not
         */
        L1_get_txlev( 100, 1, l1_tch_amr_data[gas_id].amr_txlev, gas_id );

        ded_dtr_ptr->ms_power_level_to_report = (uint8)l1_tch_amr_data[gas_id].amr_txlev[3];

        L1_precompute_txlev ( ded_dtr_ptr->ordered_ms_power_level,
                              ded_dtr_ptr->ms_power_level_in_use,
                              104,
                              chnl1_info_ptr->frequency_list.channel[0],
                              gas_id );

        ded_dtr_ptr->timing_advance_in_use =
          L1_change_ta( ded_dtr_ptr->timing_advance_to_use,
                        ded_dtr_ptr->timing_advance_in_use ,gas_id);
    }
    /*
     * amr_rx_phase is set to 1, indicating that the next
     * speech frame that the MDSP decodes will have
     * codec_mode_indication.
     */
    if( (frame_counters[gas_id].FNmod26 == 7)  ||
        (frame_counters[gas_id].FNmod26 == 16) ||
        (frame_counters[gas_id].FNmod26 == 24) )
    {
       if( curr_phase[gas_id] == CMI_PHASE_ODD )
       {
          l1_tch_amr_data[gas_id].amr_rx_phase = 1;
       }
       else
       {
          l1_tch_amr_data[gas_id].amr_rx_phase = 0;
       }
    }

    if( (frame_counters[gas_id].FNmod26 == 3)  ||
        (frame_counters[gas_id].FNmod26 == 11) ||
        (frame_counters[gas_id].FNmod26 == 20)  )
    {
       if( curr_phase[gas_id] == CMI_PHASE_EVEN )
       {
          l1_tch_amr_data[gas_id].amr_rx_phase = 1;
       }
       else
       {
          l1_tch_amr_data[gas_id].amr_rx_phase = 0;
       }
    }

    if( l1_tch_amr_data[gas_id].amr_prepare_msg )
    {
        boolean    sending_FACCH  = FALSE;

        /*
         * If previous block was FACCH, we need to send one more TCH block to
         * ensure that the interleaver has 8 bursts, so override DEDICATED_STOP
         * till the "completing" TCH block has been sent out
         */
        if(  ( l1_tskisr_blk->main_command != L1_DEDICATED_STOP)  ||
             l1_tch_amr_data[gas_id].amr_prev_block_was_facch )
        {
            int16 offset = 1;
            int16 FNrptperiod = 0;
            gl1_msg_amr_frame_info_type frame;
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
            uint16 i;
#endif
            boolean tx_success = TRUE;

            /* Did we break the loop to send FACCH in last block? If so re-enable
             * loopback */
            if(l1_tch_amr_data[gas_id].amr_enable_loopback_after_facch_block == TRUE)
            {

               /* If we broke loopback to send a FACCH not related to Opening loopback
                * C, resume loopback
                */
               if(ded_dtr_ptr->l1_loopback_disabled == LOOPBACK_C_TEMP_OPEN)
               {
                 ded_dtr_ptr->l1_loopback_disabled = LOOPBACK_C_CLOSED;
                 gl1_msg_tch_loopback(GL1_DEFS_LOOPBACK_TYPE_C, gas_id);
               }
               l1_tch_amr_data[gas_id].amr_facch_blk_count = 0;
            }

            /*
             * if there is new FACCH data or if it is a
             * SIGNALLING only channel then send DSP a tx facch command
             */
#ifdef FEATURE_GSM_COEX
            if( L1_get_CCH_message( FALSE,
                                    FACCH_F,
                                    ded_dtr_ptr,
                                    (l1_tch_amr_data[gas_id].amr_facch_data + 2),
                                    &l1_tch_amr_data[gas_id].amr_data_length, gas_id ) == FALSE)
            {
              if( (l1_tch_amr_data[gas_id].amr_coex_denials_for_facch <= MAX_AMR_COEX_DENIALS_FOR_FACCH) &&
                  (l1_tch_amr_data[gas_id].amr_coex_request_tx != NULL) &&
                  (l1_tch_amr_data[gas_id].amr_coex_request_tx(GL1_MSG_FACCH_AFS, gas_id) == FALSE) )
              {
                L1_get_CCH_filler_message(FACCH_F, l1_tch_amr_data[gas_id].amr_tx_data, &l1_tch_amr_data[gas_id].amr_data_length, gas_id);

                l1_tch_amr_data[gas_id].amr_facch_not_available = TRUE;

                l1_tch_amr_data[gas_id].amr_coex_denials_for_facch++;
              }
              else
              {
                L1_free_CCH_message(FACCH_F, ded_dtr_ptr, gas_id);

                l1_tch_amr_data[gas_id].amr_facch_not_available = FALSE;

                l1_tch_amr_data[gas_id].amr_coex_denials_for_facch = 0;

                sending_FACCH = TRUE;
              }
            }
			else
			{
			  l1_tch_amr_data[gas_id].amr_facch_not_available = TRUE;
			}
#else
            /* Is there a facch to transmit */
            l1_tch_amr_data[gas_id].amr_facch_not_available =
            L1_get_CCH_message( FALSE,
                                FACCH_F,
                                ded_dtr_ptr,
                                (l1_tch_amr_data[gas_id].amr_facch_data + 2),
                                &l1_tch_amr_data[gas_id].amr_data_length, gas_id );

            if( l1_tch_amr_data[gas_id].amr_facch_not_available == FALSE )
            {
              sending_FACCH = TRUE;
            }
#endif

            /* Get the next block of ARFCN's and signal strengths */

            /* Passing in before and after freq parameters to L1_get_ARFCNS
                to guarantee a seamless transition between the before and
                after channels in the case of the start time lying in the
                middle of a block.  Not very elegant but the least complicated
                solution for now
            */
            L1_get_ARFCNS
            ( offset,
              1,
              4,
              l1_tch_amr_data[gas_id].amr_arfcns ,
              &ded_dtr_ptr->channel_specification.channel_info_1_before,
              &ded_dtr_ptr->channel_specification.channel_info_1_after,
              &ded_dtr_ptr->new_channel_info, gas_id );

#ifdef FEATURE_VAMOS_II
            if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3))
			  {
			  if(vamos2_tch_wait_state[gas_id] == WAIT_AFTER_BLOCK_START)
			  	{	
			  	offset = 3;
				L1_get_ARFCNS
				( offset,
				  1,
				  3,
				  l1_tch_amr_data[gas_id].amr_arfcns + 1 ,
				  &ded_dtr_ptr->channel_specification.channel_info_1_before,
				  &ded_dtr_ptr->channel_specification.channel_info_1_after,
				  &ded_dtr_ptr->new_channel_info, gas_id );
			  	}
			  else if(vamos2_tch_wait_state[gas_id] == WAIT_AFTER_BURST2)
			  	{			  	
			  	offset = 5;
				L1_get_ARFCNS
				( offset,
				  1,
				  1,
				  l1_tch_amr_data[gas_id].amr_arfcns + 3 ,
				  &ded_dtr_ptr->channel_specification.channel_info_1_before,
				  &ded_dtr_ptr->channel_specification.channel_info_1_after,
				  &ded_dtr_ptr->new_channel_info, gas_id );
			  	}
              }
#endif

            /* FNrptperiod is the FN relative to the start of the reporting period
                This is used to index into the precomputed TXLEVs since they are
                computed with reference to the start of the reporting period
            */
            /* As below logic gives correct FN relative to the start of the reporting period
               adding offset is not needed as relative FN goes wrong when offset can be different in VAMOS cases
               which causes Tx power fluctuation
            */
            FNrptperiod = frame_counters[gas_id].FNmod104 -  l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1;

            if(FNrptperiod < 0)
            {
                FNrptperiod += 104;
            }

            if(FNrptperiod >= 104)
            {
                FNrptperiod -= 104;
            }

            /* Convert TXLEV to power index before passing to the msg layer */
            L1_get_txlev( (uint8)FNrptperiod, 1, l1_tch_amr_data[gas_id].amr_txlev, gas_id );

            /* Update ded data ptr with most recent power level*/
            ded_dtr_ptr->ms_power_level_in_use = (uint8)l1_tch_amr_data[gas_id].amr_txlev[3];

            /* get a vocoder packet */
            if ( l1_vs_get_amr_packet_from_client( l1_tch_amr_data[gas_id].amr_voc_data + 4, 
                    sizeof(l1_tch_amr_data[gas_id].amr_voc_data) - 4, &frame, gas_id) )
            {
               gl1_set_dtx_used_flag(gas_id);
            }

            /*
             * If the vocoder is initialized and the codec mode received from the
             * vocoder is out of bound, then set tx_success equal to false
             * which will result in the frame to be dropped except when we are
             * trying to complete the FACCH.
             */
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
            if( l1_vs_is_client_ready(gas_id) )
            {
                if( frame.ul_info.mode == GL1_MSG_AMR_MODE_UNDEF )
                {
                    MSG_GERAN_HIGH_1_G("Tx AMR_FR Homing Seq",
                             0);
                    tx_success = FALSE;
                }
                else
                {
                    /*
                     * Map the codec mode received from the vocoder to one in the
                     * Active Codec Set. This is the current ul codec mode indication.
                     */
                    for( i = 0; i < curr_ul_num_acm[gas_id]; i++ )
                    {
                        if( curr_ul_acs[gas_id][i] == frame.ul_info.mode )
                        {
                            curr_ul_cmi[gas_id] = (uint8)i;
                            break;
                        }
                    }

                    if( i == curr_ul_num_acm[gas_id] )
                    {
                        MSG_GERAN_ERROR_1_G("Rcvd mode %d from vocoder not in ACS",
                                  frame.ul_info.mode);
                        tx_success = FALSE;
                    }
                    else
                    {
                        /*
                         * Look at the ul_info of the frame to see the codec mode
                         * used by the vocoder to encode the packet
                         */
                        if( l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.valid &&
                            (l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.cmc == curr_ul_cmi[gas_id] ) )
                        {
                            l1_log_amr_cmc_turnaround_time
                            ( l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.frame_count, gas_id );
                            l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.valid = FALSE;
                        }

                    }
                }
            }
#endif
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
            if(!l1_vs_is_client_ready(gas_id) || !tx_success )
#endif
            {
                /*
                 * If the vocoder is not intialized or we didn't receive a
                 * vocoder packet, tx a dummy speech frame.
                 */
                frame.ul_info.frame = GL1_MSG_AMR_SPEECH_GOOD;

                /*
                 * Transmit packet using Initial codec mode untill we receive a
                 * CMC. Once we receive a CMC, we use the phase to determine,
                 * if the U/L frame is a CMI or CMR frame and only apply the
                 * the CMC received on the D/L to the CMI frames on the U/L
                 */
                if( l1_tch_amr_data[gas_id].amr_cmc_rcvd == FALSE)
                {
                    frame.ul_info.mode = curr_ul_acs[gas_id][curr_ul_cmi[gas_id] = curr_ul_icm[gas_id]];
                }
                else
                {
                    boolean cmi_frame = ( (frame_counters[gas_id].FNmod26 % 26  ==  7)     ||
                                          (frame_counters[gas_id].FNmod26 % 26  == 16)     ||
                                          (frame_counters[gas_id].FNmod26 % 26  == 25 ) );
                    if( cmi_frame)
                    {
                        frame.ul_info.mode = curr_ul_acs[gas_id][curr_ul_cmi[gas_id] = curr_cmc[gas_id]];
                    }
                    else
                    {
                        frame.ul_info.mode = curr_ul_acs[gas_id][curr_ul_cmi[gas_id]];
                    }
                }

                /*
                 * Get the homing sequence for the codec rate used on the U/L
                 */
                memscpy ( (byte *)l1_tch_amr_data[gas_id].amr_voc_data+4, sizeof(l1_tch_amr_data[gas_id].amr_voc_data)-4, &amr_homing_seq[curr_ul_acs[gas_id][curr_ul_cmi[gas_id]]][0],
                         (AMR_CHNLS_FS_DATA_WIDTH_OCTETS-4));

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
                /*
                 * If the current TX state is DTX and the vocoder is initialized and we
                 * did not receive a frame from the vocoder then we will transmit a
                 * SPEECH_GOOD with homing sequence. Since this SPEECH_GOOD is artificially
                 * generated by PL1, we set the sid_first_pending flag in order to transition
                 * back into DTX mode after transmitting this SPEECH frame.
                 */
                if( !tx_success && (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX))
                {
                    l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
            }
            }

            if( !l1_tch_amr_data[gas_id].amr_access_burst_scheduled )
            {
                boolean ratscch_permitted = TRUE;
               
#ifdef FEATURE_GSM_COEX
                if( (!sending_FACCH) && ratscch_rsp_pending[gas_id] )
                {
                   if( ( l1_tch_amr_data[gas_id].amr_coex_denials_for_ratscch <= MAX_AMR_COEX_DENIELS_FOR_RATSCCH) &&
                       ( l1_tch_amr_data[gas_id].amr_coex_request_tx != NULL) &&
                       ( l1_tch_amr_data[gas_id].amr_coex_request_tx(GL1_MSG_RATSCCH_FS, gas_id) == FALSE ) )
                   {
                       l1_tch_amr_data[gas_id].amr_coex_denials_for_ratscch++;

                       ratscch_permitted = FALSE;
                   }
                   else
                   {
                       l1_tch_amr_data[gas_id].amr_coex_denials_for_ratscch = 0;

                       ratscch_permitted = TRUE;
                   }
                }
#endif

                /* Now tx a tch, ratscch, or facch message */
                if( sending_FACCH )
                {
                  l1_amr_tx_facch_fr( frame, ded_dtr_ptr, gas_id );
                }
                else if( ratscch_rsp_pending[gas_id] && ratscch_permitted )
                {
                    l1_amr_tx_ratscch_fr( frame, gas_id );
                    ratscch_rsp_pending[gas_id] = FALSE;
                }
                else
                {

                    l1_tch_amr_data[gas_id].amr_voc_data[3] = (uint8)frame.ul_info.frame;
                    l1_tch_amr_data[gas_id].amr_voc_data[2] = (uint8)frame.ul_info.mode;
                    l1_amr_tx_tch_fr( frame, gas_id );


                    /*
                     * After transmitting the SPEECH_GOOD frame, determine if the SPEECH
                     * frame was internally generated. If it was then do not clear the
                     * sid_first pending and sid_update_pending flags.
                     */
                    if( (frame.ul_info.frame == GL1_MSG_AMR_SPEECH_GOOD ) &&
                        !l1_tch_amr_data[gas_id].amr_prev_block_was_facch &&
                        tx_success  )
                    {
                        l1_tch_amr_data[gas_id].amr_sid_first_pending = l1_tch_amr_data[gas_id].amr_sid_update_pending = FALSE;
                    }
                }

                /*
                 * Log the change in AMR state.
                 */
                if( l1_tch_amr_data[gas_id].amr_curr_tx_state != l1_tch_amr_data[gas_id].amr_new_tx_state )
                {
                    l1_log_amr_state_change_T data;
                    data.old_state = (uint8)l1_tch_amr_data[gas_id].amr_curr_tx_state;
                    data.new_state = (uint8)l1_tch_amr_data[gas_id].amr_new_tx_state;
                    l1_log_amr_state_change( &data, gas_id );
                }

                /*
                 * Update the current AMR TX state.
                 */
                l1_tch_amr_data[gas_id].amr_curr_tx_state = l1_tch_amr_data[gas_id].amr_new_tx_state;
            }

            /* Go get an rx message */
            gl1_msg_rx_tch_facch_amr_fr(l1_tch_amr_data[gas_id].amr_arfcns,
                                        l1_tch_amr_data[gas_id].amr_agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                                        l1_tch_amr_data[gas_id].amr_agc_ptr_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/  
                                        &l1_tch_amr_data[gas_id].amr_t_rpt,
                                        l1_tch_amr_data[gas_id].amr_rx_phase,
#ifdef FEATURE_VAMOS_II
                                        vamos2_tch_wait_state[gas_id],
#endif 
                                        gas_id);

        }
    }
    if (l1_tch_amr_data[gas_id].amr_prev_block_was_facch)
    {
      l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter--;
      if (l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter <= 0)
      {
        l1_tch_amr_data[gas_id].amr_prev_block_was_facch = FALSE;
      }
      if (l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter < 0)
      {
        MSG_GERAN_ERROR_1_G("AFS:amr_prev_block_was_facch_cnt =%d",l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter);
      }
    }
}


/*===========================================================================
FUNCTION l1_amr_start_ftn_fr_hr

DESCRIPTION
   This function starts Frame Tick Notification for AMR Full and Half Rate.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void  l1_amr_start_ftn_fr_hr (gas_id_t gas_id)
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    gl1_msg_frame_tick_type    ftn_type;

    /* Now start the FTN */
    gl1_msg_init_frame_tick_info(&ftn_type);
    ftn_type.next_frame_is_dl_block_start             = l1_tch_amr_data[gas_id].amr_block_start;
    ftn_type.next_frame_is_ul_block_start             = l1_tch_amr_data[gas_id].amr_block_start || l1_tch_amr_data[gas_id].amr_access_burst_scheduled;
    ftn_type.next_frame_is_ul_ptcch_sacch_block_start = l1_tch_amr_data[gas_id].amr_sacch_ul_block_start;
    ftn_type.next_frame_is_dl_ptcch_sacch_block_start = (l1_tch_amr_data[gas_id].amr_send_sacch==frame_counters[gas_id].FNmod104) ? TRUE : FALSE;
    ftn_type.next_frame_is_acq_ncell_block_start      = gl1_is_ncell_block_start(gas_id);
    ftn_type.next_frame_is_pwr_meas_block_start       = gl1_is_pwr_meas_block_start(gas_id);


#ifdef FEATURE_GSM_DTM

   if(l1_tskisr_blk->l1_state != L1_DTM_MODE)
   {
     /* Pass FTN structure to NPL1 Message layer */
    gl1_msg_frame_tick_notification ( &ftn_type,gas_id );

    /* Reset block start flags */
    gl1_set_ncell_block_start(FALSE,gas_id);
    gl1_set_pwr_meas_block_start(FALSE,gas_id);
   }else
   {
     /* Store FTN structure away until PS activities are scheduled in l1_dtm.c */
     l1_ded_store_ftn_info(ftn_type, gas_id);
   }

#else

    /* Pass FTN structure to NPL1 Message layer */
    gl1_msg_frame_tick_notification ( &ftn_type,gas_id );

    /* Reset block start flags */
    gl1_set_ncell_block_start(FALSE,gas_id);
    gl1_set_pwr_meas_block_start(FALSE,gas_id);

#endif
}

/*===========================================================================
FUNCTION l1_amr_tch_abort_fr

DESCRIPTION
   Function to abort the AMR Full Rate Traffic Channel

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void  l1_amr_tch_abort_fr ( gas_id_t gas_id )
{
    /* Wait for SCE */
    l1_tch_amr_data[gas_id].amr_idle_status = !l1_sci_is_op_in_progress(gas_id);

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
    l1_tch_amr_data[gas_id].amr_idle_status &= gl1_msg_abort_tx_tch_amr_fr(gas_id);

    /* Abort Rx SACCH */
    gl1_msg_abort_rx_sacch(gas_id);

    /* Abort Rx TCH/FACCH/RATSCCH */
    l1_tch_amr_data[gas_id].amr_idle_status &= gl1_msg_abort_rx_tch_amr_fr(gas_id);

    /* let SCE terminate - it already knows it should */
    l1_sci_tick_receive(gas_id);

    MSG_GERAN_HIGH_0_G("Tch abort");
}


/*===========================================================================
   FUNCTION l1_amr_tx_facch_fr

DESCRIPTION
   This function is the part of the TX side state machine implementation for
   AMR Full Rate and handles FACCH channel processing by resolving contention
   between the Vocoder frames and FACCH frame, giving priority to the FACCH
   frame.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_tx_facch_fr( gl1_msg_amr_frame_info_type frame_info,
                                dedicated_data_T *ded_dtr_ptr, gas_id_t gas_id )
{
    uint8 phase = 0;
    boolean onset_flag = FALSE;
    gl1_msg_chan_type chan_type = GL1_INVALID;
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    /*
     * Get the new state
     */

    l1_tch_amr_data[gas_id].amr_new_tx_state = amr_state_table[l1_tch_amr_data[gas_id].amr_curr_tx_state][AMR_FACCH];

    /*
     * Determine if the MDSP should be programmed to encode CMI or CMR
     */
    if( (frame_counters[gas_id].FNmod26 % 26) == 7     ||
        (frame_counters[gas_id].FNmod26 % 26) == 16    ||
        (frame_counters[gas_id].FNmod26 % 26) == 25  )
    {
        phase = 1;
    }

    /*
     * Init/DTX state procssing and logging support. Make sure to
     * indicate to the MDSP to generate and ONSET frame if were in DTX
     * state
     */
    if( (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX) || (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT) )
    {
        onset_flag = TRUE;

        l1_log_facch_in_dtx(gas_id);

        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT)
        {
            l1_tch_amr_data[gas_id].amr_nsync_cnt = 0;
        }
    }

    /*
     * If there was a RATSCCH message that got pre-empted by FACCH, generate
     * a log event to report a collision between FACCH and RATSCCH.
     */
    if( ratscch_rsp_pending[gas_id] )
    {
        l1_log_facch_and_ratscch_collision(gas_id);
    }

#ifdef GL1_TCH_AMR_DEBUG
    MSG_GERAN_HIGH_3_G( "TXing FACCH with cmi %d cmr %d phase %d",
              curr_ul_cmi[gas_id], curr_cmr[gas_id], phase);
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_FACCH_TX);
#endif

    /*
     * TX FACCH
     */
    gl1_msg_tx_facch_amr_fr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                            l1_tch_amr_data[gas_id].amr_facch_data, onset_flag,
                            curr_ul_cmi[gas_id], curr_cmr[gas_id], phase,
#ifdef FEATURE_VAMOS_II
                            vamos2_tch_wait_state[gas_id],
#endif 
                            gas_id
                           );

    if (l1_tch_amr_data[gas_id].amr_channel_type == GL1_MSG_TCH_AFS)
    {
        chan_type = GL1_MSG_FACCH_AFS;
    }
#ifdef FEATURE_GSM_AMR_WB
    else
    {
        chan_type = GL1_MSG_FACCH_WFS;
    }
#endif

    /* Log tx burst metrics */
    l1_log_transmit_burst_metrics
    ( chan_type,
      1, 1, 4,
      l1_tch_amr_data[gas_id].amr_arfcns,
      l1_tch_amr_data[gas_id].amr_txlev,
      l1_tch_amr_data[gas_id].amr_current_TSC,
      l1_dedicated_data_ptr->timing_advance_in_use, gas_id );

    /* Tx FACCH during TCH loopback, loopback was opened when we received PH_DATA_REQ in l1_ded-if.c
     * so re-enable once FACCH has been transmitted. Note this take 8 frames due to interleaving
     */
    l1_tch_amr_data[gas_id].amr_enable_loopback_after_facch_block = TRUE;

    l1_tch_amr_data[gas_id].amr_prev_block_was_facch = TRUE;

    /* This counter is used to reset the amr_prev_block_was_facch if we are
     * in UL DTX mode - without the counter we will not be able to abort
     * the TCH in this scenario - CR169424
     */
     if(l1_tskisr_blk->main_command != L1_DEDICATED_STOP)
     {
       l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter = L1_WAIT_FOUR_FR_BURSTS_BEFORE_ABORT;
     }

    /*
     * Handle Vocoder frame
     */
    switch(frame_info.ul_info.frame)
    {
    case GL1_MSG_AMR_SPEECH_GOOD:
        break;

    case GL1_MSG_AMR_SID_FIRST:
        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_TCH )
        {
            /*
             * There is a collision of SID_FIRST and FACCH, we set the
             * sid_first_pending flag to indicate that.
             */
            l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
        }
        else if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT )
        {
            /*
             * Should not receive a SID_FIRST in the INIT state if
             * l1_tch_amr_data[gas_id].amr_nsync_cnt is greater that 0. If the nsync count
             * is 0, the SID_FIRST frame is dropped to the floor in the
             * INIT state.
             */
            if( l1_tch_amr_data[gas_id].amr_nsync_cnt )
            {
                MSG_GERAN_ERROR_2_G("Current State: %d, Frame Type: %d",
                          l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame );
            }
        }
        else
        {
            MSG_GERAN_ERROR_2_G("Current State: %d, Frame Type: %d",
                      l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame );
        }
        break;

    case GL1_MSG_AMR_SID_UPDATE:
        /*
         * Save the sid_update frame
         */
        gl1_msg_save_sid_update_amr_fr_hr(l1_tch_amr_data[gas_id].amr_voc_data,gas_id);
        l1_log_facch_and_sid_update_collision(gas_id);
        l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
        l1_tch_amr_data[gas_id].amr_sid_update_pending = TRUE;
        break;

    case GL1_MSG_AMR_NO_DATA:
#ifdef GL1_TCH_AMR_DEBUG
        l1_tch_amr_data[gas_id].amr_no_data_cnt++;
#endif
        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX )
        {
            l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
        }
        break;

    default:
        MSG_GERAN_ERROR_1_G("Invalid Frame Type: %d", frame_info.ul_info.frame);
        break;
    }
}

/*===========================================================================
FUNCTION l1_amr_tx_ratscch

DESCRIPTION
   This function is the part of the TX side state machine implementation for
   AMR Full Rate and handles RATSCCH channel processing by resolving contention
   between the Vocoder frames and RATSCCH frame, giving priority to the
   RATSCCH frame.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_tx_ratscch_fr( gl1_msg_amr_frame_info_type frame_info, gas_id_t gas_id )
{
    uint8 phase = 0;
    boolean onset_flag = FALSE;
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    l1_tch_amr_data[gas_id].amr_new_tx_state = amr_state_table[l1_tch_amr_data[gas_id].amr_curr_tx_state][AMR_RATSCCH];

    if( (frame_counters[gas_id].FNmod26 % 26) == 7     ||
        (frame_counters[gas_id].FNmod26 % 26) == 16    ||
        (frame_counters[gas_id].FNmod26 % 26) == 25  )
    {
        phase = 1;
    }

    if( (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX) || (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT) )
    {
        onset_flag = TRUE;

        l1_log_ratscch_in_dtx(gas_id);

        if(l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT )
        {
            l1_tch_amr_data[gas_id].amr_nsync_cnt = 0;
        }
    }

#ifdef GL1_TCH_AMR_DEBUG
    MSG_GERAN_HIGH_3_G( "TXing RATSCCH, cmi %d, cmr %d, phase %d",
              curr_ul_cmi[gas_id], curr_cmr[gas_id], phase);
#endif

    l1_amr_update_cmr(phase, TRUE, gas_id);

    gl1_msg_tx_ratscch_amr_fr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                              ratscch_rsp[gas_id], onset_flag,
                              curr_ul_cmi[gas_id], curr_cmr[gas_id], phase,                              
#ifdef FEATURE_VAMOS_II
                              vamos2_tch_wait_state[gas_id],
#endif 
                              gas_id);

    /* Log tx burst metrics */
    l1_log_transmit_burst_metrics
    ( GL1_MSG_RATSCCH_FS,
      1, 1, 4,
      l1_tch_amr_data[gas_id].amr_arfcns,
      l1_tch_amr_data[gas_id].amr_txlev,
      l1_tch_amr_data[gas_id].amr_current_TSC,
      l1_dedicated_data_ptr->timing_advance_in_use, gas_id );

    /* Log RATSCCH response, ACK_OK/ACK_ERR/ACK_UNKNOWN */
    l1_log_amr_ratscch_rsp( ratscch_rsp[gas_id][4], gas_id );

    switch(frame_info.ul_info.frame)
    {
    case GL1_MSG_AMR_SPEECH_GOOD:
        break;

    case GL1_MSG_AMR_SID_FIRST:
        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_TCH )
        {
            /*
             * There is a collision of SID_FIRST and RATSCCH, we set the
             * sid_first_pending flag to indicate that.
             */
            l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
        }
        else if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT )
        {
            /*
             * Should not receive a SID_FIRST in the INIT state if
             * amr_nsync_cnt is greater that 0. If the nsync count
             * is 0, the SID_FIRST frame is dropped to the floor in the
             * INIT state.
             */
            if( l1_tch_amr_data[gas_id].amr_nsync_cnt )
            {
                MSG_GERAN_ERROR_2_G("Current State: %d, Frame Type: %d",
                          l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame );
            }
        }
        else
        {
            MSG_GERAN_ERROR_2_G("Current State: %d, Frame Type: %d",
                      l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame );
        }
        break;

    case GL1_MSG_AMR_SID_UPDATE:
        gl1_msg_save_sid_update_amr_fr_hr( l1_tch_amr_data[gas_id].amr_voc_data ,gas_id);
        l1_log_ratscch_and_sid_update_collision(gas_id);
        l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
        l1_tch_amr_data[gas_id].amr_sid_update_pending = TRUE;
        break;

    case GL1_MSG_AMR_NO_DATA:
#ifdef GL1_TCH_AMR_DEBUG
        l1_tch_amr_data[gas_id].amr_no_data_cnt++;
#endif
        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX )
        {
            l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
        }
        break;

    default:
        MSG_GERAN_ERROR_1_G("Invalid Frame Type: %d", frame_info.ul_info.frame);
        break;
    }
}


/*===========================================================================
FUNCTION l1_amr_tx_tch_fr

DESCRIPTION
   This function is the part of the TX side state machine implementation for
   AMR Full Rate and handles TCH channel processing.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void  l1_amr_tx_tch_fr( gl1_msg_amr_frame_info_type frame_info, gas_id_t gas_id )
{
    amr_chnl_type amr_frame;
    boolean onset_flag = FALSE;
    uint8 phase = 0;
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    if( (frame_counters[gas_id].FNmod26 % 26) == 7     ||
        (frame_counters[gas_id].FNmod26 % 26) == 16    ||
        (frame_counters[gas_id].FNmod26 % 26) == 25  )
    {
        phase = 1;
    }

    switch( frame_info.ul_info.frame )
    {
    case GL1_MSG_AMR_SPEECH_GOOD:
        amr_frame = AMR_SPEECH_GOOD;
        break;

    case GL1_MSG_AMR_SID_FIRST:
        amr_frame = AMR_SID_FIRST;
        break;

    case GL1_MSG_AMR_SID_UPDATE:
        amr_frame = AMR_SID_UPDATE;
        break;

    case GL1_MSG_AMR_NO_DATA:
        amr_frame = AMR_NO_DATA;
#ifdef GL1_TCH_AMR_DEBUG
        l1_tch_amr_data[gas_id].amr_no_data_cnt++;
#endif
        break;

    default:
        MSG_GERAN_ERROR_1_G("Invalid frame type: %d", frame_info.ul_info.frame);
        return;
    }

    l1_tch_amr_data[gas_id].amr_new_tx_state = amr_state_table[l1_tch_amr_data[gas_id].amr_curr_tx_state][amr_frame];

    switch( amr_frame )
    {
    /*-------------------------------------------------------------------------
     * Rcvd SPEECH frame from the vocoder.
     *-------------------------------------------------------------------------*/
    case AMR_SPEECH_GOOD:
        if( (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX) || (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT) )
        {
            onset_flag = TRUE;

            if(l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT)
            {
                l1_tch_amr_data[gas_id].amr_nsync_cnt= 0;
            }
        }

        l1_amr_update_cmr ( phase, FALSE, gas_id );


        if( !l1_tch_amr_data[gas_id].amr_loopback_I_in_progress )
        {
            gl1_msg_tx_tch_amr_fr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                  l1_tch_amr_data[gas_id].amr_voc_data, onset_flag,
                                  curr_ul_cmi[gas_id], curr_dl_cmi[gas_id],
                                  curr_cmr[gas_id], curr_cmc[gas_id],
                                  curr_ul_acs[gas_id][curr_ul_cmi[gas_id]],
                                  phase,
#ifdef FEATURE_VAMOS_II
                                  vamos2_tch_wait_state[gas_id],
#endif 
                                  gas_id
                                 );
        }
        else
        {
            gl1_msg_tx_tch_amr_fr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                  l1_tch_amr_data[gas_id].amr_voc_data, onset_flag,
                                  curr_ul_cmi[gas_id], curr_dl_cmi[gas_id],
                                  curr_cmr[gas_id], curr_cmc[gas_id],
                                  curr_ul_acs[gas_id][curr_cmc[gas_id]],
                                  phase,
#ifdef FEATURE_VAMOS_II
                                  vamos2_tch_wait_state[gas_id],
#endif 
                                  gas_id 
                                 );
        }


        l1_log_transmit_burst_metrics
        ( l1_tch_amr_data[gas_id].amr_channel_type,
          1, 1, 4,
          l1_tch_amr_data[gas_id].amr_arfcns,
          l1_tch_amr_data[gas_id].amr_txlev,
          l1_tch_amr_data[gas_id].amr_current_TSC,
          l1_dedicated_data_ptr->timing_advance_in_use, gas_id );

#ifdef GL1_TCH_AMR_DEBUG
        MSG_GERAN_HIGH_3_G( "TXing SPEECH_GOOD, cmi %d, cmr %d, phase %d",
                  curr_ul_cmi[gas_id], curr_cmr[gas_id], phase);
#endif

        break;

        /*-------------------------------------------------------------------------
         * Rcvd SID_FIRST frame from the vocoder.
         *-------------------------------------------------------------------------*/
    case AMR_SID_FIRST:
        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_TCH )
        {
            l1_amr_update_cmr (phase, FALSE, gas_id);

            gl1_msg_tx_sid_first_amr_fr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                        l1_tch_amr_data[gas_id].amr_voc_data,
                                        curr_ul_cmi[gas_id], curr_cmr[gas_id], phase,                                        
#ifdef FEATURE_VAMOS_II
                                        vamos2_tch_wait_state[gas_id],
#endif 
                                        gas_id);

            /* Log tx burst metrics */
            l1_log_transmit_burst_metrics
            ( GL1_MSG_SID_FIRST_FS,
              1, 1, 4,
              l1_tch_amr_data[gas_id].amr_arfcns,
              l1_tch_amr_data[gas_id].amr_txlev,
              l1_tch_amr_data[gas_id].amr_current_TSC,
              l1_dedicated_data_ptr->timing_advance_in_use, gas_id );

#ifdef GL1_TCH_AMR_DEBUG
            l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
            MSG_GERAN_HIGH_3_G( "TXing SID_FIRST, cmi %d, cmr %d, phase %d",
                      curr_ul_cmi[gas_id], curr_cmr[gas_id], phase);
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_FIRST_TX);
#endif
        }
        else if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT )
        {
            /*
             * Should not receive a SID_FIRST in the INIT state if
             * amr_nsync_cnt is greater that 0. If the nsync count
             * is 0, the SID_FIRST frame is dropped to the floor in the
             * INIT state.
             */
            if( l1_tch_amr_data[gas_id].amr_nsync_cnt )
            {
                MSG_GERAN_ERROR_2_G("Current State: %d, Frame Type: %d",
                          l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame );
            }
        }
        else
        {
            MSG_GERAN_ERROR_2_G("Current State: %d, Frame Type: %d",
                      l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame );
        }
        break;

        /*-------------------------------------------------------------------------
         * Rcvd SID_UPDATE frame from the vocoder.
         *-------------------------------------------------------------------------*/
    case AMR_SID_UPDATE:
        if( (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_TCH) && ( l1_tch_amr_data[gas_id].amr_sid_first_pending == TRUE ) )
        {
            /*
             * If in the TCH state a sid_first is pending and we
             * recieve a SID_UPDATE from the vocoder, we save the
             * received SID_UPDATE and transmit the sid_first.
             * We also change our state to go to DTX state and
             * set sid_update_pending to TRUE, so that we can
             * schedule the SID_UPDATE next time around.
             */

            /*
             * NOTE -- This condition can only happen if a internally generated
             * SPEECH_GOOD, FACCH or a RATSCCH collided with a NO_DATA frame
             * from the vocoder, right before getting a SID_UPDATE
             * frame from the vocoder.
             */
            gl1_msg_save_sid_update_amr_fr_hr( l1_tch_amr_data[gas_id].amr_voc_data ,gas_id);
            l1_amr_update_cmr (phase, FALSE, gas_id);
            gl1_msg_tx_sid_first_amr_fr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                        l1_tch_amr_data[gas_id].amr_voc_data,
                                        curr_ul_cmi[gas_id], curr_cmr[gas_id], phase,                                        
#ifdef FEATURE_VAMOS_II
                                        vamos2_tch_wait_state[gas_id],
#endif 
                                        gas_id );
            /* Log tx burst metrics */
            l1_log_transmit_burst_metrics
            ( GL1_MSG_SID_FIRST_FS,
              1, 1, 4,
              l1_tch_amr_data[gas_id].amr_arfcns,
              l1_tch_amr_data[gas_id].amr_txlev,
              l1_tch_amr_data[gas_id].amr_current_TSC,
              l1_dedicated_data_ptr->timing_advance_in_use, gas_id );


#ifdef GL1_TCH_AMR_DEBUG
            l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
            MSG_GERAN_HIGH_3_G( "TXing saved SID_FIRST, cmi %d, cmr %d, phase %d",
                      curr_ul_cmi[gas_id], curr_cmr[gas_id], phase);
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_FIRST_TX);
#endif
            l1_tch_amr_data[gas_id].amr_new_tx_state =
            amr_state_table[l1_tch_amr_data[gas_id].amr_curr_tx_state][AMR_SID_FIRST];

            l1_tch_amr_data[gas_id].amr_sid_first_pending = FALSE;
            l1_tch_amr_data[gas_id].amr_sid_update_pending = TRUE;
        }
        else
        {
            if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_TCH )
            {
                /*
                 * Even though we received a SID_UPDATE in TCH state,
                 * transition the state to DTX and transmit the SID_UPDATE.
                 */
                MSG_GERAN_HIGH_1_G("Rcvd SID_UPDATE from vocoder, forcing tx state to DTX",
                         0);
                l1_tch_amr_data[gas_id].amr_new_tx_state = AMR_DTX;
            }
            l1_amr_update_cmr(phase, TRUE, gas_id);
            gl1_msg_tx_sid_update_amr_fr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                         l1_tch_amr_data[gas_id].amr_voc_data,
                                         curr_ul_cmi[gas_id], curr_cmr[gas_id],
                                         phase,                                         
#ifdef FEATURE_VAMOS_II
                                         vamos2_tch_wait_state[gas_id],
#endif 
                                         gas_id );

            /* Log tx burst metrics */
            l1_log_transmit_burst_metrics
            ( GL1_MSG_SID_UPDATE_FS,
              1, 1, 4,
              l1_tch_amr_data[gas_id].amr_arfcns,
              l1_tch_amr_data[gas_id].amr_txlev,
              l1_tch_amr_data[gas_id].amr_current_TSC,
              l1_dedicated_data_ptr->timing_advance_in_use, gas_id );

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_UPDATE_TX);
#endif

#ifdef GL1_TCH_AMR_DEBUG
            MSG_GERAN_HIGH_3_G( "TXing SID_UPDATE, cmi %d, cmr %d, cnt %d",
                      curr_ul_cmi[gas_id], curr_cmr[gas_id], l1_tch_amr_data[gas_id].amr_no_data_cnt);
            l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
#endif
        }

        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT )
        {
            /*
             * Increment NSYNC count
             */
            l1_tch_amr_data[gas_id].amr_nsync_cnt++;

            /*
             * Init state processing requires the nsync count to reach 12
             * to move into DTX state.
             */
            if( l1_tch_amr_data[gas_id].amr_nsync_cnt == AMR_MAX_NSYNC_CNT )
            {
                l1_tch_amr_data[gas_id].amr_new_tx_state  = AMR_DTX;
                l1_tch_amr_data[gas_id].amr_nsync_cnt = 0;
            }
        }
        break;
        /*-------------------------------------------------------------------------
         * Rcvd NO_DATA frame from the vocoder.
         *-------------------------------------------------------------------------*/
    case AMR_NO_DATA:
        if( l1_tch_amr_data[gas_id].amr_sid_first_pending )
        {
            if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_TCH )
            {
                /*
                 * If received NO_DATA from the vocoder, and were in TCH state
                 * and sid_first is pending, transmit the pending sid_first and
                 * transition the state to DTX.
                 */
                l1_amr_update_cmr(phase, FALSE, gas_id);

                gl1_msg_tx_sid_first_amr_fr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                            l1_tch_amr_data[gas_id].amr_voc_data,
                                            curr_ul_cmi[gas_id], curr_cmr[gas_id], phase,                                            
#ifdef FEATURE_VAMOS_II
                                            vamos2_tch_wait_state[gas_id],
#endif 
                                            gas_id );

                /* Log tx burst metrics */
                l1_log_transmit_burst_metrics
                ( GL1_MSG_SID_FIRST_FS,
                  1, 1, 4,
                  l1_tch_amr_data[gas_id].amr_arfcns,
                  l1_tch_amr_data[gas_id].amr_txlev,
                  l1_tch_amr_data[gas_id].amr_current_TSC,
                  l1_dedicated_data_ptr->timing_advance_in_use, gas_id );


#ifdef GL1_TCH_AMR_DEBUG
                l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
                MSG_GERAN_HIGH_3_G( "TXing SID_FIRST, cmi %d, cmr %d, phase %d",
                          curr_ul_cmi[gas_id], curr_cmr[gas_id], phase);
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_FIRST_TX);
#endif
                l1_tch_amr_data[gas_id].amr_new_tx_state =
                amr_state_table[l1_tch_amr_data[gas_id].amr_curr_tx_state][AMR_SID_FIRST];

                l1_tch_amr_data[gas_id].amr_sid_first_pending = FALSE;
            }
            else
            {
                MSG_GERAN_ERROR_3_G("Curr State:%d,Frame Type:%d,SID FIRST PEND:%d",
                          l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame,
                          l1_tch_amr_data[gas_id].amr_sid_first_pending);
            }
        }
        else if( l1_tch_amr_data[gas_id].amr_sid_update_pending )
        {
            if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX )
            {
                /*
                 * If received NO_DATA from the vocoder, and we were in DTX
                 * state and sid_update is pending, transmit the pending
                 * sid_update and stay in the DTX state.
                 */
                l1_amr_update_cmr(phase, TRUE, gas_id);
                gl1_msg_tx_saved_sid_update_amr_fr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                                   curr_ul_cmi[gas_id], curr_cmr[gas_id],
                                                   phase,                                                   
#ifdef FEATURE_VAMOS_II
                                                   vamos2_tch_wait_state[gas_id],
#endif 
                                                   gas_id );

                /* Log tx burst metrics */
                l1_log_transmit_burst_metrics
                ( GL1_MSG_SID_UPDATE_FS,
                  1, 1, 4,
                  l1_tch_amr_data[gas_id].amr_arfcns,
                  l1_tch_amr_data[gas_id].amr_txlev,
                  l1_tch_amr_data[gas_id].amr_current_TSC,
                  l1_dedicated_data_ptr->timing_advance_in_use, gas_id );

#ifdef GL1_TCH_AMR_DEBUG
                MSG_GERAN_HIGH_3_G( "TXing Saved SID_UPDATE, cmi %d, cmr %d, cnt %d",
                          curr_ul_cmi[gas_id], curr_cmr[gas_id], l1_tch_amr_data[gas_id].amr_no_data_cnt);
                l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_UPDATE_TX);
#endif
                l1_tch_amr_data[gas_id].amr_sid_update_pending = FALSE;
            }
            else
            {
                MSG_GERAN_ERROR_3_G("Curr State:%d,Frame Type:%d,SID UPDATE PEND:%d",
                          l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame,
                          l1_tch_amr_data[gas_id].amr_sid_update_pending);
            }
        }
        else if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT )
        {
            /*
             * If received NO_DATA from the vocoder, and we were in INIT
             * state, transmit a sid_update using the payload received
             * in the NO_DATA frame and stay in the INIT state.
             */
            l1_tch_amr_data[gas_id].amr_nsync_cnt++;

            l1_amr_update_cmr(phase, TRUE, gas_id);

            gl1_msg_tx_sid_update_amr_fr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev, l1_tch_amr_data[gas_id].amr_voc_data,
                                         curr_ul_cmi[gas_id], curr_cmr[gas_id],
                                         phase,                                         
#ifdef FEATURE_VAMOS_II
                                         vamos2_tch_wait_state[gas_id],
#endif 
                                         gas_id );

            /* Log tx burst metrics */
            l1_log_transmit_burst_metrics
            ( GL1_MSG_SID_UPDATE_FS,
              1, 1, 4,
              l1_tch_amr_data[gas_id].amr_arfcns,
              l1_tch_amr_data[gas_id].amr_txlev,
              l1_tch_amr_data[gas_id].amr_current_TSC,
              l1_dedicated_data_ptr->timing_advance_in_use, gas_id );


#ifdef GL1_TCH_AMR_DEBUG
            MSG_GERAN_HIGH_3_G( "TXing Init State SID_UPDATE, cmi %d, cmr %d, cnt %d",
                      curr_ul_cmi[gas_id], curr_cmr[gas_id], l1_tch_amr_data[gas_id].amr_no_data_cnt);
            l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_UPDATE_TX);
#endif

            if( l1_tch_amr_data[gas_id].amr_nsync_cnt == AMR_MAX_NSYNC_CNT )
            {
                l1_tch_amr_data[gas_id].amr_new_tx_state = AMR_DTX;
                l1_tch_amr_data[gas_id].amr_nsync_cnt = 0;
            }
        }
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        else
        {
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_NO_DATA_TX);
        }
#endif
        break;

    default:
        MSG_GERAN_ERROR_1_G("Invalid Frame Type: %d", amr_frame);
        break;
    }
}

/*===========================================================================
FUNCTION l1_amr_process_multirate_ie

DESCRIPTION
   This functions handles the multirate IE received from the RR.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void  l1_amr_process_multirate_ie( dedicated_data_T *ded_dtr_ptr, gas_id_t gas_id )
{
    uint16 i, j;
    uint8 mask = 0x1, num_modes_rcvd = 0;
    uint8 max_modes = 0;
    multirate_config_T *local_mr_ptr;
#ifdef FEATURE_GSM_AMR_WB
    channel_mode_T      channel_mode;
#endif


    if(!ded_dtr_ptr)
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL ded_dtr_ptr");
        return;
    }

    local_mr_ptr = &(ded_dtr_ptr->multirate_configuration);

#ifdef FEATURE_GSM_AMR_WB
    channel_mode = ded_dtr_ptr->channel_specification.channel_mode_1;
#endif

    if( amr_channel[gas_id] == FULL_RATE_TRAFFIC )
    {
        max_modes = MAX_FULL_RATE_MODES;

#ifdef FEATURE_GSM_AMR_WB

        if ((channel_mode == SPEECH_V5)
#ifdef FEATURE_VAMOS_II
          ||(channel_mode == SPEECH_V5_VAMOS2)
#endif
           )
        {
            /*We currently only support the lowest 3 WB AMR rates*/
            max_modes = MAX_WB_FULL_RATE_MODES;
        }
#endif
    }
    else
    {
        max_modes = MAX_HALF_RATE_MODES;
    }

    if( local_mr_ptr->valid)
    {
        /*
         * Extract the codec modes from the received ACS.
         */
        for( i = 0, j = 0; i < max_modes; i++, mask <<= 1 )
        {
            if( (local_mr_ptr->codec_modes) & mask )
            {
                num_modes_rcvd++;

                if( num_modes_rcvd > MAX_CODEC_MODES )
                {
                    MSG_GERAN_ERROR_2_G("Invalid num of modes rcvd %d", num_modes_rcvd,
                               0);
                    return;
                }

                if ( j >= MAX_CODEC_MODES)
                {
                  j = MAX_CODEC_MODES-1;
                  MSG_GERAN_ERROR_1_G("Current ACS capped to %d !!",j);
                }
                  curr_ul_acs[gas_id][j]   = (gl1_msg_amr_mode_type)i;
                  curr_dl_acs[gas_id][j]   = (gl1_msg_amr_mode_type)i;
#ifdef FEATURE_GSM_AMR_WB
                if ((channel_mode == SPEECH_V5)
#ifdef FEATURE_VAMOS_II
                     ||(channel_mode == SPEECH_V5_VAMOS2)
#endif
                    )
                {
                   /*
                    * NW sends a different list of codec modes dependant
                    * on MR speech version. If MR speech ver.=2
                    * we need to map this to gl1_msg_amr_mode_type
                    */
   
                    curr_ul_acs[gas_id][j]   = (gl1_msg_amr_mode_type)
                                       (i + (uint16)GL1_MSG_AMR_WB_MODE_0660);
                    curr_dl_acs[gas_id][j]   = (gl1_msg_amr_mode_type)
                                       (i + (uint16)GL1_MSG_AMR_WB_MODE_0660);
#ifdef ABORT_F3_REDUCTION
                    MSG_GERAN_HIGH_3_G("curr_ul_acs[gas_id][x] =%d, curr_dl_acs[x]=%d, x=%d",
                             curr_ul_acs[gas_id][j],curr_dl_acs[gas_id][j],j);
#endif
                }
#endif
                j++;
            }
        }

        if ( num_modes_rcvd > 0 )
        {
           /*
            * Fill a valid mode in the remaining entries of the ACS so that
            * when it is sent to the MDSP, all four entries are populated
            */
           for( i = num_modes_rcvd; i < MAX_CODEC_MODES; i++)
           {
              curr_ul_acs[gas_id][i] = curr_ul_acs[gas_id][num_modes_rcvd - 1];
              curr_dl_acs[gas_id][i] = curr_dl_acs[gas_id][num_modes_rcvd - 1];
           }

           curr_ul_num_acm[gas_id] = curr_dl_num_acm[gas_id] = num_modes_rcvd;
        }
        else
        {
           MSG_GERAN_ERROR_2_G("Invalid num of modes rcvd %d", num_modes_rcvd,
                               0);
        }

        /*
         * Check to see if we received a valid ICM. If not, use the
         * default ICM as specified in 05.09.
         */
        if( local_mr_ptr->start_mode_valid )
        {
            curr_ul_icm[gas_id] = (uint8)local_mr_ptr->start_mode;
            curr_dl_icm[gas_id] = (uint8)local_mr_ptr->start_mode;

            if( (curr_ul_icm[gas_id] >= curr_ul_num_acm[gas_id] ) ||
                (curr_dl_icm[gas_id] >= curr_dl_num_acm[gas_id]) )
            {
                MSG_GERAN_ERROR_0_G("ICM out of bound wrt acs");

                set_ul_default_icm(gas_id);
                set_dl_default_icm(gas_id);
            }
        }
        else
        {
            set_ul_default_icm(gas_id);
            set_dl_default_icm(gas_id);
        }
#ifdef FEATURE_GSM_AMR_WB
        MSG_GERAN_HIGH_3_G("Rcvd ACS = %d, ICM=%d in Multirate IE, WB_AMR=%d",
                 local_mr_ptr->codec_modes, curr_ul_icm[gas_id],((channel_mode == SPEECH_V5)? TRUE:FALSE ) );
#else
        MSG_GERAN_HIGH_2_G("Rcvd ACS = %d, ICM=%d in Multirate IE, WB_AMR not enabled",
                 local_mr_ptr->codec_modes, curr_ul_icm[gas_id]);
#endif
        /*
         * Now set the thresholds and hysteresis
         */
        for( i = 0; i < (MAX_CODEC_MODES - 1); i++ )
        {
            /*
             * There is one less Thresh/Hyst pair valid than
             * the number of active codec modes. For all others
             * set the thresholds to 0x3F and hyst to 0xF.
             */
            if( i < (num_modes_rcvd - 1) )
            {

#ifdef FEATURE_GSM_MDSP_ESAIC
                 sys_band_T serving_cell_band = gl1_get_band ( gl1_get_serving_cell_arfcn(gas_id) );
                 if(mcfg_gcf_nv_get_status() &&
                    ( (serving_cell_band == SYS_BAND_PCS_1900) || (serving_cell_band == SYS_BAND_DCS_1800) ) &&
                    ( local_mr_ptr->thresholds[i] != 0x3F )
                   )

                 {
                   /*Check to ensure the 2dB can be added to this threashold*/
                   if (local_mr_ptr->thresholds[i] > 26)
                   {
                     curr_thresh_tbl[gas_id][i] = 31;
                   }
                   else
                   {
                     curr_thresh_tbl[gas_id][i] = (local_mr_ptr->thresholds[i] + 4);
                   }
                 }
                 else
#endif
                 {
                   curr_thresh_tbl[gas_id][i] = local_mr_ptr->thresholds[i];
                 }

                curr_hyst_tbl[gas_id][i] = local_mr_ptr->hysteresis[i];
            }
            else
            {
                curr_thresh_tbl[gas_id][i] = 0x3F;
                curr_hyst_tbl[gas_id][i] = 0xF;
            }
        }

        gl1_msg_set_dl_acs(curr_dl_acs[gas_id],gas_id);
    }
    else
    {
        MSG_GERAN_HIGH_0_G("No MR IE rcvd from RR");
        /*
         * This can only happen if it's an AMR-AMR handover and we are
         * being told to use the old codec parameters. curr_ul_acs, curr_dl_acs
         * curr_hyst_tbl, curr_thresh_tbl, cull_ul_icm, curr_dl_icm,
         * curr_dl_num_acm, curr_ul_num_acm are good to be used in the new
         * cell.
         */
    }

    curr_cmr[gas_id] = computed_cmr[gas_id] = curr_dl_icm[gas_id];
    curr_cmc[gas_id] = new_cmc[gas_id] = curr_ul_icm[gas_id];
    curr_dl_cmi[gas_id] = curr_dl_icm[gas_id];
    curr_ul_cmi[gas_id] = curr_ul_icm[gas_id];
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
    l1_vs_set_ul_amr_mode( curr_ul_acs[gas_id][curr_ul_cmi[gas_id]], gas_id );
#endif /* FEATURE_MODEM_NO_VOICE_SUPPORT */

    amr_thresh_hyst_defined[gas_id] = (num_modes_rcvd == 1)?FALSE:TRUE;

    /*
     * Now reset any of the pending RATSCCH procedure.
     */
    ratscch_reset_pending_proc(gas_id);
}


/*****************************************************

 CALLBACK FUCNTIONS

******************************************************/


/*===========================================================================
FUNCTION amr_tch_tx_callback_fr_hr

DESCRIPTION
   This function is called in the first frame of a TCH block (ie 4 frames).
   This function is common to AMR Full and Half rate.

DEPENDENCIES
   None.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void amr_tch_tx_callback_fr_hr( gas_id_t gas_id )
{
    /* This callback is invoked in the frame the first burst is scheduled.
        i.e FNmod13 = 4,8,12.  We set this flag to FALSE here because if we
        set it when we prepare the TCH msg (FnMod13 = 2,6 etc), the abort
        (which checks for prev_block_was_facch == FALSE)
        kicks in immediately even before the completing TCH block is scheduled.
        Thus we need to set this after the first burst of the completing TCH
        block has been scheduled
    */
    l1_tch_amr_data[gas_id].amr_prev_block_was_facch = FALSE;

    /* reset counter */
    l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter = 0;
}

/*===========================================================================
FUNCTION amr_facch_tx_callback_fr_hr

DESCRIPTION
   This function is called in the first frame of a FACCH block (ie 4 frames).
   This function is common to AMR Full and Half rate.

DEPENDENCIES
   None.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void amr_facch_tx_callback_fr_hr( gas_id_t gas_id )
{
    /* nothing to do */

    if( l1_tch_amr_data[gas_id].amr_hr_facch_tx_in_progress )
    {
        l1_tch_amr_data[gas_id].amr_hr_facch_tx_in_progress--;
    }

    /*
     * amr_facch_not_available in this case indicates whether the FACCH block
     * for which this callback is invoked is a filler frame or not.  If its
     * a filler frame, its basically a completion block for the 8 bursts, so
     * we can set amr_prev_block_was_facch = FALSE
     */
    if( l1_tch_amr_data[gas_id].amr_facch_not_available )
    {
        l1_tch_amr_data[gas_id].amr_prev_block_was_facch = FALSE;
        /* reset counter */
        l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter = 0;
    }
}


/*===========================================================================
FUNCTION amr_ratscch_tx_callback_fr_hr

DESCRIPTION
   This function is called in the first frame of a RATSCCH block (ie 4 frames).
   This function is common to AMR Full and Half rate.

DEPENDENCIES
   None.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void amr_ratscch_tx_callback_fr_hr( gas_id_t gas_id )
{
    ratscch_start_ack_activation_counter(gas_id);

    if( l1_tch_amr_data[gas_id].amr_hr_ratscch_tx_in_progress )
    {
        l1_tch_amr_data[gas_id].amr_hr_ratscch_tx_in_progress--;
    }
}


/*===========================================================================
FUNCTION amr_sid_first_tx_callback_fr_hr

DESCRIPTION
   This function is called in the first frame of a SID_FIRST block (ie 4 frames).
   This function is common to AMR Full and Half rate.

DEPENDENCIES
   None.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void amr_sid_first_tx_callback_fr_hr( gas_id_t gas_id )
{

   /* If we were in DTX and we have just sent a FACCH, we need to clear
    * amr_prev_block_was_facch since we have just sent SID_FIRST
    */
   l1_tch_amr_data[gas_id].amr_prev_block_was_facch = FALSE;

    if( l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress )
    {
        l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress--;
    }
}


/*===========================================================================
FUNCTION amr_sid_update_tx_callback_fr_hr

DESCRIPTION
   This function is called in the first frame of a SID_UPDATE block (ie 4 frames).
   This function is common to AMR Full and Half rate.

DEPENDENCIES
   None.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void amr_sid_update_tx_callback_fr_hr( gas_id_t gas_id )
{
    if( l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress )
    {
        l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress--;
    }
}


/*===========================================================================

FUNCTION amr_tch_rx_callback_fr

DESCRIPTION
   This function is called when the data from the TCH has been decoded. The
   data is then passed to the vocoder or up to L2 if it is a FACCH message.
   Receiver Ready (layer 2) are filtered and not passed on up to reduce
   overhead.

   This function implements the RX side state machine for AMR Full Rate.

DEPENDENCIES
   None.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void amr_tch_rx_callback_fr(gl1_msg_decode_rpt *rpt_i, gas_id_t gas_id )
{
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    tch_facch_decode_rpt * rpt = (tch_facch_decode_rpt*)rpt_i;

    gl1_msg_amr_frame_info_type frame;
    boolean cmi_rcvd = FALSE, cmc_rcvd = FALSE;

    uint8 dl_acs = 0, ul_acs = 0;
    uint16 amr_status = 0;
    uint8 i, ack_act_counter = 0;
#ifdef FEATURE_GSM_RX_DIVERSITY
    gl1_rxd_control_type gl1_rxd_ctl_flag;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

    if(!rpt)
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
        return;
    }

    /* Do RXLEV_VAL processing */
    gl1_update_emr_parameters ( &(rpt->hdr), FALSE, TRUE, gas_id );

    if( l1_tch_amr_data[gas_id].amr_first_rx_block )
    {
#ifdef TCH_PROFILING
        l1_tch_amr_data[gas_id].total_crc_cnt_fr = l1_tch_amr_data[gas_id].total_crc_bad_fr = l1_tch_amr_data[gas_id].total_bfi_cnt_fr = 0;
        l1_tch_amr_data[gas_id].crc_cnt_fr = l1_tch_amr_data[gas_id].crc_bad_fr = l1_tch_amr_data[gas_id].bfi_cnt_fr = 0;
#endif
        l1_tch_amr_data[gas_id].amr_first_rx_block = FALSE;
        return;
    }

    ack_act_counter = ratscch_update_ack_activation_counter(gas_id);

    if(l1_tch_amr_data[gas_id].amr_need_to_expire_ack_act_timer)
    {
        l1_tch_amr_data[gas_id].amr_need_to_expire_ack_act_timer = FALSE;
        ratscch_expire_ack_activation_timer(gas_id);
    }

    if( rpt->hdr.onset )
    {

#ifdef GL1_TCH_AMR_DEBUG
        MSG_GERAN_HIGH_0_G("Rcvd FR ONSET frame from MDSP");
#endif
        cmi_rcvd = TRUE;
        frame.dl_info.frame = GL1_MSG_AMR_ONSET;
        /* If onset flag is set, reset the channel type from GL1_MSG_TCH_AFS
         * to GL1_MSG_ONSET just for logging purpose
         */
        rpt->hdr.chan = (uint16)GL1_MSG_ONSET;
        l1_tch_amr_data[gas_id].amr_rx_state = AMR_TCH;
    }
    else
    {
        switch(rpt->hdr.chan)
        {
#ifdef FEATURE_GSM_AMR_WB
        case GL1_MSG_FACCH_WFS:
#endif
        case GL1_MSG_FACCH_AFS:
            l1_log_rfacch_metrics((gl1_msg_decode_rpt*)rpt, gas_id);

            if (l1_handle_rfacch(rpt, GL1_MSG_FACCH_AFS, gas_id) )

            {

#ifdef GL1_TCH_AMR_DEBUG
#ifdef FEATURE_GSM_AMR_WB
              MSG_GERAN_HIGH_1_G("Rcvd (WB=%d)AMR FR FACCH frame from MDSP",
                        (rpt->hdr.chan==(uint16)GL1_MSG_FACCH_WFS)?TRUE:FALSE);
#else
              MSG_GERAN_HIGH_0_G("Rcvd AMR FR FACCH frame from MDSP");
#endif
#endif
              L1_send_PH_DATA_IND (TRUE,
                                   DCCH,
                                   FALSE,  /* no paging involved */
                                   NULL_ARFCN,   /*ARFCN only in idle mode*/
                                   rpt->data,
                                   N201_MAX, gas_id);

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
              gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_VALID_FACCH_RX);
#endif

            }

            if (!rpt->hdr.good_data)
            {
                l1_tch_amr_data[gas_id].crc_bad_fr++;
                l1_tch_amr_data[gas_id].total_crc_bad_fr++;
            }

            /* BFI always set to 1 for FACCH */
            /* DSP supposed to handle this but this is just to be sure */
            rpt->hdr.bfi = 1;
            frame.dl_info.frame = GL1_MSG_AMR_NO_DATA;
            break;

        case GL1_MSG_RATSCCH_FS:
#ifdef FEATURE_GSM_AMR_WB
        case GL1_MSG_RATSCCH_WFS:
#endif
#ifdef GL1_TCH_AMR_DEBUG
#ifdef FEATURE_GSM_AMR_WB
            MSG_GERAN_HIGH_1_G("Rcvd (WB=%d)FR RATSCCH frame from MDSP",
                        (rpt->hdr.chan==(uint16)GL1_MSG_RATSCCH_WFS)?TRUE:FALSE);
#else
            MSG_GERAN_HIGH_0_G("Rcvd FR RATSCCH frame from MDSP");
#endif
#endif

            /*
             * For RATSCCH message, both CMI and CMC are valid
             */
            ratscch_process_req(rpt->data, rpt->hdr.good_data,gas_id);

            frame.dl_info.frame = GL1_MSG_AMR_NO_DATA;

            if( !rpt->hdr.good_data )
            {
                l1_tch_amr_data[gas_id].crc_bad_fr++;
                l1_tch_amr_data[gas_id].total_crc_bad_fr++;
            }

            cmi_rcvd = cmc_rcvd = TRUE;

            break;

        case GL1_MSG_SID_FIRST_FS:
#ifdef GL1_TCH_AMR_DEBUG
            MSG_GERAN_HIGH_0_G("Rcvd FR SID_FIRST frame from MDSP");
#endif
            l1_tch_amr_data[gas_id].amr_rx_state = AMR_DTX;

            frame.dl_info.frame = GL1_MSG_AMR_SID_FIRST;

            cmc_rcvd = ( (cmi_rcvd = (boolean)(rpt->hdr.phase == 1) )==TRUE)?FALSE:TRUE;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_FIRST_RX);
#endif
            break;

        case GL1_MSG_SID_UPDATE_FS:
            l1_tch_amr_data[gas_id].amr_rx_state = AMR_DTX;

            /*
             * For SID_UPDATE message, both CMI and CMC are valid
             */
            if( rpt->hdr.good_data )
            {
                frame.dl_info.frame = GL1_MSG_AMR_SID_UPDATE;
#ifdef GL1_TCH_AMR_DEBUG
                MSG_GERAN_HIGH_0_G("Rcvd FR SID_UPDATE frame from MDSP");
#endif
            }
            else
            {
                frame.dl_info.frame = GL1_MSG_AMR_SID_BAD;
#ifdef GL1_TCH_AMR_DEBUG
                MSG_GERAN_HIGH_0_G("Rcvd FR SID_BAD frame from MDSP");
#endif
                l1_tch_amr_data[gas_id].crc_bad_fr++;
                l1_tch_amr_data[gas_id].total_crc_bad_fr++;
            }
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_UPDATE_RX);
#endif
            cmi_rcvd = cmc_rcvd =  TRUE;

            gl1_msg_rxqual_sub_update( (uint16)rpt->hdr.rx_qual, gas_id );
#ifdef RXQUAL_MSG
            MSG_GERAN_MED_1_G("Update: rxqual_sub tch(dtx) = %d",rpt->hdr.rx_qual);
#endif


            break;

        case GL1_MSG_TCH_AFS:
#ifdef FEATURE_GSM_AMR_WB
        case GL1_MSG_TCH_WFS:
#endif
            /* Full Rate Speech Frame */
            if( !rpt->hdr.bfi )
            {
                if(rpt->hdr.rx_qual > amr_tch_fr_thresholds[curr_dl_acs[gas_id][rpt->hdr.cmi]])
                {
                    l1_tch_amr_data[gas_id].amr_rx_state = AMR_TCH;

                    frame.dl_info.frame = GL1_MSG_AMR_SPEECH_GOOD;
#ifdef GL1_TCH_AMR_DEBUG
#ifdef FEATURE_GSM_AMR_WB
                    MSG_GERAN_HIGH_1_G("Rcvd (WB=%d)AMR FR SPEECH_GOOD frame from MDSP",
                        (rpt->hdr.chan==(uint16)GL1_MSG_TCH_WFS)?TRUE:FALSE);
#else
                    MSG_GERAN_HIGH_0_G("Rcvd AMR FR SPEECH_GOOD frame from MDSP");
#endif
#endif
                    /*
                        SPEECH_GOOD/SPEECH_DEGRADED has either CMI or CMC
                        based on the phase
                    */
                    cmc_rcvd = ( (cmi_rcvd = (boolean)(rpt->hdr.phase == 1) )==TRUE)?FALSE:TRUE;
#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
                    gl1_msg_amr_pd_update_hard_rxqual(rpt->hdr.rx_qual, gas_id);
#endif
                }
                else
                {
                    if( l1_tch_amr_data[gas_id].amr_rx_state == AMR_DTX )
                    {
                        frame.dl_info.frame = GL1_MSG_AMR_NO_DATA;
#ifdef GL1_TCH_AMR_DEBUG
                        MSG_GERAN_HIGH_0_G("Rcvd FR NO_DATA frame from MDSP");
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                        gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_NO_DATA_RX);
#endif
                    }
                    else
                    {
                        frame.dl_info.frame = GL1_MSG_AMR_SPEECH_DEGRADED;
                        cmc_rcvd = ( (cmi_rcvd = (boolean)(rpt->hdr.phase == 1) )==TRUE)?FALSE:TRUE;
#ifdef GL1_TCH_AMR_DEBUG
                        MSG_GERAN_HIGH_0_G("Rcvd FR SPEECH_DEGRADED frame from MDSP");
#endif
                    }
                }
            }
            else
            {
                /* No FR Data Rx'ed. */
                MSG_GERAN_HIGH_2_G("Rcvd FR NO_DATA FN=%d,ST=%d", GSTMR_GET_FN_GERAN( gas_id),l1_tch_amr_data[gas_id].amr_rx_state);

                if( l1_tch_amr_data[gas_id].amr_rx_state == AMR_DTX )
                {
                    frame.dl_info.frame = GL1_MSG_AMR_NO_DATA;
#ifdef GL1_TCH_AMR_DEBUG
                    MSG_GERAN_HIGH_0_G("Rcvd FR NO_DATA frame from MDSP");
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                    gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_NO_DATA_RX);
#endif
                }
                else
                {
                    cmc_rcvd = ( (cmi_rcvd = (boolean)(rpt->hdr.phase == 1) )==TRUE)?FALSE:TRUE;
                    frame.dl_info.frame = GL1_MSG_AMR_SPEECH_BAD;
#ifdef GL1_TCH_AMR_DEBUG
                    MSG_GERAN_HIGH_0_G("Rcvd FR SPEECH_BAD frame from MDSP");
#endif
                }
                l1_tch_amr_data[gas_id].crc_bad_fr++;
                l1_tch_amr_data[gas_id].total_crc_bad_fr++;
            }
            break;

        default:
            MSG_GERAN_ERROR_0_G("Invalid frame rcvd from MDSP");
            frame.dl_info.frame = GL1_MSG_AMR_NO_DATA;
            break;
        }
    }

    /*
     * If the cmi received is greater than the number of codec modes -1
     * in D/L ACS or cmc received is greater than the number of codec
     * modes in U/L ACS, print an F3 message. Use the previous CMI/CMC
     */
    if(cmi_rcvd)
    {
        if( rpt->hdr.cmi >= curr_dl_num_acm[gas_id] )
        {
            MSG_GERAN_HIGH_2_G("Rcvd CMI %d out of bounds wrt to ACS", rpt->hdr.cmi,
                      0);
        }
        else
        {
            curr_dl_cmi[gas_id] = (uint8)rpt->hdr.cmi;
#ifdef GL1_TCH_AMR_DEBUG
            MSG_GERAN_HIGH_1_G("Rcvd CMI = %d", curr_dl_cmi[gas_id]);
#endif
        }
    }

    if(cmc_rcvd )
    {
        if( rpt->hdr.cmc >= curr_ul_num_acm[gas_id] )
        {
            MSG_GERAN_HIGH_2_G("Rcvd CMC %d out of bounds wrt to ACS", rpt->hdr.cmc,
                      0);
        }
        else
        {
            l1_amr_store_cmc( (uint8)rpt->hdr.cmc, gas_id );

            if(l1_tch_amr_data[gas_id].amr_cmc_rcvd == FALSE )
            {
                l1_tch_amr_data[gas_id].amr_cmc_rcvd = TRUE;
            }

#ifdef GL1_TCH_AMR_DEBUG
            MSG_GERAN_HIGH_1_G("Rcvd CMC = %d", (uint8)rpt->hdr.cmc);
#endif
        }
    }

    /*
     * Now map the curr_dl_cmi received to the vocoder codec mode
     */
    frame.dl_info.mode = curr_dl_acs[gas_id][curr_dl_cmi[gas_id]];

    if(ack_act_counter == 10 )
    {
        l1_tch_amr_data[gas_id].amr_need_to_expire_ack_act_timer = TRUE;
    }

    if( curr_phase[gas_id] == CMI_PHASE_ODD )
    {
        /*
         * If this is a CMI frame, don't give CMC to the vocoder.
         * Note that if CMC is received in this frame because
         * it is a SID_UPDATE or RATSCCH frame, we have saved
         * the received CMC in curr_cmc above and will apply
         * in the next speech frame which should be CMC frame.
         */
    if(l1_tch_amr_data[gas_id].amr_need_to_expire_ack_act_timer)
    {
        /* Initialize curr_cmc, new_cmc to the new icm */
        curr_cmc[gas_id] = new_cmc[gas_id] = new_ul_icm[gas_id];
        ratscch_set_ul_voc_amr_mode_using_new_icm(gas_id);
    }
        /* If amr_rx_phase = 0 then rcvd frame is a CMI frame
         * If amr_rx_phase = 1 then rcvd frame is a CMC frame
     */
        else if ( l1_tch_amr_data[gas_id].amr_rx_phase )
    {
        l1_tch_amr_data[gas_id].amr_cmc_changed = l1_amr_update_cmc(gas_id);

        if( l1_tch_amr_data[gas_id].amr_cmc_changed )
        {
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
            l1_vs_set_ul_amr_mode( curr_ul_acs[gas_id][curr_cmc[gas_id]], gas_id );

            MSG_GERAN_HIGH_3_G("CMC changed curr_cmc=%d, ul acs=%d, gas_id %d",
                     curr_cmc[gas_id], curr_ul_acs[gas_id][curr_cmc[gas_id]],gas_id);
#endif /* FEATURE_MODEM_NO_VOICE_SUPPORT */
            /*
            * This is where we are commanding the vocoder to apply
            * the new codec mode. Log this information, so that
            * we can check when the vocoder actually applies the
            * new codec mode.
            */
            l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.phase = curr_phase[gas_id];
            l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.cmc = curr_cmc[gas_id];
            l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.frame_count = 0;
            l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.valid = TRUE;
        }
    }
    }
    else
    {
        /*
         * If this is a CMI frame, give the CMC to the vocoder.
         * Note that if CMC is received in this frame because
         * it is a SID_UPDATE or RATSCCH frame, we have saved
         * the received CMC in curr_dl_cmc above.
         */
        if(l1_tch_amr_data[gas_id].amr_need_to_expire_ack_act_timer)
        {
            /* Initialize curr_cmc, new_cmc to the new icm */
            curr_cmc[gas_id] = new_cmc[gas_id] = new_ul_icm[gas_id];
            ratscch_set_ul_voc_amr_mode_using_new_icm(gas_id);
        }
        /* If amr_rx_phase = 0 then rcvd frame is a CMI frame
         * If amr_rx_phase = 1 then rcvd frame is a CMC frame
         */
        else if ( !l1_tch_amr_data[gas_id].amr_rx_phase )
        {
            l1_tch_amr_data[gas_id].amr_cmc_changed = l1_amr_update_cmc(gas_id);

            if( l1_tch_amr_data[gas_id].amr_cmc_changed )
            {
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
                l1_vs_set_ul_amr_mode( curr_ul_acs[gas_id][curr_cmc[gas_id]], gas_id );

                MSG_GERAN_HIGH_3_G("CMC changed curr_cmc=%d, ul acs=%d gas_id %d",
                         curr_cmc[gas_id], curr_ul_acs[gas_id][curr_cmc[gas_id]], gas_id);
#endif /* FEATURE_MODEM_NO_VOICE_SUPPORT */
                /*
                 * This is where we are commanding the vocoder to apply
                 * the new codec mode. Log this information, so that
                 * we can check when the vocoder actually applies the
                 * new codec mode.
                 */
                l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.phase = curr_phase[gas_id];
                l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.cmc = curr_cmc[gas_id];
                l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.frame_count = 0;
                l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.valid = TRUE;
            }
        }
    }

    /*--------------------------------------------------------------------------------------
     * Generate CMR only if we did not receive
     * 1. NO_DATA   frame from the MDSP and
     * 2. SID_FIRST frame from the MDSP. SID_FIRST for full rate is not used since when the
     * MDSP decodes SID_FIRST we are looking at the burst metrics for noise. This is because
     * the network stops transmitting after the SID_FIRST block. Hence C/I and other burst
     * metrics for full rate SID_FIRST should not be used.
     *
     * Below fig. shows the interleaving for full rate SID_FIRST. Upper row indicates even
     * bits and lower row odd bits:
     *
     * SP_2 SP_2 SP_2 SP_2    SID_F SID_F SID_F SID_F    NOISE NOISE NOISE NOISE
     * SP_1 SP_1 SP_1 SP_1    SP_2  SP_2  SP_2  SP_2     NOISE NOISE NOISE NOISE
     *                       |                          |                         |
     *                       ^                          ^                         ^
     *                 SP_1 decoded               SP_2 decode                SID_F decoded
     * 3. SID UPDATE with failed CRC.
     *------------------------------------------------------------------------------------*/
    if( frame.dl_info.frame != GL1_MSG_AMR_NO_DATA   &&
        frame.dl_info.frame != GL1_MSG_AMR_SID_FIRST &&
        frame.dl_info.frame != GL1_MSG_AMR_SID_BAD )
    {
        for(i = 0; i < 4; i++ )
        {
            /*
             * Saturate c_over_i before passing to the filter
             */
            if ( l1_tch_amr_data[gas_id].amr_saved_c_over_i[i] > AMR_MAX_C_OVER_I )
            {
               l1_tch_amr_data[gas_id].amr_saved_c_over_i[i] = AMR_MAX_C_OVER_I;
            }
            filter_c_over_i_fr( l1_tch_amr_data[gas_id].amr_saved_c_over_i[i],gas_id );
        }

        gen_codec_mode_request(gas_id);

    }

    /*--------------------------------------------------------------------------
     * LOG MESSAGE METRICS
     *-------------------------------------------------------------------------*/

    /*
     * Fill in amr_status
     */
    amr_status |= (uint16) (((rpt->hdr.phase << 12) |
                             (curr_ul_icm[gas_id] << 10)    |
                             (curr_dl_icm[gas_id] << 8 )    |
                             (curr_ul_cmi[gas_id] << 6)     |
                             (curr_dl_cmi[gas_id] << 4 )    |
                             (curr_cmr[gas_id] << 2 ) |
                             curr_cmc[gas_id]) );

    /*
     * Fill in dl ACS
     */
    for( i = 0; i < curr_dl_num_acm[gas_id]; i++ )
    {
       uint8 dl_amr_mode =0;
#ifdef FEATURE_GSM_AMR_WB
       if (curr_dl_acs[gas_id][i] >= GL1_MSG_AMR_WB_MODE_0660)
       {
           dl_amr_mode = (uint8)curr_dl_acs[gas_id][i] - (uint8)GL1_MSG_AMR_WB_MODE_0660;
       }
       else
       {
           dl_amr_mode = (uint8)curr_dl_acs[gas_id][i];
       }

#endif
       dl_acs |= (uint8)(1 << dl_amr_mode);
    }

    /*
     * Fill in ul ACS
     */
    for( i = 0; i < curr_ul_num_acm[gas_id]; i++ )
    {
       uint8 ul_amr_mode =0;
#ifdef FEATURE_GSM_AMR_WB
       if ((uint8)curr_ul_acs[gas_id][i] >= (uint8)GL1_MSG_AMR_WB_MODE_0660)
       {
           ul_amr_mode = (uint8)curr_ul_acs[gas_id][i] - (uint8)GL1_MSG_AMR_WB_MODE_0660;
       }
       else
       {
           ul_amr_mode = (uint8)curr_ul_acs[gas_id][i];
       }

#endif
       ul_acs |= (uint8)(1 << ul_amr_mode);
    }

    /*
     * Now log the message metrics
     */
    l1_log_message_metrics_amr( (gl1_msg_decode_rpt*)rpt,
                                (uint8)curr_dl_acs[gas_id][curr_dl_cmi[gas_id]],
                                amr_status,
                                dl_acs,
                                ul_acs,
                                (uint16)curr_norm_c_over_i[gas_id], gas_id);

    l1_log_message_metrics_compact_amr  ( (gl1_msg_decode_rpt*) rpt,
                                          (uint8)curr_dl_acs[gas_id][curr_dl_cmi[gas_id]], gas_id );

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
    l1_log_amr_pd_dtx_message_metrics ( (gl1_msg_decode_rpt*)rpt,
                                        (uint8)curr_dl_acs[gas_id][curr_dl_cmi[gas_id]], 
                                        gas_id );
#endif

    {
      eng_mode_amr_info_t amr_info = {0};

      amr_info.ul_chan_mode = (uint8)l1_get_tch_chan_mode(gas_id);
      amr_info.dl_chan_mode = (uint8)l1_get_tch_chan_mode(gas_id);

      for (i = 0; i < MAX_CODEC_MODES; i++)
      {
        amr_info.ul_acs[i] = (uint8)curr_ul_acs[gas_id][i];
        amr_info.dl_acs[i] = (uint8)curr_dl_acs[gas_id][i];

        if (amr_info.ul_acs[i] == GL1_MSG_AMR_MODE_UNDEF)
        {
          amr_info.ul_acs[i] = 0xFF;
        }

        if (amr_info.dl_acs[i] == GL1_MSG_AMR_MODE_UNDEF)
        {
          amr_info.dl_acs[i] = 0xFF;
        }
      }

      if (l1_tch_amr_data[gas_id].amr_rx_state == AMR_DTX)
      {
        amr_info.dl_dtx = TRUE;
      }
      else
      {
        amr_info.dl_dtx = FALSE;
      }

      amr_info.dl_c_over_i = (uint16)curr_norm_c_over_i[gas_id];
      geran_eng_mode_data_write(ENG_MODE_AMR_INFO, (void *)&amr_info, TRUE, gas_id);
    }

    l1_vs_send_amr_packet_to_client(rpt->data, &frame, gas_id);

     /*
     * Update the Counters of RATSCCH handler here. It will
     * have no effect if there is not RATSCCH request pending. Note that
     * the RATSCCH request should take effect in the N+12 speech frame.
     */
    ratscch_update_req_activation_counter(gas_id);

#ifdef FEATURE_GSM_DYNAMIC_SAIC
    if(gl1_hw_feature_disabled_saic(gas_id) != TRUE)
    {
      MSG_GERAN_LOW_3_G(" RXQUAL %d saic d/e %d state %d",rpt->hdr.rx_qual,gl1_msg_get_saic_state(gas_id),
                                              l1_dedicated_data_ptr->l1_saic_dis_ena_state);

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
      MSG_GERAN_HIGH_2_G("TCH AMR FR Hard RxQUAL:%d,FN:%d",rpt->hdr.rx_qual,gl1_get_FN(gas_id));
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
    MSG_GERAN_MED_1_G("Update: rxqual_full tch = %d",rpt->hdr.rx_qual);
#endif

#ifdef TCH_PROFILING
#ifdef FEATURE_GSM_AMR_WB
    if (rpt->hdr.bfi && ( ((gl1_msg_chan_type)rpt->hdr.chan != GL1_MSG_FACCH_AFS)
                        ||((gl1_msg_chan_type)rpt->hdr.chan != GL1_MSG_FACCH_WFS) ) )
#else
    if (rpt->hdr.bfi && (gl1_msg_chan_type)rpt->hdr.chan != GL1_MSG_FACCH_AFS)
#endif
    {
       l1_tch_amr_data[gas_id].total_bfi_cnt_fr++;
       l1_tch_amr_data[gas_id].bfi_cnt_fr++;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
       gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_BFI_RCVD);
#endif
    }

    l1_tch_amr_data[gas_id].total_crc_cnt_fr++;
    if( ++l1_tch_amr_data[gas_id].crc_cnt_fr >= 100 )
    {
        MSG_6(MSG_SSID_DFLT, MSG_LEGACY_MED,
              "CRC errors: %d/100, %d/%d  (BFI: %d/100, %d/%d)",
              l1_tch_amr_data[gas_id].crc_bad_fr, l1_tch_amr_data[gas_id].total_crc_bad_fr, l1_tch_amr_data[gas_id].total_crc_cnt_fr, l1_tch_amr_data[gas_id].bfi_cnt_fr, l1_tch_amr_data[gas_id].total_bfi_cnt_fr,
              l1_tch_amr_data[gas_id].total_crc_cnt_fr);
        l1_tch_amr_data[gas_id].crc_cnt_fr=0;
        l1_tch_amr_data[gas_id].crc_bad_fr = 0;
        l1_tch_amr_data[gas_id].bfi_cnt_fr = 0;
    }
#endif


}/* Suppress warning "crc_bad not accessed" */ /*lint !e550*/

/*===========================================================================
FUNCTION amr_tch_metrix_callback_fr

DESCRIPTION
   This function is called when the burst metrics has been decoded. The data
   is used to generate the U/L Codec Mode Request amongst other things

DEPENDENCIES
   None.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void amr_tch_metrics_callback_fr( gl1_msg_metrics_rpt *rpt[], uint16 chan, gas_id_t gas_id )
{
    l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

    int i;

    if(!rpt)
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
        return;
    }

    /* Store rx power over the current blocks for RXLEV_VAL computation later */
    gl1_store_block_rx_power ( rpt, 4, gas_id  );

    if( l1_get_serv_pwr_meas_permitted(gas_id) )
    {
        /* Assuming a FULL RATE channel type here */
        for(i=0; i<4; i++)
        {
            /* accumulate frames in which there is a guaranteed burst */
            if( (gl1_msg_chan_type)chan == GL1_MSG_SID_UPDATE_FS )
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
                     l1_serving_cell_meas_ptr->divrx_power_sub  = 0;
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
                 l1_serving_cell_meas_ptr->divrx_power = 0;
                 }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
            }
            else
            {
#ifdef FEATURE_GSM_RX_DIVERSITY
               MSG_GERAN_LOW_3_G("Excluding ARFCN %d RSSI %d from from PRx_RXLEV %d from DRx_RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4,rpt[i]->pwr_dBm_x16_divrx>>4);
#else
               MSG_GERAN_LOW_2_G("Excluding ARFCN %d RSSI %d from RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4);
#endif
            }
        }
    }



    /* Only update the AFC on SID_UPDATE  if the UE is DL DTXing */
    /* The  AMR_TCH test pick up all but SID_UPDATE, and NO_DATA */
    /* For AMR/FS the SID_FIRST is not set correctly by the mDSP */
    if( l1_tch_amr_data[gas_id].amr_rx_state == AMR_TCH ||
        (gl1_msg_chan_type)chan == GL1_MSG_FACCH_AFS ||
        (gl1_msg_chan_type)chan == GL1_MSG_SID_UPDATE_FS )
    {
      /* Update AFC/TT */
      for(i = 0; i < 4; i++)
      {
        /* If SAIC is used in a burst the mDSP sets bit 14 of */
        /* the misc_flag true. Additionally, for AFC tracking */
        /* the flag selects the specific GMSK SAIC threshold. */
        if ( gl1_msg_saic_in_burst( rpt[i]->misc_flags) )
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
    }

    /* Compute mean c_over_i to be fed to the codec mode adaptor */
    for(i = 0; i < 4; i++ )
    {
#ifdef FEATURE_GSM_DYNAMIC_SAIC
    /* For dynamic SAIC, SAIC is only disabled unitl Rx Qual is not max */
    /* so if SAIC is disabled set C/I to max */
        if( (gl1_hw_feature_disabled_saic(gas_id) != TRUE) && (gl1_msg_get_saic_state(gas_id) != TRUE) )
        {
            l1_tch_amr_data[gas_id].amr_saved_c_over_i[i] =  AMR_MAX_C_OVER_I;
        }
        else
#endif
        {
            l1_tch_amr_data[gas_id].amr_saved_c_over_i[i] =  rpt[i]->c_over_i;
        }
    }

    /* LOG BURST METRICS */
    l1_log_burst_metrics ( (gl1_msg_chan_type)chan, rpt, gas_id  );

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
    l1_log_amr_pd_dtx_burst_metrics ( (gl1_msg_chan_type)chan, rpt, gas_id  );
#endif

#if  defined (DEBUG_FEATURE_GSM_R_FACCH)
    #error code not present
#endif

}

    #ifdef DED_LOG
#error code not present
#endif

/*===========================================================================
FUNCTION l1_amr_store_cmc

DESCRIPTION
   This function stores the newly commanded CMC by the network. It will be
   used to update the current CMC at the appropriate frame.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void l1_amr_store_cmc( uint8 rcvd_cmc, gas_id_t gas_id )
{
   new_cmc[gas_id] = rcvd_cmc;
}

/*===========================================================================
FUNCTION l1_amr_update_cmc

DESCRIPTION
   This function updates the CMC before it is sent to the vocoder. It changes
   the current CMC towards the newly commanded CMC one step at a time.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE  - current CMC has been updated
  FALSE - current CMC remains same

SIDE EFFECTS
  None
===========================================================================*/
static boolean l1_amr_update_cmc(gas_id_t gas_id)
{
   boolean codec_mode_changed = TRUE;

   if ( (new_cmc[gas_id] > curr_cmc[gas_id]) &&
             (new_cmc[gas_id] <= curr_ul_num_acm[gas_id] - 1) )
   {
      curr_cmc[gas_id]++;
   }
   else if ( (new_cmc[gas_id] < curr_cmc[gas_id]) &&
             (curr_cmc[gas_id] > 0) )
   {
      curr_cmc[gas_id]--;
   }
   else
   {
      codec_mode_changed = FALSE;
   }
   return codec_mode_changed;
}

/*===========================================================================
FUNCTION l1_amr_update_cmr

DESCRIPTION
   This function updates the CMR before sending it out on the U/L. This
   function is only called when there is something to be sent out. It is
   not called when there is no transmission.

   Only update CMR if phase is ODD or we need to transmit a RATSCCH or
   SID_UPDATE frames

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void l1_amr_update_cmr( uint8 phase, boolean ratscch_sid_update_tx, gas_id_t gas_id )
{

#ifdef GL1_TCH_AMR_DEBUG
    MSG_GERAN_HIGH_3_G("phase %d tx ratscch_sid_update %d computed_cmr %d",phase,ratscch_sid_update_tx,computed_cmr[gas_id]);
#endif
    if( (phase == 0) || (ratscch_sid_update_tx == TRUE) )
    {
        if( (computed_cmr[gas_id] > curr_cmr[gas_id]) &&
            (computed_cmr[gas_id] <= curr_dl_num_acm[gas_id] - 1) )
        {
            curr_cmr[gas_id]++;
        }
        else if( ( computed_cmr[gas_id] < curr_cmr[gas_id] ) &&
                 ( curr_cmr[gas_id] > 0 ) )
        {
            curr_cmr[gas_id]--;
        }
    }
}

/*===========================================================================
FUNCTION gl1_amr_setup_loopback_I

DESCRIPTION
   This function is used to setup loopback I. It resets any pending RATSCCH
   procedures and sets the D/L phase to ODD.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_amr_setup_loopback_I( gas_id_t gas_id )
{
    ratscch_reset_pending_proc(gas_id);
    l1_tch_amr_data[gas_id].amr_loopback_I_prev_phase = curr_phase[gas_id];
    curr_phase[gas_id] = CMI_PHASE_ODD;
    l1_tch_amr_data[gas_id].amr_loopback_I_in_progress = TRUE;
}

/*===========================================================================
FUNCTION gl1_amr_reset_loopback_I

DESCRIPTION
  This function is used to restore the state prior to closing Loopback I.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_amr_reset_loopback_I( gas_id_t gas_id )
{
    l1_tch_amr_data[gas_id].amr_loopback_I_in_progress = FALSE;
    curr_phase[gas_id] = l1_tch_amr_data[gas_id].amr_loopback_I_prev_phase;
}

/*===========================================================================
FUNCTION L1_CONTROL_AMR_HR

DESCRIPTION
   This function controls the AMR half traffic channel.

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
boolean  L1_control_amr_hr ( dedicated_data_T       *ded_dtr_ptr,
                             channel_information_T  *chnl1_info_ptr,
                             boolean                apply_freq_redef, gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    uint8 FNmod13_HR;
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION
    static uint8 no_vocoder_change_after_handover;
#endif

    /*
     * Initialize global flags.
     */
    l1_tch_amr_data[gas_id].amr_block_start = FALSE;
    l1_tch_amr_data[gas_id].amr_prepare_msg = FALSE;
    l1_tch_amr_data[gas_id].amr_access_burst_scheduled = FALSE;
    l1_tch_amr_data[gas_id].amr_idle_status  = FALSE;
    l1_tch_amr_data[gas_id].amr_sacch_ul_block_start = FALSE;

    amr_channel[gas_id] = HALF_RATE_TRAFFIC;

    if( !ded_dtr_ptr || !chnl1_info_ptr )
    {
        MSG_GERAN_ERROR_2_G("Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
                  ded_dtr_ptr, chnl1_info_ptr);
        return FALSE;
    }

    /*
     *  agc pointer
     */
    l1_tch_amr_data[gas_id].amr_agc_ptr = &ded_dtr_ptr->c0_agc;
#ifdef FEATURE_GSM_RX_DIVERSITY
    l1_tch_amr_data[gas_id].amr_agc_ptr_divrx = &ded_dtr_ptr->agc_divrx;
#endif   

      switch( l1_tskisr_blk->sub_sub_state )
      {

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM

      case  L1_EDTM_INIT:

#endif
#endif

      case L1_DED_INIT :
        l1_acch_nv_flag_initialise(gas_id);
        /*
         * Reset these global variables in this state. Once
         * intialized this variable will maintain it's
         * state as long as we are in RUNNING state.
         */
        l1_tch_amr_data[gas_id].amr_prev_block_was_facch = FALSE;
        l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter = 0;
        l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.valid = FALSE;
        l1_tch_amr_data[gas_id].amr_sid_first_pending = l1_tch_amr_data[gas_id].amr_sid_update_pending = FALSE;
        l1_tch_amr_data[gas_id].amr_vfr_aligned = FALSE;
        l1_tch_amr_data[gas_id].amr_cmc_rcvd = FALSE;
        l1_tch_amr_data[gas_id].amr_cmc_changed = FALSE;
        l1_tch_amr_data[gas_id].amr_first_rx_block = TRUE;
        l1_tch_amr_data[gas_id].amr_loopback_I_in_progress = FALSE;
        l1_tch_amr_data[gas_id].amr_need_to_expire_ack_act_timer = FALSE;
#ifdef GL1_TCH_AMR_DEBUG
        l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
#endif
        l1_tch_amr_data[gas_id].amr_hr_subchannel = chnl1_info_ptr->subchannel;
        l1_tch_amr_data[gas_id].amr_afc_correction_frame = chnl1_info_ptr->TN % 2  ? 102 : 89;
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION
        no_vocoder_change_after_handover = FALSE;
#endif
        /*
         * Re-initialize this variable to FALSE if we are in DED_INIT.
         * It's a static variable and maintains the previous state if
         * we are not in DED_INIT but in DED_RUNNING state.
         */
        l1_tch_amr_data[gas_id].sacch_tx_started_hr = FALSE;

        /* Save TSC for logging */
        l1_tch_amr_data[gas_id].amr_current_TSC = chnl1_info_ptr->TSC;

        /*
         * Convert channel_type and channel_mode to CHT parameters for
         * the DSP
         */
        l1_tch_amr_data[gas_id].amr_channel_type = GL1_MSG_TCH_AHS;
        MSG_GERAN_HIGH_0_G("Tch init: AMR HR voice");
        l1_tch_amr_data[gas_id].amr_voc_type = GL1_MSG_AMR;

        /*
         * Initialize the AMR rx and tx state machines
         */
        l1_tch_amr_data[gas_id].amr_rx_state = l1_tch_amr_data[gas_id].amr_curr_tx_state = l1_tch_amr_data[gas_id].amr_new_tx_state = AMR_INIT;

        /*
         * Init the AMR phase to ODD
         */
        curr_phase[gas_id] = CMI_PHASE_ODD;

        /*
         * Initialize all the codec related variables
         */
        curr_cmc[gas_id] = new_cmc[gas_id] = curr_cmr[gas_id] = computed_cmr[gas_id] = curr_ul_cmi[gas_id] = curr_dl_cmi[gas_id] = 0;
        curr_norm_c_over_i[gas_id] = 0;
        prev_c_over_i_filter_output[gas_id] = 0;

        /*
         * Look at the multirate IE and set up the initial_code_mode
         * acs etc.
         */
        l1_amr_process_multirate_ie(ded_dtr_ptr, gas_id);

#ifdef FEATURE_VAMOS_II
         /* If RR indicates that UE has to operate is VAMOS 2 mode and if TSC set 2 is allocated,
          * the SACCH position is shifted as per 3GPP 45.002, section 7, Table 1a. 
          */
        if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3))
        {
          l1_amr_setup_time_slot_params_vamos2_hr(chnl1_info_ptr, gas_id);
        }
        else
#endif /*FEATURE_VAMOS_II*/
        {
            /*
             * Set up parameters that depend on the timeslot used
            */
            l1_amr_setup_time_slot_params_hr( chnl1_info_ptr, gas_id );
        }

#ifdef FEATURE_GSM_COEX_SW_CXM
        /* Notify the arbitrator of the channel assignment */
        garb_intf_notify_cs_assignment(
           (l1_tskisr_blk->l1_state == L1_DTM_MODE ? GL1_DEFS_TN_2 : GL1_DEFS_TN_0),
           chnl1_info_ptr->frequency_list,
           chnl1_info_ptr->subchannel,
           FALSE,
           TRUE,
           l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame+1,
           l1_tch_amr_data[gas_id].amr_idle_frame,
           l1_tch_amr_get_arfcns,
          &l1_tch_amr_data[gas_id].amr_coex_request_tx,
           l1_tskisr_blk->l1_state,
           gas_id
        );
#endif /* FEATURE_GSM_COEX_SW_CXM */

        /*
         * Pass idle frame to SCE -- for ncell scheduling
         */
        l1_sc_set_tch_idle_frame (l1_tch_amr_data[gas_id].amr_idle_frame, gas_id );

        /*
         * Setup message layer
         */
        l1_amr_setup_message_layer_hr(ded_dtr_ptr, chnl1_info_ptr, gas_id);

        /* Install callback to trigger serving cell measurement reporting. */
        l1_sci_monscan_set_next_period_ded_cb( L1_send_dedicated_report, gas_id );

        MSG_GERAN_MED_1_G("Tch msg init FNmod42432 = %d", frame_counters[gas_id].FNmod42432);

        l1_amr_schedule_monitors_fr_hr(gas_id);

        FNmod13_HR = ( frame_counters[gas_id].FNmod26 - chnl1_info_ptr->subchannel + 13 ) % 13 ;

         if ( FNmod13_HR >= 9  ||
              FNmod13_HR <= 0 )
         {
             /* VFR will be aligned to frame 4 and 20000 of 13M count*/
             l1_tch_amr_data[gas_id].amr_vfr_align_frame_hr = 3;
         }
         else if ( FNmod13_HR >= 1  &&
                   FNmod13_HR <= 4 )
         {
             /* VFR will be aligned to frame 8 and 40000 of 13M count*/
             l1_tch_amr_data[gas_id].amr_vfr_align_frame_hr = 7;
         }
         else
         {
             /* VFR will be aligned to frame 0 and 0 of 13M count*/
             l1_tch_amr_data[gas_id].amr_vfr_align_frame_hr = 12;
         }

        if (( l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_STARTED ) ||
             ( l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_READY ))
         {
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION
            /*If no change in vocoder id, do not stop the vocoder and align VFR in immediate possible frame */
            if( l1_vs_change_in_voc(l1_tch_amr_data[gas_id].amr_channel_type,l1_tch_amr_data[gas_id].amr_voc_type, gas_id) )
            { 
               l1_vs_stop_client(gas_id);
            }
            else
            {
               no_vocoder_change_after_handover = TRUE;

               if(  FNmod13_HR == 3   ||
                    FNmod13_HR == 7   ||
                    FNmod13_HR == 12   )
               {
                 gl1_msg_align_vfr( TRUE, gas_id );
                 l1_tch_amr_data[gas_id].amr_vfr_aligned = TRUE;
                 MSG_GERAN_MED_1_G("Aligning Real VFR FNmod13=%d",frame_counters[gas_id].FNmod13);
               }
               else if ( FNmod13_HR >= 12  ||
                         FNmod13_HR <= 2 )
               {
                 /* VFR will be aligned to frame 4 and 20000 of 13M count*/
                 l1_tch_amr_data[gas_id].amr_vfr_align_frame_hr = 3;
               }
               else if ( FNmod13_HR >= 3  &&
                         FNmod13_HR <= 6 )
               {
                 /* VFR will be aligned to frame 8 and 40000 of 13M count*/
                 l1_tch_amr_data[gas_id].amr_vfr_align_frame_hr = 7;
               }
               else
               {
                /* VFR will be aligned to frame 0 and 0 of 13M count*/
                l1_tch_amr_data[gas_id].amr_vfr_align_frame_hr = 12;
               }
            }
#else
           l1_vs_stop_client(gas_id);
#endif
         }

        l1_tskisr_blk->sub_sub_state  = L1_DED_RUNNING;

/* Exclude as PMIC signaling is only needed for Solo.
** Mustang uses RTR6285 and diff PMIC
*/


/* For Poseidon enable cipher clock for mDSP */
/* For Poseidon enable cipher clock for mDSP */
        if (gl1_msg_is_ciphering_enabled(gas_id))
        {
          MSG_GERAN_HIGH_0_G("DG: AHR: cipher required.");
          gl1_hw_cipher_clock_required(TRUE, gas_id);
        }

        /* End the Channel Init here so the DED_RUNNING is run on the next frame
        tick, otherwise this may result in possible ISR Overrun. */
        l1_amr_start_ftn_fr_hr(gas_id);

        break;

      case L1_DED_RUNNING:
        /*
          If frequency redefinition occurs midway through a SACCH block,
          we need to recompute the SACCH ARFCNs based on the new parameters
          and inform the msg layer.  We don't do this for the TCH because
          they are contiguous and this scenario is extremely unlikely.  We
          need to recompute because the ARFCNs are pre-computed before the
          block starts
        */
        if( apply_freq_redef )
        {
            l1_amr_recompute_sacch_arfcns_fr_hr( ded_dtr_ptr, gas_id );
        }

        /* Check up front to prevent Tx TCH from being scheduled in sacch - 1 */
        if( L1_more_abs_to_send(ded_dtr_ptr, FACCH_H, gas_id) )
        {
            l1_tch_amr_data[gas_id].amr_access_burst_scheduled = TRUE;
        }

        /*
         * SACCH Processing
         */
        if(  frame_counters[gas_id].FNmod26 == l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame )
        {
            if( l1_tskisr_blk->main_command != L1_DEDICATED_STOP )
            {
                if( ( frame_counters[gas_id].FNmod104 == l1_tch_amr_data[gas_id].amr_send_sacch) ||
                    ( !l1_tch_amr_data[gas_id].sacch_tx_started_hr ) )
                {
                    if(l1_amr_process_sacch_fr_hr(ded_dtr_ptr, gas_id) == TRUE)
                    {
                       /*
                        Once an incomplete or complete SACCH block is started,
                        sacch_tx_started is set to TRUE and is reset only
                        when we get onto another channel.  sacch_ul_block
                        start indicates the start of an incomplete or complete
                        block.  The old indicator was ticking the handler only
                        on a SACCH block boundary
                       */
                       l1_tch_amr_data[gas_id].amr_sacch_ul_block_start = TRUE;
                       l1_tch_amr_data[gas_id].sacch_tx_started_hr = TRUE;
                    }
                }
            }
        }
        else if( frame_counters[gas_id].FNmod26 != l1_tch_amr_data[gas_id].amr_idle_minus_one_frame )
        {
            /*
             * If it is not sacch - 1 frame and is not idle - 1 frame,
             * then see if have to process a RACH.
             */
            if( l1_tskisr_blk->main_command != L1_DEDICATED_STOP )
            {
                l1_amr_process_rach_hr(ded_dtr_ptr, chnl1_info_ptr, gas_id);
            }


            /*
             * If it is not sacch - 1 frame and is not idle - 1 frame, check
             * to see if it is an idle frame.
             */
            if( frame_counters[gas_id].FNmod26 == l1_tch_amr_data[gas_id].amr_idle_frame )
            {
                l1_amr_process_idle_fr_hr(gas_id);
            }

            /*
             * If it is not sacch - 1 frame and is not idle - 1 frame, check
             * to see if it is an idle - 1 - 1 frame.
             */
            if( frame_counters[gas_id].FNmod26 == (l1_tch_amr_data[gas_id].amr_idle_minus_one_frame - 1) )
            {
                l1_sci_tick_receive(gas_id);
            }

            /*
             * If it is not sacch - 1 frame and is not idle - 1 frame, check
             * to see if it is an idle - 1 - 3 frame.
             */
            if( frame_counters[gas_id].FNmod26 == ( l1_tch_amr_data[gas_id].amr_idle_minus_one_frame - 3) )
            {
                l1_amr_process_sce_pwr_data_fr_hr(gas_id);
            }
        }

        /* Check if the VFR needs aligning */
        if( frame_counters[gas_id].FNmod13 == l1_tch_amr_data[gas_id].amr_vfr_align_frame_hr &&
            !l1_tch_amr_data[gas_id].amr_vfr_aligned )
        {
          /* Check Vocoder is in the correct state before changing VFR */
          if ( (l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_OPENED)
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION			  
			  || (TRUE == no_vocoder_change_after_handover)
#endif
		     )
          {
            gl1_msg_align_vfr( TRUE, gas_id );
            l1_tch_amr_data[gas_id].amr_vfr_aligned = TRUE;
            MSG_GERAN_MED_1_G("Aligning Real VFR FNmod13=%d",frame_counters[gas_id].FNmod13);
          }
          else
          {
          
            MSG_GERAN_MED_1_G("Vocoder is not stopped, aligning VFR postponed FNmod13=%d",
                                                                       frame_counters[gas_id].FNmod13);
            /* If vocoder was not stopped, calculate when is the 
             * next oportunity to align the vocoder */
            switch (l1_tch_amr_data[gas_id].amr_vfr_align_frame_hr)
            {
              case 3:
                l1_tch_amr_data[gas_id].amr_vfr_align_frame_hr  = 7;
                break;

              case 7:
                l1_tch_amr_data[gas_id].amr_vfr_align_frame_hr  = 12;
                break;

              case 12:
                l1_tch_amr_data[gas_id].amr_vfr_align_frame_hr  = 3;
            }
          }
        }

        /* Check if vocoder needs to be enabled */
        if( l1_vs_get_voice_server_state(gas_id) == GL1_VOICE_SERVER_OPENED &&
            l1_tch_amr_data[gas_id].amr_vfr_aligned )
        {
          MSG_GERAN_HIGH_1_G("Enabling vocoder FNmod13=%d",frame_counters[gas_id].FNmod13);
          l1_vs_start_client(l1_tch_amr_data[gas_id].amr_channel_type,
                             l1_tch_amr_data[gas_id].amr_voc_type, gas_id);
        }

        /*
         * Now process the traffic frame.
         */
        l1_amr_process_tch_hr( ded_dtr_ptr, chnl1_info_ptr, gas_id );

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
        if( frame_counters[gas_id].FNmod104 == l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 - 1 )
        {
            l1_sci_monscan_next_period_ded(gas_id);
        }

        /*
         *   TT / AFC correction
         */
        if( frame_counters[gas_id].FNmod104 == l1_tch_amr_data[gas_id].amr_afc_correction_frame )
        {
            gl1_msg_dch_tt_adjust(gas_id);
        }

        /* Don't abort till the extra TCH block to complete the FACCH has been
           sent out
        */
        if( l1_tskisr_blk->main_command == L1_DEDICATED_STOP )
        {
            l1_dedicated_data_ptr->no_of_ab_to_send = 0;

            if( !l1_tch_amr_data[gas_id].amr_prev_block_was_facch )
            {
               /* Abort after FTN will save us a frame during channel changes
                * since we can abort as soon as NPL1 is done aborting.
                * Important from an audio perspective
                */
                l1_amr_tch_abort_hr(gas_id);
            }
        }

         /* If amr_idle_status flag is set to ture then TCH will be aborted. While running Tch abort no need to process FTN*/
         if( l1_tch_amr_data[gas_id].amr_idle_status == FALSE)
         {
           /* FTN processing  */
           l1_amr_start_ftn_fr_hr(gas_id);
         }

        break;

      default:
        break;
      }  /* of switch */


    return(l1_tch_amr_data[gas_id].amr_idle_status);

}  /* end of function */


/*===========================================================================
FUNCTION l1_amr_setup_time_slot_params_hr

DESCRIPTION
   This function sets up the time slot specific parameters for AMR Half Rate
   traffic channel.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_setup_time_slot_params_hr(channel_information_T *chnl1_info_ptr,
                                             gas_id_t gas_id)
{
    if(!chnl1_info_ptr)
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
    switch( (chnl1_info_ptr->TN) & 7 )
    {
    case 0:
    case 1:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame =
        ( 11 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame =
        ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_frame =
        ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 =
        ( 103 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
        l1_tch_amr_data[gas_id].amr_send_sacch = ( 11 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
        break;

    case 2:
    case 3:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame =
        ( 11 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame =
        ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_frame =
        ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 =
        ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
        l1_tch_amr_data[gas_id].amr_send_sacch= ( 37 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
        break;

    case 4:
    case 5:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame =
        ( 11 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame =
        ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_frame =
        ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 =
        ( 51 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
        l1_tch_amr_data[gas_id].amr_send_sacch= ( 63 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
        break;

    case 6:
    case 7:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame =
        ( 11 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame =
        ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_frame = ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 =
        ( 77 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
        l1_tch_amr_data[gas_id].amr_send_sacch= ( 89 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
        break;

    default:
        break;

    }
    
	L1_set_reporting_period(l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1, 104, gas_id);

}


#ifdef FEATURE_VAMOS_II
/*===========================================================================
FUNCTION l1_amr_setup_time_slot_params_vamos2_hr

DESCRIPTION
   This function sets up the time slot specific parameters for AMR Half Rate
   traffic channel in case of VAMOS2 is enabled and TSC set 2 is allocated.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_setup_time_slot_params_vamos2_hr(channel_information_T *chnl1_info_ptr, gas_id_t gas_id )
{
    if(!chnl1_info_ptr)
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
    switch( (chnl1_info_ptr->TN) & 7 )
    {
    case 0:
    case 1:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame =
        ( 12 + ( chnl1_info_ptr->subchannel * 11) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame =
        ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_frame =
        ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 =
        ( 103 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
        l1_tch_amr_data[gas_id].amr_send_sacch = ( 12 + ( chnl1_info_ptr->subchannel * 11) ) % 104;
        break;

    case 2:
    case 3:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame =
        ( 12 + ( chnl1_info_ptr->subchannel * 11) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame =
        ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_frame =
        ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 =
        ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
        l1_tch_amr_data[gas_id].amr_send_sacch= ( 38 + ( chnl1_info_ptr->subchannel * 11) ) % 104;
        break;

    case 4:
    case 5:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame =
        ( 12 + ( chnl1_info_ptr->subchannel * 11) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame =
        ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_frame =
        ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 =
        ( 51 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
        l1_tch_amr_data[gas_id].amr_send_sacch = ( 64 + ( chnl1_info_ptr->subchannel * 11) ) % 104;
        break;

    case 6:
    case 7:
        l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame =
        ( 12 + ( chnl1_info_ptr->subchannel * 11) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_minus_one_frame =
        ( 24 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_idle_frame = ( 25 + ( chnl1_info_ptr->subchannel * 13) ) % 26;
        l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 =
        ( 77 + ( chnl1_info_ptr->subchannel * 13) ) % 104;
        l1_tch_amr_data[gas_id].amr_send_sacch= ( 90 + ( chnl1_info_ptr->subchannel * 11) ) % 104;
        break;

    default:
        break;

    }
}
#endif /*FEATURE_VAMOS_II*/

/*===========================================================================
FUNCTION l1_amr_setup_message_layer_hr

DESCRIPTION
   This function sets up the Message layer for AMR Half Rate.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_setup_message_layer_hr ( dedicated_data_T       *ded_dtr_ptr,
                                            channel_information_T  *chnl1_info_ptr, gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    gl1_msg_tx_cb_type     tx_cb;
    gl1_msg_rx_cb_type     rx_cb;

    if(!ded_dtr_ptr || ! chnl1_info_ptr )
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

    gl1_msg_init_tch_amr((gl1_msg_chan_type)l1_tch_amr_data[gas_id].amr_channel_type, l1_tch_amr_data[gas_id].amr_voc_type,gas_id);

    tx_cb.data_cb    = amr_tch_tx_callback_fr_hr;
    tx_cb.metrics_cb = NULL;
    tx_cb.log_cb     = NULL;
    tx_cb.abort_cb   = NULL;
    rx_cb.metrics_cb = NULL;
    rx_cb.data_cb    = amr_tch_rx_callback_hr;
    rx_cb.tch_metrics_cb = amr_tch_metrics_callback_hr;
#ifdef DED_LOG
    #error code not present
#else
    rx_cb.log_cb     = NULL;
#endif
    rx_cb.abort_cb   = NULL;
    gl1_msg_cfg_ul_tch_amr_fr_hr(chnl1_info_ptr->TSC, &tx_cb ,gas_id);

    /*
       FACCH callback...
    */
    tx_cb.data_cb    = amr_facch_tx_callback_fr_hr;
    gl1_msg_cfg_ul_facch_amr_fr_hr(chnl1_info_ptr->TSC, &tx_cb,gas_id );
    gl1_msg_cfg_dl_tch_facch_amr_fr_hr(chnl1_info_ptr->TSC, &rx_cb,gas_id );

    /*
       RATSCCH callback...
    */
    tx_cb.data_cb    = amr_ratscch_tx_callback_fr_hr;
    gl1_msg_cfg_ul_ratscch_amr_fr_hr(chnl1_info_ptr->TSC, &tx_cb,gas_id );

    /*
       SID_FIRST callback...
    */
    tx_cb.data_cb    = amr_sid_first_tx_callback_fr_hr;
    gl1_msg_cfg_ul_sid_first_amr_fr_hr(chnl1_info_ptr->TSC, &tx_cb ,gas_id);

    /*
       SID_UPDATE callback...
    */
    tx_cb.data_cb    = amr_sid_update_tx_callback_fr_hr;
    gl1_msg_cfg_ul_sid_update_amr_fr_hr(chnl1_info_ptr->TSC, &tx_cb ,gas_id);

    /*
       SACCH callback....
    */
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

    gl1_msg_cfg_ul_sacch(chnl1_info_ptr->TSC, &tx_cb, gas_id);
    gl1_msg_cfg_dl_sacch(chnl1_info_ptr->TSC, &rx_cb, gas_id);

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

    gl1_msg_dch_afc_tt_init( chnl1_info_ptr->frequency_list.channel[0],gas_id);

    l1_tch_amr_data[gas_id].amr_hr_facch_tx_in_progress = 0;
    l1_tch_amr_data[gas_id].amr_hr_ratscch_tx_in_progress = 0;
    l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress = 0;
    l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress = 0;

    /* Precompute TXLEVS for first reporting period.  Transmit at the
        command power level immediately as soon as we jump onto the new
        channel. ms_power_level_to_use is initially set to ms_power_level_in_use
        ,so L1_precompute_txlev will not step up/step down the power
        The 2dB step algo kicks in only after the next power
        level change commanded by the base station
    */

    L1_precompute_txlev ( ded_dtr_ptr->ms_power_level_to_use,
                          ded_dtr_ptr->ms_power_level_in_use,
                          104,
                          chnl1_info_ptr->frequency_list.channel[0],
                          gas_id );
}

/*===========================================================================
FUNCTION l1_amr_process_rach_hr

DESCRIPTION
   This function process the AMR Half Rate RACH channel

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_process_rach_hr (dedicated_data_T *ded_dtr_ptr,
                                    channel_information_T  *chnl1_info_ptr, gas_id_t gas_id )
{
    int i = 0;
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    if(!ded_dtr_ptr || !chnl1_info_ptr)
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
    if( l1_tch_amr_data[gas_id].amr_access_burst_scheduled  )
    {
        /* Transmit only on the assigned subchannel.  Remember that
           the actual burst gets sent out in the next frame
        */
        if( ( (chnl1_info_ptr->subchannel == 0) &&
              ( ( ( frame_counters[gas_id].FNmod13 )%2 == 1) || ((frame_counters[gas_id].FNmod13) == 12) ) &&
              ( ( frame_counters[gas_id].FNmod13 ) != 11 )
            ) ||
            ( (chnl1_info_ptr->subchannel == 1) &&
              ( ((frame_counters[gas_id].FNmod13 )%2 == 0) || ((frame_counters[gas_id].FNmod13) == 0 )) &&
              ( (frame_counters[gas_id].FNmod13) != 12 )
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
          L1_get_ARFCNS
          ( 1, 2, 4, l1_tch_amr_data[gas_id].amr_arfcns,
            &ded_dtr_ptr->channel_specification.channel_info_1_before,
            &ded_dtr_ptr->channel_specification.channel_info_1_after,
            &ded_dtr_ptr->new_channel_info, gas_id);
          /*
             Transmit handover access bursts. Single burst, so just
             pass in the first ARFCN in the block
          */
          gl1_msg_tx_rach
          (  l1_tch_amr_data[gas_id].amr_arfcns[0],
               l1_dedicated_data_ptr->ms_power_level_to_use,
               0,
               l1_dedicated_data_ptr->access_burst_content,
               gas_id);

          for( i = 0; i < 4; i++ )
          {
                l1_tch_amr_data[gas_id].amr_txlev[i] = l1_dedicated_data_ptr->ms_power_level_to_use;
          }

          /* Log tx burst metrics */
          l1_log_transmit_burst_metrics
            ( GL1_MSG_RACH,
              1, 1, 1,
              l1_tch_amr_data[gas_id].amr_arfcns,
              l1_tch_amr_data[gas_id].amr_txlev,
              l1_tch_amr_data[gas_id].amr_current_TSC,
              0, gas_id );


        }
    }
}

/*===========================================================================
FUNCTION l1_amr_process_tch_hr

DESCRIPTION
   The function processes the AMR Half Rate Traffic Channel. It implements
   the TX side state machine to resolve conflict between FACCH, RATSCCH and
   TCH frames giving priority to FACCH, RATSCCH and TCH is decreasing order.
   This function is also responsible for scheduling the frames.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_process_tch_hr( dedicated_data_T      *ded_dtr_ptr,
                                   channel_information_T *chnl1_info_ptr, gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    if ( !ded_dtr_ptr || !chnl1_info_ptr )
    {
        MSG_GERAN_ERROR_2_G( "Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
                   ded_dtr_ptr, chnl1_info_ptr);

        return;
    }

#ifdef FEATURE_VAMOS_II

    if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3) && (chnl1_info_ptr->subchannel == 0))
    {
	  if( (frame_counters[gas_id].FNmod13 == 3)||
          (frame_counters[gas_id].FNmod13 == 7)||
          (frame_counters[gas_id].FNmod26 == 11)||
          (frame_counters[gas_id].FNmod26 == 25))
      {		   
	    l1_tch_amr_data[gas_id].amr_prepare_msg = TRUE;
	    l1_tch_amr_data[gas_id].amr_block_start = TRUE;
	  }
    }
   else
#endif  /*FEATURE_VAMOS_II*/
    {
    /*
        HR subchannel 1 is essentially HR subchannel 0 shifted by 1
        frame, so we use the subchannel number to shift everything by
        a frame.  Only TCH/H is symmetric w.r.t FNmod13.  FACCH needs
        separate processing
    */
    switch( ( frame_counters[gas_id].FNmod26 % 13 - chnl1_info_ptr->subchannel + 13 ) % 13 )
    {
    case 3:
    case 7:
    case 12:
        l1_tch_amr_data[gas_id].amr_prepare_msg = TRUE;
        l1_tch_amr_data[gas_id].amr_block_start = TRUE;
        break;
    default:
        break;
    }
   }

#ifdef FEATURE_VAMOS_II
   vamos2_tch_wait_state[gas_id] = NO_WAIT;

   if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3))
     {
     if((frame_counters[gas_id].FNmod26 == 11) && (l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame == 12) && (chnl1_info_ptr->subchannel == 0))
       {
       vamos2_tch_wait_state[gas_id] = WAIT_AFTER_BLOCK_START;
       }
     if((frame_counters[gas_id].FNmod26 == 21) && (l1_tch_amr_data[gas_id].amr_sacch_minus_one_frame == 23) && (chnl1_info_ptr->subchannel == 1))
       {
       vamos2_tch_wait_state[gas_id] = WAIT_AFTER_BLOCK_START;
       }
     }
#endif

    l1_tch_amr_data[gas_id].amr_facch_tx_allowed = l1_tch_amr_data[gas_id].amr_facch_rx_expected = l1_tch_amr_data[gas_id].amr_middle_facch_block = FALSE;
    l1_tch_amr_data[gas_id].amr_sacch_in_middle = FALSE;

#ifdef FEATURE_VAMOS_II
   l1_tch_amr_data[gas_id].amr_middle_facch_block_vamos2 = FALSE;
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
    switch( ( frame_counters[gas_id].FNmod26  % 26 - chnl1_info_ptr->subchannel + 26 ) % 26 )
    {
      case 0:
        break;
      case 7:
        l1_tch_amr_data[gas_id].amr_middle_facch_block = TRUE;
        l1_tch_amr_data[gas_id].amr_sacch_in_middle = TRUE;
        l1_tch_amr_data[gas_id].amr_middle_facch_block_vamos2 = TRUE;
        /* Fall through */ /*lint !e616*/
      case 16:
      case 25:
        l1_tch_amr_data[gas_id].amr_facch_tx_allowed = TRUE;
        break;

      case 20:
        l1_tch_amr_data[gas_id].amr_sacch_in_middle = TRUE;
        /* Fall through */ /*lint !e616*/
      case 3:
      case 11:
        l1_tch_amr_data[gas_id].amr_facch_rx_expected = TRUE;
        break;
      default:
        break;
     }
    }   
   if((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3) && (chnl1_info_ptr->subchannel == 1))
    {
    switch( ( frame_counters[gas_id].FNmod26 % 26 - chnl1_info_ptr->subchannel + 26 ) % 26 )
    {
      case 0:
        break;
      case 7:
        l1_tch_amr_data[gas_id].amr_middle_facch_block = TRUE;
        l1_tch_amr_data[gas_id].amr_sacch_in_middle = TRUE;
        l1_tch_amr_data[gas_id].amr_facch_tx_allowed = TRUE;
        break;
        /* Fall through */ /*lint !e616*/
      case 16:
	    l1_tch_amr_data[gas_id].amr_middle_facch_block_vamos2 = TRUE;	   
	    l1_tch_amr_data[gas_id].amr_facch_tx_allowed = TRUE;
		
      case 25:
        l1_tch_amr_data[gas_id].amr_facch_tx_allowed = TRUE;
        break;

      case 20:
        l1_tch_amr_data[gas_id].amr_sacch_in_middle = TRUE;
        /* Fall through */ /*lint !e616*/
      case 3:
      case 12:
        l1_tch_amr_data[gas_id].amr_facch_rx_expected = TRUE;
        break;
      default:
        break;
    }
   }
   else
#endif /*FEATURE_VAMOS_II*/
   {
    switch( ( frame_counters[gas_id].FNmod26 % 26 - chnl1_info_ptr->subchannel + 26 ) % 26 )
    {
      case 0:
        break;
      case 7:
        l1_tch_amr_data[gas_id].amr_middle_facch_block = TRUE;
        l1_tch_amr_data[gas_id].amr_sacch_in_middle = TRUE;
        /* Fall through */ /*lint !e616*/
      case 16:
      case 25:
        l1_tch_amr_data[gas_id].amr_facch_tx_allowed = TRUE;
        break;

      case 20:
        l1_tch_amr_data[gas_id].amr_sacch_in_middle = TRUE;
        /* Fall through */ /*lint !e616*/
      case 3:
      case 12:
        l1_tch_amr_data[gas_id].amr_facch_rx_expected = TRUE;
        break;
      default:
        break;
    }
   }

    /*
     * Increament the frame count. This frame count is used to see how
     * quickly the vocoder applies the new codec mode.
     */
    if(l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.valid)
    {
        l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.frame_count++;
    }

    /*
     * Ordered TA and TXLEV is applied only at the start of the next reporting
     * period. We precompute TXLEV for the entire reporting period and then
     * retrieve it later for each transmitted block.  We do this in
     * reporting_period - 3 because the block is prepared and scheduled a few
     * frames before its actually scheduled to start
     */
    if( frame_counters[gas_id].FNmod104 == l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1 - 2)
    {
        /*
         * Update the power level and TA to use with that commanded by the base
         * station
         */
        ded_dtr_ptr->ms_power_level_to_use =
        ded_dtr_ptr->ordered_ms_power_level;
        ded_dtr_ptr->timing_advance_to_use =
        ded_dtr_ptr->ordered_timing_advance;

        /*
         * We need to report the power level of the last burst on the previous
         * SACCH multiframe.   100 would be the start of the last block.
         * 103 would be the idle frame but the specs say we have to consider
         * frames whether actual transmission takes place or not
         */
        L1_get_txlev( 100, 1, l1_tch_amr_data[gas_id].amr_txlev, gas_id );

        ded_dtr_ptr->ms_power_level_to_report = (uint8)l1_tch_amr_data[gas_id].amr_txlev[3];

        L1_precompute_txlev ( ded_dtr_ptr->ordered_ms_power_level,
                              (int16)ded_dtr_ptr->ms_power_level_in_use,
                              104,
                              chnl1_info_ptr->frequency_list.channel[0],
                              gas_id );

        ded_dtr_ptr->timing_advance_in_use =
          L1_change_ta( ded_dtr_ptr->timing_advance_to_use,
                        ded_dtr_ptr->timing_advance_in_use,gas_id );
    }

    /*
     * l1_tch_amr_data[gas_id].amr_rx_phase is set to 1, indicating that the next
     * speech frame that the MDSP decodes will have
     * codec_mode_indication.
     */
    if( (frame_counters[gas_id].FNmod26 == (6  + chnl1_info_ptr->subchannel)) ||
        (frame_counters[gas_id].FNmod26 == (15 + chnl1_info_ptr->subchannel)) ||
        (frame_counters[gas_id].FNmod26 == (23 + chnl1_info_ptr->subchannel)) )
    {
       if( curr_phase[gas_id] == CMI_PHASE_ODD )
       {
          l1_tch_amr_data[gas_id].amr_rx_phase = 1;
       }
       else
       {
          l1_tch_amr_data[gas_id].amr_rx_phase = 0;
       }
    }

    if( (frame_counters[gas_id].FNmod26 == (2  + chnl1_info_ptr->subchannel)) ||
        (frame_counters[gas_id].FNmod26 == (10 + chnl1_info_ptr->subchannel)) ||
        (frame_counters[gas_id].FNmod26 == (19 + chnl1_info_ptr->subchannel)) )
    {
       if( curr_phase[gas_id] == CMI_PHASE_EVEN )
       {
          l1_tch_amr_data[gas_id].amr_rx_phase = 1;
       }
       else
       {
          l1_tch_amr_data[gas_id].amr_rx_phase = 0;
       }
    }


    if( l1_tch_amr_data[gas_id].amr_prepare_msg )
    {
        boolean    sending_FACCH  = FALSE;

        /*
         * If previous block was FACCH, we need to send one more TCH block to
         * ensure that the interleaver has 8 bursts, so override DEDICATED_STOP
         * till the "completing" TCH block has been sent out
         */
        if(( l1_tskisr_blk->main_command != L1_DEDICATED_STOP)  ||
           l1_tch_amr_data[gas_id].amr_prev_block_was_facch )
        {
            int16 offset = 1;
            int16 FNrptperiod = 0;
            gl1_msg_amr_frame_info_type frame;
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
            uint16 i;
#endif
            boolean tx_success = TRUE;

            if( l1_tch_amr_data[gas_id].amr_facch_tx_allowed )
            {

              /* Did we break the loop to send FACCH in last block? If so re-enable
               * loopback */
              if(l1_tch_amr_data[gas_id].amr_enable_loopback_after_facch_block == TRUE)
              {
                /* Wait for interleaving, FACCH iis interleaved over 8 frames so we need to
                 * wait for 2 blocks before closing the loop again
                 */
                if(++l1_tch_amr_data[gas_id].amr_facch_blk_count == 2)
                {
                  l1_tch_amr_data[gas_id].amr_enable_loopback_after_facch_block = FALSE;

                  /* If we broke loopback to send a FACCH not related to Opening loopback
                   * C, resume loopback
                   */
                  if(ded_dtr_ptr->l1_loopback_disabled == LOOPBACK_C_TEMP_OPEN)
                  {
                    ded_dtr_ptr->l1_loopback_disabled = LOOPBACK_C_CLOSED;
                    gl1_msg_tch_loopback(GL1_DEFS_LOOPBACK_TYPE_C, gas_id);
                  }

                  l1_tch_amr_data[gas_id].amr_facch_blk_count = 0;
                }
              }


              /*
               * if there is new FACCH data or if it is a
               * SIGNALLING only channel then send DSP a tx facch command
               */
#ifdef FEATURE_GSM_COEX
              if( L1_get_CCH_message( FALSE,
                                      FACCH_H,
                                      ded_dtr_ptr,
                                      (l1_tch_amr_data[gas_id].amr_facch_data + 2),
                                      &l1_tch_amr_data[gas_id].amr_data_length, gas_id ) == FALSE )
              {
                 if( (l1_tch_amr_data[gas_id].amr_coex_denials_for_facch <= MAX_AMR_COEX_DENIALS_FOR_FACCH) &&
                     (l1_tch_amr_data[gas_id].amr_coex_request_tx != NULL) &&
                     (l1_tch_amr_data[gas_id].amr_coex_request_tx(GL1_MSG_FACCH_AHS, gas_id) == FALSE) )
                 {
                    L1_get_CCH_filler_message(FACCH_H, l1_tch_amr_data[gas_id].amr_tx_data, &l1_tch_amr_data[gas_id].amr_data_length, gas_id);

                    l1_tch_amr_data[gas_id].amr_facch_not_available = TRUE;

                    l1_tch_amr_data[gas_id].amr_coex_denials_for_facch++;
                 }
                 else
                 {
                    L1_free_CCH_message(FACCH_H, ded_dtr_ptr, gas_id);

                    l1_tch_amr_data[gas_id].amr_facch_not_available = FALSE;

                    l1_tch_amr_data[gas_id].amr_coex_denials_for_facch = 0;

                    sending_FACCH = TRUE;
                 }
              }
			  else
			  {
                 l1_tch_amr_data[gas_id].amr_facch_not_available = TRUE;
			  }
#else
              /* Is there a facch to transmit */
              l1_tch_amr_data[gas_id].amr_facch_not_available =
              L1_get_CCH_message( FALSE,
                                  FACCH_H,
                                  ded_dtr_ptr,
                                  (l1_tch_amr_data[gas_id].amr_facch_data + 2),
                                  &l1_tch_amr_data[gas_id].amr_data_length, gas_id );

              if( l1_tch_amr_data[gas_id].amr_facch_not_available == FALSE )
              {
                 sending_FACCH = TRUE;
              }
#endif
            }

            /* Get the next block of ARFCN's and signal strengths */

            /* Passing in before and after freq parameters to L1_get_ARFCNS
                to guarantee a seamless transition between the before and
                after channels in the case of the start time lying in the
                middle of a block.  Not very elegant but the least complicated
                solution for now
            */
            L1_get_ARFCNS
            ( offset,
              2,
              4,
              l1_tch_amr_data[gas_id].amr_arfcns ,
              &ded_dtr_ptr->channel_specification.channel_info_1_before,
              &ded_dtr_ptr->channel_specification.channel_info_1_after,
              &ded_dtr_ptr->new_channel_info, gas_id );

						
#ifdef FEATURE_VAMOS_II
			if ((gl1_hw_get_vamos2_support(gas_id)) && ((chnl1_info_ptr->TSC & 0x8) >> 3))
			{
			   if((chnl1_info_ptr->subchannel == 0) && (vamos2_tch_wait_state[gas_id] == WAIT_AFTER_BLOCK_START))
			   {			   
			   offset = 4;
			   L1_get_ARFCNS ( offset, 2, 3, l1_tch_amr_data[gas_id].amr_arfcns + 1 ,
							   &ded_dtr_ptr->channel_specification.channel_info_1_before,
							   &ded_dtr_ptr->channel_specification.channel_info_1_after,
							   &ded_dtr_ptr->new_channel_info,gas_id );
			   }
			   else if( (chnl1_info_ptr->subchannel == 0) && (l1_tch_amr_data[gas_id].amr_middle_facch_block_vamos2) )
			   {
			   offset = 8;
			   L1_get_ARFCNS ( offset, 2, 1, l1_tch_amr_data[gas_id].amr_arfcns + 3 ,
							   &ded_dtr_ptr->channel_specification.channel_info_1_before,
							   &ded_dtr_ptr->channel_specification.channel_info_1_after,
							   &ded_dtr_ptr->new_channel_info,gas_id );
			   }
			   else if((chnl1_info_ptr->subchannel == 0) && (l1_tch_amr_data[gas_id].amr_sacch_in_middle) )
			   {			   
			   offset = 6;
			   L1_get_ARFCNS
			   (offset,
				2,
				2, l1_tch_amr_data[gas_id].amr_arfcns + 2 ,
				&ded_dtr_ptr->channel_specification.channel_info_1_before,
				&ded_dtr_ptr->channel_specification.channel_info_1_after,
				&ded_dtr_ptr->new_channel_info, gas_id );
			   }
			   else if((chnl1_info_ptr->subchannel == 1) && (l1_tch_amr_data[gas_id].amr_middle_facch_block))
               {
                offset = 6;
                L1_get_ARFCNS
                (offset,
                 2,
                 2, l1_tch_amr_data[gas_id].amr_arfcns + 2 ,
                 &ded_dtr_ptr->channel_specification.channel_info_1_before,
                 &ded_dtr_ptr->channel_specification.channel_info_1_after,
                 &ded_dtr_ptr->new_channel_info, gas_id );
               }
			   else if((chnl1_info_ptr->subchannel == 1) && (l1_tch_amr_data[gas_id].amr_middle_facch_block_vamos2))
               {
               offset = 8;
               L1_get_ARFCNS ( offset, 2, 1, l1_tch_amr_data[gas_id].amr_arfcns + 3 ,
                               &ded_dtr_ptr->channel_specification.channel_info_1_before,
                               &ded_dtr_ptr->channel_specification.channel_info_1_after,
                               &ded_dtr_ptr->new_channel_info,gas_id );
               }  
			   else if((chnl1_info_ptr->subchannel == 1) && (l1_tch_amr_data[gas_id].amr_sacch_in_middle))
			   {			   
			   offset = 4;
			   L1_get_ARFCNS ( offset, 2, 3, l1_tch_amr_data[gas_id].amr_arfcns + 1 ,
							   &ded_dtr_ptr->channel_specification.channel_info_1_before,
							   &ded_dtr_ptr->channel_specification.channel_info_1_after,
							   &ded_dtr_ptr->new_channel_info,gas_id );
			   }
		   }
		else
#endif
           {
            /*
                Middle FACCH block and SID_UPDATE blocks are asymetric because the SACCH/idle
                comes in between, so recompute ARFCNs for last two bursts
            */
            if( (l1_tch_amr_data[gas_id].amr_middle_facch_block) || (l1_tch_amr_data[gas_id].amr_sacch_in_middle) )
            {
                offset = 6;
                L1_get_ARFCNS
                (offset,
                 2,
                 2, l1_tch_amr_data[gas_id].amr_arfcns + 2 ,
                 &ded_dtr_ptr->channel_specification.channel_info_1_before,
                 &ded_dtr_ptr->channel_specification.channel_info_1_after,
                 &ded_dtr_ptr->new_channel_info, gas_id );
            }
           }

            /* FNrptperiod is the FN relative to the start of the reporting period
               This is used to index into the precomputed TXLEVs since they are
               computed with reference to the start of the reporting period
            */
            /* As below logic gives correct FN relative to the start of the reporting period
               adding offset is not needed as relative FN goes wrong when there is SACCH burst in middle
               which causes Tx power fluctuation
            */
            FNrptperiod = frame_counters[gas_id].FNmod104 -  l1_tch_amr_data[gas_id].amr_reporting_period_end_minus_1;

            if(FNrptperiod < 0)
            {
                FNrptperiod += 104;
            }

            if(FNrptperiod >= 104)
            {
                FNrptperiod -= 104;
            }

            /* Convert TXLEV to power index before passing to the msg layer */
            L1_get_txlev( (uint8)FNrptperiod, 1, l1_tch_amr_data[gas_id].amr_txlev, gas_id );

            /* Update ded data ptr with most recent power level*/
            ded_dtr_ptr->ms_power_level_in_use = (uint8)l1_tch_amr_data[gas_id].amr_txlev[3];


            /* get a vocoder packet */
            if (l1_vs_get_amr_packet_from_client( l1_tch_amr_data[gas_id].amr_voc_data + 4, 
                sizeof(l1_tch_amr_data[gas_id].amr_voc_data) - 4, &frame,gas_id ) )
            {
               gl1_set_dtx_used_flag(gas_id);
            }


            /*
             * If the vocoder is initialized and the codec mode received from the
             * vocoder is out of bound, then set tx_success equal to false
             * which will result in the frame to be dropped except when we are
             * trying to complete the FACCH.
             */
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
            if( l1_vs_is_client_ready(gas_id) )
            {
                if( frame.ul_info.mode  == GL1_MSG_AMR_MODE_UNDEF )
                {
                    MSG_GERAN_HIGH_0_G("Tx AMR_HR Homing Seq");
                    tx_success = FALSE;
                }
                else
                {
                    /*
                     * Map the codec mode received from the vocoder to one in the
                     * Active Codec Set. This is the current ul codec mode indication.
                     */
                    for( i = 0; i < curr_ul_num_acm[gas_id]; i++ )
                    {
                        if( curr_ul_acs[gas_id][i] == frame.ul_info.mode )
                        {
                            curr_ul_cmi[gas_id] = (uint8)i;
                            break;
                        }
                    }

                    if( i == curr_ul_num_acm[gas_id] )
                    {
                        MSG_GERAN_ERROR_1_G("Rcvd mode %d from vocoder not in ACS",
                                  frame.ul_info.mode);
                        tx_success = FALSE;
                    }
                    else
                    {
                        /*
                         * Look at the ul_info of the frame to see the codec mode
                         * used by the vocoder to encode the packet
                         */
                        if( l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.valid &&
                            (l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.cmc == curr_ul_cmi[gas_id] ) )
                        {
                            l1_log_amr_cmc_turnaround_time
                            ( l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.frame_count, gas_id );

                            l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.valid = FALSE;
                        }

                    }
                }
            }
#endif
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
            if(!l1_vs_is_client_ready(gas_id) || !tx_success )
#endif
            {
                /*
                 * If the vocoder is not intialized or we didn't receive a
                 * vocoder packet, tx a dummy speech frame.
                 */
                frame.ul_info.frame = GL1_MSG_AMR_SPEECH_GOOD;

                /*
                 * Transmit packet using Initial codec mode untill we receive a
                 * CMC. Once we receive a CMC, we use the phase to determine,
                 * if the U/L frame is a CMI or CMR frame and only apply the
                 * the CMC received on the D/L to the CMI frames on the U/L
                 */
                if( l1_tch_amr_data[gas_id].amr_cmc_rcvd == FALSE)
                {
                    frame.ul_info.mode = curr_ul_acs[gas_id][curr_ul_cmi[gas_id] = curr_ul_icm[gas_id]];
                }
                else
                {
                    boolean cmi_frame = ( (frame_counters[gas_id].FNmod26 % 26)  ==  ((7+chnl1_info_ptr->subchannel)%26)     ||
                                          (frame_counters[gas_id].FNmod26 % 26)  == ((16+chnl1_info_ptr->subchannel)%26)     ||
                                          (frame_counters[gas_id].FNmod26 % 26)  == ((25+chnl1_info_ptr->subchannel)%26) );
                    if( cmi_frame)
                    {
                        frame.ul_info.mode = curr_ul_acs[gas_id][curr_ul_cmi[gas_id] = curr_cmc[gas_id]];
                    }
                    else
                    {
                        frame.ul_info.mode = curr_ul_acs[gas_id][curr_ul_cmi[gas_id]];
                    }
                }

                /*
                 * Get the homing sequence for the codec rate used on the U/L
                 */
                memscpy ( (byte *)l1_tch_amr_data[gas_id].amr_voc_data+4,sizeof(l1_tch_amr_data[gas_id].amr_voc_data)-4, &amr_homing_seq[curr_ul_acs[gas_id][curr_ul_cmi[gas_id]]][0],
                         (AMR_CHNLS_FS_DATA_WIDTH_OCTETS-4));

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
                /*
                 * If the current TX state is DTX and the vocoder is initialized and we
                 * did not receive a frame from the vocoder then we will transmit a
                 * SPEECH_GOOD with homing sequence. Since this SPEECH_GOOD is artficially
                 * generated by PL1, we set the sid_first_pending flag in order to transition
                 * back into DTX mode after transmitting this SPEECH frame.
                 */
                if( !tx_success && (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX))
                {
                    l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
            }
            }

            if( !l1_tch_amr_data[gas_id].amr_access_burst_scheduled )
            {
                boolean ratscch_permitted = TRUE;

#ifdef FEATURE_GSM_COEX
                if( (!sending_FACCH) && ratscch_rsp_pending[gas_id] && !l1_tch_amr_data[gas_id].amr_hr_facch_tx_in_progress )
                {
                   if( ( l1_tch_amr_data[gas_id].amr_coex_denials_for_ratscch <= MAX_AMR_COEX_DENIELS_FOR_RATSCCH) &&
                       ( l1_tch_amr_data[gas_id].amr_coex_request_tx != NULL) &&
                       ( l1_tch_amr_data[gas_id].amr_coex_request_tx(GL1_MSG_RATSCCH_HS, gas_id) == FALSE ) )
                   {
                      l1_tch_amr_data[gas_id].amr_coex_denials_for_ratscch++;

                      ratscch_permitted = FALSE;
                   }
                   else
                   {
                      l1_tch_amr_data[gas_id].amr_coex_denials_for_ratscch = 0;

                      ratscch_permitted = TRUE;
                   }
                }
#endif

                /* Now tx a tch, ratscch, or facch message */
                if( sending_FACCH )
                {
                    /*------------------------------------------------------------
                     * If we were txing ratscch and are being pre-empted by facch
                     * we will have to re-transmit the entire ratscch message
                     * again. Let's set the ratscch_rsp_pending flag to TRUE
                     * so that the next time we prepare message, and there is no
                     * more facch to send, we will tx ratscch response again
                     *------------------------------------------------------------*/
                    if( l1_tch_amr_data[gas_id].amr_hr_ratscch_tx_in_progress )
                    {
                        ratscch_rsp_pending[gas_id] = TRUE;
                        l1_tch_amr_data[gas_id].amr_hr_ratscch_tx_in_progress = 0;
                        (void)gl1_msg_abort_tx_tch_amr_hr(gas_id);
                    }

                    l1_amr_tx_facch_hr(frame, chnl1_info_ptr->subchannel,ded_dtr_ptr, gas_id );
                }
                else if( ( ratscch_rsp_pending[gas_id] && !l1_tch_amr_data[gas_id].amr_hr_facch_tx_in_progress ) && ratscch_permitted )
                {
                    /*-----------------------------------------------------------
                     * TX ratscch only is we are not in the middle of sending
                     * a facch
                     *-----------------------------------------------------------*/
                    l1_amr_tx_ratscch_hr( frame, chnl1_info_ptr->subchannel, gas_id );
                    ratscch_rsp_pending[gas_id] = FALSE;
                }
                else
                {
                    if(!l1_tch_amr_data[gas_id].amr_hr_facch_tx_in_progress &&
                       !l1_tch_amr_data[gas_id].amr_hr_ratscch_tx_in_progress )
                    {
                        l1_tch_amr_data[gas_id].amr_voc_data[3] = (uint8)frame.ul_info.frame;
                        l1_tch_amr_data[gas_id].amr_voc_data[2] = (uint8)frame.ul_info.mode;

                        /*
                         * If sid_first or sid_update tx is in progress, it will be
                         * inhibited by this speech frame
                         */
                        if( frame.ul_info.frame == GL1_MSG_AMR_SPEECH_GOOD)
                        {
                            l1_amr_tx_tch_hr( frame, chnl1_info_ptr->subchannel, gas_id );
                        }
                        else
                        {
                            /*
                             * If we receive no_data, don't schedule saved sid_first
                             * or saved sid_update, until the current transmission is
                             * done
                             */
                            if( !l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress &&
                                !l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress )
                            {
                                l1_amr_tx_tch_hr( frame, chnl1_info_ptr->subchannel, gas_id );
                            }
                        }

                        /*
                         * After transmitting the SPEECH_GOOD frame, determine if the SPEECH
                         * frame was internally generated. If it was then do not clear the
                         * sid_first pending and sid_update_pending flags.
                         */
                        if( (frame.ul_info.frame == GL1_MSG_AMR_SPEECH_GOOD ) &&
                            !l1_tch_amr_data[gas_id].amr_prev_block_was_facch &&
                            tx_success  )
                        {
                            l1_tch_amr_data[gas_id].amr_sid_first_pending = l1_tch_amr_data[gas_id].amr_sid_update_pending = FALSE;
                        }

                    }
                    else
                    {
                        /*-----------------------------------------------------------
                         * Delay SID_FIRST/SID_UPDATE if we are in the middle of
                         * txing a facch or a ratscch
                         *-----------------------------------------------------------*/
                        if( frame.ul_info.frame == GL1_MSG_AMR_SID_FIRST)
                        {
                            l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
                        }
                        else if(frame.ul_info.frame == GL1_MSG_AMR_SID_UPDATE )
                        {
                            l1_tch_amr_data[gas_id].amr_sid_update_pending = TRUE;
                        }
#ifdef GL1_TCH_AMR_DEBUG
                        else if(frame.ul_info.frame == GL1_MSG_AMR_NO_DATA)
                        {
                            l1_tch_amr_data[gas_id].amr_no_data_cnt++;
                        }
#endif
                    }
                }


                /*
                 * Log change in the AMR TX state
                 */
                if( l1_tch_amr_data[gas_id].amr_curr_tx_state != l1_tch_amr_data[gas_id].amr_new_tx_state )
                {
                    l1_log_amr_state_change_T data;
                    data.old_state = (uint8)l1_tch_amr_data[gas_id].amr_curr_tx_state;
                    data.new_state = (uint8)l1_tch_amr_data[gas_id].amr_new_tx_state;
                    l1_log_amr_state_change( &data, gas_id );
                }

                /*
                 * Update the AMR TX state
                 */
                l1_tch_amr_data[gas_id].amr_curr_tx_state = l1_tch_amr_data[gas_id].amr_new_tx_state;
            }

            /* Go get an rx message */
            gl1_msg_rx_tch_facch_amr_hr(l1_tch_amr_data[gas_id].amr_arfcns,
                                        l1_tch_amr_data[gas_id].amr_agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                                        l1_tch_amr_data[gas_id].amr_agc_ptr_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/  
                                        &l1_tch_amr_data[gas_id].amr_t_rpt,
                                        l1_tch_amr_data[gas_id].amr_facch_rx_expected,
                                        l1_tch_amr_data[gas_id].amr_rx_phase,
#ifdef FEATURE_VAMOS_II
                                        vamos2_tch_wait_state[gas_id],
#endif
                                        gas_id);

            if(l1_tch_amr_data[gas_id].amr_need_to_expire_ack_act_timer)
            {
                l1_tch_amr_data[gas_id].amr_need_to_expire_ack_act_timer = FALSE;
                ratscch_expire_ack_activation_timer(gas_id);
            }
        }
    }
    if (l1_tch_amr_data[gas_id].amr_prev_block_was_facch)
    {
      l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter--;
      if (l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter <= 0)
      {
        l1_tch_amr_data[gas_id].amr_prev_block_was_facch = FALSE;
      }
      if (l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter < 0)
      {
        MSG_GERAN_ERROR_1_G("AHS:prev_block_was_facch_cnt =%d",l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter);
      }
    }

}

/*===========================================================================
FUNCTION l1_amr_tch_abort_hr

DESCRIPTION
   Function to abort the AMR Half Rate Traffic Channel

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void  l1_amr_tch_abort_hr ( gas_id_t gas_id )
{
    /* Wait for SCE */
    l1_tch_amr_data[gas_id].amr_idle_status = !l1_sci_is_op_in_progress(gas_id);

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
    l1_tch_amr_data[gas_id].amr_idle_status &= gl1_msg_abort_tx_tch_amr_hr(gas_id);

    /* Abort Rx SACCH */
    gl1_msg_abort_rx_sacch(gas_id);

    /* Abort Rx TCH/FACCH/RATSCCH */
    l1_tch_amr_data[gas_id].amr_idle_status &= gl1_msg_abort_rx_tch_amr_hr(gas_id);

    /* let SCE terminate - it already knows it should */
    l1_sci_tick_receive(gas_id);

    MSG_GERAN_HIGH_0_G("Tch abort");
}


/*===========================================================================
FUNCTION l1_amr_tx_facch_hr

DESCRIPTION
   This function is the part of the TX side state machine implementation for
   AMR Half Rate and handles FACCH channel processing by resolving contention
   between the Vocoder frames and FACCH frame, giving priority to the FACCH
   frame.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void  l1_amr_tx_facch_hr ( gl1_msg_amr_frame_info_type frame_info,
                                  uint8 subchannel,
                                  dedicated_data_T *ded_dtr_ptr, gas_id_t gas_id)
{
    uint8 phase = 0;
    boolean onset_flag = FALSE;
    boolean sid_first_inh, sid_update_inh;
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    sid_first_inh = l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress?TRUE:FALSE;

    sid_update_inh = l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress?TRUE:FALSE;

    if(sid_first_inh)
    {
        (void)gl1_msg_abort_tx_tch_amr_hr(gas_id);
        l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress = 0;
    }
    else if(sid_update_inh)
    {
        (void)gl1_msg_abort_tx_tch_amr_hr(gas_id);
        l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress = 0;
    }

    l1_tch_amr_data[gas_id].amr_new_tx_state = amr_state_table[l1_tch_amr_data[gas_id].amr_curr_tx_state][AMR_FACCH];

    if( (frame_counters[gas_id].FNmod26 % 26) == ((7  + subchannel)%26)     ||
        (frame_counters[gas_id].FNmod26 % 26) == ((16 + subchannel)%26)     ||
        (frame_counters[gas_id].FNmod26 % 26) == ((25 + subchannel)%26))
    {
        phase = 1;
    }

    if( (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX) || (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT) )
    {
        onset_flag = TRUE;
        l1_log_facch_in_dtx(gas_id);
        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT)
        {
            l1_tch_amr_data[gas_id].amr_nsync_cnt = 0;
        }
    }

    if( ratscch_rsp_pending[gas_id] )
    {
        l1_log_facch_and_ratscch_collision(gas_id);
    }

#ifdef GL1_TCH_AMR_DEBUG
    MSG_GERAN_HIGH_3_G( "TXing HR FACCH with cmi %d cmr %d phase %d",
              curr_ul_cmi[gas_id], curr_cmr[gas_id], phase);
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_FACCH_TX);
#endif

    gl1_msg_tx_facch_amr_hr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                            l1_tch_amr_data[gas_id].amr_facch_data, onset_flag,
                            curr_ul_cmi[gas_id], curr_cmr[gas_id], phase,
                            l1_tch_amr_data[gas_id].amr_middle_facch_block,
                            sid_first_inh,
                            sid_update_inh,
#ifdef FEATURE_VAMOS_II
                            l1_tch_amr_data[gas_id].amr_middle_facch_block_vamos2,
                            vamos2_tch_wait_state[gas_id],
#endif                            
                            gas_id);

    /* Log tx burst metrics */
    l1_log_transmit_burst_metrics
    ( GL1_MSG_FACCH_AHS,
      1, 2, 2,
      l1_tch_amr_data[gas_id].amr_arfcns,
      l1_tch_amr_data[gas_id].amr_txlev,
      l1_tch_amr_data[gas_id].amr_current_TSC,
      l1_dedicated_data_ptr->timing_advance_in_use, gas_id );

   /* Tx FACCH during TCH loopback, loopback was opened when we received PH_DATA_REQ in l1_ded_if.c
    * so re-enable once FACCH has been transmitted. Note this take 8 frames due to interleaving
    */
    l1_tch_amr_data[gas_id].amr_enable_loopback_after_facch_block = TRUE;

    l1_tch_amr_data[gas_id].amr_hr_facch_tx_in_progress++;
    l1_tch_amr_data[gas_id].amr_prev_block_was_facch = TRUE;

    /* This counter is used to reset the prev_block_was_facch if we are
     * in UL DTX mode - without the counter we will not be able to abort
     * the TCH in this scenario - CR169424
     */
     if(l1_tskisr_blk->main_command != L1_DEDICATED_STOP)
     {
       l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter = L1_WAIT_FOUR_HR_BURSTS_BEFORE_ABORT;
     }

    /* On UL B0 is 11 frames, B1, B2 are 12 frames wide. We need
     * to account for this if we wish to delay prev_blk_was_facch
     * flag being reset until 4 bursts have been TX'd
     */
    if ((( frame_counters[gas_id].FNmod26 % 26 - subchannel + 26 ) % 26 ) != 25 )
    {
       l1_tch_amr_data[gas_id].amr_prev_block_was_facch_counter++;
    }

    switch(frame_info.ul_info.frame)
    {
      case GL1_MSG_AMR_SPEECH_GOOD:
        break;

      case GL1_MSG_AMR_SID_FIRST:
        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_TCH )
        {
            /*
             * There is a collision of SID_FIRST and RATSCCH, we set the
             * sid_first_pending flag to indicate that.
             */
            l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
        }
        else if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT )
        {
            /*
             * Should not receive a SID_FIRST in the INIT state if
             * amr_nsync_cnt is greater that 0. If the nsync count
             * is 0, the SID_FIRST frame is dropped to the floor in the
             * INIT state.
             */
            if( l1_tch_amr_data[gas_id].amr_nsync_cnt )
            {
                MSG_GERAN_ERROR_2_G("Current State: %d, Frame Type: %d",
                          l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame );
            }
        }
        else
        {
            MSG_GERAN_ERROR_2_G("Current State: %d, Frame Type: %d",
                      l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame );
        }
        break;

      case GL1_MSG_AMR_SID_UPDATE:
        gl1_msg_save_sid_update_amr_fr_hr(l1_tch_amr_data[gas_id].amr_voc_data,gas_id);
        l1_log_facch_and_sid_update_collision(gas_id);
        l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
        l1_tch_amr_data[gas_id].amr_sid_update_pending = TRUE;
        break;

      case GL1_MSG_AMR_NO_DATA:
#ifdef GL1_TCH_AMR_DEBUG
        l1_tch_amr_data[gas_id].amr_no_data_cnt++;
#endif
        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX )
        {
            l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
        }
        break;

      default:
        MSG_GERAN_ERROR_1_G("Invalid Frame Type: %d", frame_info.ul_info.frame);
        break;
    }
}

/*===========================================================================
FUNCTION l1_amr_tx_ratscch_hr

DESCRIPTION
   This function is the part of the TX side state machine implementation for
   AMR Half Rate and handles RATSCCH channel processing by resolving contention
   between the Vocoder frames and RATSCCH frame, giving priority to the
   RATSCCH frame.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void l1_amr_tx_ratscch_hr (gl1_msg_amr_frame_info_type frame_info,
                                  uint8 subchannel, gas_id_t gas_id)
{
    uint8 phase = 0;
    boolean onset_flag = FALSE;
    boolean sid_first_inh, sid_update_inh;
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    sid_first_inh = l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress?TRUE:FALSE;

    sid_update_inh = l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress?TRUE:FALSE;

    if(sid_first_inh)
    {
        (void)gl1_msg_abort_tx_tch_amr_hr(gas_id);
        l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress = 0;
    }
    else if(sid_update_inh)
    {
        (void)gl1_msg_abort_tx_tch_amr_hr(gas_id);
        l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress = 0;
    }

    l1_tch_amr_data[gas_id].amr_new_tx_state = amr_state_table[l1_tch_amr_data[gas_id].amr_curr_tx_state][AMR_RATSCCH];

    if( (frame_counters[gas_id].FNmod26 % 26) == ((7  + subchannel)%26)    ||
        (frame_counters[gas_id].FNmod26 % 26) == ((16 + subchannel)%26)    ||
        (frame_counters[gas_id].FNmod26 % 26) == ((25 + subchannel)%26))
    {
        phase = 1;
    }

    if( (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX) || (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT) )
    {
        onset_flag = TRUE;

        l1_log_ratscch_in_dtx(gas_id);

        if(l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT )
        {
            l1_tch_amr_data[gas_id].amr_nsync_cnt = 0;
        }
    }

#ifdef GL1_TCH_AMR_DEBUG
    MSG_GERAN_HIGH_3_G( "TXing HR RATSCCH, cmi %d, cmr %d, phase %d",
              curr_ul_cmi[gas_id], curr_cmr[gas_id], phase);
#endif

    l1_amr_update_cmr(phase, TRUE, gas_id);

    gl1_msg_tx_ratscch_amr_hr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                              ratscch_rsp[gas_id], onset_flag,
                              curr_ul_cmi[gas_id], curr_cmr[gas_id], phase,
                              l1_tch_amr_data[gas_id].amr_sacch_in_middle,
                              sid_first_inh,
                              sid_update_inh,
#ifdef FEATURE_VAMOS_II
                              l1_tch_amr_data[gas_id].amr_middle_facch_block_vamos2,
                              vamos2_tch_wait_state[gas_id],
#endif  
                              gas_id);

    /* Log tx burst metrics */
    l1_log_transmit_burst_metrics
    ( GL1_MSG_RATSCCH_HS,
      1, 2, 2,
      l1_tch_amr_data[gas_id].amr_arfcns,
      l1_tch_amr_data[gas_id].amr_txlev,
      l1_tch_amr_data[gas_id].amr_current_TSC,
      l1_dedicated_data_ptr->timing_advance_in_use, gas_id );

    /* Log RATSCCH response, ACK_OK/ACK_ERR/ACK_UNKNOWN */
    l1_log_amr_ratscch_rsp( ratscch_rsp[gas_id][4], gas_id );

    l1_tch_amr_data[gas_id].amr_hr_ratscch_tx_in_progress++;

    switch(frame_info.ul_info.frame)
    {
      case GL1_MSG_AMR_SPEECH_GOOD:
        break;

      case GL1_MSG_AMR_SID_FIRST:
        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_TCH )
        {
            /*
             * There is a collision of SID_FIRST and RATSCCH, we set the
             * sid_first_pending flag to indicate that.
             */
            l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
        }
        else if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT )
        {
            /*
             * Should not receive a SID_FIRST in the INIT state if
             * amr_nsync_cnt is greater that 0. If the nsync count
             * is 0, the SID_FIRST frame is dropped to the floor in the
             * INIT state.
             */
            if( l1_tch_amr_data[gas_id].amr_nsync_cnt )
            {
                MSG_GERAN_ERROR_2_G("Current State: %d, Frame Type: %d",
                          l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame );
            }
        }
        else
        {
            MSG_GERAN_ERROR_2_G("Current State: %d, Frame Type: %d",
                      l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame );
        }
        break;

      case GL1_MSG_AMR_SID_UPDATE:
        gl1_msg_save_sid_update_amr_fr_hr( l1_tch_amr_data[gas_id].amr_voc_data,gas_id );
        l1_log_ratscch_and_sid_update_collision(gas_id);
        l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
        l1_tch_amr_data[gas_id].amr_sid_update_pending = TRUE;
        break;

      case GL1_MSG_AMR_NO_DATA:
#ifdef GL1_TCH_AMR_DEBUG
        l1_tch_amr_data[gas_id].amr_no_data_cnt++;
#endif
        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX )
        {
            l1_tch_amr_data[gas_id].amr_sid_first_pending = TRUE;
        }
        break;

      default:
        MSG_GERAN_ERROR_1_G("Invalid Frame Type: %d", frame_info.ul_info.frame);
        break;
    }
}

/*===========================================================================
FUNCTION l1_amr_tx_tch_hr

DESCRIPTION
   This function is the part of the TX side state machine implementation for
   AMR Half Rate and handles TCH channel processing.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void  l1_amr_tx_tch_hr (gl1_msg_amr_frame_info_type frame_info,
                               uint8 subchannel, gas_id_t gas_id)
{
    amr_chnl_type amr_frame;
    boolean onset_flag = FALSE;
    uint8 phase = 0;
    boolean sid_first_inh, sid_update_inh;
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    if( (frame_counters[gas_id].FNmod26 % 26) == ((7  + subchannel)%26)    ||
        (frame_counters[gas_id].FNmod26 % 26) == ((16 + subchannel)%26)    ||
        (frame_counters[gas_id].FNmod26 % 26) == ((25 + subchannel)%26) )
    {
        phase = 1;
    }

    switch( frame_info.ul_info.frame )
    {
      case GL1_MSG_AMR_SPEECH_GOOD:
        amr_frame = AMR_SPEECH_GOOD;
        break;

      case GL1_MSG_AMR_SID_FIRST:
        amr_frame = AMR_SID_FIRST;
        break;

      case GL1_MSG_AMR_SID_UPDATE:
        amr_frame = AMR_SID_UPDATE;
        break;

      case GL1_MSG_AMR_NO_DATA:
        amr_frame = AMR_NO_DATA;
#ifdef GL1_TCH_AMR_DEBUG
        l1_tch_amr_data[gas_id].amr_no_data_cnt++;
#endif
        break;

      default:
        MSG_GERAN_ERROR_1_G("Invalid frame type: %d", frame_info.ul_info.frame);
        return;
    }

    l1_tch_amr_data[gas_id].amr_new_tx_state = amr_state_table[l1_tch_amr_data[gas_id].amr_curr_tx_state][amr_frame];

    switch( amr_frame )
    {
      /*-------------------------------------------------------------------------
       * Rcvd SPEECH frame from the vocoder.
       *-------------------------------------------------------------------------*/
      case AMR_SPEECH_GOOD:

        if( (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX) || (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT) )
        {
            onset_flag = TRUE;

            if(l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT)
            {
                l1_tch_amr_data[gas_id].amr_nsync_cnt = 0;
            }
        }

        /*
         * check to see if there was a sid_first or sid_update transmission
         * is in progress. If true, set the INHIBIT flag(s).
         */
        sid_first_inh = l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress?TRUE:FALSE;
        sid_update_inh = l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress?TRUE:FALSE;

        /*
         * If sid_first needs to be inhibited, abort the current transmission
         * of sid_first.
         */
        if(sid_first_inh)
        {
            (void)gl1_msg_abort_tx_tch_amr_hr(gas_id);
            l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress = 0;
        }
        /*
         * If sid_update needs to be inhibited, abort the current transmission
         * of sid_update.
         */
        else if(sid_update_inh)
        {
            (void)gl1_msg_abort_tx_tch_amr_hr(gas_id);
            l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress = 0;
        }

        l1_amr_update_cmr(phase, FALSE, gas_id);

        if(!l1_tch_amr_data[gas_id].amr_loopback_I_in_progress)
        {
            gl1_msg_tx_tch_amr_hr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                  l1_tch_amr_data[gas_id].amr_voc_data, onset_flag,
                                  curr_ul_cmi[gas_id], curr_dl_cmi[gas_id],
                                  curr_cmr[gas_id], curr_cmc[gas_id],
                                  curr_ul_acs[gas_id][curr_ul_cmi[gas_id]],
                                  phase,
                                  sid_first_inh,
                                  sid_update_inh,
#ifdef FEATURE_VAMOS_II
                                  vamos2_tch_wait_state[gas_id],
#endif
                                  gas_id
                                 );
        }
        else
        {
            gl1_msg_tx_tch_amr_hr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                  l1_tch_amr_data[gas_id].amr_voc_data, onset_flag,
                                  curr_ul_cmi[gas_id], curr_dl_cmi[gas_id],
                                  curr_cmr[gas_id], curr_cmc[gas_id],
                                  curr_ul_acs[gas_id][curr_cmc[gas_id]],
                                  phase,
                                  sid_first_inh,
                                  sid_update_inh,
#ifdef FEATURE_VAMOS_II
                                  vamos2_tch_wait_state[gas_id],
#endif
                                  gas_id);
        }

        /* Log tx burst metrics */
        l1_log_transmit_burst_metrics
        ( GL1_MSG_TCH_AHS,
          1, 2, 2,
          l1_tch_amr_data[gas_id].amr_arfcns,
          l1_tch_amr_data[gas_id].amr_txlev,
          l1_tch_amr_data[gas_id].amr_current_TSC,
          l1_dedicated_data_ptr->timing_advance_in_use, gas_id );


#ifdef GL1_TCH_AMR_DEBUG
        MSG_GERAN_HIGH_3_G( "TXing HR SPEECH_GOOD, cmi %d, cmr %d, phase %d",
                  curr_ul_cmi[gas_id], curr_cmr[gas_id], phase);
#endif

        break;

        /*-------------------------------------------------------------------------
         * Rcvd SID_FIRST frame from the vocoder.
         *-------------------------------------------------------------------------*/
      case AMR_SID_FIRST:
        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_TCH )
        {
            l1_amr_update_cmr(phase, FALSE, gas_id);
            gl1_msg_tx_sid_first_amr_hr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                        l1_tch_amr_data[gas_id].amr_voc_data,
                                        curr_ul_cmi[gas_id], curr_cmr[gas_id], phase,
                                        l1_tch_amr_data[gas_id].amr_sacch_in_middle,
#ifdef FEATURE_VAMOS_II
                                        l1_tch_amr_data[gas_id].amr_middle_facch_block_vamos2,
                                        vamos2_tch_wait_state[gas_id],
#endif  
                                        gas_id);

            /* Log tx burst metrics */
            l1_log_transmit_burst_metrics
            ( GL1_MSG_SID_FIRST_HS,
              1, 2, 2,
              l1_tch_amr_data[gas_id].amr_arfcns,
              l1_tch_amr_data[gas_id].amr_txlev,
              l1_tch_amr_data[gas_id].amr_current_TSC,
              l1_dedicated_data_ptr->timing_advance_in_use, gas_id );


            l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress++;
#ifdef GL1_TCH_AMR_DEBUG
            l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
            MSG_GERAN_HIGH_3_G( "TXing HR SID_FIRST, cmi %d, cmr %d, phase %d",
                      curr_ul_cmi[gas_id], curr_cmr[gas_id], phase);
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_FIRST_TX);
#endif
        }
        else if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT )
        {
            /*
             * Should not receive a SID_FIRST in the INIT state if
             * amr_nsync_cnt is greater that 0. If the nsync count
             * is 0, the SID_FIRST frame is dropped to the floor in the
             * INIT state.
             */
            if( l1_tch_amr_data[gas_id].amr_nsync_cnt )
            {
                MSG_GERAN_ERROR_2_G("Current State: %d, Frame Type: %d",
                          l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame );
            }
        }
        else
        {
            MSG_GERAN_ERROR_2_G("Current State: %d, Frame Type: %d",
                      l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame );
        }
        break;

        /*-------------------------------------------------------------------------
         * Rcvd SID_UPDATE frame from the vocoder.
         *-------------------------------------------------------------------------*/
      case AMR_SID_UPDATE:
        if( (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_TCH ) && ( l1_tch_amr_data[gas_id].amr_sid_first_pending == TRUE ) )
        {
            /*
             * If in the TCH state a sid_first is pending and we
             * recieve a SID_UPDATE from the vocoder, we save the
             * received SID_UPDATE and transmit the sid_first.
             * We also change our state to go to DTX state and
             * set sid_update_pending to TRUE, so that we can
             * schedule the SID_UPDATE next time around.
             */

            /*
             * NOTE -- This condition can only happen if a
             * FACCH or a RATSCCH collided with a NO_DATA frame
             * from the vocoder, right before getting a SID_UPDATE
             * frame from the vocoder.
             */

            gl1_msg_save_sid_update_amr_fr_hr( l1_tch_amr_data[gas_id].amr_voc_data,gas_id );

            l1_amr_update_cmr(phase, FALSE, gas_id);

            gl1_msg_tx_sid_first_amr_hr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                        l1_tch_amr_data[gas_id].amr_voc_data,
                                        curr_ul_cmi[gas_id], curr_cmr[gas_id], phase,
                                        l1_tch_amr_data[gas_id].amr_sacch_in_middle,                                        
#ifdef FEATURE_VAMOS_II
                                        l1_tch_amr_data[gas_id].amr_middle_facch_block_vamos2,
                                        vamos2_tch_wait_state[gas_id],
#endif  
                                        gas_id);

            /* Log tx burst metrics */
            l1_log_transmit_burst_metrics
            ( GL1_MSG_SID_FIRST_HS,
              1, 2, 2,
              l1_tch_amr_data[gas_id].amr_arfcns,
              l1_tch_amr_data[gas_id].amr_txlev,
              l1_tch_amr_data[gas_id].amr_current_TSC,
              l1_dedicated_data_ptr->timing_advance_in_use, gas_id );


            l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress++;

#ifdef GL1_TCH_AMR_DEBUG
            l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
            MSG_GERAN_HIGH_3_G( "TXing saved SID_FIRST, cmi %d, cmr %d, phase %d",
                      curr_ul_cmi[gas_id], curr_cmr[gas_id], phase);
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_FIRST_TX);
#endif

            l1_tch_amr_data[gas_id].amr_new_tx_state =
            amr_state_table[l1_tch_amr_data[gas_id].amr_curr_tx_state][AMR_SID_FIRST];

            l1_tch_amr_data[gas_id].amr_sid_first_pending = FALSE;
            l1_tch_amr_data[gas_id].amr_sid_update_pending = TRUE;
        }
        else
        {
            if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_TCH )
            {
                /*
                 * Even though we received a SID_UPDATE in TCH state,
                 * transition the state to DTX and transmit the SID_UPDATE.
                 */
                l1_tch_amr_data[gas_id].amr_new_tx_state = AMR_DTX;
                MSG_GERAN_HIGH_1_G("Rcvd HR SID_UPDATE from vocoder, forcing TX state to DTX",
                         0);
            }

            if( !l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress )
            {
                l1_amr_update_cmr(phase, TRUE, gas_id);
                gl1_msg_tx_sid_update_amr_hr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                             l1_tch_amr_data[gas_id].amr_voc_data,
                                             curr_ul_cmi[gas_id], curr_cmr[gas_id],
                                             phase,
                                             l1_tch_amr_data[gas_id].amr_sacch_in_middle,
#ifdef FEATURE_VAMOS_II
                                             l1_tch_amr_data[gas_id].amr_middle_facch_block_vamos2,
                                             vamos2_tch_wait_state[gas_id],
#endif  
                                             gas_id);

                /* Log tx burst metrics */
                l1_log_transmit_burst_metrics
                ( GL1_MSG_SID_UPDATE_HS,
                  1, 2, 2,
                  l1_tch_amr_data[gas_id].amr_arfcns,
                  l1_tch_amr_data[gas_id].amr_txlev,
                  l1_tch_amr_data[gas_id].amr_current_TSC,
                  l1_dedicated_data_ptr->timing_advance_in_use, gas_id );

                l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress++;
#ifdef GL1_TCH_AMR_DEBUG
                MSG_GERAN_HIGH_3_G( "TXing HR SID_UPDATE, cmi %d, cmr %d, cnt %d",
                          curr_ul_cmi[gas_id], curr_cmr[gas_id], l1_tch_amr_data[gas_id].amr_no_data_cnt);
                l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_UPDATE_TX);
#endif
            }
            else
            {
#ifdef GL1_TCH_AMR_DEBUG
                MSG_GERAN_HIGH_0_G("Skipping HR SID_UPDATE");
#endif
            }
        }

        if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT )
        {
            /*
             * Increment NSYNC count
             */
            l1_tch_amr_data[gas_id].amr_nsync_cnt++;

            /*
             * Init state processing requires the nsync count to reach 12
             * to move into DTX state.
             */
            if( l1_tch_amr_data[gas_id].amr_nsync_cnt == AMR_MAX_NSYNC_CNT )
            {
                l1_tch_amr_data[gas_id].amr_new_tx_state  = AMR_DTX;
                l1_tch_amr_data[gas_id].amr_nsync_cnt = 0;
            }
        }
        break;

        /*-------------------------------------------------------------------------
         * Rcvd NO_DATA frame from the vocoder.
         *-------------------------------------------------------------------------*/
      case AMR_NO_DATA:
        if( l1_tch_amr_data[gas_id].amr_sid_first_pending )
        {
            if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_TCH )
            {
                /*
                 * If received NO_DATA from the vocoder, and were in TCH state
                 * and sid_first is pending, transmit the pending sid_first and
                 * transition the state to DTX.
                 */
                l1_amr_update_cmr(phase, FALSE, gas_id);
                gl1_msg_tx_sid_first_amr_hr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                            l1_tch_amr_data[gas_id].amr_voc_data,
                                            curr_ul_cmi[gas_id], curr_cmr[gas_id], phase,
                                            l1_tch_amr_data[gas_id].amr_sacch_in_middle,
#ifdef FEATURE_VAMOS_II
                                            l1_tch_amr_data[gas_id].amr_middle_facch_block_vamos2,
                                            vamos2_tch_wait_state[gas_id],
#endif  
                                            gas_id);

                /* Log tx burst metrics */
                l1_log_transmit_burst_metrics
                ( GL1_MSG_SID_FIRST_HS,
                  1, 2, 2,
                  l1_tch_amr_data[gas_id].amr_arfcns,
                  l1_tch_amr_data[gas_id].amr_txlev,
                  l1_tch_amr_data[gas_id].amr_current_TSC,
                  l1_dedicated_data_ptr->timing_advance_in_use, gas_id );


                l1_tch_amr_data[gas_id].amr_hr_sid_first_tx_in_progress++;

#ifdef GL1_TCH_AMR_DEBUG
                l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
                MSG_GERAN_HIGH_3_G( "TXing HR SID_FIRST, cmi %d, cmr %d, phase %d",
                          curr_ul_cmi[gas_id], curr_cmr[gas_id], phase);
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_FIRST_TX);
#endif
                l1_tch_amr_data[gas_id].amr_new_tx_state =
                amr_state_table[l1_tch_amr_data[gas_id].amr_curr_tx_state][AMR_SID_FIRST];
                l1_tch_amr_data[gas_id].amr_sid_first_pending = FALSE;
            }
            else
            {
                MSG_GERAN_ERROR_3_G("Curr State:%d,Frame Type:%d,SID FIRST PEND:%d",
                          l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame,
                          l1_tch_amr_data[gas_id].amr_sid_first_pending);
            }
        }
        else if( l1_tch_amr_data[gas_id].amr_sid_update_pending )
        {
            if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX )
            {
                /*
                 * If received NO_DATA from the vocoder, and we were in DTX
                 * state and sid_update is pending, transmit the pending
                 * sid_update and stay in the DTX state.
                 */
                if(!l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress)
                {
                    l1_amr_update_cmr(phase, TRUE, gas_id);

                    gl1_msg_tx_saved_sid_update_amr_hr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev,
                                                       curr_ul_cmi[gas_id], curr_cmr[gas_id],
                                                       phase,
                                                       l1_tch_amr_data[gas_id].amr_sacch_in_middle,
#ifdef FEATURE_VAMOS_II
                                                       l1_tch_amr_data[gas_id].amr_middle_facch_block_vamos2,
                                                       vamos2_tch_wait_state[gas_id],
#endif  
                                                       gas_id);

                    /* Log tx burst metrics */
                    l1_log_transmit_burst_metrics
                    ( GL1_MSG_SID_UPDATE_HS,
                      1, 2, 2,
                      l1_tch_amr_data[gas_id].amr_arfcns,
                      l1_tch_amr_data[gas_id].amr_txlev,
                      l1_tch_amr_data[gas_id].amr_current_TSC,
                      l1_dedicated_data_ptr->timing_advance_in_use, gas_id );


                    l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress++;
#ifdef GL1_TCH_AMR_DEBUG
                    MSG_GERAN_HIGH_3_G( "TXing HR Saved SID_UPDATE, cmi %d, cmr %d, cnt %d",
                              curr_ul_cmi[gas_id], curr_cmr[gas_id], l1_tch_amr_data[gas_id].amr_no_data_cnt);
                    l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                    gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_UPDATE_TX);
#endif
                    l1_tch_amr_data[gas_id].amr_sid_update_pending = FALSE;
                }
                else
                {
#ifdef GL1_TCH_AMR_DEBUG
                    MSG_GERAN_HIGH_0_G("Skipping HR SID_UPDATE");
#endif
                }
            }
            else
            {
                MSG_GERAN_ERROR_3_G("Curr State:%d,Frame Type:%d,SID UPDATE PEND:%d",
                          l1_tch_amr_data[gas_id].amr_curr_tx_state, frame_info.ul_info.frame,
                          l1_tch_amr_data[gas_id].amr_sid_update_pending);
            }
        }
        else if( l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_INIT )
        {
            /*
             * If received NO_DATA from the vocoder, and we were in INIT
             * state, transmit a sid_update using the payload received
             * in the NO_DATA frame and stay in the INIT state.
             */
            l1_tch_amr_data[gas_id].amr_nsync_cnt++;

            if(!l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress)
            {
                l1_amr_update_cmr(phase, TRUE, gas_id);
                gl1_msg_tx_sid_update_amr_hr(l1_tch_amr_data[gas_id].amr_arfcns, l1_tch_amr_data[gas_id].amr_txlev, l1_tch_amr_data[gas_id].amr_voc_data,
                                             curr_ul_cmi[gas_id], curr_cmr[gas_id],
                                             phase,
                                             l1_tch_amr_data[gas_id].amr_sacch_in_middle,
#ifdef FEATURE_VAMOS_II
                                             l1_tch_amr_data[gas_id].amr_middle_facch_block_vamos2,
                                             vamos2_tch_wait_state[gas_id],
#endif  
                                             gas_id);

                /* Log tx burst metrics */
                l1_log_transmit_burst_metrics
                ( GL1_MSG_SID_UPDATE_HS,
                  1, 2, 2,
                  l1_tch_amr_data[gas_id].amr_arfcns,
                  l1_tch_amr_data[gas_id].amr_txlev,
                  l1_tch_amr_data[gas_id].amr_current_TSC,
                  l1_dedicated_data_ptr->timing_advance_in_use, gas_id );

                l1_tch_amr_data[gas_id].amr_hr_sid_update_tx_in_progress++;

#ifdef GL1_TCH_AMR_DEBUG
                MSG_GERAN_HIGH_3_G( "TXing Init State HR SID_UPDATE, cmi %d, cmr %d, cnt %d",
                          curr_ul_cmi[gas_id], curr_cmr[gas_id], l1_tch_amr_data[gas_id].amr_no_data_cnt);
                l1_tch_amr_data[gas_id].amr_no_data_cnt = 0;
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_UPDATE_TX);
#endif
            }
            else
            {
#ifdef GL1_TCH_AMR_DEBUG
                MSG_GERAN_HIGH_0_G("Skipping HR SID_UPDATE");
#endif
            }

            if( l1_tch_amr_data[gas_id].amr_nsync_cnt == AMR_MAX_NSYNC_CNT )
            {
                l1_tch_amr_data[gas_id].amr_new_tx_state = AMR_DTX;
                l1_tch_amr_data[gas_id].amr_nsync_cnt = 0;
            }
        }
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        else
        {
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_NO_DATA_TX);
        }
#endif
        break;

      default:
        MSG_GERAN_ERROR_1_G("Invalid Frame Type: %d", amr_frame);
        break;
    }
}

/*===========================================================================
FUNCTION amr_tch_rx_callback_hr

DESCRIPTION
   This function is called when the data from the TCH has been decoded. The
   data is then passed to the vocoder or up to L2 if it is a FACCH message.
   Receiver Ready (layer 2) are filtered and not passed on up to reduce
   overhead.

   This function implements the RX side state machine for AMR Half Rate.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
static void amr_tch_rx_callback_hr (gl1_msg_decode_rpt *rpt_i, gas_id_t gas_id )
{
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    tch_facch_decode_rpt * rpt = (tch_facch_decode_rpt*)rpt_i;

    gl1_msg_amr_frame_info_type frame;
    boolean cmi_rcvd = FALSE, cmc_rcvd = FALSE;
    uint16 amr_status = 0;
    uint8 dl_acs = 0, ul_acs = 0;
    uint8 i, ack_act_counter = 0;
    boolean compute_bep = FALSE;
#ifdef FEATURE_GSM_RX_DIVERSITY
    gl1_rxd_control_type gl1_rxd_ctl_flag;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

    if(!rpt)
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
        return;
    }

    if ( l1_tch_amr_data[gas_id].amr_first_rx_block )
    {
       l1_tch_amr_data[gas_id].bep_index = 0;
    }

    /* Accumulate 4BEPs from 2 HR blocks */
    for ( i = 0; i < 2; i++ )
    {
      l1_tch_amr_data[gas_id].bep[l1_tch_amr_data[gas_id].bep_index++] = rpt->hdr.bep[i];
    }

    /* After accumulating 4 bursts, copy it back to rpt hdr to pass
     * down to gl1_update_emr_parameters
     */
    if ( l1_tch_amr_data[gas_id].bep_index == 4 )
    {
      for ( i = 0; i < l1_tch_amr_data[gas_id].bep_index; i++ )
      {
        rpt->hdr.bep[i] = l1_tch_amr_data[gas_id].bep[i];
#ifdef DEBUG_GSM_GPRS_ENHANCED_MEASUREMENT_REPORTING
        #error code not present
#endif
      }
      compute_bep = TRUE;
      l1_tch_amr_data[gas_id].bep_index = 0;
    }

    /* Do RXLEV_VAL and BEP processing */
    gl1_update_emr_parameters ( &(rpt->hdr), FALSE, compute_bep, gas_id );

    if( l1_tch_amr_data[gas_id].amr_first_rx_block )
    {
#ifdef TCH_PROFILING
       l1_tch_amr_data[gas_id].total_crc_cnt_hr = l1_tch_amr_data[gas_id].total_crc_bad_hr = l1_tch_amr_data[gas_id].total_bfi_cnt_hr = 0;
       l1_tch_amr_data[gas_id].crc_cnt_hr = l1_tch_amr_data[gas_id].crc_bad_hr = l1_tch_amr_data[gas_id].bfi_cnt_hr = 0;
#endif
       l1_tch_amr_data[gas_id].amr_first_rx_block = FALSE;
       return;
    }

    ack_act_counter = ratscch_update_ack_activation_counter(gas_id);

    if( rpt->hdr.onset )
    {
#ifdef GL1_TCH_AMR_DEBUG
        MSG_GERAN_HIGH_0_G("Rcvd ONSET frame from MDSP");
#endif
        cmi_rcvd = TRUE;
        frame.dl_info.frame = GL1_MSG_AMR_ONSET;
        /* If onset flag is set, reset the channel type from GL1_MSG_TCH_AHS
         * to GL1_MSG_ONSET just for logging purpose
         */
        rpt->hdr.chan = (uint16)GL1_MSG_ONSET;
        l1_tch_amr_data[gas_id].amr_rx_state = AMR_TCH;
    }
    else
    {
        switch(rpt->hdr.chan)
        {
        case GL1_MSG_FACCH_AHS:
            
            l1_log_rfacch_metrics((gl1_msg_decode_rpt*)rpt, gas_id);

            /*check to see if R-FACCH support is active*/
            if (l1_handle_rfacch(rpt, GL1_MSG_FACCH_AHS, gas_id) )
            {

#ifdef GL1_TCH_AMR_DEBUG
               MSG_GERAN_HIGH_0_G("Rcvd HR FACCH frame from MDSP");
#endif
               L1_send_PH_DATA_IND (TRUE,
                                    DCCH,
                                    FALSE,  /* no paging involved */
                                    NULL_ARFCN,   /*ARFCN only in idle mode*/
                                    rpt->data,
                                    N201_MAX, gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
               gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_VALID_FACCH_RX);
#endif
            }

            if (!rpt->hdr.good_data)
            {
                l1_tch_amr_data[gas_id].crc_bad_hr++;
                l1_tch_amr_data[gas_id].total_crc_bad_hr++;
            }

            /* BFI always set to 1 for FACCH */
            /* DSP supposed to handle this but this is just to be sure */
            rpt->hdr.bfi = 1;
            frame.dl_info.frame = GL1_MSG_AMR_NO_DATA;
            break;

        case GL1_MSG_RATSCCH_HS:
#ifdef GL1_TCH_AMR_DEBUG
            MSG_GERAN_HIGH_0_G("Rcvd HR RATSCCH frame from MDSP");
#endif

            ratscch_process_req(rpt->data, rpt->hdr.good_data,gas_id  );

            frame.dl_info.frame = GL1_MSG_AMR_NO_DATA;

            if( !rpt->hdr.good_data )
            {
                l1_tch_amr_data[gas_id].crc_bad_hr++;
                l1_tch_amr_data[gas_id].total_crc_bad_hr++;
            }

            cmc_rcvd = ( (cmi_rcvd = (boolean)(rpt->hdr.phase == 1) )==TRUE)?FALSE:TRUE;

            break;

        case GL1_MSG_RATSCCH_MARKER:
#ifdef GL1_TCH_AMR_DEBUG
            MSG_GERAN_HIGH_0_G("Rcvd RATSCCH_MARKER frame from MDSP");
#endif

            frame.dl_info.frame = GL1_MSG_AMR_NO_DATA;

            cmc_rcvd = ( (cmi_rcvd = (boolean)(rpt->hdr.phase == 1) )==TRUE)?FALSE:TRUE;

            break;

        case GL1_MSG_SID_FIRST_HS:
#ifdef GL1_TCH_AMR_DEBUG
            MSG_GERAN_HIGH_0_G("Rcvd HR SID_FIRST frame from MDSP");
#endif

            l1_tch_amr_data[gas_id].amr_rx_state = AMR_DTX;

            frame.dl_info.frame = GL1_MSG_AMR_SID_FIRST;

            cmi_rcvd = cmc_rcvd = TRUE;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_FIRST_RX);
#endif
            break;

        case GL1_MSG_SID_FIRST_INH:
#ifdef GL1_TCH_AMR_DEBUG
            MSG_GERAN_HIGH_0_G("Rcvd SID_FIRST_INH frame from MDSP");
#endif

            frame.dl_info.frame = GL1_MSG_AMR_NO_DATA;

            cmc_rcvd = ( (cmi_rcvd = (boolean)(rpt->hdr.phase == 1) )==TRUE)?FALSE:TRUE;

            break;

        case GL1_MSG_SID_UPDATE_HS:
            /*
             * For SID_UPDATE message, both CMI and CMC are valid
             */
            l1_tch_amr_data[gas_id].amr_rx_state = AMR_DTX;

            if( rpt->hdr.good_data )
            {
                frame.dl_info.frame = GL1_MSG_AMR_SID_UPDATE;
#ifdef GL1_TCH_AMR_DEBUG
                MSG_GERAN_HIGH_0_G("Rcvd HR SID_UPDATE frame from MDSP");
#endif
            }
            else
            {
                frame.dl_info.frame = GL1_MSG_AMR_SID_BAD;
#ifdef GL1_TCH_AMR_DEBUG
                MSG_GERAN_HIGH_0_G("Rcvd HR SID_BAD frame from MDSP");
#endif
                l1_tch_amr_data[gas_id].crc_bad_hr++;
                l1_tch_amr_data[gas_id].total_crc_bad_hr++;
            }

            cmi_rcvd = cmc_rcvd = TRUE;

            gl1_msg_rxqual_sub_update( (uint16)rpt->hdr.rx_qual, gas_id );
#ifdef RXQUAL_MSG
            MSG_GERAN_MED_1_G("Update: rxqual_sub tch(dtx) = %d",rpt->hdr.rx_qual);
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_SID_UPDATE_RX);
#endif

            break;

        case GL1_MSG_SID_UPDATE_INH:
#ifdef GL1_TCH_AMR_DEBUG
            MSG_GERAN_HIGH_0_G("Rcvd SID_UPDATE_INH frame from MDSP");
#endif
            frame.dl_info.frame = GL1_MSG_AMR_NO_DATA;
            l1_tch_amr_data[gas_id].amr_rx_state = AMR_TCH;
            cmi_rcvd = TRUE;
            break;

        case GL1_MSG_TCH_AHS:
            /* Half  Rate Speech Frame */
            if( !rpt->hdr.bfi )
            {
                if(rpt->hdr.rx_qual > amr_tch_hr_thresholds[curr_dl_acs[gas_id][rpt->hdr.cmi]])
                {
                    l1_tch_amr_data[gas_id].amr_rx_state = AMR_TCH;

                    frame.dl_info.frame = GL1_MSG_AMR_SPEECH_GOOD;

                    cmc_rcvd = ( (cmi_rcvd = (boolean)(rpt->hdr.phase == 1) )==TRUE)?FALSE:TRUE;

#ifdef GL1_TCH_AMR_DEBUG
                    MSG_GERAN_HIGH_0_G("Rcvd HR SPEECH_GOOD frame from MDSP");
#endif
                }
                else
                {
                    if( l1_tch_amr_data[gas_id].amr_rx_state == AMR_DTX )
                    {
                        frame.dl_info.frame = GL1_MSG_AMR_NO_DATA;
#ifdef GL1_TCH_AMR_DEBUG
                        MSG_GERAN_HIGH_0_G("Rcvd HR NO_DATA frame from MDSP");
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                        gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_NO_DATA_RX);
#endif
                    }
                    else
                    {
                        frame.dl_info.frame = GL1_MSG_AMR_SPEECH_DEGRADED;

                        cmc_rcvd = ( (cmi_rcvd = (boolean)(rpt->hdr.phase == 1) )==TRUE)?FALSE:TRUE;
#ifdef GL1_TCH_AMR_DEBUG
                        MSG_GERAN_HIGH_0_G("Rcvd HR SPEECH_DEGRADED frame from MDSP");
#endif
                    }
                }
            }
            else
            {
                if( l1_tch_amr_data[gas_id].amr_rx_state == AMR_DTX )
                {
                    frame.dl_info.frame = GL1_MSG_AMR_NO_DATA;
#ifdef GL1_TCH_AMR_DEBUG
                    MSG_GERAN_HIGH_0_G("Rcvd HR NO_DATA frame from MDSP");
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                    gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_DTX_NO_DATA_RX);
#endif
                }
                else
                {
                    cmc_rcvd = ( (cmi_rcvd = (boolean)(rpt->hdr.phase == 1) )==TRUE)?FALSE:TRUE;
                    frame.dl_info.frame = GL1_MSG_AMR_SPEECH_BAD;
#ifdef GL1_TCH_AMR_DEBUG
                    MSG_GERAN_HIGH_0_G("Rcvd HR SPEECH_BAD frame from MDSP");
#endif
                }
                l1_tch_amr_data[gas_id].crc_bad_hr++;
                l1_tch_amr_data[gas_id].total_crc_bad_hr++;
            }
            break;

        default:
            MSG_GERAN_ERROR_0_G("Invalid frame rcvd from MDSP");
            frame.dl_info.frame = GL1_MSG_AMR_NO_DATA;
            break;
        }
    }

    /*
     * If the cmi received is greater than the number of codec modes -1
     * in D/L ACS or cmc received is greater than the number of codec
     * modes in U/L ACS, print an F3 message. Use the previous CMI/CMC
     */
    if( cmi_rcvd )
    {
        if( rpt->hdr.cmi >= curr_dl_num_acm[gas_id] )
        {
            MSG_GERAN_HIGH_2_G("Rcvd CMI %d out of bounds wrt to ACS", rpt->hdr.cmi,
                      0);
        }
        else
        {
            curr_dl_cmi[gas_id] = (uint8)rpt->hdr.cmi;
#ifdef GL1_TCH_AMR_DEBUG
            MSG_GERAN_HIGH_1_G("Rcvd CMI = %d", curr_dl_cmi[gas_id]);
#endif
        }
    }

    if(cmc_rcvd)
    {
        if( rpt->hdr.cmc >= curr_ul_num_acm[gas_id] )
        {
            MSG_GERAN_HIGH_2_G("Rcvd CMC %d out of bounds wrt to ACS", rpt->hdr.cmc,
                      0);
        }
        else
        {
            l1_amr_store_cmc( (uint8)rpt->hdr.cmc, gas_id );

            if(!l1_tch_amr_data[gas_id].amr_cmc_rcvd)
            {
                l1_tch_amr_data[gas_id].amr_cmc_rcvd = TRUE;
            }

#ifdef GL1_TCH_AMR_DEBUG
            MSG_GERAN_HIGH_1_G("Rcvd CMC = %d", (uint8)rpt->hdr.cmc);
#endif
        }
    }

    /*
     * Now map the curr_dl_cmi received to the vocoder codec mode
     */
    frame.dl_info.mode = curr_dl_acs[gas_id][curr_dl_cmi[gas_id]];

    if(ack_act_counter == 10)
    {
        l1_tch_amr_data[gas_id].amr_need_to_expire_ack_act_timer = TRUE;
    }

    if( curr_phase[gas_id] == CMI_PHASE_ODD )
    {
        /*
         * If this is a CMI frame, don't give CMC to the vocoder.
         * Note that if CMC is received in this frame because
         * it is a SID_UPDATE or RATSCCH frame, we have saved
         * the received CMC in curr_cmc above and will apply
         * in the next speech frame which should be CMC frame.
         */

    if(l1_tch_amr_data[gas_id].amr_need_to_expire_ack_act_timer)
    {
        /* Initialize curr_cmc, new_cmc to the new icm */
        curr_cmc[gas_id] = new_cmc[gas_id] = new_ul_icm[gas_id];
        ratscch_set_ul_voc_amr_mode_using_new_icm(gas_id);
    }
        /* If amr_rx_phase = 0 then rcvd frame is a CMI frame
         * If amr_rx_phase = 1 then rcvd frame is a CMC frame
     */
        else if ( l1_tch_amr_data[gas_id].amr_rx_phase )
    {
        l1_tch_amr_data[gas_id].amr_cmc_changed = l1_amr_update_cmc(gas_id);

        if( l1_tch_amr_data[gas_id].amr_cmc_changed )
        {
 
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
            l1_vs_set_ul_amr_mode( curr_ul_acs[gas_id][curr_cmc[gas_id]], gas_id );
#endif /* FEATURE_MODEM_NO_VOICE_SUPPORT */

            /*
             * This is where we are commanding the vocoder to apply
             * the new codec mode. Log this information, so that
             * we can check when the vocoder actually applies the
             * new codec mode.
             */
            l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.phase = curr_phase[gas_id];
            l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.cmc = curr_cmc[gas_id];
            l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.frame_count = 0;
            l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.valid = TRUE;
        }
    }
    }
    else
    {
        /*
         * If this is a CMI frame, give the CMC to the vocoder.
         * Note that if CMC is received in this frame because
         * it is a SID_UPDATE or RATSCCH frame, we have saved
         * the received CMC in curr_dl_cmc above.
         */
        if(l1_tch_amr_data[gas_id].amr_need_to_expire_ack_act_timer)
        {
            /* Initialize curr_cmc, new_cmc to the new icm */
            curr_cmc[gas_id] = new_cmc[gas_id] = new_ul_icm[gas_id];
            ratscch_set_ul_voc_amr_mode_using_new_icm(gas_id);
        }
        /* If amr_rx_phase = 0 then rcvd frame is a CMI frame
         * If amr_rx_phase = 1 then rcvd frame is a CMC frame
         */
        else if ( !l1_tch_amr_data[gas_id].amr_rx_phase )
        {
            l1_tch_amr_data[gas_id].amr_cmc_changed = l1_amr_update_cmc(gas_id);

            if( l1_tch_amr_data[gas_id].amr_cmc_changed )
            {
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
              l1_vs_set_ul_amr_mode( curr_ul_acs[gas_id][curr_cmc[gas_id]], gas_id );
#endif /* FEATURE_MODEM_NO_VOICE_SUPPORT */

                /*
                 * This is where we are commanding the vocoder to apply
                 * the new codec mode. Log this information, so that
                 * we can check when the vocoder actually applies the
                 * new codec mode.
                 */
                l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.phase = curr_phase[gas_id];
                l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.cmc = curr_cmc[gas_id];
                l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.frame_count = 0;
                l1_tch_amr_data[gas_id].amr_voc_turnaround_time_data.valid = TRUE;
            }
        }
    }

    /*--------------------------------------------------------------------------
     * Generate CMR only if we did not receive a NO_DATA frame from the MDSP
     * and not a SID UPDATE with failed CRC.
     *-------------------------------------------------------------------------*/
    if( frame.dl_info.frame != GL1_MSG_AMR_NO_DATA &&
        frame.dl_info.frame != GL1_MSG_AMR_SID_BAD )
    {
        for(i = 0; i < 2; i++ )
        {
            /*
             * Saturate c_over_i before passing to the filter
             */
            if ( l1_tch_amr_data[gas_id].amr_saved_c_over_i[i] > AMR_MAX_C_OVER_I )
            {
               l1_tch_amr_data[gas_id].amr_saved_c_over_i[i] = AMR_MAX_C_OVER_I;
            }
         /*CR228666 - call FR filter twice*/
         filter_c_over_i_fr( l1_tch_amr_data[gas_id].amr_saved_c_over_i[i],gas_id);
         filter_c_over_i_fr( l1_tch_amr_data[gas_id].amr_saved_c_over_i[i],gas_id);

        }

        gen_codec_mode_request(gas_id);
    }


    /*--------------------------------------------------------------------------
     * LOG MESSAGE METRICS
     *-------------------------------------------------------------------------*/
    amr_status |= (uint16) (((rpt->hdr.phase << 12) |
                             (curr_ul_icm[gas_id] << 10)    |
                             (curr_dl_icm[gas_id] << 8 )    |
                             (curr_ul_cmi[gas_id] << 6)     |
                             (curr_dl_cmi[gas_id] << 4 )    |
                             (curr_cmr[gas_id] << 2 ) |
                             curr_cmc[gas_id]) );

    for( i = 0; i < curr_dl_num_acm[gas_id]; i++ )
    {
        dl_acs |= (uint8)(1 << (uint8)curr_dl_acs[gas_id][i]);
    }

    for( i = 0; i < curr_ul_num_acm[gas_id]; i++ )
    {
        ul_acs |= (uint8)(1 << (uint8)curr_ul_acs[gas_id][i]);
    }

    l1_log_message_metrics_amr((gl1_msg_decode_rpt*)rpt,
                               (uint8)curr_dl_acs[gas_id][curr_dl_cmi[gas_id]],
                               amr_status,
                               dl_acs,
                               ul_acs,
                               (uint16)curr_norm_c_over_i[gas_id], gas_id);


    l1_log_message_metrics_compact_amr((gl1_msg_decode_rpt*) rpt,
                                      (uint8)curr_dl_acs[gas_id][curr_dl_cmi[gas_id]], gas_id);

    {
       eng_mode_amr_info_t amr_info = {0};

       amr_info.ul_chan_mode = (uint8)l1_get_tch_chan_mode(gas_id);
       amr_info.dl_chan_mode = (uint8)l1_get_tch_chan_mode(gas_id);

       for (i = 0; i < MAX_CODEC_MODES; i++)
       {
         amr_info.ul_acs[i] = (uint8)curr_ul_acs[gas_id][i];
         amr_info.dl_acs[i] = (uint8)curr_dl_acs[gas_id][i];

         if (amr_info.ul_acs[i] == GL1_MSG_AMR_MODE_UNDEF)
         {
           amr_info.ul_acs[i] = 0xFF;
         }

         if (amr_info.dl_acs[i] == GL1_MSG_AMR_MODE_UNDEF)
         {
           amr_info.dl_acs[i] = 0xFF;
         }
       }

       if (l1_tch_amr_data[gas_id].amr_rx_state == AMR_DTX)
       {
         amr_info.dl_dtx = TRUE;
       }
       else
       {
         amr_info.dl_dtx = FALSE;
       }

       amr_info.dl_c_over_i = (uint16)curr_norm_c_over_i[gas_id];
       geran_eng_mode_data_write(ENG_MODE_AMR_INFO, (void *)&amr_info, TRUE, gas_id);
    }

    l1_vs_send_amr_packet_to_client(rpt->data, &frame ,gas_id);

    /*
     * Update the Counters of RATSCCH handler here. It will
     * have no effect if there is not RATSCCH request pending. Note that
     * the RATSCCH request should take effect in the N+12 speech frame.
     */
    ratscch_update_req_activation_counter(gas_id);

#ifdef FEATURE_GSM_DYNAMIC_SAIC
    if(gl1_hw_feature_disabled_saic(gas_id) != TRUE)
    {
      MSG_GERAN_LOW_3_G(" RXQUAL %d saic d/e %d state %d",rpt->hdr.rx_qual,gl1_msg_get_saic_state(gas_id),
                                              l1_dedicated_data_ptr->l1_saic_dis_ena_state);

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
    MSG_GERAN_MED_1_G("Update: rxqual_full tch = %d",rpt->hdr.rx_qual);
#endif

#ifdef TCH_PROFILING
    if (rpt->hdr.bfi && (gl1_msg_chan_type)rpt->hdr.chan != GL1_MSG_FACCH_AHS)
    {
       l1_tch_amr_data[gas_id].total_bfi_cnt_hr++;
       l1_tch_amr_data[gas_id].bfi_cnt_hr++;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
       gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_BFI_RCVD);
#endif
    }

    l1_tch_amr_data[gas_id].total_crc_cnt_hr++;
    if( ++l1_tch_amr_data[gas_id].crc_cnt_hr >= 100 )
    {
        MSG_6(MSG_SSID_DFLT, MSG_LEGACY_MED,
              "CRC errors: %d/100, %d/%d  (BFI: %d/100, %d/%d)",
              l1_tch_amr_data[gas_id].crc_bad_hr, l1_tch_amr_data[gas_id].total_crc_bad_hr, l1_tch_amr_data[gas_id].total_crc_cnt_hr, l1_tch_amr_data[gas_id].bfi_cnt_hr, l1_tch_amr_data[gas_id].total_bfi_cnt_hr,
              l1_tch_amr_data[gas_id].total_crc_cnt_hr);

        l1_tch_amr_data[gas_id].crc_cnt_hr = 0;
        l1_tch_amr_data[gas_id].crc_bad_hr = 0;
        l1_tch_amr_data[gas_id].bfi_cnt_hr = 0;
    }
#endif

}/* Suppress warning "crc_bad not accessed" */ /*lint !e550*/


/*===========================================================================
FUNCTION amr_tch_metrics_callback_hr

DESCRIPTION
   This function is called when the burst metrics has been decoded. The data
   is used to generate the U/L Codec Mode Request amongst other things

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void  amr_tch_metrics_callback_hr ( gl1_msg_metrics_rpt *rpt[], uint16 chan, gas_id_t gas_id  )
{
    l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

    int i;

    if(!rpt)
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
        return;
    }

  /* Store rx power over the current blocks for RXLEV_VAL computation later */
   gl1_store_block_rx_power ( rpt, 2, gas_id  );

    if( l1_get_serv_pwr_meas_permitted(gas_id) )
    {
        /* Assuming a FULL RATE channel type here */
        for(i=0; i<2; i++)
        {
            if( (gl1_msg_chan_type)chan == GL1_MSG_SID_UPDATE_HS)
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
                      l1_serving_cell_meas_ptr->divrx_power_sub   += rpt[i]->pwr_dBm_x16;
                      }
                    else
                      {
                      l1_serving_cell_meas_ptr->divrx_power_sub  += rpt[i]->pwr_dBm_x16_divrx;
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
                   l1_serving_cell_meas_ptr->divrx_power   += rpt[i]->pwr_dBm_x16;
                   }
                 else
                   {
                   l1_serving_cell_meas_ptr->divrx_power   += rpt[i]->pwr_dBm_x16_divrx;
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


    /* Only update the AFC on an  SID_UPDATE  if the UE is DL DTXing here. */
    /* The AMR_TCH test picks up all but SID_FIRST,SID_UPDATE,and NO_DATA */
    if( l1_tch_amr_data[gas_id].amr_rx_state == AMR_TCH ||
        (gl1_msg_chan_type)chan == GL1_MSG_FACCH_AHS    ||
        (gl1_msg_chan_type)chan == GL1_MSG_SID_FIRST_HS ||
        (gl1_msg_chan_type)chan == GL1_MSG_SID_UPDATE_HS )
    {
      /* Update AFC/TT */
      for(i = 0; i < 2; i++)
      {
        /* If SAIC is used in a burst the mDSP sets bit 14 of */
        /* the misc_flag true. Additionally, AFC/TT tracking  */
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
    }

    /* Compute mean c_over_i to be fed to the codec mode adaptor */
    for(i = 0; i < 2; i++ )
    {
#ifdef FEATURE_GSM_DYNAMIC_SAIC
    /* For dynamic SAIC, SAIC is only disabled unitl Rx Qual is not max */
    /* so if SAIC is disabled set C/I to max */
        if( (gl1_hw_feature_disabled_saic(gas_id) != TRUE) && (gl1_msg_get_saic_state(gas_id) != TRUE) )
        {
            l1_tch_amr_data[gas_id].amr_saved_c_over_i[i] =  AMR_MAX_C_OVER_I;
        }
        else
#endif
        {
            l1_tch_amr_data[gas_id].amr_saved_c_over_i[i] =  rpt[i]->c_over_i;
        }
    }

    /* LOG BURST METRICS */
    l1_log_burst_metrics ( (gl1_msg_chan_type)chan, rpt, gas_id  );

#if  defined (DEBUG_FEATURE_GSM_R_FACCH)
    #error code not present
#endif
}


/*===========================================================================
FUNCTION l1_amr_get_hr_subchannel

DESCRIPTION
   This function returns the value of amr_hr_variable

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
uint8 l1_amr_get_hr_subchannel(gas_id_t gas_id)
{
  return l1_tch_amr_data[gas_id].amr_hr_subchannel;
}

/*===========================================================================
FUNCTION l1_amr_get_channel_type

DESCRIPTION
   This function returns the value of amr_hr_variable

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
gl1_msg_chan_type l1_amr_get_channel_type(gas_id_t gas_id)
{
  return l1_tch_amr_data[gas_id].amr_channel_type;
}

/*===========================================================================

FUNCTION  l1_get_tch_amr_dtx_state

DESCRIPTION
  This function returns if the tch state machine is currently in dtx mode

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_get_tch_amr_dtx_state(gas_id_t gas_id)
{
   return (l1_tch_amr_data[gas_id].amr_curr_tx_state == AMR_DTX);
}

#ifdef FEATURE_GSM_COEX_SW_CXM
/*===========================================================================

FUNCTION  l1_tch_amr_get_arfcns

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
static void l1_tch_amr_get_arfcns(int16 start, uint16 increment, uint16 num_arfcns, ARFCN_T arfcns[], gas_id_t gas_id)
{
  dedicated_data_T *ded_dtr_ptr = &l1_dedicated_data[gas_id];

  L1_get_ARFCNS ( start, increment, num_arfcns, arfcns,
                  &ded_dtr_ptr->channel_specification.channel_info_1_before,
                  &ded_dtr_ptr->channel_specification.channel_info_1_after,
                  &ded_dtr_ptr->new_channel_info, gas_id );
}
#endif /* FEATURE_GSM_COEX_SW_CXM */

#endif /* FEATURE_GSM_AMR */
