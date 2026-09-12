/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            L P A   A S N 1   E N C O D E R S   D E C O D E R S


GENERAL DESCRIPTION

  This source file contains LPA ASN1 encoder APIs and utility functions.

                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2018, 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_asn1_encoder.c#6 $ $DateTime: 2020/10/12 10:06:49 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/07/20   vgd     Use Context specific tags in Cancelsession request
07/09/20   vgd     Corrected encoding of TAC value in TBCD format
07/08/20   vgd     Corrected integer encoding when 8th bit of MSB is 1
07/08/20   vgd     Corrected logic for DER encoding of INT typte
12/18/18   bcho    LPA version checks updated for versions greater than 2.0
03/29/18   bcho    LPA phase2.1 changes
04/12/17   av      Add support to configure/retrieve default server address
04/11/17   av      Fixes for RAT decoding etc
12/07/16   ll      LPA phase2 initial changes
09/20/16   ll      Treat prepareDownload SMDP data as TLV instead raw data
05/15/16   ll      ASN1 encoder code clean up
03/28/16   ll      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "rex.h"
#include <stringl/stringl.h>
#include "lpa.h"
#include "lpalib.h"
#include "lpa_util.h"
#include "lpa_platform.h"
#include "lpa_lpd.h"
#include "lpa_asn1_encoder.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
/* ASN1 tag field */
#define LPA_ASN1_MAX_TAG_FIELD_LEN                        0x02
#define LPA_ASN1_GEN_TAG_LEN_SIMPLE                       0x01
#define LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM          0x01
#define LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM         0x02
/* Simple type tags -- reference: X.208 */
#define LPA_ASN1_GEN_TAG_BOOLEAN                          0x01
#define LPA_ASN1_GEN_TAG_INT                              0x02
#define LPA_ASN1_GEN_TAG_BIT_STRING                       0x03
#define LPA_ASN1_GEN_TAG_OCTET                            0x04
#define LPA_ASN1_GEN_TAG_OBJECT_IDENTIFIER                0x06
#define LPA_ASN1_GEN_TAG_UTF8STRING                       0x0C
/* Structured type tags */
#define LPA_ASN1_GEN_TAG_SEQUENCE                         0x30
/* Explicitly tagged type tags -- Context specific */
/* NOTE: low tag number(1 byte) for tag number <= 30, high tag number(2 byte) for tag number > 30*/
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0_CONSTRUCTED    0xA0
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1_CONSTRUCTED    0xA1
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0                0x80
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1                0x81
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC2                0x82
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC3                0x83
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC4                0x84
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC5                0x85
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC6                0x86
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC7                0x87
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC8                0x88
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC16               0x90
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST           0xBF
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC32_SECOND        0x20
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC33_SECOND        0x21
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC41_SECOND        0x29
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC43_SECOND        0x2B
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC45_SECOND        0x2D
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC46_SECOND        0x2E
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC48_SECOND        0x30
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC49_SECOND        0x31
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC50_SECOND        0x32
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC51_SECOND        0x33
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC52_SECOND        0x34
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC53_SECOND        0x35
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC56_SECOND        0x38
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC60_SECOND        0x3C
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC62_SECOND        0x3E
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC63_SECOND        0x3F
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC65_SECOND        0x41
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC67_SECOND        0x43
/* Explicitly tagged type tags -- Application */
#define LPA_ASN1_GEN_TAG_APPLICATION15                    0x49
#define LPA_ASN1_GEN_TAG_APPLICATION26                    0x5A
#define LPA_ASN1_GEN_TAG_APPLICATION28                    0x5C

/* ASN1 len field */
#define LPA_ASN1_MAX_LEN_FIELD_LEN                        0x0A
#define LPA_ASN1_LEN_LEN_SINGLE                           0x01
#define LPA_ASN1_LEN_ZERO                                 0x00
#define LPA_ASN1_SINGLE_LEN_BYTE_MAX_LEN_VALUE            0x7F
#define LPA_ASN1_LEN_LEN_BYTE_80_MASK                     0x80
#define LPA_ASN1_BOOLEAN_LEN                              0x01
#define LPA_ASN1_REFRESH_FLAG_LEN                         0x01
#define LPA_ASN1_NOTIFICATION_EVENT_LEN                   0x01
#define LPA_ASN1_DEVICE_CAPABILITIES_MAX_LEN              0x7F
#define LPA_ASN1_MAX_INT_LEN                              0x04
#define LPA_ASN1_MAX_SMDPOID_INT_LEN                      LPA_SMDP_ID_LEN_MAX
#define LPA_ASN1_GET_EID_TAG_LIST_LEN                     0x01
#define LPA_ASN1_CANCEL_SESSION_REASON_LEN                0x01
#define LPA_ASN1_BIT_STRING_NO_BIT_SET_LEN                0x01
#define LPA_ASN1_BIT_STRING_BIT_SET_LEN                   0x02
#define LPA_ASN1_GET_PROFILE_INFO_TAG_LIST_LEN_MAX        0x0D

/* LPA ASN1 value field */
#define LPA_ASN1_CANCEL_SESSION_REASON_VALUE_MAX_VER_2_0  0x03
#define LPA_ASN1_CANCEL_SESSION_REASON_VALUE_MAX_VER_2_1  0x05
#define LPA_ASN1_PROFILE_CLASS_VALUE_MAX                  0x02
#define LPA_ASN1_RESET_OPTIONS_VALUE_MAX                  0x02
#define LPA_ASN1_DELETE_OPERATIONAL_PROFILES_BIT_MASK     0x80
#define LPA_ASN1_DELETE_TEST_PROFILES_BIT_MASK            0x40
#define LPA_ASN1_DELETE_DEFAULT_SMDP_ADDRESS_BIT_MASK     0x20
#define LPA_ASN1_BOOLEAN_TRUE                             0xFF
#define LPA_ASN1_BOOLEAN_FALSE                            0x00

/* Generic */
#define LPA_ASN1_NUMBER_OF_BIT_IN_BYTE                    0x08
#define LPA_ASN1_HIGHEST_BYTE_IN_32BIT_MASK               0xFF000000
#define LPA_ASN1_LOWEST_BYTE_IN_32BIT_MASK                0x000000FF

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_ASN1_CONST_DATA_TYPE

   DESCRIPTION:
     This structure is used for all the LPA ASN1 tags defined in SGP22
-----------------------------------------------------------------------------*/
typedef struct
{
  uint16         tag_len;
  uint8          tag[LPA_ASN1_MAX_TAG_FIELD_LEN];
} lpa_asn1_tag_field_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_ASN1_LEN_FIELD_TYPE

   DESCRIPTION:
     This structure is used for all the LPA ASN1 tags defined in SGP22
-----------------------------------------------------------------------------*/
typedef struct
{
  uint8          len_len;
  uint8          len[LPA_ASN1_MAX_LEN_FIELD_LEN];
} lpa_asn1_len_field_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_ASN1_OID_TYPE

   DESCRIPTION:
     This structure is used for the parsed OID(Object Identifier) from OID string.
     The OID data type consists of a sequence of one or more non-negative integers.
     The formal definition of OIDs comes from ITU-T recommendation X.208 (ASN.1)
     [Ed. Note: ISO/IEC 8824:1994]
-----------------------------------------------------------------------------*/
typedef struct
{
  uint16         oid_len;
  uint32         oid[LPA_ASN1_MAX_SMDPOID_INT_LEN];
}lpa_asn1_oid_type;


/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_ASN1_INT_TYPE

   DESCRIPTION:
     This structure is used for integer ASN1 encoding
-----------------------------------------------------------------------------*/
typedef struct
{
  uint16         int_len;
  uint8          int_array[LPA_ASN1_MAX_INT_LEN];
}lpa_asn1_int_type;


/*=============================================================================

                              FUNCTIONS

=============================================================================*/
/*===========================================================================
FUNCTION LPA_ASN1_GET_LEN_FIELD

DESCRIPTION
  Utility function to populate a TLV to an provided buffer TLV_array
  For multiple length field, it will be in format 0x8X, XX, XX...
  where the first X in the 0x8X indicate the number of byte of the rest of
  length fields.

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_util_get_len_field (
  uint32                     len,
  lpa_asn1_len_field_type*   len_ptr
)
{
  uint32 i        = 0;
  uint32 j        = 0;
  uint32 temp_len = len;

  if (NULL == len_ptr)
  {
    LPA_MSG_ERR_0("len_field_ptr is NULL");
    return LPA_GENERIC_ERROR;
  }

  if(len <= LPA_ASN1_SINGLE_LEN_BYTE_MAX_LEN_VALUE)
  {
    /* 1 byte length field */
    len_ptr->len_len    = 1;
    len_ptr->len[0]     = (uint8)(len & 0xFF);
  }
  else
  {
    /* multi byte length fields */
    for(i = sizeof(uint32); i > 0; i--)
    {
      /* Find the highest byte for number of length byte */
      if((len & LPA_ASN1_HIGHEST_BYTE_IN_32BIT_MASK) > 0)
      {
        len_ptr->len_len = i + 1;
        /* 0x8i where i indicate the length of length field */
        len_ptr->len[0]  = (uint8)(LPA_ASN1_LEN_LEN_BYTE_80_MASK | i);
        /* populate the each length field value */
        for(j = i; j > 0 ; j--)
        {
          len_ptr->len[j] = temp_len & LPA_ASN1_LOWEST_BYTE_IN_32BIT_MASK;
          temp_len = temp_len >> LPA_ASN1_NUMBER_OF_BIT_IN_BYTE;
        }
        break;
      }
      len = len << 8;
    }
  }

  return LPA_SUCCESS;
}/*lpa_asn1_util_get_len_field*/


/*===========================================================================
FUNCTION LPA_ASN1_UTIL_GET_INT_ARRAY

DESCRIPTION
  Utility function will use least bytes to represent int by removing all 0 byte
  per ASN1 int incoding rule.

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_util_get_int_array (
  uint32                     int_value,
  lpa_asn1_int_type*         int_asn1_ptr
)
{
  uint8   i        = 0;
  uint8   j        = 0;
  uint8 int_byte = 0;

  if (NULL == int_asn1_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  if (0 == int_value)
  {
    /* If the int value is zero return below values */
    int_asn1_ptr->int_len      = 1;
    int_asn1_ptr->int_array[0] = 0;
    return LPA_SUCCESS;
  }

  /* get the number of int byte */
  for (i = 0; i < LPA_ASN1_MAX_INT_LEN; i++)
  {
    int_byte = (uint8)(((int_value << 8*i) & 0xFF000000) >> 8*(LPA_ASN1_MAX_INT_LEN -1));
    if(0 == j && 0 < int_byte)
    {
      /* Found the first none zero byte */
      int_asn1_ptr->int_len        = LPA_ASN1_MAX_INT_LEN - i;

      /* If bit 8 in MSB is 1, then it means the number is negative in 2's complement,
         to make it positive, 0x00 need to be added in 0th index. */
      if(int_byte & 0x80)
      {
         int_asn1_ptr->int_len++;
         int_asn1_ptr->int_array[j++] = 0x00;
      }

      int_asn1_ptr->int_array[j++] = int_byte;
    }
    /* Copying the rest of int byte */
    else if (0 < j)
    {
      int_asn1_ptr->int_array[j++] = int_byte;
    }
  }

  return LPA_SUCCESS;
}/*lpa_asn1_util_get_int_array*/


