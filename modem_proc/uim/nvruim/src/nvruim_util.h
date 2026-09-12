#ifndef NVRUIM_UTIL_H
#define NVRUIM_UTIL_H
/*===========================================================================


              N V    R - U I M    S U B S Y S T E M    T A S K


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016 QUALCOMM Technologies, Incorporated and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:

when       who     what, where, why
--------   ---     -----------------------------------------------------------
09/28/16   ar      Adding support for as_id in NVRUIM APIs for DS
03/15/16   ar      Initial revision
=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "customer.h"
#include "comdef.h"
#include "uim_msg.h"
#include "nvruimi.h"
#include "nvruim.h"
#include "nvruim_p.h"
#include <stringl/stringl.h>

#include "mmgsdilib_common.h"
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/

/*=============================================================================

                       STRUCTURE DECLARATIONS

=============================================================================*/

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION NVRUIM_CONVERT_SLOT_ID_TO_INDEX

DESCRIPTION
  Converts the passed MMGSDI slot enum to a slot array index.

DEPENDENCIES
  None

RETURN VALUE
  Boolean to indicate success or failure

SIDE EFFECTS
  None

===========================================================================*/
boolean nvruim_convert_slot_id_to_index
(
  mmgsdi_slot_id_enum_type    mmgsdi_slot_type,
  uint8                     * slot_index_ptr
);

/*===========================================================================

FUNCTION NVRUIM_CONVERT_AS_ID_TO_NVRUIM_SESSION

DESCRIPTION
  Converts the passed CM SUB_ID to NVRUIM session_type

DEPENDENCIES
  None

RETURN VALUE
  nvruim_session_enum_type

SIDE EFFECTS
  None

===========================================================================*/
nvruim_session_enum_type nvruim_convert_as_id_to_nvruim_session
(
  sys_modem_as_id_e_type        as_id
);

/*===========================================================================

FUNCTION NVRUIM_MAP_CONTEXT_TO_NVRUIM_SESSION

DESCRIPTION
  Converts the passed context to NVRUIM session_type

DEPENDENCIES
  None

RETURN VALUE
  nvruim_session_enum_type

SIDE EFFECTS
  None

===========================================================================*/
nvruim_session_enum_type nvruim_map_context_to_nvruim_session
(
  nvruim_nv_context_type   nvruim_context
);

/*===========================================================================

FUNCTION NVRUIM_CONVERT_SESSION_TO_AS_ID

DESCRIPTION
  Converts the passed NVRUIM session_type to CM as_id

DEPENDENCIES
  None

RETURN VALUE
  sys_modem_as_id_e_type

SIDE EFFECTS
  None

===========================================================================*/
sys_modem_as_id_e_type nvruim_convert_session_to_as_id
(
  nvruim_session_enum_type        nvruim_session
);

/*===========================================================================

FUNCTION NVRUIM_CONVERT_CONTEXT_TO_AS_ID

DESCRIPTION
  Converts the passed NVRUIM context to CM as_id

DEPENDENCIES
  None

RETURN VALUE
  sys_modem_as_id_e_type

SIDE EFFECTS
  None

===========================================================================*/
sys_modem_as_id_e_type nvruim_convert_context_to_as_id
(
  nvruim_nv_context_type              context
);

/*===========================================================================

FUNCTION NVRUIM_MAP_NVRUIM_SESSION_TO_CONTEXT

DESCRIPTION
  Converts the passed NVRUIM session_type to context

DEPENDENCIES
  None

RETURN VALUE
  uint16

SIDE EFFECTS
  None

===========================================================================*/
nvruim_nv_context_type nvruim_map_nvruim_session_to_context
(
  nvruim_session_enum_type       nvruim_session
);

#ifdef FEATURE_UIM_SUPPORT_LBS
/*===========================================================================

FUNCTION nvruim_get_ipv4_digit_char_string

DESCRIPTION
  IPv4 addresses are usually written in dot-decimal notation, which consists
  of the four octets of the address expressed in decimal and separated by
  periods. For example: 123.0.0.121.
  Data are stored in card as 8-bit ASCII string, where each character
   is store in one byte information. For example the given string is store as
   31 32 33 25 (123.)  First  Sub Address
   30 25       (0.)    Second Sub Address
   30 25       (0.)    Third  Sub Address
   31 32 31    (0.)    Fourth Sub Address
  This function will get IPv4 address byte from EF data buffer and return
  4 hex byte information. so for given example, The result will be 0x7B000079
  where First Sub Adress is 123  -> 7B,
        Second Sub Address is 0  -> 0,
        Third Sub Address is  0  -> 0,
        Forth Sub Address is 121 -> 79,
  Note: Maximum length of a subaddress is 3.
DEPENDENCIES
  None.

RETURN VALUE
  0:  When data buffer does not have valid ipv4 address data.
  IPv4 Hex data: When data buffer have valid ipv4 address.

SIDE EFFECTS
  None

===========================================================================*/
uint32 nvruim_get_ipv4_digit_char_string
(
  const byte *data_ptr,
  byte        data_len
);

/*===========================================================================

FUNCTION nvruim_get_ipv6_hex_char_string

DESCRIPTION
   IPv6 have eight groups of 4 hex digits separated by colons.
   it also can have 2 colon ‘::’ together but only once is allowed in entire string.
   For example, all are pointing to same addresss:
   2001:0db8:0000:0000:0000:0000:1428:57ab
   2001:0db8:0000:0000:0000::1428:57ab
   2001:0db8:0:0:0:0:1428:57ab
   2001:0db8:0:0::1428:57ab
   2001:0db8::1428:57ab
   2001:db8::1428:57ab

   Data are stored in card as 8-bit ASCII string, where each character is
   store in one byte information. for example given string 2001:db8::1428:57ab
   is stored as
   32 30 30 31 3A -> 2001:
   64 62 38 3A 3A -> db8::
   31 34 32 38 3A -> 1428:
   35 37 61 62    -> 57ab

   This function will get IPv6 address byte from EF data buffer and stored in
   array of 8 word index. So for given example, output will be
   Ipv6[0] = 0x2001
   Ipv6[1] = 0x0db8
   Ipv6[2] = 0x0000
   Ipv6[3] = 0x0000
   Ipv6[4] = 0x0000
   Ipv6[5] = 0x0000
   Ipv6[6] = 0x1428
   Ipv6[7] = 0x57ab

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  When data buffer have valid ipv6 address data.
  FALSE: When data buffer will not have valid ipv6 address.

SIDE EFFECTS
  None

===========================================================================*/
boolean nvruim_get_ipv6_hex_char_string
(
  const byte  *data_ptr,
  byte         data_len,
  uint16      *ipv6_address_ptr
);
#endif /* FEATURE_UIM_SUPPORT_LBS */

#endif /* NVRUIM_UTIL_H */
