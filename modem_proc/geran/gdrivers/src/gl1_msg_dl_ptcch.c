/*===========================================================================

     IMPLEMENTATION OF GPRS NPL1 DOWNLINK DL PTCCH FUNCTIONS

GENERAL DESCRIPTION
   This file implements GPRS NPL1 downlink DL PTCCH functions.

   Some points of consideration are:
   1. The processing is based on a PTCCH/D block consisting of 104 frames. The
   command to receive message may be given in the first 3 frames of a block
   period. ( block period consists of 4 frames) (block N).
   In the fourth frame of the block, a single receive burst is scheduled. After
   26 frames the next burst is scheduled. At the end of 4 bursts, the data is
   available two frames after the scheduled receive burst.
   The metrics and log data are available two frames after each burst is
   scheduled.


EXTERNALIZED FUNCTIONS

gl1_msgi_dl_ptcch_init
gl1_msgi_dl_ptcch_terminate


INITIALIZATION AND SEQUENCING REQUIREMENTS
The following is the desired order in which functions should be called

gl1_msgi_dl_ptcch_init
gl1_msgi_dl_ptcch_terminate

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_dl_ptcch.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ---------------------------------------------------------- 
29/05/15   pa       CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD. 
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
16/12/14   jk       CR753423:LIF for Idle,voice and data
10/10/14   cos      CR736560 BO 2.x Idle RxD mirror combined exit/sleep requests according to the enter/wakeup modes 
07/10/14   pg       CR730132 F3 cleanup 
12/09/14   fm       CR718734 to avoid RX window is stuck in close state, open rx window when GL1 is in L1_TRANSFER_INIT state 
20/06/13   cs       Major Triton TSTS Syncup
10/06/13   cs       Further TSTS Updates
21/05/13   cs       TSTS Updates
29/04/13   ws       CR481359 enable GTA support
10/04/13   ap       CR472264 DSDS: Fix the gas_id problems when the IQ logging is enabled
02/12/09   ws       Added FEATURE_GSM_GPRS_AGC_UPDATE for GPRS/EGPRS AGC
                    Improvements
23/09/09   ps       Resolve Klocwork Buffer Overflow errors
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
01/12/08   cs       Add new include gl1_hw_debug.h
14/01/08   cs       Allow for separate IQ logging and saving
06/07/07   agv      CR123023 - Update fix for AFC/AGC corruption.
27/06/07   agv      CR123023 Fix for AGC corruption, and TT.
12/08/05   gfr      Modify gl1_hw_save_iq_samples to work with L4
11/07/05   gfr      QLint
07/29/05   gfr      DTM support
06/21/05   gfr      Add support for storing the camped on timeslot
06/16/05   gfr      Added calls to gl1_hw_set_iq_samples_tn
06/08/05   gfr      Added gl1_msg_inc_dl_ptcch_frame_count
06/02/05   gfr      Make handler static
05/25/05   gfr      Add IQ sample saving support
04/26/05   gfr      Handle invalid rx metrics gracefully
04/22/05   gfr      Remove conditional abort as it is no longer needed
04/21/05   yh       Support for panic reset
04/20/05   yh       Remove use of RF fifo
04/14/05   gfr      Consolidate GSM hw layer parameters
04/13/05   gfr      Change pending_tn as well when update_dl_ptcch_tn is called
04/08/05   gfr      Combined GSM and GPRS channel types
08/26/04   gfr      Added first_burst parameter to gprs rx command.
07/22/04   gfr      EGPRS support.
03/22/04   gfr      Only apply the configured timeslot and TSC for the next rx.
11/05/03   gfr      Support for quad-band.
08/13/03   gfr      Pass correct metrics parameter to AGC
06/03/03   gfr      Pass last attempt parameter when looking for decoded data.
05/01/03   gfr      Change featuring from FEATURE_GSM to FEATURE_GSM_GPRS
02/24/03   gfr      Lint cleanup.
01/17/03   gfr      Configured parameters take effect immediately.
11/15/02   gfr      Return the arfcn in the metrics structure.
09/18/02   gfr      Added conditional abort for PTCCH to support TBF suspension.
09/06/02   gfr      Added support for GPRS AGC.
08/28/02   gfr      Changed DL abort functionality to always force the abort.
06/28/02   bk       Added abort functionality and removed params from log
                    callback
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
04/16/02   bk       Made changes based on metrics hw layer i/f change
03/14/02   bk       Get metrics even if metrics callback is not installed
02/08/02   bk       changed interface to add agc info.
                    ....many revisions later
10/29/01   bk       Initial revision

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
#include "gl1_msg_g.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_debug.h"

#include "gl1_msgi.h"
#include "gl1_msg_pdch.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "msg.h" /* Error/Output */

