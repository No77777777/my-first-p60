/*===========================================================================

     IMPLEMENTATION OF GPRS NPL1 DOWNLINK PBCCH FUNCTIONS

GENERAL DESCRIPTION
   This file implements GPRS NPL1 downlink PBCCH functions.

   Some points of consideration are:
   1. The processing is based on a block period consisting of 4 frames. The
   command to receive message may be given in the first 3 frames of a block
   (block N).
   In the fourth frame of the block, 4 receive bursts are scheduled. The metrics
   and log data are avaiable starting from Frame 3 of block N+1 and the data is
   available starting from Frame 2 of block N+2.


EXTERNALIZED FUNCTIONS

gl1_msgi_pbcch_init
gl1_msgi_pbcch_terminate


INITIALIZATION AND SEQUENCING REQUIREMENTS
The following is the desired order in which functions should be called

gl1_msgi_pbcch_init
gl1_msgi_pbcch_terminate

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:   L:/src/asw/KOVU/drivers/vcs/gl1_msg_pbcch.c_v   1.1   09 Nov 2001 17:20:30 $
$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_pbcch.c#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
04/12/05   gfr      Set the search width
11/05/03   gfr      Support for quad-band.
05/01/03   gfr      Change featuring from FEATURE_GSM to FEATURE_GSM_GPRS
09/06/02   gr       Added support for GPRS AGC.
08/28/02   gr       Added abort functionality.
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
02/08/02   bk       changed interface to add agc info.

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

/* header files containing system algorithms for AGC/AFC/TT etc */
#include "gl1_sys_algo.h"
#include "gl1_sys_algo_gprs.h"


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
#define MSG_NUM 0
static sm_rx_params_type  chn_rx_params;
static sm_cfg_params_type chn_cfg_params;


/*===========================================================================

FUNCTION gl1_msgi_pbcch_init

DESCRIPTION
   This function initializes data structures that are used by pbcch.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_pbcch_init( void )
{
  /* Initialize the internal variables */
  memset(&chn_rx_params, 0, sizeof(chn_rx_params));
  memset(&chn_cfg_params, 0, sizeof(chn_cfg_params));
  
  chn_cfg_params.chan_type = GL1_HW_PBCCH;
  chn_rx_params.search_width = GL1_HW_GPRS_SEARCH_WIDTH;

}


/*===========================================================================

FUNCTION gl1_msgi_pbcch_terminate

DESCRIPTION
   Terminates all pbcch processing

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_pbcch_terminate( void )
{
  /* Terminate all dl_pacch_pdtch processing */
  chn_rx_params.num_msgs                    = 0;
  chn_cfg_params.rx_pkt_cb.data_callback    = 0;
  chn_cfg_params.rx_pkt_cb.log_callback     = 0;
  chn_cfg_params.rx_pkt_cb.metrics_callback = 0;
}


/*=============================================================================
FUNCTION gl1_msg_cfg_pbcch

DESCRIPTION
   PACCH and PDTCH are configured and ready to transmit data.


DEPENDENCIES
   The message layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_cfg_pbcch
(
  gl1_defs_tn_type              tn,
  uint8                         tsc,
  const gl1_defs_rx_pkt_cb_type  *rx_msg_cb_info
)
{

  /* Store the specified parameters */
  chn_cfg_params.tsc                        = tsc;

  chn_cfg_params.rx_pkt_cb.data_callback    = rx_msg_cb_info->data_callback;
  chn_cfg_params.rx_pkt_cb.log_callback     = rx_msg_cb_info->log_callback;
  chn_cfg_params.rx_pkt_cb.metrics_callback = rx_msg_cb_info->metrics_callback;

  /* There is only one message for PBCCH */
  chn_rx_params.signal_info[ 0 ].tn         = tn;
}


/*=============================================================================
FUNCTION gl1_msg_rx_pbcch

DESCRIPTION
   Schedule a receive and decode of a PBCCH block of data at the specified
   frequencies. This function also causes the metrics and data callback handlers
   to be invoked every frame in the block.

   The pointers to the buffers for decoded data, metrics and log will
   subsequently be filled by NPL1 and the corresponding callback will be invoked
   to notify that the data/information has been copied on to the buffer.

   This function is to be called anywhere in the first 3 frames of the previous
   block. The first received burst is scheduled at the start of the current
   block boundary.

DEPENDENCIES
   The channel must be configured.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_rx_pbcch
(
  const ARFCN_T                  arfcn[ GL1_DEFS_FRAMES_IN_BLK_PERIOD ],
  sys_algo_agc_T                 *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  sys_algo_agc_T                 *agc_ptr_divrx,
#endif
  const gl1_defs_rx_pkt_mem_type *rx_data_info
)
{
  uint8 frame_num;
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;

  /*--------------------------------------------------------------------------
  * For the received message,store the frequency and signal info.
  -------------------------------------------------------------------------- */
  for ( frame_num = 0; frame_num < GL1_DEFS_FRAMES_IN_BLK_PERIOD; frame_num++ )
  {
    chn_rx_params.arfcn[ frame_num ] = arfcn[ frame_num ];
  }

  chn_rx_params.signal_info[ MSG_NUM ].rx_signal_strength
    = sys_algo_agc_gprs_expected_pwr(agc_ptr->pwr_dBm_x16, GL1_HW_PBCCH,gas_id);
  chn_rx_params.agc_ptr = agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  chn_rx_params.signal_info[ MSG_NUM ].rx_signal_strength_divrx
    = sys_algo_agc_gprs_expected_pwr(agc_ptr_divrx->pwr_dBm_x16, GL1_HW_PBCCH,gas_id);
  chn_rx_params.agc_ptr_divrx = agc_ptr_divrx;
#endif
  /* store the metrics,log and data pointers, that will be used later */
  gl1_msg_pdch_store_sm_info
    (
      rx_data_info,
      &chn_rx_params,
      &chn_cfg_params,
      gas_id
    );
}




/*=============================================================================
FUNCTION gl1_msg_abort_dl_pbcch

DESCRIPTION
   Immediately bborts the processing of the PBCCH. Some of the drivers
   are left in a incorrect state. gl1_msg_reset() needs to be invoked following
   this function.


DEPENDENCIES
   The channel must be configured.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_abort_pbcch( void )
{
    /* Aborts the specified function */
    gl1_msg_pdch_abort_dl_pbcch_pccch_pacch_pdtch( GL1_HW_PBCCH,GERAN_ACCESS_STRATUM_ID_1);
}




