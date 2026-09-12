/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         G L 1  M E S S A G E  L A Y E R -  S A C C H   C H A N N E L

GENERAL DESCRIPTION
   This module contains the procedures used for communicating on the SACCH.
   The procedures used herein are intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

   gl1_msg_cfg_ul_sacch
   gl1_msg_cfg_dl_sacch
   gl1_msg_tx_sacch
   gl1_msg_tx_sacch51
   gl1_msg_rx_sacch
   gl1_msg_rx_sacch51

   gl1_msg_abort_tx_sacch
   gl1_msg_abort_tx_sacch51
   gl1_msg_abort_rx_sacch
   gl1_msg_abort_rx_sacch51




INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_sacch.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
07/03/17   mk       CR1097205: explicitly state the array size in all extern definitions
08/12/16   mk       CR1097205: Fixing AGC initialisation when Rxd is getting enabled and enhancing serving cell measurement reporting when Rxd was partially enabled in a SACCH period
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
29/04/15   npt      CR804870:RF timeline error recovery mechanism
23/02/15   jk       CR793098:LIF additional logging changes
16/12/14   jk       CR753423:LIF for Idle,voice and data
19/08/14   pjr      CR711506 Support for AMR Partial Decode and DTX detection FR19667
17/10/14   jk       CR741194:Initialize LIF params to 0 for L2G measurements
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
24/07/14   jk       CR699271:GL1 LIF changes for RxD
10/07/14   jk       CR688587:GL1 changes to support LIF feature
19/12/13   jk       CR503359 - GL1 changes to support Rx diversity
15/08/13   sk       CR524039 GBTA bringup changes
20/06/13   cs       Major Triton TSTS Syncup
13/03/13   mc       CR459487 : COEX Support - GERAN arbitrator framework
08/06/12   pa       GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
15/06/10   cja      CR241658. Add catch for Tx sacch not active (for CS end in EDTM)
04/06/10   cs       Lint/KW fixes
11/05/10   cja      CR237386 For BEP use burst snr, not pre-mimo for high SNR.
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
24/07/09   ws       CR 180476 - Improve performance of Dedicated AGC
01/12/08   cs       Add new include gl1_hw_debug.h
19/03/08   og       Use pre-mimo SNR when producing bep values for enhanced
                    measurements reporting pruposes. Resolves CR141764.
14/01/08    cs      Allow for separate IQ logging and saving
09/11/07    nf      Added FEATURE_GSM_R_SACCH
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
06/07/07   agv      CR123023 - Update fix for AFC/AGC corruption.
27/06/07   agv      CR123023 Fix for AGC corruption, and TT.
23/05/07   agv      CR118309 - Large RSSI variation initial fix.
26/02/07   dv       Fix for CR11743 - SACCH metrics and data callback in wrong
                    sequence.
07/02/07   cs       Fix compile if EDTM not set
23/01/07   agv      Switch SAIC off if Idle Mode to reduce current draw.
12/08/05   gfr      Modify gl1_hw_save_iq_samples to work with L4
08/31/05   gfr      Replace gl1_msg_metrics_rpt with gl1_defs_metrics_rpt to
                    remove need for double copy
08/22/05   gfr      Ignore spurious start of block for transmit
08/09/05   gfr      Set the valid field in the rx burst metrics struture
07/29/05   gfr      DTM support
05/25/05   gfr      Add channel-specific IQ sample saving enabling
04/21/05   yh       Support for panic reset
04/20/05   yh       Remove use of RF fifo
04/12/05   gfr      Consolidate GSM hw layer parameters
03/16/05   yh       Lint
03/15/05   dp       Moved metrics callback before data callback to support
                    EMR
03/09/05   yh       API change for gl1_hw_get_sync_rx_data() and added bep
                    calculation.
12/07/04   yhong    Lint cleanup
09/02/04   yhong    Optimized the rx and tx storages.
06/15/04   gfr      Added early decode and look_for_facch flag to rx.
06/02/04   gfr      Lint cleanup.
03/05/04   gfr      Lint cleanup.
11/05/03   gfr      Support for quad-band.
09/22/03   gfr      Save the TSC with the I/Q samples.
09/10/03   gfr      Support for saving I/Q samples to flash.
07/25/03   gfr      Removed the channel filter delay timing error adjustment.
01/13/03   dp       Adjusted timing error to account for new channel filter
                    delay
10/16/02   dp       Changes to retrieve tx and rx data before each burst
                    in the SACCH 26 rx and tx handlers.  This is to enable
                    hopping sequence changes in the middle of a SACCH block
                    to be picked up (as in freq redefinition )
10/09/02   dp       Undid changes to delay log callback (consequence of
                    reduction in mdsp debug buffer size )
08/29/02   JC       Modified gl1_msg_tx_sacch() and gl1_msg_tx_sacch51() to
                    accept TXLEV as input instead of power index to accomodate
                    FEATURE_GSM_MULTIBAND and because it makes more sense.
08/15/02   bd       Delaying log callback for GPRS
07/11/02   gw       Changed references to power_dBm_x16 in metrics reports to
                    pwr_dBm_x16.
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
06/06/02   mk       Support for aborts on all channels.
05/08/02   JC       Added offset_to_sob to metrics report for logging
                    integrity.
04/30/02   JC       Added ARFCN info to CCCH burst metrics.  Not the cleanest
                    approach, needs further review.
04/19/02   JC       Added rx_gain_range info in metrics callbacks to enhance
                    logging capabilities. Also, added SNR to metrics for
                    logging.
03/29/02   dp       Changed gl1_msg_rx_abort_sacch to return TRUE only if the burst
                    metrics have been retrieved
03/20/02   dp       Changed txlevs to tx power index
03/15/02   mk       Added support for direct frequency correction.
03/05/02   JC       Added AGC support to SACCH-26 handler for dedicated mode.
                    Changed sacch_rx_handler to invoke metrics_callback
                    after receive of all 4 SACCH frames in 104 frame sequence.
02/13/02   jc       Add support for ciphering.
02/05/02   JC       Added support for AGC; Also, call to
                    gl1_hw_get_syncrx_metrics in rx_handler to unwind HW
                    layer.
12/12/01   gw       Changed the way the debug (logging) callback works.
12/07/01   mk       Introduced sacch26 abort for both rx and tx. Changed sacch26
                    handlers to support abort.
11/6/01    bk       Changed NPL1 function calls to reflect change in interface
10/31/01   gw/dp    Added support for logging debug info.  Added support for
                    returning metrics.
                    Made changes to pass in the required number of data octets
                    when retrieving SACCH decoded data.
10/31/01   dp       Made changes to use new gl1_hw_get_syncrx_data interface
                    to eliminate multiple copies when retrieving DSP results
08/31/01   jc       Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

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
#include "gl1_sys_algo.h"
#include "gl1_hwi.h"
#include "msg.h"
#include "gtmrs.h"
#include "gtmrs_g.h"

#include "gl1_sys_params.h"

#include "geran_dual_sim.h"
#ifdef GERAN_L1_ENHANCED_RX
#include "l1_utils.h"
#endif

#ifdef GERAN_L1_HLLL_LNASTATE
#include "l1_utils.h"
#endif

extern boolean l1_ded_drx_agc_init[NUM_GERAN_DATA_SPACES];

/*****************************************************

 LOCAL STORAGE

******************************************************/


#define NUM_SACCH_SM 2
#define NUM_TX_STORAGE 2

/* This is the tag used for retrieving data, corresponds to the index
   where the decode flag was set */
#define DECODE_TAG 3

typedef enum
{
    UNDEFINED,
    SACCH_BLOCK_26,
    SACCH_BLOCK_51
} sacch_block_type;