/* header files containing system algorithms for AGC/AFC/TT etc */
#include "gl1_sys_algo.h"
#include "gl1_sys_algo_gprs.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
#define NUM_BURSTS_TO_RECEIVE            GL1_DEFS_FRAMES_IN_BLK_PERIOD
#define NUM_TN_TO_RX_PER_FRAME           ( 1 )
#define NUM_FRAMES_BW_BURSTS             ( 26 )

#define FRAMES_FROM_BURST_TO_RX_METRICS  ( 3 )
#define FRAMES_FROM_BURST_TO_RX_DATA     ( 6 )

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
#define FRAMES_FROM_BURST_TO_RX_LOG      ( 2 )
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */

#define DECODE_TAG                      ( NUM_BURSTS_TO_RECEIVE - 1 )

/* 1dB threshold for valid update of AGC */
#define PTCCH_SNR_THRESHOLD 320

typedef struct
{
  gl1_hw_rx_signal_type       signal_info;
  gl1_hw_rx_pkt_burst_type    burst_info;
  ARFCN_T                     arfcn[ NUM_BURSTS_TO_RECEIVE ];
  sys_algo_agc_T              *agc_ptr;
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA)
  sys_algo_agc_T              *agc_ptr_divrx;
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA */
  gl1_hw_rx_tags_type         tags[ NUM_BURSTS_TO_RECEIVE ];
} dl_ptcch_msg_info_type;

/* Data space for MultiSim  */
static dl_ptcch_msg_info_type ptcch_params[NUM_GERAN_DATA_SPACES];

static gl1_defs_rx_pkt_cb_type rx_pkt_cb[NUM_GERAN_DATA_SPACES];
static gl1_defs_rx_pkt_mem_type ptcch_ptr_info[NUM_GERAN_DATA_SPACES];

static  boolean  rx_window_open[NUM_GERAN_DATA_SPACES] =
                   { INITIAL_VALUE( TRUE ) };

static  boolean  issued_rx_cmd[NUM_GERAN_DATA_SPACES] =
                   { INITIAL_VALUE( FALSE ) };

static  boolean  reg_rx_hdlr_flag[NUM_GERAN_DATA_SPACES] =
                   { INITIAL_VALUE( FALSE ) };

typedef struct
{
  uint8    rx_num_bursts_in_block;
  uint8    rx_data_count;
  uint8    frame_count;
  boolean  proc_in_progress;
  uint8    burst_num;
  gl1_defs_tn_type rx_tn;
  gl1_defs_tn_type camp_tn;

  gl1_defs_tn_type pending_tn;
  uint8    pending_tsc;
} dl_ptcch_msg_info_global_type;

#define GL1_PTCCH_INFO_INIT { 0, 0, 0, FALSE, 0, GL1_DEFS_TN_0, GL1_DEFS_TN_0, GL1_DEFS_TN_0, 0 }

static dl_ptcch_msg_info_global_type dl_ptcch_msg_info_glb[NUM_GERAN_DATA_SPACES] =
                           {INITIAL_VALUE(GL1_PTCCH_INFO_INIT)};

static boolean gl1_msgi_dl_ptcch_hdlr
(
   boolean next_frame_is_ptcch_sacch_block_start,
   uint32  time_stamp,
   gas_id_t gas_id
);

/*===========================================================================

FUNCTION gl1_msgi_dl_ptcch_init

DESCRIPTION
   This function initializes data structures that are used by Dl PTCCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_dl_ptcch_init( gas_id_t gas_id )
{
   /* Initialize the parameters */
   memset(&ptcch_params[gas_id], 0, sizeof(ptcch_params[gas_id]));

   rx_pkt_cb[gas_id].data_callback    = NULL;
   rx_pkt_cb[gas_id].log_callback     = NULL;
   rx_pkt_cb[gas_id].metrics_callback = NULL;

   /* Initialize internal variables */
   dl_ptcch_msg_info_glb[gas_id].rx_num_bursts_in_block  = 0;
   dl_ptcch_msg_info_glb[gas_id].rx_data_count           = 0;
   dl_ptcch_msg_info_glb[gas_id].frame_count             = 0;
   dl_ptcch_msg_info_glb[gas_id].proc_in_progress        = FALSE;
   dl_ptcch_msg_info_glb[gas_id].burst_num               = 0;

   OPEN_RX_WINDOW(gas_id);
   CLEAR_RX_CMD_ISSUED_FLAG(gas_id);
   ALLOW_RX_HDLR_TO_BE_REG(gas_id);
}

