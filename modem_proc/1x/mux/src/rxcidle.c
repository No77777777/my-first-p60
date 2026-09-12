/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              CDMA2000 Receive Task Idle State Processing.

GENERAL DESCRIPTION
  Contains implementation of idle state operations that are not specifically
  tied to a physical layer channel.  Paging Channel, BCCH, and FCCCH all use
  this common functionality.

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2002 - 2013 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*===========================================================================

                              Edit History

$PVCSPath: O:/src/asw/COMMON/vcs/rxcidle.c_v   1.15   23 Oct 2002 16:27:16   baon  $
$Header: //components/rel/1x.mpss/8.0/mux/src/rxcidle.c#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/31/13   srk     Mainlined FEATURE_MODEM_1X_NON_SIG.
05/09/13   cjb     Exporting MEMPOOL capture API outside of 1x-L1.
04/19/13   pap     Adding mempool capture capability for diag and fade timers.
02/15/13   srk     Replace mcs hwio with m1x hwio
07/16/12   srk     Removed FEATURE_IS2000_REL_A_CC
06/15/12   srk     Removed FEATURE_IS2000_REL_A_CC_SMMC
05/16/12   srk     Removed FEATURE_FULL_POWER_COLLAPSE.
01/23/12   srk     Feature Cleanup
11/22/11   srk     Feature clean up: Mainlined and Internalized Features
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
01/28/11   jtm     Additional FTM decoupling.
11/15/10   jtm     Initial SU API split for CP.
11/05/10   jtm     Added more _i.h includes for Modem 1x SU API support.
10/30/10   jtm     Modem 1x SU API changes.
11/30/09   vks     Group together mdsp writes for 1xADV channel gain ratios.
11/07/09   vks     Use muxmdsp_set_channel_gains() instead of
                   TXC_SET_<CHANNEL>_GAIN macros.
09/22/09   jtm     Corrected compiler warnings.
06/11/09   jtm     Additional 1x Advanced RL support.
05/20/09   jtm     Added include for rx_v.h for CMI clean up.
04/28/09   jtm     Initial 1X Advanced feature support.
04/13/09   adw     Updated rf includes to support rf mode refactoring.
04/01/09   jtm     Include clean up.
03/27/09   jtm     Eliminated implicit includes.
03/26/09   jtm     Replaced T_QSC60X5 with FEATURE_MODEM_1X_FREQ_HOP
03/26/09   jtm     Added include for modem_1x_defs.h
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
11/06/08   jtm     Updated rfm_notify_poor_signal() call according to latest api
                   definition from RF team and uncommented it.
10/24/08   jtm     Commented out rfm_notify_poor_signal() call until RF portion is
                   ready for mainlining the feature.
10/22/08   jtm     Added call to rfm_notify_poor_signal() when page channel is lost
08/02/06   trc     Add NCCCH failsafe timer support
07/20/06   bn      Check in the powercollapse code under FEATURE_FULL_POWER_COLLAPSE
07/25/05   rkc     Fixed lint errors.
06/16/04   sr      Merged in changes from 6500 5004 build
06/04/04   bt      Use srch_mux_get_curr_20ms_frame instead of directly
                   accessing symbol combiner.
04/20/04   jrp     Fixed lint warnings.
03/03/04   bn      Don't delete the FCCCH deint task when the next state is PC
11/04/03   bn      Improved the 5ms delay on FCCCH
10/22/03   sr      Fixed compilation warning errors when using RVCT compiler
07/31/03   bn      Removed the redundancy checking skip flag in the rxcidle_first_frame()
                   This fixes the wrong skipping frame when in simultaneously FCCCH/BCCH
06/16/03   bn      Fixed the hard-handoff problem when switching from NCCCH to PCH
05/22/03   vlc     Corrected typo for FEATURE_FACTORY_TESTMODE.
05/05/03   bn      Optimized the NCCCH logics and F3 debug messages
03/20/03   bn,sr   Added logic to support simultaneously monitor BCCH and FCCCH
03/04/03   fc      Consolidated f-csh fade timer implementation.
02/25/03   va      Removed changing MC and SRCH priority by MC.
10/21/02   bn      Added support for NCC statistic counters
10/03/02   bn      Added support for Broadcast BCCH
10/14/02   sr      Changed FEATURE_IS2000_REL_A to ...REL_A_CC
                   in rxcidle_first_frame()
08/13/02   sr      Removed debug message
08/13/02   sr      Removed rxc_idle_chan_config(). New SRCH-RXC command
                   interface for fast RAHO replaces this.
06/03/02   sr      Resolved compile errors for non-REL_A builds
05/31/02   sr      Added rxcidle_first_frame() to check if we are in the first
                   frame after wakeup
02/15/02   lad     Added external routine rxc_idle_chan_config() for Fast RAHO.
                   Added rxcidle_chan_config().  Added rxcidle_pending_state()
                   for state transitions triggered in external context.
01/23/02   lad     Updated rxcidle_init() to only set up timer at the first
                   time.
01/17/02   lad     Created file.  Includes fade timer support.

===========================================================================*/

//common
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

//mux
#include "rxcidle.h"
#include "rxccommon.h"
#include "rxccommon_i.h"
#include "rxcpaging.h"
#include "muxmdsp_i.h"
#include "txccommon_i.h"
#include "rxcmc_i.h"
#include "txcmc_i.h"

//drivers
#include "deint_i.h"
#include "enc.h"
#include "m1x_hwio_mpss.h"
#include "msm_drv.h"
#include "msm_drv_i.h"

//cp
#include "parm_i.h"
#include "cai_v.h"
#include "mccrx.h"
#include "mclog.h"
#include "mclog_v.h"
#include "rx.h"
#include "rx_v.h"
#include "mc_v.h"

//srch
#include "srch_mux.h"
#include "srch_v.h"

//diag
#include "onex_non_sig.h"

//other
#include "rex.h"
#include "err.h"
#include "msg.h"
#include "m1x_diag.h"
#include "rfm_1x.h"
#include "queue.h"


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* Fade timer information.  This structure is used to apply T30M and T40M.
   T72M is handled explicitly. */
static struct {
  rxcidle_fade_type curr_fade_type; /* Type of timer in use */

  /* Timer for amount of time since mobile last detected a valid SAT while
     communicating with the base station over the voice channel */
  rex_timer_type timer;

  /* Duration of 'timer' */
  word timer_duration;

  /* AHO support */
  /* This timer is used for T72M timer monitoring */
  rex_timer_type t72_timer;

  /* Transmitter status */
  boolean tx_disabled;
} rxcidle_fade_info;

/*--------------------------------------------------------------------------
 * Internal Function Prototypes
 *--------------------------------------------------------------------------*/
static void rxcidle_rpt (mccrx_rpt_name_type);
static void rxcidle_config_tx (boolean);

/*--------------------------------------------------------------------------
 * Function Implementations
 *--------------------------------------------------------------------------*/
/*===========================================================================*/
void
rxcidle_def_timer (void)
{
  rex_def_timer (&rxcidle_fade_info.timer, RX_TCB_PTR, RXC_FADE_TIMER_SIG);
  rex_def_timer (&rxcidle_fade_info.t72_timer, RX_TCB_PTR, RXC_AHO_FADE_TIMER_SIG);
}

/*===========================================================================*/
void
rxcidle_init (void)
{
  rxcidle_fade_info.curr_fade_type = RXCIDLE_IDLE_FADE;
  rxcidle_fade_info.tx_disabled = FALSE;
  rxcidle_fade_info.timer_duration = 0;
}

/*===========================================================================*/
rex_sigs_type
rxcidle_wait_mask (void)
{
  rex_sigs_type mask = 0;

  switch (rxcidle_fade_info.curr_fade_type) {
    case RXCIDLE_ACCESS_FADE:
      mask = RXC_AHO_FADE_TIMER_SIG | RXC_FADE_TIMER_SIG;
      break;
    case RXCIDLE_IDLE_FADE:
      mask = RXC_FADE_TIMER_SIG;
      break;
    default:
      mask = 0;
  }

  return mask;
}


/*===========================================================================*/
void
rxcidle_set_fade_timer (rxcidle_fade_type fade_type)
{

  if (!onex_nonsig_is_ftm_mode())
  {
    if (fade_type == RXCIDLE_CURR_FADE)
    {
      fade_type = rxcidle_fade_info.curr_fade_type;
    }
    else if (rxcidle_fade_info.curr_fade_type != fade_type)
    {
      /* Changing timer types, clear existing timer and set new type */
      rxcidle_clr_fade_timer();
      rxcidle_fade_info.curr_fade_type = fade_type;
    }

    switch (fade_type)
    {
      case RXCIDLE_IDLE_FADE:
        rxcidle_fade_info.timer_duration = cai_tmo.t30m;
        break;

      case RXCIDLE_ACCESS_FADE:

        /* If TX was disabled, and we are now setting the timer, TX will
           no longer be disabled.  Therefore, send a message to MC to
           notify that a valid message was received after T72M expired. */
        if (rxcidle_fade_info.tx_disabled)
        {
          rxcidle_rpt (RXC_VALID_PC_MSG_R);
        }

        if (P_REV_IS_4_OR_GREATER)
        {
          /* Access fade uses both T40M (3 secs) and T72M (1 sec) */
          rxcidle_fade_info.timer_duration = cai_tmo.t40m;

          /* Enable the transmitter */
          rxcidle_config_tx (TRUE);

          (void) rex_set_timer(&rxcidle_fade_info.t72_timer, cai_tmo.t72m);
        }
        else
        {
          /* Access fade uses only T40M (1 sec) */
          rxcidle_fade_info.timer_duration = 1000;
        }
        break;

     default:
       rxcidle_fade_info.timer_duration = 0;
       break;
    }

    if (rxcidle_fade_info.timer_duration > 0)
    {
      (void) rex_set_timer(&rxcidle_fade_info.timer,
                           rxcidle_fade_info.timer_duration);
    }
  }
}

/*===========================================================================*/
void
rxcidle_clr_fade_timer (void)
{
  switch (rxcidle_fade_info.curr_fade_type) {
    case RXCIDLE_ACCESS_FADE:
      (void) rex_clr_timer (&rxcidle_fade_info.t72_timer);
      (void) rex_clr_sigs (RX_TCB_PTR, RXC_AHO_FADE_TIMER_SIG);
      /* Fall through for regular fade timer */

    case RXCIDLE_IDLE_FADE:
      (void) rex_clr_timer (&rxcidle_fade_info.timer);
      (void) rex_clr_sigs (RX_TCB_PTR, RXC_FADE_TIMER_SIG);
      break;

    case RXCIDLE_CURR_FADE:
      /* Do nothing */
      break;
  }
}

/*===========================================================================*/
boolean
rxcidle_process_sigs (rex_sigs_type sigs)
{
  boolean OK_to_continue = TRUE;
  cai_timer_id_type timer_id = CAI_TIMER_T30M;
  mccrx_rpt_name_type rpt = RXC_PC_T30M_TMO_R;

  /* wait for command or interrupt or fade timer to expire */
  if (sigs & RXC_FADE_TIMER_SIG) {
    (void) rex_clr_sigs (RX_TCB_PTR, RXC_FADE_TIMER_SIG);

    switch (rxcidle_fade_info.curr_fade_type) {
      case RXCIDLE_IDLE_FADE: /* Values set by default */
        M1X_MSG( MUX, LEGACY_MED,
          "T30M expired");
        /* These values are set by default. */

        /* Log the primary chain mempool data*/
        srch_mempool_capture();
        break;

      case RXCIDLE_ACCESS_FADE:
        M1X_MSG( MUX, LEGACY_MED,
          "T40M expired");
        rpt = RXC_PC_T40M_TMO_R;
        timer_id = CAI_TIMER_T40M;

        /* Log the primary chain mempool data*/
        srch_mempool_capture();
        break;

      default:
        /* Invalid state */
        break;
    }
    if ((rxcidle_fade_info.curr_fade_type == RXCIDLE_IDLE_FADE) ||
        (rxcidle_fade_info.curr_fade_type == RXCIDLE_ACCESS_FADE) )
    {
      /* Increment he counter which keeps track the number of time that the
         MS declared a loss of PCH channel */
      (void) parm_inc(CAI_PAG_6_ID, 1);
    }

//XXX This looks like some hack in rxcpaging.c.  I wonder why...
//    rxcidle_fade_info.tx_disabled = FALSE;
//    -or-
//    rxcidle_config_tx (TRUE);

    /* Report timer expiration to upper layer. */
    rxcidle_rpt (rpt);

    /* Event report: Timer expired. */
    mclog_report_event_timer (timer_id);
  }

  if (sigs & RXC_AHO_FADE_TIMER_SIG) {
    (void) rex_clr_sigs (RX_TCB_PTR, RXC_AHO_FADE_TIMER_SIG);

    /* ------------------------------------------------------------
    ** T72M timer has expired, Disable the transmitter and suspend
    ** the probe transmission.  Send a T72M Fade report to MC task.
    ** -----------------------------------------------------------*/
    mclog_report_event_timer (CAI_TIMER_T72M);

    /* Log the primary chain mempool data*/
    srch_mempool_capture();

    /* Disable the transmitter */
    rxcidle_config_tx (FALSE);

    rxcidle_rpt (RXC_PC_T72M_TMO_R);

    OK_to_continue = FALSE; /* This timer expiration is a show stopper */
  }

  return OK_to_continue;

}

/*===========================================================================

FUNCTION RXCIDLE_RPT

DESCRIPTION
  Sends a report to the upper layer (MC).

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void
rxcidle_rpt (mccrx_rpt_name_type rpt_name)
{
  mccrx_rpt_type *rpt_ptr = NULL;

  if ((rpt_ptr = (mccrx_rpt_type *) q_get (&mcc_rx_free_q)) != NULL) {
    rpt_ptr->hdr.rpt = rpt_name;
    rxc_mc_rpt (rpt_ptr);
  } else {
    M1X_MSG( MUX, LEGACY_ERROR,
      "No buffers on mcc_rx_free_q");
  }
}

/*===========================================================================

FUNCTION RXCIDLE_CONFIG_TX

DESCRIPTION
  This function enables/disables transmitter.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void
rxcidle_config_tx (boolean enable)
{
  boolean suspend = (enable) ? FALSE : TRUE;

  txc_suspend_tx (suspend);
  rxcidle_fade_info.tx_disabled = suspend;
}

/*===========================================================================

FUNCTION RXCIDLE_FIRST_FRAME

DESCRIPTION
  This function determines whether it is the first frame after wake
  up

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if it is the first frame; FALSE otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean rxcidle_first_frame( void )
{
  boolean result = FALSE;
  srch_mux_20ms_frame_type frame_20ms = srch_mux_get_curr_20ms_frame();

  if( frame_20ms == FRAME_20_MS )
  {
    /* Decoder Interrupt occured between 20.0ms and 39.9ms.(PCH)
     * This must be for the frame from 0ms to 20ms */

    result = TRUE;
    /* Don't skip frames anymore. */
  }

  return( result );
}
