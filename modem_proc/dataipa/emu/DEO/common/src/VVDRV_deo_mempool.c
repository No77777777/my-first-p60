/**-----------------------------------------------------------------------------
 * @file  VVDRV_deo_mempool.c
 *
 * @brief Implements the DEO memory-management service
 *
 * Copyright (c) 2010-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/

/*lint -save -e* suppress all lint messages for the entire file*/
#include "VVDRV_deo_mempool.h"
#include "VVDRV_deo_debug.h"
#include "VVDRV_deo_lists.h"
#include "VVDRV_deo_mempool_os.h"
#include "VVDRV_deo_mutex.h"

#ifndef DEO_CONFIG_MEMPOOL
#include <stdlib.h>         /* For malloc() and free() */
#endif /* DEO_CONFIG_MEMPOOL */

/*------------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *----------------------------------------------------------------------------*/

/*----- Common: both mempool and DMA mempool ---------------------------------*/

/**
 * @def DEO_CONFIG_POOL_DMA
 *
 * @brief Define this flag to enable the DMA mempool and related APIs
 *        #VVDRV_deo_dma_alloc(), #VVDRV_deo_dma_free() and 
 *        #VVDRV_deo_dma_add_block()
 */

/**
 * @def DEO_CONFIG_MEMPOOL
 *
 * @brief If defined, APIs #VVDRV_deo_alloc() and #VVDRV_deo_free() handle 
 *        memory allocated from block provided at the call to 
 *        #VVDRV_deo_mem_init(). If not defined these APIs map to the OS APIs 
 *        malloc() and free()
 */

/**
 * @def DEO_PAGE_SIZE
 *
 * @brief Memory page size (in bytes) used when requesting allocation with
 *        flag M_PAGE_ALIGN
 */

/**
 * @def DEO_CONFIG_MEMPOOL_USAGE_SHOW
 *
 * @brief Track usage of memory allocators, including statistics
 */
//#define DEO_CONFIG_MEMPOOL_USAGE_SHOW

/**
 * @def DEO_MEM_STAT_DEBUG
 *
 * @brief Enable debug of memory allocator statistics
 */
//#define DEO_MEM_STAT_DEBUG

#ifndef DEO_CONFIG_MEMPOOL_ALIGN
/** Native (minimal) memory pool alignment, in bytes.
 *
 *  @warning NOTE CARFULLY  This value must be a multiple of sizeof(void *),
 *           otherwise code might crash at run-time!
 *
 *  @warning NOTE CARFULLY  This will be the minimal alignment of each block
 *           physical address and size
 */
#define DEO_CONFIG_MEMPOOL_ALIGN    (sizeof(void *))
#endif /* !DEO_CONFIG_MEMPOOL_ALIGN */

/* Protection from contradicting defines */
#if (defined(DEO_CONFIG_MEMPOOL_USAGE_SHOW) && !defined(DEO_CONFIG_MEMPOOL))
#error Cannot define DEO_CONFIG_MEMPOOL_USAGE_SHOW without DEO_CONFIG_MEMPOOL
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW && !DEO_CONFIG_MEMPOOL */

#if (defined(DEO_MEM_STAT_DEBUG) && !defined(DEO_CONFIG_MEMPOOL_USAGE_SHOW))
#error Cannot define DEO_MEM_STAT_DEBUG without DEO_CONFIG_MEMPOOL_USAGE_SHOW
#endif /* DEO_MEM_STAT_DEBUG && !DEO_CONFIG_MEMPOOL_USAGE_SHOW */

/* Current implementation supports up to 254 different DMA pools */
#if DEO_MEMPOOL_DMA_POOL_COUNT >= 255
#error Invalid DEO_MEMPOOL_DMA_POOL_COUNT size
#endif /* DEO_MEMPOOL_DMA_POOL_COUNT >= 255 */

#if !defined(DEO_PAGE_SIZE) || (DEO_PAGE_SIZE > 0xFFFFFFFF)
#error DEO_PAGE_SIZE not defined or invalid DEO_PAGE_SIZE
#endif /* DEO_PAGE_SIZE > 0xFFFFFFFF */

#if (0 != (DEO_PAGE_SIZE & (DEO_PAGE_SIZE - 1)))
#error Invalid DEO_PAGE_SIZE defined, must be power of 2
#endif /* (0 != (DEO_PAGE_SIZE & (DEO_PAGE_SIZE -1))) */

/*----- Mempool only ---------------------------------------------------------*/
#ifdef DEO_CONFIG_MEMPOOL

/** Minimal size mempool allocation size
 *
 *  @warning NOTE: Must be multiple of #DEO_CONFIG_MEMPOOL_ALIGN
 */
#define DEO_MEMP_MIN_BLOCK          DEO_CONFIG_MEMPOOL_ALIGN

/** Minimal size of block to split at the mempool
 *
 *  @warning NOTE: Value must be multiple of #DEO_CONFIG_MEMPOOL_ALIGN, and
 *           minimal value is (sizeof(deo_memp_item_t) + #DEO_MEMP_MIN_BLOCK)
 */
#define DEO_MEMP_MIN_BLOCK_SPLIT (sizeof(deo_memp_item_t) + DEO_MEMP_MIN_BLOCK)

/** Maximal surplus size to re-use block, in bytes - mempool */
#define DEO_MEMP_MAX_SURPLUS        256

#ifndef DEO_MEMP_CONST_LIST_ITEM_SIZE
/** Size of const-list item size (in bytes) if DMA allocator not used.
 *  Not currently a use-case; if needed, set to a commonly used buffer size.
 *
 *  @warning NOTE: Must be multiple of #DEO_CONFIG_MEMPOOL_ALIGN
 */
#define DEO_MEMP_CONST_LIST_ITEM_SIZE    32
#endif /* DEO_MEMP_CONST_LIST_ITEM_SIZE */

/** Returns a pointer to the data block in a given item */
#define DEO_MEMP_ITEM_PTR_TO_DATA_PTR(item)  \
              ((void *)(((uint8 *)(item)) + sizeof(*(item))))

/** Returns a pointer to an item from a given data block */
#define DEO_MEMP_DATA_PTR_TO_ITEM_PTR(ptr)   \
              ((deo_memp_item_t *)(((uint8 *)(ptr)) - sizeof(deo_memp_item_t)))

/** Number of const-size lists */
#define DEO_MEMP_CONST_LIST_CNT     (ARR_SIZE(deo_memp_alloc_sizes))

#define DEO_MEMP_MARK_FREE  0xF12EEDE0  /**< Mark of mempool free block */
#define DEO_MEMP_MARK_ALLOC 0xDE0A110C  /**< Mark of mempool allocated block */

#endif /* DEO_CONFIG_MEMPOOL */

/*----- DMA mempool only -----------------------------------------------------*/
#ifdef DEO_CONFIG_POOL_DMA

/**
 * @def DEO_DMA_DEFRAG_DEBUG
 *
 * @brief If defined, call to #VVDRV_deo_dma_pool_defrag() will output debug 
 *        information
 */
//#define DEO_DMA_DEFRAG_DEBUG

/**
 * @def DEO_CONFIG_DMA_DEBUG
 *
 * @brief   Enable run-time debugging DMA functions
 *
 * @warning This feature requires the CPU to have R/W access to DMA memory. If
 *          CPU does not have access, the allocator will crash
 */
//#define DEO_CONFIG_DMA_DEBUG

/** Minimal size of a DMA allocated block
 *
 *  @warning NOTE: Must be multiple of #DEO_CONFIG_MEMPOOL_ALIGN and must be
 *                 at-least #DEO_CONFIG_MEMPOOL_ALIGN, preferably bigger
 */
#define DEO_DMA_MIN_BLOCK       32

/** Maximal surplus size to re-use block, in bytes
 *
 *  @note    Should be multiple of #DEO_CONFIG_MEMPOOL_ALIGN and be at-least
 *           #DEO_DMA_MIN_BLOCK
 *
 *  @note    Value is heuristic and affects performance. Small values will
 *           increase fragmentation. Large values might waste some memory.
 */
#define DEO_DMA_MAX_SURPLUS     256

/** Limit of DMA free blocks to trigger auto-defrag */
#ifndef DEO_DMA_AUTO_DEFRAG_LIMIT_FREE
#define DEO_DMA_AUTO_DEFRAG_LIMIT_FREE      500
#endif /* DEO_DMA_AUTO_DEFRAG_LIMIT_FREE */

#ifdef DEO_CONFIG_DMA_DEBUG
/** Magic pattern that will be written in the first word of each DMA block that
 *  is found in the DMA free-list
 */
#define DMA_CORRUPT_CHECK_MAGIC    0xAABB00CC
#endif /* DEO_CONFIG_DMA_DEBUG */

/** Maximal alignment supported by DMA alloc.
 *
 * @warning Changing this value must be done carefully.
 *          In any case this value must be smaller then the maximal
 *          allocation size.
 */
#define DEO_DMA_MEMPOOL_MAX_ALIGN           0x800000     /* 8 MByte */

/** Marks an invalid DMA block */
#define DEO_DMA_BLOCK_TYPE_INVALID          0

/** Marks an allocated DMA block */
#define DEO_DMA_BLOCK_TYPE_TAKEN            1

/** Marks a dummy DMA block */
#define DEO_DMA_BLOCK_TYPE_DUMMY            2

/** Marks a free DMA block (in a free-list) */
#define DEO_DMA_BLOCK_TYPE_FREE             3

/* Check for invalid changes to pool size or handles */

#if (0 == DEO_MEMPOOL_DMA_POOL_COUNT)
#error Defined DEO_CONFIG_POOL_DMA and DEO_MEMPOOL_DMA_POOL_COUNT = 0
#endif /* 0 == DEO_MEMPOOL_DMA_POOL_COUNT */

#if (0 != DEO_MEMPOOL_DMA_POOL_INVALID)
#error Setting DEO_MEMPOOL_DMA_POOL_INVALID != 0 might require code changes!
#endif /* 0 != DEO_MEMPOOL_DMA_POOL_INVALID */

#if (1 > DEO_MEMPOOL_DMA_POOL_DEFAULT) || \
    (DEO_MEMPOOL_DMA_POOL_COUNT < DEO_MEMPOOL_DMA_POOL_DEFAULT)
#error Must define (0 < DEO_MEMPOOL_DMA_POOL_DEFAULT <= DEO_MEMPOOL_DMA_POOL_COUNT)
#endif /* (1 > DEO_MEMPOOL_DMA_POOL_DEFAULT) ||
          (DEO_MEMPOOL_DMA_POOL_COUNT < DEO_MEMPOOL_DMA_POOL_DEFAULT) */

#endif /* DEO_CONFIG_POOL_DMA */

/*------------------------------------------------------------------------------
 * Type Declarations
 *----------------------------------------------------------------------------*/

/*----- Common: both mempool and DMA mempool ---------------------------------*/

/*----- DMA mempool only -----------------------------------------------------*/
#ifdef DEO_CONFIG_POOL_DMA

/** DMA block descriptor */
typedef struct deo_dma_block_s
{
    uint8       *paddr;               /**< Block physical pointer */
    uint8       *vaddr;               /**< Block virtual pointer */
    uint32       size;                /**< Block size */
    uint32       align;               /**< Block alignment */
    LIST_ENTRY(deo_dma_block_s) next; /**< Free-list next & prev item pointer */
} deo_dma_block_t;

/** DMA Mempool statistics struct */
typedef struct deo_dma_stat_s
{
    /** Current number of free DMA mempool blocks */
    uint32 free_block_cnt_cur;

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW

    /** Maximal number of free DMA mempool blocks */
    uint32 free_block_cnt_max;

    /** Current total size allocated from DMA mempool, in bytes */
    uint64 total_alloc_size_cur;

    /** Maximal total size allocated from DMA mempool, in bytes */
    uint64 total_alloc_size_max;

    /** Current number of allocated DMA mempool blocks */
    uint32 alloc_block_cnt_cur;

    /** Maximal number of allocated DMA mempool blocks */
    uint32 alloc_block_cnt_max;

    /** Total number of DMA mempool blocks (both allocated and free) */
    uint32 total_block_cnt_cur;

    /** Maximal number of DMA mempool blocks (both allocated and free) */
    uint32 total_block_cnt_max;

    /** Number of #VVDRV_deo_dma_alloc() failures (only due to out-of-memory) */
    uint32 failed_alloc_cnt;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

} deo_dma_stat_t;

/** DEO DMA free-list type */
typedef LIST_HEAD(dma_free_list_s, deo_dma_block_s) deo_dma_freelist_t;

/** DMA pool data structure */
typedef struct deo_dma_pool_s
{
    deo_dma_freelist_t freelist;   /** Pool free-list */
    boolean            is_created; /** TRUE if the pool was created */
    deo_dma_stat_t     stats;      /** Pool statistics */
} deo_dma_pool_t;

#endif /* DEO_CONFIG_POOL_DMA */

/*----- Mempool only ---------------------------------------------------------*/
#ifdef DEO_CONFIG_MEMPOOL

/**
 * @brief   DEO mempool free-list item. Each item is of the following form:
 *          first bytes hold the information of the mempool item, rest of the
 *          block is the data buffer
 *
 * @warning Size of this struct must be multiple of #DEO_CONFIG_MEMPOOL_ALIGN
 *
 * @warning Order of struct members is critical - if changed struct size might
 *          change due to padding
 */
typedef struct deo_memp_item_s deo_memp_item_t;
struct deo_memp_item_s
{
    void            *data;      /**< Pointer to data buffer start */
    deo_memp_item_t *next;      /**< Pointer to next item in free list */
    uint32           size;      /**< Usable storage size, in bytes */
    uint32           mark;      /**< Flag to mark free/busy */
};

#ifdef DEO_CONFIG_POOL_DMA
/** Set distributions of item sizes, a const-sized list is created for each
 *  NOTE: sizes must be in an increasing order.
 */
static const uint32 deo_memp_alloc_sizes[] = {sizeof(deo_dma_block_t)};
#else  /* !DEO_CONFIG_POOL_DMA */
static const uint32 deo_memp_alloc_sizes[] = {DEO_MEMP_CONST_LIST_ITEM_SIZE};
#endif /* !DEO_CONFIG_POOL_DMA */

