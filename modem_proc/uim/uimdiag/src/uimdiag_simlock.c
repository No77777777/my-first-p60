/*===========================================================================

===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/uimdiag/src/uimdiag_simlock.c#8 $$ $DateTime: 2020/08/04 23:12:22 $

when       who   what, where, why
--------   ---   -----------------------------------------------------------
07/23/20   cj    Added new UIMDIAG command UIMDIAG_SIMLOCK_GET_RSU_MODE_CMD 
01/24/20   cj    Added check for min length of GID
01/23/20   sg    Correct the UIMDIAG NS+SP lock category enum value as per spec
01/20/20   sg    Add NS+SP category as max category supported for ck lock
12/28/19   sg    Fix gid code parsing issue for NS+SP category
12/28/19   sg    Added support for NS+SP category
12/27/19   cj    Make device dual SIM to single SIM via RSU BLOB
09/10/19   vdc   SimLock solution for activation at Point Of Sale
04/26/19   mm    Allowing ADD HCK CODE and SET HCK LOCK for mode B
12/03/18   vgd   Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
05/07/18   dd    Remote SIM Unlock support to switch configuration
11/16/17   me    Fix the size in memecpy in uimdiag_simlock_parse_nw_codes()  
07/13/17   nr    Remote SIM Unlock supported in common build
06/22/17   tq    Disable adding locks using HCK
04/11/17   tq    Decrement uimdiag command length
02/20/17   vdc   Added support to probe CK for correctness
12/02/16   vdc   Added support to check RSA public key for remote lock
11/03/16   bcho  Added support for ICCID simlock
10/28/16   nr    Support remote and local configurations together
10/06/16   ar    Added support for SP + EHPLMN simlock
08/03/16   vdc   Added support for SPN based SIMLOCK feature
05/20/16   vdc   Remove F3 messages for memory allocation failure
04/22/16   av    Do not free user_data_ptr in uimdiag simlock callback
03/11/16   ar    Remove FEATURE_UIMDIAG
11/19/15   vdc   Reduce stack space for SIMLOCK APIs
09/19/15   vr    Fix potential memory leak in UIMDIAG for SimLock operations
07/10/15   stv   Deprecate UIMDIAG_SIMLOCK_GET_CATEGORY_DATA_CMD
05/15/15   vv    Support for RSU set configuration in simlock
01/16/15   vv    Fix issue related to filling the response data
01/14/15   bcho  Remove use of SIMLOCK_MIN_GID_LEN
01/14/15   bcho  Use proper index while copying category data
01/12/15   vv    Fix parsing issue related to code data
12/30/14   bcho  Support for more then 1 bytes of GID1/GID2
12/26/14   vv    Support for whitelist and blacklist codes in same category
09/26/14   vv    Do not allow CK lock, if CK length is zero
06/04/14   vv    Remove FEATURE_SIMLOCK
05/12/14   vv    Fix compiler errors
04/15/14   vv    Added support for quad SIM
03/25/14   tl    SIM Lock secondary revisions
02/24/14   tl    Initial Revision -introduce new SIM Lock feature

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#include "uimdiag_util.h"
#include "comdef.h"
#include "uimdiag.h"
#include "intconv.h"
#include "diagpkt.h"
#include "diagcmd.h"
#include "event.h" /* event_report function */
#include "rex.h"
#include "uim_msg.h"
#include "uimdiag_simlock.h"
#include "simlock_modem_lib.h"

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
#define UIMDIAG_CODE_TYPE_OFFSET       0
#define UIMDIAG_MCC_OFFSET             1
#define UIMDIAG_MNC_LEN_OFFSET         4
#define UIMDIAG_MNC_OFFSET             5
#define UIMDIAG_D6_OFFSET              8
#define UIMDIAG_D7_OFFSET              9
#define UIMDIAG_GID1_LEN_OFFSET        8
#define UIMDIAG_GID1_OFFSET            9
#define UIMDIAG_NS_SP_GID1_LEN_OFFSET  10
#define UIMDIAG_NS_SP_GID1_OFFSET      11
#define UIMDIAG_GID2_LEN_OFFSET        14
#define UIMDIAG_GID2_OFFSET            15
#define UIMDIAG_MSIN_LEN_OFFSET        8
#define UIMDIAG_MSIN_OFFSET            9
#define UIMDIAG_IRM_OFFSET             1
#define UIMDIAG_SPN_OFFSET             4
#define UIMDIAG_EHPLMN_OFFSET          17
#define UIMDIAG_ICCID_LEN_OFFSET       8
#define UIMDIAG_ICCID_OFFSET           9

/* valid wild character for lock code is '*' */
#define UIMDIAG_LOCK_CODE_WILD_CHAR   '*'

/* UIMDIAG supports only 5 bytes of GID for SP and CP categories*/
#define UIMDIAG_SUPPORTED_GID_LEN_MAX  5


static uimdiag_return_enum_type uimdiag_decode_status;

/* ----------------------------------------------------------------------------
  STRUCTURE:      UIMDIAG_SIMLOCK_CATEGORY_MAPPING

  DESCRIPTION:
    This structure maps simlock category to uimdiag category
-------------------------------------------------------------------------------*/
static PACKED struct PACKED_POST
{
  simlock_category_enum_type          simlock_category;
  uimdiag_simlock_category_enum_type  uimdiag_category;
} uimdiag_simlock_category_mapping[] =
{
  { SIMLOCK_CATEGORY_3GPP_NW,        UIMDIAG_SIMLOCK_CATEGORY_3GPP_NW},
  { SIMLOCK_CATEGORY_3GPP_NS,        UIMDIAG_SIMLOCK_CATEGORY_3GPP_NS},
  { SIMLOCK_CATEGORY_3GPP_SP,        UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP},
  { SIMLOCK_CATEGORY_3GPP_SP,        UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP_EXT},
  { SIMLOCK_CATEGORY_3GPP_CP,        UIMDIAG_SIMLOCK_CATEGORY_3GPP_CP},
  { SIMLOCK_CATEGORY_3GPP_CP,        UIMDIAG_SIMLOCK_CATEGORY_3GPP_CP_EXT},
  { SIMLOCK_CATEGORY_3GPP_SIM,       UIMDIAG_SIMLOCK_CATEGORY_3GPP_SIM},
  { SIMLOCK_CATEGORY_3GPP2_NW_TYPE1, UIMDIAG_SIMLOCK_CATEGORY_3GPP2_NW_TYPE1},
  { SIMLOCK_CATEGORY_3GPP2_NW_TYPE2, UIMDIAG_SIMLOCK_CATEGORY_3GPP2_NW_TYPE2},
  { SIMLOCK_CATEGORY_3GPP2_HRPD,     UIMDIAG_SIMLOCK_CATEGORY_3GPP2_HRPD},
  { SIMLOCK_CATEGORY_3GPP2_SP,       UIMDIAG_SIMLOCK_CATEGORY_3GPP2_SP},
  { SIMLOCK_CATEGORY_3GPP2_CP,       UIMDIAG_SIMLOCK_CATEGORY_3GPP2_CP},
  { SIMLOCK_CATEGORY_3GPP2_RUIM,     UIMDIAG_SIMLOCK_CATEGORY_3GPP2_RUIM},
  { SIMLOCK_CATEGORY_3GPP_SPN,       UIMDIAG_SIMLOCK_CATEGORY_3GPP_SPN},
  { SIMLOCK_CATEGORY_3GPP_SP_EHPLMN, UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP_EHPLMN},
  { SIMLOCK_CATEGORY_3GPP_ICCID,     UIMDIAG_SIMLOCK_CATEGORY_3GPP_ICCID},
  { SIMLOCK_CATEGORY_3GPP_NS_SP,     UIMDIAG_SIMLOCK_CATEGORY_3GPP_NS_SP}
};

static uimdiag_set_configuration_msg_req_type simlock_set_configuration;

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/


/*=============================================================================

                              FUNCTIONS

=============================================================================*/

/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_MAP_CATEGORY

DESCRIPTION
  This function maps uimdiag category to simlock category

DEPENDENCIES
  None.

RETURN VALUE
  simlock_category_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static simlock_category_enum_type uimdiag_simlock_map_category
(
  uimdiag_simlock_category_enum_type uimdiag_category
)
{
  uint8                      map_index        = 0;
  simlock_category_enum_type simlock_category = SIMLOCK_CATEGORY_3GPP_NW;

  for(map_index = 0; map_index < sizeof(uimdiag_simlock_category_mapping) / sizeof(uimdiag_simlock_category_mapping[0]); map_index++)
  {
    if(uimdiag_simlock_category_mapping[map_index].uimdiag_category == uimdiag_category)
    {
      simlock_category = uimdiag_simlock_category_mapping[map_index].simlock_category;
      break;
    }
  }

  return simlock_category;
} /* uimdiag_simlock_map_category */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_MAP_SIMLOCK_CATEGORY

DESCRIPTION
  This function maps to simlock category to uimdiag category

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_simlock_category_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_simlock_category_enum_type uimdiag_simlock_map_simlock_category
(
  simlock_category_enum_type  simlock_category
)
{
  uint8                              map_index        = 0;
  uimdiag_simlock_category_enum_type uimdiag_category = UIMDIAG_SIMLOCK_CATEGORY_3GPP_NW;

  for(map_index = 0; map_index < sizeof(uimdiag_simlock_category_mapping) / sizeof(uimdiag_simlock_category_mapping[0]); map_index++)
  {
    if(uimdiag_simlock_category_mapping[map_index].simlock_category == simlock_category)
    {
      uimdiag_category = uimdiag_simlock_category_mapping[map_index].uimdiag_category;
      break;
    }
  }

  return uimdiag_category;
} /* uimdiag_simlock_map_simlock_category */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_DECODE_UINT8

DESCRIPTION
  This function copies uint8 from diag_req_ptr to cmd_ptr,
  from offset value.
  Before copy, it checks diag_req_len, to validate the data in diag_req_ptr.
  The offset is updated after the copy.

DEPENDENCIES
  None.

RETURN VALUE
  uint8

SIDE EFFECTS
  None.
===========================================================================*/
static uint8 uimdiag_simlock_decode_uint8
(
  uint32       * uimdiag_offset,
  const uint8  * uimdiag_req_ptr,
  const uint32   uimdiag_req_len
)
{
  uint8  ret_val = 0;
  if (uimdiag_decode_status != UIMDIAG_SUCCESS)
  {
    return 0;
  }

  if (uimdiag_offset == NULL ||
      uimdiag_req_ptr == NULL)
  {
    uimdiag_decode_status = UIMDIAG_ERROR;
    return 0;
  }

  if(*uimdiag_offset + sizeof(uint8) > uimdiag_req_len)
  {
    UIMDIAG_MSG_ERROR_0("uimdiag_simlock_decode_uint8 - Cmd error");
    uimdiag_decode_status = UIMDIAG_ERROR;
    return 0;
  }

  ret_val =  *(uimdiag_req_ptr + *uimdiag_offset);
  *uimdiag_offset  += sizeof(uint8);
  return ret_val;
} /* uimdiag_simlock_decode_uint8 */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_DECODE_UINT32

DESCRIPTION
  This function copies uint32 from diag_req_ptr to cmd_ptr,
  from offset value.
  Before copy, it checks diag_req_len, to validate the data in diag_req_ptr.
  The offset is updated after the copy.

DEPENDENCIES
  None.

RETURN VALUE
  uint32

SIDE EFFECTS
  None.
===========================================================================*/
static uint32 uimdiag_simlock_decode_uint32
(
  uint32       * uimdiag_offset,
  const uint8  * uimdiag_req_ptr,
  const uint32   uimdiag_req_len
)
{
  uint32  ret_val = 0;
  if (uimdiag_decode_status != UIMDIAG_SUCCESS)
  {
    return 0;
  }

  if (uimdiag_offset == NULL ||
      uimdiag_req_ptr == NULL)
  {
    uimdiag_decode_status = UIMDIAG_ERROR;
    return 0;
  }

  if(*uimdiag_offset + sizeof(uint32) > uimdiag_req_len)
  {
    UIMDIAG_MSG_ERROR_0("uimdiag_simlock_decode_uint32 - Cmd error");
    uimdiag_decode_status = UIMDIAG_ERROR;
    return 0;
  }

  (void)memscpy(((void *)&(ret_val)),
         sizeof(uint32),
         (void *)((uint8*)uimdiag_req_ptr + *uimdiag_offset),
         sizeof(uint32));

  *uimdiag_offset  += sizeof(uint32);
  return ret_val;
} /* uimdiag_simlock_decode_uint32 */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_CONVERT_SLOT

DESCRIPTION
  This function converts uimdiag slot to simlock slot enum type

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_convert_slot
(
  uint8                      uimdiag_slot,
  simlock_slot_enum_type   * simlock_slot_ptr
)
{
  if (simlock_slot_ptr == NULL)
  {
    return UIMDIAG_INCORRECT_PARAM;
  }

  switch (uimdiag_slot)
  {
    case 1:
      *simlock_slot_ptr = SIMLOCK_SLOT_1;
      break;
    case 2:
      *simlock_slot_ptr = SIMLOCK_SLOT_2;
      break;
    case 3:
      *simlock_slot_ptr = SIMLOCK_SLOT_3;
      break;
    case 4:
      *simlock_slot_ptr = SIMLOCK_SLOT_4;
      break;
    default:
      UIMDIAG_MSG_ERROR_1("Invalid slot parameter: 0x%x", uimdiag_slot);
      return UIMDIAG_INCORRECT_PARAM;
  }

  return UIMDIAG_SUCCESS;
} /* uimdiag_simlock_convert_slot */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_POPULATE_SET_POLICY_CMD

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_populate_set_policy_cmd(
  simlock_set_config_policy_msg_req_type  * cmd_ptr,
  void                                    * diag_req_ptr,
  uint32                                    diag_req_len)
{
  uint32      offset = 0;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(cmd_ptr, diag_req_ptr);

  uimdiag_decode_status = UIMDIAG_SUCCESS;

  cmd_ptr->set_operation = SIMLOCK_SET_POLICY_OPERATION;

  memset(&cmd_ptr->disabled_slots, 0x00, sizeof(cmd_ptr->disabled_slots));

  cmd_ptr->slot_policy = (simlock_slot_policy_enum_type)uimdiag_simlock_decode_uint8(
                                                          &offset,
                                                          (uint8*)diag_req_ptr,
                                                          diag_req_len);

  cmd_ptr->sub_policy = (simlock_subscription_policy_enum_type)uimdiag_simlock_decode_uint8(
                                                                 &offset,
                                                                 (uint8*)diag_req_ptr,
                                                                 diag_req_len);

  /* If additional byte is present which is optional, also perform enable/disable slot operation. */
  if(diag_req_len - offset > 0)
  {
    uint8  slot_operation = 0;

    slot_operation = uimdiag_simlock_decode_uint8(&offset,
                                                  (uint8*)diag_req_ptr,
                                                  diag_req_len);

    /* slot_operation provides info on powering up/powering down of slots.
       First 5 bits is set as mentioned as below
       B1  0: disable slot1
           1: enable slot1 (if the slot exists)
       B2  0: disable slot2
           1: enable slot2 (if the slot exists)
       B3  0: disable slot3
           1: enable slot3 (if the slot exists)
       B4  0: disable slot4
           1: enable slot4 (if the slot exists)
       B5  0: configuration of active slots does not change and value in b1 to b4 is ignored.
           1: configuration of active slots indicated in b1 to b4 is valid. */
    if(slot_operation & 0x10)
    {
      uint8 index = 0;

      cmd_ptr->set_operation = SIMLOCK_SET_POLICY_AND_SLOT_CONFIG_OPERATION;

      for (index = 0; index < SIMLOCK_SLOT_COUNT_MAX; index++)
      {
        if(!(slot_operation & (0x01 << index)))
        {
          cmd_ptr->disabled_slots.slot[index] = TRUE;
        }
      }
    }
  }

  return uimdiag_decode_status;
}/* uimdiag_simlock_populate_set_policy_cmd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_PARSE_GID_CODES

DESCRIPTION
  This function copies gid len and gid data from diag buffer

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_parse_gid_codes(
  const uint8                             * diag_req_ptr,
  uint32                                    diag_req_len,
  uimdiag_simlock_category_enum_type        uimdiag_category,
  uint32                                  * offset_ptr,
  simlock_gid_type                        * gid_code_ptr)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_3(diag_req_ptr, offset_ptr, gid_code_ptr);

  gid_code_ptr->gid_len =
    (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                        diag_req_ptr,
                                        diag_req_len);
  if (gid_code_ptr->gid_len < SIMLOCK_MIN_GID_LEN)
  {
    UIMDIAG_MSG_ERROR_0("GID data length too small.");
    return UIMDIAG_ERROR;
  }
  
  /* Currently, UIMDIAG support is only for 5 bytes GID. In case of SP + EHPLMN
     and NS_SP only, the GID is supported for 8 bytes */
  if (uimdiag_category == UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP_EHPLMN ||
      uimdiag_category == UIMDIAG_SIMLOCK_CATEGORY_3GPP_NS_SP)
  {
    if (gid_code_ptr->gid_len > SIMLOCK_MAX_GID_LEN)
    {
      UIMDIAG_MSG_ERROR_1("GID data length too long for SP_EHPLMN/NS_SP category. length: 0x%x",
                          gid_code_ptr->gid_len);
      return UIMDIAG_ERROR;
    }
  }
  else
  {
    if(gid_code_ptr->gid_len > UIMDIAG_SUPPORTED_GID_LEN_MAX)
    {
      UIMDIAG_MSG_ERROR_1("GID data length too long. length: 0x%x",
                          gid_code_ptr->gid_len);
      return UIMDIAG_ERROR;
    }
  }

  memscpy(gid_code_ptr->gid_data,
          sizeof(gid_code_ptr->gid_data),
          ((uint8*)diag_req_ptr + *offset_ptr),
          gid_code_ptr->gid_len);

  if (uimdiag_category == UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP_EHPLMN ||
      uimdiag_category == UIMDIAG_SIMLOCK_CATEGORY_3GPP_NS_SP)
  {
    *offset_ptr += SIMLOCK_MAX_GID_LEN;
  }
  else
  {
    *offset_ptr += UIMDIAG_SUPPORTED_GID_LEN_MAX;
  }

  return UIMDIAG_SUCCESS;
} /* uimdiag_simlock_parse_gid_codes */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_PARSE_NW_CODES

DESCRIPTION
  Parses MCC - MNC values provided in the diag input

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_parse_nw_codes(
  const uint8                             * diag_req_ptr,
  uint32                                    diag_req_len,
  uint32                                  * offset_ptr,
  simlock_nw_code_data_type               * nw_code_ptr)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_3(diag_req_ptr, offset_ptr, nw_code_ptr);

  memscpy(nw_code_ptr->mcc,
          sizeof(nw_code_ptr->mcc),
          ((uint8*)diag_req_ptr + *offset_ptr),
          SIMLOCK_MCC_LEN);

  *offset_ptr += SIMLOCK_MCC_LEN;

  nw_code_ptr->mnc.mnc_len = (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                                                 diag_req_ptr,
                                                                 diag_req_len);

  if (nw_code_ptr->mnc.mnc_len > SIMLOCK_MNC_MAX)
  {
    UIMDIAG_MSG_ERROR_1("MNC data length too long. length: 0x%x",
                        nw_code_ptr->mnc.mnc_len);
    return UIMDIAG_ERROR;
  }

  memscpy(nw_code_ptr->mnc.mnc_data,
          sizeof(nw_code_ptr->mnc.mnc_data),
          ((uint8*)diag_req_ptr + *offset_ptr),
          nw_code_ptr->mnc.mnc_len);

  *offset_ptr += SIMLOCK_MNC_MAX;

  return UIMDIAG_SUCCESS;
} /* uimdiag_simlock_parse_nw_codes */


