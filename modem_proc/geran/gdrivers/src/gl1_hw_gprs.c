/*===========================================================================

        IMPLEMENTATION OF FUNCTIONS COMMON TO GSM GPRS L1 FRAME LAYER

GENERAL DESCRIPTION
   This file implements functions that are common the GSM and GPRS Non-Portable
   Layer 1 (NPL1) frame layer functions.
   Since the Frame Layer interacts with the hardware/DSP it is denoted as 'hw'

EXTERNALIZED FUNCTIONS

gl1_hw_init
gl1_hw_terminate


INITIALIZATION AND SEQUENCING REQUIREMENTS
The following is the desired order in which functions should be called

gl1_hw_init
(All other functions )
gl1_hw_terminate


Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_gprs.c#2 $ $DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
27/01/20    rks     CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
25/04/18    rv      CR2225983 - GERAN F3 and log pkt Reduction TA.3.0
10/08/16    cws     CR1044687 Use ARFCN from pwr sm directly for pwr monitor log packet
23/09/16    km      CR1038585: Reset header egprs.hdr_crc_passed for logging improvement
08/04/16    og      CR993589. Stack usage reduction.
28/02/17    og      CR2006578. FRs 39363 and 39364.
27/05/16   snjv     CR1000909 pass ACI400 switchpoint detection stat from GFW to GRF via GL1
24/05/16    sn      CR1016717:Removing Excess F3 logging for packet transfer
17/09/15    jk      CR901965:Correcting logging issues when PDTCH bursts are cancelled
21/07/15    jk      CR864645:Data RxD ON/OFF changes for GCF TC 14.18.3 & 14.18.1
07/01/15    jj      CR777805 Add probe burst flag for diversity rx
28/02/15    cja     CR768169 Add RF Seq Number
23/02/15    jk      CR793098:LIF additional logging changes
21/01/15    jk      CR782488:Use updated gain tags for PRx and DRx in DTM mode
16/12/14    jk      CR753423:LIF for Idle,voice and data
23/09/14    jj      CR 728792  Discrete Fourier transform (DFT) spur metric log packet
12/09/14	pa		CR723754 Initialise skip flag on scheduling interference measurement.  
09/09/14    jk      CR697308:GL1 changes to support HLin LLin sawless feature
04/09/14    am      CR682760 Addressing KW GL1 P1 errors
14/04/14    pjr     CR641577 Replaced FN with Seq Number in cxm log table
23/07/14    pjr     CR682764 TP IR Feature - implement IR mem Full handling from GFW to GRLC
10/07/14    jk      CR688587:GL1 changes to support LIF feature
15/05/14    jj      CR 664355 Memclr rx_chan_params to avoid junk values in rx params send to rf 
30/05/14    jj      CR656854  Resolve Critical Klocwork errors in gdrivers code space
17/04/14    cos     CR636543 - Changes for enabling Rx Diversity on Bolt - GL1/GRF intf updates
07/04/14    cja     CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF and FEATURE_GSM_GPRS_QDSP6
19/12/13    jk      CR503359 - GL1 changes to support Rx diversity 
13/12/13    mc      CR.583042 : COEX : DSDA CS + PS - PS call drops in RxTx overlap scenario
09/11/13    mc      CR.572746 : COEX : DSDA G+G VBATT power back-offs for TxTx collisions are not correctly applied
03/01/14    jk      CR572347 - GL1 FTM API changes to support Rx diversity
10/12/13    ws      CR 587586 - Remove DUAL_SIM featurisation in API
28/11/13    cja     CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
22/11/13    cja     CR455527 Remove featurisation from files in API directory
11/18/13    pjr     CR563534 Fix incorrect channel type shown in CXM logs packets
24/09/13    sp      CR482765 - Define new packet for EDGE Enhanced Log update
24/09/13    sp      CR449651: Retrieve Burst data as per updated GFW-GL1 interface
12/08/13    og      CR521847. Featurize out the calls to rfm_gsm_set_high_multislot_mode( ).
17/09/13    pjr     CR538893 Changes to enable CXM logging
03/09/13    npt     CR524770 - GL1 changes to support RFLM FED 
21/08/13    npt     CR531633 - Remove FEATURE_DIME_MODEM dependencies
15/08/13    sk      CR524039 GBTA bringup changes
29/07/13    sk      CR519663 Partial QBTA bringup changes
7/16/13     zc       CR470197 Replace memcpy with memscpy
25/04/13    pjr     CR480105 Updated parameters for mdsp_dtm_abort_rx
25/03/13    mc      CR.464496 : COEX Support - GERAN arbitrator SW CXM integration
13/03/13    mc      CR.459487 : COEX Support - GERAN arbitrator framework
06/02/13    jk      CR449735: Resolving KW errors
05/12/12    pg      CR427361: GFW/RF shared memory interface cleanup  
05/11/12   cja      CR414098 Tidy-up F3 for MCPM builds 
31/07/12    ky      Fixed the compilation issues after removing the redundant strcutures
                    in SW INTF.h file
20/06/12    ky      Added support of using new CCS interface and passing new CCS
                    buffer pointers in RF task messages
25/05/12    pg      Add support for RF Task
26/03/12    jj      CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
31/10/11    jj      CR315402 fix for interference measurement crash on HMSC
08/09/11    jj      CR302378  Added GL1 changes for RSB along with spectral inversion
12/07/11    cja     Remove include of rfllgsm.h
20/04/11    cja     Add spectral inversion setting for Nikel
18/03/11    ws      Added Initial MCPM updates
17/03/11    cja     CR279518 Add single slot FTM power store for GPRS/EDGE
24/11/10    cja     Integrate HMSC
24/09/10    cja     Fix Klocwork warnings
03/09/10    ws      Added FEATURE_GSM_MDSP_AEQ_SCH for QDSP6 targets
29/06/10    ky      Added the performace logging for gstmr handlers and rf api's
05/05/10    pg      Add AEQ metrics logging for QDSP6
23/03/10    cs      CR230952 Deregister NPA idle client in EDGE mode
04/03/10    ip      CR228168 Added DEVMAN code changes for mDSP speed
13/01/10    pg      Fixed typo for reading rssi into metrics report
04/11/09    ip      Added first draft of NPA for cpu clock resource
28/08/09    ip      CR193535 added for demand paging fault
05/08/09    ws      Removed FEATURE_GSM_MDSP_DTM it's now mainlined
30/03/09    ws      Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
01/12/08    cs      Add in new gl1_hw_clk_ctl.h
17/09/08    ws      Backed out leaving MDSP clock on at 122 Mhz for LCU since
                    it's not required for B0
04/08/08    ws      Initialised  metrics_data mdsp structure for klowork warning
29/07/08    ws      mdsp Stucture initialisations to resolve Klockwork warnings
30/04/08    ws      Leave Mdsp clock at 122 Mhz for LCU so don't turn off
14/03/08    agv     Added array init for QE2. Possible glitch if target is a non-QE2 RF.
14/02/08    nf      Added feature enabling other Tech L1's to find if GSM is in a TX state
14/01/08    agv     New feature for Switched Mode Power Supply which will
                    provide better talk time.
15/01/08    ws      Added suport for ESIAC logging and SAIC AFC in PTM
21/09/07    cs/ip   Unified GL1 clock control
17/07/07    ws      CR123746 Add LNA_GAIN_RANGE to rfm_set_rx_gain() API
28/11/06    ws      High Lint warning fixes
14/08/06    ws      Allow gl1_hw_set_egprs_mode() to execute without
                    registering or de-registering the EDGE CLK
31/07/06    agv     CR85810 NB AB TSC, and TA fix.
05/18/06    gfr     Support for clock regime resource manager.
03/14/06    ws      Corrected prototype for rfm_set_amam_profile()
02/17/06    gfr     Added modulation for power backoff to RF Driver.
01/13/06    gfr     Support for new RFM API (FEATURE_RFM_INTERFACE)
11/04/05    gfr     Pass EGPRS mode to rfgsm_set_amam_profile
10/31/05    gfr     Delay disabling of EDGE clock by one frame to avoid turning
                    it off before the mDSP is done using it.
10/07/05    gfr     Improve ACI detection featurization
08/30/05    gfr     Register for EDGE CPU resources when entering GSM or GPRS
                    since in DTM we cannot change mDSP clock speed on-the-fly
08/19/05    gfr     QLint cleanup
07/29/05    gfr     DTM support
06/21/05    gfr     Add gl1_hw_schedule_tt_ok and rework schedule_frame_delay,
                    move all schedule code to gl1_hw_sched.c
06/10/05    gfr     Use correct offset when scheduling gstmr adjustments
06/07/05    gfr     Enable/disable EDGE clock regimes when necessary
06/01/05    gfr     Add support for narrow ACI filter
04/18/05    gfr     Rename mdsp_pdch_id to mdsp_timeslot_type
05/18/05    gfr     Add extra delay between interference and power measures
05/17/05    yh      Use channel filter stage2 as default.
05/04/05    gfr     Only call rfgsm_set_pa_profile if FEATURE_POLAR is not on.
05/02/05    gfr     Use correct interference measure length
04/26/05    gfr     Handle invalid rx metrics gracefully
04/22/05    gfr     Abort interference measures if a conflict is detected
04/21/05    gfr     Call rfgsm_compute_rx_power for each message
04/20/05    yh      Remove use of RF fifo
04/12/05    gfr     Consolidate GSM hw layer parameters
04/08/05    gfr     Allow caller to specify whether to discard untxed data
04/08/05    gfr     Combined GSM and GPRS channel types
04/08/05    gfr     Move scheduler prototypes into gl1_hwi.h
03/22/05    gfr     Use tx tag to keep track of encoded data, minor mDSP
                    interface cleanup
01/31/05    gfr     Support for NV based disabling of ACI detection.
01/27/05    gfr     Report negative RSSI values as an error and set to 1,
                    support for ACI filter used bits in the burst metrics.
01/25/05    gfr     Only enable ACI detection if the mDSP supports it
01/24/05    gfr     Support for new mDSP interference measure interface and
                    added gl1_hw_set_aci_detection
01/12/05    gfr     Support for 8PSK frequency error
12/15/04    kt      Support for ACI/CCI detection in mdsp
12/07/04    yhong   Lint cleanup
12/06/04    yhong   Support for FEATURE_GSM_MDSP_EGPRS
12/06/04    gfr     Featurize the mDSP SBI controller
12/08/04    gfr     Featurize polar RF support.
10/25/04    gfr     Fix bug in pwr measure scheduling if a conflict occurs.
10/08/04    yhong   Added conflict check for delay frame
04/09/04    gfr     When using firmware stage 3 for async rx, add extra delay.
09/02/04    gfr     Added ACI fields to burst metrics.
09/01/04    gfr     Increase guard period to avoid power measure conflicts.
08/26/04    gfr     Added first_burst parameter to gprs rx command.
07/29/04    gfr     Added FORCE_ZERO_8PSK_FREQ_ERROR feature.
07/22/04    gfr     EGPRS support.
06/12/04    gfr     Lint.
06/25/04    gfr     Better conflict check for non-contiguous interference measures.
06/10/04    gfr     Get the tx burst offset from the mDSP driver.
05/20/04    gfr     Mainlined FEATURE_COMMON_PWR_HANDLER
04/27/04    hg      Added FEATURE_MULTI_RF_CHAIN_API.
04/09/04    gfr     When using firmware stage 3, add extra delay.
04/08/04    gfr     Wrap the sample_ram scheduler with G2W ifdefs.
03/05/04    gfr     Lint cleanup.
02/23/04    gfr     Support for new rfgsm_set_pa_profile interface, and
                    changed timing offsets to unsigned.
02/11/04    gfr     New HW scheduler to support WCDMA.
01/30/04    gfr     Support for logging mdsp scaling factors.
01/12/04    gfr     Support for 6 monitors per frame.
01/07/04    bm      Mainlined RF_MULTIMODE
11/05/03    gfr     Support for quad-band.
08/27/03    gfr     Reduced how strict SCH schedule checking is to avoid corner
                    cases and keep power measure scheduling working properly.
08/26/03    gfr     Fixed another spurious conflict error between SCH and TCH.
08/21/03    gfr     Report PCH as BCCH instead of TCH in conflict messages.
08/20/03    gfr     Fixed spurious conflict error between SCH and TCH.
08/14/03    gfr     Fixed spurious conflict error with acquisition.
08/06/03    gfr     Removed the #undef FEATURE_HW_SCHEDULE_CHECK.
07/25/03    gfr     Print out a message when discarding a transmit for an SCH.
06/23/03    gfr     Int measures use RX tune time not mon tune time.  Clean up
                    some compiler warnings.
06/17/03    gfr     Different tune times for monitor versus rx bursts.
06/13/03    gfr     Do not schedule check stop acq when not in GSM/GPRS mode.
06/05/03    gfr     Do not use the timing advance when calculating the start
                    of the acquisition window.
06/04/03    gfr     Dynamically determine where any peripheral SBI transactions
                    should go in each frame.
06/04/03    gfr     Added scheduler support for calculating the start of the
                    acquisition window.
06/03/03    gfr     Pass last attempt parameter when looking for decoded data.
05/21/03    gfr     Compress monitors into available space.
05/20/03    gfr     Common power handler support.
05/15/03    bk      Issue a single set rx gain command for all rx bursts.
05/14/03    gfr     Check the acq window when scheduling power measures.
05/12/03    gfr     Schedule power measures in the host.
05/12/03    gfr     Tx Alpha is now sent in the tx command.
05/05/03    thh     As soon as we start receiving in GPRS, call
                    rfgsm_set_tx_setup_time() instead of waiting 'til Tx.'
                    There is just too much activities at the time we are ready
                    to transmit for the RF driver.
05/01/03    thh     Channel filter start/stop time is now handled by the RF
                    driver.
05/01/03    gfr     Change featuring from FEATURE_GSM to FEATURE_GSM_GPRS
04/24/03    thh     Added calls to rfgsm_set_tx_setup_time().
03/28/03    gfr     Log GPRS HW schedule if FEATURE_LOG_HW_SCHEDULE is
                    defined.  Replaces DEBUG_GPRS_SCHEDULE.
                    Log monitors using gl1_hw_log_monitor.
                    Report schedule conflict if PDTCHU occurs after SCH.
03/06/03    gfr     Add feature DEBUG_GPRS_SCHEDULE to dump out hw schedule
                    check information each frame.
02/24/03    gfr     Lint cleanup.
01/31/03    gfr     Do not report schedule conflicts between SCH and PDTCHU.
01/13/03     mk     Removed mdsp taf bit from ycm field.
01/02/03    gfr     Pass number of metrics to retrieve to the mdsp and moved
                    some defines into mdsp_gprs.h
12/13/02     sd     Changed tune commands to Rx tune commands, removing
                    dependency on PWR, MON, and RX tunes.
12/10/02    gfr     Do not pass up USFs if none were found in the mdsp
12/06/02     JC     Replaced rfgsm_set_rx_gain_data() with rfgsm_set_rx_gain()
11/15/02    gfr     Store the gain range and power status in the metrics.
10/11/02     ih     Changed gl1_hw_setup_* calls to rfgsm_setup_* (moved to
                    resolve circular dependency between HW and RF.
10/10/02     gr     Changed GPRS TX interface to accept TXLEV vs. power index.
09/24/02     gs     Supports USF Granularity on target
09/12/02     sd     Added support for RF driver multimode API that includes
                    GSM drivers.
09/09/02     gr     Return packet length based on coding scheme not mDSP
09/06/02     gr     Support for new uplink power control interface.
08/28/02     gr     Added DL abort functionality gl1_hw_gprs_reset().
07/23/02     gr     Returned all RF functions to use the prefix rfgsm_*().
06/28/02    thh     Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
06/27/02    thh     All RF functions now use the prefix rfgsm_*().
04/29/02     bk     Hard coded the tx power to power index 9
04/29/02     bk     Made changes rf_compute_rx_power based on i/f changes
04/24/02     ih     Removed CHANNEL_FILTER definitions. Moved to rfgsm.h.
04/16/02     bk     Added abort functionality, DA support and USF valid only in
                    4th metrics
03/29/02     dp     Merged Gabe's changes to rename RF_CTL to MDSP_RF_CTL
03/20/02     ih     Chaged rf_pa_precharge_stop_time to change with
                    rf_pa_start_time to allow PA start/stop calibration
02/21/02     bk     Fixed a bug relating to ordering of monitoring frequencies
02/09/02     bk     Added AGC related changes and changed RSSI to uint32
01/23/02     bk     Separated the tx burst into tx_bursts and tx_access_burst
                    and made some bug fixes
12/12/01     bk     Updated code to reflect change in rf interface code
             bk     Fixed the contruction of the msg header
             bk     Added CHANNEL_FILTER DEFINES
             bk     Fixed the search width and removed and extra tune in tx_bursts
             bk     Updated the file to conform to the latest mdsp_gprs.h
                    interfaces
             bk     Filled the timeslot field in the header
10/29/01     bk     Initial Revision

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* definitions */

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