/** The mempool data struct */
typedef struct deo_memp_data_s
{
    /** Array of list-heads for mempool free-lists. Last one is head of the
     *  variable-size list. All the rest are for const-size lists.
     */
    deo_memp_item_t *freelist_head[DEO_MEMP_CONST_LIST_CNT + 1];

    void            *addr_min;     /**< Minimal valid mempool address */
    void            *addr_max;     /**< Maximal valid mempool address */
} deo_memp_data_t;

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW

/** Mempool statistics struct */
typedef struct deo_memp_stat_s
{
    /** Current total size allocated from mempool (with overhead), in bytes */
    uint32 total_alloc_size_cur;

    /** Maximal total size allocated from mempool (with overhead), in bytes */
    uint32 total_alloc_size_max;

    /** Current number of free mempool blocks, per list */
    uint32 list_block_cnt_cur[DEO_MEMP_CONST_LIST_CNT + 1];

    /** Maximal number of free mempool blocks, per list */
    uint32 list_block_cnt_max[DEO_MEMP_CONST_LIST_CNT + 1];

    /** Number of #VVDRV_deo_alloc() failures (only due to out-of-memory) */
    uint32 failed_alloc_cnt;

    /** Number of #VVDRV_deo_free() failures (due to invalid pointer, etc) */
    uint32 failed_free_cnt;

} deo_memp_stat_t;

#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

#endif /* DEO_CONFIG_MEMPOOL */

/*------------------------------------------------------------------------------
 * Global Data Definitions
 *----------------------------------------------------------------------------*/

/*----- Common: both mempool and DMA mempool ---------------------------------*/
/*----- Mempool only ---------------------------------------------------------*/
/*----- DMA mempool only -----------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Static Variable Definitions
 *----------------------------------------------------------------------------*/

/*----- Common: both mempool and DMA mempool ---------------------------------*/

/** @brief Mempool service initialized flag. TRUE - service initialized */
static boolean deo_mem_initialized = FALSE;

/*----- Mempool only ---------------------------------------------------------*/
#ifdef DEO_CONFIG_MEMPOOL

/** Mutex protecting mempool */
DEO_MUTEX_DECLARE(static deo_memp_mutex);

/** The mempool allocator data */
static deo_memp_data_t deo_memp_data = {0};

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW

/** Mempool allocator statistics */
static deo_memp_stat_t deo_memp_stat = {0};

#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

#endif /* DEO_CONFIG_MEMPOOL */

/*----- DMA mempool only -----------------------------------------------------*/
#ifdef DEO_CONFIG_POOL_DMA

/** Mutex protecting dmapool */
DEO_MUTEX_DECLARE(static deo_dmap_mutex);

/** DEO DMA pool structure, each elements is a different pool */
static deo_dma_pool_t deo_dma_pool[DEO_MEMPOOL_DMA_POOL_COUNT] = {0};

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
/** Number of #VVDRV_deo_dma_free() failures (due to invalid pointer, etc) */
static uint32 deo_dma_failed_free_cnt = 0;

/** Maximal number of allocated DMA dummy blocks */
static uint32 deo_dma_dummy_alloc_block_cnt_max = 0;

/** Current number of allocated DMA dummy blocks */
static uint32 deo_dma_dummy_alloc_block_cnt_cur = 0;

#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

#endif /* DEO_CONFIG_POOL_DMA */

/*------------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *----------------------------------------------------------------------------*/

/*----- Common: both mempool and DMA mempool ---------------------------------*/

/**
 * @brief       Calculate offset of given size from #DEO_CONFIG_MEMPOOL_ALIGN
 *
 * @param[in]   size    Size to calculate for, in bytes
 *
 * @return      Align offset relative to #DEO_CONFIG_MEMPOOL_ALIGN, in bytes
 */
static uint32 deo_mem_size_align_offset(uint32 size)
{
    /* No modulo optimizations, compiler handles well constant denominator */
    return (size % DEO_CONFIG_MEMPOOL_ALIGN);
}

/**
 * @brief       Align up size to multiple of #DEO_CONFIG_MEMPOOL_ALIGN
 *
 * @param[in]   size    Size to align for, in bytes
 *
 * @return      New size aligned up to #DEO_CONFIG_MEMPOOL_ALIGN, in bytes
 */
static uint32 deo_mem_size_align_up(uint32 size)
{
    /* No modulo optimizations, compiler handles well constant denominator */
    uint32 offset = size % DEO_CONFIG_MEMPOOL_ALIGN;
    if (0 != offset)
    {
        size += DEO_CONFIG_MEMPOOL_ALIGN - offset;
    }

    return size;
}

/**
 * @brief       Calculate align offset of pointer from
 *              #DEO_CONFIG_MEMPOOL_ALIGN
 *
 * @param[in]   ptr     Pointer to check align for
 *
 * @return      Align offset relative to #DEO_CONFIG_MEMPOOL_ALIGN, in bytes
 */
static uint32 deo_mem_ptr_align_mempool_offset(const void *ptr)
{
    /* No modulo optimizations, compiler handles well constant denominator */
    return (uint32)(((size_t)ptr) % DEO_CONFIG_MEMPOOL_ALIGN);
}

/**
 * @brief       Calculate align offset of pointer from given align
 *
 * @param[in]   ptr     Pointer to check align for
 * @param[in]   align   Alignment to check for, must be power of 2
 *
 * @return      Align offset, in bytes
 */
static uint32 deo_mem_ptr_align_offset(const void *ptr, uint32 align)
{
    /* Note: function assumes 'align' is power of 2 so calculation done by
     * ( & (align -1)) to remove heavy SW modulo implementation on target
     */
    return (uint32)(((size_t)ptr) & (align - 1));
}

/*----- Mempool only ---------------------------------------------------------*/
#ifdef DEO_CONFIG_MEMPOOL

/**
 * @brief       Check if a pointer is valid: in range of the mempool memory and
 *              has correct alignment
 *
 * @param[in]   ptr     Pointer to check
 *
 * @return      TRUE if pointer is valid, FALSE otherwise
 */
static boolean deo_memp_is_valid_ptr(const void *ptr)
{
    if ((deo_memp_data.addr_min > ptr) || (ptr > deo_memp_data.addr_max))
    {
        /* Specific check for NULL, to give detailed message */
        if (NULL == ptr)
        {
            DEO_DBG_E(DEO_MEM,
                        ("DEO_MEM: invalid ptr %p: NULL pointer\n", ptr));
        }
        else
        {
            DEO_DBG_E(DEO_MEM,
                        ("DEO_MEM: invalid ptr %p: out of range\n", ptr));
        }

        return FALSE;
    }

    if (0 != deo_mem_ptr_align_mempool_offset(ptr))
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: invalid ptr %p: not aligned\n", ptr));
        return FALSE;
    }

    return TRUE;
}

#ifdef DEO_MEM_STAT_DEBUG

/**
 * @brief       Counts the number of elements in a given mempool list
 *
 * @param[in]   list_index      Index of the mempool list
 *
 * @return      Number of elements in a given mempool list
 */
static uint32 deo_memp_calc_freelist_cnt(uint32 list_index)
{
    const deo_memp_item_t *item;
    uint32 cnt = 0;

    for (item = deo_memp_data.freelist_head[list_index];
         NULL != item;
         item = item->next)
    {
        cnt++;
    }

    return cnt;
}

/**
 * @brief   Validates the integrity of the mempool statistics
 *
 * @note    Used for statistics debug only
 */
static void deo_memp_check_stats(void)
{
    uint32 list;
    for (list = 0; list < DEO_MEMP_CONST_LIST_CNT + 1; list++)
    {
        uint32 tmp_cnt     = deo_memp_stat.list_block_cnt_cur[list];
        uint32 tmp_counted = deo_memp_calc_freelist_cnt(list);
        DEO_ASSERT((tmp_cnt == tmp_counted),
            ("DEO_MEM: MEMP mismatch: List %u, Counter %u, Counted: %u\n",
             list, tmp_cnt, tmp_counted));
    }
}

#endif /* DEO_MEM_STAT_DEBUG */

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW

/**
 * @brief   Prints out the mempool statistics
 */
static void deo_memp_dump_status(void)
{
    uint32 list;

    DEO_DBG_I(DEO_MEM, ("DEO_MEM: ===== Mempool statistics start\n"));

    DEO_DBG_I(DEO_MEM, ("DEO_MEM: Initial buffer size: %u bytes\n",
                               (uint32)((uint8 *)deo_memp_data.addr_max -
                                        (uint8 *)deo_memp_data.addr_min + 1)));

    DEO_DBG_I(DEO_MEM, ("DEO_MEM: Max total memory allocated: %u bytes\n",
                        deo_memp_stat.total_alloc_size_max));

    for (list = 0; list < (DEO_MEMP_CONST_LIST_CNT); list++)
    {
        DEO_DBG_I(DEO_MEM,
            ("DEO_MEM: Const-size list: Item size %u, Max items %u, "
             "Current items %u\n",
              deo_memp_alloc_sizes[list],
              deo_memp_stat.list_block_cnt_max[list],
              deo_memp_stat.list_block_cnt_cur[list]));
    }

    DEO_DBG_I(DEO_MEM,
           ("DEO_MEM: Variable-size list: Max items %u, Current items %u\n",
              deo_memp_stat.list_block_cnt_max[list],
              deo_memp_stat.list_block_cnt_cur[list]));

    DEO_DBG_I(DEO_MEM, ("DEO_MEM: Failed alloc requests: %u\n",
                        deo_memp_stat.failed_alloc_cnt));

    DEO_DBG_I(DEO_MEM, ("DEO_MEM: Failed free requests: %u\n",
                        deo_memp_stat.failed_free_cnt));

    DEO_DBG_I(DEO_MEM, ("DEO_MEM: ===== Mempool statistics end\n"));
}
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

/**
 * @brief       Initialize the DEO mempool
 *
 * @param[in]   mem_buf     Start address of memory-pool memory
 * @param[in]   size        Size of memory-pool memory, in bytes
 *
 * @return      DEO_SUCCESS if successful, otherwise failed
 */
static int32 deo_memp_init(void *mem_buf, uint32 size)
{
    uint8 *ptr = (uint8 *)mem_buf;
    deo_memp_item_t *item = NULL;

    /* Create mempool mutex and lock */
    if (DEO_SUCCESS != VVDRV_deo_mutex_create(&deo_memp_mutex, 
                                               DEO_MUTEX_ATTR_CRITICAL_SECTION))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex creation failure\n"));
        return DEO_ERROR;
    }

    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&deo_memp_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex lock failure\n"));
        return DEO_ERROR;
    }

    /* Check if size is at least minimal */
    if (size < (sizeof(deo_memp_item_t) + DEO_MEMP_MIN_BLOCK_SPLIT))
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: Mempool block too small to init\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_memp_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Allocator considers zero (NULL) an invalid address. If user wants to
     * start from zero address, adjust forward
     */
    if (NULL == ptr)
    {
        size -= DEO_CONFIG_MEMPOOL_ALIGN;
        ptr  += DEO_CONFIG_MEMPOOL_ALIGN;
    }
    else
    {
        /* If needed, align ptr */
        uint32 offset = deo_mem_ptr_align_mempool_offset(ptr);
        if (0 != offset)
        {
            /* Offset pointer forward to re-align */
            offset = DEO_CONFIG_MEMPOOL_ALIGN - offset;
            size -= offset;
            ptr  += offset;
        }
    }

    /* If needed, align size down - every block size is aligned */
    size -= deo_mem_size_align_offset(size);

    /* Check if the size left is still at least minimal */
    if (size < (sizeof(deo_memp_item_t) + DEO_MEMP_MIN_BLOCK_SPLIT))
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: Mempool block too small to init\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_memp_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Clear all mempool data */
    deo_os_memset(&deo_memp_data, 0, sizeof(deo_memp_data));

    /* Keep min/max addresses to use for sanity checks later */
    deo_memp_data.addr_min = ptr;
    deo_memp_data.addr_max = ptr + size - 1;

    /* Create a new item */
    item       = (deo_memp_item_t *)ptr;         /* First part is item info */
    item->size = size - sizeof(deo_memp_item_t); /* Set data buffer size */
    item->data = ptr +  sizeof(deo_memp_item_t); /* Set data buffer pointer */
    item->mark = DEO_MEMP_MARK_FREE;             /* Mark item as free */
    item->next = NULL;                           /* Set no next item */

    /* Set item as first in variable-size list */
    deo_memp_data.freelist_head[DEO_MEMP_CONST_LIST_CNT] = item;

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
    /* Clear statistics */
    deo_os_memset(&deo_memp_stat, 0, sizeof(deo_memp_stat));

    deo_memp_stat.list_block_cnt_cur[DEO_MEMP_CONST_LIST_CNT] = 1;
    deo_memp_stat.list_block_cnt_max[DEO_MEMP_CONST_LIST_CNT] = 1;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_memp_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        return DEO_ERROR;
    }

    return DEO_SUCCESS;
}

/**
 * @brief       Get an item from a const-size free-list
 *
 * @param[in]   list_index  Index of const-size free-list to find the item in
 *
 * @return      If succeeded pointer to an item, NULL otherwise
 */
static deo_memp_item_t *deo_memp_get_const_size_item(uint32 list_index)
{
    deo_memp_item_t **head = &(deo_memp_data.freelist_head[list_index]);
    deo_memp_item_t  *item;

    if (NULL == *head)  /* Check for empty list */
    {
        return NULL;    /* List is empty */
    }

    /* List has items, remove first item */
    item  = *head;              /* Set found item (the head) */
    *head = (*head)->next;      /* Update head, remove from list */
    item->next = NULL;          /* Clear pointer to rest of list */

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
    /* Count one less item at the const-sized list */
    deo_memp_stat.list_block_cnt_cur[list_index]--;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

    /* Sanity checks */
    DEO_ASSERT((*head != item),
               ("DEO_MEM: Circular item detected\n"));
    DEO_ASSERT((DEO_MEMP_ITEM_PTR_TO_DATA_PTR(item) == item->data),
               ("DEO_MEM: Item corrupted in free-list (const size)\n"));
    DEO_ASSERT((DEO_MEMP_MARK_FREE == item->mark),
               ("DEO_MEM: Item marked used in free-list (const size)\n"));

    item->mark = DEO_MEMP_MARK_ALLOC;     /* Mark item as allocated */

    return item;
}

