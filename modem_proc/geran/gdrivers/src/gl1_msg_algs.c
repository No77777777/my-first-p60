/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         G L 1  M E S S A G E  L A Y E R -  S Y S T E M  A L G O R I T H M S

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

    gl1_msg_acq_afc_init

    gl1_msg_dch_afc_tt_init
    gl1_msg_dch_tt_update
    gl1_msg_dch_tt_adjust
    gl1_msg_dch_afc_update
    gl1_msg_dch_afc_adjust

    gl1_msg_cch_afc_tt_init
    gl1_msg_cch_tt_update
    gl1_msg_cch_tt_adjust
    gl1_msg_cch_afc_adjust

    gl1_msg_rxqual_init
    gl1_msg_rxqual_full_update
    gl1_msg_rxqual_sub_update
    gl1_msg_get_rxqual_full
    gl1_msg_get_rxqual_sub

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_algs.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
30/08/16   tsk      CR1026504: Update to 0x5096 RxD log packet with additional details.
17/04/17   nm       CR2032010 make rf power class variable dual data spaced
16/08/16   bg       CR1047602 Updating frequency offset value for invalid bursts from the valid bursts in PDTCH case
30/01/17   mk       CR1113152 Keep RxD always enabled in FTM mode based on RxDForcedON NV
16/02/17   cc       CR1010614 G2L idle time transfer and MEAS only support
03/01/16   mn       CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
17/02/17   mk       CR1110002 Change Rxqual_H threshold for TCH-EFR BFI computation as per new recommendation
08/06/16   hd       CR1023028 Use a new SNR to BEP table in TCH channel mode
18/5/16    sn       CR1007565 PACKET Transfer DTF Events/Handlers Phase1
10/28/15   tsk      CR929961 Update rpush_freq_err_in_ppm during BCCH to IDLE state transition.
28/09/15   pg       CR909305 Update dynamic RxD switching and remove pwr_meas suspension  
02/09/15   us       CR891688 Enable FEATURE_XO_RX_GAP_TEMP_COMP 
14/08/14    ws      CR889273 Fix Static declarations of variables which are not Dual data spaced
08/07/15   cja      CR868167 For dynamic EGPRS AEq reduce clock bump after block is complete
30/06/15   ws       CR852519 - Resolve GL1 enum conversion compile warnings
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
25/03/15   pg       CR810444 Adjust AEQ/RxD dynamic switching and add support for DRx IQ scaling  
19/03/15   jk       CR810476:VAMOS2 BFI gating algorithm should be applied only for TCH loop back A mode
05/02/15   pjr      CR791243 print message *** RSSI = 0 *** as medium priority
27/01/14   pjr      CR783314 Disable AMR-PD and DTX Detection in DTM
15/12/14   pg       CR630281 implementation of RxD support for data/MSRD 
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
08/12/14   fm       CR687284 update AFC algorithm to enable fast AFC in dedicated state
07/11/14   pjr      CR752564 Include l1_task.h in all build flavours
17/11/14   npt      CR642426 Added power reporting log packet
14/10/14   br       CR732289 Change in TCH/HS bfi algorithm to consider prev hard rx_qual
19/08/14   pjr      CR711506 Support for AMR Partial Decode and DTX detection FR19667
07/08/14   cja      CR643648 Desense for channel adjacent to spur
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
16/04/14   ws       CR 645067 - Increase Q6 clk speed when AEQ2A is enabled in EGPRS mode
16/04/14   jk       CR650105:Only if NV67228 is set to VAMOS2 mode use VAMOS2 BFI handling
10/04/14   am       CR643747 Consider all SACCH bursts for AFC update
08/04/14   jk       CR649527:VAMOS2 Fixes for Freqeucny hopping and DTX ON cases
02/01/14   jk       CR595130: GL1 changes to support VAMOS-II
18/07/13   pg       CR466815 For GPRS use non-VAMOS AFC threshold
15/01/14   nn       CR 591768 revised BEP tables to pass both Step2 and Step3 of GCF TC 21.9
05/12/13   am       CR581771 Ensure frequency correction to re-selection cell is always applied
28/11/13   cja      CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
11/10/13   jj       CR 557408 New BEP table
05/11/13   cja      CR571346 Use pre-mimo SNR for EDGE AFC loop
27/09/13   ab       CR550563 TR 1.1 - GBTA - Make Pseudo TRM_GRANT NULL after activity over.
16/09/13   cs       CR503042 Updated XO RGS handling for Triton
05/09/13   cja      CR538896 Correct step size in idle temperature assisted AFC
21/08/13   npt      CR531633 - Remove FEATURE_DIME_MODEM dependencies
20/08/13   cja      CR515629 Check schedule OK for handover TT
18/01/13   jj       CR423913 DUT download throughput  is slower than REF
12/07/13   cs       CR509747 Apply frequency drift to TRM_DENIED SUB(s)
19/06/13   cs       Major Triton TSTS Syncup
06/06/13   cs       More TSTS Updates
28/05/13   ap       CR461268: DSDS XO management + async time drift problem
21/05/13   cs       TSTS Updates
08/04/13   cja      CR471261 Reset idle_temp_comp when starting GSM
12/09/12   cs       CR399433 Always gate the idle SAIC disbale decision with the idle
                    mode status flag
10/10/12   cja      CR379483 Modify 8-PSK BEP table for GCF.
12/09/12   cja      CR398903 Enable FEATURE_XO_IDLE_MODE_TEMP_ASSISTANT as default
24/08/12   cja      CR379178 New TT algorithm for dedicated mode
22/06/12   pg       CR367679: Modify TCH/FS & TCH/HS BFI algs to use pre-mimo SNR
31/05/12   ab       CR 366880 RX and TX Time Power averaging Feature for DSDA as part of SAR Req.
17/05/12   cgc      CR332796  add support for Tx power pa info api
13/03/12   ws       CR 330862 - Change dedicated mode F3 level's and add sacch burst
                    metrics and compact message metrics log packets
25/05/12   pg       Add support for RF Task
25/08/11   ab       CR303214 Request for Configurable option for changing GSM Power Class value
13/01/12   ws       Added Vamos Feature
22/09/11   jj       CR308254 GL1 Changes for GSM DC spur removal
29/06/11   nt       DSDS CR289454: ASYNC should ignore timing adjustments for CS and PS connections
27/06/11   cs       Reduce unnecessary F3
18/05/11   cja      Fix compile error for non XO builds
04/05/11   cja      CR284460 Add temp compensation to freq drift in idle mode after TCH
22/03/11   ap       CR279430 DSDS Timing value wraps casusing frame misalignment
05/04/11   ws       Fixed compile error in CR24488
31/03/11   cs       CR278415 Use correct snr threshold for gating AFC in non-SAIC
10/11/10   ws       CR 254488 - Use higher timeslots for AFC in packet transfer mode
                    if lowest timesot has poor SNR
24/03/11   cja      CR280412 For H/O failure remove time tracking on failed cell
21/01/11   cs       CR246418 DSDS Allow disabling of SAIC F/W processing in idle
14/10/10   ws       FEATURE_GSM_MDSP_AEQ_8PSK - support AEQ Ph2
30/09/10   cs       CR Avoid saturation in the mDSP hztopdm conversion factors
22/09/10   cja      CR253370 Add frequency and time drift based on temperature during TCH Rx gap
14/09/10   og       CR255453 Setup the serving cell arfcn and band before WCDMA ID search.
21/07/10   cs       CR246418 Allow disabling of SAIC F/W processing in idle
17/05/10   ws       CR 238377 Update AEQ GCF table for 21.8 stage 3
11/05/10   cja      CR237386 For BEP use burst snr, not pre-mimo for high SNR.
27/04/10   cs       CR236460 Latest AFC\XO handling updates
06/04/10   cja      CR228952. New BEP table for AEq in GCF
17/03/10   og       Further x2g and g2x updates.
16/03/10   cja      CR229972. Always run afc_init when afc_data->cnt > SMALL_FREQ_ERR_AVG_LEN
10/02/10   ws       Merged changes from QDSP6 dev branch
11/02/10   cja      Fix LINT error and tidy-up afc_adjust_tcxo
08/02/10   cja      XO Update
01/02/10   cs       Fix previous checkin featurisation for MOB builds
25/01/10   cja      CR224507. For XO correct freq error setting for NCell.
20/01/10   cs       CR221032 21.3.1 failure with SAIC enabled as AFC/TT not updating
23/09/09   ps       Resolve Klocwork Buffer Overflow errors
26/05/09   cs       CR167848 fix for expanded afc_freq I/F to mDSP
29/05/09   ws       FEATURE_GSM_MDSP_AEQ phase 1 for all GMSK bursts
12/03/09   og       FEATURE_XO support.
26/02/09   cs/hy    Add in initial support for XO
19/01/09   ap       Previously there was no 8PSK LCU-specific table for GCF,
                    now there is one
08/12/08   cs       Add new split gl1_hw_clk_ctl.h from gl1_hw.h
03/12/08   ws       Added FEATURE_GSM_DYNAMIC_SAIC
07/11/08   kps      Remove dependency on tcxo_srm if we're using TCXOMGR30
21/10/08   ws       CR161163 modify BEP only for LCU target
                    for 21.8 test case
30/09/08   dv       CR158382:BFI alg. change for WB_AMR
16/06/08   nt       CR147984:- Changes for TCXO reset.
12/05/08   dv       Added FEATURE_GSM_AMR_WB - L1 component
20/05/08   cs       Fix an array indexing problem in AMR_HR threshold lookup
27/03/08   ap       CR 139993 RXQUAL_FULL/SUB calculation is now always based
                    on a full measurement report cycle (100 frames).Previously
                    if the cycle was shortened due to for e.g a HO, the
                    RXQUAL_FULL/SUB would still be reported but based on frames
                    received since the Hand Over.
15/01/08   ws       Added suport for ESIAC logging and SAIC AFC in PTM
08/01/08   og       YABT (completes the solution to CR127504)
05/11/07   og       Add NV item 1027 support to EGPRS BEP.
31/08/07   agv      CR127468 - Fix for SAIC TT handling in Dedicated.
31/08/07   agv      CR127469 - Temp fix for Timing Alignment for Handover.
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
04/05/07   cs       Updates for TCXO restriction support
12/02/07   agv      Added TCXO Mgr 3.0 Rotator Push for GPS.
28/11/06   ws       High Lint warning fixes
18/10/06   agv      Added TCXO MGR 3.0 for 7600.
11/07/06   cs       CR104631 - Disable CHIPXN_DIV2 clk regime in GSM TCH mode on 7200
21/07/06   agv      Cleaned up the SAIC snr threshold handling.
17/05/06   ws       Corrected FT_SNR_THRESH[] array
06/04/06   agv      Feature SAIC MIMO elements added to the burst metrics.
01/13/06   gfr      Fix AMR BFI thresholds
11/04/05   gfr      Added #include "clkregim.h", QLint cleanup
08/31/05   gfr      Replace gl1_msg_metrics_rpt with gl1_defs_metrics_rpt
08/21/05   gfr      Changed prototype for gl1_msg_set_band_group
08/19/05   gfr      Added gl1_msg_set_band_group
07/29/05   gfr      DTM support
07/27/05   gfr      Increase AFC medium term averaging length to allow for
                    higher 8PSK variability in frequency error
07/13/05   gfr      New rxqual thresholds and backed off previous BEP table
                    changes since we are going back to new channel filter
07/05/05   gfr      YABT
06/23/05   gfr      Check if TT would cause a conflict and delay if necessary
06/21/05   gfr      YABT (yet another BEP table)
05/10/05   gfr      New 8PSK BEP table (yes, another one).
05/05/05   gfr      New 8PSK BEP table.
05/05/05   gfr      New BEP table.
04/25/05   gfr      New BEP table for wider channel filter.
04/18/05   gfr      Fixed some values in the BEP conversion tables
04/07/05   gfr      Changed uint to uint32
04/03/05   gfr      Convert GMSK SNR to BEP correctly.
03/24/05   gfr      Rename EMR feature properly.
03/22/05   gfr      Lint cleanup
03/15/05   dp       Added support for enhanced measurement reporting
03/09/05   yh       Removed FEATURE_GSM_PLT for BEP calculations.
02/10/05   gfr      Optimized BEP processing routines.
01/19/05   gfr      Added GMSK and updated 8PSK SNR to BEP lookup tables
01/14/05   gfr      Added SNR to BEP conversion function (gl1_msg_snr_to_bep)
12/01/05   gfr      Added support for packet channel TT and AFC
01/10/05   gfr      Only show TT delay message when necessary
11/30/04   gfr      Support for BEP processing for enhanced measurement reporting
09/14/04   gfr      Support for suspending time tracking.
08/18/04   gsc      New thresholds for AMR BFI algorithm.
08/09/04   gfr      Mainlined FEATURE_GSM_GPRS_L1_SERV_CELL_MEAS
06/15/04   gfr      Moved local types from gl1_msgi.h to this file, moved txlev
                    functions here from gl1_msg_com.c
06/09/04   gfr      Moved gl1_msg_correct_timing into this file.
06/25/04   gsc      Added support for new BFI algorithm for AMR.
06/23/04   gfr      Lint cleanup.
06/22/04   gfr      Inform HW layer when the AFC loop is locked.
05/26/04   gfr      Pass dedicated mode flag to hw afc routines.
05/19/04   gfr      Support for informing the tcxo srm of afc changes
03/31/04   dp       Fix to factor hard RXQUAL into BFI computation
03/05/04   gfr      Lint cleanup.
02/17/04   gfr      GSM rotators support.
01/30/04   dp       Merged Fix for 16.1(leopard).  Adjusted TT slope asper
                    new algo from systems folks
01/28/04   gw       Added function to convert SNR into dB.
01/14/04   gw       Changed TT_SNR_THRESH and FT_SNR_THRESH from 1.5dB to 2dB.
11/05/03   gfr      Support for quad-band.
05/07/03   gsc      Added AMR support
05/01/03   mk       Cleaned up F3 messages.
03/21/03   gsc      Added BFI/UFI algorithm for Half Rate. Added num_bursts
                    parameter of type uint8 to gl1_msg_get_snr.
03/12/03   pjr      Added GPRS function gpl1_gprs_calc_rxqual_lev.
02/26/03   dp       Added BFI and SID algorithms for half rate
02/20/03   rrr/mk   Changed afc/tt snr thresholds from 0dB to 1.5dB.
01/16/03   mk       Increased snr threshold in the BFI algorithm by 0.5dB
                    (from 256 to 290).
01/13/03   mk       Starting with this release, RXQUAL_FULL and _SUB are now
                    based on MDSP hard rx qual (was MDSP soft rx qual).
                    Added ARM based (was in MDSP) BFI algorithm (using crc,
                    soft rx qual and snr).
12/06/02   drl      Undefine TT_DEBUG
11/22/02   mk       Added multiband AFC support.  Changed crystal constant
                    for Dinsdale (VCTCXO slope )
09/17/02   plm      Added call to MSG_ERROR() and set rxqual_lev to 7 when
                    block_cnt is zero in the function calc_rxqual_lev().
07/18/02   ATM      Added PDM adjust logging.
07/15/02   jc       Undefine TT_DEBUG.
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
05/14/02   mk       Changed functions for RXQUAL_FULL and _SUB averaging.
04/30/02   mk       Defined TT_DEBUG.
03/29/02   mk       Updated threshold values for RXQUAL calculation.
                    Added function to select dfc/ifc method.
03/15/02   mk       Added support for direct frequency correction for DCH
                    channels.
02/28/02   mk       Added initial support for RXQUAL_FULL and RXQUAL_SUB.
02/15/02   mk       Updated afc/tt.
01/16/02   mk       Initial version with afc/tt(CCH) and afc/tt(DCH)
                    algorithms.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif

#include "mcfg_nv_api.h"
#include "comdef.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hwi.h"
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "geran_eng_mode_read_api.h"
#include "msg.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "sys.h"
#include "sys_v.h"  /* for sys_band_class_e_type */


#include "tcxomgr.h"
#include "gl1_hw_tcxo_mgr3.h"

#include "l1_log.h"
#include "l1_task.h"
#include "gpl1_grm_intf.h"

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "l1i.h"
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
#include "gl1_sys_params.h"
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/* Scaling factor for SNR returned from mDSP.
 * This sets the 0dB point.
 */
#define SNR_SCALE_FACTOR_dBx16 385

/* AFC defines */



typedef enum
{
   IDLE_TEMP_COMP_OFF = 0,
   IDLE_TEMP_COMP_FIRST_SAMPLE,
   IDLE_TEMP_COMP_ACTIVE,
   IDLE_TEMP_COMP_FAST_AFC
} gl1_msg_afc_idle_temp_mode_types;

static gl1_msg_afc_idle_temp_mode_types  idle_temp_comp[NUM_GERAN_DATA_SPACES] =
                                           { INITIAL_VALUE( IDLE_TEMP_COMP_OFF ) };

#define FAST_AFC_MIN_FT_VALID_BURSTS 4

#define FAST_AFC_MED_FREQ_ERR_AVG_LEN       8
#define FAST_AFC_SMALL_FREQ_ERR_AVG_LEN     20

#define FAST_LARGE_FREQ_ERROR      100
#define FAST_AFC_SMALL_FREQ_ERROR  25

#define SHORT_TERM_FT_AVG_LEN_IDLE 4

#define MIN_FT_VALID_BURSTS        8

#define SMALL_FREQ_ERR_AVG_LEN     48
#define SHORT_TERM_FT_AVG_LEN      8

#define GL1_MAX_FREQ_CORR_HZ       100
/* for dedicated mode modified max correction */
#define GL1_MAX_DED_FREQ_CORR_HZ   150

#define LARGE_FREQ_ERROR           150
#define SMALL_FREQ_ERROR           40


#define MED_FREQ_ERR_AVG_LEN       20

#define SAIC_GMSK_ELEMENT          3

/* TT defines */
#define TT_MAX_TIMING_ERR 8     /* limiting of the timing error */
#ifdef GSM_HO_TIMING_ADJ
#define TT_MAX_HO_TIMING_ERR 8
#endif

#define TT_MIN_TIMING_ERR_TO_PRINT 4     /* set min timing error limit for generating F3 message */

#define TT_DED_MIN_SACCH_PERIODS    3
#define TT_DED_MIN_NUMBER_BURSTS    16
/* Rxqual define to indicate to RR that no rxqual meas. were taken */
#define INVALID_RXQUAL_MEAS        255

/* Time tracking SNR thresholds */
uint16 TT_SNR_THRESH[] =
{
   407,  /* GMSK = 2.0 dB */
   407   /* 8PSK = 2.0 dB */
};

/* Frequency tracking SNR thresholds */
/* x = 10^ (kdB/10) x 256; Q8 Format */
/* -0.5dB is: 10^(-0.05) * 256 = 228 */
/* NOTE:   Don't change the position */
/* of the GMSK SAIC 2dB"407" element */
/* without SAIC_GMSK_ELEMENT change. */
uint16 FT_SNR_THRESH[] =
{
   407,  /* GMSK = 2.0 dB */
   1615, /* 8PSK = 8.0 dB */
   0,    /*dummy - needed to collect the right index for SAIC threhold */
   407   /* GMSK SAIC = 2.0dB Warning Don't move. */
};


/* Structure to hold the frequency estimates for frequency estimation */
typedef struct
{
   int32   acc;
   uint16  cnt;
   int16   acc_lastN;
   int16   lastN_freq[SHORT_TERM_FT_AVG_LEN];
   int16   lastN_index;
   int32   avg_sum;
   int32   var_sum;
   int32   total_cnt;
   ARFCN_T arfcn;
} gl1_msgi_afc_struct;


/* Structure to hold the timing error */
typedef struct
{
   int16 acc; /* accumulation of the timing errors reported */
   int16 g2l_acc; /* For G2L timing transfer */
   int16 g2l_slot_change;
   int32 num_bursts;

   /* These are only for logging purposes */
   int16 min;
   int16 max;
   int32 avg_sum;
   int32 var_sum;
   int32 total_cnt;

   /* Flag if an adjustment occured while suspended */
   boolean adjust_pending;
   int16 num_sacch_periods;
} gl1_msgi_tt_struct;

#ifdef FEATURE_XO_RX_GAP_TEMP_COMP
typedef struct
{
  uint32  last_adjust_frame;
  uint32  last_frame_temp_xo;
  uint8 xo_drift_count;
  int32 xo_drift[2];
  int32 previous_temp_xo;
  int32 gap_freq_adjust;
  boolean gap_init_sample;
}gl1_msgi_xo_rx_gap_temp_comp_struct;
#endif
/*****************************************************

 LOCAL STORAGE

******************************************************/
typedef struct
{
 sys_band_T  afc_band;
 uint16      afc_arfcn;
}gl1_msg_algs_struct;

#define GL1_MSG_ALGS_INIT { SYS_BAND_EGSM_900, 20 }

static gl1_msg_algs_struct  gl1_msg_algs_data[NUM_GERAN_DATA_SPACES]  =
                                            {INITIAL_VALUE(GL1_MSG_ALGS_INIT) };

static gl1_msg_algs_struct  *gl1_msg_algs_data_ptr[NUM_GERAN_DATA_SPACES]=
                               { INITIAL_VALUE_INDEXED( &gl1_msg_algs_data ) };

static gl1_msgi_tt_struct   dch_tt_data[NUM_GERAN_DATA_SPACES] =
                              { INITIAL_VALUE_S( 0 ) };

static gl1_msgi_afc_struct  dch_afc_data[NUM_GERAN_DATA_SPACES] =
                              { INITIAL_VALUE_S( 0 ) };

static gl1_msgi_tt_struct   cch_tt_data[NUM_GERAN_DATA_SPACES] =
                              { INITIAL_VALUE_S( 0 ) };

static gl1_msgi_tt_struct  *cch_tt_data_ptr[NUM_GERAN_DATA_SPACES]=
                              { INITIAL_VALUE_INDEXED( &cch_tt_data ) };

static gl1_msgi_afc_struct  cch_afc_data[NUM_GERAN_DATA_SPACES] =
                              { INITIAL_VALUE_S( 0 ) };

static gl1_msgi_afc_struct *cch_afc_data_ptr[NUM_GERAN_DATA_SPACES]=
                              { INITIAL_VALUE_INDEXED( &cch_afc_data ) };

#ifdef FEATURE_XO_RX_GAP_TEMP_COMP
#define _GL1_XO_RX_GAP_TEMP_INIT  { 0, 0, 0, { 0, 0 }, 0, 0, TRUE }

static gl1_msgi_xo_rx_gap_temp_comp_struct  gl1_msgi_xo_rx_gap_temp_comp[NUM_GERAN_DATA_SPACES]  =
                                            { INITIAL_VALUE(_GL1_XO_RX_GAP_TEMP_INIT) };
#undef _GL1_XO_RX_GAP_TEMP_INIT

static gl1_msgi_xo_rx_gap_temp_comp_struct  *gl1_msgi_xo_rx_gap_temp_comp_ptr[NUM_GERAN_DATA_SPACES] =
         { INITIAL_VALUE_INDEXED( &gl1_msgi_xo_rx_gap_temp_comp ) };
#endif

#ifdef FEATURE_GSM_DTM
static gl1_msg_afc_tt_mode          afc_tt_mode[NUM_GERAN_DATA_SPACES] =
                                      { INITIAL_VALUE( AFC_TT_MODE_DCH ) };
#endif

static boolean afc_update ( gl1_msgi_afc_struct *afc_data,
                            int16 freq_err,
                            uint16 snr,
                            gl1_defs_modulation_type mod,
                            boolean saic_used,
                            boolean idle_mode,
                            gas_id_t gas_id);

static void afc_adjust_tcxo(gl1_msgi_afc_struct *afc_data,
                            boolean idle_mode,gas_id_t gas_id);

static void tt_update( gl1_msgi_tt_struct *tt_data,
                       int16 timing_offset,
                       uint16 snr,
                       gl1_defs_modulation_type mod,
                       boolean idle_mode );

static void afc_force_tcxo_update( gl1_msgi_afc_struct *afc_data, boolean idle_mode,gas_id_t gas_id );


typedef struct
{
  /* RXQUAL_FULL */
  uint32 full_qual_total;
  uint16 full_qual_blocks;
  uint32 mean_bep_total;
  uint16 mean_bep_blocks;
  uint32 cv_bep_total;
  uint16 cv_bep_blocks;

  /* RXQUAL_SUB */
  int32 sub_qual_total;
  int16 sub_qual_blocks;
 }rx_qual_params_t;

