#ifndef __VVDRV_DEO_COMMON_H__
#define __VVDRV_DEO_COMMON_H__
/**---------------------------------------------------------------------------
 * @file  VVDRV_deo_common.h
 *
 * @brief common definitions for the DEO
 *
 * Copyright (c) 2010-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/************************************************************************/
/*                           INCLUDES                                   */
/************************************************************************/

#include "comdef.h"                      /**< boolean, uint8 and uint32 */

/************************************************************************/
/*                         MACROS & DEFINITIONS                         */
/************************************************************************/

/* Return values for DEO functions */

#define DEO_SUCCESS  0      /**<  Success value  */
#define DEO_ERROR    1      /**<  General error value  */
#define DEO_PENDING  2      /**<  Timer is still pending, not an error */

#endif /* __VVDRV_DEO_COMMON_H__ */
