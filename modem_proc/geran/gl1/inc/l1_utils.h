#ifndef L1_UTILS_H
#define L1_UTILS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  L1   U T I L I T I E S   P R O C E S S I N G
                       H E A D E R   F I L E

DESCRIPTION
   Layer 1 utility function module

EXTERNALIZED FUNCTIONS
  L1_map_dBm_x16_to_rxlev
    Function that maps dBx16_T to RXLEV per GSM05.08.

Copyright (c) 2002-2015 Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/inc/l1_utils.h#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   -------- --------------------------------------------- 
20/03/17   mk       CR2020999 Prx Drx power difference needs to be calculated & applied to NCELLs before REP_PERIOD_IND to avoid delays
09/02/16   mk       CR2002074 Add LB_MS_TXPWR_MAX_CCH for dedicated Tx power level initialization
03/01/18   ksb      CR2164555 Geran support to send Global Time Services Information TO GPS module
07/07/17   snjv     CR2073295 Integer Overflow When Avg. Tx pwr for MSlot Config in Packet Transfer
28/04/17   og       CR2048710 For MCS STX PWR IND: Avoid IIR Filter and PCL Conversion when averaging
24/03/17   nm       CR2023665 Correct the Tx power averaging required for SAR reporting
12/10/16   gmr      CR1036802 Gl1 to use actual Tx power in MCS power reports for RT SAR feature
08/09/17   sn        CR1061730 FR43082: TA during PS Procedures- GL1 changes 
15/12/17   ap       CR1103567 FR36133: Extended LTE Signaling (ELS) to manage tune-away gaps in DSDS 
08/09/17   sn       CR1061730 FR43082: TA during PS Procedures- GL1 changes 
29/03/16   mn       CR995137 Access tune away support: Allowing other tech's paging at GPRS access
21/04/17   br       CR1041084 FR 36219: GDET – GMSK Modulation Detector
03/01/16   mn       CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
03/12/15   dp       CR929648 reduce power usage when waiting for TRM during find_bcch
10/09/15   sk       CR899847 GERAN changes to trigger QSH events and handler for debugging
29/07/15   nk       CR881383 Update gain tags only if DTM is in running sub state
17/07/15   mc       CR871205 : Incorrect CXM Channel ID assigned to W2G activities in W+W mode
17/06/15   cjl      CR853555 Use dynamic connected mode reasons for CS/PS traffic
15/06/15   am        CR854340 Adding simulation support through diag
12/05/15   mko      CR831541: G21x Tuneaway Optimization - during T3168/T3190 & PDAN G21x TA allowed with NV control
28/05/15   akh      CR822523 (FR26957) Modem power tx stats
07/05/15   mc       CR830988 : COEX Channel ID selection for X2G IRAT measurements does not support TRM_UMTS2
21/04/15   shr        CR806878 GPRS DSDS Power Control and WWCoex Power Info do not log the Tx Power in use
07/04/15   ab       CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX 
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
31/03/15   pa       CR815621:GRM restructuring for TH 2.0. 
30/03/15   nm       CR765891 Allowing TA for TDS during PS RACH
24/03/15   ap       CR811053:FR22272 - TRM Unification  - Access and Traffic attempts 
25/03/15   ap       CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic 
12/03/15   pa        CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
05/12/14   sp        CR767083: Override paging multiframe to 2, for PDT tests
27/02/15   am       CR646584 WLAN coex
04/02/14   nk       CR767734 Allowing TA during 2-phase access
18/12/13   ka       CR580134 RLF optimizations for CMCC for SGLTE (FR18608)
22/09/14   ap       CR720862: FR 20912: SGLTE+G DSDA with WTR asymmetric GSM support
15/09/14   sp       CR681169:Log control block data from MAC
06/08/14   ka       CR699964 Implement Idle RXD support in GL1
10/07/14   jk       CR688587:GL1 changes to support LIF feature
29/07/14   ap       CR695234: Changes for GL1's New Rach interface
02/07/14   mc       CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
25/06/14   cs       Remove FEATURE_ASYMMETRIC_GERAN_BANDS
17/06/14   cs/ch    CR651795 TRM Updated changes for DR-DSDS
08/05/14   sp       CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
15/04/14   ka       CR611185: Ensure information from the MPH_START_GPRS_IDLE_MODE_REQ is used in RACH
16/04/14   smd      CR650539:Added sending MCS_CXM_STX_SET_POWER_IND back.
23/03/14   sjw      CR637079:new interface for BPLMN failures
17/03/14   jk       CR631867:Enable aeqPhase2a only during EGPRS mode 
19/02/14   jk       CR608290:GERAN RxD ON/OFF adaptive algorithm
19/02/14   ap       CR618768 In SGLTE+G SS Retain TRM lock for TRAFFIC
06/02/14   smd      CR548408: Remove sending MCS_CXM_STX messages. 
01/11/13   ab       CR.568623 : COEX : DSDA - update Rx Tx power for COEX with a faster convergence
11/14/13   ab       CR 554820 DSDA GL1 - Update Tx power as per slot transmission for RF COEX
03/10/13   ws       CR499846 - Bug fix for RACH PWR reduction feature
23/05/13   pg       CR460555: Enable 3GPP Rel10 RACH Power Reduction feature  
15/08/13   sk       CR524039 GBTA bringup changes
21/06/13   cs       Major TSTS Triton Syncup
16/05/13   ab       CR487396 - Support EGPRS IR Reduced Window in GFW
22/05/13   ab       CR450173 Remove FEATURE_CXM Dependency for FEATURE_GSM_GPRS_TIME_PWR_AVG
31/05/12   ab       CR 415136 RX and TX Time Power averaging update using msg router intf.
31/05/12   ab       CR 366880 RX and TX Time Power averaging Feature for DSDA as part of SAR Req.
30/11/10   ab       Support EGPRS WS REDUCTION during allocation of dynamic memory in start
                    gsm mode request when FEATURE_GSM_EGPRS_IR_OPT is defined.