/*===========================================================================
FUNCTION LPA_ASN1_POPULATE_TLV

DESCRIPTION
  Utility function to populate a TLV to an provided buffer TLV_array

DEPENDENCIES
  Caller is responsible to not call this function if the optional TLV
  is not present.
  Caller should ensure tlv_array_ptr is not NULL before calling. No return
  code added due to trying to aviod too many return value checks for the caller.

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static void lpa_asn1_util_populate_tlv (
  lpa_data_type*             tlv_array_ptr,
  uint32                     max_tlv_len,
  lpa_asn1_tag_field_type*   tag_ptr,
  lpa_asn1_len_field_type*   len_ptr,
  lpa_data_type*             data_ptr
)
{
  if(NULL == tlv_array_ptr)
  {
    LPA_MSG_ERR_0("lpa_asn1_util_populate_tlv failed due to tlv_array_ptr is NULL");
    return;
  }

  if(NULL != tag_ptr)
  {
    memscpy((uint8*)&tlv_array_ptr->data_ptr[tlv_array_ptr->data_len],
             max_tlv_len,
             tag_ptr->tag,
             tag_ptr->tag_len);

   tlv_array_ptr->data_len += tag_ptr->tag_len;
  }

  if(NULL != len_ptr)
  {
    memscpy((uint8*)&tlv_array_ptr->data_ptr[tlv_array_ptr->data_len],
             max_tlv_len,
             (const uint8*)len_ptr->len,
             len_ptr->len_len);

    tlv_array_ptr->data_len += len_ptr->len_len;
  }

  if(NULL != data_ptr)
  {
    memscpy((uint8*)&tlv_array_ptr->data_ptr[tlv_array_ptr->data_len],
             max_tlv_len,
             (const uint8*)data_ptr->data_ptr,
             data_ptr->data_len);

       tlv_array_ptr->data_len += data_ptr->data_len;
    }
}/*lpa_asn1_util_populate_tlv*/


/*===========================================================================
FUNCTION LPA_ASN1_INIT_TAGS_SET_DEFAULT_DP_ADDRESS_REQUEST

DESCRIPTION
  Initialize the tags that will be used when encode SetDefaultDPAddress
  This function can be used to change the tag value easily e.g. per spec change

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void lpa_asn1_init_tags_set_default_dp_address_request (
  lpa_asn1_tag_field_type* default_dpaddress_tag_ptr,
  lpa_asn1_tag_field_type* set_default_dp_request_tag_ptr
)
{
  if ( NULL == default_dpaddress_tag_ptr ||
       NULL == set_default_dp_request_tag_ptr)
  {
    LPA_MSG_ERR_0("Either iccid_tag_ptr, profile_nickname_tag_ptr or set_nick_name_request_tag_ptr is NULL");
    return;
  }

  default_dpaddress_tag_ptr->tag_len             = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  default_dpaddress_tag_ptr->tag[0]              = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;

  set_default_dp_request_tag_ptr->tag_len        = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  set_default_dp_request_tag_ptr->tag[0]         = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  set_default_dp_request_tag_ptr->tag[1]         = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC63_SECOND;
}/*lpa_asn1_init_tags_set_default_dp_address_request*/


/*===========================================================================
FUNCTION LPA_ASN1_INIT_TAGS_SET_NICKNAME_REQUEST

DESCRIPTION
  Initialize the tags that will be used when encode SetNicknameRequest
  This function can be used to change the tag value easily e.g. per spec change

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void lpa_asn1_init_tags_set_nickname_request (
  lpa_asn1_tag_field_type* iccid_tag_ptr,
  lpa_asn1_tag_field_type* profile_nickname_tag_ptr,
  lpa_asn1_tag_field_type* set_nick_name_request_tag_ptr
)
{
  if ( NULL == iccid_tag_ptr ||
       NULL == profile_nickname_tag_ptr ||
       NULL == set_nick_name_request_tag_ptr )
  {
    LPA_MSG_ERR_0("Either iccid_tag_ptr, profile_nickname_tag_ptr or set_nick_name_request_tag_ptr is NULL");
    return;
  }

  iccid_tag_ptr->tag_len                        = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  iccid_tag_ptr->tag[0]                         = LPA_ASN1_GEN_TAG_APPLICATION26;

  profile_nickname_tag_ptr->tag_len             = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  profile_nickname_tag_ptr->tag[0]              = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC16;

  set_nick_name_request_tag_ptr->tag_len        = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  set_nick_name_request_tag_ptr->tag[0]         = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  set_nick_name_request_tag_ptr->tag[1]         = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC41_SECOND;
}/*lpa_asn1_init_tags_set_nickname_request*/


/*===========================================================================
FUNCTION LPA_ASN1_INIT_TAGS_DEVICE_CAPABILITIES

DESCRIPTION
  Initialize the tags that will be used when encode DeviceCapabilities
  This function can be used to change the tag value easily e.g. per spec change

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void lpa_asn1_init_tags_device_capabilities (
  lpa_asn1_tag_field_type* octet1_context_specific_tag_ptr,
  lpa_asn1_tag_field_type* device_capabilities_tag_ptr
)
{
  if ( NULL == octet1_context_specific_tag_ptr ||
       NULL == device_capabilities_tag_ptr)
  {
    LPA_MSG_ERR_0("Either octet1_context_specific_tag_ptr or device_capabilities_tag_ptr is NULL");
    return;
  }
  /* for the total technologies are less than 31, so use low tag number form */
  octet1_context_specific_tag_ptr->tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  device_capabilities_tag_ptr->tag_len     = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  device_capabilities_tag_ptr->tag[0]      = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1_CONSTRUCTED;
}/*lpa_asn1_init_tags_device_capabilities*/


/*===========================================================================
FUNCTION LPA_ASN1_INIT_TAGS_DEVICE_INFO

DESCRIPTION
  Initialize the tags that will be used when encode DeviceInfo
  This function can be used to change the tag value easily e.g. per spec change

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void lpa_asn1_init_tags_device_info (
  lpa_asn1_tag_field_type* tac_tag_ptr,
  lpa_asn1_tag_field_type* device_info_tag_ptr
)
{
  if ( NULL == tac_tag_ptr ||
       NULL == device_info_tag_ptr)
  {
    LPA_MSG_ERR_0("Either tac_tag_ptr, device_info_tag_ptr or tac_tag_ptr is NULL");
    return;
  }
  /* Using 0x80 instead of octet_tag 0x04 for tac_tag;
     if change back to 0x04 remove the tac_tag and use common_octet_tag */
  tac_tag_ptr->tag_len              = LPA_ASN1_GEN_TAG_LEN_SIMPLE;
  tac_tag_ptr->tag[0]               = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;

  device_info_tag_ptr->tag_len      = LPA_ASN1_GEN_TAG_LEN_SIMPLE;
  device_info_tag_ptr->tag[0]       = LPA_ASN1_GEN_TAG_SEQUENCE;
}/*lpa_asn1_init_tags_device_info*/