/* Store the local frames */
typedef struct
{
    /* State Variables */
    boolean                     active;
    uint8                       burst_num;
    uint8                       fn;

    /* Configured tx parameters */
    int16                       power_index[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
    ARFCN_T                     ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
    sacch_block_type            block;

    /* Frame layer parameters */
    gl1_hw_tx_signal_type       signal_info;
    gl1_hw_tx_burst_type        burst_info;
    gl1_hw_tx_tags_type         tags;
} tx_sacch_data_storage_type;

typedef struct
{
    /* Number of messages to send or receive */
    uint8                       tx_sacch_cnt;
    uint8                       tx_sacch_rd_ptr;
    uint8                       tx_sacch_wr_ptr;
    uint8                       tx_TSC;
    gl1_msg_tx_cb_type          tx_sacch_callback;
    tx_sacch_data_storage_type  sm[NUM_TX_STORAGE];
} sacch_tx_params_type;

/* SACCH RX states */
typedef enum
{
   SACCH_INACTIVE = 0,
   SACCH_WAITING_FOR_BLOCK_START = 1,
   SACCH_RECEIVE_1 = 2,
   SACCH_RECEIVE_2 = 3,
   SACCH_RECEIVE_3 = 4,
   SACCH_RECEIVE_4 = 5,
   SACCH_RECEIVE_5 = 6,
   SACCH_RECEIVE_6 = 7,
   SACCH_RECEIVE_26 = 27,
   SACCH_RECEIVE_28 = 29,
   SACCH_RECEIVE_52 = 53,
   SACCH_RECEIVE_54 = 55,
   SACCH_RECEIVE_78 = 79,
   SACCH_RECEIVE_80 = 81,
   SACCH_RECEIVE_81 = 82
} sacch_state_type;

/* Structure containing information for a single SACCH rx */
typedef struct
{
   /* State variables */
   sacch_state_type    state;
   uint8               burst_num;
   uint8               metrics_num;

   /* Configured rx parameters */
   sys_algo_agc_T      *agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY   
   sys_algo_agc_T      *agc_ptr_divrx;
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   ARFCN_T             ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
   sacch_block_type    block;

   /* Frame layer parameters */
   gl1_hw_rx_signal_type     signal_info;
   gl1_hw_rx_burst_type      burst_info;
   gl1_hw_rx_tags_type       tags[GL1_DEFS_FRAMES_IN_BLK_PERIOD];

   /* Callback parameters */
   gl1_msg_rx_rpt      rpt;
} sacch_rx_state_machine_type;

/* Structure containing all SACCH parameters */
typedef struct
{
   /* Array of state machines */
   uint8                   rx_sacch_cnt;
   uint8                   rx_sacch_rd_ptr;
   uint8                   rx_sacch_wr_ptr;
   uint8                   rx_TSC;
   gl1_msg_rx_cb_type      rx_sacch_callback;
   sacch_rx_state_machine_type sm[NUM_SACCH_SM];
} sacch_rx_params_type;

/* Static local storage */
static sacch_rx_params_type sacch_rx_params[NUM_GERAN_DATA_SPACES];
static sacch_tx_params_type sacch_tx_params[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_GSM_RX_DIVERSITY   
static boolean       init_agc_divrx = FALSE;
#endif

/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/

static boolean sacch_tx_handler( boolean start_of_block,
                                 uint32  time_stamp,gas_id_t gas_id );

static boolean sacch51_tx_handler( boolean start_of_block,
                                 uint32  time_stamp,gas_id_t gas_id );

static boolean sacch_rx_handler( boolean start_of_block,
                                 uint32  time_stamp,gas_id_t gas_id );

static boolean sacch51_rx_handler( boolean start_of_block,
                                 uint32  time_stamp,gas_id_t gas_id );

/*****************************************************

 LOCAL FUNCTION

******************************************************/


/*===========================================================================
FUNCTION gl1_msgi_init_sacch

DESCRIPTION
   This function initializes data structures used SACCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_init_sacch(gas_id_t gas_id)
{
   uint8 i = 0;

   /* Init downlink struct */
   sacch_rx_params[gas_id].rx_sacch_cnt                 = 0;
   sacch_rx_params[gas_id].rx_sacch_rd_ptr              = 0;
   sacch_rx_params[gas_id].rx_sacch_wr_ptr              = 0;
   sacch_rx_params[gas_id].rx_sacch_callback.data_cb    = NULL;
   sacch_rx_params[gas_id].rx_sacch_callback.metrics_cb = NULL;
   sacch_rx_params[gas_id].rx_sacch_callback.abort_cb   = NULL;
   sacch_rx_params[gas_id].rx_sacch_callback.log_cb     = NULL;

   for(i = 0; i < NUM_SACCH_SM; i++)
   {
      sacch_rx_params[gas_id].sm[i].state               = SACCH_INACTIVE;
      sacch_rx_params[gas_id].sm[i].burst_num           = 0;
      sacch_rx_params[gas_id].sm[i].metrics_num         = 0;
      sacch_rx_params[gas_id].sm[i].block               = UNDEFINED;
   }

   /* Init uplink struct */
   sacch_tx_params[gas_id].tx_sacch_cnt                 = 0;
   sacch_tx_params[gas_id].tx_sacch_rd_ptr              = 0;
   sacch_tx_params[gas_id].tx_sacch_wr_ptr              = 0;
   sacch_tx_params[gas_id].tx_sacch_callback.data_cb    = NULL;
   sacch_tx_params[gas_id].tx_sacch_callback.metrics_cb = NULL;
   sacch_tx_params[gas_id].tx_sacch_callback.abort_cb   = NULL;
   sacch_tx_params[gas_id].tx_sacch_callback.log_cb     = NULL;

   for(i = 0; i < NUM_TX_STORAGE; i++)
   {
      sacch_tx_params[gas_id].sm[i].block               = UNDEFINED;
      sacch_tx_params[gas_id].sm[i].active              = FALSE;
      sacch_tx_params[gas_id].sm[i].burst_num           = 0;
      sacch_tx_params[gas_id].sm[i].fn                  = 0;
   }
}


/*===========================================================================

FUNCTION gl1_msg_update_sacch_arfcns

DESCRIPTION
  This function updates the arfcns during the middle of a sacch26 msg

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_update_sacch_arfcns( ARFCN_T ARFCN[4],gas_id_t gas_id )
{
  uint8 i;

  if (sacch_rx_params[gas_id].sm[sacch_tx_params[gas_id].tx_sacch_rd_ptr].block == SACCH_BLOCK_26 &&
      sacch_rx_params[gas_id].sm[sacch_rx_params[gas_id].rx_sacch_rd_ptr].block == SACCH_BLOCK_26)
  {
     for (i=0; i<GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
     {
        sacch_tx_params[gas_id].sm[sacch_tx_params[gas_id].tx_sacch_rd_ptr].ARFCN[i] = ARFCN[i];
        sacch_rx_params[gas_id].sm[sacch_rx_params[gas_id].rx_sacch_rd_ptr].ARFCN[i] = ARFCN[i];
     }
  }
  else
  {
     MSG_GERAN_ERROR_1_G("Unexpected sacch block type %d, cannot update ARFCN",
               sacch_rx_params[gas_id].sm[sacch_tx_params[gas_id].tx_sacch_rd_ptr].block);
  }
}



/*===========================================================================

FUNCTION sacch_process_metrics

DESCRIPTION
   Retrieves burst metrics for the given state machine.

===========================================================================*/
static void sacch_process_metrics
(
   sacch_rx_state_machine_type *sm,
   uint32 time_stamp,
   gas_id_t gas_id
)
{
   gl1_defs_metrics_rpt metrics, *metrics_ptr;
   boolean              valid;
   /* Interferer Id. */
   boolean              blocker = FALSE;
#ifdef FEATURE_GSM_RX_DIVERSITY
   boolean              valid_divrx = FALSE;
   int32                agc_init_value;
#endif

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

   /* Get metrics */
   blocker = gl1_hw_get_sync_rx_metrics(
               metrics_ptr, 
               &sm->tags[sm->metrics_num],
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
               LOG_SACCH,
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */
               gas_id);

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
   gl1_hw_get_iq_samples( GL1_HW_SACCH, 2, sm->metrics_num, sm->burst_info.tsc, gas_id );
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */

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
    channel_type = GL1_MSG_CS_SACCH;
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
       /* Slew for Non Blocker. */
       sm->agc_ptr_divrx->srl_enabled = TRUE;
    
	   if(metrics_ptr->pwr_dBm_x16_divrx != 0)
	     {
	     valid_divrx = TRUE;
         /* Perform AGC on the burst */
         sys_algo_agc_update(sm->agc_ptr_divrx, metrics_ptr->pwr_dBm_x16_divrx,
					         valid_divrx, time_stamp,gas_id );	  
	  
	     }
	  else
	     {	
	     valid_divrx = TRUE;
	     /* Perform AGC on the burst */
	     sys_algo_agc_update(sm->agc_ptr_divrx, metrics_ptr->pwr_dBm_x16,
		         			  valid_divrx, time_stamp,gas_id );	  
	     }
      }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   }
   else
   {
      /* No Slew for Blocker. */
      sm->agc_ptr->srl_enabled = FALSE;

      /* Perform AGC on the burst */
      sys_algo_agc_update(sm->agc_ptr, metrics_ptr->pwr_dBm_x16,
                          valid, time_stamp,gas_id );
#ifdef FEATURE_GSM_RX_DIVERSITY
      if(gl1_get_rxdiv_agc_start(gas_id))
      	{      	
		/* Slew for Non Blocker. */
		sm->agc_ptr_divrx->srl_enabled = FALSE;
		
	    if(metrics_ptr->pwr_dBm_x16_divrx != 0)
	      {	    
		  valid_divrx = TRUE;
		  /* Perform AGC on the burst */
		  sys_algo_agc_update(sm->agc_ptr_divrx, metrics_ptr->pwr_dBm_x16_divrx,
							  valid_divrx, time_stamp, gas_id );		
	     }
	    else
	     {   
		 valid_divrx = TRUE;
		 /* Perform AGC on the burst */
		 sys_algo_agc_update(sm->agc_ptr_divrx, metrics_ptr->pwr_dBm_x16,
							valid_divrx, time_stamp, gas_id );		
	     }
       }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   }

   sm->rpt.data->cch.hdr.bep[sm->metrics_num] = gl1_msg_tch_snr_to_bep(metrics_ptr,gas_id);

   /* If the metrics callback is defined then get the metrics.      */
   if (sacch_rx_params[gas_id].rx_sacch_callback.metrics_cb)
   {
      /* sob = report frame 81 (since rx state has been incremented by 1 to accomodate
      ** INACTIVE state, it is 82) - cuurent sm->state which has
      ** been post-incremented (+1) and also -1 since
      ** the actual OTA burst in the prior frame.
      */
      metrics_ptr->offset_to_sob = 82 - (int32)sm->state + 2;
      metrics_ptr->arfcn         = sm->ARFCN[sm->metrics_num];
      metrics_ptr->valid         = TRUE;
   }

   /* Same story for the log callback */
   if (sacch_rx_params[gas_id].rx_sacch_callback.log_cb)
   {
      sacch_rx_params[gas_id].rx_sacch_callback.log_cb();
   }

