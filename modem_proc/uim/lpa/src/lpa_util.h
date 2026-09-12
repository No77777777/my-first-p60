#ifndef LPA_UTIL_H
#define LPA_UTIL_H
/*===========================================================================


            L P A   U T I L I T Y   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016-2017, 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_util.h#3 $ $DateTime: 2019/12/27 02:07:35 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/11/19   vgd     Increase MAX BPP TLV count to 200
04/12/17   av      Add support to configure/retrieve default server address
12/07/16   av      LPA phase2 initial changes
05/12/16   av      Fix confirmation code parsing in activation code
04/11/16   av      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "rex.h"

#include "lpa.h"
#include "mmgsdilib_common.h"
#include "lpa_cmd.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
#define LPA_ACTIVATION_CODE_LEN_MAX                      255
#define LPA_ACTIVATION_CODE_LEN_MIN                        5
#define LPA_SERVER_ADDRESS_LEN_MAX                       255
#define LPA_SMDP_ID_LEN_MAX                              (LPA_ACTIVATION_CODE_LEN_MAX - \
                                                            LPA_ACTIVATION_CODE_LEN_MIN)
#define LPA_ACTIVATION_TOKEN_LEN_MAX                     (LPA_ACTIVATION_CODE_LEN_MAX - \
                                                            LPA_ACTIVATION_CODE_LEN_MIN)
#define LPA_UTIL_TAC_LEN_MAX                               8
#define LPA_UTIL_VERSION_TYPE_LEN_MAX                      3
#define LPA_UTIL_GSM_SUPPORTED_RELEASE                    11
#define LPA_UTIL_UTRAN_SUPPORTED_RELEASE                  11
#define LPA_UTIL_EUTRAN_SUPPORTED_RELEASE                 11
#define LPA_UTIL_CDMA2000_ONEX_SUPPORTED_RELEASE           6
#define LPA_UTIL_CDMA2000_HRPD_SUPPORTED_RELEASE           1
#define LPA_UTIL_CDMA2000_EHRPD_SUPPORTED_RELEASE          1
#define LPA_UTIL_CONTACTLESS_SUPPORTED_RELEASE             7
#define LPA_UTIL_BPP_MAX_TLVS                            200
#define LPA_UTIL_RELEASE_VERSION_LEN_PHASE1             0x01
#define LPA_UTIL_RELEASE_VERSION_LEN                    0x03

/* Critical section responsible for protecting data in the LPA task */
rex_crit_sect_type           lpa_task_state_crit_sect;
rex_crit_sect_type           lpa_data_crit_sect;

/* ===========================================================================
   MACRO:       LPA_UTIL_ENTER_TASK_STATE_CRIT_SECT
   DESCRIPTION: To enter rex critical section for LPA task state
   ===========================================================================*/
#define LPA_UTIL_ENTER_TASK_STATE_CRIT_SECT                                   \
        rex_enter_crit_sect(&lpa_task_state_crit_sect);

/* ===========================================================================
   MACRO:       LPA_UTIL_LEAVE_TASK_STATE_CRIT_SECT
   DESCRIPTION: To leave rex critical section for LPA task state
   ===========================================================================*/
#define LPA_UTIL_LEAVE_TASK_STATE_CRIT_SECT                                   \
        rex_leave_crit_sect(&lpa_task_state_crit_sect);

/* ===========================================================================
   MACRO:       LPA_UTIL_ENTER_DATA_CRIT_SECT
   DESCRIPTION: To enter rex critical section for cached data
   ===========================================================================*/
#define LPA_UTIL_ENTER_DATA_CRIT_SECT                                          \
        rex_enter_crit_sect(&lpa_data_crit_sect);

/* ===========================================================================
   MACRO:       LPA_UTIL_LEAVE_DATA_CRIT_SECT
   DESCRIPTION: To leave rex critical section for cached data
   ===========================================================================*/