/*===========================================================================

FUNCTION gl1_msgi_dl_ptcch_terminate

DESCRIPTION       DL PTCCH processing

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_dl_ptcch_terminate( gas_id_t gas_id )
{
   /* Reset internal variables */
   dl_ptcch_msg_info_glb[gas_id].rx_num_bursts_in_block  = 0;
   dl_ptcch_msg_info_glb[gas_id].rx_data_count           = 0;
   dl_ptcch_msg_info_glb[gas_id].frame_count             = 0;
   dl_ptcch_msg_info_glb[gas_id].proc_in_progress        = FALSE;
   dl_ptcch_msg_info_glb[gas_id].burst_num               = 0;

   /* Remove handler if installed */
   if (IS_RX_HDLR_REG(gas_id))
   {
      gl1_msgi_remove_ft_handler( gl1_msgi_dl_ptcch_hdlr,gas_id );
   }

   /* Terminate all ptcch processing */
   CLOSE_RX_WINDOW(gas_id);
   CLEAR_RX_CMD_ISSUED_FLAG(gas_id);
   DONT_ALLOW_RX_HDLR_TO_BE_REG(gas_id);
}

/*===========================================================================

FUNCTION gl1_msg_abort_dl_ptcch

DESCRIPTION
  Aborts a PTCCH receive in progress.
  The DL PTCCH channel is aborted when the function returns.
  Subsequent metrics or data callback will not be invoked.

===========================================================================*/
void gl1_msg_abort_dl_ptcch( gas_id_t gas_id )
{
   /* If we are in the middle of a PTCCH message, print a debug message */
   if (dl_ptcch_msg_info_glb[gas_id].proc_in_progress)
   {
      MSG_4(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
            "Aborting PTCCH: burst=%d, fc=%d, dc=%d, FN=%d",
            dl_ptcch_msg_info_glb[gas_id].burst_num,
            dl_ptcch_msg_info_glb[gas_id].frame_count,
            dl_ptcch_msg_info_glb[gas_id].rx_data_count,
            GSTMR_GET_FN_GERAN(gas_id));
   }

   /* Terminate processing in the NPL1 if we are in the middle of decoding,
      non-DTM mDSP driver does not require this abort. */
   if (dl_ptcch_msg_info_glb[gas_id].rx_data_count > 0)
   {
      gl1_hw_abort_rx_decode(&ptcch_params[gas_id].tags[DECODE_TAG],gas_id);
   }

   /* Reset internal variables */
   dl_ptcch_msg_info_glb[gas_id].rx_num_bursts_in_block  = 0;
   dl_ptcch_msg_info_glb[gas_id].rx_data_count           = 0;
   dl_ptcch_msg_info_glb[gas_id].frame_count             = 0;
   dl_ptcch_msg_info_glb[gas_id].proc_in_progress        = FALSE;
   dl_ptcch_msg_info_glb[gas_id].burst_num               = 0;

   /* Terminate all ptcch processing */
   OPEN_RX_WINDOW(gas_id);
   CLEAR_RX_CMD_ISSUED_FLAG(gas_id);

   /* Remove handler if it is installed */
   if (IS_RX_HDLR_REG(gas_id))
   {
      gl1_msgi_remove_ft_handler(gl1_msgi_dl_ptcch_hdlr,gas_id);
      ALLOW_RX_HDLR_TO_BE_REG(gas_id);
   }
}

/*=============================================================================
FUNCTION gl1_msg_cfg_dl_ptcch

DESCRIPTION
   Configures the DL PTCCH channel.  Configuration takes place immediately, ie
   is not delayed until the next block.

DEPENDENCIES
   The message layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_cfg_dl_ptcch
(
  gl1_defs_tn_type              tn,
  uint8                         tsc,
  const gl1_defs_rx_pkt_cb_type  *rx_msg_cb_info,
  gas_id_t gas_id
)
{
  /* Store the specified parameters */
  dl_ptcch_msg_info_glb[gas_id].pending_tsc = tsc;
  dl_ptcch_msg_info_glb[gas_id].pending_tn = tn;

  rx_pkt_cb[gas_id].data_callback    = rx_msg_cb_info->data_callback;
  rx_pkt_cb[gas_id].log_callback     = rx_msg_cb_info->log_callback;
  rx_pkt_cb[gas_id].metrics_callback = rx_msg_cb_info->metrics_callback;
}

