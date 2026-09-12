/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     G L 1  M E S S A G E  L A Y E R -  N C E L L   S C H   C H A N N E L

GENERAL DESCRIPTION
   This module contains the procedures used for decoding the SCH from a
   neighbor cell.
   The procedures used herein are intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

   gl1_msg_cfg_ncell_sch
   gl1_msg_rx_ncell_sch
   ncell_sch_rx_handler





INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_ncell_sch.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
01/02/18   sk       CR1101677 Issue X2G Cleanup if required when aborting NSCH/NBCCH inside QBTA
15/12/16   sk       CR1101677 Do not call qta_fw_rf_deinit from sch handler if QBTA is not supported
16/07/15   js       CR872948/896782 Increasing cleanup offset for Ncell SCH in QTA
28/07/15   js       CR841536 Changes for NCELL SCH as QBTA activity.
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
09/03/15   dg       CR800014 During first attempt of NCELL SCH decode, ignore SCH data if metrics is not available
07/01/15   ak       CR771936: GSM Ncell metrics does not reflect correct ARFCN and band
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
07/08/14   cja      CR643648 Desense for channel adjacent to spur
19/12/13   jk       CR503359 - GL1 changes to support Rx diversity
21/08/13   npt      CR531633 - Remove FEATURE_DIME_MODEM dependencies
20/06/13   cs       Major Triton TSTS Syncup
20/05/13   cs       TSTS Updates
22/11/10   ky       Panic reset mechanism is changed. For all errors gl1 first
                    sees an nv item set or not. if this nv item is set then does
                    error fatal immediately other wise gl1 sends signal to grr for
                    recovery
27/04/10   cs       CR236460 Latest AFC\XO handling updates
01/02/10   ky       Extracting Ncell SCH Burst metrics should happen from gfwSchRxBurstMetricsBuffer
                    instead of gfwAsyncMetricBuffers for Q6.
08/02/10   cja      XO update
25/01/10   cja      CR224507. For XO correct freq error setting for NCell.
13/03/09   og       FEATURE_XO updates.
12/03/09   og       FEATURE_XO.
26/02/09   cs/hy    Add in initial XO support
12/06/07   og       Upgrade the FEATURE_GSM_L1_HFREQ_ERR_NCELL_SCH_DECODE to
                    include dedicated TCH NCELL SCH attempts.
03/29/06   gfr      Add frequency offset to ncell functions to support non
                    spec-compliant networks in WCDMA mode.
09/22/05   gfr      Removed FEATURE_SBI_MDSP_CONTROLLER
08/31/05   gfr      Replace gl1_msg_metrics_rpt with gl1_defs_metrics_rpt to
                    remove need for double copy
08/05/05   gfr      Simplify post-abort cleanup
07/29/05   gfr      DTM support
06/21/05   gfr      Remove calls to tt_suspend and tt_resume
04/21/05   yh       Support for panic reset
04/20/05   yh       Remove use of RF fifo
03/09/05   yh       BEP set to 0 for async_rx.
02/02/05   gfr      Handle if the SCH data is ready before metrics.
12/06/04   gfr      Featurize the mDSP SBI controller
11/01/04   yhong    Changed NCell Abort API to take a parameter
09/23/04   gfr      Suspend time tracking during activity.
08/30/04   gfr      Fill in metrics as long as the given pointer is valid.
08/12/04   gfr      Add more info to the abort message.
07/22/04   gfr      Removed QS_PER_FRAME definition.
07/14/04   gfr      Moved logging into L1.
06/09/04   gfr      Consolidate static variables for robustness and poll for
                    results a frame earlier.
