#ifndef __VVDRV_DEO_MEMPOOL_H__
#define __VVDRV_DEO_MEMPOOL_H__
/**-----------------------------------------------------------------------------
 * @file    VVDRV_deo_mempool.h
 *
 * @brief   Declares the interface to the DEO memory-management service
 *
 *  In order to use the memory management service 4 defines are available:
 *
 *  1. DEO_CONFIG_POOL_DMA       - Define this flag to enable the DMA mempool
 *                                 and related APIs deo_dma_alloc(),
 *                                 deo_dma_free() and deo_dma_add_block()
 *
 *  2. DEO_CONFIG_MEMPOOL        - If defined, APIs deo_alloc() and deo_free()
 *                                 handle memory allocated from block provided
 *                                 at the call to deo_mem_init(). If not
 *                                 defined these APIs map to the OS APIs
 *                                 malloc() and free()
 *
 *  3. DEO_PAGE_SIZE             - Memory page size (in bytes) used when
 *                                 requesting allocation with flag M_PAGE_ALIGN
 *
 *  4. DEO_CONFIG_MEMPOOL_ALIGN  - Native (minimal) memory pool alignment, in
 *                                 bytes
 *
 * Copyright (c) 2010-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *----------------------------------------------------------------------------*/

/******************************************************************************/
/*                                INCLUDES                                    */
/******************************************************************************/

#include "VVDRV_deo_common.h"  /* DEO_SUCCESS, uint32 and more */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************/
/*                          CONSTANTS & MACROS                                */
/******************************************************************************/

/** Memory management flags */
#define M_ZERO          0x0001  /**< Zero the allocation */
#define M_CACHABLE      0x0002  /**< @deprecated, ignored */
#define M_PAGE_ALIGN    0x0004  /**< Block must not cross page boundaries */

/** DEO DMA number of different allocation pools */
#if !defined(DEO_MEMPOOL_DMA_POOL_COUNT)
#if defined(DEO_DRIVER_CONCURRENT_EXECUTION)
#define DEO_MEMPOOL_DMA_POOL_COUNT     32 /* Default: 32 pools (concurrenct) */
#else /* !DEO_DRIVER_CONCURRENT_EXECUTION */
#define DEO_MEMPOOL_DMA_POOL_COUNT      1 /* Default: single pool */
#endif /* DEO_DRIVER_CONCURRENT_EXECUTION */
#endif /* !DEO_MEMPOOL_DMA_POOL_COUNT */

/** DEO DMA pool invalid handle */
#define DEO_MEMPOOL_DMA_POOL_INVALID    0

/** DEO DMA default pool handle */
#define DEO_MEMPOOL_DMA_POOL_DEFAULT    1

/******************************************************************************/
/*                         TYPEDEFS & STRUCTS                                 */
/******************************************************************************/

/**
 * @brief   Defines a handle single DMA allocation.
 *          The handle is allocated by the user and passed to APIs like
 *          #VVDRV_deo_dma_alloc() and #VVDRV_deo_dma_free(). User must not 
 *          change the internal content of this data structure.
 */
typedef void *deo_dma_handle_t;

/**
 * @brief   Defines a handle to a DMA pool.
 *          The handle is allocated by the user and passed to APIs like
 *          #VVDRV_deo_dma_pool_alloc() and #VVDRV_deo_dma_pool_add_block(). 
 *          User must not change the internal content of this data structure.
 */
typedef uint32 deo_dma_pool_handle_t;

/**
 * @brief   Arguments for initialization of the memory-allocator service.
 *          If #DEO_CONFIG_MEMPOOL defined then arguments mem_buf and
 *          mem_buf_size will be used.
 *
 *          If #DEO_CONFIG_POOL_DMA defined and dma_buf_size is not zero, then
 *          arguments dma_vbuf and dma_pbuf will be used.
 */
