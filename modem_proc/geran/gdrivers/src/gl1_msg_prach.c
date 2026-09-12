/*===========================================================================

     IMPLEMENTATION OF GPRS NPL1 PRACH FUNCTIONS

GENERAL DESCRIPTION
   This file implements GPRS NPL1 PRACH functions.

   Some PRACH points of consideration are:
   1. The processing is based on a block period consisting of 4 frames. Messages
   may be submitted only in Frame 2 and 3 of a block. In the fourth frame of the
   block, if there are messages to be sent then a single transmit burst is
   scheduled. If burst may be transmitted at some offset (in frames ) from the
   block boundary.

   2.Dynamic allocation adds some additional complexity since submitted messages
   may not always be transmitted next block. Transmission is dependent on
   whether an USF was decoded in the previous block.

   3. The transmitted data notification callback is invoked 2 frames after the
   burst is sent.


EXTERNALIZED FUNCTIONS

gl1_msgi_prach_init
gl1_msgi_prach_terminate


INITIALIZATION AND SEQUENCING REQUIREMENTS
The following is the desired order in which functions should be called

gl1_msgi_prach_init
gl1_msgi_prach_terminate

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_prach.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
15/08/13   sk       CR524039 GBTA bringup changes
21/05/13   cs       TSTS Updates
13/03/13   mc       CR.459487 : COEX Support - GERAN arbitrator framework
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
26/03/09   ws       Added PACKED_POST for QDSP6 targets
05/03/07   ws       Resolved filtered lint warnings
31/07/06   agv      CR85810 NB AB TSC, and TA fix.
16/03/06   og       Take timeslot gaps into account when deciding on Tta/Trb
                    or Ttb/Tra. Resolves CR 88935 and CR 88975.
02/17/06   gfr      Added modulation for power backoff to RF driver.
11/29/05   gfr      Cast log packets to correct types to fix compiler warning
11/22/05   gfr      Change tx scheduled and discarded callbacks to take
                    structures instead of just message data pointers.  Return
                    the timeslot and offset_to_sob for scheduled transmits.
08/03/05   gfr      Do not remove handler while executing the handler
07/29/05   gfr      DTM support
06/02/05   gfr      Make handler static
04/12/05   gfr      Consolidate GSM hw layer parameters
04/08/05   gfr      Use new interface to specify saving of untxed messages
04/08/05   gfr      Combined GSM and GPRS channel types
04/07/05   gfr      Remove reference to "uint"
03/30/05   gfr      Return the msg data pointer even if it was not txed.
03/22/05   gfr      Use tx tag to keep track of encoded data.
11/08/04   gfr      Error check that non-zero TSC is sent as ext access.
07/22/04   gfr      EGPRS support.
03/05/04   gfr      Lint cleanup.
02/26/04   gfr      Added missing arfcn_to_word prototype.
01/27/04   gfr      Support for logging tx scheduled results.
11/05/03   gfr      Support for quad-band.
07/11/03   gfr      Correctly use the new arfcn in the next block.
05/12/03   gfr      Remove burst_schedule code.
05/01/03   gfr      Change featuring from FEATURE_GSM to FEATURE_GSM_GPRS
04/04/03   gfr      Pass the timing advance to gl1_msgi_burst_schedule
02/24/03   gfr      Lint cleanup.
02/19/03   gfr      Update the arfcn for retransmits correctly.
01/15/03   gfr      Use new burst indication function.
10/10/02    gr      Changed GPRS TX interface to accept TXLEV vs. power index.
09/06/02    gr      Abort EXT_ACCESS or ACCESS as required.
07/16/02    gr      Set timing advance to 0 for access bursts
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
04/16/02    bk      Added abort functionality.
02/09/02    bk      Changed MSG_HIGH to MSG_LOW
01/23/02    bk      Made changes based on the tx_burst i/f change in the Frm layer
01/15/02    bk/sg   Added support for monitoring and dynamic PRACH
12/12/01    bk      Initialized channel to indicate it is not a PTCCH burst
11/09/01    bk      Added output error messages
11/2/01     bk      Hacked the code to indicate the channels is a PTCCH burst
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
#define FRAMES_AFTER_BURST_TO_INVOKE_TX_CB 2

static  boolean  reg_tx_hdlr_flag[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

static  boolean  tx_window_open[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( TRUE ) };

static  boolean  submitted_tx_msg[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

typedef struct
{
   gl1_hw_tx_signal_type     signal_info;
   gl1_hw_tx_pkt_burst_type  burst_info;
   int16                     TXLEV;
   gl1_hw_tx_tags_type       tags;
   uint8                     *msg_data;
   gl1_defs_tn_type          camp_tn;
} prach_params_type;

static prach_params_type prach_params[ NUM_SETS_OF_PARAMS ];

static gl1_hw_pkt_msg_type     msg_type;
static gl1_defs_coding_type    chn_coding_scheme;
static gl1_msg_tx_pkt_cb_type  tx_pkt_cb;

static boolean burst_is_to_be_sent = FALSE;
static uint8   frame_to_invoke_cb  = 0;
static boolean retransmit_data     = FALSE;
static boolean abort_prach         = FALSE;

static ARFCN_T  retransmit_arfcn;

/* Prototypes of private functions */
static void gl1_msgi_reset_prach_sm( void );
static void gl1_msgi_process_abort_prach( void );
static void gl1_msgi_log_tx_scheduled_results_prach( void );