#include "comdef.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_clk_ctl.h"
#include "gl1_hwi.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "rfm.h"
#include "geran_msgs.h"
#include "sys_type.h"
#include "stdio.h" /* for sprintf */
#include "gl1_hw_arbitration.h"
#include  "l1_log.h"
#include "l1_utils.h"
#include <stringl/stringl.h>
#include "gpl1_grm_intf.h"
#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#endif


#ifdef FEATURE_GSM_MDSP_EGPRS
const mdsp_puncturing_type mdsp_puncturing_type_lut[] =
{
   MDSP_PUNCTURING_1,  /* GL1_DEFS_PUNCTURING_1 */
   MDSP_PUNCTURING_2,
   MDSP_PUNCTURING_3
};
#endif


const gl1_defs_coding_type mdsp_coding_type_lut[] =
{
   GL1_DEFS_CS1_CODING,   /* MDSP_CODING_CS1 */
   GL1_DEFS_CS2_CODING,
   GL1_DEFS_CS3_CODING,
   GL1_DEFS_CS4_CODING,
   GL1_DEFS_MCS1_CODING,  /* MDSP_CODING_MCS1 */
   GL1_DEFS_MCS2_CODING,
   GL1_DEFS_MCS3_CODING,
   GL1_DEFS_MCS4_CODING,
   GL1_DEFS_MCS5_CODING,
   GL1_DEFS_MCS6_CODING,
   GL1_DEFS_MCS7_CODING,
   GL1_DEFS_MCS8_CODING,
   GL1_DEFS_MCS9_CODING
};

