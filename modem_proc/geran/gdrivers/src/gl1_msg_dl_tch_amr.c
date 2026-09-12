/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    G L 1  M E S S A G E  L A Y E R -  T R A F F I C   C H A N N E L

GENERAL DESCRIPTION
    This module contains the procedures used for communicating on the dl TCH.
    The procedures used herein are intended for use by the
    portable layer 1 software.

EXTERNALIZED FUNCTIONS
    gl1_msg_vocoder_rx_amr_fr
    gl1_msg_rx_tch_facch_amr_fr
    gl1_msg_abort_rx_tch_amr_fr
    gl1_msg_set_dl_acs
    gl1_msg_cfg_dl_tch_facch_amr_fr_hr
    gl1_msg_vocoder_rx_amr_hr
    gl1_msg_rx_tch_facch_amr_fr
    gl1_msg_abort_rx_tch_amr_hr

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

   EDIT HISTORY FOR MODULE

   This section contains comments describing changes made to the module.
   Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_dl_tch_amr.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ---------------------------------------------------------
07/03/17   mk       CR1097205: explicitly state the array size in all extern definitions
08/12/16   mk       CR1097205: Fixing AGC initialisation when Rxd is getting enabled and enhancing serving cell measurement reporting when Rxd was partially enabled in a SACCH period
02/05/16   mk       CR1010068 Add DTF events in dedicated mode
17/08/15   pjr      CR891678 Remove references to mvs.h
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
19/03/15   jk       CR810476:VAMOS2 BFI gating algorithm should be applied only for TCH loop back A mode
3/02/15    jk       CR793098:LIF additional logging changes
16/12/14   jk       CR753423:LIF for Idle,voice and data
25/11/14   hd       CR762469 Remove the AHS delay functionality based on gl1_hw_gcf_flag_enabled flag
19/08/14   pjr      CR711506 Support for AMR Partial Decode and DTX detection FR19667
17/10/14   jk       CR741194:Initialize LIF params to 0 for L2G measurements
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
11/07/14   pjr      CR 692177 Added support for RFACCH 2.0
24/07/14   jk       CR699271:GL1 LIF changes for RxD
10/07/14   jk       CR688587:GL1 changes to support LIF feature
08/05/14   cgc      CR642356 fix prototype for tch_rfacch_indicator() added to header.
02/01/14   jk       CR595130: GL1 changes to support VAMOS-II
19/12/13   jk       CR503359 - GL1 changes to support Rx diversity
07/08/13   jk       CR490807:Support for shifted SACCH  
11/11/13   ws       CR 573757 Featurise GSMTR-1 registers and Voice services driver
7/16/13    zc       CR470197 Replace memcpy with memscpy
20/06/13   cs       Major Triton TSTS Syncup
10/06/13   cs       Further TSST Updates
24/05/13   ap       CR472264 DSDS: Fix the gas_id problems when the IQ logging is enabled
21/05/13   cs       TSTS Updates
03/04/13   pjr      CR470940 included gl1_hw_vs_g.h
07/03/13   dv       CR461080: Introduction of new GSM VS interface
06/12/12   cja      CR383165 Remove compiler warnings
13/03/12   ws       CR 330862 - Change dedicated mode F3 level's and add sacch burst
                    metrics and compact message metrics log packets.
16/12/11   ws       Free floating API change - provide API for NAS GSVOICE to copy vocoder
                    data from/to GL1 to MVS structures
26/04/11   ws       Removed deprecated header voc.h
17/01/10   ab       Updated - Resolve Klocwork Buffer Overflow errors
14/01/11   ab       Resolve Klocwork Buffer Overflow errors
02/08/10   tjw      Removed unused clk.h (CMI phase 1 re-clean up)
04/06/10   cs       Lint/KW fixes
11/05/10   cja      CR237386 For BEP use burst snr, not pre-mimo for high SNR.
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
24/07/09   ws       CR 180476 - Improve performance of Dedicated AGC
01/12/08   cs       Add new include gl1_hw_debug.h
30/09/08   dv       CR158382:BFI alg. change for WB_AMR
02/09/08   cs       CR155904 remove setting of INVALID_BAND in HR to avoid l1_log errors
28/07/98   hv       Added KxMutex support
13/06/08   ws       Swap Endian for 8K targets that support QDSP6 Vocoder
12/05/08   dv       Added FEATURE_GSM_AMR_WB - L1 component
28/03/08   ws       Corrected merge error of CR 128758
19/03/08   og       Use pre-mimo SNR when producing bep values for enhanced
                    measurements reporting pruposes. Resolves CR141764.
14/01/08   cs       Allow for separate IQ logging and saving
05/12/07   dv       Fix bug that caused mdsp warnings: 0x00000000, 0x00000100
28/11/07   cs       Lint tidyup of some leftover code
19/09/07   agv      CR128758 - Blocker check in know AGC frames only implemented.
17/09/07   agv      Added I/Q logging to code base.
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
06/07/07   agv      CR123023 - Update fix for AFC/AGC corruption.
27/06/07   agv      CR123023 Fix for AGC corruption, and TT.
28/06/07   cs       Lint fixes
23/05/07   agv      CR118309 - Large RSSI variation initial fix.
27/03/07   dv       Added FEATURE_GSM_R_FACCH
05/03/07   og       Ensure the half rate 4 burst metrics structure elements are
                    reset before being filled in. Resolves CR 113334.
05/07/06   agv      Removed avg_snr_saic to correct Lint warning.
05/23/06   gfr      Featurize SAIC BFI fixes with FEATURE_GSM_MDSP_SAIC
05/22/06   kting    SAIC BFI fixes
12/08/05   gfr      Modify gl1_hw_save_iq_samples to work with L4
11/07/05   gfr      QLint, move shared prototypes to gl1_msgi_tch_amr.h
09/20/05   gfr      Store 8 frames of TCH IQ samples to handle interleaving
08/31/05   gfr      Replace gl1_msg_metrics_rpt with gl1_defs_metrics_rpt to
                    remove need for double copy
08/30/05   dp       Fix for 26.16.9.11 failure on half rate
08/25/05   gfr      Use the correct decode tag for half-rate
08/05/05   gfr      Clean up delay_callbacks flag
07/29/05   gfr      DTM support
06/08/05   dp       Changes to get decoded data in N+1 for FR and HR
05/25/05   gfr      Add support for logging IQ samples
05/06/05   gfr      Call rx_normal_burst_amr for HR as well.
05/05/05   yh       Code clean up
04/21/05   yh       Support for panic reset
04/20/05   yh       Remove use of RF fifo
04/12/05   gfr      Consolidated GSM hw layer parameters
03/09/05   yh       API change for gl1_hw_get_sync_rx_data_amr() and added bep
                    calculation.
02/22/05   yh       Fixed offset_to_sob calculation
01/05/05   dp       Merged audio optimizations to mainline
09/07/04   gsc      In gl1_msg_abort_rx_tch_amr_hr, intialize amr_hr_snrs
                    instead of amr_fr_snrs
08/19/04   gsc      Pass in the code rate based on the d/l cmi to BFI
                    algorithm.
08/18/04   yhong    Initial version. Contains only the downlink functions taken
                    from gl1_msg_tch_amr.c
===========================================================================*/

/*===========================================================================

    INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#include "comdef.h"

#ifdef CUST_H
    #include "customer.h"
#endif

#if  defined (FEATURE_GSM_AMR)

#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_debug.h"
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "msg.h"
#include "gtmrs.h"
#include "gtmrs_g.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#include "gl1_msgi_tch_amr.h"

#include "gl1_sys_algo.h"

#ifdef GERAN_L1_ENHANCED_RX
#include "l1_utils.h"
#endif

#ifdef GERAN_L1_HLLL_LNASTATE
#include "l1_utils.h"
#endif

#include <string.h>
#include "gl1_mutex.h"
#include <stringl/stringl.h>

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#define QSH_REF_GOOD_SNR 15*16
#define QSH_REF_GOOD_RX_PWR -75*16

static uint16 qsh_amr_tch_hr_thresholds[6] =
{30800, 30900, 31000, 31000, 31000, 31000};

#ifdef FEATURE_GSM_AMR_WB
/*The last 3 values are just made up and for devlopment use - need to be refined!*/
static uint16 qsh_amr_tch_fr_thresholds[GL1_MSG_AMR_MODE_MAX] =
{22500, 26000, 27000, 27500, 27500, 27500, 28200, 29200, 20000, 20000, 20000};
#else
static uint16 qsh_amr_tch_fr_thresholds[8] =
{22500, 26000, 27000, 27500, 27500, 27500, 28200, 29200};
#endif

