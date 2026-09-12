/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         G L 1  M E S S A G E  L A Y E R -  T R A F F I C   C H A N N E L

GENERAL DESCRIPTION
   This module contains the procedures used for communicating on the dl TCH.
   The procedures used herein are intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

   gl1_msg_cfg_dl_tch_facch
   gl1_msg_cfg_ul_tch_facch
   gl1_msg_rx_tch_facch
   gl1_msg_fr_vocoder_rx

   gl1_msg_abort_rx_tch_fr
   gl1_msg_abort_rx_tch_hr



INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_dl_tch.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
07/03/17   mk       CR1097205: explicitly state the array size in all extern definitions
17/02/17   mk       CR1110002 Change Rxqual_H threshold for TCH-EFR BFI computation as per new recommendation
08/12/16   mk       CR1097205: Fixing AGC initialisation when Rxd is getting enabled and enhancing serving cell measurement reporting when Rxd was partially enabled in a SACCH period
02/05/16   mk       CR1010068 Add DTF events in dedicated mode
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
19/03/15   jk       CR810476:VAMOS2 BFI gating algorithm should be applied only for TCH loop back A mode
23/02/15   jk       CR793098:LIF additional logging changes
26/01/15   jk       CR786615: Fixed Logging issue 
16/12/14   jk       CR753423:LIF for Idle,voice and data
24/09/14   jk       CR729382:Enable LIF feature by default
14/10/14   br       CR732289 Change in TCH/HS bfi algorithm to consider prev hard rx_qual
19/08/14   pjr      CR711506 Support for AMR Partial Decode and DTX detection FR19667
17/10/14   jk       CR741194:Initialize LIF params to 0 for L2G measurements
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
25/07/14   hd       CR699946 Update correct metrics cb in the function gl1_msg_cfg_dl_tch_facch() for FR/HR
04/09/14   am       CR682760 Addressing KW GL1 P1 errors
11/07/14   pjr      CR 692177 Added support for RFACCH 2.0
24/07/14   jk       CR699271:GL1 LIF changes for RxD
10/07/14   jk       CR688587:GL1 changes to support LIF feature
08/05/14   cgc      CR642356 fix prototype for tch_rfacch_indicator() added to header.
19/03/14   ws       CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
02/01/14   jk       CR595130: GL1 changes to support VAMOS-II
19/12/13   jk       CR503359 - GL1 changes to support Rx diversity
07/08/13   jk       CR490807:Support for shifted SACCH
11/11/13   ws       CR 573757 Featurise GSMTR-1 registers and Voice services driver
7/16/13    zc       CR470197 Replace memcpy with memscpy
20/06/13   cs       Major Triton TSTS Syncup
24/05/13   ap       CR472264 DSDS: Fix the gas_id problems when the IQ logging is enabled
20/05/13   cs       TSTS Updates
03/04/13   pjr      CR470940 included gl1_hw_vs_g.h
07/03/13   dv       CR461080: Introduction of new GSM VS interface
22/06/12   pg       CR367679: Modify TCH/FS & TCH/HS BFI algs to use pre-mimo SNR
08/06/12   pa       GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
13/03/12   ws       CR 330862 - Change dedicated mode F3 level's and add sacch burst
                    metrics and compact message metrics log packets.
13/03/12   pg       CR330159: Ensure R-FACCH flag is set for all 4 bursts of block
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
25/05/11   ws       CR 288191 - Fix Bug where Rxlev samples from FACCH that pass
                    CRC where not added to AGC
16/12/11   ws       Free floating API change - provide API for NAS GSVOICE to copy vocoder
                    data from/to GL1 to MVS structures
25/05/11   ws       CR 288191 - Fix Bug where Rxlev samples from FACCH that pass
                    CRC where not added to AGC
28/04/11   pg       Fix compiler warnings
26/04/11   ws       REmoved deprecated header voc.h
11/01/11   cgc      Fix compiler warning add NULL #2156
17/01/10   ab       Updated - Resolve Klocwork Buffer Overflow errors
14/01/11   ab       Resolve Klocwork Buffer Overflow errors
17/11/10   dv       CR263536 - CSD callback ptr being overwritten
02/08/10   tjw      Removed unused clk.h (CMI phase 1 re-clean up)
04/06/10   cs       Lint/KW fixes
11/05/10   cja      CR237386 For BEP use burst snr, not pre-mimo for high SNR.
10/09/09   dv       CR193756 - GCF TC 29.3.3.3 failure
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
24/07/09   ws       CR 180476 - Improve performance of Dedicated AGC
28/05/09   dv       CR171098 - Gobi2000 fails TS51.010 14.2.25 and 14.4.31
13/02/09   ap       Lint fixes for high Warning
01/12/08   cs       Add new include gl1_hw_debug.h
01/10/08   cs       Lint fixes
02/09/08   cs       CR155904 remove setting of INVALID_BAND in HR to avoid l1_log errors
28/07/98   hv       Added KxMutex support
28/07/08   ws       Added FEATURE_GSM_8K_QDSP6_AUDIO for chanding Endian of homing sequence
19/03/08   og       Use pre-mimo SNR when producing bep values for enhanced
                    measurements reporting pruposes. Resolves CR141764.
14/01/08   cs       Allow for separate IQ logging and saving
05/12/07   dv       Fix bug that caused mdsp warnings: 0x00000000, 0x00000100
30/10/07   og       Added support for FTM BER calculation in loopback A, B,  C
                    and SRB.
19/09/07   agv      CR128758 - Blocker check in know AGC frames only implemented.
17/09/07   agv      Added I/Q logging to code base.
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
06/07/07   agv      CR123023 - Update fix for AFC/AGC corruption.
27/06/07   agv      CR123023 Fix for AGC corruption, and TT.
28/06/07   cs       Lint cleanup fixes
23/05/07   agv      CR118309 - Large RSSI variation initial fix.
27/03/07   dv       Added FEATURE_GSM_R_FACCH
05/03/07   og       Ensure the half rate 4 burst metrics structure elements are
                    reset before being filled in. Resolves CR 113334.
12/08/05   gfr      Modify gl1_hw_save_iq_samples to work with L4
09/20/05   gfr      Store 8 frames of TCH IQ samples to handle interleaving
08/31/05   gfr      Replace gl1_msg_metrics_rpt with gl1_defs_metrics_rpt to
                    remove need for double copy
08/25/05   gfr      Use the correct decode tag for half-rate, QLint cleanup
08/05/05   gfr      Clean up delay_callbacks flag
07/29/05   gfr      DTM support
06/08/05   dp       Changes to get decoded data in N+1 for FR and HR
05/25/05   gfr      Add support for logging IQ samples
05/18/05   gfr      Lint cleanup
05/05/05   yh       Code clean up
04/21/05   yh       Support for panic reset
04/20/05   yh       Remove use of RF fifo
04/12/05   gfr      Consolidated GSM hw layer parameters
04/08/05   gfr      Combined GSM and GPRS channel types
03/30/05   yh       Retrieve CSD data to dummy buffer if data ptr is NULL.
03/09/05   yh       API change for gl1_hw_get_sync_rx_data() and added bep
                    calculation.
02/22/05   yh       Fixed offset_to_sob calculation
01/05/05   dp       Merged audio optimizations to mainline
08/18/04   yhong    Initial version. Contains only the downlink functions taken
                    from gl1_msg_tch.c
===========================================================================*/

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


#include "comdef.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_debug.h"
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "msg.h"
#include "gl1_msgi_tch.h"
#include "gtmrs.h"
#include "gtmrs_g.h"

#include "ftm.h"
#include "ftm_gsm_ber.h"

#ifdef FEATURE_DATA_GCSD
#include "l1_ds.h"
#include "l1_ds_v.h"
#include "l1_ds_v_g.h"
#endif

#include "sys_cnst.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#ifdef GERAN_L1_ENHANCED_RX
#include "l1_utils.h"
#endif

#ifdef GERAN_L1_HLLL_LNASTATE
#include "l1_utils.h"
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#define  QSH_REF_GOOD_SNR 15*16
#define  QSH_REF_GOOD_RX_PWR -75*16
#endif

#include <string.h>
#include "gl1_mutex.h"
#include <stringl/stringl.h>


extern boolean l1_ded_drx_agc_init[NUM_GERAN_DATA_SPACES];

/* DL HR var for AGG fix. */
uint8  dl_hr_sub_chan[NUM_GERAN_DATA_SPACES];


static gl1_msg_rx_cb_type   dl_tch_callback[NUM_GERAN_DATA_SPACES]   =
                                             {INITIAL_VALUE_S(0)};

#ifdef FEATURE_DATA_GCSD
static tch_facch_decode_rpt **cs_data_rx_rpt_ptr[NUM_GERAN_DATA_SPACES] =
                                { INITIAL_VALUE( NULL ) };
#endif

static uint16 prev_block_rx_qual_s[NUM_GERAN_DATA_SPACES] =
                                { INITIAL_VALUE( 0 ) };

static uint16 prev_block_rx_qual_h[NUM_GERAN_DATA_SPACES] =
                                { INITIAL_VALUE( 0 ) };

static uint16 prev_block_bfi[NUM_GERAN_DATA_SPACES] =
                                { INITIAL_VALUE( 1 ) };

static uint16 avg_snr[NUM_GERAN_DATA_SPACES] =
                                { INITIAL_VALUE( 0 ) };

static boolean blank_loopback_data[NUM_GERAN_DATA_SPACES] =
                                { INITIAL_VALUE( FALSE ) };

static uint32  hr_snrs[NUM_GERAN_DATA_SPACES][4] =
                                { INITIAL_VALUE_S( 0 ) };

/* DL HR defn's for AGG fix. */
#define HR_SUB_CHAN_ZERO   0
#define HR_SUB_CHAN_ONE    1
#define NO_SUB_CHAN        0xFF

#define NUM_TCH_SM 2

/* Number of frames between OTA and burst metrics for dedicated mode */
#define BURST_METRIC_OFFSET 1

/* This is the tag used for retrieving data, corresponds to the index
   where the decode flag was set */
#define DECODE_TAG    3
#define DECODE_TAG_HR 1

/* TCH states */
typedef enum
{
    TCH_INACTIVE,
    TCH_WAITING_FOR_BLOCK_START,
    TCH_RECEIVE_1,
    TCH_RECEIVE_2,
    TCH_RECEIVE_3,
    TCH_RECEIVE_4,
    TCH_RECEIVE_5
#ifdef FEATURE_VAMOS_II
    ,TCH_RECEIVE_6
#endif
} tch_state_type;

/* Structure containing information for a single TCH rx */
typedef struct
{
    /* State variables */
    tch_state_type      state;
    uint8               burst_num;
    uint8               metrics_num;
    boolean             delay_hr_callbacks;

#ifdef FEATURE_DATA_GCSD
    /* To be used only on cstch handler */
    boolean             receive_burst;
    boolean             decode_flag;
    boolean             force_facch;
    tch_facch_decode_rpt *cs_data_rx_rpt;
#endif

    /* Configured rx parameters */
    sys_algo_agc_T      *agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
    sys_algo_agc_T    *agc_ptr_divrx;
#endif /*FEATURE_GSM_RX_DIVERSITY*/
    ARFCN_T             ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD];

    /* Frame layer parameters */
    gl1_hw_rx_signal_type  signal_info;
    gl1_hw_rx_burst_type   burst_info;
    gl1_hw_rx_tags_type    tags[GL1_DEFS_FRAMES_IN_BLK_PERIOD];

    /* Callback parameters */
    gl1_msg_rx_cb_type  callbacks;
    gl1_msg_rx_rpt      rpt;

#ifdef FEATURE_VAMOS_II
    gl1_vamos2_tch_wait_type  vamos2_tch_wait_state;
#endif
} tch_state_machine_type;


/* Structure containing all TCH parameters */
typedef struct
{
    /* Array of state machines */
    uint8                   sm_cnt;
    uint8                   sm_rd_ptr;
    uint8                   sm_wr_ptr;
    tch_state_machine_type  sm[NUM_TCH_SM];
} tch_params_type;

/* Static local storage */
static tch_params_type tch_params[NUM_GERAN_DATA_SPACES];