/*=============================================================================
FUNCTION gl1_msg_update_dl_ptcch_tn

DESCRIPTION
   Updates the current PTCCH timeslot - used if the timeslot timing has
   changed in between bursts.

DEPENDENCIES
   The message layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_update_dl_ptcch_tn( gl1_defs_tn_type tn ,gas_id_t gas_id)
{
   /* If we are in the middle of a PTCCH message, print a debug message */
   if (dl_ptcch_msg_info_glb[gas_id].proc_in_progress && ptcch_params[gas_id].signal_info.tn != tn)
   {
      MSG_GERAN_MED_3_G("Active PTCCH tn changed (%d to %d), FN=%d",
              ptcch_params[gas_id].signal_info.tn, tn, GSTMR_GET_FN_GERAN(gas_id));
   }

   /* Set the current and pending timeslots to the given one */
   dl_ptcch_msg_info_glb[gas_id].pending_tn = tn;
   ptcch_params[gas_id].signal_info.tn = tn;
}

/*=============================================================================
FUNCTION gl1_msg_inc_dl_ptcch_frame_count

DESCRIPTION
   Increments the current PTCCH frame count, called if frame tick slamming
   has resulted in missing a frame between PTCCH bursts.

============================================================================= */
void gl1_msg_inc_dl_ptcch_frame_count (gas_id_t gas_id)
{
   /* Don't care if we are not processing yet */
   if (!dl_ptcch_msg_info_glb[gas_id].proc_in_progress)
   {
      return;
   }

   /* Abort if we are incrementing in such a way to miss a frame */
   if (dl_ptcch_msg_info_glb[gas_id].frame_count == 0)
   {
      MSG_GERAN_ERROR_1_G("Aborting dl PTCCH due to inc frame count, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
      gl1_msg_abort_dl_ptcch(gas_id);
      return;
   }

   dl_ptcch_msg_info_glb[gas_id].frame_count++;
   if ( dl_ptcch_msg_info_glb[gas_id].frame_count >= NUM_FRAMES_BW_BURSTS )
   {
      dl_ptcch_msg_info_glb[gas_id].frame_count = 0;
   }
}

/*=============================================================================
FUNCTION gl1_msg_rx_dl_ptcch

DESCRIPTION
   Schedule a receive and decode of a PTCCH block of data at the specified
   frequencies. This function also causes the metrics and data callback handlers
   to be invoked every frame.

   The pointers to the buffers for decoded data, metrics and log will
   subsequently be filled by NPL1 and the corresponding callback will be invoked
   to notify that the data/information has been copied on to the buffer.

   This function is to be called anywhere in the first 3 frames of the previous
   block (block with 4 frames). The first received burst is scheduled to be
   received in the frame following the end of the previous block boundary. NPL1
   then schedules the remaining 3 bursts 26 frames apart.

DEPENDENCIES
   The channel must be configured.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_rx_ptcch
(
  const ARFCN_T                  arfcn[ GL1_DEFS_FRAMES_IN_BLK_PERIOD ],
  sys_algo_agc_T                 *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  sys_algo_agc_T                 *agc_ptr_divrx,
#endif
  const gl1_defs_rx_pkt_mem_type *rx_data_info,
  gas_id_t gas_id
)
{
   uint8                       frame_num;

   if ( IS_RX_WINDOW_OPEN(gas_id) == FALSE )
   {
     /* got receive command in frames other than 1, 2 or 3. */
     MSG_GERAN_ERROR_0_G("Got Receive Cmd while Rx window was closed\n");
     return;
   }

   if ( RX_CMD_ISSUED_IN_THIS_BLOCK(gas_id) )
   {
      /* got receive command in frames other than 1, 2 or 3. */
      MSG_GERAN_ERROR_0_G("Got duplicate PTCCH Rx cmd\n");
      return;
   }

   /*---------------------------------------------------------------------------
     Add the PTCCH handler. There is no danger of another PTCCH handler being
     installed while the current handler is registered.
   ---------------------------------------------------------------------------*/
   gl1_msgi_add_ft_handler( gl1_msgi_dl_ptcch_hdlr, GL1_MSG_FT_HDLR_DL_PTCCH_SACCH,gas_id );

   /* For the received message store the frequency */
   for ( frame_num = 0; frame_num < GL1_DEFS_FRAMES_IN_BLK_PERIOD; frame_num++ )
   {
     ptcch_params[gas_id].arfcn[ frame_num ] = arfcn[ frame_num ];
   }
   ptcch_params[gas_id].agc_ptr = agc_ptr;

   /* Fill in static frame layer parameters */