#ifdef GERAN_L1_HLLL_LNASTATE
   if(gl1_hw_get_sawless_HLLL_support(metrics_ptr->arfcn.band,gas_id))
    {
    gl1_metrics_lnastate.NoOfChannels              = 1;
    gl1_metrics_lnastate.gsm_metrics[0].arfcn      = metrics_ptr->arfcn;
    gl1_metrics_lnastate.gsm_metrics[0].rx_lvl_dbm = metrics_ptr->pwr_dBm_x16;
    gl1_metrics_lnastate.gsm_metrics[0].snr        = metrics_ptr->snr;
    gl1_metrics_lnastate.gsm_metrics[0].jdet_value = metrics_ptr->jdet_value;
    if(gl1_is_current_state_dtm(gas_id))
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
}

/*===========================================================================

FUNCTION sacch51_process_metrics

DESCRIPTION
   Retrieves burst metrics for the given state machine.

===========================================================================*/
static void sacch51_process_metrics
(
   sacch_rx_state_machine_type *sm,
   uint32 time_stamp,
   gas_id_t gas_id
)
{
   gl1_defs_metrics_rpt metrics, *metrics_ptr;
   boolean              valid;
   /* INterferer Id. */
   boolean              blocker = FALSE;
   dBx16_T              burst_avg = 0;
#ifdef FEATURE_GSM_RX_DIVERSITY
   dBx16_T 			    burst_avg_divrx = 0;   
   boolean              valid_divrx = FALSE;
   int32                agc_init_value;
#endif

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

   /* Get the metrics */
   blocker = gl1_hw_get_sync_rx_metrics(
               metrics_ptr, 
               &sm->tags[sm->metrics_num],
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
               LOG_SACCH,
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
    channel_type = GL1_MSG_CS_SACCH;
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

   if (!blocker)
   {
     /* Slew for Non Blocker. */
     sm->agc_ptr->srl_enabled = TRUE;
     burst_avg = sys_algo_agc_average_output(metrics_ptr->pwr_dBm_x16,gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
     if(gl1_get_rxdiv_agc_start(gas_id))
       {
	   sm->agc_ptr_divrx->srl_enabled = TRUE;

	   /* AGC loop for diversity RX should be ON even if RX diveristy is disabled under good channel conditions
	      * If Rx diversity is disabled for the burst, burst RSSI and pwr_dBm_x16_divrx are set to 0. If Rx diversity 
	      * is disabled, use received power from primary chain to maintain the AGC loop for diversity chain.
	      */
	   if (metrics_ptr->pwr_dBm_x16_divrx != 0)
	     {
	     burst_avg_divrx = sys_algo_agc_average_output_divrx(metrics_ptr->pwr_dBm_x16_divrx,gas_id);
	     valid_divrx = TRUE;	   
	     }
	   else
	     {	 
	     burst_avg_divrx = sys_algo_agc_average_output_divrx(metrics_ptr->pwr_dBm_x16,gas_id);
	     valid_divrx = TRUE;	   
	     }
       }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

     /* update AGC when 4 bursts are averaged */
     if(sm->metrics_num == 3)
     {
       /* Perform AGC on the burst */
       sys_algo_agc_update(sm->agc_ptr, burst_avg,
                           valid, time_stamp,gas_id );
#ifdef FEATURE_GSM_RX_DIVERSITY
       if(burst_avg_divrx != 0)
       {              
	    valid_divrx = TRUE;
		sys_algo_agc_update(sm->agc_ptr_divrx, burst_avg_divrx,
							valid_divrx, time_stamp, gas_id );		
       }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
     }
   }
   else
   {
     /* No Slew for Blocker. */
     sm->agc_ptr->srl_enabled = FALSE;

     /* Perform AGC on the burst */
     sys_algo_agc_update(sm->agc_ptr, metrics_ptr->pwr_dBm_x16,
                         valid, time_stamp,gas_id );
#ifdef FEATURE_GSM_RX_DIVERSITY
     if(gl1_get_rxdiv_agc_start(gas_id))
       {
       /* No Slew for Blocker. */
       sm->agc_ptr_divrx->srl_enabled = FALSE;

	   if(metrics_ptr->pwr_dBm_x16_divrx != 0)
	    {	 
	    valid_divrx = TRUE;
        /* Perform AGC on the burst */
        sys_algo_agc_update(sm->agc_ptr_divrx, metrics_ptr->pwr_dBm_x16_divrx,
		    			       valid_divrx, time_stamp, gas_id );	   
	    }
	  else
	    {	 
	    valid_divrx = TRUE;
	    /* Perform AGC on the burst */
	    sys_algo_agc_update(sm->agc_ptr_divrx, metrics_ptr->pwr_dBm_x16,
						   valid_divrx, time_stamp, gas_id );	   
	    }
      }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   }


#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
   gl1_hw_get_iq_samples( GL1_HW_SACCH, 0, sm->metrics_num, sm->burst_info.tsc, gas_id );
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */

   /* If the metrics callback is defined then get the metrics.      */
   if (sacch_rx_params[gas_id].rx_sacch_callback.metrics_cb)
   {
      metrics_ptr->offset_to_sob = 7 - (int32) sm->state + 2;
      metrics_ptr->arfcn         = sm->ARFCN[sm->metrics_num];
      metrics_ptr->valid         = TRUE;
   }

   /* Same story for the log callback, except that we call */
   /* the callback after each frame.                       */
   if (sacch_rx_params[gas_id].rx_sacch_callback.log_cb)
   {
      sacch_rx_params[gas_id].rx_sacch_callback.log_cb();
   }

#ifdef GERAN_L1_HLLL_LNASTATE
   if(gl1_hw_get_sawless_HLLL_support(metrics_ptr->arfcn.band,gas_id))
    {
    gl1_metrics_lnastate.NoOfChannels              = 1;
    gl1_metrics_lnastate.gsm_metrics[0].arfcn      = metrics_ptr->arfcn;
    gl1_metrics_lnastate.gsm_metrics[0].rx_lvl_dbm = metrics_ptr->pwr_dBm_x16;
    gl1_metrics_lnastate.gsm_metrics[0].snr        = metrics_ptr->snr;
    gl1_metrics_lnastate.gsm_metrics[0].jdet_value = metrics_ptr->jdet_value;
    if(gl1_is_current_state_dtm(gas_id))
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
}/*lint !e715 */ /*time_stamp not referenced */


/*===========================================================================

FUNCTION gl1_msg_cfg_ul_sacch  ,  gl1_msg_cfg_dl_sacch

DESCRIPTION
  This function is used to configure the SACCH and callbacks for the SACCH.  The
  callbacks will be invoked by the frame tick handler as required.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_cfg_ul_sacch ( uint8 TSC,
                            gl1_msg_tx_cb_type *cb,
                            gas_id_t gas_id)
{
    sacch_tx_params[gas_id].tx_TSC = TSC;
    sacch_tx_params[gas_id].tx_sacch_callback.data_cb = cb->data_cb;
}


void gl1_msg_cfg_dl_sacch ( uint8 TSC,
                            gl1_msg_rx_cb_type *cb,
                            gas_id_t gas_id)
{
    sacch_rx_params[gas_id].rx_TSC = TSC;
    sacch_rx_params[gas_id].rx_sacch_callback.data_cb    = cb->data_cb;
    sacch_rx_params[gas_id].rx_sacch_callback.metrics_cb = cb->metrics_cb;
    sacch_rx_params[gas_id].rx_sacch_callback.log_cb     = cb->log_cb;

    /* Signal Dedicated to the HW driver. */
    gl1_hw_set_dedicated( TRUE,gas_id );
}


/*===========================================================================

FUNCTION gl1_msg_tx_sacch

DESCRIPTION
  This function is used to transmit a message on the SACCH on the next sacch
  block boundary.  The message contents should span 104 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.

DEPENDENCIES
  gl1_msg_cfg_ul_sacch() and gl1_msg_store_ta(), must have been called at
  some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_tx_sacch ( ARFCN_T ARFCN[4], int16  TXLEV[4], uint8  *data,gas_id_t gas_id )
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
// In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_msg_tx_sacch: Return ");
   return;
#endif
   tx_sacch_data_storage_type *sm;
   int16 TX_PWR_IDX[4];
   uint8 i;

   /* Sanity check */
   if (sacch_tx_params[gas_id].tx_sacch_cnt >= NUM_TX_STORAGE)
   {
      MSG_GERAN_ERROR_1_G("SACCH tx overflow, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

   sm = &sacch_tx_params[gas_id].sm[sacch_tx_params[gas_id].tx_sacch_wr_ptr];
   sacch_tx_params[gas_id].tx_sacch_wr_ptr =
     (sacch_tx_params[gas_id].tx_sacch_wr_ptr + 1) % NUM_TX_STORAGE;
   sacch_tx_params[gas_id].tx_sacch_cnt++;

   gl1_hw_write_tx_data(GL1_HW_SACCH, data, MAX_SACCH_OCTETS, NULL, 0,
                        gl1_msg_get_cs_timeslot(gas_id), &sm->tags,gas_id);

   /* Translate GSM TXLEV to HW power index. ARFCN[0] sufficient since
   ** the MESSAGE (4 blocks) will be transmitted on the same band.
   */
   gl1_msgi_map_txlev_to_pwr_idx(ARFCN[0], TXLEV, TX_PWR_IDX, 4,gas_id);

   /* Store parameters */
   sm->block             = SACCH_BLOCK_26;
   sm->burst_num         = 0;
   sm->active            = FALSE;

   /* Prepare static frame layer parameters */
   memset(&sm->signal_info, 0, sizeof(sm->signal_info));
   memset(&sm->burst_info, 0, sizeof(sm->burst_info));
   sm->signal_info.tn            = gl1_msg_get_cs_timeslot(gas_id);
   sm->burst_info.tsc            = sacch_tx_params[gas_id].tx_TSC;
   sm->burst_info.bsic           = NO_BSIC;
   sm->burst_info.timing_advance = gl1_msg_retrieve_ta(gas_id);
   sm->burst_info.channel_type   = GL1_HW_SACCH;

   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      sm->ARFCN[i] = ARFCN[i];
      sm->power_index[i] = TX_PWR_IDX[i];
   }

   /* register a handler if one does not exist */
   if (sacch_tx_params[gas_id].tx_sacch_cnt == 1)
   {
       gl1_msgi_add_ft_handler( sacch_tx_handler, GL1_MSG_FT_HDLR_UL_PTCCH_SACCH,gas_id );
   }

}


/*===========================================================================

FUNCTION gl1_msg_tx_sacch51

DESCRIPTION
  This function is used to transmit a message on the SACCH on the next sacch
  block boundary.  The message contents should span 104 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.

DEPENDENCIES
  gl1_msg_store_tx_TSC(), gl1_msg_cfg_ul_sacch() and gl1_msg_store_ta(),
  must have been called at some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_tx_sacch51 ( ARFCN_T ARFCN[4], int16  TXLEV[4], uint8  *data ,gas_id_t gas_id)
{
   tx_sacch_data_storage_type *sm;
   int16 TX_PWR_IDX[4];
   uint8 i;

   /* Sanity check */
   if (sacch_tx_params[gas_id].tx_sacch_cnt >= NUM_TX_STORAGE)
   {
      MSG_GERAN_ERROR_1_G("SACCH tx overflow, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

   sm = &sacch_tx_params[gas_id].sm[sacch_tx_params[gas_id].tx_sacch_wr_ptr];
   sacch_tx_params[gas_id].tx_sacch_wr_ptr =
     (sacch_tx_params[gas_id].tx_sacch_wr_ptr + 1) % NUM_TX_STORAGE;
   sacch_tx_params[gas_id].tx_sacch_cnt++;

   gl1_hw_write_tx_data(GL1_HW_SACCH, data, MAX_SACCH_OCTETS, NULL, 0,
                        gl1_msg_get_cs_timeslot(gas_id), &sm->tags,gas_id);

   /* Translate GSM TXLEV to HW power index. ARFCN[0] sufficient since
   ** the MESSAGE (4 blocks) will be transmitted on the same band.
   */
   gl1_msgi_map_txlev_to_pwr_idx(ARFCN[0], TXLEV, TX_PWR_IDX, 4,gas_id);

   /* Fill in the state machine parameters */
   sm->block             = SACCH_BLOCK_51;
   sm->burst_num         = 0;
   sm->active            = FALSE;

   /* Prepare static frame layer parameters */
   memset(&sm->signal_info, 0, sizeof(sm->signal_info));
   memset(&sm->burst_info, 0, sizeof(sm->burst_info));
   sm->signal_info.tn            = gl1_msg_get_cs_timeslot(gas_id);
   sm->burst_info.tsc            = sacch_tx_params[gas_id].tx_TSC;
   sm->burst_info.bsic           = NO_BSIC;
   sm->burst_info.timing_advance = gl1_msg_retrieve_ta(gas_id);
   sm->burst_info.channel_type   = GL1_HW_SACCH;

   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      sm->ARFCN[i] = ARFCN[i];
      sm->power_index[i] = TX_PWR_IDX[i];
   }

   /* register a handler if one does not exist */
   if (sacch_tx_params[gas_id].tx_sacch_cnt == 1)
   {
      gl1_msgi_add_ft_handler( sacch51_tx_handler, GL1_MSG_FT_HDLR_UL,gas_id );
   }
}



/*===========================================================================

FUNCTION gl1_msg_rx_sacch

DESCRIPTION
  This function is used to receive a message on the SACCH on the next sacch
  block boundary.  The message contents should span 104 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.

DEPENDENCIES
  gl1_msg_cfg_ul_sacch() and gl1_msg_store_ta(), must have been called at
  some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_rx_sacch ( ARFCN_T        ARFCN[4],
                        sys_algo_agc_T *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                        sys_algo_agc_T *agc_ptr_divrx,
#endif
                        gl1_msg_rx_rpt *rpt,
                        boolean         rsacch_active,
                        gas_id_t        gas_id)
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
// In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_msg_rx_sacch: Return ");
   return;
#endif
   sacch_rx_state_machine_type *sm;
   uint8 i;

   /* Sanity check */
   if (sacch_rx_params[gas_id].rx_sacch_cnt >= NUM_SACCH_SM)
   {
      MSG_GERAN_ERROR_1_G("SACCH rx overflow, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

   sm = &sacch_rx_params[gas_id].sm[sacch_rx_params[gas_id].rx_sacch_wr_ptr];
   sacch_rx_params[gas_id].rx_sacch_wr_ptr =
     (sacch_rx_params[gas_id].rx_sacch_wr_ptr + 1) % NUM_SACCH_SM;
   sacch_rx_params[gas_id].rx_sacch_cnt++;

   agc_ptr->arfcn.num = 0xFFFF;

   /* Fill in the state machine parameters */
   sm->state             = SACCH_WAITING_FOR_BLOCK_START;
   sm->agc_ptr           = agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
   sm->agc_ptr_divrx 	 = agc_ptr_divrx;
#endif
   sm->rpt.data          = rpt->data;
   sm->block             = SACCH_BLOCK_26;

   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      sm->ARFCN[i] = ARFCN[i];
      sm->rpt.metrics[i] = rpt->metrics[i];
   }

   /* Fill in static frame layer parameters */
   memset(&sm->signal_info, 0, sizeof(sm->signal_info));
   memset(&sm->burst_info, 0, sizeof(sm->burst_info));
   sm->burst_info.tsc                = sacch_rx_params[gas_id].rx_TSC;
   sm->burst_info.channel_type       = GL1_HW_SACCH;
   sm->burst_info.search_width       = GL1_HW_TCH_SEARCH_WIDTH;
   sm->signal_info.tn                = gl1_msg_get_cs_timeslot(gas_id);
   sm->burst_info.look_for_rsacch    = rsacch_active;
   /* Add the handler if necessary */
   if (sacch_rx_params[gas_id].rx_sacch_cnt == 1)
   {
       gl1_msgi_add_ft_handler( sacch_rx_handler, GL1_MSG_FT_HDLR_DL_PTCCH_SACCH,gas_id );
   }
}



/*===========================================================================

FUNCTION gl1_msg_rx_sacch51

DESCRIPTION
  This function is used to receive a message on the SACCH on the next sacch
  block boundary.  The message contents should span 4 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.

DEPENDENCIES
  gl1_msg_cfg_ul_sacch() and gl1_msg_store_ta(), must have been called at
  some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_rx_sacch51 ( ARFCN_T        ARFCN[4],
                          sys_algo_agc_T *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                          sys_algo_agc_T *agc_ptr_divrx,
#endif
                          gl1_msg_rx_rpt *rpt,
                          gas_id_t        gas_id)

{
   sacch_rx_state_machine_type *sm;
   uint8 i;

   /* Sanity check */
   if (sacch_rx_params[gas_id].rx_sacch_cnt >= NUM_SACCH_SM)
   {
      MSG_GERAN_ERROR_1_G("SACCH rx overflow, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

   sm = &sacch_rx_params[gas_id].sm[sacch_rx_params[gas_id].rx_sacch_wr_ptr];
   sacch_rx_params[gas_id].rx_sacch_wr_ptr =
     (sacch_rx_params[gas_id].rx_sacch_wr_ptr + 1) % NUM_SACCH_SM;
   sacch_rx_params[gas_id].rx_sacch_cnt++;

   agc_ptr->arfcn.num = 0xFFFF;

   /* Fill in the state machine parameters */
   sm->state             = SACCH_WAITING_FOR_BLOCK_START;
   sm->agc_ptr           = agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
   sm->agc_ptr_divrx 	 = agc_ptr_divrx;
#endif
   sm->rpt.data          = rpt->data;
   sm->block             = SACCH_BLOCK_51;

   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      sm->ARFCN[i] = ARFCN[i];
      sm->rpt.metrics[i] = rpt->metrics[i];
   }

   /* Fill in static frame layer parameters */
   memset(&sm->signal_info, 0, sizeof(sm->signal_info));
   memset(&sm->burst_info, 0, sizeof(sm->burst_info));
   sm->burst_info.tsc                = sacch_rx_params[gas_id].rx_TSC;
   sm->burst_info.channel_type       = GL1_HW_SACCH;
   sm->burst_info.search_width       = GL1_HW_TCH_SEARCH_WIDTH;
   sm->signal_info.tn                = gl1_msg_get_cs_timeslot(gas_id);

   /* Add the handler if necessary */
   if (sacch_rx_params[gas_id].rx_sacch_cnt == 1)
   {
      gl1_msgi_add_ft_handler( sacch51_rx_handler, GL1_MSG_FT_HDLR_DL,gas_id );
   }
}



/*===========================================================================

FUNCTION gl1_msg_abort_tx_sacch

DESCRIPTION
  This function is used to abort Tx SACCH26.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_abort_tx_sacch ( gas_id_t gas_id )
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
// In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_msg_abort_tx_sacch: Return ");
   return;
#endif
   uint8 cnt, idx;

   /* If we are not active, nothing to do */
   if (sacch_tx_params[gas_id].tx_sacch_cnt == 0) return;

   idx = sacch_tx_params[gas_id].tx_sacch_rd_ptr;
   for (cnt = 0; cnt < sacch_tx_params[gas_id].tx_sacch_cnt; cnt++)
   {
      /* Abort any ongoing transmit */
      (void) gl1_hw_abort_tx_bursts(&sacch_tx_params[gas_id].sm[idx].tags,gas_id);

      sacch_tx_params[gas_id].sm[idx].block = UNDEFINED;
      sacch_tx_params[gas_id].sm[idx].active = FALSE;
      idx = (idx + 1) % NUM_SACCH_SM;
   }

   /* Reset fifo pointers */
   sacch_tx_params[gas_id].tx_sacch_cnt = 0;
   sacch_tx_params[gas_id].tx_sacch_rd_ptr = 0;
   sacch_tx_params[gas_id].tx_sacch_wr_ptr = 0;

   /* Remove handler */
   gl1_msgi_remove_ft_handler( sacch_tx_handler,gas_id );
}



/*===========================================================================

FUNCTION gl1_msg_abort_tx_sacch51

DESCRIPTION
  This function is used to abort Tx SACCH51.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_abort_tx_sacch51 ( gas_id_t gas_id )
{
   uint8 cnt, idx;

   /* If we are not active, nothing to do */
   if (sacch_tx_params[gas_id].tx_sacch_cnt == 0) return;

   idx = sacch_tx_params[gas_id].tx_sacch_rd_ptr;
   for (cnt = 0; cnt < sacch_tx_params[gas_id].tx_sacch_cnt; cnt++)
   {
      /* Abort any ongoing transmit */
      (void) gl1_hw_abort_tx_bursts(&sacch_tx_params[gas_id].sm[idx].tags,gas_id);

      sacch_tx_params[gas_id].sm[idx].block = UNDEFINED;
      sacch_tx_params[gas_id].sm[idx].active = FALSE;
      idx = (idx + 1) % NUM_SACCH_SM;
   }

   /* Reset fifo pointers */
   sacch_tx_params[gas_id].tx_sacch_cnt = 0;
   sacch_tx_params[gas_id].tx_sacch_rd_ptr = 0;
   sacch_tx_params[gas_id].tx_sacch_wr_ptr = 0;

   /* Remove the handler */
   gl1_msgi_remove_ft_handler( sacch51_tx_handler,gas_id );
}


