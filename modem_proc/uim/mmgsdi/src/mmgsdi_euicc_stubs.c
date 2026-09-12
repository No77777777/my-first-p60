/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


S T U B S   F O R   M M G S D I   E  U I C C   R E L A T E D  F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains stubs for routines used for eUICCs.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS



                        COPYRIGHT INFORMATION

Copyright (c) 2016 QUALCOMM Technologies, Inc (QTI) and
its licensors. All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_euicc_stubs.c#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/27/16   av      Don't do protocol switch if card returns 69 99 for ISDR select
07/06/16   av      Initial revision
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "uim_msg.h"
#include "err.h"
#include "mmgsdiutil.h"

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/* ============================================================================
FUNCTION MMGSDI_EUICC_IS_ISDR_FOUND

DESCRIPTION
  This function attemmpts to select ISDR (which is present on an eUICC, but not
  on a non-eUICC).

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_euicc_is_isdr_found(
  mmgsdi_slot_id_enum_type           slot_id,
  boolean                            *is_isdr_found_ptr
)
{
  (void) slot_id;

  if(is_isdr_found_ptr == NULL)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  *is_isdr_found_ptr = FALSE;

  return MMGSDI_SUCCESS;
} /* mmgsdi_euicc_is_isdr_found */


/*===========================================================================
FUNCTION MMMGSDI_EUICC_IS_ISDR_AID

DESCRIPTION
  This function is used to find out if the input AID is the ISDR AID or not.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None
===========================================================================*/
boolean mmgsdi_euicc_is_isdr_aid(
  const mmgsdi_static_data_type *aid_ptr
)
{
  (void) aid_ptr;

  return FALSE;
} /* mmgsdi_euicc_is_isdr_aid */

