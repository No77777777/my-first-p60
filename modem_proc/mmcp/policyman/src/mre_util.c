/**
  @file mre_util.c

  @brief Policy Manager utility functions.
*/

/*
    Copyright (c) 2013-2015,2017,2018 QUALCOMM Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Technologies Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this
  document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/mre_util.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_util_i.h"
#include "mre_dbg.h"
#include <atomic_ops.h>
#include "time_types.h"
#include "time_svc.h"
#include "modem_mem.h"
#include <stringl/stringl.h>
#include "sys_v.h"

#if defined(WIN32)
#define strcasecmp  _stricmp
#endif

#if defined(TEST_FRAMEWORK)
#error code not present
#endif /* +TEST_FRAMEWORK. */



/*=============================================================================
  Memory allocation APIs
=============================================================================*/

/*-------- mre_util_mem_alloc --------*/
#if defined(TEST_FRAMEWORK)
#error code not present
#else /* +TEST_FRAMEWORK- */
void * mre_util_mem_alloc(
  size_t       size,
  char const  *pFileName,
  size_t       lineNumber
  )
#endif /* -TEST_FRAMEWORK. */
{
  void  *pMem = modem_mem_alloc_ext( size,
                                     MODEM_MEM_CLIENT_MMODE,
                                     (char *) pFileName,
                                     lineNumber );

  if (pMem == NULL)
  {
    MRE_ERR_FATAL("Unable to allocate %d bytes at line %d", size, lineNumber, 0);
  }

#if defined(TEST_FRAMEWORK)
  #error code not present
#endif /* +TEST_FRAMEWORK. */

  memset(pMem, 0, size);

  return pMem;
}


/*-------- mre_util_mem_free --------*/
void mre_util_mem_free(
  void        *pMem,
  char const  *pFileName,
  size_t       lineNumber
  )
{
  modem_mem_free_ext(pMem, MODEM_MEM_CLIENT_MMODE, (char *) pFileName, lineNumber);

#if defined(TEST_FRAMEWORK)
  #error code not present
#endif /* +TEST_FRAMEWORK. */
}



/*-------- mre_simple_dtor --------*/
void mre_simple_dtor(
  void  *pObj
)
{
  mre_mem_free(pObj);
}



/*-------- mre_str_dup --------*/
char * mre_str_dup(
  char const  *pStr
)
{
  size_t   len;
  char    *pDup = NULL;

  if (pStr != NULL)
  {
    len = strlen(pStr) + 1;
    pDup = mre_mem_alloc(len);
    memscpy(pDup, len, pStr, len);
  }

  return pDup;
}



/*=============================================================================
  Tokenization APIs.
=============================================================================*/


static boolean mre_util_is_whitespace(
  char  c
)
{
  return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}


/*-------- NEXT_CHAR --------*/
/**
@brief  Return the next character in the string.
*/
#define NEXT_CHAR             **ppStr


/*-------- END_OF_STR --------*/
/**
@brief  Evaluates to whether the current pointer to the string is at the
        the end of the string.
*/
#define END_OF_STR            (NEXT_CHAR == '\0')


/*-------- ADVANCE --------*/
/**
@brief  Advance the string pointer by one character.
*/
#define ADVANCE               ++*ppStr



/*-------- mre_util_consume_whitespace --------*/
static void mre_util_consume_whitespace(
  char const  **ppStr
)
{
  while (!END_OF_STR && mre_util_is_whitespace(NEXT_CHAR))
  {
    ADVANCE;
  }
}


/*-------- mre_get_token_ex --------*/
boolean mre_get_token_ex(
  char const  **ppStr,
  char         *pToken,
  size_t       maxSize,
  char         terminator
)
{
  size_t  index = 0;

  mre_util_consume_whitespace(ppStr);

  while (
            index < maxSize - 1
         && !END_OF_STR
         && NEXT_CHAR != terminator
         && !mre_util_is_whitespace(NEXT_CHAR)
        )
  {
    pToken[index++] = NEXT_CHAR;
    ADVANCE;
  }

  pToken[index] = '\0';

  return index > 0;
}



/*=============================================================================
  Atomic APIs
=============================================================================*/

/*-------- mre_atomic_swap --------*/
uint32  mre_atomic_swap(
  uint32 volatile *pTarget,
  uint32           value
)
{
  uint32  old_value;
  
  do
  {
    old_value = *pTarget;
  } while (!atomic_compare_and_set((atomic_word_t *) pTarget, old_value, value));

  return old_value;
}