/* This is actually defined in GSM L1 - slight layering violation */
extern uint16 arfcn_to_word(ARFCN_T arfcn);

static boolean gl1_msgi_prach_hdlr
(
   boolean next_frame_is_chn_ul_block_boundary,
   uint32  time_stamp,
   gas_id_t gas_id
);


/*===========================================================================

FUNCTION gl1_msgi_prach_init

DESCRIPTION
   This function initializes data structures that are used by all PDCH.

===========================================================================*/
void gl1_msgi_prach_init( void )
{
   uint8 param_set;

   /* Initialize both the parameter sets */
   for (param_set = 0; param_set < (uint8)NUM_SETS_OF_PARAMS; param_set++ )
   {
      memset(&prach_params[ param_set ].burst_info, 0,
             sizeof(prach_params[ param_set ].burst_info));
      memset(&prach_params[ param_set ].signal_info, 0,
             sizeof(prach_params[ param_set ].signal_info));
      prach_params[ param_set ].burst_info.encode_flag = TRUE;
      /* Modulation for power backoff. */
      prach_params[ param_set ].signal_info.modulation = GL1_DEFS_MODULATION_GMSK;
   }

   tx_pkt_cb.discarded_callback = 0;
   tx_pkt_cb.scheduled_callback = 0;
   msg_type                     = GL1_HW_PDCH;
   chn_coding_scheme            = GL1_DEFS_ACCESS_CODING;
   burst_is_to_be_sent          = FALSE;
   frame_to_invoke_cb           = 0;
   retransmit_data              = FALSE;
   abort_prach                  = FALSE;
   retransmit_arfcn             = NULL_ARFCN;

   OPEN_TX_WINDOW(GERAN_ACCESS_STRATUM_ID_1);
   CLEAR_MSGS_SUBMITTED_FLAG(GERAN_ACCESS_STRATUM_ID_1);
   ALLOW_TX_HDLR_TO_BE_REG(GERAN_ACCESS_STRATUM_ID_1);
}


/*===========================================================================

FUNCTION gl1_msgi_prach_terminate

DESCRIPTION
   Terminates all common PDCH processing

===========================================================================*/
void gl1_msgi_prach_terminate( void )
{
   /* Reset internal variables */
   tx_pkt_cb.discarded_callback = NULL;
   tx_pkt_cb.scheduled_callback = NULL;

   /* Reset SM variables */
   gl1_msgi_reset_prach_sm();

   /* Remove handler */
   if (IS_TX_HDLR_REG(GERAN_ACCESS_STRATUM_ID_1))
   {
      gl1_msgi_remove_ft_handler( gl1_msgi_prach_hdlr,GERAN_ACCESS_STRATUM_ID_1 );
      ALLOW_TX_HDLR_TO_BE_REG(GERAN_ACCESS_STRATUM_ID_1);
   }
}