static rx_qual_params_t rx_qual_params[NUM_GERAN_DATA_SPACES] =
                          { INITIAL_VALUE_S( 0 ) };

static uint8 calc_rxqual_lev( uint32 rxqual_sum,
                              uint16 block_cnt,gas_id_t gas_id );

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
typedef struct
{
  boolean enabled;
  uint32 count;
} dtx_detection_t;

typedef struct
{
  boolean enabled;
  uint32 count;
  uint32 min_rxqual;
} amr_partial_decode_t;


#define _DTX_DETECTION_INIT {FALSE, 0}
#define _AMR_PD_INIT {FALSE, 0, L1_PERFECT_RXQUAL}
static dtx_detection_t  dtx_detection[NUM_GERAN_DATA_SPACES] = 
                          { INITIAL_VALUE( _DTX_DETECTION_INIT ) };

static amr_partial_decode_t amr_partial_decode[NUM_GERAN_DATA_SPACES] = 
                              { INITIAL_VALUE( _AMR_PD_INIT ) };
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */

dBx16_T gl1_msg_algs_linear_to_dBx16( uint32 x );

#define GL1_SIZEOF_MEAN_BEP_4_BLKS  4
#define GL1_SIZEOF_MEAN_BEP_48_BLKS 48/GL1_SIZEOF_MEAN_BEP_4_BLKS /* 6 */

typedef struct
{
  uint16  mean_bep_per_48_blks[GL1_SIZEOF_MEAN_BEP_48_BLKS];
  uint16  mean_bep_per_48_blks_index;
  uint16  mean_bep_per_4_blks[GL1_SIZEOF_MEAN_BEP_4_BLKS];
  uint16  mean_bep_per_4_blks_index;
#ifdef FEATURE_GSM_DYNAMIC_AEQ_8PSK
  uint16 bep_averaging_aeq_off_count;
#endif /* FEATURE_GSM_DYNAMIC_AEQ_8PSK */
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  uint16 bep_averaging_rxd_off_count;
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */
}mean_bep_params_type;

static mean_bep_params_type mean_bep_params[NUM_GERAN_DATA_SPACES];



/* Used to hold SAIC disable status locally */
static boolean gl1_msg_saic_dis_in_idle[NUM_GERAN_DATA_SPACES];

/* Used to store away the hard_rx_qual samples for averaging */
static int32 gl1_hard_rx_qual_sum[NUM_GERAN_DATA_SPACES]     =
               { INITIAL_VALUE( 0 ) };
static int32 gl1_hard_rx_qual_samples[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 0 ) };
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/* indicates if two GSM clients are currently active */
boolean gl1_msg_two_gsm_clients_active = FALSE;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

static int16 ho_tt_adjust[NUM_GERAN_DATA_SPACES];
static boolean gl1_msg_tt_failed_ho[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

/* RF power class per band */
extern byte   gl1_rf_power_class_per_band[NUM_GERAN_DATA_SPACES][5];
extern uint8  gl1_max_pcl_pwr_class[5][5][2];


/*---------------------------------------------------------------------------
  Rx RSSI -> dB translation tables. For each entry in rssi_range_tbl, there
  is a corresponding entry in ten_log_power_x16_tbl.
---------------------------------------------------------------------------*/
#define SIZEOF_GL1_MSG_RSSI_RANGE_TBL \
          ( sizeof(gl1_msg_rssi_range_tbl)/sizeof(gl1_msg_rssi_range_tbl[0]) )

static const uint32 gl1_msg_rssi_range_tbl[] =
{
        1,      /*  0 */
        2,      /*  1 */
        3,      /*  2 */
        4,      /*  3 */
        6,      /*  4 */
        9,      /*  5 */
       12,      /*  6 */
       18,      /*  7 */
       25,      /*  8 */
       37,      /*  9 */
       52,      /* 10 */
       75,      /* 11 */
      107,      /* 12 */
      154,      /* 13 */
      221,      /* 14 */
      316,      /* 15 */
      453,      /* 16 */
      649,      /* 17 */
      931,      /* 18 */
     1334,      /* 19 */
     1911,      /* 20 */
     2738,      /* 21 */
     3924,      /* 22 */
     5623,      /* 23 */
     8058,      /* 24 */
    11548,      /* 25 */
    16548,      /* 26 */
    23714,      /* 27 */
    33982,      /* 28 */
    48697,      /* 29 */
    69783,      /* 30 */
   100000,      /* 31 */
   143301,      /* 32 */
   205353,      /* 33 */
   294273,      /* 34 */
   421697,      /* 35 */
   604296,      /* 36 */
   865964,      /* 37 */
  1240938,      /* 38 */
  1778279,      /* 39 */
  2548297,      /* 40 */
  3651741,      /* 41 */
  5232991,      /* 42 */
  7498942,      /* 43 */
 10746078,      /* 44 */
 15399265,      /* 45 */
 22067341,      /* 46 */
 31622777,      /* 47 */
 45315836,      /* 48 */
 64938163,      /* 49 */
 93057204,      /* 50 */
133352143,      /* 51 */
191095297,      /* 52 */
273841963,      /* 53 */
392418976,      /* 54 */
562341325,      /* 55 */
805842188,      /* 56 */
1154781985,     /* 57 */
1654817100,     /* 58 */
2371374000LL,   /* 59 */
3398208000LL,   /* 60 */
4294967295LL,   /* 61 */
};
/* dB16 of raw rssi: rssi_db16=10*log10(rssi)*16 */
static const uint16 gl1_msg_ten_log_power_x16_tbl[] =
{
   1,   /*  0 */
  49,   /*  1 */
  75,   /*  2 */
 100,   /*  3 */
 125,   /*  4 */
 150,   /*  5 */
 175,   /*  6 */
 200,   /*  7 */
 225,   /*  8 */
 250,   /*  9 */
 275,   /* 10 */
 300,   /* 11 */
 325,   /* 12 */
 350,   /* 13 */
 375,   /* 14 */
 400,   /* 15 */
 425,   /* 16 */
 450,   /* 17 */
 475,   /* 18 */
 500,   /* 19 */
 525,   /* 20 */
 550,   /* 21 */
 575,   /* 22 */
 600,   /* 23 */
 625,   /* 24 */
 650,   /* 25 */
 675,   /* 26 */
 700,   /* 27 */
 725,   /* 28 */
 750,   /* 29 */
 775,   /* 30 */
 800,   /* 31 */
 825,   /* 32 */
 850,   /* 33 */
 875,   /* 34 */
 900,   /* 35 */
 925,   /* 36 */
 950,   /* 37 */
 975,   /* 38 */
1000,   /* 39 */
1025,   /* 40 */
1050,   /* 41 */
1075,   /* 42 */
1100,   /* 43 */
1125,   /* 44 */
1150,   /* 45 */
1175,   /* 46 */
1200,   /* 47 */
1225,   /* 48 */
1250,   /* 49 */
1275,   /* 50 */
1300,   /* 51 */
1325,   /* 52 */
1350,   /* 53 */
1375,   /* 54 */
1400,   /* 55 */
1425,   /* 56 */
1450,   /* 57 */
1475,   /* 58 */
1500,   /* 59 */
1525,   /* 60 */
1541,   /* 61 */
};

/* =======================================================================
**                   FUNCTION PROTOTYPES
** ======================================================================= */

/*===========================================================================

FUNCTION gl1_msg_update_xo_rgs

DESCRIPTION
  This is used to set the XO RGS correctly for the chosen SUB.
  Typically used only during select specific to ensure the XO
  manager has the most recent value for this SUB so when lock
  is released and regained the appropriate latest G SUB value is
  available.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_update_xo_rgs( boolean is_idle, boolean state_info, gas_id_t gas_id );

/*===========================================================================

FUNCTION gl1_msg_acq_tcxo_and_rotator_init

DESCRIPTION This is the setup for the PDM, and Rotator so Ageing is used.


DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_acq_tcxo_and_rotator_init( gas_id_t gas_id )
{
  gl1_hw_acq_tcxo_and_rotator_init(gas_id);
}

/*===========================================================================

FUNCTION gl1_msg_snr_threshold

DESCRIPTION This returns GMSK only until Packet SAIC is introduced.


DEPENDENCIES
  None

RETURN VALUE
  gl1_defs_modulation_type   snr threshold

SIDE EFFECTS
  None
===========================================================================*/
gl1_defs_modulation_type gl1_msg_snr_threshold(boolean saic_used)
{
   return(GL1_DEFS_MODULATION_GMSK);
}

/*===========================================================================

FUNCTION  gl1_msg_set_band_group

DESCRIPTION
  Informs the NPL1 of which band group we will be operating in.
  Currently only used to inform clock regime which high clock speed to
  use since certain speeds cause RF interference in certain bands.

===========================================================================*/
void gl1_msg_set_band_group (sys_band_mask_type band_mask)
{
/*lint -esym(715,band_mask)*/
}



/*===========================================================================

FUNCTION  set_afc_band

DESCRIPTION
  Sets the band for AFC, and informs the HW layer of the new center
  frequency.

===========================================================================*/
void set_afc_band (ARFCN_T arfcn, boolean dedicated_mode,gas_id_t gas_id)
{
  gl1_msg_algs_data_ptr[gas_id]->afc_band = arfcn.band;

  gl1_msg_algs_data_ptr[gas_id]->afc_arfcn = arfcn.num;

  gl1_hw_set_afc_band(gl1_msg_algs_data_ptr[gas_id]->afc_band, dedicated_mode,gas_id);
}

/*===========================================================================

FUNCTION  gl1_msg_is_arfcn_xo_desense

DESCRIPTION
  Informs the lower layers if the current ARFCN will cause desense issue
  for XO enabled builds on LCU.

===========================================================================*/
mdsp_rf_desense_chan_type gl1_msg_is_arfcn_xo_desense( ARFCN_T arfcn , gas_id_t gas_id)
{
  mdsp_rf_desense_chan_type xo_desense_arfcn;

  xo_desense_arfcn =mdsp_is_desense_arfcn( arfcn.num, arfcn.band  ,gas_id);

  MSG_GERAN_LOW_3_G( "Is XO arfcn:%d band:%d desense:%d",
           arfcn.num, arfcn.band, xo_desense_arfcn );

  return ( xo_desense_arfcn );
}

/*===========================================================================

FUNCTION tt_update

DESCRIPTION This function is called every time a burst returns a timing offset


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tt_update
(
   gl1_msgi_tt_struct *tt_data,
   int16 timing_offset,
   uint16 snr,
   gl1_defs_modulation_type mod,
   boolean idle_mode
)
{
  if ( snr >= TT_SNR_THRESH[mod] )
  {
    if ( !idle_mode )
    {
      /* in dedicated mode limit the timing error */
      if ( timing_offset > TT_MAX_TIMING_ERR )
        timing_offset = TT_MAX_TIMING_ERR;
      else if ( timing_offset < -TT_MAX_TIMING_ERR )
        timing_offset = -TT_MAX_TIMING_ERR;
    }

    tt_data->acc += timing_offset;
    tt_data->num_bursts++;
    if ((timing_offset > TT_MIN_TIMING_ERR_TO_PRINT) || (timing_offset < -TT_MIN_TIMING_ERR_TO_PRINT))
    {
      /* Debug for TT. */
      MSG_GERAN_HIGH_3("TT Update:Acc=%d,Bsts=%d,To=%d",
                tt_data->acc,
                tt_data->num_bursts,
                timing_offset);
    }
  }
}


/*===========================================================================

FUNCTION afc_update

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  Boolean TRUE/FALSE if afc loop has been updated as SNR threshold has been met

SIDE EFFECTS
  None
===========================================================================*/
static boolean afc_update
(
  gl1_msgi_afc_struct *afc_data,
  int16 freq_err,
  uint16 snr,
  gl1_defs_modulation_type mod,
  boolean saic_used,
  boolean idle_mode,
  gas_id_t gas_id
)
{
  int16 i;
  uint16 snr_threshold;
  boolean afc_updated = FALSE;

  /* Use the appropriate element. */
  if (saic_used == TRUE)
  {
     /* This is for SAIC and if the position in */
     /* FT_SNR_THRESH  is moved this must also. */
     snr_threshold = FT_SNR_THRESH[SAIC_GMSK_ELEMENT];
  }
  else
  {
     snr_threshold = FT_SNR_THRESH[mod];
  }

  if ( snr >= snr_threshold )
  {
    afc_updated = TRUE;
    afc_data->acc += freq_err;
    afc_data->cnt++;
    afc_data->lastN_freq[afc_data->lastN_index] = freq_err;
    afc_data->acc_lastN = 0;
	
    for (i = 0; i < SHORT_TERM_FT_AVG_LEN_IDLE; i++)
    {
        afc_data->acc_lastN += afc_data->lastN_freq[i];
    }
    afc_data->lastN_index = (afc_data->lastN_index + 1 )%SHORT_TERM_FT_AVG_LEN_IDLE;


    gl1_hw_update_raw_fe_tcxo_mgr( freq_err,gas_id);
  }

   return (afc_updated);
}


/*===========================================================================

FUNCTION gl1_msg_acq_afc_init

DESCRIPTION
   Initializes acquisition AFC

===========================================================================*/
#define ACQ_AFC_SMALL_FREQ_LOOP_NUMERATOR   1
#define ACQ_AFC_SMALL_FREQ_LOOP_DENOMINATOR 2

#define ACQ_AFC_LARGE_SMALL_INIT_VALUE      ( 0x7FFF )
#define ACQ_AFC_LARGE_SMALL_SHIFT_VALUE     1

void gl1_msg_acq_afc_init( ARFCN_T arfcn,gas_id_t gas_id )
{

  set_afc_band( arfcn, FALSE,gas_id );

/* Q6 GFW does not use the PDM large/small */
/* FErr should not be written to the mDSP here. */
}

/*===========================================================================

FUNCTION afc_init

DESCRIPTION


===========================================================================*/
void afc_init (gl1_msgi_afc_struct *afc_data)
{
  int16 i;

  afc_data->acc         = 0;
  afc_data->cnt         = 0;
  afc_data->acc_lastN   = 0;
  afc_data->lastN_index = 0;

  for (i = 0; i < SHORT_TERM_FT_AVG_LEN; i++)
  {
    afc_data->lastN_freq[i] = 0;
  }

  afc_data->avg_sum   = 0;
  afc_data->var_sum   = 0;
  afc_data->total_cnt = 0;
}

/*===========================================================================

FUNCTION tt_init

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tt_init( gl1_msgi_tt_struct *tt_data )
{
  tt_data->acc            = 0;
  tt_data->num_bursts     = 0;
  tt_data->avg_sum        = 0;
  tt_data->var_sum        = 0;
  tt_data->total_cnt      = 0;
  tt_data->max            = 100;
  tt_data->min            = -100;
  tt_data->adjust_pending = FALSE;
  tt_data->num_sacch_periods = 0;
}


/*===========================================================================

FUNCTION gl1_msg_cch_afc_tt_init

DESCRIPTION Initializes the time tracking/Frequency tracking for idle mode


===========================================================================*/
void gl1_msg_cch_afc_tt_init( ARFCN_T arfcn,gas_id_t gas_id )
{
  /* Slam residual dch adjust */
  afc_force_tcxo_update( &dch_afc_data[gas_id], FALSE,gas_id );

  set_afc_band(arfcn, FALSE,gas_id);
  tt_init(cch_tt_data_ptr[gas_id]);
  afc_init(cch_afc_data_ptr[gas_id]);

  cch_afc_data_ptr[gas_id]->arfcn = arfcn;

  MSG_GERAN_LOW_3_G( "CCH AFC INIT FN:%d arfcn:%d band:%d",
           GSTMR_GET_FN_GERAN(gas_id), arfcn.num, arfcn.band );
}

/*===========================================================================
FUNCTION gl1_msg_bcch_to_idle_init

DESCRIPTION Initializes the dch_data for bcch to idle transitions.
            Force update TCXO values for BCCH to IDLE transitions.

===========================================================================*/
void gl1_msg_bcch_to_idle_init(gas_id_t gas_id)
{
  gl1_hw_update_raw_fe_tcxo_mgr(0, gas_id);
}

/*===========================================================================

FUNCTION gl1_msg_dch_afc_tt_init

DESCRIPTION Initializes the time tracking/Frequency tracking for ded mode


===========================================================================*/
void gl1_msg_dch_afc_tt_init( ARFCN_T arfcn,gas_id_t gas_id )
{
#ifdef FEATURE_GSM_DTM
   if (afc_tt_mode[gas_id] == AFC_TT_MODE_DCH)
#endif
   {
      /* Slam residual cch adjust */
      afc_force_tcxo_update( cch_afc_data_ptr[gas_id], TRUE ,gas_id);

      set_afc_band(arfcn, TRUE,gas_id);
      tt_init( &dch_tt_data[gas_id] );
      afc_init( &dch_afc_data[gas_id] );

      dch_afc_data[gas_id].arfcn = arfcn;

      MSG_GERAN_LOW_3_G( "DCH AFC INIT FN:%d arfcn:%d band:%d",
               GSTMR_GET_FN_GERAN(gas_id), arfcn.num, arfcn.band );
   }
}


/*===========================================================================

FUNCTION gl1_msg_pdch_afc_tt_init

DESCRIPTION
   Initializes the time tracking/Frequency tracking for packet transfer mode


===========================================================================*/
void gl1_msg_pdch_afc_tt_init( ARFCN_T arfcn ,gas_id_t gas_id)
{
#ifdef FEATURE_GSM_DTM
   if (afc_tt_mode[gas_id] == AFC_TT_MODE_PDCH)
#endif
   {
      /* Slam residual cch adjust */
      afc_force_tcxo_update( cch_afc_data_ptr[gas_id], TRUE ,gas_id);

      set_afc_band(arfcn, TRUE,gas_id);
      tt_init( &dch_tt_data[gas_id] );
      afc_init( &dch_afc_data[gas_id] );

      dch_afc_data[gas_id].arfcn = arfcn;

      MSG_GERAN_LOW_3_G( "PDCH AFC INIT FN:%d arfcn:%d band:%d",
               GSTMR_GET_FN_GERAN(gas_id), arfcn.num, arfcn.band );
   }
}



#ifdef FEATURE_GSM_DTM
/*===========================================================================

FUNCTION gl1_msg_set_afc_tt_mode

DESCRIPTION
   Sets which mode AFC/TT will operate in.  AFC/TT calls from the other
   mode will be ignored.

===========================================================================*/
void gl1_msg_set_afc_tt_mode (gl1_msg_afc_tt_mode mode,gas_id_t gas_id)
{
   if (afc_tt_mode[gas_id] != mode)
   {
      MSG_GERAN_HIGH_2_G("AFC/TT mode=%d (0=DCH, 1=PDCH), FN=%d", mode, GSTMR_GET_FN_GERAN(gas_id));
      afc_tt_mode[gas_id] = mode;
   }
}
#endif

/*===========================================================================

FUNCTION gl1_msg_tt_do_pending_adjust

DESCRIPTION
  Attempts to perform any pending time tracking adjustments.

===========================================================================*/
void gl1_msg_tt_do_pending_adjust (gas_id_t gas_id)
{
   /* Check idle mode for update */
   if (cch_tt_data_ptr[gas_id]->adjust_pending)
   {
      gl1_msg_cch_tt_adjust(gas_id);
   }

   /* Check dedicated mode for update */
   if (dch_tt_data[gas_id].adjust_pending || (gl1_msg_tt_failed_ho[gas_id] == TRUE))
   {
      MSG_GERAN_MED_0_G("TT do pending adjustment");
      gl1_msg_dch_tt_adjust(gas_id);

#ifdef FEATURE_GSM_DTM
      /* In DTM mode both of these need to be called since only one will
         actually perform the adjustment based on afc_tt_mode[gas_id] */
      gl1_msg_pdch_tt_adjust(gas_id);
#endif
   }
}

/*===========================================================================

FUNCTION gl1_msg_set_tt_failed_ho

DESCRIPTION Force removal of any TT of failed cell after failed handover.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_set_tt_failed_ho( gas_id_t gas_id )
{
   gl1_msg_tt_failed_ho[gas_id] = TRUE;
   MSG_GERAN_LOW_1_G("TT during HO = %d", ho_tt_adjust[gas_id]);
}

/*===========================================================================

FUNCTION gl1_msg_reset_ho_tt_adjust

DESCRIPTION Reset ho_tt_adjust[gas_id] so can track any adjusts on new cell in case of handover failure

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_reset_ho_tt_adjust( gas_id_t gas_id)
{
   ho_tt_adjust[gas_id] = 0;
}

#ifdef GSM_HO_TIMING_ADJ
/*===========================================================================

FUNCTION gl1_msg_tt_ho_bursts

DESCRIPTION Temp workaround for timing error at Handover.


DEPENDENCIES
  None

RETURN VALUE
  Number of bursts.

SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_msg_tt_ho_bursts( gas_id_t gas_id )
{
  return (dch_tt_data[gas_id].num_bursts);
}

/*===========================================================================

FUNCTION gl1_msg_tt_ho_adjust

DESCRIPTION Temp workaround for timing error at Handover.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_tt_ho_adjust( gas_id_t gas_id )
{
  int16 timing_corr;

  if (dch_tt_data[gas_id].num_bursts > 0)
  {
    /* Calculate average timing error */
    timing_corr = dch_tt_data[gas_id].acc / dch_tt_data[gas_id].num_bursts;

    /* In dedicated mode limit the timing error; currently 8QS */
    if ( timing_corr > TT_MAX_HO_TIMING_ERR )
    {
      timing_corr = TT_MAX_HO_TIMING_ERR;
    }
    else if ( timing_corr < -TT_MAX_HO_TIMING_ERR )
    {
      timing_corr = -TT_MAX_HO_TIMING_ERR;
    }

    if (timing_corr != 0)
    {
      if (!gl1_hw_schedule_tt_ok(-timing_corr,gas_id))
      {
         MSG_GERAN_HIGH_2_G("TT HO: adjust of %d not applied due to schedule clash, FN=%d", timing_corr, GSTMR_GET_FN_GERAN(gas_id));
         return;
      }
      gl1_msg_correct_timing((int32)-timing_corr,gas_id);
      MSG_GERAN_HIGH_2_G("TT HO: GSTMR timing adjust = %d, FN=%d",
               timing_corr, GSTMR_GET_FN_GERAN(gas_id));
      ho_tt_adjust[gas_id] += timing_corr;
    }
    /* Reset */
    dch_tt_data[gas_id].acc            = 0;
    dch_tt_data[gas_id].num_bursts     = 0;
    dch_tt_data[gas_id].adjust_pending = FALSE;
  }
}
#endif

/*===========================================================================

FUNCTION gl1_msg_tt_adjust_gstmr

DESCRIPTION Initializes the time tracking/Frequency tracking for idle mode


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_tt_adjust_gstmr
(
  gl1_msgi_tt_struct *tt_data,
  boolean idle_mode,
  gas_id_t gas_id
)
{
   int16 timing_corr;

   /* For failed HO force adjust to remove any TT from failed cell */
   if ((gl1_msg_tt_failed_ho[gas_id] == TRUE) && gl1_hw_schedule_tt_ok(ho_tt_adjust[gas_id],gas_id))
   {
      gl1_msg_tt_failed_ho[gas_id] = FALSE;
      if (ho_tt_adjust[gas_id] != 0)
      {
         gl1_msg_correct_timing(ho_tt_adjust[gas_id],gas_id);
            MSG_GERAN_HIGH_2_G("Failed HO TT: GSTMR timing adjust = %d, FN=%d",
                    ho_tt_adjust[gas_id], GSTMR_GET_FN_GERAN(gas_id));
         ho_tt_adjust[gas_id] = 0;
      }
   }
   /* Only works if we have bursts to process */
   if (tt_data->num_bursts > 0)
   {
      /* Calculate average timing error */
      timing_corr = tt_data->acc / tt_data->num_bursts;

      /* apply static timing error to gstmr */
      if (!idle_mode)
      {
         /* We apply the correction after accumulating over 3 SACCH
            periods. i.e once every 1.5 secs (Fix for 16.1) provided we have at least 16 good bursts */
         tt_data->num_sacch_periods++;

         MSG_GERAN_HIGH_3_G("TT: num brst %d acc = %d sacch_periods %d",tt_data->num_bursts,tt_data->acc,tt_data->num_sacch_periods);

         if ((tt_data->num_sacch_periods >= TT_DED_MIN_SACCH_PERIODS) && (tt_data->num_bursts >= TT_DED_MIN_NUMBER_BURSTS))
         {
            if(timing_corr >= 1)
            {
              timing_corr = 1;
            }
            else if (timing_corr <= -1)
            {
              timing_corr = -1;
            }
         }
         else
         {
            timing_corr = 0;
         }
      }

      if (timing_corr != 0)
      {
         if (!gl1_hw_schedule_tt_ok(-timing_corr,gas_id))
         {
            MSG_GERAN_HIGH_2_G("Delaying TT adjust of %d, FN=%d", timing_corr, GSTMR_GET_FN_GERAN(gas_id));
            tt_data->adjust_pending = TRUE;
            return;
         }
         else
         {
            gl1_msg_correct_timing((int32)-timing_corr,gas_id);
            MSG_GERAN_HIGH_2_G("TT: GSTMR timing adjust = %d, FN=%d",
                    timing_corr, GSTMR_GET_FN_GERAN(gas_id));
            ho_tt_adjust[gas_id] += timing_corr;
            tt_data->adjust_pending = FALSE;
         }
      }
   }

   if (idle_mode || ((tt_data->num_sacch_periods >= TT_DED_MIN_SACCH_PERIODS) && (tt_data->num_bursts >= TT_DED_MIN_NUMBER_BURSTS)))
   {
      /* Reset */
      tt_data->acc            = 0;
      tt_data->num_bursts     = 0;
      tt_data->num_sacch_periods = 0;
      tt_data->adjust_pending = FALSE;
   }
}