06/23/04   gfr      Set the last time flag to TRUE for polling data.
06/10/04   gfr      Cleaned up abort mechanism, and get results a frame earlier.
06/02/04   gfr      Lint cleanup.
05/20/04   gfr      Mainlined FEATURE_COMMON_PWR_HANDLER
05/07/04   gfr      Removed broken legacy support for back-to-back operation.
12/19/03   gfr      Fixed abort handler to wait for the mdsp to be finished.
11/05/03   gfr      Support for quad-band.
07/22/03   gw       Added call to gl1_hw_reset() to gl1_msg_abort_rx_ncell_sch()
                    and updated comments.
07/11/03   gfr      Account for the sch search window when calculating at what
                    offset to wait a frame.
06/09/03   gw       Changed MSG_HIGH for too many messages to MSG_ERROR.
06/03/03   gfr      Suspend PSBI transactions during ncell activity.
05/13/03   gfr      Common power handler support.
05/01/03   ATM      Moved Frame Number management entirely inside GTMRS
04/16/03   gfr      Pass the function pointer when removing the handler.
01/31/03   gfr      Schedule SCH burst one frame earlier in certain cases.
01/24/03   ATM      Added NCELL_SCH msg/burst logging
10/28/02   gw       Changed calls to gl1_hw_was_async_burst_decoded to reflect
                    new interface supporting back-to-back async receives.
10/11/02   ih       Removed redef of FRAME_TICK_SETUP_MARGIN_QS. Now sourced
                    from mdsp with circular dependency gone.
09/24/02   dp       Workaround for mdsp SBI errors when Ncell SCH offset lies
                    near FTSM.  Problem is due to large ZIF tune time.  Will
                    be pulled out when ZIF tune time is fixed
08/16/02   JC       Corrected RSSI reporting s.t it is true RSSI from the MDSP.
07/30/02   dp       Resumed power measurements after getting async rx metrics
                    to avoid conflict with serving cell gain states
07/18/02   ATM      Added FCCH/SCH decode logging.
06/28/02   bk       Added support to handle sliding window during NCELL acquisition
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
03/25/02   gw       Cleaned up formatting.
03/21/02   gw       Made sure metrics callback is always called even if metrics
                    are not retrieved from the mDSP.
03/18/02   gw       Changed handling of metrics from polling to retrieving
                    when they are guaranteed to be available.  (New mDSP
                    drivers have made this possible.)
02/20/02   dp       Cleaned up SCH scheduling.  Added support to poll metrics
                    to handle all offsets
02/14/02   gw       Added support for AGC.
02/07/02   gw       Removed use of l1_scdrv_call_in_task().
12/18/01   dp       Added support to abort handler
11/6/01    bk       Changed NPL1 function calls to reflect change in interface
10/31/01   gw/dp    Updated comments. Made changes to pass in the required number
                    of data octets when retrieving async SCH decoded data.
10/31/01   dp       Made changes to use new gl1_hw_was_async_burst_decoded interface
                    to eliminate multiple copies when retrieving DSP results
10/24/01   dp       Added support for SCH decodes scheduling on the TCH
                    which takes into account the frame tick setup margin
10/23/01   gw       Corrected call to gl1_hw_get_asyncrx_metrics.
10/16/01   gw       Changes to support returning RSSI in dBm.
10/08/01   gw       Initial version.
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
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "geran_msgs.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

/*****************************************************

 LOCAL STORAGE

******************************************************/
/* Define this feature to delay aborting the SCH until the all driver
   processing is complete.  This avoids a problem where we go to sleep
   after aborting, but before the firmware is finished.  Unfortunately
   delaying the abort messed up the GPL1 so for now disable it */
/* #define DELAYED_SCH_ABORT_CALLBACK */

static int32 sch_rot_offset[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 0 ) };

extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[];

typedef enum
{
   NCELL_SCH_INACTIVE,
   NCELL_SCH_WAITING_FOR_BLOCK_START,
   NCELL_SCH_START,
   NCELL_SCH_RECEIVE,
   NCELL_SCH_WAITING,
   NCELL_SCH_GET_DATA,
   NCELL_SCH_ABORT_WAIT,
   NCELL_SCH_ABORT
} ncell_sch_state_type;