#define MAX_CS  (sizeof(mdsp_coding_type_lut)/sizeof(mdsp_coding_type_lut[0]))

const gl1_defs_aci_filter_type mdsp_aci_filter_lut[] =
{
   GL1_DEFS_ACI_FILTER_CENTER,  /* MDSP_ACI_FILTER_0HZ */
   GL1_DEFS_ACI_FILTER_HIGH,    /* MDSP_ACI_FILTER_P200KHZ */
   GL1_DEFS_ACI_FILTER_LOW,     /* MDSP_ACI_FILTER_N200KHZ */
   GL1_DEFS_ACI_FILTER_NARROW   /* MDSP_ACI_FILTER_NARROW */
};


#ifdef FEATURE_GSM_MDSP_EGPRS
static const gl1_defs_modulation_type mdsp_modulation_lut[] =
{
   GL1_DEFS_MODULATION_8PSK,    /* MDSP_MODULATION_8PSK */
   GL1_DEFS_MODULATION_GMSK     /* MDSP_MODULATION_GMSK */
};
#endif


/* lookup table for mapping msg length from coding scheme */
const uint8 cs_to_msg_length_lut[] =
{
   GL1_DEFS_CS1_BYTES,        /* GL1_DEFS_CS1_CODING */
   GL1_DEFS_CS2_BYTES,
   GL1_DEFS_CS3_BYTES,
   GL1_DEFS_CS4_BYTES,
   GL1_DEFS_ACCESS_BYTES,     /* GL1_DEFS_ACCESS_CODING */
   GL1_DEFS_EXT_ACCESS_BYTES, /* GL1_DEFS_EXT_ACCESS_CODING */
   GL1_DEFS_MCS1_BYTES,       /* GL1_DEFS_MCS1_CODING */
   GL1_DEFS_MCS2_BYTES,
   GL1_DEFS_MCS3_BYTES,
   GL1_DEFS_MCS4_BYTES,
   GL1_DEFS_MCS5_BYTES,
   GL1_DEFS_MCS6_BYTES,
   GL1_DEFS_MCS7_BYTES,
   GL1_DEFS_MCS8_BYTES,
   GL1_DEFS_MCS9_BYTES
};

/* Defs needed for Power Backoff to RF Driver. */
uint8 gl1_hw_num_assigned_tx_slots[NUM_GERAN_DATA_SPACES];
rfgsm_modulation_type rfgsm_modulation_lut[] =
{
   RF_MOD_GMSK,    /* GL1_DEFS_MODULATION_GMSK */
   RF_MOD_8PSK,    /* GL1_DEFS_MODULATION_8PSK */
   RF_MOD_UNKNOWN  /* GL1_DEFS_MODULATION_UNKNOWN */
};

extern gl1_hw_arbitration_t gl1_hw_arbitration[NUM_GERAN_DATA_SPACES];