/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_PARSE_NW_PACKED_CODES

DESCRIPTION
  Parses packed MCC - MNC values provided in the diag input

|-----------------------------------------------------------------|
| Nibble 1   Nibble 2 | Nibble 1   Nibble 2 | Nibble 1   Nibble 2 |
|----------|----------|-------------------------------------------|
| MCC Hex  | MCC Hex  | MCC Hex  | MNC Hex  | MNC Hex  | MNC Hex  |
| Digit1   | Digit 2  | Digit3   | Digit 1  | Digit2   | Digit 3  |
|----------|----------|------------------------------------------ |
|        Byte 1       |        Byte 2       |        Byte 3       |
|-----------------------------------------------------------------|

MNC Hex Digit 1 is set to F if MNC length is 0x02

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_parse_nw_packed_codes(
  const uint8                             * diag_req_ptr,
  uint32                                    diag_req_len,
  uint32                                  * offset_ptr,
  simlock_nw_code_data_type               * nw_code_ptr)
{
  uint8    mcc_mnc_d1     = 0x00;
  uint8    mcc_mnc_d2     = 0x00;
  uint8    mcc_mnc_d3     = 0x00;

  UIMDIAG_UTIL_RETURN_IF_NULL_3(diag_req_ptr, offset_ptr, nw_code_ptr);

  mcc_mnc_d1 = uimdiag_simlock_decode_uint8(offset_ptr, diag_req_ptr, diag_req_len);
  mcc_mnc_d2 = uimdiag_simlock_decode_uint8(offset_ptr, diag_req_ptr, diag_req_len);
  mcc_mnc_d3 = uimdiag_simlock_decode_uint8(offset_ptr, diag_req_ptr, diag_req_len);

  if(uimdiag_decode_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_0("MCC-MNC parsing failed");
    return uimdiag_decode_status;
  }

  /* MCC calculation */
  if((mcc_mnc_d1 & 0xF0) == 0xE0)
  {
    nw_code_ptr->mcc[0] = UIMDIAG_LOCK_CODE_WILD_CHAR;
  }
  else
  {
    nw_code_ptr->mcc[0] = ((mcc_mnc_d1 & 0xF0) >> 0x04) + '0';
  }

  if((mcc_mnc_d1 & 0x0F) == 0x0E)
  {
    nw_code_ptr->mcc[1] = UIMDIAG_LOCK_CODE_WILD_CHAR;
  }
  else
  {
    nw_code_ptr->mcc[1] = (mcc_mnc_d1 & 0x0F) + '0';
  }

  if((mcc_mnc_d2 & 0xF0) == 0xE0)
  {
    nw_code_ptr->mcc[2] = UIMDIAG_LOCK_CODE_WILD_CHAR;
  }
  else
  {
    nw_code_ptr->mcc[2] = ((mcc_mnc_d2 & 0xF0) >> 0x04) + '0';
  }

  /* MNC calculation */
  if ((mcc_mnc_d2 & 0x0F) != 0x0F)
  {
    nw_code_ptr->mnc.mnc_len = SIMLOCK_MNC_MAX;

    if((mcc_mnc_d2 & 0x0F) == 0x0E)
    {
      nw_code_ptr->mnc.mnc_data[0] = UIMDIAG_LOCK_CODE_WILD_CHAR;
    }
    else
    {
      nw_code_ptr->mnc.mnc_data[0] = (mcc_mnc_d2 & 0x0F) + '0';
    }

    if((mcc_mnc_d3 & 0xF0) == 0xE0)
    {
      nw_code_ptr->mnc.mnc_data[1] = UIMDIAG_LOCK_CODE_WILD_CHAR;
    }
    else
    {
      nw_code_ptr->mnc.mnc_data[1] = ((mcc_mnc_d3 & 0xF0) >> 0x04) + '0';
    }

    if((mcc_mnc_d3 & 0x0F) == 0x0E)
    {
      nw_code_ptr->mnc.mnc_data[2] = UIMDIAG_LOCK_CODE_WILD_CHAR;
    }
    else
    {
      nw_code_ptr->mnc.mnc_data[2] = (mcc_mnc_d3 & 0x0F) + '0';
    }
  }
  else
  {
    nw_code_ptr->mnc.mnc_len = 0x02;

    if((mcc_mnc_d3 & 0xF0) == 0xE0)
    {
      nw_code_ptr->mnc.mnc_data[0] = UIMDIAG_LOCK_CODE_WILD_CHAR;
    }
    else
    {
      nw_code_ptr->mnc.mnc_data[0] = ((mcc_mnc_d3 & 0xF0) >> 0x04) + '0';
    }

    if((mcc_mnc_d3 & 0x0F) == 0x0E)
    {
      nw_code_ptr->mnc.mnc_data[1] = UIMDIAG_LOCK_CODE_WILD_CHAR;
    }
    else
    {
      nw_code_ptr->mnc.mnc_data[1] = (mcc_mnc_d3 & 0x0F) + '0';
    }
  }

  return UIMDIAG_SUCCESS;
} /* uimdiag_simlock_parse_nw_packed_codes */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_CONVERT_BCD_TO_ASCII

