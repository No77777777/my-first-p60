/*===========================================================================

     IMPLEMENTATION OF GPRS NPL1 UPLINK PTCCH FUNCTIONS

GENERAL DESCRIPTION
   This file implements GPRS NPL1 UPLINK PTCCH functions.

   Some PTCCH points of consideration are:
   1. The processing is based on a block period consisting of 104 frames.
   Messages may be submitted only in Frame 2 and 3 of a 4 frame block period.
   In the fourth frame of the block, if there are messages to be sent then a
   single transmit burst is scheduled.

   2. The transmitted data notification callback is invoked 2 frames after the
   burst is sent.



EXTERNALIZED FUNCTIONS

gl1_msgi_ul_ptcch_init
gl1_msgi_ul_ptcch_terminate


INITIALIZATION AND SEQUENCING REQUIREMENTS
The following is the desired order in which functions should be called

gl1_msgi_ul_ptcch_init
gl1_msgi_ul_ptcch_terminate

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_ul_ptcch.c#2 $
$DateTime: 2019/06/07 00:32:11 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
07/06/19   rks      CR2464129 Resolving compilation issues for new compiler upgrade on HA1.0
17/08/15   jj       CR864724 prevent two set tx band in Consecutive fram
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
19/03/15   shr      CR806808 0x5237 GPRS Tx Schedule to log OTA TN rather than slammed TN
15/08/13   sk       CR524039 GBTA bringup changes
29/07/13   sk       CR519663 Partial QBTA bringup changes
21/05/13   cs       TSTS Updates
13/03/13   mc       CR.459487 : COEX Support - GERAN arbitrator framework
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
26/03/09   ws       Added PACKED_POST for QDSP6 targets
05/03/07   ws       Resolved filtered lint warnings
31/07/06   agv      CR85810 NB AB TSC, and TA fix.
16/03/06   og       Take timeslot gaps into account when deciding on Tta/Trb
                    or Ttb/Tra. Resolves CR 88935 and CR 88975.
02/17/06   gfr      Added modulation for power backoff to RF Driver.
11/29/05   gfr      Cast log packets to correct types to fix compiler warning
11/22/05   gfr      Change tx scheduled and discarded callbacks to take
                    structures instead of just message data pointers.  Return
                    the timeslot and offset_to_sob for scheduled transmits.
07/29/05   gfr      DTM support
06/02/05   gfr      Make handler static
04/12/05   gfr      Consolidate GSM hw layer parameters
04/08/05   gfr      Use new interface to specify saving of untxed messages
04/08/05   gfr      Combined GSM and GPRS channel types
04/07/05   gfr      Remove reference to "uint"
03/22/05   gfr      Use tx tag to keep track of encoded data.
07/22/04   gfr      EGPRS support.
03/05/04   gfr      Lint cleanup.
02/26/04   gfr      Added missing arfcn_to_word prototype.
01/27/04   gfr      Support for logging tx scheduled results.
11/05/03   gfr      Support for quad-band.
05/01/03   gfr      Change featuring from FEATURE_GSM to FEATURE_GSM_GPRS
02/24/03    gfr     Lint cleanup.
10/10/02    gr      Changed GPRS TX interface to accept TXLEV vs. power index.
07/16/02    gr      Set timing advance to 0 for access bursts
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
04/16/02    bk      Made changes based on discarded cb i/f change.
02/09/02    bk      Changed MSG_HIGH to MSG_LOW
01/23/02    bk      Made changes based on the tx_burst i/f change in the Frm layer
10/29/01    bk      Initial Revision

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

/* header files to support GPRS NPL1 channel operations */
#include "gl1_msg_pdch.h"

#include "gl1_msgi.h"

#include "msg.h" /* Error/Output */
#include "gtmrs.h"
#include "gtmrs_g.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
#define  FRAMES_AFTER_BURST_TO_INVOKE_TX_CB 2


typedef struct
{
   gl1_hw_tx_signal_type     signal_info;
   gl1_hw_tx_pkt_burst_type  burst_info;
   gl1_hw_tx_tags_type       tags;
   uint8                     *msg_data;
   gl1_defs_tn_type          camp_tn;
} ptcch_params_type;