/*===========================================================================

FUNCTION gl1_msg_abort_rx_sacch

DESCRIPTION
  This function is used to abort Rx SACCH26.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_abort_rx_sacch ( gas_id_t gas_id )
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_msg_abort_rx_sacch: Return ");
   return ;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
   uint8 cnt, idx;

   /* If the cnt is greater than zero a handler must be installed */
   if (sacch_rx_params[gas_id].rx_sacch_cnt == 0) return;

   idx = sacch_rx_params[gas_id].rx_sacch_rd_ptr;
   for (cnt = 0; cnt < sacch_rx_params[gas_id].rx_sacch_cnt; cnt++)
   {
      /* Abort frame layer if decoding already started */
      if (sacch_rx_params[gas_id].sm[idx].state > SACCH_RECEIVE_78)
      {
         gl1_hw_abort_rx_decode(&sacch_rx_params[gas_id].sm[idx].tags[DECODE_TAG],gas_id);
      }
      sacch_rx_params[gas_id].sm[idx].state = SACCH_INACTIVE;
      sacch_rx_params[gas_id].sm[idx].block = UNDEFINED;
      sacch_rx_params[gas_id].sm[idx].metrics_num = 0;
      sacch_rx_params[gas_id].sm[idx].burst_num = 0;
      idx = (idx + 1) % NUM_SACCH_SM;
   }

   /* Reset fifo pointers */
   sacch_rx_params[gas_id].rx_sacch_cnt = 0;
   sacch_rx_params[gas_id].rx_sacch_rd_ptr = 0;
   sacch_rx_params[gas_id].rx_sacch_wr_ptr = 0;

   /* Remove handler */
   gl1_msgi_remove_ft_handler( sacch_rx_handler,gas_id );
}