extern gl1_msg_amr_mode_type curr_dl_acs[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES];
#endif

/*****************************************************

 LOCAL STORAGE

******************************************************/
/* Configured Callback pointers  */
static gl1_msg_rx_cb_type   amr_dl_tch_callback[NUM_GERAN_DATA_SPACES] =
                                             {
                                                 { 0 }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                                               , { 0 }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#if defined ( FEATURE_TRIPLE_SIM )
                                               , { 0 }
#endif /* FEATURE_TRIPLE_SIM */
                                             };

static uint32        amr_fr_snrs[NUM_GERAN_DATA_SPACES][8];
static uint32        amr_fr_snrs_saic[NUM_GERAN_DATA_SPACES][8];
static boolean       amr_fr_saic_used[NUM_GERAN_DATA_SPACES][8];

static gl1_msg_amr_mode_type gl1_msg_dl_acs_amr[NUM_GERAN_DATA_SPACES][4];

static uint32        amr_hr_snrs[NUM_GERAN_DATA_SPACES][4];
static uint32        amr_hr_snrs_saic[NUM_GERAN_DATA_SPACES][4];
static boolean       amr_hr_saic_used[NUM_GERAN_DATA_SPACES][4];

static uint16 amr_avg_snr[NUM_GERAN_DATA_SPACES] =
                { INITIAL_VALUE( 0 ) };

static gl1_hw_channel_type dl_amr_channel_type[NUM_GERAN_DATA_SPACES] =
                { INITIAL_VALUE( GL1_HW_TCH_AFS ) };

extern gl1_defs_loopback_type loopback_type[NUM_GERAN_DATA_SPACES];

extern boolean l1_ded_drx_agc_init[NUM_GERAN_DATA_SPACES];

#define BFI_SNR_THRESHOLD_SAIC_AFS  573  /* 3.5dB in Q8 only for AFS 12.2 */
#define BFI_SNR_THRESHOLD_SAIC_AHS  643  /* 4.0dB in Q8 only for AHS 7.95 */

#define NUM_TCH_SM 2

/* This is the tag used for retrieving data, corresponds to the index
   where the decode flag was set */
#define DECODE_TAG    3
#define DECODE_TAG_HR 1

/* SID UPDATE after a TCH_AMR HO, CMM, or ASSIGN CHAN is bad. */
#define TCH_AMR_SNR_THRESH  322

#ifdef FEATURE_VAMOS_II
static gl1_vamos2_tch_wait_type vamos2_amr_dl_tch_wait_state[NUM_GERAN_DATA_SPACES] = 
                                { INITIAL_VALUE ( NO_WAIT) };
#endif 

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
} amr_tch_state_type;

/* Structure containing information for a single TCH rx */
typedef struct
{
    /* State variables */
    amr_tch_state_type  state;
    uint8               burst_num;
    uint8               metrics_num;
    boolean             delay_hr_callbacks;

    /* Configured rx parameters */
    sys_algo_agc_T      *agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
    sys_algo_agc_T      *agc_ptr_divrx;
#endif
    ARFCN_T             ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD];

    /* Frame layer parameters */
    gl1_hw_rx_signal_type   signal_info;
    gl1_hw_rx_burst_type    burst_info;
    gl1_hw_rx_tags_type     tags[GL1_DEFS_FRAMES_IN_BLK_PERIOD];

    /* Callback parameters */
    gl1_msg_rx_rpt      rpt;
#ifdef FEATURE_VAMOS_II
    gl1_vamos2_tch_wait_type  vamos2_tch_wait_state;
#endif 
} amr_tch_state_machine_type;

/* Structure containing all TCH parameters */
typedef struct
{
    /* Array of state machines */
    uint8                   sm_cnt;
    uint8                   sm_rd_ptr;
    uint8                   sm_wr_ptr;
    amr_tch_state_machine_type sm[NUM_TCH_SM];
} amr_tch_params_type;

/* Static local storage */
static amr_tch_params_type amr_tch_params[NUM_GERAN_DATA_SPACES];