/* Structure containing information necessary for an ncell SCH rx */
typedef struct
{
   /* State variables */
   ncell_sch_state_type state;
   boolean              metrics_done;
   boolean              data_done;

   /* State when abort was called */
   ncell_sch_state_type aborted_state;

   /* Callbacks and report pointers */
   gl1_msg_rx_cb_type    callbacks;
   gl1_msg_decode_rpt    *data;
   gl1_defs_metrics_rpt  *metrics;

   /* Configured rx parameters */
   sys_algo_agc_T       *agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
   sys_algo_agc_T       *agc_ptr_divrx;
#endif
   ARFCN_T              ARFCN;
   uint16               timing_offset;

   gl1_hw_rx_tags_type  tags;
} ncell_sch_params_type;

/* NCell SCH stored parameters */
static ncell_sch_params_type ncell_sch_params[NUM_GERAN_DATA_SPACES];


/* Maximum offset at which to start the state machine, this guarantees the timeline
   is as expected, otherwise an offset too large may cause results to not be available
   for an extra frame */
#define MAX_NCELL_SCH_OFFSET (6250 + (GL1_HW_SCH_SEARCH_WIDTH_DEFAULT * 4 / 2))

#define NSCH_QBTA_OFFSET 1600

/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/
static boolean ncell_sch_rx_handler( boolean start_of_block,
                                     uint32  time_stamp,gas_id_t gas_id );


/*===========================================================================

FUNCTION gl1_msg_init_ncell_sch

DESCRIPTION
  This function is used to initialize the ncell SCH data structures.

===========================================================================*/
void gl1_msgi_init_ncell_sch (gas_id_t gas_id)
{
   L1_STATE_CHANGE(ncell_sch_params[gas_id].state                = NCELL_SCH_INACTIVE, "NCELL_SCH_INACTIVE");

   ncell_sch_params[gas_id].metrics_done         = FALSE;
   ncell_sch_params[gas_id].data_done            = FALSE;
   L1_STATE_CHANGE(ncell_sch_params[gas_id].aborted_state        = NCELL_SCH_INACTIVE, "NCELL_SCH_INACTIVE");
   ncell_sch_params[gas_id].data                 = NULL;
   ncell_sch_params[gas_id].metrics              = NULL;

   ncell_sch_params[gas_id].callbacks.data_cb    = NULL;
   ncell_sch_params[gas_id].callbacks.metrics_cb = NULL;
   ncell_sch_params[gas_id].callbacks.log_cb     = NULL;
   ncell_sch_params[gas_id].callbacks.abort_cb   = NULL;
}


/*===========================================================================

FUNCTION gl1_msg_cfg_ncell_sch

DESCRIPTION
  This function is used to configure the callbacks for the neighbor
  cell SCH. The callbacks will be invoked by the frame tick handler as
  required.

===========================================================================*/
void gl1_msg_cfg_ncell_sch ( gl1_msg_rx_cb_type *cb,gas_id_t gas_id)
{
   ncell_sch_params[gas_id].callbacks.data_cb    = cb->data_cb;
   ncell_sch_params[gas_id].callbacks.metrics_cb = cb->metrics_cb;
   ncell_sch_params[gas_id].callbacks.log_cb     = cb->log_cb;
   ncell_sch_params[gas_id].callbacks.abort_cb   = cb->abort_cb;
}


