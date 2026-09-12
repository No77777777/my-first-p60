#ifndef _MRE_UTIL_H_
#define _MRE_UTIL_H_

/**
  @file mre_util.h

  @brief
*/

/*
    Copyright (c) 2017,2018 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/api/public/mre_util.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/


#include "comdef.h"
#include <sys/types.h>

#include "mre.h"


#define LOW_32(l)   ((uint32) ((l) & 0xffffffff))
#define HIGH_32(l)  ((uint32) ((l) >> 32))


/*=============================================================================
    Memory management functions.
=============================================================================*/

/*-------- mre_util_mem_alloc --------*/
/**
@brief  Allocate memory.  Private function - code should use use
        the mre_mem_alloc() macro in mre_util.h rather than use this directly.
*/
#if defined(TEST_FRAMEWORK)
#error code not present
#else /* +TEST_FRAMEWORK- */
void * mre_util_mem_alloc(
  size_t       size,
  char const  *pFileName,
  size_t       lineNumber
);
#endif /* -TEST_FRAMEWORK. */

/*-------- mre_util_mem_free --------*/
/**
@brief  Free memory.  Private function -  code should use the mre_mem_free()
        macro in mre_util.h rather than use this directly.
*/
void mre_util_mem_free(
  void        *pMem,
  char const  *pFileName,
  size_t       lineNumber
);


/*-------- mre_mem_alloc --------*/
/**
@brief  Allocate memory.

@param[in]  size  Size of memory buffer to allocate.

@return
  Pointer to zero-filled memory of the given size.

  This does not return if unable to allocate the requested amount of memory,
  but rather calls MRE_ERR_FATAL to print a message and exit.
*/
#if defined(TEST_FRAMEWORK)
#error code not present
#else /* +TEST_FRAMEWORK- */
#define mre_mem_alloc(size)  mre_util_mem_alloc((size), __FILENAME__, __LINE__)
#endif /* -TEST_FRAMEWORK. */


/*-------- mre_mem_free --------*/
/**
@brief  Free memory.

@param[in]  pMem        Pointer to memory allocated using mre_util_mem_alloc().

@return
  None
*/
#define mre_mem_free(pMem)   mre_util_mem_free((pMem), __FILENAME__, __LINE__)

#define MRE_MEM_FREE_IF(p)                                            \
    do {                                                              \
      void  *pObj;                                                    \
                                                                      \
      pObj = (void *) mre_atomic_swap((uint32 *) &(p), (uint32) NULL);\
      if (NULL != pObj)                                               \
      {                                                               \
        mre_mem_free(pObj);                                           \
      }                                                               \
    } while (0)


/*-------- mre_str_dup --------*/
/**
@brief  Duplicate a string.

Returns a pointer to memory allocated by a call to mre_mem_alloc()
that contains the string that is passed in.  Call mre_mem_free() to
free the buffer when done.

@param[in]  pStr  Pointer to string to duplicate.

@return
  Pointer to memory holding duplicated string.
*/
char * mre_str_dup(
  char const  *pStr
);



/*=============================================================================
  General purpose functions
=============================================================================*/

/*-------- mre_get_token_ex --------*/
/**
@brief

@param

@return

*/
boolean mre_get_token_ex(
  char const  **ppStr,
  char         *pToken,
  size_t        maxSize,
  char          terminator
);


#define mre_get_token(ppStr, pToken, maxSize) \
          mre_get_token_ex(ppStr, pToken, maxSize, '\0')




/*-------- mre_plmns_are_equal --------*/
boolean mre_plmns_are_equal(
  sys_plmn_id_s_type const *pPlmn1,
  sys_plmn_id_s_type const *pPlmn2
);

/*-------- mre_parse_ascii_plmn  --------*/
boolean mre_parse_ascii_plmn(
  char const          *plmnStr,
  sys_plmn_id_s_type  *pPlmn
);


/*-------- mre_extract_plmn_from_imsi --------*/
void mre_extract_plmn_from_imsi(
  mmgsdi_data_type      imsiData,
  size_t                numMncDigits,
  sys_plmn_id_s_type   *pPlmn
);

#endif /* _MRE_UTIL_H_ */
