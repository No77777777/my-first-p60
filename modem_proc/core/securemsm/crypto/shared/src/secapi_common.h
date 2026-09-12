#ifndef SECAPI_COMMON_H
#define SECAPI_COMMON_H

/** 
  @brief SECAPI-UCLIB interface utility functions
 * 
 */

/*===========================================================================
Copyright (c) 2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
===========================================================================*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "IxErrno.h"
#include "uclib.h"
#include "secapi_engine.h"
#include "secapi_hash.h"
#include "UxCommon.h"
#include "msg.h"

#define CONVERT_UCLIB_ERROR(err)                                              \
{                                                                             \
  if (UCLIB_SUCCESS != err)                                                   \
  {                                                                           \
    MSG_ERROR("SECAPI-UCLIB: uclib error 0x%x, line %d", err, __LINE__, 0);   \
    err = E_FAILURE;                                                          \
  }                                                                           \
  err = E_SUCCESS;                                                            \
}


/* Convert a SECAPI engine type to a UCLIB engine type */
UCLIB_ENGINE_TYPE secapi_engine_to_uclib(SECAPI_ENGINE_TYPE p);

/* Convert a SECAPI hash algorithm type to a UCLIB hash algorithm type */
UCLIB_HASH_ALG secapi_hash_alg_to_uclib(SECAPI_HASH_ALG p);

#endif