static uint8 rx_TSC[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_VAMOS_II
static gl1_vamos2_tch_wait_type vamos2_dl_tch_wait_state[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE( NO_WAIT ) };
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
static boolean       init_agc_divrx = FALSE;
#endif

/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/

static boolean tch_fr_rx_handler( boolean start_of_block,
                                  uint32  time_stamp,gas_id_t gas_id );
static boolean tch_hr_rx_handler( boolean start_of_block,
                                  uint32  time_stamp,gas_id_t gas_id );

#ifdef FEATURE_DATA_GCSD
static boolean cstch_rx_handler( boolean start_of_block,
                                 uint32  time_stamp,gas_id_t gas_id );

extern void gcsd_ota_rx_callback(gcsd_buffer_ptr curr_buf,
                                 gcsd_buffer_ptr *next_buf,
                                 gcsd_receive_status status);
#endif

static boolean tch_process_metrics(uint8 sm_num,
                                   uint32 time_stamp,
                                   boolean full_rate_tch,
                                   gl1_hw_channel_type tch_chan,
                                   uint8 hr_sub_chan,
                                   gas_id_t gas_id);


static void tch_process_data_hr(uint8 sm_num,gas_id_t gas_id);

static void tch_process_data_fr(uint8 sm_num,gas_id_t gas_id);

/*===========================================================================

FUNCTION gl1_msg_agc_last_frame_detect

DESCRIPTION
  This function allows MSG Layer to detect the last known good AGC frame for TCH
  FS, and HS

DEPENDENCIES
  none.

RETURN VALUE
  True/False

SIDE EFFECTS
  None
===========================================================================*/
static boolean gl1_msg_agc_last_frame_detect(gl1_hw_channel_type tch_chan,
                                        uint8 hr_sub_chan,gas_id_t gas_id)
{
  boolean result = FALSE;

  switch(tch_chan)
  {
    case GL1_HW_TCH_FS:
    {
      /* Return true if this is the last frame of one of the
       * two SID frames.
       * Note Metrics frame is retrieved 1 frame after the OTA FN.
       */
      if ( (frame_counters[gas_id].FNmod104 == 56) || (frame_counters[gas_id].FNmod104 == 60) )
      {
        result = TRUE;
      }
    }
    break;
    case GL1_HW_TCH_HS:
    {
      switch(hr_sub_chan)
      {
        case HR_SUB_CHAN_ZERO:
        {
        /* Return true if this is the last frame of one of the
         * two SID frames.
         * Note Metrics frame is retrieved 1 frame after the OTA FN
         */
          if (frame_counters[gas_id].FNmod104 == 7  || frame_counters[gas_id].FNmod104 == 59)
          {
            result = TRUE;
          }
        }
        break;
        case HR_SUB_CHAN_ONE:
        {
         /* Return true if this is the last frame of one of the
          * two SID frames.
          * Note Metrics frame is retrieved 1 frame after the OTA FN
          */
          if (frame_counters[gas_id].FNmod104 == 21 || frame_counters[gas_id].FNmod104 == 73)
          {
            result = TRUE;
          }
        }
        break;
        default:
           MSG_GERAN_ERROR_2_G("Incorrect Sub Chan %d %d", tch_chan,hr_sub_chan);
        break;
      }
    }
    break;
    default:
       MSG_GERAN_ERROR_2_G("Incorrect TCH Chan %d %d", tch_chan,hr_sub_chan);
    break;
  }

  return (result);
}

/*===========================================================================

FUNCTION gl1_msg_agc_frame_detect

DESCRIPTION
  This function allows MSG Layer to detect know good AGC frame for TCH
  FS, and HS

DEPENDENCIES
  none.

RETURN VALUE
  True/False

SIDE EFFECTS
  None
===========================================================================*/
static boolean gl1_msg_agc_frame_detect(gl1_hw_channel_type tch_chan,
                                        uint8 hr_sub_chan,gas_id_t gas_id)
{
  boolean result = FALSE;

  switch(tch_chan)
  {
    case GL1_HW_TCH_FS:
    {
      /* Add 1 TDMA frame for comparison since metrics is 1 frame later than OTA burst */
      if (frame_counters[gas_id].FNmod104 >= (52 + BURST_METRIC_OFFSET) && frame_counters[gas_id].FNmod104 <= (59 + BURST_METRIC_OFFSET) )
      {
        result = TRUE;
      }
    }
    break;
    case GL1_HW_TCH_HS:
    {
      switch(hr_sub_chan)
      {
        case HR_SUB_CHAN_ZERO:
        {
         /* Add 1 TDMA frame for comparison since metrics is 1 frame later than OTA burst */
          if (frame_counters[gas_id].FNmod104 == (0 + BURST_METRIC_OFFSET)  || frame_counters[gas_id].FNmod104 == (2 + BURST_METRIC_OFFSET)  ||
              frame_counters[gas_id].FNmod104 == (4 + BURST_METRIC_OFFSET)  || frame_counters[gas_id].FNmod104 == (6 + BURST_METRIC_OFFSET)  ||
              frame_counters[gas_id].FNmod104 == (52 + BURST_METRIC_OFFSET) || frame_counters[gas_id].FNmod104 == (54 + BURST_METRIC_OFFSET)  ||
              frame_counters[gas_id].FNmod104 == (56 + BURST_METRIC_OFFSET) || frame_counters[gas_id].FNmod104 == (58 + BURST_METRIC_OFFSET) )
          {
            result = TRUE;
          }
        }
        break;
        case HR_SUB_CHAN_ONE:
        {
          /* Add 1 TDMA frame for comparison since metrics is 1 frame later than OTA burst */
          if (frame_counters[gas_id].FNmod104 == (14 + BURST_METRIC_OFFSET) || frame_counters[gas_id].FNmod104 == (16 + BURST_METRIC_OFFSET) ||
              frame_counters[gas_id].FNmod104 == (18 + BURST_METRIC_OFFSET) || frame_counters[gas_id].FNmod104 == (20 + BURST_METRIC_OFFSET) ||
              frame_counters[gas_id].FNmod104 == (66 + BURST_METRIC_OFFSET) || frame_counters[gas_id].FNmod104 == (68 + BURST_METRIC_OFFSET) ||
              frame_counters[gas_id].FNmod104 == (70 + BURST_METRIC_OFFSET) || frame_counters[gas_id].FNmod104 == (72 + BURST_METRIC_OFFSET) )
          {
            result = TRUE;
          }
        }
        break;
        default:
           MSG_GERAN_ERROR_2_G("Incorrect Sub Chan %d %d", tch_chan,hr_sub_chan);
        break;
      }
    }
    break;
    default:
       MSG_GERAN_ERROR_2_G("Incorrect TCH Chan %d %d", tch_chan,hr_sub_chan);
    break;
  }

  return (result);
}

/*===========================================================================

FUNCTION gl1_msg_rx_tch_hr_sub_chan

DESCRIPTION
  This function allows PL1 via MSG Layer to signal GSM Half Rate Sub Channel

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_rx_tch_hr_sub_chan(uint8 hr_sub_chan,gas_id_t gas_id)
{
  dl_hr_sub_chan[gas_id] = hr_sub_chan;
}


/*===========================================================================
FUNCTION gl1_msgi_init_dl_tch

DESCRIPTION
   This function initializes data structures used by downlink TCH AMR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_init_dl_tch(gas_id_t gas_id)
{
   uint8 i = 0;
   dl_tch_callback[gas_id].data_cb                   = NULL;
   dl_tch_callback[gas_id].tch_metrics_cb            = NULL;
   dl_tch_callback[gas_id].abort_cb                  = NULL;
   dl_tch_callback[gas_id].log_cb                    = NULL;

   tch_params[gas_id].sm_cnt                         = 0;
   tch_params[gas_id].sm_rd_ptr                      = 0;
   tch_params[gas_id].sm_wr_ptr                      = 0;

   for(i = 0; i < NUM_TCH_SM; i++)
   {
      L1_STATE_CHANGE(tch_params[gas_id].sm[i].state = TCH_INACTIVE, "TCH_INACTIVE");
      tch_params[gas_id].sm[i].callbacks.data_cb        = NULL;
      tch_params[gas_id].sm[i].callbacks.tch_metrics_cb = NULL;
      tch_params[gas_id].sm[i].callbacks.abort_cb       = NULL;
      tch_params[gas_id].sm[i].callbacks.log_cb         = NULL;
      tch_params[gas_id].sm[i].burst_num                = 0;
      tch_params[gas_id].sm[i].metrics_num              = 0;
   }
}



/*===========================================================================

FUNCTION tch_process_metrics

DESCRIPTION
   Retrieves burst metrics for the given state machine. This function is called
   by cstch, tch_fr and tch_hr handlers.

===========================================================================*/
static boolean tch_process_metrics
(
   uint8 sm_num,
   uint32 time_stamp,
   boolean full_rate_tch,
   gl1_hw_channel_type tch_chan,
   uint8 hr_sub_chan,
   gas_id_t gas_id
)
{
   gl1_defs_metrics_rpt metrics, *metrics_ptr;
   tch_state_machine_type *sm = &tch_params[gas_id].sm[sm_num];
   boolean              valid;
   /* Interferer Ind. */
   boolean              blocker = FALSE;
   dBx16_T              burst_avg = 0;
#ifdef FEATURE_GSM_RX_DIVERSITY
   dBx16_T              burst_avg_divrx = 0;
   int32                agc_init_value = (-70*16);
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef GERAN_L1_HLLL_LNASTATE
   gl1_defs_metrics_lnastate gl1_metrics_lnastate;
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GERAN_L1_ENHANCED_RX 
   uint8 NoOfSlots = 0;
   gl1_msg_lif_chan_type channel_type = GL1_INVALID_CHAN;
#endif /*GERAN_L1_ENHANCED_RX*/


   /* Get storage for the metrics, we need to get them even if PL1 is not
      interested in the results */
   metrics_ptr = sm->rpt.metrics[sm->metrics_num];
   if (metrics_ptr == NULL)
   {
      metrics_ptr = &metrics;
   }

   /* Get the burst metrics */
   blocker = gl1_hw_get_sync_rx_metrics(
               metrics_ptr, 
               &sm->tags[sm->metrics_num],
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
               LOG_TCH,
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */
               gas_id);

   valid = (metrics_ptr->powerStatus == RF_IN_RANGE ? TRUE : FALSE);

#ifdef GERAN_L1_ENHANCED_RX
   if(gl1_hw_get_lif_feature_flag(gas_id) && 
      (gl1_msg_get_multi_sim_sys_mode( ) != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
#ifdef FEATURE_SGLTE
      && (gl1_hw_get_sglte_mode(gas_id) == FALSE)
#endif
      )
    {
    NoOfSlots = 1;
    channel_type = (tch_chan == GL1_HW_TCH_FS)?GL1_MSG_CS_TCH:GL1_MSG_CS_TCH_HS;

     gl1_hw_update_lif_metrics(metrics_ptr,NULL,sm->ARFCN[sm->metrics_num],channel_type,NoOfSlots,
                               GSTMR_GET_FN_GERAN(gas_id),UPDATE_LIF_CS_METRICS,gas_id);
    }
#endif /*GERAN_L1_ENHANCED_RX*/


#ifdef FEATURE_GSM_RX_DIVERSITY
   if(gl1_get_rxdiv_agc_start(gas_id))
   {
   if(!init_agc_divrx)
     {
     agc_init_value = gl1_hw_get_dtm_expected_power_divrx(gas_id);
     sys_algo_agc_init(sm->agc_ptr_divrx, (dBx16_T)agc_init_value);
     init_agc_divrx = TRUE;
     }
    }
   else
    {
    init_agc_divrx = FALSE;
    }
#endif

   /* For TCH/FS/HS if there is a interferer feed direct */
   /* to the RF driver, and this is done by no slew flag */
   if (tch_chan == GL1_HW_TCH_FS || tch_chan == GL1_HW_TCH_HS)
   {
     if (gl1_msg_agc_frame_detect(tch_chan, hr_sub_chan,gas_id))
     {
       if (!blocker)
       {
         /* Slew for non Blocker. */
         sm->agc_ptr->srl_enabled = TRUE;
#ifdef FEATURE_GSM_RX_DIVERSITY
         sm->agc_ptr_divrx->srl_enabled = TRUE;
#endif /*FEATURE_GSM_RX_DIVERSITY*/
       }
       else
       {
         /* No slew for Blocker. */
         sm->agc_ptr->srl_enabled = FALSE;
#ifdef FEATURE_GSM_RX_DIVERSITY
    sm->agc_ptr_divrx->srl_enabled = FALSE;
#endif
       }
       burst_avg = sys_algo_agc_average_output(metrics_ptr->pwr_dBm_x16,gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
     if(gl1_get_rxdiv_agc_start(gas_id))
       {
         /* AGC loop for diversity RX should be ON even if RX diveristy is disabled under good channel conditions
                 * If Rx diversity is disabled for the burst, burst RSSI and pwr_dBm_x16_divrx are set to 0. If Rx diversity
                 * is disabled, use received power from primary chain to maintain the AGC loop for diversity chain.
                 */
         if(metrics_ptr->pwr_dBm_x16_divrx != 0)
          {
          burst_avg_divrx = sys_algo_agc_average_output_divrx(metrics_ptr->pwr_dBm_x16_divrx,gas_id);
          }
       else
        {
          /* Under good channel conditions if Rx diversity is disabled, maintain the diversity Rx AGC loop with
                   * the received power from primary chain.
                   */
        burst_avg_divrx = sys_algo_agc_average_output_divrx(metrics_ptr->pwr_dBm_x16,gas_id);
        }
      }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

       /* Only update AGC once all 4 bursts of AGC have been averaged */
       if(gl1_msg_agc_last_frame_detect(tch_chan, hr_sub_chan,gas_id) )
       {
         /* Perform AGC on the burst */
         sys_algo_agc_update(sm->agc_ptr, burst_avg,
                             valid, time_stamp,gas_id );
#ifdef FEATURE_GSM_RX_DIVERSITY
         if(burst_avg_divrx != 0)
           {
       /* Perform AGC on the burst for diversity Rx */
       sys_algo_agc_update(sm->agc_ptr_divrx, burst_avg_divrx,
                     valid, time_stamp,gas_id);
          }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
       }
     }
   }
   /* CSD9.6/14/4 if  there is an interferer feed direct */
   /* to the RF driver, and this is done by no slew flag */
   else if (tch_chan == GL1_HW_TCH_F96_AND_FACCH)
   {
     if (!blocker)
     {
       /* Slew for Non Blocker. */
       sm->agc_ptr->srl_enabled = TRUE;

       /* Perform AGC on the burst */
       sys_algo_agc_update(sm->agc_ptr, metrics_ptr->pwr_dBm_x16,
                           valid, time_stamp,gas_id );
#ifdef FEATURE_GSM_RX_DIVERSITY
     if(gl1_get_rxdiv_agc_start(gas_id))
       {
       sm->agc_ptr_divrx->srl_enabled = TRUE;

         /* AGC loop for diversity RX should be ON even if RX diveristy is disabled under good channel conditions
                 * If Rx diversity is disabled for the burst, burst RSSI and pwr_dBm_x16_divrx are set to 0. If Rx diversity
                 * is disabled, use received power from primary chain to maintain the AGC loop for diversity chain.
                 */
       if(metrics_ptr->pwr_dBm_x16_divrx != 0)
         {
       /* Perform AGC on the burst */
       sys_algo_agc_update(sm->agc_ptr_divrx, metrics_ptr->pwr_dBm_x16_divrx,
                   valid, time_stamp,gas_id);
         }
       else
         {
         sys_algo_agc_update(sm->agc_ptr_divrx, metrics_ptr->pwr_dBm_x16,
                   valid, time_stamp,gas_id);
         }
      }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
     }
     else
     {
       /* No slew for Blocker. */
       sm->agc_ptr->srl_enabled = FALSE;

       /* Perform AGC on the burst */
       sys_algo_agc_update(sm->agc_ptr, metrics_ptr->pwr_dBm_x16,
                           valid, time_stamp,gas_id );
#ifdef FEATURE_GSM_RX_DIVERSITY
    if(gl1_get_rxdiv_agc_start(gas_id))
      {
      sm->agc_ptr_divrx->srl_enabled = FALSE;

        /* AGC loop for diversity RX should be ON even if RX diveristy is disabled under good channel conditions
                * If Rx diversity is disabled for the burst, burst RSSI and pwr_dBm_x16_divrx are set to 0. If Rx diversity
                * is disabled, use received power from primary chain to maintain the AGC loop for diversity chain.
                */
      if(metrics_ptr->pwr_dBm_x16_divrx != 0)
        {
        /* Perform AGC on the burst */
        sys_algo_agc_update(sm->agc_ptr_divrx, metrics_ptr->pwr_dBm_x16_divrx,
                  valid, time_stamp,gas_id);
        }
      else
        {
        sys_algo_agc_update(sm->agc_ptr_divrx, metrics_ptr->pwr_dBm_x16,
                  valid, time_stamp,gas_id);
        }
      }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
     }
   }

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
   /* Since TCH is interleaved over two blocks we need to store the
      IQ samples in two places */
   /* Only collect samples if trigger set. */
   if ( gl1_msg_trigger_iq( gas_id ) )
   {
     gl1_hw_get_iq_samples
      (
        sm->burst_info.channel_type,
        sm_num,
        (full_rate_tch ? 4 : 2) + sm->metrics_num,
        sm->burst_info.tsc,
        gas_id
      );

     gl1_hw_get_iq_samples
      (
        sm->burst_info.channel_type,
        !sm_num,
        sm->metrics_num,
        sm->burst_info.tsc,
        gas_id
      );
   }
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */

   /* Fill out the BEP values in data ptr */
   sm->rpt.data->tch_facch.hdr.bep[sm->metrics_num] = gl1_msg_tch_snr_to_bep(metrics_ptr,gas_id);

   /* Fill in reset of fields if PL1 is interested */
   if (sm->callbacks.tch_metrics_cb != NULL)
   {
      metrics_ptr->arfcn = sm->ARFCN[sm->metrics_num];
      metrics_ptr->valid = TRUE;
      metrics_ptr->time_stamp = time_stamp;

      /* Figure out the offset_to_sob */
      if (full_rate_tch)
      {
         /* Calculation of offset_to_sob: The burst will be received
          * in state RECEIVE_6, which in numbers, it is equivalent to
          * 7. The +1 is due to the increment of the state prior to
          * entering this function. Example: For the first frame, we
          * are 4 frames from the start of burst. The sm->state is
          * 4 (it is actually 3, but incremented before this function).
          * So, 7 - 4 + 1 = 4 */
          if (sm->state == TCH_INACTIVE )
          {
             metrics_ptr->offset_to_sob = 1;
          }
          else
          {
#ifdef FEATURE_VAMOS_II
             metrics_ptr->offset_to_sob = (sm->vamos2_tch_wait_state != NO_WAIT)?(8 - (int32)sm->state + 1):(7 - (int32)sm->state + 1);
#else
             metrics_ptr->offset_to_sob = ( 7 - (int32)sm->state + 1 );
#endif
          }
      }
      else
      {
         /* If it is in TCH_INACTIVE, it was called during TCH_RECEIVE_5.
          * The internal state is set to TCH_INACTIVE at this point, so,
          * the offset is 1 */
          if (sm->state == TCH_INACTIVE )
          {
             metrics_ptr->offset_to_sob = 1;
          }
          else
          {
#ifdef FEATURE_VAMOS_II
             metrics_ptr->offset_to_sob = (sm->vamos2_tch_wait_state != NO_WAIT)?(7 - (int32)sm->state + 1):(6 - (int32)sm->state + 1);
#else
             metrics_ptr->offset_to_sob = ( 6 - (int32)sm->state + 1 );
#endif
          }

          /* We get data in N+1 or N+2 so the offset to sob needs to be
             adjusted accordingly */
          if ( sm->delay_hr_callbacks )
          {
             metrics_ptr->offset_to_sob++;
          }
      }
   }

   if( GL1_DEFS_LOOPBACK_TYPE_C == loopback_type[gas_id] )
   {
     /* Pack the hard decisions in 8-bit words
      * instead of 16-bit words.
      * We actually need only 15 8bit words but make the array 16
      * in size, makes the copying loop easier.
      */
     ftm_gsm_ber_rx_data_ind_type    rx_data_ind;

     static uint8  hard_decision_8bit_word[NUM_GERAN_DATA_SPACES][ 16 ] =
                     { INITIAL_VALUE_S( 0 ) };

     /* local loop counter */
     uint32 count = 0;

     while( count < 8 )
     {
       /* copy from field n to fields 2n and 2n+1 */
       /*   bits d(k) . . . d(k+7) d(k+8) . . . d(k+15) in one 16 bit word */
       /*   store as d(k) . . . d(k+7) as 1 byte followed by
        *            d(k+8) . . . d(k+15) in next byte
        *   k = 16*n
        */
       hard_decision_8bit_word[gas_id][ (count<<1) ]       = (uint8) (metrics_ptr->hard_decisions[ count ]>>8);
       hard_decision_8bit_word[gas_id][ ((count<<1) + 1) ] = (uint8) metrics_ptr->hard_decisions[ count ];

       count++;
     }

     /* Mask off bits 4 - 7 (inclusive) of the 15th 8bit word (index 14)
      * and all bits in the 16th 8bit word (index 15).
      */
     hard_decision_8bit_word[gas_id][ 14 ] &= 0xF0;

     hard_decision_8bit_word[gas_id][ 15 ] &= 0x0;

     /* We might need to remove the stealing flag bits
      * Do not bother yet.
      */

     rx_data_ind.fn       = GSTMR_GET_FN_GERAN(gas_id);                  /* fn */
     rx_data_ind.slot_num = 0;                               /* timeslot */
     rx_data_ind.bfi      = TRUE;                            /* bfi boolean     */
     rx_data_ind.num_bits = 116;                             /* uint16 num bits */
     rx_data_ind.data     = &(hard_decision_8bit_word[gas_id][0]);   /* uint8* data     */


     /* Send the hard decisions to FTM */
     ftm_gsm_ber_send_rx_data_to_ftm(  &rx_data_ind );
   }

   /* Same story for the log callback, except that we call */
   /* the callback after each frame.                       */
   if (sm->callbacks.log_cb)
   {
      sm->callbacks.log_cb();
   }

#ifdef GERAN_L1_HLLL_LNASTATE
   if(gl1_hw_get_sawless_HLLL_support(metrics_ptr->arfcn.band,gas_id))
     {
     gl1_metrics_lnastate.NoOfChannels              = 1;
     gl1_metrics_lnastate.gsm_metrics[0].arfcn      = metrics_ptr->arfcn;
     gl1_metrics_lnastate.gsm_metrics[0].rx_lvl_dbm = metrics_ptr->pwr_dBm_x16;
     gl1_metrics_lnastate.gsm_metrics[0].snr        = metrics_ptr->snr;
     gl1_metrics_lnastate.gsm_metrics[0].jdet_value = metrics_ptr->jdet_value;
     if(gl1_is_current_state_dtm(gas_id) == TRUE)
       {
       gl1_hw_update_dtm_metrics(&gl1_metrics_lnastate,GSTMR_GET_FN_GERAN(gas_id),UPDATE_CS_METRICS,gas_id);
       }
     else
       {
       gl1_hw_rf_burst_metrics(&gl1_metrics_lnastate,gas_id);
       }
     }
#endif

   /* We are done with these metrics */
   sm->metrics_num++;

   return(blocker);

}/*lint !e715*/

/*===========================================================================

FUNCTION tch_process_data_hr

DESCRIPTION
   Get the data that is already available for the given state machine. This is
   called when the state machine is in the state 5 of a tch_hr.

===========================================================================*/
static void tch_process_data_hr
(
    uint8 sm_num,
    gas_id_t gas_id
)
{
    uint16 diff_c1, diff_c1c2;
    tch_state_machine_type *sm = &tch_params[gas_id].sm[sm_num];	
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH	
    uint8 i, all_good_snr, all_good_rxpwr;
    tch_facch_decode_rpt *rpt_data_ptr;
#endif

    /* This is not a typo, we retrieve MAX_TCH_FS_OCTETS as
       a safety measure since the number of FACCH octets is >
       MAX_TCH_HS_OCTETS. We would not be retrieving all
       FACCH octets in that case
    */
    gl1_hw_get_sync_rx_data( sm->burst_info.channel_type,
                             &sm->tags[DECODE_TAG_HR],
                             &sm->rpt.data->tch_facch.hdr,
                             sm->rpt.data->tch_facch.data,
                             (uint16)MAX_TCH_FS_OCTETS,
                             gas_id);


    /* Map the HW layer channel type enum to the MSG layer*/
    sm->rpt.data->tch_facch.hdr.chan =  (int16)
      (((gl1_hw_channel_type)(sm->rpt.data->tch_facch.hdr.chan) == GL1_HW_FACCH) ?
        GL1_MSG_FACCH : GL1_MSG_TCH_HS);

    /*
       mdsp returns NUM1S_C1C2 and NUM1S_C1.  Convert to DIFF_C1 and DIFF_C1C2
    */
    diff_c1 = 62 - (sm->rpt.data->tch_facch.hdr.sid & 0x00FF );
    diff_c1c2 = 79 - ((sm->rpt.data->tch_facch.hdr.sid & 0xFF00 ) >> 8 );
    /*Compute SID flag from DIFF_C1, DIFF_C1C2 */
    sm->rpt.data->tch_facch.hdr.sid = gl1_msg_compute_sid
                                       (
                                         diff_c1,
                                         diff_c1c2,
                                         (uint8)sm->rpt.data->tch_facch.hdr.voicing_mode
                                       );
  if ( (gl1_msg_chan_type)(sm->rpt.data->tch_facch.hdr.chan) == GL1_MSG_FACCH )
    {
        sm->rpt.data->tch_facch.hdr.bfi = 1;
		sm->rpt.data->tch_facch.hdr.ufi = 1;
    }
	else
	{
    /* Compute BFI and UFI.  This has now been moved to the ARM */
    gl1_msg_compute_bfi_ufi(sm->rpt.data->tch_facch.hdr.good_data,
#ifdef FEATURE_VAMOS_II
                            sm->rpt.data->tch_facch.hdr.vamospaired,
                            loopback_type[gas_id],
                            gas_id,
#endif
                            sm->rpt.data->tch_facch.hdr.rx_qual_s,prev_block_rx_qual_s[gas_id],
                            prev_block_bfi[gas_id], &(sm->rpt.data->tch_facch.hdr.bfi),
                            &(sm->rpt.data->tch_facch.hdr.ufi), avg_snr[gas_id],
                            sm->rpt.data->tch_facch.hdr.rx_qual,
							prev_block_rx_qual_h[gas_id]);
   }
    /*
       Store previous RXQUAL,BFI and UFI.  This is uses in the BFI/UFI algorithm
    */
    prev_block_rx_qual_s[gas_id] = sm->rpt.data->tch_facch.hdr.rx_qual_s;
	prev_block_rx_qual_h[gas_id] = sm->rpt.data->tch_facch.hdr.rx_qual;
    prev_block_bfi[gas_id] = sm->rpt.data->tch_facch.hdr.bfi;

    if (sm->rpt.data->tch_facch.hdr.chan == (int16)GL1_MSG_FACCH)
    {
        sm->rpt.data->tch_facch.hdr.sid = 0;
    }

    /* Only loopback A,B,D,E,F implemented in s/w. C still handled my mdsp */
    if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_A )
    {
        if ( sm->rpt.data->tch_facch.hdr.bfi )
        {
            blank_loopback_data[gas_id] = TRUE;
        }
    }
    else if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_D )
    {
        if ( sm->rpt.data->tch_facch.hdr.bfi == 1 ||
             sm->rpt.data->tch_facch.hdr.ufi == 1 )
        {
            blank_loopback_data[gas_id] = TRUE;
        }
    }
    else if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_E )
    {
        if ( sm->rpt.data->tch_facch.hdr.sid == 0 )
        {
            blank_loopback_data[gas_id] = TRUE;
        }
    }
    else if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_F )
    {
        if ( sm->rpt.data->tch_facch.hdr.sid == 0 ||
             sm->rpt.data->tch_facch.hdr.sid == 1 ||
             sm->rpt.data->tch_facch.hdr.bfi == 1 ||
             sm->rpt.data->tch_facch.hdr.ufi == 1 )
        {
            blank_loopback_data[gas_id] = TRUE;
        }
    }

    if ( blank_loopback_data[gas_id] )
    {
        memset(gl1_loopback_data[gas_id], NULL, MAX_TCH_HS_OCTETS );
    }
    else
    {
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000)  /*lint !e553*/
        /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
        {
            uint32 i;
            for (i=0; i<MAX_TCH_HS_OCTETS; i++)
            {
                gl1_loopback_data[gas_id][i] = sm->rpt.data->tch_facch.data[i+2];
            }
        }