/*===========================================================================
FUNCTION LPA_ASN1_INIT_TAGS_PREPARE_DOWNLOAD_REQUEST

DESCRIPTION
  Initialize the tags that will be used when encode PrepareDownloadRequest
  This function can be used to change the tag value easily e.g. per spec change

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void lpa_asn1_init_tags_prepare_download_request (
  lpa_asn1_tag_field_type* activation_code_token_tag_ptr,
  lpa_asn1_tag_field_type* smdpOID_tag_ptr,
  lpa_asn1_tag_field_type* hashed_confirmation_code_tag_ptr,
  lpa_asn1_tag_field_type* prepare_download_request_tag_ptr
)
{
  /*For Phase 2 we still populate the activation_code_token_tag_ptr and smdpOID_tag_ptr tags, but will not use them*/
  if ( NULL == activation_code_token_tag_ptr ||
       NULL == smdpOID_tag_ptr ||
       NULL == hashed_confirmation_code_tag_ptr ||
       NULL == prepare_download_request_tag_ptr )
  {
    LPA_MSG_ERR_0("Some tags of prepare download is NULL");
    return;
  }

  activation_code_token_tag_ptr->tag_len    = LPA_ASN1_GEN_TAG_LEN_SIMPLE;
  activation_code_token_tag_ptr->tag[0]     = LPA_ASN1_GEN_TAG_UTF8STRING;

  smdpOID_tag_ptr->tag_len                  = LPA_ASN1_GEN_TAG_LEN_SIMPLE;
  smdpOID_tag_ptr->tag[0]                   = LPA_ASN1_GEN_TAG_OBJECT_IDENTIFIER;

  hashed_confirmation_code_tag_ptr->tag_len = LPA_ASN1_GEN_TAG_LEN_SIMPLE;
  hashed_confirmation_code_tag_ptr->tag[0]  = LPA_ASN1_GEN_TAG_OCTET;

  prepare_download_request_tag_ptr->tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  prepare_download_request_tag_ptr->tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  prepare_download_request_tag_ptr->tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC33_SECOND;
}/*lpa_asn1_init_tags_prepare_download_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_DEVICE_CAPABILITIES

DESCRIPTION
  Encode DeviceCapabilities

DEPENDENCIES
  caller is responsible to free the the data pointer inside of parsed_smdpOID

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_encode_device_capabilities (
  lpa_util_device_capability_type             device_capability,
  lpa_data_type*                              device_capabilities_encoded_data_buff_ptr,
  lpa_euicc_phase_enum_type                   phase
)
{
  uint32                   device_capabilities_encoded_data_buff_total_len   = 0;
  uint32                   device_capabilities_encoded_data_buff_payload_len = 0;
  uint32                   number_of_capabilities                            = 0;

  /* tag fields */
  /* this is a temporary tag that is going to have fix length 1 and tag value from 0x80 ~ 0x86  */
  lpa_asn1_tag_field_type  octet1_context_specific_tag;
  lpa_asn1_tag_field_type  device_capabilities_tag;
  /* length field types -- the use of the common one is due to the length is fix for multiple TLV length */
  lpa_asn1_len_field_type  common_octet1_len;
  lpa_asn1_len_field_type  device_capabilities_len;
  /* value field types -- the use of the common one is due to the length is fix for multiple TLV value */
  lpa_data_type            common_capability;

  if (NULL == device_capabilities_encoded_data_buff_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  /* initialize tag fields */
  lpa_asn1_init_tags_device_capabilities(&octet1_context_specific_tag,
                                         &device_capabilities_tag);
  /* initialize len fields */
  memset(&common_octet1_len, 0, sizeof(lpa_asn1_len_field_type));
  memset(&device_capabilities_len, 0, sizeof(lpa_asn1_len_field_type));
  common_octet1_len.len_len    = 1;
  if(LPA_PHASE1_EUICC == phase ||
     LPA_PHASE1_1_EUICC == phase)
  {
    common_octet1_len.len[0]     = LPA_UTIL_RELEASE_VERSION_LEN_PHASE1;
  }
  else
  {
    common_octet1_len.len[0]     = LPA_UTIL_RELEASE_VERSION_LEN;
  }

  if ((device_capability.gsm_supported_release.version_len > 0 &&
       device_capability.gsm_supported_release.version_len != common_octet1_len.len[0]) ||
      (device_capability.utran_supported_release.version_len >0 &&
       device_capability.utran_supported_release.version_len != common_octet1_len.len[0]) ||
      (device_capability.cdma2000onex_supported_release.version_len > 0 &&
       device_capability.cdma2000onex_supported_release.version_len != common_octet1_len.len[0]) ||
      (device_capability.cdma2000hrpd_supported_release.version_len > 0 &&
       device_capability.cdma2000hrpd_supported_release.version_len != common_octet1_len.len[0]) ||
      (device_capability.cdma2000ehrpd_supported_release.version_len > 0 &&
       device_capability.cdma2000ehrpd_supported_release.version_len != common_octet1_len.len[0]) ||
      (device_capability.eutran_supported_release.version_len > 0 &&
       device_capability.eutran_supported_release.version_len != common_octet1_len.len[0]) ||
      (device_capability.contactless_supported_release.version_len > 0 &&
       device_capability.contactless_supported_release.version_len != common_octet1_len.len[0]))
  {
    return LPA_GENERIC_ERROR;
  }
  /* initialize value fields */
  /* the common_capability structure will hold the value of release support for each technology*/
  memset(&common_capability, 0, sizeof(lpa_data_type));
  common_capability.data_len   = common_octet1_len.len[0];

  /* get total length and malloc device_info_encoded_data_buff_ptr */
  if(device_capability.gsm_supported_release.version_len > 0)
  {
    number_of_capabilities++;
  }
  if(device_capability.utran_supported_release.version_len > 0)
  {
    number_of_capabilities++;
  }
  if(device_capability.cdma2000onex_supported_release.version_len > 0)
  {
    number_of_capabilities++;
  }
  if(device_capability.cdma2000hrpd_supported_release.version_len > 0)
  {
    number_of_capabilities++;
  }
  if(device_capability.cdma2000ehrpd_supported_release.version_len > 0)
  {
    number_of_capabilities++;
  }
  if(device_capability.eutran_supported_release.version_len > 0)
  {
    number_of_capabilities++;
  }
  if(device_capability.contactless_supported_release.version_len > 0)
  {
    number_of_capabilities++;
  }

  device_capabilities_encoded_data_buff_payload_len = number_of_capabilities*
                                                      (octet1_context_specific_tag.tag_len +
                                                      common_octet1_len.len_len +
                                                      common_octet1_len.len[0]);

  /* populate device_capabilities_len with the total paload data */
  device_capabilities_len.len_len = 1;
  /* The max total payload len should be less than 128, so one byte for len is enough */
  if(device_capabilities_encoded_data_buff_payload_len > LPA_ASN1_DEVICE_CAPABILITIES_MAX_LEN)
  {
    return LPA_GENERIC_ERROR;
  }

  device_capabilities_len.len[0] = (uint8)device_capabilities_encoded_data_buff_payload_len;

  /* get the total length with sequence header */
  device_capabilities_encoded_data_buff_total_len = device_capabilities_tag.tag_len +
                                                    device_capabilities_len.len_len +
                                                    device_capabilities_encoded_data_buff_payload_len;

  device_capabilities_encoded_data_buff_ptr->data_ptr = (uint8 *)LPA_MALLOC(
                                                        device_capabilities_encoded_data_buff_total_len);
  if(NULL == device_capabilities_encoded_data_buff_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  /* populate device_capabilities_encoded_data_buff_ptr */
  device_capabilities_encoded_data_buff_ptr->data_len = 0;
  /* populate header */
  lpa_asn1_util_populate_tlv(device_capabilities_encoded_data_buff_ptr,
                             device_capabilities_encoded_data_buff_total_len,
                             &device_capabilities_tag,
                             &device_capabilities_len,
                             NULL);

  /* populate capabilities fields */
  /* assume 0 means release is not information is not present */
  if(device_capability.gsm_supported_release.version_len > 0)
  {
    octet1_context_specific_tag.tag[0]        = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;
    common_capability.data_ptr                =
      device_capability.gsm_supported_release.version;

    lpa_asn1_util_populate_tlv(device_capabilities_encoded_data_buff_ptr,
                               device_capabilities_encoded_data_buff_total_len,
                               &octet1_context_specific_tag,
                               &common_octet1_len,
                               &common_capability);
  }

  if(device_capability.utran_supported_release.version_len > 0)
  {
    octet1_context_specific_tag.tag[0]        = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1;
    common_capability.data_ptr                =
      device_capability.utran_supported_release.version;
    lpa_asn1_util_populate_tlv(device_capabilities_encoded_data_buff_ptr,
                               device_capabilities_encoded_data_buff_total_len,
                               &octet1_context_specific_tag,
                               &common_octet1_len,
                               &common_capability);
  }

  if(device_capability.cdma2000onex_supported_release.version_len > 0)
  {
    octet1_context_specific_tag.tag[0]        = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC2;
    common_capability.data_ptr                =
      device_capability.cdma2000onex_supported_release.version;
    lpa_asn1_util_populate_tlv(device_capabilities_encoded_data_buff_ptr,
                               device_capabilities_encoded_data_buff_total_len,
                               &octet1_context_specific_tag,
                               &common_octet1_len,
                               &common_capability);
  }

  if(device_capability.cdma2000hrpd_supported_release.version_len > 0)
  {
    octet1_context_specific_tag.tag[0]        = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC3;
    common_capability.data_ptr                =
      device_capability.cdma2000hrpd_supported_release.version;
    lpa_asn1_util_populate_tlv(device_capabilities_encoded_data_buff_ptr,
                               device_capabilities_encoded_data_buff_total_len,
                               &octet1_context_specific_tag,
                               &common_octet1_len,
                               &common_capability);
  }

  if(device_capability.cdma2000ehrpd_supported_release.version_len > 0)
  {
    octet1_context_specific_tag.tag[0]        = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC4;
    common_capability.data_ptr                =
      device_capability.cdma2000ehrpd_supported_release.version;
    lpa_asn1_util_populate_tlv(device_capabilities_encoded_data_buff_ptr,
                               device_capabilities_encoded_data_buff_total_len,
                               &octet1_context_specific_tag,
                               &common_octet1_len,
                               &common_capability);
  }

  if(device_capability.eutran_supported_release.version_len > 0)
  {
    octet1_context_specific_tag.tag[0]        = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC5;
    common_capability.data_ptr                =
      device_capability.eutran_supported_release.version;
    lpa_asn1_util_populate_tlv(device_capabilities_encoded_data_buff_ptr,
                               device_capabilities_encoded_data_buff_total_len,
                               &octet1_context_specific_tag,
                               &common_octet1_len,
                               &common_capability);
  }

  if(device_capability.contactless_supported_release.version_len > 0)
  {
    octet1_context_specific_tag.tag[0]        = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC6;
    common_capability.data_ptr                =
      device_capability.contactless_supported_release.version;
    lpa_asn1_util_populate_tlv(device_capabilities_encoded_data_buff_ptr,
                               device_capabilities_encoded_data_buff_total_len,
                               &octet1_context_specific_tag,
                               &common_octet1_len,
                               &common_capability);
  }

  return LPA_SUCCESS;
}/*lpa_asn1_encode_device_capabilities*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_DEVICE_INFO

DESCRIPTION
  Encode device info

DEPENDENCIES
  caller is responsible to free the the data pointer inside of parsed_smdpOID

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_encode_device_info (
  lpa_util_device_info_type        device_info,
  lpa_data_type*                   device_info_encoded_data_buff_ptr,
  boolean                          is_v_only,
  lpa_euicc_phase_enum_type        phase
)
{
  /* intermediate encoding results */
  lpa_data_type            device_capability_encoded_data_buff;
  /* tag fields */
  lpa_asn1_tag_field_type  tac_tag;
  lpa_asn1_tag_field_type  device_info_tag;
  /* length fields */
  lpa_asn1_len_field_type  tac_len;
  lpa_asn1_len_field_type  device_info_len;
  /* value fields, all none lpa_data_type input parameters are held in temp lpa_data_type */
  lpa_data_type            tac;
  /* temporary total lenth, for calculating total malloc size */
  uint32                   device_info_encoded_data_buff_total_len = 0;
  /* encoding status */
  lpa_result_enum_type     status                                  = LPA_SUCCESS;

  /*lpa tag to encode in it octet4*/
  lpa_util_tac_type        tag_octet4;

  /* init intermediate buff */
  memset(&device_capability_encoded_data_buff, 0, sizeof(lpa_data_type));
  memset(&tag_octet4, 0x00, sizeof(lpa_util_tac_type));

  /* initialize tag fields */
  lpa_asn1_init_tags_device_info(&tac_tag, &device_info_tag);

  /* initialize len fields */
  memset(&tac_len,0, sizeof(lpa_asn1_len_field_type));
  memset(&device_info_len,0, sizeof(lpa_asn1_len_field_type));

  /* initialize value fields */
  if(phase == LPA_PHASE2_1_EUICC)
  {
    uint8 index = 0;

    /*Encode TAC to octet4 as telephony binary coded decimal as per 29.002
      Bits 8765 of octet n encoding digit 2n
      Bits 4321 of octet n encoding digit 2n-1 */
    tag_octet4.tac_len = device_info.tac.tac_len / 2 + device_info.tac.tac_len % 2;

    for(index = 0; index < device_info.tac.tac_len; index++)
    {
      if(index % 2 == 1)
      {
        tag_octet4.tac[index / 2] = tag_octet4.tac[index / 2] + (device_info.tac.tac[index] << 0x04);
      }
      else
      {
        tag_octet4.tac[index / 2] = device_info.tac.tac[index];
      }
    }

    tac.data_len = tag_octet4.tac_len;
    tac.data_ptr = tag_octet4.tac;
  }
  else
  {
    tac.data_len = device_info.tac.tac_len;
    tac.data_ptr = device_info.tac.tac;
  }

  /* Encode device capability */
  status = lpa_asn1_encode_device_capabilities(device_info.device_capability,
                                               &device_capability_encoded_data_buff,
                                               phase);
  if(LPA_SUCCESS != status)
  {
    /* caller of lpa_asn1_encode_device_capabilities is responsible for FREE.
       This might be a defensive FREE */
    LPA_FREE(device_capability_encoded_data_buff.data_ptr);
    return status;
  }

  /* Get total length and malloc device_info_encoded_data_buff_ptr */
  if(LPA_SUCCESS != lpa_asn1_util_get_len_field(tac.data_len, &tac_len))
  {
    LPA_FREE(device_capability_encoded_data_buff.data_ptr);
    return LPA_GENERIC_ERROR;
  }
  /* Add total payload length -- tac TLV and the device capability ASN1 data*/
  device_info_encoded_data_buff_total_len     = tac_tag.tag_len + tac_len.len_len + tac.data_len +
                                                device_capability_encoded_data_buff.data_len;
  /* Add the device info header len */
  if(FALSE == is_v_only)
  {
    /* Need the full TLV */
    if(LPA_SUCCESS != lpa_asn1_util_get_len_field(device_info_encoded_data_buff_total_len,
                                                  &device_info_len))
    {
      LPA_FREE(device_capability_encoded_data_buff.data_ptr);
      return LPA_GENERIC_ERROR;
    }
    /* Use 0xA1 instead of common sequence tag per ObjSys */
    device_info_encoded_data_buff_total_len    +=
      device_info_tag.tag_len + device_info_len.len_len;
  }


  device_info_encoded_data_buff_ptr->data_ptr =
    (uint8*)LPA_MALLOC(device_info_encoded_data_buff_total_len);
  if(NULL == device_info_encoded_data_buff_ptr->data_ptr)
  {
    LPA_FREE(device_capability_encoded_data_buff.data_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  device_info_encoded_data_buff_ptr->data_len = 0;
  /* Header LPA_ASN1_GEN_TAG_SEQUENCE */
  if(FALSE == is_v_only)
  {
    /* Need full TLV */
    lpa_asn1_util_populate_tlv(device_info_encoded_data_buff_ptr,
                               device_info_encoded_data_buff_total_len,
                               &device_info_tag,
                               &device_info_len,
                               NULL);
  }

  /* tac LPA_ASN1_GEN_TAG_OCTET */
  lpa_asn1_util_populate_tlv(device_info_encoded_data_buff_ptr,
                             device_info_encoded_data_buff_total_len,
                             &tac_tag,
                             &tac_len,
                             &tac);

  /* Concatenate device_capabilities LPA_ASN1_GEN_TAG_SEQUENCE*/
  lpa_asn1_util_populate_tlv(device_info_encoded_data_buff_ptr,
                             device_info_encoded_data_buff_total_len,
                             NULL,
                             NULL,
                             &device_capability_encoded_data_buff);

  /* Memory cleanup */
  LPA_FREE(device_capability_encoded_data_buff.data_ptr);

  return LPA_SUCCESS;
} /* lpa_asn1_encode_device_info */


/*===========================================================================
FUNCTION LPA_PARSE_OID

DESCRIPTION
  Parse the OID string from activation code e.g.XX.XX.XXXXX to int array

DEPENDENCIES
  caller is responsible to free the the data pointer inside of parsed_oid

RETURN VALUE
  NONE

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_parse_oid (
  lpa_smdp_id_type     oid_string,
  lpa_asn1_oid_type*   parsed_oid_ptr
)
{
  uint32 i = 0;
  uint32 j = 0;
  uint32 number_of_int_in_oid = 1;   /*at least 1 int in OID*/

  if(parsed_oid_ptr == NULL)
  {
    LPA_MSG_ERR_0("parsed_oid_ptr is NULL, check caller function");
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  /* get the total number of OID int */
  for(i = 0; i < oid_string.data_len; i++)
  {
    if(oid_string.data[i] == '.')
    {
      number_of_int_in_oid++;
    }
  }

  /* Note: number_of_int_in_oid > LPA_ASN1_MAX_SMDPOID_INT_LEN */
  /* Set OID array len */
  parsed_oid_ptr->oid_len = number_of_int_in_oid;

  /* Populate OID int array */
  j = 0;
  for( i = 0; i < oid_string.data_len; i++ )
  {
    if(oid_string.data[i] != '.')
    {
      if ( j >=  LPA_ASN1_MAX_SMDPOID_INT_LEN)
      {
        /* this should never happen */
        LPA_MSG_ERR_0("Too many OID digits, array out of range");
        return LPA_GENERIC_ERROR;
      }
      parsed_oid_ptr->oid[j] = parsed_oid_ptr->oid[j]*10 + (oid_string.data[i] - '0');
    }
    else
    {
      j++;
    }
  }

  return LPA_SUCCESS;
}/*lpa_asn1_parse_OID*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_OID

DESCRIPTION
  Encode the OID int array to ASN1 byte array and malloc and populate the
  encoded_OID data_ptr.
  OBJECT IDENTIFIER encoding rules:
  1) The first two nodes of the OID are encoded onto a single byte.
     The first node is multiplied by the decimal 40 and the result is added to the value of the second node.
  2) Node values less than or equal to 127 are encoded on one byte.
  3) Node values greater than or equal to 128 are encoded on multiple bytes.
     Bit 7 of the leftmost byte is set to one. Bits 0 through 6 of each byte contains the encoded value.

DEPENDENCIES
  caller is responsible to free the the data pointer inside of parsed_smdpOID

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_encode_oid (
  lpa_data_type*    encoded_oid_ptr,
  lpa_asn1_oid_type *parsed_oid
)
{
  uint32 i                           = 0;
  uint32 j                           = 0;
  uint32 k                           = 0;
  uint32 numb_of_byte_for_single_int = 0;
  uint32 total_encoded_data_len      = 1;   /* At least 1 byte */
  uint32 temp_int_leftshift          = 0;
  uint32 temp_int_rightshift         = 0;

  if(NULL == encoded_oid_ptr ||
     NULL == parsed_oid)
  {
    return LPA_GENERIC_ERROR;
  }
  /* Get length */
  if(parsed_oid->oid_len > 2)
  {
    /* Loop through and try to get the total length */
    for(i = 2; i < parsed_oid->oid_len; i++)
    {
      /* Node values less than or equal to 127 are encoded on one byte */
      if(parsed_oid->oid[i] <= 127)
      {
        total_encoded_data_len++;
      }
      else
      {
        /* Multiple byte coding */
        temp_int_leftshift = parsed_oid->oid[i];
        for(j = sizeof(uint32)*8; j > 0; j--)
        {
          /* Finding the highest byte */
          if((temp_int_leftshift & 0x80000000) > 0)
          {
            /* Found the highest non-zero bit and add the needed bytes to the total len*/
            total_encoded_data_len += j/7;
            if(0 != j%7)
            {
              total_encoded_data_len++;
            }
            break;
          }
          temp_int_leftshift = temp_int_leftshift << 1;
        }/*end of finding highest non-zero bit loop*/
      }
    }/* end of find toto_encoded_data_len loop */
  }

  encoded_oid_ptr->data_len = total_encoded_data_len;
  encoded_oid_ptr->data_ptr = (uint8*)LPA_MALLOC(encoded_oid_ptr->data_len);
  if(NULL ==encoded_oid_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  if(1 == parsed_oid->oid_len)
  {
    encoded_oid_ptr->data_ptr[0] = (uint8)parsed_oid->oid[0];
  }

  else if(parsed_oid->oid_len >1)
  {
    encoded_oid_ptr->data_ptr[0] = (uint8)(parsed_oid->oid[0]*40 + parsed_oid->oid[1]);
    j = 1; /* Encode the 2nd and up OID int */

    for(i = 2; i < parsed_oid->oid_len; i++)
    {
      if(parsed_oid->oid[i] <= 127)
      {
        encoded_oid_ptr->data_ptr[j]  = (uint8)parsed_oid->oid[i];
        j++;
      }
      else
      {
        temp_int_leftshift = parsed_oid->oid[i];
        /* looping through each bit 8 bit in a byte * 4 byte in uint32(for oid) */
        for(k = sizeof(uint32)*8; k >0 ; k--)
        {
          if((temp_int_leftshift&0x80000000) > 0)
          {
            /* Found the highest bit*/
            numb_of_byte_for_single_int = k/7;
            if((k%7) > 0)
            {
              numb_of_byte_for_single_int++;
            }
            break;
          }
          else
          {
            temp_int_leftshift = temp_int_leftshift << 1;
          }
        }
        /* populate */
        temp_int_rightshift = parsed_oid->oid[i];
        for(k = numb_of_byte_for_single_int; k > 0 ; k--)
        {
          encoded_oid_ptr->data_ptr[j + k -1] = (temp_int_rightshift & 0x0000007F)|0x80;
          temp_int_rightshift = temp_int_rightshift >> 7;
        }
        break;
      }
    }/* end of encoding all OID int loop */
  }

  return LPA_SUCCESS;
}/*lpa_asn1_encode_oid*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_CTX_PARAMS

DESCRIPTION
  This function encodes the ctx params

DEPENDENCIES
  caller is responsible to free the the data pointer inside of
  authenticate_server_request_encoded_data_buff_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_encode_ctx_params (
  lpa_asn1_ctx_params_for_common_auth_type ctx_params_for_common_auth,
  lpa_data_type*                           ctx_params_for_common_auth_request_encoded_data_buff_ptr,
  lpa_euicc_phase_enum_type                phase
)
{
  uint8                   current_index                                    = 0;
  uint8                   ctx_params_for_common_auth_data_buff_payload_len = 0;
  lpa_data_type           device_info;

  memset(&device_info,  0x00, sizeof(device_info));

  if(NULL == ctx_params_for_common_auth_request_encoded_data_buff_ptr)
  {
    return LPA_GENERIC_ERROR;
  }
  if(LPA_SUCCESS != lpa_asn1_encode_device_info(ctx_params_for_common_auth.device_info,
                                               &device_info,
                                                TRUE,
                                                phase))
  {
    LPA_FREE(device_info.data_ptr);
    return LPA_GENERIC_ERROR;
  }
  /* Get total malloc length */

  ctx_params_for_common_auth_data_buff_payload_len =
          LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM +  /* machingID T*/
          LPA_ASN1_LEN_LEN_SINGLE +                   /* machingID L*/
          ctx_params_for_common_auth.matching_id.data_len;

  ctx_params_for_common_auth_data_buff_payload_len += LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM +  /*device_info T*/
                                                      LPA_ASN1_LEN_LEN_SINGLE +                   /*device_info L*/
                                                      device_info.data_len;

  ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_len =
       LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM +               /*CtxParamsForCommonAuthentication T*/
       LPA_ASN1_LEN_LEN_SINGLE +                                /*CtxParamsForCommonAuthentication L*/
       ctx_params_for_common_auth_data_buff_payload_len;        /*CtxParamsForCommonAuthentication V*/

  ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_ptr = (uint8 *)LPA_MALLOC(
                          ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_len);

  if(NULL == ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_ptr)
  {
    LPA_FREE(device_info.data_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  /*Populate CtxParamsForCommonAuthentication*/
  ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_ptr[current_index++] =
           LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0_CONSTRUCTED;
  ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_ptr[current_index++] =
           ctx_params_for_common_auth_data_buff_payload_len;
  /* Populate machingId TLV */
  ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_ptr[current_index++] =
          LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;
  ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_ptr[current_index++] =
          (uint8)ctx_params_for_common_auth.matching_id.data_len;
  if(0 != ctx_params_for_common_auth.matching_id.data_len)
  {
    memscpy((uint8*)(ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_ptr + current_index),
            (ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_len - current_index),
            (const uint8*)ctx_params_for_common_auth.matching_id.data_ptr,
             ctx_params_for_common_auth.matching_id.data_len);
    current_index += ctx_params_for_common_auth.matching_id.data_len;
  }

  /* device_info*/
  ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_ptr[current_index++] =
          LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1_CONSTRUCTED;
  ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_ptr[current_index++] =
          device_info.data_len;

  /* Populate deviceInfo TLV */
  memscpy((uint8*)(ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_ptr + current_index),
          (ctx_params_for_common_auth_request_encoded_data_buff_ptr->data_len - current_index),
          (const uint8*)device_info.data_ptr,
           device_info.data_len);

  LPA_FREE(device_info.data_ptr);

  return LPA_SUCCESS;
}/*lpa_asn1_encode_ctx_params*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_GET_EUICC_CONFIG_ADDRESS_REQUEST

DESCRIPTION
  This function encodes the GetEuiccConfiguredAddresses

DEPENDENCIES
  caller is responsible to free the the data pointer inside of
  out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_get_euicc_config_address_request (
  lpa_data_type*     out_ptr
)
{
  if(NULL == out_ptr)
  {
    return LPA_GENERIC_ERROR;
  }
  out_ptr->data_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM +
                      LPA_ASN1_LEN_LEN_SINGLE;
  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(out_ptr->data_len);
  if(NULL == out_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  out_ptr->data_ptr[0] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  out_ptr->data_ptr[1] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC60_SECOND;
  out_ptr->data_ptr[2] = LPA_ASN1_LEN_ZERO;

  return LPA_SUCCESS;
}/*lpa_asn1_encode_get_euicc_config_address_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_SET_DEFAULT_DPADDRESS_REQUEST

DESCRIPTION
  This function encodes the SetDefaultDpAddress

DEPENDENCIES
  caller is responsible to free the the data pointer inside of
  out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_set_default_dpaddress_request (
  lpa_data_type  default_dpaddress,
  lpa_data_type* out_ptr
)
{
  uint32                  set_default_dp_address_request_data_buff_total_len   = 0;
  uint32                  set_default_dp_address_request_data_buff_payload_len = 0;
  /* tag fields */
  lpa_asn1_tag_field_type default_dpaddress_tag;
  lpa_asn1_tag_field_type set_default_dp_request_tag;
  /* lenth fields */
  lpa_asn1_len_field_type default_dpaddress_len;
  lpa_asn1_len_field_type set_default_dp_address_request_len;

  if(NULL == out_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Pre-encoding preparation */
  /* Input data validation -- check for maditory data field */
  memset(&set_default_dp_address_request_len, 0, sizeof(lpa_asn1_len_field_type));
  memset(&default_dpaddress_len, 0, sizeof(lpa_asn1_len_field_type));
  /* init tag field */
  lpa_asn1_init_tags_set_default_dp_address_request(&default_dpaddress_tag,
                                                    &set_default_dp_request_tag);

  /* Get total malloc length: add passing length/fixed length */
  if(LPA_SUCCESS != lpa_asn1_util_get_len_field((uint32)default_dpaddress.data_len, &default_dpaddress_len))
  {
    return LPA_GENERIC_ERROR;
  }
  set_default_dp_address_request_data_buff_payload_len += default_dpaddress_tag.tag_len +
                                                          default_dpaddress_len.len_len +
                                                          default_dpaddress.data_len;

  if(LPA_SUCCESS != lpa_asn1_util_get_len_field(set_default_dp_address_request_data_buff_payload_len,
                                                &set_default_dp_address_request_len))
  {
    return LPA_GENERIC_ERROR;
  }

  set_default_dp_address_request_data_buff_total_len = set_default_dp_request_tag.tag_len +
                                                       set_default_dp_address_request_len.len_len +
                                                       set_default_dp_address_request_data_buff_payload_len;

  /* malloc for out_ptr */
  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(set_default_dp_address_request_data_buff_total_len);
  if(NULL == out_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  /* populate set_default_dp_address_request TL */
  out_ptr->data_len = 0;
  lpa_asn1_util_populate_tlv(out_ptr,
                             set_default_dp_address_request_data_buff_total_len,
                             &set_default_dp_request_tag,
                             &set_default_dp_address_request_len,
                             NULL);

  /* populate default_dpaddress TLV*/
  lpa_asn1_util_populate_tlv(out_ptr,
                             set_default_dp_address_request_data_buff_total_len,
                             &default_dpaddress_tag,
                             &default_dpaddress_len,
                             &default_dpaddress);

  return LPA_SUCCESS;
}/*lpa_asn1_encode_set_default_dpaddress_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_PREPARE_DOWNLOAD_REQUEST

DESCRIPTION
  this function encodes PrepareDownloadRequest

DEPENDENCIES
  caller is responsible to free the the data pointer inside of
  out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_prepare_download_request (
  lpa_asn1_prepare_download_request_type    prepare_download_request,
  lpa_data_type*                            out_ptr,
  lpa_slot_id_enum_type                     slot_id
)
{
  lpa_euicc_phase_enum_type     phase = LPA_PHASE1_1_EUICC;
  /* intermediate encoding results */
  lpa_asn1_oid_type             *parsed_smdpoid_ptr;
  lpa_data_type                 device_info_encoded_data_buff;
  lpa_data_type                 smdpoid_info_encoded_data_buff;
  /* tag fields */
  lpa_asn1_tag_field_type       activation_code_token_tag;
  lpa_asn1_tag_field_type       smdpoid_tag;
  lpa_asn1_tag_field_type       hashed_confirmation_code_tag;
  lpa_asn1_tag_field_type       prepare_download_request_tag;
  /* lenth fields */
  lpa_asn1_len_field_type       prepare_download_request_len;
  lpa_asn1_len_field_type       smdp_signature_len;
  lpa_asn1_len_field_type       activation_code_token_len;
  lpa_asn1_len_field_type       smdpoid_len;
  lpa_asn1_len_field_type       hashed_confirmation_code_len;
  lpa_asn1_len_field_type       cert_format_to_be_used_len;
  lpa_asn1_len_field_type       curve_to_be_used_len;
  /* temporary total lenth */
  uint32                        prepare_download_encoded_data_buff_total_len = 0;
  /* encoding status */
  lpa_result_enum_type          status                                       = LPA_GENERIC_ERROR;

  if(slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  if(LPA_SUCCESS != lpa_retrieve_euicc_phase(slot_id, &phase))
  {
    return LPA_GENERIC_ERROR;
  }

  /* Pre-encoding preparation */
  /* Input data validation -- check for maditory data field */
  if(NULL == prepare_download_request.dp_signed_raw.data_ptr      ||
     NULL == prepare_download_request.smdp_signature_raw.data_ptr ||
     NULL == prepare_download_request.smdp_certificate_raw.data_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  if((LPA_PHASE1_EUICC == phase ||
      LPA_PHASE1_1_EUICC == phase) &&
     (NULL == prepare_download_request.activation_code_token.data_ptr      ||
      NULL == prepare_download_request.cert_format_to_be_used_raw.data_ptr ||
      NULL == prepare_download_request.curve_to_be_used_raw.data_ptr))
  {
    return LPA_GENERIC_ERROR;
  }

  memset(&device_info_encoded_data_buff, 0, sizeof(lpa_data_type));
  memset(&smdpoid_info_encoded_data_buff, 0, sizeof(lpa_data_type));
  memset(&prepare_download_request_len, 0, sizeof(lpa_asn1_len_field_type));
  memset(&smdp_signature_len, 0, sizeof(lpa_asn1_len_field_type));
  memset(&activation_code_token_len, 0, sizeof(lpa_asn1_len_field_type));
  memset(&smdpoid_len, 0, sizeof(lpa_asn1_len_field_type));
  memset(&hashed_confirmation_code_len, 0, sizeof(lpa_asn1_len_field_type));
  memset(&cert_format_to_be_used_len, 0, sizeof(lpa_asn1_len_field_type));
  memset(&curve_to_be_used_len, 0, sizeof(lpa_asn1_len_field_type));

  /* init tag fields */
  lpa_asn1_init_tags_prepare_download_request(&activation_code_token_tag,
                                              &smdpoid_tag,
                                              &hashed_confirmation_code_tag,
                                              &prepare_download_request_tag);

  /* Get total malloc length: add passing length/fixed length */
  prepare_download_encoded_data_buff_total_len = 0;

  /* dp_signed_raw -- Already in TLV format from SMDP*/
  prepare_download_encoded_data_buff_total_len +=
    prepare_download_request.dp_signed_raw.data_len;

  /*smdp_signature_raw -- Already in TLV format from SMDP*/
  prepare_download_encoded_data_buff_total_len +=
    prepare_download_request.smdp_signature_raw.data_len;

  /*activation_code_token TAG_UTF8STRING*/
  if(LPA_PHASE1_EUICC == phase ||
     LPA_PHASE1_1_EUICC == phase)
  {
    if(LPA_SUCCESS != lpa_asn1_util_get_len_field(prepare_download_request.activation_code_token.data_len,
                                                  &activation_code_token_len))
    {
      return LPA_GENERIC_ERROR;
    }
    prepare_download_encoded_data_buff_total_len += (activation_code_token_tag.tag_len +
                                                     activation_code_token_len.len_len +
                                                     prepare_download_request.activation_code_token.data_len);
    /*device_info LPA_ASN1_GEN_TAG_SEQUENCE*/
    status = lpa_asn1_encode_device_info(prepare_download_request.device_info,
                                         &device_info_encoded_data_buff,
                                         FALSE,
                                         phase);
    if(LPA_SUCCESS != status)
    {
      LPA_FREE(device_info_encoded_data_buff.data_ptr);
      return status;
    }
    prepare_download_encoded_data_buff_total_len += device_info_encoded_data_buff.data_len;

    /*smdpOID LPA_ASN1_GEN_TAG_OBJECT_IDENTIFIER OPTIONAL*/
    if(prepare_download_request.smdpoid_string.data_len > 0)
    {
      parsed_smdpoid_ptr = (lpa_asn1_oid_type*)LPA_MALLOC(sizeof(lpa_asn1_oid_type));
      if(LPA_SUCCESS != lpa_asn1_parse_oid(prepare_download_request.smdpoid_string,
                                           parsed_smdpoid_ptr))
      {
        LPA_FREE(device_info_encoded_data_buff.data_ptr);
        LPA_FREE(parsed_smdpoid_ptr);
        return LPA_GENERIC_ERROR;
      }
      if(LPA_SUCCESS != lpa_asn1_encode_oid(&smdpoid_info_encoded_data_buff,
                                             parsed_smdpoid_ptr))
      {
        /* currently the return of the failure is due to heap exhausted,
           hence this free is just a defensive code */
        LPA_FREE(device_info_encoded_data_buff.data_ptr);
        LPA_FREE(parsed_smdpoid_ptr);
        return LPA_GENERIC_ERROR;
      }
      LPA_FREE(parsed_smdpoid_ptr);
      if(LPA_SUCCESS != lpa_asn1_util_get_len_field(smdpoid_info_encoded_data_buff.data_len,
                                                   &smdpoid_len))
      {
        LPA_FREE(device_info_encoded_data_buff.data_ptr);
        return LPA_GENERIC_ERROR;
      }
      prepare_download_encoded_data_buff_total_len += (smdpoid_tag.tag_len +
                                                       smdpoid_len.len_len +
                                                       smdpoid_info_encoded_data_buff.data_len);
    }

    /*cert_format_to_be_used_raw -- Already in TLV format from SMDP*/
    prepare_download_encoded_data_buff_total_len +=
      prepare_download_request.cert_format_to_be_used_raw.data_len;

    /*curve_to_be_used_raw -- Already in TLV format from SMDP*/
    prepare_download_encoded_data_buff_total_len +=
      prepare_download_request.curve_to_be_used_raw.data_len;
  }

  /*hashed_confirmation_code LPA_ASN1_GEN_TAG_OCTET OPTIONAL*/
  if(prepare_download_request.hashed_confirmation_code.data_len > 0)
  {
    if(LPA_SUCCESS != lpa_asn1_util_get_len_field(
                               prepare_download_request.hashed_confirmation_code.data_len,
                               &hashed_confirmation_code_len))
    {
      LPA_FREE(device_info_encoded_data_buff.data_ptr);
      return LPA_GENERIC_ERROR;
    }
    prepare_download_encoded_data_buff_total_len +=
                                  (hashed_confirmation_code_tag.tag_len +
                                  hashed_confirmation_code_len.len_len +
                                  prepare_download_request.hashed_confirmation_code.data_len);
  }

  /*smdp_certificate_raw -- Already in TLV format from SMDP*/
  prepare_download_encoded_data_buff_total_len +=
    prepare_download_request.smdp_certificate_raw.data_len;

  /*prepare_download_request LPA_ASN1_GEN_TAG_SEQUENCE*/
  if(LPA_SUCCESS != lpa_asn1_util_get_len_field(prepare_download_encoded_data_buff_total_len,
                                                &prepare_download_request_len))
  {
    LPA_FREE(device_info_encoded_data_buff.data_ptr);
    return LPA_GENERIC_ERROR;
  }
  prepare_download_encoded_data_buff_total_len += (prepare_download_request_tag.tag_len +
                                                   prepare_download_request_len.len_len);

  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(prepare_download_encoded_data_buff_total_len);
  if(NULL == out_ptr->data_ptr)
  {
    LPA_FREE(device_info_encoded_data_buff.data_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  /*populate out_ptr*/
  out_ptr->data_len = 0;
  /*T and L for out_ptr LPA_ASN1_GEN_TAG_PREPARE_DOWNLOAD_REQUEST*/
  lpa_asn1_util_populate_tlv(out_ptr,
                             prepare_download_encoded_data_buff_total_len,
                             &prepare_download_request_tag,
                             &prepare_download_request_len,
                             NULL);

  /*dp_signed_raw LPA_ASN1_GEN_TAG_SEQUENCE*/
  lpa_asn1_util_populate_tlv(out_ptr,
                             prepare_download_encoded_data_buff_total_len,
                             NULL, NULL,
                             &prepare_download_request.dp_signed_raw);

  /*smdp_signature_raw*/
  lpa_asn1_util_populate_tlv(out_ptr,
                             prepare_download_encoded_data_buff_total_len,
                             NULL, NULL,
                             &prepare_download_request.smdp_signature_raw);

  if(LPA_PHASE1_EUICC == phase ||
     LPA_PHASE1_1_EUICC == phase)
  {
    /*activation_code_token LPA_ASN1_GEN_TAG_SMDPSIGNATURE1*/
    lpa_asn1_util_populate_tlv(out_ptr,
                               prepare_download_encoded_data_buff_total_len,
                               &activation_code_token_tag,
                               &activation_code_token_len,
                               &prepare_download_request.activation_code_token);

    /*device_info LPA_ASN1_GEN_TAG_SEQUENCE*/
    lpa_asn1_util_populate_tlv(out_ptr,
                               prepare_download_encoded_data_buff_total_len,
                               NULL, NULL,
                               &device_info_encoded_data_buff);


    if(prepare_download_request.smdpoid_string.data_len > 0)
    {
      /*smdpOID_raw LPA_ASN1_GEN_TAG_OBJECT_IDENTIFIER*/
      lpa_asn1_util_populate_tlv(out_ptr,
                                 prepare_download_encoded_data_buff_total_len,
                                 &smdpoid_tag, &smdpoid_len,
                                 &smdpoid_info_encoded_data_buff);
    }
  }

  if(prepare_download_request.hashed_confirmation_code.data_len > 0)
  {
     /*hashed_confirmation_code LPA_ASN1_GEN_TAG_OCTET*/
     lpa_asn1_util_populate_tlv(out_ptr,
                                prepare_download_encoded_data_buff_total_len,
                                &hashed_confirmation_code_tag,
                                &hashed_confirmation_code_len,
                                &prepare_download_request.hashed_confirmation_code);
  }

  if(LPA_PHASE1_EUICC == phase ||
     LPA_PHASE1_1_EUICC == phase)
  {
    /*cert_format_to_be_used_raw*/
    lpa_asn1_util_populate_tlv(out_ptr,
                               prepare_download_encoded_data_buff_total_len,
                               NULL, NULL,
                               &prepare_download_request.cert_format_to_be_used_raw);

    /*curve_to_be_used_raw*/
    lpa_asn1_util_populate_tlv(out_ptr,
                               prepare_download_encoded_data_buff_total_len,
                               NULL, NULL,
                               &prepare_download_request.curve_to_be_used_raw);
  }

  /*smdp_certificate_raw*/
  lpa_asn1_util_populate_tlv(out_ptr,
                             prepare_download_encoded_data_buff_total_len,
                             NULL, NULL,
                             &prepare_download_request.smdp_certificate_raw);

  /*memory cleanup*/
  if((LPA_PHASE1_EUICC == phase ||
      LPA_PHASE1_1_EUICC == phase) &&
     prepare_download_request.smdpoid_string.data_len > 0)
  {
    LPA_FREE(smdpoid_info_encoded_data_buff.data_ptr);
  }
  LPA_FREE(device_info_encoded_data_buff.data_ptr);

  return LPA_SUCCESS;
}/*lpa_asn1_encode_prepare_download_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_GET_EUICC_CHALLENGE_REQUEST

DESCRIPTION
  This function encodes the GetEuiccChallengeRequest

DEPENDENCIES
  caller is responsible to free the the data pointer inside of
  out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_get_euicc_challenge_request (
  lpa_data_type* out_ptr
)
{
  if(NULL == out_ptr)
  {
    return LPA_GENERIC_ERROR;
  }
  out_ptr->data_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM +
                      LPA_ASN1_LEN_LEN_SINGLE;
  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(out_ptr->data_len);
  if(NULL == out_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  out_ptr->data_ptr[0] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  out_ptr->data_ptr[1] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC46_SECOND;
  out_ptr->data_ptr[2] = LPA_ASN1_LEN_ZERO;

  return LPA_SUCCESS;
}/*lpa_asn1_encode_get_euicc_challenge_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_GET_EUICC_INFO_REQUEST

DESCRIPTION
  This function encodes the GetEUICCInfo

DEPENDENCIES
  caller is responsible to free the the data pointer inside of
  out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_get_euicc_info_request (
  lpa_data_type* out_ptr
)
{
  if(NULL == out_ptr)
  {
    return LPA_GENERIC_ERROR;
  }
  out_ptr->data_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM +
                      LPA_ASN1_LEN_LEN_SINGLE;
  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(out_ptr->data_len);
  if(NULL == out_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  out_ptr->data_ptr[0] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  out_ptr->data_ptr[1] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC32_SECOND;
  out_ptr->data_ptr[2] = LPA_ASN1_LEN_ZERO;

  return LPA_SUCCESS;
}/*lpa_asn1_encode_get_euicc_info_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_RETRIEVE_NOTIFICATION_LIST_REQUEST

DESCRIPTION
  This function encodes the RetrieveNotificationsList

DEPENDENCIES
  caller is responsible to free the the data pointer inside of
  out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_retrieve_notification_list_request (
  lpa_data_type*  out_ptr
)
{
  uint8 offset                                                   = 0;
  uint8 retrieve_notification_list_request_data_buff_payload_len = 0;

  if(NULL == out_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  out_ptr->data_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM +
                      LPA_ASN1_LEN_LEN_SINGLE +
                      retrieve_notification_list_request_data_buff_payload_len;

  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(out_ptr->data_len);

  if(NULL == out_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate retrieve_notification_list_request TL*/
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC43_SECOND;
  out_ptr->data_ptr[offset++] =
    (uint8)retrieve_notification_list_request_data_buff_payload_len;

  return LPA_SUCCESS;
}/*lpa_asn1_encode_retrieve_notification_list_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_REMOVE_NOTIFICATION_FROM_LIST_REQUEST

DESCRIPTION
  This function encodes the RemoveNotificationFromList
  Note: seq_number_raw max value is 2,147,483,647

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_remove_notification_from_list_request (
  uint32          seq_number_raw,
  lpa_data_type*  out_ptr
)
{
  uint8 offset                                                      = 0;
  uint8 remove_notification_from_list_request_data_buff_payload_len = 0;
  lpa_asn1_int_type int_asn1;

  if(NULL == out_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  if(LPA_SUCCESS != lpa_asn1_util_get_int_array(seq_number_raw, &int_asn1))
  {
    return LPA_GENERIC_ERROR;
  }
  /* Get total malloc length: add passing length/fixed length */
  remove_notification_from_list_request_data_buff_payload_len =
        LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM +
        LPA_ASN1_LEN_LEN_SINGLE +
        int_asn1.int_len;

  out_ptr->data_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM +
                      LPA_ASN1_LEN_LEN_SINGLE +
                      remove_notification_from_list_request_data_buff_payload_len;

  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(out_ptr->data_len);

  if(NULL == out_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate remove_notification_request TL*/
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC48_SECOND;
  out_ptr->data_ptr[offset++] =
    (uint8)remove_notification_from_list_request_data_buff_payload_len;
  /* Populate seq_number TLV */
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;
  out_ptr->data_ptr[offset++] = (uint8)int_asn1.int_len;
  memscpy((uint8*)(out_ptr->data_ptr + offset),
          (out_ptr->data_len - offset),
          (const uint8*)int_asn1.int_array,
          int_asn1.int_len);

  return LPA_SUCCESS;
}/*lpa_asn1_encode_remove_notification_from_list_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_AUTHENTICATE_SERVER_REQUEST

DESCRIPTION
  This function encodes the AuthenticateServer

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_authenticate_server_request (
  lpa_data_type                            server_signed1_raw,
  lpa_data_type                            server_signature1_raw,
  lpa_data_type                            euicc_ci_pk_id_to_be_used_raw,
  lpa_data_type                            server_certificate_raw,
  lpa_asn1_ctx_params_for_common_auth_type ctx_params_for_common_auth,
  lpa_data_type*                           out_ptr,
  lpa_slot_id_enum_type                    slot_id
)
{
  uint32                    offset                                          = 0;
  uint32                    authenticate_server_data_buff_payload_len       = 0;
  lpa_asn1_len_field_type   authenticate_server_data_buff_payload_len_field;
  lpa_data_type             ctx_params_for_common_auth_raw;
  lpa_euicc_phase_enum_type phase                                           = LPA_PHASE1_1_EUICC;

  if(NULL == out_ptr                                          ||
     LPA_NUM_SLOTS <= slot_id                                 ||
     LPA_SUCCESS != lpa_retrieve_euicc_phase(slot_id, &phase) ||
     server_signed1_raw.data_len            <= 0              ||
     server_signature1_raw.data_len         <= 0              ||
     euicc_ci_pk_id_to_be_used_raw.data_len <= 0              ||
     server_certificate_raw.data_len        <= 0)
  {
    return LPA_GENERIC_ERROR;
  }
  if(LPA_SUCCESS != lpa_asn1_encode_ctx_params(ctx_params_for_common_auth,
                                               &ctx_params_for_common_auth_raw,
                                               phase))
  {
    LPA_FREE(ctx_params_for_common_auth_raw.data_ptr);
    return LPA_GENERIC_ERROR;
  }

  /* Get total malloc length */
  authenticate_server_data_buff_payload_len =
    server_signed1_raw.data_len +
    server_signature1_raw.data_len +
    euicc_ci_pk_id_to_be_used_raw.data_len +
    server_certificate_raw.data_len +
    ctx_params_for_common_auth_raw.data_len;

  if(LPA_SUCCESS != lpa_asn1_util_get_len_field((uint32)authenticate_server_data_buff_payload_len,
                                                &authenticate_server_data_buff_payload_len_field))
  {
    LPA_FREE(ctx_params_for_common_auth_raw.data_ptr);
    return LPA_GENERIC_ERROR;
  }

  out_ptr->data_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM +
                      authenticate_server_data_buff_payload_len_field.len_len +
                      authenticate_server_data_buff_payload_len;

  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(out_ptr->data_len);

  if(NULL == out_ptr->data_ptr)
  {
    LPA_FREE(ctx_params_for_common_auth_raw.data_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  /*Populate CtxParamsForCommonAuthentication*/
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC56_SECOND;

  memscpy((uint8*)(out_ptr->data_ptr + offset),
          (out_ptr->data_len - offset),
          (const uint8*)authenticate_server_data_buff_payload_len_field.len,
          authenticate_server_data_buff_payload_len_field.len_len);
  offset += authenticate_server_data_buff_payload_len_field.len_len;

  memscpy((uint8*)(out_ptr->data_ptr + offset),
          (out_ptr->data_len - offset),
          (const uint8*)server_signed1_raw.data_ptr,
          server_signed1_raw.data_len);

  offset += server_signed1_raw.data_len;
  memscpy((uint8*)(out_ptr->data_ptr + offset),
          (out_ptr->data_len - offset),
          (const uint8*)server_signature1_raw.data_ptr,
          server_signature1_raw.data_len);

  offset += server_signature1_raw.data_len;
  memscpy((uint8*)(out_ptr->data_ptr + offset),
          (out_ptr->data_len - offset),
          (const uint8*)euicc_ci_pk_id_to_be_used_raw.data_ptr,
          euicc_ci_pk_id_to_be_used_raw.data_len);

  offset += euicc_ci_pk_id_to_be_used_raw.data_len;
  memscpy((uint8*)(out_ptr->data_ptr + offset),
          (out_ptr->data_len - offset),
          (const uint8*)server_certificate_raw.data_ptr,
          server_certificate_raw.data_len);

  offset += server_certificate_raw.data_len;

  memscpy((uint8*)(out_ptr->data_ptr + offset),
          (out_ptr->data_len - offset),
          (const uint8*)ctx_params_for_common_auth_raw.data_ptr,
           ctx_params_for_common_auth_raw.data_len);

  LPA_FREE(ctx_params_for_common_auth_raw.data_ptr);

  return LPA_SUCCESS;
}/*lpa_asn1_encode_authenticate_server_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_GET_EID_REQUEST

DESCRIPTION
  This function encodes the GetEuiccDataRequest

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_get_eid_request (
  lpa_data_type*     out_ptr
)
{
  uint8 get_eid_request_data_buff_payload_len = 0;

  if(NULL == out_ptr)
  {
    return LPA_GENERIC_ERROR;
  }
  get_eid_request_data_buff_payload_len =
       LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM +  /* tagList T */
       LPA_ASN1_LEN_LEN_SINGLE +                   /* tagList L */
       LPA_ASN1_GET_EID_TAG_LIST_LEN;              /* tagList V */
  out_ptr->data_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM + /* GetEIDRequest T */
                      LPA_ASN1_LEN_LEN_SINGLE +                   /* GetEIDRequest L */
                      get_eid_request_data_buff_payload_len;

  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(out_ptr->data_len);
  if(NULL == out_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  out_ptr->data_ptr[0] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  out_ptr->data_ptr[1] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC62_SECOND;
  out_ptr->data_ptr[2] = get_eid_request_data_buff_payload_len;
  out_ptr->data_ptr[3] = LPA_ASN1_GEN_TAG_APPLICATION28;
  out_ptr->data_ptr[4] = LPA_ASN1_GET_EID_TAG_LIST_LEN;
  out_ptr->data_ptr[5] = LPA_ASN1_GEN_TAG_APPLICATION26;

  return LPA_SUCCESS;
}/*lpa_asn1_encode_get_eid_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_CANCEL_SESSION_REQUEST

DESCRIPTION
  This function encodes the CancelSession

DEPENDENCIES
  caller is responsible to free the the data pointer inside of
  out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_cancel_session_request (
  lpa_data_type              transaction_id,
  uint8                      cancel_session_reason,
  lpa_data_type*             out_ptr,
  lpa_euicc_phase_enum_type  phase
)
{
  uint8 offset                                       = 0;
  uint8 cancel_session_request_data_buff_payload_len = 0;

  if(NULL == out_ptr ||
     phase < LPA_PHASE2_EUICC ||
     (phase == LPA_PHASE2_1_EUICC &&
      cancel_session_reason > LPA_ASN1_CANCEL_SESSION_REASON_VALUE_MAX_VER_2_1) ||
     (phase == LPA_PHASE2_EUICC &&
      cancel_session_reason > LPA_ASN1_CANCEL_SESSION_REASON_VALUE_MAX_VER_2_0))
  {
    return LPA_GENERIC_ERROR;
  }

  /* Get total malloc length: add passing length/fixed length */
  cancel_session_request_data_buff_payload_len =
       LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM +  /* transactionId T*/
       LPA_ASN1_LEN_LEN_SINGLE +                   /* transactionId L*/
       transaction_id.data_len +                   /* transactionId V*/
       LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM +  /* CancelSessionReason T*/
       LPA_ASN1_LEN_LEN_SINGLE +                   /* CancelSessionReason L*/
       LPA_ASN1_CANCEL_SESSION_REASON_LEN;         /* CancelSessionReason V*/
  out_ptr->data_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM + /*CancelSessionRequest T*/
                      LPA_ASN1_LEN_LEN_SINGLE +                   /*CancelSessionRequest L*/
                      cancel_session_request_data_buff_payload_len;

  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(out_ptr->data_len);

  if(NULL == out_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate cancel_session_request TL*/
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC65_SECOND;
  out_ptr->data_ptr[offset++] = cancel_session_request_data_buff_payload_len;
  /* Populate transaction_id TLV */
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;
  out_ptr->data_ptr[offset++] = (uint8)transaction_id.data_len;
  memscpy((uint8*)(out_ptr->data_ptr + offset),
          (out_ptr->data_len - offset),
          (const uint8*)transaction_id.data_ptr,
           transaction_id.data_len);
  offset += transaction_id.data_len;
  /* Populate cancel_session_reason TLV */
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1;
  out_ptr->data_ptr[offset++] = LPA_ASN1_CANCEL_SESSION_REASON_LEN;
  out_ptr->data_ptr[offset]   = (uint8)cancel_session_reason;

  return LPA_SUCCESS;
}/*lpa_asn1_encode_cancel_session_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_GET_PROFILE_INFO_REQUEST

DESCRIPTION
  This function encodes the GetProfilesInfo

DEPENDENCIES
  caller is responsible to free the the data pointer inside of
  out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_get_profiles_info_request (
  lpa_data_type*     out_ptr
)
{
  uint8 offset                                  = 0;

  if(NULL == out_ptr)
  {
    return LPA_GENERIC_ERROR;
  }
  out_ptr->data_len =
       LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM +
       LPA_ASN1_LEN_LEN_SINGLE;

  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(
                          out_ptr->data_len);

  if(NULL == out_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate get_profiles_info_request TL*/
  out_ptr->data_ptr[offset++] =
           LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  out_ptr->data_ptr[offset++] =
           LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC45_SECOND;
  out_ptr->data_ptr[offset++] =
           LPA_ASN1_LEN_ZERO;

  return LPA_SUCCESS;
}/*lpa_asn1_encode_get_profiles_info_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_MANAGE__PROFILE_REQUEST

DESCRIPTION
  This function encodes the EnableProfile or DisableProfile based on enable_flag

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_manage_profile_request (
  lpa_iccid_type                      iccid,
  boolean                             refresh_flag,
  lpa_data_type*                      out_ptr,
  lpa_asn1_profile_manage_action_type action
)
{
  uint8 offset                                       = 0;
  uint8 profile_manage_request_data_buff_payload_len = 0;
  uint8 profile_manage_request_choice_payload_len    = 0;

  if(NULL == out_ptr ||
     LPA_ICCID_LEN != iccid.data_len ||
     LPA_ASN1_ENCODE_UNKNOW_ACTION <= action)
  {
    return LPA_GENERIC_ERROR;
  }

  profile_manage_request_choice_payload_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM +  /* iccid T*/
                                              LPA_ASN1_LEN_LEN_SINGLE                  +  /* iccid L*/
                                              LPA_ICCID_LEN;                              /* iccid V*/ ;

  profile_manage_request_data_buff_payload_len = profile_manage_request_choice_payload_len;
  if(LPA_ASN1_ENCODE_DELETE_PROFILE != action)
  {
  /* Get total malloc length: add passing length/fixed length */
    profile_manage_request_data_buff_payload_len += LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM +  /* CHOICE T*/
                                                    LPA_ASN1_LEN_LEN_SINGLE;                    /* CHOICE T*/


    profile_manage_request_data_buff_payload_len  +=
         (LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM +  /* refreshFlag T*/
         LPA_ASN1_LEN_LEN_SINGLE +                    /* refreshFlag L*/
         LPA_ASN1_REFRESH_FLAG_LEN);                  /* refreshFlag V*/
  }

  out_ptr->data_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM +
                      LPA_ASN1_LEN_LEN_SINGLE +
                      profile_manage_request_data_buff_payload_len;

  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(out_ptr->data_len);

  if(NULL == out_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  /* Populate profile_manage_request TL*/
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  if(LPA_ASN1_ENCODE_ENABLE_PROFILE == action)
  {
    /* This is a enable profile request */
    out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC49_SECOND;
  }
  else if(LPA_ASN1_ENCODE_DISABLE_PROFILE == action)
  {
    /* This is a disable profile request */
    out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC50_SECOND;
  }
  else
  {
    /* This is a delete profile request */
    out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC51_SECOND;
  }

  out_ptr->data_ptr[offset++] = profile_manage_request_data_buff_payload_len;

  if(LPA_ASN1_ENCODE_DELETE_PROFILE != action)
  {
    /* Populate Choice TL */
    out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0_CONSTRUCTED;
    out_ptr->data_ptr[offset++] = profile_manage_request_choice_payload_len;
  }

  /* Populate iccid TLV */
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_APPLICATION26;
  out_ptr->data_ptr[offset++] = (uint8)iccid.data_len;
  memscpy((uint8*)(out_ptr->data_ptr + offset),
          (out_ptr->data_len - offset),
          (const uint8*)iccid.data,
          iccid.data_len);
  offset += iccid.data_len;

  if(LPA_ASN1_ENCODE_DELETE_PROFILE != action)
  {
    /* Populate refreshFlag TLV */
    out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1;
    out_ptr->data_ptr[offset++] = LPA_ASN1_BOOLEAN_LEN;
    if(FALSE == refresh_flag)
    {
      out_ptr->data_ptr[offset]   = LPA_ASN1_BOOLEAN_FALSE;
    }
    else
    {
      out_ptr->data_ptr[offset]   = LPA_ASN1_BOOLEAN_TRUE;
    }
  }

  return LPA_SUCCESS;
}/*lpa_asn1_encode_manage_profile_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_EUICC_MEMORY_RESET_REQUEST

DESCRIPTION
  This function encodes the EuiccMemoryResetRequest

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_euicc_memory_reset_request (
  lpa_card_memory_reset_option_type     reset_option,
  lpa_data_type*                        out_ptr
)
{
  uint32  offset                                           = 0;
  uint32  euicc_memory_reset_request_data_buff_payload_len = 0;
  uint32  encoded_bit_string_len                           = 0;

  if(NULL == out_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  if(TRUE == reset_option.reset_operational_profiles ||
     TRUE == reset_option.reset_test_profiles        ||
     TRUE == reset_option.reset_smdp_address)
  {
    encoded_bit_string_len = LPA_ASN1_BIT_STRING_BIT_SET_LEN;
  }
  else
  {
    encoded_bit_string_len = LPA_ASN1_BIT_STRING_NO_BIT_SET_LEN;
  }
  /* Get total malloc length: add passing length/fixed length */
  euicc_memory_reset_request_data_buff_payload_len =
       LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM +  /* resetOptions T*/
       LPA_ASN1_LEN_LEN_SINGLE +                   /* resetOptions L*/
       encoded_bit_string_len;                     /* resetOptions V*/
  out_ptr->data_len =
       LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM + /*CancelSessionRequest T*/
       LPA_ASN1_LEN_LEN_SINGLE +                   /*CancelSessionRequest L*/
       euicc_memory_reset_request_data_buff_payload_len;

  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(out_ptr->data_len);

  if(NULL == out_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate euicc_memory_reset_request TL*/
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC52_SECOND;
  out_ptr->data_ptr[offset++] = euicc_memory_reset_request_data_buff_payload_len;
  /* Populate resetOptions TLV */
  out_ptr->data_ptr[offset++] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC2;
  /* Set the len */
  out_ptr->data_ptr[offset]   = encoded_bit_string_len;
  if(LPA_ASN1_BIT_STRING_BIT_SET_LEN == encoded_bit_string_len)
  {
    /* Initialize subsequent octet, since we currently only
       have 3 bits, hence only 1 subsequent octet possible */
    out_ptr->data_ptr[offset + 2] = 0; /* subsequent octet */
  }
  else if (LPA_ASN1_BIT_STRING_NO_BIT_SET_LEN == encoded_bit_string_len)
  {
    /* X690-0207 8.6.2.3 If the bitstring is empty,
       there shall be no subsequent octets,
       and the initial octet shall be zero. */
    out_ptr->data_ptr[offset + 1] = 0; /* initial octet */
    return LPA_SUCCESS;
  }
  else
  {
    /* This should never happen */
    return LPA_GENERIC_ERROR;
  }

  if(TRUE == reset_option.reset_operational_profiles)
  {
    out_ptr->data_ptr[offset + 1]  = LPA_ASN1_NUMBER_OF_BIT_IN_BYTE - 1; /* Len */
    out_ptr->data_ptr[offset + 2] |= LPA_ASN1_DELETE_OPERATIONAL_PROFILES_BIT_MASK;
  }
  if(TRUE == reset_option.reset_test_profiles)
  {
    out_ptr->data_ptr[offset + 1]  = LPA_ASN1_NUMBER_OF_BIT_IN_BYTE - 2; /* Len */
    out_ptr->data_ptr[offset + 2] |= LPA_ASN1_DELETE_TEST_PROFILES_BIT_MASK;
  }
  if(TRUE == reset_option.reset_smdp_address)
  {
    out_ptr->data_ptr[offset + 1]  = LPA_ASN1_NUMBER_OF_BIT_IN_BYTE - 3; /* Len */
    out_ptr->data_ptr[offset + 2] |= LPA_ASN1_DELETE_DEFAULT_SMDP_ADDRESS_BIT_MASK;
  }

  return LPA_SUCCESS;
}/*lpa_asn1_encode_euicc_memory_reset_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_SET_NICKNAME_REQUEST

DESCRIPTION
  This function encodes the SetNicknameRequest

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_set_nickname_request (
  lpa_asn1_set_nickname_request_type set_nickname_request,
  lpa_data_type*                     out_ptr
)
{
  uint32                  set_nickname_request_data_buff_total_len   = 0;
  uint32                  set_nickname_request_data_buff_payload_len = 0;
  /* tag fields */
  lpa_asn1_tag_field_type iccid_tag;
  lpa_asn1_tag_field_type profile_nickname_tag;
  lpa_asn1_tag_field_type set_nick_name_request_tag;
  /* lenth fields */
  lpa_asn1_len_field_type set_nickname_request_len;
  lpa_asn1_len_field_type iccid_len;
  lpa_asn1_len_field_type profile_nickname_len;
  /* local asn1 format data type */
  lpa_data_type           asn1_iccid;
  lpa_data_type           asn1_profile_nickname;

  /* Pre-encoding preparation */
  /* Input data validation -- check for maditory data field */
  memset(&set_nickname_request_len, 0, sizeof(lpa_asn1_len_field_type));
  memset(&iccid_len, 0, sizeof(lpa_asn1_len_field_type));
  memset(&profile_nickname_len, 0, sizeof(lpa_asn1_len_field_type));
  memset(&asn1_iccid, 0, sizeof(lpa_data_type));
  memset(&asn1_profile_nickname, 0, sizeof(lpa_data_type));
  /* init tag field */
  lpa_asn1_init_tags_set_nickname_request(&iccid_tag,
                                          &profile_nickname_tag,
                                          &set_nick_name_request_tag);
  /* populate input data in ASN1 object format */
  asn1_iccid.data_len            = set_nickname_request.iccid.data_len;
  asn1_iccid.data_ptr            = set_nickname_request.iccid.data;
  asn1_profile_nickname.data_len = set_nickname_request.profile_nickname.name_len;
  asn1_profile_nickname.data_ptr = set_nickname_request.profile_nickname.name;

  /* Get total malloc length: add passing length/fixed length */
  if(LPA_SUCCESS != lpa_asn1_util_get_len_field((uint32)set_nickname_request.iccid.data_len,
                                                &iccid_len))
  {
    return LPA_GENERIC_ERROR;
  }
  set_nickname_request_data_buff_payload_len += iccid_tag.tag_len +
                                                iccid_len.len_len +
                                                set_nickname_request.iccid.data_len;

  if(LPA_SUCCESS != lpa_asn1_util_get_len_field((uint32)set_nickname_request.profile_nickname.name_len,
                                                 &profile_nickname_len))
  {
    return LPA_GENERIC_ERROR;
  }
  set_nickname_request_data_buff_payload_len += profile_nickname_tag.tag_len +
                                                profile_nickname_len.len_len +
                                                set_nickname_request.profile_nickname.name_len;

  if(LPA_SUCCESS != lpa_asn1_util_get_len_field(set_nickname_request_data_buff_payload_len,
                                                &set_nickname_request_len))
  {
    return LPA_GENERIC_ERROR;
  }

  /* malloc for out_ptr */
  set_nickname_request_data_buff_total_len = set_nick_name_request_tag.tag_len +
                                             set_nickname_request_len.len_len +
                                             set_nickname_request_data_buff_payload_len;

  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(set_nickname_request_data_buff_total_len);
  if(NULL == out_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  /* populate out_ptr */
  out_ptr->data_len = 0;
  /* header LPA_ASN1_GEN_TAG_PREPARE_DOWNLOAD_REQUEST*/
  lpa_asn1_util_populate_tlv(out_ptr,
                             set_nickname_request_data_buff_total_len,
                             &set_nick_name_request_tag,
                             &set_nickname_request_len,
                             NULL);

  /* iccid LPA_ASN1_GEN_TAG_APPLICATION26*/
  lpa_asn1_util_populate_tlv(out_ptr,
                             set_nickname_request_data_buff_total_len,
                             &iccid_tag,
                             &iccid_len,
                             &asn1_iccid);
  /*profileNickname LPA_ASN1_GEN_TAG_UTF8STRING*/
  lpa_asn1_util_populate_tlv(out_ptr,
                             set_nickname_request_data_buff_total_len,
                             &profile_nickname_tag,
                             &profile_nickname_len,
                             &asn1_profile_nickname);

  return LPA_SUCCESS;
}/*lpa_asn1_encode_set_nickname_request*/


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_GET_RAT_REQUEST

DESCRIPTION
  This function encodes the GetEuiccDataRequest

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_get_rat_request (
  lpa_data_type*     out_ptr
)
{
  if(NULL == out_ptr)
  {
    return LPA_GENERIC_ERROR;
  }
  out_ptr->data_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM + /* GetRATRequest T */
                      LPA_ASN1_LEN_LEN_SINGLE;                    /* GetRATRequest L */

  out_ptr->data_ptr = (uint8 *)LPA_MALLOC(out_ptr->data_len);
  if(NULL == out_ptr->data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  out_ptr->data_ptr[0] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
  out_ptr->data_ptr[1] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC67_SECOND;
  out_ptr->data_ptr[2] = LPA_ASN1_LEN_ZERO;

  return LPA_SUCCESS;
}/*lpa_asn1_encode_get_rat_request*/