/*===========================================================================

FUNCTION gl1_msg_rx_ncell_sch

DESCRIPTION
  This function is used to receive an SB  on the SCH of a neighbor cell.

DEPENDENCIES
  gl1_msg_cfg_ncell_sch() must have been called at some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_rx_ncell_sch
(
   ARFCN_T          ARFCN,
   sys_algo_agc_T  *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
   sys_algo_agc_T  *agc_ptr_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   uint16           timing_offset,
   int32            freq_error,
   gl1_msg_rx_rpt  *rpt,
   gas_id_t         gas_id
)
{
   /* Make sure we are not already active */
   if (ncell_sch_params[gas_id].state == NCELL_SCH_WAITING_FOR_BLOCK_START)
   {
      MSG_GERAN_ERROR_2_G( GL1_ACQ_HDR"Double NCell SCH rx issued (arfcns=%d,%d)",
                 ncell_sch_params[gas_id].ARFCN.num, ARFCN.num);

      /* Remove the handler to avoid spurious errors, it will be re-installed
         below */
      gl1_msgi_remove_ft_handler(ncell_sch_rx_handler,gas_id);
   }
   else if (ncell_sch_params[gas_id].state != NCELL_SCH_INACTIVE)
   {
      MSG_GERAN_ERROR_2_G( GL1_ACQ_HDR"NCell SCH collision (state=%d, arfcn=%d)",
                 ncell_sch_params[gas_id].state, ARFCN.num);

      /* At this point L1 is probably stuck, but to continue with this
         sch could kill the lower layers even worse.  To try and recover
         reinstall the handler in case somehow it got uninstalled and hope
         for the best. */
      gl1_msgi_add_ft_handler(ncell_sch_rx_handler, GL1_MSG_FT_HDLR_ACQ_NCELL,gas_id);
      return;
   }

   /* Store the data */
   ncell_sch_params[gas_id].timing_offset  = timing_offset;
   ncell_sch_params[gas_id].agc_ptr        = agc_ptr;
   ncell_sch_params[gas_id].ARFCN          = ARFCN;
   ncell_sch_params[gas_id].data           = rpt->data;
   ncell_sch_params[gas_id].metrics        = rpt->metrics[0];

   /* Save the ARFCN in the AGC structure for debugging */
   agc_ptr->arfcn                  = ARFCN;

#ifdef FEATURE_GSM_RX_DIVERSITY
   ncell_sch_params[gas_id].agc_ptr_divrx  = agc_ptr_divrx;
   agc_ptr_divrx->arfcn       = ARFCN;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

   /* Convert to phase for rotator */
   sch_rot_offset[gas_id] = gl1_hw_hztophase( freq_error );


   MSG_GERAN_MED_3_G( GL1_ACQ_HDR"NCell SCH delta freq_error %dHz sch_rot_offset %d curr %dXO",
            freq_error, sch_rot_offset[gas_id], gl1_hw_get_xo_acc_freq_err(gas_id) );


   /* Add the handler and set the initial state */
   gl1_msgi_add_ft_handler(ncell_sch_rx_handler, GL1_MSG_FT_HDLR_ACQ_NCELL,gas_id);

   L1_STATE_CHANGE(ncell_sch_params[gas_id].state = NCELL_SCH_WAITING_FOR_BLOCK_START, "NCELL_SCH_WAITING_FOR_BLOCK_START");
}