/**
 * @brief       Get an item from the variable-size list. If a suitable item
 *              cannot be found, a big enough item will be split. If no item
 *              can be split then fail
 *
 * @param[in]   size    Size of requested data block, in bytes
 *
 * @return      If succeeded pointer to an item, NULL otherwise
 */
static deo_memp_item_t *deo_memp_get_variable_size_item(uint32 size)
{
    deo_memp_item_t **head =
                       &(deo_memp_data.freelist_head[DEO_MEMP_CONST_LIST_CNT]);
    deo_memp_item_t  *item = NULL;
    deo_memp_item_t **item_fit = NULL;
    deo_memp_item_t **item_min = NULL;
    deo_memp_item_t **curr = head;

    /* Allow oversize of double required minus one, but not more than limit */
    uint32 oversize = MIN(size * 2, size + DEO_MEMP_MAX_SURPLUS + 1);

    /* Search the mempool free-list for suitable item */
    while (NULL != *curr)
    {
        if ((*curr)->size >= size)          /* Check if size is enough */
        {
            if ((*curr)->size < oversize)   /* Check for oversize */
            {
                item_fit = curr;            /* Set: item found */
                break;
            }
            else
            {
                /* Item is over-sized, but keep if minimal */
                if ((NULL == item_min) ||
                    ((*curr)->size < (*item_min)->size))
                {
                    item_min = curr;        /* Set: minimal over-sized item */
                }
            }
        }

        /* Continue to next item */
        curr = &((*curr)->next);
    }

    /* Check if we found a suitable item */
    if (NULL != item_fit)
    {
        /* Found appropriate item, remove from the list */
        item = *item_fit;               /* Set found item */
        *item_fit = (*item_fit)->next;  /* Remove from the list */
        item->next = NULL;              /* Clear pointer to rest of list */

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
        /* Count one less item at the variable-size-list */
        deo_memp_stat.list_block_cnt_cur[DEO_MEMP_CONST_LIST_CNT]--;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */
    }
    else if (NULL != item_min) /* Check if found an over-sized item */
    {
        /* Split an over-sized item */

        /* Calculate alloc size, while preserving minimal size align */
        uint32 min_split_size;
        uint32 alloc_size = deo_mem_size_align_up(size);

        /* Calculate minimal size eligible for split */
        min_split_size =
               alloc_size + sizeof(deo_memp_item_t) + DEO_MEMP_MIN_BLOCK_SPLIT;

        DEO_ASSERT((0 == deo_mem_size_align_offset(min_split_size)),
                   ("DEO_MEM: invalid min_split_size\n"));

        /* Check if we can split */
        if ((*item_min)->size < min_split_size)
        {
            /* Can't split: use minimal block as-is, remove from the list */
            item = *item_min;               /* Set found item */
            *item_min = (*item_min)->next;  /* Remove from the list */
            item->next = NULL;              /* Clear pointer to rest of list */

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
            /* Count one less item at the variable-size-list */
            deo_memp_stat.list_block_cnt_cur[DEO_MEMP_CONST_LIST_CNT]--;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */
        }
        else
        {
            /* New name for simplicity */
            deo_memp_item_t *item_org = (*item_min);
            uint32 item_org_orig_size = item_org->size;

            /* Split
             * One big item in the list is split as follows:
             * - Original item remains in the list, with reduced size
             * - New item returned to user
             */

            /* Update item in the list - all the same, just size changes */
            item_org->size -= (alloc_size + sizeof(deo_memp_item_t));

            /* Prepare the new item */
            item = (deo_memp_item_t *)(((uint8 *)item_org) +
                                   (sizeof(deo_memp_item_t) + item_org->size));
            item->size = alloc_size;
            item->data = ((uint8 *)item) + sizeof(deo_memp_item_t);
            item->mark = DEO_MEMP_MARK_FREE;
            item->next = NULL;

            /* Sanity checks for remaining original item
             * (for new item see below)
             */
            DEO_ASSERT((0 == deo_mem_ptr_align_mempool_offset(item_org)),
                       ("DEO_MEM: invalid remaining ptr\n"));
            DEO_ASSERT((0 == deo_mem_ptr_align_mempool_offset(item_org->data)),
                       ("DEO_MEM: invalid remaining data ptr\n"));
            DEO_ASSERT((0 == deo_mem_size_align_offset(item_org->size)),
                       ("DEO_MEM: invalid remaining size\n"));

            /* Sanity checks, overall */
            DEO_ASSERT((item_org_orig_size ==
                       (item_org->size + item->size + sizeof(deo_memp_item_t))),
                       ("DEO_MEM: overall size mismatch\n"));
        }
    }

    /* Check if we have allocation */
    if (NULL == item)
    {
        return NULL;    /* Failed, no suitable allocation */
    }

    /* Sanity checks for item */
    DEO_ASSERT((DEO_MEMP_ITEM_PTR_TO_DATA_PTR(item) == item->data),
               ("DEO_MEM: Item corrupted in free-list (var size)\n"));
    DEO_ASSERT((DEO_MEMP_MARK_FREE == item->mark),
               ("DEO_MEM: Item marked used in free-list (var size)\n"));

    item->mark = DEO_MEMP_MARK_ALLOC;           /* Item is now allocated */

    return item;
}

/**
 * @brief       Allocate a block from the mempool, the algorithm is as follows:
 *              1. Find the smallest const-size list with (const_size >= size)
 *                 and try to allocate from it, if successful, return the block
 *              2. Allocate from the variable-size list
 *
 * @param[in]   size    requested block size, in bytes
 * @param[in]   flags   requested flags
 *
 * @return      If succeeded pointer to the allocated block, NULL otherwise
 */
static void *deo_memp_alloc(uint32 size, uint16 flags)
{
    uint32 list;
    deo_memp_item_t *item = NULL;

    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&deo_memp_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex lock failure\n"));
        return NULL;
    }

    /* Find smallest matching const-size list and try to alloc from it */
    for (list = 0; list < DEO_MEMP_CONST_LIST_CNT; list++)
    {
        if (deo_memp_alloc_sizes[list] >= size)
        {
            /* Adjust size to const-list size. If needed, this will be
             * the size to allocate from the variable-size list
             */
            size = deo_memp_alloc_sizes[list];
            item = deo_memp_get_const_size_item(list);
            break;
        }
    }

    /* Not allocated by const-size lists, try variable-size list */
    if (NULL == item)
    {
        item = deo_memp_get_variable_size_item(size);

        if (NULL == item)                           /* No allocation found */
        {
            if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_memp_mutex))
            {
                DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
            }
            return NULL;                              /* Allocation Failed */
        }
    }

    /* Sanity checks for allocated item */
    DEO_ASSERT((0 == deo_mem_ptr_align_mempool_offset(item)),
               ("DEO_MEM: alloc: invalid item ptr\n"));
    DEO_ASSERT((NULL != item->data),
               ("DEO_MEM: alloc: NULL item data ptr\n"));
    DEO_ASSERT((0 == deo_mem_ptr_align_mempool_offset(item->data)),
               ("DEO_MEM: alloc: invalid item data ptr\n"));
    DEO_ASSERT((0 == deo_mem_size_align_offset(item->size)),
               ("DEO_MEM: alloc: invalid item size\n"));

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
    /* Update allocation size statistics.
     * Count real amount: overhead + allocated memory.
     */
    deo_memp_stat.total_alloc_size_cur += sizeof(deo_memp_item_t) + item->size;
    deo_memp_stat.total_alloc_size_max =
                            MAX(deo_memp_stat.total_alloc_size_max,
                                deo_memp_stat.total_alloc_size_cur);
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

    if (0 != (flags & M_ZERO))
    {
        deo_os_memset(item->data, 0, size);
    }

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_memp_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        return NULL;
    }

    return item->data;
}

/**
 * @brief       Internal implementation of the mempool allocation function.
 *
 * @note        Assumes that #VVDRV_deo_mem_init() was called before calling
 *              this function
 *
 * @warning     Must not call any DMA functions, only mempool functions
 *
 * @param[in]   size    Size of the required buffer, in bytes
 * @param[in]   flags   Allocation flags
 *
 * @return      NULL if failed, else pointer to the allocated buffer
 */
static void *deo_alloc_internal(uint32 size, uint16 flags)
{
    /* Make sure we allocate at least DEO_MEMP_MIN_BLOCK size
     * This also handles a request for empty block
     */
    size = MAX(size, DEO_MEMP_MIN_BLOCK);

    /* allocate data */
    return deo_memp_alloc(size, flags);
}

#else /* !DEO_CONFIG_MEMPOOL */

/**
 * @brief       Internal implementation of the mempool allocation function.
 *
 * @note        Assumes that #VVDRV_deo_mem_init() was called before calling
 *              this function
 *
 * @warning     Must not call any DMA functions, only mempool functions
 *
 * @param[in]   size    Size of the required buffer, in bytes
 * @param[in]   flags   Allocation flags
 *
 * @return      NULL if failed, else pointer to the allocated buffer
 */
static void *deo_alloc_internal(uint32 size, uint16 flags)
{
    void *pBuffer = malloc(size);

    if ((NULL != pBuffer) && (0 != (flags & M_ZERO)))
    {
        deo_os_memset(pBuffer, 0, size);
    }

    return pBuffer;
}

#endif /* DEO_CONFIG_MEMPOOL */

/*----- DMA mempool only -----------------------------------------------------*/
#ifdef DEO_CONFIG_POOL_DMA

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW

/**
 * @brief       Prints out the DMA mempool statistics
 *
 * @param[in]   pool_idx    Pool index to print statistics of
 */
static void deo_dma_dump_status(uint32 pool_idx)
{
    const deo_dma_stat_t *dma_stat = &(deo_dma_pool[pool_idx].stats);

    DEO_DBG_I(DEO_MEM, ("DEO_MEM: ===== DMA pool #%u statistics start\n",
                        pool_idx));

    DEO_DBG_I(DEO_MEM,
        ("DEO_MEM: Total allocated memory (bytes): Max %lu, Current %lu\n",
                        dma_stat->total_alloc_size_max,
                        dma_stat->total_alloc_size_cur));

    DEO_DBG_I(DEO_MEM,
        ("DEO_MEM: Allocated blocks: Max %u, Current %u\n",
                        dma_stat->alloc_block_cnt_max,
                        dma_stat->alloc_block_cnt_cur));

    DEO_DBG_I(DEO_MEM,
        ("DEO_MEM: Free blocks: Max %u, Current %u\n",
                        dma_stat->free_block_cnt_max,
                        dma_stat->free_block_cnt_cur));

    DEO_DBG_I(DEO_MEM,
        ("DEO_MEM: Total blocks: Max %u, Current %u\n",
                        dma_stat->total_block_cnt_max,
                        dma_stat->total_block_cnt_cur));

    DEO_DBG_I(DEO_MEM, ("DEO_MEM: Failed alloc requests: %u\n",
                        dma_stat->failed_alloc_cnt));

    DEO_DBG_I(DEO_MEM, ("DEO_MEM: ===== DMA pool #%u statistics end\n",
                        pool_idx));

    /* If DEO debug level excludes INFO level then above debug messages are
     * removed and we get warning that variable is unused, so add the
     * following to remove the warning
     */
    (void)dma_stat;
}

#endif  /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

/**
 * @brief       Check DMA block and returns the block type and relevant DMA
 *              pool
 *
 *              Check is done by values of the linked-list element members
 *              "le_prev" and "le_next" pointers, as follows:
 *
 *  Type  | le_next                       | le_prev
 * -------+-------------------------------+----------------------------
 *  Free  | Next element (possibly NULL)  | Previous element (never NULL)
 *  Taken | Pointer to DMA pool struct    | Pointer to DMA pool struct
 *  Dummy | Pointer to DMA block struct   | Pointer to DMA block struct
 *
 * @param[in]   dma     DMA block
 * @param[out]  pool    Pointer to relevant DMA pool. Ignored if NULL. For
 *                      dummy block, the "pool" is returned as NULL.
 *
 * @return      Type of the block, one of DEO_DMA_BLOCK_TYPE_INVALID,
 *              DEO_DMA_BLOCK_TYPE_TAKEN, DEO_DMA_BLOCK_TYPE_DUMMY,
 *              DEO_DMA_BLOCK_TYPE_FREE
 */
static uint32 deo_dma_check_block_type
(
    const deo_dma_block_t  *dma,
    deo_dma_pool_t        **pool
)
{
    uint32          block_type = DEO_DMA_BLOCK_TYPE_INVALID;
    deo_dma_pool_t *pool_tmp   = NULL;

    /* First check if the handle (pointer) we got is valid */
#ifdef DEO_CONFIG_MEMPOOL
    if (TRUE != deo_memp_is_valid_ptr(dma)) /* Check for valid mempool ptr */
#else /* !DEO_CONFIG_MEMPOOL */
    if (NULL == dma)                    /* No mempool, just check for NULL */
#endif /* DEO_CONFIG_MEMPOOL */
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: Invalid DMA handle 0x%p\n", dma));
    }
    else
    {
        void *prev = dma->next.le_prev;
        void *next = dma->next.le_next;

        if (prev == next)           /* Either Taken, Dummy or Invalid block */
        {
            if (dma != prev)        /* Either Taken or Invalid block */
            {
                pool_tmp = (deo_dma_pool_t *)prev;

                /* Check that DMA pool handle points to valid, created pool */
                if ((pool_tmp >= &(deo_dma_pool[0])) &&
                    (pool_tmp <=
                            &(deo_dma_pool[DEO_MEMPOOL_DMA_POOL_COUNT - 1])) &&
                    (TRUE == pool_tmp->is_created))
                {
                    block_type = DEO_DMA_BLOCK_TYPE_TAKEN;
                }
                else
                {
                    pool_tmp   = NULL;
                    block_type = DEO_DMA_BLOCK_TYPE_INVALID;
                }
            }
            else /* ((prev == next) && (dma == prev))   so Dummy block */
            {
                block_type = DEO_DMA_BLOCK_TYPE_DUMMY;
            }
        }
        else                        /* Free */
        {
            block_type = DEO_DMA_BLOCK_TYPE_FREE;
        }
    }

    /* Assign pool. Note that Dummy and Free blocks do not have pool info */
    if (NULL != pool)
    {
        *pool = pool_tmp;
    }

    return block_type;
}

/**
 * @brief       Set DMA block to a specific type
 *              See deo_dma_check_block_type() documentation for specifics
 *
 * @param[in]   dma         DMA block to assign type to
 * @param[in]   pool        Pointer to relevant DMA pool
 * @param[in]   block_type  Block type to assign, must be either
 *                          DEO_DMA_BLOCK_TYPE_TAKEN or
 *                          DEO_DMA_BLOCK_TYPE_DUMMY
 *
 * @note        This function is not called to assign DEO_DMA_BLOCK_TYPE_FREE,
 *              this type is implicitly set by inserting the block into the
 *              free-list
 */
static void deo_dma_set_block_type
(
    deo_dma_block_t *dma,
    deo_dma_pool_t  *pool,
    uint32           block_type
)
{
    if (DEO_DMA_BLOCK_TYPE_TAKEN == block_type)
    {
        dma->next.le_prev = (deo_dma_block_t **)pool;
        dma->next.le_next = (deo_dma_block_t *)pool;
    }
    else if (DEO_DMA_BLOCK_TYPE_DUMMY == block_type)
    {
        dma->next.le_prev = (deo_dma_block_t **)dma;
        dma->next.le_next = (deo_dma_block_t *)dma;
    }
    else if (DEO_DMA_BLOCK_TYPE_INVALID == block_type)
    {
        /* Note: this is only one possibility of invalid */
        dma->next.le_prev = NULL;
        dma->next.le_next = NULL;
    }
    else
    {
        /* Function must not be called for other block types! */
        DEO_ASSERT(((void)0, 0), ("DEO_MEM: Invalid set block type\n"));
    }
}

/**
 * @brief       Inserts the given DMA block to head of DMA free-list
 *
 * @param[in]   dma     DMA block to be added to DMA free-list
 * @param[in]   pool    DMA pool which free-list will be used
 */
static void deo_dma_block_insert_to_freelist
(
    deo_dma_block_t *dma,
    deo_dma_pool_t  *pool
)
{
    deo_dma_freelist_t *dma_freelist = &(pool->freelist);
    deo_dma_stat_t     *dma_stat     = &(pool->stats);

#ifdef DEO_CONFIG_DMA_DEBUG
   ((uint32 *)(dma->vaddr))[0] = DMA_CORRUPT_CHECK_MAGIC;
#endif /* DEO_CONFIG_DMA_DEBUG */

    /* Insert to free-list. Note that by doing this, future calls to
     * to deo_dma_check_block_type() will return type free
     */
    LIST_INSERT_HEAD(dma_freelist, dma, next);
    dma_stat->free_block_cnt_cur++;

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
    /* Update statistics */
    dma_stat->free_block_cnt_max = MAX(dma_stat->free_block_cnt_max,
                                       dma_stat->free_block_cnt_cur);
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */
}

/**
 * @brief       Calculates the best (biggest) alignment of a given address
 *
 * @param[in]   address     Given address
 *
 * @return      Best (biggest) alignment of the given address
 */
static uint32 deo_dma_calc_align(const void *address)
{
    /* Note: when we calculate a 32 bit alignment of a 64-bit
     * address, it is enough to check the lowest 32-bits
     */
    const uint32 addr = (((size_t)address) & 0xFFFFFFFF);

    /* Calculate best (biggest) buffer alignment. Note carefully that this
     * is the same as finding the least significant bit set which
     * is calculated by (x & ~(x-1))
     */
    uint32 align = (addr & ~(addr - 1));

    /* Alignments in allocator must be at least the minimal */
    DEO_ASSERT((0 == (addr % DEO_CONFIG_MEMPOOL_ALIGN)),
               ("DEO_MEM: Invalid align found\n"));

    /* Handle corner cases:
     * - Do not allow alignment bigger than max align
     * - Address zero will return zero align - adjust to max align.
     */
    if ((DEO_DMA_MEMPOOL_MAX_ALIGN < align) || (0 == align))
    {
        align = DEO_DMA_MEMPOOL_MAX_ALIGN;
    }

    return align;
}

/**
 * @brief       Checks if a given memory block is page aligned.
 *              A block is page aligned if it starts and ends in the same page.
 *              Page size is defined by macro #DEO_PAGE_SIZE.
 *
 * @param[in]   address     start address of the memory block
 * @param[in]   size        size of the memory block
 *
 * @return      TRUE if page is aligned, FALSE otherwise
 */
static boolean deo_dma_is_page_aligned(const void *address, uint32 size)
{
    /* Check if block does not cross page boundary */
    if (size <= DEO_PAGE_SIZE)    /* Lightweight check for negative cases */
    {
        size_t addr = (size_t)address;
        if (((addr % DEO_PAGE_SIZE) + size) <= DEO_PAGE_SIZE)
        {
            return TRUE;
        }

        /* An alternative formula */
        /*
        if ((addr / DEO_PAGE_SIZE) == ((addr + size - 1) / DEO_PAGE_SIZE))
        {
            return TRUE;
        }
        */
    }

    return FALSE;
}

/**
 * @brief       Checks if the pool handle is valid and returns pointer to pool
 *
 * @param[in]   pool_handle     DEO DMA pool handle
 *
 * @return      Pointer to pool struct if successful, otherwise NULL
 */
static deo_dma_pool_t *deo_dma_pool_is_valid_handle
(
    deo_dma_pool_handle_t pool_handle
)
{
    deo_dma_pool_t *pool = NULL;

    /* NOTE: a handle is the (pool array index) + 1 */
    if ((DEO_MEMPOOL_DMA_POOL_INVALID == pool_handle) ||
        (DEO_MEMPOOL_DMA_POOL_COUNT   <  pool_handle))
    {
        DEO_DBG_E(DEO_MEM,
            ("DEO_MEM: Invalid pool handle (%u)\n", (uint32)pool_handle));
        return NULL;        /* Invalid handle */
    }

    pool = &(deo_dma_pool[((uint32)pool_handle) - 1]);

    /* Check if pool was not created */
    if (TRUE != pool->is_created)
    {
        DEO_DBG_E(DEO_MEM,
            ("DEO_MEM: Pool not created (handle %u)\n", (uint32)pool_handle));
        return NULL;        /* Invalid handle */
    }

    return pool;            /* Valid handle */
}

/**
 * @brief       Create a new free DMA block from a memory region
 *
 * @warning     The passed size and pointers must be valid for a new block
 *
 * @param[in]   pool    Pointer to relevant DMA pool
 * @param[in]   size    Size of available memory region
 * @param[in]   vaddr   Virtual address of start of memory region
 * @param[in]   paddr   Physical address of start of memory region
 *
 * @return      DEO_SUCCESS if successful, otherwise failed
 */
static int32 deo_dma_block_create_free
(
    deo_dma_pool_t *pool,
    uint32          size,
    uint8          *vaddr,
    uint8          *paddr
)
{
    deo_dma_block_t *dma;

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
    deo_dma_stat_t *dma_stat = &(pool->stats);
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

    DEO_ASSERT((DEO_DMA_MIN_BLOCK <= size),
        ("DEO_MEM: invalid create block size\n"));

    DEO_ASSERT((0 == deo_mem_ptr_align_mempool_offset(paddr)),
               ("DEO_MEM: invalid create block align\n"));

    /* Allocate management block */
    dma = (deo_dma_block_t *)deo_alloc_internal(sizeof(deo_dma_block_t),
                                                M_ZERO);
    if (NULL == dma)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: out of mempool memory\n"));
        return DEO_ERROR;
    }

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
    dma_stat->total_block_cnt_cur++;
    dma_stat->total_block_cnt_max = MAX(dma_stat->total_block_cnt_max,
                                        dma_stat->total_block_cnt_cur);
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

    dma->vaddr = vaddr;
    dma->paddr = paddr;
    dma->size  = size;
    dma->align = deo_dma_calc_align(dma->paddr);

    deo_dma_block_insert_to_freelist(dma, pool);

    return DEO_SUCCESS;
}

#ifdef DEO_DMA_DEFRAG_DEBUG
/**
 * @brief       Prints out the DMA freelist
 *
 * @param[in]   dma_freelist    DMA free-list to print
 */
static void deo_dma_output_freelist(const deo_dma_freelist_t *dma_freelist)
{
    const deo_dma_block_t *tmp_block = NULL;
    uint32 total_free_blocks = 0;
    uint32 total_free_bytes  = 0;

    DEO_DBG_I(DEO_MEM, ("DEO_MEM: --- DMA Freelist start\n"));
    for (tmp_block = LIST_FIRST(dma_freelist);
         NULL != tmp_block;
         tmp_block = LIST_NEXT(tmp_block, next))
    {
        DEO_DBG_I(DEO_MEM,
                  ("DEO_MEM: Block #%3u, start 0x%5p, size %10u, align %7u\n",
                  total_free_blocks, tmp_block->paddr, tmp_block->size,
                  tmp_block->align));
        total_free_blocks++;
        total_free_bytes += tmp_block->size;
    }
    DEO_DBG_I(DEO_MEM, ("DEO_MEM: ---------------------------------------\n"));
    DEO_DBG_I(DEO_MEM, ("DEO_MEM: Total %u blocks, %u bytes\n",
                                    total_free_blocks, total_free_bytes));
    DEO_DBG_I(DEO_MEM, ("DEO_MEM: ---Free-list end\n"));
}
#endif /* DEO_DMA_DEFRAG_DEBUG */

#ifdef DEO_MEM_STAT_DEBUG

/**
 * @brief       Counts the number of elements in pool's dma_freelist
 *
 * @note        Used for statistics debug only
 *
 * @param[in]   dma_freelist      Pointer to DMA free-list
 *
 * @return      Number of elements in pool's dma_freelist
 */
static uint32 deo_dma_calc_freelist_cnt(const deo_dma_freelist_t *dma_freelist)
{
    const deo_dma_block_t *tmp_block = NULL;
    uint32 cnt = 0;

    for (tmp_block = LIST_FIRST(dma_freelist);
         NULL != tmp_block;
         tmp_block = LIST_NEXT(tmp_block, next))
    {
        cnt++;
    }

    return cnt;
}

/**
 * @brief       Check DMA statistics validity
 *
 * @param[in]   pool    Pointer to relevant DMA pool
 *
 * @note        Used for statistics debug only
 */
static void deo_dma_check_stats(const deo_dma_pool_t *pool)
{
    const deo_dma_freelist_t *dma_freelist = &(pool->freelist);
    const deo_dma_stat_t     *dma_stat     = &(pool->stats);
    uint32 tmp_cnt = deo_dma_calc_freelist_cnt(dma_freelist);

    DEO_ASSERT((tmp_cnt == dma_stat->free_block_cnt_cur),
               ("DEO_MEM: DMA free-list mismatch: Counter %u, Counted: %u\n",
               dma_stat->free_block_cnt_cur, tmp_cnt));

    DEO_ASSERT(
        (dma_stat->total_block_cnt_cur ==
         (dma_stat->free_block_cnt_cur + dma_stat->alloc_block_cnt_cur)),
        ("DEO_MEM: DMA block count mismatch. Total: %u, free %u, alloc: %u\n",
         dma_stat->total_block_cnt_cur,
         dma_stat->free_block_cnt_cur,
         dma_stat->alloc_block_cnt_cur));
}

#endif /* DEO_MEM_STAT_DEBUG */

/**
 * @brief       Find the free block with lowest address
 *
 * @param[in]   dma_freelist    Pointer to DMA free-list to use
 *
 * @return      DMA free block with lowest address, or NULL if free list is
 *              empty
 */
static deo_dma_block_t *deo_dma_find_lowest_free_block
(
    const deo_dma_freelist_t *dma_freelist
)
{
    deo_dma_block_t *tmp_block = NULL;
    deo_dma_block_t *low_block = NULL;

    for (low_block = tmp_block = LIST_FIRST(dma_freelist);
         NULL != tmp_block;
         tmp_block = LIST_NEXT(tmp_block, next))
    {
        if (tmp_block->paddr < low_block->paddr)
        {
            low_block = tmp_block;
        }
    }

    return low_block;
}

/**
 * @brief       De-fragments a DMA pool free-list
 *
 * @param[in]   pool    Pointer to relevant DMA pool
 *
 * @return      DEO_SUCCESS if successful, otherwise failed
 */
static int32 deo_dma_pool_defrag(deo_dma_pool_t *pool)
{
    deo_dma_freelist_t *dma_freelist = &(pool->freelist);
    deo_dma_block_t    *tmp_block = NULL;
    deo_dma_block_t    *cur_block = NULL;
    deo_dma_block_t    *nxt_block = NULL;
    deo_dma_stat_t     *dma_stat = &(pool->stats);

#ifdef DEO_DMA_DEFRAG_DEBUG
    uint32 cnt = 0;
#endif /* DEO_DMA_DEFRAG_DEBUG */

#ifdef DEO_DMA_DEFRAG_DEBUG
    {
        const uint32 pool_idx =
            (uint32)(((uint8 *)pool - (uint8 *)&(deo_dma_pool[0])) /
                     sizeof(deo_dma_pool[0]));
        DEO_DBG_I(DEO_MEM, ("DEO_MEM: *** Defragmenting DMA memory pool #%u\n",
                  pool_idx));
        deo_dma_output_freelist(dma_freelist);      /* Output by list order */
        DEO_DBG_I(DEO_MEM,
                  ("DEO_MEM: --- deo_dma_pool_defrag - Ordered start\n"));

        /* If DEO debug level excludes INFO level then above debug messages are
         * removed and we get warning that variable is unused, so add the
         * following to remove the warning
         */
        (void)pool_idx;
    }
#endif /* DEO_DMA_DEFRAG_DEBUG */

    /* Find the first block, with smallest address */
    cur_block = deo_dma_find_lowest_free_block(dma_freelist);

#ifdef DEO_DMA_DEFRAG_DEBUG
    if (NULL != cur_block)
    {
        DEO_DBG_I(DEO_MEM,
         ("DEO_MEM: Block #%3u, start 0x%5p, size %10u, next 0x%5p\n",
          cnt,
          cur_block->paddr,
          cur_block->size,
          (cur_block->paddr + cur_block->size)));
        cnt++;
    }
#endif /* DEO_DMA_DEFRAG_DEBUG */

    while (NULL != cur_block)
    {
        boolean can_defrag = FALSE;
        nxt_block = NULL;

        /* Find next block with smallest address still bigger than current */
        for (tmp_block = LIST_FIRST(dma_freelist);
             NULL != tmp_block;
             tmp_block = LIST_NEXT(tmp_block, next))
        {
            if (tmp_block->paddr > cur_block->paddr)
            {
                if ((NULL == nxt_block) ||
                    (tmp_block->paddr < nxt_block->paddr))
                {
                    nxt_block = tmp_block;
                }
            }
        }

        if (NULL != nxt_block)
        {
            /* Calculate size of the gap between end of current block and start
             * of next block (possibly gap has size zero).
             * If gap is big enough for a block then that block is still
             * allocated - cannot defrag current and next blocks.
             * Otherwise, gap is a "hole" - can defrag current and next blocks.
             * Calculation as follows:
             * gap_start = cur_end + 1 = cur_start + cur_size - 1 + 1
             *           = cur_start + cur_size
             * gap_end   = nxt_start - 1
             * gap_size  = gap_end -  gap_start + 1 =
             *           = nxt_start - (cur_start + cur_size - 1) - 1 =
             *           = nxt_start - cur_start - cur_size
             */
            const size_t gap_size = nxt_block->paddr - cur_block->paddr - 
                                    cur_block->size;
            can_defrag = FALSE;
            if (DEO_DMA_MIN_BLOCK > gap_size)
            {
                /* Defrag two blocks only if total size <= (max uint32) */
                if (((~((uint32)0)) - cur_block->size) >= nxt_block->size)
                {
                    can_defrag = TRUE;
                }
            }

#ifdef DEO_DMA_DEFRAG_DEBUG
            DEO_DBG_I(DEO_MEM,
                ("DEO_MEM: Cur block  start 0x%5p, end 0x%5p, size %10u\n",
                 cur_block->paddr, cur_block->paddr + cur_block->size - 1,
                 cur_block->size));
            DEO_DBG_I(DEO_MEM,
                ("DEO_MEM: Nxt block  start 0x%5p, end 0x%5p, size %10u\n",
                 nxt_block->paddr, nxt_block->paddr + nxt_block->size - 1,
                 nxt_block->size));
            DEO_DBG_I(DEO_MEM, ("DEO_MEM: Can Defrag %u\n", can_defrag));
            cnt++;
#endif /* DEO_DMA_DEFRAG_DEBUG */
        }

        if (FALSE == can_defrag)
        {
            cur_block = nxt_block; /* Continue from next block */
        }
        else
        {
            /* Coalesce current and next blocks */

            LIST_REMOVE(nxt_block, next);/* Remove next block from free list */
            dma_stat->free_block_cnt_cur--;     /* Count one less free block */

            /* Add to current block size the gap between the blocks
             * (if exists) and the whole next block. Calculate as follows:
             * nxtEnd = nxt_block->paddr + (nxt_block->size - 1)
             * curEnd = cur_block->paddr + (cur_block->size - 1)
             * cur_block->size += nxtEnd - curEnd
             * cur_block->size += (nxt_block->paddr + nxt_block->size - 
             *                     cur_block->paddr - cur_block->size)
             */
            cur_block->size += (uint32)(nxt_block->paddr + nxt_block->size -
                                        cur_block->paddr - cur_block->size);

            /* Current block paddr, vaddr and align stay as before */

            /* Free handle of next block. No special handling if failed */
            (void)VVDRV_deo_free(nxt_block);

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
            dma_stat->total_block_cnt_cur--;      /* Count one less block */
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

#ifdef DEO_DMA_DEFRAG_DEBUG
            DEO_DBG_I(DEO_MEM, (
               "DEO_MEM: Coalesced cur block  start %5p, end %5p, size %10u\n",
               cur_block->paddr, cur_block->paddr + cur_block->size - 1,
               cur_block->size));
#endif /* DEO_DMA_DEFRAG_DEBUG */

            /* Continue iterating from current block */
        }
    }

#ifdef DEO_DMA_DEFRAG_DEBUG
    DEO_DBG_I(DEO_MEM, ("DEO_MEM: Total %u blocks\n", cnt));
    DEO_DBG_I(DEO_MEM, ("DEO_MEM: --- deo_dma_pool_defrag - Ordered End\n"));
#endif /* DEO_DMA_DEFRAG_DEBUG */

    return DEO_SUCCESS;
}

/**
 * @brief       Find an existing block with enough size and matching alignment
 *
 * @param[in]   pool    Pointer to relevant DMA pool
 * @param[in]   size    Requested block size
 * @param[in]   align   Requested block alignment
 * @param[in]   flags   Requested block flags
 *
 * @return      Pointer to appropriate block if one found, NULL otherwise
 */
static deo_dma_block_t *deo_dma_find_free_block
(
    deo_dma_pool_t *pool,
    uint32          size,
    uint32          align,
    uint16          flags
)
{
    deo_dma_block_t *cur_block;
    deo_dma_freelist_t *dma_freelist = &(pool->freelist);
    boolean need_page_align = FALSE;
    uint32 oversize = 0;

    if (0 != (flags & M_PAGE_ALIGN))
    {
        need_page_align = TRUE;
    }

    /* Allow oversize up to double required, but not more than limit */
    oversize = MIN(size * 2, size + DEO_DMA_MAX_SURPLUS + 1);

    /* Scan the free list and look for a fit */
    for (cur_block = LIST_FIRST(dma_freelist);
         NULL != cur_block;
         cur_block = LIST_NEXT(cur_block, next))
    {
#ifdef DEO_CONFIG_DMA_DEBUG
        if (((uint32 *)(cur_block->vaddr))[0] != DMA_CORRUPT_CHECK_MAGIC)
        {
            DEO_DBG_E(DEO_MEM,
                ("DEO_MEM: DMA block 0x%p or freelist became corrupted\n",
                cur_block));
            return NULL;
        }
#endif /* DEO_CONFIG_DMA_DEBUG */

        /* Make sure the alignment requirement is met */
        if (cur_block->align < align)
        {
            continue;
        }

        /* Check for enough size */
        if (cur_block->size < size)
        {
            continue;
        }

        /* Check if block is too big */
        if (cur_block->size >= oversize)
        {
            continue;
        }

        /* Make sure buffer is page aligned, if required */
        if (TRUE == need_page_align)
        {
            if (FALSE == deo_dma_is_page_aligned(cur_block->paddr, size))
            {
                continue; /* This block with requested size not page aligned */
            }
        }

        break;      /* FOUND ! */
    }

    if (NULL != cur_block)
    {
        deo_dma_stat_t *dma_stat = &(pool->stats);

        LIST_REMOVE(cur_block, next);
        dma_stat->free_block_cnt_cur--;
        deo_dma_set_block_type(cur_block, pool, DEO_DMA_BLOCK_TYPE_TAKEN);
    }

    return cur_block;
}

/**
 * @brief       Find an existing block with bigger size and correct alignment.
 *              Split block and recover surplus space at end of block.
 *
 * @param[in]   pool    Pointer to relevant DMA pool
 * @param[in]   size    Requested block size
 * @param[in]   align   Requested block alignment
 * @param[in]   flags   Requested block flags
 *
 * @return      Pointer to appropriate block if one found, NULL otherwise
 */
static deo_dma_block_t *deo_dma_find_free_block_split
(
    deo_dma_pool_t *pool,
    uint32          size,
    uint32          align,
    uint16          flags
)
{
    deo_dma_block_t *cur_block;
    deo_dma_freelist_t *dma_freelist = &(pool->freelist);
    boolean need_page_align = FALSE;

    if (0 != (flags & M_PAGE_ALIGN))
    {
        need_page_align = TRUE;
    }

    /* Scan the free list and look for a block to split */
    for (cur_block = LIST_FIRST(dma_freelist);
         NULL != cur_block;
         cur_block = LIST_NEXT(cur_block, next))
    {
        /* Make sure the alignment requirement is met */
        if (cur_block->align < align)
        {
            continue;
        }

        /* We need a block with enough size */
        if (cur_block->size < size)
        {
            continue;
        }

        if (TRUE == need_page_align)
        {
            /* Check if cur_block can be split to get a page-aligned buffer */
            if (FALSE == deo_dma_is_page_aligned(cur_block->paddr, size))
            {
                continue;   /* Cannot cut the block to be page aligned */
            }
        }

        break;      /* FOUND ! */
    }

    /* If suitable block found, handle accordingly */
    if (NULL != cur_block)
    {
        deo_dma_stat_t *dma_stat = &(pool->stats);
        const uint32 new_block_size = cur_block->size - size;

        LIST_REMOVE(cur_block, next);
        dma_stat->free_block_cnt_cur--;
        deo_dma_set_block_type(cur_block, pool, DEO_DMA_BLOCK_TYPE_TAKEN);

        /* If enough size left at block end create new block from it.
         * If not, keep the original block size
         */
        if (DEO_DMA_MIN_BLOCK <= new_block_size)
        {
            if (DEO_SUCCESS != deo_dma_block_create_free(
                                                    pool,
                                                    new_block_size,
                                                    cur_block->vaddr + size,
                                                    cur_block->paddr + size))
            {
                /* Error, so return the DMA block to free list to prevent a
                 * memory leak
                 */
                deo_dma_block_insert_to_freelist(cur_block, pool);

                return NULL;    /* Error */
            }

            /* Adjust found block size */
            cur_block->size -= new_block_size;
        }
    }

    return cur_block;
}

/**
 * @brief       Find an existing block with enough size but mismatched
 *              alignment. Split, re-align and recover surplus space at start
 *              and end of block
 *
 * @param[in]   pool    Pointer to relevant DMA pool
 * @param[in]   size    requested block size
 * @param[in]   align   requested block alignment
 * @param[in]   flags   requested block flags
 *
 * @return      Pointer to appropriate block if one found, NULL otherwise
 */
static deo_dma_block_t *deo_dma_find_free_block_realign
(
    deo_dma_pool_t *pool,
    uint32          size,
    uint32          align,
    uint16          flags
)
{
    deo_dma_block_t *cur_block;
    deo_dma_freelist_t *dma_freelist = &(pool->freelist);
    uint32 alloc_size;
    boolean need_page_align = FALSE;

    if (0 != (flags & M_PAGE_ALIGN))
    {
        need_page_align = TRUE;
        alloc_size = (2 * size) + align;        /* Page align request */
    }
    else
    {
        alloc_size = size + align - 1;
    }

    /* Scan the free list and look for a block to re-align */
    for (cur_block = LIST_FIRST(dma_freelist);
         NULL != cur_block;
         cur_block = LIST_NEXT(cur_block, next))
    {
        /* No need to check alignment, since we will re-align */

        /* We need a block with enough size */
        if (cur_block->size < alloc_size)
        {
            continue;
        }

        break;      /* FOUND ! */
    }

    /* If suitable block found, handle accordingly */
    if (NULL != cur_block)
    {
        deo_dma_stat_t *dma_stat = &(pool->stats);
        uint32 needed_offset = 0;
        uint32 new_block_size  = 0;

        LIST_REMOVE(cur_block, next);
        dma_stat->free_block_cnt_cur--;
        deo_dma_set_block_type(cur_block, pool, DEO_DMA_BLOCK_TYPE_TAKEN);

        /* Check if new block must not to cross page boundaries */
        if (FALSE == need_page_align)
        {
            /* Simple case, no page align: Realign the found buffer */
            uint32 current_align = deo_mem_ptr_align_offset(cur_block->paddr,
                                                            align);

            if (0 != current_align)
            {
                needed_offset += align - current_align;
            }
        }
        else
        {
            /* Block must not cross page boundaries
             * Note: no need to worry from data loss since DEO_PAGE_SIZE is
             *       at most 4GByte
             */
            uint32 paddr_remaider = deo_mem_ptr_align_offset(cur_block->paddr,
                                                             DEO_PAGE_SIZE);
            uint32 tmp_align;

            /* We have two cases when considering the PAGE_ALIGN requirement,
             * and each case has two sub-cases
             *
             ****** First case, sub case 1:
             * In this case we find the needed buffer in the first page out
             * of the current block, and also the alignments fit
             *  __                                        __
             * |              cur_block                     |    cur_block
             * |              start                         |    end
             * |                 |                          |      |
             * |_paddr_remider___|___newBlock___ |_________________|
             * |                 |   size        |          |      |
             * |                 |               |          |
             * |              newBlock      newBlock        |
             * |__            start         end           __|
             * 0                               (PAGE_SIZE -1)
             *
             ****** First case, sub case 2:
             * In this case we find the needed buffer in the first page out
             * of the current block but the the alignments do not fit
             * (Note that current block end might be inside or outside of page)
             *  __                                                          __
             * |              cur_block                            cur_block  |
             * |              start                                     end   |
             * |                 |                                        |   |
             * |_paddr_remider___|_(align -          ___|__newBlock__|____|   |
             * |                 |  cur_block->align)   |  size      |    |   |
             * |                                        |            |        |
             * |                                  newBlock      newBlock      |
             * |__                                start         end         __|
             * 0                                                 (PAGE_SIZE -1)
             *
             * In the first case we have enough space in cur_block, so we only
             * have to adjust the alignment - same as in any other block
             *
             * Notice that not necessarily align > cur_block->align, this
             * could be FALSE when ever we are in the second case. In this case
             * the align variable does not take any part in the calculation.
             *
             ****** Second case, sub case 1:
             * In the second case, we provide the new block from the second
             * page, and the new start will be at the start of the second page
             *
             *  __                          __  __                          __
             * |            cur_block         ||                              |
             * |            start             ||                              |
             * |              |               ||                              |
             * |              |_____align_____||___size___||___size___|       |
             * |                              ||                              |
             * |__                          __||__                          __|
             * 0     ...        (PAGE_SIZE -1)  PAGE_SIZE ...  (2*PAGE_SIZE -1)
             *
             ****** Second case, sub case 2:
             *
             *  __                          __  __                          __
             * |  cur_block                   ||                              |
             * |  start                       ||                              |
             * |   |                          ||                              |
             * |   |___align___|______size________||______size________|       |
             * |                              ||                              |
             * |__                          __||__                          __|
             * 0     ...        (PAGE_SIZE -1)  PAGE_SIZE ...  (2*PAGE_SIZE -1)
             *
             * In this case, in order to meet the page align requirement, we
             * must change start the new block from the beginning of the page.
             *
             * Notice that we will not exceed the cur_block size since we
             * required that:
             *     cur_block->size > (2 * size) + align
             */

            /* Compare alignments, to find the first needed offset */
            tmp_align = 0;
            if (align > cur_block->align)
            {
                tmp_align = (uint32)align - (uint32)cur_block->align;
            }

            /* If (start + offset + size) > PAGE_SIZE, this is second case */
            if ((paddr_remaider + tmp_align + size - 1) > DEO_PAGE_SIZE)
            {
                needed_offset = DEO_PAGE_SIZE - paddr_remaider; /* Next page */
            }

            /* Realign the found buffer */
            tmp_align =
             deo_mem_ptr_align_offset(cur_block->paddr + needed_offset, align);
            if (0 != tmp_align)
            {
                needed_offset += align - tmp_align;
            }
        }

        /* If enough size available at block start, create new block from it */
        new_block_size = needed_offset;
        if (DEO_DMA_MIN_BLOCK <= new_block_size)
        {
            if (DEO_SUCCESS != deo_dma_block_create_free(pool,
                                                         new_block_size,
                                                         cur_block->vaddr,
                                                         cur_block->paddr))
            {
                /* Error, so return the DMA block to free list to prevent a
                 * memory leak.
                 */
                deo_dma_block_insert_to_freelist(cur_block, pool);

                return NULL;    /* Error */
            }
        }

        /* Adjust found block pointers, size and align
         * Note that we adjust even if a new block was not created, since
         * the pointers most be moved to the needed alignment
         */
        cur_block->vaddr += needed_offset;
        cur_block->paddr += needed_offset;
        cur_block->size  -= needed_offset;
        cur_block->align  = deo_dma_calc_align(cur_block->paddr);

        /* If enough size available at block end, create new block from it.
         * If not, keep the original block size
         */
        new_block_size = cur_block->size - size;
        if (DEO_DMA_MIN_BLOCK <= new_block_size)
        {
            if (DEO_SUCCESS != deo_dma_block_create_free(
                                                    pool,
                                                    new_block_size,
                                                    cur_block->vaddr + size,
                                                    cur_block->paddr + size))
            {
                /* Error, so return the DMA block to free list to prevent a
                 * memory leak.
                 * Notice that in this case we only need to revert the end of
                 * the block.
                 */
                deo_dma_block_insert_to_freelist(cur_block, pool);

                return NULL;    /* Error */
            }

            /* Adjust found block size */
            cur_block->size -= new_block_size;
        }
    }

    return cur_block;
}

/**
 * @brief       Remove all allocated DMA blocks. Has no effect on the mempool
 *              DMA memory, but frees the management block from the mempool
 *              memory and sets counters correctly
 *
 * @param[in]   pool    Pointer to relevant DMA pool
 *
 * @warning     This function is only valid to call from #VVDRV_deo_mem_uninit()
 */
static void deo_dma_remove_all(deo_dma_pool_t *pool)
{
    deo_dma_freelist_t *dma_freelist = &(pool->freelist);
    deo_dma_stat_t     *dma_stat     = &(pool->stats);
    deo_dma_block_t    *dma;

    /* Iterate all blocks */
    for (dma = LIST_FIRST(dma_freelist);
         NULL != dma;
         dma = LIST_FIRST(dma_freelist))
    {
        LIST_REMOVE(dma, next);                   /* Remove from the list */
        dma_stat->free_block_cnt_cur--;      /* Count one less free block */

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
        dma_stat->total_block_cnt_cur--;          /* Count one less block */
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

        (void)VVDRV_deo_free(dma);           /* Free the management block */
    }
}

#endif /* DEO_CONFIG_POOL_DMA */

/*------------------------------------------------------------------------------
 * Externalized Function Definitions
 *----------------------------------------------------------------------------*/

/*----- Mempool only ---------------------------------------------------------*/
#ifndef DEO_CONFIG_MEMPOOL

void *VVDRV_deo_alloc(uint32 size, uint16 flags)
{
    if (FALSE == deo_mem_initialized)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_alloc: Not initialized\n"));
        return NULL;
    }

   return deo_alloc_internal(size, flags);
}

int32 VVDRV_deo_free(void *ptr)
{
    if (FALSE == deo_mem_initialized)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_free: Not initialized\n"));
        return DEO_ERROR;
    }

    if (NULL != ptr)
    {
        free(ptr);
    }

    return DEO_SUCCESS;
}

#else /* defined(DEO_CONFIG_MEMPOOL) */

void *VVDRV_deo_alloc(uint32 size, uint16 flags)
{
    void *ret = NULL;

    if (FALSE == deo_mem_initialized)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_alloc: Not initialized\n"));
        return NULL;
    }

    /* Try to allocate */
    ret = deo_alloc_internal(size,flags);
    if (NULL == ret)
    {
        /* Try to defrag DMA, which may free some mempool memory */
        if (DEO_SUCCESS == VVDRV_deo_mem_defrag())
        {
            /* Retry after defrag */
            ret = deo_alloc_internal(size,flags);
            if (NULL == ret)
            {
                DEO_DBG_I(DEO_MEM,
                          ("DEO_MEM: deo_alloc: no mem after defrag\n"));
            }
        }
        /* else (DEO_SUCCESS != VVDRV_deo_mem_defrag())- ret is still NULL */
    }

    /* Final result */
    if (NULL == ret)
    {
#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
        deo_memp_stat.failed_alloc_cnt++;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_alloc: out of memory\n"));
    }

#ifdef DEO_MEM_STAT_DEBUG
    deo_memp_check_stats();
#endif /* DEO_MEM_STAT_DEBUG */

    return ret;
}

int32 VVDRV_deo_free(void *ptr)
{
    uint32 size;
    uint32 list;
    deo_memp_item_t *item;

    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&deo_memp_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex lock failure\n"));
        return DEO_ERROR;
    }

    if (FALSE == deo_mem_initialized)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_free: Not initialized\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_memp_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    if (TRUE != deo_memp_is_valid_ptr(ptr))
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_free: Invalid free pointer\n"));

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
        deo_memp_stat.failed_free_cnt++;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_memp_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR; /* Tried to free pointer not from deo_alloc() */
    }

    /* Get pointer to item information */
    item = DEO_MEMP_DATA_PTR_TO_ITEM_PTR(ptr);

    /* Check if item is corrupted */
    if (ptr != item->data)
    {
        DEO_DBG_E(DEO_MEM,
                ("DEO_MEM: deo_free: tried to free corrupted block\n"));

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
        deo_memp_stat.failed_free_cnt++;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_memp_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR; /* Tried to free a corrupted block */
    }

    /* Check if block is marked as allocated */
    if (DEO_MEMP_MARK_ALLOC != item->mark)
    {
        /* Ignore block which is already free */
        if (DEO_MEMP_MARK_FREE == item->mark)
        {
            DEO_DBG_E(DEO_MEM,
                ("DEO_MEM: deo_free: tried to free an already free block\n"));
        }
        else /* ((DEO_MEMP_MARK_ALLOC != item->free) &&
                 (DEO_MEMP_MARK_FREE  != item->free))    */
        {
            /* Unexpected value, block info is corrupted */
            DEO_DBG_E(DEO_MEM,
                    ("DEO_MEM: deo_free: tried to free corrupted block\n"));
        }

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
        deo_memp_stat.failed_free_cnt++;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_memp_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;       /* Tried to free a free or corrupted block */
    }

    /* Find appropriate free-list for the item */
    size = item->size;
    for (list = 0; list < DEO_MEMP_CONST_LIST_CNT; list++)
    {
        if (deo_memp_alloc_sizes[list] == size)
        {
            break;
        }
    }

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
    deo_memp_stat.list_block_cnt_cur[list]++;
    deo_memp_stat.list_block_cnt_max[list] =
                        MAX(deo_memp_stat.list_block_cnt_max[list],
                            deo_memp_stat.list_block_cnt_cur[list]);
    deo_memp_stat.total_alloc_size_cur -= sizeof(deo_memp_item_t) + item->size;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

    /* Insert item into head of free items list */
    item->next = deo_memp_data.freelist_head[list];
    deo_memp_data.freelist_head[list] = item;
    item->mark = DEO_MEMP_MARK_FREE;

#ifdef DEO_MEM_STAT_DEBUG
    deo_memp_check_stats();
#endif /* DEO_MEM_STAT_DEBUG */

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_memp_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        return DEO_ERROR;
    }

    return DEO_SUCCESS;
}

#endif /* DEO_CONFIG_MEMPOOL */

/*----- DMA mempool only -----------------------------------------------------*/
#ifndef DEO_CONFIG_POOL_DMA

int32 deo_dma_alloc
(
    uint32             size,
    uint32             align,
    void             **vaddr,
    void             **paddr,
    uint16             flags,
    deo_dma_handle_t  *handle
)
{
    (void)size;     /* Unused */
    (void)align;    /* Unused */
    (void)vaddr;    /* Unused */
    (void)paddr;    /* Unused */
    (void)flags;    /* Unused */
    (void)handle;   /* Unused */

    return DEO_ERROR;   /* Not supported, DEO_CONFIG_POOL_DMA not defined */
}

int32 VVDRV_deo_dma_free(deo_dma_handle_t handle)
{
    (void)handle;   /* Unused */

    return DEO_ERROR;   /* Not supported, DEO_CONFIG_POOL_DMA not defined */
}

int32 VVDRV_deo_dma_add_block
(
   uint32  size,
   void   *vaddr,
   void   *paddr,
   uint16  flags
)
{
    (void)size;     /* Unused */
    (void)vaddr;    /* Unused */
    (void)paddr;    /* Unused */
    (void)flags;    /* Unused */

    return DEO_ERROR;   /* Not supported, DEO_CONFIG_POOL_DMA not defined */
}

int32 VVDRV_deo_dma_dummy_set
(
    uint32            size,
    void             *vaddr,
    void             *paddr,
    deo_dma_handle_t  handle
)
{
    (void)size;     /* Unused */
    (void)vaddr;    /* Unused */
    (void)paddr;    /* Unused */
    (void)handle;   /* Unused */

    return DEO_ERROR;   /* Not supported, DEO_CONFIG_POOL_DMA not defined */
}

void *VVDRV_deo_dma_phys_addr(deo_dma_handle_t handle, uint64 offset)
{
    (void)handle;   /* Unused */
    (void)offset;   /* Unused */

    return NULL;        /* Not supported, DEO_CONFIG_POOL_DMA not defined */
}

void *VVDRV_deo_dma_virt_addr(deo_dma_handle_t handle, uint64 offset)
{
    (void)handle;   /* Unused */
    (void)offset;   /* Unused */

    return NULL;        /* Not supported, DEO_CONFIG_POOL_DMA not defined */
}

int32 VVDRV_deo_dma_pool_create(deo_dma_pool_handle_t *pool)
{
    (void)pool;   /* Unused */

    return DEO_ERROR;   /* Not supported, DEO_CONFIG_POOL_DMA not defined */
}

int32 VVDRV_deo_dma_pool_release(deo_dma_pool_handle_t *pool)
{
    (void)pool;   /* Unused */

    return DEO_ERROR;   /* Not supported, DEO_CONFIG_POOL_DMA not defined */
}

int32 VVDRV_deo_dma_pool_add_block
(
    deo_dma_pool_handle_t pool,
    uint32                size,
    void                 *vaddr,
    void                 *paddr,
    uint16                flags
)
{
    (void)pool;     /* Unused */
    (void)size;     /* Unused */
    (void)vaddr;    /* Unused */
    (void)paddr;    /* Unused */
    (void)flags;    /* Unused */

    return DEO_ERROR;   /* Not supported, DEO_CONFIG_POOL_DMA not defined */
}

int32 VVDRV_deo_dma_pool_alloc
(
    deo_dma_pool_handle_t   pool,
    uint32                  size,
    uint32                  align,
    void                  **vaddr,
    void                  **paddr,
    uint16                  flags,
    deo_dma_handle_t       *handle
)
{
    (void)pool;     /* Unused */
    (void)size;     /* Unused */
    (void)align;    /* Unused */
    (void)vaddr;    /* Unused */
    (void)paddr;    /* Unused */
    (void)flags;    /* Unused */
    (void)handle;   /* Unused */

    return DEO_ERROR;   /* Not supported, DEO_CONFIG_POOL_DMA not defined */
}

#else /* defined DEO_CONFIG_POOL_DMA */

int32 VVDRV_deo_dma_pool_create(deo_dma_pool_handle_t *pool)
{
    uint32 pool_idx;

    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&deo_dmap_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex lock failure\n"));
        return DEO_ERROR;
    }

    if (FALSE == deo_mem_initialized)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_dma_pool_create: Not initialized\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    if (NULL == pool)
    {
        DEO_DBG_E(DEO_MEM,
                  ("DEO_MEM: deo_dma_pool_create: NULL pool handle\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Search pool array and create the first non-created pool
     *
     * NOTE CAREFULLY: if this is the first time the API is called after
     * the service is initialized, the created pool MUST be the default
     * DMA pool, as specified by DEO_MEMPOOL_DMA_POOL_DEFAULT
     */
    for (pool_idx = 0; pool_idx < DEO_MEMPOOL_DMA_POOL_COUNT; pool_idx++)
    {
        deo_dma_pool_t *pool_ptr = &(deo_dma_pool[pool_idx]);
        if (TRUE != pool_ptr->is_created)
        {
            /* Found available pool, create it */
            DEO_ASSERT((TRUE == LIST_EMPTY(&(pool_ptr->freelist))),
                       ("DEO_MEM: freelist mismatch\n"));
            deo_os_memset(&(pool_ptr->stats), 0, sizeof(pool_ptr->stats));
            pool_ptr->is_created = TRUE;

            /* NOTE: a handle is the (pool array index) + 1 */
            *pool = (deo_dma_pool_handle_t)(pool_idx + 1);
            if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
            {
                DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
                return DEO_ERROR;
            }
            return DEO_SUCCESS;
        }
    }

    DEO_DBG_E(DEO_MEM,
        ("DEO_MEM: No DMA pool left. Increase DEO_MEMPOOL_DMA_POOL_COUNT\n"));

    *pool = DEO_MEMPOOL_DMA_POOL_INVALID;

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
    }

    return DEO_ERROR;
}

int32 VVDRV_deo_dma_pool_release(deo_dma_pool_handle_t *pool)
{
    deo_dma_pool_t *pool_ptr = NULL;

    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&deo_dmap_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex lock failure\n"));
        return DEO_ERROR;
    }

    if (FALSE == deo_mem_initialized)
    {
        DEO_DBG_E(DEO_MEM,("DEO_MEM: deo_dma_pool_release: Not initialized\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    if (NULL == pool)
    {
        DEO_DBG_E(DEO_MEM,
                  ("DEO_MEM: deo_dma_pool_release: NULL pool handle\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Also validates the integrity of the pool handler */
    pool_ptr = deo_dma_pool_is_valid_handle(*pool);
    if (NULL == pool_ptr)
    {
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Free all elements in free-list */
    deo_dma_remove_all(pool_ptr);

    /* Mark not created */
    pool_ptr->is_created = FALSE;

    /* Note: stats not zeroed to allow further debug. Stats are zeroed at
     * pool create
     */

    *pool = DEO_MEMPOOL_DMA_POOL_INVALID;

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        return DEO_ERROR;
    }

    return DEO_SUCCESS;
}

int32 VVDRV_deo_dma_alloc
(
    uint32             size,
    uint32             align,
    void             **vaddr,
    void             **paddr,
    uint16             flags,
    deo_dma_handle_t  *handle
)
{
    return VVDRV_deo_dma_pool_alloc(
       DEO_MEMPOOL_DMA_POOL_DEFAULT, size, align, vaddr, paddr, flags, handle);
}

int32 VVDRV_deo_dma_pool_alloc
(
    deo_dma_pool_handle_t   pool,
    uint32                  size,
    uint32                  align,
    void                  **vaddr,
    void                  **paddr,
    uint16                  flags,
    deo_dma_handle_t       *handle
)
{
    deo_dma_block_t *dma = NULL;
    deo_dma_pool_t  *pool_ptr = NULL;

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
    deo_dma_stat_t  *dma_stat = NULL;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&deo_dmap_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex lock failure\n"));
        return DEO_ERROR;
    }

    if (NULL == handle)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_dma_pool_alloc: NULL handle\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Set handle to NULL, so users which do not check the return code will
     * not use a previous handle by mistake, in case of function failure
     */
    *handle = NULL;

    if (FALSE == deo_mem_initialized)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_dma_pool_alloc: Not initialized\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    pool_ptr = deo_dma_pool_is_valid_handle(pool);
    if (NULL == pool_ptr)
    {
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
    dma_stat = &(pool_ptr->stats);
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

    /* Handle empty dummy blocks */
    if (0 == size)
    {
        dma = (deo_dma_block_t *)deo_alloc_internal(sizeof(deo_dma_block_t),
                                                    M_ZERO);
        if (NULL == dma)
        {
#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
            dma_stat->failed_alloc_cnt++;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */
            DEO_DBG_E(DEO_MEM,
                ("DEO_MEM: deo_dma_pool_alloc: out of mempool memory\n"));

            if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
            {
                DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
            }
            return DEO_ERROR;
        }

        /* Mark block as dummy. Note that dummy block cannot be in free list */
        deo_dma_set_block_type(dma, NULL, DEO_DMA_BLOCK_TYPE_DUMMY);

        *handle = (deo_dma_handle_t)dma;

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
        /* Update dummy blocks statistics */
        deo_dma_dummy_alloc_block_cnt_cur++;
        deo_dma_dummy_alloc_block_cnt_max =
                                        MAX(deo_dma_dummy_alloc_block_cnt_max,
                                            deo_dma_dummy_alloc_block_cnt_cur);
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
            return DEO_ERROR;
        }
        return DEO_SUCCESS;
    }

    /* Make sure we allocate at least DMA_MEM_BLOCK size */
    size = MAX(size, DEO_DMA_MIN_BLOCK);

    /* Make sure size is aligned */
    size = deo_mem_size_align_up(size);

    /* Check that required alignment is power of 2 or zero */
    if (((0 != align) && (0 != (align & (align - 1)))) ||
        (align > DEO_DMA_MEMPOOL_MAX_ALIGN))
    {
        DEO_DBG_E(DEO_MEM,
         ("DEO_MEM: Invalid align, must be 0 or power of 2, max %u\n",
          DEO_DMA_MEMPOOL_MAX_ALIGN));

        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Make alignment reasonable */
    align = MAX(align, DEO_CONFIG_MEMPOOL_ALIGN);

    /* M_CACHABLE deprecated and ignored */
    if (0 != (flags & M_CACHABLE))
    {
        flags &= ~M_CACHABLE;

        DEO_DBG_V(DEO_MEM, ("DEO_MEM: M_CACHABLE flag ignored\n"));
    }

    /* If required to page align, verify size is within limit */
    if ((0 != (flags & M_PAGE_ALIGN)) && (size > DEO_PAGE_SIZE))
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: Cannot page-align if size>page_size\n"));

        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Try to find an existing buffer on the free list */
    dma = deo_dma_find_free_block(pool_ptr, size, align, flags);
    if (NULL == dma)
    {
        /* Try to split a buffer from the free list */
        dma = deo_dma_find_free_block_split(pool_ptr, size, align, flags);
        if (NULL == dma)
        {
            /* Try to re-align a buffer from the free list */
            dma = deo_dma_find_free_block_realign(pool_ptr, size, align, flags);
            if (NULL == dma)
            {
                if (DEO_SUCCESS != deo_dma_pool_defrag(pool_ptr))
                {
                    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
                    {
                        DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
                    }
                    /* deo_dma_pool_defrag() doesn't allocate, so cannot fail
                     * due to out-of memory, so do not increment
                     * failed_alloc_cnt
                     */
                    return DEO_ERROR;
                }

                /* Retry to find an existing buffer on the free list */
                dma = deo_dma_find_free_block(pool_ptr, size, align, flags);
                if (NULL == dma)
                {
                    dma = deo_dma_find_free_block_split(pool_ptr,size, align,
                                                        flags);
                    if (NULL == dma)
                    {
                        dma = deo_dma_find_free_block_realign(pool_ptr, size,
                                                              align, flags);
                        if (NULL == dma)
                        {
#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
                            dma_stat->failed_alloc_cnt++;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */
                            DEO_DBG_E(DEO_MEM,
                             ("DEO_MEM: deo_dma_pool_alloc: Out of memory\n"));

                            if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
                            {
                                DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
                            }
                            return DEO_ERROR;
                        }
                    }
                }
            }
        }
    }

    /* Suitable block found */

    *handle = (deo_dma_handle_t)dma;

    if (NULL != vaddr)
    {
        *vaddr = dma->vaddr;
    }

    if (NULL != paddr)
    {
        *paddr = dma->paddr;
    }

    if (0 != (flags & M_ZERO))
    {
        deo_os_memset(dma->vaddr, 0, size);
    }

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
    dma_stat->total_alloc_size_cur += dma->size;
    dma_stat->total_alloc_size_max = MAX(dma_stat->total_alloc_size_max,
                                         dma_stat->total_alloc_size_cur);
    dma_stat->alloc_block_cnt_cur++;
    dma_stat->alloc_block_cnt_max = MAX(dma_stat->alloc_block_cnt_max,
                                        dma_stat->alloc_block_cnt_cur);
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

    /* Sanity check - size */
    if (size > dma->size)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: Alloc buffer size mismatch\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Sanity check - align */
    if (0 != deo_mem_ptr_align_offset(dma->paddr, align))
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: Alloc buffer align mismatch\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Sanity check - page align */
    if (0 != (flags & M_PAGE_ALIGN))
    {
        if (TRUE != deo_dma_is_page_aligned(dma->paddr, size))
        {
            DEO_DBG_E(DEO_MEM, ("DEO_MEM: Alloc buffer page align mismatch\n"));
            if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
            {
                DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
            }
            return DEO_ERROR;
        }
    }

#ifdef DEO_MEM_STAT_DEBUG
    deo_dma_check_stats(pool_ptr);
#endif /* DEO_MEM_STAT_DEBUG */

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        return DEO_ERROR;
    }

    return DEO_SUCCESS;
}

int32 VVDRV_deo_dma_free(deo_dma_handle_t handle)
{
    deo_dma_block_t *dma        = (deo_dma_block_t *)handle;
    deo_dma_pool_t  *pool       = NULL;
    uint32           block_type = DEO_DMA_BLOCK_TYPE_INVALID;
    int32            ret        = DEO_ERROR;

    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&deo_dmap_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex lock failure\n"));
        return DEO_ERROR;
    }

    if (FALSE == deo_mem_initialized)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_dma_free: Not initialized\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Get DMA block type and pool pointer */
    block_type = deo_dma_check_block_type(dma, &pool);

    /* Check if valid (taken) block */
    if (DEO_DMA_BLOCK_TYPE_TAKEN == block_type)
    {
        deo_dma_stat_t *dma_stat = &(pool->stats);

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
        dma_stat->total_alloc_size_cur -= dma->size;
        dma_stat->alloc_block_cnt_cur--;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

        deo_dma_block_insert_to_freelist(dma, pool);
        dma = NULL;

#ifdef DEO_MEM_STAT_DEBUG
        deo_dma_check_stats(pool);
#endif /* DEO_MEM_STAT_DEBUG */

        /* Check if auto-defrag limits passed */
        if (DEO_DMA_AUTO_DEFRAG_LIMIT_FREE < dma_stat->free_block_cnt_cur)
        {
            /* Defragment DMA pool. Note: even if auto-defrag failed, the
             * dma free worked correctly, so no change to return value
             */
            (void)deo_dma_pool_defrag(pool);
        }

        ret = DEO_SUCCESS; /* Success - block was freed */
    }
    else if (DEO_DMA_BLOCK_TYPE_DUMMY == block_type) /* Check if dummy block */
    {
        /* Handle free of dummy blocks */
        if (0 != dma->size)
        {
            DEO_DBG_E(DEO_MEM,
               ("DEO_MEM: deo_dma_free: Dummy block not reset before free\n"));
        }

        /* Make block as invalid, so it cannot be used after being freed */
        deo_dma_set_block_type(dma, NULL, DEO_DMA_BLOCK_TYPE_INVALID);

        /* Free management block. Even if fails, still one less DMA block */
        (void)VVDRV_deo_free(dma);
        dma = NULL;

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
        /* In this case we have one less block in the pool */
        deo_dma_dummy_alloc_block_cnt_cur--;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

        ret = DEO_SUCCESS; /* Success - block was freed */
    }
    else    /* We have an error */
    {
        if (DEO_DMA_BLOCK_TYPE_FREE == block_type)  /* Block already free? */
        {
            DEO_DBG_E(DEO_MEM,
             ("DEO_MEM: deo_dma_free: tried to free an already free block\n"));
        }
        else /* (DEO_DMA_BLOCK_TYPE_INVALID == block_type) - Invalid block */
        {
            DEO_DBG_E(DEO_MEM,
             ("DEO_MEM: deo_dma_free: invalid DMA handle data\n"));
        }

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
        deo_dma_failed_free_cnt++;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */
    }

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        return DEO_ERROR;
    }

    return ret;
}

int32 VVDRV_deo_dma_add_block
(
    uint32  size,
    void   *vaddr,
    void   *paddr,
    uint16  flags
)
{
    return VVDRV_deo_dma_pool_add_block(
                    DEO_MEMPOOL_DMA_POOL_DEFAULT, size, vaddr, paddr, flags);
}

int32 VVDRV_deo_dma_pool_add_block
(
    deo_dma_pool_handle_t pool,
    uint32                size,
    void                 *vaddr,
    void                 *paddr,
    uint16                flags
)
{
    deo_dma_pool_t  *pool_ptr = NULL;
    uint8           *phys_ptr = (uint8 *)paddr;
    uint8           *virt_ptr = (uint8 *)vaddr;
    int32            ret;

    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&deo_dmap_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex lock failure\n"));
        return DEO_ERROR;
    }

    if (FALSE == deo_mem_initialized)
    {
        DEO_DBG_E(DEO_MEM,
                  ("DEO_MEM: deo_dma_pool_add_block: Not initialized\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    pool_ptr = deo_dma_pool_is_valid_handle(pool);
    if (NULL == pool_ptr)
    {
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Check if size is at least minimal */
    if (size < DEO_DMA_MIN_BLOCK)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: DMA block too small to add\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Allocator considers zero (NULL) an invalid address. If user wants to
     * start from zero address, adjust forward
     */
    if ((NULL == phys_ptr) || (NULL == virt_ptr))
    {
        size     -= DEO_CONFIG_MEMPOOL_ALIGN;
        phys_ptr += DEO_CONFIG_MEMPOOL_ALIGN;
        virt_ptr += DEO_CONFIG_MEMPOOL_ALIGN;
    }
    else
    {
        /* If needed align ptr */
        uint32 offset = deo_mem_ptr_align_mempool_offset(phys_ptr);
        if (0 != offset)
        {
            /* Offset pointers forward to re-align */
            offset = DEO_CONFIG_MEMPOOL_ALIGN - offset;
            size     -= offset;
            phys_ptr += offset;
            virt_ptr += offset;
        }
    }

    /* If needed, align size down - every block size is aligned */
    size -= deo_mem_size_align_offset(size);

    /* Check if size is still at least minimal */
    if (DEO_DMA_MIN_BLOCK > size)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: DMA block too small to add\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Zero block, if requested */
    if (0 != (flags & M_ZERO))
    {
        deo_os_memset(virt_ptr, 0, size);
    }

    /* Create a new free block from the memory */
    ret = deo_dma_block_create_free(pool_ptr, size, virt_ptr, phys_ptr);

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        return DEO_ERROR;
    }

    return ret;
}

int32 VVDRV_deo_dma_dummy_set
(
    uint32            size,
    void             *vaddr,
    void             *paddr,
    deo_dma_handle_t  handle
)
{
    deo_dma_block_t *dma = (deo_dma_block_t *)handle;

    if (DEO_DMA_BLOCK_TYPE_DUMMY != deo_dma_check_block_type(dma, NULL))
    {
        DEO_DBG_E(DEO_MEM,
            ("DEO_MEM: deo_dma_dummy_set: invalid or non-dummy block\n"));
        return DEO_ERROR;
    }

    dma->paddr = (uint8 *)paddr;
    dma->vaddr = (uint8 *)vaddr;
    dma->size  = size;

    return DEO_SUCCESS;
}

void *VVDRV_deo_dma_phys_addr(deo_dma_handle_t handle, uint64 offset)
{
    const deo_dma_block_t *dma = (deo_dma_block_t *)handle;

    /* This function might be called often, so perform just lightweight check,
     * unless the DMA debug flag is defined
     */
#ifdef DEO_CONFIG_DMA_DEBUG
    uint32 block_type = deo_dma_check_block_type(dma, NULL); /* Check block */

    if ((DEO_DMA_BLOCK_TYPE_TAKEN != block_type) &&
        (DEO_DMA_BLOCK_TYPE_DUMMY != block_type))
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_dma_phys_addr: invalid handle\n"));
        return NULL;        /* Failure */
    }
#else /* (!DEO_CONFIG_DMA_DEBUG) */
    if (NULL == dma)                           /* Minimal lightweight check */
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_dma_phys_addr: NULL handle\n"));
        return NULL;        /* Failure */
    }
#endif /* DEO_CONFIG_DMA_DEBUG */

    return (void *)(dma->paddr + offset);
}

void *VVDRV_deo_dma_virt_addr(deo_dma_handle_t handle, uint64 offset)
{
    const deo_dma_block_t *dma = (deo_dma_block_t *)handle;

    /* This function might be called often, so perform just lightweight check,
     * unless the DMA debug flag is defined
     */
#ifdef DEO_CONFIG_DMA_DEBUG
    uint32 block_type = deo_dma_check_block_type(dma, NULL); /* Check block */

    if ((DEO_DMA_BLOCK_TYPE_TAKEN != block_type) &&
        (DEO_DMA_BLOCK_TYPE_DUMMY != block_type))
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_dma_virt_addr: invalid handle\n"));
        return NULL;        /* Failure */
    }
#else /* (!DEO_CONFIG_DMA_DEBUG) */
    if (NULL == dma)                           /* Minimal lightweight check */
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_dma_virt_addr: NULL handle\n"));
        return NULL;        /* Failure */
    }
#endif /* DEO_CONFIG_DMA_DEBUG */

    return (void *)(dma->vaddr + offset);
}

#endif /* DEO_CONFIG_POOL_DMA */

/*----- Common: both mempool and DMA mempool ---------------------------------*/

int32 VVDRV_deo_mem_init(const deo_mempool_args_t *args)
{
#ifdef DEO_CONFIG_POOL_DMA
    uint32 pool_idx;
#endif /* DEO_CONFIG_POOL_DMA */

    if (TRUE == deo_mem_initialized)
    {
        return DEO_SUCCESS;
    }

    /* Initialize mutex services */
    if (DEO_SUCCESS != VVDRV_deo_mutex_init())
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex initialization failure\n"));
        return DEO_ERROR;
    }

    /* If mempool defined, verify mempool arguments */
#ifdef DEO_CONFIG_MEMPOOL
    if (NULL == args)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_mem_init: NULL param\n"));
        return DEO_ERROR;
    }

    if ((NULL == args->mem_buf) || (0 == args->mem_buf_size))
    {
        DEO_DBG_E(DEO_MEM,
            ("DEO_MEM: deo_mem_init: mem_buf is NULL or mem_buf_size is 0\n"));
        return DEO_ERROR;
    }
#endif /* DEO_CONFIG_MEMPOOL */

    /* If DMA pool defined, verify DMA pool arguments */
#ifdef DEO_CONFIG_POOL_DMA
    if ((NULL != args) &&
        (0    != args->dma_buf_size) &&
        (NULL == args->dma_vbuf))
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_mem_init: dma_vbuf is NULL\n"));
        return DEO_ERROR;
    }
#endif /* DEO_CONFIG_POOL_DMA */

    /* Check for valid DEO_CONFIG_MEMPOOL_ALIGN definition */
    if ((((void)0, 0) != (DEO_CONFIG_MEMPOOL_ALIGN % sizeof(void *))) ||
        (((void)0, 0) != (DEO_CONFIG_MEMPOOL_ALIGN &
                          (DEO_CONFIG_MEMPOOL_ALIGN - 1))))
    {
        DEO_DBG_E(DEO_MEM,
                  ("DEO_MEM: invalid value of DEO_CONFIG_MEMPOOL_ALIGN\n"));
        return DEO_ERROR;
    }

    /* Initialize mempool */
#if defined(DEO_CONFIG_MEMPOOL)
    if (DEO_SUCCESS != deo_memp_init(args->mem_buf, args->mem_buf_size))
    {
        return DEO_ERROR;
    }
#endif /* DEO_CONFIG_MEMPOOL */

    /* Initialize DMA mempools */
#ifdef DEO_CONFIG_POOL_DMA

    if (DEO_SUCCESS != VVDRV_deo_mutex_create(&deo_dmap_mutex, 
                                              DEO_MUTEX_ATTR_CRITICAL_SECTION))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex create failure\n"));

#if defined(DEO_CONFIG_MEMPOOL)
        if (DEO_SUCCESS != VVDRV_deo_mutex_destroy(&deo_memp_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
#endif /* DEO_CONFIG_MEMPOOL */

        return DEO_ERROR;
    }

    deo_os_memset(&deo_dma_pool, 0, sizeof(deo_dma_pool));

    for (pool_idx = 0; pool_idx < DEO_MEMPOOL_DMA_POOL_COUNT; pool_idx++)
    {
        LIST_INIT(&(deo_dma_pool[pool_idx].freelist));
    }

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
    /* Zero DMA general statistics counter */
    deo_dma_failed_free_cnt           = 0;
    deo_dma_dummy_alloc_block_cnt_cur = 0;
    deo_dma_dummy_alloc_block_cnt_max = 0;
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */

    /* User allowed to not specify DMA pool now, but create it later by
     * calling #VVDRV_deo_dma_pool_create()
     */
    if ((NULL != args) && (0 < args->dma_buf_size))
    {
        deo_dma_pool_handle_t pool_handle = DEO_MEMPOOL_DMA_POOL_INVALID;

        /* Mark as initialized, so #VVDRV_deo_dma_pool_create() will work */
        deo_mem_initialized = TRUE;

        /* Create default DMA pool */
        if (DEO_SUCCESS != VVDRV_deo_dma_pool_create(&pool_handle))
        {
            deo_mem_initialized = FALSE;

            if (DEO_SUCCESS != VVDRV_deo_mutex_destroy(&deo_dmap_mutex))
            {
                DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
            }
#if defined(DEO_CONFIG_MEMPOOL)
            if (DEO_SUCCESS != VVDRV_deo_mutex_destroy(&deo_memp_mutex))
            {
                DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
            }
#endif /* DEO_CONFIG_MEMPOOL */

            return DEO_ERROR;
        }

        /* Sanity check that indeed created default DMA pool */
        DEO_ASSERT((DEO_MEMPOOL_DMA_POOL_DEFAULT == pool_handle),
                   ("DEO_MEM: not created default pool\n"));

        /* Add new block to default DMA pool */
        if (DEO_SUCCESS != VVDRV_deo_dma_add_block(args->dma_buf_size,
                                                   args->dma_vbuf,
                                                   args->dma_pbuf,
                                                   0))
        {
            (void)VVDRV_deo_dma_pool_release(&pool_handle);

            deo_mem_initialized = FALSE;

            if (DEO_SUCCESS != VVDRV_deo_mutex_destroy(&deo_dmap_mutex))
            {
                DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
            }
#if defined(DEO_CONFIG_MEMPOOL)
            if (DEO_SUCCESS != VVDRV_deo_mutex_destroy(&deo_memp_mutex))
            {
                DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
            }
#endif /* DEO_CONFIG_MEMPOOL */
            return DEO_ERROR;
        }
    }
#endif /* DEO_CONFIG_POOL_DMA */

#ifdef DEO_CONFIG_POOL_DMA_CACHABLE
    DEO_DBG_W(DEO_MEM,
           ("Flag DEO_CONFIG_POOL_DMA_CACHABLE is deprecated and ignored\n"));
#endif /* DEO_CONFIG_POOL_DMA_CACHABLE */

    deo_mem_initialized = TRUE;

    return DEO_SUCCESS;
}

void VVDRV_deo_mem_uninit(void)
{
#ifdef DEO_CONFIG_POOL_DMA
    uint32 pool_idx;
#endif /* DEO_CONFIG_POOL_DMA */

    if (FALSE == deo_mem_initialized)
    {
        return;
    }

#ifdef DEO_CONFIG_POOL_DMA
    /* Remove all DMA pools */
    for (pool_idx = 0; pool_idx < DEO_MEMPOOL_DMA_POOL_COUNT; pool_idx++)
    {
        if (TRUE == deo_dma_pool[pool_idx].is_created)
        {
            deo_dma_remove_all(&(deo_dma_pool[pool_idx]));
            deo_dma_pool[pool_idx].is_created = FALSE; /* Mark not created */

            /* Note: stats not zeroed to allow further debug. Stats are
             * zeroed at pool create
             */
        }
    }

    if (DEO_SUCCESS != VVDRV_deo_mutex_destroy(&deo_dmap_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex destroy failure\n"));
    }

#endif /* DEO_CONFIG_POOL_DMA */

    /* Initialize mempool */
#if defined(DEO_CONFIG_MEMPOOL)
    if (DEO_SUCCESS != VVDRV_deo_mutex_destroy(&deo_memp_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex destroy failure\n"));
    }
#endif /* DEO_CONFIG_MEMPOOL */

    deo_mem_initialized = FALSE;
}

int32 VVDRV_deo_mem_defrag(void)
{
#ifdef DEO_CONFIG_POOL_DMA
    uint32 pool_idx;
    int32 status = DEO_SUCCESS; /* If zero pools to defrag, then success */

    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&deo_dmap_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex lock failure\n"));
        return DEO_ERROR;
    }
#endif /* DEO_CONFIG_POOL_DMA */

    if (FALSE == deo_mem_initialized)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: deo_mem_defrag: Not initialized\n"));

#ifdef DEO_CONFIG_POOL_DMA
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
        {
            DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        }
#endif /* DEO_CONFIG_POOL_DMA */

        return DEO_ERROR;
    }

#ifdef DEO_CONFIG_POOL_DMA
    /* Defragment all DMA pools */
    for (pool_idx = 0; pool_idx < DEO_MEMPOOL_DMA_POOL_COUNT; pool_idx++)
    {
        deo_dma_pool_t *pool = &(deo_dma_pool[pool_idx]);
        if (TRUE == pool->is_created)
        {
            if (DEO_SUCCESS != deo_dma_pool_defrag(pool))
            {
                status = DEO_ERROR;
            }
        }
    }

    if (DEO_SUCCESS != status)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: Defragmentation failed\n"));
    }

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&deo_dmap_mutex))
    {
        DEO_DBG_E(DEO_MEM, ("DEO mutex unlock failure\n"));
        return DEO_ERROR;
    }
#endif /* DEO_CONFIG_POOL_DMA */

    return DEO_SUCCESS;
}

int32 VVDRV_deo_mem_log_print(void)
{
#ifdef DEO_CONFIG_POOL_DMA
    uint32 pool_idx;
    uint32 pool_count;
#endif /* DEO_CONFIG_POOL_DMA */

    if (FALSE == deo_mem_initialized)
    {
        DEO_DBG_E(DEO_MEM, ("DEO_MEM: Not initialized\n"));
        return DEO_ERROR;
    }

#ifdef DEO_CONFIG_MEMPOOL
#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW
    deo_memp_dump_status();
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */
#endif /* DEO_CONFIG_MEMPOOL */

#ifdef DEO_CONFIG_POOL_DMA
    DEO_DBG_I(DEO_MEM, ("DEO_MEM: DMA pools count: %u\n",
                        DEO_MEMPOOL_DMA_POOL_COUNT));

    pool_count = 0;
    for (pool_idx = 0; pool_idx < DEO_MEMPOOL_DMA_POOL_COUNT; pool_idx++)
    {
        if (TRUE == deo_dma_pool[pool_idx].is_created)
        {
            pool_count++;
        }
    }

    DEO_DBG_I(DEO_MEM, ("DEO_MEM: DMA active pools count: %u\n",pool_count));

#ifdef DEO_CONFIG_MEMPOOL_USAGE_SHOW

    /* Failed attempts to call #VVDRV_deo_dma_free() */
    DEO_DBG_I(DEO_MEM, ("DEO_MEM: Failed DMA free requests: %u\n",
                        deo_dma_failed_free_cnt));

    /* Dump statistics for all created DMA pools */
    for (pool_idx = 0; pool_idx < DEO_MEMPOOL_DMA_POOL_COUNT; pool_idx++)
    {
        if (TRUE == deo_dma_pool[pool_idx].is_created)
        {
            deo_dma_dump_status(pool_idx);
        }
    }
#endif /* DEO_CONFIG_MEMPOOL_USAGE_SHOW */
#ifdef DEO_DMA_DEFRAG_DEBUG
    /* Output freelist for all created DMA pools */
    for (pool_idx = 0; pool_idx < DEO_MEMPOOL_DMA_POOL_COUNT; pool_idx++)
    {
        if (TRUE == deo_dma_pool[pool_idx].is_created)
        {
            DEO_DBG_I(DEO_MEM, ("DEO_MEM: DMA memory pool #%u, Free-list:\n",
                      pool_idx));

            deo_dma_output_freelist(&(deo_dma_pool[pool_idx].freelist));
        }
    }
#endif /* DEO_DMA_DEFRAG_DEBUG */
#endif /* DEO_CONFIG_POOL_DMA */

    return DEO_SUCCESS;
}

/*lint -restore*/