#else
        /*
         * Source is 33 octets, dest is 38 octets, and len(MAX_TCH_HS_OCTETS) is 16
         */
        memscpy(gl1_loopback_data[gas_id],  sizeof(gl1_loopback_data[gas_id]), &(sm->rpt.data->tch_facch.data[2]),
               MAX_TCH_HS_OCTETS); /*lint !e420*/
#endif
    }

    /* We are reporting in offset 6
     ** eliminate the hardcode and tie this tothe reporting
     ** frame, i.e., rx_state[machine]
     ** we report after 4 bursts + 1 frame to decode
     */
    sm->rpt.data->tch_facch.offset_to_sob = ( sm->delay_hr_callbacks ? 4 : 3 );
	
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    rpt_data_ptr = (tch_facch_decode_rpt*)sm->rpt.data;
    all_good_snr = TRUE;
    all_good_rxpwr= TRUE;

    for(i=0;i<2;i++)
    {
       if(gl1_msg_SNR_to_dBx16(sm->rpt.metrics[i]->snr) < QSH_REF_GOOD_SNR)
       {
         all_good_snr = FALSE;
       }
       if(sm->rpt.metrics[i]->pwr_dBm_x16 < QSH_REF_GOOD_RX_PWR)
       {
         all_good_rxpwr = FALSE;
       } 
    }

    if((rpt_data_ptr->hdr.rx_qual < 25000) && all_good_rxpwr)
    {
        gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_HRXQUAL_DROP_GOOD_RXPWR);
    }

    if(!rpt_data_ptr->hdr.good_data)
    {	  
        if(all_good_snr && all_good_rxpwr)
        {
           gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_CRC_FAIL_GOOD_SNR_RXPWR);
        }
        else if(!all_good_snr && all_good_rxpwr)
        {
           gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_CRC_FAIL_BAD_SNR_GOOD_RXPWR);
        }
        else if(all_good_snr && !all_good_rxpwr)
        {
           gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_CRC_FAIL_GOOD_SNR_BAD_RXPWR);
        }
    }