/****************************************************************************
 ****************************************************************************
 ****************************************************************************
                            LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/


/*===========================================================================

FUNCTION ncell_sch_process_metrics

DESCRIPTION
  Polls for metrics results and processes them if available

===========================================================================*/
void ncell_sch_process_metrics (boolean last_try, uint32 time_stamp,gas_id_t gas_id)
{
   gl1_defs_metrics_rpt  metrics, *metrics_ptr;
   boolean               valid, got_metrics;
#ifdef GERAN_L1_HLLL_LNASTATE
   uint8 SawlessHLLLState[MAX_NUMBER_OF_CHANNELS] = {0};
   gl1_defs_metrics_lnastate gl1_metrics_lnastate;
#endif

   /* Get a pointer to valid metrics storage */
   metrics_ptr = ncell_sch_params[gas_id].metrics;
   if (metrics_ptr == NULL)
   {
      metrics_ptr = &metrics;
   }

   metrics_ptr->arfcn         = ncell_sch_params[gas_id].ARFCN;
   /* Try and get the metrics */

   /* sch flag is true here because we are doing here ncell sch*/
   got_metrics = gl1_hw_get_async_rx_metrics_geran
    (
      metrics_ptr,
      last_try,
      &ncell_sch_params[gas_id].tags
      , TRUE
      ,geran_map_gas_id_to_nas_id(gas_id)
    );

   /* Check if they are ready */
   if (got_metrics)
   {
      /* Call the system algorithm for AGC */
      valid = (metrics_ptr->powerStatus == RF_IN_RANGE ? TRUE : FALSE);
      sys_algo_agc_update
       (
         ncell_sch_params[gas_id].agc_ptr,
         metrics_ptr->pwr_dBm_x16,
         valid,
         time_stamp,
         gas_id
       );

#ifdef FEATURE_GSM_RX_DIVERSITY
      if(metrics_ptr->rssi_divrx != 0)
       {
       sys_algo_agc_update
        (
      ncell_sch_params[gas_id].agc_ptr_divrx,
      metrics_ptr->pwr_dBm_x16_divrx,
      valid,
      time_stamp,
      gas_id
        );
       }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

      /* If a metrics callback is registered then call it. */
      metrics_ptr->valid         = TRUE;
      metrics_ptr->offset_to_sob = (last_try ? 2 : 1);

      /* DSP has the opposite notation, negative implies the burst
         arrived later than expected and vice versa */
      metrics_ptr->timing_offset = -metrics_ptr->timing_offset;
   }

   /* If we are giving up, set the valid flag to false */
   else if (last_try)
   {
      metrics_ptr->valid         = FALSE;
      metrics_ptr->offset_to_sob = 2;

      MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"NCell SCH metrics unavailable FN=%d",
                 GSTMR_GET_FN_GERAN(gas_id));
   }

   /* Have to call callback regardless */
   if (got_metrics || last_try)
   {
#ifdef GERAN_L1_HLLL_LNASTATE
      gl1_metrics_lnastate.NoOfChannels  = 0;
      if(gl1_hw_get_sawless_HLLL_support(metrics_ptr->arfcn.band,gas_id))
        {
        gl1_metrics_lnastate.NoOfChannels              = 1;
        gl1_metrics_lnastate.gsm_metrics[0].arfcn      = metrics_ptr->arfcn;
        gl1_metrics_lnastate.gsm_metrics[0].rx_lvl_dbm = metrics_ptr->pwr_dBm_x16;
        gl1_metrics_lnastate.gsm_metrics[0].snr        = metrics_ptr->snr;
        gl1_metrics_lnastate.gsm_metrics[0].jdet_value = metrics_ptr->jdet_value;
        SawlessHLLLState[0]                            = metrics_ptr->SawlessHLLLState;
        gl1_hw_rf_burst_metrics(&gl1_metrics_lnastate,gas_id);
        }
      if(gl1_metrics_lnastate.NoOfChannels)
       {
       gl1_hw_log_HLin_LLin_metrics(&gl1_metrics_lnastate,SawlessHLLLState,gas_id);
       }
#endif /*GERAN_L1_HLLL_LNASTATE*/

      if (ncell_sch_params[gas_id].callbacks.metrics_cb)
      {
         ncell_sch_params[gas_id].callbacks.metrics_cb(&ncell_sch_params[gas_id].metrics,gas_id);
      }
      ncell_sch_params[gas_id].metrics_done = TRUE;
   }
}