typedef struct deo_mempool_args_s
{
    void   *mem_buf;         /**< Start address of memory pool  */
    uint32  mem_buf_size;    /**< Size of memory pool, in bytes */

    void   *dma_vbuf;        /**< Virtual  start address of default DMA pool */
    void   *dma_pbuf;        /**< Physical start address of default DMA pool */
    uint32  dma_buf_size;    /**< Size of default DMA memory pool, in bytes  */

    void   *dma_cachable_vbuf;  /**< Deprecated, ignored. */
    void   *dma_cachable_pbuf;  /**< Deprecated, ignored. */
    uint32  dma_cachable_size;  /**< Deprecated, ignored. */
} deo_mempool_args_t;

/******************************************************************************/
/*                                 FUNCTIONS                                  */
/******************************************************************************/

/**
 * @brief       Initializes the memory allocator service
 *
 * @note        This API might be called by #VVDRV_deo_init() depending on 
 *              argument passed to that API. If not called by deo_init(), user 
 *              can call this API directly.
 *
 * @param[in]   args  Memory-allocator initialization arguments. For further
 *                    details see #deo_mempool_args_t.
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_mem_init(const deo_mempool_args_t *args);

/**
 * @brief       Un-initializes the memory-allocator service
 */
void VVDRV_deo_mem_uninit(void);

/**
 * @brief       Create a new DMA pool
 *
 * @note        #VVDRV_deo_mem_init() should be called prior to calling this API
 *
 * @note        If VVDRV_deo_mem_init() or deo_init() were called with the 
 *              default DMA pool params then the default DMA pool was already 
 *              created and no need to call this API for using the default DMA 
 *              pool. If not, the first successful call to this API will create 
 *              the default DMA pool
 *
 * @param[out]  pool    if successful, contains a new DMA pool handle
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_dma_pool_create(deo_dma_pool_handle_t *pool);

/**
 * @brief           Release a DMA memory pool
 *
 * @note            #VVDRV_deo_mem_init() should be called prior to calling 
 *                  this API
 *
 * @param[in,out]   pool    Handle to DMA memory pool previously allocated by
 *                          #VVDRV_deo_dma_pool_create() that will be released.
 *                          On success handle will be set to NULL.
 *
 * @return          #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_dma_pool_release(deo_dma_pool_handle_t *pool);

/**
 * @brief       Allocate a memory buffer from the memory pool
 *
 * @note        #VVDRV_deo_mem_init() should be called prior to calling this API
 *
 * @warning     Every block allocated by this API must be freed by calling
 *              #VVDRV_deo_free() or else it will result in memory leak
 *
 * @note        Implementation depends on definition of #DEO_CONFIG_MEMPOOL,
 *              See further details in top of this file
 *
 * @param[in]   size    Size of the required buffer, in bytes
 * @param[in]   flags   Allocation flags
 *
 * @return      NULL if failed, else pointer to the allocated buffer
 */
void *VVDRV_deo_alloc(uint32 size, uint16 flags);

