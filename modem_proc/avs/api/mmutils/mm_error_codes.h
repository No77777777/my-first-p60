#ifndef _MM_ERROR_CODES_H_
#define _MM_ERROR_CODES_H_

/**
@file mm_error_codes.h
@brief This file contains common error code definitions to be used across
       multimedia code bases.
*/

/*===========================================================================
  Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
===========================================================================*/

/*========================================================================
Edit History

$Header: //components/rel/avs.mpss/9.0.c51/api/mmutils/mm_error_codes.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------
05/02/2018 cl      Created file.

========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

#include "mmdefs.h"

/** @addtogroup mm_error_codes
@{ */

/** Status messages (error codes) returned by command responses. */
typedef uint32_t mm_result_t;

/** Success. The operation completed with no errors. */
#define MM_EOK              ((uint32_t)0x0)
/** General failure. */
#define MM_EFAILED          ((uint32_t)0x1 << 0)
/** Bad operation parameter. */
#define MM_EBADPARAM        ((uint32_t)0x1 << 1)
/** Unsupported routine or operation. */
#define MM_EUNSUPPORTED     ((uint32_t)0x1 << 2)
/** Unsupported version. */
#define MM_EVERSION         ((uint32_t)0x1 << 3)
/** Unexpected problem encountered. */
#define MM_EUNEXPECTED      ((uint32_t)0x1 << 4)
/** Unhandled problem occurred. */
#define MM_EPANIC           ((uint32_t)0x1 << 5)
/** Unable to allocate resource. */
#define MM_ENORESOURCE      ((uint32_t)0x1 << 6)
/** Invalid handle. */
#define MM_EHANDLE          ((uint32_t)0x1 << 7)
/** Operation is already processed. */
#define MM_EALREADY         ((uint32_t)0x1 << 8)
/** Operation is not ready to be processed. */
#define MM_ENOTREADY        ((uint32_t)0x1 << 9)
/** Operation is pending completion. */
#define MM_EPENDING         ((uint32_t)0x1 << 10)
/** Operation cannot be accepted or processed. */
#define MM_EBUSY            ((uint32_t)0x1 << 11)
/** Operation was aborted due to an error. */
#define MM_EABORTED         ((uint32_t)0x1 << 12)
/** Operation requests an intervention to complete. */
#define MM_ECONTINUE        ((uint32_t)0x1 << 13)
/** Operation requests an immediate intervention to complete. */
#define MM_EIMMEDIATE       ((uint32_t)0x1 << 14)
/** Operation was not implemented. */
#define MM_ENOTIMPL         ((uint32_t)0x1 << 15)
/** Operation needs more data or resources. */
#define MM_ENEEDMORE        ((uint32_t)0x1 << 16)
/** Operation does not have memory. */
#define MM_ENOMEMORY        ((uint32_t)0x1 << 17)
/** Item does not exist. */
#define MM_ENOTEXIST        ((uint32_t)0x1 << 18)
/** Operation is finished. */
#define MM_ETERMINATED      ((uint32_t)0x1 << 19)

/** @} */  /* end_addtogroup mm_error_codes */

/** Checks if a result is a success or failure. */
#define MM_SUCCEEDED(x)   ( MM_EOK == (x) )
#define MM_DID_FAIL(x)    ( MM_EOK != (x) )

/** Sets a flag. */
#define MM_SET_ERROR(error_flags, new_flag) ((error_flags) |= (new_flag))

/** Checks if error_code is set in error_flags. */
#define MM_CHECK_ERROR(error_flags, error_code) (((error_flags) & (error_code)) != MM_EOK)

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* _MM_ERROR_CODES_H_ */
