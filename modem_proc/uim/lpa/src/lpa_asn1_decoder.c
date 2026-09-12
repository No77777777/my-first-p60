/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            L P A   A S N 1   D E C O D E R S


GENERAL DESCRIPTION

  This source file contains the ASN1 content decoders.

                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_asn1_decoder.c#6 $ $DateTime: 2021/08/05 07:51:25 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/04/21   pvb     Fix KW P1 issues
10/28/20   vgd     Send cancelsession if BPP doesn't contain SequenceOf86 TLV
03/29/18   bcho    LPA phase2.1 changes
06/01/17   av      LPA wrongly assumes ISDP AID to always be of 16 bytes
04/12/17   av      Add support to configure/retrieve default server address
04/11/17   av      Fixes for RAT decoding etc
01/12/17   av      Bug fixes for phase2 PRRID related logic
12/07/16   ll      LPA phase2 initial changes
09/22/16   ll      Peek into profile installation result
09/01/16   av      Do not return error if no data returned for GET STATUS
06/21/16   av      Add SVN check for supporting phase1, phase1.1 and phase2
05/19/16   ll      ASN1 decoder enhancement
04/11/16   av      Initial revision
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
#include "lpa_asn1_decoder.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
/* ASN1 tag field */
#define LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM            0x01
#define LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM           0x02
#define LPA_ASN1_LEN_ZERO                                   0x00
#define LPA_ASN1_MAX_TAG_FIELD_LEN                          0x02
#define LPA_ASN1_FIRST_TAG_INDEX                            0x00
#define LPA_ASN1_SECOND_TAG_INDEX                           0x01
/* Simple type tags -- reference: X.208 */
#define LPA_ASN1_GEN_TAG_BOOLEAN                            0x01
#define LPA_ASN1_GEN_TAG_INT                                0x02
#define LPA_ASN1_GEN_TAG_BIT_STRING                         0x03
#define LPA_ASN1_GEN_TAG_OCTET                              0x04
#define LPA_ASN1_GEN_TAG_OBJECT_IDENTIFIER                  0x06
#define LPA_ASN1_GEN_TAG_UTF8STRING                         0x0C
/* Structured type tags */
#define LPA_ASN1_GEN_TAG_SEQUENCE                           0x30
/* Explicitly tagged type tags -- Context specific */
/* NOTE: low tag number(1 byte) for tag number <= 30, high tag number(2 byte) for tag number > 30*/
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0_CONSTRUCTED      0xA0
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1_CONSTRUCTED      0xA1
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC2_CONSTRUCTED      0xA2
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC3_CONSTRUCTED      0xA3
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC22_CONSTRUCTED     0xB6
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC23_CONSTRUCTED     0xB7
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC24_CONSTRUCTED     0xB8
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0                  0x80
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1                  0x81
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC2                  0x82
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC3                  0x83
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC4                  0x84
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC5                  0x85
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC6                  0x86
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC7                  0x87
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC8                  0x88
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC16                 0x90
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC17                 0x91
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC18                 0x92
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC19                 0x93
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC20                 0x94
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC21                 0x95
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC25                 0x99
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED 0xBF
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST             0x9F
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC32_SECOND          0x20
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC33_SECOND          0x21
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC34_SECOND          0x22
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC35_SECOND          0x23
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC37_SECOND          0x25
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC39_SECOND          0x27
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC40_SECOND          0x28
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC41_SECOND          0x29
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC43_SECOND          0x2B
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC45_SECOND          0x2D
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC46_SECOND          0x2E
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC47_SECOND          0x2F
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC48_SECOND          0x30
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC49_SECOND          0x31
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC50_SECOND          0x32
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC51_SECOND          0x33
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC52_SECOND          0x34
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC53_SECOND          0x35
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC54_SECOND          0x36
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC55_SECOND          0x37
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC56_SECOND          0x38
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC60_SECOND          0x3C
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC62_SECOND          0x3E
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC63_SECOND          0x3F
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC65_SECOND          0x41
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC67_SECOND          0x43
#define LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC112_SECOND         0x70
/* Explicitly tagged type tags -- Application */
#define LPA_ASN1_GEN_TAG_APPLICATION15                      0x4F
#define LPA_ASN1_GEN_TAG_APPLICATION26                      0x5A
#define LPA_ASN1_GEN_TAG_APPLICATION55                      0x77
#define LPA_ASN1_GEN_TAG_APPLICATION_FIRST                  0x5F
#define LPA_ASN1_GEN_TAG_APPLICATION73_SECOND               0x49
/* Explicitly tagged type tags -- Private */
#define LPA_ASN1_GEN_TAG_PRIVATE0                           0xE0
#define LPA_ASN1_GEN_TAG_PRIVATE3                           0xE3
/* Others */
#define LPA_ASN1_TAG_SELECT_ISDR                            0x6F
#define LPA_ASN1_TAG_ANY_LOW_TAG_NUMBER_FORM                0x00

/* ASN1 len field */
#define LPA_ASN1_NUM_LEN_BYTES_MASK                         0x7F
#define LPA_ASN1_ONE_BYTE_LEN                               0x80
#define LPA_ASN1_LPAE_SUPPORT_LEN                           0x01
#define LPA_ASN1_ERROR_CODE_LEN                             0x01
#define LPA_ASN1_MCC_MNC_LEN                                0x03
#define LPA_ASN1_PROFILE_STATE_LEN                          0x01
#define LPA_ASN1_ICON_TYPE_LEN                              0x01
#define LPA_ASN1_PROFILE_CLASS_TYPE_LEN                     0x01
#define LPA_ASN1_NOTIFICATIONEVEN_LEN                       0x02
#define LPA_ASN1_TRANSACTION_ID_MIN_LEN                     0x01
#define LPA_ASN1_TRANSACTION_ID_MAX_LEN                     0x10
#define LPA_ASN1_BPP_SEGMENT_MAX_LEN                        0x3FC  /* 1020 */
#define LPA_ASN1_INSTALLATION_RECEIPT_RESULT_CODE_LEN       LPA_INSTALLATION_RESULT_CODE_LEN

/* LPA ASN1 value field */
#define LPA_ASN1_PPR1_MASK                                  0x40
#define LPA_ASN1_PPR2_MASK                                  0x20
#define LPA_ASN1_PPR3_MASK                                  0x10
#define LPA_ASN1_LPAE_USING_CAT_MASK                        0x01
#define LPA_ASN1_LPAE_USING_SCWS_MASK                       0x02
#define LPA_ASN1_USER_CONSENT_REQUIRED_MASK                 0x80
#define LPA_ASN1_ONE_BYTE_BIT_STRING                        0x01
#define LPA_ASN1_TWO_BYTE_BIT_STRING                        0x02
#define LPA_ASN1_8BIT_STRING_MAX_LEN                        0x08

/* Generic */
#define LPA_ASN1_MAX_NUM_CMD_CMD_PARSING                    0x02
#define LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN                     0x02
#define LPA_ASN1_MAX_NUM_BPP_88_TLVS                        0x02
#define LPA_ASN1_LOW_NIBBLE_MASK                            0x0F

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
/* LPA ASN1 tag type type */
typedef enum {
  GET_EUICC_CONFIGURED_ADDRESS,
  SET_DEFAULT_DPADDRESS,
  PREPARE_DOWNLOAD,
  GET_EUICC_CHALLENGE,
  GET_EUICC_INFO1,
  GET_EUICC_INFO2,
  RETRIEVE_NOTIFICATION_LIST,
  REMOVE_NOTIFICATION_FROM_LIST,
  AUTHENTICATE_SERVER,
  CANCEL_SESSION,
  GET_PROFILES_INFO,
  ENABLE_PROFILE,
  DISABLE_PROFILE,
  DELETE_PROFILE,
  EUICC_MEMORY_RESET,
  GET_EID,
  SET_NICKNAME,
  GET_RAT,
  PROFILE_INSTALLATION_RESULT,
  INTERNAL_CONTEXT_SPECIFIC0_CONSTRUCTED,
  INTERNAL_CONTEXT_SPECIFIC1_CONSTRUCTED,
  INTERNAL_CONTEXT_SPECIFIC0,
  INTERNAL_CONTEXT_SPECIFIC1,
  INTERNAL_CONTEXT_SPECIFIC2,
  INTERNAL_CONTEXT_SPECIFIC6,
  INTERNAL_CONTEXT_SPECIFIC8,
  INTERNAL_CONTEXT_SPECIFIC16,
  INTERNAL_CONTEXT_SPECIFIC17,
  INTERNAL_CONTEXT_SPECIFIC18,
  INTERNAL_CONTEXT_SPECIFIC19,
  INTERNAL_CONTEXT_SPECIFIC20,
  INTERNAL_CONTEXT_SPECIFIC21,
  INTERNAL_CONTEXT_SPECIFIC22,
  INTERNAL_CONTEXT_SPECIFIC23,
  INTERNAL_CONTEXT_SPECIFIC24,
  INTERNAL_CONTEXT_SPECIFIC25,
  INTERNAL_CONTEXT_SPECIFIC112,
  INTERNAL_PRIVATE0,
  INTERNAL_PRIVATE3,
  INTERNAL_APPLICATION15,
  INTERNAL_APPLICATION26,
  INTERNAL_SEQUENCE,
  INTERNAL_INT,
  INTERNAL_BIT_STRING,
  ANY_TLV_TYPE,
  UNKNOWN_TLV_TYPE
}lpa_asn1_tlv_type_type;

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
   STRUCTURE:      LPA_LPD_NOTIFICATION_LIST_RESP_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  uint8                                           num_of_tlv_types;
  lpa_asn1_tlv_type_type                          tlv_type[LPA_ASN1_MAX_NUM_CMD_CMD_PARSING];
  lpa_asn1_tag_field_type                         tlv_tag[LPA_ASN1_MAX_NUM_CMD_CMD_PARSING];
} lpa_asn1_cmds_type;