DESCRIPTION
  This function converts BCD string to ascii string

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_convert_bcd_to_ascii(
  uint8         bcd_digit_len,
  const uint8 * bcd_string_ptr,
  char        * ascii_string_ptr
)
{
  uint8 index = 0;

  if(bcd_digit_len == 0 ||
     bcd_digit_len > SIMLOCK_ICCID_DIGITS_MAX ||
     bcd_string_ptr == NULL ||
     ascii_string_ptr == NULL)
  {
    return UIMDIAG_ERROR;
  }

  for(index = 0; index < bcd_digit_len;)
  {
    if((bcd_string_ptr[index/2] & 0x0F) <= 9)
    {
      ascii_string_ptr[index] = (bcd_string_ptr[index/2]& 0x0F) + '0';
    }
    else if((bcd_string_ptr[index/2] & 0x0F) == 0x0F)
    {
      ascii_string_ptr[index] = '*';
    }
    else
    {
      return UIMDIAG_ERROR;
    }

    index++;

    if(((bcd_string_ptr[index/2] >> 4) & 0x0F) <= 9)
    {
      ascii_string_ptr[index] = ((bcd_string_ptr[index/2] >> 4)& 0x0F) + '0';
    }
    else if(((bcd_string_ptr[index/2] >> 4) & 0x0F) == 0x0F)
    {
      ascii_string_ptr[index] = '*';
    }
    else
    {
      return UIMDIAG_ERROR;
    }

    index++;
  }
  return UIMDIAG_SUCCESS;
}/* uimdiag_simlock_convert_bcd_to_ascii */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_PARSE_CODES

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_parse_codes(
  const uint8                             * diag_req_ptr,
  uint32                                    diag_req_len,
  uint32                                  * offset_ptr,
  simlock_category_data_type              * category_data_ptr,
  uimdiag_simlock_category_enum_type        uimdiag_category)
{
  uint32      data_offset        = 0;
  uint32      i                  = 0;

  UIMDIAG_UTIL_RETURN_IF_NULL_3(diag_req_ptr, offset_ptr, category_data_ptr);

  switch (uimdiag_category)
  {
    case UIMDIAG_SIMLOCK_CATEGORY_3GPP_NW:
      category_data_ptr->code_data.nw_3gpp_code_list.num_nw_codes =
        (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                            diag_req_ptr,
                                            diag_req_len);

      if (category_data_ptr->code_data.nw_3gpp_code_list.num_nw_codes > SIMLOCK_NUM_CODES_MAX)
      {
        UIMDIAG_MSG_ERROR_1("SIM Lock number of codes: 0x%x",
                            category_data_ptr->code_data.nw_3gpp_code_list.num_nw_codes);
        return UIMDIAG_ERROR;
      }

      for (i = 0; i < category_data_ptr->code_data.nw_3gpp_code_list.num_nw_codes; i++)
      {
        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_0("CK parsing failed");
          return uimdiag_decode_status;
        }

        if ((*offset_ptr + UIMDIAG_SIMLOCK_DATA_BLOCK_LEN) > diag_req_len)
        {
          UIMDIAG_MSG_ERROR_1("SIM Lock data length too long. offset: 0x%x", *offset_ptr);
          return UIMDIAG_ERROR;
        }

        data_offset = *offset_ptr;

        uimdiag_decode_status = uimdiag_simlock_parse_nw_codes(
                                  diag_req_ptr,
                                  diag_req_len,
                                  &data_offset,
                                  &category_data_ptr->code_data.nw_3gpp_code_list.nw_code_data[i]);

        *offset_ptr += UIMDIAG_SIMLOCK_DATA_BLOCK_LEN;
      }
      break;

    case UIMDIAG_SIMLOCK_CATEGORY_3GPP_NS:
      category_data_ptr->code_data.ns_3gpp_code_list.num_ns_codes =
        (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                            diag_req_ptr,
                                            diag_req_len);

      if (category_data_ptr->code_data.ns_3gpp_code_list.num_ns_codes > SIMLOCK_NUM_CODES_MAX)
      {
        UIMDIAG_MSG_ERROR_1("SIM Lock number of codes: 0x%x",
                            category_data_ptr->code_data.ns_3gpp_code_list.num_ns_codes);
        return UIMDIAG_ERROR;
      }

      for (i = 0; i < category_data_ptr->code_data.ns_3gpp_code_list.num_ns_codes; i++)
      {
        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_0("CK parsing failed");
          return uimdiag_decode_status;
        }

        if ((*offset_ptr + UIMDIAG_SIMLOCK_DATA_BLOCK_LEN) > diag_req_len)
        {
          UIMDIAG_MSG_ERROR_1("SIM Lock data length too long. offset: 0x%x", *offset_ptr);
          return UIMDIAG_ERROR;
        }

        data_offset = *offset_ptr;

        uimdiag_decode_status = uimdiag_simlock_parse_nw_codes(
                                  diag_req_ptr,
                                  diag_req_len,
                                  &data_offset,
                                  &category_data_ptr->code_data.ns_3gpp_code_list.ns_code_data[i].nw_code);

        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_1("Failed to parse nw codes 0x%x", uimdiag_decode_status);
          return uimdiag_decode_status;
        }

        category_data_ptr->code_data.ns_3gpp_code_list.ns_code_data[i].imsi_digit6 =
          (char)uimdiag_simlock_decode_uint8(&data_offset,
                                             diag_req_ptr,
                                             diag_req_len);

        category_data_ptr->code_data.ns_3gpp_code_list.ns_code_data[i].imsi_digit7 =
          (char)uimdiag_simlock_decode_uint8(&data_offset,
                                             diag_req_ptr,
                                             diag_req_len);

        *offset_ptr += UIMDIAG_SIMLOCK_DATA_BLOCK_LEN;
      }
      break;

    case UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP_EXT:
    case UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP:
      category_data_ptr->code_data.sp_3gpp_code_list.num_sp_codes =
        (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                            diag_req_ptr,
                                            diag_req_len);

      if (category_data_ptr->code_data.sp_3gpp_code_list.num_sp_codes > SIMLOCK_NUM_CODES_MAX)
      {
        UIMDIAG_MSG_ERROR_1("SIM Lock number of codes: 0x%x",
                            category_data_ptr->code_data.sp_3gpp_code_list.num_sp_codes);
        return UIMDIAG_ERROR;
      }

      for (i = 0; i < category_data_ptr->code_data.sp_3gpp_code_list.num_sp_codes; i++)
      {
        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_0("CK parsing failed");
          return uimdiag_decode_status;
        }

        if ((*offset_ptr + UIMDIAG_SIMLOCK_DATA_BLOCK_LEN) > diag_req_len)
        {
          UIMDIAG_MSG_ERROR_1("SIM Lock data length too long. offset: 0x%x", *offset_ptr);
          return UIMDIAG_ERROR;
        }

        data_offset = *offset_ptr;

        uimdiag_decode_status = uimdiag_simlock_parse_nw_codes(
                                  diag_req_ptr,
                                  diag_req_len,
                                  &data_offset,
                                  &category_data_ptr->code_data.sp_3gpp_code_list.sp_code_data[i].nw_code);

        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_1("Failed to parse nw codes 0x%x", uimdiag_decode_status);
          return uimdiag_decode_status;
        }

        if(uimdiag_category == UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP_EXT)
        {
          uimdiag_decode_status = uimdiag_simlock_parse_gid_codes(
                                    diag_req_ptr,
                                    diag_req_len,
                                    UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP_EXT,
                                    &data_offset,
                                    &category_data_ptr->code_data.sp_3gpp_code_list.sp_code_data[i].gid1);

          if (uimdiag_decode_status != UIMDIAG_SUCCESS)
          {
            UIMDIAG_MSG_ERROR_1("Failed to parse gid1 codes 0x%x", uimdiag_decode_status);
            return uimdiag_decode_status;
          }
        }
        else
        {
          category_data_ptr->code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_data[0] =
            (uint8)uimdiag_simlock_decode_uint8(&data_offset,
                                                diag_req_ptr,
                                                diag_req_len);
          category_data_ptr->code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_len = 1;
        }

        *offset_ptr += UIMDIAG_SIMLOCK_DATA_BLOCK_LEN;
      }
      break;

    case UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP_EHPLMN:
      category_data_ptr->code_data.sp_ehplmn_3gpp_code_list.num_sp_ehplmn_codes =
        (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                            diag_req_ptr,
                                            diag_req_len);

      if (category_data_ptr->code_data.sp_ehplmn_3gpp_code_list.num_sp_ehplmn_codes > SIMLOCK_NUM_CODES_MAX)
      {
        UIMDIAG_MSG_ERROR_1("SIM Lock number of codes: 0x%x",
                            category_data_ptr->code_data.sp_ehplmn_3gpp_code_list.num_sp_ehplmn_codes);
        return UIMDIAG_ERROR;
      }

      for (i = 0; i < category_data_ptr->code_data.sp_ehplmn_3gpp_code_list.num_sp_ehplmn_codes; i++)
      {
        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_0("CK parsing failed");
          return uimdiag_decode_status;
        }

        if ((*offset_ptr + UIMDIAG_SIMLOCK_DATA_BLOCK_LEN) > diag_req_len)
        {
          UIMDIAG_MSG_ERROR_1("SIM Lock data length too long. offset: 0x%x", *offset_ptr);
          return UIMDIAG_ERROR;
        }

        data_offset = *offset_ptr;

        uimdiag_decode_status = uimdiag_simlock_parse_nw_codes(
                                  diag_req_ptr,
                                  diag_req_len,
                                  &data_offset,
                                  &category_data_ptr->code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].nw_code);

        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_1("Failed to parse nw codes 0x%x", uimdiag_decode_status);
          return uimdiag_decode_status;
        }

        uimdiag_decode_status = uimdiag_simlock_parse_gid_codes(
                                  diag_req_ptr,
                                  diag_req_len,
                                  UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP_EHPLMN,
                                  &data_offset,
                                  &category_data_ptr->code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1);

        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_1("Failed to parse gid1 codes 0x%x", uimdiag_decode_status);
          return uimdiag_decode_status;
        }

        uimdiag_decode_status = uimdiag_simlock_parse_nw_packed_codes(
                                  diag_req_ptr,
                                  diag_req_len,
                                  &data_offset,
                                  &category_data_ptr->code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].ehplmn_nw_code);

        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_1("Failed to parse gid1 codes 0x%x", uimdiag_decode_status);
          return uimdiag_decode_status;
        }

        *offset_ptr += UIMDIAG_SIMLOCK_DATA_BLOCK_LEN;
      }
      break;

    case UIMDIAG_SIMLOCK_CATEGORY_3GPP_CP_EXT:
    case UIMDIAG_SIMLOCK_CATEGORY_3GPP_CP:
      category_data_ptr->code_data.cp_3gpp_code_list.num_cp_codes =
        (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                            diag_req_ptr,
                                            diag_req_len);

      if (category_data_ptr->code_data.cp_3gpp_code_list.num_cp_codes > SIMLOCK_NUM_CODES_MAX)
      {
        UIMDIAG_MSG_ERROR_1("SIM Lock number of codes: 0x%x",
                            category_data_ptr->code_data.cp_3gpp_code_list.num_cp_codes);
        return UIMDIAG_ERROR;
      }

      for (i = 0; i < category_data_ptr->code_data.cp_3gpp_code_list.num_cp_codes; i++)
      {
        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_0("CK parsing failed");
          return uimdiag_decode_status;
        }

        if ((*offset_ptr + UIMDIAG_SIMLOCK_DATA_BLOCK_LEN) > diag_req_len)
        {
          UIMDIAG_MSG_ERROR_1("SIM Lock data length too long. offset: 0x%x", *offset_ptr);
          return UIMDIAG_ERROR;
        }

        data_offset = *offset_ptr;

        uimdiag_decode_status = uimdiag_simlock_parse_nw_codes(
                                  diag_req_ptr,
                                  diag_req_len,
                                  &data_offset,
                                  &category_data_ptr->code_data.cp_3gpp_code_list.cp_code_data[i].nw_code);

        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_1("Failed to parse nw codes 0x%x", uimdiag_decode_status);
          return uimdiag_decode_status;
        }

        if(uimdiag_category == UIMDIAG_SIMLOCK_CATEGORY_3GPP_CP_EXT)
        {
          uimdiag_decode_status = uimdiag_simlock_parse_gid_codes(
                                    diag_req_ptr,
                                    diag_req_len,
                                    UIMDIAG_SIMLOCK_CATEGORY_3GPP_CP_EXT,
                                    &data_offset,
                                    &category_data_ptr->code_data.cp_3gpp_code_list.cp_code_data[i].gid1);

          if (uimdiag_decode_status != UIMDIAG_SUCCESS)
          {
            UIMDIAG_MSG_ERROR_1("Failed to parse gid1 codes 0x%x", uimdiag_decode_status);
            return uimdiag_decode_status;
          }

          uimdiag_decode_status = uimdiag_simlock_parse_gid_codes(
                                    diag_req_ptr,
                                    diag_req_len,
                                    UIMDIAG_SIMLOCK_CATEGORY_3GPP_CP_EXT,
                                    &data_offset,
                                    &category_data_ptr->code_data.cp_3gpp_code_list.cp_code_data[i].gid2);

          if (uimdiag_decode_status != UIMDIAG_SUCCESS)
          {
            UIMDIAG_MSG_ERROR_1("Failed to parse gid2 codes 0x%x", uimdiag_decode_status);
            return uimdiag_decode_status;
          }
        }
        else
        {
          category_data_ptr->code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_data[0] =
            (uint8)uimdiag_simlock_decode_uint8(&data_offset,
                                                diag_req_ptr,
                                                diag_req_len);
          category_data_ptr->code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_len = 1;

          category_data_ptr->code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_data[0] =
            (uint8)uimdiag_simlock_decode_uint8(&data_offset,
                                                diag_req_ptr,
                                                diag_req_len);
          category_data_ptr->code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_len = 1;
        }

        *offset_ptr += UIMDIAG_SIMLOCK_DATA_BLOCK_LEN;
      }
      break;

    case UIMDIAG_SIMLOCK_CATEGORY_3GPP_SIM:
      category_data_ptr->code_data.sim_3gpp_code_list.num_sim_codes =
        (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                            diag_req_ptr,
                                            diag_req_len);

      if (category_data_ptr->code_data.sim_3gpp_code_list.num_sim_codes > SIMLOCK_NUM_CODES_MAX)
      {
        UIMDIAG_MSG_ERROR_1("SIM Lock number of codes: 0x%x",
                            category_data_ptr->code_data.sim_3gpp_code_list.num_sim_codes);
        return UIMDIAG_ERROR;
      }

      for (i = 0; i < category_data_ptr->code_data.sim_3gpp_code_list.num_sim_codes; i++)
      {
        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_0("CK parsing failed");
          return uimdiag_decode_status;
        }

        if ((*offset_ptr + UIMDIAG_SIMLOCK_DATA_BLOCK_LEN) > diag_req_len)
        {
          UIMDIAG_MSG_ERROR_1("SIM Lock data length too long. offset: 0x%x", *offset_ptr);
          return UIMDIAG_ERROR;
        }

        data_offset = *offset_ptr;

        uimdiag_decode_status = uimdiag_simlock_parse_nw_codes(
                                  diag_req_ptr,
                                  diag_req_len,
                                  &data_offset,
                                  &category_data_ptr->code_data.sim_3gpp_code_list.sim_code_data[i].nw_code);

        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_1("Failed to parse nw codes 0x%x", uimdiag_decode_status);
          return uimdiag_decode_status;
        }

        category_data_ptr->code_data.sim_3gpp_code_list.sim_code_data[i].imsi_msin.msin_len =
          (uint8)uimdiag_simlock_decode_uint8(&data_offset,
                                              diag_req_ptr,
                                              diag_req_len);

        if (category_data_ptr->code_data.sim_3gpp_code_list.sim_code_data[i].imsi_msin.msin_len > SIMLOCK_MSIN_MAX)
        {
          UIMDIAG_MSG_ERROR_1("MSIN data length too long. length: 0x%x",
                              category_data_ptr->code_data.sim_3gpp_code_list.sim_code_data[i].imsi_msin.msin_len);
          return UIMDIAG_ERROR;
        }

        memscpy(category_data_ptr->code_data.sim_3gpp_code_list.sim_code_data[i].imsi_msin.msin_data,
                sizeof(category_data_ptr->code_data.sim_3gpp_code_list.sim_code_data[i].imsi_msin.msin_data),
                ((uint8*)diag_req_ptr + data_offset),
                category_data_ptr->code_data.sim_3gpp_code_list.sim_code_data[i].imsi_msin.msin_len);

        *offset_ptr += UIMDIAG_SIMLOCK_DATA_BLOCK_LEN;
      }
      break;

    case UIMDIAG_SIMLOCK_CATEGORY_3GPP_SPN:
      category_data_ptr->code_data.spn_3gpp_code_list.num_spn_codes =
        (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                            diag_req_ptr,
                                            diag_req_len);

      if (category_data_ptr->code_data.spn_3gpp_code_list.num_spn_codes > SIMLOCK_NUM_CODES_MAX)
      {
        UIMDIAG_MSG_ERROR_1("SIM Lock number of codes: 0x%x",
                            category_data_ptr->code_data.spn_3gpp_code_list.num_spn_codes);
        return UIMDIAG_ERROR;
      }

      for (i = 0; i < category_data_ptr->code_data.spn_3gpp_code_list.num_spn_codes; i++)
      {
        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_0("SPN lock codes are incorrect");
          return uimdiag_decode_status;
        }

        if ((*offset_ptr + UIMDIAG_SIMLOCK_DATA_BLOCK_LEN) > diag_req_len)
        {
          UIMDIAG_MSG_ERROR_1("SIM Lock data length too long. offset: 0x%x", *offset_ptr);
          return UIMDIAG_ERROR;
        }

        data_offset = *offset_ptr;

        uimdiag_decode_status = uimdiag_simlock_parse_nw_packed_codes(
                                  diag_req_ptr,
                                  diag_req_len,
                                  &data_offset,
                                  &category_data_ptr->code_data.spn_3gpp_code_list.spn_code_data[i].nw_code);

        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_1("Failed to parse packed nw codes 0x%x", uimdiag_decode_status);
          return uimdiag_decode_status;
        }

        category_data_ptr->code_data.spn_3gpp_code_list.spn_code_data[i].spn.spn_valid = TRUE;

        memscpy(category_data_ptr->code_data.spn_3gpp_code_list.spn_code_data[i].spn.spn_data,
                sizeof(category_data_ptr->code_data.spn_3gpp_code_list.spn_code_data[i].spn.spn_data),
                ((uint8*)diag_req_ptr + data_offset),
                SIMLOCK_SPN_LEN);

        *offset_ptr += UIMDIAG_SIMLOCK_DATA_BLOCK_LEN;
      }
      break;

    case UIMDIAG_SIMLOCK_CATEGORY_3GPP_ICCID:
        category_data_ptr->code_data.iccid_3gpp_code_list.num_iccid_codes =
          (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                              diag_req_ptr,
                                              diag_req_len);
      
        if (category_data_ptr->code_data.iccid_3gpp_code_list.num_iccid_codes > SIMLOCK_NUM_CODES_MAX)
        {
          UIMDIAG_MSG_ERROR_1("SIM Lock number of codes: 0x%x",
                              category_data_ptr->code_data.iccid_3gpp_code_list.num_iccid_codes);
          return UIMDIAG_ERROR;
        }
      
        for (i = 0; i < category_data_ptr->code_data.iccid_3gpp_code_list.num_iccid_codes; i++)
        {
          if (uimdiag_decode_status != UIMDIAG_SUCCESS)
          {
            UIMDIAG_MSG_ERROR_0("SIM Lock data parsing failed");
            return uimdiag_decode_status;
          }
      
          if ((*offset_ptr + UIMDIAG_SIMLOCK_DATA_BLOCK_LEN) > diag_req_len)
          {
            UIMDIAG_MSG_ERROR_1("SIM Lock data length too long. offset: 0x%x", *offset_ptr);
            return UIMDIAG_ERROR;
          }
      
          data_offset = *offset_ptr;
      
          uimdiag_decode_status = uimdiag_simlock_parse_nw_codes(
                                    diag_req_ptr,
                                    diag_req_len,
                                    &data_offset,
                                    &category_data_ptr->code_data.iccid_3gpp_code_list.iccid_code_data[i].nw_code);
      
          if (uimdiag_decode_status != UIMDIAG_SUCCESS)
          {
            UIMDIAG_MSG_ERROR_1("Failed to parse nw codes 0x%x", uimdiag_decode_status);
            return uimdiag_decode_status;
          }

          category_data_ptr->code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len =
            (uint8)uimdiag_simlock_decode_uint8(&data_offset,
                                                diag_req_ptr,
                                                diag_req_len);

          uimdiag_decode_status = uimdiag_simlock_convert_bcd_to_ascii(
                                    category_data_ptr->code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len,
                                    diag_req_ptr + data_offset,
                                    category_data_ptr->code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_data);
    
          if (uimdiag_decode_status != UIMDIAG_SUCCESS)
          {
            UIMDIAG_MSG_ERROR_1("Failed to parse ICCID codes 0x%x", uimdiag_decode_status);
            return uimdiag_decode_status;
          }
      
          *offset_ptr += UIMDIAG_SIMLOCK_DATA_BLOCK_LEN;
        }
        break;

     case UIMDIAG_SIMLOCK_CATEGORY_3GPP_NS_SP:
      category_data_ptr->code_data.ns_sp_3gpp_code_list.num_ns_sp_codes =
        (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                            diag_req_ptr,
                                            diag_req_len);

      if (category_data_ptr->code_data.ns_sp_3gpp_code_list.num_ns_sp_codes > SIMLOCK_NUM_CODES_MAX)
      {
        UIMDIAG_MSG_ERROR_1("Number of NS +SP lock codes present: 0x%x",
                            category_data_ptr->code_data.ns_sp_3gpp_code_list.num_ns_sp_codes);
        return UIMDIAG_ERROR;
      }

      for (i = 0; i < category_data_ptr->code_data.ns_sp_3gpp_code_list.num_ns_sp_codes; i++)
      {
        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_0("Failed to parse NS + SP lock codes");
          return uimdiag_decode_status;
        }

        if ((*offset_ptr + UIMDIAG_SIMLOCK_DATA_BLOCK_LEN) > diag_req_len)
        {
          UIMDIAG_MSG_ERROR_1("SIM Lock data length too long. offset: 0x%x", *offset_ptr);
          return UIMDIAG_ERROR;
        }

        data_offset = *offset_ptr;

        uimdiag_decode_status = uimdiag_simlock_parse_nw_codes(
                                  diag_req_ptr,
                                  diag_req_len,
                                  &data_offset,
                                  &category_data_ptr->code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].ns_code.nw_code);

        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_1("Failed to parse nw codes 0x%x", uimdiag_decode_status);
          return uimdiag_decode_status;
        }

        category_data_ptr->code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].ns_code.imsi_digit6 =
          (char)uimdiag_simlock_decode_uint8(&data_offset,
                                             diag_req_ptr,
                                             diag_req_len);

        category_data_ptr->code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].ns_code.imsi_digit7 =
          (char)uimdiag_simlock_decode_uint8(&data_offset,
                                             diag_req_ptr,
                                             diag_req_len);

          uimdiag_decode_status = uimdiag_simlock_parse_gid_codes(
                                    diag_req_ptr,
                                    diag_req_len,
                                    UIMDIAG_SIMLOCK_CATEGORY_3GPP_NS_SP,
                                    &data_offset,
                                    &category_data_ptr->code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1);

          if (uimdiag_decode_status != UIMDIAG_SUCCESS)
          {
            UIMDIAG_MSG_ERROR_1("Failed to parse gid1 codes 0x%x", uimdiag_decode_status);
            return uimdiag_decode_status;
          }

        *offset_ptr += UIMDIAG_SIMLOCK_DATA_BLOCK_LEN;
      }
      break;

    case UIMDIAG_SIMLOCK_CATEGORY_3GPP2_NW_TYPE1:
      category_data_ptr->code_data.nw_type1_3gpp2_code_list.num_nw_codes =
        (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                            diag_req_ptr,
                                            diag_req_len);

      if (category_data_ptr->code_data.nw_type1_3gpp2_code_list.num_nw_codes > SIMLOCK_NUM_CODES_MAX)
      {
        UIMDIAG_MSG_ERROR_1("SIM Lock number of codes: 0x%x",
                            category_data_ptr->code_data.nw_type1_3gpp2_code_list.num_nw_codes);
        return UIMDIAG_ERROR;
      }

      for (i = 0; i < category_data_ptr->code_data.nw_type1_3gpp2_code_list.num_nw_codes; i++)
      {
        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_0("CK parsing failed");
          return uimdiag_decode_status;
        }

        if ((*offset_ptr + UIMDIAG_SIMLOCK_DATA_BLOCK_LEN) > diag_req_len)
        {
          UIMDIAG_MSG_ERROR_1("SIM Lock data length too long. offset: 0x%x", *offset_ptr);
          return UIMDIAG_ERROR;
        }

        data_offset = *offset_ptr;

        uimdiag_decode_status = uimdiag_simlock_parse_nw_codes(
                                  diag_req_ptr,
                                  diag_req_len,
                                  &data_offset,
                                  &category_data_ptr->code_data.nw_type1_3gpp2_code_list.nw_code_data[i]);

        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_1("Failed to parse nw codes 0x%x", uimdiag_decode_status);
          return uimdiag_decode_status;
        }

        *offset_ptr += UIMDIAG_SIMLOCK_DATA_BLOCK_LEN;
      }
      break;

    case UIMDIAG_SIMLOCK_CATEGORY_3GPP2_NW_TYPE2:
      category_data_ptr->code_data.nw_type2_3gpp2_code_list.num_nw_type2_codes =
        (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                            diag_req_ptr,
                                            diag_req_len);

      if (category_data_ptr->code_data.nw_type2_3gpp2_code_list.num_nw_type2_codes > SIMLOCK_NUM_CODES_MAX)
      {
        UIMDIAG_MSG_ERROR_1("SIM Lock number of codes: 0x%x",
                            category_data_ptr->code_data.nw_type2_3gpp2_code_list.num_nw_type2_codes);
        return UIMDIAG_ERROR;
      }

      for (i = 0; i < category_data_ptr->code_data.nw_type2_3gpp2_code_list.num_nw_type2_codes; i++)
      {
        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_0("CK parsing failed");
          return uimdiag_decode_status;
        }

        if ((*offset_ptr + UIMDIAG_SIMLOCK_DATA_BLOCK_LEN) > diag_req_len)
        {
          UIMDIAG_MSG_ERROR_1("SIM Lock data length too long. offset: 0x%x", *offset_ptr);
          return UIMDIAG_ERROR;
        }

        data_offset = *offset_ptr;

        memscpy(category_data_ptr->code_data.nw_type2_3gpp2_code_list.nw_type2_code_data[i].irm,
                sizeof(category_data_ptr->code_data.nw_type2_3gpp2_code_list.nw_type2_code_data[i].irm),
                ((uint8*)diag_req_ptr + data_offset),
                sizeof(category_data_ptr->code_data.nw_type2_3gpp2_code_list.nw_type2_code_data[i].irm));

        *offset_ptr += UIMDIAG_SIMLOCK_DATA_BLOCK_LEN;
      }
      break;

    case UIMDIAG_SIMLOCK_CATEGORY_3GPP2_RUIM:
      category_data_ptr->code_data.ruim_3gpp2_code_list.num_sim_codes =
        (uint8)uimdiag_simlock_decode_uint8(offset_ptr,
                                            diag_req_ptr,
                                            diag_req_len);

      if (category_data_ptr->code_data.ruim_3gpp2_code_list.num_sim_codes > SIMLOCK_NUM_CODES_MAX)
      {
        UIMDIAG_MSG_ERROR_1("SIM Lock number of codes: 0x%x",
                            category_data_ptr->code_data.ruim_3gpp2_code_list.num_sim_codes);
        return UIMDIAG_ERROR;
      }

      for (i = 0; i < category_data_ptr->code_data.ruim_3gpp2_code_list.num_sim_codes; i++)
      {
        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_0("CK parsing failed");
          return uimdiag_decode_status;
        }

        if ((*offset_ptr + UIMDIAG_SIMLOCK_DATA_BLOCK_LEN) > diag_req_len)
        {
          UIMDIAG_MSG_ERROR_1("SIM Lock data length too long. offset: 0x%x", *offset_ptr);
          return UIMDIAG_ERROR;
        }

        data_offset = *offset_ptr;

        uimdiag_decode_status = uimdiag_simlock_parse_nw_codes(
                                  diag_req_ptr,
                                  diag_req_len,
                                  &data_offset,
                                  &category_data_ptr->code_data.ruim_3gpp2_code_list.sim_code_data[i].nw_code);

        if (uimdiag_decode_status != UIMDIAG_SUCCESS)
        {
          UIMDIAG_MSG_ERROR_1("Failed to parse nw codes 0x%x", uimdiag_decode_status);
          return uimdiag_decode_status;
        }

        category_data_ptr->code_data.ruim_3gpp2_code_list.sim_code_data[i].imsi_msin.msin_len =
          (uint8)uimdiag_simlock_decode_uint8(&data_offset,
                                              diag_req_ptr,
                                              diag_req_len);

        if (category_data_ptr->code_data.ruim_3gpp2_code_list.sim_code_data[i].imsi_msin.msin_len > SIMLOCK_MSIN_MAX)
        {
          UIMDIAG_MSG_ERROR_1("MSIN data length too long. length: 0x%x",
                              category_data_ptr->code_data.ruim_3gpp2_code_list.sim_code_data[i].imsi_msin.msin_len);
          return UIMDIAG_ERROR;
        }

        memscpy(category_data_ptr->code_data.ruim_3gpp2_code_list.sim_code_data[i].imsi_msin.msin_data,
                sizeof(category_data_ptr->code_data.ruim_3gpp2_code_list.sim_code_data[i].imsi_msin.msin_data),
                ((uint8*)diag_req_ptr + data_offset),
                category_data_ptr->code_data.ruim_3gpp2_code_list.sim_code_data[i].imsi_msin.msin_len);

        *offset_ptr += UIMDIAG_SIMLOCK_DATA_BLOCK_LEN;
      }
      break;

    case UIMDIAG_SIMLOCK_CATEGORY_3GPP2_HRPD:
    case UIMDIAG_SIMLOCK_CATEGORY_3GPP2_SP:
    case UIMDIAG_SIMLOCK_CATEGORY_3GPP2_CP:
    case UIMDIAG_SIMLOCK_CATEGORY_3GPP_IMPI:
      break;
  }

  return uimdiag_decode_status;
} /* uimdiag_simlock_parse_codes */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_POPULATE_CK_LOCK_CMD

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_populate_ck_lock_cmd(
  simlock_set_lock_ck_msg_req_type        * cmd_ptr,
  const uint8                             * diag_req_ptr,
  uint32                                    diag_req_len)
{
  uint32                             offset           = 0;
  uint8                              uimdiag_slot     = 0;
  uint8                              category         = 0;
  uimdiag_simlock_category_enum_type uimdiag_category = UIMDIAG_SIMLOCK_CATEGORY_3GPP_NW;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(cmd_ptr, diag_req_ptr);

  uimdiag_decode_status = UIMDIAG_SUCCESS;

  uimdiag_slot = (simlock_slot_enum_type)uimdiag_simlock_decode_uint8(
                                           &offset,
                                           diag_req_ptr,
                                           diag_req_len);

  uimdiag_decode_status = uimdiag_simlock_convert_slot(uimdiag_slot,
                                                       &cmd_ptr->slot);

  category = uimdiag_simlock_decode_uint8(&offset,
                                          diag_req_ptr,
                                          diag_req_len);

  if(category > UIMDIAG_SIMLOCK_CATEGORY_3GPP_NS_SP)
  {
    UIMDIAG_MSG_ERROR_0("Out of range lock category");
    return UIMDIAG_ERROR;
  }
  uimdiag_category = (uimdiag_simlock_category_enum_type)category;

  cmd_ptr->category.category_type = uimdiag_simlock_map_category(uimdiag_category);

  cmd_ptr->auto_lock = (boolean)uimdiag_simlock_decode_uint8(&offset,
                                                             diag_req_ptr,
                                                             diag_req_len);

  cmd_ptr->blacklist = (boolean)uimdiag_simlock_decode_uint8(&offset,
                                                             diag_req_ptr,
                                                             diag_req_len);

  cmd_ptr->num_retries_max = (uint32)uimdiag_simlock_decode_uint32(&offset,
                                                                   diag_req_ptr,
                                                                   diag_req_len);

  cmd_ptr->iteration_cnt = (uint32)uimdiag_simlock_decode_uint32(&offset,
                                                                 diag_req_ptr,
                                                                 diag_req_len);

  cmd_ptr->ck.simlock_ck_length = (uint8)uimdiag_simlock_decode_uint8(&offset,
                                                                      diag_req_ptr,
                                                                      diag_req_len);

  if (uimdiag_decode_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_0("CK lock parsing failed");
    return uimdiag_decode_status;
  }

  if ((offset + cmd_ptr->ck.simlock_ck_length) > diag_req_len ||
      cmd_ptr->ck.simlock_ck_length > SIMLOCK_CK_MAX)
  {
    UIMDIAG_MSG_ERROR_1("CK length invalid: 0x%x", cmd_ptr->ck.simlock_ck_length);
    return UIMDIAG_ERROR;
  }

  if(cmd_ptr->ck.simlock_ck_length > 0)
  {
    memscpy(cmd_ptr->ck.simlock_ck_data,
            sizeof(cmd_ptr->ck.simlock_ck_data),
            ((uint8*)diag_req_ptr + offset),
            cmd_ptr->ck.simlock_ck_length);

    offset += cmd_ptr->ck.simlock_ck_length;
  }

  return uimdiag_simlock_parse_codes(diag_req_ptr,
                                     diag_req_len,
                                     &offset,
                                     &cmd_ptr->category,
                                     uimdiag_category);
}/* uimdiag_simlock_populate_ck_lock_cmd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_POPULATE_HCK_LOCK_CMD

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_populate_hck_lock_cmd(
  simlock_set_lock_hck_msg_req_type        * cmd_ptr,
  const uint8                              * diag_req_ptr,
  uint32                                     diag_req_len)
{
  uint32                             offset           = 0;
  uint8                              uimdiag_slot     = 0;
  uint8                              category         = 0;
  uimdiag_simlock_category_enum_type uimdiag_category = UIMDIAG_SIMLOCK_CATEGORY_3GPP_NW;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(cmd_ptr, diag_req_ptr);

  uimdiag_decode_status = UIMDIAG_SUCCESS;

  uimdiag_slot = (simlock_slot_enum_type)uimdiag_simlock_decode_uint8(
                                           &offset,
                                           diag_req_ptr,
                                           diag_req_len);

  uimdiag_decode_status = uimdiag_simlock_convert_slot(uimdiag_slot,
                                                       &cmd_ptr->slot);

  category = uimdiag_simlock_decode_uint8(&offset,
                                          diag_req_ptr,
                                          diag_req_len);

  if(category > UIMDIAG_SIMLOCK_CATEGORY_3GPP_ICCID)
  {
    UIMDIAG_MSG_ERROR_0("Out of range lock category");
    return UIMDIAG_ERROR;
  }
  uimdiag_category = (uimdiag_simlock_category_enum_type)category;

  cmd_ptr->category.category_type = uimdiag_simlock_map_category(uimdiag_category);

  cmd_ptr->auto_lock = (boolean)uimdiag_simlock_decode_uint8(&offset,
                                                             diag_req_ptr,
                                                             diag_req_len);

  cmd_ptr->blacklist = (boolean)uimdiag_simlock_decode_uint8(&offset,
                                                             diag_req_ptr,
                                                             diag_req_len);

  cmd_ptr->num_retries_max = (uint32)uimdiag_simlock_decode_uint32(&offset,
                                                                   diag_req_ptr,
                                                                   diag_req_len);

  cmd_ptr->iteration_cnt = (uint32)uimdiag_simlock_decode_uint32(&offset,
                                                                 diag_req_ptr,
                                                                 diag_req_len);

  if (uimdiag_decode_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_0("HCK lock parsing failed");
    return uimdiag_decode_status;
  }

  if ((offset + SIMLOCK_SALT_LEN + SIMLOCK_HCK_LEN) > diag_req_len)
  {
    UIMDIAG_MSG_ERROR_1("HCK length too short: 0x%x", diag_req_len);
    return UIMDIAG_ERROR;
  }

  memscpy(cmd_ptr->salt,
          sizeof(cmd_ptr->salt),
          ((uint8*)diag_req_ptr + offset),
          SIMLOCK_SALT_LEN);

  offset += SIMLOCK_SALT_LEN;

  memscpy(cmd_ptr->hck,
          sizeof(cmd_ptr->hck),
          ((uint8*)diag_req_ptr + offset),
          SIMLOCK_HCK_LEN);

  offset += SIMLOCK_HCK_LEN;

  return uimdiag_simlock_parse_codes(diag_req_ptr,
                                     diag_req_len,
                                     &offset,
                                     &cmd_ptr->category,
                                     uimdiag_category);
}/* uimdiag_simlock_populate_hck_lock_cmd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_POPULATE_UNLOCK_CMD

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_populate_unlock_cmd(
  simlock_unlock_device_msg_req_type      * cmd_ptr,
  void                                    * diag_req_ptr,
  uint32                                    diag_req_len)
{
  uint32                             offset           = 0;
  uint8                              uimdiag_slot     = 0;
  uint8                              category         = 0;
  uimdiag_simlock_category_enum_type uimdiag_category = UIMDIAG_SIMLOCK_CATEGORY_3GPP_NW;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(cmd_ptr, diag_req_ptr);

  uimdiag_decode_status = UIMDIAG_SUCCESS;

  uimdiag_slot = (simlock_slot_enum_type)uimdiag_simlock_decode_uint8(
                                           &offset,
                                           (uint8*)diag_req_ptr,
                                           diag_req_len);

  uimdiag_decode_status = uimdiag_simlock_convert_slot(uimdiag_slot,
                                                       &cmd_ptr->slot);

  category = uimdiag_simlock_decode_uint8(&offset,
                                          diag_req_ptr,
                                          diag_req_len);

  if(category > UIMDIAG_SIMLOCK_CATEGORY_3GPP_ICCID)
  {
    UIMDIAG_MSG_ERROR_0("Out of range lock category");
    return UIMDIAG_ERROR;
  }
  uimdiag_category = (uimdiag_simlock_category_enum_type)category;

  cmd_ptr->category = uimdiag_simlock_map_category(uimdiag_category);

  cmd_ptr->ck.simlock_ck_length = (uint8)uimdiag_simlock_decode_uint8(&offset,
                                                                      (uint8*)diag_req_ptr,
                                                                      diag_req_len);

  if (uimdiag_decode_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_0("CK parsing failed");
    return uimdiag_decode_status;
  }

  if ((offset + cmd_ptr->ck.simlock_ck_length) > diag_req_len ||
      cmd_ptr->ck.simlock_ck_length > SIMLOCK_CK_MAX)
  {
    UIMDIAG_MSG_ERROR_1("CK length too long: 0x%x", cmd_ptr->ck.simlock_ck_length);
    return UIMDIAG_ERROR;
  }

  memscpy(cmd_ptr->ck.simlock_ck_data,
          sizeof(cmd_ptr->ck.simlock_ck_data),
          ((uint8*)diag_req_ptr + offset),
          cmd_ptr->ck.simlock_ck_length);

  cmd_ptr->ignore_ck = FALSE;
  cmd_ptr->probe_ck  = FALSE;

  return uimdiag_decode_status;
}/* uimdiag_simlock_populate_unlock_cmd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_POPULATE_GET_CATEGORY_DATA_CMD

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_populate_get_category_data_cmd(
  simlock_get_category_data_msg_req_type  * cmd_ptr,
  void                                    * diag_req_ptr,
  uint32                                    diag_req_len)
{
  uint32                             offset           = 0;
  uint8                              uimdiag_slot     = 0;
  uint8                              category         = 0;
  uimdiag_simlock_category_enum_type uimdiag_category = UIMDIAG_SIMLOCK_CATEGORY_3GPP_NW;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(cmd_ptr, diag_req_ptr);

  uimdiag_decode_status = UIMDIAG_SUCCESS;

  uimdiag_slot = (simlock_slot_enum_type)uimdiag_simlock_decode_uint8(
                                           &offset,
                                           (uint8*)diag_req_ptr,
                                           diag_req_len);

  uimdiag_decode_status = uimdiag_simlock_convert_slot(uimdiag_slot,
                                                       &cmd_ptr->slot);

  category = uimdiag_simlock_decode_uint8(&offset,
                                          (uint8*)diag_req_ptr,
                                          diag_req_len);

  if(category > UIMDIAG_SIMLOCK_CATEGORY_3GPP_ICCID)
  {
    UIMDIAG_MSG_ERROR_0("Out of range lock category");
    return UIMDIAG_ERROR;
  }
  uimdiag_category = (uimdiag_simlock_category_enum_type)category;

  cmd_ptr->category = uimdiag_simlock_map_category(uimdiag_category);

  return uimdiag_decode_status;
}/* uimdiag_simlock_populate_get_category_data_cmd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_POPULATE_ADD_LOCK_CODES_CK_CMD

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_populate_add_lock_codes_ck_cmd(
  simlock_add_lock_codes_ck_msg_req_type  * cmd_ptr,
  const uint8                             * diag_req_ptr,
  uint32                                    diag_req_len)
{
  uint32                             offset           = 0;
  uint8                              uimdiag_slot     = 0;
  uint8                              category         = 0;
  uimdiag_simlock_category_enum_type uimdiag_category = UIMDIAG_SIMLOCK_CATEGORY_3GPP_NW;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(cmd_ptr, diag_req_ptr);

  uimdiag_decode_status = UIMDIAG_SUCCESS;

  uimdiag_slot = (simlock_slot_enum_type)uimdiag_simlock_decode_uint8(
                                           &offset,
                                           diag_req_ptr,
                                           diag_req_len);

  uimdiag_decode_status = uimdiag_simlock_convert_slot(uimdiag_slot,
                                                       &cmd_ptr->slot);

  category = uimdiag_simlock_decode_uint8(&offset,
                                          diag_req_ptr,
                                          diag_req_len);

  if(category > UIMDIAG_SIMLOCK_CATEGORY_3GPP_ICCID)
  {
    UIMDIAG_MSG_ERROR_0("Out of range lock category");
    return UIMDIAG_ERROR;
  }
  uimdiag_category = (uimdiag_simlock_category_enum_type)category;

  cmd_ptr->category.category_type = uimdiag_simlock_map_category(uimdiag_category);

  cmd_ptr->blacklist = (boolean)uimdiag_simlock_decode_uint8(&offset,
                                                             diag_req_ptr,
                                                             diag_req_len);

  cmd_ptr->ck.simlock_ck_length = (uint8)uimdiag_simlock_decode_uint8(&offset,
                                                                      diag_req_ptr,
                                                                      diag_req_len);

  if (uimdiag_decode_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_0("CK lock parsing failed");
    return uimdiag_decode_status;
  }

  if ((!cmd_ptr->blacklist && cmd_ptr->ck.simlock_ck_length == 0) ||
      (offset + cmd_ptr->ck.simlock_ck_length) > diag_req_len ||
      cmd_ptr->ck.simlock_ck_length > SIMLOCK_CK_MAX)
  {
    UIMDIAG_MSG_ERROR_2("CK length invalid: 0x%x blacklist 0x%x", cmd_ptr->ck.simlock_ck_length, cmd_ptr->blacklist);
    return UIMDIAG_ERROR;
  }

  if(cmd_ptr->ck.simlock_ck_length > 0)
  {
    memscpy(cmd_ptr->ck.simlock_ck_data,
            sizeof(cmd_ptr->ck.simlock_ck_data),
            ((uint8*)diag_req_ptr + offset),
            cmd_ptr->ck.simlock_ck_length);

    offset += cmd_ptr->ck.simlock_ck_length;
  }

  return uimdiag_simlock_parse_codes(diag_req_ptr,
                                     diag_req_len,
                                     &offset,
                                     &cmd_ptr->category,
                                     uimdiag_category);
}/* uimdiag_simlock_populate_add_lock_codes_ck_cmd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_POPULATE_ADD_LOCK_CODES_HCK_CMD

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_populate_add_lock_codes_hck_cmd(
  simlock_add_lock_codes_hck_msg_req_type  * cmd_ptr,
  const uint8                              * diag_req_ptr,
  uint32                                     diag_req_len)
{
  uint32                             offset           = 0;
  uint8                              uimdiag_slot     = 0;
  uint8                              category         = 0;
  uimdiag_simlock_category_enum_type uimdiag_category = UIMDIAG_SIMLOCK_CATEGORY_3GPP_NW;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(cmd_ptr, diag_req_ptr);

  uimdiag_decode_status = UIMDIAG_SUCCESS;

  uimdiag_slot = (simlock_slot_enum_type)uimdiag_simlock_decode_uint8(
                                           &offset,
                                           diag_req_ptr,
                                           diag_req_len);

  uimdiag_decode_status = uimdiag_simlock_convert_slot(uimdiag_slot,
                                                       &cmd_ptr->slot);

  category = uimdiag_simlock_decode_uint8(&offset,
                                          diag_req_ptr,
                                          diag_req_len);

  if(category > UIMDIAG_SIMLOCK_CATEGORY_3GPP_ICCID)
  {
    UIMDIAG_MSG_ERROR_0("Out of range lock category");
    return UIMDIAG_ERROR;
  }
  uimdiag_category = (uimdiag_simlock_category_enum_type)category;

  cmd_ptr->category.category_type = uimdiag_simlock_map_category(uimdiag_category);

  cmd_ptr->blacklist = (boolean)uimdiag_simlock_decode_uint8(&offset,
                                                             diag_req_ptr,
                                                             diag_req_len);

  if ((offset + SIMLOCK_HCK_LEN) > diag_req_len)
  {
    UIMDIAG_MSG_ERROR_0("HCK length too short");
    return UIMDIAG_ERROR;
  }

  memscpy(cmd_ptr->hck,
          sizeof(cmd_ptr->hck),
          ((uint8*)diag_req_ptr + offset),
          SIMLOCK_HCK_LEN);

  offset += SIMLOCK_HCK_LEN;

  return uimdiag_simlock_parse_codes(diag_req_ptr,
                                     diag_req_len,
                                     &offset,
                                     &cmd_ptr->category,
                                     uimdiag_category);
}/* uimdiag_simlock_populate_add_lock_codes_hck_cmd */