#endif
    if (sm->callbacks.data_cb)
    {
        sm->callbacks.data_cb( sm->rpt.data,gas_id);
    }
    else
    {
        MSG_GERAN_ERROR_0_G("No dl callback registered");
    }
}



/*===========================================================================

FUNCTION tch_process_data_fr

DESCRIPTION
   Get the data that is already available for the given state machine. This is
   called when the state machine is in the state 6 of a tch_fr.

===========================================================================*/
static void tch_process_data_fr
(
   uint8 sm_num,
   gas_id_t gas_id
)
{
    tch_state_machine_type *sm = &tch_params[gas_id].sm[sm_num];

    static ftm_gsm_ber_rx_data_ind_type  rx_data_ind[NUM_GERAN_DATA_SPACES];
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    uint8 i, all_good_snr, all_good_rxpwr;
    tch_facch_decode_rpt *rpt_data_ptr;
#endif

    /* Now the data is available, go get it */
    gl1_hw_get_sync_rx_data( sm->burst_info.channel_type,
                             &sm->tags[DECODE_TAG],
                             &sm->rpt.data->tch_facch.hdr,
                             sm->rpt.data->tch_facch.data,
                             (uint16)MAX_TCH_FS_OCTETS,
                             gas_id);


    /* Map the HW layer channel type enum to the MSG layer*/
    sm->rpt.data->tch_facch.hdr.chan = (int16)
      (((gl1_hw_channel_type)(sm->rpt.data->tch_facch.hdr.chan) == GL1_HW_FACCH) ?
       GL1_MSG_FACCH : GL1_MSG_TCH);

    if ( (gl1_msg_chan_type)(sm->rpt.data->tch_facch.hdr.chan) == GL1_MSG_FACCH )
    {
        sm->rpt.data->tch_facch.hdr.bfi = 1;
    }
    else
    {
        /* Compute BFI. ARM is doing BFI calculation now.*/
        gl1_msg_compute_bfi( sm->rpt.data->tch_facch.hdr.good_data,
#ifdef FEATURE_VAMOS_II
                             sm->rpt.data->tch_facch.hdr.vamospaired,
                             loopback_type[gas_id],
                             gas_id,
#endif
                             sm->rpt.data->tch_facch.hdr.rx_qual_s,
                             prev_block_rx_qual_s[gas_id],
                             sm->rpt.data->tch_facch.hdr.rx_qual,
                             prev_block_bfi[gas_id], avg_snr[gas_id],
                             &(sm->rpt.data->tch_facch.hdr.bfi),
                             current_chan[gas_id] );
    }

    prev_block_rx_qual_s[gas_id] = sm->rpt.data->tch_facch.hdr.rx_qual_s;
    prev_block_bfi[gas_id] = sm->rpt.data->tch_facch.hdr.bfi;

    /* Only loopback A & B implemented in s/w. C still handled my mdsp */
    if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_B )
    {
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
        /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
        {
            uint32 i;
            for (i=0; i<MAX_TCH_FS_OCTETS; i++)
            {
                gl1_loopback_data[gas_id][i] = sm->rpt.data->tch_facch.data[i];
            }
        }
#else
        memscpy
         (
           (uint8 *)gl1_loopback_data[gas_id],
           sizeof(gl1_loopback_data[gas_id]),
           (uint8 *)sm->rpt.data->tch_facch.data,
           MAX_TCH_FS_OCTETS
         );
#endif

        /* If we are in loopback A or B then report the 260 bits
         * to FTM.
         */

        rx_data_ind[gas_id].fn       = GSTMR_GET_FN_GERAN(gas_id);                  /* fn */
        rx_data_ind[gas_id].slot_num = 0;                               /* timeslot */
        rx_data_ind[gas_id].bfi      = FALSE;                           /* bfi boolean -- fix to false to assume good data */
        rx_data_ind[gas_id].num_bits = 260;                             /* uint16 num bits */
        rx_data_ind[gas_id].data     = &(gl1_loopback_data[gas_id][0]);         /* uint8* data     */

        /* Send the hard decisions to FTM */
        ftm_gsm_ber_send_rx_data_to_ftm(  &rx_data_ind[gas_id] );

    }
    else if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_A )
    {
        if ( sm->rpt.data->tch_facch.hdr.bfi )
        {
            memset((uint8 *) gl1_loopback_data[gas_id], NULL, MAX_TCH_FS_OCTETS );
        }
        else
        {
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
            /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
            {
                uint32 i;
                for (i=0; i<MAX_TCH_FS_OCTETS; i++)
                {
                    gl1_loopback_data[gas_id][i] = sm->rpt.data->tch_facch.data[i];
                }
            }
#else
            memscpy((uint8 *)gl1_loopback_data[gas_id],
                    sizeof(gl1_loopback_data[gas_id]),
                   (uint8 *)(sm->rpt.data->tch_facch.data),
                   MAX_TCH_FS_OCTETS);
#endif
        }

        /* If we are in loopback A or B then report the 260 bits
         * to FTM.
         */

        rx_data_ind[gas_id].fn       = GSTMR_GET_FN_GERAN(gas_id);                  /* fn */
        rx_data_ind[gas_id].slot_num = 0;                               /* timeslot */
        rx_data_ind[gas_id].bfi      = sm->rpt.data->tch_facch.hdr.bfi; /* bfi boolean     */
        rx_data_ind[gas_id].num_bits = 260;                             /* uint16 num bits */
        rx_data_ind[gas_id].data     = &(gl1_loopback_data[gas_id][0]);         /* uint8* data     */

        /* Send the hard decisions to FTM */
        ftm_gsm_ber_send_rx_data_to_ftm(  &rx_data_ind[gas_id] );

    }
    /* We are reporting in offset 6
    ** eliminate the hardcode and tie this tothe reporting
    ** frame, i.e., rx_state[machine]
    ** we report after 4 bursts + 1 frame to decode
    */
    sm->rpt.data->tch_facch.offset_to_sob = 4;
	
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    rpt_data_ptr = (tch_facch_decode_rpt*)sm->rpt.data;
    all_good_snr = TRUE;
    all_good_rxpwr= TRUE;

    for(i=0;i<4;i++)
    {
       if(gl1_msg_SNR_to_dBx16(sm->rpt.metrics[i]->snr) < QSH_REF_GOOD_SNR)
       {
         all_good_snr = FALSE;
       }
       if(sm->rpt.metrics[i]->pwr_dBm_x16 < QSH_REF_GOOD_RX_PWR)
       {
         all_good_rxpwr = FALSE; 		  
       }
    }

    if((rpt_data_ptr->hdr.rx_qual < 25000) && all_good_rxpwr)
    {
        gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_HRXQUAL_DROP_GOOD_RXPWR);	
    }

    if(!rpt_data_ptr->hdr.good_data)
    {		  
        if(all_good_snr && all_good_rxpwr)
        {
           gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_CRC_FAIL_GOOD_SNR_RXPWR);
        }
        else if(!all_good_snr && all_good_rxpwr)
        {
           gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_CRC_FAIL_BAD_SNR_GOOD_RXPWR);
        }
        else if(all_good_snr && !all_good_rxpwr)
        {
           gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_CRC_FAIL_GOOD_SNR_BAD_RXPWR);
        }		
    }
#endif

    if (sm->callbacks.data_cb)
    {
        sm->callbacks.data_cb( sm->rpt.data,gas_id);
    }
    else
    {
        MSG_GERAN_ERROR_0_G("No dl callback registered");
    }
}