static uint8 rx_TSC[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_GSM_RX_DIVERSITY   
static boolean       init_agc_divrx = FALSE;
#endif

/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/

static boolean amr_tch_rx_handler_fr( boolean start_of_block,
                                      uint32  time_stamp,gas_id_t gas_id );

static boolean amr_tch_rx_handler_hr( boolean start_of_block,
                                      uint32  time_stamp,gas_id_t gas_id );

static boolean amr_tch_process_metrics(uint8 sm_num,
                                       uint32 time_stamp,
                                       boolean full_rate_tch,gas_id_t gas_id);

static void amr_tch_process_data_hr(uint8 sm_num, uint16 *channel,gas_id_t gas_id);

static void amr_tch_process_data_fr(uint8 sm_num, uint16 *channel,gas_id_t gas_id);

/*===========================================================================
FUNCTION gl1_msgi_init_dl_tch_amr

DESCRIPTION
   This function initializes data structures used by downlink TCH AMR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_init_dl_tch_amr(gas_id_t gas_id)
{
   uint8 i = 0;
   amr_dl_tch_callback[gas_id].data_cb         = NULL;
   amr_dl_tch_callback[gas_id].tch_metrics_cb  = NULL;
   amr_dl_tch_callback[gas_id].metrics_cb      = NULL;
   amr_dl_tch_callback[gas_id].log_cb          = NULL;
   amr_dl_tch_callback[gas_id].abort_cb        = NULL;

   amr_tch_params[gas_id].sm_cnt               = 0;
   amr_tch_params[gas_id].sm_rd_ptr            = 0;
   amr_tch_params[gas_id].sm_wr_ptr            = 0;
   for(i = 0; i < NUM_TCH_SM; i++)
   {
      L1_STATE_CHANGE(amr_tch_params[gas_id].sm[i].state       = TCH_INACTIVE, "TCH_INACTIVE");
      amr_tch_params[gas_id].sm[i].burst_num   = 0;
      amr_tch_params[gas_id].sm[i].metrics_num = 0;
   }
}

/*===========================================================================

FUNCTION amr_tch_process_metrics

DESCRIPTION
   Retrieves burst metrics for the given state machine. This function is called
   by tch_fr and tch_hr handlers.

===========================================================================*/
static boolean amr_tch_process_metrics
(
   uint8 sm_num,
   uint32 time_stamp,
   boolean full_rate_tch,
   gas_id_t gas_id
)
{
   gl1_defs_metrics_rpt metrics, *metrics_ptr;
   amr_tch_state_machine_type    *sm = &amr_tch_params[gas_id].sm[sm_num];
   /* Interferer Ind. */
   boolean              blocker = FALSE;
#ifdef GERAN_L1_HLLL_LNASTATE
   gl1_defs_metrics_lnastate gl1_metrics_lnastate;
#endif

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

   /* Retrieve metrics */
   blocker = gl1_hw_get_sync_rx_metrics(
               metrics_ptr, 
               &sm->tags[sm->metrics_num],
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
               LOG_TCH,
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */
               gas_id);

#ifdef GERAN_L1_ENHANCED_RX
   if(gl1_hw_get_lif_feature_flag(gas_id) &&
      (gl1_msg_get_multi_sim_sys_mode( ) != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
#ifdef FEATURE_SGLTE
      && (gl1_hw_get_sglte_mode(gas_id) == FALSE)
#endif
      )
    {
    NoOfSlots = 1;
    channel_type = (full_rate_tch == TRUE)?GL1_MSG_CS_TCH_AFS:GL1_MSG_CS_TCH_AHS;

    gl1_hw_update_lif_metrics(metrics_ptr,NULL,sm->ARFCN[sm->metrics_num],channel_type,NoOfSlots,
                              GSTMR_GET_FN_GERAN(gas_id),UPDATE_LIF_CS_METRICS,gas_id);
    }
#endif /*GERAN_L1_ENHANCED_RX*/

   if (!blocker)
   {
     /* Slew for Non Blocker. */
     sm->agc_ptr->srl_enabled = TRUE;
#ifdef FEATURE_GSM_RX_DIVERSITY
     sm->agc_ptr_divrx->srl_enabled = TRUE;
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   }
   else
   {
     /* No Slew for Blocker. */
     sm->agc_ptr->srl_enabled = FALSE;
#ifdef FEATURE_GSM_RX_DIVERSITY
	 sm->agc_ptr_divrx->srl_enabled = FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY*/
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

   /* If the metrics callback is defined then get the rest of the metrics */
   if (amr_dl_tch_callback[gas_id].tch_metrics_cb)
   {
      /* Calculation of offset_to_sob: The burst will be received
       * in state RECEIVE_5, which in numbers, it is equivalent to
       * 6. The +1 is due to the increment of the state prior to
       * entering this function. */
      if (full_rate_tch)
      {
         /* If it is in TCH_INACTIVE, it was called during TCH_RECEIVE_5.
          * The internal state is set to TCH_INACTIVE at this point, so,
          * the offset is 0 */
         if (sm->state == TCH_INACTIVE)
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
         if (sm->state == TCH_INACTIVE)
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

         /* We get data in N+1 or N+2 depending on whether loopback is
          * active. The offset to sob needs to be adjusted accordingly
          */
         if ( sm->delay_hr_callbacks )
         {
            metrics_ptr->offset_to_sob++;
         }
      }

      metrics_ptr->arfcn = sm->ARFCN[sm->metrics_num];
      metrics_ptr->valid = TRUE;
      metrics_ptr->time_stamp = time_stamp;

   }

   /* Same story for the log callback, except that we call */
   /* the callback after each frame.                       */
   if (amr_dl_tch_callback[gas_id].log_cb)
   {
      amr_dl_tch_callback[gas_id].log_cb();
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
#endif /*GERAN_L1_HLLL_LNASTATE*/

   sm->metrics_num++;

   return (blocker);

}/*lint !e715*/


/*===========================================================================

FUNCTION amr_tch_process_data_fr

DESCRIPTION
   Get the data that is already available for the given state machine. This is
   called when the state machine is in the state 5 of a tch_hr.

===========================================================================*/
static void amr_tch_process_data_fr (uint8 sm_num, uint16 *channel,gas_id_t gas_id)
{
    static gl1_msg_chan_type chan_type[NUM_GERAN_DATA_SPACES] =
                               { INITIAL_VALUE( GL1_INVALID ) };

    amr_tch_state_machine_type *sm = &amr_tch_params[gas_id].sm[sm_num];
    uint16 data_length;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH	
    uint8 i, all_good_snr, all_good_rxpwr;
    tch_facch_decode_rpt *rpt_data_ptr;
#endif

#ifdef FEATURE_GSM_AMR_WB
    if (sm->burst_info.channel_type == GL1_HW_TCH_WFS)
    {
        data_length = MAX_TCH_OCTETS_AMR_WB;
    }
    else
#endif
    {
        data_length = MAX_TCH_OCTETS_AMR;
    }
    /* Now the data is available, go get it */
    gl1_hw_get_sync_rx_data_amr( sm->burst_info.channel_type,
                                 &sm->tags[DECODE_TAG],
                                 &sm->rpt.data->tch_facch.hdr,
                                 sm->rpt.data->tch_facch.data,
                                 data_length,
                                 gas_id);

    /* Pass back so AGC can use SID Update */
    *channel = sm->rpt.data->tch_facch.hdr.chan;


#ifdef FEATURE_GSM_AMR_WB
    if ( ( (gl1_msg_chan_type)sm->rpt.data->tch_facch.hdr.chan == GL1_MSG_FACCH_AFS ) ||
         ( (gl1_msg_chan_type)sm->rpt.data->tch_facch.hdr.chan == GL1_MSG_FACCH_WFS ) )
#else
    if ( (gl1_msg_chan_type)sm->rpt.data->tch_facch.hdr.chan == GL1_MSG_FACCH_AFS )
#endif
    {
        sm->rpt.data->tch_facch.hdr.bfi = 1;
    }
    else
    {
        uint16 i, j;
        uint32 avg_snr = 0;

        for (i = 0, j = 4; i < 4; i++, j++ )
        {
            amr_fr_snrs[gas_id][i] = amr_fr_snrs[gas_id][j];
            amr_fr_snrs[gas_id][j] = sm->rpt.metrics[i]->snr;

            amr_fr_snrs_saic[gas_id][i] = amr_fr_snrs_saic[gas_id][j];
            amr_fr_snrs_saic[gas_id][j] = sm->rpt.metrics[i]->pre_mimo_trained_complete_snr;

            amr_fr_saic_used[gas_id][i] = amr_fr_saic_used[gas_id][j];
            amr_fr_saic_used[gas_id][j] = gl1_msg_saic_in_burst(sm->rpt.metrics[i]->misc_flags);
        }

        for (i = 0; i < 8; i++)
        {
            /* If SAIC is on, replace BurstSNR by PreSNR if BurstSNR is
               under a threshold */
            #ifdef FEATURE_GSM_AMR_WB
            /* We would like to do this check for all WB_AMR rates */
            if (amr_fr_saic_used[gas_id][i] &&
                (gl1_msg_dl_acs_amr[gas_id][sm->rpt.data->tch_facch.hdr.cmi] >= GL1_MSG_AMR_MODE_1220 &&
                 gl1_msg_dl_acs_amr[gas_id][sm->rpt.data->tch_facch.hdr.cmi] <= GL1_MSG_AMR_WB_MODE_1265) &&
                amr_fr_snrs[gas_id][i] < BFI_SNR_THRESHOLD_SAIC_AFS)
            #else
            if (amr_fr_saic_used[gas_id][i] &&
                gl1_msg_dl_acs_amr[gas_id][sm->rpt.data->tch_facch.hdr.cmi] == GL1_MSG_AMR_MODE_1220 &&
                amr_fr_snrs[gas_id][i] < BFI_SNR_THRESHOLD_SAIC_AFS)
            #endif /*FEATURE_GSM_AMR_WB*/
            {
                avg_snr += amr_fr_snrs_saic[gas_id][i];
            }
            else
            {
                avg_snr += amr_fr_snrs[gas_id][i];
            }
        }

        amr_avg_snr[gas_id] = (uint16)(avg_snr / 8);

        /* Compute BFI. ARM is doing BFI calculation now.*/
        gl1_msg_compute_bfi_amr_fr( sm->rpt.data->tch_facch.hdr.good_data,
#ifdef FEATURE_VAMOS_II  
                                    sm->rpt.data->tch_facch.hdr.vamospaired,
                                    loopback_type[gas_id],
                                    gas_id,
#endif
                                    sm->rpt.data->tch_facch.hdr.rx_qual_s,
                                    amr_avg_snr[gas_id],
                                    gl1_msg_dl_acs_amr[gas_id][sm->rpt.data->tch_facch.hdr.cmi],
                                    &(sm->rpt.data->tch_facch.hdr.bfi) );

        /* This sets the BFI to 1 after the SID Update for MOS. */
        if (chan_type[gas_id] == GL1_MSG_SID_UPDATE_FS)
        {
          sm->rpt.data->tch_facch.hdr.bfi = 1;
        }

    }

    /* Save the last Chan Type for the MOS BFI setting. */
    chan_type[gas_id] = (gl1_msg_chan_type) sm->rpt.data->tch_facch.hdr.chan;

    /* We are reporting in offset 6
    ** eliminate the hardcode and tie this tothe reporting
    ** frame
    ** we report after 4 bursts + 1 frame to decode
    */
    sm->rpt.data->tch_facch.offset_to_sob = 4;

    /* COMMENT */
    /* If there is a metrics callback then call it with the metrics */
    /* data for the four bursts of the block                        */
    if (amr_dl_tch_callback[gas_id].tch_metrics_cb)
    {
        amr_dl_tch_callback[gas_id].tch_metrics_cb( sm->rpt.metrics,
                                            sm->rpt.data->tch_facch.hdr.chan,gas_id);
    }
	
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    rpt_data_ptr = (tch_facch_decode_rpt*)sm->rpt.data;
    all_good_snr = TRUE;
    all_good_rxpwr = TRUE;

    for(i=0;i<4;i++)
    {
        if(sm->rpt.metrics[i]->pwr_dBm_x16 < QSH_REF_GOOD_RX_PWR)
        {
          all_good_rxpwr = FALSE;			
        } 	
        if(gl1_msg_SNR_to_dBx16(sm->rpt.metrics[i]->snr) < QSH_REF_GOOD_SNR)
        {
          all_good_snr = FALSE;
        }
    }

    if((rpt_data_ptr->hdr.rx_qual < qsh_amr_tch_fr_thresholds[curr_dl_acs[gas_id][rpt_data_ptr->hdr.cmi]]) && all_good_rxpwr)
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

    /*------------------------------------------------------------
     * Handle loopbacks for AMR full rate.
     *------------------------------------------------------------*/
    amr_handle_loopback_fr(&(sm->rpt.data->tch_facch),gas_id);

    if (amr_dl_tch_callback[gas_id].data_cb)
    {
        /*------------------------------------------------------------
         * Callback only if either 1)we are not looping back or 2) if
         * we are looping back and it's not a RATSCCH block.
         *------------------------------------------------------------*/
#ifdef FEATURE_GSM_AMR_WB
        if ( (loopback_type[gas_id] == GL1_DEFS_NO_LOOPBACK) ||
             ((gl1_msg_chan_type)(sm->rpt.data->tch_facch.hdr.chan) !=
              GL1_MSG_RATSCCH_FS )                   ||
             ((gl1_msg_chan_type)(sm->rpt.data->tch_facch.hdr.chan) !=
              GL1_MSG_RATSCCH_WFS )
           )
#else
        if ( (loopback_type[gas_id] == GL1_DEFS_NO_LOOPBACK) ||
             ((gl1_msg_chan_type)(sm->rpt.data->tch_facch.hdr.chan) !=
              GL1_MSG_RATSCCH_FS )
           )
#endif /* FEATURE_GSM_AMR_WB */
        {
            amr_dl_tch_callback[gas_id].data_cb( sm->rpt.data,gas_id);
        }
    }
    else
    {
        MSG_GERAN_ERROR_0_G("No dl callback registered");
    }
}



/*===========================================================================

FUNCTION amr_tch_process_data_hr

DESCRIPTION
   Get the data that is already available for the given state machine. This is
   called when the state machine is in the state 5 of a tch_hr.

===========================================================================*/
static void amr_tch_process_data_hr (uint8 sm_num, uint16 *channel,gas_id_t gas_id)
{
    static gl1_msg_chan_type chan_type[NUM_GERAN_DATA_SPACES] =
                               { INITIAL_VALUE( GL1_INVALID ) };

    amr_tch_state_machine_type *sm = &amr_tch_params[gas_id].sm[sm_num];
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH	
    uint8 i, all_good_snr, all_good_rxpwr;
    tch_facch_decode_rpt *rpt_data_ptr;
#endif

    /* Now the data is available, go get it */
    gl1_hw_get_sync_rx_data_amr( sm->burst_info.channel_type,
                                 &sm->tags[DECODE_TAG_HR],
                                 &sm->rpt.data->tch_facch.hdr,
                                 sm->rpt.data->tch_facch.data,
                                 (uint16)MAX_TCH_OCTETS_AMR,
                                 gas_id);

    /* Pass back so AGC can use SID Update */
    *channel = sm->rpt.data->tch_facch.hdr.chan;


    if ( (gl1_msg_chan_type)sm->rpt.data->tch_facch.hdr.chan == GL1_MSG_FACCH_AHS )
    {
        sm->rpt.data->tch_facch.hdr.bfi = 1;
    }
    else
    {
        uint16 i, j;
        uint32 avg_snr = 0;

        for (i = 0, j = 2; i < 2; i++, j++ )
        {
            amr_hr_snrs[gas_id][i] = amr_hr_snrs[gas_id][j];
            amr_hr_snrs[gas_id][j] = sm->rpt.metrics[i]->snr;

            amr_hr_snrs_saic[gas_id][i] = amr_hr_snrs_saic[gas_id][j];
            amr_hr_snrs_saic[gas_id][j] = sm->rpt.metrics[i]->pre_mimo_trained_complete_snr;

            amr_hr_saic_used[gas_id][i] = amr_hr_saic_used[gas_id][j];
            amr_hr_saic_used[gas_id][j] = gl1_msg_saic_in_burst(sm->rpt.metrics[i]->misc_flags);
        }

        for (i = 0; i < 4; i++)
        {
            /* If SAIC is on, replace BurstSNR by PreSNR if BurstSNR is
               under a threshold */
            if (amr_hr_saic_used[gas_id][i] &&
                gl1_msg_dl_acs_amr[gas_id][sm->rpt.data->tch_facch.hdr.cmi] == GL1_MSG_AMR_MODE_0795 &&
                amr_hr_snrs[gas_id][i] < BFI_SNR_THRESHOLD_SAIC_AHS)
            {
                avg_snr += amr_hr_snrs_saic[gas_id][i];
            }
            else
            {
                avg_snr += amr_hr_snrs[gas_id][i];
            }
        }

        amr_avg_snr[gas_id] = (uint16)(avg_snr / 4);

        /* Compute BFI. ARM is doing BFI calculation now.*/
        gl1_msg_compute_bfi_amr_hr(sm->rpt.data->tch_facch.hdr.good_data,
#ifdef FEATURE_VAMOS_II  
                                   sm->rpt.data->tch_facch.hdr.vamospaired,
                                   loopback_type[gas_id],
                                   gas_id,
#endif
                                   &(sm->rpt.data->tch_facch.hdr.bfi),
                                   amr_avg_snr[gas_id],
                                   gl1_msg_dl_acs_amr[gas_id][sm->rpt.data->tch_facch.hdr.cmi],
                                   sm->rpt.data->tch_facch.hdr.rx_qual );

        /* Save the last Chan Type for the MOS BFI setting. */
        if (chan_type[gas_id] == GL1_MSG_SID_UPDATE_HS)
        {
          sm->rpt.data->tch_facch.hdr.bfi = 1;
        }
    }

    /* Save the last Chan Type for the MOS BFI setting. */
    chan_type[gas_id] = (gl1_msg_chan_type) sm->rpt.data->tch_facch.hdr.chan;

    /* We are reporting in offset 6
    ** eliminate the hardcode and tie this to the reporting
    ** frame
    ** we report after 4 bursts + 1 frame to decode
    */
    /* We get data in N+1 or N+2 depending on whether loopback is active.
     * The offset to sob needs to be adjusted accordingly
     */
    sm->rpt.data->tch_facch.offset_to_sob = (sm->delay_hr_callbacks ? 4 : 3);

    /* COMMENT */
    /* If there is a metrics callback then call it with the metrics */
    /* data for the four bursts of the block                        */
    if (amr_dl_tch_callback[gas_id].tch_metrics_cb)
    {
        amr_dl_tch_callback[gas_id].tch_metrics_cb(sm->rpt.metrics,
                                           sm->rpt.data->tch_facch.hdr.chan,gas_id);
    }
	
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    rpt_data_ptr = (tch_facch_decode_rpt*)sm->rpt.data;
    all_good_snr = TRUE;
    all_good_rxpwr = TRUE;

    for(i=0;i<2;i++)
    {
        if(sm->rpt.metrics[i]->pwr_dBm_x16 < QSH_REF_GOOD_RX_PWR)
        {
          all_good_rxpwr = FALSE;			
        } 	
        if(gl1_msg_SNR_to_dBx16(sm->rpt.metrics[i]->snr) < QSH_REF_GOOD_SNR)
        {
            all_good_snr = FALSE;
        }
    }

    if((rpt_data_ptr->hdr.rx_qual < qsh_amr_tch_hr_thresholds[curr_dl_acs[gas_id][rpt_data_ptr->hdr.cmi]]) && all_good_rxpwr)
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

    /*------------------------------------------------------------
     * Handle loopbacks for AMR half rate.
     *------------------------------------------------------------*/
    amr_handle_loopback_hr(&(sm->rpt.data->tch_facch),gas_id);
    if (amr_dl_tch_callback[gas_id].data_cb)
    {
        /*------------------------------------------------------------
         * Callback only if either 1)we are not looping back or 2) if
         * we are looping back and it's not a RATSCCH block.
         *------------------------------------------------------------*/
        if ( (loopback_type[gas_id] == GL1_DEFS_NO_LOOPBACK) ||
             (((gl1_msg_chan_type)sm->rpt.data->tch_facch.hdr.chan
               != GL1_MSG_RATSCCH_HS ) &&
              ((gl1_msg_chan_type)sm->rpt.data->tch_facch.hdr.chan
               != GL1_MSG_RATSCCH_MARKER ) ) )
        {
            amr_dl_tch_callback[gas_id].data_cb( sm->rpt.data,gas_id);
        }
    }
    else
    {
        MSG_GERAN_ERROR_0_G("No dl callback registered");
    }
}



/*===========================================================================
FUNCTION gl1_msg_cfg_dl_tch_facch_amr_fr_hr

DESCRIPTION
   These functions are used to configure the TSC and callbacks for the TCH,
   FACCH.  The callbacks will be invoked by the frame tick handler as required.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_cfg_dl_tch_facch_amr_fr_hr ( uint8 TSC,
                                          gl1_msg_rx_cb_type *cb,gas_id_t gas_id  )
{
    if (!cb)
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL ARFCN pointer");
        return;
    }
    rx_TSC[gas_id] = TSC;
    amr_dl_tch_callback[gas_id].data_cb    = cb->data_cb;
    amr_dl_tch_callback[gas_id].metrics_cb = cb->metrics_cb;
    amr_dl_tch_callback[gas_id].tch_metrics_cb = cb->tch_metrics_cb;
    amr_dl_tch_callback[gas_id].log_cb     = cb->log_cb;
}



/*===========================================================================

FUNCTION dl_tch_amr_add_storage

DESCRIPTION
  This function allocates the next available state machine storage unit
  and prepares it with the given parameters.

===========================================================================*/
static void dl_tch_amr_add_storage
(
   ARFCN_T              ARFCN[4],
   sys_algo_agc_T       *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
   sys_algo_agc_T	    *agc_ptr_divrx,	
#endif /*FEATURE_GSM_RX_DIVERSITY*/ 
   gl1_msg_rx_rpt       *rpt,
   gl1_hw_channel_type  chan,
   boolean              facch_only,
   uint8                phase,
   gas_id_t             gas_id
)
{
   amr_tch_state_machine_type *sm;
   uint32 i;

   sm = &amr_tch_params[gas_id].sm[amr_tch_params[gas_id].sm_wr_ptr];
   amr_tch_params[gas_id].sm_wr_ptr = (amr_tch_params[gas_id].sm_wr_ptr + 1) % NUM_TCH_SM;
   amr_tch_params[gas_id].sm_cnt++;

   /* Fill in the state machine parameters */
   agc_ptr->arfcn.num         = 0xFFFF;
   L1_STATE_CHANGE(sm->state                  = TCH_WAITING_FOR_BLOCK_START, "TCH_WAITING_FOR_BLOCK_START");
   sm->agc_ptr                = agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
   agc_ptr_divrx->arfcn.num	  = 0xFFFF;
   sm->agc_ptr_divrx 		  = agc_ptr_divrx;
#endif
   sm->rpt.data               = rpt->data;

   /* If loopback is active, get half-rate decoded data in N+2.  This will
    * ensure that (as a result of the PL1 scheduling mechanism) the
    * decoded data for the current block does not get looped-back
    * immediately without the usual one block delay.  This scenario
    * happens in the block just before the SACCH or IDLE frame.
    */
   if (loopback_type[gas_id] == GL1_DEFS_NO_LOOPBACK)
   {
      sm->delay_hr_callbacks = FALSE;
   }
   else
   {
      sm->delay_hr_callbacks = TRUE;
   }

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
   sm->burst_info.amr_phase          = phase;
   sm->burst_info.look_for_facch     = facch_only;
   sm->burst_info.channel_type       = chan;
   sm->signal_info.tn                = gl1_msg_get_cs_timeslot(gas_id);
}



/*===========================================================================
FUNCTION gl1_msg_rx_tch_facch_amr_fr

DESCRIPTION
   This function is used to receive a message on either the TCH or the FACCH.
   The message contents should span 4 frames ie 1 block.
   This function may be called at task level but should be called in the block
   before the data is to transmitted.   If data to be received is for
   frame #4,5,6,7, then this function should be completed before start of
   frame #3, ie called in frames 0,1,2.

DEPENDENCIES
   gl1_msg_cfg_dl_tch_facch() , must have been called at some time prior.

RETURN VALUE
 None

SIDE EFFECTS
 None
===========================================================================*/
void gl1_msg_rx_tch_facch_amr_fr
(
   ARFCN_T              ARFCN[4],
   sys_algo_agc_T       *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
   sys_algo_agc_T       *agc_ptr_divrx,   
#endif /*FEATURE_GSM_RX_DIVERSITY*/ 
   gl1_msg_rx_rpt       *rpt,
   uint8                phase,
#ifdef FEATURE_VAMOS_II
                        gl1_vamos2_tch_wait_type vamos2_tch_wait_state,
#endif 
   gas_id_t             gas_id
)
{
   /* Allocate new state machine storage */
   dl_tch_amr_add_storage(ARFCN,
                          agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                          agc_ptr_divrx,	
#endif /*FEATURE_GSM_RX_DIVERSITY*/  
                          rpt,
                          dl_amr_channel_type[gas_id],
                          FALSE,
                          phase,
                          gas_id);

#ifdef FEATURE_VAMOS_II
   vamos2_amr_dl_tch_wait_state[gas_id] = vamos2_tch_wait_state;
#endif 

   /* Add the handler if necessary */
   if (amr_tch_params[gas_id].sm_cnt == 1)
   {
      gl1_msgi_add_ft_handler(amr_tch_rx_handler_fr, GL1_MSG_FT_HDLR_DL,gas_id);
   }
}

/*===========================================================================
FUNCTION gl1_msg_abort_rx_tch_amr_fr

DESCRIPTION
   This function is used to abort TCH/FACCH.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
boolean gl1_msg_abort_rx_tch_amr_fr ( gas_id_t gas_id )
{
   uint8 idx;

   if ( amr_tch_params[gas_id].sm_cnt == 0 )
   {
      /* Reset the fifo to be safe */
      amr_tch_params[gas_id].sm_rd_ptr = 0;
      amr_tch_params[gas_id].sm_wr_ptr = 0;

      for (idx = 0; idx < 8; idx++)
      {
         amr_fr_snrs[gas_id][idx] = 0;
      }
   }
   else if (amr_tch_params[gas_id].sm_cnt > 1)
   {
      /* Let current state machine complete and abort all others */
      idx = (amr_tch_params[gas_id].sm_rd_ptr + 1) % NUM_TCH_SM;
      while (amr_tch_params[gas_id].sm_cnt > 1)
      {
         MSG_GERAN_MED_2_G("AMR TCH fr rx aborting: state=%d, FN=%d",
                  amr_tch_params[gas_id].sm[idx].state,GSTMR_GET_FN_GERAN(gas_id));
         L1_STATE_CHANGE(amr_tch_params[gas_id].sm[idx].state = TCH_INACTIVE, "TCH_INACTIVE");
         amr_tch_params[gas_id].sm_cnt--;
         idx = (idx + 1) % NUM_TCH_SM;
      }

      /* Set the write pointer */
      amr_tch_params[gas_id].sm_wr_ptr = (amr_tch_params[gas_id].sm_rd_ptr + 1) % NUM_TCH_SM;
   }

   return( amr_tch_params[gas_id].sm_cnt == 0 );
}



/*===========================================================================
FUNCTION gl1_msg_set_dl_acs

DESCRIPTION
   This function is used to pass along to the MDSP the the new ACS either
   received in the multi-rate IE from the network or thru the RATSCCH message

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_set_dl_acs( gl1_msg_amr_mode_type *dl_acs,gas_id_t gas_id)
{
    memscpy(gl1_msg_dl_acs_amr[gas_id], sizeof(gl1_msg_dl_acs_amr[gas_id]), dl_acs, 4);
    gl1_hw_set_dl_acs_amr( (mdsp_amr_codec_mode_type *)dl_acs,gas_id );
}


/****************************************************************************
 ****************************************************************************
 ****************************************************************************
   LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/

/*===========================================================================
FUNCTION amr_tch_rx_handler_fr

DESCRIPTION
   This function should be called every frame on the 26 frame multiframe. The
   handler is installed as part of a request to rx a msg by
   gl1_msg_rx_tch_facch_amr_fr().
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
static boolean amr_tch_rx_handler_fr
(
   boolean start_of_block,
   uint32  time_stamp,
   gas_id_t gas_id
)
{
    boolean remove_handler           = FALSE;
    boolean receive_burst;
    boolean metrics_flag;
    boolean decode_flag;
    boolean get_data_flag;
    uint8 cnt, sm_num;
    amr_tch_state_machine_type *sm;
    /* Used for passing SID Update Chan */
    uint16 channel;
    uint8  i;
    /* Interferer Ind. */
    boolean blocker = FALSE;
    dBx16_T burst_avg = 0;
    boolean update_agc = FALSE;
#ifdef FEATURE_GSM_RX_DIVERSITY
    dBx16_T burst_avg_divrx = 0;
    boolean update_agc_divrx = FALSE;
	boolean is_block_start = FALSE;
	int32 agc_init_value;
#endif

    /* Sanity check */
    if (amr_tch_params[gas_id].sm_cnt == 0)
    {
        MSG_GERAN_ERROR_0_G("No active TCH state machines");
        return TRUE;
    }

    /* Go through the state machine fifo */
    sm_num = amr_tch_params[gas_id].sm_rd_ptr;
    for (cnt = 0; cnt < amr_tch_params[gas_id].sm_cnt; cnt++)
    {
        sm = &amr_tch_params[gas_id].sm[sm_num];
        metrics_flag  = FALSE;
        decode_flag   = FALSE;
        receive_burst = FALSE;
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
	        sm->vamos2_tch_wait_state = vamos2_amr_dl_tch_wait_state[gas_id];
#endif
            sm->state++;
            receive_burst = TRUE;
            metrics_flag  = FALSE;
            break;

        case TCH_RECEIVE_1:
#ifdef FEATURE_VAMOS_II
	        receive_burst = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?FALSE:TRUE;
#else
            receive_burst = TRUE;
#endif
            metrics_flag  = FALSE;
            sm->state++;
            break;

        case TCH_RECEIVE_2:
            receive_burst = TRUE;
            metrics_flag  = TRUE;
            sm->state++;
            break;

        case TCH_RECEIVE_3:
#ifdef FEATURE_VAMOS_II
	        receive_burst = (sm->vamos2_tch_wait_state == WAIT_AFTER_BURST2)?FALSE:TRUE;
	        metrics_flag  = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?FALSE:TRUE;
	        decode_flag   = (sm->vamos2_tch_wait_state == NO_WAIT)?TRUE:FALSE;
#else
	        receive_burst = TRUE;
                metrics_flag  = TRUE;
                decode_flag   = TRUE;
#endif
            sm->state++;
            break;

        case TCH_RECEIVE_4:
#ifdef FEATURE_VAMOS_II
	      receive_burst = (sm->vamos2_tch_wait_state != NO_WAIT)?TRUE:FALSE;
	      decode_flag   = (sm->vamos2_tch_wait_state != NO_WAIT)?TRUE:FALSE;
#else
            receive_burst = FALSE;
#endif			 
            metrics_flag  = TRUE;
            sm->state++;
            break;

        case TCH_RECEIVE_5:
            receive_burst = FALSE;
#ifdef FEATURE_VAMOS_II
	    metrics_flag  = (sm->vamos2_tch_wait_state == WAIT_AFTER_BURST2)?FALSE:TRUE;
	    get_data_flag = (sm->vamos2_tch_wait_state == NO_WAIT)?TRUE:FALSE; 		 
            if(sm->vamos2_tch_wait_state == NO_WAIT)
            {				
              L1_STATE_CHANGE(sm->state     = TCH_INACTIVE, "TCH_INACTIVE");
            }
            else
            {
              sm->state = TCH_RECEIVE_6;
            }
#else
            metrics_flag  = TRUE;
            get_data_flag = TRUE;
            L1_STATE_CHANGE(sm->state     = TCH_INACTIVE, "TCH_INACTIVE");
#endif
            break;

#ifdef FEATURE_VAMOS_II
	    case TCH_RECEIVE_6:
	        receive_burst = FALSE; 		
	        metrics_flag  = (sm->vamos2_tch_wait_state != NO_WAIT)?TRUE:FALSE;
	        get_data_flag = (sm->vamos2_tch_wait_state != NO_WAIT)?TRUE:FALSE; 		
            L1_STATE_CHANGE(sm->state     = TCH_INACTIVE, "TCH_INACTIVE");
            break;
#endif

        default:
            receive_burst = FALSE;
            metrics_flag  = FALSE;
            MSG_GERAN_ERROR_1_G("Invalid TCH state: %d",sm->state);
            L1_STATE_CHANGE(sm->state = TCH_INACTIVE, "TCH_INACTIVE");
            break;
        }

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

        /* Check if this is a frame where metrics and log info are available */
        if (metrics_flag)
        {
            blocker = amr_tch_process_metrics(sm_num, time_stamp, TRUE,gas_id);
        }   /* end if */

        /* Ready to get data */
        if (get_data_flag)
        {
            amr_tch_process_data_fr(sm_num, &channel,gas_id);
            /* Prepare dynamic parameters */
            if ( (channel == GL1_MSG_SID_UPDATE_FS)||
                 (channel == GL1_MSG_FACCH_AFS && sm->rpt.data->tch_facch.hdr.good_data) ||
                 (!(sm->rpt.data->tch_facch.hdr.bfi)) )
            {
              for (i=0; i < 4;i++)
              {
                if (!blocker)
                {
                  /* Slew for Non Blocker. */
                  sm->agc_ptr->srl_enabled = TRUE;
#ifdef FEATURE_GSM_RX_DIVERSITY                  
                  sm->agc_ptr_divrx->srl_enabled = TRUE;				
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                }
                else
                {
                  /* No Slew for Blocker. */
                  sm->agc_ptr->srl_enabled = FALSE;
#ifdef FEATURE_GSM_RX_DIVERSITY                  
				  sm->agc_ptr_divrx->srl_enabled = FALSE;				  
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                }

                /* Only average bursts that have good enough SNR */
                if (sm->agc_ptr->srl_enabled == TRUE)
                {
                  burst_avg = sys_algo_agc_average_output(sm->rpt.metrics[i]->pwr_dBm_x16,gas_id);
                  update_agc = TRUE;
                }

#ifdef FEATURE_GSM_RX_DIVERSITY
			   if(gl1_get_rxdiv_agc_start(gas_id))
			   	 {
                 /* Only average bursts that have good enough SNR */
			     /* AGC loop for diversity RX should be ON even if RX diveristy is disabled under good channel conditions
				   * If Rx diversity is disabled for the burst, burst RSSI and pwr_dBm_x16_divrx are set to 0. If Rx diversity 
				   * is disabled, use received power from primary chain to maintain the AGC loop for diversity chain.
				   */
				   
                 if ((sm->agc_ptr_divrx->srl_enabled == TRUE) && (sm->rpt.metrics[i]->pwr_dBm_x16_divrx != 0))
                   {
                   burst_avg_divrx = sys_algo_agc_average_output_divrx(sm->rpt.metrics[i]->pwr_dBm_x16_divrx,gas_id);
                   update_agc_divrx = TRUE;				 
                   }
			     else if((sm->agc_ptr_divrx->srl_enabled == TRUE) && (sm->rpt.metrics[i]->pwr_dBm_x16 != 0))
			       {			   
			       burst_avg_divrx = sys_algo_agc_average_output_divrx(sm->rpt.metrics[i]->pwr_dBm_x16,gas_id);				 
			       update_agc_divrx = TRUE;				 
			       }
			   	}
#endif /*FEATURE_GSM_RX_DIVERSITY*/
              } /* End of for(i=0.. */

              if(update_agc)
              {
                sys_algo_agc_update(sm->agc_ptr,
                                    burst_avg,
                                    sm->rpt.metrics[3]->valid,
                                    sm->rpt.metrics[3]->time_stamp,gas_id);
              }


              /* Use last update, and the rest will roll in frame by frame. */
              sm->signal_info.rx_signal_strength = sm->agc_ptr->pwr_dBm_x16;
#ifdef FEATURE_GERAN_REDUCED_F3
              MSG_GERAN_HIGH_3_G("TCH/AMR FS Block AGC:=%d dBm,FN=%d,B=%d",
                        sm->agc_ptr->pwr_dBm_x16/16,
                        GSTMR_GET_FN_GERAN(gas_id),sm->agc_ptr->srl_enabled);
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
			  sm->signal_info.rx_signal_strength_divrx = 0;
              if(update_agc_divrx)
              {
                sys_algo_agc_update(sm->agc_ptr_divrx,
                                    burst_avg_divrx,
                                    sm->rpt.metrics[3]->valid,
                                    sm->rpt.metrics[3]->time_stamp,gas_id);
				
				/* Use last update, and the rest will roll in frame by frame. */
				sm->signal_info.rx_signal_strength_divrx = sm->agc_ptr_divrx->pwr_dBm_x16;
              }
			  
#ifdef FEATURE_GERAN_REDUCED_F3
              MSG_GERAN_HIGH_3_G("TCH/AMR FS Block AGC Div Rx:=%d dBm,FN=%d,B=%d",
                        sm->agc_ptr_divrx->pwr_dBm_x16/16,
                        GSTMR_GET_FN_GERAN(gas_id),sm->agc_ptr_divrx->srl_enabled);
#endif /*FEATURE_GERAN_REDUCED_F3*/
#endif /*FEATURE_GSM_RX_DIVERSITY*/

              /* Flag this is a valid burst. */
              sm->signal_info.valid = TRUE;
            }
        }

        /* Now go get the bursts if there are any to get */
        if (receive_burst)
        {
            uint8 rfacch_buffer_index;

            if (gl1_msg_is_ciphering_enabled(gas_id))
            {
                gl1_hw_update_ciphering_fn(gas_id);
            }

             /* Always pass the output of the AGC down to Rf driver */
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
#endif /*FEATURE_GSM_RX_DIVERSITY*/

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

              MSG_GERAN_MED_2_G("RFACCH: frame_counters[gas_id].FNmod42432 = %d, FW buffer index = %d",
                  frame_counters[gas_id].FNmod42432, rfacch_buffer_index);

              clear_rfacch_decode_frame(rfacch_buffer_index, gas_id);
            }
            sm->burst_info.rfacch_fw_buffer_index = l1_tch_get_rfacch_fw_buffer_index(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY	
		   is_block_start = (sm->burst_num == 0)?TRUE:FALSE;
#endif

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
           sm->burst_info.amr_partial_decode = gl1_msg_amr_pd_get_status(gas_id);
           sm->burst_info.dtx_detected       = gl1_msg_dtx_detection_get_status(gas_id);
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

    }  /* end for */

    /* Clear out inactive state machines */
    while (amr_tch_params[gas_id].sm[amr_tch_params[gas_id].sm_rd_ptr].state == TCH_INACTIVE)
    {
        amr_tch_params[gas_id].sm[amr_tch_params[gas_id].sm_rd_ptr].burst_info.amr_phase = 0;
        amr_tch_params[gas_id].sm_rd_ptr = (amr_tch_params[gas_id].sm_rd_ptr + 1) % NUM_TCH_SM;
        if (--amr_tch_params[gas_id].sm_cnt == 0) break;
    }
    /* Are there any more messages to receive */
    if (amr_tch_params[gas_id].sm_cnt == 0)
    {
        remove_handler = TRUE;
    }

    return(remove_handler);

}/*lint !e715*/

#ifdef FEATURE_GSM_AMR_WB
/*===========================================================================
FUNCTION gl1_msg_dl_set_wb_amr_mode

DESCRIPTION
   This function should be called whenever we get any assignment, HandOver,
   channel_mode_modify or DTM assignment command. It informs the drivers
   of the use of the WB AMR codec.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_dl_set_wb_amr_mode(boolean wb_amr_flag,gas_id_t gas_id)
{
    dl_amr_channel_type[gas_id] = (wb_amr_flag) ? GL1_HW_TCH_WFS:GL1_HW_TCH_AFS;
}
#endif /*FEATURE_GSM_AMR_WB*/


/*===========================================================================
FUNCTION gl1_msg_rx_tch_facch_amr_hr

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
void gl1_msg_rx_tch_facch_amr_hr
(
   ARFCN_T              ARFCN[4],
   sys_algo_agc_T       *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
   sys_algo_agc_T       *agc_ptr_divrx,	
#endif /*FEATURE_GSM_RX_DIVERSITY*/ 
   gl1_msg_rx_rpt       *rpt,
   boolean              facch_only,
   uint8                phase,
#ifdef FEATURE_VAMOS_II
   gl1_vamos2_tch_wait_type  vamos2_hr_tch_wait_state,
#endif 
   gas_id_t             gas_id
)
{
   /* Allocate new state machine storage */
   dl_tch_amr_add_storage(ARFCN, agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
	             	  agc_ptr_divrx, 
#endif /*FEATURE_GSM_RX_DIVERSITY*/ 
                      rpt, GL1_HW_TCH_AHS, facch_only, phase,
                      gas_id);

#ifdef FEATURE_VAMOS_II
   vamos2_amr_dl_tch_wait_state[gas_id] = vamos2_hr_tch_wait_state;
#endif 

   /* Add the handler if necessary */
   if (amr_tch_params[gas_id].sm_cnt == 1)
   {
      gl1_msgi_add_ft_handler(amr_tch_rx_handler_hr, GL1_MSG_FT_HDLR_DL,gas_id);
   }
}