/*=============================================================================

                       GLOBAL VARIABLES

=============================================================================*/
/*=============================================================================

                              FUNCTIONS

=============================================================================*/
/*===========================================================================
FUNCTION LPA_ASN1_GET_ACTION_RESULT_CODE

DESCRIPTION
  Utility function to get action result code byte

DEPENDENCIES

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_decode_util_get_action_result_code_enum (
  uint32                                               data_len,
  uint8                                                *data_ptr,
  lpa_asn1_tlv_type_type                               tlv_type,
  boolean                                              is_full_tlv
)
{
  uint32                  offset       = 0;
  lpa_result_enum_type    result_enum;

  /* if is full TLV is true check the TL bytes, otherwise parse the error code directly*/
  if(TRUE == is_full_tlv)
  {
    /* We assume there should be 1 byte result code present in the TLV */
    if(NULL == data_ptr                                         ||
      (LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN +1) > data_len           ||
       LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0 != data_ptr[offset++] ||
       1 != data_ptr[offset++])
    {
      return LPA_GENERIC_ERROR;
    }
  }
  /* Get the error code byte. NOTE we assume error code is incoded in a byte.
     Currently the number of errors is less than 128. Map the error code byte
     to lpa_result_enum_type*/
  switch(tlv_type)
  {
    case PREPARE_DOWNLOAD:
      switch(data_ptr[offset])
      {
        case 1:
          result_enum = LPA_ERROR_SMDP_INVALID_CERTIFICATE;
          break;
        case 2:
          result_enum = LPA_ERROR_SMDP_INVALID_SIGNATURE;
          break;
        case 3:
          result_enum = LPA_ERROR_SMDP_UNSUPPORTED_CURVE;
          break;
        case 4:
          result_enum = LPA_ERROR_SMDP_NO_SESSION_CONTEXT;
          break;
        case 5:
          result_enum = LPA_ERROR_SMDP_INVALID_TRANSACTION_ID;
          break;
        case 127:
          result_enum = LPA_ERROR_NOT_SUPPORTED;
          break;
        default:
          result_enum = LPA_GENERIC_ERROR;
          break;
      }
      break;
    case RETRIEVE_NOTIFICATION_LIST:
      /* For notification return generic error for now,
         instead of LPA_ERROR_NOTIFICATION_NO_RESULT_AVAILABLE*/
      result_enum = LPA_GENERIC_ERROR;
      break;
    case REMOVE_NOTIFICATION_FROM_LIST:
      switch(data_ptr[offset])
      {
        case 0:
          result_enum = LPA_SUCCESS;
          break;
        case 1:
          result_enum = LPA_ERROR_NO_EFFECT_FOR_ACTION;
          break;
        case 127:
          result_enum = LPA_ERROR_NOT_SUPPORTED;
          break;
        default:
          result_enum = LPA_GENERIC_ERROR;
          break;
      }
      break;
    case AUTHENTICATE_SERVER:
      switch(data_ptr[offset])
      {
        case 1:
          result_enum = LPA_ERROR_SMDP_INVALID_CERTIFICATE;
          break;
        case 2:
          result_enum = LPA_ERROR_SMDP_INVALID_SIGNATURE;
          break;
        case 3:
          result_enum = LPA_ERROR_SMDP_UNSUPPORTED_CURVE;
          break;
        case 4:
          result_enum = LPA_ERROR_SMDP_NO_SESSION_CONTEXT;
          break;
        case 5:
          result_enum = LPA_ERROR_SMDP_INVALID_OID;
          break;
        case 6:
          result_enum = LPA_ERROR_SMDP_EUICC_CHALLENGE_MISMATCH;
          break;
        case 7:
          result_enum = LPA_ERROR_SMDP_CI_PK_UNKNOWN;
          break;
        case 127:
          result_enum = LPA_ERROR_NOT_SUPPORTED;
          break;
        default:
          result_enum = LPA_GENERIC_ERROR;
          break;
      }
      break;
    case CANCEL_SESSION:
      switch(data_ptr[offset])
      {
        case 5:
          result_enum = LPA_ERROR_SMDP_INVALID_TRANSACTION_ID;
          break;
        case 127:
          result_enum = LPA_ERROR_NOT_SUPPORTED;
          break;
        default:
          result_enum = LPA_GENERIC_ERROR;
          break;
      }
      break;
    case ENABLE_PROFILE:
      switch(data_ptr[offset])
      {
        case 0:
          result_enum = LPA_SUCCESS;
          break;
        case 1:
          result_enum = LPA_ERROR_PROFILE_NOT_FOUND;
          break;
        case 2:
          result_enum = LPA_ERROR_PROFILE_NOT_IN_DISABLED_STATE;
          break;
        case 3:
          result_enum = LPA_ERROR_OPERATION_NOT_ALLOWED;
          break;
        case 4:
          result_enum = LPA_ERROR_PROFILE_WRONG_PROFILE_REENABLING;
          break;
        case 5:
          result_enum = LPA_ERROR_CAT_BUSY;
          break;
        case 127:
          result_enum = LPA_ERROR_NOT_SUPPORTED;
          break;
        default:
          result_enum = LPA_GENERIC_ERROR;
          break;
      }
      break;
    case DISABLE_PROFILE:
      switch(data_ptr[offset])
      {
        case 0:
          result_enum = LPA_SUCCESS;
          break;
        case 1:
          result_enum = LPA_ERROR_PROFILE_NOT_FOUND;
          break;
        case 2:
          result_enum = LPA_ERROR_PROFILE_NOT_IN_ENABLED_STATE;
          break;
        case 3:
          result_enum = LPA_ERROR_OPERATION_NOT_ALLOWED;
          break;
        case 5:
          result_enum = LPA_ERROR_CAT_BUSY;
          break;
        case 127:
          result_enum = LPA_ERROR_NOT_SUPPORTED;
          break;
        default:
          result_enum = LPA_GENERIC_ERROR;
          break;
      }
      break;
    case DELETE_PROFILE:
      switch(data_ptr[offset])
      {
        case 0:
          result_enum = LPA_SUCCESS;
          break;
        case 1:
          result_enum = LPA_ERROR_PROFILE_NOT_FOUND;
          break;
        case 2:
          result_enum = LPA_ERROR_PROFILE_NOT_IN_DISABLED_STATE;
          break;
        case 3:
          result_enum = LPA_ERROR_OPERATION_NOT_ALLOWED;
          break;
        case 127:
          result_enum = LPA_ERROR_NOT_SUPPORTED;
          break;
        default:
          result_enum = LPA_GENERIC_ERROR;
          break;
      }
      break;
    case EUICC_MEMORY_RESET:
      switch(data_ptr[offset])
      {
        case 0:
          result_enum = LPA_SUCCESS;
          break;
        case 1:
          result_enum = LPA_ERROR_NO_EFFECT_FOR_ACTION;
          break;
        case 5:
          result_enum = LPA_ERROR_CAT_BUSY;
          break;
        case 127:
          result_enum = LPA_ERROR_NOT_SUPPORTED;
          break;
        default:
          result_enum = LPA_GENERIC_ERROR;
          break;
      }
      break;
    case SET_NICKNAME:
      switch(data_ptr[offset])
      {
        case 0:
          result_enum = LPA_SUCCESS;
          break;
        case 1:
          result_enum = LPA_ERROR_PROFILE_NOT_FOUND;
          break;
        case 127:
          result_enum = LPA_ERROR_NOT_SUPPORTED;
          break;
        default:
          result_enum = LPA_GENERIC_ERROR;
          break;
      }
      break;
    case GET_EUICC_CONFIGURED_ADDRESS:
      switch(data_ptr[offset])
      {
        case 0:
          result_enum = LPA_SUCCESS;
          break;
        case 127:
          result_enum = LPA_ERROR_NOT_SUPPORTED;
          break;
        default:
          result_enum = LPA_GENERIC_ERROR;
          break;
      }
      break;
    default:
      result_enum = LPA_GENERIC_ERROR;
      break;
  }

  LPA_MSG_ERR_1("lpa_asn1_decode_util_get_action_result_code_enum returned=0x%x", result_enum);

  return result_enum;
}/*lpa_asn1_decode_util_get_action_result_code_enum*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_UTIL_PARSE_TLV

DESCRIPTION
  Function parses ASN1 TLV based on input data.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_decode_util_parse_tlv (
  uint32                                               data_len,
  uint8                                                *data_ptr,
  uint32                                               offset,
  lpa_asn1_cmds_type                                   cmds,
  uint32                                               min_len,
  uint32                                               max_len,
  uint32                                               *offset_out_ptr,
  uint32                                               *v_len_out_ptr,
  lpa_asn1_tlv_type_type                               *tlv_type_out_ptr,
  boolean                                              is_optional
)
{
  uint32                 i                   = 0;
  uint8                  current_tlv_len_len = 0;
  uint32                 current_tlv_len     = 0;
  uint32                 current_offset      = offset;
  boolean                found_tag           = FALSE;

  if((1 < cmds.num_of_tlv_types &&
      NULL == tlv_type_out_ptr) ||
      NULL == data_ptr          ||
      data_len == 0             ||
      offset > data_len)
  {
    return LPA_GENERIC_ERROR;
  }

  if(offset == data_len)
  {
    /* Nothing to further process
       Return */
    return LPA_SUCCESS;
  }

  /* Initialize the output type value */
  if(NULL != tlv_type_out_ptr)
  {
    *tlv_type_out_ptr = UNKNOWN_TLV_TYPE;
  }

  /* Find and check the correct tag is present */
  for(i = 0; i < cmds.num_of_tlv_types; i++)
  {
    if(ANY_TLV_TYPE == cmds.tlv_type[i])
    {
      /* Any tag will be consider as a match.
         This is used to skip TLVs that we don't care */
      found_tag = TRUE;
      if(LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM <= (data_len - current_offset) &&
          LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM == cmds.tlv_tag[i].tag_len)
      {
        current_offset += LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
      }
      else if(LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM <= (data_len - current_offset) &&
              LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM == cmds.tlv_tag[i].tag_len)
      {
        current_offset += LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
      }
      /* Normally, the caller suppose to pass in the tlv_type_out_ptr
         when using ANY_TLV_TYPE, this is how this util function
         tell the caller that it found a TLV*/
      if(NULL != tlv_type_out_ptr)
      {
        *tlv_type_out_ptr = cmds.tlv_type[i];
      }
      break;
    }

    if(LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM      <= (data_len - current_offset) &&
       cmds.tlv_tag[i].tag[LPA_ASN1_FIRST_TAG_INDEX] == data_ptr[current_offset])
    {
      if(LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM   <= (data_len - current_offset)  &&
         LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM   == cmds.tlv_tag[i].tag_len)
      {
        if(cmds.tlv_tag[i].tag[LPA_ASN1_SECOND_TAG_INDEX] == data_ptr[current_offset + 1])
        {
          found_tag = TRUE;
          current_offset += LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
          if(NULL != tlv_type_out_ptr)
          {
            *tlv_type_out_ptr = cmds.tlv_type[i];
          }
          break;
        }
      }
      else if(LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM   == cmds.tlv_tag[i].tag_len)
      {
        found_tag = TRUE;
        current_offset += LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
        if(NULL != tlv_type_out_ptr)
        {
          *tlv_type_out_ptr = cmds.tlv_type[i];
        }
        break;
      }
    }
  }

  if (FALSE == found_tag)
  {
    if(FALSE == is_optional)
    {
      /* Did match any expecting tags */
      return LPA_GENERIC_ERROR;
    }
  }
  else
  {
    /* Get the V len */
    if((data_len <= current_offset) ||
        LPA_SUCCESS != lpa_util_get_len_from_tlv(&current_tlv_len,
                                                 &current_tlv_len_len,
                                                 (data_len - current_offset),
                                                 (data_ptr + current_offset)) ||
       (data_len - current_offset - current_tlv_len_len) < current_tlv_len)
    {
      /* Any of the following error happened:
           1.Length field not present
           2.Unable to decode length field
           3.Not enough data for PrepareDownloadResponseOk
             or PrepareDownloadResponseError*/
      return LPA_GENERIC_ERROR;
    }
    current_offset += current_tlv_len_len;

    /* Check the V len is in range for min_len, max_len and (data_len-offset) */
    if((min_len > 0 && (min_len > current_tlv_len)) ||
       (max_len > 0 && (max_len < current_tlv_len)))
    {
      return LPA_GENERIC_ERROR;
    }
    /* Update outputs, is this the offset for the value field*/
    if(NULL != offset_out_ptr)
    {
      *offset_out_ptr = current_offset;
    }

    if(NULL != v_len_out_ptr)
    {
      /* If v_len_out_ptr is NULL, it means
         the caller doesn't care about the v len value.
         Most likely it's a fixed value.*/
      *v_len_out_ptr  = current_tlv_len;
    }
  }

  return LPA_SUCCESS;
}/*lpa_asn1_decode_util_parse_tlv*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_PPR_IDS

DESCRIPTION
  Function parses the ASN1 formatted pprids TLV.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_util_decode_ppr_ids (
  uint32             data_len,
  uint8              *data_ptr,
  uint32             offset,
  uint32             *offset_out_ptr,
  lpa_ppr_ids_type   *ppr_ids_ptr,
  boolean            is_optional
)
{
  lpa_asn1_cmds_type     cmds;
  lpa_asn1_tlv_type_type current_tlv_type  = UNKNOWN_TLV_TYPE;
  uint32                 current_tlv_len   = LPA_ASN1_LEN_ZERO;

  memset(&cmds, 0, sizeof(lpa_asn1_cmds_type));
  /* Input validation */
  if (NULL == data_ptr    ||
      NULL == ppr_ids_ptr ||
      NULL == offset_out_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  cmds.num_of_tlv_types   = 2;
  cmds.tlv_type[0]        = INTERNAL_CONTEXT_SPECIFIC25;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC25;
  cmds.tlv_type[1]        = GET_RAT;
  cmds.tlv_tag[1].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[1].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   offset_out_ptr,
                                                   &current_tlv_len,
                                                   &current_tlv_type,
                                                   is_optional))
  {
    return LPA_GENERIC_ERROR;
  }

  if (INTERNAL_CONTEXT_SPECIFIC25 == current_tlv_type ||
      GET_RAT == current_tlv_type)
  {
    /* If some bit is enabled, BIT STRING must be encoded in 2 bytes, where,
       first byte represents the number of unused bits in the second byte AND
       second byte is the mask representing the bits that are enabled.
       IF no bit is enabled, BIT STRING must be encoded in one byte, with value
       as 0 */
    if (current_tlv_len != LPA_ASN1_ONE_BYTE_BIT_STRING &&
        current_tlv_len != LPA_ASN1_TWO_BYTE_BIT_STRING )
    {
      LPA_MSG_ERR_1("Decoding PPR failed as bit string is wrongly encoded over 0x%x bytes", current_tlv_len);
      return LPA_GENERIC_ERROR;
    }

    if (data_ptr[*offset_out_ptr] == 0)
    {
      /* No bit is set in the BIT STRING */
      ppr_ids_ptr->ppr1 = FALSE;
      ppr_ids_ptr->ppr2 = FALSE;
      ppr_ids_ptr->ppr3 = FALSE;
    }
    else if (data_ptr[*offset_out_ptr] < LPA_ASN1_8BIT_STRING_MAX_LEN)
    {
      ppr_ids_ptr->ppr1 = ((data_ptr[*offset_out_ptr+1] & LPA_ASN1_PPR1_MASK)) ? TRUE : FALSE;
      ppr_ids_ptr->ppr2 = ((data_ptr[*offset_out_ptr+1] & LPA_ASN1_PPR2_MASK)) ? TRUE : FALSE;
      ppr_ids_ptr->ppr3 = ((data_ptr[*offset_out_ptr+1] & LPA_ASN1_PPR3_MASK)) ? TRUE : FALSE;
    }
    else
    {
      LPA_MSG_ERR_1("Decoding PPR failed as bit mask 0x%x is incorrect", data_ptr[*offset_out_ptr]);
      return LPA_GENERIC_ERROR;
    }
  }

  /* Increment the offset_out_ptr by current_tlv_len */
  *offset_out_ptr += current_tlv_len;

  return LPA_SUCCESS;
}/*lpa_asn1_util_decode_ppr_ids*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_OPERATOR_ID

DESCRIPTION
  Function parses the ASN1 formatted SEQUENCE OF OperatorId.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_util_decode_operator_id (
  uint32                     data_len,
  uint8                      *data_ptr,
  uint32                     offset,
  uint32                     *offset_out_ptr,
  lpa_operator_id_type       *lpa_operator_id_ptr,
  boolean                    is_optional
)
{
  lpa_asn1_cmds_type     cmds;
  lpa_asn1_cmds_type     cmds1;
  uint32                 current_tlv_len        = LPA_ASN1_LEN_ZERO;
  lpa_asn1_tlv_type_type current_tlv_type       = UNKNOWN_TLV_TYPE;
  uint32                 current_offset         = offset;
  uint32                 operator_id_tlv_len    = LPA_ASN1_LEN_ZERO;

  memset(&cmds, 0x00, sizeof(lpa_asn1_cmds_type));
  memset(&cmds1, 0x00, sizeof(lpa_asn1_cmds_type));
  cmds.num_of_tlv_types    = 2;
  cmds.tlv_type[0]         = INTERNAL_CONTEXT_SPECIFIC23;
  cmds.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC23_CONSTRUCTED;
  cmds.tlv_type[1]         = GET_RAT;
  cmds.tlv_tag[1].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[1].tag[0]   = LPA_ASN1_GEN_TAG_SEQUENCE; /* For GET RAT request */

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   current_offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &current_offset,
                                                   &operator_id_tlv_len,
                                                   &current_tlv_type,
                                                   is_optional))
  {
    return LPA_GENERIC_ERROR;
  }

  if (INTERNAL_CONTEXT_SPECIFIC23 == current_tlv_type ||
      GET_RAT == current_tlv_type)
  {
    /* Found operator id tag*/
    /* mccMnc */
    current_tlv_len = LPA_ASN1_LEN_ZERO;
    cmds1.num_of_tlv_types   = 1;
    cmds1.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
    cmds1.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;
    if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                     data_ptr,
                                                     current_offset,
                                                     cmds1,
                                                     LPA_ASN1_MCC_MNC_LEN,
                                                     LPA_ASN1_MCC_MNC_LEN,
                                                     &current_offset,
                                                     NULL,
                                                     NULL,
                                                     FALSE))
    {
      return LPA_GENERIC_ERROR;
    }

    lpa_operator_id_ptr->mcc[0]              = data_ptr[current_offset] & LPA_ASN1_LOW_NIBBLE_MASK;
    lpa_operator_id_ptr->mcc[1]              = (data_ptr[current_offset] >> 4) & LPA_ASN1_LOW_NIBBLE_MASK;
    lpa_operator_id_ptr->mcc[2]              = data_ptr[current_offset+1] & LPA_ASN1_LOW_NIBBLE_MASK;
    lpa_operator_id_ptr->mnc.mnc_data[0]     =  data_ptr[current_offset+2] & LPA_ASN1_LOW_NIBBLE_MASK;
    lpa_operator_id_ptr->mnc.mnc_data[1]     = (data_ptr[current_offset+2] >> 4) & LPA_ASN1_LOW_NIBBLE_MASK;
    if((data_ptr[current_offset+1] & LPA_ASN1_LOW_NIBBLE_MASK) == LPA_ASN1_LOW_NIBBLE_MASK)
    {
      lpa_operator_id_ptr->mnc.mnc_len = 2;
    }
    else
    {
      lpa_operator_id_ptr->mnc.mnc_data[2]     = (data_ptr[current_offset+1] >> 4) & LPA_ASN1_LOW_NIBBLE_MASK;
      lpa_operator_id_ptr->mnc.mnc_len = 3;
    }

    current_offset += LPA_ASN1_MCC_MNC_LEN;

    if((current_offset - offset - LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN) >= operator_id_tlv_len)
    {
      /* Operator id TLV only contains MCCMNC */
      *offset_out_ptr = current_offset;
      return LPA_SUCCESS;
    }

    /* gid1 OPTIONAL */
    current_tlv_len = LPA_ASN1_LEN_ZERO;
    cmds1.num_of_tlv_types   = 1;
    cmds1.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
    cmds1.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1;
    if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                     data_ptr,
                                                     current_offset,
                                                     cmds1,
                                                     0,
                                                     LPA_GID_LEN_MAX,
                                                     &current_offset,
                                                     &current_tlv_len,
                                                     NULL,
                                                     TRUE))
    {
      return LPA_GENERIC_ERROR;
    }
    if(0 != current_tlv_len)
    {
      lpa_operator_id_ptr->gid1.gid_len = current_tlv_len;
      memscpy(lpa_operator_id_ptr->gid1.gid_data,
              LPA_GID_LEN_MAX,
             &data_ptr[current_offset],
              current_tlv_len);
      current_offset += current_tlv_len;
    }

    if((current_offset - offset - LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN) >= operator_id_tlv_len)
    {
      /* Operator id TLV only contains MCCMNC and GID1 */
      *offset_out_ptr = current_offset;
      return LPA_SUCCESS;
    }

    /* gid2 OPTIONAL */
    current_tlv_len = LPA_ASN1_LEN_ZERO;
    cmds1.num_of_tlv_types   = 1;
    cmds1.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
    cmds1.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC2;
    if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                     data_ptr,
                                                     current_offset,
                                                     cmds1,
                                                     0,
                                                     LPA_GID_LEN_MAX,
                                                     &current_offset,
                                                     &current_tlv_len,
                                                     NULL,
                                                     TRUE))
    {
      return LPA_GENERIC_ERROR;
    }
    if(0 != current_tlv_len)
    {
      lpa_operator_id_ptr->gid2.gid_len = current_tlv_len;
      memscpy(lpa_operator_id_ptr->gid2.gid_data,
              LPA_GID_LEN_MAX,
             &data_ptr[current_offset],
              current_tlv_len);
      current_offset += current_tlv_len;
    }
  }
  *offset_out_ptr = current_offset;

  return LPA_SUCCESS;
}/*lpa_asn1_util_decode_operator_id*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_OPERATOR_IDS

DESCRIPTION
  Function parses the ASN1 formatted SEQUENCE OF OperatorId.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_util_decode_operator_ids (
  uint32                    data_len,
  uint8                     *data_ptr,
  uint32                    offset,
  uint32                    *offset_out_ptr,
  lpa_ppar_table_entry_type *ppar_table_entry_ptr
)
{
  uint32                 previous_offset        = 0;
  uint32                 current_offset         = 0;
  uint8                  current_operator_index = 0;
  uint32                 current_tlv_len        = LPA_ASN1_LEN_ZERO;
  lpa_asn1_cmds_type     cmds;

  /* Input validation */
  if(NULL == data_ptr             ||
     NULL == ppar_table_entry_ptr ||
     NULL == offset_out_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  memset(&cmds, 0x00, sizeof(cmds));

  /* Initialize num of operator ids */
  ppar_table_entry_ptr->num_operator_ids = 0;

  /* Tag A1 */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1_CONSTRUCTED;

  current_tlv_len = LPA_ASN1_LEN_ZERO;
  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   offset_out_ptr,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  previous_offset = *offset_out_ptr;
  current_offset = *offset_out_ptr;

  /* Populate allowed Operators */
  for(current_operator_index = 0;
     (data_len > offset && current_operator_index < LPA_NUM_OPERATOR_ID_MAX);
      current_operator_index++)
  {
    if(LPA_SUCCESS != lpa_asn1_util_decode_operator_id(data_len,
                                                       data_ptr,
                                                       current_offset,
                                                       &current_offset,
                                                       &ppar_table_entry_ptr->operator_ids[current_operator_index],
                                                       FALSE))
    {
      if(current_operator_index == 0)
      {
        /* At least one OperatorID must be present */
      return LPA_GENERIC_ERROR;
    }

      /* At least one OperatorID was present */
      break;
    }

    /* Found one more allowed operator */
    ppar_table_entry_ptr->num_operator_ids++;
  }

  *offset_out_ptr = current_offset;

  return LPA_SUCCESS;
}/*lpa_asn1_util_decode_operator_ids*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_NOTIFICATION_METADATA

DESCRIPTION
  Function parses the ASN1 formatted NotificationMetadata and populate
  lpa_notification_type. NOTE: this function will not advance the caller's
  offset counter.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_util_decode_notification_metadata (
  uint32                 data_len,
  uint8                  *data_ptr,
  uint32                 offset,
  lpa_notification_type  *lpa_notification_ptr
)
{
  lpa_asn1_cmds_type  cmds;
  uint32              current_offset      = offset;
  uint32              current_tlv_len     = 0;
  uint32              i                   = 0;
  lpa_asn1_tlv_type_type current_tlv_type = UNKNOWN_TLV_TYPE;

  /* Input validation */
  if(NULL == data_ptr    ||
     NULL == lpa_notification_ptr)
  {
    return LPA_GENERIC_ERROR;
  }
  memset(&cmds, 0, sizeof(lpa_asn1_cmds_type));

  /* Looking for transaction ID in case it's profileInstallationResult */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_type[0]        = INTERNAL_CONTEXT_SPECIFIC0;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;
  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   current_offset,
                                                   cmds,
                                                   LPA_ASN1_TRANSACTION_ID_MIN_LEN,
                                                   LPA_ASN1_TRANSACTION_ID_MAX_LEN,
                                                   &current_offset,
                                                   &current_tlv_len,
                                                   &current_tlv_type,
                                                   TRUE))
  {
    return LPA_GENERIC_ERROR;
  }

  if (INTERNAL_CONTEXT_SPECIFIC0 == current_tlv_type)
  {
    current_offset += current_tlv_len;
  }

  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC47_SECOND;
  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   current_offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &current_offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  /* seqNumber */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;
  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   current_offset,
                                                   cmds,
                                                   sizeof(uint8),
                                                   sizeof(uint32),
                                                   &current_offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  lpa_notification_ptr->seq_number = 0;
  /* Calculate the int value */
  for(i = 0; i < current_tlv_len; i++)
  {
    lpa_notification_ptr->seq_number +=
      ((uint32)data_ptr[current_offset + i] << (8 * (current_tlv_len - i -1)));
  }
  current_offset += current_tlv_len;

  /* profileManagementOperation */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1;
  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   current_offset,
                                                   cmds,
                                                   LPA_ASN1_NOTIFICATIONEVEN_LEN,
                                                   LPA_ASN1_NOTIFICATIONEVEN_LEN,
                                                   &current_offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }
  /* Don't care able what notification evt it is */
  current_offset += LPA_ASN1_NOTIFICATIONEVEN_LEN;

  /* notificationAddress */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_UTF8STRING;
  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   current_offset,
                                                   cmds,
                                                   0,
                                                   sizeof(lpa_notification_ptr->smdp_address.data),
                                                   &current_offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }
  lpa_notification_ptr->smdp_address.data_len = current_tlv_len;
  memscpy((uint8 *)lpa_notification_ptr->smdp_address.data,
          sizeof(lpa_notification_ptr->smdp_address.data),
          &data_ptr[current_offset],
          current_tlv_len);

  return LPA_SUCCESS;
}/*lpa_asn1_util_decode_notification_metadata*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_PROFILE_INSTALLATION_RECEIPT

DESCRIPTION
  Function parses the ASN1 formatted profile installation receipt as part of
  profile installation result.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_asn1_decode_profile_installation_receipt (
  uint32                                               data_len,
  uint8                                               *data_ptr,
  lpa_lpd_profile_installation_result_code_array_type  result_code_array
)
{
  uint32  offset                        = 0;
  uint8   current_tlv_len_len           = 0;
  uint32  current_tlv_len               = 0;

  if(NULL == result_code_array || NULL == data_ptr)
  {
    /* NULL result_code_array or data_ptr */
    return LPA_GENERIC_ERROR;
  }

  if(data_len < 1 ||
     LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0 != data_ptr[offset++])
  {
    return LPA_GENERIC_ERROR;
  }

  if((data_len <= offset) ||
      LPA_SUCCESS != lpa_util_get_len_from_tlv(&current_tlv_len,
                                               &current_tlv_len_len,
                                               (data_len - offset),
                                               (data_ptr  + offset)) ||
      0 == current_tlv_len)
  {
    /* Any of the following error happened:
       1.Length field not present
       2.Unable to decode length field
       3.No data in the 1-16bytes transaction ID value field.*/
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len_len;
  /* Skip the value field */
  offset += current_tlv_len;

  /* Parse Result code */
  if((data_len - 2) < offset ||
      data_ptr[offset++] != LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1 ||
      data_ptr[offset++] != LPA_ASN1_INSTALLATION_RECEIPT_RESULT_CODE_LEN ||
      (data_len - LPA_ASN1_INSTALLATION_RECEIPT_RESULT_CODE_LEN) < offset)
  {
    /* Any of the following error happened:
       1.Not enough data for result code tag and len
       2.Incorrect tag or len value for result code
       3.Not enough data left for value field of the result code.*/
    return LPA_GENERIC_ERROR;
  }

  (void)memscpy((void*)result_code_array,
                sizeof(lpa_lpd_profile_installation_result_code_array_type),
                (void*)(data_ptr + offset),
                LPA_ASN1_INSTALLATION_RECEIPT_RESULT_CODE_LEN);

  LPA_MSG_HIGH_2("LPA profile installation receipt result code[0] = 0x%x, [1] = 0x%x",
                  result_code_array[0], result_code_array[1]);

  return LPA_SUCCESS;
}/*lpa_asn1_decode_profile_installation_receipt*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_PROFILE_INSTALLATION_RESULT_PHASE_1

DESCRIPTION
  Function parses the ASN1 formatted APDU response received as a result of
  LoadBoundProfilePackage or GetProfileInstallationResult request.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_profile_installation_result_phase1 (
  uint32                                               data_len,
  uint8                                                *data_ptr,
  lpa_lpd_profile_installation_result_code_array_type  result_code_array
)
{
  uint32  offset                        = 0;
  uint8   current_tlv_len_len           = 0;
  uint32  current_tlv_len               = 0;

  if(NULL == result_code_array || NULL == data_ptr)
  {
    /* NULL result_code_array or data_ptr */
    return LPA_GENERIC_ERROR;
  }

  /* Parse profile installation result */
  if(data_len < 2 ||
     LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED != data_ptr[offset++] ||
     LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC40_SECOND != data_ptr[offset++])
  {
    /* First tag bytes must be the profile installation result tag */
    return LPA_GENERIC_ERROR;
  }

  if(data_len <= offset ||
     LPA_SUCCESS != lpa_util_get_len_from_tlv(&current_tlv_len,
                                              &current_tlv_len_len,
                                              (data_len - offset),
                                              (data_ptr  + offset)) ||
     0 == current_tlv_len)
  {
    /* Any of the following error happened:
       1.Length field not present
       2.Unable to decode length field
       3.No data in the manditory profile installation result.*/
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len_len;

  /* Parse the data of profile installation result */
  /* Parse eUICC's InitialiseSecureChannel */
  if((data_len - 2) < offset                                                    ||
      LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED != data_ptr[offset++] ||
      LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC35_SECOND          != data_ptr[offset++])
  {
    /* Second tag bytes must be the initialise secure channel tag */
    return LPA_GENERIC_ERROR;
  }

  if((data_len <= offset) ||
      LPA_SUCCESS != lpa_util_get_len_from_tlv(&current_tlv_len,
                                               &current_tlv_len_len,
                                               (data_len - offset),
                                               (data_ptr  + offset)) ||
     0 == current_tlv_len)
  {
    /* Any of the following error happened:
       1.Length field not present
       2.Unable to decode length field
       3.No data in the manditory Initialise Secure Channel response.*/
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len_len;
  /* Skip the value field */
  offset += current_tlv_len;

  /* Parse eUICC's signature for InitialiseSecureChannel */
  if((data_len - 2) < offset                                           ||
     LPA_ASN1_GEN_TAG_APPLICATION_FIRST         != data_ptr[offset++]  ||
     LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC55_SECOND != data_ptr[offset++])
  {
    /* Second tag bytes must be the initialise secure channel signature tag */
    return LPA_GENERIC_ERROR;
  }

  if((data_len <= offset)                                            ||
     LPA_SUCCESS != lpa_util_get_len_from_tlv(&current_tlv_len,
                                              &current_tlv_len_len,
                                              (data_len - offset),
                                              (data_ptr  + offset))  ||
     0 == current_tlv_len)
  {
    /* Any of the following error happened:
       1.Length field not present
       2.Unable to decode length field
       3.No data in the manditory Initialise Secure Channel response signature.*/
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len_len;
  /* Skip the value field */
  offset += current_tlv_len;

  /* Parse 87 TLVs if we have we have 87 tag byte */
  /* The first 2 87 TLVs are mandatory however we don't put restriction here for now */
  while(data_len > offset &&
        LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC7 == data_ptr[offset])
  {
    /* Move to next byte after tag */
    offset++;
    if((data_len <= offset)                                            ||
        LPA_SUCCESS != lpa_util_get_len_from_tlv(&current_tlv_len,
                                                 &current_tlv_len_len,
                                                 (data_len - offset),
                                                 (data_ptr  + offset)) ||
       0 == current_tlv_len)
    {
      /* Any of the following error happened:
         1.Length field not present
         2.Unable to decode length field
         3.No data in 87 value field is not allowed.R-MAC should be present*/
      return LPA_GENERIC_ERROR;
    }
    offset += current_tlv_len_len;
    /* Skip the value field */
    offset += current_tlv_len;
  }

  /* Parse optional 86 TLVs if we have we have 86 tag byte.
     There should be one 1 mandatory 86 TLV, however we don't put
     restriction here for now */
  while(data_len > offset &&
        LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC6 == data_ptr[offset])
  {
    /*move to next byte after tag*/
    offset++;
    if((data_len <= offset)                                            ||
        LPA_SUCCESS != lpa_util_get_len_from_tlv(&current_tlv_len,
                                                 &current_tlv_len_len,
                                                 (data_len - offset),
                                                 (data_ptr  + offset)) ||
        0 == current_tlv_len)
    {
      /* Any of the following error happened:
         1.Length field not present
         2.Unable to decode length field
         3.No data in 86 value field is not allowed, R-MAC should be present.*/
      return LPA_GENERIC_ERROR;
    }
    offset += current_tlv_len_len;
    /* Skip the value field */
    offset += current_tlv_len;
  }

  /* Parse profile installation receipt */
  if((data_len - 2) < offset                                                     ||
      data_ptr[offset++] != LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED  ||
      data_ptr[offset++] != LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC39_SECOND)
  {
    /* Second tag bytes must be the receipt tag */
    return LPA_GENERIC_ERROR;
  }

  if((data_len <= offset)                                            ||
      LPA_SUCCESS != lpa_util_get_len_from_tlv(&current_tlv_len,
                                               &current_tlv_len_len,
                                               (data_len - offset),
                                               (data_ptr  + offset)) ||
      0 == current_tlv_len)
  {
    /* Any of the following error happened:
       1.Length field not present
       2.Unable to decode length field
       3.No data in receipt value field is not allowed due to mandatory data.*/
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len_len;

  /* Decode profile installation receipt */
  if(LPA_SUCCESS != lpa_asn1_decode_profile_installation_receipt(current_tlv_len,
                                                                 (data_ptr + offset),
                                                                 result_code_array))
  {
    return LPA_GENERIC_ERROR;
  }

  return LPA_SUCCESS;
}/*lpa_asn1_decode_profile_installation_result_phase1*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_PROFILE_INSTALLATION_RESULT

DESCRIPTION
  Function parses the ASN1 formatted APDU response received as a result of
  LoadBoundProfilePackage or GetProfileInstallationResult request.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_profile_installation_result (
  uint32                                               data_len,
  uint8                                                *data_ptr,
  lpa_lpd_profile_installation_result_code_array_type  result_code_array
)
{
  uint32                 offset                      = 0;
  uint32                 current_tlv_len             = 0;
  lpa_asn1_tlv_type_type current_tlv_type            = UNKNOWN_TLV_TYPE;
  lpa_asn1_cmds_type     cmds;

  memset(&cmds, 0x00, sizeof(lpa_asn1_cmds_type));

  /* Parse BF37 ProfileInstallationResult TLV */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC55_SECOND;

  if(NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  /* Parse BF27 ProfileInstallationResultData TLV */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC39_SECOND;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  /* Parse 80 transactionId TLV */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   LPA_ASN1_TRANSACTION_ID_MIN_LEN,
                                                   LPA_ASN1_TRANSACTION_ID_MAX_LEN,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len;

  /* Parse BF2F notificationMetadata TLV */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC47_SECOND;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len;

  /* Parse 06 smdpOid optional TLV */
  current_tlv_len = LPA_ASN1_LEN_ZERO;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_OBJECT_IDENTIFIER;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   TRUE))
  {
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len;

  /* Parse A2 finalResult TLV */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC2_CONSTRUCTED;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  cmds.num_of_tlv_types   = 2;
  cmds.tlv_type[0]        = INTERNAL_CONTEXT_SPECIFIC0_CONSTRUCTED;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0_CONSTRUCTED;
  cmds.tlv_type[1]        = INTERNAL_CONTEXT_SPECIFIC1_CONSTRUCTED;
  cmds.tlv_tag[1].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[1].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1_CONSTRUCTED;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   &current_tlv_type,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }
  if(INTERNAL_CONTEXT_SPECIFIC0_CONSTRUCTED == current_tlv_type)
  {
    result_code_array[0] = 0;
  }
  else if(INTERNAL_CONTEXT_SPECIFIC1_CONSTRUCTED == current_tlv_type)
  {
    result_code_array[0] = 1;

    cmds.num_of_tlv_types   = 1;
    cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
    cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0_CONSTRUCTED;
    if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                     data_ptr,
                                                     offset,
                                                     cmds,
                                                     0,
                                                     0,
                                                     &offset,
                                                     &current_tlv_len,
                                                     NULL,
                                                     FALSE))
    {
      return LPA_GENERIC_ERROR;
    }
    offset += current_tlv_len;

    cmds.num_of_tlv_types   = 1;
    cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
    cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1_CONSTRUCTED;
    if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                     data_ptr,
                                                     offset,
                                                     cmds,
                                                     LPA_ASN1_ERROR_CODE_LEN,
                                                     LPA_ASN1_ERROR_CODE_LEN,
                                                     &offset,
                                                     &current_tlv_len,
                                                     NULL,
                                                     FALSE))
    {
      return LPA_GENERIC_ERROR;
    }
    result_code_array[1] = data_ptr[offset];
  }

  return LPA_SUCCESS;
}/*lpa_asn1_decode_profile_installation_result*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_GET_PROFILES_INFO_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  get profile info.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_get_profiles_info_resp (
  uint32                                 data_len,
  uint8                                  *data_ptr,
  lpa_get_profiles_info_resp_type        *resp_ptr
)
{
  uint32                 offset                      = 0;
  uint32                 first_profileinfo_offset    = 0;
  uint32                 current_tlv_len             = 0;
  lpa_asn1_tlv_type_type current_tlv_type            = UNKNOWN_TLV_TYPE;
  uint32                 current_profileinfo_index   = 0;
  lpa_asn1_cmds_type     cmds;
  lpa_asn1_cmds_type     cmds1;

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC45_SECOND;

  /* Parse BF2D TLV */
  if(NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   TRUE))
  {
    return LPA_GENERIC_ERROR;
  }

  /* This is a CHOICE with 2 possible cmds */
  cmds.num_of_tlv_types    = 2;
  cmds.tlv_type[0]         = INTERNAL_SEQUENCE;
  cmds.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0_CONSTRUCTED;
  cmds.tlv_type[1]         = INTERNAL_INT;
  cmds.tlv_tag[1].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[1].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   &current_tlv_type,
                                                   TRUE))
  {
    return LPA_GENERIC_ERROR;
  }

  if(LPA_ASN1_ERROR_CODE_LEN == current_tlv_len &&
     INTERNAL_INT            == current_tlv_type)
  {
    /* Parse and return result code */
    return lpa_asn1_decode_util_get_action_result_code_enum((data_len - offset),
                                                            (uint8*)(data_ptr + offset),
                                                            GET_PROFILES_INFO,
                                                            FALSE);
  }
  else
  {
    /* profileInfoListOk */
   /* SEQUENCE OF ProfileInfo */
    cmds.num_of_tlv_types    = 1;
    cmds.tlv_type[0]         = INTERNAL_PRIVATE3;
    cmds.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
    cmds.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_PRIVATE3;

    /* Reset notification list */
    resp_ptr->num_profiles    = 0;
    first_profileinfo_offset  = offset;
    /* Count how many ProfileInfos */
    while(data_len > offset &&
          LPA_NUM_PROFILES_MAX > resp_ptr->num_profiles)
    {
      /* At this point, we know that the TLV should only
         contain a sequence of ProfileInfo. Hence, set
         is_optional flag to FALSE.*/
      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds,
                                                       0,
                                                       0,
                                                       &offset,
                                                       &current_tlv_len,
                                                       NULL,
                                                       FALSE))
      {
        return LPA_GENERIC_ERROR;
      }
      resp_ptr->num_profiles++;
      offset += current_tlv_len;
    }

    /* Populate notification_list */
    offset = first_profileinfo_offset;
    while(data_len > offset &&
          current_profileinfo_index < resp_ptr->num_profiles)
    {
      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds,
                                                       0,
                                                       0,
                                                       &offset,
                                                       NULL,
                                                       NULL,
                                                       FALSE))
      {
        return LPA_GENERIC_ERROR;
      }

      /* Start parsing the profile info content */
      /* Optional iccid */
      cmds1.num_of_tlv_types    = 1;
      cmds1.tlv_type[0]         = INTERNAL_APPLICATION26;
      cmds1.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
      cmds1.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_APPLICATION26;

      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds1,
                                                       LPA_ICCID_LEN,
                                                       LPA_ICCID_LEN,
                                                       &offset,
                                                       &current_tlv_len,
                                                       &current_tlv_type,
                                                       TRUE))
      {
        return LPA_GENERIC_ERROR;
      }

      if(INTERNAL_APPLICATION26 == current_tlv_type)
      {
        /* Found Optional iccid */
        resp_ptr->profile_info[current_profileinfo_index].iccid.data_len = LPA_ICCID_LEN;
        memscpy((uint8*)(resp_ptr->profile_info[current_profileinfo_index].iccid.data),
                LPA_ICCID_LEN,
                (const uint8 *)(data_ptr + offset),
                LPA_ICCID_LEN);
        offset += LPA_ICCID_LEN;
      }
      if(offset >= data_len)
      {
        /* No more data, and no manditory field afterwards */
        break;
      }

      /* Optional isdpAid */
      cmds1.num_of_tlv_types    = 1;
      cmds1.tlv_type[0]         = INTERNAL_APPLICATION15;
      cmds1.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
      cmds1.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_APPLICATION15;

      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds1,
                                                       LPA_ISDP_AID_LEN_MIN,
                                                       LPA_ISDP_AID_LEN,
                                                       &offset,
                                                       &current_tlv_len,
                                                       &current_tlv_type,
                                                       TRUE))
      {
        return LPA_GENERIC_ERROR;
      }

      if(INTERNAL_APPLICATION15 == current_tlv_type)
      {
        /* Found Optional isdpAid */
        resp_ptr->profile_info[current_profileinfo_index].isdp_aid.data_len = current_tlv_len;
        memscpy((uint8 *)(resp_ptr->profile_info[current_profileinfo_index].isdp_aid.data),
                LPA_ISDP_AID_LEN,
                (const uint8 *)(data_ptr + offset),
                current_tlv_len);
        offset += current_tlv_len;
      }
      if (offset >= data_len)
      {
        /* No more data, and no manditory field afterwards */
        break;
      }

      /* Optional profileState */
      cmds1.num_of_tlv_types    = 1;
      cmds1.tlv_type[0]         = INTERNAL_CONTEXT_SPECIFIC112;
      cmds1.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
      cmds1.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST;
      cmds1.tlv_tag[0].tag[1]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC112_SECOND;

      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds1,
                                                       LPA_ASN1_PROFILE_STATE_LEN,
                                                       LPA_ASN1_PROFILE_STATE_LEN,
                                                       &offset,
                                                       NULL,
                                                       &current_tlv_type,
                                                       TRUE))
      {
        return LPA_GENERIC_ERROR;
      }

      if(INTERNAL_CONTEXT_SPECIFIC112 == current_tlv_type)
      {
        if(data_ptr[offset] == 0x01)
        {
          resp_ptr->profile_info[current_profileinfo_index].state =
             LPA_PROFILE_STATE_ENABLED;
        }
        else if(data_ptr[offset] == 0x00)
        {
          resp_ptr->profile_info[current_profileinfo_index].state =
             LPA_PROFILE_STATE_DISABLED;
        }
        else
        {
          return LPA_GENERIC_ERROR;
        }
        offset += LPA_ASN1_PROFILE_STATE_LEN;
      }
      if(offset >= data_len)
      {
        /* No more data, and no manditory field afterwards */
        break;
      }

      /* Optional profileNickname */
      cmds1.num_of_tlv_types    = 1;
      cmds1.tlv_type[0]         = INTERNAL_CONTEXT_SPECIFIC16;
      cmds1.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
      cmds1.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC16;

      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds1,
                                                       0,
                                                       LPA_NAME_LEN_MAX,
                                                       &offset,
                                                       &current_tlv_len,
                                                       &current_tlv_type,
                                                       TRUE))
      {
        return LPA_GENERIC_ERROR;
      }

      if(INTERNAL_CONTEXT_SPECIFIC16 == current_tlv_type)
      {
        resp_ptr->profile_info[current_profileinfo_index].nickname.name_len = current_tlv_len;
        /* Nickname may be 0, hence this if-check */
        memscpy(resp_ptr->profile_info[current_profileinfo_index].nickname.name,
                LPA_NAME_LEN_MAX,
               &data_ptr[offset],
                current_tlv_len);
        offset += current_tlv_len;
      }
      if (offset >= data_len)
      {
        /* No more data, and no manditory field afterwards */
        break;
      }

      /* Optional serviceProviderName */
      cmds1.num_of_tlv_types    = 1;
      cmds1.tlv_type[0]         = INTERNAL_CONTEXT_SPECIFIC17;
      cmds1.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
      cmds1.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC17;

      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds1,
                                                       0,
                                                       LPA_SPN_LEN_MAX,
                                                       &offset,
                                                       &current_tlv_len,
                                                       &current_tlv_type,
                                                       TRUE))
      {
        return LPA_GENERIC_ERROR;
      }
      if(offset >= data_len)
      {
        /* No more data, and no manditory field afterwards */
        break;
      }
      if(INTERNAL_CONTEXT_SPECIFIC17 == current_tlv_type)
      {
        resp_ptr->profile_info[current_profileinfo_index].spn.name_len = current_tlv_len;
        memscpy(resp_ptr->profile_info[current_profileinfo_index].spn.name,
                LPA_SPN_LEN_MAX,
               &data_ptr[offset],
                current_tlv_len);
        offset += current_tlv_len;
      }
      if(offset >= data_len)
      {
        /* No more data, and no manditory field afterwards
           break */
        break;
      }

      /* Optional ProfileName */
      cmds1.num_of_tlv_types    = 1;
      cmds1.tlv_type[0]         = INTERNAL_CONTEXT_SPECIFIC18;
      cmds1.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
      cmds1.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC18;

      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds1,
                                                       0,
                                                       LPA_NAME_LEN_MAX,
                                                       &offset,
                                                       &current_tlv_len,
                                                       &current_tlv_type,
                                                       TRUE))
      {
        return LPA_GENERIC_ERROR;
      }

      if(INTERNAL_CONTEXT_SPECIFIC18 == current_tlv_type)
      {
        resp_ptr->profile_info[current_profileinfo_index].name.name_len = current_tlv_len;
        memscpy(resp_ptr->profile_info[current_profileinfo_index].name.name,
                LPA_NAME_LEN_MAX,
                &data_ptr[offset],
                current_tlv_len);
        offset += current_tlv_len;
      }
      if (offset >= data_len)
      {
        /* No more data, and no manditory field afterwards */
        break;
      }

      /* Optional IconType */
      cmds1.num_of_tlv_types    = 1;
      cmds1.tlv_type[0]         = INTERNAL_CONTEXT_SPECIFIC19;
      cmds1.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
      cmds1.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC19;

      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds1,
                                                       LPA_ASN1_ICON_TYPE_LEN,
                                                       LPA_ASN1_ICON_TYPE_LEN,
                                                       &offset,
                                                       &current_tlv_len,
                                                       &current_tlv_type,
                                                       TRUE))
      {
        return LPA_GENERIC_ERROR;
      }

      if(INTERNAL_CONTEXT_SPECIFIC19 == current_tlv_type)
      {
        /* We know iconType is 1 byte for now, so only grab the 1st byte*/
        resp_ptr->profile_info[current_profileinfo_index].icon_type = data_ptr[offset];
        offset += LPA_ASN1_ICON_TYPE_LEN;
      }
      if(offset >= data_len)
      {
        /* No more data, and no manditory field afterwards */
        break;
      }

      /* Optional Icon */
      cmds1.num_of_tlv_types    = 1;
      cmds1.tlv_type[0]         = INTERNAL_CONTEXT_SPECIFIC20;
      cmds1.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
      cmds1.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC20;

      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds1,
                                                       0,
                                                       0,
                                                       &offset,
                                                       &current_tlv_len,
                                                       &current_tlv_type,
                                                       TRUE))
      {
        return LPA_GENERIC_ERROR;
      }
      if(offset >= data_len)
      {
        /* No more data, and no manditory field afterwards */
        break;
      }

      if(INTERNAL_CONTEXT_SPECIFIC20 == current_tlv_type)
      {
        resp_ptr->profile_info[current_profileinfo_index].icon.data_len = current_tlv_len;
        resp_ptr->profile_info[current_profileinfo_index].icon.data_ptr = LPA_MALLOC(current_tlv_len);
        if (NULL == resp_ptr->profile_info[current_profileinfo_index].icon.data_ptr)
        {
          return LPA_GENERIC_ERROR;
        }
        memscpy(resp_ptr->profile_info[current_profileinfo_index].icon.data_ptr,
                current_tlv_len,
                &data_ptr[offset],
                current_tlv_len);
        offset += current_tlv_len;
      }
      if(offset >= data_len)
      {
        /* No more data, and no manditory field afterwards */
        break;
      }

      /* Optional profileClass */
      cmds1.num_of_tlv_types    = 1;
      cmds1.tlv_type[0]         = INTERNAL_CONTEXT_SPECIFIC21;
      cmds1.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
      cmds1.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC21;

      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds1,
                                                       LPA_ASN1_PROFILE_CLASS_TYPE_LEN,
                                                       LPA_ASN1_PROFILE_CLASS_TYPE_LEN,
                                                       &offset,
                                                       &current_tlv_len,
                                                       &current_tlv_type,
                                                       TRUE))
      {
        return LPA_GENERIC_ERROR;
      }

      if(INTERNAL_CONTEXT_SPECIFIC21 == current_tlv_type)
      {
        /* We know profileClass  is 1 byte for now, so only grab the 1st byte,
           Also the value match the profile_class enum*/
        resp_ptr->profile_info[current_profileinfo_index].profile_class = data_ptr[offset];
        offset += LPA_ASN1_PROFILE_CLASS_TYPE_LEN;
      }
      if(offset >= data_len)
      {
        /* No more data, and no manditory field afterwards */
        break;
      }

      /* Optional NotificationConfigurationInformation sequence*/
      /* We currently don't care about NotificationConfigurationInformation, hence skip */
      cmds1.num_of_tlv_types    = 1;
      cmds1.tlv_type[0]         = INTERNAL_CONTEXT_SPECIFIC22;
      cmds1.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
      cmds1.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC22_CONSTRUCTED;
      while(data_len > offset)
      {
        if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                         data_ptr,
                                                         offset,
                                                         cmds1,
                                                         LPA_ASN1_PROFILE_CLASS_TYPE_LEN,
                                                         LPA_ASN1_PROFILE_CLASS_TYPE_LEN,
                                                         &offset,
                                                         &current_tlv_len,
                                                         &current_tlv_type,
                                                         TRUE))
        {
          return LPA_GENERIC_ERROR;
        }
        if (INTERNAL_CONTEXT_SPECIFIC22 == current_tlv_type)
        {
          /* Skip the data */
          offset += current_tlv_len;
        }
        else
        {
          /* Found all of NotificationConfigurationInformation present, proceed*/
          break;
        }
      }
      if(offset >= data_len)
      {
        /* No more data, and no manditory field afterwards */
        break;
      }

      /* Optional profileOwner  (OperatorID ) */
      if(LPA_SUCCESS != lpa_asn1_util_decode_operator_id(data_len,
                                                         data_ptr,
                                                         offset,
                                                         &offset,
                                                         &resp_ptr->profile_info[current_profileinfo_index].ppr.operator_id,
                                                         TRUE))
      {
        return LPA_GENERIC_ERROR;
      }
      if (offset >= data_len)
      {
        /* No more data, and no manditory field afterwards */
        break;
      }

      /* Optional dpProprietaryData */
      cmds1.num_of_tlv_types    = 1;
      cmds1.tlv_type[0]         = INTERNAL_CONTEXT_SPECIFIC24;
      cmds1.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
      cmds1.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC24_CONSTRUCTED;

      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds1,
                                                       0,
                                                       0,
                                                       &offset,
                                                       &current_tlv_len,
                                                       &current_tlv_type,
                                                       TRUE))
      {
        return LPA_GENERIC_ERROR;
      }

      if(INTERNAL_CONTEXT_SPECIFIC24 == current_tlv_type)
      {
        offset += current_tlv_len;
      }
      if(offset >= data_len)
      {
        /* No more data, and no manditory field afterwards */
        break;
      }

      /* Optional profilePolicyRules (PprIds) */
      if(LPA_SUCCESS != lpa_asn1_util_decode_ppr_ids(data_len,
                                                     data_ptr,
                                                     offset,
                                                     &offset,
                                                     &resp_ptr->profile_info[current_profileinfo_index].ppr.ppr_ids,
                                                     TRUE))
      {
        return LPA_GENERIC_ERROR;
      }
      current_profileinfo_index ++;
    }
  }

  return LPA_SUCCESS;
} /* lpa_asn1_decode_get_profiles_info_apdu */


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_REMOVE_NOTIFICATION_LIST_RESP