#if !defined (FEATURE_GSM_GPRS_AGC_UPDATE)
   /* Update AGC for each PTCCH burst rather than same AGC for all 4 bursts */
   ptcch_params[gas_id].signal_info.rx_signal_strength
     = sys_algo_agc_gprs_expected_pwr(agc_ptr->pwr_dBm_x16, GL1_HW_PTCCHD,gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
   ptcch_params[gas_id].agc_ptr_divrx = agc_ptr_divrx;
   /* Update Rx diversity AGC for each PTCCH burst rather than same AGC for all 4 bursts */
   ptcch_params[gas_id].signal_info.rx_signal_strength_divrx
     = sys_algo_agc_gprs_expected_pwr(agc_ptr_divrx->pwr_dBm_x16, GL1_HW_PTCCHD,gas_id);
#endif
#endif

   ptcch_params[gas_id].signal_info.tn = dl_ptcch_msg_info_glb[gas_id].pending_tn;
   ptcch_params[gas_id].burst_info.tsc = dl_ptcch_msg_info_glb[gas_id].pending_tsc;
   ptcch_params[gas_id].burst_info.channel_type = GL1_HW_PTCCHD;
   ptcch_params[gas_id].burst_info.search_width = GL1_HW_GPRS_SEARCH_WIDTH;

   /* store the metrics and data pointers, that will be used later */
   ptcch_ptr_info[gas_id] = *rx_data_info;

   REG_RX_CMD_WAS_ISSUED(gas_id);
   DONT_ALLOW_RX_HDLR_TO_BE_REG(gas_id);
}

