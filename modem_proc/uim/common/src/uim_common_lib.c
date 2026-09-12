/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


             U I M   C O M M O N   L I B R A R Y


GENERAL DESCRIPTION

  This file contains the common Library functions that all
  UIM modules can call.

EXTERNALIZED FUNCTIONS

uim_common_get_modem_rat_capabilities
  Retrieves the modem tech capability (i.e. RATs supported)


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/common/src/uim_common_lib.c#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/04/19   vdc     Calculate data length of BPP as per ISO/IEC 8825-1
03/18/19   vdc     Fix overflow in uim_common_util_alloc_base64string_to_bin
05/07/18   dd      Fix overflow in uim_common_util_alloc_base64string_to_bin
05/07/18   dd      Remote SIM Unlock support to switch configuration
01/19/17   vdc     Added wrapper function to commit log message
07/27/16   shr     Initial revision

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_common_lib.h"
#include "uim_msg.h"
#include "policyman.h"
#include "hwio_cap.h"
#include "log.h"
#include "modem_mem.h"
#include <stringl/stringl.h>

/*===========================================================================
FUNCTION:      UIM_COMMON_GET_MODEM_RAT_CAPABILITIES

DESCRIPTION:
  Retrieves the RATs supported by the modem

PARAMETERS:
  None

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
 uint32: Mask of RATs supported
         GSM Mask     -> 0x01
         1X Mask      -> 0x02
         WCDMA Mask   -> 0x04
         HSPA Mask    -> 0x08
         TDSCDMA Mask -> 0x10
         LTE Mask     -> 0x20

SIDE EFFECTS:
  None
=============================================================================*/
uint32 uim_common_get_modem_rat_capabilities
(
  void
)
{
  static boolean      is_modem_cap_init;
  static uint32       modem_cap;
  uint32              rat_mask = 0;

  if(is_modem_cap_init)
  {
    return modem_cap;
  }

  /*-----------------------------------------------------------------------------
    Get RATs that are enabled 
    NOTES:
    1. mcs_modem_has_capability() need not be called seperately for any RATs - 
       policyman_get_hardware_rats_bands() already takes that into account.
    2. policyman_get_hardware_rats_bands() takes band availability into account
       when reporting RATs (removing RATs that have no available bands), so NULL
       can be passed for all the band pointers and only look only at the RATs
       that are returned.
  -----------------------------------------------------------------------------*/
  policyman_get_hardware_rats_bands(&rat_mask, NULL, NULL, NULL);

#ifdef FEATURE_GSM
  if(rat_mask & SYS_SYS_MODE_MASK_GSM)
  {
    modem_cap = modem_cap | UIM_MODEM_CAP_FEATURE_GSM_MASK;
  }
#endif /* FEATURE_GSM */

#ifdef FEATURE_CDMA
  if(rat_mask & SYS_SYS_MODE_MASK_CDMA)
  {
    modem_cap = modem_cap | UIM_MODEM_CAP_FEATURE_1X_MASK;
  }
#endif /* FEATURE_CDMA */

#ifdef FEATURE_WCDMA
  if(rat_mask & SYS_SYS_MODE_MASK_WCDMA)
  {
    modem_cap = modem_cap | UIM_MODEM_CAP_FEATURE_WCDMA_MASK;
  }
#endif /* FEATURE_WCDMA */

#ifdef FEATURE_HSDPA
  if((mcs_modem_has_capability(MCS_MODEM_CAPABILITY_FEATURE_HSPA) ==
      MCS_MODEM_CAP_AVAILABLE) &&
     (rat_mask & SYS_SYS_MODE_MASK_WCDMA))
  {
    modem_cap = modem_cap | UIM_MODEM_CAP_FEATURE_HSPA_MASK;
  }
#endif /* FEATURE_HSDPA */

#ifdef FEATURE_TDSCDMA
  if(rat_mask & SYS_SYS_MODE_MASK_TDS)
  {
    modem_cap = modem_cap | UIM_MODEM_CAP_FEATURE_TDSCDMA_MASK;
  }
#endif /* FEATURE_TDSCDMA */

#ifdef FEATURE_LTE
  if(rat_mask & SYS_SYS_MODE_MASK_LTE)
  {
    modem_cap = modem_cap | UIM_MODEM_CAP_FEATURE_LTE_MASK;
  }
#endif /* FEATURE_LTE */

  is_modem_cap_init = TRUE;

  UIM_MSG_LOW_1("Modem Capabilities: 0x%x", modem_cap);

  return modem_cap;
} /* uim_common_get_modem_rat_capabilities */