/**
 * @brief       Free a memory buffer previously allocated by #VVDRV_deo_alloc()
 *
 * @note        #VVDRV_deo_mem_init() should be called prior to calling 
 *              this API
 *
 * @param[in]   pBuffer  Pointer to a memory block previously allocated
 *                       by #VVDRV_deo_alloc(), that needs to be freed
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_free(void *pBuffer);

/**
 * @brief       Add a memory block to the default DMA memory pool. This block
 *              can then be used by #VVDRV_deo_dma_alloc() to allocate blocks 
 *              from it
 *
 * @note        #VVDRV_deo_mem_init() should be called prior to calling
  *             this API
 *
 * @note        This API is the same as calling #VVDRV_deo_dma_pool_add_block()
 *              with the default DMA pool handle
 *
 * @warning     When adding multiple blocks the gap between adjacent blocks
 *              must be greater than #DEO_DMA_MIN_BLOCK bytes, meaning the
 *              difference between last byte of lower block and first byte of
 *              upper block must be greater than #DEO_DMA_MIN_BLOCK bytes
 *
 * @param[in]   size    Size of the added block, in bytes
 * @param[in]   vaddr   Virtual address of added block
 * @param[in]   paddr   Physical address of added block
 * @param[in]   flags   Allocation flags
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_dma_add_block
(
    uint32 size,
    void *vaddr,
    void *paddr,
    uint16 flags
);

/**
 * @brief       Add a memory block to a DMA memory pool. This block can then be
 *              used by #VVDRV_deo_dma_pool_alloc() to allocate blocks from it
 *
 * @note        #VVDRV_deo_mem_init() and #VVDRV_deo_dma_pool_create() should
 *              be called prior to calling this API
 *
 * @warning     When adding multiple blocks the gap between adjacent blocks
 *              must be greater than #DEO_DMA_MIN_BLOCK bytes, meaning the
 *              difference between last byte of lower block and first byte of
 *              upper block must be greater than #DEO_DMA_MIN_BLOCK bytes
 *
 * @param[in]   pool    DMA pool handle as provided by 
 *                      #VVDRV_deo_dma_pool_create()
 * @param[in]   size    Size of the added block, in bytes
 * @param[in]   vaddr   Virtual address of added block
 * @param[in]   paddr   Physical address of added block
 * @param[in]   flags   Allocation flags
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_dma_pool_add_block
(
    deo_dma_pool_handle_t pool,
    uint32                size,
    void                 *vaddr,
    void                 *paddr,
    uint16                flags
);

/**
 * @brief       Allocate a memory buffer from the default DMA memory pool
 *
 * @note        #VVDRV_deo_mem_init() should be called prior to calling
  *             this API
 *
 * @warning     Every block allocated by this API must be freed by calling
 *              #VVDRV_deo_dma_free() or else it will result in memory leak
 *
 * @note        This API is the same as calling #VVDRV_deo_dma_pool_alloc()
 *              with the default DMA pool handle
 *
 * @param[in]   size    Size of the required buffer, in bytes
 * @param[in]   align   Alignment of the physical address of the required
 *                      buffer, in bytes. Must be a power of 2.
 *                      Maximal possible value is 0x800000 bytes.
 *                      Pass zero or one if no special alignment needed
 * @param[out]  vaddr   Virtual address of allocated buffer. Ignored if NULL
 * @param[out]  paddr   Physical address of allocated buffer. Ignored if NULL
 * @param[in]   flags   Allocation flags
 * @param[out]  handle  Handle to reference the allocation. Must be allocated
 *                      by the user and passed to this API. Passed later to
 *                      #VVDRV_deo_dma_free() to free the buffer. Will be set
 *                      to NULL if the allocation failed
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 *
 * @n   Please see the following example code that allocates a
 *      buffer of 128 byte size and frees it.
 *
 * @code
 *
 *  #define DEO_CONFIG_POOL_DMA
 *  #define BUF_SIZE   128
 *
 *  typedef struct buffer_s
 *  {
 *      void    *vaddr;
 *      void    *paddr;
 *      void    *handle;
 *  } buffer_t;
 *
 *  deo_mempool_args_t args;
 *  buffer_t *pBuffer;
 *
 *  ...
 *  Initialize args
 *  ...
 *
 *  if (DEO_SUCCESS != VVDRV_deo_mem_init(&args))
 *  {
 *      return DEO_ERROR;
 *  }
 *
 *  if (DEO_SUCCESS != VVDRV_deo_dma_alloc(BUF_SIZE,
 *                                         0,
 *                                         &(pBuffer->vaddr),
 *                                         &(pBuffer->paddr),
 *                                         0,
 *                                         &(pBuffer->handle)))
 *  {
 *      return DEO_ERROR;
 *  }
 *
 *  ...
 *  Use the allocated buffer
 *  ...
 *
 *  VVDRV_deo_dma_free(pBuffer->handle);
 *
 * @endcode
 */
int32 VVDRV_deo_dma_alloc
(
    uint32              size,
    uint32              align,
    void              **vaddr,
    void              **paddr,
    uint16              flags,
    deo_dma_handle_t   *handle
);

/**
 * @brief       Allocate a memory buffer from a DMA memory pool
 *
 * @note        #VVDRV_deo_mem_init() and deo_dma_pool_create() should be 
 *              called prior to calling this API
 *
 * @warning     Every block allocated by this API must be freed by calling
 *              #VVDRV_deo_dma_free() or else it will result in memory leak
 *
 * @param[in]   pool    DMA pool handle as provided by 
 *                      #VVDRV_deo_dma_pool_create()
 * @param[in]   size    Size of the required buffer, in bytes
 * @param[in]   align   Alignment of the physical address of the required
 *                      buffer, in bytes. Must be a power of 2.
 *                      Maximal possible value is 0x800000 bytes.
 *                      Pass zero or one if no special alignment needed
 * @param[out]  vaddr   Virtual address of allocated buffer. Ignored if NULL
 * @param[out]  paddr   Physical address of allocated buffer. Ignored if NULL
 * @param[in]   flags   Allocation flags
 * @param[out]  handle  Handle to reference the allocation. Must be allocated
 *                      by the user and passed to this API. Passed later to
 *                      #VVDRV_deo_dma_free() to free the buffer. Will be set 
 *                      to NULL if the allocation failed
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 *
 * @n   Please see the following example code that allocates two buffers of
 *      size 128 byte from two different pools and frees them.
 *
 * @code
 *
 *  #define DEO_CONFIG_POOL_DMA
 *  #define BUF_SIZE   128
 *
 *  typedef struct buffer_s
 *  {
 *      void    *vaddr;
 *      void    *paddr;
 *      void    *handle;
 *  } buffer_t;
 *
 *  deo_mempool_args_t args;
 *  deo_mempool_args_t args2;
 *  buffer_t *pBuffer1;
 *  buffer_t *pBuffer2;
 *  deo_dma_pool_handle_t pool_handle;
 *
 *  ...
 *  Initialize args and args2
 *  ...
 *
 *  // This will also create the default DMA memory pool
 *  if (DEO_SUCCESS != VVDRV_deo_mem_init(&args))
 *  {
 *      return DEO_ERROR;
 *  }
 *
 *  // Create second DMA memory pool
 *  if (DEO_SUCCESS != VVDRV_deo_dma_pool_create(&pool_handle))
 *  {
 *      return DEO_ERROR;
 *  }
 *
 *  // Add a block to second DMA memory pool
 *  if (DEO_SUCCESS != VVDRV_deo_dma_pool_add_block(pool_handle,
 *                                                  args.size,
 *                                                  &args.vaddr,
 *                                                  &args.paddr,
 *                                                  0))
 *  {
 *      return DEO_ERROR;
 *  }
 *
 *  // Allocate one buffer from the default DMA memory pool.
 *  // Note that this is the same as calling:
 *  //     VVDRV_deo_dma_pool_alloc(DEO_MEMPOOL_DMA_POOL_DEFAULT, BUF_SIZE, ...
 *  if (DEO_SUCCESS != VVDRV_deo_dma_alloc(BUF_SIZE,
 *                                         0,
 *                                         &(pBuffer1->vaddr),
 *                                         &(pBuffer1->paddr),
 *                                         0,
 *                                         &(pBuffer1->handle)))
 *  {
 *      return DEO_ERROR;
 *  }
 *
 *  // Allocate one block from the second DMA memory pool
 *  if (DEO_SUCCESS != VVDRV_deo_dma_pool_alloc(pool_handle,
 *                                              BUF_SIZE,
 *                                              0,
 *                                              &(pBuffer2->vaddr),
 *                                              &(pBuffer2->paddr),
 *                                              0,
 *                                              &(pBuffer2->handle)))
 *  {
 *      return DEO_ERROR;
 *  }
 *
 *  ...
 *  Use the allocated buffers
 *  ...
 *
 *  VVDRV_deo_dma_free(pBuffer1->handle);
 *  VVDRV_deo_dma_free(pBuffer2->handle);
 *
 *  ...
 *
 *  // Release the second DMA pool
 *  if (DEO_SUCCESS != VVDRV_deo_dma_pool_release(&pool_handle))
 *  {
 *      return DEO_ERROR;
 *  }
 *
 * @endcode
 */
int32 VVDRV_deo_dma_pool_alloc
(
    deo_dma_pool_handle_t   pool,
    uint32                  size,
    uint32                  align,
    void                  **vaddr,
    void                  **paddr,
    uint16                  flags,
    deo_dma_handle_t       *handle
);

/**
 * @brief       Free a memory buffer previously allocated by 
 *              #VVDRV_deo_dma_alloc() or #VVDRV_deo_dma_pool_alloc()
 *
 * @note        #VVDRV_deo_mem_init() should be called prior to calling
 *              this API
 *
 * @param[in]   handle  Handle to a memory block previously allocated by
 *                      #VVDRV_deo_dma_alloc() or #VVDRV_deo_dma_pool_alloc(),
 *                      that needs to be freed
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_dma_free(deo_dma_handle_t handle);

/**
 * @brief   Returns the physical address plus an offset of the memory buffer
 *          referenced by the handle
 *
 * @param[in]   handle      DMA handle as returned from #VVDRV_deo_dma_alloc() 
 *                          or #VVDRV_deo_dma_pool_alloc()
 * @param[in]   offset      Offset in bytes, can be zero
 *
 * @return  Pointer to physical address plus offset, or NULL if failed
 */
void *VVDRV_deo_dma_phys_addr(deo_dma_handle_t handle, uint64 offset);

/**
 * @brief   Returns the virtual address plus an offset of the memory buffer
 *          referenced by the handle
 *
 * @param[in]   handle      DMA handle as returned from #VVDRV_deo_dma_alloc()
 *                          or #VVDRV_deo_dma_pool_alloc()
 * @param[in]   offset      Offset in bytes, can be zero
 *
 * @return  Pointer to virtual address plus offset, or NULL if failed
 */
void *VVDRV_deo_dma_virt_addr(deo_dma_handle_t handle, uint64 offset);

/**
 * @brief   Set attributes of a "dummy" block, that can be created by calling
 *          #VVDRV_deo_dma_alloc() with zero size parameter. The attributes set
 *          by this API can be later retrieved by the
 *          #VVDRV_deo_dma_phys_addr() and #VVDRV_deo_dma_virt_addr() APIs
 *
 * @param[in]       size     Buffer size, in bytes
 * @param[in]       vaddr    Buffer virtual address
 * @param[in]       paddr    Buffer physical address
 * @param[in,out]   handle   DMA handle as returned from #VVDRV_deo_dma_alloc()
 *                           or #VVDRV_deo_dma_pool_alloc()
 *
 * @return  #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_dma_dummy_set
(
    uint32            size,
    void             *vaddr,
    void             *paddr,
    deo_dma_handle_t  handle
);

/**
 * @brief       De-fragments the freed memory blocks of all pools
 *
 * @note        #VVDRV_deo_mem_init() should be called prior to calling
 *              this API
 *
 * @note        Under normal operation there should be no need to call this
 *              API. De-fragmentation will automatically be attempted when
 *              an allocation attempt fails
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_mem_defrag(void);

/**
 * @brief       Debug-only API. Prints the memory dump and statistics for debug
 *              purposes
 *
 * @note        #VVDRV_deo_mem_init() should be called prior to calling
 *              this API
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_mem_log_print(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DEO_MEMPOOL_H_ */