/*===========================================================================

FUNCTION gl1_msg_abort_rx_sacch51

DESCRIPTION
  This function is used to abort Rx SACCH51.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_abort_rx_sacch51 ( gas_id_t gas_id )
{
   uint8 cnt, idx;

   /* Check if anything is active */
   if (sacch_rx_params[gas_id].rx_sacch_cnt == 0) return;

   idx = sacch_rx_params[gas_id].rx_sacch_rd_ptr;
   for (cnt = 0; cnt < sacch_rx_params[gas_id].rx_sacch_cnt; cnt++)
   {
       /* Abort frame layer if decoding already started */
       if (sacch_rx_params[gas_id].sm[idx].state > SACCH_RECEIVE_3)
       {
          gl1_hw_abort_rx_decode(&sacch_rx_params[gas_id].sm[idx].tags[DECODE_TAG],gas_id);
       }

      sacch_rx_params[gas_id].sm[idx].state = SACCH_INACTIVE;
      sacch_rx_params[gas_id].sm[idx].block = UNDEFINED;
      sacch_rx_params[gas_id].sm[idx].metrics_num = 0;
      sacch_rx_params[gas_id].sm[idx].burst_num = 0;
      idx = (idx + 1) % NUM_SACCH_SM;
   }

   /* Reset fifo pointers */
   sacch_rx_params[gas_id].rx_sacch_cnt = 0;
   sacch_rx_params[gas_id].rx_sacch_rd_ptr = 0;
   sacch_rx_params[gas_id].rx_sacch_wr_ptr = 0;

   /* Remove the handler */
   gl1_msgi_remove_ft_handler( sacch51_rx_handler,gas_id );
}