/*===========================================================================

FUNCTION gl1_msg_dch_tt_adjust

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_dch_tt_adjust (gas_id_t gas_id)
{
#ifdef FEATURE_GSM_DTM
   if (afc_tt_mode[gas_id] == AFC_TT_MODE_DCH)
#endif
   {
      gl1_msg_tt_adjust_gstmr(&dch_tt_data[gas_id], FALSE,gas_id);
   }
}

/*===========================================================================

FUNCTION gl1_msg_pdch_tt_adjust

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_pdch_tt_adjust (gas_id_t gas_id)
{
#ifdef FEATURE_GSM_DTM
   if (afc_tt_mode[gas_id] == AFC_TT_MODE_PDCH)
#endif
   {
      gl1_msg_tt_adjust_gstmr(&dch_tt_data[gas_id], FALSE,gas_id);
   }
}

/*===========================================================================

FUNCTION gl1_msg_cch_tt_adjust

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_cch_tt_adjust (gas_id_t gas_id)
{
  gl1_msg_tt_adjust_gstmr(cch_tt_data_ptr[gas_id], TRUE,gas_id);
}

/*===========================================================================

FUNCTION afc_adjust_tcxo_rotator

DESCRIPTION
  Corrects the given frequency error using the rotator and PDM, and
  move rotator correction to the PDM if possible.

===========================================================================*/
void afc_adjust_tcxo_rotator( int32 freq_corr, ARFCN_T arfcn,gas_id_t gas_id)
{
  int32 curr_xo_val = gl1_hw_get_xo_acc_freq_err(gas_id);

  /* update accumulated xo_freq_err for Tx compensation as well */
  if ( freq_corr != 0 )
  {
    gl1_hw_update_xo_acc_freq_err(
      gl1_hw_freqtonormxofreq( arfcn, freq_corr, gas_id ),gas_id );

    /* Only update cswallower when we have freq adjust */
    gl1_hw_set_active_cycle_swallower_adj( arfcn, gas_id );

    /* As new valid AFC setting update FEU sample */
    gl1_hw_update_raw_fe_tcxo_mgr( freq_corr, gas_id );

    /* Log the info */
    MSG_GERAN_HIGH_3_G( GL1_AFC_HDR"Adjust %dHz error before %dXO after %dXO",
              freq_corr, curr_xo_val, gl1_hw_get_xo_acc_freq_err(gas_id) );

    gl1_log_afc_adjust( ( gl1_hw_get_xo_acc_freq_err(gas_id) - curr_xo_val ),
                        0, /* No rot adjust applied yet */
                        LOG_AFC_TRACKING, gas_id );
  }
}

/*===========================================================================

FUNCTION afc_force_tcxo_update

DESCRIPTION This will take whatever residual freq correction that we have
            stored in the AFC struct for the idle/dedicated/pdch channel
            and force it to be applied, prior to the clearing of the
            averaged data.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void afc_force_tcxo_update( gl1_msgi_afc_struct *afc_data, boolean idle_mode ,gas_id_t gas_id)
{
  int32  freq_corr = 0, freq_avg_short_term = 0;
  uint32 st_abs;

  /* If we do not have enough bursts yet, we are done */
  if ( afc_data->cnt < MIN_FT_VALID_BURSTS )
  {
    if ( gl1_hw_debug_gsm_afc )
    {
      MSG_4( MSG_SSID_GSM_L1, MSG_LEGACY_LOW,
             GL1_AFC_HDR"Force update return < CNT : FLTACC=%d FLTCNT=%d : FSTACC=%d FSTCNT=%d",
             afc_data->acc, afc_data->cnt, afc_data->acc_lastN, MIN_FT_VALID_BURSTS );
    }

    return;
  }

  /* For the force case we use short term averaged freq for fast update */
  if (idle_mode)
  {
     freq_avg_short_term = ( afc_data->acc_lastN / SHORT_TERM_FT_AVG_LEN_IDLE );
  }
  else
  {
     freq_avg_short_term = ( afc_data->acc_lastN / SHORT_TERM_FT_AVG_LEN );
  }

  st_abs = abs( freq_avg_short_term );

  /* Limit incase we have a large residual error to avoid large excursions */
  if ( st_abs >= LARGE_FREQ_ERROR )
  {
    freq_corr = GL1_MAX_FREQ_CORR_HZ;

    if ( freq_avg_short_term < 0 )
    {
      freq_corr *= (-1);
    }
  }
  else
  {
    freq_corr = freq_avg_short_term;
  }

  /* If we are not making any frequency adjustments, we are done */
  if ( freq_corr != 0 )
  {
    afc_adjust_tcxo_rotator( freq_corr, afc_data->arfcn ,gas_id);
  }

  if ( freq_corr != 0 )
  {
    MSG_GERAN_LOW_2_G( GL1_AFC_HDR"Force AFC adjusted: ARFCN %d short term error %dHz",
             afc_data->arfcn.num, freq_corr);
  }

  /* As short term update has been applied clear this AFC struct */
  afc_init( afc_data );
}

/*===========================================================================

FUNCTION gl1_msg_afc_init

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_afc_init ( gas_id_t gas_id )
{
   idle_temp_comp[gas_id] = IDLE_TEMP_COMP_OFF;
}

/*===========================================================================

FUNCTION afc_adjust_tcxo

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#define FREQ_DRIFT_AVE_LENGTH    4
/* Measure XO drift 312 every (= 3 SACCH blocks = 1.44s, must be greater than 1s */
/* Adjust every 1248 frames (= 12 SACCH blocks = 5.76s) */
#define XO_TEMP_MEAS_GAP         312
#define ADJUST_GAP_FRAMES        (XO_TEMP_MEAS_GAP * FREQ_DRIFT_AVE_LENGTH)
#define FREQ_DRIFT_FOR_TT_HIGH_BAND   200
#define FREQ_DRIFT_FOR_TT_LOW_BAND    100
/* For freq drift due to cooling in idle after TCH, for 60s compensate for temp, */
/* next 90s have faster AFC (although in fact have fast AFC all the time in idle */
/* 12480 frames = 57.6s, 18720 frames = 86.4s */
#define FRAMES_FOR_IDLE_TEMP_COMP     12480
#define FRAMES_FOR_IDLE_FAST_AFC      (18720 + FRAMES_FOR_IDLE_TEMP_COMP)

static void afc_adjust_tcxo
(
  gl1_msgi_afc_struct *afc_data,
  boolean              idle_mode,
 gas_id_t              gas_id
)
{
  int32   freq_corr = 0, freq_avg_short_term = 0, freq_avg_long_term = 0;
  uint32  lt_abs, st_abs;
  boolean adjust_freq = TRUE;
  uint16  min_ft_valid_bursts = FAST_AFC_MIN_FT_VALID_BURSTS;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  uint16  small_freq_err_avg_len = SMALL_FREQ_ERR_AVG_LEN, med_freq_err_avg_len = MED_FREQ_ERR_AVG_LEN;
  uint32  small_freq_error = SMALL_FREQ_ERROR, large_freq_error = LARGE_FREQ_ERROR;

  static uint8 xo_drift_count[NUM_GERAN_DATA_SPACES];
  static int32 total_xo_drift[NUM_GERAN_DATA_SPACES], xo_drift[NUM_GERAN_DATA_SPACES][2], previous_temp_xo[NUM_GERAN_DATA_SPACES];
  uint32 current_frame_number;
#if defined (FEATURE_XO_RX_GAP_TEMP_COMP)
  int32  freq_drift_for_tt;
  int16  timing_corr;
#endif
  static uint32  last_idle_adjust_frame[NUM_GERAN_DATA_SPACES] =
                   { INITIAL_VALUE( 0 ) };
  static uint32  last_idle_frame_temp_xo[NUM_GERAN_DATA_SPACES] =
                   { INITIAL_VALUE( 0 ) };

  static uint32  idle_start_frame[NUM_GERAN_DATA_SPACES];

  if (idle_mode == TRUE)
  {
    /* Set parameters for fast AFC */
    min_ft_valid_bursts = FAST_AFC_MIN_FT_VALID_BURSTS;
    small_freq_error = FAST_AFC_SMALL_FREQ_ERROR;
    small_freq_err_avg_len = FAST_AFC_SMALL_FREQ_ERR_AVG_LEN;
    med_freq_err_avg_len = FAST_AFC_MED_FREQ_ERR_AVG_LEN;
    large_freq_error = FAST_LARGE_FREQ_ERROR;

    if (idle_temp_comp[gas_id] != IDLE_TEMP_COMP_OFF)
    {
      /* Only run idle mode temp compensation for 30s after TCH */
       current_frame_number = GSTMR_GET_FN_GERAN(gas_id);
       if (current_frame_number < idle_start_frame[gas_id])
       {
          current_frame_number += FRAMES_PER_HYPERFRAME;
       }
       if (current_frame_number > (idle_start_frame[gas_id] + FRAMES_FOR_IDLE_TEMP_COMP))
       {
          idle_temp_comp[gas_id] = IDLE_TEMP_COMP_FAST_AFC;
       }
       if (current_frame_number > (idle_start_frame[gas_id] + FRAMES_FOR_IDLE_TEMP_COMP + FRAMES_FOR_IDLE_FAST_AFC))
       {
          idle_temp_comp[gas_id] = IDLE_TEMP_COMP_OFF;
       }

       if ((idle_temp_comp[gas_id] == IDLE_TEMP_COMP_ACTIVE)||(idle_temp_comp[gas_id] == IDLE_TEMP_COMP_FIRST_SAMPLE))
       {
         /* During Rx gap during TCH adjust freq and time tracking every 12 SACCH blocks (5.76s) */
         /* Sign and size of adjustment set by TCXO manager drift over period */
          current_frame_number = GSTMR_GET_FN_GERAN(gas_id);
          if (current_frame_number < last_idle_frame_temp_xo[gas_id])
          {
             current_frame_number += FRAMES_PER_HYPERFRAME;
          }
          /* XO drift estimate from TCXO Manager based on temperatue */
          if ((current_frame_number - last_idle_frame_temp_xo[gas_id]) >= XO_TEMP_MEAS_GAP)
          {
             /* If first temp meas in adjust gap, reset count. If first in Rx gap then read X0 */
             if (last_idle_frame_temp_xo[gas_id] == last_idle_adjust_frame[gas_id])
             {
                xo_drift_count[gas_id] = 0;
                if (idle_temp_comp[gas_id] == IDLE_TEMP_COMP_FIRST_SAMPLE)
                {
                   previous_temp_xo[gas_id] = tcxomgr_ft_curr();
                }
             }
             else
             {
                xo_drift_count[gas_id] += 1;
                if (xo_drift_count[gas_id] == 1)
                {
                   if (idle_temp_comp[gas_id] == IDLE_TEMP_COMP_FIRST_SAMPLE)
                   {
                      xo_drift[gas_id][0] = (tcxomgr_ft_curr() + previous_temp_xo[gas_id])/2;
                   }
                   else
                   {
                      xo_drift[gas_id][0]= xo_drift[gas_id][1];
                      xo_drift_count[gas_id] += 1;
                   }
                }
                if (xo_drift_count[gas_id] == (FREQ_DRIFT_AVE_LENGTH-2))
                {
                   previous_temp_xo[gas_id] = tcxomgr_ft_curr();
                }
                if (xo_drift_count[gas_id] == (FREQ_DRIFT_AVE_LENGTH-1))
                {
                   xo_drift[gas_id][1] = (tcxomgr_ft_curr() + previous_temp_xo[gas_id])/2;
                   MSG_GERAN_MED_3_G("Rx gap xo drift = %d - %d (last reading = %d)", xo_drift[gas_id][0], xo_drift[gas_id][1], previous_temp_xo[gas_id]);
                }
             }
             last_idle_frame_temp_xo[gas_id] = current_frame_number;

             /* if ((current_frame_number - last_idle_adjust_frame) > ADJUST_GAP_FRAMES) */
             /* Estimated drift = difference between average of 2 readings at the end of */
             /* the last gap and the second pair of readings at the end of this gap. */
             if (xo_drift_count[gas_id] == (FREQ_DRIFT_AVE_LENGTH-1))
             {
                last_idle_adjust_frame[gas_id] = current_frame_number;

#if defined  FEATURE_TCXOMGR_NEGATE_FREQ_ERROR
                total_xo_drift[gas_id] = xo_drift[gas_id][0] - xo_drift[gas_id][1];
#else
                total_xo_drift[gas_id] = xo_drift[gas_id][1] - xo_drift[gas_id][0];
#endif /* FEATURE_TCXOMGR_NEGATE_FREQ_ERROR | FEATURE_UMTS_NEGATE_ALL_TCXOMGR_VALUES */

                if (idle_temp_comp[gas_id] == IDLE_TEMP_COMP_FIRST_SAMPLE)
                {
                   idle_temp_comp[gas_id] = IDLE_TEMP_COMP_ACTIVE;
                }

                /* Additionally adjust XO by 3/4 of TCXO estimated drift, as want to err on the low side*/
                total_xo_drift[gas_id] = (total_xo_drift[gas_id] * 3) / 4;
                freq_corr = gl1_hw_normxofreqtofreq( afc_data->arfcn, total_xo_drift[gas_id],gas_id);
                afc_adjust_tcxo_rotator( freq_corr, afc_data->arfcn,gas_id );
                MSG_GERAN_HIGH_2_G( "AFC adjust temp drift during idle: ARFCN %d %dHz error",
                   afc_data->arfcn.num, freq_corr);

                /* Reset AFC data as adjustment has been made, also remove stale values */
                afc_init( afc_data );
             }
          }
       }
       else   /* Idle Fast AFC mode, for 90s after temp compensation - 120s total*/
       {
           /* Use fast AFC parameters, already set as also used during temp compensation phase */
       }
    }
  }
  else
  {
     last_idle_adjust_frame[gas_id] = GSTMR_GET_FN_GERAN(gas_id);
     last_idle_frame_temp_xo[gas_id] = last_idle_adjust_frame[gas_id];
     idle_start_frame[gas_id] = last_idle_adjust_frame[gas_id];
     idle_temp_comp[gas_id] = IDLE_TEMP_COMP_FIRST_SAMPLE;
  }


  /* If we do not have enough bursts yet, do not run AFC */
  if ( afc_data->cnt < min_ft_valid_bursts )
  {
    if ( gl1_hw_debug_gsm_afc )
    {
      MSG_4( MSG_SSID_GSM_L1, MSG_LEGACY_LOW,
             GL1_AFC_HDR"I/P return < CNT : FLTACC=%d FLTCNT=%d : FSTACC=%d FSTCNT=%d",
             afc_data->acc, afc_data->cnt, afc_data->acc_lastN, MIN_FT_VALID_BURSTS );
    }
    adjust_freq = FALSE;
  }
  else
  {
    freq_avg_long_term  = ( afc_data->acc / afc_data->cnt );

    freq_avg_short_term = ( afc_data->acc_lastN / SHORT_TERM_FT_AVG_LEN_IDLE );

    lt_abs = abs( freq_avg_long_term );
    st_abs = abs( freq_avg_short_term );
    if ( st_abs >= large_freq_error )
    {
      if ( idle_mode || mcfg_gcf_nv_get_status())
      {
        freq_corr = GL1_MAX_FREQ_CORR_HZ;
      }
      else
      {
        /* for dedicated mode modified max correction */
        freq_corr = GL1_MAX_DED_FREQ_CORR_HZ;
      }

      if ( freq_avg_short_term < 0 )
      {
        freq_corr *= (-1);
      }
    }
    else if ( ( lt_abs >= small_freq_error ) &&
            ( afc_data->cnt >= med_freq_err_avg_len ) )
    {
      freq_corr = freq_avg_long_term;
    }
    else if ( ( lt_abs <= small_freq_error ) &&
            ( afc_data->cnt >= small_freq_err_avg_len ) )
    {
      freq_corr = freq_avg_long_term;

      /* Always reset data after SMALL_FREQ_ERR_AVG_LEN,
         even if freq_corr == 0 */
      afc_init( afc_data );
    }
    else
    {
      freq_corr   = 0;

      adjust_freq = idle_mode;
    }

    /* Make an adjustment if the error is non-zero */
    if ( freq_corr != 0 )
    {
      afc_init( afc_data );

      afc_adjust_tcxo_rotator( freq_corr, afc_data->arfcn,gas_id );

      MSG_GERAN_LOW_2_G( "AFC adjusted: ARFCN %d %dHz error",
               afc_data->arfcn.num, freq_corr);
    }
  }  /*if ( afc_data->cnt < MIN_FT_VALID_BURSTS ).. else*/

#ifdef FEATURE_XO_RX_GAP_TEMP_COMP
  /* Update average drift, to be used during large Rx gap */
  if ( adjust_freq )
  {
     gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->last_adjust_frame = GSTMR_GET_FN_GERAN(gas_id);
     gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->last_frame_temp_xo = gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->last_adjust_frame;
     gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->gap_freq_adjust = 0;
     gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->gap_init_sample = TRUE;
  }
  else
  {
     if (idle_mode == FALSE)
     {
      /* During Rx gap during TCH adjust freq and time tracking every 12 SACCH blocks (5.76s) */
      /* Sign and size of adjustment set by TCXO manager drift over period */
       current_frame_number = GSTMR_GET_FN_GERAN(gas_id);
       if (current_frame_number < gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->last_frame_temp_xo)
       {
          current_frame_number += FRAMES_PER_HYPERFRAME;
       }
       /* XO drift estimate from TCXO Manager based on temperatue */
       if ((current_frame_number - gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->last_frame_temp_xo) >= XO_TEMP_MEAS_GAP)
       {
          /* If first temp meas in adjust gap, reset count. If first in Rx gap then read X0 */
          if (gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->last_frame_temp_xo == gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->last_adjust_frame)
          {
             gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift_count = 0;
             if (gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->gap_init_sample == TRUE)
             {
                gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->previous_temp_xo = tcxomgr_ft_curr();
             }
          }
          else
          {
             gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift_count += 1;
             if (gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift_count == 1)
             {
                if (gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->gap_init_sample == TRUE)
                {
                   gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift[0] = tcxomgr_ft_curr() + gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->previous_temp_xo;
                }
                else
                {
                   gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift[0]= gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift[1];
                }
             }
             if (gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift_count == (FREQ_DRIFT_AVE_LENGTH-2))
             {
                gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->previous_temp_xo = tcxomgr_ft_curr();
             }
             if (gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift_count == (FREQ_DRIFT_AVE_LENGTH-1))
             {
                gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift[1] = tcxomgr_ft_curr() + gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->previous_temp_xo;
                MSG_GERAN_HIGH_3_G("Rx gap xo drift = %d - %d (last reading = %d)",
                 gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift[0], gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift[1], gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->previous_temp_xo);
             }
          }
          gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->last_frame_temp_xo = current_frame_number;

          /* if ((current_frame_number - last_adjust_frame) > ADJUST_GAP_FRAMES) */
          /* Estimated drift = difference between average of 2 readings at the end of */
          /* the last gap and the second pair of readings at the end of this gap. */
          if (gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift_count == (FREQ_DRIFT_AVE_LENGTH-1))
          {
             gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->last_adjust_frame = current_frame_number;

#if defined  FEATURE_TCXOMGR_NEGATE_FREQ_ERROR
             total_xo_drift[gas_id] = (gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift[0] - gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift[1])/2;
#else
             total_xo_drift[gas_id] = (gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift[1] - gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->xo_drift[0])/2;
#endif /* FEATURE_TCXOMGR_NEGATE_FREQ_ERROR | FEATURE_UMTS_NEGATE_ALL_TCXOMGR_VALUES */

             /* For init gap the drift is FREQ_DRIFT_AVE_LENGTH/(FREQ_DRIFT_AVE_LENGTH - 2) times freq drift */
             /* between xo_drift readings (assuming constant dFreq/dTime). So for FREQ_DRIFT_AVE_LENGTH = 4 */
             /* this is a factor of 2 (ie drift has been measured over half the gap). */
             if (gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->gap_init_sample == TRUE)
             {
                gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->gap_init_sample = FALSE;
                total_xo_drift[gas_id] = total_xo_drift[gas_id] * 2;
             }

             /* Additionally adjust XO by 3/4 of TCXO estimated drift, as want to err on the low side*/
             total_xo_drift[gas_id] = (total_xo_drift[gas_id] * 3) / 4;
             freq_corr = gl1_hw_normxofreqtofreq( afc_data->arfcn, total_xo_drift[gas_id],gas_id);
             afc_adjust_tcxo_rotator( freq_corr, afc_data->arfcn ,gas_id);
             MSG_GERAN_HIGH_2_G( "AFC adjust drift during rx gap: ARFCN %d %dHz error",
                afc_data->arfcn.num, freq_corr);

             /* Adjust the time tracking based on the total freq drift in the gap */
             if ( (afc_data->arfcn.band == SYS_BAND_PCS_1900) || (afc_data->arfcn.band == SYS_BAND_DCS_1800) )
             {
                freq_drift_for_tt = FREQ_DRIFT_FOR_TT_HIGH_BAND;
             }
             else
             {
                freq_drift_for_tt = FREQ_DRIFT_FOR_TT_LOW_BAND;
             }

             gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->gap_freq_adjust += freq_corr;
             timing_corr = 0;
             if (gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->gap_freq_adjust > freq_drift_for_tt)
             {
                gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->gap_freq_adjust -= freq_drift_for_tt;
                timing_corr = -1;
             }
             else if (gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->gap_freq_adjust < (0 - freq_drift_for_tt))
             {
                gl1_msgi_xo_rx_gap_temp_comp_ptr[gas_id]->gap_freq_adjust += freq_drift_for_tt;
                timing_corr = 1;
             }
             if (timing_corr != 0)
             {
                gl1_msg_correct_timing(timing_corr,gas_id);
                MSG_GERAN_HIGH_1_G("TT drift: GSTMR timing adjust = %d",
                       timing_corr);
             }

             /* Reset AFC data as adjustment has been made, also remove stale values */
             afc_init( afc_data );
          }
       }
     }
  }
#endif /* FEATURE_XO_RX_GAP_TEMP_COMP */

  if ( adjust_freq )
  {
    /* Inform the drivers the AFC loop is locked */
    gl1_hw_afc_locked(idle_mode,gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    MSG_GERAN_MED_2_G("firmware_loaded %d,trm_status %d",
                      l1_tskisr_blk->firmware_loaded,
                      grm_get_trm_status(l1_tskisr_blk->client_id, gas_id));
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  }
}


/*===========================================================================

FUNCTION gl1_msg_dch_afc_adjust

DESCRIPTION


===========================================================================*/
void gl1_msg_dch_afc_adjust( gas_id_t gas_id )
{
#ifdef FEATURE_GSM_DTM
   if (afc_tt_mode[gas_id] == AFC_TT_MODE_DCH)
#endif
   {
      afc_adjust_tcxo(&dch_afc_data[gas_id], FALSE,gas_id);
   }
}


/*===========================================================================

FUNCTION gl1_msg_pdch_afc_adjust

DESCRIPTION


===========================================================================*/
void gl1_msg_pdch_afc_adjust(gas_id_t gas_id)
{
#ifdef FEATURE_GSM_DTM
   if (afc_tt_mode[gas_id] == AFC_TT_MODE_PDCH)
#endif
   {
      afc_adjust_tcxo(&dch_afc_data[gas_id], FALSE,gas_id);
   }
}


/*===========================================================================

FUNCTION gl1_msg_cch_afc_adjust()

DESCRIPTION


===========================================================================*/
void gl1_msg_cch_afc_adjust( gas_id_t gas_id )
{
  afc_adjust_tcxo(cch_afc_data_ptr[gas_id], TRUE,gas_id);
}


/* For XO/non-XO TT updates are more reliable so keep threshold the same */
#define SAIC_MIN_GOOD_TT_BURST_SNR  (643)   /* 4dB linear threshold for TT update */

/*
 * For non-XO AFC updates are less reliable so use a larger threshold to avoid
 * unwanted AFC updates affecting the uplink with large intereference on the downlink
 * For XO we have a greater rate of drift so need to keep feeding AFC even though
 * this will introduce a more variable uplink frequency performance
 */
#ifdef FEATURE_VAMOS
#define SAIC_MIN_GOOD_AFC_BURST_SNR      (643)   /* 4dB linear threshold for AFC update */
#define SAIC_MIN_GOOD_AFC_BURST_SNR_GPRS (1615)  /* 8dB linear threshold for AFC update */
#else
#define SAIC_MIN_GOOD_AFC_BURST_SNR      (1615)  /* 8dB linear threshold for AFC update */
#define SAIC_MIN_GOOD_AFC_BURST_SNR_GPRS (SAIC_MIN_GOOD_AFC_BURST_SNR)  /* non-VAMOS use SAIC_MIN_GOOD_AFC_BURST_SNR */
#endif /* FEATURE_VAMOS */

/*===========================================================================

FUNCTION gl1_msg_gprs_saic_tt_threshold_snr()

DESCRIPTION
         This will check that if pre_mimo SNR is below 2dB threshold and
         burst SNR is better than 4dB then data can be fed into TT using
         the burst SNR value otherwise the pre_mimo SNR is used.

DEPENDENCIES
  Saic needs to be enabled.

RETURN VALUE
  Appropriate SNR value either pre_mimo or burst_snr.

SIDE EFFECTS
  None
===========================================================================*/
uint16 gl1_msg_gprs_saic_tt_threshold_snr( gl1_defs_rx_pkt_metrics_type *m )
{
  uint16 adjusted_snr = m->pre_mimo_trained_complete_snr;

  /* If pre_mimo SNR is below 2dB and burst snr > 4dB then use burst snr */
  if ( ( adjusted_snr < FT_SNR_THRESH[ SAIC_GMSK_ELEMENT ] ) &&
       ( m->snr > SAIC_MIN_GOOD_TT_BURST_SNR ) )
  {
    adjusted_snr = m->snr;
  }

  return ( adjusted_snr );
}

/*===========================================================================

FUNCTION gl1_msg_gprs_saic_afc_threshold_snr()

DESCRIPTION
         This will check that if pre_mimo SNR is below 2dB threshold and
         burst SNR is better than 8dB then data can be fed into AFC using
         the burst SNR value otherwise the pre_mimo SNR is used.

DEPENDENCIES
  Saic needs to be enabled.

RETURN VALUE
  Appropriate SNR value either pre_mimo or burst_snr.

SIDE EFFECTS
  None
===========================================================================*/
uint16 gl1_msg_gprs_saic_afc_threshold_snr( gl1_defs_rx_pkt_metrics_type *m )
{
  uint16 adjusted_snr = m->pre_mimo_trained_complete_snr;

  /* If pre_mimo SNR is below 2dB and burst snr > 8dB then use burst snr */
  if ( ( adjusted_snr < FT_SNR_THRESH[ SAIC_GMSK_ELEMENT ] ) &&
       ( m->snr > SAIC_MIN_GOOD_AFC_BURST_SNR_GPRS ) )
  {
    adjusted_snr = m->snr;
  }

  return ( adjusted_snr );
}

/*===========================================================================

FUNCTION gl1_msg_saic_tt_threshold_snr()

DESCRIPTION
         This will check that if pre_mimo SNR is below 2dB threshold and
         burst SNR is better than 4dB then data can be fed into TT using
         the burst SNR value otherwise the pre_mimo SNR is used.

DEPENDENCIES
  Saic needs to be enabled.

RETURN VALUE
  Appropriate SNR value either pre_mimo or burst_snr.

SIDE EFFECTS
  None
===========================================================================*/
uint16 gl1_msg_saic_tt_threshold_snr( gl1_defs_metrics_rpt *m )
{
  uint16 adjusted_snr = m->pre_mimo_trained_complete_snr;

  if ( ( adjusted_snr < FT_SNR_THRESH[ SAIC_GMSK_ELEMENT ] ) &&
       ( m->snr > SAIC_MIN_GOOD_TT_BURST_SNR ) )
  {
    adjusted_snr = m->snr;
  }

  return ( adjusted_snr );
}

/*===========================================================================

FUNCTION gl1_msg_saic_afc_threshold_snr()

DESCRIPTION
         This will check that if pre_mimo SNR is below 2dB threshold and
         burst SNR is better than 8dB then data can be fed into AFC using
         the burst SNR value otherwise the pre_mimo SNR is used.

DEPENDENCIES
  Saic needs to be enabled.

RETURN VALUE
  Appropriate SNR value either pre_mimo or burst_snr.

SIDE EFFECTS
  None
===========================================================================*/
uint16 gl1_msg_saic_afc_threshold_snr( gl1_defs_metrics_rpt *m )
{
  uint16 adjusted_snr = m->pre_mimo_trained_complete_snr;

  if ( ( adjusted_snr < FT_SNR_THRESH[ SAIC_GMSK_ELEMENT ] ) &&
       ( m->snr > SAIC_MIN_GOOD_AFC_BURST_SNR ) )
  {
    adjusted_snr = m->snr;
  }

  return ( adjusted_snr );
}

/*===========================================================================

FUNCTION gl1_msg_dch_tt_update()

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_dch_tt_update
(
   int16 timing_offset,
   uint16 snr,
   boolean saic_used,
   gas_id_t gas_id
)
{
   gl1_defs_modulation_type mod;

#ifdef FEATURE_GSM_DTM
   if (afc_tt_mode[gas_id] == AFC_TT_MODE_DCH)
#endif
   {
      mod = gl1_msg_snr_threshold(saic_used);
      tt_update(&dch_tt_data[gas_id], timing_offset, snr, mod, FALSE);
   }
}


/*===========================================================================

FUNCTION gl1_msg_pdch_tt_update()

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_pdch_tt_update
(
   int16 timing_offset,
   uint16 snr,
   gl1_defs_modulation_type mod,
   gas_id_t gas_id
)
{
#ifdef FEATURE_GSM_DTM
   if (afc_tt_mode[gas_id] == AFC_TT_MODE_PDCH)
#endif
   {
      tt_update(&dch_tt_data[gas_id], timing_offset, snr, mod, FALSE);
   }
}


/*===========================================================================

FUNCTION gl1_msg_cch_tt_update()

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_cch_tt_update
(
   int16 timing_offset,
   uint16 snr,
   boolean saic_used,
   gas_id_t gas_id
)
{
  gl1_defs_modulation_type mod;

  mod = gl1_msg_snr_threshold(saic_used);
  tt_update(cch_tt_data_ptr[gas_id], timing_offset, snr, mod, TRUE);
}


/*===========================================================================

FUNCTION gl1_msg_dch_afc_update()

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_dch_afc_update (int16 freq_err, uint16 snr, boolean saic_used,gas_id_t gas_id)
{
   gl1_defs_modulation_type mod;
#ifdef FEATURE_GSM_DTM
   if (afc_tt_mode[gas_id] == AFC_TT_MODE_DCH)
#endif
   {
      mod = gl1_msg_snr_threshold(saic_used);

      (void) afc_update(&dch_afc_data[gas_id], freq_err, snr, mod, saic_used, FALSE,gas_id);
   }
}


/*===========================================================================

FUNCTION gl1_msg_pdch_afc_update()

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void gl1_msg_pdch_afc_update( uint8                          num_timeslots,
                              uint8                          burst_num,
                              gl1_defs_rx_pkt_metrics_type*  metrics_ptr[GL1_DEFS_MAX_DL_MSGS],
                              gas_id_t gas_id
                            )
{
#ifdef FEATURE_GSM_DTM
   if (afc_tt_mode[gas_id] == AFC_TT_MODE_PDCH)
#endif
  {
    uint8   i = 0;
    static uint8  num_valid_bursts = 0; /* used to store the no.of valid bursts in a block */
    static int16 b1_freq_offset[GL1_DEFS_MAX_DL_MSGS];/* used to store frequency offset value for the burst1*/
    static int16 b2_freq_offset[GL1_DEFS_MAX_DL_MSGS];/* used to store frequency offset value for the burst2*/
    static int16 b3_freq_offset[GL1_DEFS_MAX_DL_MSGS];/* used to store frequency offset value for the burst3*/

    if(metrics_ptr[0]->brstCancel == 0)
    {
      num_valid_bursts++;
    }

    while( i < num_timeslots )
    { 
      /* Storing frequency offset value for the all the valid time slots */
      if (0 == burst_num)
      {
        b1_freq_offset[i] = metrics_ptr[i]->freq_offset;
      }
      if (1 == burst_num)
      {
        b2_freq_offset[i] = metrics_ptr[i]->freq_offset;
      }
      if (2 == burst_num)
      {
        b3_freq_offset[i] = metrics_ptr[i]->freq_offset;
      }

      switch(num_valid_bursts)
      {
        case 2:
          if( ((2 == burst_num)||(3 == burst_num))&&(metrics_ptr[0]->brstCancel == 1) )
          {
            /* This is a case where first two bursts are valid and 3rd,4th bursts are invalid bursts(Cancelled bursts).
             * Taking average of valid bursts frequency offset as invalid burst frequency offset */
            metrics_ptr[i]->freq_offset = (b1_freq_offset[i] + b2_freq_offset[i])>>1;
          }
        break;
        case 3:
          if( (3 == burst_num)&&(metrics_ptr[0]->brstCancel == 1) )
          {
            /* This is a case where first three bursts are valid and 4th burst is invalid burst(Cancelled burst).
             * Taking average of valid bursts frequency offset as invalid burst frequency offset */
            metrics_ptr[i]->freq_offset = (b1_freq_offset[i] + b2_freq_offset[i]+ b3_freq_offset[i])/3;
          }
        break;
        default:
        break;
      }
      i++;
    }
    i=0;

    while( i < num_timeslots )
    {
      uint16   snr;
      boolean  saic_used = FALSE;

#ifdef FEATURE_GSM_MDSP_ESAIC
      /* SAIC active so use best of pre-mimo and post-snr */
      if( gl1_msg_saic_in_burst(metrics_ptr[i]->misc_flags) )
      {
        snr       = metrics_ptr[i]->pre_mimo_trained_complete_snr;
        saic_used = TRUE;
      }
      else
#endif /* FEATURE_GSM_MDSP_ESAIC */
      {
        /* No SAIC active so use normal burst snr */
        snr = metrics_ptr[i]->snr;
      }


      /* Update using one timeslot then exit */
      if (afc_update( &dch_afc_data[gas_id], metrics_ptr[i]->freq_offset,snr,metrics_ptr[i]->modulation,saic_used,FALSE,gas_id) )
      {
        /* BURST ID == 1 */

        /* Print a message when the BURST ID is 1 and
         * the timeslot is not the lowest.
         */
        if( i != 0 )
        {
          uint32  FN = GSTMR_GET_FN_GERAN(gas_id );

          switch( FN %13 )
          {
          case 2:
          case 6:
          case 10:
             MSG_GERAN_MED_2_G("AFC ts %d offset %d Hz update",i,metrics_ptr[i]->freq_offset);
           break;

          default:
           break;
          }
        }

        break;
      }

      i++;

    } /* While( i < num_timeslots ) */
    if(3 == burst_num)
    {
      num_valid_bursts = 0;
    }
  }   /* if (afc_tt_mode[gas_id] == AFC_TT_MODE_PDCH) */
}



