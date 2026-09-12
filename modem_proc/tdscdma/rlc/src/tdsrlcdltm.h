#ifndef TDSRLCDLTM_H
#define TDSRLCDLTM_H
/*===========================================================================
                  D O W N L I N K   T R A N S P A R E N T   M O D E   

              R A D I O   L I N K   C O N T R O L : H E A D E R  F I L E

DESCRIPTION
  This file contains function prototypes associated with the
  Downlink Transparent Mode RLC module.
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlcdltm.h_v   1.3   22 May 2001 14:28:56   rchar  $
$Header: //components/rel/tdscdma.mpss/4.3/rlc/src/tdsrlcdltm.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/

#include "tdsrlcdl.h"
#include "tdsmacrlcif.h"


/*===========================================================================

FUNCTION tdsrlci_dl_reassemble_tm_pdus

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
void tdsrlci_dl_reassemble_tm_pdus
(       
  tdsrlci_dl_ctrl_blk_type *ctrl_blk, /* Pointer to Downlink Control block    */
  dsm_item_type         *pdu_ptr,  /* Packet Data Unit                     */
  uint16                pdu_len,   /* PDU length                           */
  rlc_lc_id_type        rlc_id     /* RLC logical channel ID               */
);

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
                                    tds_complete_pdu_type *complete_pdu_ptr);

#endif