/*=============================================================================
FUNCTION gl1_hw_set_num_assigned_tx_slots

DESCRIPTION
   Sets the number of assigned uplink slots.  This is used for determining
   the power backoff for multi-slot uplink.  Note that "assigned" is used
   meaning the number of slots in the PUA, whereas "allocated" refers to
   the number of USFs allocated in dynamic allocation.

============================================================================= */
void gl1_hw_set_num_assigned_tx_slots (uint8 num_assigned_tx_slots,gas_id_t gas_id)
{
   gl1_hw_num_assigned_tx_slots[gas_id] = num_assigned_tx_slots;
}

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
   gl1_defs_rx_pkt_metrics_type *metrics_info[GL1_DEFS_MAX_DL_MSGS],
   uint8                        num_msgs,
   boolean                      get_usf_info,
   boolean                      get_hard_decisions,
   gl1_hw_rx_tags_type          *tags,
#ifdef GERAN_L1_ENHANCED_RX
   gl1_msg_lif_chan_type        chan_type,
#endif
   gas_id_t                      gas_id
)
{
   GfwBurstMetricsStruct          metrics_data = {0};
   GfwBurstMetricsStruct          *m;  /* metrics shortcut */
   GfwSchedulerResultStruct       schedule_results = {0};
   GfwUsfBufStruct                usf_data  = {0};
   uint8                          i, tn;
   int32                          rssi;
   boolean                        valid_usf_info = FALSE, valid_metrics, valid_sched;
#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_rxd_control_type gl1_rxd_ctl_flag;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef GERAN_L1_HLLL_LNASTATE
   static gl1_defs_metrics_lnastate   gl1_metrics_lnastate[NUM_GERAN_DATA_SPACES];
#endif

  ARFCN_T arfcn;
#ifdef GERAN_L1_ENHANCED_RX 
  
  uint8 NoOfSlots = 0;
#endif /*GERAN_L1_ENHANCED_RX*/

   /*ACI 400 Switchpoint Detection*/
   boolean aci400_det_in_multislot = FALSE;

   /* Set the shortcut pointer */
   m = &metrics_data;

   /* Get the decoded USF information */
   if (get_usf_info)
   {
      valid_usf_info = mdsp_gprs_get_rx_usfs(&usf_data ,gas_id);
   }

   /* Get the burst schedule results */
   valid_sched = mdsp_gprs_get_rx_burst_schedule(&schedule_results ,gas_id);

   /* Catch overflow */
   if (num_msgs > GL1_DEFS_MAX_DL_MSGS)
   {
      MSG_GERAN_ERROR_2_G("Max burst metrics received per frame exceeded %d > %d",
                num_msgs, GL1_DEFS_MAX_DL_MSGS);
      num_msgs = GL1_DEFS_MAX_DL_MSGS;
   }

#ifdef GERAN_L1_HLLL_LNASTATE
   gl1_metrics_lnastate[gas_id].NoOfChannels = 0;
#endif

#ifdef GERAN_L1_ENHANCED_RX
   NoOfSlots = num_msgs;
#endif /*GERAN_L1_ENHANCED_RX*/

   /* Convert the packed structure format into an unpacked struct format */
   for (i = 0; i < num_msgs; i++)
   {
      /* Extract the expected TN from the given metrics info */
      tn = (uint8)metrics_info[i]->tn;

      arfcn = metrics_info[i]->arfcn;
      /* Burst schedule */
      if (valid_sched)
      {
         metrics_info[i]->schedule_status = schedule_results.reason[tn];
      }
      else
      {
         metrics_info[i]->schedule_status = 0;
      }

      /* usf_data will be filled if and only if valid_usf_info is true */
      if (valid_usf_info && (usf_data.usf[tn] & MDSP_USF_DEC_VALID_MASK)) /*lint !e644*/
      {
         metrics_info[i]->usf          =
           (gl1_defs_usf_type)(usf_data.usf[tn] & MDSP_USF_DEC_USF_VAL_MASK);
      }
      else
      {
         metrics_info[i]->usf          = GL1_DEFS_PDCH_UNALLOCATED;
      }


      /* Retrieve metrics if the burst was scheduled */
      if (metrics_info[i]->schedule_status & MDSP_GPRS_RX_BURST_SCHED_OK)
      {
         valid_metrics = mdsp_dtm_get_rx_burst_metrics
          (
            tags[i].metrics_tag,
            &metrics_data,
            get_hard_decisions
            ,gas_id
          );
      }
      else
      {
         valid_metrics = FALSE;
      }


      /* If the metrics data is not valid, we cannot fill in anything else */
      if (!valid_metrics)
      {
         metrics_info[i]->valid = FALSE;
         continue;
      }

      /* If the timeslot is not as expected, then mark as invalid as well */
      if (tn != m->pdch)
      {
         MSG_GERAN_ERROR_3_G("Rx metrics tn mismatch: got=%d, exp=%d, FN=%d",
                   m->pdch, tn, GSTMR_GET_FN_GERAN(gas_id));
         metrics_info[i]->valid = FALSE;
         continue;
      }

      /* The metrics are valid */
      metrics_info[i]->valid = TRUE;

      /* Calculate the rssi */
      rssi  = (m->rssiMsw << 16) + m->rssiLsw;

      /* Sanity check - RSSI should always be positive, but have seen dsp
         return negative values at times.  Set to 1 to avoid RF driver
         spitting error about RSSI being 0. */
      if (rssi < 0)
      {
         MSG_GERAN_ERROR_3_G("Negative RSSI (%d), tn=%d, FN=%d", rssi,tn,GSTMR_GET_FN_GERAN(gas_id));
         rssi = 1;
      }

      /* Compute the received power in dBm */
      metrics_info[i]->power_status =
      gl1_msg_compute_rx_power((uint32)rssi,
                               &metrics_info[i]->pwr_dBm_x16,
                               &metrics_info[i]->gain_range,
                               &tags->gain);

#ifdef FEATURE_GSM_RX_DIVERSITY
      metrics_info[i]->dc_offset_i_divrx	= (int16)m->rxdLogPacket.dcIDRx;
      metrics_info[i]->dc_offset_q_divrx	= (int16)m->rxdLogPacket.dcQDRx;   

      metrics_info[i]->rssi_divrx = m->rxdLogPacket.rssiDRx;
	  
      if(metrics_info[i]->rssi_divrx != 0)
        {
        metrics_info[i]->powerStatus_divrx = gl1_msg_compute_rx_power( metrics_info[i]->rssi_divrx,
                                                                       &metrics_info[i]->pwr_dBm_x16_divrx,
                                                                       &metrics_info[i]->rx_gain_range_divrx,
                                                                       &tags->gain_divrx);
        }
      else
        {
        metrics_info[i]->pwr_dBm_x16_divrx   = 0;
        metrics_info[i]->rx_gain_range_divrx = 0;
        metrics_info[i]->powerStatus_divrx   = RF_NOISY;	 
        }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

      metrics_info[i]->rssi          = (uint32)rssi;
      metrics_info[i]->dc_offset_i   = m->dcI;
      metrics_info[i]->dc_offset_q   = m->dcQ;
      metrics_info[i]->freq_offset   = m->freqOffset;
      metrics_info[i]->timing_offset = m->timingOffset;
      metrics_info[i]->snr           = m->snr;
      metrics_info[i]->modulation    = GL1_DEFS_MODULATION_GMSK;

      /* ACI power metrics */
      metrics_info[i]->aci_pwr_low     = m->aci_pwr_n200kHz;
      metrics_info[i]->aci_pwr_center  = m->aci_pwr_0Hz;
      metrics_info[i]->aci_pwr_high    = m->aci_pwr_p200kHz;
      metrics_info[i]->aci_filter_used = (gl1_defs_aci_filter_type)m->aciIndex;;
      metrics_info[i]->brstCancel = m->brstCancel;
      metrics_info[i]->reliabFactorQ16 = m->reliabFactorQ16;   

#ifdef GERAN_L1_HLLL_LNASTATE 
      metrics_info[i]->jdet_value       = m->jdetReading;
      metrics_info[i]->SawlessHLLLState = tags->sawless_lnastate;
#endif /*GERAN_L1_HLLL_LNASTATE*/

      /* Store power level of first slot for FTM */
      if (i == 0)
      {
#ifdef FEATURE_GSM_RX_DIVERSITY
         gl1_hw_ftm_store_rx_power(metrics_info[i]->rssi,metrics_info[i]->pwr_dBm_x16,
                                   metrics_info[i]->rssi_divrx,metrics_info[i]->pwr_dBm_x16_divrx,gas_id);
#else  /*FEATURE_GSM_RX_DIVERSITY*/
         gl1_hw_ftm_store_rx_power(metrics_info[i]->rssi,metrics_info[i]->pwr_dBm_x16,
                                   0,0,gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/
      }

#ifdef FEATURE_GSM_MDSP_EGPRS
      /* Modulation scheme */
      metrics_info[i]->modulation = mdsp_modulation_lut[m->modDet];
#endif /* FEATURE_GSM_MDSP_EGPRS */

#ifdef FEATURE_GSM_MDSP_ESAIC
      metrics_info[i]->pre_mimo_trained_complete_snr = m->pre_mimo_trained_complete_snr;
      metrics_info[i]->post_mimo_trained_complete_snr = m->post_mimo_trained_complete_snr;
      metrics_info[i]->misc_flags = m->misc_flags;
#endif

#ifdef GERAN_L1_HLLL_LNASTATE
      if(gl1_hw_get_sawless_HLLL_support(metrics_info[i]->arfcn.band,gas_id))
        {
        gl1_metrics_lnastate[gas_id].gsm_metrics[i].arfcn      = metrics_info[i]->arfcn;
        gl1_metrics_lnastate[gas_id].gsm_metrics[i].rx_lvl_dbm = metrics_info[i]->pwr_dBm_x16;
        gl1_metrics_lnastate[gas_id].gsm_metrics[i].snr        = metrics_info[i]->snr;
        gl1_metrics_lnastate[gas_id].gsm_metrics[i].jdet_value = metrics_info[i]->jdet_value;
        gl1_metrics_lnastate[gas_id].NoOfChannels++;
        }
#endif /*GERAN_L1_HLLL_LNASTATE*/

      
#ifdef GERAN_L1_ENHANCED_RX
      
      metrics_info[i]->aci400Detected = m->gfwLifBurstMetricsStruct.aci400Detected;
      metrics_info[i]->accISqr = m->gfwLifBurstMetricsStruct.rsbEstimation.accISqr;
      metrics_info[i]->accQSqr = m->gfwLifBurstMetricsStruct.rsbEstimation.accQSqr;;
      metrics_info[i]->accIQ = m->gfwLifBurstMetricsStruct.rsbEstimation.accIQ;;
      metrics_info[i]->wbdcEstI_PRx = m->gfwLifBurstMetricsStruct.wbeeWbdcEstPRx.wbdcEstI;
      metrics_info[i]->wbdcEstQ_PRx = m->gfwLifBurstMetricsStruct.wbeeWbdcEstPRx.wbdcEstQ;
      metrics_info[i]->wbdcEstI_DRx = m->gfwLifBurstMetricsStruct.wbeeWbdcEstDRx.wbdcEstI;
      metrics_info[i]->wbdcEstQ_DRx = m->gfwLifBurstMetricsStruct.wbeeWbdcEstDRx.wbdcEstQ;
      metrics_info[i]->rsbImgAciMetric = m->gfwLifBurstMetricsStruct.rsbImgAciMetric;
      metrics_info[i]->lif_offset_freq = m->gfwLifBurstMetricsStruct.nextLifFreqKhz;
      metrics_info[i]->curLifFreqKhz = m->gfwLifBurstMetricsStruct.curLifFreqKhz;
      metrics_info[i]->WBEE = (m->rxdLogPacket.rxdMisc.rxdSelStream == 2)?m->gfwLifBurstMetricsStruct.wbeeWbdcEstDRx.wbPwrEstRaw:m->gfwLifBurstMetricsStruct.wbeeWbdcEstPRx.wbPwrEstRaw;
      metrics_info[i]->WBEE_DRx = m->gfwLifBurstMetricsStruct.wbeeWbdcEstDRx.wbPwrEstRaw;
#endif /*GERAN_L1_ENHANCED_RX*/

#ifdef FEATURE_GSM_RX_DIVERSITY
     /*  Run a continuous average on stream selection to conclude later whether we can turn RXD off 
      *  only based on the RXQUAL level.*/
      if((m->rxdLogPacket.rxdMisc.rxdOn)&&(m->rxdLogPacket.rxdMisc.rxdSelStream == 1))
       {
       gl1_run_ps_divrx_stream_sel_avg(TRUE,gas_id);
       }
      else
       {
       gl1_run_ps_divrx_stream_sel_avg(FALSE,gas_id);
       }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef FEATURE_GSM_MDSP_EGPRS
      /* Copy over the hard decisions if requested */
      if (get_hard_decisions)
      {
         memscpy
          (
            metrics_info[i]->hard_decisions,
            sizeof(metrics_info[i]->hard_decisions),
            (uint16 *)m->hard_decisions,
            MDSP_MAX_HARD_DECISIONS * 2
          );
      }
#endif

  gl1_log_gsm_dft_spur_metric(metrics_data.gfwDftSpurAciMetrics,metrics_data.pdch,gas_id) ;

     /* Log AEQ metrics*/
     gl1_hw_log_aeq_metrics ((uint16) i, tags[i].metrics_tag,TRUE,gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
     /* Log Rx Diversity metrics only if Rx diversity NV is enabled*/
     gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);
     if ( ( gl1_rxd_ctl_flag.enableRxd )
#ifdef FEATURE_GERAN_LOG_OPT	 
	      && ( ( m->rxdLogPacket.rxdMisc.rxdOn != 0 ) || ( gl1_hw_disable_conditional_logging()!= FALSE ) ) 
#endif	 
        )
     {
        if( (gl1_rxd_ctl_flag.enableDrx) && !(gl1_rxd_ctl_flag.enablePrx) )
        {
           gl1_hw_log_rxd_metrics( 0,
                                metrics_info[i]->pwr_dBm_x16_divrx,								 
                                0,
                                metrics_info[i]->rx_gain_range_divrx,
                                metrics_info[i]->powerStatus_divrx,
                                &(m->rxdLogPacket),
                                LOG_PDTCH,
                                gas_id);
        }
        else
        {
          gl1_hw_log_rxd_metrics (metrics_info[i]->pwr_dBm_x16,
                                  metrics_info[i]->pwr_dBm_x16_divrx,								 
                                  metrics_info[i]->gain_range,
                                  metrics_info[i]->rx_gain_range_divrx,
                                  metrics_info[i]->powerStatus_divrx,
                                  &(m->rxdLogPacket),
                                  LOG_PDTCH,
                                  gas_id);
        }
     }
#endif

     /*Check if ACI 400 is detected or not,  We have to OR the result across multiple Slots 
       So, Set it to TRUE only when Detected else it defaults to FALSE*/
     if (metrics_data.gfwDftSpurAciMetrics.aciJammer)
     {
       aci400_det_in_multislot = TRUE;       
     }
   }//For Loop - NumSlots/NumMsgs

   /*Update Global Struct reg. ACI Status*/
   if (num_msgs > 0)
   {
     gl1_hw_set_aci_fr_arfcn(arfcn, aci400_det_in_multislot, gas_id); 
   }

#ifdef GERAN_L1_ENHANCED_RX
   if(gl1_hw_get_lif_feature_flag(gas_id) &&
      (gl1_msg_get_multi_sim_sys_mode( ) != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
#ifdef FEATURE_SGLTE
      && (gl1_hw_get_sglte_mode(gas_id) == FALSE)
#endif
      && (NoOfSlots)
      )
    {
     gl1_hw_update_lif_metrics(NULL, metrics_info,arfcn,chan_type,NoOfSlots,GSTMR_GET_FN_GERAN(gas_id),UPDATE_LIF_PS_METRICS,gas_id);
    }
#endif /*GERAN_L1_ENHANCED_RX*/


#ifdef GERAN_L1_HLLL_LNASTATE
  if(gl1_metrics_lnastate[gas_id].NoOfChannels)
   {
   if(gl1_is_current_state_dtm(gas_id) == TRUE)
     {
     gl1_hw_update_dtm_metrics(&gl1_metrics_lnastate[gas_id],GSTMR_GET_FN_GERAN(gas_id),UPDATE_PS_METRICS,gas_id);
     }
   else
     {
     gl1_hw_rf_burst_metrics(&gl1_metrics_lnastate[gas_id],gas_id);
     }
   }
#endif /*GERAN_L1_HLLL_LNASTATE*/


}



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
   boolean                       ptcch_type,
   gl1_defs_tn_type              tn,
   gl1_hw_rx_tags_type           *tag,
   boolean                       last_attempt,
   gl1_defs_rx_pkt_data_type     *msg_info,
   gas_id_t                      gas_id
)
{
   static GfwDecodedDataStruct hdr[NUM_GERAN_DATA_SPACES];

   gl1_defs_coding_type cs;
   boolean msg_available;
   uint16 *data[GL1_DEFS_MSG_PER_SLOT];
   uint8 hdr_ccs;

   /* Sanity check */
   if (msg_info == NULL || tag == NULL)
   {
      MSG_GERAN_ERROR_3_G("Bad params %d %d, FN=%d", msg_info,tag,GSTMR_GET_FN_GERAN(gas_id));
      return FALSE;
   }

   /* Prepare the data buffer pointers */
   data[0] = (uint16 *)msg_info->msg[0].data;
   data[1] = (uint16 *)msg_info->msg[1].data;

   /* Unaligned memory accesses are handled by the mDSP, but are slow */
   MDSP_PTR_CHECK(data[0]);
   MDSP_PTR_CHECK(data[1]);

   /* Query the DSP for the data */
   msg_available = mdsp_dtm_get_rx_data(tag->data_tag, &hdr[gas_id], data, last_attempt ,gas_id);

   /* Check if data was available from the firmware */
   if (!msg_available) return FALSE;

   /* Get the coding scheme */
   hdr_ccs = hdr[gas_id].gfwDecodedCtrlWord2.CCS;

   /* Check valid CS */
   if (hdr_ccs >= MAX_CS)
   {
       MSG_GERAN_ERROR_2_G("Bad hdr CCS %d, FN=%d", hdr_ccs, GSTMR_GET_FN_GERAN(gas_id));
       return FALSE;
   }
   cs = mdsp_coding_type_lut[hdr_ccs];

   /* Fill in the results */
   msg_info->tn = tn;
   msg_info->coding_scheme = cs;

   msg_info->msg[0].msg_len = cs_to_msg_length_lut[cs];

   msg_info->msg[0].crc_passed = hdr[gas_id].gfwDecodedCtrlWord2.crc1;
   msg_info->msg[0].rx_qual = hdr[gas_id].hardRxQual;
   msg_info->msg[0].firecode_used = hdr[gas_id].gfwDecodedCtrlWord2.Ir2WOrFireCode;

#ifdef FEATURE_GSM_MDSP_EGPRS
   /* Clear the second message to 0 length to be safe */
   msg_info->msg[1].msg_len = 0;
   msg_info->msg[1].crc_passed = FALSE;
   msg_info->msg[1].firecode_used = FALSE;
   msg_info->egprs.hdr_crc_passed = FALSE;

   if (IS_EGPRS_CODING(cs))
   {
      /* Fill in the EGPRS header info */
      msg_info->egprs.hdr_crc_passed = hdr[gas_id].gfwDecodedCtrlWord2.crcH;

      memscpy(msg_info->egprs.hdr, sizeof(msg_info->egprs.hdr), (void *)hdr[gas_id].egprs_hdr, 6);

      msg_info->egprs.enhEdgeLogPkt = hdr[gas_id].enhEdgeLogPkt;
      MSG_GERAN_DEBUG_OPT_3_G("gl1_hw_get_rx_data enhEdgeDecHdrLog 0x%x, mcs_0 = 0x%x, mcs_1 0x%x",
            hdr[gas_id].enhEdgeLogPkt.logPktHdr.enhEdgeDecHdrLog,
            hdr[gas_id].enhEdgeLogPkt.logPktMsg[0].mcs,
            hdr[gas_id].enhEdgeLogPkt.logPktMsg[1].mcs);

      /* No firecoding or rxqual, but maybe IR */
      msg_info->msg[0].rx_qual = 0;
      msg_info->msg[0].firecode_used = FALSE;
      msg_info->egprs.IR_used[0] = hdr[gas_id].gfwDecodedCtrlWord2.Ir1;
      msg_info->egprs.IR_written[0] = hdr[gas_id].gfwDecodedCtrlWord2.Ir1W;
      msg_info->egprs.IR_mem_full[0] = hdr[gas_id].gfwDecodedCtrlWord2.Ir1DatabaseFull;

      /* Grab the second message if it exists */
      if (cs >= GL1_DEFS_MCS7_CODING)
      {
         msg_info->msg[1].msg_len = cs_to_msg_length_lut[cs];
         msg_info->msg[1].crc_passed = hdr[gas_id].gfwDecodedCtrlWord2.crc2;
         msg_info->msg[1].rx_qual = 0;
         msg_info->egprs.IR_used[1] = hdr[gas_id].gfwDecodedCtrlWord2.Ir2;
         msg_info->egprs.IR_written[1] = hdr[gas_id].gfwDecodedCtrlWord2.Ir2WOrFireCode;
         msg_info->egprs.IR_mem_full[1] = hdr[gas_id].gfwDecodedCtrlWord2.Ir2DatabaseFull;
      }
      else
      {
         /* Clear out flags in second message to be safe */
         msg_info->egprs.IR_used[1] = FALSE;
         msg_info->egprs.IR_written[1] = FALSE;
         msg_info->egprs.IR_mem_full[1] = FALSE;
      }
   }
#endif

#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
   #error code not present
#endif

   /* Data was available */
   return TRUE;
}



/*=============================================================================
FUNCTION gl1_hw_abort_rx_decode

DESCRIPTION
   Aborts a receive in progress.  Only needs to be called if a receive with
   the decode flag was issued, otherwise it is not necessary to inform the
   frame layer of the abort.

============================================================================= */
void gl1_hw_abort_rx_decode (gl1_hw_rx_tags_type *tags,gas_id_t gas_id)
{
   mdsp_dtm_abort_rx(tags->data_tag, gas_id);
}


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
    uint8                           num_alloc_tn,
    gas_id_t                        gas_id
)
{
    uint8 tn, i;
    uint16 usf_list[ GL1_DEFS_SLOTS_IN_FRAME ];
    gl1_defs_usf_type temp_usf;

    /* First disable the USF list */
    for ( tn = 0; tn < GL1_DEFS_SLOTS_IN_FRAME; tn++ )
    {
       usf_list[ tn ] = MDSP_GPRS_UNALLOCATED_USF;
    }

    /* Based on the usf_info, fill up the USF list */
    for ( i = 0; i < num_alloc_tn; i++ )
    {
       temp_usf = usf_info[ i ].usf;
       /* Check if a usf value has been specified */
       if ( temp_usf <= GL1_DEFS_USF_FREE )
       {
          tn             = (uint8)usf_info[ i ].tn;
          usf_list[ tn ] = (int16)temp_usf;
       }
    }

    mdsp_gprs_set_usf_list( usf_list  ,gas_id);

}