/*===========================================================================

FUNCTION gl1_msg_cch_afc_update()

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_cch_afc_update (int16 freq_err, uint16 snr, boolean saic_used,gas_id_t gas_id)
{
  gl1_defs_modulation_type mod;

  mod = gl1_msg_snr_threshold(saic_used);

  (void) afc_update(cch_afc_data_ptr[gas_id], freq_err, snr, mod, saic_used, TRUE,gas_id);
}



/*===========================================================================

FUNCTION gl1_msg_correct_timing

DESCRIPTION
  This function is used to change the MS timing.  This is done by advancing
  or delaying the GSTMR.

  Negative values of timing_error will advance the GSTMR, positive values
  will delay the GSTMR.  The GSTMR count is changed by adjusting the
  terminal count for the current frame.  Large negative values should not
  be used since they may represent a time that has already passed!  The
  behaviour when this occurs is undefined.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_correct_timing( int32 timing_error ,gas_id_t gas_id )
{
   gl1_msgi_tt_struct *tt_data = cch_tt_data_ptr[gas_id];
   gl1_hw_delay_frame_tick( timing_error ,gas_id);

   if ( timing_error > DEF_QSYMB_TIME_TRACKING_ADJ )
   {
     timing_error = -tt_data->g2l_slot_change;
     tt_data->g2l_slot_change = 0;
   }
   else
   {
     timing_error = -timing_error;
   }

   tt_data->g2l_acc += timing_error;
}

/*===========================================================================

FUNCTION gl1_msg_update_g2l_slot_change

DESCRIPTION
  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_update_g2l_slot_change( int32 timing_error, gas_id_t gas_id )
{
   gl1_msgi_tt_struct *tt_data = cch_tt_data_ptr[gas_id];

   tt_data->g2l_slot_change = timing_error;
}

/*===========================================================================

FUNCTION gl1_msg_get_g2l_tt_acc

DESCRIPTION
  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
int16 gl1_msg_get_g2l_tt_acc( gas_id_t gas_id )
{
   gl1_msgi_tt_struct *tt_data = cch_tt_data_ptr[gas_id];
   int16 ret = tt_data->g2l_acc;
   tt_data->g2l_acc = 0;
   tt_data->g2l_slot_change = 0;

   MSG_GERAN_HIGH_1_G("TT: G2L timing adjust = %d", ret);

   return ret;
}

/*===========================================================================

                         Radio link measurements
                         -----------------------
                             RXQUAL_FULL
                             RXQUAL_SUB

  See GSM 05.08 for requirements:
  - section 8.2 ... Signal quality
  - section 8.3 ... DTX transmission (aspects of...)
  - section 8.4 ... Measurement reporting

===========================================================================*/

/*===========================================================================

FUNCTION gl1_msg_rxqual_init

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_rxqual_init( gas_id_t gas_id )
{
  rx_qual_params[gas_id].full_qual_total  = 0;
  rx_qual_params[gas_id].full_qual_blocks = 0;
  rx_qual_params[gas_id].sub_qual_total   = 0;
  rx_qual_params[gas_id].sub_qual_blocks  = 0;
}

#ifdef FEATURE_GSM_DYNAMIC_SAIC
/*===========================================================================

FUNCTION gl1_msg_get_hard_rxqual_full_avg

DESCRIPTION
 Calculates Average hard Rxqual over the number of blocks received

DEPENDENCIES
  None

RETURN VALUE
  Averaged Hard Rxqual in raw DSP format 0..32767

SIDE EFFECTS
  None
===========================================================================*/
uint32 gl1_msg_get_hard_rxqual_full_avg(gas_id_t gas_id)
{
  if ( rx_qual_params[gas_id].full_qual_blocks )
  {
    return(rx_qual_params[gas_id].full_qual_total / rx_qual_params[gas_id].full_qual_blocks);
  }
  else
  {
    return(0);
  }
}
#endif

/* Include measurements for all TCH/FACCH and SACCH blocks in
 * RXQUAL_FULL
 */
void gl1_msg_rxqual_full_update( uint16 dsp_rxqual,gas_id_t gas_id)
{
  rx_qual_params[gas_id].full_qual_total += dsp_rxqual;
  rx_qual_params[gas_id].full_qual_blocks++;
}

/* Include measurements for 2nd SID and SACCH blocks
 * in RXQUAL_SUB
 */
void gl1_msg_rxqual_sub_update( uint16 dsp_rxqual,gas_id_t gas_id)
{
  rx_qual_params[gas_id].sub_qual_total += dsp_rxqual;
  rx_qual_params[gas_id].sub_qual_blocks++;
}

uint8 gl1_msg_get_rxqual_full(boolean *meas_report_valid,gas_id_t gas_id)
{
  if(*meas_report_valid)
  {
      return( calc_rxqual_lev( rx_qual_params[gas_id].full_qual_total, rx_qual_params[gas_id].full_qual_blocks ,gas_id) );
  }
  else
  {
      *meas_report_valid=TRUE;
      /* no measurements have been taken so indicate this to RR */
      return INVALID_RXQUAL_MEAS;
  }
}

uint8 gl1_msg_get_rxqual_sub(gas_id_t gas_id)
{
   /*
    * If function gl1_msg_get_rxqual_full returns INVALID_RXQUAL_MEAS,
    * then this functions return value is ignored.
    */
      return( calc_rxqual_lev( rx_qual_params[gas_id].sub_qual_total, rx_qual_params[gas_id].sub_qual_blocks,gas_id) );
}

/*===========================================================================

FUNCTION gl1_msg_bep_init

DESCRIPTION
     Initializes mean and CV BEP totals

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_bep_init( gas_id_t gas_id )
{
   rx_qual_params[gas_id].mean_bep_total = 0;
   rx_qual_params[gas_id].mean_bep_blocks = 0;
   rx_qual_params[gas_id].cv_bep_total   = 0;
   rx_qual_params[gas_id].cv_bep_blocks  = 0;
}

/*===========================================================================

FUNCTION gl1_msg_mean_bep_update

DESCRIPTION
     Accumulates passed in mean BEP

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_mean_bep_update ( uint32 mean_bep,gas_id_t gas_id )
{
   rx_qual_params[gas_id].mean_bep_total += mean_bep;
   rx_qual_params[gas_id].mean_bep_blocks++;
}

/*===========================================================================

FUNCTION gl1_msg_cv_bep_update

DESCRIPTION
     Accumulates passed in CV BEP

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_cv_bep_update ( uint32 cv_bep,gas_id_t gas_id )
{
   rx_qual_params[gas_id].cv_bep_total += cv_bep;
   rx_qual_params[gas_id].cv_bep_blocks++;
}

/*===========================================================================

FUNCTION gl1_msg_get_mean_bep

DESCRIPTION
     Computes and returns the mean BEP ( total / num blocks )

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
uint8 gl1_msg_get_mean_bep( gas_id_t gas_id )
{
   uint32 mean_bep = 0;
   if ( rx_qual_params[gas_id].mean_bep_blocks )
   {
      mean_bep = rx_qual_params[gas_id].mean_bep_total / rx_qual_params[gas_id].mean_bep_blocks;
      return ( gl1_msg_calc_mean_bep_gmsk ( mean_bep ) );
   }
   else
   {
      MSG_GERAN_ERROR_0_G("Num mean BEP measurements = 0");
      return ( 0 );
   }
}

/*===========================================================================

FUNCTION gl1_msg_get_cv_bep

DESCRIPTION
     Computes and returns the CV BEP ( total / num blocks )

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
uint8 gl1_msg_get_cv_bep( gas_id_t gas_id )
{
   uint32 cv_bep = 0;
   if ( rx_qual_params[gas_id].cv_bep_blocks )
   {
      cv_bep = rx_qual_params[gas_id].cv_bep_total / rx_qual_params[gas_id].cv_bep_blocks;
      return ( gl1_msg_calc_cv_bep ( cv_bep ) );
   }
   else
   {
      MSG_GERAN_ERROR_0_G("Num CV BEP measurements = 0");
      return ( 0 );
   }
}


uint8 gpl1_gprs_calc_rxqual_lev( uint32 rxqual_sum, uint16 block_cnt, gas_id_t gas_id)
{
  return( calc_rxqual_lev( rxqual_sum, block_cnt,gas_id ) );
}


#define GSM_900_RXQUAL_H_THR_0 32635
#define GSM_900_RXQUAL_H_THR_1 32485
#define GSM_900_RXQUAL_H_THR_2 32225
#define GSM_900_RXQUAL_H_THR_3 31630
#define GSM_900_RXQUAL_H_THR_4 30450
#define GSM_900_RXQUAL_H_THR_5 28450
#define GSM_900_RXQUAL_H_THR_6 24379

#define DCS_RXQUAL_H_THR_0 32655
#define DCS_RXQUAL_H_THR_1 32475
#define DCS_RXQUAL_H_THR_2 32225
#define DCS_RXQUAL_H_THR_3 31650
#define DCS_RXQUAL_H_THR_4 30450
#define DCS_RXQUAL_H_THR_5 28450
#define DCS_RXQUAL_H_THR_6 24379

#define GSM_850_RXQUAL_H_THR_0 32655
#define GSM_850_RXQUAL_H_THR_1 32460
#define GSM_850_RXQUAL_H_THR_2 32150
#define GSM_850_RXQUAL_H_THR_3 31550
#define GSM_850_RXQUAL_H_THR_4 30450
#define GSM_850_RXQUAL_H_THR_5 28450
#define GSM_850_RXQUAL_H_THR_6 24379

#define PCS_RXQUAL_H_THR_0 32655
#define PCS_RXQUAL_H_THR_1 32460
#define PCS_RXQUAL_H_THR_2 32150
#define PCS_RXQUAL_H_THR_3 31550
#define PCS_RXQUAL_H_THR_4 30450
#define PCS_RXQUAL_H_THR_5 28450
#define PCS_RXQUAL_H_THR_6 24379

static uint8 calc_rxqual_lev( uint32 rxqual_sum, uint16 block_cnt,gas_id_t gas_id )
{
  uint32  rxqual_avg;
  uint8 rxqual_lev;

  if ( block_cnt )
  {
    rxqual_avg = rxqual_sum / block_cnt;

    switch (gl1_msg_algs_data_ptr[gas_id]->afc_band)
    {
      case SYS_BAND_PGSM_900:
      case SYS_BAND_EGSM_900:
        if ( rxqual_avg > GSM_900_RXQUAL_H_THR_0 )
          rxqual_lev = 0;
        else if ( rxqual_avg > GSM_900_RXQUAL_H_THR_1 )
          rxqual_lev = 1;
        else if ( rxqual_avg > GSM_900_RXQUAL_H_THR_2 )
          rxqual_lev = 2;
        else if ( rxqual_avg > GSM_900_RXQUAL_H_THR_3 )
          rxqual_lev = 3;
        else if ( rxqual_avg > GSM_900_RXQUAL_H_THR_4 )
          rxqual_lev = 4;
        else if ( rxqual_avg > GSM_900_RXQUAL_H_THR_5 )
          rxqual_lev = 5;
        else if ( rxqual_avg > GSM_900_RXQUAL_H_THR_6 )
          rxqual_lev = 6;
        else
          rxqual_lev = 7;
        break;

      case SYS_BAND_DCS_1800:
        if ( rxqual_avg > DCS_RXQUAL_H_THR_0 )
          rxqual_lev = 0;
        else if ( rxqual_avg > DCS_RXQUAL_H_THR_1 )
          rxqual_lev = 1;
        else if ( rxqual_avg > DCS_RXQUAL_H_THR_2 )
          rxqual_lev = 2;
        else if ( rxqual_avg > DCS_RXQUAL_H_THR_3 )
          rxqual_lev = 3;
        else if ( rxqual_avg > DCS_RXQUAL_H_THR_4 )
          rxqual_lev = 4;
        else if ( rxqual_avg > DCS_RXQUAL_H_THR_5 )
          rxqual_lev = 5;
        else if ( rxqual_avg > DCS_RXQUAL_H_THR_6 )
          rxqual_lev = 6;
        else
          rxqual_lev = 7;
        break;

      case SYS_BAND_PCS_1900:
        if ( rxqual_avg > PCS_RXQUAL_H_THR_0 )
          rxqual_lev = 0;
        else if ( rxqual_avg > PCS_RXQUAL_H_THR_1 )
          rxqual_lev = 1;
        else if ( rxqual_avg > PCS_RXQUAL_H_THR_2 )
          rxqual_lev = 2;
        else if ( rxqual_avg > PCS_RXQUAL_H_THR_3 )
          rxqual_lev = 3;
        else if ( rxqual_avg > PCS_RXQUAL_H_THR_4 )
          rxqual_lev = 4;
        else if ( rxqual_avg > PCS_RXQUAL_H_THR_5 )
          rxqual_lev = 5;
        else if ( rxqual_avg > PCS_RXQUAL_H_THR_6 )
          rxqual_lev = 6;
        else
          rxqual_lev = 7;
        break;

      case SYS_BAND_CELL_850:
        if ( rxqual_avg > GSM_850_RXQUAL_H_THR_0 )
          rxqual_lev = 0;
        else if ( rxqual_avg > GSM_850_RXQUAL_H_THR_1 )
          rxqual_lev = 1;
        else if ( rxqual_avg > GSM_850_RXQUAL_H_THR_2 )
          rxqual_lev = 2;
        else if ( rxqual_avg > GSM_850_RXQUAL_H_THR_3 )
          rxqual_lev = 3;
        else if ( rxqual_avg > GSM_850_RXQUAL_H_THR_4 )
          rxqual_lev = 4;
        else if ( rxqual_avg > GSM_850_RXQUAL_H_THR_5 )
          rxqual_lev = 5;
        else if ( rxqual_avg > GSM_850_RXQUAL_H_THR_6 )
          rxqual_lev = 6;
        else
          rxqual_lev = 7;
        break;

      default:
        MSG_GERAN_ERROR_1_G("Unknown band %d", gl1_msg_algs_data_ptr[gas_id]->afc_band);
        rxqual_lev = 0;
        break;
    }
  }
  else
  {
    rxqual_lev = 7;
    MSG_GERAN_ERROR_2_G("cnt=%d rxqual=%d", block_cnt, rxqual_lev);
  }

  return( rxqual_lev );
}

/*
 *  This BFI algorithm is from Ryan's doc with adding check for snr.
 */