typedef struct {

  ptcch_params_type       ptcch_params[ NUM_SETS_OF_PARAMS ];

  gl1_msg_tx_pkt_cb_type  tx_pkt_cb;
  gl1_defs_coding_type    chn_coding_scheme;
  boolean                 burst_is_to_be_sent;
  uint8                   frame_to_invoke_cb;
}gl1_msg_ptcch_params_type;

static gl1_msg_ptcch_params_type ul_ptcch_params[NUM_GERAN_DATA_SPACES];


static  boolean  reg_tx_hdlr_flag[NUM_GERAN_DATA_SPACES] =
                   { INITIAL_VALUE( FALSE ) };

static  boolean  tx_window_open[NUM_GERAN_DATA_SPACES] =
                   { INITIAL_VALUE( TRUE ) };

static void gl1_msgi_log_tx_scheduled_results_ptcch(gas_id_t  gas_id);

/* This is actually defined in GSM L1 - slight layering violation */
extern uint16 arfcn_to_word(ARFCN_T arfcn);

extern uint8 gpl1_gprs_ts_conv_unslam(uint8 slammed, gas_id_t gas_id );

static boolean gl1_msgi_ul_ptcch_hdlr
(
   boolean   snext_frame_is_ul_ptcch_sacch_block_start,
   uint32    time_stamp,
   gas_id_t  gas_id
);


/*===========================================================================

FUNCTION gl1_msgi_ul_ptcch_init

DESCRIPTION
   This function initializes data structures that are used by all PDCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_ul_ptcch_init( gas_id_t  gas_id )
{
   uint8 param_set;

   /* Initialize both the parameter sets */
   for (param_set = 0; param_set < (uint8)NUM_SETS_OF_PARAMS; param_set++ )
   {
      memset(&ul_ptcch_params[gas_id].ptcch_params[ param_set ].burst_info, 0,
             sizeof(ul_ptcch_params[gas_id].ptcch_params[ param_set ].burst_info));
      memset(&ul_ptcch_params[gas_id].ptcch_params[ param_set ].signal_info, 0,
             sizeof(ul_ptcch_params[gas_id].ptcch_params[ param_set ].signal_info));
      ul_ptcch_params[gas_id].ptcch_params[ param_set ].burst_info.encode_flag = TRUE;
      ul_ptcch_params[gas_id].ptcch_params[ param_set ].signal_info.modulation = GL1_DEFS_MODULATION_GMSK;
   }

   ul_ptcch_params[gas_id].tx_pkt_cb.discarded_callback = 0;
   ul_ptcch_params[gas_id].tx_pkt_cb.scheduled_callback = 0;
   ul_ptcch_params[gas_id].chn_coding_scheme            = GL1_DEFS_ACCESS_CODING;
   ul_ptcch_params[gas_id].burst_is_to_be_sent          = FALSE;
   ul_ptcch_params[gas_id].frame_to_invoke_cb           = 0;

   OPEN_TX_WINDOW(gas_id);
   ALLOW_TX_HDLR_TO_BE_REG(gas_id);

}


/*===========================================================================

FUNCTION gl1_msgi_ul_ptcch_terminate

DESCRIPTION
   Terminates all common PDCH processing

===========================================================================*/
void gl1_msgi_ul_ptcch_terminate( gas_id_t  gas_id )
{
   /* Reset internal variables */
   ul_ptcch_params[gas_id].tx_pkt_cb.discarded_callback = NULL;
   ul_ptcch_params[gas_id].tx_pkt_cb.scheduled_callback = NULL;
   ul_ptcch_params[gas_id].burst_is_to_be_sent          = FALSE;
   ul_ptcch_params[gas_id].frame_to_invoke_cb           = 0;

   /* Terminate all common PDCH processing*/
   if (IS_TX_HDLR_REG(gas_id))
   {
      gl1_msgi_remove_ft_handler( gl1_msgi_ul_ptcch_hdlr ,gas_id);
   }

   CLOSE_TX_WINDOW(gas_id);
   DONT_ALLOW_TX_HDLR_TO_BE_REG(gas_id);
}