/*=============================================================================
FUNCTION gl1_hw_abort_tx_bursts

DESCRIPTION
   This function may be invoked to clear the mDSP memory of the submitted
   payloads.

============================================================================= */
boolean gl1_hw_abort_tx_bursts
(
   gl1_hw_tx_tags_type *tags,
   gas_id_t            gas_id
)
{
   /* Pass down to the mdsp driver */
   return mdsp_dtm_abort_tx(tags->data_tag ,gas_id);
}

/*=============================================================================
FUNCTION  gl1_hw_write_tx_pkt_data

DESCRIPTION
   Writes a block/frame of data to the DSP memory. If the message buffer in the
   mDSP is full then one of the messages in the mDSP buffer will be discarded
   to make space for the submitted message (if the submitted message is a higher
   priority message).

DEPENDENCIES
   The frame layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
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
)
{

   mdsp_dtm_tx_msg_type msg;
   boolean auto_clear;
   boolean write_ok;

   /* Add one byte to get the len to an even num. and then convert to word */
   len = (len + 1) / 2;

   /* Fill in the mdsp message structure */
#ifdef FEATURE_GSM_MDSP_EGPRS
   if (chan_type >= GL1_HW_PDTCHU_MCS1 && chan_type <= GL1_HW_PDTCHU_MCS9)
   {
      msg.egprs.hdr = (uint16 *)msg_ptr->egprs.hdr;
      msg.egprs.len = len;
      msg.egprs.data[0] = (uint16 *)msg_ptr->egprs.data[0];
      msg.egprs.puncturing[0] = mdsp_puncturing_type_lut[msg_ptr->egprs.puncturing[0]];
      if (chan_type >= GL1_HW_PDTCHU_MCS7)
      {
         msg.egprs.data[1] = (uint16 *)msg_ptr->egprs.data[1];
         msg.egprs.puncturing[1] =
           mdsp_puncturing_type_lut[msg_ptr->egprs.puncturing[1]];
      }
      else
      {
         msg.egprs.data[1] = NULL;
         msg.egprs.puncturing[1] = MDSP_PUNCTURING_1;
      }

      /* Unaligned memory accesses are handled by the mDSP but are slow */
      MDSP_PTR_CHECK(msg.egprs.data[0]);
      MDSP_PTR_CHECK(msg.egprs.data[1]);
   }
   else