/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_POPULATE_CHECK_PUBLIC_KEY_CMD

DESCRIPTION
  This function populates the public key (modulus and exponent) to simlock remote lock structure

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_populate_check_public_key_cmd(
  simlock_remote_lock_public_key_type  * cmd_ptr,
  const uint8                          * diag_req_ptr,
  uint32                                 diag_req_len)
{
  uint32                             offset           = 0;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(cmd_ptr, diag_req_ptr);

  uimdiag_decode_status = UIMDIAG_SUCCESS;

  if(offset + SIMLOCK_PUBKEY_MODULUS_LEN + SIMLOCK_PUBKEY_EXPONENT_LEN > diag_req_len)
  {
    UIMDIAG_MSG_ERROR_1("Request length is less: 0x%x", diag_req_len);
    return UIMDIAG_ERROR;
  }

  memscpy(cmd_ptr->modulus,
          sizeof(cmd_ptr->modulus),
          (uint8*)diag_req_ptr,
          SIMLOCK_PUBKEY_MODULUS_LEN);

  offset += SIMLOCK_PUBKEY_MODULUS_LEN;

  memscpy(cmd_ptr->exponent,
          sizeof(cmd_ptr->exponent),
          ((uint8*)diag_req_ptr + offset),
          SIMLOCK_PUBKEY_EXPONENT_LEN);

  return uimdiag_decode_status;
}/* uimdiag_simlock_populate_check_public_key_cmd */


/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_POPULATE_PROBE_CK_CMD

DESCRIPTION
  This function populates simlock unlock request to probe CK

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_populate_probe_ck_cmd(
  simlock_unlock_device_msg_req_type   * cmd_ptr,
  const uint8                          * diag_req_ptr,
  uint32                                 diag_req_len)
{
  uint32                             offset           = 0;
  uint8                              uimdiag_slot     = 0;
  uint8                              category         = 0;
  uimdiag_simlock_category_enum_type uimdiag_category = UIMDIAG_SIMLOCK_CATEGORY_3GPP_NW;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(cmd_ptr, diag_req_ptr);

  uimdiag_decode_status = UIMDIAG_SUCCESS;

  uimdiag_slot = (simlock_slot_enum_type)uimdiag_simlock_decode_uint8(
                                           &offset,
                                           (uint8*)diag_req_ptr,
                                           diag_req_len);

  uimdiag_decode_status = uimdiag_simlock_convert_slot(uimdiag_slot,
                                                       &cmd_ptr->slot);

  category = uimdiag_simlock_decode_uint8(&offset,
                                          diag_req_ptr,
                                          diag_req_len);
  if(category > UIMDIAG_SIMLOCK_CATEGORY_3GPP_ICCID)
  {
    UIMDIAG_MSG_ERROR_0("Out of range lock category");
    return UIMDIAG_ERROR;
  }

  uimdiag_category = (uimdiag_simlock_category_enum_type)category;

  cmd_ptr->category = uimdiag_simlock_map_category(uimdiag_category);

  cmd_ptr->ck.simlock_ck_length = (uint8)uimdiag_simlock_decode_uint8(&offset,
                                                                      (uint8*)diag_req_ptr,
                                                                      diag_req_len);

  if (uimdiag_decode_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_0("CK parsing failed");
    return uimdiag_decode_status;
  }

  if ((offset + cmd_ptr->ck.simlock_ck_length) > diag_req_len ||
      cmd_ptr->ck.simlock_ck_length > SIMLOCK_CK_MAX)
  {
    UIMDIAG_MSG_ERROR_1("CK length too long: 0x%x", cmd_ptr->ck.simlock_ck_length);
    return UIMDIAG_ERROR;
  }

  memscpy(cmd_ptr->ck.simlock_ck_data,
          sizeof(cmd_ptr->ck.simlock_ck_data),
          ((uint8*)diag_req_ptr + offset),
          cmd_ptr->ck.simlock_ck_length);

  cmd_ptr->ignore_ck = FALSE;
  cmd_ptr->probe_ck  = TRUE;

  return uimdiag_decode_status;
}/* uimdiag_simlock_populate_probe_ck_cmd */


/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_POPULATE_MODE_CONFIG_CMD

DESCRIPTION
  This function populates simlock mode config request to configure required
  build configuration

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_populate_mode_config_cmd(
  simlock_set_rsu_mode_msg_req_type    * cmd_ptr,
  const uint8                          * diag_req_ptr,
  uint32                                 diag_req_len)
{
  uint32      offset = 0;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(cmd_ptr, diag_req_ptr);

  uimdiag_decode_status = UIMDIAG_SUCCESS;

  cmd_ptr->rsu_mode = (simlock_rsu_mode_enum_type)uimdiag_simlock_decode_uint8(
                                                          &offset,
                                                          (uint8*)diag_req_ptr,
                                                          diag_req_len);

  return uimdiag_decode_status;
}/* uimdiag_simlock_populate_mode_config_cmd */


/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_POPULATE_DEVICE_KEY_CMD