13/06/10   pg       Featurise GL1 MUTEX debug
22/04/10   ab       CR: 234113 - For FEATURE_GSM_EGPRS_IR_OPT - 32 byte alignment for
                    heap address as part of requirement for DMA by FW
11/03/10   pg       Add GL1 MUTEX debug capabilities
10/11/09   ws       Merged Mutex changes from Mustang baseline
03/07/09   sjw      Support Extended BCCH parameter for l1_compute_tc()
26/06/09   sjw      Remove use of RR public store from L1 (for GCF NV flag)
06/06/09   ab       Feature FEATURE_GSM_EGPRS_IR_OPT for LCU
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
20/10/08   cs       Disable the debug printout in RX Sweep
16/10/08   cs       Update RX Sweep to use two slots for each measurement
09/10/08   cs       Allow the RX Sweep acquisition to be restarted
28/08/08   cs       Implement the Fast Rx Sweep functionality for FTM
28/07/08   hv       Added KxMutex support
20/03/08   agv      Fixed the LCU PMIC Band setup for a Channel Change.
14/12/07   ws       merged LCU Bringup and demo changes from 6260 branch
02/10/07   og       Clean up of unused code.
17/09/07   agv      CR127820 - iRAT W2G handover AGC setting.
21/05/07   dv       Update gl1_calc_ho_timing_advance()
02/03/07   cs       Add support for new CGPS timetag
10/02/05   gfr      Fixed sleep timetag featurization
08/21/05   gfr      Changed prototype for gl1_set_band_group
08/19/05   gfr      Added gl1_set_band_group
04/11/05   gfr      Added support for sleep timetag
03/16/05   pp       Added prototype for L1_map_rxlev_to_dBm_x16()
12/23/04   dp       Fix to not cap power level if Anite GCF flag is set
08/26/04   sv       Lint cleanup
03/26/04   gfr      Time tagging support.
02/25/04   gw       Lint clean up.
01/28/04   gw       Changes to support reselection optimizations.
01/09/04   gw       Added prototypes for gl1_compute_BCCH_mask() and
                    gl1_compute_ext_BCCH_mask().
11/05/03   gfr      Support for quad-band.
08/29/03   gw       Added prototype for function l1_snr_msg().
05/12/03   ATM      Moved Frame Number management entirely inside GTMRS
05/02/03   gw       Merged in power transfer changes from branch.
04/02/03   gw       Removed unneeded function L1_sort_surr_signal_strengths().
02/25/03   dp       Changed L1_get_ARFCNS interface for half rate
11/18/02   mk       Updated GPRS baseline with release 2.0 functionality.
11/15/02   dp       Added gl1_get_band prototype
11/14/02   ws/gw    Added macro IS_FRAME_NUM_LATER for non drx timer
11/08/02   ATM      Added central handover routines
10/16/02   dp       Changed L1_get_arfcns interface to handle negative offsets
10/01/02   ATM      Added central get and set FN routines
09/18/02   dp       Changed L1_get_arfcns interface to pass in freq redef
                    information
09/06/02   gr       Added commands for converting GPRS AGC bitfields into
                    dBx16 values.
08/12/02   gw       Added function l1_get_agc_ptr(). Removed function
                    L1_condition_rxlev.
06/10/02   dp       Changed l1_get_ARFCNS interface to pass in the before
                    and after channel parameters.
05/06/02   JC       API change L1_sort_signal_strengths().
04/30/02   gw       Added prototype for function l1_compute_tc.
03/20/02   dp       Added support for power control in steps of 2dB.
                    Split up L1_get_sig_strengths into separate functions
                    for tx and rx.
02/15/02   JC       Added L1_map_dBm_x16_to_rxlev function.
02/01/02   JC       Modifications to support AGC.
01/24/02   ATM      Added l1_change_state to allow central routine
                    for logging state changes