#endif

   /* PRACH is the same channel type as RACH in the mDSP so we need to
      treat it as a gsm type channel */
   if (chan_type == GL1_HW_PRACH_ACC)
   {
      msg.gsm.len[0]  = len;
      msg.gsm.data[0] = (uint16 *)msg_ptr->gprs.data;
      msg.gsm.len[1]  = 0;
      msg.gsm.data[1] = NULL;

      /* Unaligned memory accesses are handled by the mDSP but are slow */
      MDSP_PTR_CHECK(msg.gprs.data);
   }
   else

   {
      msg.gprs.data = (uint16 *)msg_ptr->gprs.data;
      msg.gprs.len = len;

      /* Unaligned memory accesses are handled by the mDSP but are slow */
      MDSP_PTR_CHECK(msg.gprs.data);
   }

   /*--------------------------------------------------------------------------
      Many of the params passed in this func have been typecasted to the mDSP
      type. The ASSUMPTION is that the gl1_defs values and the mDSP values
      match.
   --------------------------------------------------------------------------*/
   /* Let the driver auto clear schedule for loopback data */
   auto_clear = (msg_type == GL1_HW_LOOPBACK ? TRUE : FALSE);

   write_ok = mdsp_dtm_write_tx_data
   (
      mdsp_channel_type_lut[chan_type],
      &msg,
      (mdsp_specific_msg_type)msg_type,
      (mdsp_timeslot_type)tn,
      auto_clear,
      &tags->data_tag
      ,gas_id
   );


   return write_ok;
}


