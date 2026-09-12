#ifndef TDSRLCULTM_H
#define TDSRLCULTM_H
/*===========================================================================
                  U P L I N K   T R A N S P A R E N T   M O D E   

            R A D I O   L I N K   C O N T R O L  : H E A D E R   F I L E 

DESCRIPTION
  This file contains data types and function prototypes associated with the
  Uplink Transparent Mode RLC module.
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlcultm.h_v   1.3   28 Jun 2001 11:15:28   rchar  $
$Header: //components/rel/tdscdma.mpss/4.3/rlc/src/tdsrlcultm.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/24/10    guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/

#include "tdsrlcul.h"
#include "tdsrlcrrcif.h"
#include "tdsrlcdsapif.h"
#include "tdsmacrlcif.h"


/*===========================================================================

FUNCTION RLCI_GET_TM_BUFFER_STATUS

DESCRIPTION
  Calculates the number of SDUs and size of the SDU. MAC uses this 
  information to determine the number of PDUs to give to the channel.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_get_tm_buffer_status
(
  tdsrlci_ul_ctrl_blk_type     *ctrl_blk,  /* Pointer to Uplink Control Block */
  tdsrlc_ul_lc_buf_status_type *status_ptr /* Pointer for buffer status       */
);



/*===========================================================================

FUNCTION rlci_ul_build_tm_pdus

DESCRIPTION
  Builds Uplink Transparent Mode PDUs.
  Gets the items from the Queue and sends it down to MAC.
  When segmentation is not performed, the SDU size should be equal to the 
  PDU size.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_build_tm_pdus
(       
  tdsrlci_ul_ctrl_blk_type      *ctrl_blk, /* Pointer to UL TM control Block  */
  tdsrlc_ul_logchan_format_type *chan_fmt, /* PDU information                 */
  tdsrlc_ul_logchan_data_type   *tdsdata_ptr  /* Pointer for storing PDUs        */
);


#endif
