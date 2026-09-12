/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            G B A   U T I L   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the GBA utility functions.

                        COPYRIGHT INFORMATION

Copyright (c) 2015, 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/07/18   dd      Remote SIM Unlock support to switch configuration
11/16/15   yt      Support for second BSF and key status events
06/10/15   tkl     Add support of IPv6 BSF address
05/12/15   tkl     Integrated review comment
05/05/15   lxu     Fixed various issues
03/22/15   tkl     Add TMPI support
03/16/15   lxu     Use gba_url and gba_port in nv if they are valid
01/26/15   tl      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include <stringl/stringl.h>
#include "string.h"
#include <ctype.h>
#include "mmgsdilib_common.h"

#include "gba.h"
#include "gba_lib.h"
#include "gba_platform.h"
#include "gba_util.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#define GBA_MAX_ITOA_LEN                 6
#define GBA_DECIMAL_BASE                 10

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION GBA_UTIL_GET_SESSION_INDEX_AND_MMGSDI_SESSION_TYPE

DESCRIPTION
  This is a utility function gets the session index corresponding to a
  session type and convert gba session type to mmgsdi session type

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  gba_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
gba_result_enum_type gba_util_get_session_index_and_mmgsdi_session_type(
  gba_session_type                     session_type,
  uint8                              * session_index_ptr,
  mmgsdi_session_type_enum_type      * mmgsdi_session_type_ptr
)
{
  mmgsdi_session_type_enum_type mmgsdi_session_type = MMGSDI_GW_PROV_PRI_SESSION;
  uint8                         index               = 0;

  if(mmgsdi_session_type_ptr == NULL &&
     session_index_ptr == NULL)
  {
    return GBA_GENERIC_ERROR;
  }

  switch (session_type)
  {
    case GBA_3GPP_PROV_SESSION_PRI:
      mmgsdi_session_type = MMGSDI_GW_PROV_PRI_SESSION;
      index = 0;
      break;
    case GBA_3GPP_PROV_SESSION_SEC:
      mmgsdi_session_type = MMGSDI_GW_PROV_SEC_SESSION;
      index = 1;
      break;
    case GBA_NON_PROV_SESSION_SLOT_1:
      mmgsdi_session_type = MMGSDI_NON_PROV_SESSION_SLOT_1;
      index = 2;
      break;
    case GBA_NON_PROV_SESSION_SLOT_2:
      mmgsdi_session_type = MMGSDI_NON_PROV_SESSION_SLOT_2;
      index = 3;
      break;
    default:
      GBA_MSG_LOW_1("GBA session type: 0x%x not supported", session_type);
      return GBA_GENERIC_ERROR;
  }

  if (mmgsdi_session_type_ptr != NULL)
  {
    *mmgsdi_session_type_ptr = mmgsdi_session_type;
  }

  if (session_index_ptr != NULL)
  {
    *session_index_ptr = index;
  }

  return GBA_SUCCESS;
} /* gba_util_get_session_index_and_mmgsdi_session_type */


/*===========================================================================
FUNCTION GBA_UTIL_SESSION_INDEX_TO_SESSION_TYPE

DESCRIPTION
  This is a utility function converts the session index to GBA session type

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  gba_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
gba_result_enum_type gba_util_session_index_to_session_type(
  uint8                                session_index,
  gba_session_type                   * session_type_ptr
)
{
  if (session_type_ptr == NULL)
  {
    return GBA_GENERIC_ERROR;
  }

  switch (session_index)
  {
    case 0:
      *session_type_ptr = GBA_3GPP_PROV_SESSION_PRI;
      break;
    case 1:
      *session_type_ptr = GBA_3GPP_PROV_SESSION_SEC;
      break;
    case 2:
      *session_type_ptr = GBA_NON_PROV_SESSION_SLOT_1;
      break;
    case 3:
      *session_type_ptr = GBA_NON_PROV_SESSION_SLOT_2;
      break;
    default:
      return GBA_GENERIC_ERROR;
  }

  return GBA_SUCCESS;
} /* gba_util_session_index_to_session_type */