#define LPA_UTIL_LEAVE_DATA_CRIT_SECT                                          \
        rex_leave_crit_sect(&lpa_data_crit_sect);

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_ADD_PROFILE_METHOD_ENUM_TYPE

   DESCRIPTION: Method using which the user intended to trigger the AddProfile
                operation
-------------------------------------------------------------------------------*/
typedef enum {
  LPA_ADD_PROFILE_METHOD_ACTIVATION_CODE,
  LPA_ADD_PROFILE_METHOD_DEFAULT_SMDP,
  LPA_ADD_PROFILE_METHOD_SMDS
} lpa_add_profile_method_enum_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_BPP_TLVS_DATA_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  lpa_data_type                           tlvs_data[LPA_UTIL_BPP_MAX_TLVS];
  uint8                                   tlvs_num;
} lpa_bpp_tlvs_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_SMDP_ADDRESS_TYPE

   DESCRIPTION:
     Structure containing smdp address

-----------------------------------------------------------------------------*/
typedef struct {
  uint8                                         data_len;
  char                                          data[LPA_SERVER_ADDRESS_LEN_MAX];
} lpa_smdp_address_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_SMDP_ID_TYPE

   DESCRIPTION:
     Structure containing smdp id

-----------------------------------------------------------------------------*/
typedef struct {
  uint8                                         data_len;
  char                                          data[LPA_SMDP_ID_LEN_MAX];
} lpa_smdp_id_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_ACTIVATION_TOKEN_TYPE

   DESCRIPTION:
     Structure containing activation token

-----------------------------------------------------------------------------*/
typedef struct {
  uint8                                         data_len;
  char                                          data[LPA_ACTIVATION_TOKEN_LEN_MAX];
} lpa_activation_token_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_ACTIVATION_CODE_TYPE

   DESCRIPTION:
     Structure containing parsed activation code

-----------------------------------------------------------------------------*/
typedef struct {
  lpa_smdp_address_type                         smdp_address;
  boolean                                       confirmation_code_required;
  lpa_activation_token_type                     activation_token;
  lpa_smdp_id_type                              smdp_id;
} lpa_activation_code_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_UTIL_TAC_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  uint8            tac_len;
  uint8            tac[LPA_UTIL_TAC_LEN_MAX];
} lpa_util_tac_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_UTIL_VERSION_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  uint8            version_len;
  uint8            version[LPA_UTIL_VERSION_TYPE_LEN_MAX];
} lpa_util_version_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_UTIL_CAPABILITY_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct
{
  lpa_util_version_type            gsm_supported_release;
  lpa_util_version_type            utran_supported_release;
  lpa_util_version_type            cdma2000onex_supported_release;
  lpa_util_version_type            cdma2000hrpd_supported_release;
  lpa_util_version_type            cdma2000ehrpd_supported_release;
  lpa_util_version_type            eutran_supported_release;
  lpa_util_version_type            contactless_supported_release;
} lpa_util_device_capability_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_UTIL_DEVICE_INFO_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct
{
  lpa_util_tac_type                tac;
  lpa_util_device_capability_type  device_capability;
}lpa_util_device_info_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION LPA_UTIL_CRIT_SECT_INIT

DESCRIPTION
  Initializes the critical sections.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_util_crit_sect_init (
  void
);