/*=============================================================================
FUNCTION gl1_msg_cfg_ul_ptcch

DESCRIPTION
   PACCH and PDTCH are configured and ready to transmit data.


DEPENDENCIES
   The message layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_cfg_ul_ptcch
(
    gl1_defs_tn_type              tn,
    gl1_defs_coding_type          coding_scheme,
    uint8                         bsic,
    const gl1_msg_tx_pkt_cb_type *tx_msg_cb_info,
    gas_id_t                      gas_id
)
{
    /* Store the specified parameters */
    /* Always 1 burst */
    ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ].burst_info.burst_num = 0;
    ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ].burst_info.bsic = bsic;
    ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ].signal_info.tn  = tn;
    ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ].signal_info.priority  =
      GPDU_HIGH_PRIORITY;

    /* Added for NB/AB compatibility. Always 0 for PTCCH. */
    ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ].burst_info.ab_tsc = TS0;
    ul_ptcch_params[gas_id].tx_pkt_cb.discarded_callback = tx_msg_cb_info->discarded_callback;
    ul_ptcch_params[gas_id].tx_pkt_cb.scheduled_callback = tx_msg_cb_info->scheduled_callback;
    ul_ptcch_params[gas_id].chn_coding_scheme            = coding_scheme;
}


/*=============================================================================
FUNCTION gl1_msg_tx_ptcch

DESCRIPTION
   Write PTCCH data and schedule an encode and transmit of the data at the
   configured power and frequency. The PTCCH data is transmitted as a single
   access burst.

   This function is to be called anywhere in frame 2 or 3 of the previous block.
   The transmitted burst is scheduled in the frame following the end of the
   previous block boundary.

   Note that if this function fails due to the mDSP message buffers being full
   then the discarded message callback is invoked.

DEPENDENCIES
   The channel must be configured.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_tx_ptcch
(
    uint8    *msg_data,
    int16    TXLEV,
    ARFCN_T  arfcn,
    gas_id_t gas_id
)
{
   uint8                msg_len;
   gl1_hw_channel_type  chan_type;
   boolean              write_ok;
   gl1_defs_tx_pkt_data_type msg;
   gl1_msg_tx_discarded_data_type discarded_data;

   if ( IS_TX_WINDOW_OPEN(gas_id) == FALSE )
   {
      /* submitted messages in frames other than 2 and 3 */
      MSG_GERAN_ERROR_0_G("Submitted message while tx window was closed\n");

      /* Discard the submitted message */
      if (ul_ptcch_params[gas_id].tx_pkt_cb.discarded_callback)
      {
         discarded_data.msg_data = msg_data;
         discarded_data.offset_to_sob = 0;
         ul_ptcch_params[gas_id].tx_pkt_cb.discarded_callback(&discarded_data, 1,gas_id);
      }

      return;
   }

   /*---------------------------------------------------------------------------
   * First check if the handler is installed, if not then install the handler.
   * and dont allow any further handlers to be registered
   ---------------------------------------------------------------------------*/
   if (!IS_TX_HDLR_REG(gas_id))
   {
      if (gl1_hw_get_rf_gsm_band(gas_id)== ((int8)(RFGSM_BAND_NONE))) 
      { 
        gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(arfcn),gas_id); 
      } 
      gl1_msgi_add_ft_handler(gl1_msgi_ul_ptcch_hdlr, GL1_MSG_FT_HDLR_UL_PTCCH_SACCH,gas_id);
      DONT_ALLOW_TX_HDLR_TO_BE_REG(gas_id);
   }

   if ( ul_ptcch_params[gas_id].chn_coding_scheme == GL1_DEFS_ACCESS_CODING )
   {
      msg_len   = GL1_DEFS_ACCESS_BYTES;
      chan_type = GL1_HW_PTCCHU_ACC;
   }
   else
   {
      msg_len   = GL1_DEFS_EXT_ACCESS_BYTES;
      chan_type = GL1_HW_PTCCHU_EXT;
   }

   /* Fill in the tx pkt data structure */
   msg.gprs.data = msg_data;
   ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ].msg_data = msg_data;

   /* Fill in channel type associated with this burst */
   ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ].signal_info.channel_type = chan_type;

   /*--------------------------------------------------------------------------
   * For the received message,store the frequency,bsic and signal str.
   -------------------------------------------------------------------------- */
   ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ].burst_info.arfcn = arfcn;

   /* Convert from TXLEV to a power index */
   gl1_msgi_map_txlev_to_pwr_idx
   (
     arfcn,
     &TXLEV,
     &ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ].signal_info.tx_power_index,
     1,
     gas_id
   );

   /* Timing advance is 0 for access bursts */
   ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ].burst_info.timing_advance = 0;

   ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ].burst_info.channel_type = chan_type;

   /*---------------------------------------------------------------------------
   * Submit the message to the frame layer. Note that the parameters passed in
   * here are the parameters that have been last been configured.
   *---------------------------------------------------------------------------*/
   write_ok = gl1_hw_write_tx_pkt_data
    (
      &msg,
      msg_len,
      ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ].signal_info.tn,
      GL1_HW_PDCH,
      chan_type,
      &ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ].tags,
      gas_id
    );

   if (!write_ok && ul_ptcch_params[gas_id].tx_pkt_cb.discarded_callback)
   {
      discarded_data.msg_data = msg_data;
      discarded_data.offset_to_sob = 0;
      ul_ptcch_params[gas_id].tx_pkt_cb.discarded_callback(&discarded_data, 1,gas_id);
   }

}


/*=============================================================================
FUNCTION  gl1_msgi_ul_ptcch_hdlr

DESCRIPTION
   When this handler is registered, it is invoked every frame until the handler
   decides to deregister itself.

   The handler does nothing until the frame before the start of the PTCCH block
   boundary. In that frame, the frame layer is commanded to encode the submitted
   data and send a burst. A timer is started. The timer expires two frames later
   and a transmitted notification callback is sent and the handler is
   deregistered.


DEPENDENCIES
None

RETURN VALUE
   'TRUE' - do not deregister the handler
   'FALSE' - deregister the handler

SIDE EFFECTS
   None
============================================================================= */
static boolean gl1_msgi_ul_ptcch_hdlr
(
   boolean next_frame_is_ul_ptcch_sacch_block_start,
   uint32  time_stamp,
   gas_id_t  gas_id
)
{
   boolean remove_hdlr = FALSE;
   ptcch_params_type *params;
   gl1_msg_tx_scheduled_data_type sched_data;
   gl1_msg_tx_discarded_data_type discarded_data;

   /*---------------------------------------------------------------------------
   * Processing to be done in the a frame before the PTCCH block.
   * The following processing is done
     - The message submission window is closed.
     - Indicate that a burst is to be sent
     - Take a snapshot of the configured parameters.
   ---------------------------------------------------------------------------*/
   if( next_frame_is_ul_ptcch_sacch_block_start )
   {
      /* At the 4th frame of the block, close the window for msg submission */
      CLOSE_TX_WINDOW(gas_id);

      /*Indicate that a burst is to be  sent this frame */
      ul_ptcch_params[gas_id].burst_is_to_be_sent = TRUE;

      /* Take a snapshot of the configured parameters */
      ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_THIS_BLOCK ] =
      ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_NEXT_BLOCK ];

      /* Latch the camp tn */
      ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_THIS_BLOCK ].camp_tn = gl1_msg_pdch_get_camp_tn(gas_id);
   }

   /* Parameter shortcut */
   params = &ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_THIS_BLOCK ];

   /* Process if it is time to invoke the tx notification callback */
   if ( ul_ptcch_params[gas_id].frame_to_invoke_cb )
   {
       --ul_ptcch_params[gas_id].frame_to_invoke_cb;

       /*-----------------------------------------------------------------------
         The timer has expired, invoke the callback. Once the callback is
         invoked the handler can be removed.
       -----------------------------------------------------------------------*/
       if ( !ul_ptcch_params[gas_id].frame_to_invoke_cb )
       {
          /* Get schedule, discard any untransmitted data */
          if (gl1_hw_get_tx_scheduled_result(&params->tags, TRUE,gas_id))
          {
             if (ul_ptcch_params[gas_id].tx_pkt_cb.scheduled_callback)
             {
                sched_data.msg_data = params->msg_data;
                sched_data.tn = params->signal_info.tn;
                sched_data.camp_tn = params->camp_tn;
                sched_data.offset_to_sob = FRAMES_AFTER_BURST_TO_INVOKE_TX_CB - 1;
                ul_ptcch_params[gas_id].tx_pkt_cb.scheduled_callback(&sched_data, 1,gas_id);
             }
             gl1_msgi_log_tx_scheduled_results_ptcch(gas_id);
          }
          else
          {
             MSG_GERAN_ERROR_1_G("PTCCHU discarded, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
             if (ul_ptcch_params[gas_id].tx_pkt_cb.discarded_callback)
             {
                discarded_data.msg_data = params->msg_data;
                discarded_data.offset_to_sob = FRAMES_AFTER_BURST_TO_INVOKE_TX_CB - 1;
                ul_ptcch_params[gas_id].tx_pkt_cb.discarded_callback(&discarded_data, 1,gas_id);
             }
          }

          /*--------------------------------------------------------------------
            Open message submission window and remove this handler. Removing the
            handler would result this function not being called anymore.
            Allow this handler to be registered in the future
          --------------------------------------------------------------------*/
          OPEN_TX_WINDOW(gas_id);
          ALLOW_TX_HDLR_TO_BE_REG(gas_id);

          MSG_GERAN_LOW_0_G("Removing PTCCH/U handler\n");

          remove_hdlr = TRUE;
          return remove_hdlr;
       }
   }


   /*---------------------------------------------------------------------------
   * The following block of code does the following:
   * - Checks if there a burst needs to be sent in this block
   * - Indicate that a tx notification is to be sent 2 frames later.
   ----------------------------------------------------------------------------*/
   if ( ul_ptcch_params[gas_id].burst_is_to_be_sent )
   {
      /* Retrieve the parameters and transmit the burst */
      gl1_hw_tx_dtm_gprs
       (
         &params->signal_info,
         1,   /* 1 timeslot */
         &params->burst_info,
         &params->tags,
         1,    /* 1 message */
         1,
         gas_id
       );

      /* Record that the burst has been sent */
      ul_ptcch_params[gas_id].burst_is_to_be_sent = FALSE;

      /*-----------------------------------------------------------------------
         Initialize a timer, at the end of which, invoke the tx notification
         callback
      ------------------------------------------------------------------------*/
      ul_ptcch_params[gas_id].frame_to_invoke_cb = FRAMES_AFTER_BURST_TO_INVOKE_TX_CB;
   }

   return remove_hdlr;

}



/*===========================================================================

FUNCTION gl1_msgi_log_tx_scheduled_results_ptcch

DESCRIPTION
   This function logs the tx schedule.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_msgi_log_tx_scheduled_results_ptcch (gas_id_t  gas_id)
{
   LOG_GPRS_TX_SCHEDULED_RESULTS_C_type *log_pkt;
   uint32 size;
   ptcch_params_type *params;
   uint32  current_FN = GSTMR_GET_FN_GERAN(gas_id);

   /* Calculate how much space we need */
   size = sizeof(LOG_GPRS_TX_SCHEDULED_RESULTS_C_type)
           - (GL1_DEFS_MAX_ASSIGNED_UL_TS - 1)
             * sizeof(log_gprs_tx_scheduled_result_type);

   /* Try to allocate memory for the log packet */
   log_pkt = (LOG_GPRS_TX_SCHEDULED_RESULTS_C_type *)
              log_alloc(LOG_GPRS_TX_SCHEDULED_RESULTS_C, size);
   if (log_pkt == NULL) return;

   /* Shortcut */
   params = &ul_ptcch_params[gas_id].ptcch_params[ PARAMS_TO_USE_THIS_BLOCK ];

   /* Fill in the log info */
   log_pkt->num_msg_tx = 1;
   log_pkt->FN = SUB_FN(current_FN,1);
   log_pkt->msg_info[0].tn = gpl1_gprs_ts_conv_unslam((uint8)params->signal_info.tn, gas_id);
   log_pkt->msg_info[0].power_index = params->signal_info.tx_power_index;
   log_pkt->msg_info[0].timing_advance = 0;
   log_pkt->msg_info[0].dynamic_alloc = FALSE;
   log_pkt->msg_info[0].coding_scheme = ul_ptcch_params[gas_id].chn_coding_scheme;
   log_pkt->msg_info[0].msg_type = GL1_MSG_UL_PTCCH;
   log_pkt->arfcn[0] = arfcn_to_word(params->burst_info.arfcn);
   log_pkt->arfcn[1] = log_pkt->arfcn[2] = log_pkt->arfcn[3] = 0;

   /* Send the log packet */
   log_commit((void *)log_pkt);
}