/*===========================================================================

FUNCTION ncell_sch_process_data

DESCRIPTION
  Polls for data and processes it if available

===========================================================================*/
void ncell_sch_process_data (boolean last_try,gas_id_t gas_id)
{
   uint8 i;

   ncell_sch_params[gas_id].data_done = gl1_hw_get_async_rx_data_geran
    (
      GL1_HW_SCH,
      &ncell_sch_params[gas_id].data->sch.hdr,
      ncell_sch_params[gas_id].data->sch.data,
      (uint16)MAX_SCH_OCTETS,
      last_try,
      geran_map_gas_id_to_nas_id(gas_id)
    );

   /* If we have another chance, then we wait */
   if (!last_try && !ncell_sch_params[gas_id].data_done) return;

   /* Make sure that we report the decode failed if nothing was available */
   if (!ncell_sch_params[gas_id].data_done)
   {
      ncell_sch_params[gas_id].data->sch.hdr.good_data = 0;

      MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"NCell SCH data not available FN=%d",
                 GSTMR_GET_FN_GERAN(gas_id));
   }

   /* This should not happen, but we need to guarantee that the metrics
      callback is made and made before the data callback */
   if (!ncell_sch_params[gas_id].metrics_done)
   {
      MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"NCell SCH data ready but no metrics FN=%d",
                 GSTMR_GET_FN_GERAN(gas_id));

      if (ncell_sch_params[gas_id].metrics != NULL)
      {
         ncell_sch_params[gas_id].metrics->valid         = FALSE;
         ncell_sch_params[gas_id].metrics->arfcn         = ncell_sch_params[gas_id].ARFCN;
      }

      if (ncell_sch_params[gas_id].callbacks.metrics_cb)
      {
         ncell_sch_params[gas_id].callbacks.metrics_cb(&ncell_sch_params[gas_id].metrics,gas_id);
      }

      ncell_sch_params[gas_id].metrics_done = TRUE;
   }

   /* Fill in remaining fields */
   ncell_sch_params[gas_id].data->sch.arfcn         = ncell_sch_params[gas_id].ARFCN;
   ncell_sch_params[gas_id].data->sch.offset_to_sob = (last_try ? 2 : 1);

   /* Compute BEP for each burst */
   for(i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
       ncell_sch_params[gas_id].data->sch.hdr.bep[i] = 0;
   }

   /* The mdsp is finished now, so call the appropriate callback */
   if (ncell_sch_params[gas_id].callbacks.data_cb)
   {
      ncell_sch_params[gas_id].callbacks.data_cb(ncell_sch_params[gas_id].data,gas_id);
   }
   else
   {
      MSG_GERAN_HIGH_0_G( GL1_ACQ_HDR"No ncell sch dl callback registered");
   }

   /* Clean up drivers */
   gl1_hw_cleanup_async_rx_geran(gas_id);
}