/*=============================================================================
FUNCTION  gl1_hw_get_tx_scheduled_result

DESCRIPTION
   Returns if the message associated with the given tag was transmitted.
   If discard_untxed is set then untransmitted messages are cleared.

============================================================================= */
boolean gl1_hw_get_tx_scheduled_result
(
   gl1_hw_tx_tags_type *tags,
   boolean discard_untxed,
   gas_id_t gas_id
)
{
   return mdsp_dtm_get_tx_schedule(tags->data_tag, discard_untxed ,gas_id);
}


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
)
{
   mdsp_gprs_convert_usf_to_pdch(tags->data_tag, (mdsp_timeslot_type)tn ,gas_id);
}



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
)
{
   uint32 i;
   mdsp_timeslot_type tn[GL1_DEFS_MAX_ASSIGNED_DL_TS];
   mdsp_gprs_monitor_coex_params coex_params[GL1_DEFS_MAX_ASSIGNED_DL_TS];
   ARFCN_T arfcns[GL1_DEFS_MAX_ASSIGNED_DL_TS];
   rfgsm_rx_chan_params_type rx_chan_params[ GL1_DEFS_MAX_ASSIGNED_DL_TS ];
   uint32 rf_seq_num[GL1_DEFS_MAX_ASSIGNED_DL_TS];


   rfgsm_rx_diversity_chan_params_type  div_rx_chan_params[GL1_DEFS_MAX_ASSIGNED_DL_TS];
   memset (rx_chan_params,0,( sizeof (rfgsm_rx_chan_params_type)*GL1_DEFS_MAX_ASSIGNED_DL_TS));
   /* Inform the scheduler about the bursts, abort if conflict detected */
   if (!gl1_hw_schedule_int_meas(signal_info, num_freq,gas_id)) return FALSE;

   if( num_freq > GL1_DEFS_MAX_ASSIGNED_DL_TS )
   {
     MSG_GERAN_ERROR_2_G("Invalid num_freq %d, set to max DL %d", num_freq,GL1_DEFS_MAX_ASSIGNED_DL_TS);
     num_freq = GL1_DEFS_MAX_ASSIGNED_DL_TS;
   }

   /* --------------------------------------------------------------------
   **          Establish Miscellaneous Control Settings
   ** --------------------------------------------------------------------
   */
   for (i = 0; i < num_freq; i++)
   {
      rx_chan_params[i].burst_type = RF_MON_BURST;
      rx_chan_params[i].band       = gl1_hw_arfcn_to_rfgsm_band(signal_info[i].arfcn);
      rx_chan_params[i].arfcn      = (uint16)signal_info[i].arfcn.num;
      rx_chan_params[i].rx_lvl_dbm = (dBx16_T)signal_info[i].expected_power;
      rx_chan_params[i].lna_range  = NULL;  // not applicable for interference level measurements
      /* gain will need to become a ptr to FW interface memory */
      rx_chan_params[i].gain_ptr   = &tags[i].gain;
      rx_chan_params[i].freq_err   = gl1_hw_rf_get_freq_error(gas_id);
#ifdef GERAN_L1_HLLL_LNASTATE
      rx_chan_params[i].high_lin_mode_active = &(tags[i].sawless_lnastate);
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GERAN_L1_ENHANCED_RX
      rx_chan_params[i].enh_rx_params.enh_rx_enable_flag = FALSE;
      rx_chan_params[i].enh_rx_params.enh_rx_offset_freq = 0;
#endif /*GERAN_L1_ENHANCED_RX*/

      div_rx_chan_params[i].rx_lvl_dbm = 0;
      div_rx_chan_params[i].lna_range = NULL;
      div_rx_chan_params[i].probe_burst = NULL;	
#ifdef FEATURE_GSM_RX_DIVERSITY	  
      div_rx_chan_params[i].gain_ptr = &tags[i].gain;
#else
      div_rx_chan_params[i].gain_ptr = NULL;	
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef FEATURE_GSM_COEX
     gl1_hw_arbitration[gas_id].monitors.monitor[i].arfcn = signal_info[i].arfcn;
#endif

      /* Save the timeslot to pass to the dsp */
      tn[i] = (mdsp_timeslot_type)signal_info[i].tn;
      arfcns[i] = signal_info[i].arfcn;

      rx_chan_params[i].rf_sequence_number = gl1_hw_get_next_rf_seq_num();
      rf_seq_num[i] = rx_chan_params[i].rf_sequence_number;

   }

#ifdef FEATURE_GSM_COEX
   gl1_hw_arbitration[gas_id].monitors.count = num_freq;
   /* Perform arbitration of activities prior to configuring RF and sending mDSP commands */
   gl1_hw_arbitration[gas_id].notify_event(ARBITRATE_MONITORS, gas_id );

   for (i = 0; i < num_freq; i++)
   {   
     coex_params[i].coex_desense_id = gl1_hw_arbitration[gas_id].monitors.monitor[i].desense_id;
     coex_params[i].coex_priority   = gl1_hw_arbitration[gas_id].monitors.monitor[i].priority;  
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     if (gl1_drdsds_enabled(gas_id))
     {
       coex_params[i].coex_skip = gl1_hw_arbitration[gas_id].monitors.monitor[i].skip;
     }
     else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
     {
       coex_params[i].coex_skip = FALSE;
     }
#ifdef FEATURE_GSM_LOG_COEX
     /* Log interference measurement in cxm_log_table */
     l1_log_cxm_new_entry(mdsp_get_current_seq_num(gas_id), arfcns[i].num, CXM_LOG_INTERFERENCE_MEAS,
                          CXM_LOG_UNKNOWN_VALUE, tn[i], 0, 0, 0, CXM_LOG_ACTIVITY_RX, gas_id);
#endif
   }
#endif
   if ( num_freq <= GL1_DEFS_MAX_ASSIGNED_DL_TS )
   {   
   gl1_hw_rf_rx_burst((uint16)num_freq,
                      rx_chan_params
                      ,div_rx_chan_params
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
   /* Issue the mdsp command */
   mdsp_gprs_interference_measure((uint16)num_freq, tn, coex_params, arfcns, rf_seq_num, gas_id);

   return TRUE;
}



/*=============================================================================
FUNCTION gl1_hw_set_medium_access_mode

DESCRIPTION
   Set the medium access mode to fixed, dynamic or extended dynamic.

============================================================================= */
void gl1_hw_set_medium_access_mode
(
   gl1_defs_access_mode_type mode,
   gas_id_t                  gas_id
)
{
    /* Tell the mDSP */
    mdsp_gprs_set_medium_access_mode( (mdsp_access_mode_type)mode ,gas_id);

    /* Tell the scheduler */
    gl1_hw_schedule_set_medium_access_mode(mode,gas_id);

}



/*=============================================================================
FUNCTION gl1_hw_disable_egprs_mode_handler

DESCRIPTION
   GSTMR handler that executes once to disabling EGPRS mode once the mDSP
   is ready.

============================================================================= */
void gl1_hw_disable_egprs_mode_handler (sys_modem_as_id_e_type as_id)
{
  /* We are no longer needed */
  gstmr_deregister_handler_geran(gl1_hw_disable_egprs_mode_handler,as_id);
}

/*=============================================================================
FUNCTION gl1_hw_set_egprs_mode

DESCRIPTION
   Enables or disables EGPRS mode.

============================================================================= */
void gl1_hw_set_egprs_mode
(
   boolean enable,
   boolean reg_clk,
   gas_id_t gas_id
)
{
#ifdef FEATURE_GSM_MDSP_EGPRS
   /* Set mode in the mDSP - will take effect at the next frame tick */
   mdsp_gprs_set_egprs_mode(enable ,gas_id);
#else
   if (enable)
   {
      MSG_GERAN_ERROR_0_G("MDSP does not support EGPRS");
   }
#endif

   /* Make sure EDGE clk regimes are enabled or disabled appropriately */
   if (enable)
   {
      /* Make sure disable handler is not installed */
      if (gstmr_is_handler_registered_geran(gl1_hw_disable_egprs_mode_handler,geran_map_gas_id_to_nas_id(gas_id)))
      {
         gstmr_deregister_handler_geran(gl1_hw_disable_egprs_mode_handler,geran_map_gas_id_to_nas_id(gas_id));
      }
   }
   else
   {
      /* Register a handler to disable EGPRS mode at the next frame tick
         once the mDSP has picked up the disable command */
      gstmr_register_handler_geran(gl1_hw_disable_egprs_mode_handler,geran_map_gas_id_to_nas_id(gas_id));
   }

}



/*=============================================================================
FUNCTION gl1_hw_gprs_reset

DESCRIPTION
   Resets the mdsp gprs data structures and fifos.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_hw_gprs_reset ( gas_id_t gas_id )
{
    mdsp_gprs_reset_rx_seq_nums(gas_id);
}

