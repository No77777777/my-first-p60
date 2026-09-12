
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               DL PAGER TOP LEVEL HEADER FILE

GENERAL DESCRIPTION

  Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

            EDIT HISTORY FOR MODULE


when       who    what, where, why
--------   ---    ----------------------------------------------------------
12/09/14   rr     Header file for all config params for dlpager
===========================================================================*/
#include <q6zip_params.h>

#define PAGE_SHIFT (12)
#define PAGE_SIZE (1 << PAGE_SHIFT)


// FIXME: Ensure maximums are correct.
//        Note that MAX_PENDING_PAGES may need additional adjusting (e.g. MAX * 2),
//        because the the same pending page may exist in two different lists.
#define MAX_PENDING_TASKS 32
#define MAX_PENDING_PAGES Q6ZIP_PARAM_CONTEXT_MAX_ITEMS

#define RWBUFFER_SIZE 4352  // worst case no match RW compress = 1024 * 34 / 8

/** @brief Bits to encode the *maximum* number of swap memory instances that
           can be supported *per swappool* */
#define DLPAGER_SWAPMEM_MEM_IDX_BITS 4
#define DLPAGER_SWAPMEM_MEM_IDX_MAX  ( 1 << DLPAGER_SWAPMEM_MEM_IDX_BITS )
#define DLPAGER_SWAPMEM_MEM_IDX_MASK ( DLPAGER_SWAPMEM_MEM_IDX_MAX - 1 )

/** @brief Bits for indexing pages within a swap memory instance
    @details This essentially bounds the maximum size of a single swap memory
             instance */
#define DLPAGER_SWAPMEM_PAGE_IDX_BITS 8
#define DLPAGER_SWAPMEM_PAGE_IDX_MAX  ( 1 << DLPAGER_SWAPMEM_PAGE_IDX_BITS )
#define DLPAGER_SWAPMEM_PAGE_IDX_MASK ( DLPAGER_SWAPMEM_PAGE_IDX_MAX - 1 )

/** @brief Maximum number of swap memory instances a swap pool can support */
#define DLPAGER_MAX_SWAPMEMS DLPAGER_SWAPMEM_MEM_IDX_MAX

/** @brief Maximum pages a swap memory instance can support */
#define DLPAGER_SWAPMEM_MAX_PAGES DLPAGER_SWAPMEM_PAGE_IDX_MAX

#if ( ( DLPAGER_SWAPMEM_MEM_IDX_BITS + DLPAGER_SWAPMEM_PAGE_IDX_BITS ) != 12 )
#error Sum of DLPAGER_SWAPMEM_MEM_IDX_BITS and DLPAGER_SWAPMEM_PAGE_IDX_BITS *MUST BE* == 12
#endif

/** @brief Minimum number of pages in a gap for it to be usable for swap */
#define DLPAGER_GAPS_MIN_NUM_PAGES 2

#if !defined( DLPAGER_RX_SWAP_TCM_DUMP_SHARED_MEM_POOL_NAME )
/** @brief Name of QuRT memory pool that is shared with TCM dump (during full
           Q6 power-collapse) and RX swap functionality */
#define DLPAGER_RX_SWAP_TCM_DUMP_SHARED_MEM_POOL_NAME "DL_PAGER_RX_SWAP_POOL"
#endif
