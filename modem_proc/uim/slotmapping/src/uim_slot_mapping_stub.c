/*===========================================================================

                         U I M _ S L O T _ M A P P I N G _ S T U B . C

DESCRIPTION

 Stubs file for the uim slot mapping handling source file.

Copyright (c) 2017 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/slotmapping/src/uim_slot_mapping_stub.c#1 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
04/03/17    ks    Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "comdef.h"

/*===========================================================================

                             FUNCTION DEFINATIONS 

===========================================================================*/

/*===========================================================================
FUNCTION SLOT_MAPPING_INIT()

DESCRIPTION
  Slot mapping initialisation

PARAMETERS
  None
  
RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_slot_mapping_init
(
  void
)
{
  return;
}/* uim_slot_mapping_init */


/*===========================================================================
FUNCTION SLOT_MAPPING_SIG_HANDLER

DESCRIPTION
  QMI UIM signal handler for MMGSDI task signals

PARAMETERS
  None.

  Returns TRUE if signal should be cleared from the set signal mask,
  FALSE if further signal processing is needed and hence signal
  should not be cleared.
===========================================================================*/
boolean uim_slot_mapping_sig_handler
(
  void
)
{
  return TRUE;
} /* uim_slot_mapping_sig_handler */
