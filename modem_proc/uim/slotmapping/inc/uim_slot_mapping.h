#ifndef UIM_SLOT_MAPPING_H
#define UIM_SLOT_MAPPING_H
/*===========================================================================

                         U I M _ S L O T _ M A P P I N G . H

DESCRIPTION

The uim slot mapping handling header file.

Copyright (c) 2017 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/slotmapping/inc/uim_slot_mapping.h#1 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
04/03/17    ks    Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "uim_p.h"
#include "uim_common_efs.h"


/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================
FUNCTION UIM_SLOT_MAPPING_INIT

DESCRIPTION
  Initializes the module that is used for slot mapping

PARAMETERS
  None
  
RETURN VALUE
  None

DEPENDENCIES
  QMI UIM service must be initialized and registered with Framework

SIDE EFFECTS
  None

===========================================================================*/

extern void uim_slot_mapping_init
(
  void
);

/*===========================================================================
FUNCTION UIM_SLOT_MAPPING_SIG_HANDLER

DESCRIPTION
  UIM slot mapping signal handler for MMGSDI task signals

PARAMETERS
  None.

  Returns TRUE if signal should be cleared from the set signal mask,
  FALSE if further signal processing is needed and hence signal
  should not be cleared.
===========================================================================*/
extern boolean uim_slot_mapping_sig_handler
(
  void
);
#endif /* UIM_SLOT_MAPPING_H */
