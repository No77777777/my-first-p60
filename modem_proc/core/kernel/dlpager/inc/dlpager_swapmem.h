#ifndef __DLPAGER_SWAPMEM_H__
#define __DLPAGER_SWAPMEM_H__
/*==============================================================================
 * FILE:         dlpager_swapmem.h
 *
 * SERVICES:     DL Pager Swap Memory
 *
 * DESCRIPTION:  A swap-memory is a piece of physically contiguous memory sourced
 *               either from gaps (collected by QuRT image builder), named 
 *               fixed-size memory pools or from default physical pool. This
 *               module implements a swap-memory abstract datatype and 
 *               operations on it
 *
 *  Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/

/*===========================================================================

  EDIT HISTORY FOR MODULE
  
  
when       who     what, where, why
-------- -------- --------------------------------------------------------------
07/01/16 anandj   Re-design to support gap reclamation in async DL pager
09/30/14 ao       Initial revision
==============================================================================*/

#include <assert.h>
#include <dlpager_params.h>
#include <dlpager_types.h>
#include <qurt.h>
#if defined( DLPAGER_UNIT_TEST )
#include <stdio.h>
#endif

/** @brief Abstract datatype representing physical memory that can be used for
           swapping */
typedef struct
{
    qurt_mem_region_t region;    /**< QuRT memory region handled (if sourced from a pool) */
    qurt_paddr_t      paddr;     /**< Physical address of this swap memory */
    unsigned int      num_pages; /**< Number of pages */
    qurt_perm_t       perms;     /**< Access types to pages from this memory */
    qurt_addr_t       vaddr_rx;  /**< Virtual address for RX usage */
    qurt_addr_t       vaddr_rw;  /**< Virtual address for RW usage */
    unsigned int      is_inited; /**< Initialized? 1 = yes, 0 = no */

} dlpager_swapmem_t;


/**
 * @brief Create swap-memory from the specified pool
 * 
 * @param [in,out] swapmem Swap-memory instance
 * @param [in] num_pages Number of (PAGE_SIZE) pages to allocate. This will 
 *        eventually be limited to DLPAGER_SWAPMEM_MAX_PAGES
 * @param [in] pool_name Name of pool to allocate from. If NULL, 
 *        "DEFAULT_PHYSPOOL" will be used
 * @param [in] pool_size If pool_name is non-NULL, size (bytes) of pool
 * @param [in] perms Intended access 
 *  
 * @warning After this function returns, invoke dlpager_swapmem_is_inited on the 
 *          swap-memory instance to see if it was initialized
 */
extern void dlpager_swapmem_create (
    dlpager_swapmem_t * swapmem, 
    unsigned short      num_pages,
    char const *        pool_name,
    unsigned int        pool_size,
    qurt_perm_t         perms
);

/**
 * @brief Create swap-memory from a block of (physically contiguous) memory
 * 
 * @param [in,out] swapmem Swap-memory instance
 * @param [in] va Virtual address of the memory block that is *ASSUMED* to be 
 *        physically contiguous as well!
 * @param [in] size Size (bytes) of the memory block
 * @param [in] perms Intended access
 */
void dlpager_swapmem_create_from (
    dlpager_swapmem_t * swapmem,
    qurt_addr_t         va,
    unsigned int        size,
    qurt_perm_t         perms
);

/**
 * @brief Get *BASE* virtual address for the given swap-memory
 * 
 * @param [in] swapmem Swap-memory instance
 * 
 * @return qurt_addr_t *BASE* virtual address for the given swap-memory
 */
static inline qurt_addr_t dlpager_swapmem_get_base_virtaddr (
    dlpager_swapmem_t const * swapmem
)
{
    return swapmem->vaddr_rw;
}

/**
   @brief Return physical address for a given page index in this swap memory 
   @param [in] swapmem Swap memory instance 
   @param [in] page_idx Page index 
   @return > 0 represending physical address of page on success, 0 on failure 
*/
static inline qurt_paddr_t dlpager_swapmem_get_physaddr (
    dlpager_swapmem_t const * swapmem,
    unsigned int              page_idx
)
{
    return page_idx < swapmem->num_pages ? 
        swapmem->paddr + ( page_idx * PAGE_SIZE ) : 0;
}