/*===========================================================================
FUNCTION gl1_msg_abort_rx_tch_amr_hr

DESCRIPTION
   This function is used to abort TCH/FACCH.

DEPENDENCIES
   None

RETURN VALUE
   None
x
SIDE EFFECTS
   None
===========================================================================*/
boolean gl1_msg_abort_rx_tch_amr_hr ( gas_id_t gas_id )
{
   uint8 idx;

   if ( amr_tch_params[gas_id].sm_cnt == 0 )
   {
      /* Reset the fifo to be safe */
      amr_tch_params[gas_id].sm_rd_ptr = 0;
      amr_tch_params[gas_id].sm_wr_ptr = 0;

      amr_loopback_sid_update_saved[gas_id] = FALSE;

      for (idx = 0; idx < 4; idx++)
      {
          amr_hr_snrs[gas_id][idx] = 0;
      }
   }
   else if (amr_tch_params[gas_id].sm_cnt > 1)
   {
      /* Let current state machine complete and abort all others */
      idx = (amr_tch_params[gas_id].sm_rd_ptr + 1) % NUM_TCH_SM;
      while (amr_tch_params[gas_id].sm_cnt > 1)
      {
         MSG_GERAN_MED_2_G("AMR TCH hr rx aborting: state=%d, FN=%d",
                  amr_tch_params[gas_id].sm[idx].state,GSTMR_GET_FN_GERAN(gas_id));
         L1_STATE_CHANGE(amr_tch_params[gas_id].sm[idx].state = TCH_INACTIVE, "TCH_INACTIVE");
         amr_tch_params[gas_id].sm_cnt--;
         idx = (idx + 1) % NUM_TCH_SM;
      }

      /* Set the write pointer */
      amr_tch_params[gas_id].sm_wr_ptr = (amr_tch_params[gas_id].sm_rd_ptr + 1) % NUM_TCH_SM;
   }

   return( amr_tch_params[gas_id].sm_cnt == 0 );
}