/*===========================================================================

FUNCTION ncell_sch_rx_handler

DESCRIPTION
  This function should be called every frame on the 51 frame multiframe. The
  handler is installed as part of a request to rx a msg by gl1_msg_rx_ncell_sch().
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
static boolean ncell_sch_rx_handler
(
   boolean start_of_block,   /* next frame is start of block */
   uint32  time_stamp,       /* currently not used           */
   gas_id_t gas_id
)
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /* Kick the state machine */
   switch (ncell_sch_params[gas_id].state)
   {
      case NCELL_SCH_INACTIVE:
         MSG_GERAN_ERROR_0_G( GL1_ACQ_HDR"NCell SCH inactive");
         return TRUE;

      case NCELL_SCH_WAITING_FOR_BLOCK_START:
         /* Wait for the start of block flag to be set */
         if (!start_of_block) return FALSE;

         ncell_sch_params[gas_id].data_done = FALSE;
         ncell_sch_params[gas_id].metrics_done = FALSE;
         ncell_sch_params[gas_id].state++;
         /* fall through */

      case NCELL_SCH_START:
         /* In order to ensure enough time for the mdsp to cancel any
            conflicting transmits, we only delay the SCH if the offset
            is greater than this amount */
         if (ncell_sch_params[gas_id].timing_offset >= MAX_NCELL_SCH_OFFSET)
         {
            ncell_sch_params[gas_id].timing_offset -= QS_PER_FRAME;
            break;
         }

         MSG_GERAN_HIGH_1_G( GL1_ACQ_HDR"Applying sch_rot_offset %d prior to SCH decode",
                   sch_rot_offset[gas_id]);

         /* Start the receive */
         gl1_hw_rx_async_sch_burst2 (
                                      ncell_sch_params[gas_id].ARFCN,
                                      ncell_sch_params[gas_id].agc_ptr->pwr_dBm_x16,
#ifdef FEATURE_GSM_RX_DIVERSITY
                                      ncell_sch_params[gas_id].agc_ptr_divrx->pwr_dBm_x16,
#endif
                                      ncell_sch_params[gas_id].timing_offset,
                                      sch_rot_offset[gas_id],
                                      &ncell_sch_params[gas_id].tags,
                                      gas_id
                                      );

         MSG_GERAN_MED_3_G( GL1_ACQ_HDR"NCell SCH (arfcn=%d): offset=%d, FN=%d",
                  ncell_sch_params[gas_id].ARFCN.num, ncell_sch_params[gas_id].timing_offset, GSTMR_GET_FN_GERAN(gas_id) );
#if defined (FEATURE_QBTA)		 
         if(l1_tskisr_blk->qta_in_progress && gl1_hw_get_qta_cm_fw_rf_init_status(gas_id) && gl1_hw_check_in_QBTA(gas_id))
         {
           gl1_hw_qta_fw_rf_deinit(gas_id, ( ncell_sch_params[gas_id].timing_offset + NSCH_QBTA_OFFSET) ,TRUE);
         }
#endif
         ncell_sch_params[gas_id].state++;
         break;

      case NCELL_SCH_RECEIVE:
         ncell_sch_params[gas_id].state++;
         break;

      case NCELL_SCH_WAITING:
         /* Check metrics and data */
         ncell_sch_process_metrics(FALSE, time_stamp,gas_id);
         ncell_sch_process_data(FALSE,gas_id);

         /* If the data is done, we can finish early */
         if (ncell_sch_params[gas_id].data_done)
         {
            L1_STATE_CHANGE(ncell_sch_params[gas_id].state = NCELL_SCH_INACTIVE, "NCELL_SCH_INACTIVE");
         }
         else
         {
            L1_STATE_CHANGE(ncell_sch_params[gas_id].state = NCELL_SCH_GET_DATA, "NCELL_SCH_GET_DATA");
         }
         break;

      case NCELL_SCH_GET_DATA:
         /* Check metrics and data for the last time */
         if (!ncell_sch_params[gas_id].metrics_done)
         {
            ncell_sch_process_metrics(TRUE, time_stamp,gas_id);
         }
         ncell_sch_process_data(TRUE,gas_id);

         L1_STATE_CHANGE(ncell_sch_params[gas_id].state = NCELL_SCH_INACTIVE, "NCELL_SCH_INACTIVE");
         break;

      case NCELL_SCH_ABORT_WAIT:

         L1_STATE_CHANGE(ncell_sch_params[gas_id].state = NCELL_SCH_ABORT, "NCELL_SCH_ABORT");
         break;

      case NCELL_SCH_ABORT:
         /* Clean up drivers */
         gl1_hw_cleanup_async_rx_geran(gas_id);

         L1_STATE_CHANGE(ncell_sch_params[gas_id].state = NCELL_SCH_INACTIVE, "NCELL_SCH_INACTIVE");

         if (ncell_sch_params[gas_id].callbacks.abort_cb)
         {
            ncell_sch_params[gas_id].callbacks.abort_cb(gas_id);
         }
         else
         {
            MSG_GERAN_HIGH_0_G( GL1_ACQ_HDR"No Ncell SCH abort CB registered");
         }
         break;

      default:
         MSG_GERAN_HIGH_1_G( GL1_ACQ_HDR"Error in rx state machine %d",
                   ncell_sch_params[gas_id].state);

         L1_STATE_CHANGE(ncell_sch_params[gas_id].state = NCELL_SCH_INACTIVE, "NCELL_SCH_INACTIVE");
         break;
   } /* end of switch */

   /* If we are inactive, remove the handler */
   return (ncell_sch_params[gas_id].state == NCELL_SCH_INACTIVE ? TRUE: FALSE);
}