/* ==========================================================================
FUNCTION GBA_UTIL_TRIM_SPACES

DESCRIPTION:
  This is an utility function that remove leading/trailing spaces and modify
  the input string. If the input string is allocated dynamically, caller
  should not overwrite that pointer with the returned value. Also, caller
  should not deallocate the return pointer.

DEPENDENCIES:
  None

RETURN VALUE:
  char* pointer to the string. NULL if fail to trim the string.

SIDE EFFECTS:
  trialing spaces of the input string will be trimmed, if input string has
  only space, then trimmed str = "\0".
==========================================================================*/
char* gba_util_trim_spaces
(
  char   * str_ptr
)
{
  char *end_ptr = 0;

  if(str_ptr == NULL)
  {
    return NULL;
  }

  /* Trim leading space*/
  while(isspace(str_ptr[0]))
  {
    str_ptr++;
  }

  /* str_ptr has all spaces */
  if(str_ptr[0] == '\0')
  {
    return NULL;
  }

  /* Trim tailing spaces */
  end_ptr = str_ptr + strlen(str_ptr) - 1;

  while(end_ptr > str_ptr && isspace(end_ptr[0]))
  {
    end_ptr--;
  }
  end_ptr[1] = '\0';

  return str_ptr;
} /* gba_util_trim_spaces */


/*===========================================================================
FUNCTION GBA_UTIL_ITOA

DESCRIPTION
  This function converts an integer to ASCII.
  The resulting string is null terminated.

DEPENDENCIES
  The length of the returned string depends on the input_value and
  the output_size. The array pointed by 'output_string' must be
  large enough for returned string(at most equal to output_size).

RETURN VALUE
  gba_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
gba_result_enum_type gba_util_itoa
(
  uint16  input_value,
  char   *output_string_ptr,
  uint8   output_size
)
{
  char   buf[GBA_MAX_ITOA_LEN] = {'\0'};
  uint8  residue               = 0x0;
  uint8  buf_index             = sizeof(buf) - 1;

  if (output_string_ptr == NULL ||
      output_size <= 1)
  {
    return GBA_GENERIC_ERROR;
  }

  do
  {
    residue = (uint8) (input_value % GBA_DECIMAL_BASE);

    if (buf_index <= 0)
    {
      return GBA_GENERIC_ERROR;
    }

    buf[--buf_index] = (char) ( residue + '0');
  } while ( (input_value /= GBA_DECIMAL_BASE) > 0);

  while ( (*output_string_ptr++ = buf[buf_index++]) != 0)
  {
    output_size--;

    if (output_size <= 0)
    {
      return GBA_GENERIC_ERROR;
    }
  }

  return GBA_SUCCESS;
} /*  gba_util_itoa */


/*===========================================================================
FUNCTION GBA_UTIL_BSF_TYPE_TO_BSF_INDEX

DESCRIPTION
  This is a utility function converts the BSF type to BSF index

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  uint8

SIDE EFFECTS
  None
===========================================================================*/
uint8 gba_util_bsf_type_to_bsf_index(
  gba_bsf_enum_type                    bsf_type
)
{
  uint8 bsf_index = 0;

  switch (bsf_type)
  {
    case GBA_BSF_DEFAULT:
      bsf_index = 0;
      break;
    case GBA_BSF_NON_DEFAULT:
      bsf_index = 1;
      break;
  }

  return bsf_index;
} /* gba_util_bsf_type_to_bsf_index */


/*===========================================================================
FUNCTION GBA_UTIL_BSF_INDEX_TO_BSF_TYPE

DESCRIPTION
  This is a utility function converts the BSF index to BSF type

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  gba_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
gba_result_enum_type gba_util_bsf_index_to_bsf_type(
  uint8                                bsf_index,
  gba_bsf_enum_type                  * bsf_type_ptr
)
{
  if (bsf_type_ptr == NULL)
  {
    return GBA_GENERIC_ERROR;
  }

  switch (bsf_index)
  {
    case 0:
      *bsf_type_ptr = GBA_BSF_DEFAULT;
      return GBA_SUCCESS;
    case 1:
      *bsf_type_ptr = GBA_BSF_NON_DEFAULT;
      return GBA_SUCCESS;
    default:
      return GBA_GENERIC_ERROR;
  }
} /* gba_util_bsf_index_to_bsf_type */