DESCRIPTION
  This function populates device key config request to configure required
  symmetric key in the device.

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_populate_device_key_cmd(
  simlock_set_rsu_device_key_msg_req_type  * cmd_ptr,
  const uint8                              * diag_req_ptr,
  uint32                                     diag_req_len)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cmd_ptr, diag_req_ptr);

  if (diag_req_len != SIMLOCK_RSU_DEVICE_KEY_LEN)
  {
    return UIMDIAG_ERROR;
  }

  uimdiag_decode_status = UIMDIAG_SUCCESS;

  memscpy(cmd_ptr->rsu_device_key,
          sizeof(cmd_ptr->rsu_device_key),
          (uint8*)diag_req_ptr,
          diag_req_len);

  return uimdiag_decode_status;
}/* uimdiag_simlock_populate_device_key_cmd */


static uimdiag_return_enum_type uimdiag_simlock_populate_set_configuration_cmd(
  uimdiag_set_configuration_msg_req_type   * cmd_ptr,
  const uint8                              * diag_req_ptr,
  uint32                                     diag_req_len)
{
  uint32                             offset           = 0;
  uint8                              uimdiag_slot     = 0;
  uint8                              category         = 0;
  uimdiag_simlock_category_enum_type uimdiag_category = UIMDIAG_SIMLOCK_CATEGORY_3GPP_NW;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(cmd_ptr, diag_req_ptr);

  uimdiag_decode_status = UIMDIAG_SUCCESS;
  
  /* Populate config id */
  cmd_ptr->config_id = uimdiag_simlock_decode_uint8(&offset,
                                                    (uint8*)diag_req_ptr,
                                                    diag_req_len);
  /*To check if config id is less max value allowed*/
  if (cmd_ptr->config_id > SIMLOCK_NUM_INACTIVE_CONFIG_MAX)
  {
    UIMDIAG_MSG_ERROR_1("config id 0x%x is greater than max value ", cmd_ptr->config_id);  	 
    return UIMDIAG_ERROR;
  }		

  cmd_ptr->config_nickname.nickname_len = (uint8)uimdiag_simlock_decode_uint32(
                                     &offset,
                                     (uint8*)diag_req_ptr,
                                     diag_req_len);

  if ((offset + cmd_ptr->config_nickname.nickname_len) > diag_req_len ||
      cmd_ptr->config_nickname.nickname_len > SIMLOCK_CONFIG_NICKNAME_LEN_MAX)
  {
    UIMDIAG_MSG_ERROR_1("nickname length too long: 0x%x", cmd_ptr->config_nickname.nickname_len);
    return UIMDIAG_ERROR;
  }

  memscpy(cmd_ptr->config_nickname.nickname,
          sizeof(cmd_ptr->config_nickname.nickname),
          ((uint8*)diag_req_ptr + offset),
          cmd_ptr->config_nickname.nickname_len);

  return uimdiag_decode_status;
}/* uimdiag_simlock_populate_set_configuration_cmd */


/*===========================================================================
FUNCTION: UIMDIAG_SIMLOCK_PARSE_CMD_DATA

DESCRIPTION:
  Populates the request structure with data from the Diag packet.

INPUTS:
  *req_ptr:       Simlock_command struct request pointer
                  which will be populated with data from the diag_req_ptr
  *diag_req_ptr:  Pointer to raw Diag request packet
  diag_req_len:   Length of the diag request packet

DEPENDENCIES:
  None

RETURN VALUE:
  uimdiag_return_enum_type

OUTPUT:
  None
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_parse_cmd_data (
  uimdiag_simlock_req_type  *req_ptr,
  PACKED void               *diag_req_ptr,
  uint32                     diag_req_len
)
{
  uimdiag_subsystem_cmd_enum_type uimdiag_subsys_cmd = UIMDIAG_MAX_CMD;

  if ((req_ptr == NULL) ||
      (diag_req_ptr == NULL) ||
      (diag_req_len < sizeof(uimdiag_cmd_header_type)))
  {
    UIMDIAG_MSG_ERROR_0("req_ptr or diag_req_ptr is NULL or diag_req_len to small:");
    return UIMDIAG_ERROR;
  }

  /* -----------------------------------------------------
  Parse following fields from diag buffer

  1. COMMAND CODE
  2. SUBSYSTEM ID
  3. SUBSYSTEM CMD CODE
  4. COMMAND Specific params
  ----------------------------------------------------- */
  /* command code */
  (void)memscpy( ((uint8*)&(req_ptr->cmd_header.command_code)),
                 sizeof(req_ptr->cmd_header.command_code),
                 (uint8*)diag_req_ptr, sizeof(uint8));

  diag_req_ptr  = (char*)diag_req_ptr + sizeof(uint8);
  diag_req_len -=  sizeof(uint8);
  
  /* subsystem id */
  (void)memscpy(((uint8*)&(req_ptr->cmd_header.subsys_id)),
                sizeof(req_ptr->cmd_header.subsys_id),
                (uint8*)diag_req_ptr, sizeof(diagpkt_subsys_id_type));

  diag_req_ptr  = (char*)diag_req_ptr + sizeof(diagpkt_subsys_id_type);
  diag_req_len -=  sizeof(diagpkt_subsys_id_type);

  /* subsystem command code */
  (void) memscpy( ((void*)&(req_ptr->cmd_header.subsys_cmd_code)),
                  sizeof(req_ptr->cmd_header.subsys_cmd_code),
                  (void*)diag_req_ptr,sizeof(diagpkt_subsys_cmd_code_type));

  diag_req_ptr  = (char*)diag_req_ptr + sizeof(diagpkt_subsys_cmd_code_type);
  diag_req_len -=  sizeof(diagpkt_subsys_cmd_code_type);

  uimdiag_subsys_cmd = (uimdiag_subsystem_cmd_enum_type)req_ptr->cmd_header.subsys_cmd_code;

  switch(uimdiag_subsys_cmd)
  {
    case UIMDIAG_SIMLOCK_SET_POLICY_CMD:
      return uimdiag_simlock_populate_set_policy_cmd(
                     &req_ptr->data.config_policy,
                     (void*)diag_req_ptr, diag_req_len);

    case UIMDIAG_SIMLOCK_CK_LOCK_CMD:
      return uimdiag_simlock_populate_ck_lock_cmd(
                     &req_ptr->data.lock_ck,
                     (void*)diag_req_ptr, diag_req_len);

    case UIMDIAG_SIMLOCK_HCK_LOCK_CMD:
      return uimdiag_simlock_populate_hck_lock_cmd(
                     &req_ptr->data.lock_hck,
                     (void*)diag_req_ptr, diag_req_len);

    case UIMDIAG_SIMLOCK_UNLOCK_CMD:
      return uimdiag_simlock_populate_unlock_cmd(
                     &req_ptr->data.unlock,
                     (void*)diag_req_ptr, diag_req_len);

    case UIMDIAG_SIMLOCK_GET_STATUS_CMD:
      return UIMDIAG_SUCCESS;

    case UIMDIAG_SIMLOCK_GET_RSU_MODE_CMD:
      return UIMDIAG_SUCCESS;

    case UIMDIAG_SIMLOCK_GET_CATEGORY_DATA_EXT_CMD:
      return uimdiag_simlock_populate_get_category_data_cmd(
                     &req_ptr->data.get_category_data,
                     (void*)diag_req_ptr, diag_req_len);

    case UIMDIAG_SIMLOCK_ADD_LOCK_CODES_CK_CMD:
      return uimdiag_simlock_populate_add_lock_codes_ck_cmd(
                     &req_ptr->data.add_lock_codes_ck,
                     (void*)diag_req_ptr, diag_req_len);

    case UIMDIAG_SIMLOCK_ADD_LOCK_CODES_HCK_CMD:
      return uimdiag_simlock_populate_add_lock_codes_hck_cmd(
                     &req_ptr->data.add_lock_codes_hck,
                     (void*)diag_req_ptr, diag_req_len);

    case UIMDIAG_SIMLOCK_REMOTE_LOCK_CHECK_PUBLIC_KEY_CMD:
      return uimdiag_simlock_populate_check_public_key_cmd(
                     &req_ptr->data.remote_lock_public_key,
                     (void *)diag_req_ptr, diag_req_len);

    case UIMDIAG_SIMLOCK_PROBE_CK_CMD:
      return uimdiag_simlock_populate_probe_ck_cmd(
                     &req_ptr->data.unlock,
                     (void *)diag_req_ptr, diag_req_len);

    case UIMDIAG_SIMLOCK_SET_RSU_MODE_CMD:
      return uimdiag_simlock_populate_mode_config_cmd(
                     &req_ptr->data.rsu_mode,
                     (void *)diag_req_ptr, diag_req_len);

    case UIMDIAG_SIMLOCK_SET_RSU_KEY_CMD:
      return uimdiag_simlock_populate_device_key_cmd(
                       &req_ptr->data.rsu_device_key,
                       (void *)diag_req_ptr, diag_req_len);

    case UIMDIAG_SIMLOCK_SET_CONFIGURATION_CMD:
      return uimdiag_simlock_populate_set_configuration_cmd(
                       &req_ptr->data.set_configuration,
                       (void *)diag_req_ptr, diag_req_len);

    default:
      UIMDIAG_MSG_ERROR_1("Invalid subcmd 0x%x", uimdiag_subsys_cmd);
      return UIMDIAG_ERROR;
  }
} /* uimdiag_simlock_parse_cmd_data */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_PARSE_COMMANDS

DESCRIPTION
  Function to preparse and populate the response pointer with the conformation
  data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_parse_commands(
  PACKED void                           * cmd_ptr,
  uint16                                  pkt_len,
  uimdiag_generic_rsp_type             ** rsp_pptr,
  diagpkt_subsys_delayed_rsp_id_type    * rsp_id_ptr,
  int32                                 * index_ptr,
  uimdiag_simlock_req_type              * request_ptr,
  uimdiag_subsystem_cmd_enum_type         subsys_cmd
)
{
  uimdiag_return_enum_type              uimdiag_status         = UIMDIAG_ERROR;

  UIMDIAG_UTIL_RETURN_IF_NULL_3(cmd_ptr, rsp_pptr, rsp_id_ptr);
  UIMDIAG_UTIL_RETURN_IF_NULL_2(index_ptr, request_ptr);

  /*  allocate immediate response pointer  */
  *rsp_pptr = (uimdiag_generic_rsp_type *)
                diagpkt_subsys_alloc_v2(
                  (diagpkt_subsys_id_type) DIAG_SUBSYS_GSDI,
                  (diagpkt_subsys_cmd_code_type) subsys_cmd,
                  sizeof(uimdiag_generic_rsp_type));

  if (*rsp_pptr == NULL)
  {
    return UIMDIAG_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  memset(&(*rsp_pptr)->rsp_payload,0x00, sizeof(uimdiag_cmd_rsp_pyld_type));

  /* Initialize to SIMLOCK_GENERIC_ERROR for the immediate response */
  (*rsp_pptr)->rsp_payload.mmgsdi_status = (mmgsdi_return_enum_type)SIMLOCK_GENERIC_ERROR;

  *rsp_id_ptr = diagpkt_subsys_get_delayed_rsp_id(*rsp_pptr );

  /* extract data from req packet
  and populate fields in the request structure */
  uimdiag_status = uimdiag_simlock_parse_cmd_data(request_ptr, cmd_ptr, pkt_len);

  if (uimdiag_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_1("uimdiag_simlock_parse_commands: Parsing error 0x%x",
                        uimdiag_status);
    /* Do an immediate rsp - No delayed response expected*/
    diagpkt_subsys_reset_delayed_rsp_id(*rsp_pptr);
    diagpkt_subsys_set_status(*rsp_pptr, (uint32) UIMDIAG_SUCCESS);
    diagpkt_commit(*rsp_pptr);
    return uimdiag_status;
  }
  /* find a free space in req_table to hold information for callback */
  *index_ptr = uimdiag_get_index();
  if (*index_ptr == UIMDIAG_GET_FN_ERR )
  {
    /*  req_table is full */
    uimdiag_status = UIMDIAG_MAX_ASYNC_REQ_EXCEED_ERR;
    UIMDIAG_MSG_ERROR_1("uimdiag_simlock_parse_commands: Parsing error 0x%x",
                        uimdiag_status);
    /* Do an immediate rsp - No delayed response expected*/
    diagpkt_subsys_reset_delayed_rsp_id(*rsp_pptr);
    diagpkt_subsys_set_status(*rsp_pptr, (uint32) UIMDIAG_SUCCESS);
    diagpkt_commit(*rsp_pptr);
    return uimdiag_status;
  }

  return UIMDIAG_SUCCESS;
} /* uimdiag_simlock_parse_commands */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_POST_PARSE_COMMANDS

DESCRIPTION
  Function post parsing the command to commit delayed response

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_post_parse_commands(
  uimdiag_generic_rsp_type             ** rsp_pptr,
  diagpkt_subsys_delayed_rsp_id_type      rsp_id,
  int32                                   index,
  simlock_result_enum_type                simlock_status,
  uimdiag_subsystem_cmd_enum_type         subsys_cmd
)
{
  uimdiag_generic_rsp_type               * immediate_rsp_ptr = NULL;
  UIMDIAG_UTIL_RETURN_IF_NULL(rsp_pptr);

  diagpkt_subsys_set_status(* rsp_pptr, (uint32) UIMDIAG_SUCCESS);
  diagpkt_commit(* rsp_pptr);

  if ( SIMLOCK_SUCCESS != simlock_status )
  {
    /* error in handling API , we need a delayed response too */
    UIMDIAG_MSG_ERROR_2("FAILED TO QUEUE SIMLOCK COMMAND, SIMLOCK status is 0x%x for command 0x%x",
                         simlock_status, subsys_cmd);
    uimdiag_free_index(index);
    immediate_rsp_ptr = (uimdiag_generic_rsp_type *)
                        diagpkt_subsys_alloc_v2_delay(
                          (diagpkt_subsys_id_type) DIAG_SUBSYS_GSDI,
                          (diagpkt_subsys_cmd_code_type) subsys_cmd,
                           rsp_id,
                           sizeof(uimdiag_generic_rsp_type));

    if (immediate_rsp_ptr == NULL)
    {
      return UIMDIAG_ERROR;
    }

    memset(&immediate_rsp_ptr->rsp_payload, 0x00, sizeof(uimdiag_cmd_rsp_pyld_type));
    immediate_rsp_ptr->rsp_payload.mmgsdi_status = (mmgsdi_return_enum_type)simlock_status;
    diagpkt_subsys_set_status(immediate_rsp_ptr,(uint32)UIMDIAG_SUCCESS);
    rex_sleep(10);
    diagpkt_delay_commit(immediate_rsp_ptr);
    return UIMDIAG_SUCCESS;
  }

  /*Set the immediate response to SUCCESS*/
  (*rsp_pptr)->rsp_payload.mmgsdi_status = (mmgsdi_return_enum_type)SIMLOCK_SUCCESS;
  return UIMDIAG_SUCCESS;
} /* uimdiag_simlock_post_parse_commands */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_PROCESS_UNLOCK_RESP

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_process_unlock_resp(
  const simlock_unlock_device_msg_resp_type      * simlock_unlock_ptr,
  uimdiag_simlock_unlock_rsp_type                * uimdiag_unlock_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(simlock_unlock_ptr, uimdiag_unlock_ptr);

  uimdiag_unlock_ptr->num_retries_max =
    simlock_unlock_ptr->num_retries_max;
  uimdiag_unlock_ptr->curr_retries =
    simlock_unlock_ptr->curr_retries;

  return UIMDIAG_SUCCESS;
} /* uimdiag_simlock_process_unlock_resp */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_PROCESS_TOKEN_RESP

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_process_token_resp(
  const simlock_token_type               * simlock_token_ptr,
  uimdiag_simlock_token_rsp_type         * uimdiag_token_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(simlock_token_ptr, uimdiag_token_ptr);

  if(simlock_token_ptr->token_present)
  {
    memscpy(uimdiag_token_ptr->simlock_token, sizeof(uimdiag_token_ptr->simlock_token),
            simlock_token_ptr->token_data, SIMLOCK_TOKEN_LEN);
  }
  else
  {
    memset(uimdiag_token_ptr->simlock_token, 0x00, sizeof(uimdiag_simlock_token_rsp_type));
  }

  return UIMDIAG_SUCCESS;
} /* uimdiag_simlock_process_token_resp */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_PROCESS_GET_STATUS_RESP

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_process_get_status_resp(
  const simlock_get_status_msg_resp_type         * simlock_get_status_ptr,
  uimdiag_simlock_get_status_rsp_type            * uimdiag_get_status_ptr
)
{
  uint32  i = 0;
  uint32  j = 0;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(simlock_get_status_ptr, uimdiag_get_status_ptr);

  uimdiag_get_status_ptr->slot_policy =
    simlock_get_status_ptr->slot_policy;
  uimdiag_get_status_ptr->subscription_policy =
    simlock_get_status_ptr->subscription_policy;

  for (i = 0; i < SIMLOCK_SLOT_COUNT_MAX; i++)
  {
    for (j = 0; j < SIMLOCK_CATEGORY_COUNT_MAX; j++)
    {
      uimdiag_get_status_ptr->lock_info[i].category_info[j].category =
        (uint8)uimdiag_simlock_map_simlock_category(simlock_get_status_ptr->lock_info[i].category_info[j].category);
      uimdiag_get_status_ptr->lock_info[i].category_info[j].category_status =
        simlock_get_status_ptr->lock_info[i].category_info[j].category_status;
      uimdiag_get_status_ptr->lock_info[i].category_info[j].blacklist =
        simlock_get_status_ptr->lock_info[i].category_info[j].blacklist;
      uimdiag_get_status_ptr->lock_info[i].category_info[j].num_retries_max =
        simlock_get_status_ptr->lock_info[i].category_info[j].num_retries_max;
      uimdiag_get_status_ptr->lock_info[i].category_info[j].curr_retries =
        simlock_get_status_ptr->lock_info[i].category_info[j].curr_retries;
    }
  }

  return UIMDIAG_SUCCESS;
} /* uimdiag_simlock_process_get_status_resp */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_PROCESS_GET_RSU_MODE_RESP

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_process_get_rsu_mode_resp(
  const simlock_get_rsu_mode_msg_resp_type     * simlock_get_status_ptr,
  uimdiag_simlock_get_rsu_mode_rsp_type        * uimdiag_get_status_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(simlock_get_status_ptr, uimdiag_get_status_ptr);

  uimdiag_get_status_ptr->get_rsu_mode =
    simlock_get_status_ptr->rsu_mode;

  return UIMDIAG_SUCCESS;
} /* uimdiag_simlock_process_get_rsu_mode_resp */

/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_GET_CATEGORY_FROM_RESP

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_get_category_from_resp
(
  const simlock_get_category_data_msg_resp_type   * simlock_get_category_data_ptr,
  simlock_category_enum_type                      * category_ptr
)
{
  if((simlock_get_category_data_ptr == NULL) ||
     (category_ptr == NULL))
  {
    return UIMDIAG_ERROR;
  }

  if(simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
  {
    *category_ptr =
         simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr->category_type;
    return UIMDIAG_SUCCESS;
  }
  else if (simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL)
  {
    *category_ptr =
         simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr->category_type;
    return UIMDIAG_SUCCESS;
  }

  return UIMDIAG_ERROR;
} /* uimdiag_simlock_get_category_from_resp */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_GET_NUMBER_OF_CODES_FROM_RESP

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uint8

SIDE EFFECTS
  None.
===========================================================================*/
static uint8 uimdiag_simlock_get_number_of_codes_from_resp
(
  const simlock_get_category_data_msg_resp_type   * simlock_get_category_data_ptr
)
{
  simlock_category_enum_type   category     = SIMLOCK_CATEGORY_3GPP_NW;
  uint8                        num_of_codes = 0;
  uint8                        list_cnt     = 0;

  if(simlock_get_category_data_ptr == NULL)
  {
    return 0;
  }

  if(uimdiag_simlock_get_category_from_resp(simlock_get_category_data_ptr,
                                            &category) != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_0("uimdiag_simlock_get_category_from_resp failed!");
    return 0;
  }

  UIMDIAG_MSG_MED_1("category: 0x%x", category);

  switch (category)
  {
    case SIMLOCK_CATEGORY_3GPP_NW:
     if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.nw_3gpp_code_list.num_nw_codes;
       }
     }

     if(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.nw_3gpp_code_list.num_nw_codes;
       }
     }
     return num_of_codes;

   case SIMLOCK_CATEGORY_3GPP_NS:
     if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_3gpp_code_list.num_ns_codes;
       }
     }

     if(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_3gpp_code_list.num_ns_codes;
       }
     }
     return num_of_codes;

   case SIMLOCK_CATEGORY_3GPP_SP:
     if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.num_sp_codes;
       }
     }

     if(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.num_sp_codes;
       }
     }
     return num_of_codes;

   case SIMLOCK_CATEGORY_3GPP_CP:
     if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.num_cp_codes;
       }
     }

     if(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.num_cp_codes;
       }
     }
     return num_of_codes;

   case SIMLOCK_CATEGORY_3GPP_SIM:
     if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sim_3gpp_code_list.num_sim_codes;
       }
     }

     if(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sim_3gpp_code_list.num_sim_codes;
       }
     }
     return num_of_codes;

   case SIMLOCK_CATEGORY_3GPP_SPN:
     if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.spn_3gpp_code_list.num_spn_codes;
       }
     }

     if(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.spn_3gpp_code_list.num_spn_codes;
       }
     }
     return num_of_codes;

   case SIMLOCK_CATEGORY_3GPP_SP_EHPLMN:
     if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.num_sp_ehplmn_codes;
       }
     }

     if(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.num_sp_ehplmn_codes;
       }
     }
     return num_of_codes;

   case SIMLOCK_CATEGORY_3GPP_ICCID:
     if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.num_iccid_codes;
       }
     }

     if(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.num_iccid_codes;
       }
     }
     return num_of_codes;

    case SIMLOCK_CATEGORY_3GPP_NS_SP:
     if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.num_ns_sp_codes;
       }
     }

     if(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.num_ns_sp_codes;
       }
     }
     return num_of_codes;

   case SIMLOCK_CATEGORY_3GPP2_NW_TYPE1:
     if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.nw_type1_3gpp2_code_list.num_nw_codes;
       }
     }

     if(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.nw_type1_3gpp2_code_list.num_nw_codes;
       }
     }
     return num_of_codes;

    case SIMLOCK_CATEGORY_3GPP2_NW_TYPE2:
     if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.nw_type2_3gpp2_code_list.num_nw_type2_codes;
       }
     }

     if(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.nw_type2_3gpp2_code_list.num_nw_type2_codes;
       }
     }
     return num_of_codes;

   case SIMLOCK_CATEGORY_3GPP2_RUIM:
     if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.num_sim_codes;
       }
     }

     if(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL)
     {
       for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
           list_cnt++)
       {
         num_of_codes +=
            simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.num_sim_codes;
       }
     }
     return num_of_codes;

    case SIMLOCK_CATEGORY_3GPP2_HRPD:
    case SIMLOCK_CATEGORY_3GPP2_SP:
    case SIMLOCK_CATEGORY_3GPP2_CP:
    default:
      break;
  }

  return num_of_codes;
} /* uimdiag_simlock_get_number_of_codes_from_resp*/


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_COPY_MSIN_CODE_TO_RESP

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_copy_msin_code_to_resp(
  const simlock_msin_type                           * simlock_msin_data_ptr,
  uint8                                               uimdiag_resp_data_len,
  uint8                                             * uimdiag_resp_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(simlock_msin_data_ptr, uimdiag_resp_data_ptr);

  if (simlock_msin_data_ptr->msin_len > SIMLOCK_MSIN_MAX ||
      simlock_msin_data_ptr->msin_len == 0)
  {
    UIMDIAG_MSG_ERROR_0("Invalid MSIN length");
    return UIMDIAG_ERROR;
  }

  if (UIMDIAG_MSIN_LEN_OFFSET < uimdiag_resp_data_len)
  {
    memscpy(&uimdiag_resp_data_ptr[UIMDIAG_MSIN_LEN_OFFSET],
            sizeof(simlock_msin_data_ptr->msin_len),
            &simlock_msin_data_ptr->msin_len,
            sizeof(simlock_msin_data_ptr->msin_len));
  }

  if ((UIMDIAG_MSIN_OFFSET + simlock_msin_data_ptr->msin_len) < uimdiag_resp_data_len)
  {
    memscpy(&uimdiag_resp_data_ptr[UIMDIAG_MSIN_OFFSET],
            SIMLOCK_MSIN_MAX,
            simlock_msin_data_ptr->msin_data,
            simlock_msin_data_ptr->msin_len);
  }

  return UIMDIAG_SUCCESS;
} /* uimdiag_simlock_copy_msin_code_to_resp */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_COPY_NW_CODE_TO_RESP

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_copy_nw_code_to_resp(
  const simlock_nw_code_data_type                   * simlock_nw_data_ptr,
  uint8                                               uimdiag_resp_data_len,
  uint8                                             * uimdiag_resp_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(simlock_nw_data_ptr, uimdiag_resp_data_ptr);

  if ((UIMDIAG_MCC_OFFSET + SIMLOCK_MCC_LEN) < uimdiag_resp_data_len)
  {
    memscpy(&uimdiag_resp_data_ptr[UIMDIAG_MCC_OFFSET],
            SIMLOCK_MCC_LEN,
            simlock_nw_data_ptr->mcc,
            sizeof(simlock_nw_data_ptr->mcc));
  }

  if (simlock_nw_data_ptr->mnc.mnc_len > SIMLOCK_MNC_MAX ||
      simlock_nw_data_ptr->mnc.mnc_len == 0)
  {
    UIMDIAG_MSG_ERROR_0("Invalid MNC length");
    return UIMDIAG_ERROR;
  }

  if (UIMDIAG_MNC_LEN_OFFSET < uimdiag_resp_data_len)
  {
    memscpy(&uimdiag_resp_data_ptr[UIMDIAG_MNC_LEN_OFFSET],
            sizeof(simlock_nw_data_ptr->mnc.mnc_len),
            &simlock_nw_data_ptr->mnc.mnc_len,
            sizeof(simlock_nw_data_ptr->mnc.mnc_len));
  }

  if ((UIMDIAG_MNC_OFFSET + SIMLOCK_MNC_MAX) < uimdiag_resp_data_len)
  {
    memscpy(&uimdiag_resp_data_ptr[UIMDIAG_MNC_OFFSET],
            SIMLOCK_MNC_MAX,
            simlock_nw_data_ptr->mnc.mnc_data,
            simlock_nw_data_ptr->mnc.mnc_len);
  }

  return UIMDIAG_SUCCESS;
} /* uimdiag_simlock_copy_nw_code_to_resp */


