/*!
  @file mutils_common.h

  @brief
  The commmon header file for MODEM UTILS

*/

/*==============================================================================

  Copyright (c) 2009 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/06/19   mm      CR 2380036: QTrace throttle feature
05/16/16   cx      Initial version  
==============================================================================*/

#ifndef MUTILS_COMMON_H
#define MUTILS_COMMON_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include <comdef.h>
#include <sys.h>
#include <err.h>
#include <modem_mem.h>

/*==============================================================================

                   MACROS 

==============================================================================*/  
#if defined(__GNUC__) && !defined(_lint)
  #define MUTILS_LIKELY_TRUE(x) __builtin_expect((x), 1)
  #define MUTILS_UNLIKELY_TRUE(x) __builtin_expect((x), 0)
#else
  #define MUTILS_LIKELY_TRUE(x) (x)
  #define MUTILS_UNLIKELY_TRUE(x) (x)
#endif /* ! (__GNUC__) */

#define MUTILS_ASSERT(exp)  \
  /*lint -save -e506 -e774 */ \
  if(MUTILS_UNLIKELY_TRUE(!(exp)) ) \
  /*lint -restore */ \
  { \
     ERR_FATAL( "Assert failed: " #exp, 0, 0, 0 ); \
  } 

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/

/*==============================================================================

  MACRO: MUTILS_SUB_MOD

==============================================================================*/
/*!
  Subtraction with modulo.
*/
/*============================================================================*/
#define MUTILS_SUB_MOD(t1, t2, m) \
  ( ( (t2) >= (t1) ) ? ( (t2) - (t1) ) : ( (m) - ((t1) - (t2)) ) )

/*==============================================================================

  MACRO: MUTILS_SUB_MOD_64

==============================================================================*/
/*!
  Subtraction of 64-bit values with power of 2 modulus provided via width.
*/
/*============================================================================*/
#define MUTILS_SUB_MOD_64( \
  t1, \
  t2, \
  width \
) \
  ((t2 << (64 - width)) - (t1 << (64 - width)))

/*==============================================================================

  MACROS: MUTILS_MEM_NEW
          MUTILS_MEM_ALLOC
          MUTILS_MEM_ALLOC_ZERO
          MUTILS_MEM_REF_INC
          MUTILS_MEM_REF_DEC
          MUTILS_MEM_FREE

==============================================================================*/
/*!
  Memory management wrappers.

  @note
  Use macros for accurate file/line tracking.
*/
/*============================================================================*/
#define MUTILS_MEM_NEW(mem_ptr) \
  do { \
    MUTILS_MEM_ALLOC((mem_ptr), sizeof(*(mem_ptr))); \
    memset((mem_ptr), 0, sizeof(*(mem_ptr))); \
  } while (0)

#define MUTILS_MEM_ALLOC(mem_ptr, size) \
  do { \
    (mem_ptr) = (typeof(mem_ptr)) modem_mem_alloc((size), \
      MODEM_MEM_CLIENT_UTILS_CRIT); \
    MUTILS_ASSERT((mem_ptr) != NULL); \
  } while (0)

#define MUTILS_MEM_FREE(mem_ptr) \
  do { \
    modem_mem_free((mem_ptr), MODEM_MEM_CLIENT_UTILS_CRIT); \
  } while (0)

/*==============================================================================

  MACRO: MUTILS_STR

==============================================================================*/
/*!
  Macro stringification.
*/
/*============================================================================*/
#define MUTILS_STR(x) \
  _MUTILS_STR(x)
#define _MUTILS_STR(x) \
  #x

/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

/*==============================================================================

  FUNCTION:  mutils_sum_mod_64

==============================================================================*/
/*!
  Return sum with wraparound.
*/
/*============================================================================*/
static inline uint64 mutils_sum_mod_64
(
  uint64    t1,
  uint64    t2,
  uint64    max
)
{
  uint64 ret, headroom_t1;

  headroom_t1 = max - t1;

  if (headroom_t1 < t2)
  {
    /* handle wraparound */
    ret = t2 - headroom_t1;
  }
  else
  {
    /* no wraparound */
    ret = t1 + t2;
  }

  return ret;
}

/*==============================================================================

  FUNCTION:  mutils_round_down_pow2

==============================================================================*/
/*!
  @brief
  Round down to the nearest power of 2.

  @note
  See <http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2>.
*/
/*============================================================================*/
static inline uint32 mutils_round_down_pow2
(
  uint32  val
)
{
  if (val == 0)
  {
    return 0;
  }

  val |= (val >> 1);
  val |= (val >> 2);
  val |= (val >> 4);
  val |= (val >> 8);
  val |= (val >> 16);

  return val - (val >> 1);
}

#endif /* MUTILS_COMMON_H */