/*===========================================================================
   FUNCTION:      UIM_LOG_PACKET_COMMIT

   DESCRIPTION:
     This function commit the log packet

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
    Void

   SIDE EFFECTS:
     None
=============================================================================*/
void uim_log_packet_commit(
  sys_modem_as_id_e_type   as_id,
  void                    *log_ptr
)
{
  (void)as_id;

  log_commit(log_ptr);
} /* uim_log_packet_commit */


/* ==========================================================================
FUNCTION UIM_COMMON_UTIL_FIND_BASE64_VALUES

DESCRIPTION:
  This is an utility function which find the base64 value for the input character

DEPENDENCIES:
  None

RETURN VALUE:
  gba_result_enum_type

SIDE EFFECTS:
  None
==========================================================================*/
static uint8 uim_common_util_find_base64_values
(
  boolean  find_char,
  char     input_char,
  uint8    input_index
)
{
  const char base64_table[] =
    {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
  const char * value_ptr = NULL;

  if (find_char)
  {
    value_ptr = strchr(base64_table, input_char);
    if (value_ptr)
    {
      return (uint8)(value_ptr - base64_table);
    }
  }
  else
  {
    if (input_index < (sizeof(base64_table)/sizeof(base64_table[0])))
    {
      return (uint8)base64_table[input_index];
    }
  }

  return 0;
} /* uim_common_util_find_base64_values */


/* ==========================================================================
FUNCTION UIM_COMMON_UTIL_ALLOC_BASE64STRING_TO_BIN

DESCRIPTION:
  Allocates memory and converts a Base64 encoded ASCII string to a binary
  datastream.

DEPENDENCIES:
  None

RETURN VALUE:
  uint8 *

SIDE EFFECTS:
  None
==========================================================================*/
uint8* uim_common_util_alloc_base64string_to_bin (
  const char                *input_ptr,
  uint32                     input_str_len,
  uint32                    *output_len_ptr
)
{
  uint32  i           = 0;
  uint32  j           = 0;
  uint32  output_len  = 0;
  uint8  *output_ptr  = NULL;

  if (input_ptr == NULL || output_len_ptr == NULL)
  {
    return NULL;
  }

  if ((input_str_len == 0) ||
      (input_str_len % 4) != 0)
  {
    UIM_MSG_ERR_1("Invalid Base64 string length: 0x%x", input_str_len);
    return NULL;
  }

  output_len = (input_str_len / 4) * 3;

  /* Update output buffer size if input was padded with '='s
     Only 2 '=' padded bytes are allowed per 4 byte-block */
  if (input_ptr[input_str_len - 1] == '=')
  {
    output_len--;
    if (input_ptr[input_str_len - 2] == '=')
    {
      output_len--;
    }
  }

  output_ptr = modem_mem_calloc(1, sizeof(uint8) * output_len, MODEM_MEM_CLIENT_UIM);
  if (output_ptr == NULL)
  {
    return NULL;
  }

  memset(output_ptr, 0x00, output_len);

  /* Decode the string & convert to binary */
  while ((i+3) < input_str_len)
  {
    /* Decode each block of 4 Base64 bytes to 3 binary bytes */
    uint32 first  = uim_common_util_find_base64_values(TRUE, input_ptr[i++], 0);
    uint32 second = uim_common_util_find_base64_values(TRUE, input_ptr[i++], 0);
    uint32 third  = uim_common_util_find_base64_values(TRUE, input_ptr[i++], 0);
    uint32 fourth = uim_common_util_find_base64_values(TRUE, input_ptr[i++], 0);

    uint32 all_three = (first  << (3 * UIM_NUM_BITS_PER_BASE64_CHAR)) +
                       (second << (2 * UIM_NUM_BITS_PER_BASE64_CHAR)) +
                       (third  << (1 * UIM_NUM_BITS_PER_BASE64_CHAR)) +
                        fourth;

    /* 3 binary bytes */
    if(j >= output_len)
    {
      break;
    }
    output_ptr[j++] = (all_three >> (2 * UIM_NUM_BITS_PER_ASCII_CHAR)) & 0xFF;
    if(j >= output_len)
    {
      break;
    }
    output_ptr[j++] = (all_three >> (1 * UIM_NUM_BITS_PER_ASCII_CHAR)) & 0xFF;
    if(j >= output_len)
    {
      break;
    }
    output_ptr[j++] = all_three & 0xFF;
  }

  *output_len_ptr = output_len;
  return output_ptr;
} /* uim_common_util_alloc_base64string_to_bin */


/* ==========================================================================
FUNCTION UIM_COMMON_UTIL_ALLOC_BIN_TO_BASE64STRING

DESCRIPTION:
  Allocates memory and converts a binary datastream to a Base64 encoded
  ASCII string format. the output string is null terminated

DEPENDENCIES:
  None

RETURN VALUE:
  char*

SIDE EFFECTS:
  None
==========================================================================*/
char* uim_common_util_alloc_bin_to_base64string (
  const uint8                       *input_ptr,
  uint32                             input_len
)
{
  uint32  i                = 0;
  uint32  j                = 0;
  uint8   extra_bytes      = 0;
  uint32  output_len       = 0;
  char  * output_ptr       = NULL;

  if ((input_ptr == NULL) || (input_len == 0))
  {
    UIM_MSG_ERR_1("Invalid input parameters: input_len 0x%x", input_len);
    return NULL;
  }

  /* Calculate the max buffer size needed for the encoded Base64 string,
     3 binary bytes make 4 Base64 bytes */
  output_len =  sizeof(char) + ((((input_len % 3 > 0) ? 1 : 0) +
                                  (input_len / 3 )) * 4);

  output_ptr = modem_mem_calloc(1, sizeof(char) * (output_len + 1), MODEM_MEM_CLIENT_UIM);
  if (output_ptr == NULL)
  {
    return NULL;
  }

  memset(output_ptr, 0, output_len + 1);

  /* Now encode the ASCII string to Base64 string */
  while (i < input_len)
  {
    /* Encode each block of 4 bytes from 3 ASCII bytes */
    uint32 first  = i < input_len ? input_ptr[i++] : 0;
    uint32 second = i < input_len ? input_ptr[i++] : 0;
    uint32 third  = i < input_len ? input_ptr[i++] : 0;

    uint32 all_three = (first  << (2 * UIM_NUM_BITS_PER_ASCII_CHAR)) +
                       (second << (1 * UIM_NUM_BITS_PER_ASCII_CHAR)) +
                        third;

    /* 4 Base64 bytes */
    if ((j+3) < output_len)
    {
      output_ptr[j++] = uim_common_util_find_base64_values(
                          FALSE, 0, (all_three >> (3 * UIM_NUM_BITS_PER_BASE64_CHAR)) & 0x3F);
      output_ptr[j++] = uim_common_util_find_base64_values(
                          FALSE, 0, (all_three >> (2 * UIM_NUM_BITS_PER_BASE64_CHAR)) & 0x3F);
      output_ptr[j++] = uim_common_util_find_base64_values(
                          FALSE, 0, (all_three >> (1 * UIM_NUM_BITS_PER_BASE64_CHAR)) & 0x3F);
      output_ptr[j++] = uim_common_util_find_base64_values(
                          FALSE, 0, all_three & 0x3F);
    }
  }

  /* Update pading if required. It is needed if ASCII string's
     last group has either 1 or 2 bytes */
  extra_bytes = input_len % 3;
  if (extra_bytes)
  {
    uint8 bytes_to_fill = (extra_bytes == 1) ? 2 : 1;
    for (i = 1; i < 3 && bytes_to_fill; i++, --bytes_to_fill)
    {
      output_ptr[output_len - 1 - i] = '=';
    }
  }
  return output_ptr;
} /* uim_common_util_alloc_bin_to_base64string */


/* =============================================================================
FUNCTION:      UIM_COMMON_UTIL_CALC_NUMBER_POWER

DESCRIPTION:
  This function calculates the power of a given number

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  TRUE: Calculated power of a given number successfully
  FALSE: Calculation of power of a given number has failed

SIDE EFFECTS:
  None

CRITICAL SECTIONS:
  None
=============================================================================*/
static boolean uim_common_util_calc_number_power(
  uint16   base,
  uint8    exponent,
  uint64  *result_ptr
)
{
  if(result_ptr == NULL)
  {
    return FALSE;
  }

  /* Initialize result to 1 */
  *result_ptr = 0x01;

  if(base == 0 && exponent == 0)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }
  else if(exponent == 0)
  {
    *result_ptr = 0x01;
  }
  else if(base == 0)
  {
    *result_ptr = 0x00;
  }
  else
  {
    while(exponent != 0)
    {
      *result_ptr = *result_ptr * base;
      exponent--;
    }
  }

  return TRUE;
} /* uim_common_util_calc_number_power */


