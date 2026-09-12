#ifndef __VVDRV_DEO_MEMPOOL_OS_H__
#define __VVDRV_DEO_MEMPOOL_OS_H__
/**---------------------------------------------------------------------------
 * @file  deo_mempool_os.h
 *
 * @brief DEO memory allocator definitions for VI_Build version
 *
 * Copyright (c) 2010-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/************************************************************************/
/*                       CONSTANTS & MACROS                             */
/************************************************************************/

#ifndef DEO_PAGE_SHIFT
/** Logical shift amount that equals LOG2(#DEO_PAGE_SIZE) */
#define DEO_PAGE_SHIFT  12
#endif /* DEO_PAGE_SHIFT */

#ifndef DEO_PAGE_SIZE
/** DEO allocator page size, in bytes. Useful with the #M_PAGE_ALIGN option */
#define DEO_PAGE_SIZE   (1 << DEO_PAGE_SHIFT)
#endif /* DEO_PAGE_SIZE */

#ifndef DEO_CONFIG_MEMPOOL
#define DEO_CONFIG_MEMPOOL                        /**< Enable mempool */
#endif /* DEO_CONFIG_MEMPOOL */

#ifndef DEO_CONFIG_POOL_DMA
#define DEO_CONFIG_POOL_DMA                       /**< Enable DMA mempool */
#endif /* DEO_CONFIG_POOL_DMA */

#endif /* __VVDRV_DEO_MEMPOOL_OS_H__ */