/*=============================================================================
  Other APIs
=============================================================================*/

/*-------- mre_util_msg_separator --------*/
void mre_util_msg_separator(
  mre_msg_separator_t which
)
{
  switch (which)
  {
    case MRE_SEPARATOR_HYPHEN:
      MRE_MSG_ERROR_0("---------------------------------------------");
      break;

    case MRE_SEPARATOR_STAR:
      MRE_MSG_ERROR_0("*********************************************");
      break;

    case MRE_SEPARATOR_UNDER:
      MRE_MSG_ERROR_0("_____________________________________________");
      break;

    case MRE_SEPARATOR_PLUS:
      MRE_MSG_ERROR_0("+++++++++++++++++++++++++++++++++++++++++++++");
      break;

    case MRE_SEPARATOR_EQUAL:
      MRE_MSG_ERROR_0("=============================================");
      break;

    default:
      break;
  }
}

/*-------- mre_util_get_time --------*/
uint32 mre_util_get_time(
  void
)
{
  time_type  time;    // Quadword

  time_get_ms(time);
  return qw_lo(time); // return the low 32-bits (ms resolution, about 1.5 days)
}


/*-------- mre_str_to_boolean --------*/
mre_status_t mre_str_to_boolean(
  char const  *pStr,
  boolean     *pBoolean
)
{
  mre_status_t  status = MRE_STATUS_ERR_MALFORMED_XML;

  if (pStr == NULL)
  {
    goto Done;
  }

  if      (strcasecmp(pStr, "true") == 0)
  {
    *pBoolean = TRUE;
  }
  else if (strcasecmp(pStr, "false") == 0)
  {
    *pBoolean = FALSE;
  }
  else
  {
    goto Done;
  }

  status = MRE_STATUS_SUCCESS;

Done:
  return status;
}


/*-------- mre_lookup_str_value --------*/
mre_status_t mre_lookup_str_value(
  mre_str_value_entry_t *pTable,
  size_t                 nEntries,
  char const            *pStr,
  uint32                *pValue
  )
{
  size_t        idx;
  mre_status_t  status = MRE_STATUS_ERR_NOT_PRESENT;

  for (idx = 0 ; idx < nEntries ; ++idx)
  {
    if (strcasecmp(pStr, pTable[idx].pStr) == 0)
    {
      *pValue = pTable[idx].value;
      status = MRE_STATUS_SUCCESS;
      break;
    }
  }

  return status;
}


/*-------- mre_lookup_str_value64 --------*/
mre_status_t mre_lookup_str_value64(
  mre_str_value64_entry_t *pTable,
  size_t                   nEntries,
  char const              *pStr,
  uint64                  *pValue
  )
{
  size_t        idx;
  mre_status_t  status = MRE_STATUS_ERR_NOT_PRESENT;

  for (idx = 0 ; idx < nEntries ; ++idx)
  {
    if (strcasecmp(pStr, pTable[idx].pStr) == 0)
    {
      *pValue = pTable[idx].value;
      status = MRE_STATUS_SUCCESS;
      break;
    }
  }

  return status;
}


/*-----------------------------------------------------------------------------
  PLMN functions
-----------------------------------------------------------------------------*/

/*-------- is_a_digit --------*/
STATIC boolean is_a_digit(
  char   c
)
{
  return (c >= '0' && c <= '9');
}

/*-------- plmn_validate_string --------*/
char *plmn_validate_string(
  char   const  *pPlmnStr,
  size_t        *pMncLen
)
{
  char   *pMncLoc = NULL;
  size_t  len     = 0;

  while (*pPlmnStr != 0)
  {
    // Validate digit
    if (is_a_digit(*pPlmnStr))
    {
      ++len;
    }
    // Digit valid, handle '-' or count decimal digit
    else if ((*pPlmnStr == '-') && pMncLoc == NULL && len == 3)
    {
      pMncLoc = ((char *)pPlmnStr) + 1;  // points to first mnc digit or NULL
      len = 0;                           // reset len to count mnc digits
    }
    else
    {
      // invalid digit for PLMN found
      pMncLoc = NULL;
      break;
    }

    ++pPlmnStr;
  }

  // Final validation and computing mnc length
  if (NULL != pMncLoc)
  {
    if (len == 2 || len == 3)
  {
      *pMncLen = len;
  }
  else
  {
      // invalid mnc length
      pMncLoc = NULL;
    }
  }

  return pMncLoc;
}


