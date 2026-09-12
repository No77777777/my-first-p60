/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         G L 1  M E S S A G E  L A Y E R -  S D C C H   C H A N N E L

GENERAL DESCRIPTION
   This module contains the procedures used for communicating on the SDCCH.
   The procedures used herein are intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

   gl1_msg_cfg_ul_sdcch
   gl1_msg_cfg_dl_sdcch
   gl1_msg_tx_sdcch
   gl1_msg_rx_sdcch

   gl1_abort_tx_sdcch
   gl1_abort_rx_sdcch





INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_sdcch.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
07/03/17   mk       CR1097205: explicitly state the array size in all extern definitions
08/12/16   mk       CR1097205: Fixing AGC initialisation when Rxd is getting enabled and enhancing serving cell measurement reporting when Rxd was partially enabled in a SACCH period
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
23/02/15   jk       CR793098:LIF additional logging changes
16/12/14   jk       CR753423:LIF for Idle,voice and data
19/08/14   pjr      CR711506 Support for AMR Partial Decode and DTX detection FR19667
17/10/14   jk       CR741194:Initialize LIF params to 0 for L2G measurements
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
24/07/14   jk       CR699271:GL1 LIF changes for RxD
10/07/14   jk       CR688587:GL1 changes to support LIF feature
19/12/13   jk       CR503359 - GL1 changes to support Rx diversity
04/11/13   mc       CR556742 G+G COEX : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
15/08/13   sk       CR524039 GBTA bringup changes
20/06/13   cs       Major Triton TSTS Syncup
20/05/13   cs       TSTS Updates
10/04/13   ap       CR472264 DSDS: Fix the gas_id problems when the IQ logging is enabled
04/06/10   cs       Lint/KW fixes
11/05/10   cja      CR237386 For BEP use burst snr, not pre-mimo for high SNR.
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
24/07/09   ws       CR 180476 - Improve performance of Dedicated AGC
01/12/08   cs       Add new include gl1_hw_debug.h
01/10/08   cs       Lint HIGH fixes
19/03/08   og       Use pre-mimo SNR when producing bep values for enhanced
                    measurements reporting pruposes. Resolves CR141764.
14/01/08   cs       Allow for separate IQ logging and saving
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
12/06/07   nt       CR118485: Localised fix to disable SDCCH bursts when aborting.
12/08/05   gfr      Modify gl1_hw_save_iq_samples to work with L4
08/31/05   gfr      Replace gl1_msg_metrics_rpt with gl1_defs_metrics_rpt to
                    remove need for double copy
08/22/05   gfr      Ignore spurious uplink block starts and add sanity check
08/09/05   gfr      Set the valid field in the rx burst metrics struture
07/29/05   gfr      DTM support
05/25/05   gfr      Add channel-specific IQ sample saving enabling
04/21/05   yh       Support for panic reset
04/20/05   yh       Remove use of RF fifo
04/12/05   gfr      Consolidate GSM hw layer parameters
03/16/05   yh       Lint
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
08/29/02   JC       Changed gl1_msg_tx_sdcch() to accept TXLEV instead of
                    power index to support multi-band operation and because
                    of better abstraction.
07/11/02   gw       Changed references to power_dBm_x16 in metrics reports to
                    pwr_dBm_x16.
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
06/14/02   dp       Changed tx aborts for SDCCH to return boolean when abort is
                    complete.  i.e we complete txing the block before returning
06/06/02   mk       Support for aborts on all channels.
05/08/02   JC       Added offset_to_sob to metrics and data structs for
                    logging integrity.
05/01/02   mk       Added snr to burst metrics.
04/30/02   JC       Added ARFCN info to CCCH burst metrics.  Not the cleanest
                    approach, needs further review.
04/19/02   JC       Added rx_gain_range info in metrics callbacks to enhance
                    logging capabilities.
03/20/02   dp       Changed txlevs to tx power index
02/13/02   jc       Add support for ciphering.
02/05/02   JC       Added support for AGC; Also, call to
                    gl1_hw_get_syncrx_metrics in rx_handler to unwind HW
                    layer.