DESCRIPTION
  Function parses the ASN formatted APDU response received as a result of
  select ISDR and decodes the SVN and lpae related info. This function might MALLOC
  resp_ptr->.profile_info[p].icon.data_ptr
  (where p ranges from 0 to LPA_NUM_PROFILES_MAX) which should be freed by
  the caller.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_remove_notification_from_list_resp (
  uint32                                               data_len,
  uint8                                                *data_ptr
)
{
  uint32                offset          = 0;
  lpa_asn1_cmds_type    cmds;

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC48_SECOND;

  /* Parse BF30 TLV */
  if(NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN+
                                                   LPA_ASN1_ERROR_CODE_LEN,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN+
                                                   LPA_ASN1_ERROR_CODE_LEN,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  /* Parse and return result code */
  return lpa_asn1_decode_util_get_action_result_code_enum(data_len - offset,
                                                          data_ptr + offset,
                                                          REMOVE_NOTIFICATION_FROM_LIST,
                                                          TRUE);
} /*lpa_asn1_decode_remove_notification_from_list_resp*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_ENABLE_PROFILE_RESP

DESCRIPTION
  Function parses the ASN formatted APDU response received as a result of
  Get Profiles Info request. This function might MALLOC
  resp_ptr->.profile_info[p].icon.data_ptr
  (where p ranges from 0 to LPA_NUM_PROFILES_MAX) which should be freed by
  the caller.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_enable_profile_resp (
  uint32                                 data_len,
  uint8                                  *data_ptr
)
{
  uint32                offset          = 0;
  lpa_asn1_cmds_type    cmds;

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC49_SECOND;

  /* Parse BF31 TLV */
  if(NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN+
                                                   LPA_ASN1_ERROR_CODE_LEN,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN+
                                                   LPA_ASN1_ERROR_CODE_LEN,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;

  return lpa_asn1_decode_util_get_action_result_code_enum(data_len - offset,
                                                          data_ptr + offset,
                                                          ENABLE_PROFILE,
                                                          TRUE);
} /*lpa_asn1_decode_enable_profile_resp*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_DISABLE_PROFILE_RESP

DESCRIPTION
  Function parses the ASN formatted APDU response received as a result of
  Get Profiles Info request. This function might MALLOC
  resp_ptr->.profile_info[p].icon.data_ptr
  (where p ranges from 0 to LPA_NUM_PROFILES_MAX) which should be freed by
  the caller.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_disable_profile_resp (
  uint32                                 data_len,
  uint8                                  *data_ptr
)
{
  uint32                offset          = 0;
  lpa_asn1_cmds_type    cmds;

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC50_SECOND;

  /* Parse BF32 TLV */
  if(NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN+
                                                   LPA_ASN1_ERROR_CODE_LEN,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN+
                                                   LPA_ASN1_ERROR_CODE_LEN,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;

  return lpa_asn1_decode_util_get_action_result_code_enum(data_len - offset,
                                                          data_ptr + offset,
                                                          DISABLE_PROFILE,
                                                          TRUE);
} /*lpa_asn1_decode_disable_profile_resp*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_DELETE_PROFILE_RESP

DESCRIPTION
  Function parses the ASN formatted APDU response received as a result of
  Get Profiles Info request. This function might MALLOC
  resp_ptr->.profile_info[p].icon.data_ptr
  (where p ranges from 0 to LPA_NUM_PROFILES_MAX) which should be freed by
  the caller.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_delete_profile_resp (
  uint32                                 data_len,
  uint8                                  *data_ptr
)
{
  uint32                offset          = 0;
  lpa_asn1_cmds_type    cmds;

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC51_SECOND;

  /* Parse BF33 TLV */
  if(NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN+
                                                   LPA_ASN1_ERROR_CODE_LEN,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN+
                                                   LPA_ASN1_ERROR_CODE_LEN,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  return lpa_asn1_decode_util_get_action_result_code_enum(data_len - offset,
                                                          data_ptr + offset,
                                                          DELETE_PROFILE,
                                                          TRUE);
} /*lpa_asn1_decode_enable_profile_resp*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_EUICC_MEMORY_RESET_RESP

DESCRIPTION
  Function parses the ASN formatted APDU response received as a result of
  Get Profiles Info request. This function might MALLOC
  resp_ptr->.profile_info[p].icon.data_ptr
  (where p ranges from 0 to LPA_NUM_PROFILES_MAX) which should be freed by
  the caller.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_euicc_memory_reset_resp (
  uint32                                 data_len,
  uint8                                  *data_ptr
)
{
  uint32                offset          = 0;
  lpa_asn1_cmds_type    cmds;

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC52_SECOND;

  /* Parse BF34 TLV */
  if(NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN+
                                                   LPA_ASN1_ERROR_CODE_LEN,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN+
                                                   LPA_ASN1_ERROR_CODE_LEN,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  return lpa_asn1_decode_util_get_action_result_code_enum(data_len - offset,
                                                          data_ptr + offset,
                                                          EUICC_MEMORY_RESET,
                                                          TRUE);
} /*lpa_asn1_decode_euicc_memory_reset_resp*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_SET_NICKNAME_RESP

DESCRIPTION
  Function parses the ASN formatted APDU response received as a result of
  Get Profiles Info request. This function might MALLOC
  resp_ptr->.profile_info[p].icon.data_ptr
  (where p ranges from 0 to LPA_NUM_PROFILES_MAX) which should be freed by
  the caller.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_set_nickname_resp (
  uint32                                 data_len,
  uint8                                 *data_ptr
)
{
  uint32                offset          = 0;
  lpa_asn1_cmds_type    cmds;

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC41_SECOND;

  /* Parse BF29 TLV */
  if(NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN+
                                                   LPA_ASN1_ERROR_CODE_LEN,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN+
                                                   LPA_ASN1_ERROR_CODE_LEN,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  return lpa_asn1_decode_util_get_action_result_code_enum(data_len - offset,
                                                          data_ptr + offset,
                                                          SET_NICKNAME,
                                                          TRUE);
} /*lpa_asn1_decode_set_nickname_resp*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_GET_EUICC_CHALLENGE_RESP

DESCRIPTION
  Function parses the ASN formatted APDU response received as a result of
  select ISDR and decodes the SVN and lpae related info. This function might MALLOC
  resp_ptr->.profile_info[p].icon.data_ptr
  (where p ranges from 0 to LPA_NUM_PROFILES_MAX) which should be freed by
  the caller.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_get_euicc_challenge_resp (
  uint32                                 data_len,
  uint8                                 *data_ptr,
  lpa_lpd_get_euicc_challenge_resp_type *resp_ptr
)
{
  uint32                offset          = 0;
  lpa_asn1_cmds_type    cmds;

  memset(&cmds, 0x00, sizeof(lpa_asn1_cmds_type));

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC46_SECOND;

  /* Parse BF2E TLV */
  if (NULL == data_ptr ||
      LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                    data_ptr,
                                                    offset,
                                                    cmds,
                                                    LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN +
                                                    LPA_EUICC_CHALLENGE_LEN,
                                                    LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN +
                                                    LPA_EUICC_CHALLENGE_LEN,
                                                    &offset,
                                                    NULL,
                                                    NULL,
                                                    FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  /* Extract eUICC challenge */
  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;

  /* Parse OCT 0x04 TLV */
  if (NULL == data_ptr ||
      LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                    data_ptr,
                                                    offset,
                                                    cmds,
                                                    LPA_EUICC_CHALLENGE_LEN,
                                                    LPA_EUICC_CHALLENGE_LEN,
                                                    &offset,
                                                    NULL,
                                                    NULL,
                                                    FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  memscpy((uint8 *)(resp_ptr->euicc_challenge),
          LPA_EUICC_CHALLENGE_LEN,
          (const uint8 *)(data_ptr + offset),
          LPA_EUICC_CHALLENGE_LEN);

  return LPA_SUCCESS;
} /*lpa_asn1_decode_get_euicc_challenge_resp*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_GET_EUICC_INFO_RESP

DESCRIPTION
  Function parses the ASN formatted APDU response of Get EUICC Info.
  This function might MALLOC resp_ptr->euicc_info.data_ptr which should
  be freed by the caller.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_get_euicc_info_resp (
  uint32                                 data_len,
  uint8                                 *data_ptr,
  lpa_lpd_get_euicc_info_resp_type      *resp_ptr
)
{
  uint32                 offset           = 0;
  uint32                 current_tlv_len  = 0;
  lpa_asn1_tlv_type_type current_tlv_type = UNKNOWN_TLV_TYPE;
  lpa_asn1_cmds_type     cmds;

  /* This is a CHOICE with 2 possible cmds */
  cmds.num_of_tlv_types    = 2;
  cmds.tlv_type[0]         = GET_EUICC_INFO1;
  cmds.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC32_SECOND;
  cmds.tlv_type[1]         = GET_EUICC_INFO2;
  cmds.tlv_tag[1].tag_len  = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[1].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[1].tag[1]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC34_SECOND;

  if(NULL == resp_ptr ||
     NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   &current_tlv_type,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  if(GET_EUICC_INFO1 == current_tlv_type)
  {
    /* This is an eUICCInfo1 */
    resp_ptr->euicc_info1.data_len = data_len;
    resp_ptr->euicc_info1.data_ptr = LPA_MALLOC(data_len);
    if(resp_ptr->euicc_info1.data_ptr == NULL)
    {
      return LPA_ERROR_HEAP_EXHAUSTED;
    }
    memscpy(resp_ptr->euicc_info1.data_ptr,
            resp_ptr->euicc_info1.data_len,
            data_ptr,
            data_len);
  }
  else
  {
    /* This is an eUICCInfo2 */
    resp_ptr->euicc_info2.data_len = data_len;
    resp_ptr->euicc_info2.data_ptr = LPA_MALLOC(data_len);
    if(resp_ptr->euicc_info2.data_ptr == NULL)
    {
      return LPA_ERROR_HEAP_EXHAUSTED;
    }
    memscpy(resp_ptr->euicc_info2.data_ptr,
            resp_ptr->euicc_info2.data_len,
            data_ptr,
            data_len);
    /* Parse profileVersion and skip */
    cmds.num_of_tlv_types    = 1;
    cmds.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
    cmds.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1;

    if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                      data_ptr,
                                                      offset,
                                                      cmds,
                                                      LPA_SVN_LEN,
                                                      LPA_SVN_LEN,
                                                      &offset,
                                                      NULL,
                                                      NULL,
                                                      FALSE))
    {
      return LPA_GENERIC_ERROR;
    }
    offset += LPA_SVN_LEN;
  }
  /* Extract the svn number */
  cmds.num_of_tlv_types    = 1;
  cmds.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC2;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   LPA_SVN_LEN,
                                                   LPA_SVN_LEN,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  memscpy(&resp_ptr->svn,
          LPA_SVN_LEN,
          &data_ptr[offset],
          LPA_SVN_LEN);

  return LPA_SUCCESS;
} /*lpa_asn1_decode_get_euicc_challenge_resp*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_GET_EID_RESP

DESCRIPTION
  Function parses the ASN formatted APDU response received as a result of
  Get Profiles Info request. This function might MALLOC
  resp_ptr->.profile_info[p].icon.data_ptr
  (where p ranges from 0 to LPA_NUM_PROFILES_MAX) which should be freed by
  the caller.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_get_eid_resp (
  uint32                                 data_len,
  uint8                                 *data_ptr,
  lpa_get_eid_resp_type                 *resp_ptr
)
{
  uint32                offset          = 0;
  lpa_asn1_cmds_type    cmds;

  memset(&cmds, 0x00, sizeof(lpa_asn1_cmds_type));

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC62_SECOND;

  if(NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN +
                                                   LPA_EID_LEN,
                                                   LPA_ASN1_TLV_SINGLE_TAG_MIN_LEN +
                                                   LPA_EID_LEN,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_APPLICATION26;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   LPA_EID_LEN,
                                                   LPA_EID_LEN,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  memscpy((uint8 *)(resp_ptr->eid),
          LPA_EID_LEN,
          (const uint8 *)(data_ptr + offset),
          LPA_EID_LEN);

  return LPA_SUCCESS;
} /*lpa_asn1_decode_get_eid_resp*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_GET_RAT_RESP

DESCRIPTION
  Function parses the ASN formatted APDU response received as a result of
  Get Profiles Info request. This function might MALLOC
  resp_ptr->.profile_info[p].icon.data_ptr
  (where p ranges from 0 to LPA_NUM_PROFILES_MAX) which should be freed by
  the caller.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_get_rat_resp (
  uint32                                 data_len,
  uint8                                 *data_ptr,
  lpa_get_rat_resp_type                 *resp_ptr
)
{
  uint32              offset                   = 0;
  uint32              first_ppar_entry_offset  = 0;
  uint32              current_ppr              = 0;
  uint32              current_tlv_len          = LPA_ASN1_LEN_ZERO;
  lpa_asn1_cmds_type  cmds;
  lpa_asn1_cmds_type  cmds1;

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC67_SECOND;

  if(NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  if (current_tlv_len == LPA_ASN1_LEN_ZERO)
  {
    return LPA_GENERIC_ERROR;
  }

  /* SEQUENCE OF tag A0 */
  current_tlv_len = LPA_ASN1_LEN_ZERO;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0_CONSTRUCTED;
  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  if (current_tlv_len == LPA_ASN1_LEN_ZERO)
  {
    /* GETRAT PPAR list is empty, which is perfectly valid */
    LPA_MSG_HIGH_0("GETRAT PPAR list is empty, which is perfectly valid");
    return LPA_SUCCESS;
  }

  /* Get the number of rules. Parse ProfilePolicyAuthorisationRule TLVs,
     if we have we have 30 tag byte */
  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_SEQUENCE;

  /* Remember the offset for the first PPR */
  resp_ptr->rules_auth_table.num_ppar_entries = 0;
  first_ppar_entry_offset                     = offset;

  while(data_len > offset)
  {
    current_tlv_len = LPA_ASN1_LEN_ZERO;
    if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                     data_ptr,
                                                     offset,
                                                     cmds,
                                                     0,
                                                     0,
                                                     &offset,
                                                     &current_tlv_len,
                                                     NULL,
                                                     FALSE))
    {
      return LPA_GENERIC_ERROR;
    }
    resp_ptr->rules_auth_table.num_ppar_entries++;

    if (0x31 < resp_ptr->rules_auth_table.num_ppar_entries)
    {
      /* To prevent uint8 overflow */
      return LPA_GENERIC_ERROR;
    }
    offset += current_tlv_len;
  }

  /* Allocate memory for auth_table */
  resp_ptr->rules_auth_table.ppar_entry_ptr =
     LPA_MALLOC(resp_ptr->rules_auth_table.num_ppar_entries *
                sizeof(lpa_ppar_table_entry_type));
  if(NULL == resp_ptr->rules_auth_table.ppar_entry_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate auth_table, one entry (starting with tag 30) at a time */
  offset = first_ppar_entry_offset;
  while(data_len > offset)
  {
    current_tlv_len = LPA_ASN1_LEN_ZERO;
    cmds.num_of_tlv_types   = 1;
    cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
    cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_SEQUENCE;
    if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                     data_ptr,
                                                     offset,
                                                     cmds,
                                                     0,
                                                     0,
                                                     &offset,
                                                     &current_tlv_len,
                                                     NULL,
                                                     FALSE))
    {
      return LPA_GENERIC_ERROR;
    }

    /* Extract pprIds */
    if(LPA_SUCCESS != lpa_asn1_util_decode_ppr_ids(data_len,
                                                   data_ptr,
                                                   offset,
                                                   &offset,
                                                   &resp_ptr->rules_auth_table.ppar_entry_ptr[current_ppr].ppr_ids,
                                                   FALSE))
    {
      return LPA_GENERIC_ERROR;
    }

    LPA_MSG_MED_3("current_ppr=0x%x, PPR1=0x%x, PPR2=0x%x", current_ppr,
                   resp_ptr->rules_auth_table.ppar_entry_ptr[current_ppr].ppr_ids.ppr1,
                   resp_ptr->rules_auth_table.ppar_entry_ptr[current_ppr].ppr_ids.ppr2);
    /* Extract pprIds */
    if(LPA_SUCCESS != lpa_asn1_util_decode_operator_ids(data_len,
                                                        data_ptr,
                                                        offset,
                                                        &offset,
                                                        &resp_ptr->rules_auth_table.ppar_entry_ptr[current_ppr]))
    {
      return LPA_GENERIC_ERROR;
    }

    LPA_MSG_MED_3("MNC[0]=0x%x, MNC[1]=0x%x, MNC[2]=0x%x",
                   resp_ptr->rules_auth_table.ppar_entry_ptr[current_ppr].operator_ids[0].mnc.mnc_data[0],
                   resp_ptr->rules_auth_table.ppar_entry_ptr[current_ppr].operator_ids[0].mnc.mnc_data[1],
                   resp_ptr->rules_auth_table.ppar_entry_ptr[current_ppr].operator_ids[0].mnc.mnc_data[2]);
    LPA_MSG_MED_3("MCC[0]=0x%x, MCC[1]=0x%x, MCC[2]=0x%x",
                   resp_ptr->rules_auth_table.ppar_entry_ptr[current_ppr].operator_ids[0].mcc[0],
                   resp_ptr->rules_auth_table.ppar_entry_ptr[current_ppr].operator_ids[0].mcc[1],
                   resp_ptr->rules_auth_table.ppar_entry_ptr[current_ppr].operator_ids[0].mcc[2]);

    /* Extract pprFlags */
    cmds1.num_of_tlv_types    = 1;
    cmds1.tlv_type[0]         = INTERNAL_BIT_STRING;
    cmds1.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
    cmds1.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC2;

    if(data_len <= offset ||
       LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                     data_ptr,
                                                     offset,
                                                     cmds1,
                                                     0,
                                                     0,
                                                     &offset,
                                                     &current_tlv_len,
                                                     NULL,
                                                     FALSE) ||
      (current_tlv_len != LPA_ASN1_ONE_BYTE_BIT_STRING &&
       current_tlv_len != LPA_ASN1_TWO_BYTE_BIT_STRING))
    {
      return LPA_GENERIC_ERROR;
    }

    if (current_tlv_len == LPA_ASN1_ONE_BYTE_BIT_STRING)
    {
      resp_ptr->rules_auth_table.ppar_entry_ptr[current_ppr].user_consent_required = FALSE;
    }
    else
    {
    resp_ptr->rules_auth_table.ppar_entry_ptr[current_ppr].user_consent_required =
        (data_ptr[offset + 1] & LPA_ASN1_USER_CONSENT_REQUIRED_MASK) ? TRUE : FALSE;
    }
    current_ppr++;
    offset += current_tlv_len;
  }

  return LPA_SUCCESS;
} /*lpa_asn1_decode_get_rat_resp*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_SELECT_ISDR_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  select ISDR and decodes the SVN and lpae related info.
DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_select_isdr_resp (
  uint32                                 data_len,
  uint8                                 *data_ptr,
  lpa_mmgsdi_select_isdr_resp_type      *resp_ptr
)
{
  uint32                 offset           = 0;
  uint32                 current_tlv_len  = 0;
  lpa_asn1_tlv_type_type current_tlv_type = UNKNOWN_TLV_TYPE;
  lpa_asn1_cmds_type     cmds;

  if (NULL == data_ptr || NULL == resp_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  memset(&cmds, 0x00, sizeof(lpa_asn1_cmds_type));
  
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_TAG_SELECT_ISDR;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    resp_ptr->svn[0] = 2;
    resp_ptr->svn[1] = 0;
    resp_ptr->svn[2] = 0;
    return LPA_SUCCESS;
  }

  /* This is a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 2;
  cmds.tlv_type[0]        = INTERNAL_PRIVATE0;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_PRIVATE0;
  cmds.tlv_type[1]        = ANY_TLV_TYPE;
  cmds.tlv_tag[1].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[1].tag[0]  = LPA_ASN1_TAG_ANY_LOW_TAG_NUMBER_FORM;

  while(offset < data_len)
  {
    if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                     data_ptr,
                                                     offset,
                                                     cmds,
                                                     0,
                                                     0,
                                                     &offset,
                                                     &current_tlv_len,
                                                     &current_tlv_type,
                                                     TRUE))
    {
      return LPA_GENERIC_ERROR;
    }

    if(ANY_TLV_TYPE == current_tlv_type)
    {
      /* Found some TLV that we don't care,
         skip the value field*/
      offset += current_tlv_len;
    }
    else if(INTERNAL_PRIVATE0 == current_tlv_type)
    {
      /* Found ISDRProprietaryApplicationTemplate,
         proceed */
      break;
    }
    else
    {
      /* This should never happen */
      return LPA_GENERIC_ERROR;
    }
  }

  if(offset >= data_len)
  {
    /* Didn't find ISDRProprietaryApplicationTemplate
       in the ISDR response. Assume this is a non-phase2
       card, and set svn to phase 1 for now */
    resp_ptr->svn[0] = 1;
    resp_ptr->svn[1] = 1;
    resp_ptr->svn[2] = 0;

    return LPA_SUCCESS;
  }

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC2;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   LPA_SVN_LEN,
                                                   LPA_SVN_LEN,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  memscpy((uint8 *)(resp_ptr->svn),
          LPA_SVN_LEN,
          (const uint8 *)(data_ptr + offset),
          LPA_SVN_LEN);

  offset += LPA_SVN_LEN;
  if (offset >= data_len)
  {
    /* LPAe support TLV is not present. It is anyways optional so dont
       return error here */
    return LPA_SUCCESS;
  }

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_type[0]        = INTERNAL_BIT_STRING;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_BIT_STRING;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   LPA_ASN1_LPAE_SUPPORT_LEN,
                                                   LPA_ASN1_LPAE_SUPPORT_LEN,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   TRUE))
  {
    return LPA_GENERIC_ERROR;
  }
  if(INTERNAL_BIT_STRING == current_tlv_type)
  {
    /* lpaeSupport present */
    resp_ptr->lpa_e_using_cat  = ((data_ptr[offset] & LPA_ASN1_LPAE_USING_CAT_MASK) > 0);
    resp_ptr->lpa_e_using_scws = ((data_ptr[offset] & LPA_ASN1_LPAE_USING_SCWS_MASK)> 0);
  }

  return LPA_SUCCESS;
} /*lpa_asn1_decode_select_isdr_resp*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_EUICC_CONFIG_ADDRESS_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  get or set server address (configured on the eUICC) request and decodes:
  - defaultDpAddress(optional) and rootDsAddress if it is a GET request
  - Result if it is a SET request
DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_euicc_config_address_resp (
  uint32                                           data_len,
  uint8                                           *data_ptr,
  lpa_default_server_address_resp_type            *resp_ptr
)
{
  uint32                offset                        = 0;
  uint32                current_tlv_len               = LPA_ASN1_LEN_ZERO;
  lpa_asn1_cmds_type    cmds;

  if(data_len == 0 ||
     data_ptr == NULL ||
     resp_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Check if it is a SET request */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC63_SECOND; //Set request tag
  if (LPA_SUCCESS == lpa_asn1_decode_util_parse_tlv(data_len,
                                                    data_ptr,
                                                    offset,
                                                    cmds,
                                                    0,
                                                    0,
                                                    &offset,
                                                    NULL,
                                                    NULL,
                                                    FALSE))
  {
    /* This was a response for the SET request... the response only contains the
       Result and nothing else */
    return lpa_asn1_decode_util_get_action_result_code_enum(data_len - offset,
                                                            data_ptr + offset,
                                                            GET_EUICC_CONFIGURED_ADDRESS,
                                                            TRUE);
  }

  /* It is not a SET request... lets reset the offset to the begging and start
     decoding according to the ASN1 encoding of the GET request's response */
  offset = 0;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC60_SECOND; //Get request tag
  if (LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                    data_ptr,
                                                    offset,
                                                    cmds,
                                                    0,
                                                    0,
                                                    &offset,
                                                    NULL,
                                                    NULL,
                                                    FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  /* Extract optional default SM-DP+ address */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0;
  if (LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                    data_ptr,
                                                    offset,
                                                    cmds,
                                                    0,
                                                    0,
                                                    &offset,
                                                    &current_tlv_len,
                                                    NULL,
                                                    TRUE))
  {
    return LPA_GENERIC_ERROR;
  }

  if(LPA_ASN1_LEN_ZERO != current_tlv_len)
  {
    resp_ptr->default_smdp_address.data_ptr = LPA_MALLOC(current_tlv_len);
    if(resp_ptr->default_smdp_address.data_ptr == NULL)
    {
      return LPA_ERROR_HEAP_EXHAUSTED;
    }
    resp_ptr->default_smdp_address.data_len = current_tlv_len;
    memscpy(resp_ptr->default_smdp_address.data_ptr,
            resp_ptr->default_smdp_address.data_len,
            (const uint8 *)(data_ptr + offset),
            current_tlv_len);
  }

  /* Now lets try to extract the mandatory Root SM-DS address.
     Even if the SM-DP+ address TLV (first optional context specific TLV)
     is not present, SM-DS TLV shall always be present with tag 0x81.
     So, lets try to find out if TLV with tag 0x81 is present or not. If the
     TLV is not present, it is an error as the SM-DS TLV is mandatory.
     It's ok to have its length as zero. */
  offset += current_tlv_len;

  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1;

  current_tlv_len = LPA_ASN1_LEN_ZERO;
  if (LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                    data_ptr,
                                                    offset,
                                                    cmds,
                                                    0,
                                                    0,
                                                    &offset,
                                                    &current_tlv_len,
                                                    NULL,
                                                    FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  if(LPA_ASN1_LEN_ZERO != current_tlv_len)
  {
    resp_ptr->default_smds_address.data_ptr = LPA_MALLOC(current_tlv_len);
    if(resp_ptr->default_smds_address.data_ptr == NULL)
    {
      LPA_FREE(resp_ptr->default_smdp_address.data_ptr);
      return LPA_ERROR_HEAP_EXHAUSTED;
    }
    resp_ptr->default_smds_address.data_len = current_tlv_len;
    memscpy(resp_ptr->default_smds_address.data_ptr,
            resp_ptr->default_smds_address.data_len,
            (const uint8 *)(data_ptr + offset),
            current_tlv_len);
  }

  return LPA_SUCCESS;
} /*lpa_asn1_decode_euicc_config_address_resp*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_RETRIEVE_NOTIFICATION_LIST_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  retrieve notification list request response

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_retrieve_notification_list_resp (
   uint32                                                  data_len,
   uint8                                                   *data_ptr,
   lpa_notification_retrieve_notification_list_resp_type   *resp_ptr
)
{
  uint32                 offset                         = 0;
  uint32                 install_result_data_offset     = 0;
  uint32                 first_notification_offset      = 0;
  uint32                 current_tlv_len                = 0;
  uint32                 current_notification_index     = 0;
  uint32                 pending_notification_offset    = 0;
  lpa_asn1_tlv_type_type current_tlv_type               = UNKNOWN_TLV_TYPE;
  lpa_asn1_cmds_type     cmds;
  lpa_asn1_cmds_type     cmds1;

  memset(&cmds1, 0x00, sizeof(cmds1));
  memset(&cmds, 0x00, sizeof(cmds));

  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC43_SECOND;

  if(NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    LPA_MSG_ERR_1("lpa_asn1_decode_retrieve_notification_list_resp returned=0x%x", LPA_GENERIC_ERROR);
    return LPA_GENERIC_ERROR;
  }

  /* This is a CHOICE with 2 possible cmds */
  cmds.num_of_tlv_types    = 2;
  cmds.tlv_type[0]         = INTERNAL_SEQUENCE;
  cmds.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0_CONSTRUCTED;
  cmds.tlv_type[1]         = INTERNAL_INT;
  cmds.tlv_tag[1].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[1].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1;

  if(NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   &current_tlv_type,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  if(INTERNAL_INT == current_tlv_type)
  {
    /* This is an notificationsListResultError, return the error code */
    return lpa_asn1_decode_util_get_action_result_code_enum(data_len - offset,
                                                            data_ptr + offset,
                                                            RETRIEVE_NOTIFICATION_LIST,
                                                            FALSE);
  }
  else
  {
    /* SEQUENCE OF PendingNotification */
    /* This is a CHOICE with 2 possible cmds */
    cmds.num_of_tlv_types    = 2;
    cmds.tlv_type[0]         = PROFILE_INSTALLATION_RESULT;               /* profileInstallationResult */
    cmds.tlv_tag[0].tag_len  = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
    cmds.tlv_tag[0].tag[0]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
    cmds.tlv_tag[0].tag[1]   = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC55_SECOND;
    cmds.tlv_type[1]         = INTERNAL_SEQUENCE;                         /* otherSignedNotification */
    cmds.tlv_tag[1].tag_len  = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
    cmds.tlv_tag[1].tag[0]   = LPA_ASN1_GEN_TAG_SEQUENCE;

    /* Reset notification list */
    resp_ptr->num_notifications               = 0;
    first_notification_offset                 = offset;
    /* Count how many notifications */
    while(data_len > offset)
    {
      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds,
                                                       0,
                                                       0,
                                                       &offset,
                                                       &current_tlv_len,
                                                       &current_tlv_type,
                                                       FALSE))
      {
        return LPA_GENERIC_ERROR;
      }
      resp_ptr->num_notifications++;
      if(0xFF <= resp_ptr->num_notifications)
      {
        /* To prevent uint8 overflow */
        return LPA_GENERIC_ERROR;
      }
      offset += current_tlv_len;
    }
    /* Allocate memory for notification_list */
    resp_ptr->notification_list_ptr =
      LPA_MALLOC(resp_ptr->num_notifications * sizeof(lpa_notification_type));
    if(NULL == resp_ptr->notification_list_ptr)
    {
      return LPA_ERROR_HEAP_EXHAUSTED;
    }

    /* Populate notification_list */
    offset = first_notification_offset;
    while(data_len > offset &&
          current_notification_index < resp_ptr->num_notifications)
    {
      pending_notification_offset = offset;
      if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                       data_ptr,
                                                       offset,
                                                       cmds,
                                                       0,
                                                       0,
                                                       &offset,
                                                       &current_tlv_len,
                                                       &current_tlv_type,
                                                       FALSE))
      {
        return LPA_GENERIC_ERROR;
      }
      /* Parse NotificationMetadata. This function will only extract data without moving the offset */
      if(INTERNAL_SEQUENCE == current_tlv_type)
      {
        resp_ptr->notification_list_ptr[current_notification_index].notification_type = LPA_NOTIFICATION_OTHER_SIGNED_NOTIFICATION;
        resp_ptr->notification_list_ptr[current_notification_index].data.other_signed_notification.data_len =
                  current_tlv_len + (offset - pending_notification_offset);
        /* Allocate memory for PendingNotification othernotification This should change!!! */
        resp_ptr->notification_list_ptr[current_notification_index].data.other_signed_notification.data_ptr =
           LPA_MALLOC(resp_ptr->notification_list_ptr[current_notification_index].data.other_signed_notification.data_len);
        if(NULL == resp_ptr->notification_list_ptr[current_notification_index].data.other_signed_notification.data_ptr)
        {
          return LPA_ERROR_HEAP_EXHAUSTED;
        }
        memscpy(resp_ptr->notification_list_ptr[current_notification_index].data.other_signed_notification.data_ptr,
                resp_ptr->notification_list_ptr[current_notification_index].data.other_signed_notification.data_len,
                &data_ptr[pending_notification_offset],
                resp_ptr->notification_list_ptr[current_notification_index].data.other_signed_notification.data_len);

        if(LPA_SUCCESS != lpa_asn1_util_decode_notification_metadata(data_len,
                                                                     data_ptr,
                                                                     offset,
                                                                    &resp_ptr->notification_list_ptr[current_notification_index]))
        {
          return LPA_GENERIC_ERROR;
        }
      }
      else if(PROFILE_INSTALLATION_RESULT == current_tlv_type)
      {
        resp_ptr->notification_list_ptr[current_notification_index].notification_type = LPA_NOTIFICATION_PROFILE_INSTALLATION_RESULT;
        resp_ptr->notification_list_ptr[current_notification_index].data.profile_installation_result.data_len =
                current_tlv_len + (offset - pending_notification_offset);
        /* Allocate memory for profileInstallationResult */
        resp_ptr->notification_list_ptr[current_notification_index].data.profile_installation_result.data_ptr =
           LPA_MALLOC(resp_ptr->notification_list_ptr[current_notification_index].data.profile_installation_result.data_len);
        if(NULL == resp_ptr->notification_list_ptr[current_notification_index].data.profile_installation_result.data_ptr)
        {
          return LPA_ERROR_HEAP_EXHAUSTED;
        }
        memscpy(resp_ptr->notification_list_ptr[current_notification_index].data.profile_installation_result.data_ptr,
                resp_ptr->notification_list_ptr[current_notification_index].data.profile_installation_result.data_len,
                &data_ptr[pending_notification_offset],
                resp_ptr->notification_list_ptr[current_notification_index].data.profile_installation_result.data_len);

        cmds1.num_of_tlv_types   = 1;
        cmds1.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
        cmds1.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
        cmds1.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC39_SECOND;
        if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                         data_ptr,
                                                         offset,
                                                         cmds1,
                                                         0,
                                                         0,
                                                         &install_result_data_offset,
                                                         NULL,
                                                         NULL,
                                                         FALSE))
        {
          return LPA_GENERIC_ERROR;
        }
        if(LPA_SUCCESS != lpa_asn1_util_decode_notification_metadata(data_len,
                                                                     data_ptr,
                                                                     install_result_data_offset,
                                                                     &resp_ptr->notification_list_ptr[current_notification_index]))
        {
          return LPA_GENERIC_ERROR;
        }
      }
      offset += current_tlv_len;
      current_notification_index++;
    }
  }

  return LPA_SUCCESS;
} /*lpa_asn1_decode_retrieve_notification_list_resp*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_PROFILE_METADATA

DESCRIPTION
  Function parses the ASN1 formatted response received as a result of
  ES9+ES9+.InitiateAuthentication JSON request. it returns operator_id
  and ppr_ids in profileMetadata.

DEPENDENCIES

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_profile_metadata (
   uint32                                              data_len,
   uint8                                               *data_ptr,
   lpa_operator_id_type                                *operator_id_ptr,
   lpa_ppr_ids_type                                    *ppr_ids_ptr
)
{
  uint32                offset                        = 0;
  uint32                current_tlv_len               = 0;
  lpa_asn1_cmds_type    cmds;

  memset(&cmds, 0x00, sizeof(cmds));

  /* Parse StoreMetadataRequest */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC37_SECOND;

  if(NULL == data_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  /* Parse iccid */
  current_tlv_len = LPA_ASN1_LEN_ZERO;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0] = LPA_ASN1_GEN_TAG_APPLICATION26;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len;

  /* Parse serviceProviderName */
  current_tlv_len = LPA_ASN1_LEN_ZERO;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0] = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC17;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len;

  /* Parse profileName  */
  current_tlv_len = LPA_ASN1_LEN_ZERO;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC18;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len;

  /* Parse iconType */
  current_tlv_len = LPA_ASN1_LEN_ZERO;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC19;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   TRUE))
  {
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len;
  if (offset >= data_len)
  {
    /* No more data, and no manditory field afterwards */
    return LPA_SUCCESS;
  }

  /* Parse icon */
  current_tlv_len = LPA_ASN1_LEN_ZERO;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC20;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   TRUE))
  {
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len;
  if(offset >= data_len)
  {
    /* No more data, and no manditory field afterwards */
    return LPA_SUCCESS;
  }

  /* Parse profileClass */
  current_tlv_len = LPA_ASN1_LEN_ZERO;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC21;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   TRUE))
  {
    return LPA_GENERIC_ERROR;
  }
  offset += current_tlv_len;
  if (offset >= data_len)
  {
    /* No more data, and no manditory field afterwards */
    return LPA_SUCCESS;
  }

  /* Parse OPTIONAL notificationConfigurationInfo  */
  current_tlv_len = LPA_ASN1_LEN_ZERO;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC22_CONSTRUCTED;

  while( data_len > offset )
  {
    current_tlv_len = LPA_ASN1_LEN_ZERO;
    if(NULL == data_ptr ||
       LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                     data_ptr,
                                                     offset,
                                                     cmds,
                                                     0,
                                                     0,
                                                     &offset,
                                                     &current_tlv_len,
                                                     NULL,
                                                     TRUE))
    {
      return LPA_GENERIC_ERROR;
    }
    if(LPA_ASN1_LEN_ZERO == current_tlv_len)
    {
      /* Didn't find the notificationConfigurationInfo tag or
         notificationConfigurationInfo len is 0. We use this
         as an indication that we finished processing
         notificationConfigurationInfo*/
      break;
    }
    offset += current_tlv_len;
  }
  if(offset >= data_len)
  {
    /* No more data, and no manditory field afterwards */
    return LPA_SUCCESS;
  }

  /* Extract profileOwner(OperatorId) */
  if(LPA_SUCCESS != lpa_asn1_util_decode_operator_id(data_len,
                                                      data_ptr,
                                                      offset,
                                                      &offset,
                                                      operator_id_ptr,
                                                      TRUE))
  {
    return LPA_GENERIC_ERROR;
  }
  if (offset >= data_len)
  {
    /* No more data, and no manditory field afterwards */
    return LPA_SUCCESS;
  }

  /* Extract profilePolicyRules (PprIds) */
  if(LPA_SUCCESS != lpa_asn1_util_decode_ppr_ids(data_len,
                                                 data_ptr,
                                                 offset,
                                                 &offset,
                                                 ppr_ids_ptr,
                                                 TRUE))
  {
    return LPA_GENERIC_ERROR;
  }

  return LPA_SUCCESS;
}/*lpa_asn1_decode_profile_metadata*/