/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_COPY_PACKED_NW_CODE_TO_RESP

DESCRIPTION
  Packs MCC-MNC values and copies it to uimdiag response.
  MCC-MNC can be inserted at any place in response based
  on the uimdiag_data_offset provided.

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_copy_packed_nw_code_to_resp(
  const simlock_nw_code_data_type                   * simlock_nw_data_ptr,
  uint8                                               uimdiag_resp_data_len,
  uint8                                               uimdiag_data_offset,
  uint8                                             * uimdiag_resp_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(simlock_nw_data_ptr, uimdiag_resp_data_ptr);

  if(simlock_nw_data_ptr->mcc[0] == UIMDIAG_LOCK_CODE_WILD_CHAR)
  {
    uimdiag_resp_data_ptr[uimdiag_data_offset] = 0xE0;
  }
  else
  {
    uimdiag_resp_data_ptr[uimdiag_data_offset] = ((simlock_nw_data_ptr->mcc[0] -'0') << 0x04);
  }

  if(simlock_nw_data_ptr->mcc[1] == UIMDIAG_LOCK_CODE_WILD_CHAR)
  {
    uimdiag_resp_data_ptr[uimdiag_data_offset] = uimdiag_resp_data_ptr[uimdiag_data_offset] | 0x0E;
  }
  else
  {
    uimdiag_resp_data_ptr[uimdiag_data_offset] = uimdiag_resp_data_ptr[uimdiag_data_offset] | (simlock_nw_data_ptr->mcc[1] -'0');
  }

  if(simlock_nw_data_ptr->mcc[2] == UIMDIAG_LOCK_CODE_WILD_CHAR)
  {
    uimdiag_resp_data_ptr[uimdiag_data_offset + 1] = 0xE0;
  }
  else
  {
    uimdiag_resp_data_ptr[uimdiag_data_offset + 1] = (simlock_nw_data_ptr->mcc[2] -'0') << 0x04;
  }


  if(simlock_nw_data_ptr->mnc.mnc_len == SIMLOCK_MNC_MAX)
  {
    if(simlock_nw_data_ptr->mnc.mnc_data[0] == UIMDIAG_LOCK_CODE_WILD_CHAR)
    {
      uimdiag_resp_data_ptr[uimdiag_data_offset + 1] = uimdiag_resp_data_ptr[uimdiag_data_offset + 1] | 0x0E;
    }
    else
    {
      uimdiag_resp_data_ptr[uimdiag_data_offset + 1] = uimdiag_resp_data_ptr[uimdiag_data_offset + 1] | (simlock_nw_data_ptr->mnc.mnc_data[0] - '0');
    }

    if(simlock_nw_data_ptr->mnc.mnc_data[1] == UIMDIAG_LOCK_CODE_WILD_CHAR)
    {
      uimdiag_resp_data_ptr[uimdiag_data_offset + 2] = 0xE0;
    }
    else
    {
      uimdiag_resp_data_ptr[uimdiag_data_offset + 2] = (simlock_nw_data_ptr->mnc.mnc_data[1] - '0') << 0x04;
    }

    if(simlock_nw_data_ptr->mnc.mnc_data[2] == UIMDIAG_LOCK_CODE_WILD_CHAR)
    {
      uimdiag_resp_data_ptr[uimdiag_data_offset + 2] = uimdiag_resp_data_ptr[uimdiag_data_offset + 2] | 0x0E;
    }
    else
    {
      uimdiag_resp_data_ptr[uimdiag_data_offset + 2] = uimdiag_resp_data_ptr[uimdiag_data_offset + 2] | (simlock_nw_data_ptr->mnc.mnc_data[2] - '0');
    }

  }
  else if(simlock_nw_data_ptr->mnc.mnc_len == 0x02)
  {
    uimdiag_resp_data_ptr[uimdiag_data_offset + 1] = uimdiag_resp_data_ptr[uimdiag_data_offset + 1] | 0x0F;

    if(simlock_nw_data_ptr->mnc.mnc_data[0] == UIMDIAG_LOCK_CODE_WILD_CHAR)
    {
      uimdiag_resp_data_ptr[uimdiag_data_offset + 2] = 0xE0;
    }
    else
    {
      uimdiag_resp_data_ptr[uimdiag_data_offset + 2] = (simlock_nw_data_ptr->mnc.mnc_data[0] - '0') << 0x04;
    }

    if(simlock_nw_data_ptr->mnc.mnc_data[1] == UIMDIAG_LOCK_CODE_WILD_CHAR)
    {
      uimdiag_resp_data_ptr[uimdiag_data_offset + 2] =  uimdiag_resp_data_ptr[uimdiag_data_offset + 2] | 0x0E;
    }
    else
    {
      uimdiag_resp_data_ptr[uimdiag_data_offset + 2] =  uimdiag_resp_data_ptr[uimdiag_data_offset + 2] | (simlock_nw_data_ptr->mnc.mnc_data[1] - '0');
    }

  }
  else
  {
    UIMDIAG_MSG_ERROR_0("Invalid MNC length");
    return UIMDIAG_ERROR;
  }

  return UIMDIAG_SUCCESS;
} /* uimdiag_simlock_copy_packed_nw_code_to_resp */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_CONVERT_ASCII_TO_BCD

