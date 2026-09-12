#ifndef RFNV_WCDMA_800_H
#define RFNV_WCDMA_800_H

/*!
  @file
  rfnv_wcdma_800.h

  @brief

  @details
  This header file contains definitions to interface with RF NV for WCDMA
  800.

*/

/*===========================================================================

Copyright (c) 2008-2013 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:08:47 $ $Author: pwbldsvc $
$Header: //components/rel/rftech_wcdma.mpss/3.11/rf/nv/inc/rfnv_wcdma_800.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/10/14   rmb     Removed apt_commit API which is unused 
07/08/13   kcj     Can't use designated initialization with OFT's compiler. 
06/05/13   kcj     Dynamically allocated NV struct memory. 
09/30/10   ac      efs support
08/13/09   ckl     Merge diversity NV items into main NV item list.
03/15/09   ckl     Deleted rfnv.h
03/09/09   ckl     Code cleanup
05/05/04   dhh     Ported from rfnv_wcdma_1900.h.

===========================================================================*/

/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "rfnv_wcdma_types.h"

/*===========================================================================

                          PUBLIC DATA DECLARATIONS

===========================================================================*/

extern rfnv_wcdma_nv_items_list rfnv_wcdma_800_nv_items_list;

/*===========================================================================

                            FUNCTION PROTOTYPES

===========================================================================*/

/*--------------------------------------------------------------------------*/
void rfnv_wcdma_800_init_nv_item_list(void);

#endif /* RFNV_WCDMA_800_H */