/*=============================================================================
FUNCTION gl1_msg_cfg_prach

DESCRIPTION
   PACCH and PDTCH are configured and ready to transmit data. Note: To avoid
   confusion the tsc defined here is not the NB TSC, but the AB Sync Seq Bits
   as per 3GPP 45.002, Clause 5.2.7.



DEPENDENCIES
   The message layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_cfg_prach
(
  gl1_defs_tn_type               tn,
  int16                          TXLEV,
  gl1_defs_coding_type           coding_scheme,
  uint8                          ab_tsc, /* Access Burst TSC */
  uint8                          bsic,
  boolean                        usf_enabled,
  const gl1_msg_tx_pkt_cb_type  *tx_msg_cb_info

)
{
    /* Store the specified parameters */
    /* AB TSC Sync Seq Bits; 3GPP 45.002, Clause 5.2.7. */
    /* Always 1 burst */
    prach_params[ PARAMS_TO_USE_NEXT_BLOCK ].burst_info.burst_num = 0;
    prach_params[ PARAMS_TO_USE_NEXT_BLOCK ].burst_info.ab_tsc = ab_tsc;
    prach_params[ PARAMS_TO_USE_NEXT_BLOCK ].burst_info.bsic  = bsic;
    prach_params[ PARAMS_TO_USE_NEXT_BLOCK ].signal_info.tn   = tn;
    prach_params[ PARAMS_TO_USE_NEXT_BLOCK ].TXLEV = TXLEV;

    /* check whether USF is enabled */
    if ( usf_enabled )
    {
       gl1_defs_rx_pkt_alloc_type usf_info;
       usf_info.tn  = tn;
       usf_info.usf = GL1_DEFS_USF_FREE;
       msg_type     = GL1_HW_USF;
       gl1_hw_set_usf_list( &usf_info, 1,GERAN_ACCESS_STRATUM_ID_1 );
    }
    else
    {
       msg_type = GL1_HW_PDCH;
    }

    /* Non-zero training sequences must be sent as extended access */
    /* AB TSC Sync Seq Bits; Reference  3GPP 45.002, Clause 5.2.7. */
    if (ab_tsc != 0 && coding_scheme != GL1_DEFS_EXT_ACCESS_CODING)
    {
       MSG_GERAN_ERROR_1("AB_TSC=%d requires extended access coding!", ab_tsc);
       coding_scheme = GL1_DEFS_EXT_ACCESS_CODING;
    }

    tx_pkt_cb.discarded_callback = tx_msg_cb_info->discarded_callback;
    tx_pkt_cb.scheduled_callback = tx_msg_cb_info->scheduled_callback;
    chn_coding_scheme            = coding_scheme;

}



/*=============================================================================
FUNCTION gl1_msg_tx_prach

DESCRIPTION
   Write PRACH data and schedule an encode and transmit of the data at the
   configured power and frequency. The PRACH data is transmitted as a single
   access burst.

   This function is to be called anywhere in frame 2 or 3 of the previous block.
   The first transmitted burst is scheduled at the specified offset from the
   start of the transmitting block boundary.

   Note that if this function fails due to the mDSP message buffers being full
   then the discarded message callback is invoked.


DEPENDENCIES
   The channel must be configured.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_tx_prach
(
  uint8    *msg_data,
  ARFCN_T  arfcn
)
{
   uint8                msg_len;
   gl1_hw_channel_type  chan_type;
   boolean              write_ok;
   gl1_defs_tx_pkt_data_type msg;
   gl1_msg_tx_discarded_data_type discarded_data;

   if (msg_data == NULL)
   {
      return;
   }

   if ( IS_TX_WINDOW_OPEN(GERAN_ACCESS_STRATUM_ID_1) == FALSE )
   {
      /* submitted messages in frames other than 2 and 3 */
      MSG_GERAN_ERROR_1("Submitted message while tx window was closed, FN=%d",
                GSTMR_GET_FN_GERAN(GERAN_ACCESS_STRATUM_ID_1));

      /* Discard the submitted message */
      if (tx_pkt_cb.discarded_callback)
      {
         discarded_data.msg_data = msg_data;
         discarded_data.offset_to_sob = 0;
         tx_pkt_cb.discarded_callback(&discarded_data, 1,GERAN_ACCESS_STRATUM_ID_1);
      }

      return;
   }

   /*---------------------------------------------------------------------------
   * First check if the handler is installed, if not then install the handler.
   * and dont allow any further handlers to be registered
   ---------------------------------------------------------------------------*/
   if (!IS_TX_HDLR_REG(GERAN_ACCESS_STRATUM_ID_1))
   {
      gl1_msgi_add_ft_handler(gl1_msgi_prach_hdlr, GL1_MSG_FT_HDLR_UL, GERAN_ACCESS_STRATUM_ID_1);

      DONT_ALLOW_TX_HDLR_TO_BE_REG(GERAN_ACCESS_STRATUM_ID_1);
   }

   if (chn_coding_scheme == GL1_DEFS_ACCESS_CODING)
   {
      msg_len   = GL1_DEFS_ACCESS_BYTES;
      chan_type = GL1_HW_PRACH_ACC;
   }
   else
   {
      msg_len   = GL1_DEFS_EXT_ACCESS_BYTES;
      chan_type = GL1_HW_PRACH_EXT;
   }

   /* Fill in the tx pkt data structure */
   msg.gprs.data = msg_data;
   prach_params[ PARAMS_TO_USE_NEXT_BLOCK ].msg_data = msg_data;

   /*--------------------------------------------------------------------------
   * For the received message,store the frequency and set the frame offset
   * to 0 since this is an access burst.
   -------------------------------------------------------------------------- */
   prach_params[ PARAMS_TO_USE_NEXT_BLOCK ].burst_info.arfcn  = arfcn;
   prach_params[ PARAMS_TO_USE_NEXT_BLOCK ].burst_info.timing_advance = 0;
   prach_params[ PARAMS_TO_USE_THIS_BLOCK ].burst_info.channel_type = chan_type;
#if defined(FEATURE_GSM_MDSP_EGPRS)
   /* Set the loopback flag to False */
   prach_params[ PARAMS_TO_USE_THIS_BLOCK ].burst_info.loopback_flag  = FALSE;
#endif /* defined(FEATURE_GSM_MDSP_EGPRS) && defined(FEATURE_GSM_EGPRS) */
   retransmit_arfcn = arfcn;

   /* Convert the TXLEV into a proper power index */
   gl1_msgi_map_txlev_to_pwr_idx
   (
     arfcn,
     &prach_params[ PARAMS_TO_USE_NEXT_BLOCK ].TXLEV,
     &prach_params[ PARAMS_TO_USE_NEXT_BLOCK ].signal_info.tx_power_index,
     1,
     GERAN_ACCESS_STRATUM_ID_1 // can be hardcoded - this handler is not used anymore
   );

   /*---------------------------------------------------------------------------
   * Submit the message to the frame layer. Note that the parameters passed in
   * here are the parameters that have been last been configured.
   *---------------------------------------------------------------------------*/
   write_ok = gl1_hw_write_tx_pkt_data
    (
      &msg,
      msg_len,
      prach_params[ PARAMS_TO_USE_NEXT_BLOCK ].signal_info.tn,
      msg_type,
      chan_type,
      &prach_params[ PARAMS_TO_USE_NEXT_BLOCK ].tags,
      GERAN_ACCESS_STRATUM_ID_1
    );

   if (!write_ok && tx_pkt_cb.discarded_callback)
   {
      discarded_data.msg_data = msg_data;
      discarded_data.offset_to_sob = 0;
      tx_pkt_cb.discarded_callback(&discarded_data, 1,GERAN_ACCESS_STRATUM_ID_1);
      return;
   }

   REG_MSG_WAS_SUBMITTED(GERAN_ACCESS_STRATUM_ID_1);

   /* Don't allow more than 1 PRACH to be submitted */
   CLOSE_TX_WINDOW(GERAN_ACCESS_STRATUM_ID_1);


}


/*=============================================================================
FUNCTION  gl1_msgi_prach_hdlr

DESCRIPTION
   When this handler is registered, it is invoked every frame until the handler
   decides to deregister itself.

   The processing of the handler occurs in two modes. In fixed mode, the data
   is submitted in block N and somewhere in block N+1 the data is transmitted.
   In dynamic mode, the data is submitted in block N and only when the USF is
   docoded in some block X, the data is transmitted in block X+1. Until then
   the submitted message remains in hardware buffer.

   When this handler is first invoked the following happens:
   The handler does nothing until the frame before the start of the 4frame block
   boundary. In that frame, the frame layer is commanded to encode the submitted
   data and send a burst. A timer is started. The timer expires two frames later
   and a check is made to determine if a USF was decoded and the data was
   transmitted.
   If the data transmission is successful (USF decoded in the previous block)
   then the transmission notification callback is invoked and the handler is
   deregistered.

============================================================================= */
static boolean gl1_msgi_prach_hdlr
(
   boolean next_frame_is_chn_ul_block_boundary,
   uint32  time_stamp,
   gas_id_t gas_id
)
{
   boolean remove_hdlr = FALSE;
   prach_params_type *params;
   gl1_msg_tx_scheduled_data_type sched_data;

   /*---------------------------------------------------------------------------
    Process the abort command if received.
    - If abort was received after tx cmd was issued. Don't abort until sched.
      cb is invoked.
    ---------------------------------------------------------------------------*/
   if ( abort_prach )
   {
      if ( !frame_to_invoke_cb )
      {
         gl1_msgi_process_abort_prach();

         /* Nothing more to be done so exit the function */
         ALLOW_TX_HDLR_TO_BE_REG(GERAN_ACCESS_STRATUM_ID_1);
         return TRUE;
      }
   }


   /*---------------------------------------------------------------------------
   * Processing to be done in the a frame before the block on which PRACH is tx
   * The following processing is done
     _ It is determined if a message has been submitted in this block or if the
       message from the previous block is untransmitted (if USF was not
       decoded), in either case register that a burst has to be sent. However,
       if a message was submitted in this block, take a snapshot of the
       configured parameters.
   ---------------------------------------------------------------------------*/
   if( next_frame_is_chn_ul_block_boundary )
   {
      /* Check if a message has been submitted in this block period */
      if( MSG_SUBMITTED_IN_THIS_BLOCK(GERAN_ACCESS_STRATUM_ID_1) )
      {
         /*Indicate that a burst is to be sent somewhere in the next 4 frames */
         burst_is_to_be_sent = TRUE;

         /* Take a snapshot of the configured parameters.*/
         prach_params[ PARAMS_TO_USE_THIS_BLOCK ] =
          prach_params[ PARAMS_TO_USE_NEXT_BLOCK ];

         /* Set the default retransmit arfcn to be the same */
         prach_params[ PARAMS_TO_USE_THIS_BLOCK ].burst_info.arfcn = retransmit_arfcn;
#if defined(FEATURE_GSM_MDSP_EGPRS)
         /* Set the loopback flag to False */
         prach_params[ PARAMS_TO_USE_THIS_BLOCK ].burst_info.loopback_flag = FALSE;
#endif /* defined(FEATURE_GSM_MDSP_EGPRS) && defined(FEATURE_GSM_EGPRS) */
         /* Latch the camp tn */
         prach_params[ PARAMS_TO_USE_THIS_BLOCK ].camp_tn = gl1_msg_pdch_get_camp_tn(GERAN_ACCESS_STRATUM_ID_1);

      }
      else if( retransmit_data == TRUE )
      {
         /*---------------------------------------------------------------------
         For dynamic allocation, the submitted USF message may still be in the
         buffer, but no new messages were submitted. So continue issuing tx
         burst command to the hardware layer, until the submitted message has
         been transmitted.
         ---------------------------------------------------------------------*/
         /* Indicate that a burst is to be sent in the next frame */
         burst_is_to_be_sent = TRUE;

         /* Update the arfcn for the retransmit */
         prach_params[ PARAMS_TO_USE_THIS_BLOCK ].burst_info.arfcn = retransmit_arfcn;
#if defined(FEATURE_GSM_MDSP_EGPRS)
         /* Set the loopback flag to False */
         prach_params[ PARAMS_TO_USE_THIS_BLOCK ].burst_info.loopback_flag = FALSE;
#endif /* defined(FEATURE_GSM_MDSP_EGPRS) && defined(FEATURE_GSM_EGPRS) */
      }

      /*Always clear the message submitted flag at the last frame of the block*/
      CLEAR_MSGS_SUBMITTED_FLAG(gas_id);
   }

   /* Parameter shortcut */
   params = &prach_params[ PARAMS_TO_USE_THIS_BLOCK ];

   /*-----------------------------------------------------------------------
    If the timer has expired, check if the transmission was successful, that
    is,if an USF was decoded. If the USF was decoded and the msg was tx then
    invoke the callback. Once the callback is invoked the handler is
    removed.
   -----------------------------------------------------------------------*/
   if ( frame_to_invoke_cb )
   {
       --frame_to_invoke_cb;

       if (!frame_to_invoke_cb)
       {
          /* Fill in schedule data info */
          sched_data.msg_data = params->msg_data;
          sched_data.tn = params->signal_info.tn;
          sched_data.camp_tn = params->camp_tn;
          sched_data.offset_to_sob = FRAMES_AFTER_BURST_TO_INVOKE_TX_CB - 1;

          /* Check the schedule, do not discard untransmitted messages */
          if (gl1_hw_get_tx_scheduled_result(&params->tags, FALSE,GERAN_ACCESS_STRATUM_ID_1))
          {
             if (tx_pkt_cb.scheduled_callback != NULL)
             {
                tx_pkt_cb.scheduled_callback(&sched_data, 1,GERAN_ACCESS_STRATUM_ID_1);
             }
             gl1_msgi_log_tx_scheduled_results_prach();
          }
          else
          {
             if (tx_pkt_cb.scheduled_callback != NULL)
             {
                tx_pkt_cb.scheduled_callback(&sched_data, 0,GERAN_ACCESS_STRATUM_ID_1);
             }

             /*-----------------------------------------------------------------
                No message was transmitted, indicate that the msg has to be
                retransmitted when the block boundary is set next time. Note
                that this scenario can occur only for dynamic alloc.
             -----------------------------------------------------------------*/
             retransmit_data = TRUE;
             return remove_hdlr;
          }

          retransmit_data = FALSE;
          /*-----------------------------------------------------------------
            The message has been transmitted, open the message submission
            window and remove this handler. Removing the
            handler would result in this function not being called anymore.
            Also allow this handler to be registered in the future.
            Cancel any abort if pending and invoke the discard callback
          -----------------------------------------------------------------*/
          OPEN_TX_WINDOW(GERAN_ACCESS_STRATUM_ID_1);
          ALLOW_TX_HDLR_TO_BE_REG(GERAN_ACCESS_STRATUM_ID_1);

          /* Abort processing */
          if ( abort_prach )
          {
             /* Notify via the callback that there is nothing to be aborted */
             if ( tx_pkt_cb.discarded_callback )
             {
                tx_pkt_cb.discarded_callback(NULL, 0,GERAN_ACCESS_STRATUM_ID_1);
             }
             abort_prach = FALSE;
          }

          remove_hdlr = TRUE;
          return remove_hdlr;
       }
   }


   /*---------------------------------------------------------------------------
   * The following block of code does the following:
   * - Determine the frame in the block to send the burst
   * - Checks if there a burst needs to be sent this frame
   * - Indicate that a tx notification is to be sent 2 frames later.
   ---------------------------------------------------------------------------*/

   /* Check if a burst is to be scheduled */
   if ( burst_is_to_be_sent )
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
         GERAN_ACCESS_STRATUM_ID_1
       );

        /* Record that the burst has been sent */
       burst_is_to_be_sent = FALSE;

       /*--------------------------------------------------------------------
         Initialize a timer, at the end of which, invoke the tx notification
         callback
       ---------------------------------------------------------------------*/
       frame_to_invoke_cb = FRAMES_AFTER_BURST_TO_INVOKE_TX_CB;
   }

   return remove_hdlr;

}


/*=============================================================================
FUNCTION gl1_msg_update_prach_params

DESCRIPTION
   This function may be invoked when the frequency has to be changed after data
   has been submitted.

   The frequency takes effect at the next block boundary.

DEPENDENCIES
   PRACH data must be submitted.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_update_prach_params
(
   ARFCN_T arfcn
)
{
    /* Update the frequency */
   retransmit_arfcn = arfcn;

}


/*=============================================================================
FUNCTION gl1_msg_abort_prach

DESCRIPTION
   This function saves the abort state to be process in the handler.
DEPENDENCIES
   PRACH data must be submitted.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_abort_prach( void )
{
   if ( IS_TX_WINDOW_OPEN(GERAN_ACCESS_STRATUM_ID_1) == FALSE )
   {
      /* This means data has been submitted and processing is in progress*/
      abort_prach = TRUE;
   }
   else
   {
      abort_prach = FALSE;

      /* Notify via the callback that there is nothing to be aborted */
      if ( tx_pkt_cb.discarded_callback )
      {
         tx_pkt_cb.discarded_callback(NULL, 0,GERAN_ACCESS_STRATUM_ID_1);
      }
   }

}


/*===========================================================================

FUNCTION gl1_msgi_reset_prach_sm

DESCRIPTION
   This function initializes data structures that are used by all PDCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_msgi_reset_prach_sm( void )
{
   /* reset the state machine to the configured state */
   burst_is_to_be_sent          = FALSE;
   frame_to_invoke_cb           = 0;
   retransmit_data              = FALSE;
   abort_prach                  = FALSE;

   /* Terminate all common PDCH processing*/
   OPEN_TX_WINDOW(GERAN_ACCESS_STRATUM_ID_1);
   CLEAR_MSGS_SUBMITTED_FLAG(GERAN_ACCESS_STRATUM_ID_1);
}



/*=============================================================================
FUNCTION gl1_msgi_abort_prach

DESCRIPTION
   This function may be invoked to abort ongoing prach transmission.

   This function is applicable mainly to abort dynamic prach processing. In
   case a USF is not detected the NPL1 will continously issue transmit commands
   to the mDSP whenever the ul block start flag is set.

   Invoking this function will result in resetting the PRACH state machine to
   the last configured state and the discarded callback will be invoked
   with a pointer to the previously submitted payload.

   This function may be invoked in any frame in the multiframe.

DEPENDENCIES
   PRACH data must be submitted.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
static void gl1_msgi_process_abort_prach( void )
{
   uint8 num_msg_aborted = 0;
   gl1_msg_tx_discarded_data_type discarded_data;

   if (gl1_hw_abort_tx_bursts(&prach_params[PARAMS_TO_USE_THIS_BLOCK].tags,GERAN_ACCESS_STRATUM_ID_1))
   {
      num_msg_aborted = 1;
      discarded_data.msg_data = prach_params[PARAMS_TO_USE_THIS_BLOCK].msg_data;
      discarded_data.offset_to_sob = 0;

      /* Abort was successful reset the state machine to the configured state */
      gl1_msgi_reset_prach_sm();
   }

   if (tx_pkt_cb.discarded_callback)
   {
      tx_pkt_cb.discarded_callback(&discarded_data, num_msg_aborted,GERAN_ACCESS_STRATUM_ID_1);
   }

}



/*===========================================================================

FUNCTION gl1_msgi_log_tx_scheduled_results_prach

DESCRIPTION
   This function logs the tx schedule.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_msgi_log_tx_scheduled_results_prach (void)
{
   LOG_GPRS_TX_SCHEDULED_RESULTS_C_type *log_pkt;
   uint32 size;
   prach_params_type *params;
   uint32 current_FN = GSTMR_GET_FN_GERAN(GERAN_ACCESS_STRATUM_ID_1);

   /* Calculate how much space we need */
   size = sizeof(LOG_GPRS_TX_SCHEDULED_RESULTS_C_type)
           - (GL1_DEFS_MAX_ASSIGNED_UL_TS - 1)
             * sizeof(log_gprs_tx_scheduled_result_type);

   /* Try to allocate memory for the log packet */
   log_pkt = (LOG_GPRS_TX_SCHEDULED_RESULTS_C_type *)
              log_alloc(LOG_GPRS_TX_SCHEDULED_RESULTS_C, size);
   if (log_pkt == NULL) return;

   /* Shortcut */
   params = &prach_params[ PARAMS_TO_USE_THIS_BLOCK ];

   /* Fill in the log info */
   log_pkt->num_msg_tx = 1;
   log_pkt->FN = SUB_FN(current_FN,1);   /* nolint !e666*/
   log_pkt->msg_info[0].tn = params->signal_info.tn;
   log_pkt->msg_info[0].power_index = params->signal_info.tx_power_index;
   log_pkt->msg_info[0].timing_advance = 0;
   log_pkt->msg_info[0].dynamic_alloc = (msg_type == GL1_HW_USF);
   log_pkt->msg_info[0].coding_scheme = chn_coding_scheme;
   log_pkt->msg_info[0].msg_type = GL1_MSG_PRACH;
   log_pkt->arfcn[0] = arfcn_to_word(params->burst_info.arfcn);
   log_pkt->arfcn[1] = log_pkt->arfcn[2] = log_pkt->arfcn[3] = 0;

   /* Send the log packet */
   log_commit((void *)log_pkt);
}