DESCRIPTION
  This function converts ascii string to BCD string

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_convert_ascii_to_bcd(
  uint8         ascii_digit_len,
  const char  * ascii_string_ptr,
  uint8       * bcd_string_ptr
)
{
  uint8 index = 0;

  if(ascii_digit_len == 0 ||
     ascii_digit_len > SIMLOCK_ICCID_DIGITS_MAX ||
     bcd_string_ptr == NULL ||
     ascii_string_ptr == NULL)
  {
    return UIMDIAG_ERROR;
  }

  for(index = 0; index < ascii_digit_len; index++)
  {
    uint8 bcd = 0;

    if((ascii_string_ptr[index] >= '0') && 
       (ascii_string_ptr[index] <= '9'))
    {
      bcd = (0x0F & (ascii_string_ptr[index] - '0'));
    }
    else if(ascii_string_ptr[index] == '*')
    {
      bcd = 0x0F;
    }
    else
    {
      return UIMDIAG_ERROR;
    }

    /* For even index, copy bcd value to lower nibble and for odd index,  copy
       bcd value to upper nibble */
    if(index % 2)
    {
      bcd_string_ptr[index/2] = bcd_string_ptr[index/2] | (bcd << 4);
    }
    else
    {
      bcd_string_ptr[index/2] = bcd_string_ptr[index/2] | bcd;
    }
  }
  return UIMDIAG_SUCCESS;
}/* uimdiag_simlock_convert_ascii_to_bcd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_PROCESS_GET_CATEGORY_DATA_EXT_RESP

DESCRIPTION
  This function processes the get category data response

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_simlock_process_get_category_data_ext_resp(
  const simlock_get_category_data_msg_resp_type         * simlock_get_category_data_ptr,
  uimdiag_simlock_get_category_data_ext_rsp_type        * uimdiag_get_category_data_ext_ptr
)
{
  uimdiag_return_enum_type           uimdiag_status = UIMDIAG_SUCCESS;
  simlock_category_enum_type         category       = SIMLOCK_CATEGORY_3GPP_NW;
  uimdiag_simlock_code_data_type   * code_data_ptr  = NULL;
  uint8                              list_cnt       = 0;
  uint8                              code_cnt       = 0;
  uint8                              i              = 0;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(simlock_get_category_data_ptr, uimdiag_get_category_data_ext_ptr);

  if(uimdiag_simlock_get_category_from_resp(simlock_get_category_data_ptr,
                                            &category) != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_0("uimdiag_simlock_get_category_from_resp failed!");
    return UIMDIAG_ERROR;
  }

  uimdiag_get_category_data_ext_ptr->category_type = (uint8)uimdiag_simlock_map_simlock_category(category);
  uimdiag_get_category_data_ext_ptr->num_of_codes  = uimdiag_simlock_get_number_of_codes_from_resp(simlock_get_category_data_ptr);

  /* point to the first data set */
  code_data_ptr = &uimdiag_get_category_data_ext_ptr->code_data;
  if(code_data_ptr == NULL)
  {
    return UIMDIAG_ERROR;
  }

  switch (category)
  {
    case SIMLOCK_CATEGORY_3GPP_NW:
      if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.nw_3gpp_code_list.num_nw_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates whitelist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x00;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.nw_3gpp_code_list.nw_code_data[i],
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }
          }
        }
      }
      if ((simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL) &&
          (code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes))
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.nw_3gpp_code_list.num_nw_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates blacklist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x01;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.nw_3gpp_code_list.nw_code_data[i],
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }
          }
        }
      }
      break;

    case SIMLOCK_CATEGORY_3GPP_NS:
      if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_3gpp_code_list.num_ns_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates whitelist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x00;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_3gpp_code_list.ns_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            code_data_ptr[code_cnt].code[UIMDIAG_D6_OFFSET] =
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_3gpp_code_list.ns_code_data[i].imsi_digit6;

            code_data_ptr[code_cnt].code[UIMDIAG_D7_OFFSET] =
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_3gpp_code_list.ns_code_data[i].imsi_digit7;
          }
        }
      }
      if ((simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL) &&
          (code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes))
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_3gpp_code_list.num_ns_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates whitelist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x01;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_3gpp_code_list.ns_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            code_data_ptr[code_cnt].code[UIMDIAG_D6_OFFSET] =
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_3gpp_code_list.ns_code_data[i].imsi_digit6;

            code_data_ptr[code_cnt].code[UIMDIAG_D7_OFFSET] =
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_3gpp_code_list.ns_code_data[i].imsi_digit7;
          }
        }
      }
      break;

    case SIMLOCK_CATEGORY_3GPP_SP:
      if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.num_sp_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates whitelist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x00;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            /* For SP lock, 5 bytes GID is supported only. So, the lock codes
               provided by UIMDIAG while locking should have 5 bytes GID only */
            if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_len > UIMDIAG_SUPPORTED_GID_LEN_MAX ||
                simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_len == 0)
            {
              UIMDIAG_MSG_ERROR_0("Invalid GID1 length");
              return UIMDIAG_ERROR;
            }

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID1_LEN_OFFSET],
                    sizeof(simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_len),
                    &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_len,
                    sizeof(simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_len));

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID1_OFFSET],
              UIMDIAG_SUPPORTED_GID_LEN_MAX,
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_data,
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_len);
          }
        }
      }
      if ((simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL) &&
          (code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes))
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.num_sp_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates blacklist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x01;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            if (simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_len > UIMDIAG_SUPPORTED_GID_LEN_MAX ||
                simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_len == 0)
            {
              UIMDIAG_MSG_ERROR_0("Invalid GID1 length");
              return UIMDIAG_ERROR;
            }

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID1_LEN_OFFSET],
                    sizeof(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_len),
                    &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_len,
                    sizeof(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_len));

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID1_OFFSET],
              UIMDIAG_SUPPORTED_GID_LEN_MAX,
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_data,
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[i].gid1.gid_len);
          }
        }
      }
      break;

    case SIMLOCK_CATEGORY_3GPP_CP:
      if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.num_cp_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates whitelist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x00;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }
            /* For CP lock, 5 bytes GID is supported only. So, the lock codes
               provided by UIMDIAG while locking should have 5 bytes GID only */
            if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_len > UIMDIAG_SUPPORTED_GID_LEN_MAX ||
                simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_len == 0)
            {
              UIMDIAG_MSG_ERROR_0("Invalid GID1 length");
              return UIMDIAG_ERROR;
            }

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID1_LEN_OFFSET],
                    sizeof(simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_len),
                    &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_len,
                    sizeof(simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_len));

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID1_OFFSET],
              UIMDIAG_SUPPORTED_GID_LEN_MAX,
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_data,
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_len);

            if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_len > UIMDIAG_SUPPORTED_GID_LEN_MAX ||
                simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_len == 0)
            {
              UIMDIAG_MSG_ERROR_0("Invalid GID2 length");
              return UIMDIAG_ERROR;
            }

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID2_LEN_OFFSET],
                    sizeof(simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_len),
                    &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_len,
                    sizeof(simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_len));

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID2_OFFSET],
              UIMDIAG_SUPPORTED_GID_LEN_MAX,
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_data,
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_len);
          }
        }
      }
      if ((simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL) &&
          (code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes))
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.num_cp_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates blacklist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x01;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            if (simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_len > UIMDIAG_SUPPORTED_GID_LEN_MAX ||
                simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_len == 0)
            {
              UIMDIAG_MSG_ERROR_0("Invalid GID1 length");
              return UIMDIAG_ERROR;
            }

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID1_LEN_OFFSET],
                    sizeof(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_len),
                    &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_len,
                    sizeof(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_len));

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID1_OFFSET],
              UIMDIAG_SUPPORTED_GID_LEN_MAX,
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_data,
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid1.gid_len);

            if (simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_len > UIMDIAG_SUPPORTED_GID_LEN_MAX ||
                simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_len == 0)
            {
              UIMDIAG_MSG_ERROR_0("Invalid GID2 length");
              return UIMDIAG_ERROR;
            }

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID2_LEN_OFFSET],
                    sizeof(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_len),
                    &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_len,
                    sizeof(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_len));

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID2_OFFSET],
              UIMDIAG_SUPPORTED_GID_LEN_MAX,
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_data,
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[i].gid2.gid_len);
          }
        }
      }
      break;

    case SIMLOCK_CATEGORY_3GPP_SIM:
      if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sim_3gpp_code_list.num_sim_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates whitelist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x00;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sim_3gpp_code_list.sim_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            uimdiag_status = uimdiag_simlock_copy_msin_code_to_resp(
              &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sim_3gpp_code_list.sim_code_data[i].imsi_msin,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }
          }
        }
      }
      if ((simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL) &&
          (code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes))
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sim_3gpp_code_list.num_sim_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates blacklist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x01;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sim_3gpp_code_list.sim_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            uimdiag_status = uimdiag_simlock_copy_msin_code_to_resp(
              &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sim_3gpp_code_list.sim_code_data[i].imsi_msin,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }
          }
        }
      }
      break;

    case SIMLOCK_CATEGORY_3GPP_SPN:
      if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.spn_3gpp_code_list.num_spn_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates whitelist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x00;

            /* MCC-MNC is at the starting of response to UIMDIAG. So, offset is 1 */
            uimdiag_status = uimdiag_simlock_copy_packed_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.spn_3gpp_code_list.spn_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              UIMDIAG_MCC_OFFSET,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_SPN_OFFSET],
                    sizeof(code_data_ptr[code_cnt].code) - UIMDIAG_SPN_OFFSET,
                    &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.spn_3gpp_code_list.spn_code_data[i].spn.spn_data,
                    SIMLOCK_SPN_LEN);
          }
        }
      }
      if ((simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL) &&
          (code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes))
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.spn_3gpp_code_list.num_spn_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates blacklist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x01;

            uimdiag_status = uimdiag_simlock_copy_packed_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.spn_3gpp_code_list.spn_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              UIMDIAG_MCC_OFFSET,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

             memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_SPN_OFFSET],
                     sizeof(code_data_ptr[code_cnt].code) - UIMDIAG_SPN_OFFSET,
                     &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.spn_3gpp_code_list.spn_code_data[i].spn.spn_data,
                     SIMLOCK_SPN_LEN);
          }
        }
      }
      break;

    case SIMLOCK_CATEGORY_3GPP_SP_EHPLMN:
      if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.num_sp_ehplmn_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates whitelist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x00;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            /* For SP +EHPLMN lock, 8 bytes GID is supported. */
            if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_len > SIMLOCK_MAX_GID_LEN ||
                simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_len == 0)
            {
              UIMDIAG_MSG_ERROR_0("Invalid GID1 length");
              return UIMDIAG_ERROR;
            }

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID1_LEN_OFFSET],
                    sizeof(simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_len),
                    &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_len,
                    sizeof(simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_len));

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID1_OFFSET],
              SIMLOCK_MAX_GID_LEN,
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_data,
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_len);

            /* EHPLMN packed MCC-MNC is at the end in the response to UIMDIAG.
               So, offset is UIMDIAG_EHPLMN_OFFSET */
            uimdiag_status = uimdiag_simlock_copy_packed_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].ehplmn_nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              UIMDIAG_EHPLMN_OFFSET,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }
          }
        }
      }
      if ((simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL) &&
          (code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes))
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.num_sp_ehplmn_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates blacklist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x01;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            if (simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_len > SIMLOCK_MAX_GID_LEN ||
                simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_len == 0)
            {
              UIMDIAG_MSG_ERROR_0("Invalid GID1 length");
              return UIMDIAG_ERROR;
            }

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID1_LEN_OFFSET],
                    sizeof(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_len),
                    &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_len,
                    sizeof(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_len));

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_GID1_OFFSET],
              SIMLOCK_MAX_GID_LEN,
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_data,
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].gid1.gid_len);

            uimdiag_status = uimdiag_simlock_copy_packed_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[i].ehplmn_nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              UIMDIAG_EHPLMN_OFFSET,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }
          }
        }
      }
      break;

      case SIMLOCK_CATEGORY_3GPP_ICCID:
        if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
        {
          for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
              list_cnt++)
          {
            for (i = 0; i < simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.num_iccid_codes &&
                        i < SIMLOCK_NUM_CODES_MAX &&
                        code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
            {
              /* indicates whitelist */
              code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x00;

              uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
                &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].nw_code,
                UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
                code_data_ptr[code_cnt].code);

              if (uimdiag_status != UIMDIAG_SUCCESS)
              {
                return uimdiag_status;
              }

              if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len > SIMLOCK_ICCID_DIGITS_MAX ||
                  simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len == 0)
              {
                UIMDIAG_MSG_ERROR_0("Invalid ICCID length");
                return UIMDIAG_ERROR;
              }

              memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_ICCID_LEN_OFFSET],
                      sizeof(simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len),
                      &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len,
                      sizeof(simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len));

              if(UIMDIAG_ERROR == uimdiag_simlock_convert_ascii_to_bcd(
                                    simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len,
                                    simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_data,
                                    &code_data_ptr[code_cnt].code[UIMDIAG_ICCID_OFFSET]))
              {
                return UIMDIAG_ERROR;
              }
            }
          }
        }
        if ((simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL) &&
            (code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes))
        {
          for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
              list_cnt++)
          {
            for (i = 0; i < simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.num_iccid_codes &&
                        i < SIMLOCK_NUM_CODES_MAX &&
                        code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
            {
              /* indicates blacklist */
              code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x01;

              uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
                &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].nw_code,
                UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
                code_data_ptr[code_cnt].code);

              if (uimdiag_status != UIMDIAG_SUCCESS)
              {
                return uimdiag_status;
              }

              if (simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len > SIMLOCK_ICCID_DIGITS_MAX ||
                  simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len == 0)
              {
                UIMDIAG_MSG_ERROR_0("Invalid ICCID length");
                return UIMDIAG_ERROR;
              }

              memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_ICCID_LEN_OFFSET],
                      sizeof(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len),
                      &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len,
                      sizeof(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len));

              if(UIMDIAG_ERROR == uimdiag_simlock_convert_ascii_to_bcd(
                                    simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_len,
                                    simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[i].iccid.iccid_data,
                                    &code_data_ptr[code_cnt].code[UIMDIAG_ICCID_OFFSET]))
              {
                return UIMDIAG_ERROR;
              }
            }
          }
        }
        break;

    case SIMLOCK_CATEGORY_3GPP_NS_SP:
      if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.num_ns_sp_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates whitelist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x00;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].ns_code.nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            code_data_ptr[code_cnt].code[UIMDIAG_D6_OFFSET] =
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].ns_code.imsi_digit6;

            code_data_ptr[code_cnt].code[UIMDIAG_D7_OFFSET] =
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].ns_code.imsi_digit7;

            /* For NS_SP lock, 8 bytes GID is supported */
            if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_len > SIMLOCK_MAX_GID_LEN ||
                simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_len == 0)
            {
              UIMDIAG_MSG_ERROR_0("Invalid GID1 length");
              return UIMDIAG_ERROR;
            }

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_NS_SP_GID1_LEN_OFFSET],
                    sizeof(simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_len),
                    &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_len,
                    sizeof(simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_len));

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_NS_SP_GID1_OFFSET],
              SIMLOCK_MAX_GID_LEN,
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_data,
              simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_len);
          }
        }
      }
      if ((simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL) &&
          (code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes))
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.num_ns_sp_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates blacklist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x01;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].ns_code.nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            code_data_ptr[code_cnt].code[UIMDIAG_D6_OFFSET] =
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].ns_code.imsi_digit6;

            code_data_ptr[code_cnt].code[UIMDIAG_D7_OFFSET] =
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].ns_code.imsi_digit7;

            if (simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_len > SIMLOCK_MAX_GID_LEN ||
                simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_len == 0)
            {
              UIMDIAG_MSG_ERROR_0("Invalid GID1 length");
              return UIMDIAG_ERROR;
            }

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_NS_SP_GID1_LEN_OFFSET],
                    sizeof(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_len),
                    &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_len,
                    sizeof(simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_len));

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_NS_SP_GID1_OFFSET],
              SIMLOCK_MAX_GID_LEN,
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_data,
              simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ns_sp_3gpp_code_list.ns_sp_code_data[i].gid1.gid_len);
          }
        }
      }
      break;

    case SIMLOCK_CATEGORY_3GPP2_NW_TYPE1:
      if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.nw_type1_3gpp2_code_list.num_nw_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates whitelist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x00;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.nw_type1_3gpp2_code_list.nw_code_data[i],
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }
          }
        }
      }
      if ((simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL) &&
          (code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes))
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.nw_type1_3gpp2_code_list.num_nw_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates blacklist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x01;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.nw_type1_3gpp2_code_list.nw_code_data[i],
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }
          }
        }
      }
      break;

    case SIMLOCK_CATEGORY_3GPP2_NW_TYPE2:
      if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.nw_type2_3gpp2_code_list.num_nw_type2_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates whitelist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x00;

            memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_IRM_OFFSET],
                    SIMLOCK_IRM_LEN,
                    simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.nw_type2_3gpp2_code_list.nw_type2_code_data[i].irm,
                    SIMLOCK_IRM_LEN);
          }
        }
      }
      if ((simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL) &&
          (code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes))
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.nw_type2_3gpp2_code_list.num_nw_type2_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates blacklist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x01;

             memscpy(&code_data_ptr[code_cnt].code[UIMDIAG_IRM_OFFSET],
                     SIMLOCK_IRM_LEN,
                     simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.nw_type2_3gpp2_code_list.nw_type2_code_data[i].irm,
                     SIMLOCK_IRM_LEN);
          }
        }
      }
      break;

    case SIMLOCK_CATEGORY_3GPP2_RUIM:
      if (simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr != NULL)
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_whitelist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.num_sim_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates whitelist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x00;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.sim_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            uimdiag_status = uimdiag_simlock_copy_msin_code_to_resp(
              &simlock_get_category_data_ptr->category_data_whitelist.whitelist_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.sim_code_data[i].imsi_msin,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }
          }
        }
      }
      if ((simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr != NULL) &&
          (code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes))
      {
        for(list_cnt = 0; list_cnt < simlock_get_category_data_ptr->category_data_blacklist.num_of_lists;
            list_cnt++)
        {
          for (i = 0; i < simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.num_sim_codes &&
                      i < SIMLOCK_NUM_CODES_MAX &&
                      code_cnt < uimdiag_get_category_data_ext_ptr->num_of_codes; i++, code_cnt++)
          {
            /* indicates blacklist */
            code_data_ptr[code_cnt].code[UIMDIAG_CODE_TYPE_OFFSET] = 0x01;

            uimdiag_status = uimdiag_simlock_copy_nw_code_to_resp(
              &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.sim_code_data[i].nw_code,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }

            uimdiag_status = uimdiag_simlock_copy_msin_code_to_resp(
              &simlock_get_category_data_ptr->category_data_blacklist.blacklist_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.sim_code_data[i].imsi_msin,
              UIMDIAG_SIMLOCK_DATA_BLOCK_LEN,
              code_data_ptr[code_cnt].code);

            if (uimdiag_status != UIMDIAG_SUCCESS)
            {
              return uimdiag_status;
            }
          }
        }
      }
      break;

    case SIMLOCK_CATEGORY_3GPP2_HRPD:
    case SIMLOCK_CATEGORY_3GPP2_SP:
    case SIMLOCK_CATEGORY_3GPP2_CP:
    default:
      UIMDIAG_MSG_ERROR_1("Invalid category type: 0x%x", category);
      uimdiag_status = UIMDIAG_ERROR;
      break;
  }

  return uimdiag_status;
} /* uimdiag_simlock_process_get_category_data_ext_resp */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_CB

DESCRIPTION
  Generic command callback function, handles all SIM Lock command response and
  prepares diag response packet

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void uimdiag_simlock_cb(
  simlock_result_enum_type                    status,
  const simlock_message_response_data_type  * resp_ptr,
  const void                                * user_data_ptr
)
{
  uimdiag_generic_rsp_type   *rsp_ptr                 = NULL;
  int32                       index                   = (int32)user_data_ptr;
  uint32                      len                     = 0;
  boolean                     index_valid             = FALSE;
  uimdiag_return_enum_type    uimdiag_return          = UIMDIAG_ERROR;

  index_valid = UIMDIAG_REQ_TABLE_IS_INDEX_VALID(index);

  if (FALSE == index_valid)
  {
    UIMDIAG_MSG_ERROR_0("Invalid client data, index is invalid");
    return;
  }

  if (resp_ptr == NULL)
  {
    UIMDIAG_MSG_ERROR_0("Invalid callback response data");
    uimdiag_free_index(index);
    return;
  }

  if(resp_ptr->msg_type == SIMLOCK_GET_CATEGORY_DATA_MSG)
  {
    uint8   num_of_codes = 0;

    /* calculate the memory for code data */
    /* for header, 4 bytes for status and one set of codes by default */
    len = sizeof(uimdiag_cmd_rsp_hdr_type) +
          sizeof(uimdiag_simlock_get_category_data_ext_rsp_type) + 4;

    num_of_codes = uimdiag_simlock_get_number_of_codes_from_resp(&resp_ptr->message.get_category_data_resp);
    if (num_of_codes > 1)
    {
      len += ((num_of_codes - 1) * sizeof(uimdiag_simlock_code_data_type));
    }

    if(num_of_codes > 0)
    {
      /* Allocate memory for the delayed response pointer */
      rsp_ptr =(uimdiag_generic_rsp_type*)diagpkt_subsys_alloc_v2_delay(
                (diagpkt_subsys_id_type)DIAG_SUBSYS_UIM,
                (diagpkt_subsys_cmd_code_type)uimdiag_req_table_ptr[index].subcmd,
                (diagpkt_subsys_delayed_rsp_id_type)
                uimdiag_req_table_ptr[index].rsp_id,
                len);
    }
  }
  else
  {
    /* Allocate memory for the delayed response pointer */
    rsp_ptr =(uimdiag_generic_rsp_type*)diagpkt_subsys_alloc_v2_delay(
              (diagpkt_subsys_id_type)DIAG_SUBSYS_UIM,
              (diagpkt_subsys_cmd_code_type)uimdiag_req_table_ptr[index].subcmd,
              (diagpkt_subsys_delayed_rsp_id_type)
              uimdiag_req_table_ptr[index].rsp_id,
              sizeof(uimdiag_generic_rsp_type));
  }

  if(rsp_ptr == NULL)
  {
    rsp_ptr =(uimdiag_generic_rsp_type  * )diagpkt_subsys_alloc_v2_delay(
          (diagpkt_subsys_id_type)DIAG_SUBSYS_GSDI,
          (diagpkt_subsys_cmd_code_type)uimdiag_req_table_ptr[index].subcmd,
          (diagpkt_subsys_delayed_rsp_id_type)
          uimdiag_req_table_ptr[index].rsp_id,
          sizeof(uimdiag_generic_rsp_type));

    if(rsp_ptr == NULL)
    {
      uimdiag_free_index(index);
      return;
    }

    diagpkt_subsys_set_status(rsp_ptr, (uint32) UIMDIAG_SUCCESS);
    rsp_ptr->rsp_payload.mmgsdi_status = (mmgsdi_return_enum_type)SIMLOCK_GENERIC_ERROR;
    diagpkt_delay_commit(rsp_ptr);
    uimdiag_free_index(index);
    return;
  }

  if(uimdiag_req_table_ptr[index].subcmd == UIMDIAG_SIMLOCK_GET_CATEGORY_DATA_EXT_CMD)
  {
    memset(&rsp_ptr->rsp_payload,0x00, (len - sizeof(uimdiag_cmd_rsp_hdr_type)));
  }
  else
  {
    memset(&rsp_ptr->rsp_payload,0x00, sizeof(uimdiag_cmd_rsp_pyld_type));
  }

  diagpkt_subsys_set_status(rsp_ptr, (uint32) UIMDIAG_SUCCESS);
  rsp_ptr->rsp_payload.mmgsdi_status = (mmgsdi_return_enum_type)status;

  switch(resp_ptr->msg_type)
  {
    case SIMLOCK_SET_SIMLOCK_POLICY_MSG:
    case SIMLOCK_ADD_LOCK_CODES_HCK_MSG:
    case SIMLOCK_SET_RSU_MODE_MSG:
    case SIMLOCK_SET_RSU_DEVICE_KEY_MSG:
      uimdiag_return = UIMDIAG_SUCCESS;
      break;
    case SIMLOCK_SET_LOCK_CK_MSG:
    case SIMLOCK_SET_LOCK_HCK_MSG:
      uimdiag_return = uimdiag_simlock_process_token_resp(
                         &resp_ptr->message.token_resp.token,
                         &rsp_ptr->rsp_payload.response.token_rsp);
      break;

    case SIMLOCK_ADD_LOCK_CODES_CK_MSG:
      uimdiag_return = uimdiag_simlock_process_token_resp(
                         &resp_ptr->message.add_lock_resp.token,
                         &rsp_ptr->rsp_payload.response.token_rsp);
      break;

    case SIMLOCK_UNLOCK_DEVICE_MSG:
      uimdiag_return = uimdiag_simlock_process_unlock_resp(
                         &resp_ptr->message.unlock_resp,
                         &rsp_ptr->rsp_payload.response.unlock_rsp);
      break;

    case SIMLOCK_GET_STATUS_MSG:
      uimdiag_return = uimdiag_simlock_process_get_status_resp(
                         &resp_ptr->message.get_status_resp,
                         &rsp_ptr->rsp_payload.response.get_status_rsp);
      break;

    case SIMLOCK_GET_RSU_MODE_MSG:
      uimdiag_return = uimdiag_simlock_process_get_rsu_mode_resp(
                         &resp_ptr->message.get_rsu_mode,
                         &rsp_ptr->rsp_payload.response.get_rsu_mode);
      break;

    case SIMLOCK_GET_CATEGORY_DATA_MSG:
      uimdiag_return = uimdiag_simlock_process_get_category_data_ext_resp(
                       &resp_ptr->message.get_category_data_resp,
                       &rsp_ptr->rsp_payload.response.get_category_data_ext_rsp);
      break;

    default:
      UIMDIAG_MSG_ERROR_1("Invalid message type: 0x%x", resp_ptr->msg_type);
      uimdiag_return = UIMDIAG_ERROR;
      break;
  }

  if (uimdiag_return != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_0("Failed to construct valid response");
    rsp_ptr->rsp_payload.mmgsdi_status = (mmgsdi_return_enum_type)SIMLOCK_GENERIC_ERROR;
  }

  UIMDIAG_MSG_HIGH_1("uimdiag_simlock_cb: response sent. status is 0x%x",
                     rsp_ptr->rsp_payload.mmgsdi_status);
  uimdiag_free_index(index);
  diagpkt_delay_commit(rsp_ptr);
}/* uimdiag_simlock_cb */