/****************************************************************************
 ****************************************************************************
 ****************************************************************************
                            LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/



/*===========================================================================

FUNCTION sacch_tx_handler

DESCRIPTION
  This function should be called every frame on the 26 frame multiframe. The
  handler is installed as part of a request to tx a msg by gl1_msg_tx_sacch().
  The handler is told when the start of a block is to occur and will call
  the frame layer functions at the proper time.   When the handler senses
  that there are no more messages queued up to send, it will signal that
  it should be deregistered.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if no messages to tx
  FALSE if messages still being transmitted

SIDE EFFECTS
  None
===========================================================================*/
static boolean sacch_tx_handler
(
   boolean start_of_block,
   uint32  timestamp,
   gas_id_t gas_id
)
{
   boolean remove_handler = FALSE;
   boolean encode_flag    = FALSE;
   tx_sacch_data_storage_type *sm = &sacch_tx_params[gas_id].sm[sacch_tx_params[gas_id].tx_sacch_rd_ptr];

   /* Sanity check */
   if (sacch_tx_params[gas_id].tx_sacch_cnt == 0)
   {
      MSG_GERAN_ERROR_0_G("No active Tx SACCH state machines");
      return TRUE;
   }

   /* Check if we are waiting for the block start */
   if (!sm->active)
   {
      if (start_of_block)
      {
         sm->active = TRUE;
         sm->burst_num = 0;
         sm->fn = 0;
         encode_flag = TRUE;
      }
      else
      {
         return remove_handler;
      }
   }

   /* Now transmit the burst if there are bursts to transmit */
   switch (sm->fn++)
   {
      case 0:
      case 26:
      case 52:
      case 78:
      {
         if (gl1_msg_is_ciphering_enabled(gas_id))
         {
            gl1_hw_update_ciphering_fn(gas_id);
         }

         /* Fill in dynamic frame layer parameters */
         sm->signal_info.tx_power_index = sm->power_index[sm->burst_num];
         sm->burst_info.arfcn           = sm->ARFCN[sm->burst_num];
         sm->burst_info.encode_flag     = encode_flag;
         /* Set the slot again in case it changed between bursts */
         sm->signal_info.tn             = gl1_msg_get_cs_timeslot(gas_id);
         /* burst number for tx */
         sm->burst_info.burst_num       = sm->burst_num;

         /* Issue the tx command */
         gl1_hw_tx_dtm_gsm(&sm->signal_info, &sm->burst_info, &sm->tags,gas_id);
         sm->burst_num++;
         break;
      }

      default:
         /* Make lint happy */
         break;
   }


   /* Do the callback in the 1st burst frame ie right before burst is sent*/
   if (sm->fn == 27 && sm->burst_num == 2)
   {
       if (sacch_tx_params[gas_id].tx_sacch_callback.data_cb != NULL)
       {
           sacch_tx_params[gas_id].tx_sacch_callback.data_cb (gas_id);
       }
   }

   /* Now was this the last burst */
   if (sm->burst_num == 4)
   {
       sacch_tx_params[gas_id].tx_sacch_rd_ptr =
         (sacch_tx_params[gas_id].tx_sacch_rd_ptr + 1) % NUM_TX_STORAGE;
       sacch_tx_params[gas_id].tx_sacch_cnt--;

       /* Are there any more messages to transmit */
       if (sacch_tx_params[gas_id].tx_sacch_cnt == 0)
       {
           remove_handler = TRUE;
       }
   }

   return (remove_handler);

/*lint -esym(715,timestamp)*/
}