/*-------- mre_parse_ascii_plmn --------*/
boolean mre_parse_ascii_plmn(
  char const          *pPlmnStr,
  sys_plmn_id_s_type  *pPlmn
)
{
  boolean success = FALSE;
  char    *pMncLoc;
  size_t   mncLen;

  pMncLoc = plmn_validate_string(pPlmnStr, &mncLen);

  if (NULL != pMncLoc)
  {
    sys_mcc_type  mcc;
    sys_mnc_type  mnc;
    boolean       mnc_includes_pcs_digit;

    mcc = atoi(pPlmnStr);
    mnc = atoi(pMncLoc);
    mnc_includes_pcs_digit = (mncLen == 3);

    success = sys_plmn_set_mcc_mnc(mnc_includes_pcs_digit, mcc, mnc, pPlmn);
  }

  return success;
}


/*-------- mre_plmns_are_equal --------*/
boolean mre_plmns_are_equal(
  sys_plmn_id_s_type const *pPlmn1,
  sys_plmn_id_s_type const *pPlmn2
)
{
  return memcmp(pPlmn1->identity, pPlmn2->identity, sizeof(pPlmn1->identity)) == 0;
}


/*-------- mre_extract_plmn_from_imsi --------*/
void mre_extract_plmn_from_imsi(
  mmgsdi_data_type      imsiData,
  size_t                numMncDigits,
  sys_plmn_id_s_type   *pPlmn
)
{
  uint8 *pImsi;

  if (NULL != imsiData.data_ptr)
  {
    pImsi = (uint8 *)imsiData.data_ptr;

    pPlmn->identity[0] = (byte)(( ( pImsi[1] & 0xF0 ) / 0x10 ) +
                                ( ( pImsi[2] & 0x0F ) * 0x10 ));

    // Fill MCC Digit 3 and MNC Digit 3
    pPlmn->identity[1] = (byte)(( ( pImsi[2] & 0xF0 ) / 0x10 ) +
                                ( ( pImsi[4] & 0x0F ) * 0x10 ));

    // If 2 digits MNC is indicated in EF-AD then set MNC digit 3 to 'F'.
    if (numMncDigits == 2)
    {
      pPlmn->identity[1] |= 0xF0;
    }

    // Fill MNC Digit 1 and MNC Digit 2
    pPlmn->identity[2] = pImsi[3];
  }
}


/*-------- mre_plmn_get_mcc --------*/
uint32 mre_plmn_get_mcc(
  sys_plmn_id_s_type  *pPlmn
)
{
  byte  p0 = pPlmn->identity[0];
  byte  p1 = pPlmn->identity[1];

  return  (p0 & 0xf) * 100 + (p0 >> 4) * 10 + (p1 & 0x0f);
}


/*-----------------------------------------------------------------------------
  MCC lookup in PLMN list
-----------------------------------------------------------------------------*/

typedef struct
{
  sys_mcc_type mcc;
  boolean      mccFound;
} mcc_check_ctx_t;


/*-------- plmn_has_mcc --------*/
STATIC boolean plmn_has_mcc(
  void  *pElem,
  void  *pCtx
)
{
  sys_plmn_id_s_type *pPlmn         = (sys_plmn_id_s_type *) pElem;
  mcc_check_ctx_t    *pMccCheckCtx  = (mcc_check_ctx_t *) pCtx;
  sys_mcc_type        mcc;
  boolean             stopEval;
 
  mcc = mre_plmn_get_mcc(pPlmn);

  pMccCheckCtx->mccFound = (mcc == pMccCheckCtx->mcc);
  stopEval = !pMccCheckCtx->mccFound;

  return stopEval;
}


/*-------- mre_util_plmn_set_has_mcc --------*/
boolean mre_util_plmn_set_has_mcc(
  mre_set_t     *pPlmnSet,
  sys_mcc_type   mcc
)
{
  boolean          mccFound = FALSE;
  mcc_check_ctx_t  mcc_ctx;

  mcc_ctx.mcc      = mcc;
  mcc_ctx.mccFound = FALSE;

  (void)mre_set_iterate(pPlmnSet, plmn_has_mcc, &mcc_ctx);
  mccFound = mcc_ctx.mccFound;

  return mccFound;
}