/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_REMOTE_CB

DESCRIPTION
  Generic command callback function, handles all SIM Lock Remote command response and
  prepares diag response packet

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void uimdiag_simlock_remote_cb(
  simlock_result_enum_type  status,
  const simlock_rsu_message_response_data_type   *resp_data,
  const void               *user_data_ptr
)
{
  uimdiag_generic_rsp_type  *rsp_ptr      = NULL;
  int32                      index        = (int32)user_data_ptr;
  boolean                    index_valid  = FALSE;

  index_valid = UIMDIAG_REQ_TABLE_IS_INDEX_VALID(index);

  if (FALSE == index_valid)
  {
    UIMDIAG_MSG_ERROR_0("Invalid client data, index is invalid");
    return;
  }

  /* Allocate memory for the delayed response pointer */
  rsp_ptr =(uimdiag_generic_rsp_type*)diagpkt_subsys_alloc_v2_delay(
            (diagpkt_subsys_id_type)DIAG_SUBSYS_UIM,
            (diagpkt_subsys_cmd_code_type)uimdiag_req_table_ptr[index].subcmd,
            (diagpkt_subsys_delayed_rsp_id_type)
            uimdiag_req_table_ptr[index].rsp_id,
            sizeof(uimdiag_generic_rsp_type));

  if(rsp_ptr == NULL)
  {
    uimdiag_free_index(index);
    return;
  }

  diagpkt_subsys_set_status(rsp_ptr, (uint32) UIMDIAG_SUCCESS);
  rsp_ptr->rsp_payload.mmgsdi_status = (mmgsdi_return_enum_type)status;

  UIMDIAG_MSG_HIGH_1("uimdiag_simlock_cb: response sent. status is 0x%x",
                     rsp_ptr->rsp_payload.mmgsdi_status);
  uimdiag_free_index(index);
  UIMDIAGUTIL_TMC_MEM_FREE(user_data_ptr);
  diagpkt_delay_commit(rsp_ptr);
}/* uimdiag_simlock_remote_cb */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_PROCESS_CMD

DESCRIPTION

RETURN VALUE
  None.

===============================================================================*/
static PACKED void * uimdiag_simlock_process_command (
  PACKED void *                     cmd_ptr,
  word                              pkt_len,
  uimdiag_subsystem_cmd_enum_type   cmd_type
)
{
  uimdiag_simlock_req_type           *req_ptr               = NULL;
  uimdiag_return_enum_type            uimdiag_status        = UIMDIAG_ERROR;
  simlock_result_enum_type            simlock_status        = SIMLOCK_GENERIC_ERROR;
  int32                               index                 = UIMDIAG_GET_FN_ERR;
  diagpkt_subsys_delayed_rsp_id_type  rsp_id                = 0;
  uimdiag_generic_rsp_type           *rsp_ptr               = NULL;

  if (cmd_ptr == NULL)
  {
    return NULL;
  }

  UIMDIAG_UTIL_TMC_MEM_MALLOC_AND_VALIDATE(req_ptr,
                                           sizeof(uimdiag_simlock_req_type));

  if(req_ptr == NULL)
  {
    return NULL;
  }

  uimdiag_status = uimdiag_simlock_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     req_ptr,
                     cmd_type);

  if(uimdiag_status != UIMDIAG_SUCCESS)
  {
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr);
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = cmd_type;

  switch (cmd_type)
  {
    case UIMDIAG_SIMLOCK_SET_POLICY_CMD:
      req_ptr->data.config_policy.config_id = simlock_set_configuration.config_id;
      memscpy(&req_ptr->data.config_policy.config_nickname, sizeof(simlock_nickname_type),
              &simlock_set_configuration.config_nickname, sizeof(simlock_nickname_type));
      simlock_status = simlock_set_policy(req_ptr->data.config_policy,
                                          uimdiag_simlock_cb,
                                          (void *)index);
      break;
    case UIMDIAG_SIMLOCK_HCK_LOCK_CMD:
      req_ptr->data.lock_hck.sfs_type = SIMLOCK_SFS_LOCAL;
      req_ptr->data.lock_hck.config_id = simlock_set_configuration.config_id;
      memscpy(&req_ptr->data.lock_hck.config_nickname, sizeof(simlock_nickname_type),
              &simlock_set_configuration.config_nickname, sizeof(simlock_nickname_type));
      simlock_status = simlock_hck_lock(&req_ptr->data.lock_hck,
                                        uimdiag_simlock_cb,
                                        (void *)index);
      break;
    case UIMDIAG_SIMLOCK_CK_LOCK_CMD:
      req_ptr->data.lock_ck.sfs_type = SIMLOCK_SFS_LOCAL;
      req_ptr->data.lock_ck.config_id = simlock_set_configuration.config_id;
      memscpy(&req_ptr->data.lock_ck.config_nickname, sizeof(simlock_nickname_type),
              &simlock_set_configuration.config_nickname, sizeof(simlock_nickname_type));
      simlock_status = simlock_ck_lock(&req_ptr->data.lock_ck,
                                       uimdiag_simlock_cb,
                                       (void *)index);
      break;
    case UIMDIAG_SIMLOCK_UNLOCK_CMD:
    case UIMDIAG_SIMLOCK_PROBE_CK_CMD:
      simlock_status = simlock_unlock(req_ptr->data.unlock,
                                      uimdiag_simlock_cb,
                                      (void *)index);
      break;
    case UIMDIAG_SIMLOCK_GET_STATUS_CMD:
      simlock_status = simlock_get_status(uimdiag_simlock_cb,
                                          (void *)index);
      break;
    case UIMDIAG_SIMLOCK_GET_CATEGORY_DATA_EXT_CMD:
      simlock_status = simlock_get_category_data(req_ptr->data.get_category_data,
                                                 uimdiag_simlock_cb,
                                                 (void *)index);
      break;
    case UIMDIAG_SIMLOCK_ADD_LOCK_CODES_CK_CMD:
      req_ptr->data.add_lock_codes_ck.config_id = simlock_set_configuration.config_id;
      memscpy(&req_ptr->data.add_lock_codes_ck.config_nickname, sizeof(simlock_nickname_type),
              &simlock_set_configuration.config_nickname, sizeof(simlock_nickname_type));
      simlock_status = simlock_add_lock_codes_ck(&req_ptr->data.add_lock_codes_ck,
                                                 uimdiag_simlock_cb,
                                                 (void *)index);
      break;
    case UIMDIAG_SIMLOCK_ADD_LOCK_CODES_HCK_CMD:
#ifdef FEATURE_SIMLOCK_RSU
      /* ADD HCK lock is allowed only for RSU mode B. We have mode check 
         while processing the command if FEATURE_SIMLOCK_RSU is enable */
      simlock_status = simlock_add_lock_codes_hck(&req_ptr->data.add_lock_codes_hck,
                                                  uimdiag_simlock_cb,
                                                  (void *)index);
#else
      /* Adding codes using the HCK might lead to security issues. The reason is that
         HCK values are not secret, and attacker could potentially add new PLMNs to
         the list, thus allowing more SIM cards that originally unexpected.
         For this reason, this code is commented out, but left here for reference,
         in case it needs to be enabled for specific cases */
      simlock_status = SIMLOCK_UNSUPPORTED;
#endif /* FEATURE_SIMLOCK_RSU */
      break;
    case UIMDIAG_SIMLOCK_REMOTE_LOCK_CHECK_PUBLIC_KEY_CMD:
      simlock_status = simlock_rsu_check_public_key(&req_ptr->data.remote_lock_public_key,
                                                    uimdiag_simlock_remote_cb,
                                                    (void *)index);
      break;
    case UIMDIAG_SIMLOCK_SET_RSU_MODE_CMD:
      simlock_status = simlock_set_rsu_mode(req_ptr->data.rsu_mode,
                                            uimdiag_simlock_cb,
                                            (void *)index);
      break;
    case UIMDIAG_SIMLOCK_GET_RSU_MODE_CMD:
      simlock_status = simlock_get_rsu_mode(uimdiag_simlock_cb,
                                            (void *)index);
      break;
    case UIMDIAG_SIMLOCK_SET_RSU_KEY_CMD:
      simlock_status = simlock_set_rsu_device_key(&req_ptr->data.rsu_device_key,
                                                  uimdiag_simlock_cb,
                                                  (void *)index);
      break;
    default:
      UIMDIAG_MSG_ERROR_1("command type not supported: 0x%x", cmd_type);
      simlock_status = SIMLOCK_UNSUPPORTED;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(req_ptr);

  uimdiag_status = uimdiag_simlock_post_parse_commands(
                     &rsp_ptr,
                     rsp_id,
                     index,
                     simlock_status,
                     cmd_type);

  if(uimdiag_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_2("command: 0x%x; immediate resp error: 0x%x", cmd_type, uimdiag_status);
    return NULL;
  }

  return rsp_ptr;
} /* uimdiag_simlock_process_command */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_SET_POLICY_CMD

DESCRIPTION
  This function sets the slot and subscription policy in the SIM Lock feature
  and stores the information in the SFS.

DEPENDENCIES
  This can only be processed when all SIM Lock categories are disabled.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_set_policy_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_set_policy_cmd");

  return uimdiag_simlock_process_command(cmd_ptr,
                                         pkt_len,
                                         UIMDIAG_SIMLOCK_SET_POLICY_CMD);
} /* uimdiag_simlock_set_policy_cmd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_CK_LOCK_CMD

DESCRIPTION
  This function allows the client to provision one SIM Lock feature
  with up to 20 sets of that feature data. The SIM Lock feature
  will then be locked to the device to an HCK which is generated by
  the carrier using a randomly generated SALT key and the original
  control key stored by the carrier.

  In addition, an autolock indicator, blacklist indicator, maximum
  unlock attempt indicator and key generation iteration count indicator
  will be stored along with the security data in the SFS.

RETURN VALUE
  None.

===============================================================================*/
PACKED void * uimdiag_simlock_ck_lock_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_ck_lock_cmd");

  return uimdiag_simlock_process_command(cmd_ptr,
                                         pkt_len,
                                         UIMDIAG_SIMLOCK_CK_LOCK_CMD);
} /* uimdiag_simlock_ck_lock_cmd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_HCK_LOCK_CMD

DESCRIPTION
  This function allows the client to provision one SIM Lock feature
  with up to 20 sets of that feature data. The SIM Lock feature
  will then be locked to the device to an HCK which is generated by
  the carrier using a randomly generated SALT key and the original
  control key stored by the carrier.

  In addition, an autolock indicator, blacklist indicator, maximum
  unlock attempt indicator and key generation iteration count indicator
  will be stored along with the security data in the SFS.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_hck_lock_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_hck_lock_cmd");

  return uimdiag_simlock_process_command(cmd_ptr,
                                         pkt_len,
                                         UIMDIAG_SIMLOCK_HCK_LOCK_CMD);
}

/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_UNLOCK_CMD

DESCRIPTION
  This function allows the user to unlock and disable one SIMLOCK feature on an
  identified slot.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_unlock_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_unlock_cmd");

  return uimdiag_simlock_process_command(cmd_ptr,
                                         pkt_len,
                                         UIMDIAG_SIMLOCK_UNLOCK_CMD);
} /* uimdiag_simlock_unlock_cmd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_GET_STATUS_CMD

DESCRIPTION
  This function retrieves all relavent SIMLOCK information and configurations
  across all slots and subscription applications including the slot and
  subscription policies.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_get_status_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_get_status_cmd");

  return uimdiag_simlock_process_command(cmd_ptr,
                                         pkt_len,
                                         UIMDIAG_SIMLOCK_GET_STATUS_CMD);
} /* uimdiag_simlock_get_status_cmd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_GET_RSU_MODE_CMD

DESCRIPTION
  This function retrieves the RSU mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_get_rsu_mode_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_get_rsu_mode_cmd");

  return uimdiag_simlock_process_command(cmd_ptr,
                                         pkt_len,
                                         UIMDIAG_SIMLOCK_GET_RSU_MODE_CMD);
} /* uimdiag_simlock_get_rsu_mode_cmd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_ADD_LOCK_CODES_CK_CMD

DESCRIPTION
  This function adds the lock codes for one SIM Lock feature.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_add_lock_codes_ck_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_add_lock_codes_ck_cmd");

  return uimdiag_simlock_process_command(cmd_ptr,
                                         pkt_len,
                                         UIMDIAG_SIMLOCK_ADD_LOCK_CODES_CK_CMD);
} /* uimdiag_simlock_add_lock_codes_ck_cmd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_ADD_LOCK_CODES_HCK_CMD

DESCRIPTION
  This function adds the lock codes for one SIM Lock feature if the HCK
  matches

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_add_lock_codes_hck_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_add_lock_codes_hck_cmd");

  return uimdiag_simlock_process_command(cmd_ptr,
                                         pkt_len,
                                         UIMDIAG_SIMLOCK_ADD_LOCK_CODES_HCK_CMD);
} /* uimdiag_simlock_add_lock_codes_hck_cmd */


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_GET_CATEGORY_DATA_EXT_CMD

DESCRIPTION
  This function retrieves the SIM Lock category data for one SIM Lock feature.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_get_category_data_ext_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_get_category_data_ext_cmd");

  return uimdiag_simlock_process_command(cmd_ptr,
                                         pkt_len,
                                         UIMDIAG_SIMLOCK_GET_CATEGORY_DATA_EXT_CMD);
} /* uimdiag_simlock_get_category_data_ext_cmd */


/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_REMOTE_LOCK_CHECK_PUBLIC_KEY_CMD

DESCRIPTION
  This function checks if the public key is matched against the public key which
  is configured in code or EFS

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_remote_lock_check_public_key_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_remote_lock_check_public_key_cmd");

  return uimdiag_simlock_process_command(cmd_ptr,
                                         pkt_len,
                                         UIMDIAG_SIMLOCK_REMOTE_LOCK_CHECK_PUBLIC_KEY_CMD);
} /* uimdiag_simlock_remote_lock_check_public_key_cmd */


/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_PROBE_CK_CMD

DESCRIPTION
  This function checks if the input CK matches with the locked CK for a category

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_probe_ck_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_probe_ck_cmd");

  return uimdiag_simlock_process_command(cmd_ptr,
                                         pkt_len,
                                         UIMDIAG_SIMLOCK_PROBE_CK_CMD);
} /* uimdiag_simlock_probe_ck_cmd */


/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_MODE_CONFIG_CMD

DESCRIPTION
  This function configure build config according to the input received from
  diag.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_mode_config_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_mode_config_cmd");

  return uimdiag_simlock_process_command(cmd_ptr,
                                         pkt_len,
                                         UIMDIAG_SIMLOCK_SET_RSU_MODE_CMD);
} /* uimdiag_simlock_mode_config_cmd */


/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_DEVICE_KEY_CONFIG_CMD

DESCRIPTION
  This function configure device key according to the input received from
  diag.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_device_key_config_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_device_key_config_cmd");

  return uimdiag_simlock_process_command(cmd_ptr,
                                         pkt_len,
                                         UIMDIAG_SIMLOCK_SET_RSU_KEY_CMD);
} /* uimdiag_simlock_device_key_config_cmd */


/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_SET_CONFIGURATION_CMD

DESCRIPTION
  This function sets config id and config nickname for subsequent UIMDIAG
  SIMLOCK commands. Config id and config nickname corresponds to operator for
  which inactive configuration and category files will be created for below
  UIMDIAG SIMLOCK commands.

  UIMDIAG_SIMLOCK_HCK_LOCK_CMD
  UIMDIAG_SIMLOCK_HCK_LOCK_EXT_CMD
  UIMDIAG_SIMLOCK_CK_LOCK_CMD
  UIMDIAG_SIMLOCK_CK_LOCK_EXT_CMD 
  UIMDIAG_SIMLOCK_ADD_LOCK_CODES_CK_CMD
  UIMDIAG_SIMLOCK_ADD_LOCK_CODES_CK_EXT_CMD
  UIMDIAG_SIMLOCK_SET_POLICY_CMD

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_set_configuration_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_simlock_req_type          *req_ptr            = NULL;
  simlock_result_enum_type           simlock_result     = SIMLOCK_SUCCESS;

  UIMDIAG_MSG_HIGH_0("uimdiag_simlock_set_configuration_cmd");

  if( cmd_ptr == NULL)
  {
    return NULL;
  }

  UIMDIAG_UTIL_TMC_MEM_MALLOC_AND_VALIDATE(req_ptr,
                                            sizeof(uimdiag_simlock_req_type));
  
  if(req_ptr == NULL)
  {
    return NULL;
  }
  
  /* Extract data from req packet
     and populate fields in the request structure */
  uimdiag_status = uimdiag_simlock_parse_cmd_data(req_ptr, cmd_ptr, pkt_len);
  
  if (uimdiag_status == UIMDIAG_SUCCESS)
  {
    memscpy(&simlock_set_configuration, sizeof(uimdiag_set_configuration_msg_req_type),
            &req_ptr->data.set_configuration, sizeof(uimdiag_set_configuration_msg_req_type));
  }
  else
  {
    simlock_result = SIMLOCK_GENERIC_ERROR;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(req_ptr);

  /*  allocate response pointer  */
  rsp_ptr = (uimdiag_generic_rsp_type *)
                diagpkt_subsys_alloc_v2(
                  (diagpkt_subsys_id_type) DIAG_SUBSYS_GSDI,
                  (diagpkt_subsys_cmd_code_type) UIMDIAG_SIMLOCK_SET_CONFIGURATION_CMD,
                  sizeof(uimdiag_generic_rsp_type));
  
  if( rsp_ptr == NULL)
  {
    return NULL;
  }
  
  diagpkt_subsys_reset_delayed_rsp_id(rsp_ptr); 

  (void)memset((void*)&rsp_ptr->rsp_payload, 0x00, sizeof(uimdiag_cmd_rsp_pyld_type));
  rsp_ptr->rsp_payload.mmgsdi_status = (mmgsdi_return_enum_type)simlock_result;

  diagpkt_subsys_set_status(rsp_ptr, (uint32) UIMDIAG_SUCCESS);
  diagpkt_commit(rsp_ptr);

  return NULL;
} /* uimdiag_simlock_set_configuration_cmd */