#define RXQUAL_THRESHOLD_HI 28000
#define RXQUAL_THRESHOLD_LO 27000
#define HARD_RXQUAL_THRESHOLD_FR 22500
#define HARD_RXQUAL_THRESHOLD_EFR 16000

#define SNR_THRESHOLD 290

void gl1_msg_compute_bfi( boolean crc_pass,
#ifdef FEATURE_VAMOS_II
                          boolean vamos_paired,
                          gl1_defs_loopback_type loopback_type,
                          gas_id_t gas_id,
#endif
                          uint16 curr_rx_qual_s,
                          uint16 prev_rx_qual_s,
                          uint16 curr_rx_qual_h,
                          uint16 prev_bfi,
                          uint16 snr,
                          uint16 *new_bfi,
                          gl1_hw_channel_type chan      
                        )
{

#ifdef FEATURE_VAMOS_II
  if(vamos_paired && (loopback_type == GL1_DEFS_LOOPBACK_TYPE_A) && gl1_hw_get_vamos2_support(gas_id))
   {
     *new_bfi = (crc_pass == TRUE)?0:1;
   }
  else
#endif
  {
  if ( crc_pass &&
       (snr > SNR_THRESHOLD) &&
       ( (curr_rx_qual_h >= HARD_RXQUAL_THRESHOLD_FR) ||
         ((GL1_HW_TCH_EFS == chan) && (curr_rx_qual_h >= HARD_RXQUAL_THRESHOLD_EFR)) )
     )
  {
    if ( curr_rx_qual_s >= RXQUAL_THRESHOLD_HI )
    {
      *new_bfi = 0;
    }
    else if ( curr_rx_qual_s < RXQUAL_THRESHOLD_LO )
    {
      *new_bfi = 1;
    }
    else
    {
      if ( prev_bfi == 0 || (prev_rx_qual_s >= RXQUAL_THRESHOLD_HI) )
      {
        *new_bfi = 0;
      }
      else
      {
        *new_bfi = 1;
      }
    }
  }
  else
  {
    *new_bfi = 1;
  }
 }

}


#ifdef FEATURE_GSM_AMR
#ifdef FEATURE_GSM_AMR_WB
/* NOTE:last 3 values for WB_AMR have the same values as closest NB_AMR rate*/
uint16 snr_array_afs[GL1_MSG_AMR_MODE_MAX] = {220, 290, 300, 300, 300, 300, 300, 320, 300, 300, 320 };
uint16 rxqual_s_array_afs[GL1_MSG_AMR_MODE_MAX] =
                                  { 16000, 17000, 17000, 17000, 17000, 17000, 17000, 17000, 17000,17000,17000};
#else
uint16 snr_array_afs[GL1_MSG_AMR_MODE_MAX] = {220, 290, 300, 300, 300, 300, 300, 320 };
uint16 rxqual_s_array_afs[GL1_MSG_AMR_MODE_MAX] = { 16000, 17000, 17000, 17000, 17000, 17000, 17000, 17000};
#endif /* FEATURE_GSM_AMR_WB */
#define SNR_THRESHOLD_AFS_2 2500
#define RXQUAL_THRESHOLD_AFS_2 29000
void gl1_msg_compute_bfi_amr_fr( boolean crc_pass,
#ifdef FEATURE_VAMOS_II
                                 boolean vamos_paired,
                                 gl1_defs_loopback_type loopback_type,
                                 gas_id_t gas_id,
#endif
                                 uint16 curr_rx_qual_s,
                                 uint16 snr,
                                 gl1_msg_amr_mode_type dl_codec_mode,
                                 uint16 *new_bfi
                                 )
{
#ifdef FEATURE_VAMOS_II
  if(vamos_paired && (loopback_type == GL1_DEFS_LOOPBACK_TYPE_A) && gl1_hw_get_vamos2_support(gas_id))
   {
   *new_bfi = (crc_pass == TRUE)?0:1;
   }
  else
#endif
   {
   if ( crc_pass )
   {
      if (snr > snr_array_afs[dl_codec_mode])
      {
         if (snr > SNR_THRESHOLD_AFS_2)
         {
            if ( curr_rx_qual_s > RXQUAL_THRESHOLD_AFS_2)
            {
               *new_bfi = 0;
            }
            else
            {
               *new_bfi = 1;
            }
         }
         else
         {
            if ( curr_rx_qual_s > rxqual_s_array_afs[dl_codec_mode])
            {
               *new_bfi = 0;
            }
            else
            {
               *new_bfi = 1;
            }
         }
      }
      else
      {
         *new_bfi = 1;
      }
   }
   else
   {
      *new_bfi = 1;
   }
}
}

#endif



#define SNR_THRESHOLD_HR 260
#define RXQUAL_THRESHOLD_HR 21000
void gl1_msg_compute_bfi_ufi(
                              boolean crc_pass,
#ifdef FEATURE_VAMOS_II
                              boolean vamos_paired,
                              gl1_defs_loopback_type loopback_type,
                              gas_id_t gas_id,
#endif
                              uint16 curr_rx_qual,
                              uint16 prev_rx_qual,
                              uint16 prev_bfi,
                              uint16 *new_bfi,
                              uint16 *new_ufi,
                              uint16 snr,
                              uint16 curr_rx_qual_h,
							  uint16 prev_rx_qual_h
                           )
{
#ifdef FEATURE_VAMOS_II
 if(vamos_paired && (loopback_type == GL1_DEFS_LOOPBACK_TYPE_A) && gl1_hw_get_vamos2_support(gas_id))
  {
    *new_bfi = (crc_pass == TRUE)?0:1;
  }
 else
#endif
  {
  if ( crc_pass &&
       (snr > SNR_THRESHOLD_HR) &&
       (curr_rx_qual_h >= RXQUAL_THRESHOLD_HR) )
    {
       if (curr_rx_qual >= RXQUAL_THRESHOLD_HI)
       {
          *new_bfi = *new_ufi = 0;
       }
       else if ( curr_rx_qual < RXQUAL_THRESHOLD_LO )
       {
          *new_bfi = *new_ufi = 1;
       }
       else
       {
          if ( prev_bfi == 0 || ((prev_rx_qual >= RXQUAL_THRESHOLD_HI) && (prev_rx_qual_h >= RXQUAL_THRESHOLD_HR)))
          {
             *new_bfi = 0;
          }
          else
          {
             *new_bfi = 1;
          }

          *new_ufi = 1;
       }
    }
    else
    {
       *new_bfi = *new_ufi = 1;
    }
}
}



/* Final two entries are dummies but there to avoid Klocwork reporting array index overflow problems */
uint16 snr_array_ahs_low[GL1_MSG_AMR_MODE_MAX] = {300, 320, 300, 300, 300, 350, 350, 350 };
uint16 snr_array_ahs_high[GL1_MSG_AMR_MODE_MAX] = {2700, 2700, 2700, 2700, 2700, 2700, 2700, 2700};
uint16 rxqual_h_array_ahs_low[GL1_MSG_AMR_MODE_MAX] = {15500, 16500, 16000, 16000, 16000, 16000, 16000, 16000};
uint16 rxqual_h_array_ahs_high[GL1_MSG_AMR_MODE_MAX] = {16000, 17000, 16000, 16000, 16000, 16000, 16000, 16000};

void gl1_msg_compute_bfi_amr_hr(
                              boolean crc_pass,
#ifdef FEATURE_VAMOS_II
                              boolean vamos_paired,
                              gl1_defs_loopback_type loopback_type,
                              gas_id_t gas_id,
#endif
                              uint16 *new_bfi,
                              uint16 snr,
                              gl1_msg_amr_mode_type dl_codec_mode,
                              uint16 curr_rx_qual_h
                           )
{
#ifdef FEATURE_VAMOS_II
  if(vamos_paired && (loopback_type == GL1_DEFS_LOOPBACK_TYPE_A) && gl1_hw_get_vamos2_support(gas_id))
   {
   *new_bfi = (crc_pass == TRUE)?0:1;
   }
  else
#endif
   {
   if ( crc_pass )
   {
      if (snr > snr_array_ahs_low[dl_codec_mode])
      {
         if ( snr > snr_array_ahs_high[dl_codec_mode] )
         {
            if ( curr_rx_qual_h > rxqual_h_array_ahs_high[dl_codec_mode]  )
            {
               *new_bfi = 0;
            }
            else
            {
               *new_bfi = 1;
            }
         }
         else
         {
            if (curr_rx_qual_h > rxqual_h_array_ahs_low[dl_codec_mode])
            {
               *new_bfi = 0;
            }
            else
            {
               *new_bfi = 1;
            }
         }
      }
      else
      {
         *new_bfi = 1;
      }
   }
   else
   {
      *new_bfi = 1;
   }
}
}




uint16 gl1_msg_get_snr( gl1_defs_metrics_rpt *rpt[], uint8 num_bursts )
{
  int32 i;
  uint32 avg_snr;

  avg_snr = 0;

  for( i=0; i < num_bursts; i++ )
  {
    if(gl1_msg_saic_in_burst( rpt[i]->misc_flags))
    {
      if(rpt[i]->snr < BFI_SNR_THRESHOLD_SAIC_FR)
      {
        avg_snr += rpt[i]->pre_mimo_trained_complete_snr;
      }
      else
      {
        avg_snr += rpt[i]->snr;
      }
    }
    else
    {
       avg_snr += rpt[i]->snr;
    }
  }

  return( (uint16)(avg_snr / num_bursts ) );
}

/*
   HR can be 0,1 or 2 indicating if its a valid SID or not.
   Determine this from diff_c1 and diff_c1c2 computed by mdsp
Imaginary Buffer Line
*/
uint8 gl1_msg_compute_sid( uint16 diff_c1,
                           uint16 diff_c1c2,
                           uint8 mode
                          )
{
   uint8 sid = 0;

   if ( diff_c1 < 3 )
   {
      sid = ( mode == 0 ) ? 1 : 2;
   }
   else if ( diff_c1c2 < 11 )
   {
      sid = 1;
   }
   else
   {
      sid = 0;
   }

   return ( sid );
}

/*===========================================================================
FUNCTION gl1_msg_SNR_to_dBx16

DESCRIPTION
 Converts the raw SNR returned in metrics reports to dBx16.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
dBx16_T gl1_msg_SNR_to_dBx16( uint16 SNR )
{
   dBx16_T result;

   result  = gl1_msg_algs_linear_to_dBx16( SNR );

   result -= SNR_SCALE_FACTOR_dBx16;

   return result;
}

/*===========================================================================

FUNCTION  gl1_msg_algs_linear_to_dBx16

DESCRIPTION
  This function converts a linear value into a dBx16 value.  The return
  value is 16*10*log_10( x ).

DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/
/* Look up table for logarithms.     */
/* logtable[i] = round(10*log_10(i)) */
static const dBx16_T logtable[] = {
       0,  0, 48, 76, 96,112,125,135,144,153,
     160,167,173,178,183,188,193,197,201,205,
     208,212,215,218,221,224,226,229,232,234,
     236,239,241,243,245,247,249,251,253,255,
     256,258,260,261,263,265,266,268,269,270,
     272,273,275,276,277,278,280,281,282,283,
     285,286,287,288,289,290,291,292,293,294,
     295,296,297,298,299,300,301,302,303,304,
     304,305,306,307,308,309,310,310,311,312,
     313,313,314,315,316,316,317,318,319,319,
     320,321,321,322,323,323,324,325,325,326,
     327,327,328,328,329,330,330,331,332,332,
     333,333,334,334,335,336,336,337
};
#define LOG_TABLE_BITS 7

dBx16_T gl1_msg_algs_linear_to_dBx16( uint32 x )
{
   uint32  k,i;
   dBx16_T result;


   if (x >= (1UL << LOG_TABLE_BITS))
   {
      for (k = (x >> LOG_TABLE_BITS), i = 0; k > 0; i++, k >>= 1)
         ;

      result = (dBx16_T)(i * 48); /* factor of 2 in dBx16 */
      k      = (x >> i);
      result += logtable[k];  /*lint !e796*/ /* no possible out-of-bounds */
   }
   else
   {
      result = logtable[x];
   }

   return result;
}



/*===========================================================================

FUNCTION gl1_msgi_map_txlev_to_pwr_idx

DESCRIPTION
  This function maps the TXLEV based on RF band information to a power
  index used by the HW layer.
  NOTE: The power index is mapped in revers order from TXLEV.  It is
  also offset by the min TXLEV for the power class.  So for power class 4
  in GSM900 the power index is offset by 5 such that power index 0
  is TXLEV 5.


DEPENDENCIES

  arfcn       - input, channel to determine the RF band class so that the
                mapping is done correctly.
                NOTE: single ARFCN is sufficient since you can't hop from
                band to band in a message.
  txlev       - input, as specified in 3GPP specs (05 series)
  power_index - output, Index used by HW layer.

RETURN VALUE
  None

SIDE EFFECTS
  Internal YPF variable is updated by stores.

===========================================================================*/
void gl1_msgi_map_txlev_to_pwr_idx
(
  ARFCN_T     arfcn,
  const int16 txlev[],
  int16       power_index[],
  uint8       num_values,
  gas_id_t    gas_id
)
{
  uint8 i = 0;
  sys_band_T band;
  byte convert_band_index900[] = {14,18,16,14,12}; /* 33dbm,39,37,33,29 */
#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
  uint8 pwr_dBm = 0;
#endif
  band = arfcn.band;

  switch (band)
  {
    case SYS_BAND_EGSM_900:
    case SYS_BAND_PGSM_900:
    case SYS_BAND_CELL_850:
    /* Limit the PCL as per Power Class for each band */
    for (i = 0; i < num_values; i++)
    {
      if ( ( *(txlev + i)  < gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1] ) )
      {
        *(power_index + i) = convert_band_index900[gl1_rf_power_class_per_band[gas_id][band]];
      }
      else if( *(txlev + i) < 19 )
      {
        *(power_index + i) = 19 - *(txlev + i);
      }
      else /* txlev (19..31) */
      {
        *(power_index + i) = 0;                   /* Min dBm Power, 5dBm   */
      }
    }
    break;

    case SYS_BAND_DCS_1800:
    case SYS_BAND_PCS_1900:
    /* Power Class 1 Mobile, TXLEV = 0 ==> 30 dBm */
    for (i = 0; i < num_values; i++)
    {
      if( *(txlev + i) < 15 )
      {
        *(power_index + i) = 15 - *(txlev + i);
      }
      else if( *(txlev + i) < 29 )
      {
        *(power_index + i) = 0;                   /* Min dBm Power, 0dBm   */
      }
      else /* txlev (29,30,31) */
      {
        if ( gl1_rf_power_class_per_band[gas_id][band] == 0 ) /* class 1 */
          *(power_index + i) = 15;                /* Max dBm Power, 30 dBm */
        else if ( gl1_rf_power_class_per_band[gas_id][band] == 1 ) /* class 2 */
          *(power_index + i) = 12;                /* Max dBm Power, 24 dBm */
      }
    }
    break;

    default:
      MSG_GERAN_ERROR_0_G("Unsupported Band Class");
  }
#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
  l1_log_convert_txlev_to_pwr_dBm((eng_mode_sys_band_T)band, txlev[num_values-1], &pwr_dBm, gas_id);
  l1_physical_layer_power_data( UPLINK_POWER, pwr_dBm, gas_id );
#endif
  /*
  ** CgC New CMAPI feature for Tx power, need to log band used for next Tx
  */
  geran_eng_mode_data_write(ENG_MODE_TX_BAND, (void*)&band, TRUE,gas_id);
}


/*===========================================================================

FUNCTION gl1_msg_cap_reported_txlev

DESCRIPTION
  This function maps the ordered TXLEV to the reported TXLEV
  based on the power class.  Also TXLEV values that are mapped
  to the same outout power are capped to the smallest value
  in the range.   This only affect the reported TXLEV and not
  the tranmsitted power

DEPENDENCIES
  txlev - input, as specified in 3GPP specs (05 series)

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
int16 gl1_msg_cap_reported_power_level
(
  ARFCN_T     arfcn,
  int16 txlev,
  gas_id_t gas_id
)
{
  sys_band_T band;
  int16 reported_txlev = 0;

  band = arfcn.band;

  switch (band)
  {
    case SYS_BAND_EGSM_900:
    case SYS_BAND_PGSM_900:
    case SYS_BAND_CELL_850:
      /* Limit the PCL as per Power Class for each band */
      if ( txlev  < gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1] )
      {
         reported_txlev = gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1];
      }
      else if( txlev < 19 )
      {
        reported_txlev = txlev;
      }
      else /* txlev (19..31) */
      {
        reported_txlev = 19;     /* Min dBm Power, 5dBm   */
      }
    break;

    case SYS_BAND_DCS_1800:
    case SYS_BAND_PCS_1900:
    /* Power Class 1 Mobile, TXLEV = 0 ==> 30 dBm */
      if( txlev < 15 )
      {
        reported_txlev = txlev;
      }
      else if( txlev < 29 )
      {
        reported_txlev = 15;                  /* Min dBm Power, 0dBm   */
      }
      else /* txlev (29,30,31) */
      {
        if ( gl1_rf_power_class_per_band[gas_id][band] == 0 ) /* class 1 */
          reported_txlev = 0;                  /* Max dBm Power, 30dBm   */
        else if ( gl1_rf_power_class_per_band[gas_id][band] == 1 ) /* class 2 */
          reported_txlev = 3;                  /* Max dBm Power, 24dBm   */
      }
    break;

    default:
      MSG_GERAN_ERROR_0("Unsupported Band Class");
    break;
  }

  return ( reported_txlev );

}


/*===========================================================================

                         Radio link measurements
                         -----------------------
                             MEAN_BEP
                             CV_BEP

  See GSM 05.08 for requirements:
  - section 8.2 ... Signal quality
  - section 8.4 ... Measurement reporting

===========================================================================*/


/* Structure relating the SNR to the BEP */
typedef struct
{
   uint32 snr;  /* F16.16, Es/No */
   uint16 bep;  /* Q16 */
} snr_to_bep_lut_type;


/* 8PSK LUT OTA*/
static const snr_to_bep_lut_type  snr_to_bep_lut_8psk_ota[] =
{
   /*   SNR       BEP */
   {        0,    32767  },
   {     1000,    26214  },
   {    52057,    18960  },
   {    58409,    17618  },
   {    65536,    17398  },
   {    73533,    16871  },
   {    82505,    16614  },
   {   207243,    16614  },
   {   232531,    16282  },
   {   260904,    15981  },
   {   292739,    15366  },
   {   328458,    15089  },
   {   368536,    15089  },
   {   413504,    13456  },
   {   463959,    12805  },
   {   520571,    11077  },
   {   584090,    10138  },
   {   655360,     9094  },
   {   735326,     7931  },
   {   825049,     6689  },
   {   925721,     6259  },
   {  1038676,     5206  },
   {  1165413,     4748  },
   {  1307615,     4387  },
   {  1467168,     2426  },
   {  1646190,     1162  },
   {  1847055,      474  },
   {  2072430,      344  },
   {  2325305,      297  },
   {  2609035,      170  },
   {  2927386,       97  },
   {  3284581,       40  },
   {  3685360,       29  },
   {  4135042,       20  },
   {  4639593,       16  },
   {  5205710,        9  }
};

/* 8PSK LUT GCF for LCU*/
static const snr_to_bep_lut_type  snr_to_bep_lut_8psk_gcf[] =
{
   /*   SNR       BEP */
    {    1000,  26214},
    {   52057,  17618},
    {   58409,  17398},
    {   65536,  16871},
    {   73533,  16614},
    {   82505,  16282},
    {   92572,  15981},
    {  103868,  15366},
    {  116541,  15089},
    {  130762,  14694},
    {  146717,  14350},
    {  164619,  14100},
    {  184706,  13808},
    {  207243,  13728},
    {  232531,  13716},
    {  260904,  13456},
    {  292739,  12805},
    {  328458,  12000},
    {  368536,  10138},
    {  413504,   7584},
    {  463959,   7022},
    {  520571,   6553},
    {  584090,   6259},
    {  655360,   5972},
    {  735326,   4182},
    {  825049,   2072},
    {  925721,   1710},
    { 1038676,   1308},
    { 1165413,   1039},
    { 1307615,    757},
    { 1467168,    474},
    { 1646190,    297},
    { 1847055,    170},
    { 2072430,     97},
    { 2325305,     51},
    { 2609035,     29},
    { 2927386,     16},
    { 3284581,      9},
    { 3685360,      4},
    { 4135042,      3},
    { 4639593,      1},
    { 5205710,      0}
};

/* GMSK LUT OTA */
static const snr_to_bep_lut_type  snr_to_bep_lut_gmsk_ota[] =
{
   /*   SNR       BEP */
   {        0,    32767  },
   {     1000,    22938  },
   {    41350,     9737  },
   {    43801,     9184  },
   {    46396,     8948  },
   {    49145,     8564  },
   {    52057,     8357  },
   {    55142,     7938  },
   {    58409,     7601  },
   {    61870,     7172  },
   {    65536,     6817  },
   {    69419,     6451  },
   {    73533,     6039  },
   {    77890,     5650  },
   {    82505,     5183  },
   {    87394,     4729  },
   {    92572,     4288  },
   {    98057,     3851  },
   {   103868,     3373  },
   {   110022,     2875  },
   {   116541,     2478  },
   {   123447,     2028  },
   {   130762,     1672  },
   {   138510,     1384  },
   {   146717,     1122  },
   {   155410,      919  },
   {   164619,      738  },
   {   174373,      615  },
   {   184706,      508  },
   {   195650,      393  },
   {   207243,      322  },
   {   219523,      250  },
   {   232531,      192  },
   {   246309,      152  },
   {   260904,      125  },
   {   276363,      104  },
   {   292739,       89  },
   {   310084,       82  },
   {   328458,       64  },
   {   347920,       56  },
   {   520571,       35  },
   {   551417,       28  },
   {   584090,       17  },
   {   618700,       11  },
   {   655360,        8  },
   {   694193,        6  },
   {   735326,        4  },
   {   778897,        1  },
   {   825049,        0  }
};


