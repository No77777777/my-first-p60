 /*===========================================================================
                  D O W N L I N K   T R A N S P A R E N T   M O D E   

                          R A D I O   L I N K   C O N T R O L  

DESCRIPTION
  This file provides the functions to process RLC Transparent Mode 
  Downlink logical channels.  
    
EXTERNALIZED FUNCTIONS
  List any functions that are externalized in this file.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  List any initialization and sequencing that is required for this module.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlcdltm.c_v   1.9   08 May 2002 19:13:34   tliou  $
$Header: //components/rel/tdscdma.mpss/4.3/rlc/src/tdsrlcdltm.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */


#include "tdsrlcdltm.h"

/* =======================================================================
**                            Function Definitions
** ======================================================================= */




/*===========================================================================

FUNCTION tdsrlci_enh_dl_reassemble_tm_pdus

DESCRIPTION
  Enqueues the Downlink Transparent Mode PDUs that arrive from MAC into 
  the downlink queue for that channel and calls a callback function, if 
  the higher layer has registered for it.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_enh_dl_reassemble_tm_pdus(tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr, 
                                    tds_complete_pdu_type *complete_pdu_ptr)
{
  tdsl1_dec_iovec_type *iovec_ptr = &(complete_pdu_ptr->iovec);
  
  /* If Water mark is not registered, drop the PDU and return */
  if (ctrl_blk_ptr->dl_wm_ptr == NULL)
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR,"RLC_ERR: WM Not registered - Dropping the PDU");
    if (iovec_ptr->ptr != NULL)
    {
      dsm_free_packet(&(iovec_ptr->ptr));
    }
    return;
  }
  
  /*-------------------------------------------------------------------------
    Assign the pdu size(in bits) to the app field of the dsm_item_type 
    structure. Enqueue the dsm item on the watermark queue and call the 
    callback function (if registered).
  -------------------------------------------------------------------------*/
  if (iovec_ptr->ptr != NULL)
  {

    /*-----------------------------------------------------------------------
      If the processing is in task context, overwrite the app field, else
      write the pdu length to the 16 Least significant bits, as the other
      bits may be used by some other app
    -----------------------------------------------------------------------*/
    if (ctrl_blk_ptr->context == FALSE)
    {
      iovec_ptr->ptr->app_field = iovec_ptr->length;
    }
    else
    {
      iovec_ptr->ptr->app_field &= 0xFFFF0000;
      iovec_ptr->ptr->app_field |= (uint32)iovec_ptr->length;
    }
    if (tdsrlc_debug_var & TDSRLC_DBG_DL_TM)
    {
      MSG_9(MSG_SSID_TDSCDMA_RLC, MSG_LEGACY_HIGH,
              "TM DL PDU Payload: 0x%x  0x%x  0x%x pdu len %d",
               tdsrlc_copy_to_word(iovec_ptr->ptr->data_ptr), 
               tdsrlc_copy_to_word(iovec_ptr->ptr->data_ptr+4), 
               tdsrlc_copy_to_word(iovec_ptr->ptr->data_ptr+8), iovec_ptr->ptr->app_field, 0, 0, 0, 0, 0);
    }
    dsm_enqueue(ctrl_blk_ptr->dl_wm_ptr, &(iovec_ptr->ptr));
    if (ctrl_blk_ptr->rlc_post_rx_func_ptr != NULL)
    {
      ctrl_blk_ptr->rlc_post_rx_func_ptr(complete_pdu_ptr->rlc_id, 1, 
                                  ctrl_blk_ptr->rlc_post_rx_func_ptr_para);

    }
  }
  else
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: NULL TM PDU,LC %d",complete_pdu_ptr->rlc_id);
  }
}