/*===========================================================================
FUNCTION LPA_ASN1_DECODE_BPP

DESCRIPTION
  Function parses the ASN1 formatted BoundProfilePackage .

DEPENDENCIES

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_bpp (
  uint32                                            data_len,
  uint8                                             *data_ptr,
  lpa_bpp_tlvs_data_type                            *bpp_tlv_ptr,
  uint8                                             *meta_data_start_index_ptr,
  uint8                                             *meta_data_total_index_ptr
)
{
  uint32                 offset                        = 0;
  uint32                 current_tlv_len               = 0;
  uint32                 segment_start_index           = 0;
  uint8                  i                             = 0;
  boolean                is_optional                   = FALSE;
  lpa_asn1_tlv_type_type current_tlv_type              = UNKNOWN_TLV_TYPE;
  lpa_asn1_cmds_type     cmds;

  /* Parse BF36 BoundProfilePackage */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC54_SECOND;

  if(NULL == data_ptr    ||
     NULL == bpp_tlv_ptr ||
     LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }

  /* reset bpp_tlv_ptr */
  bpp_tlv_ptr->tlvs_num = 0;

  /* Parse BF23 initialiseSecureChannelRequest */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC35_SECOND;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }
  /* Copy tag and length fields of the BoundProfilePackage
     TLV plus the initialiseSecureChannelRequest TLV */
  /* Using the offset as segment len is only applicable
     to the first segment */
  bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len = offset + current_tlv_len;
  if(LPA_ASN1_BPP_SEGMENT_MAX_LEN < bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len)
  {
    return LPA_GENERIC_ERROR;
  }
  bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr =
    LPA_MALLOC(bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
  if(NULL == bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  (void)memscpy((void*)bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr,
                bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len,
                (void*)data_ptr,
                bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
  offset += current_tlv_len;
  bpp_tlv_ptr->tlvs_num++;

  /* Parse A0 */
  segment_start_index     = offset;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC0_CONSTRUCTED;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }
  /* Copy tag and length fields of the first sequenceOf87 TLV
     plus the first '87' TLV. Since we know there will be
     only 1 87 TLV per spec, we are going to have A0 TLV
     as the 2nd segment */
  bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len = offset + current_tlv_len - segment_start_index;
  if(LPA_ASN1_BPP_SEGMENT_MAX_LEN < bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len)
  {
    return LPA_GENERIC_ERROR;
  }
  bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr = LPA_MALLOC(bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
  if(NULL == bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  (void)memscpy((void*)bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr,
                bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len,
                (void*)(data_ptr + segment_start_index),
                bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
  offset += current_tlv_len;
  bpp_tlv_ptr->tlvs_num++;

  /* Parse A1 */
  segment_start_index     = offset;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC1_CONSTRUCTED;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }
  /* Copy the tag and length fields of the sequenceOf88 TLV */
  bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len = offset - segment_start_index;
  if(LPA_ASN1_BPP_SEGMENT_MAX_LEN < bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len)
  {
    return LPA_GENERIC_ERROR;
  }
  bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr =
    LPA_MALLOC(bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
  if(NULL == bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  (void)memscpy((void*)bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr,
                bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len,
                (void*)(data_ptr + segment_start_index),
                bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
  bpp_tlv_ptr->tlvs_num++;

  if(meta_data_start_index_ptr != NULL && meta_data_total_index_ptr != NULL)
  {
    *meta_data_start_index_ptr = bpp_tlv_ptr->tlvs_num;
    *meta_data_total_index_ptr = 0;
  }

  /* Parse A1 - 88 TLVs
     This SCP03t segment containing StoreMetadata, MAC protected with session
     keys resulting from the key agreement (S-CMAC).
     Content: TLV for "ES8+.StoreMetadata" function*/
  is_optional             = FALSE;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_type[0]        = INTERNAL_CONTEXT_SPECIFIC8;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC8;
  for (i = 0; i < LPA_ASN1_MAX_NUM_BPP_88_TLVS; i++)
  {
    segment_start_index     = offset;
    if(0 < i)
    {
      /* The TLVs after the first 88 TLV are conditional */
      is_optional = TRUE;
    }

    if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                     data_ptr,
                                                     offset,
                                                     cmds,
                                                     0,
                                                     0,
                                                     &offset,
                                                     &current_tlv_len,
                                                     &current_tlv_type,
                                                     is_optional))
    {
      return LPA_GENERIC_ERROR;
    }

    if(INTERNAL_CONTEXT_SPECIFIC8 == current_tlv_type)
    {
      /* Copy the Each of the '88' TLVs */
      bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len = offset +
                                                               current_tlv_len -
                                                               segment_start_index;
      if(LPA_ASN1_BPP_SEGMENT_MAX_LEN <
         bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len)
      {
        return LPA_GENERIC_ERROR;
      }

      /* SCP03t segment containing the remainder of StoreMetadata if one '88'
         TLV is not able to contain the whole data structure.  */
      bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr =
        LPA_MALLOC(bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
      if(NULL == bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr)
      {
        return LPA_ERROR_HEAP_EXHAUSTED;
      }
      (void)memscpy((void*)bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr,
                    bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len,
                    (void*)(data_ptr + segment_start_index),
                    bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
      offset += current_tlv_len;
      bpp_tlv_ptr->tlvs_num++;

      if(meta_data_total_index_ptr != NULL)
      {
        *meta_data_total_index_ptr += 1;
      }
    }
  }

  /* Parse Conditional A2 */
  current_tlv_len = LPA_ASN1_LEN_ZERO;
  segment_start_index     = offset;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC2_CONSTRUCTED;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   TRUE))
  {
    return LPA_GENERIC_ERROR;
  }

  if(LPA_ASN1_LEN_ZERO != current_tlv_len)
  {
    /* Copy tag and length fields of the second sequenceOf87 TLV
       plus the first '87' TLV. */
    bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len = offset +
                                                             current_tlv_len -
                                                             segment_start_index;
    if(LPA_ASN1_BPP_SEGMENT_MAX_LEN <
       bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len)
    {
      return LPA_GENERIC_ERROR;
    }
    bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr =
      LPA_MALLOC(bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
    if(NULL == bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr)
    {
      return LPA_ERROR_HEAP_EXHAUSTED;
    }
    (void)memscpy((void*)bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr,
                  bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len,
                  (void*)(data_ptr + segment_start_index),
                  bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
    offset += current_tlv_len;
    bpp_tlv_ptr->tlvs_num++;
  }

  /* Parse A3 */
  current_tlv_len         = LPA_ASN1_LEN_ZERO;
  segment_start_index     = offset;
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC3_CONSTRUCTED;

  if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   &current_tlv_len,
                                                   NULL,
                                                   FALSE))
  {
    return LPA_GENERIC_ERROR;
  }
  /* Copy the tag and length fields of the sequenceOf86 TLV */
  bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len = offset - segment_start_index;
  if(LPA_ASN1_BPP_SEGMENT_MAX_LEN < bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len)
  {
    return LPA_GENERIC_ERROR;
  }
  bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr =
    LPA_MALLOC(bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
  if(NULL == bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  (void)memscpy((void*)bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr,
                bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len,
                (void*)(data_ptr + segment_start_index),
                bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
  bpp_tlv_ptr->tlvs_num++;

  /* SequnceOf86 TLV is mandatory, hence return error if the
     lengh of A3 tag is 0 */
  if(current_tlv_len == LPA_ASN1_LEN_ZERO)
  {
    return LPA_GENERIC_ERROR;
  }

  cmds.num_of_tlv_types   = 1;
  cmds.tlv_type[0]        = INTERNAL_CONTEXT_SPECIFIC6;
  cmds.tlv_tag[0].tag_len = LPA_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = LPA_ASN1_GEN_TAG_CONTEXT_SPECIFIC6;
  /* Parse A3 - 86 TLVs */
  while(offset < data_len && LPA_UTIL_BPP_MAX_TLVS > bpp_tlv_ptr->tlvs_num)
  {
    segment_start_index     = offset;
    if(LPA_SUCCESS != lpa_asn1_decode_util_parse_tlv(data_len,
                                                     data_ptr,
                                                     offset,
                                                     cmds,
                                                     0,
                                                     0,
                                                     &offset,
                                                     &current_tlv_len,
                                                     &current_tlv_type,
                                                     TRUE))
    {
      return LPA_GENERIC_ERROR;
    }

    if(INTERNAL_CONTEXT_SPECIFIC6 == current_tlv_type)
    {
      /* Copy the Each of the '86' TLVs */
      bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len = offset +
                                                               current_tlv_len -
                                                               segment_start_index;
      if(LPA_ASN1_BPP_SEGMENT_MAX_LEN < bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len)
      {
        return LPA_GENERIC_ERROR;
      }
      bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr =
        LPA_MALLOC(bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
      if(NULL == bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr)
      {
        return LPA_ERROR_HEAP_EXHAUSTED;
      }
      (void)memscpy((void*)bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_ptr,
                    bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len,
                    (void*)(data_ptr + segment_start_index),
                    bpp_tlv_ptr->tlvs_data[bpp_tlv_ptr->tlvs_num].data_len);
      offset += current_tlv_len;
      bpp_tlv_ptr->tlvs_num++;
    }
    else
    {
      /* Didn't find the 86 TLV */
      break;
    }
  }

  /* Sanity check */
  if(data_len != offset)
  {
    return LPA_GENERIC_ERROR;
  }

  return LPA_SUCCESS;
}/* lpa_asn1_decode_bpp */