/*===========================================================================

FUNCTION amr_tch_rx_handler_hr

DESCRIPTION
   This function should be called every frame on the 26 frame multiframe. The
   handler is installed as part of a request to rx a msg by
   gl1_msg_rx_tch_facch_amr_hr().

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
static boolean amr_tch_rx_handler_hr
(
   boolean start_of_block,
   uint32  time_stamp,
   gas_id_t gas_id
)
{
    boolean remove_handler = FALSE;
    boolean receive_burst;
    boolean metrics_flag;
    boolean get_data_flag;
    boolean decode_flag;
    uint8 cnt, sm_num;
    amr_tch_state_machine_type *sm;
    /* Uded for passing back SID Update Chan. */
    uint16 channel;
    uint8  i;
    /* Interferer Id. */
    boolean blocker = FALSE;
    dBx16_T burst_avg = 0;
    boolean update_agc = FALSE;
#ifdef FEATURE_GSM_RX_DIVERSITY
   dBx16_T burst_avg_divrx = 0;
   boolean update_agc_divrx = FALSE;
   boolean is_block_start = FALSE;
   int32  agc_init_value;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

    /* Sanity check */
    if (amr_tch_params[gas_id].sm_cnt == 0)
    {
        MSG_GERAN_ERROR_0_G("No active TCH state machines");
        return TRUE;
    }

    /* Go through the state machine fifo */
    sm_num = amr_tch_params[gas_id].sm_rd_ptr;
    for (cnt = 0; cnt < amr_tch_params[gas_id].sm_cnt; cnt++)
    {
        sm = &amr_tch_params[gas_id].sm[sm_num];
        metrics_flag  = FALSE;
        decode_flag   = FALSE;
        receive_burst = FALSE;
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
            receive_burst   = TRUE;
#ifdef FEATURE_VAMOS_II
	    sm->vamos2_tch_wait_state = vamos2_amr_dl_tch_wait_state[gas_id];
#endif
            break;

        case TCH_RECEIVE_1:
            receive_burst = FALSE;
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
	    receive_burst = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?FALSE:TRUE;
	    decode_flag	= (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?FALSE:TRUE;
#else
            receive_burst = TRUE;
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
           receive_burst = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?TRUE:FALSE;
           decode_flag   = (sm->vamos2_tch_wait_state == WAIT_AFTER_BLOCK_START)?TRUE:FALSE;
#else
            receive_burst = FALSE;
#endif
            metrics_flag  = FALSE;
            sm->state++;
            break;

        case TCH_RECEIVE_4:
            receive_burst = FALSE;
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
                L1_STATE_CHANGE(sm->state	 = TCH_INACTIVE, "TCH_INACTIVE");				
              }
#else
               get_data_flag = TRUE;
               L1_STATE_CHANGE(sm->state     = TCH_INACTIVE, "TCH_INACTIVE");
#endif
            }
            break;

        case TCH_RECEIVE_5:
            receive_burst = FALSE;
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
            receive_burst = FALSE;		 
            metrics_flag  = FALSE;
            get_data_flag = TRUE;
            L1_STATE_CHANGE(sm->state  = TCH_INACTIVE, "TCH_INACTIVE"); 		 
            break;
#endif		
        default:
            receive_burst = FALSE;
            metrics_flag  = FALSE;
            MSG_GERAN_ERROR_1_G("Invalid TCH state: %d",sm->state);
            L1_STATE_CHANGE(sm->state     = TCH_INACTIVE, "TCH_INACTIVE");
            break;
        }

        /* Check if this is a frame where metrics and log info are available */
        if (metrics_flag)
        {
            blocker = amr_tch_process_metrics(sm_num, time_stamp, FALSE,gas_id);
        }


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
        /* Ready to get data */
        if (get_data_flag)
        {
            amr_tch_process_data_hr(sm_num , &channel,gas_id);
            /* Prepare dynamic parameters */
            if ((channel == GL1_MSG_SID_UPDATE_HS) ||
                (channel == GL1_MSG_FACCH_AHS && sm->rpt.data->tch_facch.hdr.good_data) ||
                (!(sm->rpt.data->tch_facch.hdr.bfi)) )
            {
              for (i=0; i < 2;i++)
              {
                if (!blocker)
                {
                  /* Slew for Non Blocker. */
                  sm->agc_ptr->srl_enabled = TRUE;
#ifdef FEATURE_GSM_RX_DIVERSITY                  
				  sm->agc_ptr_divrx->srl_enabled = TRUE;				  
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                }
                else
                {
                  /* No Slew for Blocker. */
                  sm->agc_ptr->srl_enabled = FALSE;
#ifdef FEATURE_GSM_RX_DIVERSITY                  
				  sm->agc_ptr_divrx->srl_enabled = TRUE;					
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                }

                /* Only average bursts that have good enough SNR */
                if (sm->agc_ptr->srl_enabled == TRUE)
                {
                  burst_avg = sys_algo_agc_average_output(sm->rpt.metrics[i]->pwr_dBm_x16,gas_id);
                  update_agc = TRUE;
                }
#ifdef FEATURE_GSM_RX_DIVERSITY
				if(gl1_get_rxdiv_agc_start(gas_id))
				  {				  
				  /* Only average bursts that have good enough SNR */
				  if ((sm->agc_ptr_divrx->srl_enabled == TRUE) && (sm->rpt.metrics[i]->pwr_dBm_x16_divrx != 0))
				    {
				    burst_avg_divrx = sys_algo_agc_average_output_divrx(sm->rpt.metrics[i]->pwr_dBm_x16_divrx,gas_id);
				    update_agc_divrx = TRUE;					
				    }
				  else if((sm->agc_ptr_divrx->srl_enabled == TRUE) && (sm->rpt.metrics[i]->pwr_dBm_x16 != 0))
				    {				
				    burst_avg_divrx = sys_algo_agc_average_output_divrx(sm->rpt.metrics[i]->pwr_dBm_x16,gas_id);
				    update_agc_divrx = TRUE;					
				    }
				  }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

              } /* End of for(i=0.. */

              if (update_agc)
              {
                sys_algo_agc_update(sm->agc_ptr,
                                    burst_avg,
                                    sm->rpt.metrics[1]->valid,
                                    sm->rpt.metrics[1]->time_stamp,gas_id);
              }


              /* Use last update, and the rest will roll in frame by frame. */
              sm->signal_info.rx_signal_strength = sm->agc_ptr->pwr_dBm_x16;
#ifdef FEATURE_GERAN_REDUCED_F3
              MSG_GERAN_LOW_3_G("TCH/AMR HS Block AGC:=%d dBm,FN=%d,B=%d",
                        sm->agc_ptr->pwr_dBm_x16/16,GSTMR_GET_FN_GERAN(gas_id),sm->agc_ptr->srl_enabled);
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
			  sm->signal_info.rx_signal_strength_divrx = 0;
			  if(update_agc_divrx)
			  {
			    sys_algo_agc_update(sm->agc_ptr_divrx,
								    burst_avg_divrx,
								    sm->rpt.metrics[3]->valid,
								    sm->rpt.metrics[3]->time_stamp,
								    gas_id);
				/* Use last update, and the rest will roll in frame by frame. */
				sm->signal_info.rx_signal_strength_divrx = sm->agc_ptr_divrx->pwr_dBm_x16;
			  }
			  
#ifdef FEATURE_GERAN_REDUCED_F3
			  MSG_GERAN_HIGH_3_G("TCH/AMR FS Block AGC Div Rx:=%d dBm,FN=%d,B=%d",
			  		             sm->agc_ptr_divrx->pwr_dBm_x16/16,
					             GSTMR_GET_FN_GERAN(gas_id),sm->agc_ptr_divrx->srl_enabled);
#endif /*FEATURE_GERAN_REDUCED_F3*/
#endif /*FEATURE_GSM_RX_DIVERSITY*/

              /* Flag this is a valid burst. */
              sm->signal_info.valid = TRUE;
            }
            else
            {
              /* Flag this is an invalid burst. */
              sm->signal_info.valid = FALSE;
            }
        }

        /* Now go get the bursts if there are any to get */
        if (receive_burst)
        {
            uint8 rfacch_buffer_index;

            if (gl1_msg_is_ciphering_enabled(gas_id))
            {
                gl1_hw_update_ciphering_fn(gas_id);
            }

            /* Always apply output of AGC to Rf driver */
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
#endif /*FEATURE_GSM_RX_DIVERSITY*/

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

              MSG_GERAN_MED_2_G("RFACCH: frame_counters[gas_id].FNmod42432 = %d, FW buffer index = %d",
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

    }  /* end for */

    /* Clear out inactive state machines */
    while (amr_tch_params[gas_id].sm[amr_tch_params[gas_id].sm_rd_ptr].state == TCH_INACTIVE)
    {
        amr_tch_params[gas_id].sm[amr_tch_params[gas_id].sm_rd_ptr].burst_info.amr_phase = 0;
        amr_tch_params[gas_id].sm_rd_ptr = (amr_tch_params[gas_id].sm_rd_ptr + 1) % NUM_TCH_SM;
        if (--amr_tch_params[gas_id].sm_cnt == 0) break;
    }
    /* Are there any more messages to receive */
    if (amr_tch_params[gas_id].sm_cnt == 0)
    {
        remove_handler = TRUE;
    }

    return(remove_handler);

}/*lint !e715*/


#endif /* FEATURE_GSM && FEATURE_GSM_AMR */