/**
 * @brief Return virtual address for given page index in this swap-memory
 * 
 * @param [in] swapmem Swap-memory instance
 * @param [in] page_idx Page index
 * 
 * @return qurt_addr_t; > 0 representing virtual address of page on success, 0 
 *         on failure
 */
static inline qurt_addr_t dlpager_swapmem_get_virtaddr (
    dlpager_swapmem_t const * swapmem,
    unsigned int              page_idx
)
{
    return page_idx < swapmem->num_pages ? 
        swapmem->vaddr_rw + ( page_idx * PAGE_SIZE ) : 0;
}

/**
 * @brief Get the size (bytes) of this swap-memory
 * 
 * @param [in] swapmem Swap-memory instance
 * 
 * @return unsigned int Size (bytes) of this swap-memory instance
 */
static inline unsigned int dlpager_swapmem_get_size (dlpager_swapmem_t const * swapmem)
{
    return swapmem->num_pages << PAGE_SHIFT;
}

/**
 * @brief Check to see if this swap-memory is initialized
 * 
 * @param [in] swapmem Swap-memory instance
 * 
 * @return unsigned int 1 if initialized, 0 otherwise
 */
static inline unsigned int dlpager_swapmem_is_inited (dlpager_swapmem_t const * swapmem)
{
    return swapmem->is_inited;
}

/**
 * @brief Get virtual & physical addresses of the page index in this swap memory
 * 
 * @param [in] swapmem Swap memory instance
 * @param [in] page_idx Page index
 * @param [out] paddr Physical address of this page
 * @param [out] vaddr Virtual address of this page
 */
static inline void dlpager_swapmem_get_addresses (
    dlpager_swapmem_t const * swapmem,
    unsigned int              page_idx,
    qurt_paddr_t *            paddr,
    qurt_addr_t *             vaddr_rw,
    qurt_addr_t *             vaddr_rx
)
{
    *paddr = *vaddr_rw = *vaddr_rx = 0;

    if ( DLPAGER_LIKELY( page_idx < swapmem->num_pages ) )
    {
        *paddr = swapmem->paddr + ( page_idx * PAGE_SIZE );
        *vaddr_rw = swapmem->vaddr_rw + ( page_idx * PAGE_SIZE );
        /* @warning We dont check permissions of swap memory instance here to
           determine whether to fill vaddr_rx. Caller is expected to do that */
        *vaddr_rx = swapmem->vaddr_rx + ( page_idx * PAGE_SIZE );
    }
}

/**
 * @brief Get the number of pages in this swap memory
 * @param [in] swapmem Swap memory instance
 * @return Number of pages in this swap memory 
 * @warning Only swap memory instances that have been created must be passed to 
 *          this function!
 */
static inline unsigned int dlpager_swapmem_get_num_pages (
    dlpager_swapmem_t const * swapmem
)
{
    return swapmem->num_pages;
}

#if defined( DLPAGER_UNIT_TEST )
/*==============================================================================
                        Unit-test functions
==============================================================================*/
/**
 * @brief Pretty-print swap-memory instance to an output file
 * 
 * @param [in] swapmem Swap-memory instance
 * @param [in] ofile Output file (to print to)
 */
extern void dlpager_swapmem_print (dlpager_swapmem_t const * swapmem, FILE * ofile);

/**
 * @brief Reserve virtual address range covering specified number of (PAGE_SIZE)
 *        pages
 * 
 * @param [in,out] attr Memory region attribute associated with VA range
 * @param [in] num_pages Number of (PAGE_SIZE) pages to reserve the VA range for
 */
extern void dlpager_swapmem_reserve_virtaddr_range (
    qurt_mem_region_attr_t * attr,
    unsigned int             num_pages
);
#endif

#endif