/*===========================================================================

FUNCTION  gl1_msg_abort_rx_ncell_sch

DESCRIPTION
  This function aborts the currently ongoing ncell SCH rx and invokes the
  registered abort callback when done

DEPENDENCIES
  Must only be called in the context of the same ISR that the message layer
  frame tick notification is called from, i.e. we don't want the ftn running
  when we are part way through this function.

PARAMETERS
  None

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void gl1_msg_abort_rx_ncell_sch(gl1_msg_ncell_abort_type type,gas_id_t gas_id)
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   /* Informational message */
   MSG_GERAN_MED_3_G( GL1_ACQ_HDR"NCell SCH aborted: state=%d ARFCN=%d FN=%d",
            ncell_sch_params[gas_id].state, ncell_sch_params[gas_id].ARFCN.num, GSTMR_GET_FN_GERAN(gas_id) );

   /* Save state when we aborted for later use */
   ncell_sch_params[gas_id].aborted_state = ncell_sch_params[gas_id].state;

   switch (type)
   {
      case DELAYED_ABORT:
          /* Check our state to see if we can abort immediately */
          switch (ncell_sch_params[gas_id].state)
          {
             /* We can abort immmediately */
             case NCELL_SCH_INACTIVE:
                MSG_GERAN_ERROR_0_G( GL1_ACQ_HDR"Calling abort in INACTIVE state");
                break;

             case NCELL_SCH_WAITING_FOR_BLOCK_START:
             case NCELL_SCH_START:
             case NCELL_SCH_GET_DATA:
             {
                L1_STATE_CHANGE(ncell_sch_params[gas_id].state = NCELL_SCH_ABORT, "NCELL_SCH_ABORT");
#if defined (FEATURE_QBTA)
                if(l1_tskisr_blk->qta_in_progress && gl1_hw_get_qta_cm_fw_rf_init_status(gas_id) && gl1_hw_check_in_QBTA(gas_id))
                {
                  gl1_hw_qta_fw_rf_deinit(gas_id, ( ncell_sch_params[gas_id].timing_offset + NSCH_QBTA_OFFSET) ,TRUE);
                }
#endif
                break;
             }

             /* We are already aborting */
             case NCELL_SCH_ABORT_WAIT:
             case NCELL_SCH_ABORT:
             {
                break;
             }

             /* Some extra wait time is required */
             default:
             {
                L1_STATE_CHANGE(ncell_sch_params[gas_id].state = NCELL_SCH_ABORT_WAIT, "NCELL_SCH_ABORT_WAIT");
                break;
             }
          }
          break;

      case IMMEDIATE_ABORT:
          L1_STATE_CHANGE(ncell_sch_params[gas_id].state = NCELL_SCH_ABORT, "NCELL_SCH_ABORT");
          break;

      default:
          if(geran_get_nv_recovery_restart_enabled(gas_id))
          {
            MSG_GERAN_ERROR_0_G( GL1_ACQ_HDR"Aborting sch with unknown type!");
            gl1_hw_handle_panic(gas_id);
          }
          else
          {
#ifdef FEATURE_QSH_MDUMP
            QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
            ERR_GERAN_FATAL_0_G( GL1_ACQ_HDR"Aborting sch with unknown type!");
          }
          break;
   }
}

/*===========================================================================

FUNCTION  gl1_hw_check_metrics_done

DESCRIPTION
  This function returns the flag which indicates the status of metrics done
  
DEPENDENCIES
  NA

PARAMETERS
  None

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/

boolean gl1_hw_check_metrics_done(gas_id_t gas_id)
{

  return (ncell_sch_params[gas_id].metrics_done);
}

