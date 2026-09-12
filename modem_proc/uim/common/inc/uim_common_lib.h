#ifndef UIM_COMMON_LIB_H
#define UIM_COMMON_LIB_H
/*===========================================================================


           U I M   C O M M O N   L I B R A R Y   H E A D E R S


GENERAL DESCRIPTION

This is the header file that provides definition for routines that
can be called by all UIM modules.



EXTERNALIZED FUNCTIONS

uim_common_get_modem_rat_capabilities
  Retrieves the modem tech capability (i.e. RATs supported)


INITIALIZATION AND SEQUENCING REQUIREMENTS

===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/common/inc/uim_common_lib.h#3 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
12/04/19   vdc     Calculate data length of BPP as per ISO/IEC 8825-1
05/07/18   dd      Fix overflow in uim_common_util_alloc_base64string_to_bin
05/07/18   dd      Remote SIM Unlock support to switch configuration
06/07/17   me      Added support for the CIOT LTE EFS with CAT1 and NB1 values 
01/19/17   vdc     Added wrapper function to commit log message
07/27/16   shr     Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/

#include "comdef.h"
#include "sys.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/

/* Masks to use to interpret uim_common_get_modem_rat_capabilities()
   return value */
#define UIM_MODEM_CAP_FEATURE_GSM_MASK              0x00000001
#define UIM_MODEM_CAP_FEATURE_1X_MASK               0x00000002
#define UIM_MODEM_CAP_FEATURE_WCDMA_MASK            0x00000004
#define UIM_MODEM_CAP_FEATURE_HSPA_MASK             0x00000008
#define UIM_MODEM_CAP_FEATURE_TDSCDMA_MASK          0x00000010
#define UIM_MODEM_CAP_FEATURE_LTE_MASK              0x00000020
#define UIM_MODEM_CAP_FEATURE_LTE_M1_MASK           0x00000040
#define UIM_MODEM_CAP_FEATURE_LTE_NB1_MASK          0x00000080

#define UIM_NUM_BITS_PER_BASE64_CHAR                6
#define UIM_NUM_BITS_PER_ASCII_CHAR                 8

/* ----------------------------------------------------------------------------
   STRUCTURE:    UIM_COMMON_DATA_TYPE
-----------------------------------------------------------------------------*/
/** @brief Generic data structure.
*/
typedef struct {
  uint32     data_len;    /**< Length of the data. */
  uint8     *data_ptr;    /**< Pointer to the data. */
} uim_common_data_type;

/*=============================================================================

                           FUNCTION DECLARATIONS

=============================================================================*/
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
         LTE M1 Mask  -> 0x40
         LTE NB1 Mask -> 0x80

SIDE EFFECTS:
  None
=============================================================================*/
uint32 uim_common_get_modem_rat_capabilities
(
  void
);

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
);

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
);

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
);

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
);
#endif /* UIM_COMMON_LIB_H */