/*===========================================================================

FUNCTION sacch51_tx_handler

DESCRIPTION
  This function should be called every frame on the 51 frame multiframe. The
  handler is installed as part of a request to tx a msg by gl1_msg_tx_sacch51().
  The handler is told when the start of a block is to occur and will call
  the frame layer functions at the proper time.   When the handler senses
  that there are no more messages queued up to send, it will signal that
  it should be deregistered.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if no messages to tx
  FALSE if messages still being transmitted

SIDE EFFECTS
  None
===========================================================================*/
static boolean sacch51_tx_handler
(
   boolean start_of_block,
   uint32  timestamp,
   gas_id_t gas_id
)
{
   boolean remove_handler = FALSE;
   boolean encode_flag    = FALSE;
   tx_sacch_data_storage_type *sm = &sacch_tx_params[gas_id].sm[sacch_tx_params[gas_id].tx_sacch_rd_ptr];

   /* Sanity check */
   if (sacch_tx_params[gas_id].tx_sacch_cnt == 0)
   {
      MSG_GERAN_ERROR_0_G("No active Tx SACCH state machines");
      return TRUE;
   }

   /* Check if we are waiting for the block start */
   if (!sm->active)
   {
      if (start_of_block)
      {
         sm->active = TRUE;
         sm->burst_num = 0;
         sm->fn = 0;
         encode_flag = TRUE;
      }
      else
      {
         return remove_handler;
      }
   }

   /* Now transmit the burst if there are bursts to transmit */
   switch (sm->fn++)
   {
      case 0:
      case 1:
      case 2:
      case 3:
      {
         if (gl1_msg_is_ciphering_enabled(gas_id))
         {
            gl1_hw_update_ciphering_fn(gas_id);
         }

         /* Fill in dynamic frame layer parameters */
         sm->signal_info.tx_power_index = sm->power_index[sm->burst_num];
         sm->burst_info.arfcn           = sm->ARFCN[sm->burst_num];
         sm->burst_info.encode_flag     = encode_flag;
         /* burst number for tx */
         sm->burst_info.burst_num       = sm->burst_num;

         /* Issue the tx command */
         gl1_hw_tx_dtm_gsm(&sm->signal_info, &sm->burst_info, &sm->tags,gas_id);
         sm->burst_num++;
         break;
      }

      default:
         /* Make lint happy */
         break;
   }

   /* Do the callback in the 1st burst frame ie right before burst is sent*/
   if (sm->burst_num == 2)
   {
      if (sacch_tx_params[gas_id].tx_sacch_callback.data_cb != NULL)
      {
         sacch_tx_params[gas_id].tx_sacch_callback.data_cb (gas_id);
      }
   }

   /* Now was this the last burst */
   if (sm->burst_num == 4)
   {
      sacch_tx_params[gas_id].tx_sacch_rd_ptr =
        (sacch_tx_params[gas_id].tx_sacch_rd_ptr + 1) % NUM_TX_STORAGE;
      sacch_tx_params[gas_id].tx_sacch_cnt--;

      /* Are there any more messages to transmit */
      if (sacch_tx_params[gas_id].tx_sacch_cnt == 0)
      {
         remove_handler = TRUE;
      }
   }

   return (remove_handler);

}



/*===========================================================================

FUNCTION sacch_rx_handler

DESCRIPTION
  This function should be called every frame on the 26 frame multiframe. The
  handler is installed as part of a request to rx a msg by gl1_msg_rx_sacch().
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
static boolean sacch_rx_handler
(
   boolean start_of_block,
   uint32  timestamp,
   gas_id_t gas_id
)
{
   boolean remove_handler = FALSE;
   boolean decode_flag    = FALSE;
   boolean receive_burst  = FALSE;
   boolean metrics_flag   = FALSE;
   boolean get_data       = FALSE;
   uint8   cnt, sm_num;
   sacch_rx_state_machine_type *sm;
#ifdef FEATURE_GSM_RX_DIVERSITY	
   boolean is_block_start = FALSE;
#endif
   /* Sanity check */
   if (sacch_rx_params[gas_id].rx_sacch_cnt == 0)
   {
      MSG_GERAN_ERROR_0_G("No active SACCH state machines");
      return TRUE;
   }

   /* Go through the state machine fifo */
   sm_num = sacch_rx_params[gas_id].rx_sacch_rd_ptr;
   for (cnt = 0; cnt < sacch_rx_params[gas_id].rx_sacch_cnt; cnt++)
   {
      sm = &sacch_rx_params[gas_id].sm[sm_num];
      decode_flag    = FALSE;
      receive_burst  = FALSE;
      metrics_flag   = FALSE;
      get_data       = FALSE;

      /* Kick the state machine */
      switch (sm->state)
      {
      case SACCH_INACTIVE:
          MSG_GERAN_ERROR_0_G("SACCH inactive");
          break;

      case SACCH_WAITING_FOR_BLOCK_START:
          if (!start_of_block) break;

          sm->burst_num   = 0;
          sm->state++;
          receive_burst = TRUE;
          metrics_flag  = FALSE;
          break;

      case SACCH_RECEIVE_26:
      case SACCH_RECEIVE_52:
          receive_burst = TRUE;
          metrics_flag  = FALSE;
          sm->state++;
          break;

      case SACCH_RECEIVE_78:
          receive_burst = TRUE;
          decode_flag   = TRUE;
          metrics_flag  = FALSE;
          sm->state++;
          break;

      case SACCH_RECEIVE_81:
          receive_burst = FALSE;
          metrics_flag  = FALSE;
          get_data      = TRUE;
          sm->state     = SACCH_INACTIVE;
          break;

      case SACCH_RECEIVE_2:
          sm->metrics_num = 0;
          /* fall through */

      case SACCH_RECEIVE_28:
      case SACCH_RECEIVE_54:
      case SACCH_RECEIVE_80:
          receive_burst = FALSE;
          metrics_flag  = TRUE;
          sm->state++;
          break;

      default:
          receive_burst = FALSE;
          metrics_flag  = FALSE;
          sm->state++;
          break;
      }

      if(get_data)
      {
         /* Now the data is available, go get it */
         gl1_hw_get_sync_rx_data(GL1_HW_SACCH,
                                 &sm->tags[DECODE_TAG],
                                 &sm->rpt.data->cch.hdr,
                                 sm->rpt.data->cch.data,
                                 (uint16)MAX_SACCH_OCTETS, /*lint !e727*/
                                 gas_id);

         sm->rpt.data->cch.offset_to_sob = 80;
         sm->rpt.data->cch.hdr.chan = (uint16)GL1_MSG_SACCH;

         if (sacch_rx_params[gas_id].rx_sacch_callback.metrics_cb)
         {
             sacch_rx_params[gas_id].rx_sacch_callback.metrics_cb( sm->rpt.metrics,gas_id );
         }

         if (sacch_rx_params[gas_id].rx_sacch_callback.data_cb)
         {
             sacch_rx_params[gas_id].rx_sacch_callback.data_cb( sm->rpt.data,gas_id);
         }
         else
         {
             MSG_GERAN_HIGH_0_G("No sacch dl callback registered");
         }

      }

      /* Check if this is a frame where metrics and log info are available */
      if (metrics_flag)
      {
         sacch_process_metrics(sm, timestamp,gas_id);
      }

      /* Now go get the bursts if there are any to get */
      if (receive_burst)
      {
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
         sm->burst_info.arfcn               = sm->ARFCN[sm->burst_num];
         sm->burst_info.decode_flag         = decode_flag;
         sm->burst_info.burst_num           = sm->burst_num;
         /* Set the slot in case in changed in between bursts */
         sm->signal_info.tn                 = gl1_msg_get_cs_timeslot(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY	
		 is_block_start = (sm->burst_num == 0)?TRUE:FALSE;
#endif

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
         sm->burst_info.amr_partial_decode = FALSE;
         sm->burst_info.dtx_detected = FALSE;
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */

         /* Issue the receive */
         gl1_hw_rx_dtm_gsm(&sm->signal_info, &sm->burst_info, &sm->tags[sm->burst_num],         
#ifdef FEATURE_GSM_RX_DIVERSITY	
                           is_block_start,
#endif
                           gas_id);
         sm->burst_num++;
      }

      /* Next state machine */
      sm_num = (sm_num + 1) % NUM_SACCH_SM;

   } /* end if */

   /* Clear out inactive state machines */
   while (sacch_rx_params[gas_id].sm[sacch_rx_params[gas_id].rx_sacch_rd_ptr].state == SACCH_INACTIVE)
   {
      sacch_rx_params[gas_id].rx_sacch_rd_ptr =
        (sacch_rx_params[gas_id].rx_sacch_rd_ptr + 1) % NUM_SACCH_SM;
      if (--sacch_rx_params[gas_id].rx_sacch_cnt == 0) break;
   }
   /* Are there any more messages to receive */
   if (sacch_rx_params[gas_id].rx_sacch_cnt == 0)
   {
      remove_handler = TRUE;
   }

   return (remove_handler);

}