/*===========================================================================
FUNCTION LPA_UTIL_HASH_SHA256

DESCRIPTION
  This function generates the SHA256 digest for the input data.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
lpa_result_enum_type lpa_util_hash_sha256(
  uint8                         *data_ptr,
  uint32                         data_len,
  lpa_data_type                 *hashed_data_ptr
);

/*===========================================================================
   FUNCTION:      LPA_UTIL_CONVERT_MMGSDI_SLOT_TO_LPA_SLOT

   DESCRIPTION:


   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_util_convert_mmgsdi_slot_to_lpa_slot(
  mmgsdi_slot_id_enum_type      mmgsdi_slot,
  lpa_slot_id_enum_type        *lpa_slot
);

/*===========================================================================
   FUNCTION:      LPA_UTIL_CONVERT_MMGSDI_STATUS_TO_LPA_STATUS

   DESCRIPTION:
     Utility to convert MMGSDI Status to LPA status

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_util_convert_mmgsdi_status_to_lpa_status(
  mmgsdi_return_enum_type                          mmgsdi_status
);


/*===========================================================================
   FUNCTION:      LPA_UTIL_CONVERT_LPA_SLOT_TO_MMGSDI_SLOT

   DESCRIPTION:


   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_util_convert_lpa_slot_to_mmgsdi_slot(
  lpa_slot_id_enum_type          lpa_slot,
  mmgsdi_slot_id_enum_type      *mmgsdi_slot
);

/*===========================================================================
   FUNCTION:      LPA_UTIL_MAP_SW1_SW2_TO_LPA_STATUS

   DESCRIPTION:
     Utility to create lpa status from SW1/SW2 returned by the card

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_util_map_sw1_sw2_to_lpa_status(
  mmgsdi_sw_type                   sw
);

/*===========================================================================
   FUNCTION:      LPA_UTIL_IS_SLOT_VALID

   DESCRIPTION:
     Utility to find out if the slot is a valid LPA slot

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     boolean

   SIDE EFFECTS:
     None
===========================================================================*/
boolean lpa_util_is_slot_valid(
  lpa_slot_id_enum_type             slot_id
);

/*===========================================================================
   FUNCTION:      LPA_UTIL_IS_ICCID_VALID

   DESCRIPTION:
     Utility to find out if the ICCID is of valid length or not

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     boolean

   SIDE EFFECTS:
     None
===========================================================================*/
boolean lpa_util_is_iccid_valid(
  lpa_iccid_type             iccid
);

/*===========================================================================
   FUNCTION:      LPA_UTIL_FREE_LPA_RESPONSE_PTR

   DESCRIPTION:
     Utility to free the members of response pointer malloc'd for sending
     LPA response to the LPA client via client's callback

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
===========================================================================*/
void lpa_util_free_lpa_response_ptr(
  lpa_response_data_type                *lpa_response_ptr
);

/*===========================================================================
FUNCTION LPA_UTIL_PARSE_ACTIVATION_CODE

DESCRIPTION
  Function parses an activation code and stores the parsed data in a struct

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_util_parse_activation_code(
  const lpa_data_type              *raw_activation_code,
  lpa_activation_code_type         *parsed_activation_code,
  lpa_add_profile_method_enum_type *method_ptr
);

/*===========================================================================
FUNCTION LPA_UTIL_PARSE_BPP

DESCRIPTION
  Function processes the http response for GET BPP and retrieves the TLVs
  from it. The output is a bunch of buffers containing those individual TLVs.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_util_parse_bpp(
  lpa_data_type                    in_bpp,
  lpa_bpp_tlvs_data_type           *out_bpp_tlvs
);

/*===========================================================================
FUNCTION LPA_UTIL_GET_DEVICE_INFO

DESCRIPTION
  Function retrieves the device info which comprises of the TAC and
  rat capabilities.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_util_get_device_info(
  lpa_util_device_info_type                *device_info,
  lpa_slot_id_enum_type                     slot_id
);

/*===========================================================================
FUNCTION LPA_UTIL_GET_LEN_FROM_TLV

DESCRIPTION
  Function parses the input buffer that contains the length bytes of a TLV
  and returns back a single uint32 with length derived from those bytes
  (For length values greater than 127, the length is encoded in a multiple
  bytes). It also returns the number of bytes used for encoding the length
  in the input buffer.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_util_get_len_from_tlv (
  uint32                              *out_len_ptr,
  uint8                               *out_num_len_bytes_ptr,
  uint32                               in_data_len,
  uint8                               *in_data_ptr
);

#endif /* LPA_UTIL_H */