/*===========================================================================

FUNCTION dl_tch_add_storage

DESCRIPTION
  This function allocates the next available state machine storage unit
  and prepares it with the given parameters.

===========================================================================*/
static void dl_tch_add_storage
(
   ARFCN_T              ARFCN[4],
   sys_algo_agc_T       *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
   sys_algo_agc_T   *agc_ptr_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   gl1_msg_rx_rpt       *rpt,
   gl1_hw_channel_type  chan,
   boolean              facch_only,
   gas_id_t             gas_id
)
{
   tch_state_machine_type *sm;
   uint32 i;

   /* Get the next state machine storage */
   sm = &tch_params[gas_id].sm[tch_params[gas_id].sm_wr_ptr];
   tch_params[gas_id].sm_wr_ptr = (tch_params[gas_id].sm_wr_ptr + 1) % NUM_TCH_SM;
   tch_params[gas_id].sm_cnt++;

   /* Fill in the state machine parameters */
   agc_ptr->arfcn.num    = 0xFFFF;
   L1_STATE_CHANGE(sm->state             = TCH_WAITING_FOR_BLOCK_START, "TCH_WAITING_FOR_BLOCK_START");
   sm->agc_ptr           = agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
   sm->agc_ptr_divrx   = agc_ptr_divrx;
#endif
   sm->rpt.data          = rpt->data;
   sm->callbacks         = dl_tch_callback[gas_id];

   /* If loopback is active, get half-rate decoded data in N+2.  This will
    * ensure that (as a result of the PL1 scheduling mechanism) the
    * decoded data for the current block does not get looped-back
    * immediately without the usual one block delay.  This scenario
    * happens in the block just before the SACCH or IDLE frame.
    */
   sm->delay_hr_callbacks = (loopback_type[gas_id] == GL1_DEFS_NO_LOOPBACK ? FALSE : TRUE);

   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
       sm->ARFCN[i] = ARFCN[i];
       sm->rpt.metrics[i] = rpt->metrics[i];
   }

   /* Set static frame layer parameters */
   memset(&sm->signal_info, 0, sizeof(sm->signal_info));
   memset(&sm->burst_info, 0, sizeof(sm->burst_info));
   sm->burst_info.tsc                = rx_TSC[gas_id];
   sm->burst_info.search_width       = GL1_HW_TCH_SEARCH_WIDTH;
   sm->burst_info.look_for_facch     = facch_only;
   sm->burst_info.channel_type       = chan;
   sm->signal_info.tn                = gl1_msg_get_cs_timeslot(gas_id);
}



/*===========================================================================

FUNCTION gl1_msg_rx_tch_facch_fr

DESCRIPTION
  This function is used to receive a message on either the TCH or the FACCH.
  The message contents should span 4 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.   If data to be received is for
  frame #4,5,6,7, then this function should be completed before start of
  frame #3, ie called in frames 0,1,2.

DEPENDENCIES
  gl1_msg_cfg_dl_tch_facch() ,must have been called at some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_rx_tch_facch_fr
(
   ARFCN_T              ARFCN[4],
   sys_algo_agc_T       *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
   sys_algo_agc_T *agc_ptr_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   gl1_msg_rx_rpt       *rpt,
#ifdef FEATURE_VAMOS_II
   gl1_vamos2_tch_wait_type  vamos2_fr_tch_wait_state,
#endif
   gas_id_t             gas_id
)
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_msg_rx_tch_facch_fr: Return ");
   return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
   /* Create new state machine storage */
   dl_tch_add_storage(ARFCN, agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                      agc_ptr_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                      rpt, current_chan[gas_id], FALSE,
                      gas_id);

#ifdef FEATURE_VAMOS_II
   vamos2_dl_tch_wait_state[gas_id] = vamos2_fr_tch_wait_state;
#endif

   /* Add the handler if necessary */
   if (tch_params[gas_id].sm_cnt == 1)
   {
       gl1_msgi_add_ft_handler(tch_fr_rx_handler, GL1_MSG_FT_HDLR_DL,gas_id);
   }
}



/*===========================================================================

FUNCTION gl1_msg_rx_tch_facch_hr

DESCRIPTION
  This function is used to receive a message on either the TCH or the FACCH.
  The message contents should span 4 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.   If data to be received is for
  frame #4,5,6,7, then this function should be completed before start of
  frame #3, ie called in frames 0,1,2.

DEPENDENCIES
  gl1_msg_cfg_dl_tch_facch() ,must have been called at some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_rx_tch_facch_hr
(
   ARFCN_T              ARFCN[4],
   sys_algo_agc_T       *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
   sys_algo_agc_T       *agc_ptr_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   gl1_msg_rx_rpt       *rpt,
   boolean              facch_only,
#ifdef FEATURE_VAMOS_II
   gl1_vamos2_tch_wait_type  vamos2_hr_tch_wait_state,
#endif
   gas_id_t             gas_id
)
{
   /* Create new state machine storage */
   dl_tch_add_storage(ARFCN, agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                      agc_ptr_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                      rpt, GL1_HW_TCH_HS, facch_only,
                      gas_id);

#ifdef FEATURE_VAMOS_II
   vamos2_dl_tch_wait_state[gas_id] = vamos2_hr_tch_wait_state;
#endif

   /* Add the handler if necessary */
   if (tch_params[gas_id].sm_cnt == 1)
   {
      gl1_msgi_add_ft_handler(tch_hr_rx_handler, GL1_MSG_FT_HDLR_DL,gas_id);
   }
}


#ifdef FEATURE_DATA_GCSD
/*===========================================================================

FUNCTION gl1_msg_rx_cstch_facch

DESCRIPTION
  This function is used to receive a message on either the TCH or the FACCH.
  The message contents should span 4 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.   If data to be received is for
  frame #4,5,6,7, then this function should be completed before start of
  frame #3, ie called in frames 0,1,2.

DEPENDENCIES
  gl1_msg_cfg_dl_tch_facch() ,must have been called at some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_rx_cstch_facch
(
   ARFCN_T               ARFCN[4],
   sys_algo_agc_T        *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
   sys_algo_agc_T        *agc_ptr_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   tch_facch_decode_rpt  **cs_rpt,
   gl1_msg_rx_rpt        *facch_rpt,
   gas_id_t              gas_id
)
{
   /* Create new state machine storage */
   dl_tch_add_storage(ARFCN, agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                      agc_ptr_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                      facch_rpt, current_chan[gas_id], FALSE,
                      gas_id);

   /* Store the pointer to where cs data rx buffer pointer will be */
   cs_data_rx_rpt_ptr[gas_id] = cs_rpt;

   /* Add the handler if necessary */
   if (tch_params[gas_id].sm_cnt == 1)
   {
      gl1_msgi_add_ft_handler(cstch_rx_handler, GL1_MSG_FT_HDLR_DL,gas_id);
   }

}
#endif /* FEATURE_DATA_GCSD */


/*===========================================================================

FUNCTION gl1_msg_cfg_dl_tch_facch

DESCRIPTION
  This function is used to configure the TSC and callbacks for the TCH.  The
  callbacks will be invoked by the frame tick handler as required.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_cfg_dl_tch_facch ( uint8 TSC,
                                gl1_msg_rx_cb_type *cb,gas_id_t gas_id  )
{
    if (!cb)
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL cb pointer");
        return;
    }

    rx_TSC[gas_id] = TSC;
    dl_tch_callback[gas_id].data_cb        = cb->data_cb;
    dl_tch_callback[gas_id].tch_metrics_cb = cb->tch_metrics_cb;
    dl_tch_callback[gas_id].log_cb         = cb->log_cb;

    prev_block_rx_qual_s[gas_id] = 0;
    prev_block_rx_qual_h[gas_id] = 0;
    prev_block_bfi[gas_id] = 1;
}

#ifdef FEATURE_DATA_GCSD
/*===========================================================================

FUNCTION gl1_msg_cs_data_rx

DESCRIPTION
  This function is used to transfer data to or from the GSM circuit
  switched data handler.  These functions may be called by the task level.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_cs_data_rx (tch_facch_decode_rpt *curr_rpt,
                         tch_facch_decode_rpt **next_rpt_buf,
                         int32 status,gas_id_t gas_id)
{
    gcsd_buffer_ptr current_buf;
    static gcsd_buffer_ptr next_buf[NUM_GERAN_DATA_SPACES] =
                             { INITIAL_VALUE( NULL ) };

    if (!next_rpt_buf)
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL next_rpt_buf ptr");
        return;
    }

    if (curr_rpt == NULL)
    {
        current_buf = NULL;
    }
    else
    {
        current_buf = (gcsd_buffer_ptr) (uint8 *) curr_rpt;
        current_buf->data_offset =
        (uint16) ((uint8 *) (curr_rpt->data) - (uint8 *) curr_rpt);
    }

    if (current_buf != next_buf[gas_id])
    {
        MSG_GERAN_HIGH_0_G("gl1_msg_cs_data_rx: msg mismatch");
    }

    gcsd_ota_rx_callback (current_buf, &next_buf[gas_id], (gcsd_receive_status)status);

    *next_rpt_buf = (tch_facch_decode_rpt *) (uint8 *) next_buf[gas_id];
}
#endif /* FEATURE_DATA_GCSD */



/*===========================================================================

FUNCTION gl1_msg_abort_rx_tch_fr

DESCRIPTION
  This function is used to abort TCH/FACCH.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_abort_rx_tch_fr ( gas_id_t gas_id )
{
   uint8 idx;

   if (tch_params[gas_id].sm_cnt == 0)
   {
      /* Reset the fifo to be safe */
      tch_params[gas_id].sm_rd_ptr = 0;
      tch_params[gas_id].sm_wr_ptr = 0;
   }
   else if (tch_params[gas_id].sm_cnt > 1)
   {
      /* Let current state machine complete and abort all others */
      idx = (tch_params[gas_id].sm_rd_ptr + 1) % NUM_TCH_SM;
      while (tch_params[gas_id].sm_cnt > 1)
      {
         MSG_GERAN_MED_2_G("TCH fr rx aborting: state=%d, FN=%d",
                  tch_params[gas_id].sm[idx].state, GSTMR_GET_FN_GERAN(gas_id));
         L1_STATE_CHANGE(tch_params[gas_id].sm[idx].state = TCH_INACTIVE, "TCH_INACTIVE");
         tch_params[gas_id].sm_cnt--;
         idx = (idx + 1) % NUM_TCH_SM;
      }

      /* Set the write pointer */
      tch_params[gas_id].sm_wr_ptr = (tch_params[gas_id].sm_rd_ptr + 1) % NUM_TCH_SM;
   }

   return (tch_params[gas_id].sm_cnt == 0);
}


/*===========================================================================

FUNCTION gl1_msg_abort_rx_tch_hr

DESCRIPTION
  This function is used to abort TCH/FACCH.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_abort_rx_tch_hr ( gas_id_t gas_id )
{
   uint8 idx;

   if (tch_params[gas_id].sm_cnt == 0)
   {
      /* Reset the fifo to be safe */
      tch_params[gas_id].sm_rd_ptr = 0;
      tch_params[gas_id].sm_wr_ptr = 0;

      for (idx = 0; idx < 4; idx++)
      {
         hr_snrs[gas_id][idx] = 0;
      }
   }
   else
   {
      /* Let current state machine complete and abort all others */
      idx = (tch_params[gas_id].sm_rd_ptr + 1) % NUM_TCH_SM;
      while (tch_params[gas_id].sm_cnt > 1)
      {
         MSG_GERAN_MED_2_G("TCH hr rx aborting: state=%d, FN=%d",
                  tch_params[gas_id].sm[idx].state,GSTMR_GET_FN_GERAN(gas_id));
         L1_STATE_CHANGE(tch_params[gas_id].sm[idx].state = TCH_INACTIVE, "TCH_INACTIVE");
         tch_params[gas_id].sm_cnt--;
         idx = (idx + 1) % NUM_TCH_SM;
      }

      /* Set the write pointer */
      tch_params[gas_id].sm_wr_ptr = (tch_params[gas_id].sm_rd_ptr + 1) % NUM_TCH_SM;
   }

   return (tch_params[gas_id].sm_cnt == 0);
}