09/20/01   jc       Added L1_get_ARFCNS, L1_get_sig_strengths
09/10/01   JC       Initial revision.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "sys_type.h"
#include "gtmrs.h"
#include "gtmrs_g.h"           /* For ADD_MOD...macros */
#include "sys.h"
#include "sys_v.h"             /* For sys_band_class_e_type */
#include "gmutex.h"
#include "gl1_sys_algo.h"
#include "l1_sc_int.h"
#include "gl1_mutex.h"

#if defined ( FEATURE_GSM_GPRS_TIME_PWR_AVG )
#include "cxm.h"
#include "rfm_device_types.h"
#include "rfcom.h"
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#include "gl1lsmif.h"

#include "cgps_api.h"
#include "ds3g_ext.h"

/*===========================================================================

                         DATA DECLARATIONS

===========================================================================*/

extern boolean gl1_gts_timer_running[NUM_GERAN_DATA_SPACES];
#define GL1_GTS_UPDATE_TIMEOUT  1000 /* ms  = 1 sec*/

#ifdef FEATURE_GSM_RX_DIVERSITY        
#define  RXD_HARD_RXQUAL_THRESHOLD 32460
#define  RXD_INVALID_RXQUAL        8
#endif

#define CHECK_G21X_TA_ENABLE_RRBP() (!GL1_HW_EFS_DEBUG_ENABLED(GL1_EFS_DEBUG_DISABLE_G21X_TA_RRBP)) 
#ifdef FEATURE_WLAN_COEX_SW_CXM
typedef struct{
  boolean   valid;
  uint32    avg_snr;
  uint32    accum_count;
}gpl1_snr_t;
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
#define GL1_RXD "GL1_RXD: "
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#define RANE_MIN_DURATION_MS 40

#ifdef FEATURE_GTA_DURING_PS_RACH

#define G2X_PAGE_DUR_FRAMES (8)

#define FRAMES_TO_MOVE_RACH (4)

#define CHECK_IF_FN_COLLIDES_WITH_GAP(FN, gap_start_FN, gap_end_FN)\
       ((IS_FRAME_NUM_LATER(FN, gap_start_FN) && \
        (IS_FRAME_NUM_LATER(gap_end_FN, FN))) || \
        (FN == gap_start_FN || FN == gap_end_FN) )

#define G2X_TUNEAWAY_DSDS_TIMEOUT_RACH 30 
#define RACH_DELAY_INVALID 0xFFF
#define CHECK_IF_GTA_ALLOWED_IN_PS_RACH()\
       (gl1_msg_get_multi_sim_standby_mode() && \
        !gl1_check_gonly_mode_active()) 

#define CHECK_GTA_PS_RACH_NV_ENABLED() GL1_HW_EFS_DEBUG_ENABLED(GL1_EFS_DEBUG_ENABLE_GTA_PS_RACH) 

#endif /*FEATURE_GTA_DURING_PS_RACH*/

#ifdef  FEATURE_GSM_GPRS_TIME_PWR_AVG

/* Structure to hold current Tx average power */
typedef  struct{
  boolean  valid;
  int64    tx_pwr_accum_Q16;
  int32    avg_tx_pwr;
  int32    avg_tx_pwr_slot;          /* average of only slot which tx occurs */
  int32    accum_count;
  int32    avg_tx_pwr_dbm_10;        /* average over all eight slots */
  int32    avg_tx_slot_pwr_db_10;    /* average of only slot which tx occurs */

  /* Tx Power info counter array - keeps counts in TS for each power bin */
  uint32   pwr_stats_tx_in_ts[DS3G_TX_POWER_BIN_MAX];
}gpl1_tx_power_t;

/* Structure to hold current Rx average power */
typedef struct{
  boolean  valid;
  int32    rx_pwr_accum;
  int32    avg_rx_pwr;
  int32    accum_count;
  int32    avg_rx_pwr_dbm_10;
  int32    avg_rx_pwr_coex;
  int32    avg_rx_pwr_dbm_10_coex;
}gpl1_rx_power_t;

/* disable pwr averaging */
void gpl1_reset_tx_pwr_avg(gas_id_t gas_id);
void gpl1_reset_rx_pwr_avg(gas_id_t gas_id);

/* enable pwr averaging */
void gpl1_set_tx_pwr_avg_active(gas_id_t gas_id);
void gpl1_set_rx_pwr_avg_active(gas_id_t gas_id);

boolean gpl1_get_tx_pwr_avg_active_state(gas_id_t gas_id);
boolean gpl1_get_rx_pwr_avg_active_state(gas_id_t gas_id);

int32 gpl1_get_tx_pwr_avg_dbm10(gas_id_t gas_id);
int32 gpl1_get_avg_tx_slot_pwr_db_10(gas_id_t gas_id);
void gpl1_set_rx_pwr_avg_dbm10_coex(int32 rx_pwr_dbm_10_coex ,gas_id_t gas_id);
int32 gpl1_get_rx_pwr_avg_dbm10_coex(gas_id_t gas_id);
int32 gpl1_get_rx_pwr_avg_dbm10(gas_id_t gas_id);