/* GMSK LUT OTA (AEQ) */
static const snr_to_bep_lut_type  snr_to_bep_lut_gmsk_aeq_ota[] =
{
   /*   SNR       BEP */
  {   1000, 22938},
  {  65536, 25888},
  {  73533, 25034},
  {  82505, 24010},
  {  92572, 13141},
  { 103868, 10327},
  { 116541, 9184},
  { 130762, 5841},
  { 146717, 4484},
  { 164619, 3851},
  { 184706, 3179},
  { 207243, 1672},
  { 232531, 1122},
  { 260904, 738},
  { 292739, 508},
  { 328458, 322},
  { 368536, 192},
  { 413504, 129},
  { 463959, 81},
  { 520571, 43},
  { 584090, 23},
  { 655360, 17},
  { 735326, 12},
  { 825049, 7},
  { 925721, 3},
  {1038676, 1},
  {1165413, 0},
  {1307615, 0},
  {1467168, 0}

};

/* GMSK LUT GCF (AEQ) */
static const snr_to_bep_lut_type snr_to_bep_lut_gmsk_aeq_gcf[] =
{
  /* SNR    BEP */
  {1000,    30000},
  {65536,   29000},
  {73533,   27000},
  {82505,   26000},
  {92572,   25888},
  {103868,  25034},
  {116541,  24010},
  {130762,  13141},
  {146717,  10327},
  {164619,  9184},
  {184706,  5841},
  {207243,  4484},
  {232531,  3179},
  {260904,  1672},
  {292739,  1122},
  {328458,  901},
  {368536,  738},
  {413504,  508},
  {463959,  322},
  {520571,  192},
  {584090,  129},
  {655360,  81},
  {735326,  43},
  {825049,  23},
  {925721,  17},
  {1038676, 12},
  {1165413, 7},
  {1307615, 3},
  {1467168, 1}
};

#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
/* 8PSK LUT OTA*/
static const snr_to_bep_lut_type  snr_to_bep_lut_8psk_aeq_ota[] =
{
   /*   SNR       BEP */
{ 1000, 32767  },
{  52057, 16614  },
{  58409, 16614  },
{  65536, 16614  },
{  73533, 16614  },
{  82505, 16614  },
{  92572, 16614  },
{ 103868, 16614  },
{ 116541, 16614  },
{ 130762, 16614  },
{146717,  16614},
{164619,  16282},
{184706,  15981},
{207243,  15366},
{ 232531, 15089  },
{260904,  15089},
{292739,  13456},
{328458,  12805},
{368536,  11077},
{413504,  10138},
{463959,  7931},
{520571,  7186},
{584090,  6862},
{655360,  6259},
{735326,  5206},
{825049,  4387},
{925721,  2426},
{1038676, 1646},
{1165413, 788},
{1307615, 344},
{1467168, 170},
{1646190, 97},
{1847055, 60},
{2072430, 40},
{2325305, 29},
{2609035, 20},
{2927386, 17},
{3284581, 15},
{3685360, 10},
{4135042, 4},
{4639593, 1},
{5205710,     0  }
};

static const snr_to_bep_lut_type  snr_to_bep_lut_8psk_aeq_gcf[] =
{
   /*   SNR       BEP */
{1000 ,   32767},
{52057,   16614},
{58409,   16614},
{65536,   16614},
{73533,   16614},
{82505,   16614},
{92572,   16614},
{103868,  16614},
{116541,  16614},
{130762,  16282},
{146717,  15981},
{164619,  15366},
{184706,  15089},
{207243,  15089},
{232531,  13456},
{260904,  12805},
{292739,  11077},
{328458,  10138},
{368536,   7931},
{413504,   6689},
{463959,   6259},
{520571,   5546},
{584090,   5206},
{655360,   4748},
{735326,  4387},
{825049,  2426},
{925721,  1162},
{1038676, 788},
{1165413, 344},
{1307615, 170},
{1467168, 97},
{1646190, 40},
{1847055, 29},
{2072430, 20},
{2325305, 17},
{2609035, 15},
{2927386, 10},
{3284581, 4},
{3685360, 1},
{4135042, 0},
{4639593, 0},
{5205710, 0}
};
#endif /* FEATURE_GSM_MDSP_AEQ_8PSK */

/* GMSK LUT GCF for LCU*/
static const snr_to_bep_lut_type  snr_to_bep_lut_gmsk_gcf[] =
{
    /*   SNR       BEP */
   {        0,    32727  },
   {     1000,    22938  },
   {    41350,     9737  },
   {    43801,     9184  },
   {    46396,     8948  },
   {    49145,     8564  },
   {    52057,     8357  },
   {    55142,     7938  },
   {    58409,     7601  },
   {    61870,     7172  },
   {    65536,     6817  },
   {    69419,     6451  },
   {    73533,     6039  },
   {    77890,     5650  },
   {    82505,     5183  },
   {    87394,     4729  },
   {    92572,     4288  },
   {    98057,     3851  },
   {   103868,     3685  },
   {   110022,     3373  },
   {   116541,     2875  },
   {   123447,     2478  },
   {   130762,     2272  },
   {   138510,     1672  },
   {   146717,     1609  },
   {   155410,     1384  },
   {   164619,     1278  },
   {   174373,     1122  },
   {   184706,      919  },
   {   195650,      884  },
   {   207243,      738  },
   {   219523,      615  },
   {   232531,      508  },
   {   246309,      485  },
   {   260904,      393  },
   {   276363,      322  },
   {   292739,      250  },
   {   310084,      232  },
   {   328458,      192  },
   {   347920,      152  },
   {   368536,      125  },
   {   390373,       64  },
   {   413504,       35  },
   {   438006,       24  },
   {   463959,       17  },
   {   491451,       11  },
   {   520571,        6  },
   {   551417,        4  },
   {   584090,        1  }
};

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
/* GMSK LUT OTA (MSRD) */
static const snr_to_bep_lut_type  snr_to_bep_lut_gmsk_msrd_ota[] =
{
   /*   SNR       BEP */
  {   1000, 22938},
  {  65536, 25888},
  {  73533, 25034},
  {  82505, 24010},
  {  92572, 13141},
  { 103868, 10327},
  { 116541, 9184},
  { 130762, 5841},
  { 146717, 4484},
  { 164619, 3851},
  { 184706, 3179},
  { 207243, 1672},
  { 232531, 1122},
  { 260904, 738},
  { 292739, 508},
  { 328458, 322},
  { 368536, 192},
  { 413504, 129},
  { 463959, 81},
  { 520571, 43},
  { 584090, 23},
  { 655360, 17},
  { 735326, 12},
  { 825049, 7},
  { 925721, 3},
  {1038676, 1},
  {1165413, 0},
  {1307615, 0},
  {1467168, 0}

};

/* GMSK LUT GCF (MSRD) */
static const snr_to_bep_lut_type snr_to_bep_lut_gmsk_msrd_gcf[] =
{
  /* SNR    BEP */
  {1000,    30000},
  {65536,   29000},
  {73533,   27000},
  {82505,   26000},
  {92572,   25888},
  {103868,  25034},
  {116541,  24010},
  {130762,  13141},
  {146717,  10327},
  {164619,  9184},
  {184706,  5841},
  {207243,  4484},
  {232531,  3179},
  {260904,  1672},
  {292739,  1122},
  {328458,  901},
  {368536,  738},
  {413504,  508},
  {463959,  322},
  {520571,  192},
  {584090,  129},
  {655360,  81},
  {735326,  43},
  {825049,  23},
  {925721,  17},
  {1038676, 12},
  {1165413, 7},
  {1307615, 3},
  {1467168, 1}
};

/* 8PSK LUT OTA*/
static const snr_to_bep_lut_type  snr_to_bep_lut_8psk_msrd_ota[] =
{
   /*   SNR       BEP */
{ 1000, 32767  },
{  52057, 16614  },
{  58409, 16614  },
{  65536, 16614  },
{  73533, 16614  },
{  82505, 16614  },
{  92572, 16614  },
{ 103868, 16614  },
{ 116541, 16614  },
{ 130762, 16614  },
{146717,  16614},
{164619,  16282},
{184706,  15981},
{207243,  15366},
{ 232531, 15089  },
{260904,  15089},
{292739,  13456},
{328458,  12805},
{368536,  11077},
{413504,  10138},
{463959,  7931},
{520571,  7186},
{584090,  6862},
{655360,  6259},
{735326,  5206},
{825049,  4387},
{925721,  2426},
{1038676, 1646},
{1165413, 788},
{1307615, 344},
{1467168, 170},
{1646190, 97},
{1847055, 60},
{2072430, 40},
{2325305, 29},
{2609035, 20},
{2927386, 17},
{3284581, 15},
{3685360, 10},
{4135042, 4},
{4639593, 1},
{5205710,     0  }
};

static const snr_to_bep_lut_type  snr_to_bep_lut_8psk_msrd_gcf[] =
{
   /*   SNR       BEP */
{1000 ,   32767},
{52057,   16614},
{58409,   16614},
{65536,   16614},
{73533,   16614},
{82505,   16614},
{92572,   16614},
{103868,  16614},
{116541,  16614},
{130762,  16282},
{146717,  15981},
{164619,  15366},
{184706,  15089},
{207243,  15089},
{232531,  13456},
{260904,  12805},
{292739,  11077},
{328458,  10138},
{368536,   7931},
{413504,   6689},
{463959,   6259},
{520571,   5546},
{584090,   5206},
{655360,   4748},
{735326,  4387},
{825049,  2426},
{925721,  1162},
{1038676, 788},
{1165413, 344},
{1307615, 170},
{1467168, 97},
{1646190, 40},
{1847055, 29},
{2072430, 20},
{2325305, 17},
{2609035, 15},
{2927386, 10},
{3284581, 4},
{3685360, 1},
{4135042, 0},
{4639593, 0},
{5205710, 0}
};
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */


static const snr_to_bep_lut_type snr_to_bep_lut_voice_aeq[] =
{
  /* SNR    BEP */
  {1000,    30000},
  {65536,   29000},
  {73533,   27000},
  {82505,   26000},
  {92572,   25888},
  {103868,  25034},
  {116541,  24010},
  {130762,  13141},
  {146717,  10327},
  {164619,  9184},
  {184706,  5841},
  {207243,  4484},
  {232531,  3179},
  {260904,  1672},
  {292739,  1122},
  {328458,  901},
  {368536,  738},
  {413504,  508},
  {463959,  322},
  {520571,  93},
  {584090,  58},
  {655360,  38},
  {735326,  22},
  {825049,  12},
  {925721,  8},
  {1038676, 5},
  {1165413, 2},
  {1307615, 1},
  {1467168, 0}
};

/* Definition of SNR threhold for switching between pre-mimo and burst SNR for TCH BEP*/
#define BEP_TABLE_SNR_THRESHOLD     500

/* 8PSK LUT sizes */
#define SIZE_SNR_TO_BEP_LUT_8PSK_OTA \
  (sizeof(snr_to_bep_lut_8psk_ota) / sizeof(snr_to_bep_lut_type))

#define SIZE_SNR_TO_BEP_LUT_8PSK_GCF \
  (sizeof(snr_to_bep_lut_8psk_gcf) / sizeof(snr_to_bep_lut_type))

/* GMSK LUT sizes */
#define SIZE_SNR_TO_BEP_LUT_GMSK_OTA \
  (sizeof(snr_to_bep_lut_gmsk_ota) / sizeof(snr_to_bep_lut_type))

#define SIZE_SNR_TO_BEP_LUT_GMSK_AEQ_OTA \
  (sizeof(snr_to_bep_lut_gmsk_aeq_ota) / sizeof(snr_to_bep_lut_type))

#define SIZE_SNR_TO_BEP_LUT_GMSK_AEQ_GCF \
  (sizeof(snr_to_bep_lut_gmsk_aeq_gcf) / sizeof(snr_to_bep_lut_type))

#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
#define SIZE_SNR_TO_BEP_LUT_8PSK_AEQ_OTA \
  (sizeof(snr_to_bep_lut_8psk_aeq_ota) / sizeof(snr_to_bep_lut_type))

#define SIZE_SNR_TO_BEP_LUT_8PSK_AEQ_GCF \
  (sizeof(snr_to_bep_lut_8psk_aeq_gcf) / sizeof(snr_to_bep_lut_type))
#endif /* FEATURE_GSM_MDSP_AEQ_8PSK */

#define SIZE_SNR_TO_BEP_LUT_GMSK_GCF \
  (sizeof(snr_to_bep_lut_gmsk_gcf) / sizeof(snr_to_bep_lut_type))

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
#define SIZE_SNR_TO_BEP_LUT_GMSK_MSRD_OTA \
  (sizeof(snr_to_bep_lut_gmsk_msrd_ota) / sizeof(snr_to_bep_lut_type))

#define SIZE_SNR_TO_BEP_LUT_GMSK_MSRD_GCF \
  (sizeof(snr_to_bep_lut_gmsk_msrd_gcf) / sizeof(snr_to_bep_lut_type))

#define SIZE_SNR_TO_BEP_LUT_8PSK_MSRD_OTA \
  (sizeof(snr_to_bep_lut_8psk_msrd_ota) / sizeof(snr_to_bep_lut_type))

#define SIZE_SNR_TO_BEP_LUT_8PSK_MSRD_GCF \
  (sizeof(snr_to_bep_lut_8psk_msrd_gcf) / sizeof(snr_to_bep_lut_type))
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */

#define SIZE_SNR_TO_BEP_LUT_VOICE_AEQ \
  (sizeof(snr_to_bep_lut_voice_aeq) / sizeof(snr_to_bep_lut_type))
/* These two are setup when the gcf information is provided
 * Initialise them to valid values for now.
 */
static uint32  gl1_msg_snr_bep_lut_gmsk_size = SIZE_SNR_TO_BEP_LUT_8PSK_OTA;
static uint32  gl1_msg_snr_bep_lut_8psk_size = SIZE_SNR_TO_BEP_LUT_GMSK_OTA;

/* Make sure the pointers are never NULL */
const snr_to_bep_lut_type*  snr_to_bep_lut_gmsk_ptr = &( snr_to_bep_lut_gmsk_ota[0] );
const snr_to_bep_lut_type*  snr_to_bep_lut_8psk_ptr = &( snr_to_bep_lut_8psk_ota[0] );

/*===========================================================================

FUNCTION gl1_msg_tch_snr_to_bep

DESCRIPTION
  Converts an SNR value for TCH into BEP.

===========================================================================*/
uint16 gl1_msg_tch_snr_to_bep ( gl1_defs_metrics_rpt *metrics_ptr ,gas_id_t gas_id)
{
   uint16 bep;
   /* If SAIC active then use pre_mimo SNR when SNR is very low */
   if( (gl1_msg_saic_in_burst( metrics_ptr->misc_flags )) && (metrics_ptr->snr < BEP_TABLE_SNR_THRESHOLD) )
   {
     bep = gl1_msg_snr_to_bep(metrics_ptr->pre_mimo_trained_complete_snr, GL1_DEFS_MODULATION_GMSK, TRUE, gas_id);
   }
   else
   {
     bep = gl1_msg_snr_to_bep(metrics_ptr->snr, GL1_DEFS_MODULATION_GMSK, TRUE,gas_id);
   }
   return bep;

}
/*===========================================================================

FUNCTION gl1_msg_snr_to_bep

DESCRIPTION
  Converts an SNR value for a given modulation scheme in the BEP.

===========================================================================*/
uint16 gl1_msg_snr_to_bep
(
   uint16 snr,
   gl1_defs_modulation_type mod,
   boolean is_tch,
   gas_id_t gas_id
)
{
   uint16 i = 0;
   int32 snr_diff, bep_diff, adj, bep, round;
   uint32 snr32, size;
   const snr_to_bep_lut_type *snr_to_bep_lut;
   /* Setup the ota / gcf pointers and sizes for both modulation schemes */
   if( FALSE == mcfg_gcf_nv_get_status())
   {
     /* Set the OTA parameter values */
     gl1_msg_snr_bep_lut_gmsk_size = SIZE_SNR_TO_BEP_LUT_GMSK_OTA;
     gl1_msg_snr_bep_lut_8psk_size = SIZE_SNR_TO_BEP_LUT_8PSK_OTA;

     snr_to_bep_lut_gmsk_ptr = &( snr_to_bep_lut_gmsk_ota[0] );
     snr_to_bep_lut_8psk_ptr = &( snr_to_bep_lut_8psk_ota[0] );
   }
   else
   {
     /* Set the GCF parameter values */
     gl1_msg_snr_bep_lut_gmsk_size = SIZE_SNR_TO_BEP_LUT_GMSK_GCF;
     gl1_msg_snr_bep_lut_8psk_size = SIZE_SNR_TO_BEP_LUT_8PSK_GCF;

     snr_to_bep_lut_gmsk_ptr = &( snr_to_bep_lut_gmsk_gcf[0] );
     snr_to_bep_lut_8psk_ptr = &( snr_to_bep_lut_8psk_gcf[0] );
   }
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
   if(gl1_get_MSRD_ctl_nv(gas_id) && gl1_get_enable_rxdiversity_data(gas_id))
   {
     if( FALSE == mcfg_gcf_nv_get_status())
     {
         /* pick up the correct table here for MSRD GMSK OTA */
         snr_to_bep_lut_gmsk_ptr = &( snr_to_bep_lut_gmsk_msrd_ota[0] );
         gl1_msg_snr_bep_lut_gmsk_size = SIZE_SNR_TO_BEP_LUT_GMSK_MSRD_OTA;
     }
     else
     {
         /* pick up the correct table here for MSRD GMSK GCF */
         snr_to_bep_lut_gmsk_ptr = &( snr_to_bep_lut_gmsk_msrd_gcf[0] );
         gl1_msg_snr_bep_lut_gmsk_size = SIZE_SNR_TO_BEP_LUT_GMSK_MSRD_GCF;
     }

     MSG_GERAN_LOW_0_G("Overloading the BEP table for MSRD GMSK");
   }
   else 
#endif
   if ( !gl1_hw_feature_disabled_aeq(gas_id) )
   {
     if( FALSE == mcfg_gcf_nv_get_status())
     {
         /* pick up the correct table here for AEQ GMSK OTA */
         snr_to_bep_lut_gmsk_ptr = &( snr_to_bep_lut_gmsk_aeq_ota[0] );
         gl1_msg_snr_bep_lut_gmsk_size = SIZE_SNR_TO_BEP_LUT_GMSK_AEQ_OTA;
     }
     else
     {
         /* pick up the correct table here for AEQ GMSK GCF */
         snr_to_bep_lut_gmsk_ptr = &( snr_to_bep_lut_gmsk_aeq_gcf[0] );
         gl1_msg_snr_bep_lut_gmsk_size = SIZE_SNR_TO_BEP_LUT_GMSK_AEQ_GCF;
     }

     MSG_GERAN_LOW_0_G("Overloading the BEP table for AEQ PH1 GMSK");
   }
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
   if(gl1_get_MSRD_ctl_nv(gas_id) && gl1_get_enable_rxdiversity_data(gas_id))
   {
     if( FALSE == mcfg_gcf_nv_get_status())
     {
         /* pick up the correct table here for MSRD 8PSK OTA */
         snr_to_bep_lut_8psk_ptr = &( snr_to_bep_lut_8psk_msrd_ota[0] );
         gl1_msg_snr_bep_lut_8psk_size = SIZE_SNR_TO_BEP_LUT_8PSK_MSRD_OTA;
     }
     else
     {
         /* pick up the correct table here for MSRD 8PSK GCF */
         snr_to_bep_lut_8psk_ptr = &( snr_to_bep_lut_8psk_msrd_gcf[0] );
         gl1_msg_snr_bep_lut_8psk_size = SIZE_SNR_TO_BEP_LUT_8PSK_MSRD_GCF;
     }

     MSG_GERAN_LOW_0_G("Overloading the BEP table for MSRD 8PSK");
   }
   else 
#endif
#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
   if ( !gl1_hw_feature_disabled_aeq_8psk(gas_id)
#ifdef FEATURE_GSM_DYNAMIC_AEQ_8PSK
        && (mdsp_get_aeq_8psk_status(gas_id))
#endif
        )
   {
     if( FALSE == mcfg_gcf_nv_get_status())
     {
         /* pick up the correct table here for AEQ GMSK OTA */
         snr_to_bep_lut_8psk_ptr = &( snr_to_bep_lut_8psk_aeq_ota[0] );
         gl1_msg_snr_bep_lut_8psk_size = SIZE_SNR_TO_BEP_LUT_8PSK_AEQ_OTA;
     }
     else
     {
         /* pick up the correct table here for AEQ GMSK GCF */
         snr_to_bep_lut_8psk_ptr = &( snr_to_bep_lut_8psk_aeq_gcf[0] );
         gl1_msg_snr_bep_lut_8psk_size = SIZE_SNR_TO_BEP_LUT_8PSK_AEQ_GCF;
     }
     MSG_GERAN_LOW_0_G("Overloading the BEP table for AEQ PH2 8PSK");
   }
#endif /* FEATURE_GSM_MDSP_AEQ_8PSK */

   if(is_tch)
   {
     /* pick up the correct table here for AEQ GMSK OTA regardless of GCF flag is set or not*/
     snr_to_bep_lut_gmsk_ptr = &( snr_to_bep_lut_voice_aeq[0] );
     gl1_msg_snr_bep_lut_gmsk_size = SIZE_SNR_TO_BEP_LUT_VOICE_AEQ;

     MSG_GERAN_LOW_0_G("Overloading the BEP table for TCH");
   }

   /* Select the LUT to use and convert bit SNR in Q8 to symbol SNR in Q16 */
   switch (mod)
   {
      case GL1_DEFS_MODULATION_GMSK:
         snr_to_bep_lut = snr_to_bep_lut_gmsk_ptr;
         size = gl1_msg_snr_bep_lut_gmsk_size;
         snr32 = snr << 8;
         break;

      case GL1_DEFS_MODULATION_8PSK:
         snr_to_bep_lut = snr_to_bep_lut_8psk_ptr;
         size = gl1_msg_snr_bep_lut_8psk_size;
         snr32 = (snr * 3) << 8;
         break;

      default:
         MSG_GERAN_ERROR_1_G("Invalid modulation %d", mod);
         return 0;
   }

   /* Check for overflow */
   if (snr32 > snr_to_bep_lut[size - 1].snr)  /*lint !e661 !e662*/
   {
      return 0;
   }

   /* Find the SNR - assumes the first entry has SNR=0 */
   /* Additional bounds check */
   for (i = 0; (i <= size - 2) && (snr32 > snr_to_bep_lut[i + 1].snr); i++)
   {
      ;
   }

   if (i > size-2)
   {
      /*  clip to max; and print an Error  */
      MSG_GERAN_ERROR_2_G("snr to bep size :%u;clamped to :%u", i, size-2);
      i = (uint16)size - 2;
   }

   /* Do a linear interpolation */
   snr_diff = (int32)(snr_to_bep_lut[i + 1].snr - snr_to_bep_lut[i].snr);
   bep_diff = (int32)(snr_to_bep_lut[i].bep - snr_to_bep_lut[i + 1].bep);

   /* Calculate the rounding term and the final interpolated adjustment */
   round = (bep_diff >= 0 ? snr_diff / 2 : -snr_diff / 2);
   adj = ((int32)(snr32 - snr_to_bep_lut[i].snr) * bep_diff);
   adj = (adj + round) / snr_diff;
   bep = (int32)snr_to_bep_lut[i].bep - adj;

   /* The compiler seems to get confused if the above math is moved around
      so we double-check that everything is sane */
   if (bep < 0)
   {
      MSG_GERAN_ERROR_2_G("BEP: %d < 0 (snr=%d)!", bep,snr);
      bep = 0;
   }

   return (uint16)bep;
}



/* Values of the coefficient of variation of
 * error probability of a radio block in F6.10
 * format.
 */
#define  CV_BEP_0  1792  /* 1.75  */
#define  CV_BEP_1  1536  /* 1.50  */
#define  CV_BEP_2  1280  /* 1.25  */
#define  CV_BEP_3  1024  /* 1.00  */
#define  CV_BEP_4   768  /* 0.75  */
#define  CV_BEP_5   512  /* 0.50  */
#define  CV_BEP_6   256  /* 0.25  */
#define  CV_BEP_7     0  /* 0.0   */

/* Provides a look up table to map cv bep values onto a scale.
 * Used for both GMSK and 8PSK coding schemes
 * Values stored in F6.10 format.
 */
static const uint16 CV_BEP_SCALE[]=
{
    CV_BEP_0, CV_BEP_1, CV_BEP_2, CV_BEP_3, CV_BEP_4, CV_BEP_5, CV_BEP_6,
    CV_BEP_7
};


/*                     GMSK MODULATION
 *
 *       ETSI MEAN BEP    Value    Percentage    Log10( BEP)
 *                         in       value
 *                        1/65535     %
 *                        units
 */