/* =============================================================================
FUNCTION:      UIM_COMMON_CALC_DATA_OBJECT_LEN

  DESCRIPTION:
    Calculates the data object length as per ISO/IEC 8825-1

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     TRUE: Data object length is calculated successfully
     FALSE: Data object length calculation has failed

   SIDE EFFECTS:
     None
=============================================================================*/
boolean uim_common_calc_data_object_len(
  const uim_common_data_type  *common_data_ptr,
  uint16                      *common_data_index_ptr,
  uint16                      *data_object_next_tag_index_ptr
)
{
  uint8  length_octet_count = 0x00;
  uint8  length_octet_index = 0x00;
  uint16 data_object_length = 0x00;

  if(common_data_ptr == NULL ||
     common_data_index_ptr == NULL)
  {
    return FALSE;
  }

  UIM_MSG_HIGH_2("Data length:0x%x, Data index: 0x%x",
                 common_data_ptr->data_len,
                 *common_data_index_ptr);

  /* The length is calculated according to ISO/IEC 8825-1.
     If MSB of length byte is set to 0, remaining 7 bits denotes the length of
     TLV data bytes. If MSB of length byte is set to 1, remaining 7 bits
     denotes number of length octets next to it.*/
  if(*common_data_index_ptr < common_data_ptr->data_len)
  {
    UIM_MSG_HIGH_1("Length bytes of length: 0x%x", common_data_ptr->data_ptr[*common_data_index_ptr]);

    if((common_data_ptr->data_ptr[*common_data_index_ptr] & 0x80) != 0x80)
    {
      data_object_length = common_data_ptr->data_ptr[*common_data_index_ptr];
      (*common_data_index_ptr)++;

      if(data_object_next_tag_index_ptr != NULL)
      {
        *data_object_next_tag_index_ptr = *common_data_index_ptr + data_object_length;
      }
    }
    else
    {
      uint64   result = 0x00;

      length_octet_count = common_data_ptr->data_ptr[*common_data_index_ptr] & 0x7F;
      (*common_data_index_ptr)++;

      for(length_octet_index = length_octet_count;
          length_octet_index > 0 &&
            *common_data_index_ptr < common_data_ptr->data_len;
          length_octet_index--)
      {
        if(FALSE == uim_common_util_calc_number_power(0x100,
                                                      length_octet_index - 1,
                                                      &result))
        {
          return FALSE;
        }

        data_object_length = data_object_length + 
           (common_data_ptr->data_ptr[*common_data_index_ptr] * result);
        (*common_data_index_ptr)++;
      }

      if(data_object_next_tag_index_ptr != NULL)
      {
        *data_object_next_tag_index_ptr = *common_data_index_ptr + data_object_length;
      }
    }
  }
  else
  {
    UIM_MSG_ERR_0("Calculation of data object length failed");
    return FALSE;
  }

  return TRUE;
} /* uim_common_calc_data_object_len */