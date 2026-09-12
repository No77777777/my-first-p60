
#ifndef DS3G_IPA_HDLR_H
#define DS3G_IPA_HDLR_H
/*===========================================================================

                3 G   D A T A   S E R V I C E S   I P A   H D L R

                           H E A D E R   F I L E

GENERAL DESCRIPTION
  This software unit contains functions for DS to directly call APIs to IPA

  Copyright (c) 2017-2017 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:
  $Header:

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/20/17   mvb     Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "queue.h"

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/



/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
#ifdef FEATURE_DATA_TSRC
/*=============================================================================
FUNCTION      DS3G_IPA_TSRC_STRETCH_INFO_INDICATION

DESCRIPTION   This function sends the TSRC Stretch information needed to IPA

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
=============================================================================*/
void ds3g_ipa_hdlr_tsrc_stretch_info_indication
(
   uint32  stretch_val,
   uint32  default_stretch_ratio
);
#endif /* FEATURE_DATA_TSRC */

#endif /* DS3G_IPA_HDLR_H */