/*=============================================================================
FUNCTION  gl1_msgi_dl_ptcch_hdlr

DESCRIPTION
   This function sends commands to receive bursts from the frame layer.

DEPENDENCIES
None

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
static boolean gl1_msgi_dl_ptcch_hdlr
(
  boolean next_frame_is_ptcch_sacch_block_start,
  uint32  time_stamp,
  gas_id_t gas_id
)
{
  uint16 ptcch_burst_snr = 0;
   /*---------------------------------------------------------------------------
   * By default initialize the decode flag to false.
   ---------------------------------------------------------------------------*/
   boolean remove_hdlr  = FALSE;

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
   boolean is_block_start = FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/

   /*---------------------------------------------------------------------------
   * Processing to be done in the a frame before the PTCCH block.
   * The following processing is done
     - The receive command window is closed.
     - Initialize the number of bursts to receive
     - Take a snapshot of the configured parameters.
   ---------------------------------------------------------------------------*/
   if ( next_frame_is_ptcch_sacch_block_start )
   {
      CLOSE_RX_WINDOW(gas_id);
      dl_ptcch_msg_info_glb[gas_id].frame_count = 0;

      if (RX_CMD_ISSUED_IN_THIS_BLOCK(gas_id))
      {
         dl_ptcch_msg_info_glb[gas_id].rx_num_bursts_in_block = NUM_BURSTS_TO_RECEIVE;
         dl_ptcch_msg_info_glb[gas_id].proc_in_progress       = TRUE;

         CLEAR_RX_CMD_ISSUED_FLAG(gas_id);
      }
   }

   /* If the PTCCH block has not started yet then return doing nothing */
   if ( !dl_ptcch_msg_info_glb[gas_id].proc_in_progress )
   {
      return remove_hdlr;
   }

   /* Start of 26 frames, receive a burst */
   /* Add Bounds check for array access */
   if ((dl_ptcch_msg_info_glb[gas_id].frame_count == 0) &&
       (dl_ptcch_msg_info_glb[gas_id].rx_num_bursts_in_block > 0))
   {
      dl_ptcch_msg_info_glb[gas_id].burst_num =
            NUM_BURSTS_TO_RECEIVE - dl_ptcch_msg_info_glb[gas_id].rx_num_bursts_in_block;

      /* If this is the last burst start the data countdown */
      if ( dl_ptcch_msg_info_glb[gas_id].rx_num_bursts_in_block == 1 )
      {
        dl_ptcch_msg_info_glb[gas_id].rx_data_count = FRAMES_FROM_BURST_TO_RX_DATA;
      }

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
      /* Set the timeslot for IQ samples logging */
      gl1_hw_set_iq_samples_tn( GL1_HW_PTCCHD, ptcch_params[gas_id].signal_info.tn, gas_id );
#endif  /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */

      /* Fill in the dynamic bursts information */
      ptcch_params[gas_id].burst_info.arfcn        = ptcch_params[gas_id].arfcn[ dl_ptcch_msg_info_glb[gas_id].burst_num ];
      ptcch_params[gas_id].burst_info.decode_flag  = ( dl_ptcch_msg_info_glb[gas_id].burst_num == 3 ? TRUE : FALSE);
      ptcch_params[gas_id].burst_info.burst_num    = dl_ptcch_msg_info_glb[gas_id].burst_num;
      ptcch_params[gas_id].signal_info.rx_signal_strength =
            sys_algo_agc_gprs_expected_pwr(ptcch_params[gas_id].agc_ptr->pwr_dBm_x16, GL1_HW_PTCCHD,gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
      ptcch_params[gas_id].signal_info.rx_signal_strength_divrx =
            sys_algo_agc_gprs_expected_pwr(ptcch_params[gas_id].agc_ptr_divrx->pwr_dBm_x16, GL1_HW_PTCCHD,gas_id);

      is_block_start = (ptcch_params[gas_id].burst_info.burst_num == 0) ? TRUE : FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/

      gl1_hw_rx_dtm_gprs
       (
         &ptcch_params[gas_id].signal_info,
         NUM_TN_TO_RX_PER_FRAME,
         &ptcch_params[gas_id].burst_info,
         &ptcch_params[gas_id].tags[dl_ptcch_msg_info_glb[gas_id].burst_num],
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
         is_block_start,
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
         gas_id
       );

      /* Save the TN we used for checking metrics and data in case it changes */
      dl_ptcch_msg_info_glb[gas_id].rx_tn = ptcch_params[gas_id].signal_info.tn;
      dl_ptcch_msg_info_glb[gas_id].camp_tn = gl1_msg_pdch_get_camp_tn(gas_id);

      dl_ptcch_msg_info_glb[gas_id].rx_num_bursts_in_block--;
   }

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
   if ( dl_ptcch_msg_info_glb[gas_id].frame_count == FRAMES_FROM_BURST_TO_RX_LOG )
   {
      gl1_hw_get_iq_samples( GL1_HW_PTCCHD, 2, ptcch_params[gas_id].burst_info.burst_num,
                             ptcch_params[gas_id].burst_info.tsc, gas_id );
   }
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */

   if ( dl_ptcch_msg_info_glb[gas_id].frame_count == FRAMES_FROM_BURST_TO_RX_METRICS )
   {
      gl1_defs_rx_pkt_metrics_type *metrics_info =
        ptcch_ptr_info[gas_id].metrics_data_mem[ dl_ptcch_msg_info_glb[gas_id].burst_num ];

      /* Set the known metrics fields - tn is used in gl1_hw_get_rx_metrics */
      metrics_info->arfcn = ptcch_params[gas_id].arfcn[ dl_ptcch_msg_info_glb[gas_id].burst_num ];
      metrics_info->tn = dl_ptcch_msg_info_glb[gas_id].rx_tn;
      metrics_info->camp_tn = dl_ptcch_msg_info_glb[gas_id].camp_tn;
      metrics_info->offset_to_burst = 2;

      gl1_hw_get_rx_metrics
       (
         &metrics_info,
         NUM_TN_TO_RX_PER_FRAME,
         FALSE,  /* No USF info */
         FALSE,  /* No hard decisions */
         &ptcch_params[gas_id].tags[ dl_ptcch_msg_info_glb[gas_id].burst_num ],
#ifdef GERAN_L1_ENHANCED_RX
         GL1_MSG_PS_PTCCH,
#endif /*GERAN_L1_ENHANCED_RX*/
         gas_id
       );

      /* Check that the burst was scheduled */
      if ((metrics_info->schedule_status & GL1_DEFS_RX_SCHEDULED) == 0)
      {
         MSG_4(MSG_SSID_DFLT, MSG_LEGACY_ERROR,
               "Rx PTCCH not scheduled: tn=%d (%d), reason=%04x, FN=%d",
               metrics_info->tn, metrics_info->camp_tn,
               metrics_info->schedule_status, GSTMR_GET_FN_GERAN(gas_id));
      }

      if (gl1_msg_saic_in_burst(metrics_info->misc_flags))
      {
        ptcch_burst_snr = metrics_info->pre_mimo_trained_complete_snr;
      }
      else
      {
        ptcch_burst_snr = metrics_info->snr;
      }

      /* Now check against SNR threshold of 1dB to update AGC */
      if(ptcch_burst_snr > PTCCH_SNR_THRESHOLD)
      {
        /* Send metrics to AGC */
        sys_algo_gprs_agc_update
        (
          ptcch_params[gas_id].agc_ptr,
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA)
          ptcch_params[gas_id].agc_ptr_divrx, 
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA */
          &metrics_info,
          NUM_TN_TO_RX_PER_FRAME,
          GL1_HW_PTCCHD,
          time_stamp,
          TRUE, /* for PTCCH this is ignored */
          gas_id
        );
      }
      else
      {
        MSG_GERAN_HIGH_1_G("SNR too low, AGC not updated %d",ptcch_burst_snr);
      }


      if (rx_pkt_cb[gas_id].metrics_callback)
      {
         rx_pkt_cb[gas_id].metrics_callback
          (
            &metrics_info,
            NUM_TN_TO_RX_PER_FRAME,
            dl_ptcch_msg_info_glb[gas_id].burst_num,
            gas_id
          );
      }

      if ( rx_pkt_cb[gas_id].log_callback )
      {
         rx_pkt_cb[gas_id].log_callback(dl_ptcch_msg_info_glb[gas_id].burst_num);
      }
   }

   if (dl_ptcch_msg_info_glb[gas_id].rx_data_count != 0)
   {
      boolean data_available = FALSE;

      dl_ptcch_msg_info_glb[gas_id].rx_data_count--;

      if (dl_ptcch_msg_info_glb[gas_id].rx_data_count == 0)
      {
         /* time to get the data */
         data_available = gl1_hw_get_rx_data
          (
            TRUE,   /* PTCCH burst */
            dl_ptcch_msg_info_glb[gas_id].rx_tn,
            &ptcch_params[gas_id].tags[DECODE_TAG],
            TRUE,  /* only one attempt to find the data */
            ptcch_ptr_info[gas_id].msg_data_mem,
            gas_id
          );


         /* Set the offset-to-start-of-burst and timeslot */
         ptcch_ptr_info[gas_id].msg_data_mem->offset_to_sob =
           (NUM_BURSTS_TO_RECEIVE - 1) * NUM_FRAMES_BW_BURSTS
            + FRAMES_FROM_BURST_TO_RX_DATA;
         ptcch_ptr_info[gas_id].msg_data_mem->tn = dl_ptcch_msg_info_glb[gas_id].rx_tn;
         ptcch_ptr_info[gas_id].msg_data_mem->camp_tn = dl_ptcch_msg_info_glb[gas_id].camp_tn;

         /* Make the data callback if it exists */
         if (rx_pkt_cb[gas_id].data_callback)
         {
            rx_pkt_cb[gas_id].data_callback(ptcch_ptr_info[gas_id].msg_data_mem, data_available,gas_id);
         }

         /* Finished processing, so remove the handler */
         OPEN_RX_WINDOW(gas_id);
         CLEAR_RX_CMD_ISSUED_FLAG(gas_id);
         ALLOW_RX_HDLR_TO_BE_REG(gas_id);

         dl_ptcch_msg_info_glb[gas_id].proc_in_progress = FALSE;
         remove_hdlr                                   = TRUE;
         return remove_hdlr;
      }
   }

   dl_ptcch_msg_info_glb[gas_id].frame_count++;
   if ( dl_ptcch_msg_info_glb[gas_id].frame_count == NUM_FRAMES_BW_BURSTS )
   {
      dl_ptcch_msg_info_glb[gas_id].frame_count = 0;
   }

   return remove_hdlr;

}
/*===========================================================================

FUNCTION gl1_msgi_ptcch_open_rx_win

DESCRIPTION
   This function open rx window for PTCCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_ptcch_open_rx_win( gas_id_t gas_id )
{
    OPEN_RX_WINDOW(gas_id);
}

