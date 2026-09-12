#ifndef __VVDRV_DEO_OS_H__
#define __VVDRV_DEO_OS_H__
/**---------------------------------------------------------------------------
 * @file  VVDRV_deo_os.h
 *
 * @brief declares the DEO common defined for VI build
 *
 * Copyright (c) 2011-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/************************************************************************/
/*                           INCLUDES                                   */
/************************************************************************/

#include "VVDRV_deo_init.h"     /* For deo_cfg_t          */
#include "string.h"             /* For memset and memcpy  */

#if !defined(DEO_CONFIG_OS_FORCE_VV_MSG)
#include "stdio.h"              /* For printf             */
#else /* DEO_CONFIG_OS_FORCE_VV_MSG */
#include "vv_msg.h"             /* For vv_msg             */
#endif /* DEO_CONFIG_OS_FORCE_VV_MSG */

/************************************************************************/
/*                         MACROS & DEFINITIONS                         */
/************************************************************************/

#if !defined(deo_os_printf)
#if !defined(DEO_CONFIG_OS_FORCE_VV_MSG)
#define deo_os_printf       printf   /* Default implementation using printf() */
#else /* defined(DEO_CONFIG_OS_FORCE_VV_MSG) */
#if  !defined(VV_MSG_FILE_NUMBER)
#define VV_MSG_FILE_NUMBER  0                          /* Default file number */
#endif /* !VV_MSG_FILE_NUMBER */
/* Use shared-code vv_msg() */
#define deo_os_printf(...)  vv_msg(SEV_INFO, ST_FUNCTION, __VA_ARGS__)
#endif /* !DEO_CONFIG_OS_FORCE_VV_MSG */
#endif /* !deo_os_printf */

/* Memory and strings operations */
#ifndef deo_os_memcpy
#define deo_os_memcpy memcpy
#endif /* !deo_os_memcpy */

#ifndef deo_os_memset
#define deo_os_memset memset
#endif /* !deo_os_memset */

#ifndef deo_os_strlen
#define deo_os_strlen strlen
#endif /* !deo_os_strlen */

#ifndef deo_os_stricmp
#define deo_os_stricmp strcasecmp
#endif /* !deo_os_stricmp */

#ifndef deo_os_snprintf
#define deo_os_snprintf _snprintf
#endif /* !deo_os_snprintf */

/* strncat_s is the windows secure implementation of safer strncat which takes 
 * maximum size of destination as parameter. Since it is not implemented in gnu,
 * convert to regular strncat, which takes maximum left of src as 3'rd parameter
 */
#ifndef deo_os_strncat_s
#define deo_os_strncat_s(dst,len,src,len2) strncat(dst,src,len-strlen(dst))
#endif /* !deo_os_strncat_s */

/**
 * @brief           Initializes DEO OS dependent services
 *
 * @param[in,out]   cfg     If non-null, DEO configuration data
 *
 * @return          #DEO_SUCCESS if successful otherwise #DEO_ERROR.
 */
int32 VVDRV_deo_init_os(deo_cfg_t *cfg);

/**
 * @brief           Un-initialize DEO OS dependent services
 */
void VVDRV_deo_uninit_os(void);

#endif  /* __VVDRV_DEO_OS_H__ */