/*===========================================================================

FUNCTION sacch51_rx_handler

DESCRIPTION
  This function should be called every frame on the 51 frame multiframe. The
  handler is installed as part of a request to rx a msg by gl1_msg_rx_sacch51().
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
static boolean sacch51_rx_handler
(
   boolean start_of_block,
   uint32  timestamp,
   gas_id_t gas_id
)
{
   boolean remove_handler = FALSE;
   boolean decode_flag    = FALSE;
   boolean receive_burst  = FALSE;
   boolean metrics_flag   = FALSE;
   boolean get_data       = FALSE;
   uint8   cnt, sm_num;
   sacch_rx_state_machine_type *sm;
#ifdef FEATURE_GSM_RX_DIVERSITY	
   boolean is_block_start = FALSE;
#endif

   /* Sanity check */
   if (sacch_rx_params[gas_id].rx_sacch_cnt == 0)
   {
      MSG_GERAN_ERROR_0_G("No active SACCH state machines");
      return TRUE;
   }

   /* Go through the state machine fifo */
   sm_num = sacch_rx_params[gas_id].rx_sacch_rd_ptr;
   for (cnt = 0; cnt < sacch_rx_params[gas_id].rx_sacch_cnt; cnt++)
   {
      sm = &sacch_rx_params[gas_id].sm[sm_num];
      decode_flag    = FALSE;
      receive_burst  = FALSE;
      metrics_flag   = FALSE;
      get_data       = FALSE;

      /* Kick the state machine */
      switch (sm->state)
      {
         case SACCH_INACTIVE:
            MSG_GERAN_ERROR_0_G("SACCH inactive");
            break;

         case SACCH_WAITING_FOR_BLOCK_START:
            if (!start_of_block) break;

            sm->burst_num   = 0;
            sm->state++;
            receive_burst = TRUE;
            metrics_flag  = FALSE;
            break;

         case SACCH_RECEIVE_1:
            receive_burst = TRUE;
            metrics_flag  = FALSE;
            sm->state++;
            break;

         case SACCH_RECEIVE_2:
            receive_burst = TRUE;
            metrics_flag  = TRUE;
            sm->metrics_num = 0;
            sm->state++;
            break;

         case SACCH_RECEIVE_3:
            receive_burst = TRUE;
            decode_flag   = TRUE;
            metrics_flag  = TRUE;
            sm->state++;
            break;

         case SACCH_RECEIVE_4:
         case SACCH_RECEIVE_5:
            receive_burst = FALSE;
            metrics_flag  = TRUE;
            sm->state++;
            break;

         case SACCH_RECEIVE_6:
            receive_burst = FALSE;
            metrics_flag  = FALSE;
            sm->state     = SACCH_INACTIVE;
            get_data      = TRUE;
            break;

         default:
            receive_burst = FALSE;
            metrics_flag  = FALSE;
            MSG_GERAN_ERROR_1_G("Invalid SACCH state: %d",sm->state);
            sm->state = SACCH_INACTIVE;
            break;
      }

      if (get_data)
      {
         /* Now the data is available, go get it */
         gl1_hw_get_sync_rx_data(GL1_HW_SACCH,
                                 &sm->tags[DECODE_TAG],
                                 &sm->rpt.data->cch.hdr,
                                 sm->rpt.data->cch.data,
                                 (uint16)MAX_SACCH_OCTETS,
                                 gas_id);


         /* We are reporting in offset 6
         ** eliminate the hardcode and tie this tothe reporting
         ** frame, i.e., rx_state[machine]
         ** we report after 4 bursts + 1 frame to decode
         */
         sm->rpt.data->cch.offset_to_sob = 5;
         sm->rpt.data->cch.hdr.chan = (uint16)GL1_MSG_SACCH;

         /* If there is a metrics callback then call it with the metrics */
         /* data for the four bursts of the block                        */
         if (sacch_rx_params[gas_id].rx_sacch_callback.metrics_cb)
         {
            sacch_rx_params[gas_id].rx_sacch_callback.metrics_cb( sm->rpt.metrics,gas_id );
         }

         if (sacch_rx_params[gas_id].rx_sacch_callback.data_cb) {
             sacch_rx_params[gas_id].rx_sacch_callback.data_cb( sm->rpt.data,gas_id);
         }
         else {
             MSG_GERAN_HIGH_0_G("No sacch dl callback registered");
         }


      }

      /* Check if this is a frame where metrics and log info are available */
      if (metrics_flag)
      {
          sacch51_process_metrics(sm, timestamp,gas_id);
      }

      /* Now go get the bursts if there are any to get */
      if (receive_burst)
      {
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
         sm->burst_info.arfcn               = sm->ARFCN[sm->burst_num];
         sm->burst_info.decode_flag         = decode_flag;
         sm->burst_info.burst_num           = sm->burst_num;

#ifdef FEATURE_GSM_RX_DIVERSITY	
		 is_block_start = (sm->burst_num == 0)?TRUE:FALSE;
#endif 


#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
         sm->burst_info.amr_partial_decode = FALSE;
         sm->burst_info.dtx_detected = FALSE;
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */

         /* Issue the receive */
         gl1_hw_rx_dtm_gsm(&sm->signal_info, &sm->burst_info, &sm->tags[sm->burst_num],
#ifdef FEATURE_GSM_RX_DIVERSITY	
                           is_block_start,
#endif
                           gas_id);
         sm->burst_num++;
      }

      /* Next state machine */
      sm_num = (sm_num + 1) % NUM_SACCH_SM;

   } /* end for */

   /* Clear out inactive state machines */
   while (sacch_rx_params[gas_id].sm[sacch_rx_params[gas_id].rx_sacch_rd_ptr].state == SACCH_INACTIVE)
   {
      sacch_rx_params[gas_id].rx_sacch_rd_ptr =
        (sacch_rx_params[gas_id].rx_sacch_rd_ptr + 1) % NUM_SACCH_SM;
      if (--sacch_rx_params[gas_id].rx_sacch_cnt == 0) break;
   }
   /* Are there any more messages to receive */
   if (sacch_rx_params[gas_id].rx_sacch_cnt == 0)
   {
      remove_handler = TRUE;
   }

   return (remove_handler);
/*lint -esym(715,timestamp)*/
}

#ifdef FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY
/*===========================================================================

FUNCTION gl1_msg_is_sacch_plus_one

DESCRIPTION
 Check if current frame is one after SACCH has been Tx-ed

DEPENDENCIES
  None

RETURN VALUE
  TRUE if current frame is SACCH + 1 frame

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_is_sacch_plus_one (gas_id_t gas_id)
{
   boolean is_sacch_plus_one = FALSE;
   tx_sacch_data_storage_type *sm = &sacch_tx_params[gas_id].sm[sacch_tx_params[gas_id].tx_sacch_rd_ptr];

   switch (sm->fn)
   {
      case 2:
      case 28:
      case 54:
      case 80:
      {
         is_sacch_plus_one = TRUE;
         break;
      }
      default:
         is_sacch_plus_one = FALSE;
         break;
   }
   return (is_sacch_plus_one);
}
#endif /* FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY */

