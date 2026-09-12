/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    G L 1  M E S S A G E  L A Y E R -  A S Y N C  C C C H  C H A N N E L S

GENERAL DESCRIPTION
   This module contains generic procedures used for decoding asynchronous
   CCCH channels.
   The procedures used herein are intended for internal use by the non-
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

   gl1_msgi_cfg_async_ccch
   gl1_msgi_rx_async_ccch


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_async_ccch.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
18/01/16   zf       CR961805: Set first burst flag in x2g_startup cmd
01/02/18   sk       CR1101677 Issue X2G Cleanup if required when aborting NSCH/NBCCH inside QBTA
03/01/16   mn       CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
19/10/15   br       CR926522 GL1 to Pass the frequency offset to rotator to correct in GBTA.
17/09/15   zf       CR909928: Do not do SBD for NBCCH
24/08/15   mc       CR874662 : CXM Freq ID support for X2G startup and X2G preload
20/08/15   am       CR890659 Ensure NAS ID to TCXO ID conversion is correct
12/06/15   js       CR850684 Change QBTA offset for X2G startup
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
16/04/15   smd      CR822202 Dual space data for IRAT and QTA
09/01/15   sp       CR778244 Correct usage of SBD/EPD flags for Async CCCH activities.
07/11/14   am       CR751764 Enable early burst decode for CBCH
11/02/15   js       CR775188 GL1 changes to receive EPD preference 
21/01/15   am       CR782577 Discard agc update when ccch rx burst is yielded
07/01/15   ak       CR771936: GSM Ncell metrics does not reflect correct ARFCN and band
03/06/13   am       CR493707 removing CR322519
27/11/14   sp       CR737103: Correct async ccch burst metrics Logging
28/11/14   am       CR760135 Enable SBD and EPD for page decode in transfer mode
14/11/14   am       CR458277 Always initialize state variables for async ccch handler
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
12/09/14   nm       CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
10/07/14   js       CR593074- ROTA/QBTA Feature implementation
20/12/11   spw      CR322519 Added in additional states for gl1_msgi_abort_rx_async_ccch
27/04/14   nh       CR655501- L1 get stuck trying to abort common control channel handler
29/01/14   mko      CR605061:Set decode_flag based on feature Macro(FEATURE_DISABLE_EARLY_DECODE).
21/01/14   cs       CR601912 Revert asynch_ccch abort changes from Apollo
10/10/13   nm       CR556131 a delayed abort for 1 more frame is required, if we are late in the frame..
21/08/13   npt      CR531633 - Remove FEATURE_DIME_MODEM dependencies
11/03/13   am       CR334207 Remove check for async_ccch_sm.state as ASYNC_CCCH_INACTIVE
                    from function gl1_msgi_abort_rx_async_ccch
11/03/13   am       CR322519 Merged from Apollo PL 
                    Added in additional states for gl1_msgi_abort_rx_async_ccch
08/06/12   pa       GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
07/06/10   cja      Fix Klockwork error
17/03/10   pg       CR232566 Avoid processing further burst metrics once the metrics
                    have been sent (avoids unnecessary Error message)
01/02/10   ky       Extracting Ncell SCH Burst metrics should happen from gfwSchRxBurstMetricsBuffer
                    instead of gfwAsyncMetricBuffers for Q6.
20/05/08   cs       Fix Klocwork array bounds overflow error
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
06/07/07   agv      CR123023 - Update fix for AFC/AGC corruption.
27/06/07   agv      CR123023 Fix for AGC corruption, and TT.
23/05/07   agv      CR118309 - Large RSSI variation initial fix.
03/29/06   gfr      Add frequency offset to ncell functions to support non
                    spec-compliant networks in WCDMA mode.
09/22/05   gfr      Removed FEATURE_SBI_MDSP_CONTROLLER
08/31/05   gfr      Replace gl1_msg_metrics_rpt with gl1_defs_metrics_rpt to
                    remove need for double copy
08/05/05   gfr      Simplify post-abort cleanup
07/29/05   gfr      DTM support
06/21/05   gfr      Remove calls to tt_suspend and tt_resume
04/20/05   yh       Remove use of RF fifo
04/12/05   gfr      Consolidate GSM hw layer parameters
03/09/05   yh       BEP set to 0 for async_rx.
11/01/05   gfr      Add message when starting receive.
12/06/04   gfr      Featurize the mDSP SBI controller
09/23/04   gfr      Suspend time tracking during activity.
08/12/04   gfr      Add frame number to abort message.
07/22/04   gfr      Removed QS_PER_FRAME definition.
07/16/04   gfr      Initial version.
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
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "msg.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "gpl1_dual_sim.h"
#include <stringl/stringl.h>
#include "mcfg_nv_api.h"

/*****************************************************

 LOCAL STORAGE

******************************************************/
  extern const uint8 null_page_data_0[MAX_CCCH_OCTETS];
  extern const uint8 null_page_data_1[MAX_CCCH_OCTETS];

/* States */
typedef enum
{
   ASYNC_CCCH_INACTIVE,
   ASYNC_CCCH_WAITING_FOR_BLOCK_START,
   ASYNC_CCCH_START,
   ASYNC_CCCH_RECEIVE_1,
   ASYNC_CCCH_RECEIVE_2,
   ASYNC_CCCH_RECEIVE_3,
   ASYNC_CCCH_RECEIVE_4,
   ASYNC_CCCH_WAITING,
   ASYNC_CCCH_GET_DATA,
   ASYNC_CCCH_ABORT_WAIT,
   ASYNC_CCCH_ABORT
} async_ccch_state_type;


/* State of decoded data for each decode issued */
typedef enum                      /*lint -esym(749,ASYNC_CCCH_RESULTS_WAITING)*/
{
  ASYNC_CCCH_RESULTS_DONE,
  ASYNC_CCCH_RESULTS_LAST_TRY,
  ASYNC_CCCH_RESULTS_FIRST_TRY,
  ASYNC_CCCH_RESULTS_WAITING,
  ASYNC_CCCH_RESULTS_CMD_ISSUED
} async_ccch_pending_results_type;

/* The above enum describes when data may be checked, it is used as follows:

   |   |   |   |   |   |   |   |
   |___|_R_|_R_|_R_|_R_|___|___|
    r   d   d   d
                1   2   3   4

   r - receive command issued
   d - rx command with decode issued
   R - burst received somewhere in the frame

   1 - At this point data may be available if the ASYNC_CCCH is early enough in the
       frame and the decode succeeded after two frames.
   2 - At this point the data may be available either from the first two
       frames if the ASYNC_CCCH is very late in the frame or from the first three
       decodes if the ASYNC_CCCH is early in the frame.  This means there are two
       decoded results buffers to check
   3 - Same as two but for another frame
   4 - This is the regular place to pick up the data and it should be available
       regardless of early decode or not and wherever in the frame the burst
       lies.
*/

/* For early decode we try three times to decode the data */
#define MAX_DECODES 3


static int32 bcch_rot_offset[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 0 ) };
/* Structure containing all parameters for the ASYNC_CCCH */
typedef struct
{
   /* State variables */
   async_ccch_state_type state;
   uint8                 receives_issued;
   uint8                 decodes_issued;
   uint8                 metrics_retrieved;
   uint8                 data_retrieved;
   uint8                 offset_to_sob;
   boolean               sent_metrics;
   boolean               delay_abort;

   /* Pending data and metrics results */
   async_ccch_pending_results_type decode_results[MAX_DECODES];
   async_ccch_pending_results_type metrics_results[GL1_DEFS_FRAMES_IN_BLK_PERIOD];

   /* Configured parameters */
   gl1_msg_chan_type               channel_type;
   gl1_msgi_async_ccch_params_type params;

   /* Frame layer parameters */
   gl1_hw_rx_signal_type           signal_info;
   gl1_hw_rx_burst_type            burst_info;

} async_ccch_state_machine_type;


/* Storage for the ASYNC_CCCH parameters */
static async_ccch_state_machine_type async_ccch_sm[NUM_GERAN_DATA_SPACES];


/* Maximum offset at which to start the state machine, this guarantees the timeline
   is as expected, otherwise an offset too large may cause results to not be available
   for an extra frame */
#define MAX_ASYNC_CCCH_OFFSET  6250
#define MIN_ASYNC_CCCH_OFFSET_FOR_DELAYED_ABORT  3500

#if defined FEATURE_QBTA
#define NBCCH_CLEANUP_OFFSET 1425
#endif

/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/
static boolean async_ccch_rx_handler( boolean start_of_block,
                                      uint32  time_stamp,gas_id_t gas_id );



/*===========================================================================

FUNCTION gl1_msgi_init_async_ccch

DESCRIPTION
  This function is used to initialize the async ccch data structures.

===========================================================================*/
void gl1_msgi_init_async_ccch (gas_id_t gas_id)
{
   L1_STATE_CHANGE(async_ccch_sm[gas_id].state = ASYNC_CCCH_INACTIVE, "CCCH_INACTIVE");

   async_ccch_sm[gas_id].receives_issued    = 0;
   async_ccch_sm[gas_id].metrics_retrieved  = 0;
   async_ccch_sm[gas_id].data_retrieved     = 0;
   async_ccch_sm[gas_id].decodes_issued     = 0;
   async_ccch_sm[gas_id].delay_abort        = FALSE;

   async_ccch_sm[gas_id].params.callbacks.data_cb    = NULL;
   async_ccch_sm[gas_id].params.callbacks.metrics_cb = NULL;
   async_ccch_sm[gas_id].params.callbacks.log_cb     = NULL;
   async_ccch_sm[gas_id].params.callbacks.abort_cb   = NULL;

   memset(&async_ccch_sm[gas_id].signal_info, 0, sizeof(async_ccch_sm[gas_id].signal_info));
   memset(&async_ccch_sm[gas_id].burst_info, 0, sizeof(async_ccch_sm[gas_id].burst_info));
}

void gl1_msg_update_bcch_rotator_value(int32 freq_error, gas_id_t  gas_id)
{
   bcch_rot_offset[gas_id] = gl1_hw_hztophase( freq_error );
   MSG_GERAN_MED_3_G( GL1_ACQ_HDR"NCell BCCH delta freq_error %dHz bcch_rot_offset %d curr %dXO",
            freq_error, bcch_rot_offset[gas_id], gl1_hw_get_xo_acc_freq_err(gas_id) );
}

/*===========================================================================

FUNCTION gl1_msgi_rx_async_ccch

DESCRIPTION
  This function is used to receive a message on an async ccch
  block boundary. This function may be called at task level but should
  be called in the block before the data is to transmitted.

===========================================================================*/
void gl1_msgi_rx_async_ccch
(
   gl1_msg_chan_type               channel_type,
   gl1_msgi_async_ccch_params_type *params,
   gas_id_t                         gas_id
)
{
   /* Make sure we are not already active */
   if (async_ccch_sm[gas_id].state == ASYNC_CCCH_WAITING_FOR_BLOCK_START)
   {
      MSG_GERAN_ERROR_3_G("Double async CCCH (%d) rx issued (arfcns=%d,%d)",channel_type,async_ccch_sm[gas_id].params.ARFCN[0].num,params->ARFCN[0].num);

      /* Remove handler to avoid spurious errors, will be reinstalled below */
      gl1_msgi_remove_ft_handler(async_ccch_rx_handler,gas_id);
   }
   else if (async_ccch_sm[gas_id].state != ASYNC_CCCH_INACTIVE)
   {
      MSG_GERAN_ERROR_3_G("Async CCCH (%d) collision (state=%d, arfcn=%d)",channel_type,async_ccch_sm[gas_id].state,params->ARFCN[0].num);

      /* At this point L1 is probably stuck, but to continue with this
         sch could kill the lower layers even worse.  To try and recover
         reinstall the handler in case somehow it got uninstalled and hope
         for the best. */
      gl1_msgi_add_ft_handler(async_ccch_rx_handler, GL1_MSG_FT_HDLR_ACQ_NCELL,gas_id);
      return;
   }

   /* Store parameters */
   async_ccch_sm[gas_id].channel_type = channel_type;
   async_ccch_sm[gas_id].params = *params;

   /* Stamp the ARFCN into the AGC structure for debugging */
   async_ccch_sm[gas_id].params.agc_ptr->arfcn = async_ccch_sm[gas_id].params.ARFCN[0];

   /* Init static frame layer parameters */
   async_ccch_sm[gas_id].burst_info.channel_type   = params->hw_channel_type;
   async_ccch_sm[gas_id].burst_info.tsc            = params->TSC;
   async_ccch_sm[gas_id].burst_info.early_decoding = params->DRX;
   async_ccch_sm[gas_id].burst_info.search_width   = GL1_HW_ASYNC_SEARCH_WIDTH_DEFAULT;

   /* Add the handler and set the initial state */
   gl1_msgi_add_ft_handler(async_ccch_rx_handler, GL1_MSG_FT_HDLR_ACQ_NCELL,gas_id);

   L1_STATE_CHANGE(async_ccch_sm[gas_id].state = ASYNC_CCCH_WAITING_FOR_BLOCK_START, "CCCH_WAITING_FOR_BLOCK_START");
}



/*===========================================================================

FUNCTION gl1_msgi_abort_rx_async_ccch

DESCRIPTION
  This function is used to abort an async CCCH.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_abort_rx_async_ccch (gl1_msg_chan_type channel_type,gas_id_t gas_id)
{
   
   if (async_ccch_sm[gas_id].state == ASYNC_CCCH_INACTIVE)
   {
      MSG_GERAN_HIGH_2_G("Aborting channel type %d with CCCH Inactive for gas_id %d",channel_type,gas_id);
   }

   /* Check the channel type matches */
   if (channel_type != async_ccch_sm[gas_id].channel_type)
   {
      MSG_GERAN_ERROR_3_G("Aborting wrong async ccch channel type (%d, %d), state=%d", channel_type, async_ccch_sm[gas_id].channel_type, async_ccch_sm[gas_id].state);
      return;
   }

   MSG_GERAN_MED_3_G("Async CCCH (%d) aborted: state=%d, FN=%d", channel_type,async_ccch_sm[gas_id].state,GSTMR_GET_FN_GERAN(gas_id));

   /* Clean up drivers */
   gl1_hw_cleanup_async_rx_geran(gas_id);

   /* Check our state to see if we can abort immediately */
   switch (async_ccch_sm[gas_id].state)
   {
      /* We can abort immmediately */
      case ASYNC_CCCH_INACTIVE:
      case ASYNC_CCCH_WAITING_FOR_BLOCK_START:
      case ASYNC_CCCH_START:
      case ASYNC_CCCH_GET_DATA:
      {
         L1_STATE_CHANGE(async_ccch_sm[gas_id].state = ASYNC_CCCH_INACTIVE, "CCCH_INACTIVE");

         gl1_msgi_remove_ft_handler(async_ccch_rx_handler,gas_id);
#if defined (FEATURE_QBTA) 
         if( (gl1_check_qbta_allowed(gas_id)) && (gl1_hw_get_qta_cm_fw_rf_init_status(gas_id) ) )
         { 
            uint32 offset = async_ccch_sm[gas_id].params.timing_offset + NBCCH_CLEANUP_OFFSET;
            gl1_hw_qta_fw_rf_deinit(gas_id,offset,TRUE);
         }
#endif

         if (async_ccch_sm[gas_id].params.callbacks.abort_cb)
         {
            async_ccch_sm[gas_id].params.callbacks.abort_cb(gas_id);
         }
         else
         {
            MSG_GERAN_HIGH_0_G("No Async CCCH abort CB registered");
         }
         break;
      }

      /* We are already aborting */
      case ASYNC_CCCH_ABORT_WAIT:
      case ASYNC_CCCH_ABORT:
      {
         break;
      }

      /* Some extra wait time is required */
      default:
      {
         L1_STATE_CHANGE(async_ccch_sm[gas_id].state = ASYNC_CCCH_ABORT_WAIT, "ASYNC_CCCH_ABORT_WAIT");
         break;
      }
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

FUNCTION async_ccch_process_metrics

DESCRIPTION
  Processes any pending metrics.  Returns whether or not all the metrics
  have been picked up.

===========================================================================*/
static boolean async_ccch_process_metrics (uint32 time_stamp,gas_id_t gas_id)
{
   boolean               last_chance;
   gl1_defs_metrics_rpt  metrics, *metrics_ptr;
   boolean               valid;
   int i;
#ifdef GERAN_L1_HLLL_LNASTATE
   gl1_defs_metrics_lnastate gl1_metrics_lnastate;
#endif

   /* Sanity check on array bounds */
   if ( ( async_ccch_sm[gas_id].metrics_retrieved > GL1_DEFS_FRAMES_IN_BLK_PERIOD ) ||
        ( async_ccch_sm[gas_id].receives_issued > GL1_DEFS_FRAMES_IN_BLK_PERIOD ) ||
        ( async_ccch_sm[gas_id].metrics_retrieved > async_ccch_sm[gas_id].receives_issued ) )
   {
     MSG_GERAN_ERROR_2_G( "Bad metrics counts retrieved %d issued %d",
                async_ccch_sm[gas_id].metrics_retrieved, async_ccch_sm[gas_id].receives_issued);

     return ( FALSE );
   }

   /* Increment the offset-to-start-of-block count for any received metrics */
   for (i = 0; i < async_ccch_sm[gas_id].metrics_retrieved; i++)
   {
      if (async_ccch_sm[gas_id].params.rpt.metrics[i] != NULL)
      {
         async_ccch_sm[gas_id].params.rpt.metrics[i]->offset_to_sob++;
      }
   }

   /* Klocwork fixes */
   if ( async_ccch_sm[gas_id].metrics_retrieved == GL1_DEFS_FRAMES_IN_BLK_PERIOD )
   {
      MSG_GERAN_ERROR_2_G( "Clamping metrics retrieved count %d to %d",
                 async_ccch_sm[gas_id].metrics_retrieved, ( GL1_DEFS_FRAMES_IN_BLK_PERIOD - 1 ));

      async_ccch_sm[gas_id].metrics_retrieved = ( GL1_DEFS_FRAMES_IN_BLK_PERIOD - 1 );
   }

   /* Go through the outstanding metrics. */
   for (i = async_ccch_sm[gas_id].metrics_retrieved; i < async_ccch_sm[gas_id].receives_issued; i++)
   {
      /* Once we find data not ready for retrieval we can skip the rest */
      async_ccch_sm[gas_id].metrics_results[i]--;

      if (async_ccch_sm[gas_id].metrics_results[i] > ASYNC_CCCH_RESULTS_FIRST_TRY)
      {
        break;
      }

      /* Check if this is the last try for these metrics */
      last_chance = (async_ccch_sm[gas_id].metrics_results[i] == ASYNC_CCCH_RESULTS_LAST_TRY);

      /* Get a pointer to the metrics storage area for these results */
      metrics_ptr = async_ccch_sm[gas_id].params.rpt.metrics[async_ccch_sm[gas_id].metrics_retrieved];
      if (metrics_ptr == NULL)
      {
         metrics_ptr = &metrics;
      }

      metrics_ptr->arfcn         =
                   async_ccch_sm[gas_id].params.ARFCN[async_ccch_sm[gas_id].metrics_retrieved];
	  
	  /* Try and get the metrics */
      /* sch flag is false here because we are doing here ccch decoding*/
      valid = gl1_hw_get_async_rx_metrics_geran(metrics_ptr, 
                                          last_chance,
                                          &async_ccch_sm[gas_id].params.tags[async_ccch_sm[gas_id].metrics_retrieved],
                                          FALSE,
                                          geran_map_gas_id_to_nas_id( gas_id));

      if (valid)
      {
         /* AGC */
         boolean meas_valid = (metrics_ptr->powerStatus == RF_IN_RANGE ? TRUE : FALSE);

         /*if reported power is too low or yield*/
         metrics_ptr->valid = ( metrics_ptr->pwr_dBm_x16 > RFGSM_MIN_RX_POWER_REPORTED );

         /* No slew for Idle. */
         async_ccch_sm[gas_id].params.agc_ptr->srl_enabled = FALSE;

         if( metrics_ptr->valid )
         {
           sys_algo_agc_update
            (
              async_ccch_sm[gas_id].params.agc_ptr,
              metrics_ptr->pwr_dBm_x16,
              meas_valid,
              time_stamp,
              gas_id 
            );
         }

           
           metrics_ptr->offset_to_sob = (last_chance ? 2 : 1);
      }
      else if (last_chance)
      {
         /* Fill in some values */
         metrics_ptr->valid         = FALSE;
         metrics_ptr->offset_to_sob = 2;

         /* This should never happen - something has gone wrong */
         MSG_GERAN_ERROR_2_G("Async CCCH (%d) metrics unavailable, FN=%d",async_ccch_sm[gas_id].channel_type,GSTMR_GET_FN_GERAN(gas_id));
      }

      /* Check if we are done with these metrics */
      if (valid || last_chance)
      {
#ifdef GERAN_L1_HLLL_LNASTATE
         if( metrics_ptr->valid && gl1_hw_get_sawless_HLLL_support(metrics_ptr->arfcn.band,gas_id) )
         {
            gl1_metrics_lnastate.NoOfChannels              = 1;
            gl1_metrics_lnastate.gsm_metrics[0].arfcn      = metrics_ptr->arfcn;
            gl1_metrics_lnastate.gsm_metrics[0].rx_lvl_dbm = metrics_ptr->pwr_dBm_x16;
            gl1_metrics_lnastate.gsm_metrics[0].snr        = metrics_ptr->snr;
            gl1_metrics_lnastate.gsm_metrics[0].jdet_value = metrics_ptr->jdet_value;
            gl1_hw_rf_burst_metrics(&gl1_metrics_lnastate,gas_id);
         }
#endif /*GERAN_L1_HLLL_LNASTATE*/

         async_ccch_sm[gas_id].metrics_results[i] = ASYNC_CCCH_RESULTS_DONE;
         async_ccch_sm[gas_id].metrics_retrieved++;
      }

      /* Klocwork fixes */
      if ( async_ccch_sm[gas_id].metrics_retrieved == GL1_DEFS_FRAMES_IN_BLK_PERIOD )
      {
        break;
      }
   }

   return (async_ccch_sm[gas_id].metrics_retrieved == GL1_DEFS_FRAMES_IN_BLK_PERIOD ? TRUE : FALSE);
}



/*===========================================================================

FUNCTION async_ccch_process_data

DESCRIPTION
  Checks for decoded data.  Returns if the data is done (either decoded
  results found, or it was the last try).

===========================================================================*/
static boolean async_ccch_process_data (boolean last_data_attempt,gas_id_t gas_id)
{
   boolean data_ready = FALSE;
   boolean valid;
   boolean last_chance;
   int i, j;

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT)) 
   MSG_GERAN_HIGH_0_G("GPLT DBG: async_ccch_process_data");
#else
   /* Go through the outstanding data requests. */
   for (i = async_ccch_sm[gas_id].data_retrieved; i < async_ccch_sm[gas_id].decodes_issued; i++)
   {
      /* Decrement the state and check if data is ready for retrieval */
      async_ccch_sm[gas_id].decode_results[i]--;
      if (async_ccch_sm[gas_id].decode_results[i] > ASYNC_CCCH_RESULTS_FIRST_TRY) break;

      /* If we get to 0 it is the last try for this data */
      last_chance = (async_ccch_sm[gas_id].decode_results[i] == ASYNC_CCCH_RESULTS_LAST_TRY);

      /* Try and get the decoded data */
      valid = gl1_hw_get_async_rx_data_geran
       (
         GL1_HW_PCH,
         &async_ccch_sm[gas_id].params.rpt.data->ccch.hdr,
         async_ccch_sm[gas_id].params.rpt.data->ccch.data,
         (uint16)MAX_CCCH_OCTETS,
         last_chance,
         geran_map_gas_id_to_nas_id(gas_id)
       );

      /* If the data is valid we are done with this decode attempt */
      if (valid || last_chance)
      {
         async_ccch_sm[gas_id].decode_results[i] = ASYNC_CCCH_RESULTS_DONE;
         async_ccch_sm[gas_id].data_retrieved++;
      }

      /* If the CRC passed then we are done with all decodes */
      if (valid && async_ccch_sm[gas_id].params.rpt.data->ccch.hdr.good_data)
      {
         data_ready = TRUE;
         break;
      }
   }
#endif /*(!defined(FEATURE_GL1_GPLT))*/

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT)) 
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: async_ccch_process_data: check only for last_data_attempt");
   if (last_data_attempt)
#else
   if (data_ready || last_data_attempt)
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT)) */
   {
      async_ccch_sm[gas_id].params.rpt.data->ccch.tag               = 1;
      async_ccch_sm[gas_id].params.rpt.data->ccch.arfcn             = async_ccch_sm[gas_id].params.ARFCN[0];
      async_ccch_sm[gas_id].params.rpt.data->ccch.own_paging_block  = FALSE;
      async_ccch_sm[gas_id].params.rpt.data->ccch.hdr.chan          = (int)async_ccch_sm[gas_id].channel_type;
      async_ccch_sm[gas_id].params.rpt.data->ccch.offset_to_sob     = async_ccch_sm[gas_id].offset_to_sob;

      for(j = 0; j < GL1_DEFS_FRAMES_IN_BLK_PERIOD ; j++)
      {
         async_ccch_sm[gas_id].params.rpt.data->ccch.hdr.bep[j] = 0;
      }

      if (async_ccch_sm[gas_id].params.callbacks.data_cb)
      {
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
        if (  gl1_msg_is_sbd_enabled( gas_id ) &&
              (async_ccch_sm[gas_id].channel_type == GL1_MSG_CCCH) &&
              ((async_ccch_sm[gas_id].receives_issued == 2 ) ||   // First burst decode.
              ((async_ccch_sm[gas_id].receives_issued == 3) && (async_ccch_sm[gas_id].params.rpt.metrics[0]->valid == FALSE))) // First burst YIELD, 2nd burst decode
           )
        {
           uint8 *null_page_data;
           uint16 pattern_detect = gl1_hw_sbd_get_null_pattern(gas_id);
           if(pattern_detect == 0)
           {
               null_page_data = (uint8 *)null_page_data_0;
           }
           else
           {
               null_page_data = (uint8 *)null_page_data_1;
           }
           MSG_GERAN_MED_1_G("PCHXFER SBD:replace burst data with a NULL Page pattern .receives_issued %d",async_ccch_sm[gas_id].receives_issued);
           memscpy(async_ccch_sm[gas_id].params.rpt.data->ccch.data,sizeof(async_ccch_sm[gas_id].params.rpt.data->ccch.data),null_page_data,MAX_CCCH_OCTETS);
        }
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
         async_ccch_sm[gas_id].params.callbacks.data_cb(async_ccch_sm[gas_id].params.rpt.data,gas_id);
      }
      else
      {
         MSG_GERAN_HIGH_1_G("No dl async ccch (%d) callback registered",async_ccch_sm[gas_id].channel_type);
      }

      /* No more receives */
      L1_STATE_CHANGE(async_ccch_sm[gas_id].state = ASYNC_CCCH_INACTIVE, "CCCH_INACTIVE");

      /* Clean up drivers */
      gl1_hw_cleanup_async_rx_geran(gas_id);
   }

   return ((data_ready || last_data_attempt) ? TRUE : FALSE);
}



/*===========================================================================

FUNCTION async_ccch_rx_handler

DESCRIPTION
  This function should be called every frame on the 51 frame multiframe. The
  handler is installed as part of a request to rx a msg by gl1_msg_rx_async_ccch().
  The handler is told when the start of a block is to occur and will call
  the frame layer functions at the proper time.   When the handler senses
  that there are no more messages queued up to send, it will signal that
  it should be deregistered.

===========================================================================*/
static boolean async_ccch_rx_handler
(
   boolean start_of_block,
   uint32  time_stamp,
   gas_id_t gas_id
)
{
   int i;

   boolean receive_flag      = FALSE;
   boolean sbd_enabled       = FALSE;
   boolean epd_allowed       = FALSE;
   boolean log_flag          = FALSE;
   boolean decode_flag       = FALSE;
   boolean last_data_attempt = FALSE;
   boolean metrics_done;
   boolean data_done;
   mdsp_burst_mode mdsp_burst_type = RX_MODE;
   
   /* Tick the state machine */
   switch (async_ccch_sm[gas_id].state)
   {
      case ASYNC_CCCH_INACTIVE:
         MSG_GERAN_ERROR_0_G("Async CCCH inactive");
         return TRUE;

      case ASYNC_CCCH_WAITING_FOR_BLOCK_START:
         if (!start_of_block) return FALSE;

         MSG_GERAN_HIGH_5_G("Async CCCH Start: isED_Dis=%d,epd_enabled=%d,sbd_enabled=%d,drx=%d, activity=%d",gl1_msg_is_early_decode_disabled(gas_id),
                             gl1_msg_is_epd_allowed(gas_id),
                             gl1_msg_is_sbd_enabled(gas_id),
                             async_ccch_sm[gas_id].params.DRX,
                             async_ccch_sm[gas_id].params.async_ccch_type);
         async_ccch_sm[gas_id].state++;
         /* fall through */

      case ASYNC_CCCH_START:
         /* Wait until the offset falls in the next frame to start
            the state machine */

          /* Init some state variables */
         async_ccch_sm[gas_id].receives_issued   = 0;
         async_ccch_sm[gas_id].decodes_issued    = 0;
         async_ccch_sm[gas_id].metrics_retrieved = 0;
         async_ccch_sm[gas_id].data_retrieved    = 0;

         if (async_ccch_sm[gas_id].params.timing_offset > MAX_ASYNC_CCCH_OFFSET)
         {
            MSG_GERAN_MED_3_G("Async CCCH (%d) offset %d too large, delaying a frame, FN=%d", async_ccch_sm[gas_id].channel_type,async_ccch_sm[gas_id].params.timing_offset, GSTMR_GET_FN_GERAN(gas_id));
            async_ccch_sm[gas_id].params.timing_offset -= QS_PER_FRAME;
            break;
         }

         async_ccch_sm[gas_id].sent_metrics      = FALSE;
         async_ccch_sm[gas_id].delay_abort        = FALSE;

         /* Receive the first burst */
         receive_flag  = TRUE;

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
         if ( !gl1_msg_is_early_decode_disabled( gas_id )
              && (async_ccch_sm[gas_id].params.async_ccch_type == ASYNC_ACTIVITY_GBTA)
            )
         {
           decode_flag = async_ccch_sm[gas_id].params.DRX;
         }

         /* SBD and EPD is only applicable to PCH. */
         if (async_ccch_sm[gas_id].channel_type == GL1_MSG_CCCH)
         {
           if ( gl1_msg_is_sbd_enabled(gas_id) )
           {
             sbd_enabled = decode_flag;
           }
           if ( gl1_msg_is_epd_allowed(gas_id) && ! mcfg_gcf_nv_get_status())
           {
             epd_allowed = sbd_enabled;
           }
         }
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

         MSG_GERAN_MED_3_G("Async CCCH (arfcn=%d): offset=%d, FN=%d", async_ccch_sm[gas_id].params.ARFCN[0].num, async_ccch_sm[gas_id].params.timing_offset, GSTMR_GET_FN_GERAN(gas_id));

         async_ccch_sm[gas_id].state++;

          /*
          * Always clear on setup to avoid any hangover if sleep
          * never happens
          */
         gl1_msg_set_mdsp_asynch_int( FALSE, gas_id );
         break;

      case ASYNC_CCCH_RECEIVE_1:
         /* first burst is being received no SBD in ASYNC CCCH */
         receive_flag  = TRUE;

         if ( !gl1_msg_is_early_decode_disabled( gas_id ) )
         {
           decode_flag   = async_ccch_sm[gas_id].params.DRX;
         }

         async_ccch_sm[gas_id].offset_to_sob = 0;
         async_ccch_sm[gas_id].state++;
         break;

      case ASYNC_CCCH_RECEIVE_2:
         /* second burst is being received */
         receive_flag  = TRUE;
         log_flag      = TRUE;

         if ( !gl1_msg_is_early_decode_disabled( gas_id ) )
         {
           decode_flag   = async_ccch_sm[gas_id].params.DRX;
         }

         async_ccch_sm[gas_id].offset_to_sob++;
         async_ccch_sm[gas_id].state++;
         if ( gl1_msg_is_asynch_sleep_enabled( gas_id ) 
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
              && 
              (async_ccch_sm[gas_id].params.async_ccch_type != ASYNC_ACTIVITY_GBTA)
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
            )
         {
           /*
           * Always set as Early Decode possible from now on
           */
           gl1_msg_set_mdsp_asynch_int( decode_flag, gas_id );
         }

         break;

      case ASYNC_CCCH_RECEIVE_3:
         /* third burst is being received */
         receive_flag  = TRUE;
         decode_flag   = TRUE;
         log_flag      = TRUE;
         async_ccch_sm[gas_id].offset_to_sob++;
         async_ccch_sm[gas_id].state++;

         if (  gl1_msg_is_asynch_sleep_enabled( gas_id ) 
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
         && 
             (async_ccch_sm[gas_id].params.async_ccch_type != ASYNC_ACTIVITY_GBTA)
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
            )
         {
           /*
            * Always set as Early Decode possible from now on
            */
           gl1_msg_set_mdsp_asynch_int( TRUE, gas_id );
         }
         break;

      case ASYNC_CCCH_RECEIVE_4:
         /* fourth burst is being received */
         log_flag      = TRUE;
         async_ccch_sm[gas_id].offset_to_sob++;
         async_ccch_sm[gas_id].state++;
         break;

      case ASYNC_CCCH_WAITING:
         /* wait */
         log_flag      = TRUE;
         async_ccch_sm[gas_id].offset_to_sob++;
         async_ccch_sm[gas_id].state++;
         break;

      case ASYNC_CCCH_GET_DATA:
         last_data_attempt = TRUE;
         async_ccch_sm[gas_id].offset_to_sob++;
         async_ccch_sm[gas_id].state++;

         if ( gl1_msg_is_asynch_sleep_enabled( gas_id ) )
         {
           /*
           * Always clear here as 4th burst RX completed
           * and any possible data indication from F/W missed
           */
           gl1_msg_set_mdsp_asynch_int( FALSE, gas_id );
         }

         break;

      /* Abort states */

      case ASYNC_CCCH_ABORT_WAIT:
	 if ( gl1_msg_is_asynch_sleep_enabled( gas_id ) )
         {
           /*
            * Always clear on abort to avoid any hangover if sleep
            * never happens
            */
           gl1_msg_set_mdsp_asynch_int( FALSE, gas_id );
         }
		 
         /*delay the abort by 1 frame frame if previous asyn cch rx had timing offset which is late in the frame*/
         if ((!async_ccch_sm[gas_id].delay_abort) && 
             (async_ccch_sm[gas_id].params.timing_offset > MIN_ASYNC_CCCH_OFFSET_FOR_DELAYED_ABORT))
         {
           /*dont change state here, we need to wait for 1 more frame in this state*/
           async_ccch_sm[gas_id].delay_abort = TRUE;
         }
         else
         {
           async_ccch_sm[gas_id].state++;
		   
           if(async_ccch_sm[gas_id].delay_abort)
           {
             async_ccch_sm[gas_id].delay_abort = FALSE;
           } 
         }
         /* Do no further processing */
         return FALSE;

      case ASYNC_CCCH_ABORT:
         /* Resume PSBI and time tracking */
         L1_STATE_CHANGE(async_ccch_sm[gas_id].state = ASYNC_CCCH_INACTIVE, "CCCH_INACTIVE");

         if (async_ccch_sm[gas_id].params.callbacks.abort_cb)
         {
            async_ccch_sm[gas_id].params.callbacks.abort_cb(gas_id);
         }
         else
         {
            MSG_GERAN_HIGH_1_G("No async CCCH (%d) abort CB registered",async_ccch_sm[gas_id].channel_type);
         }
         /* Do no further processing */
         return TRUE;

      default:
         MSG_GERAN_HIGH_2_G("Error in async rx (%d) state machine %d", async_ccch_sm[gas_id].channel_type,async_ccch_sm[gas_id].state);
         L1_STATE_CHANGE(async_ccch_sm[gas_id].state = ASYNC_CCCH_INACTIVE, "CCCH_INACTIVE");
         return TRUE;
   }

   /* Logging */
   if (log_flag)
   {
      /* NOTE: This will not work for all offsets. */
      if (async_ccch_sm[gas_id].params.callbacks.log_cb)
      {
         async_ccch_sm[gas_id].params.callbacks.log_cb();
      }
   }

   /* Process data */
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT)) 
   // In case of GPLT and no RF, return default/canned result
   if ((async_ccch_sm[gas_id].state - 1) == ASYNC_CCCH_GET_DATA)
   {
      data_done = async_ccch_process_data(last_data_attempt,gas_id);
   }
   else
   {
      data_done = FALSE;
   }
#else
   data_done = async_ccch_process_data(last_data_attempt,gas_id);
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT)) */

   /* Process metrics */
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT)) 
   MSG_GERAN_HIGH_0_G("GPLT DBG: Not calling async_ccch_process_metrics");
#else
   if(!async_ccch_sm[gas_id].sent_metrics)
   {
      metrics_done = async_ccch_process_metrics(time_stamp,gas_id);

      /* Send metrics if all done, or the data is ready */
      if (metrics_done || data_done)
      {
         if (async_ccch_sm[gas_id].params.callbacks.metrics_cb)
         {
            /* Set any unfilled metrics to not valid */
            for (i = async_ccch_sm[gas_id].metrics_retrieved; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
            {
               async_ccch_sm[gas_id].params.rpt.metrics[i]->valid = FALSE;
               async_ccch_sm[gas_id].params.rpt.metrics[i]->arfcn = async_ccch_sm[gas_id].params.ARFCN[i];
            }
            async_ccch_sm[gas_id].params.callbacks.metrics_cb(async_ccch_sm[gas_id].params.rpt.metrics,gas_id);
         }
         async_ccch_sm[gas_id].sent_metrics = TRUE;
      }
   }

   /* Now issue the rx if necessary */
   if (receive_flag && !data_done)
   {
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
      if (async_ccch_sm[gas_id].params.async_ccch_type == ASYNC_ACTIVITY_GBTA)
      {
        mdsp_burst_type = RX_MODE_ASYNC; // for using separate rx_pch_event buffer.
      }

#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

      /* Fill in dynamic frame layer parameters */
      async_ccch_sm[gas_id].signal_info.rx_signal_strength = async_ccch_sm[gas_id].params.agc_ptr->pwr_dBm_x16;
      async_ccch_sm[gas_id].burst_info.decode_flag         = decode_flag;
      async_ccch_sm[gas_id].burst_info.sbd_enabled         = sbd_enabled;
      async_ccch_sm[gas_id].burst_info.epd_allowed         = epd_allowed;
      async_ccch_sm[gas_id].burst_info.burst_num           = async_ccch_sm[gas_id].receives_issued;
      async_ccch_sm[gas_id].burst_info.arfcn               = async_ccch_sm[gas_id].params.ARFCN[async_ccch_sm[gas_id].receives_issued];

#if defined (FEATURE_QBTA) 
      if( (gl1_check_qbta_allowed(gas_id)) && (!gl1_hw_get_qta_cm_fw_rf_init_status(gas_id) ) )
      {
         uint32 offset = async_ccch_sm[gas_id].params.timing_offset ;
         gl1_hw_qta_fw_rf_init(FALSE,offset,TRUE,async_ccch_sm[gas_id].burst_info.arfcn,FALSE,FALSE,gas_id);
      }
#endif

      MSG_GERAN_HIGH_5_G( "async_ccch_rx_handler state=%d, sbd:%d epd:%d, decode=%d, ed=%d",
                                async_ccch_sm[gas_id].state,sbd_enabled, epd_allowed, decode_flag,async_ccch_sm[gas_id].burst_info.early_decoding);
      MSG_GERAN_HIGH_1_G( GL1_ACQ_HDR"Applying bcch_rot_offset %d prior to BCCH decode",
                   bcch_rot_offset[gas_id]);

      gl1_hw_rx_async_burst
       (
         &async_ccch_sm[gas_id].signal_info,
         &async_ccch_sm[gas_id].burst_info,
         async_ccch_sm[gas_id].params.timing_offset,
         bcch_rot_offset[gas_id],  /* No frequency offset in GSM mode */
         &async_ccch_sm[gas_id].params.tags[async_ccch_sm[gas_id].receives_issued],
         mdsp_burst_type,
         gas_id
       );

#if defined (FEATURE_QBTA) 
      if((gl1_check_qbta_allowed(gas_id)))
      { 
         uint32 offset = async_ccch_sm[gas_id].params.timing_offset + NBCCH_CLEANUP_OFFSET;
         gl1_hw_qta_fw_rf_deinit(gas_id,offset,TRUE);
      }
#endif
      /* Start the metrics counter */
      async_ccch_sm[gas_id].metrics_results[async_ccch_sm[gas_id].receives_issued++] = ASYNC_CCCH_RESULTS_CMD_ISSUED;

      /* If we issued a decode then start the counter for getting the data */
      if (decode_flag)
      {
         async_ccch_sm[gas_id].decode_results[async_ccch_sm[gas_id].decodes_issued++] = ASYNC_CCCH_RESULTS_CMD_ISSUED;
      }
   }
#endif /* (!defined(FEATURE_GL1_GPLT)) */

   /* If we are inactive, remove the handler */
   return (async_ccch_sm[gas_id].state == ASYNC_CCCH_INACTIVE ? TRUE : FALSE);
}

/*===========================================================================

FUNCTION gl1_msg_is_async_bcch

DESCRIPTION
  This function is called to check if async BCCH RX is happening

===========================================================================*/
boolean gl1_msg_is_async_bcch( gas_id_t gas_id )
{
  return  ( ( async_ccch_sm[gas_id].channel_type == GL1_MSG_BCCH )
             && ( async_ccch_sm[gas_id].state != ASYNC_CCCH_INACTIVE )
             && ( async_ccch_sm[gas_id].state != ASYNC_CCCH_ABORT )
          );
}

