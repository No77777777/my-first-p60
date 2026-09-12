/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    G L 1  M E S S A G E  L A Y E R -  N C E L L  B C C H   C H A N N E L

GENERAL DESCRIPTION
   This module contains the procedures used for decoding the BCCH from a
   neighbor cell.
   The procedures used herein are intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

   gl1_msg_cfg_ncell_bcch
   gl1_msg_rx_ncell_bcch
   ncell_bcch_rx_handler



INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_ncell_bcch.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
19/10/15   br       CR926522 GL1 to Pass the frequency offset to rotator to correct in GBTA.
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
12/09/14   nm       CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
20/06/13   cs       Major Triton TSTS Syncup
22/11/10   ky       Panic reset mechanism is changed. For all errors gl1 first
                    sees an nv item set or not. if this nv item is set then does
                    error fatal immediately other wise gl1 sends signal to grr for
                    recovery
11/01/04   yhong    Changed NCell Abort API to take a parameter
06/15/04   gfr      Combined ncell bcch and cbch into common async ccch handler.
06/09/04   gfr      Consolidate static variables for robustness.
06/23/04   gfr      Lint cleanup.
06/09/04   gfr      Get results a frame earlier.
06/02/04   gfr      Cleaned up abort mechanism.
05/07/04   gfr      Removed broken legacy support for back-to-back operation.
03/05/04   gfr      Lint cleanup.
02/09/04   dp       Modified abort mechanism for background HPLMN
01/29/04   gfr      Mainlined FEATURE_INHIBIT_PSBI
11/05/03   gfr      Support for quad-band.
08/12/03   gw       Modified aborting so that other receives (power measures)
                    may remain in progress.
07/25/03   gfr      Removed the channel filter delay timing error adjustment.
07/22/03   gw       Added call to gl1_hw_reset in gl1_msg_abort_rx_ncell_bcch()
                    to ensure that mDSP and RF drivers are left in the correct
                    state.
06/09/03   gw       Added check on number of messages in process at once. 
                    Maximum allowed is one.
06/03/03   gfr      Suspend PSBI transactions during ncell activity.
05/12/03   gfr      Remove burst_schedule code.
05/01/03   ATM      Moved Frame Number management entirely inside GTMRS
04/16/03   gfr      Pass the function pointer when removing the handler.
01/24/03   ATM      Added NCELL_BCCH msg/burst logging
11/15/03   gfr      Replaced old rxlev BCCH functions with new generic GPRS
                    burst indication function.                    
01/13/03   dp       Adjusted timing error to account for new channel filter
                    delay
10/28/02   gw       Changed calls to gl1_hw_was_async_burst_decoded to reflect
                    new interface supporting back-to-back async receives.
10/01/02   ATM      Removed unused references to L1's FN
09/04/02   plm      Added externs for gl1_msg_store_rx_ncell_bcch_data() and
                    gl1_msg_retrieve_rx_ncell_bcch_data() to eliminate compiler
                    warnings.
08/16/02   JC       Corrected RSSI reporting s.t it is true RSSI from the MDSP
07/18/02   ATM      Added BCCH decode logging.
06/28/02   bk       Added 4 freq to the rx function. Featurized the slamming of 
                    the frame tick
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
06/20/02   dp       Featurizing Ncell BCCH slamming to handle corner cases.
                    Default is not to slam
04/15/02   mk       Added code to handle scell-ncell timing offset corner cases.
03/25/02   gw       Changed handling of metrics and logging. Cleaned up indenting.
03/21/02   gw       Made sure metrics callback is always called even if metrics
                    are not retrieved from the mDSP.
03/18/02   gw       Changed handlers timing for retrieving results and metrics.
02/14/02   gw       Added support for AGC.
02/07/02   gw       Changed comments only.
12/18/01   dp       Added support to abort handler
11/6/01    bk       Changed NPL1 function calls to reflect change in interface
10/31/01   gw/dp    Swapped order of retrieving metrics and issuing rx burst
                    command in handler. Made changes to pass in the required number 
                    of data octets when retrieving async BCCH decoded data.
10/31/01   dp       Made changes to use new gl1_hw_was_async_burst_decoded interface
                    to eliminate multiple copies when retrieving DSP results
10/23/01   gw       Corrected call to gl1_hw_get_asyncrx_metrics.
10/16/01   gw       Changes to support returning RSSI in dBm.
09/21/01   gw       Initial version.
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
#include "err.h"

#include "geran_dual_sim.h"

/*****************************************************

 LOCAL STORAGE

******************************************************/

/* Storage for static parameters */
static gl1_msgi_async_ccch_params_type ncell_bcch_params[NUM_GERAN_DATA_SPACES];


/*===========================================================================

FUNCTION gl1_msg_init_ncell_bcch

DESCRIPTION
  This function is used to initialize ncell BCCH data.  Called from
  gl1_msg_init().

===========================================================================*/
void gl1_msgi_init_ncell_bcch (gas_id_t gas_id)
{
   ncell_bcch_params[gas_id].TSC                  = 0;
   ncell_bcch_params[gas_id].DRX                  = FALSE;

   ncell_bcch_params[gas_id].callbacks.data_cb    = NULL;
   ncell_bcch_params[gas_id].callbacks.metrics_cb = NULL;
   ncell_bcch_params[gas_id].callbacks.log_cb     = NULL;
   ncell_bcch_params[gas_id].callbacks.abort_cb   = NULL;
}



/*===========================================================================

FUNCTION gl1_msg_cfg_ncell_bcch

DESCRIPTION
  This function is used to configure the TSC and callbacks for the neighbor
  cell BCCH. The callbacks will be invoked by the frame tick handler as
  required.

===========================================================================*/
void gl1_msg_cfg_ncell_bcch
(
   uint8 TSC,
   boolean DRX,
   gl1_msg_rx_cb_type *cb,
   gas_id_t gas_id
)
{
   ncell_bcch_params[gas_id].TSC       = TSC;
   ncell_bcch_params[gas_id].DRX       = DRX;
   ncell_bcch_params[gas_id].callbacks = *cb;
}


/*===========================================================================

FUNCTION gl1_msg_rx_ncell_bcch

DESCRIPTION
  This function is used to receive a message on the BCCH of a neighbor cell.
  The message is received on the next bcch block boundary on the neighbor
  cell.

DEPENDENCIES
  gl1_msg_cfg_ncell_bcch() must have been called at some time prior.

===========================================================================*/
void gl1_msg_rx_ncell_bcch
(
   ARFCN_T          ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD],
   sys_algo_agc_T  *agc,
   uint16           timing_offset,
   gl1_msg_rx_rpt  *rpt,
   gl1_msg_chan_type  msg_chan_type,
   async_ccch_type_t  async_ccch_type,
   int32           freq_error,
   gas_id_t         gas_id
)
{
   int i;

   /* Fill in parameters */
   ncell_bcch_params[gas_id].timing_offset   = timing_offset;
   ncell_bcch_params[gas_id].rpt             = *rpt;
   ncell_bcch_params[gas_id].agc_ptr         = agc;
   ncell_bcch_params[gas_id].async_ccch_type = async_ccch_type;
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
   if (msg_chan_type == GL1_MSG_CCCH)
   {
     ncell_bcch_params[gas_id].hw_channel_type = GL1_HW_PCH;
   }
   else
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
   {
     ncell_bcch_params[gas_id].hw_channel_type = GL1_HW_BCCH;
   }

   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      ncell_bcch_params[gas_id].ARFCN[i]        = ARFCN[i];
   }
   gl1_msg_update_bcch_rotator_value( freq_error, gas_id);
   /* Pass them to the common handler */
   gl1_msgi_rx_async_ccch(msg_chan_type, &ncell_bcch_params[gas_id],gas_id);
}


/*===========================================================================

FUNCTION  gl1_msg_abort_rx_ncell_bcch

DESCRIPTION
  This function aborts the currently ongoing ncell BCCH rx and invokes the
  registered abort callback when done
  
DEPENDENCIES
  This function should only be called from the ISR - we don't want the
  handler running in the middle of it.

PARAMETERS
  Callback to

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void gl1_msg_abort_rx_ncell_bcch(gl1_msg_ncell_abort_type type,gas_id_t gas_id)
{
    if(type == DELAYED_ABORT)
    {
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
        if(ncell_bcch_params[gas_id].hw_channel_type == GL1_HW_PCH)
        {
          gl1_msgi_abort_rx_async_ccch(GL1_MSG_CCCH,gas_id);
        }
        else
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/
        {
          gl1_msgi_abort_rx_async_ccch(GL1_MSG_BCCH,gas_id);
        }
    }
    else
    {
        /* This option is not supported by the current software, not that we cannot abort it
          immediately */
        if(geran_get_nv_recovery_restart_enabled(gas_id))
        {
          MSG_GERAN_ERROR_0_G("Cannot abort bcch immediately!");
          gl1_hw_handle_panic(gas_id);
        }
        else
        {
#ifdef FEATURE_QSH_MDUMP
          QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
          MSG_GERAN_FATAL_0_G("Cannot abort bcch immediately!");
        }
    }
}