#define  MEAN_BEP_GMSK_0     16462   /* 25.119        -0.6  */
#define  MEAN_BEP_GMSK_1     13077   /* 19.953        -0.7  */
#define  MEAN_BEP_GMSK_2     10387   /* 15.849        -0.8  */
#define  MEAN_BEP_GMSK_3      8251   /* 12.589        -0.9  */
#define  MEAN_BEP_GMSK_4      6554   /* 10.000        -1.0  */
#define  MEAN_BEP_GMSK_5      5206   /*  7.943        -1.1  */
#define  MEAN_BEP_GMSK_6      4136   /*  6.310        -1.2  */
#define  MEAN_BEP_GMSK_7      3285   /*  5.012        -1.3  */
#define  MEAN_BEP_GMSK_8      2610   /*  3.981        -1.4  */
#define  MEAN_BEP_GMSK_9      2073   /*  3.162        -1.5  */
#define  MEAN_BEP_GMSK_10     1647   /*  2.512        -1.6  */
#define  MEAN_BEP_GMSK_11     1308   /*  1.995        -1.7  */
#define  MEAN_BEP_GMSK_12     1039   /*  1.585        -1.8  */
#define  MEAN_BEP_GMSK_13      826   /*  1.259        -1.9  */
#define  MEAN_BEP_GMSK_14      656   /*  1.000        -2.0  */
#define  MEAN_BEP_GMSK_15      521   /*  0.794        -2.1  */
#define  MEAN_BEP_GMSK_16      414   /*  0.630        -2.2  */
#define  MEAN_BEP_GMSK_17      329   /*  0.501        -2.3  */
#define  MEAN_BEP_GMSK_18      261   /*  0.398        -2.4  */
#define  MEAN_BEP_GMSK_19      208   /*  0.316        -2.5  */
#define  MEAN_BEP_GMSK_20      165   /*  0.251        -2.6  */
#define  MEAN_BEP_GMSK_21      131   /*  0.200        -2.7  */
#define  MEAN_BEP_GMSK_22      104   /*  0.158        -2.8  */
#define  MEAN_BEP_GMSK_23       83   /*  0.126        -2.9  */
#define  MEAN_BEP_GMSK_24       66   /*  0.100        -3.0  */
#define  MEAN_BEP_GMSK_25       53   /*  0.079        -3.1  */
#define  MEAN_BEP_GMSK_26       42   /*  0.063        -3.2  */
#define  MEAN_BEP_GMSK_27       33   /*  0.050        -3.3  */
#define  MEAN_BEP_GMSK_28       27   /*  0.040        -3.4  */
#define  MEAN_BEP_GMSK_29       21   /*  0.032        -3.5  */
#define  MEAN_BEP_GMSK_30       17   /*  0.025        -3.6  */
#define  MEAN_BEP_GMSK_31       0    /*  0.020       < -3.6 */



/* Provides a look up table to map mean bep values onto a log scale
 * Used for the GMSK modulation scheme.
 */
static const uint16  MEAN_BEP_LOG_SCALE_GMSK[]=
{
    MEAN_BEP_GMSK_0 , MEAN_BEP_GMSK_1 , MEAN_BEP_GMSK_2 , MEAN_BEP_GMSK_3 ,
    MEAN_BEP_GMSK_4 , MEAN_BEP_GMSK_5 , MEAN_BEP_GMSK_6 , MEAN_BEP_GMSK_7 ,
    MEAN_BEP_GMSK_8 , MEAN_BEP_GMSK_9 , MEAN_BEP_GMSK_10, MEAN_BEP_GMSK_11,
    MEAN_BEP_GMSK_12, MEAN_BEP_GMSK_13, MEAN_BEP_GMSK_14, MEAN_BEP_GMSK_15,
    MEAN_BEP_GMSK_16, MEAN_BEP_GMSK_17, MEAN_BEP_GMSK_18, MEAN_BEP_GMSK_19,
    MEAN_BEP_GMSK_20, MEAN_BEP_GMSK_21, MEAN_BEP_GMSK_22, MEAN_BEP_GMSK_23,
    MEAN_BEP_GMSK_24, MEAN_BEP_GMSK_25, MEAN_BEP_GMSK_26, MEAN_BEP_GMSK_27,
    MEAN_BEP_GMSK_28, MEAN_BEP_GMSK_29, MEAN_BEP_GMSK_30, MEAN_BEP_GMSK_31
};


/*                     8PSK MODULATION
 *
 *       ETSI MEAN BEP    Value    Percentage    Log10( BEP)
 *                         in       value
 *                        1/65535     %
 *                        units
 */
#define  MEAN_BEP_8PSK_0    16462  /* 25.119        -0.60  */
#define  MEAN_BEP_8PSK_1    15014  /* 22.909        -0.64  */
#define  MEAN_BEP_8PSK_2    13693  /* 20.893        -0.68  */
#define  MEAN_BEP_8PSK_3    12488  /* 19.055        -0.72  */
#define  MEAN_BEP_8PSK_4    11389  /* 17.378        -0.76  */
#define  MEAN_BEP_8PSK_5    10387  /* 15.849        -0.80  */
#define  MEAN_BEP_8PSK_6     9473  /* 14.454        -0.84  */
#define  MEAN_BEP_8PSK_7     8640  /* 13.183        -0.88  */
#define  MEAN_BEP_8PSK_8     7880  /* 12.023        -0.92  */
#define  MEAN_BEP_8PSK_9     7186  /* 10.965        -0.96  */
#define  MEAN_BEP_8PSK_10    6554  /* 10.000        -1.00  */
#define  MEAN_BEP_8PSK_11    5977  /*  9.120        -1.04  */
#define  MEAN_BEP_8PSK_12    5452  /*  8.318        -1.08  */
#define  MEAN_BEP_8PSK_13    4972  /*  7.586        -1.12  */
#define  MEAN_BEP_8PSK_14    4534  /*  6.918        -1.16  */
#define  MEAN_BEP_8PSK_15    4136  /*  6.310        -1.20  */
#define  MEAN_BEP_8PSK_16    2861  /*  4.365        -1.36  */
#define  MEAN_BEP_8PSK_17    1980  /*  3.020        -1.52  */
#define  MEAN_BEP_8PSK_18    1370  /*  2.089        -1.68  */
#define  MEAN_BEP_8PSK_19     948  /*  1.445        -1.84  */
#define  MEAN_BEP_8PSK_20     656  /*  1.000        -2.00  */
#define  MEAN_BEP_8PSK_21     454  /*  0.692        -2.16  */
#define  MEAN_BEP_8PSK_22     314  /*  0.479        -2.32  */
#define  MEAN_BEP_8PSK_23     218  /*  0.331        -2.48  */
#define  MEAN_BEP_8PSK_24     151  /*  0.230        -2.64  */
#define  MEAN_BEP_8PSK_25     104  /*  0.158        -2.80  */
#define  MEAN_BEP_8PSK_26      72  /*  0.110        -2.96  */
#define  MEAN_BEP_8PSK_27      50  /*  0.076        -3.12  */
#define  MEAN_BEP_8PSK_28      35  /*  0.052        -3.28  */
#define  MEAN_BEP_8PSK_29      24  /*  0.036        -3.44  */
#define  MEAN_BEP_8PSK_30      17  /*  0.025        -3.60  */
#define  MEAN_BEP_8PSK_31       0  /*  0.020      < -3.60  */



/* Provides a look up table to map mean bep values onto a log scale
 * Used for the 8PSK modulation scheme.
 */
static const uint16  MEAN_BEP_LOG_SCALE_8PSK[]=
{
    MEAN_BEP_8PSK_0 , MEAN_BEP_8PSK_1 , MEAN_BEP_8PSK_2 , MEAN_BEP_8PSK_3 ,
    MEAN_BEP_8PSK_4 , MEAN_BEP_8PSK_5 , MEAN_BEP_8PSK_6 , MEAN_BEP_8PSK_7 ,
    MEAN_BEP_8PSK_8 , MEAN_BEP_8PSK_9 , MEAN_BEP_8PSK_10, MEAN_BEP_8PSK_11,
    MEAN_BEP_8PSK_12, MEAN_BEP_8PSK_13, MEAN_BEP_8PSK_14, MEAN_BEP_8PSK_15,
    MEAN_BEP_8PSK_16, MEAN_BEP_8PSK_17, MEAN_BEP_8PSK_18, MEAN_BEP_8PSK_19,
    MEAN_BEP_8PSK_20, MEAN_BEP_8PSK_21, MEAN_BEP_8PSK_22, MEAN_BEP_8PSK_23,
    MEAN_BEP_8PSK_24, MEAN_BEP_8PSK_25, MEAN_BEP_8PSK_26, MEAN_BEP_8PSK_27,
    MEAN_BEP_8PSK_28, MEAN_BEP_8PSK_29, MEAN_BEP_8PSK_30, MEAN_BEP_8PSK_31,
};

/* Use in calculating the CV BEP value */
#define SQRT_3_Q14     28378U  /* sqrt(3) << 14 */



/*===========================================================================

FUNCTION  square_root

DESCRIPTION
  Local function used to find the square root of an unsigned 32 bit integer.
  It is required to calculate the BEP variance.
  square_root40 is used now and computes the square root of a 40 bit
  integer - it is much much slower and perhaps could be optimized.

===========================================================================*/
static uint32 square_root (uint32 integer)
{
   uint32 root, v;

   root = 0;
   for (v = 0x40000000; v > 0; v >>= 2)
   {
      if (v + root <= integer)
      {
         integer -= (v + root);
         root = (root >> 1) | v;
      }
      else
      {
         root = (root >> 1);
      }
   }

   /* Round to the nearest integer, cuts max error in half */
   if (root < integer) root++;

   return root;
}



/*===========================================================================

FUNCTION  calc_meas_scale

DESCRIPTION
  Local function used to scale a measurement according to the supplied
  array pointer.  The array should be in descending order with the last
  element being zero.

===========================================================================*/
static uint8 calc_meas_scale
(
   const uint16 *scale_ptr,
   uint32        target_measurement
)
{
    uint8  reported_measurement = 0;

    while( (*scale_ptr) > target_measurement )
    {
        scale_ptr++;
        reported_measurement++;
    }
    return reported_measurement;
}


/*===========================================================================

FUNCTION  gl1_msg_calc_mean_bep_gmsk

DESCRIPTION
  Takes a mean GMSK BEP value in Q16 format and returns the MEAN_BEP
  index.

===========================================================================*/
uint8 gl1_msg_calc_mean_bep_gmsk (uint32 mean_bep)
{
   return calc_meas_scale(MEAN_BEP_LOG_SCALE_GMSK, mean_bep);
}

/*===========================================================================

FUNCTION  gl1_msg_calc_mean_bep_8psk

DESCRIPTION
  Takes a mean GMSK BEP value in Q16 format and returns the
  MEAN_BEP index

===========================================================================*/
uint8 gl1_msg_calc_mean_bep_8psk (uint32 mean_bep)
{
   return calc_meas_scale(MEAN_BEP_LOG_SCALE_8PSK, mean_bep);
}

#if defined (FEATURE_GSM_DYNAMIC_AEQ_8PSK) || defined (FEATURE_GSM_RX_DIVERSITY_DATA)
#define GL1_AEQ_8PSK_ON_BEP_THRESHOLD  15                              /* 15 => 0.023% BER */
#define GL1_AEQ_8PSK_OFF_BEP_THRESHOLD GL1_SIZEOF_MEAN_BEP_48_BLKS*6  /* 10 => 0.015% BER */
#define GL1_DATA_RXD_ON_BEP_THRESHOLD  23                              /* 23 => 0.0xx% BER */
#define GL1_DATA_RXD_OFF_BEP_THRESHOLD GL1_SIZEOF_MEAN_BEP_48_BLKS*6  /* 10 => 0.015% BER */
/*===========================================================================

FUNCTION  gl1_msg_calc_aeq_8psk_mean_bep

DESCRIPTION
  Takes BEP values for 1 block in Q16 format and calculates the mean BEP over
  the block (4 bursts). Every 4 blocks, the mean BEP over these blocks is copied
  into a larger mean BEP buffer holding mean BEP per 4 block resust over the
  last 24 blocks. The mean BEP per 4 blocks and 24 blockss is then used to determine
  if AEQ PH2 should be used for the next Rx block.

===========================================================================*/
void gl1_msg_calc_aeq_8psk_mean_bep (uint16  *src_bep_ptr,
                                     boolean *schedule_error_ptr,
                                     gas_id_t gas_id)
{

   uint16 i;
   uint32 mean_bep_per_block = 0;
   static boolean delayed_aeq_speed_bump_disable[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };
   gl1_rxd_control_type gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);
   boolean    gl1_in_ftm_mode = (ftm_get_mode() == FTM_MODE)? TRUE:FALSE;

   /* Calculate mean BEP per block (4 bursts) */
   for (i=0; i<4; i++)
   {
     if(*(schedule_error_ptr - i) == FALSE)
     {
       mean_bep_per_block += *(src_bep_ptr - i);
     }
     else
     {
       MSG_GERAN_HIGH_1_G("AEQ/RxD dynamic switching: block ignored, schedule_error %d",*(schedule_error_ptr - i));
       return;
     }
   }
#ifdef FEATURE_GSM_DYNAMIC_AEQ_RXD_DEBUG
   MSG_GERAN_HIGH_2_G("mean_bep_per_block=%d, mean_bep_params[gas_id].mean_bep_per_4_blks_index=%d",mean_bep_per_block>>2,mean_bep_params[gas_id].mean_bep_per_4_blks_index);
#endif

   /* Store mean BEP per block in 4 block buffer (div by 4 to get back to Q16 format */
   mean_bep_params[gas_id].mean_bep_per_4_blks[mean_bep_params[gas_id].mean_bep_per_4_blks_index] = (uint16)((mean_bep_per_block+2)>>2);
   /* If mean BEP for 4 blocks now stored, reset index and store mean per over 4 blocks in 48 block buffer */
   if( ++mean_bep_params[gas_id].mean_bep_per_4_blks_index > (GL1_SIZEOF_MEAN_BEP_4_BLKS-1) )
   {
     mean_bep_params[gas_id].mean_bep_per_4_blks_index = 0;
     mean_bep_per_block = 0;

     /* Calculate mean BEP over 4 blocks */
     for (i=0; i<4; i++)
     {
       mean_bep_per_block += mean_bep_params[gas_id].mean_bep_per_4_blks[i];
     }
#ifdef FEATURE_GSM_DYNAMIC_AEQ_RXD_DEBUG
     MSG_GERAN_HIGH_2_G("mean_bep_per_4_blocks=%d, mean_bep_params[gas_id].mean_bep_per_48_blks_index=%d",mean_bep_per_block>>2,mean_bep_params[gas_id].mean_bep_per_48_blks_index);
#endif
     /* Store mean BEP per 4 blocks (div by 2 to get back to Q16 format) */
     mean_bep_params[gas_id].mean_bep_per_48_blks[mean_bep_params[gas_id].mean_bep_per_48_blks_index] = (uint16)((mean_bep_per_block+2)>>2);

#if defined (FEATURE_GSM_DYNAMIC_AEQ_8PSK)
     if(mean_bep_params[gas_id].bep_averaging_aeq_off_count < GL1_SIZEOF_MEAN_BEP_48_BLKS)
     {
       mean_bep_params[gas_id].bep_averaging_aeq_off_count++;
     }
     /* Only dynamically switch AEQ PH2 if enabled in NV */
     if (!gl1_hw_feature_disabled_aeq_8psk(gas_id))
     {
       if( (gl1_rxd_ctl_flag.RxdForcedOn &&(gl1_in_ftm_mode) && !mdsp_get_aeq_8psk_status(gas_id)))
       {
          mdsp_enable_aeq_8psk_dynamic(TRUE ,gas_id);
#ifdef FEATURE_GSM_DYNAMIC_AEQ_RXD_DEBUG
          MSG_GERAN_HIGH_1_G("Enabling AEQ: gl1_rxd_ctl_flag.RxdForcedOn=%d", gl1_rxd_ctl_flag.RxdForcedOn);
#endif  
          /* increase speed for EGPRS mode */
          g11_hw_egprs_mcpm_change(TRUE,gas_id);
          mean_bep_params[gas_id].bep_averaging_aeq_off_count = 0;
          delayed_aeq_speed_bump_disable[gas_id] = FALSE;  
       }
       else if (!mdsp_get_aeq_8psk_status(gas_id))
       {
         /* This is actually checking mean BEP per 4 blocks, which is stored in each of the
            6 mean_bep_params[gas_id].mean_bep_per_48_blks bins (15 => 0.023% BER). If mean BEP over 4 DL blocks > 0.023%
            switch AEQ PH2 back on */
         if(mean_bep_params[gas_id].mean_bep_per_48_blks[mean_bep_params[gas_id].mean_bep_per_48_blks_index] > GL1_AEQ_8PSK_ON_BEP_THRESHOLD)
         {
           mdsp_enable_aeq_8psk_dynamic(TRUE ,gas_id);
#ifdef FEATURE_GSM_DYNAMIC_AEQ_RXD_DEBUG
           MSG_GERAN_HIGH_1_G("Enabling AEQ: mean_bep_per_48_blocks=%d",mean_bep_params[gas_id].mean_bep_per_48_blks[mean_bep_params[gas_id].mean_bep_per_48_blks_index]);
#endif
           /* increase speed for EGPRS mode */
           g11_hw_egprs_mcpm_change(TRUE,gas_id);
           mean_bep_params[gas_id].bep_averaging_aeq_off_count = 0;
           delayed_aeq_speed_bump_disable[gas_id] = FALSE;

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
           gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_HIGH_BLER);
#endif

         }
         else
         {
           if (delayed_aeq_speed_bump_disable[gas_id] == TRUE)
           {
              delayed_aeq_speed_bump_disable[gas_id] = FALSE;
              /* decrease speed for EGPRS mode */
              g11_hw_egprs_mcpm_change(FALSE,gas_id);
           }
         }
       }
       else
       {
         mean_bep_per_block = 0;
         /* Calculate mean BEP over 48 (6*4) blocks in Q16 */
         for(i=0; i<GL1_SIZEOF_MEAN_BEP_48_BLKS; i++)
         {
           mean_bep_per_block += mean_bep_params[gas_id].mean_bep_per_48_blks[i];
         }

         /* Actual comparison value is 10 in Q16, but compare against 60 to avoid
            having to divide mean_bep_per_block result by 6 (10 => 0.015% BER). If mean BEP over 48
            DL blocks > 0.015%, switch AEQ PH2 off */
         if((mean_bep_per_block < GL1_AEQ_8PSK_OFF_BEP_THRESHOLD) && (mean_bep_params[gas_id].bep_averaging_aeq_off_count >= GL1_SIZEOF_MEAN_BEP_48_BLKS))
         {
           mdsp_enable_aeq_8psk_dynamic(FALSE ,gas_id);
#ifdef FEATURE_GSM_DYNAMIC_AEQ_RXD_DEBUG
           MSG_GERAN_HIGH_1_G("Disabling AEQ: mean_bep_per_block=%d",mean_bep_per_block);
#endif
           delayed_aeq_speed_bump_disable[gas_id] = TRUE;
         }
       }
     }
#endif /* (FEATURE_GSM_DYNAMIC_AEQ_8PSK) */
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
     if(mean_bep_params[gas_id].bep_averaging_rxd_off_count < GL1_SIZEOF_MEAN_BEP_48_BLKS)
     {
       mean_bep_params[gas_id].bep_averaging_rxd_off_count++;
     }
     /* Only dynamically switch Data RxD if enabled in NV */
     if (gl1_get_MSRD_ctl_nv(gas_id))
     {
   
       if( gl1_rxd_ctl_flag.RxdForcedOn && (gl1_in_ftm_mode) &&!gl1_get_enable_rxdiversity_data_dynamic(gas_id) )
       {
           gl1_set_enable_rxdiversity_data_dynamic(TRUE, gas_id);
#ifdef FEATURE_GSM_DYNAMIC_AEQ_RXD_DEBUG
           MSG_GERAN_HIGH_1_G("Enabling RxD: gl1_rxd_ctl_flag.RxdForcedOn=%d", gl1_rxd_ctl_flag.RxdForcedOn);
#endif   
           mean_bep_params[gas_id].bep_averaging_rxd_off_count = 0;
       }
       else if (!gl1_get_enable_rxdiversity_data_dynamic(gas_id))
       {
         /* This is actually checking mean BEP per 4 blocks, which is stored in each of the
            6 mean_bep_params[gas_id].mean_bep_per_48_blks bins (15 => 0.023% BER). If mean BEP over 4 DL blocks > 0.023%
            switch Data RxD back on */
         if(mean_bep_params[gas_id].mean_bep_per_48_blks[mean_bep_params[gas_id].mean_bep_per_48_blks_index] > GL1_DATA_RXD_ON_BEP_THRESHOLD)
         {
           gl1_set_enable_rxdiversity_data_dynamic(TRUE, gas_id);
#ifdef FEATURE_GSM_DYNAMIC_AEQ_RXD_DEBUG
           MSG_GERAN_HIGH_1_G("Enabling RxD: mean_bep_per_48_blocks=%d",mean_bep_params[gas_id].mean_bep_per_48_blks[mean_bep_params[gas_id].mean_bep_per_48_blks_index]);
#endif
           mean_bep_params[gas_id].bep_averaging_rxd_off_count = 0;
         }
       }
       else
       {
         mean_bep_per_block = 0;
         /* Calculate mean BEP over 48 (12*4) blocks in Q16 */
         for(i=0; i<GL1_SIZEOF_MEAN_BEP_48_BLKS; i++)
         {
           mean_bep_per_block += mean_bep_params[gas_id].mean_bep_per_48_blks[i];
         }

         /* Actual comparison value is 10 in Q16, but compare against 60 to avoid
            having to divide mean_bep_per_block result by 6 (10 => 0.015% BER). If mean BEP over 48
            DL blocks > 0.015%, switch Data RxD off */
         if((mean_bep_per_block < GL1_DATA_RXD_OFF_BEP_THRESHOLD) && (mean_bep_params[gas_id].bep_averaging_rxd_off_count >= GL1_SIZEOF_MEAN_BEP_48_BLKS))
         {
           gl1_set_enable_rxdiversity_data_dynamic(FALSE, gas_id);
#ifdef FEATURE_GSM_DYNAMIC_AEQ_RXD_DEBUG
           MSG_GERAN_HIGH_1_G("Disabling RxD: mean_bep_per_block=%d",mean_bep_per_block);
#endif
         }
       }

#ifdef FEATURE_GSM_RX_DIVERSITY
         gl1_hw_set_mean_bep(mean_bep_per_block, gas_id);
#endif /* FEATURE_GSM_RX_DIVERSITY */
     }
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */

     /* Calculate next index into 48 block mean BEP buffer */
     if( ++mean_bep_params[gas_id].mean_bep_per_48_blks_index > (GL1_SIZEOF_MEAN_BEP_48_BLKS-1) )
     {
       mean_bep_params[gas_id].mean_bep_per_48_blks_index = 0;
     }
   }
}

/*===========================================================================

FUNCTION  gl1_msg_init_aeq_8psk_mean_bep

DESCRIPTION
  Initialises AEQ 8PSK mean BEP buffers and buffer indices.

===========================================================================*/
void gl1_msg_init_aeq_8psk_mean_bep(gas_id_t gas_id)
{
  uint16 i;

  mean_bep_params[gas_id].mean_bep_per_48_blks_index = 0;
  mean_bep_params[gas_id].mean_bep_per_4_blks_index = 0;
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  mean_bep_params[gas_id].bep_averaging_rxd_off_count = 0;
#endif
#ifdef FEATURE_GSM_DYNAMIC_AEQ_8PSK
  mean_bep_params[gas_id].bep_averaging_aeq_off_count = 0;
#endif

  for(i=0; i<GL1_SIZEOF_MEAN_BEP_48_BLKS; i++)
  {
    /* reset mean BEP buffer values to 0 */
    mean_bep_params[gas_id].mean_bep_per_48_blks[i] = 0;
  }

  /* reset buffer index to first element*/
  mean_bep_params[gas_id].mean_bep_per_48_blks_index = 0;
  /* reset buffer index to first element*/
  mean_bep_params[gas_id].mean_bep_per_4_blks_index = 0;
}
#endif