void  gpl1_update_tx_pwr_avg( int16*                power_lvl_idx,
                              sys_band_T            band,
                              gl1_defs_coding_type  cs,
                              gas_id_t              gas_id
                            );

void  gpl1_update_coex_tx_pwr_avg( int32       tx_pwr_dBm,
                                   gas_id_t    gas_id
                                 );

void gpl1_update_rx_pwr_avg( int32 rssi,gas_id_t gas_id );

int32  gpl1_calc_avg_tx_pwr(gas_id_t gas_id);
int32  gpl1_calc_avg_rx_pwr(gas_id_t gas_id);

/* maps GSM Band to corresponding RF band enum */
rfcom_gsm_band_type gpl1_convert_to_rf_band_type(sys_band_T gsm_band,gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_cxm_stx_set_avg_power

DESCRIPTION

   Wrapper Function to send MCS_CXM_STX_SET_POWER to CXM

RETURN VALUE
  pwr in db10

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_cxm_stx_set_avg_power
(
  cxm_tech_type       tech_id,
  /* Time averaged Tx power in dBm*10 */
  int32               tx_power,
  /* Time averaged Rx power in dBm*10 */
  int32               rx_power
);

#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */
#ifdef FEATURE_GTA_DURING_PS_RACH

void gpl1_block_bound_51_multiframe(uint32 start_fn, uint32 *suspension_FN_ptr, gas_id_t gas_id);


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
boolean gl1_check_if_rach_delay_collides_with_gap  (uint32 delay, 
                                                              uint32 suspensionFN, 
                                                              uint32 margin,
                                                              uint32 duration, 
                                                              uint32* new_rach_delay,
                                                              boolean* is_rach_after_gap,
                                                              gas_id_t gas_id);
           

uint32 gl1_get_rach_fn_OTA(uint32 delay,boolean ccch_sdcch_comb, gas_id_t gas_id );
    
#endif

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION  gl1_get_FN

DESCRIPTION
  This function retrieves the L1 Frame Number.

RETURN VALUE
  dword - the L1 Frame Number

SIDE EFFECTS
  None.

===========================================================================*/
dword gl1_get_FN ( gas_id_t gas_id );

/*===========================================================================

FUNCTION  l1_change_state

DESCRIPTION
  This function changes the L1 state and sends a diag event indicating the
  state change.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_change_state ( const byte newState, gas_id_t  gas_id );

/*===========================================================================

FUNCTION  L1_SORT_SIGNAL_STRENGTHS

DESCRIPTION
  This function sorts the ARFCNs in the L1_Frequency_List in descending
  order of signal strength.

DEPENDENCIES
  None

RETURN VALUE
  uint16 - number of ARFCN's that satisfy the threshold criteria.

SIDE EFFECTS
  None

===========================================================================*/
uint16   L1_sort_signal_strengths( gas_id_t gas_id );

/*===========================================================================

FUNCTION  L1_MAP_DBM_X16_TO_RXLEV

DESCRIPTION
  This function translates a dBx16_T to RXLEV per GSM05.08, 8.1.4.

DEPENDENCIES
  rx_power

RETURN VALUE
  rxlev

SIDE EFFECTS
  None

===========================================================================*/
uint8 L1_map_dBm_x16_to_rxlev( dBx16_T pwr_lvl_x16 );

/*===========================================================================

FUNCTION  L1_MAP_RXLEV_TO_DBM_X16

DESCRIPTION
  This function translates a RXLEV to dBx16_T per GSM05.08, 8.1.4.

DEPENDENCIES
  None

RETURN VALUE
  dBx16_T - rx_power

SIDE EFFECTS
  None

===========================================================================*/
dBx16_T L1_map_rxlev_to_dBm_x16( uint8 RXLEV );

/*===========================================================================

FUNCTION  L1_NORMALISE_SIGNAL_STRENGTHS

DESCRIPTION
  This function corrects RXLEVS for the number of measurements made.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void   L1_normalise_signal_strengths( uint16         length,
                                      uint8          *no_of_measurement,
                                      dBx16_T        *rxlevs,
                                      sys_algo_agc_T *agcs
#ifdef GL1_GMSK_MOD_DET
                                      ,uint16 *decstat
#endif
									  );


/*===========================================================================

FUNCTION  L1_CALCULATE_PAGING_DATA

DESCRIPTION
  This function does the paging parameter calculations as in GSM 05.02
  section 6.5.2

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void   L1_calculate_paging_data( gas_id_t gas_id );

/*===========================================================================

FUNCTION  L1_CALCULATE_ARFCN

DESCRIPTION
  This function calculates the next frequency when frequency hopping. The
  algorithm is given in GSM 05.02 section 6.2.3.

DEPENDENCIES
  pointer to channel data

RETURN VALUE
  next frequency ( from mobile frequency allocation list )

SIDE EFFECTS
  None

===========================================================================*/
ARFCN_T L1_calculate_ARFCN(int16 offset, channel_information_T *channel_data, gas_id_t gas_id);

/*===========================================================================

FUNCTION L1_get_ARFCNS

DESCRIPTION
  This function is called to retrieve the next 4 ARFCNs to use.



DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void L1_get_ARFCNS (
                      int16 start,
                      uint16 increment,
                      uint16 num_arfcns,
                      ARFCN_T arfcns[],
                      channel_information_T *chnl1_info_ptr,
                      channel_information_T *chnl2_info_ptr,
                      channel_information_T *freq_redef_chnl_info_ptr, gas_id_t gas_id
                   );

/*===========================================================================

FUNCTION L1_get_txlev

DESCRIPTION
  This function is called to retrieve the next 4 tx power indices starting
  at the specified offset

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void L1_get_txlev (
                    uint8 block_start,
                    uint8 burst_spacing,
                    int16 block_txlev[],
                    gas_id_t gas_id);


/*===========================================================================

FUNCTION L1_convert_power_idx_to_dBm

DESCRIPTION
  This function converts power index to dB

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

int16 L1_convert_power_idx_to_dBm( sys_band_T band, uint16 pidx);


/*===========================================================================

FUNCTION L1_convert_tx_pwr_from_dBm_to_pcl

DESCRIPTION
  This function converts TX power in dBm to power control level (PCL)
  Only use this for TX power

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

uint8 L1_convert_tx_pwr_from_dBm_to_pcl( sys_band_T band, uint16 dBm, gas_id_t gas_id);

/*===========================================================================

FUNCTION  L1_CONVERT_WORD

DESCRIPTION
  This function converts a word into an ascii string
         ( decimal representation )

DEPENDENCIES
  **char    destination pointer
  *word     character running total
   word   word to convert to ascii decimal

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_convert_word(char  **dest_str, word   *char_count, word   input);

/*===========================================================================

FUNCTION  L1_PRINT_HEX

DESCRIPTION
  This function prints a line of data in hex

DEPENDENCIES
  *byte    source pointer
   byte     count - no of bytes to print

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_print_hex(byte  *source, byte   char_count);

/*===========================================================================

FUNCTION  l1_compute_tc

DESCRIPTION
  Calculate the value of TC based on the supplied frame number fn.

  TC = (fn div 51) mod 8.  See 5.02 section 6.3.4.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint8 l1_compute_tc( uint32 fn, boolean extbcch );

/*===========================================================================

FUNCTION  l1_get_agc_ptr

DESCRIPTION
  Searches through l1_Frequency_List and returns a pointer to the AGC
  struct associated with the given ARFCN.

DEPENDENCIES
  l1_Frequency_list must have been populated by doing a power scan.

RETURN VALUE
  Pointer to agc struct if ARFCN found, NULL otherwise.

SIDE EFFECTS
  None

===========================================================================*/
sys_algo_agc_T *l1_get_agc_ptr( ARFCN_T ARFCN, gas_id_t gas_id );

/*===========================================================================
FUNCTION  l1_compute_P0

DESCRIPTION
  Calculate the value of P0 based on the supplied bit field.

  P0 = in * 2

DEPENDENCIES
  None

RETURN VALUE
  Pr value in dBx16

SIDE EFFECTS
  None

===========================================================================*/
dBx16_T l1_compute_P0 (uint8 P0_bits);

/*===========================================================================
FUNCTION  l1_compute_Pb

DESCRIPTION
  Calculate the value of Pb based on the supplied bit field.

  Pb = in * 2

DEPENDENCIES
  None

RETURN VALUE
  Pr value in dBx16

SIDE EFFECTS
  None

===========================================================================*/
dBx16_T l1_compute_Pb (uint8 Pb_bits);

/*===========================================================================
FUNCTION  l1_compute_Pr

DESCRIPTION
  Calculate the value of Pr based on the supplied bit field.
  00 = 0-2 dB  = 1 dB
  01 = 4-6 dB  = 5 dB
  10 = 8-10 dB = 9 dB
  11 = no power control

DEPENDENCIES
  Pr value in dBx16

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
dBx16_T l1_compute_Pr (uint8 Pr_bits);

/*===========================================================================
FUNCTION  gl1_store_handover_details()

DESCRIPTION
  Routine shared by both interRAT and GSM handovers to store details needed
  for future activities, such as calculating timing or vocoder blanking

  IN:   mph_handover_req message
  OUT:  stored in global l1_dedicated_data.ho_details

RETURN VALUE
  none

SIDE EFFECTS
  Vocoder is blanked if vocoder modes mismatch

===========================================================================*/
void gl1_store_handover_details( mph_handover_req_T *msg_ptr );

/*===========================================================================

FUNCTION  gl1_calc_ho_timing_advance()

DESCRIPTION
  Routine shared by both interRAT and GSM handovers to calculate timing
  advance based on several parameters:

  IN:   l1_dedicated_data   -- global containing the following fields:
            ho_details.handover_synchronisation   -- the type of syncronization
            ho_details.timing_advance_valid       -- is ta valid?
            ho_details.timing_advance             -- ta value from base station
            ho_details.real_time_difference_valid -- is rtd valid?
            ho_details.real_time_difference       -- rtd value from base station
            ho_details.NCI                        -- ???
            target_cell_details.bsic_known        -- used in some calculations
            target_cell_details.qbit_lag          -- used in some calculations
            timing_advance_in_use                 -- used in some calculations
  IN:   current_timeslot -- global used for calculation

  OUT:  byte calculated_timing_advance -- range:[0..63]

DEPENDENCIES
  Must call gl1_store_handover_details() prior to this call

RETURN VALUE
  boolean - TRUE if calculation succeeded, FALSE otherwise

  NOTE: If calculation fails, L1 should send OUT_OF_RANGE to RR

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_calc_ho_timing_advance (l1_sc_bs_T *target_cell_details, byte *calculated_timing_advance, gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_get_band

DESCRIPTION
  Determines what band the specified ARFCN is in

DEPENDENCIES
===========================================================================*/
sys_band_T gl1_get_band( ARFCN_T arfcn );

/*===========================================================================

FUNCTION  l1_snr_msg

DESCRIPTION
  Display "F3" debug message containing SNR information.

  The message contains the min, max and mean of the SNRs.

DEPENDENCIES
 None

===========================================================================*/
void l1_snr_msg( dBx16_T *snrs, uint32 num, gas_id_t gas_id );

/*===========================================================================

FUNCTION  gl1_compute_BCCH_mask, gl1_compute_ext_BCCH_mask

DESCRIPTION
  Convert SI mask received from RR into mask to use for selecting TC values.
  The mask from RR is defined in rr_l1.h.  The mask for selecting TC values
  works like this:  if (mask & (1<<TC) != 0) then this block is received.

DEPENDENCIES
  None

===========================================================================*/
uint32 gl1_compute_BCCH_mask( rr_l1_si_type_T si_mask );
uint32 gl1_compute_ext_BCCH_mask( rr_l1_si_type_T si_mask );


/*===========================================================================

FUNCTION  gl1_timetag_c/gps

DESCRIPTION
  This function schedules a timetag message to be sent at the next tick.

===========================================================================*/
void gl1_timetag_cgps (byte l1_state, cgps_CellUpdateEnumType update_type, gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_timetag_c/gps_asleep

DESCRIPTION
  This function sends a timetag request response immediately using timing
  from the sleep clock.

===========================================================================*/
void gl1_timetag_cgps_asleep (byte l1_state,gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_timetag_gps_tick

DESCRIPTION
  This function sends any pending gps timetag events.

===========================================================================*/
void gl1_timetag_gps_tick (gas_id_t gas_id);


extern uint8 get_BSIC_from_sch_data( const uint8 *sch_data );

/*===========================================================================

FUNCTION  gl1_set_band_group

DESCRIPTION
  Informs GL1 of which band group we will be operating in.

===========================================================================*/
void gl1_set_band_group (sys_band_mask_type band_mask, gas_id_t gas_id);

/*===========================================================================

FUNCTION  l1_prime_agc_for_handover

DESCRIPTION
  This function sets the appropriate Handover AGC by selecting the
  target cell value from the Best 6. Additionally, it also now
  has the iRAT Handovers Blind, and CM processed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_prime_agc_for_handover(ARFCN_T arfcn, gas_id_t gas_id);

/*===========================================================================

FUNCTION  l1_interrat_ho_agc

DESCRIPTION
  This function primes the Interrat Handover AGC value.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_interrat_ho_agc(sys_algo_agc_T handover_agc, gas_id_t gas_id);

/*===========================================================================

FUNCTION  l1_interrat_ho_update

DESCRIPTION
  This function sets/resets the iRAT Handover test to prime the AGC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_interrat_ho_update(boolean update, gas_id_t gas_id);



/*===========================================================================

FUNCTION  calculate_txpwr_max_cch

DESCRIPTION
 Optional LB_MS_TXPWR_MAX_CCH parameter has been added in SI 13 rest octets
 The mobile station's maximum output power is based on parameters received in
 system information messages on (P)BCCH while in the (packet) idle mode.
 However, the network has difficulties setting the correct maximum output power
 when packet resources are allocated on a different frequency band than the
 common channels (P)BCCH. Especially because of the different mapping of power
 control levels on different frequency bands, the network cannot set the same,
 or even close to the same dBm value for the maximum output power on each
 frequency band.
RETURN VALUE
 max tx power

SIDE EFFECTS
  None.

===========================================================================*/
uint8 calculate_txpwr_max_cch (ARFCN_T arfcn
                               , boolean rach_pwr_red
                               , gas_id_t gas_id
                              );

/*===========================================================================

FUNCTION  calculate_init_txpwr_max_dedicated

DESCRIPTION
Initial Tx powerl level for dedicated channel

RETURN VALUE
 max tx power

SIDE EFFECTS
  None.

===========================================================================*/
uint8 calculate_init_txpwr_max_dedicated ( gas_id_t gas_id );


/*===========================================================================

FUNCTION  l1_idle_if_gps_notify

DESCRIPTION
  Initiate a Post Cell change notification to GPS, even when GPS is not active.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void l1_idle_if_gps_notify(gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_get_rach_pwr_reduction

DE SCRIPTION
  Get current state of RACH power reduction.

RETURN VALUE
  boolean
  0 - inactive
  1 - active

SIDE EFFECTS
  None.

===========================================================================*/
boolean gl1_get_rach_pwr_reduction(gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_set_rach_pwr_reduction_is_active

DESCRIPTION
  Set current state of RACH power reduction.

RETURN VALUE
  boolean
  0 - inactive
  1 - active

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_set_rach_pwr_reduction(boolean set, gas_id_t gas_id);
/*===========================================================================

FUNCTION  gl1_get_rach_pwr_reduction_pwr_idx

DESCRIPTION
  Get current value of RACH power reduction power index.

RETURN VALUE
  

SIDE EFFECTS
  None.

===========================================================================*/
int16 gl1_get_rach_pwr_reduction_pwr_idx(gas_id_t gas_id);
/*===========================================================================

FUNCTION  gl1_set_rach_pwr_reduction_power_pwr_idx

DESCRIPTION
  Set current value of RACH power reduction power index.

RETURN VALUE
  int16 pwr_idx

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_set_rach_pwr_reduction_pwr_idx(int16 pwr_idx, gas_id_t gas_id);
/*===========================================================================

FUNCTION  gl1_set_rach_pwr_reduction_first_rach

DESCRIPTION
  Set current value of RACH power reduction.

RETURN VALUE
  boolean
  0 - inactive
  1 - active

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_set_rach_pwr_reduction_first_rach(boolean first_rach, gas_id_t gas_id);
/*===========================================================================

FUNCTION  gl1_get_rach_pwr_reduction_first_rach

DESCRIPTION
  Get current value of RACH power reduction first RACH.

RETURN VALUE
  boolean
  0 - inactive
  1 - active

SIDE EFFECTS
  None.

===========================================================================*/
boolean gl1_get_rach_pwr_reduction_first_rach(gas_id_t gas_id);
/*===========================================================================

FUNCTION  gl1_activate_rach_pwr_reduction

DESCRIPTION
  Potentially activates RACH power reductin, depending on the parameters passed in
  PH_RANDOM_ACCESS_REQ.

RETURN VALUE
  boolean
  0 - inactive
  1 - active

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_activate_rach_pwr_reduction(boolean  pwr_reduction_valid,
                                     uint8    pwr_reduction_value,
                                     boolean  emergency_call,
                                     gas_id_t gas_id);
/*===========================================================================

FUNCTION  gl1_check_power_reduction_update 
 
DESCRIPTION
  Checks if a power update should be immediately applied (power reduction disabled)
  or if power reduction ahould continue

RETURN VALUE
  boolean
  0 - inactive
  1 - active

SIDE EFFECTS
  None.

===========================================================================*/
byte gl1_check_pwr_reduction_update (ARFCN_T arfcn, byte txlev, byte current_power_level_in_use, gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_tech_id_to_gas_id

DESCRIPTION
  CxM tech ID to GAS ID mapping function

RETURN VALUE
  GAS ID

SIDE EFFECTS
  None.

===========================================================================*/
gas_id_t gl1_tech_id_to_gas_id( cxm_tech_type tech_id );

/*===========================================================================

FUNCTION  gl1_rf_mode_type_to_cxm_tech_id

DESCRIPTION
  RF mode type to CxM tech ID mapping function

RETURN VALUE
  GAS ID

SIDE EFFECTS
  None.

===========================================================================*/
cxm_tech_type gl1_rf_mode_type_to_cxm_tech_id( rfcom_mode_enum_type rf_mode_type );

void l1_frequency_list_mem_init(gas_id_t gas_id );
void l1_frequency_list_mem_free(gas_id_t gas_id );

uint8 gpl1_get_burst_num_from_FN(uint32 FN,boolean direction,gas_id_t gas_id);
uint8 gpl1_get_burst_num_from_FN_in_IDLE(uint32 FN,boolean direction,gas_id_t gas_id);

#ifdef FEATURE_GPRS_GBTA
void gl1_gbta_read_nv_gbta_support(gas_id_t gas_id);
boolean gl1_gbta_get_nv_gbta_support(gas_id_t gas_id);
#endif /* FEATURE_GPRS_GBTA */

/*===========================================================================

FUNCTION  grm_cs_traffic_abort_callback

DESCRIPTION
  This function handles  an abort callback when holding
  CS state using TRAFFIC reason
  Note the expectation is that this callback will not get called

DEPENDENCIES
  Implements TRM "advanced" unlock callback mechanism

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_cs_traffic_abort_callback( trm_unlock_callback_data *unlock_data_p );
/*===========================================================================

FUNCTION  grm_cs_access_abort_callback

DESCRIPTION
  This function handles  an abort callback when holding
  CS state using ACCESS reason

DEPENDENCIES
  Implements TRM "advanced" unlock callback mechanism

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_cs_access_abort_callback( trm_unlock_callback_data *unlock_data_p );
/*===========================================================================

FUNCTION  grm_ignore_unlock_callback

DESCRIPTION
  This function always ignores an unlock callback

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_ignore_unlock_callback( trm_unlock_callback_data *unlock_data_p );
/*===========================================================================

FUNCTION  grm_ps_access_abort_callback

DESCRIPTION
  This function handles  an abort callback when holding
  PS using ACCESS reason

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_ps_access_abort_callback( trm_unlock_callback_data *unlock_data_p );
/*===========================================================================

FUNCTION  grm_ps_bg_traffic_abort_callback

DESCRIPTION
  This function handles an abort callback when holding
  PS using BG_TRAFFIC reason

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_ps_bg_traffic_abort_callback( trm_unlock_callback_data *unlock_data_p );
/*===========================================================================

FUNCTION  grm_ps_traffic_abort_callback

DESCRIPTION
  This function handles an abort callback when holding
  PS using TRAFFIC reason

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_ps_traffic_abort_callback( trm_unlock_callback_data *unlock_data_p );

void   gl1_handle_mph_start_gprs_idle_mode_req (mph_start_gprs_idle_mode_req_t *message_ptr );
void   gpl1_gprs_setup_start_idle(  void *,gas_id_t gas_id );

boolean gl1_is_current_state_dtm(gas_id_t gas_id);
uint32 gl1_retrieve_bplmn_fail_count(const boolean reset_count, const gas_id_t gas_id);
void   gl1_increment_bplmn_fail_count(const gas_id_t gas_id);
byte   gl1_get_current_sub_state(gas_id_t gas_id);

#ifdef FEATURE_MODEM_CONFIG_REFRESH
void gl1_refresh_nv_items(gas_id_t gas_id);
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

void gl1_print_control_block(uint8 *msg, gas_id_t gas_id);

#ifdef FEATURE_DATA_PDU_DEBUG
void gl1_print_data_block(uint8 *msg, uint8 cs, const gas_id_t gas_id);
#endif /* FEATURE_DATA_PDU_DEBUG */

void gpl1_select_band_to_req_trm(byte band_bitmap,grm_client_enum_t client_id, gas_id_t gas_id);


/*===========================================================================

FUNCTION  gl1_read_lte_prune_cells_below_threshold

DESCRIPTION
  Reads efs item (if present) to set LTE cell rsrp threshold.
  Sets global variables used to flag LTE prune feature.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_read_lte_prune_cells_below_threshold(gas_id_t gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
boolean gl1_is_client_for_bplmn(grm_client_enum_t   grm_client);
grm_client_enum_t gl1_convert_gas_id_to_bplmn_client( gas_id_t gas_id);
#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
trm_els_activity_enum_t convert_sub_reason_to_els_activity(sys_proc_type_e_type sub_reason, gas_id_t gas_id);
#endif /* FEATURE_CRAT_MSIM_TEST_FRAMEWORK */
#endif

#ifdef FEATURE_WLAN_COEX_SW_CXM
void gpl1_update_snr_avg( uint16 snr, gas_id_t gas_id );
void gpl1_reset_snr_avg( gas_id_t gas_id );
void gpl1_stop_snr_avg( gas_id_t gas_id );
void gpl1_enable_snr_avg(gas_id_t gas_id);
boolean gpl1_read_avg_snr( uint32 *filt_metric, uint32 *metric_cnt, gas_id_t gas_id );
#endif

byte l1_override_bs_pa_mfrms(byte BS_PA_MFRMS, gas_id_t gas_id);
#if defined (FEATURE_GSM_SIMULATE_GL1_RF) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
#error code not present
#endif /* FEATURE_GSM_SIMULATE_GL1_RF */

void l1_find_bcch_mcpm_change(boolean find_bcch_start, gas_id_t gas_id);

/* Functions to send Timing update to GPS module */
void gl1_gts_timer_init(gas_id_t gas_id );
void gl1_gts_timer_deactivate( gas_id_t gas_id);
void gl1_gts_send_gts_after_wakeup( gas_id_t gas_id );

#ifdef FEATURE_GSM_RX_DIVERSITY
uint8 L1_get_prxdrx_pwrdiff(gas_id_t gas_id);
#endif

#endif /* L1_UTILS_H */
