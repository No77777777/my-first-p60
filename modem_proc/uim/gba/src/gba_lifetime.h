#ifndef GBA_LIFETIME_H
#define GBA_LIFETIME_H
/*===========================================================================


            G B A   L I F E T I M E   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2015 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/16/15   yt      Support for second BSF and key status events
01/26/15   tl      Initial revision
===========================================================================*/

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION GBA_LIFETIME_IS_KS_EXPIRED

DESCRIPTION
  Function verifies the input lifetime has expired.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None
===========================================================================*/
boolean gba_lifetime_is_ks_expired (
  gba_lifetime_type lifetime
);

/*===========================================================================
FUNCTION GBA_LIFETIME_GET_REMAINING_TIME

DESCRIPTION
  Function gets the remaining lifetime in seconds.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  uint32

SIDE EFFECTS
  None
===========================================================================*/
uint32 gba_lifetime_get_remaining_time (
  gba_lifetime_type lifetime
);

#endif /* GBA_LIFETIME_H */