/*===========================================================================

FUNCTION  gl1_msg_calc_cv_bep

DESCRIPTION
  Takes a BEP variance value in Q16 and returns the CV_BEP index

===========================================================================*/
uint8 gl1_msg_calc_cv_bep (uint32 cv_bep)
{
   /* Shift by 6 to go from Q16 to Q10 (rounding the answer) */
   return calc_meas_scale(CV_BEP_SCALE, (cv_bep + 32) >> 6);
}



/*===========================================================================

FUNCTION  gl1_msg_process_bep

DESCRIPTION
  Takes BEP values (in Q16) and returns the mean and the coefficient
  of variance in Q16 format.

===========================================================================*/
void gl1_msg_process_bep
(
   const uint16 bep[GL1_DEFS_FRAMES_IN_BLK_PERIOD],
   uint32 *mean_bep,
   uint32 *cv_bep
)
{
   uint32 mean_bep_per_blk_Q18 = 0;
   uint64 acc_cv_bep_Q36 = 0;
   uint32 cv_bep_per_blk;
   uint8  burst;
   int32  temp_cv_bep;
   uint32 bep_Q18[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
   uint32 num_Q24;
   uint32 dem_Q32;

   /* Find the mean BEP per block without any truncation (use Q18) */
   for (burst = 0; burst < GL1_DEFS_FRAMES_IN_BLK_PERIOD; burst++)
   {
      bep_Q18[burst] = bep[burst] << 2;
      mean_bep_per_blk_Q18 += bep_Q18[burst];
   }
   mean_bep_per_blk_Q18 >>= 2;

   /* If the mean is zero, we are done */
   if (mean_bep_per_blk_Q18 == 0)
   {
      *mean_bep = 0;
      *cv_bep = 0;
      return;
   }

   /* Accumulate the BEP variance as a Q36 */
   for (burst = 0; burst < GL1_DEFS_FRAMES_IN_BLK_PERIOD; burst++)
   {
      temp_cv_bep = (int32)(bep_Q18[burst] - mean_bep_per_blk_Q18);
      acc_cv_bep_Q36 += (uint64)((int64)temp_cv_bep * (int64)temp_cv_bep);
   }

   /* Find the coefficient of variation:
      We have accumulated the CV in a 36 bit number, but do not want to
      calculate a 36-bit square root since the 64-bit instructions are
      really slow.  If the value is large enough we can convert to a Q32
      without too much loss in accuracy, otherwise if the top 12 MSB are
      zero we can shift left by 12 and take the square root of the
      "Q48" 32-bit number for maximum accuracy at low CV BEP values. */
   if (acc_cv_bep_Q36 > 0xFFFFF)
   {
      /* Calculated maximum error from this truncation and 32-bit square
         root is 0.002% from the true square root */
      num_Q24 = (uint32)(acc_cv_bep_Q36 >> 4);  /* Q32 */
      num_Q24 = square_root(num_Q24);           /* Q16 */
      num_Q24 <<= 8;                            /* Q24 */
   }
   else
   {
      num_Q24 = (uint32)(acc_cv_bep_Q36 << 12);  /* Q48 */
      num_Q24 = square_root(num_Q24);            /* Q24 */
   }
   /* Since the denominator is Q32, and we want the final answer in Q16 we
      shift the numerator by 24 bits and do a 64 bit divide.  This is slower
      but far more accurate than right shifting the denominator by 16 bits */
   dem_Q32 = (SQRT_3_Q14 * mean_bep_per_blk_Q18);          /* Q32 */
   cv_bep_per_blk = (uint32)(((uint64)num_Q24 << 24) / dem_Q32);   /* Q48/Q32 = Q16 */

   /* Fill in return values */
   *mean_bep = (mean_bep_per_blk_Q18 + 2) >> 2;
   *cv_bep = cv_bep_per_blk;
}


/*===========================================================================

FUNCTION gl1_msg_saic_in_burst

DESCRIPTION
  This function checks if SAIC was used in the burst. From the mDSP SAIC
  code bit 15 is used, and if it's set to '0' then SAIC was not used, and
  '1' if SAIC is used.

DEPENDENCIES
  None

RETURN VALUE
  Boolean     TRUE if SAIC is used.

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_saic_in_burst(uint16   saic_misc_word)
{
  return (((saic_misc_word>>15)&0x1));
}

/*===========================================================================

FUNCTION gl1_msg_reset_hard_rxqual_ave

DESCRIPTION
  Will reset the hard_rx_qual averaging params for a timer restart.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_reset_hard_rxqual_ave( gas_id_t gas_id )
{
  MSG_GERAN_LOW_0_G( "SAIC Disable reset" );

  gl1_hard_rx_qual_sum[gas_id]     = 0;
  gl1_hard_rx_qual_samples[gas_id] = 0;
}

/* The idealised hard_rx_qual value given by F/W on SBD */
#define PERFECT_HARD_RX_QUAL                   32767

/*
 * Minimum number of valid samples to have before SAIC disable is possible
 * based off of PA_MFRM of 9 and 50 seconds averaging period avoids the problem
 * of only taking a few samples then tunnel then reselect etc
 */
#define PERFECT_HARD_RX_QUAL_MIN_SAMPLE_THRESH 10

#define MAX_ALLOWED_HARD_RX_QUAL_SUM           \
          ( ( PERFECT_HARD_RX_QUAL_MIN_SAMPLE_THRESH * 5 ) * PERFECT_HARD_RX_QUAL )

/*===========================================================================

FUNCTION gl1_msg_is_saic_disable_in_idle_valid

DESCRIPTION
  Called to decide if we can disable SAIC in idle at the start of a PCH receive
  sequence. Will avoid switching SAIC status mid receive. Penalty is we will
  lose a bad block but should get gain from SAIC on next one and until timer
  timesout as timer is then restarted.

DEPENDENCIES
  None

RETURN VALUE
  Boolean to indicate if we can disable SAIC in idle

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_is_saic_disable_in_idle_valid( gas_id_t gas_id )
{
  boolean saic_disable_in_idle = FALSE;
  int32   hard_rxqual_ave      = 0;

  /* Calc the new average */
  if ( gl1_hard_rx_qual_samples[gas_id] )
  {
    hard_rxqual_ave =
      ( gl1_hard_rx_qual_sum[gas_id] / gl1_hard_rx_qual_samples[gas_id] );
  }

  /* If average is good and we have enough samples then set SAIC disable */
  if ( ( hard_rxqual_ave >= PERFECT_HARD_RX_QUAL ) &&
       ( gl1_hard_rx_qual_samples[gas_id] >= PERFECT_HARD_RX_QUAL_MIN_SAMPLE_THRESH ) )
  {
    saic_disable_in_idle = TRUE;
  }

  MSG_GERAN_LOW_3_G( "SAIC disable %d hard_rxqual_ave %d samples %d",
           saic_disable_in_idle, hard_rxqual_ave, gl1_hard_rx_qual_samples[gas_id] );

  return ( saic_disable_in_idle );
}


/*===========================================================================

FUNCTION gl1_msg_set/get_saic_disable_in_idle

DESCRIPTION
  Set/Get the SAIC disable status flag.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_set_saic_disable_in_idle( boolean  saic_disable_in_idle,
                                       gas_id_t gas_id )
{
  gl1_msg_saic_dis_in_idle[gas_id] = saic_disable_in_idle;
}

boolean gl1_msg_get_saic_disable_in_idle( gas_id_t gas_id )
{
  return ( gl1_msg_saic_dis_in_idle[gas_id] );
}

/* Avoid extra includes */
extern void l1_idle_saic_disable_timer_start( gas_id_t gas_id );

/*===========================================================================

FUNCTION gl1_msg_update_hard_rxqual_ave

DESCRIPTION
  Called each PCH/BCCH receive to update the stored hard_rx_qual vals
  that are later used to find the average when the timer expires.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_update_hard_rxqual_ave( uint16 current_hard_rx_qual,
                                     gas_id_t gas_id )
{
  /* As soon as we receive a value lower than threshold restart timer*/
  if ( current_hard_rx_qual < PERFECT_HARD_RX_QUAL )
  {
    MSG_GERAN_LOW_3_G( "RESET hard_rx_qual %d num_samples %d SAIC back ON FN %d",
             current_hard_rx_qual, gl1_hard_rx_qual_samples[gas_id], GSTMR_GET_FN_GERAN(gas_id) );

    /* This will reset the sum and samples vars */
    l1_idle_saic_disable_timer_start( gas_id );
  }
  else
  {
    /* Update sum for average */
    gl1_hard_rx_qual_sum[gas_id]     += current_hard_rx_qual;

    gl1_hard_rx_qual_samples[gas_id] += 1;

    /* Avoid rollover in summed hard_rx_qual for very long idle periods */
    if ( gl1_hard_rx_qual_sum[gas_id] > MAX_ALLOWED_HARD_RX_QUAL_SUM )
    {
      /*
       * Reset as if taken at least the MIN sample threshold to avoid
       * the disable not being valid anymore
       */
      gl1_hard_rx_qual_sum[gas_id] =
        ( PERFECT_HARD_RX_QUAL_MIN_SAMPLE_THRESH *
          ( gl1_hard_rx_qual_sum[gas_id] / gl1_hard_rx_qual_samples[gas_id] ) );

      gl1_hard_rx_qual_samples[gas_id] = PERFECT_HARD_RX_QUAL_MIN_SAMPLE_THRESH;
    }

    MSG_GERAN_LOW_3_G( "UPDATE hard_rx_qual %d rx_qual_sum %d num_samples %d",
             current_hard_rx_qual, gl1_hard_rx_qual_sum[gas_id],
             gl1_hard_rx_qual_samples[gas_id] );
  }
}

void  gl1_msg_algs_get_arfcn( uint16*      arfcn_ptr,
                              sys_band_T*  band_ptr,
                              gas_id_t gas_id
                            )
{
  if( NULL != arfcn_ptr )
  {
    *arfcn_ptr = gl1_msg_algs_data_ptr[gas_id]->afc_arfcn;
  }
  else
  {
    MSG_GERAN_ERROR_0_G("NULL arfcn_ptr");
  }

  if( NULL != band_ptr )
  {
    *band_ptr = gl1_msg_algs_data_ptr[gas_id]->afc_band;
  }
  else
  {
    MSG_GERAN_ERROR_0_G("NULL band_ptr");
  }
}

/*===========================================================================

FUNCTION gl1_msg_utils_binary_search

DESCRIPTION
  This function performs a binary search.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
int16 gl1_msg_utils_binary_search(const uint32 data[],
                                  uint32 input,
                                  uint16 sizeof_data)
{
  int16 low, mid = 0, high;

  //Lint, type casted to sint15
  low =0; high=(int16)sizeof_data - 1;
  while (low <= high)
  {
    mid = (high + low)/2;
    if ( data[mid] < input )
    {
      low = mid + 1;
    }
    else if ( data[mid] > input )
    {
      high = mid - 1;
    }
    else
    {
      /* found mid ! */
      break;
    }
  }

  /* Ensure that we are returning the lower point of a line segment */
  /* for the interpolation algorithm                                */
  if (data[mid] > input)
  {
    mid--;
  }

  return(mid);

}

/*===========================================================================

FUNCTION gl1_msg_ten_log_of_power_val

DESCRIPTION
  This function computes the 10log10(power).  It implemets this by
  linearly interpolating a log LUT.  The output of the LUT is a 10log10 value.
  The LUT provided a maximum error of 1/16 dB.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_ten_log_of_power_val(uint32 power, dBx16_T *dBx16_result_ptr)
{
  boolean status = FALSE;

  int16  index;
  uint32  a,b;
  uint16  f_of_a, f_of_b;

  /* Can't take log10(0) = undefined */
  if (power > 0)
  {
    index = gl1_msg_utils_binary_search(gl1_msg_rssi_range_tbl, power, SIZEOF_GL1_MSG_RSSI_RANGE_TBL);

    if (index < 0)
    {
      MSG_GERAN_ERROR_1("Invalid index for the RSSI table. index=%d",index);
      return status;
    }

    /* Linear interpolate the result value */
    a      = gl1_msg_rssi_range_tbl[index];
    f_of_a = gl1_msg_ten_log_power_x16_tbl[index];
    b      = gl1_msg_rssi_range_tbl[index+1];
    f_of_b = gl1_msg_ten_log_power_x16_tbl[index+1];

    *dBx16_result_ptr = (dBx16_T) (f_of_a + ((power-a)*(f_of_b-f_of_a))/(b-a));

    status = TRUE;
  }

  return(status);
}

/*===========================================================================

FUNCTION gl1_msg_compute_rx_power

DESCRIPTION
  This function computes and validates the Rx power at the antenna.  It
  accomplishes this by using the following formula:

              10log(rssi) - Grx

  where Grx represents the entire Rx chain gain from the antenna input to
  baseband.

  NOTE: Grx = G(digital) + G(rf)

  The design of this function assumes that rf_set_rx_gain_data() was
  previously invoked;  the gain value that is required for the Rx power
  computation is established there.

  rssi_value    - input,  (I^2 + Q^2) value
  dBm_x16_value - output, absolute power result
  gain_range    - output, gain range used in deriving the dBm_x16_value
                  (only provided for informational purposes - logging)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
rf_measurement_T gl1_msg_compute_rx_power (uint32          rssi_value,
                                           dBx16_T         *dBm_x16_value,
                                           rf_gain_range_T *gain_range,
                                           rf_gain_T       *stored_gain)
{
  rf_measurement_T  power_result = RF_NOISY;
  dBx16_T           dBx16_result;

  if (stored_gain != NULL)
  {
    if (gl1_msg_ten_log_of_power_val(rssi_value, &dBx16_result))
    {
      *dBm_x16_value  = dBx16_result - stored_gain->valx16;
      *gain_range     = stored_gain->range;

      /*
      ** Validate the signal level against the particular gain range
      ** windows. The windows are calculated at the time that the
      ** gain range is set.
      */
      if (*dBm_x16_value > (stored_gain->range_high_limitx16 + RFCOM_HOT_DELTA))
      {
        power_result   = RF_SATURATED;
      }
      else if (*dBm_x16_value > stored_gain->range_high_limitx16)
      {
        power_result   = RF_HOT;
      }
      else if (*dBm_x16_value <
               (stored_gain->range_low_limitx16 - RFCOM_COLD_DELTA))
      {
        power_result   = RF_NOISY;
      }
      else if (*dBm_x16_value < stored_gain->range_low_limitx16)
      {
        power_result   = RF_COLD;
      }
      else
      {
        power_result = RF_IN_RANGE;
      }
    }
    else
    {
      /* The RSSI should be non-zero coming from the MDSP because of
      ** thermal induced and radio noise floor.
      */
      *dBm_x16_value  = RFGSM_MIN_RX_POWER_REPORTED;
      MSG_GERAN_MED_0(" *** RSSI = 0 ***");
    }
  }
  else
  {
    MSG_GERAN_ERROR_0("No stored gain!");
  }

  return(power_result);
}

/*===========================================================================

FUNCTION gl1_msg_update_xo_rgs

DESCRIPTION
  This is used to set the XO RGS correctly for the chosen SUB.
  Typically used only during select specific to ensure the XO
  manager has the most recent value for this SUB so when lock
  is released and regained the appropriate latest G SUB value is
  available.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_update_xo_rgs( boolean is_idle, boolean state_info, gas_id_t gas_id )
{
  if( state_info )
  {
    if ( !is_idle)
    {
      gl1_hw_update_xo_rgs( gl1_hw_get_xo_acc_freq_err( gas_id ),
                            TCXOMGR_STATE_TRAFFIC, gas_id );
    }
    else
    {
      gl1_hw_update_xo_rgs( gl1_hw_get_xo_acc_freq_err( gas_id ),
                            TCXOMGR_STATE_IDLE, gas_id );
    }
  }
  else
  {
    /* As only SCH for Specific BCCH known then mark as NO_INFO for state */
    gl1_hw_update_xo_rgs( gl1_hw_get_xo_acc_freq_err( gas_id ),
                          TCXOMGR_STATE_NO_INFO, gas_id );
  }
}

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
/*===========================================================================

FUNCTION gl1_msg_dtx_detection_algorithm

DESCRIPTION
  This function implement the DTX detection algorithm. This algorithm is
  called at every SACCH report.
  To detect DL DTX the algorithm checks that the RXQUAL_SUB (SACCH frames)
  is good (ie, below SYS_PARAM_DTX_DET_RXQUAL_SUB_THRESHOLD) and the RXQUAL_FULL 
  (SACCH + TCH) is degraded (ie above SYS_PARAM_DTX_DET_RXQUAL_FULL_THRESHOLD).

  The above condition has to be true for at least SYS_PARAM_DTX_DET_COUNTER_THRESHOLD
  SACCH cycles before declaring DTX detected.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_dtx_detection_algorithm(byte rxqual_full, byte rxqual_sub, byte rxlev_sub, gas_id_t gas_id)
{
  extern dedicated_data_T l1_dedicated_data[];

  uint16 serving_cell_arcfn = l1_idle_data_store[gas_id].campedon_cell_ARFCN.num;
  channel_information_T *channel_info_ptr = NULL;

  if (l1_tsk_buffer[gas_id].sub_state == L1_DED_BEFORE )
  {
    channel_info_ptr = &l1_dedicated_data[gas_id].channel_specification.channel_info_1_before;
  }
  else if (l1_tsk_buffer[gas_id].sub_state == L1_DED_AFTER )
  {
    channel_info_ptr = &l1_dedicated_data[gas_id].channel_specification.channel_info_1_after;
  }

  /* Do not apply dtx detection if the BCCH ARFCN  == TCH ARFCN and 
   * frequency hopping not enabled */
  if (channel_info_ptr && 
      !channel_info_ptr->hopping_flag && 
      channel_info_ptr->frequency_list.channel[0].num == serving_cell_arcfn)
  {
    dtx_detection[gas_id].count = 0;
    dtx_detection[gas_id].enabled = FALSE;

    MSG_GERAN_HIGH_3_G("DTX DETECTION, DTX detection disabled, freq hopping: %d, serving cell: %d, TCH ARFCN :%", 
      channel_info_ptr->hopping_flag,
      channel_info_ptr->frequency_list.channel[0].num, 
      serving_cell_arcfn);

    return;
  }

  /* Do not apply dtx detection in DTM */
  if (l1_tsk_buffer[gas_id].l1_state == L1_DTM_MODE)
  {
    dtx_detection[gas_id].count = 0;
    dtx_detection[gas_id].enabled = FALSE;
    MSG_GERAN_HIGH_0_G("DTX DETECTION, DTX detection disabled in DTM"); 
    return;
  }


  if (rxqual_sub < SYS_PARAM_DTX_DET_RXQUAL_SUB_THRESHOLD && 
      rxqual_full > SYS_PARAM_DTX_DET_RXQUAL_FULL_THRESHOLD &&
      gl1_msg_get_tch_loopback(gas_id) != GL1_DEFS_LOOPBACK_TYPE_C)
  {
    if (dtx_detection[gas_id].count < SYS_PARAM_DTX_DET_COUNTER_THRESHOLD) 
    { 
      dtx_detection[gas_id].count++;
    }
    if (dtx_detection[gas_id].count >= SYS_PARAM_DTX_DET_COUNTER_THRESHOLD &&
        rxlev_sub > SYS_PARAM_DTX_DET_RXLEV_SUB_THRESHOLD) 
    { 
      dtx_detection[gas_id].enabled = TRUE;
    }
  }
  else
  {
    dtx_detection[gas_id].count = 0;
    dtx_detection[gas_id].enabled = FALSE;
  }
  MSG_GERAN_HIGH_5_G("DTX DETECTION, qual_sub: %d, qual_full: %d, lev_sub: %d, count: %d, detected: %d",
                     rxqual_sub, rxqual_full, rxlev_sub,
                     dtx_detection[gas_id].count,
                     dtx_detection[gas_id].enabled);
}

/*===========================================================================

FUNCTION gl1_msg_dtx_detection_get_status

DESCRIPTION
  This function TRUE if dtx is detected

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_dtx_detection_get_status(gas_id_t gas_id)
{
  return dtx_detection[gas_id].enabled;
}

/*===========================================================================

FUNCTION gl1_msg_amr_pd_update_info

DESCRIPTION
  This function implement the algorithm to determine if AMR partial decode
  should be enabled

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_amr_pd_algorithm(byte rxqual_sub, byte rxqual_full, byte rxlev_sub, gas_id_t gas_id)
{
  /* Algorithm requires rxqual_sub (SACCH quality) to below 
   * SYS_PARAM_AMR_PD_RXQUAL_SUB_THRESHOLD (good) and the minimum rxqual
   * detected during the SACCH report period has to be above 
   * SYS_PARAM_AMR_PD_HARD_RXQUAL_THRESHOLD indicating that the burst
   * can be decoded with good quality 
   * Do not enable AMR PD when in DTM */
  if (rxqual_sub < SYS_PARAM_AMR_PD_RXQUAL_SUB_THRESHOLD && 
      amr_partial_decode[gas_id].min_rxqual >= SYS_PARAM_AMR_PD_HARD_RXQUAL_THRESHOLD &&
      gl1_msg_get_tch_loopback(gas_id) != GL1_DEFS_LOOPBACK_TYPE_C &&
      l1_tsk_buffer[gas_id].l1_state != L1_DTM_MODE)
  {
    /* Only increment count when below SYS_PARAM_AMR_PD_COUNTER_THRESHOLD to avoid
     * unnecesarily overflowing count variable */
    if (amr_partial_decode[gas_id].count < SYS_PARAM_AMR_PD_COUNTER_THRESHOLD) 
    { 
      amr_partial_decode[gas_id].count++;
    }

    if ((amr_partial_decode[gas_id].count >= SYS_PARAM_AMR_PD_COUNTER_THRESHOLD ||
         rxqual_full < SYS_PARAM_AMR_PD_RXQUAL_FULL_THRESHOLD) &&
        rxlev_sub > SYS_PARAM_AMR_PD_RXLEV_SUB_THRESHOLD) 
    { 
      amr_partial_decode[gas_id].enabled = TRUE;
    }
    else
    {
      amr_partial_decode[gas_id].enabled = FALSE;
    }
  }
  else
  {
    amr_partial_decode[gas_id].count = 0;
    amr_partial_decode[gas_id].enabled = FALSE;
  }
  MSG_GERAN_HIGH_5_G("AMR PARTIAL DECODE, rxqual_sub: %d, hard rxqual: %d, rxlev_sub: %d, count: %d, enabled: %d",
                     rxqual_sub, amr_partial_decode[gas_id].min_rxqual, rxlev_sub,
                     amr_partial_decode[gas_id].count,
                     amr_partial_decode[gas_id].enabled);

  amr_partial_decode[gas_id].min_rxqual = L1_PERFECT_RXQUAL;
}

/*===========================================================================

FUNCTION gl1_msg_amr_pd_cancel_partial_decode

DESCRIPTION
  This function is used to cancel partial decode. It is usually called 
  when the burst metrics are received from firmware

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_amr_pd_cancel_partial_decode(boolean cancel, gas_id_t gas_id)
{
  if (cancel)
  {
    if (amr_partial_decode[gas_id].enabled)
    {
      MSG_GERAN_HIGH_1_G("AMR PARTIAL DECODE, Firmware canceling AMR Partial Decode, count: %d",
                         amr_partial_decode[gas_id].count);
    }

    amr_partial_decode[gas_id].count = 0;
    amr_partial_decode[gas_id].enabled = FALSE;
  }
}

/*===========================================================================

FUNCTION gl1_msg_amr_pd_update_hard_rxqual

DESCRIPTION
  This function is used to keep track of the minimum hard rxqual reported
  during the SACCH report period, when amr_pd_algorithm is executed min_rxqual
  is reset to its maximum possible value

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_amr_pd_update_hard_rxqual(uint32 hard_rxqual, gas_id_t gas_id)
{
  MSG_GERAN_LOW_2_G("AMR PARTIAL DECODE, updating hard_rxqual: %d, min_rxqual: %d",
                     hard_rxqual, 
                     amr_partial_decode[gas_id].min_rxqual);

  /* if hard_rxqual is smaller than the current min_rxqual, then set
   * min_rxqual to hard_rxqual */
  if (hard_rxqual < amr_partial_decode[gas_id].min_rxqual) 
  {
    amr_partial_decode[gas_id].min_rxqual = hard_rxqual;
  }
}

/*===========================================================================

FUNCTION gl1_msg_amr_pd_get_status

DESCRIPTION
  This function TRUE if amr partial decode is enabled

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_amr_pd_get_status(gas_id_t gas_id)
{
  return amr_partial_decode[gas_id].enabled;
}
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */
