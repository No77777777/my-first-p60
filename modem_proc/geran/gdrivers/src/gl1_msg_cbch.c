/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         G L 1  M E S S A G E  L A Y E R -  C B C H   C H A N N E L

GENERAL DESCRIPTION
   This module contains the procedures used for communicating
   on the CBCH.
   The procedures used herein are intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

   gl1_msg_cfg_dl_cbch
   gl1_msg_rx_cbch
   gl1_msg_abort_rx_cbch




INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001, 2002-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_cbch.c#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
19/10/15   br       CR926522 GL1 to Pass the frequency offset to rotator to correct in GBTA.
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
03/02/15   br       CR791092 Adding F3 to print serving cell TSC and CBCH TSC
12/09/14   nm       CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
11/07/05   gfr      QLint
06/15/04   gfr      Combined ncell bcch and cbch into common async ccch handler.
06/09/04   gfr      Consolidate static variables for robustness.
06/25/04   gfr      Set the offset_to_sob field properly.
06/23/04   gfr      Lint cleanup.
06/22/04   gfr      Resume peripheral SBI after early decode.
06/02/04   gfr      Added early decode support.
06/02/04   gfr      Lint cleanup.
05/14/04   gsc      Updated abort processing.
04/29/04   gfr      Lint cleanup.
03/15/04   gsc      Removed STAGE2_CHANNEL_FILTER_DELAY compensation.
02/08/04   gsc      Changed channel type in cbch_rx_callback from GL1_HW_PCH
                    to GL1_MSG_SDCCH.
01/29/04   gfr      Mainlined FEATURE_INHIBIT_PSBI
10/22/03   gsc      Added Quad band support
10/22/03   gsc      Fixed featurization error.
10/21/03   gsc      F3 message cleanup.
10/21/03   gsc      CBCH support. First revision featurized under 
                    FEATURE_GSM_CB
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif


#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "gl1_sys_algo.h"
#include "msg.h"


/*****************************************************

 LOCAL STORAGE

******************************************************/


/* Storage for the CBCH parameters */
static gl1_msgi_async_ccch_params_type cbch_params[NUM_GERAN_DATA_SPACES];



/*===========================================================================

FUNCTION gl1_msgi_init_cbch

DESCRIPTION
  This function is used to initialize the CBCH data structures.

===========================================================================*/
void gl1_msgi_init_cbch (gas_id_t gas_id)
{
   cbch_params[gas_id].DRX              = FALSE;
   cbch_params[gas_id].TSC              = 0;

   cbch_params[gas_id].callbacks.data_cb    = NULL;
   cbch_params[gas_id].callbacks.metrics_cb = NULL;
   cbch_params[gas_id].callbacks.log_cb     = NULL;
   cbch_params[gas_id].callbacks.abort_cb   = NULL;
}


/*===========================================================================

FUNCTION gl1_msg_cfg_cbch

DESCRIPTION
  This function is used to configure the TSC and callbacks for the CBCH. The
  callbacks will be invoked by the frame tick handler as required.

===========================================================================*/
void gl1_msg_cfg_cbch
(
   uint8 TSC, 
   boolean DRX,
   gl1_msg_rx_cb_type *cb,
   gas_id_t gas_id
)
{
   cbch_params[gas_id].callbacks   = *cb;
   cbch_params[gas_id].DRX         = DRX;
   cbch_params[gas_id].TSC         = TSC;
   MSG_GERAN_HIGH_1_G(" CBCH config init:Serving cell tsc:%d  ", cbch_params[gas_id].TSC );
}


/*===========================================================================

FUNCTION gl1_msg_store_cbch_TSC

DESCRIPTION
  This function is used to configure the TSC for the CBCH.

===========================================================================*/
void gl1_msg_store_cbch_TSC (uint8 TSC,gas_id_t gas_id)
{
   if(cbch_params[gas_id].TSC != TSC)
   {
	 MSG_GERAN_HIGH_2_G("CBCH config, old_tsc: %d  New_tsc:%d ", TSC, cbch_params[gas_id].TSC );
   } 
   cbch_params[gas_id].TSC = TSC;
}



/*===========================================================================

FUNCTION gl1_msg_rx_cbch

DESCRIPTION
  This function is used to receive a message on the CBCH on the next CBCH
  block boundary. This function may be called at task level but should
  be called in the block before the data is to transmitted.

DEPENDENCIES
  gl1_msg_cfg_dl_cbch() must have been called at some time prior.

===========================================================================*/
void gl1_msg_rx_cbch
(
   ARFCN_T          ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD],
   sys_algo_agc_T   *agc_ptr,
   int16            timing_offset,
   gl1_msg_rx_rpt   *rpt,
   gas_id_t         gas_id
)
{
   uint16 i;
   
   /* Store parameters */
   cbch_params[gas_id].timing_offset     = (uint16)timing_offset;
   cbch_params[gas_id].rpt               = *rpt;
   cbch_params[gas_id].agc_ptr           = agc_ptr;
   cbch_params[gas_id].hw_channel_type   = GL1_HW_PCH;
   cbch_params[gas_id].async_ccch_type   = ASYNC_ACTIVITY_SUSPENSION; // Default,dont care
  
   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      cbch_params[gas_id].ARFCN[i]           = ARFCN[i];
   }
   gl1_msg_update_bcch_rotator_value(0,gas_id);
   gl1_msgi_rx_async_ccch(GL1_MSG_CBCH, &cbch_params[gas_id],gas_id);
}



/*===========================================================================

FUNCTION gl1_msg_abort_rx_cbch

DESCRIPTION
  This function is used to abort CBCH.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_abort_rx_cbch (gas_id_t gas_id)
{
   /* Call the common abort function */
   gl1_msgi_abort_rx_async_ccch(GL1_MSG_CBCH,gas_id);
}