/*===========================================================================

FUNCTION tch_fr_rx_handler

DESCRIPTION
  This function should be called every frame on the 26 frame multiframe. The
  handler is installed as part of a request to rx a msg by gl1_msg_rx_tch_facch().
  The handler is told when the start of a block is to occur and will call
  the frame layer functions at the proper time.   When the handler senses
  that there are no more messages queued up to send, it will signal that
  it should be deregistered.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if message reception is complete
  FALSE if messages are still being received

SIDE EFFECTS
  None
===========================================================================*/
static boolean tch_fr_rx_handler
(
   boolean start_of_block,             /* next frame is start of block */
   uint32  time_stamp,                 /* currently not used           */
   gas_id_t gas_id
)
{
    static  boolean receive_burst[NUM_GERAN_DATA_SPACES] =
                      { INITIAL_VALUE( FALSE ) };

    boolean remove_handler = FALSE;
    boolean get_data_flag;
    boolean metrics_flag, decode_flag;
    uint8 cnt, sm_num;                  /* designate what machine is running */
    tch_state_machine_type *sm;
    uint8 i;
    dBx16_T burst_avg = 0;
#ifdef FEATURE_GSM_RX_DIVERSITY
    dBx16_T burst_avg_divrx = 0;
    boolean is_block_start = FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

    /* Sanity check */
    if (tch_params[gas_id].sm_cnt == 0)
    {
        MSG_GERAN_ERROR_0_G("No active TCH state machines");
        return TRUE;
    }

    /* Go through the state machine fifo */
    sm_num = tch_params[gas_id].sm_rd_ptr;
    for (cnt = 0; cnt < tch_params[gas_id].sm_cnt; cnt++)
    {
        sm = &tch_params[gas_id].sm[sm_num];
        metrics_flag  = FALSE;
        decode_flag   = FALSE;
        receive_burst[gas_id] = FALSE;
        get_data_flag = FALSE;

        /* Kick the state machine */
        switch (sm->state)
        {
          case TCH_INACTIVE:
            MSG_GERAN_ERROR_0_G("TCH inactive");
            break;

          case TCH_WAITING_FOR_BLOCK_START:
            if (!start_of_block) break;

            sm->burst_num   = 0;
            sm->metrics_num = 0;
#ifdef FEATURE_VAMOS_II
            sm->vamos2_tch_wait_state = vamos2_dl_tch_wait_state[gas_id];
#endif
            sm->state++;
            receive_burst[gas_id] = TRUE;
            metrics_flag  = FALSE;
            break;

          case TCH_RECEIVE_1:
#ifdef FEATURE_VAMOS_II
      receive_burst[gas_id] = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?FALSE:TRUE;
#else
            receive_burst[gas_id] = TRUE;
#endif
            metrics_flag  = FALSE;
            sm->state++;
            break;

          case TCH_RECEIVE_2:
            receive_burst[gas_id] = TRUE;
            metrics_flag  = TRUE;
            sm->state++;
            break;

          case TCH_RECEIVE_3:
#ifdef FEATURE_VAMOS_II
      receive_burst[gas_id] = (sm->vamos2_tch_wait_state == WAIT_AFTER_BURST2)?FALSE:TRUE;
            metrics_flag  = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?FALSE:TRUE;
      decode_flag   = (sm->vamos2_tch_wait_state == NO_WAIT)?TRUE:FALSE;
#else
            receive_burst[gas_id] = TRUE;
            metrics_flag  = TRUE;
            decode_flag   = TRUE;
#endif
            sm->state++;
            break;

          case TCH_RECEIVE_4:
#ifdef FEATURE_VAMOS_II
     receive_burst[gas_id] = (sm->vamos2_tch_wait_state != NO_WAIT)?TRUE:FALSE;
     decode_flag   = (sm->vamos2_tch_wait_state != NO_WAIT)?TRUE:FALSE;
#else
           receive_burst[gas_id] = FALSE;
#endif
           metrics_flag  = TRUE;
           sm->state++;
           break;

          case TCH_RECEIVE_5:
#ifdef FEATURE_VAMOS_II
            metrics_flag  = (sm->vamos2_tch_wait_state == WAIT_AFTER_BURST2)?FALSE:TRUE;
            get_data_flag = (sm->vamos2_tch_wait_state == NO_WAIT)?TRUE:FALSE;
            if(sm->vamos2_tch_wait_state == NO_WAIT)
      {
              L1_STATE_CHANGE(sm->state = TCH_INACTIVE, "TCH_INACTIVE");
            }
            else
            {
              sm->state = TCH_RECEIVE_6;
            }
#else
            metrics_flag  = TRUE;
            get_data_flag = TRUE;
            L1_STATE_CHANGE(sm->state = TCH_INACTIVE, "TCH_INACTIVE");
#endif
      break;

#ifdef FEATURE_VAMOS_II
       case TCH_RECEIVE_6:
          receive_burst[gas_id] = FALSE;
          metrics_flag  = (sm->vamos2_tch_wait_state != NO_WAIT)?TRUE:FALSE;
          get_data_flag = (sm->vamos2_tch_wait_state != NO_WAIT)?TRUE:FALSE;
            L1_STATE_CHANGE(sm->state     = TCH_INACTIVE, "TCH_INACTIVE");
            break;
#endif

          default:
            receive_burst[gas_id] = FALSE;
            metrics_flag  = FALSE;
            MSG_GERAN_ERROR_1_G("Invalid TCH state: %d",sm->state);
            L1_STATE_CHANGE(sm->state = TCH_INACTIVE, "TCH_INACTIVE");
            break;
        }

        /* Process the metrics if necessary */
        if (metrics_flag)
        {
          (void)tch_process_metrics(sm_num, time_stamp,
                                    TRUE, GL1_HW_TCH_FS,
                                    NO_SUB_CHAN,gas_id);
        }

        /* Ready to get data */
        if (get_data_flag)
        {
            tch_process_data_fr(sm_num,gas_id);

            if (sm->callbacks.tch_metrics_cb)
            {
               sm->callbacks.tch_metrics_cb( sm->rpt.metrics,
                                             sm->rpt.data->tch_facch.hdr.chan,
                                             gas_id );
               avg_snr[gas_id] = gl1_msg_get_snr( sm->rpt.metrics, 4 );
            }

            /* Update AGC every good bfi or FACCH block */
            if( (!(sm->rpt.data->tch_facch.hdr.bfi)) ||
                (sm->rpt.data->tch_facch.hdr.chan == GL1_MSG_FACCH &&
                sm->rpt.data->tch_facch.hdr.good_data) )
            {
              for (i=0; i < 4;i++)
              {
                burst_avg = sys_algo_agc_average_output(sm->rpt.metrics[i]->pwr_dBm_x16,gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY
                if(gl1_get_rxdiv_agc_start(gas_id))
                  {
                  if(sm->rpt.metrics[i]->pwr_dBm_x16_divrx != 0)
                    {
                    burst_avg_divrx = sys_algo_agc_average_output_divrx(sm->rpt.metrics[i]->pwr_dBm_x16_divrx,gas_id);
                    }
          else
            {
                    /* Under good channel conditions if Rx diversity is disabled, maintain the diversity Rx AGC loop with
                                    * the received power from primary chain
                                    */
            burst_avg_divrx = sys_algo_agc_average_output_divrx(sm->rpt.metrics[i]->pwr_dBm_x16,gas_id);
            }
                  }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
              }

              /* The srl flag is set in the tch_process_metrics */
              /* which takes care of blocker for block updates. */
              /* Update AGC with the 4th metrics valid and times stamp */
              sys_algo_agc_update
                (sm->agc_ptr,
                 burst_avg,
                 sm->rpt.metrics[3]->valid,
                 sm->rpt.metrics[3]->time_stamp,gas_id);

              /* Use last update, and the rest will roll in frame by frame. */
              sm->signal_info.rx_signal_strength = sm->agc_ptr->pwr_dBm_x16;

              MSG_GERAN_LOW_3_G("TCH/FS Block AGC:=%d dBm,FN=%d,B=%d",
                        sm->agc_ptr->pwr_dBm_x16/16,
                        GSTMR_GET_FN_GERAN(gas_id),sm->agc_ptr->srl_enabled);

#ifdef FEATURE_GSM_RX_DIVERSITY
       sm->signal_info.rx_signal_strength_divrx = 0;
       if(burst_avg_divrx != 0)
       {
        sys_algo_agc_update
          (sm->agc_ptr_divrx,
           burst_avg_divrx,
           sm->rpt.metrics[3]->valid,
           sm->rpt.metrics[3]->time_stamp,gas_id);

        /* Use last update, and the rest will roll in frame by frame. */
        sm->signal_info.rx_signal_strength_divrx = sm->agc_ptr_divrx->pwr_dBm_x16;

        MSG_GERAN_LOW_3_G("TCH/FS Block AGC DivRx:=%d dBm,FN=%d,B=%d",
              sm->agc_ptr_divrx->pwr_dBm_x16/16,
              GSTMR_GET_FN_GERAN(gas_id),sm->agc_ptr_divrx->srl_enabled);
       }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

              /* Flag this is a valid burst. */
              sm->signal_info.valid = TRUE;
            }
        }

        /* Now go get the bursts if there are any to get */
        if (receive_burst[gas_id])
        {
            uint8 rfacch_buffer_index;
            if (gl1_msg_is_ciphering_enabled(gas_id))
            {
                gl1_hw_update_ciphering_fn(gas_id);
            }

            /* Prepare dynamic parameters */
            sm->signal_info.rx_signal_strength = sm->agc_ptr->pwr_dBm_x16;
#ifdef FEATURE_GSM_RX_DIVERSITY
      sm->signal_info.rx_signal_strength_divrx = 0;
      if((gl1_get_rxdiv_agc_start(gas_id)) && init_agc_divrx)
      {
        if(l1_ded_drx_agc_init[gas_id] == TRUE)
        {
          sys_algo_agc_init(sm->agc_ptr_divrx, sm->agc_ptr->pwr_dBm_x16);
          l1_ded_drx_agc_init[gas_id] = FALSE;
        }
        sm->signal_info.rx_signal_strength_divrx = sm->agc_ptr_divrx->pwr_dBm_x16;
      }
#endif

            /* Flag this is a valid burst. */
            sm->signal_info.valid = TRUE;
            sm->burst_info.arfcn              = sm->ARFCN[sm->burst_num];
            sm->burst_info.decode_flag        = decode_flag;
            sm->burst_info.burst_num          = sm->burst_num;
            sm->burst_info.look_for_rfacch    = FALSE;

            /* Check if next frame is the last frame of an rfacch and set look_for_rfacch */
            /* Note that look_for_rfacch is only set in the last frame when the decode flag is sent */
            if (l1_tch_is_rfacch_decode_frame(ADD_MOD(frame_counters[gas_id].FNmod42432, 1, 42432), 
                                              &rfacch_buffer_index, gas_id))
            {
              sm->burst_info.look_for_rfacch    = TRUE;
              l1_tch_set_rfacch_fw_buffer_index(rfacch_buffer_index, gas_id);

              MSG_GERAN_MED_2_G("R-FACCH: frame_counters[gas_id].FNmod42432 = %d, FW buffer index = %d",
                  frame_counters[gas_id].FNmod42432, rfacch_buffer_index);

              clear_rfacch_decode_frame(rfacch_buffer_index, gas_id);
            }
            sm->burst_info.rfacch_fw_buffer_index = l1_tch_get_rfacch_fw_buffer_index(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
           is_block_start = (sm->burst_num == 0)?TRUE:FALSE;
#endif

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
            sm->burst_info.amr_partial_decode = FALSE;
            sm->burst_info.dtx_detected = FALSE;
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */

            /* Issue the receive */
            gl1_hw_rx_dtm_gsm
             (
               &sm->signal_info,
               &sm->burst_info,
               &sm->tags[sm->burst_num],
#ifdef FEATURE_GSM_RX_DIVERSITY
               is_block_start,
#endif
               gas_id
             );
            sm->burst_num++;
        }

        /* Next state machine */
        sm_num = (sm_num + 1) % NUM_TCH_SM;
    }

    /* Clear out inactive state machines */
    while (tch_params[gas_id].sm[tch_params[gas_id].sm_rd_ptr].state == TCH_INACTIVE)
    {
        tch_params[gas_id].sm_rd_ptr = (tch_params[gas_id].sm_rd_ptr + 1) % NUM_TCH_SM;
        if (--tch_params[gas_id].sm_cnt == 0) break;
    }
    /* Are there any more messages to receive */
    if (tch_params[gas_id].sm_cnt == 0)
    {
        remove_handler = TRUE;
    }

    return(remove_handler);
}

/*===========================================================================

FUNCTION tch_hr_rx_handler

DESCRIPTION
  This function should be called every frame on the 26 frame multiframe. The
  handler is installed as part of a request to rx a msg by gl1_msg_rx_tch_facch().
  The handler is told when the start of a block is to occur and will call
  the frame layer functions at the proper time.   When the handler senses
  that there are no more messages queued up to send, it will signal that
  it should be deregistered.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if message reception is complete
  FALSE if messages are still being received

SIDE EFFECTS
  None
===========================================================================*/
static boolean tch_hr_rx_handler
(
   boolean start_of_block,             /* next frame is start of block */
   uint32  time_stamp,                 /* currently not used           */
   gas_id_t gas_id
)
{
    static  boolean receive_burst[NUM_GERAN_DATA_SPACES];
    boolean remove_handler = FALSE;
    boolean get_data_flag;
    boolean metrics_flag;
    boolean decode_flag;
    uint8   cnt, sm_num;
    tch_state_machine_type *sm;
    uint16 i, j;
    uint32 avg_hr_snr = 0;
    dBx16_T burst_avg = 0;
#ifdef FEATURE_GSM_RX_DIVERSITY
    dBx16_T burst_avg_divrx = 0;
    boolean is_block_start = FALSE;
#endif

    /* Sanity check */
    if (tch_params[gas_id].sm_cnt == 0)
    {
        MSG_GERAN_ERROR_0_G("No active TCH state machines");
        return TRUE;
    }

    /* Go through the state machine fifo */
    sm_num = tch_params[gas_id].sm_rd_ptr;
    for (cnt = 0; cnt < tch_params[gas_id].sm_cnt; cnt++)
    {
       sm = &tch_params[gas_id].sm[sm_num];
       metrics_flag  = FALSE;
       decode_flag   = FALSE;
       receive_burst[gas_id] = FALSE;
       blank_loopback_data[gas_id] = FALSE;
       get_data_flag = FALSE;

       /* Kick the state machine */
       switch (sm->state)
       {
          case TCH_INACTIVE:
            MSG_GERAN_ERROR_0_G("TCH inactive");
            break;

          case TCH_WAITING_FOR_BLOCK_START:
            if (!start_of_block) break;

            metrics_flag    = FALSE;
            sm->burst_num   = 0;
            sm->metrics_num = 0;
            sm->state++;
            receive_burst[gas_id] = TRUE;
#ifdef FEATURE_VAMOS_II
            sm->vamos2_tch_wait_state = vamos2_dl_tch_wait_state[gas_id];
#endif
            break;

          case TCH_RECEIVE_1:
            receive_burst[gas_id] = FALSE;
            metrics_flag  = FALSE;
            sm->state++;
            break;

          case TCH_RECEIVE_2:
            {
              /* Make sure the metrics contents are reset before they
              * are filled in.
              */
              uint32  down_count = 4;
#ifdef FEATURE_VAMOS_II
        receive_burst[gas_id] = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?FALSE:TRUE;
              decode_flag   = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?FALSE:TRUE;
#else
              receive_burst[gas_id] = TRUE;
              decode_flag   = TRUE;
#endif
              metrics_flag  = TRUE;

              /* Use a decrementing while loop to reset the contents
               * @@Warning: Do not reset the agc pointer.
               */
              while( down_count-- != 0 )
              {
                /* Check the pointer against NULL */
                if( NULL != sm->rpt.metrics[down_count] )
                {
                    /* Assign a pointer */
                    gl1_msg_metrics_rpt*  metrics_reset_ptr =
                        sm->rpt.metrics[down_count];

                    metrics_reset_ptr->offset_to_sob = 0;
                    metrics_reset_ptr->arfcn.num     = 0;
                    metrics_reset_ptr->arfcn.band    = SYS_BAND_NONE;
                    metrics_reset_ptr->rssi          = 0;
                    metrics_reset_ptr->pwr_dBm_x16   = 0;
#ifdef FEATURE_GSM_RX_DIVERSITY
                    metrics_reset_ptr->pwr_dBm_x16_divrx = 0;
#endif
                    metrics_reset_ptr->rx_gain_range = GAIN_RANGE_1;
                    metrics_reset_ptr->dc_offset_i   = 0;
                    metrics_reset_ptr->dc_offset_q   = 0;
                    metrics_reset_ptr->freq_offset   = 0;
                    metrics_reset_ptr->timing_offset = 0;
                    metrics_reset_ptr->snr           = 0;

                } /* End of if ptr is not NULL */
              }     /* End of while loop */

              sm->state++;
            }
            break;

          case TCH_RECEIVE_3:
#ifdef FEATURE_VAMOS_II
            receive_burst[gas_id] = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?TRUE:FALSE;
      decode_flag   = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?TRUE:FALSE;
#else
            receive_burst[gas_id] = FALSE;
#endif
            metrics_flag  = FALSE;
            sm->state++;
            break;

          case TCH_RECEIVE_4:
            receive_burst[gas_id] = FALSE;
#ifdef FEATURE_VAMOS_II
            metrics_flag  = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?FALSE:TRUE;
#else
            metrics_flag  = TRUE;
#endif
            if (sm->delay_hr_callbacks)
            {
               sm->state++;
            }
            else
            {
#ifdef FEATURE_VAMOS_II
               get_data_flag = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?FALSE:TRUE;
               if(sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)
                {
                  sm->state++;
                }
                else
                {
                   L1_STATE_CHANGE(sm->state   = TCH_INACTIVE, "TCH_INACTIVE");
                }
#else
               get_data_flag = TRUE;
               L1_STATE_CHANGE(sm->state     = TCH_INACTIVE, "TCH_INACTIVE");
#endif
            }
            break;

          case TCH_RECEIVE_5:
            receive_burst[gas_id] = FALSE;
 #ifdef FEATURE_VAMOS_II
      metrics_flag  = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?TRUE:FALSE;
      if((sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START) && (sm->delay_hr_callbacks))
      {
              sm->state++;
            }
            else
            {
              get_data_flag = TRUE;
              L1_STATE_CHANGE(sm->state  = TCH_INACTIVE, "TCH_INACTIVE");
            }
#else
            metrics_flag  = FALSE;
            L1_STATE_CHANGE(sm->state     = TCH_INACTIVE, "TCH_INACTIVE");
            get_data_flag = TRUE;
#endif
            break;

#ifdef FEATURE_VAMOS_II
        case TCH_RECEIVE_6:
          receive_burst[gas_id] = FALSE;
          metrics_flag  = FALSE;
          get_data_flag = TRUE;
          L1_STATE_CHANGE(sm->state  = TCH_INACTIVE, "TCH_INACTIVE");
            break;
#endif

          default:
            receive_burst[gas_id] = FALSE;
            metrics_flag  = FALSE;
            MSG_GERAN_ERROR_1_G("Invalid TCH state: %d",sm->state);
            L1_STATE_CHANGE(sm->state     = TCH_INACTIVE, "TCH_INACTIVE");
            break;
        }

        /* Process the metrics if necessary */
        if (metrics_flag)
        {
            (void) tch_process_metrics(sm_num, time_stamp,
                                       FALSE, GL1_HW_TCH_HS,
                                       dl_hr_sub_chan[gas_id],gas_id);
        }

        /* Ready to get data */
        if (get_data_flag)
        {
           tch_process_data_hr(sm_num,gas_id);

           if ( sm->callbacks.tch_metrics_cb)
           {
              sm->callbacks.tch_metrics_cb( sm->rpt.metrics,
                                            sm->rpt.data->tch_facch.hdr.chan,
                                            gas_id );

              /* Average SNR over 4 bursts.   We store the SNRs from
               * the previous block and average over 2 blocks.  This is
               * to prevent random data which correlates with the TSC
               * from being flagged as a good frame ( BFI = 0)
               */
              for (i = 0, j = 2; i < 2; i++, j++ )
              {
                 hr_snrs[gas_id][i] = hr_snrs[gas_id][j];

                 if(gl1_msg_saic_in_burst( sm->rpt.metrics[i]->misc_flags))
                 {
                   if(sm->rpt.metrics[i]->snr < BFI_SNR_THRESHOLD_SAIC_HR)
                   {
                     hr_snrs[gas_id][j] = sm->rpt.metrics[i]->pre_mimo_trained_complete_snr;
                   }
                   else
                   {
                     hr_snrs[gas_id][j] = sm->rpt.metrics[i]->snr;
                   }
                 }
                 else
                 {
                   hr_snrs[gas_id][j] = sm->rpt.metrics[i]->snr;
                 }

              }

              for ( i = 0; i < 4; i++)
              {
                 avg_hr_snr += hr_snrs[gas_id][i];
              }

              avg_snr[gas_id] = (uint16)(avg_hr_snr / 4);
           }

           /* Update AGC every good TCH/FACCH block */
           if  (!(sm->rpt.data->tch_facch.hdr.bfi) ||
               (sm->rpt.data->tch_facch.hdr.chan == GL1_MSG_FACCH_HS &&
               sm->rpt.data->tch_facch.hdr.good_data) )
           {
             for (i=0; i < 2;i++)
             {
               burst_avg = sys_algo_agc_average_output(sm->rpt.metrics[i]->pwr_dBm_x16,gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY
        if(gl1_get_rxdiv_agc_start(gas_id))
          {
                /* AGC loop for diversity RX should be ON even if RX diveristy is disabled under good channel conditions
                             * If Rx diversity is disabled for the burst, burst RSSI and pwr_dBm_x16_divrx are set to 0. If Rx diversity
                             * is disabled, use received power from primary chain to maintain the AGC loop for diversity chain.
                             */
          if(sm->rpt.metrics[i]->pwr_dBm_x16_divrx != 0)
           {
         burst_avg_divrx = sys_algo_agc_average_output_divrx(sm->rpt.metrics[i]->pwr_dBm_x16_divrx,gas_id);
           }
          else
           {
           burst_avg_divrx = sys_algo_agc_average_output_divrx(sm->rpt.metrics[i]->pwr_dBm_x16,gas_id);
           }
         }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
             }
             /* The srl flag is set in the tch_process_metrics */
             /* which takes care of blocker for block updates. */
             /* Update AGC with the 2nd metrics valid and times stamp*/

             sys_algo_agc_update
                                (sm->agc_ptr,
                                 burst_avg,
                                 sm->rpt.metrics[1]->valid,
                                 sm->rpt.metrics[1]->time_stamp,gas_id);

             /* Use last update, and the rest will roll in frame by frame. */
             sm->signal_info.rx_signal_strength = sm->agc_ptr->pwr_dBm_x16;

             MSG_GERAN_LOW_3_G("TCH/HS Block AGC:=%d dBm,FN=%d,B=%d",
                      sm->agc_ptr->pwr_dBm_x16/16,
                      GSTMR_GET_FN_GERAN(gas_id),sm->agc_ptr->srl_enabled);

#ifdef FEATURE_GSM_RX_DIVERSITY
       sm->signal_info.rx_signal_strength_divrx = 0;
       if(burst_avg_divrx != 0)
        {
        sys_algo_agc_update
          (sm->agc_ptr_divrx,
           burst_avg_divrx,
           sm->rpt.metrics[3]->valid,
           sm->rpt.metrics[3]->time_stamp,
           gas_id);

        /* Use last update, and the rest will roll in frame by frame. */
        sm->signal_info.rx_signal_strength_divrx = sm->agc_ptr_divrx->pwr_dBm_x16;

        MSG_GERAN_LOW_3_G("TCH/FS Block AGC DivRx:=%d dBm,FN=%d,B=%d",
              sm->agc_ptr_divrx->pwr_dBm_x16/16,
              GSTMR_GET_FN_GERAN(gas_id),sm->agc_ptr_divrx->srl_enabled);
       }
#endif /*FEATURE_GSM_RX_DIVERSITY*/


             /* Flag this is a valid burst. */
             sm->signal_info.valid = TRUE;
           }
        }

        /* Now go get the bursts if there are any to get */
        if (receive_burst[gas_id] )
        {
            uint8 rfacch_buffer_index;

            if (gl1_msg_is_ciphering_enabled(gas_id))
            {
                gl1_hw_update_ciphering_fn(gas_id);
            }

            /* Prepare dynamic parameters */
            sm->signal_info.rx_signal_strength = sm->agc_ptr->pwr_dBm_x16;
#ifdef FEATURE_GSM_RX_DIVERSITY
      sm->signal_info.rx_signal_strength_divrx = 0;
      if((gl1_get_rxdiv_agc_start(gas_id)) && init_agc_divrx)
      {
        if(l1_ded_drx_agc_init[gas_id] == TRUE)
        {
          sys_algo_agc_init(sm->agc_ptr_divrx, sm->agc_ptr->pwr_dBm_x16);
          l1_ded_drx_agc_init[gas_id] = FALSE;
        }

        sm->signal_info.rx_signal_strength_divrx = sm->agc_ptr_divrx->pwr_dBm_x16;
      }
#endif

            /* Flag this is a valid burst. */
            sm->signal_info.valid = TRUE;
            sm->burst_info.arfcn              = sm->ARFCN[sm->burst_num];
            sm->burst_info.decode_flag        = decode_flag;
            sm->burst_info.burst_num          = sm->burst_num;
            sm->burst_info.look_for_rfacch    = FALSE;

            /* Check if next frame is the last frame of an rfacch and set look_for_rfacch */
            /* Note that look_for_rfacch is only set in the last frame when the decode flag is sent */
            if (l1_tch_is_rfacch_decode_frame(ADD_MOD(frame_counters[gas_id].FNmod42432, 1, 42432), 
                                              &rfacch_buffer_index, gas_id))
            {
                sm->burst_info.look_for_rfacch    = TRUE;
              l1_tch_set_rfacch_fw_buffer_index(rfacch_buffer_index, gas_id);

              MSG_GERAN_MED_2_G("R-FACCH: frame_counters[gas_id].FNmod42432 = %d, FW buffer index = %d",
                  frame_counters[gas_id].FNmod42432, rfacch_buffer_index);

              clear_rfacch_decode_frame(rfacch_buffer_index, gas_id);
            }
            sm->burst_info.rfacch_fw_buffer_index = l1_tch_get_rfacch_fw_buffer_index(gas_id);


#ifdef FEATURE_GSM_RX_DIVERSITY
            is_block_start = (sm->burst_num == 0)?TRUE:FALSE;
#endif

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
           sm->burst_info.amr_partial_decode = FALSE;
           sm->burst_info.dtx_detected = FALSE;
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */

            /* Issue the receive */
            gl1_hw_rx_dtm_gsm
             (
               &sm->signal_info,
               &sm->burst_info,
               &sm->tags[sm->burst_num],
#ifdef FEATURE_GSM_RX_DIVERSITY
               is_block_start,
#endif
               gas_id
             );
            sm->burst_num++;

        }

        /* Next state machine */
        sm_num = (sm_num + 1) % NUM_TCH_SM;
    }
    /* Clear out inactive state machines */
    while (tch_params[gas_id].sm[tch_params[gas_id].sm_rd_ptr].state == TCH_INACTIVE)
    {
        tch_params[gas_id].sm_rd_ptr = (tch_params[gas_id].sm_rd_ptr + 1) % NUM_TCH_SM;
        if (--tch_params[gas_id].sm_cnt == 0) break;
    }
    /* Are there any more messages to receive */
    if (tch_params[gas_id].sm_cnt == 0)
    {
        remove_handler = TRUE;
    }

    return(remove_handler);
}



#ifdef FEATURE_DATA_GCSD
/*===========================================================================

FUNCTION cstch_rx_handler

DESCRIPTION
  This function should be called every frame on the 26 frame multiframe. The
  handler is installed as part of a request to rx a msg by gl1_msg_rx_tch_facch().
  The handler is told when the start of a block is to occur and will call
  the frame layer functions at the proper time.   When the handler senses
  that there are no more messages queued up to send, it will signal that
  it should be deregistered.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if message reception is complete
  FALSE if messages are still being received

SIDE EFFECTS
  None
===========================================================================*/
static boolean cstch_rx_handler
(
   boolean start_of_block,          /* next frame is start of block */
   uint32  time_stamp,              /* currently not used */
   gas_id_t gas_id
)
{
    boolean remove_handler = FALSE;
    uint8 cnt, sm_num;

    /*
       gl1_hw_rx_normal_burst is now called outside the dual state machine
       loop to ensure that we always get the data before issuing a sync rx.
       This fixes the problem of overwriting the FACCH decoded data length
       when issuing the CSD TCH rx (since they use the same data buffer).
       The overwriting resulted in us thinking the FACCH data was bad and
       dropping the FACCH.  As a result of scheduling the burts outside the
       loop, some of the state variables have been declared as arrays to
       save the state for each state machine
    */
    uint8 rx_machine = 0;
    boolean metrics_flag;
    boolean get_data_flag;
    tch_state_machine_type *sm=NULL;

    /* Sanity check */
    if (tch_params[gas_id].sm_cnt == 0)
    {
        MSG_GERAN_ERROR_0_G("No active TCH state machines");
        return TRUE;
    }

    /* Go through the state machine fifo */
    sm_num = tch_params[gas_id].sm_rd_ptr;
    for (cnt = 0; cnt < tch_params[gas_id].sm_cnt; cnt++)
    {
        sm = &tch_params[gas_id].sm[sm_num];

        /* Sanity check pointer */
        if (sm == NULL)
        {
          MSG_GERAN_ERROR_0_G(" TCH state machine ptr NULL!!");
          return TRUE;
        }

        metrics_flag       = FALSE;
        get_data_flag      = FALSE;
        sm->decode_flag    = FALSE;
        sm->receive_burst  = FALSE;
        sm->force_facch    = FALSE;

        /* Kick the state machine */
        switch (sm->state)
        {
          case TCH_INACTIVE:
            MSG_GERAN_ERROR_0_G("TCH inactive");
            break;

          case TCH_WAITING_FOR_BLOCK_START:
            if (!start_of_block) break;

            if (cs_data_rx_rpt_ptr[gas_id] == NULL)
            {
                MSG_GERAN_HIGH_0_G("underrun on cs data rx buffer");
                sm->cs_data_rx_rpt = NULL;
            }
            else
            {
                sm->cs_data_rx_rpt = *cs_data_rx_rpt_ptr[gas_id];
            }
            cs_data_rx_rpt_ptr[gas_id] = NULL;
            sm->receive_burst = TRUE;
            metrics_flag  = FALSE;
            sm->burst_num   = 0;
            sm->metrics_num = 0;
            sm->state++;
            break;

          case TCH_RECEIVE_1:
            sm->receive_burst = TRUE;
            sm->decode_flag = TRUE;
            metrics_flag  = FALSE;
            sm->state++;
            break;

          case TCH_RECEIVE_2:
            sm->receive_burst = TRUE;
            metrics_flag  = TRUE;
            sm->state++;
            break;

          case TCH_RECEIVE_3:
            metrics_flag  = TRUE;
            sm->receive_burst = TRUE;
            sm->decode_flag   = TRUE;
            sm->force_facch = TRUE;
            sm->state++;
            break;

          case TCH_RECEIVE_4:
            metrics_flag  = TRUE;
            sm->receive_burst = FALSE;
            sm->state++;
            break;

          case TCH_RECEIVE_5:
            sm->receive_burst = FALSE;
            metrics_flag  = TRUE;
            L1_STATE_CHANGE(sm->state = TCH_INACTIVE, "TCH_INACTIVE");
            get_data_flag = TRUE;
            break;

          default:
            sm->receive_burst = FALSE;
            metrics_flag  = FALSE;
            MSG_GERAN_ERROR_1_G("Invalid TCH state: %d",sm->state);
            L1_STATE_CHANGE(sm->state = TCH_INACTIVE, "TCH_INACTIVE");
            break;
        }

        /* Process the metrics if necessary */
        if (metrics_flag)
        {
            (void) tch_process_metrics(sm_num, time_stamp,
                                FALSE, GL1_HW_TCH_F96_AND_FACCH,
                                NO_SUB_CHAN,gas_id);
        }

        /* if it is in state TCH_RECEIVE_3 (since it has incremented one state,
           check if it is TCH_RECEIVE_4), do a callback
        */
        if (sm->state == TCH_RECEIVE_4)
        {
            if (sm->cs_data_rx_rpt != NULL)
            {
                gl1_hw_get_sync_rx_data( sm->burst_info.channel_type,
                                         &sm->tags[DECODE_TAG],
                                         &sm->cs_data_rx_rpt->hdr,
                                         sm->cs_data_rx_rpt->data,
                                         (uint16)MAX_CSTCH_OCTETS,
                                         gas_id);

                /* force chan here to GL1_MSG_TCH? */
                sm->cs_data_rx_rpt->hdr.chan = (int16)GL1_MSG_TCH;


                /* Only loopback A & B implemented in s/w. C still handled my mdsp */
                if (( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_B ) ||
                    ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_A ))
                {
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000)  /*lint !e553*/
                    /* ADS1.2 issue - memcpy on non-aliged unions of structures */
                    {
                        uint32 i;
                        for (i=0; i<(MAX_CSTCH_OCTETS+1); i++)
                        {
                            gl1_loopback_data[gas_id][i] = sm->cs_data_rx_rpt->data[i];
                        }
                    }
#else
                    memscpy
                     (
                       (uint8 *)gl1_loopback_data[gas_id],
                       sizeof(gl1_loopback_data[gas_id]),
                       (uint8 *)sm->cs_data_rx_rpt->data,
                       MAX_CSTCH_OCTETS + 1
                     );
#endif
                }
            }
            else
            {
               tch_facch_decode_rpt dummy_buffer;

               /* Retrieve decoded data into dummy variable to be discarded */
               gl1_hw_get_sync_rx_data( sm->burst_info.channel_type,
                                        &sm->tags[DECODE_TAG],
                                        &dummy_buffer.hdr,
                                        dummy_buffer.data,
                                        (uint16)MAX_CSTCH_OCTETS,
                                        gas_id);
               MSG_GERAN_ERROR_0_G("CSD data ptr is NULL");
            }

            if (sm->callbacks.data_cb)
            {
                sm->callbacks.data_cb( (gl1_msg_decode_rpt *) sm->cs_data_rx_rpt,gas_id);
            }
            else
            {
                MSG_GERAN_ERROR_0_G("No dl callback registered");
            }
        }


        /* Ready to get data */
        if (get_data_flag)
        {
            /* Now the data is available, go get it */
            gl1_hw_get_sync_rx_data( sm->burst_info.channel_type,
                                     &sm->tags[DECODE_TAG],
                                     &sm->rpt.data->tch_facch.hdr,
                                     sm->rpt.data->tch_facch.data,
                                     (uint16)MAX_TCH_FS_OCTETS,
                                     gas_id);

            /* Map the HW layer channel type enum to the MSG layer*/
            MSG_GERAN_MED_2_G("chan_type %d FN %d",
              (sm->rpt.data->tch_facch.hdr.chan),
              GSTMR_GET_FN_GERAN(gas_id));

            switch(sm->rpt.data->tch_facch.hdr.chan)
            {
            case GL1_HW_FACCH:
                        sm->rpt.data->tch_facch.hdr.chan = (int16)GL1_MSG_FACCH;
                break;

            case GL1_HW_TCH_F96:
                        sm->rpt.data->tch_facch.hdr.chan = (int16)GL1_MSG_TCH_F96;
                break;

            case GL1_HW_TCH_F144:
                       sm->rpt.data->tch_facch.hdr.chan = (int16)GL1_MSG_TCH_F144;
               break;

            default:
               MSG_GERAN_ERROR_1_G("CSD:invalid channel type %d",sm->rpt.data->tch_facch.hdr.chan);
               sm->rpt.data->tch_facch.hdr.chan = (int16)GL1_MSG_TCH_F96;
               break;
            }



            if (sm->callbacks.data_cb)
            {
                sm->callbacks.data_cb( sm->rpt.data,gas_id);
            }
            else
            {
                MSG_GERAN_ERROR_0_G("No dl callback registered");
            }
            /* If there is a metrics callback then call it with the metrics */
            /* data for the four bursts of the block                        */
            if (sm->callbacks.tch_metrics_cb)
            {
                sm->callbacks.tch_metrics_cb( sm->rpt.metrics ,
                                              sm->rpt.data->tch_facch.hdr.chan,
                                              gas_id);
            }
        }

        /*
           Save of which machine needs to schedule a burst.  Scheduling is now
           done outside the loop
        */
        if (sm->receive_burst)
        {
            rx_machine = sm_num;
        }

        /* Next state machine */
        sm_num = (sm_num + 1) % NUM_TCH_SM;
    }

    /*
       Now go get the bursts if there are any to get.  Only one state
       machine would have a burst to receive at a time. Check that burst_num
       is not greater than 3
    */
    if ( tch_params[gas_id].sm[0].receive_burst || tch_params[gas_id].sm[1].receive_burst )
    {
      sm = &tch_params[gas_id].sm[rx_machine];

      if ( sm->burst_num < GL1_DEFS_FRAMES_IN_BLK_PERIOD )
      {
        if (gl1_msg_is_ciphering_enabled(gas_id))
        {
          gl1_hw_update_ciphering_fn(gas_id);
        }

        /* Prepare parameters */
        sm->signal_info.rx_signal_strength = sm->agc_ptr->pwr_dBm_x16;
        /* Flag this is a valid burst. */
        sm->signal_info.valid = TRUE;
        sm->burst_info.arfcn              = sm->ARFCN[sm->burst_num];
        sm->burst_info.decode_flag        = sm->decode_flag;
        sm->burst_info.force_facch        = sm->force_facch;
        sm->burst_info.burst_num          = sm->burst_num;

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
        sm->burst_info.amr_partial_decode = FALSE;
        sm->burst_info.dtx_detected = FALSE;
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */

        /* Issue the receive */
        gl1_hw_rx_dtm_gsm
         (
           &sm->signal_info,
           &sm->burst_info,
           &sm->tags[sm->burst_num],
#ifdef FEATURE_GSM_RX_DIVERSITY
           FALSE,
#endif
           gas_id
         );
        sm->burst_num++;
      }
      else
      {
        /* burst_num is greater than 3 - flag this*/
        MSG_4(MSG_SSID_DFLT, MSG_LEGACY_ERROR,
        "csd handler burst_num %d rx_machine %d sm_num %d state %d",
        sm->burst_num,rx_machine,sm_num,sm->state);
      }
    }

    /* Clear out inactive state machines */
    while (tch_params[gas_id].sm[tch_params[gas_id].sm_rd_ptr].state == TCH_INACTIVE)
    {
        tch_params[gas_id].sm_rd_ptr = (tch_params[gas_id].sm_rd_ptr + 1) % NUM_TCH_SM;
        if (--tch_params[gas_id].sm_cnt == 0) break;
    }
    /* Are there any more messages to receive */
    if (tch_params[gas_id].sm_cnt == 0)
    {
        remove_handler = TRUE;
    }

    return(remove_handler);
}
#endif /* FEATURE_DATA_GCSD */