12/12/01   gw       Changed the way the debug (logging) callback works.
11/6/01    bk       Changed NPL1 function calls to reflect change in interface
10/31/01   gw/dp    Added support for logging debug info.  Added support for
                    returning metrics.Made changes to pass in the required number
                    of data octets when retrieving SDCCH decoded data.
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
#include "gl1_hwi.h"
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "msg.h"
#include "gtmrs.h"
#include "gtmrs_g.h"

#ifdef GERAN_L1_ENHANCED_RX
#include "l1_utils.h"
#endif 

#ifdef GERAN_L1_HLLL_LNASTATE
#include "l1_utils.h"
#endif

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"


extern boolean l1_ded_drx_agc_init[NUM_GERAN_DATA_SPACES];

/*****************************************************

 LOCAL STORAGE

******************************************************/
#define NUM_SDCCH_SM 2
#define NUM_TX_STORAGE 2

/* This is the tag used for retrieving data, corresponds to the index
   where the decode flag was set */
#define DECODE_TAG 3

/* Store the local frames */
typedef struct
{
    /* State Variables */
    boolean                     active;
    uint8                       burst_num;

    /* Configured tx parameters */
    int16                       power_index[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
    ARFCN_T                     ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD];

    /* Frame layer parameters */
    gl1_hw_tx_signal_type       signal_info;
    gl1_hw_tx_burst_type        burst_info;
    gl1_hw_tx_tags_type         tags;
} tx_sdcch_data_storage_type;

typedef struct
{
    /* Number of messages to send or receive */
    uint8                       tx_sdcch_cnt;
    uint8                       tx_sdcch_rd_ptr;
    uint8                       tx_sdcch_wr_ptr;
    uint8                       tx_TSC;
    gl1_msg_tx_cb_type          tx_sdcch_callback;
    tx_sdcch_data_storage_type  sm[NUM_TX_STORAGE];
} sdcch_tx_params_type;


/* SDCCH RX states */
typedef enum
{
   SDCCH_INACTIVE,
   SDCCH_WAITING_FOR_BLOCK_START,
   SDCCH_RECEIVE_1,
   SDCCH_RECEIVE_2,
   SDCCH_RECEIVE_3,
   SDCCH_RECEIVE_4,
   SDCCH_RECEIVE_5,
   SDCCH_RECEIVE_6
} sdcch_state_type;

/* Structure containing information for a single SDCCH rx */
typedef struct
{
   /* State variables */
   sdcch_state_type    state;
   uint8               burst_num;
   uint8               metrics_num;

   /* Configured rx parameters */
   sys_algo_agc_T      *agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
   sys_algo_agc_T      *agc_ptr_divrx;
#endif
   ARFCN_T             ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD];

   /* Frame layer parameters */
   gl1_hw_rx_signal_type  signal_info;
   gl1_hw_rx_burst_type   burst_info;
   gl1_hw_rx_tags_type    tags[GL1_DEFS_FRAMES_IN_BLK_PERIOD];

   /* Callback parameters */
   gl1_msg_rx_rpt      rpt;
} sdcch_rx_state_machine_type;

/* Structure containing all SACCH parameters */
typedef struct
{
   /* Array of state machines */
   uint8                   rx_sdcch_cnt;
   uint8                   rx_sdcch_rd_ptr;
   uint8                   rx_sdcch_wr_ptr;
   uint8                   rx_TSC;
   gl1_msg_rx_cb_type      rx_sdcch_callback;
   sdcch_rx_state_machine_type sm[NUM_SDCCH_SM];
} sdcch_rx_params_type;

/* Static local storage */
static sdcch_rx_params_type sdcch_rx_params[NUM_GERAN_DATA_SPACES];
static sdcch_tx_params_type sdcch_tx_params[NUM_GERAN_DATA_SPACES];

/*Controls the aborting of the SDCCH TX handler, to allow it to
finish without transmitting */
static boolean abort_tx_received[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
#ifdef FEATURE_GSM_RX_DIVERSITY   
static boolean       init_agc_divrx = FALSE;
#endif

/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/

static boolean sdcch_tx_handler( boolean start_of_block,
                                 uint32  time_stamp,gas_id_t gas_id );

static boolean sdcch_rx_handler( boolean start_of_block,
                                 uint32  time_stamp,gas_id_t gas_id );

static void sdcch_process_metrics( sdcch_rx_state_machine_type *sm,
                                   uint32 time_stamp,gas_id_t gas_id);


/*===========================================================================
FUNCTION gl1_msgi_init_sdcch

DESCRIPTION
   This function initializes data structures used SDCCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_init_sdcch(gas_id_t gas_id)
{
   uint8 i = 0;

   sdcch_rx_params[gas_id].rx_sdcch_cnt                 = 0;
   sdcch_rx_params[gas_id].rx_sdcch_rd_ptr              = 0;
   sdcch_rx_params[gas_id].rx_sdcch_wr_ptr              = 0;
   sdcch_rx_params[gas_id].rx_sdcch_callback.data_cb    = NULL;
   sdcch_rx_params[gas_id].rx_sdcch_callback.metrics_cb = NULL;
   sdcch_rx_params[gas_id].rx_sdcch_callback.abort_cb   = NULL;
   sdcch_rx_params[gas_id].rx_sdcch_callback.log_cb     = NULL;

   for(i = 0; i < NUM_SDCCH_SM; i++)
   {
      L1_STATE_CHANGE(sdcch_rx_params[gas_id].sm[i].state = SDCCH_INACTIVE, "SDCCH_INACTIVE");
      sdcch_rx_params[gas_id].sm[i].burst_num           = 0;
      sdcch_rx_params[gas_id].sm[i].metrics_num         = 0;
   }

   sdcch_tx_params[gas_id].tx_sdcch_cnt                 = 0;
   sdcch_tx_params[gas_id].tx_sdcch_rd_ptr              = 0;
   sdcch_tx_params[gas_id].tx_sdcch_wr_ptr              = 0;
   sdcch_tx_params[gas_id].tx_sdcch_callback.data_cb    = NULL;
   sdcch_tx_params[gas_id].tx_sdcch_callback.metrics_cb = NULL;
   sdcch_tx_params[gas_id].tx_sdcch_callback.abort_cb   = NULL;
   sdcch_tx_params[gas_id].tx_sdcch_callback.log_cb     = NULL;

   for(i = 0; i < NUM_TX_STORAGE; i++)
   {
      sdcch_tx_params[gas_id].sm[i].active              = FALSE;
      sdcch_tx_params[gas_id].sm[i].burst_num           = 0;
   }
}

/*===========================================================================

FUNCTION sdcch_process_metrics

DESCRIPTION
   Retrieves burst metrics for the given state machine.

===========================================================================*/
static void sdcch_process_metrics
(
   sdcch_rx_state_machine_type *sm,
   uint32 time_stamp,
   gas_id_t gas_id
)
{
   gl1_defs_metrics_rpt metrics, *metrics_ptr;
   boolean              valid;
   /* Intferer Id. */
   boolean              blocker = FALSE;
   dBx16_T              burst_avg = 0;
#ifdef FEATURE_GSM_RX_DIVERSITY
   dBx16_T			   burst_avg_divrx = 0;
   boolean			   valid_divrx = FALSE;  
   int32 			   agc_init_value;
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

   /* Get metrics */
   blocker = gl1_hw_get_sync_rx_metrics(
   		metrics_ptr, 
                &sm->tags[sm->metrics_num],
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
                LOG_SDCCH,
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */
                gas_id );

   valid = (metrics_ptr->powerStatus == RF_IN_RANGE ? TRUE : FALSE);


#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
   gl1_hw_get_iq_samples( GL1_HW_SDCCH, 0, sm->metrics_num, sm->burst_info.tsc, gas_id );
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES || FEATURE_GSM_LOG_IQ_SAMPLES */

#ifdef GERAN_L1_ENHANCED_RX
   if(gl1_hw_get_lif_feature_flag(gas_id) &&
      (gl1_msg_get_multi_sim_sys_mode( ) != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
#ifdef FEATURE_SGLTE
      && (gl1_hw_get_sglte_mode(gas_id) == FALSE)
#endif
     )
    {
    NoOfSlots = 1;
    channel_type = GL1_MSG_CS_SDCCH;
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
		/* Slew for Non Blocker. */
		sm->agc_ptr_divrx->srl_enabled = TRUE;
 
		/* AGC loop for diversity RX should be ON even if RX diveristy is disabled under good channel conditions
			* If Rx diversity is disabled for the burst, burst RSSI and pwr_dBm_x16_divrx are set to 0. If Rx diversity 
			* is disabled, use received power from primary chain to maintain the AGC loop for diversity chain.
			*/
		if (metrics_ptr->pwr_dBm_x16_divrx != 0)
		  {
		  valid_divrx = TRUE;
		  burst_avg_divrx = sys_algo_agc_average_output_divrx(metrics_ptr->pwr_dBm_x16_divrx,gas_id);
		
		  }
		else
		  {   
		  valid_divrx = TRUE;
		  burst_avg_divrx = sys_algo_agc_average_output_divrx(metrics_ptr->pwr_dBm_x16,gas_id);
		 
		  }
	  }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

     /* Update AGC when 4 Bursts have been averaged */
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
	    /* Slew for Non Blocker. */
	    sm->agc_ptr_divrx->srl_enabled = FALSE;
	    if (metrics_ptr->pwr_dBm_x16_divrx != 0)
	     {
	     valid_divrx = TRUE;
		 sys_algo_agc_update(sm->agc_ptr_divrx, metrics_ptr->pwr_dBm_x16_divrx,
		 					valid_divrx, time_stamp,gas_id );		
	     }
	    else
	     {	  
	     valid_divrx = TRUE;
	     sys_algo_agc_update(sm->agc_ptr_divrx, metrics_ptr->pwr_dBm_x16,
		 				    valid_divrx, time_stamp, gas_id );		
	     }
	   }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   }

   sm->rpt.data->cch.hdr.bep[sm->metrics_num] = gl1_msg_tch_snr_to_bep(metrics_ptr,gas_id);

    /* If the metrics callback is defined then get the metrics.      */
   if (sdcch_rx_params[gas_id].rx_sdcch_callback.metrics_cb)
   {
      /* Report on 6 (since rx state has been incremented by 1 to accomodate
      ** INACTIVE state, it is 7), sm->state is post-incremented so what
      ** would have been sm->state=2 shows up as 3 (so need +1)
      ** but also burst actually appears one frame prior OTA (+1)
      ** these cancel out.
      */
      metrics_ptr->offset_to_sob = 7 - (int32) sm->state + 2;
      metrics_ptr->arfcn         = sm->ARFCN[sm->metrics_num];
      metrics_ptr->valid         = TRUE;
   }

   /* Same story for the log callback, except that we call */
   /* the callback after each frame.                       */
   if (sdcch_rx_params[gas_id].rx_sdcch_callback.log_cb)
   {
      sdcch_rx_params[gas_id].rx_sdcch_callback.log_cb();
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

FUNCTION gl1_msg_cfg_ul_sdcch  ,  gl1_msg_cfg_dl_sdcch

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
void gl1_msg_cfg_ul_sdcch ( uint8 TSC,
                            gl1_msg_tx_cb_type *cb,
                            gas_id_t gas_id)
{
    sdcch_tx_params[gas_id].tx_TSC = TSC;
    sdcch_tx_params[gas_id].tx_sdcch_callback.data_cb = cb->data_cb;
}



void gl1_msg_cfg_dl_sdcch ( uint8 TSC,
                            gl1_msg_rx_cb_type *cb,
                            gas_id_t gas_id)
{
    sdcch_rx_params[gas_id].rx_TSC = TSC;
    sdcch_rx_params[gas_id].rx_sdcch_callback.data_cb    = cb->data_cb;
    sdcch_rx_params[gas_id].rx_sdcch_callback.metrics_cb = cb->metrics_cb;
    sdcch_rx_params[gas_id].rx_sdcch_callback.log_cb     = cb->log_cb;
}


/*===========================================================================

FUNCTION gl1_msg_tx_sdcch

DESCRIPTION
  This function is used to transmit a message on the SDCCH on the next sdcch
  block boundary.  The message contents should span 104 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.

DEPENDENCIES
  gl1_msg_cfg_ul_sdcch() and gl1_msg_store_ta(), must have been called at
  some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_tx_sdcch ( ARFCN_T ARFCN[4], int16  TXLEV[4], uint8  *data,gas_id_t gas_id )
{
   tx_sdcch_data_storage_type *sm;
   int16 TX_PWR_IDX[4];
   uint8 i;

   /* Sanity check */
   if (sdcch_tx_params[gas_id].tx_sdcch_cnt >= NUM_TX_STORAGE)
   {
      MSG_GERAN_ERROR_1_G("SDCCH tx overflow, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

   sm = &sdcch_tx_params[gas_id].sm[sdcch_tx_params[gas_id].tx_sdcch_wr_ptr];
   sdcch_tx_params[gas_id].tx_sdcch_wr_ptr =
     (sdcch_tx_params[gas_id].tx_sdcch_wr_ptr + 1) % NUM_TX_STORAGE;
   sdcch_tx_params[gas_id].tx_sdcch_cnt++;

   gl1_hw_write_tx_data(GL1_HW_SDCCH, data, MAX_SDCCH_OCTETS, NULL, 0,
                        gl1_msg_get_cs_timeslot(gas_id), &sm->tags,gas_id);

   /* Translate GSM TXLEV to HW power index. ARFCN[0] sufficient since
   ** the MESSAGE (4 blocks) will be transmitted on the same band.
   */
   gl1_msgi_map_txlev_to_pwr_idx(ARFCN[0], TXLEV, TX_PWR_IDX, 4,gas_id);

   /* Store the parameters */
   sm->burst_num         = 0;
   sm->active            = FALSE;

   /* Prepare static frame layer parameters */
   memset(&sm->signal_info, 0, sizeof(sm->signal_info));
   memset(&sm->burst_info, 0, sizeof(sm->burst_info));
   sm->signal_info.tn            = gl1_msg_get_cs_timeslot(gas_id);
   sm->burst_info.tsc            = sdcch_tx_params[gas_id].tx_TSC;
   sm->burst_info.bsic           = NO_BSIC;
   sm->burst_info.timing_advance = gl1_msg_retrieve_ta(gas_id);
   sm->burst_info.channel_type   = GL1_HW_SDCCH;

   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      sm->ARFCN[i] = ARFCN[i];
      sm->power_index[i] = TX_PWR_IDX[i];
   }

   /* register a handler if one does not exist */
   if (sdcch_tx_params[gas_id].tx_sdcch_cnt == 1)
   {
       gl1_msgi_add_ft_handler( sdcch_tx_handler, GL1_MSG_FT_HDLR_UL,gas_id );
   }
}



/*===========================================================================

FUNCTION gl1_msg_rx_sdcch

DESCRIPTION
  This function is used to receive a message on the SDCCH on the next sdcch
  block boundary.  The message contents should span 104 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.

DEPENDENCIES
  gl1_msg_cfg_ul_sdcch() and gl1_msg_store_ta(), must have been called at
  some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_rx_sdcch ( ARFCN_T         ARFCN[4],
                        sys_algo_agc_T  *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                        sys_algo_agc_T	*agc_ptr_divrx,
#endif
                        gl1_msg_rx_rpt  *rpt,
                        gas_id_t gas_id)

{
   sdcch_rx_state_machine_type *sm;
   uint8 i;

   /* Sanity check */
   if (sdcch_rx_params[gas_id].rx_sdcch_cnt >= NUM_SDCCH_SM)
   {
      MSG_GERAN_ERROR_1_G("SDCCH rx overflow, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

   sm = &sdcch_rx_params[gas_id].sm[sdcch_rx_params[gas_id].rx_sdcch_wr_ptr];
   sdcch_rx_params[gas_id].rx_sdcch_wr_ptr =
     (sdcch_rx_params[gas_id].rx_sdcch_wr_ptr + 1) % NUM_SDCCH_SM;
   sdcch_rx_params[gas_id].rx_sdcch_cnt++;

   agc_ptr->arfcn.num = 0xFFFF;
   /* Fill in the state machine parameters */
   L1_STATE_CHANGE(sm->state             = SDCCH_WAITING_FOR_BLOCK_START, "SDCCH_WAITING_FOR_BLOCK_START");
   sm->agc_ptr           = agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
   sm->agc_ptr_divrx 	 = agc_ptr_divrx;
#endif
   sm->rpt.data          = rpt->data;

   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      sm->ARFCN[i] = ARFCN[i];
      sm->rpt.metrics[i] = rpt->metrics[i];
   }

   /* Fill in static frame layer parameters */
   memset(&sm->signal_info, 0, sizeof(sm->signal_info));
   memset(&sm->burst_info, 0, sizeof(sm->burst_info));
   sm->burst_info.tsc            = sdcch_rx_params[gas_id].rx_TSC;
   sm->burst_info.channel_type   = GL1_HW_SDCCH;
   sm->burst_info.search_width   = GL1_HW_TCH_SEARCH_WIDTH;
   sm->signal_info.tn            = gl1_msg_get_cs_timeslot(gas_id);

   /* Add the handler if necessary */
   if (sdcch_rx_params[gas_id].rx_sdcch_cnt == 1)
   {
      gl1_msgi_add_ft_handler(sdcch_rx_handler, GL1_MSG_FT_HDLR_DL,gas_id);
   }

}



/*===========================================================================

FUNCTION gl1_msg_abort_rx_sdcch

DESCRIPTION
  This function is used to abort SDCCH.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_abort_rx_sdcch ( gas_id_t gas_id )
{
   uint8 cnt, idx;

   /* Check if we are active */
   if (sdcch_rx_params[gas_id].rx_sdcch_cnt == 0) return;

   idx = sdcch_rx_params[gas_id].rx_sdcch_rd_ptr;
   for (cnt = 0; cnt < sdcch_rx_params[gas_id].rx_sdcch_cnt; cnt++)
   {
      /* Abort frame layer if decoding already started */
      if (sdcch_rx_params[gas_id].sm[idx].state > SDCCH_RECEIVE_3)
      {
         gl1_hw_abort_rx_decode(&sdcch_rx_params[gas_id].sm[idx].tags[DECODE_TAG],gas_id);
      }
      L1_STATE_CHANGE(sdcch_rx_params[gas_id].sm[idx].state = SDCCH_INACTIVE, "SDCCH_INACTIVE");
      sdcch_rx_params[gas_id].sm[idx].metrics_num = 0;
      sdcch_rx_params[gas_id].sm[idx].burst_num = 0;
      idx = (idx + 1) % NUM_SDCCH_SM;
   }

   /* Reset fifo pointers */
   sdcch_rx_params[gas_id].rx_sdcch_cnt = 0;
   sdcch_rx_params[gas_id].rx_sdcch_rd_ptr = 0;
   sdcch_rx_params[gas_id].rx_sdcch_wr_ptr = 0;

   /* Remove the handler */
   gl1_msgi_remove_ft_handler( sdcch_rx_handler,gas_id );
}



/*===========================================================================

FUNCTION gl1_msg_abort_tx_sdcch

DESCRIPTION
  This function is used to terminate any SDCCH bursts pending.  Terminates
  after the block completes

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_abort_tx_sdcch ( gas_id_t gas_id )
{
   uint8 cnt, idx;

   /* Check if we are active */
   if (sdcch_tx_params[gas_id].tx_sdcch_cnt == 0) return TRUE;

   idx = sdcch_tx_params[gas_id].tx_sdcch_rd_ptr;
   if (sdcch_tx_params[gas_id].sm[idx].burst_num == 0)
   {
      for (cnt = 0; cnt < sdcch_tx_params[gas_id].tx_sdcch_cnt; cnt++)
      {
          sdcch_tx_params[gas_id].sm[idx].burst_num = 0;
          sdcch_tx_params[gas_id].sm[idx].active = FALSE;
          idx = (idx + 1) % NUM_SDCCH_SM;
      }

      /* Reset fifo pointers */
      sdcch_tx_params[gas_id].tx_sdcch_cnt = 0;
      sdcch_tx_params[gas_id].tx_sdcch_rd_ptr = 0;
      sdcch_tx_params[gas_id].tx_sdcch_wr_ptr = 0;

      gl1_msgi_remove_ft_handler ( sdcch_tx_handler,gas_id );
      return TRUE;
   }
   else
   {
      /* Stop the TX's from getting scheduled but allow the handler to finish */
      abort_tx_received[gas_id] = TRUE;
      return FALSE;
   }
}



/****************************************************************************
 ****************************************************************************
 ****************************************************************************
                            LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/



/*===========================================================================

FUNCTION sdcch_tx_handler

DESCRIPTION
  This function should be called every frame on the 26 frame multiframe. The
  handler is installed as part of a request to tx a msg by gl1_msg_tx_sdcch().
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
static boolean sdcch_tx_handler
(
   boolean start_of_block,     /* next frame is start of block */
   uint32  timestamp,           /* currently not used           */
   gas_id_t gas_id
)
{
   boolean remove_handler = FALSE;
   boolean encode_flag    = FALSE;
   tx_sdcch_data_storage_type *sm = &sdcch_tx_params[gas_id].sm[sdcch_tx_params[gas_id].tx_sdcch_rd_ptr];

   /* Is this the start of a 4 frame block */
   if (!sm->active)
   {
       if (start_of_block)
       {
           sm->active = TRUE;
           sm->burst_num = 0;
           encode_flag = TRUE;
           abort_tx_received[gas_id] = FALSE;
       }
       else
       {
          return remove_handler;
       }
   }

   /* Now transmit the burst if there are bursts to transmit */
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

   if ( !abort_tx_received[gas_id] )
   {
     /* Issue the tx command */
     gl1_hw_tx_dtm_gsm(&sm->signal_info, &sm->burst_info, &sm->tags,gas_id);
   }

   sm->burst_num++;

   /* Do the callback in the 1st burst frame ie right before burst is sent*/
   if (sm->burst_num == 2)
   {
       if (sdcch_tx_params[gas_id].tx_sdcch_callback.data_cb != NULL)
       {
           sdcch_tx_params[gas_id].tx_sdcch_callback.data_cb (gas_id);
       }
   }

   /* Now was this the last burst */
   if (sm->burst_num == 4)
   {
      sm->active = FALSE;
      sdcch_tx_params[gas_id].tx_sdcch_rd_ptr =
        (sdcch_tx_params[gas_id].tx_sdcch_rd_ptr + 1) % NUM_TX_STORAGE ;
      sdcch_tx_params[gas_id].tx_sdcch_cnt--;

      /* Are there any more messages to transmit */
      if (sdcch_tx_params[gas_id].tx_sdcch_cnt == 0)
      {

        if (abort_tx_received[gas_id])
        {
          (void) gl1_hw_abort_tx_bursts(&sm->tags,gas_id);
          abort_tx_received[gas_id] = FALSE;
        }

        remove_handler = TRUE;
      }
   }

   return (remove_handler);

/*lint -esym(715,timestamp)*/
}



/*===========================================================================

FUNCTION sdcch_rx_handler

DESCRIPTION
  This function should be called every frame on the 26 frame multiframe. The
  handler is installed as part of a request to rx a msg by gl1_msg_rx_sdcch().
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
static boolean sdcch_rx_handler
(
   boolean start_of_block,       /* next frame is start of block */
   uint32    timestamp,           /* currently not used           */
   gas_id_t gas_id
)
{
   boolean remove_handler = FALSE;
   boolean decode_flag    = FALSE;
   boolean receive_burst  = FALSE;
   boolean metrics_flag   = FALSE;
   boolean get_data        = FALSE;
   sdcch_rx_state_machine_type *sm;
   uint8 sm_num, cnt;
#ifdef FEATURE_GSM_RX_DIVERSITY	
   boolean is_block_start = FALSE;
#endif

   /* Sanity check */
   if (sdcch_rx_params[gas_id].rx_sdcch_cnt == 0)
   {
      MSG_GERAN_ERROR_0_G("No active SDCCH state machines");
      return TRUE;
   }

   /* Go through the state machine fifo */
   sm_num = sdcch_rx_params[gas_id].rx_sdcch_rd_ptr;
   for (cnt = 0; cnt < sdcch_rx_params[gas_id].rx_sdcch_cnt; cnt++)
   {
      sm = &sdcch_rx_params[gas_id].sm[sm_num];
      decode_flag    = FALSE;
      receive_burst  = FALSE;
      metrics_flag   = FALSE;
      get_data        = FALSE;

      /* Kick the state machine */
      switch (sm->state)
      {
         case SDCCH_INACTIVE:
            MSG_GERAN_ERROR_0_G("SDCCH inactive");
            break;

         case SDCCH_WAITING_FOR_BLOCK_START:
            if (!start_of_block) break;

            sm->burst_num   = 0;
            sm->state++;
            receive_burst = TRUE;
            metrics_flag  = FALSE;
            break;

         case SDCCH_RECEIVE_1:
            receive_burst = TRUE;
            metrics_flag  = FALSE;
            sm->state++;
            break;

         case SDCCH_RECEIVE_2:
            receive_burst = TRUE;
            metrics_flag  = TRUE;
            sm->metrics_num = 0;
            sm->state++;
            break;

         case SDCCH_RECEIVE_3:
            receive_burst = TRUE;
            decode_flag   = TRUE;
            metrics_flag  = TRUE;
            sm->state++;
            break;

         case SDCCH_RECEIVE_4:
         case SDCCH_RECEIVE_5:
            receive_burst = FALSE;
            metrics_flag  = TRUE;
            sm->state++;
            break;

         case SDCCH_RECEIVE_6:
            receive_burst = FALSE;
            metrics_flag  = FALSE;
            L1_STATE_CHANGE(sm->state     = SDCCH_INACTIVE, "SDCCH_INACTIVE");
            get_data      = TRUE;
            break;

         default:
            receive_burst = FALSE;
            metrics_flag  = FALSE;
            MSG_GERAN_ERROR_1_G("Invalid SDCCH state: %d",sm->state);
            L1_STATE_CHANGE(sm->state = SDCCH_INACTIVE, "SDCCH_INACTIVE");
            break;
      }

      if (get_data)
      {
         /* Now the data is available, go get it */
         gl1_hw_get_sync_rx_data(GL1_HW_SDCCH,
                                 &sm->tags[DECODE_TAG],
                                 &sm->rpt.data->cch.hdr,
                                 sm->rpt.data->cch.data,
                                 (uint16)MAX_SDCCH_OCTETS,
                                 gas_id);


         sm->rpt.data->cch.offset_to_sob = 5;
         sm->rpt.data->cch.hdr.chan = (uint16)GL1_MSG_SDCCH;

         if (sdcch_rx_params[gas_id].rx_sdcch_callback.data_cb)
         {
             sdcch_rx_params[gas_id].rx_sdcch_callback.data_cb( sm->rpt.data,gas_id);
         }
         else
         {
             MSG_GERAN_HIGH_0_G("No sdcch dl callback registered");
         }

         /* If there is a metrics callback then call it with the metrics */
         /* data for the four bursts of the block                        */
         if (sdcch_rx_params[gas_id].rx_sdcch_callback.metrics_cb)
         {
            sdcch_rx_params[gas_id].rx_sdcch_callback.metrics_cb( sm->rpt.metrics,gas_id );
         }
      }

      /* Check if this is a frame where metrics and log info are available */
      if (metrics_flag)
      {
         sdcch_process_metrics(sm, timestamp,gas_id);
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
          sm->signal_info.valid              = TRUE;
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
      sm_num = (sm_num + 1) % NUM_SDCCH_SM;
   }

   /* Clear out inactive state machines */
   while (sdcch_rx_params[gas_id].sm[sdcch_rx_params[gas_id].rx_sdcch_rd_ptr].state == SDCCH_INACTIVE)
   {
      sdcch_rx_params[gas_id].rx_sdcch_rd_ptr =
        (sdcch_rx_params[gas_id].rx_sdcch_rd_ptr + 1) % NUM_SDCCH_SM;
      if (--sdcch_rx_params[gas_id].rx_sdcch_cnt == 0) break;
   }
   /* Are there any more messages to receive */
   if (sdcch_rx_params[gas_id].rx_sdcch_cnt == 0)
   {
      remove_handler = TRUE;
   }

   return (remove_handler);

/*lint -esym(715,timestamp)*/
}



