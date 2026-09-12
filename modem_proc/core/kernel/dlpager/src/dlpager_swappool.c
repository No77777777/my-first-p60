/*==========================================================================
 * FILE:         dlpager_swappool.c
 *
 * SERVICES:     DL PAGER SWAP POOL
 *
 * DESCRIPTION:  This file provides functionality to manage dlpager swap pool. 
 *
 * Copyright (c) 2014 Qualcomm Technologies Incorporated.
 * All Rights Reserved. QUALCOMM Proprietary and Confidential.
=============================================================================*/


/*===========================================================================

            EDIT HISTORY FOR MODULE
===========================================================================*/
/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <assert.h>
#include <qurt.h>
#include <dlpager.h>
#include <dlpager_gaps.h>
#include <dlpager_params.h>
#include <dlpager_swapmem.h>
#include <dlpager_swappool.h>
#include <dlpager_log.h>
#include <dlpager_types.h>
#include <dlpager_meta.h>
#include <dlpager_pagesm.h>

/*------------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *----------------------------------------------------------------------------*/
#define SWAPPOOL_SIZE_RX ( 2 * 1024 * 1024 )
#define SWAPPOOL_SIZE_RW ( 1 * 1024 * 1024 )

#define HARD_CLEANING_THRESHOLD  (2)
#define SOFT_CLEANING_THRESHOLD  (6)

/*------------------------------------------------------------------------------
 * Type Declarations
 *----------------------------------------------------------------------------*/
/** @brief One node per physical page. Each node will say which virtual page is
    loaded in memory or 0 for none.
    @warning Please *DONOT* change name of this type, its composition and names
    of its members as post-mortem Trace32 script depends on it */
typedef struct _swappool_node
{
   struct _swappool_node *next; //Next page was less recently used.
   struct _swappool_node *prev; //Previous page was more recently used.
   union
   {
       /* Here's the layout of data (32b) stored in each node. Given a node that
          is "tracking" a (PAGE_SIZE) swap page, it should be possible to find
          data on the physical page backing it. 'mem' indexes into the swap
          memory instance and 'page' indexes into the page within that swap
          memory instance. Encoding this information in the lower 12b prevents
          the need for any additional lookups.

              31                                 0
               +--------------------+---+---------+
               |        vpn         |mem|  page   |
               +--------------------+---+---------+
        */
       struct
       {
           unsigned int page_idx: DLPAGER_SWAPMEM_PAGE_IDX_BITS; /* Index of page within physical swap */
           unsigned int mem_idx:  DLPAGER_SWAPMEM_MEM_IDX_BITS;  /* Index of physical swap (may be gaps)*/
           unsigned int vpn:      20; /* Virtual Page Number of page loaded into swap */
       } fields;

       unsigned int raw;

   } data;

} dlpager_swappool_node_t;

typedef struct dlpager_swappool_s
{
   unsigned int region_size; /* size of the allocated swap pool */
   unsigned int seg_addr_base; /**< Base address of the segment used for loading */
   unsigned int page_shift; /**< x, where 2 power x is the page size */
   unsigned int max_swap_pages; /* Max. number of swappable pages */
   unsigned int max_glob_pages; /* Max. number of pages in the segment */
   dlpager_swappool_node_t* swap_list_head;  /*list of swap pages*/
   dlpager_swappool_node_t* loaded_pages; /* Table of virt addr of the loaded pages and swap nodes */
   dlpager_swappool_node_t ** global_pages; /* Mapping table of the overall segment */
   qurt_mutex_t mutex;        /* Mutex protects swappool operations */
   qurt_sem_t clean_page_count; /* Semaphore tracks number of clean pages in the system */
   unsigned int num_reset;      /* Number of times this swap has been reset */
   unsigned int num_mru_ignored;
   dlpager_swapmem_t swapmems[ DLPAGER_MAX_SWAPMEMS ]; /* Swap memory instances */
} dlpager_swappool_t;

/*------------------------------------------------------------------------------
 * Global Data Definitions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Static Variable Definitions
 *----------------------------------------------------------------------------*/
static dlpager_swappool_t dlpager_swappool_rx;
static dlpager_swappool_t dlpager_swappool_rw;

/** @brief Array to track swap pools
    @warning Please *DONOT* remove this. It is required by Trace32 scripts for
             post crash analysis. To guarantee that compiler doesnt optimize it
             out, its indexes are filled in dlpager_swappool_init() */
static dlpager_swappool_t const * dlpager_swappools[ 2 ];

/*------------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *----------------------------------------------------------------------------*/
static inline void swappool_node_set_vpn (dlpager_swappool_node_t * node, unsigned int vpn)
{
    node->data.fields.vpn = vpn;
}

static inline unsigned int swappool_node_get_vpn (dlpager_swappool_node_t const * node)
{
    return node->data.fields.vpn;
}

static inline unsigned int swappool_node_get_va (dlpager_swappool_node_t const * node)
{
    return swappool_node_get_vpn( node ) << PAGE_SHIFT;
}

static inline void swappool_get_swapmem_addresses (
    dlpager_swappool_t const *      pool,
    dlpager_swappool_node_t const * node,
    qurt_paddr_t *                  paddr,
    qurt_addr_t *                   vaddr_rw,
    qurt_addr_t *                   vaddr_rx
)
{
    dlpager_swapmem_get_addresses(
        &( pool->swapmems[ node->data.fields.mem_idx ] ),
        node->data.fields.page_idx,
        paddr,
        vaddr_rw,
        vaddr_rx );
}

static inline qurt_paddr_t swappool_get_swapmem_physaddr (
    dlpager_swappool_t const *      pool,
    dlpager_swappool_node_t const * node
)
{
    dlpager_swapmem_t const * swapmem = &( pool->swapmems[ node->data.fields.mem_idx ] );
    return dlpager_swapmem_get_physaddr( swapmem, node->data.fields.page_idx );
}

static inline void swappool_node_set_swap_mem (
    dlpager_swappool_node_t * node,
    unsigned int              mem_idx,
    unsigned int              page_idx
)
{
    node->data.fields.mem_idx  = mem_idx & DLPAGER_SWAPMEM_MEM_IDX_MASK;
    node->data.fields.page_idx = page_idx & DLPAGER_SWAPMEM_PAGE_IDX_MASK;
}

static void swappool_remove_node( dlpager_swappool_node_t **ring, dlpager_swappool_node_t *node )
{
   node->prev->next = node->next;
   node->next->prev = node->prev;
   if( *ring == node )
   {
      *ring = node->next;
   }
}

static void swappool_insert_head( dlpager_swappool_node_t **ring, dlpager_swappool_node_t *node )
{
   node->next = *ring;
   node->prev = (*ring)->prev;
   node->prev->next = node;
   node->next->prev = node;
   *ring = node;
}

static void swappool_create_swap (
    dlpager_swappool_t    * pool,
    qurt_perm_t             perm,
    dlpager_gap_t const *   gaps,
    unsigned int            num_gaps
)
{
    unsigned int num_swap_pages_backed = 0;
    unsigned int i, j, k;
    dlpager_swapmem_t * swapmem;

    i = 0; j = 0; k = 0;

    /* Attempt to consume gaps ... */
    for ( i = 0; i < num_gaps; i++ )
    {
        swapmem = &pool->swapmems[ k ];

        switch ( gaps[ i ].kind )
        {
            case DLPAGER_GAP_KIND_POOL:

                dlpager_swapmem_create( 
                    swapmem,
                    pool->max_swap_pages - num_swap_pages_backed,
                    gaps[ i ].detail.pool.name,
                    gaps[ i ].size,
                    perm );
                break;

            case DLPAGER_GAP_KIND_MEMORY:

                dlpager_swapmem_create_from(
                    swapmem,
                    gaps[ i ].detail.memory.addr,
                    gaps[ i ].size,
                    perm );
                break;

            default:
                ASSERT( FALSE );
        }

        if ( !dlpager_swapmem_is_inited( swapmem ) )
        {
            /* Unable to consume this gap, move on to the next ... */
            continue;
        }

        for ( j = num_swap_pages_backed;
              j < num_swap_pages_backed + dlpager_swapmem_get_num_pages( swapmem );
              j++ )
        {
            swappool_node_set_swap_mem(
                &pool->loaded_pages[ j ],
                k,
                j - num_swap_pages_backed );
        }

        num_swap_pages_backed += dlpager_swapmem_get_num_pages( swapmem );

        if ( num_swap_pages_backed >= pool->max_swap_pages )
        {
            /* All swap pages have been physically backed. We are done! */
            break;
        }

        /* Next swap-memory */
        k++;

        /* On to the next gap ... */
    }

    while ( num_swap_pages_backed < pool->max_swap_pages )
    {
        swapmem = &pool->swapmems[ k ];

        /* Create a swap memory from DEFAULT_PHYSPOOL to back the remaining pages */
        dlpager_swapmem_create( swapmem, pool->max_swap_pages - num_swap_pages_backed, NULL, 0, perm );

        /* DEFAULT_PHYSPOOL is expected to have enough memory to back the
           remaining swap. If this ASSERTion fails, DEFAULT_PHYSPOOL has been
           depleted! */
        ASSERT( dlpager_swapmem_is_inited( swapmem ) );

        for ( j = num_swap_pages_backed;
              j < num_swap_pages_backed + dlpager_swapmem_get_num_pages( swapmem );
              j++ )
        {
            swappool_node_set_swap_mem(
                &pool->loaded_pages[ j ],
                k,
                j - num_swap_pages_backed );
        }

        num_swap_pages_backed += dlpager_swapmem_get_num_pages( swapmem );

        /* Next swap-memory */
        k++;
    }
}

static void swappool_rx_swap_creator (dlpager_swappool_t * pool)
{
    dlpager_gap_t gaps[ DLPAGER_MAX_SWAPMEMS - 2 ];
    unsigned int num_gaps;

    /* Gather RX gaps ... */
    dlpager_gaps_rx_gather( gaps, DLPAGER_ARRAY_SIZE( gaps ), &num_gaps );

    swappool_create_swap( pool, QURT_PERM_READ | QURT_PERM_EXECUTE, gaps, num_gaps );
}

static void swappool_rw_swap_creator (dlpager_swappool_t * pool)
{
    dlpager_gap_t gaps[ DLPAGER_MAX_SWAPMEMS - 1 ];
    unsigned int  num_gaps;

    /* Gather RW gaps ... */
    dlpager_gaps_rw_gather( gaps, DLPAGER_ARRAY_SIZE( gaps ), &num_gaps );

    swappool_create_swap( pool, QURT_PERM_READ | QURT_PERM_WRITE, gaps, num_gaps );
}

static void swappool_init_rx( void )
{
   unsigned int segment_size;
   unsigned int mem_size;

   memset( &dlpager_swappool_rx, 0, sizeof( dlpager_swappool_rx ) );

   /* @warning Swap-pool mutex initialized *REGARDLESS* of swap-pool initialization */
   qurt_pimutex_init( &dlpager_swappool_rx.mutex );

   /* RX swap pool holds text and RO data. I assume these segments are contiguous in the linker script. */
   segment_size = (unsigned int)&__swapped_segments_rodata_end__ - (unsigned int)&__swapped_segments_text_start__;

   if ( segment_size == 0 )
   {
       /* Q6Zip RO is disabled. Dont initialize RX swap */
       return;
   }

   /* Swap pool should be smaller than the size of contents to hold.
      If the swap pool is larger than the swappable pages, someone made a mistake.
      Maybe someone tried to disable some part of pager/Q6ZIP and didn't reduce the
      size of the swap pool accordingly? */
   ASSERT( SWAPPOOL_SIZE_RX < segment_size );

   /* Only way to reach here is to have received the requested size. */
   dlpager_swappool_rx.region_size = SWAPPOOL_SIZE_RX;

   /* Fill out the remaining variables. */
   dlpager_swappool_rx.seg_addr_base = (unsigned int)&__swapped_segments_text_start__;
   dlpager_swappool_rx.page_shift = PAGE_SHIFT;
   dlpager_swappool_rx.max_swap_pages = SWAPPOOL_SIZE_RX >> PAGE_SHIFT;
   dlpager_swappool_rx.max_glob_pages = segment_size >> PAGE_SHIFT;

   /* Initial resource count set because every page in pool is evictable because it is empty. */
   qurt_sem_init_val( &dlpager_swappool_rx.clean_page_count, dlpager_swappool_rx.max_swap_pages );
   dlpager_swappool_rx.num_reset = 0;
   dlpager_swappool_rx.num_mru_ignored = 0;

   /* Once we have the physical pool and VA mappings to the physical memory, finish initializing the remaining structures. */
   mem_size = 
       ( sizeof( dlpager_swappool_node_t * ) * dlpager_swappool_rx.max_glob_pages ) +
       ( sizeof( dlpager_swappool_node_t ) * dlpager_swappool_rx.max_swap_pages );
   dlpager_swappool_rx.loaded_pages = (dlpager_swappool_node_t*)malloc( mem_size );
   ASSERT( NULL != dlpager_swappool_rx.loaded_pages );
   dlpager_swappool_rx.global_pages = (dlpager_swappool_node_t**)&(dlpager_swappool_rx.loaded_pages[dlpager_swappool_rx.max_swap_pages]);
   memset((void *)dlpager_swappool_rx.loaded_pages, (int)0, mem_size);
 
   /* Initialize loadable pages to reflect no mapping. This is a doubly linked list, circular buffer.
      First element's prev points to last element. Last element's next points to first element. */
   for( unsigned i = 0; i < dlpager_swappool_rx.max_swap_pages; i++ )
   {
      dlpager_swappool_rx.loaded_pages[i].next = &dlpager_swappool_rx.loaded_pages[i+1];
      dlpager_swappool_rx.loaded_pages[i].prev = &dlpager_swappool_rx.loaded_pages[i-1];
   }
   dlpager_swappool_rx.loaded_pages[0].prev = &dlpager_swappool_rx.loaded_pages[dlpager_swappool_rx.max_swap_pages-1];
   dlpager_swappool_rx.loaded_pages[dlpager_swappool_rx.max_swap_pages-1].next = &dlpager_swappool_rx.loaded_pages[0];

   /* The head points to the most recent swap pool page inserted into the TLB. */
   dlpager_swappool_rx.swap_list_head = &dlpager_swappool_rx.loaded_pages[0];

   swappool_rx_swap_creator( &dlpager_swappool_rx );
}

static void swappool_init_rw( void )
{
   unsigned int segment_size;
   unsigned int mem_size;

   memset( &dlpager_swappool_rw, 0, sizeof( dlpager_swappool_rw ) );

   /* @warning Swap-pool mutex initialized *REGARDLESS* of swap-pool initialization */
   qurt_pimutex_init( &dlpager_swappool_rw.mutex );

   /* RW swap pool holds RW data and BSS data. I assume these segments are contiguous in the linker script. */
   segment_size = (unsigned int)&__swapped_segments_bss_end__ - (unsigned int)&__swapped_segments_rwdata_start__;

   if ( segment_size == 0 )
   {
       /* Q6Zip RW is disabled. Dont initialize RW swap */
       return;
   }

   /* Swap pool should be smaller than the size of contents to hold.
      If the swap pool is larger than the swappable pages, someone made a mistake.
      Maybe someone tried to disable some part of pager/Q6ZIP and didn't reduce the
      size of the swap pool accordingly? */
   ASSERT( SWAPPOOL_SIZE_RW < segment_size );

   /* Only way to reach here is to have received the requested size. */
   dlpager_swappool_rw.region_size = SWAPPOOL_SIZE_RW;

   /* Fill out the remaining variables. */
   dlpager_swappool_rw.seg_addr_base = (unsigned int)&__swapped_segments_rwdata_start__;
   dlpager_swappool_rw.page_shift = PAGE_SHIFT;
   dlpager_swappool_rw.max_swap_pages = SWAPPOOL_SIZE_RW >> PAGE_SHIFT;
   dlpager_swappool_rw.max_glob_pages = segment_size >> PAGE_SHIFT;

   /* Initial resource count set because every page in pool is evictable because it is empty. */
   qurt_sem_init_val( &dlpager_swappool_rw.clean_page_count, dlpager_swappool_rw.max_swap_pages );
   dlpager_swappool_rw.num_reset = 0;
   dlpager_swappool_rw.num_mru_ignored = 0;

   /* Once we have the physical pool and VA mappings to the physical memory, finish initializing the remaining structures. */
   mem_size = 
       ( sizeof( dlpager_swappool_node_t * ) * dlpager_swappool_rw.max_glob_pages ) + 
       ( sizeof( dlpager_swappool_node_t )   * dlpager_swappool_rw.max_swap_pages );
   dlpager_swappool_rw.loaded_pages = (dlpager_swappool_node_t*)malloc( mem_size );
   ASSERT( NULL != dlpager_swappool_rw.loaded_pages );
   dlpager_swappool_rw.global_pages = (dlpager_swappool_node_t**)&(dlpager_swappool_rw.loaded_pages[dlpager_swappool_rw.max_swap_pages]);
   memset((void *)dlpager_swappool_rw.loaded_pages, (int)0, mem_size);
 
   /* Initialize loadable pages to reflect no mapping. This is a doubly linked list, circular buffer.
      First element's prev points to last element. Last element's next points to first element. */
   for( unsigned i = 0; i < dlpager_swappool_rw.max_swap_pages; i++ )
   {
      dlpager_swappool_rw.loaded_pages[i].next = &dlpager_swappool_rw.loaded_pages[i+1];
      dlpager_swappool_rw.loaded_pages[i].prev = &dlpager_swappool_rw.loaded_pages[i-1];    
   }
   dlpager_swappool_rw.loaded_pages[0].prev = &dlpager_swappool_rw.loaded_pages[dlpager_swappool_rw.max_swap_pages-1];
   dlpager_swappool_rw.loaded_pages[dlpager_swappool_rw.max_swap_pages-1].next = &dlpager_swappool_rw.loaded_pages[0];

   /* The head points to the most recent swap pool page inserted into the TLB. */
   dlpager_swappool_rw.swap_list_head = &dlpager_swappool_rw.loaded_pages[0];

   swappool_rw_swap_creator( &dlpager_swappool_rw );
}

static void swappool_lookup_rw( unsigned int va, dlpager_swappool_page_info_t *ret )
{
   unsigned int idx;
   dlpager_swappool_node_t * node;

   ret->va = va;
   
   qurt_pimutex_lock( &dlpager_swappool_rw.mutex );
   idx = ( va - dlpager_swappool_rw.seg_addr_base ) >> PAGE_SHIFT;
   /*ret->pa = dlpager_swappool_rw.global_pages[idx];
   ret->va_swap_rw = (ret->pa - dlpager_swappool_rw.p_addr_base) + dlpager_swappool_rw.v_swap_rw_addr_base;*/
   node = dlpager_swappool_rw.global_pages[ idx ];
   ASSERT( node );
   swappool_get_swapmem_addresses( &dlpager_swappool_rw, node, &ret->pa, &ret->va_swap_rw, &ret->va_swap_rx );

   /* RX VA is not relevant for RW swap */
   ret->va_swap_rx = 0;

   qurt_pimutex_unlock( &dlpager_swappool_rw.mutex );
}

static void swappool_lookup_rx( unsigned int va, dlpager_swappool_page_info_t *ret )
{
   unsigned int idx;
   dlpager_swappool_node_t * node;

   ret->va = va;
   
   qurt_pimutex_lock( &dlpager_swappool_rx.mutex );
   idx = ( va - dlpager_swappool_rx.seg_addr_base ) >> PAGE_SHIFT;
   /*ret->pa = dlpager_swappool_rx.global_pages[idx];
   ret->va_swap_rw = (ret->pa - dlpager_swappool_rx.p_addr_base) + dlpager_swappool_rx.v_swap_rw_addr_base;
   ret->va_swap_rx = (ret->pa - dlpager_swappool_rx.p_addr_base) + dlpager_swappool_rx.v_swap_rx_addr_base;*/

   node = dlpager_swappool_rx.global_pages[ idx ];
   ASSERT( node );
   swappool_get_swapmem_addresses( &dlpager_swappool_rx, node, &ret->pa, &ret->va_swap_rw, &ret->va_swap_rx );

   qurt_pimutex_unlock( &dlpager_swappool_rx.mutex );
}

static void swappool_remove_mapping( unsigned int va, dlpager_swappool_t *pPool )
{
   dlpager_swappool_node_t *node;
   unsigned int idx;

   qurt_pimutex_lock( &(pPool->mutex) );
   idx = ( va - pPool->seg_addr_base ) >> PAGE_SHIFT;
   ASSERT( idx < pPool->max_glob_pages );

   node = pPool->global_pages[ idx ];
   pPool->global_pages[ idx ] = 0;

   swappool_node_set_vpn( node, 0 );
   qurt_pimutex_unlock( &(pPool->mutex) );
}

static void swappool_insert_mapping( unsigned int va, unsigned int pa, void * token, dlpager_swappool_t *pPool )
{
   unsigned int vpn;
   dlpager_swappool_node_t *node = (dlpager_swappool_node_t *)token;

   qurt_pimutex_lock( &(pPool->mutex) );

   /* Unused! */
   (void)pa;

   swappool_node_set_vpn( node, va >> PAGE_SHIFT );
   vpn = ( va - pPool->seg_addr_base ) >> PAGE_SHIFT;
   ASSERT( vpn < pPool->max_glob_pages );
   ASSERT( 0 == pPool->global_pages[ vpn ] );
   /*pPool->global_pages[idx] = pa;*/
   pPool->global_pages[ vpn ] = node;
   swappool_remove_node( &(pPool->swap_list_head), node );
   swappool_insert_head( &(pPool->swap_list_head), node );
   qurt_pimutex_unlock( &(pPool->mutex) );
}

/**
 * @brief Reset RX swap 
 * 
 * @return unsigned int Number of (PAGE_SIZE) pages reset
 */
static unsigned int swappool_reset_rx (void)
{
    unsigned int i;
    unsigned int num_pages_cleaned = 0;
    unsigned int vpn;
    dlpager_swappool_node_t * curr;
    qurt_paddr_t pa;
    int retval;

    /* @warning No need to lock swap as this function is expected to be
       executed in single threaded mode (STM) */

    curr = dlpager_swappool_rx.loaded_pages;
    for ( i = 0; curr && ( i < dlpager_swappool_rx.max_swap_pages ); i++ )
    {
        if ( swappool_node_get_va( curr )/*curr->va*/ != 0 )
        {
            vpn = ( swappool_node_get_va( curr )/*curr->va*/ - (unsigned int)&__swapped_segments_text_start__ ) >> PAGE_SHIFT;
            pa = swappool_get_swapmem_physaddr( &dlpager_swappool_rx, curr );

            ASSERT( vpn < dlpager_swappool_rx.max_glob_pages );
            ASSERT( pa != 0 );

            /* At this point, we have VA, PA, page-size and should be able
               a. remove this mapping using qurt_mapping_remove()
                  + need to find time-complexity of this
               b. Clear out VA for this loaded page
               c. Clear out PA for this global page */

            retval = qurt_mapping_remove( swappool_node_get_va( curr )/*curr->va*/, pa, PAGE_SIZE );
            ASSERT( retval == QURT_EOK );

            /* Clear-out VA and PA from our book-keeping */
            swappool_node_set_vpn( curr, 0 ) /*curr->va = 0*/;
            dlpager_swappool_rx.global_pages[ vpn ] = 0;

            num_pages_cleaned++;
        }

        curr = curr->next;
    }

    ASSERT( i == dlpager_swappool_rx.max_swap_pages );

    dlpager_swappool_rx.swap_list_head = &dlpager_swappool_rx.loaded_pages[ 0 ];

    /* @warning Just doing this for completeness. This may not be necessary as
       clean_page_count is *unused* for RX */
    qurt_sem_init_val( &dlpager_swappool_rx.clean_page_count, dlpager_swappool_rx.max_swap_pages );

    /* @warning No need to invalidate caches as they will be powered off and 
       reset upon wakeup */

    /* At this point, the swap book-keeping is clean */
    dlpager_swappool_rx.num_reset++;

    return num_pages_cleaned;
}

/*------------------------------------------------------------------------------
 * Externalized Function Definitions
 *----------------------------------------------------------------------------*/
static void swappool_get_swapmem (
    dlpager_swappool_t const * pool,
    dlpager_swapmem_t const ** swapmem,
    unsigned int *             num_swapmem
)
{
    unsigned int i;

    /* We dont have any state (ie variable) that tracks the number of usable
       swap-memories. Therefore, we resort to iterating over the list of usable
       swap-memories here. This should be acceptable as this function is *NOT*
       expected to be invoked after initialization */
    for ( i = 0; i < DLPAGER_MAX_SWAPMEMS; i++ )
    {
        if ( !dlpager_swapmem_is_inited( &pool->swapmems[ i ] ) )
        {
            break;
        }
    }

    *swapmem = pool->swapmems;
    *num_swapmem = i;
}

void dlpager_swappool_rx_get_swapmem (
    dlpager_swapmem_t const ** swapmem,
    unsigned int *             num_swapmem
)
{
    swappool_get_swapmem( &dlpager_swappool_rx, swapmem, num_swapmem );
}

void dlpager_swappool_rw_get_swapmem (
    dlpager_swapmem_t const ** swapmem,
    unsigned int *             num_swapmem
)
{
    swappool_get_swapmem( &dlpager_swappool_rw, swapmem, num_swapmem );
}

void dlpager_swappool_rw_mru_page( unsigned int va )
{
   unsigned int idx;
   dlpager_swappool_node_t *node;

   qurt_pimutex_lock( &dlpager_swappool_rw.mutex );
   /* The index to global pages gives the PA for this VA. */
   idx = ( va - dlpager_swappool_rw.seg_addr_base ) >> PAGE_SHIFT;

   node = dlpager_swappool_rw.global_pages[ idx ];
   ASSERT( node );

   /* Put this node at the front of the list. */
   swappool_remove_node( &dlpager_swappool_rw.swap_list_head, node );
   swappool_insert_head( &dlpager_swappool_rw.swap_list_head, node );
   qurt_pimutex_unlock( &dlpager_swappool_rw.mutex );
}

void dlpager_swappool_rx_mru_page( unsigned int va )
{
   unsigned int idx;
   dlpager_swappool_node_t *node;

   qurt_pimutex_lock( &dlpager_swappool_rx.mutex );
   idx = ( va - dlpager_swappool_rx.seg_addr_base ) >> PAGE_SHIFT;

   node = dlpager_swappool_rx.global_pages[ idx ];
   ASSERT( node );

   /* Put this node at the front of the list. */
   swappool_remove_node( &dlpager_swappool_rx.swap_list_head, node );
   swappool_insert_head( &dlpager_swappool_rx.swap_list_head, node );
   qurt_pimutex_unlock( &dlpager_swappool_rx.mutex );
}

void dlpager_swappool_perform_mru( void )
{
   unsigned int number_tlbs;
   unsigned int *tlb_virtual_addresses, vaddr;
   dlpager_swappool_t * pool;
   unsigned int vpn;

   /* @warning QuRT internally maintains a single list for pager related VAs.
      To prevent more than one task (back-ends) from modifying it concurrently
      we *MUST GUARD* qurt_tlb_get_pager_virtaddr() as well */
   qurt_pimutex_lock( &dlpager_swappool_rx.mutex );
   qurt_pimutex_lock( &dlpager_swappool_rw.mutex );

   /*
      Returns number of mapped pages that occupy a TLB entry and the virtual address of the entry. 
      In theory the pages in the MMU's TLB are the Most Recently Used because they are the ones
      most recently placed into the MMU.
   */
   number_tlbs = qurt_tlb_get_pager_virtaddr( &tlb_virtual_addresses );
   
   /*
      Moving the mapped page to the top of the list makes it the most recently used and least likely
      to be evicted.
   */
   for( unsigned int i = 0; i < number_tlbs; i++ )
   {
      dlpager_swappool_node_t *node;
      vaddr = tlb_virtual_addresses[ i ];
      /* Try to find which swap pool this phys address belongs to. Only works if swap pool is contiguous in physical address. */
      if ( dlpager_meta_is_addr_in_text_or_rodata_range( vaddr ) )
      {
          pool = &dlpager_swappool_rx;
      }
      else if ( dlpager_meta_is_addr_in_rwdata_range( vaddr ) )
      {
          pool = &dlpager_swappool_rw;

      }
      else
      {
          continue;
      }

      vpn = ( vaddr - pool->seg_addr_base ) >> PAGE_SHIFT;

      node = pool->global_pages[ vpn ];
      if ( !node )
      {
          pool->num_mru_ignored++;
          continue;
      }

      swappool_remove_node( &( pool->swap_list_head ), node );
      swappool_insert_head( &( pool->swap_list_head ), node );
   }
   qurt_pimutex_unlock( &dlpager_swappool_rw.mutex );
   qurt_pimutex_unlock( &dlpager_swappool_rx.mutex );
}

void dlpager_swappool_lookup( unsigned int va, dlpager_swappool_page_info_t *ret )
{
   //Assumes dlpager_swappool_rw.seg_addr_base > dlpager_swappool_rx.seg_addr_base.
   if( va >= dlpager_swappool_rw.seg_addr_base )
   {
      swappool_lookup_rw( va, ret );
   }
   else
   {
      swappool_lookup_rx( va, ret );
   }
}

void dlpager_swappool_rw_remove_mapping( unsigned int va )
{
   swappool_remove_mapping( va, &dlpager_swappool_rw );
}

void dlpager_swappool_rx_remove_mapping( unsigned int va )
{
   swappool_remove_mapping( va, &dlpager_swappool_rx );
}

/* Decrement the count when a clean page is consumed. */
void dlpager_swappool_rw_decrement_clean_page_count( void )
{
   dlpager_swappool_node_t *node;
   unsigned int i;
   
   /* Decrements the clean count. Can block if the count is 0. */
   qurt_sem_down( &dlpager_swappool_rw.clean_page_count );
   /* If the clean count falls too low then force a clean to happen. */
   if( qurt_sem_get_val(&dlpager_swappool_rw.clean_page_count) <= HARD_CLEANING_THRESHOLD )
   {
      /* Find the first dirty page. Traverse the list from least recently used to most recently used (backwards). */
      qurt_pimutex_lock( &dlpager_swappool_rw.mutex );
      node = dlpager_swappool_rw.swap_list_head->prev;
      /* Guarantee we inspect every node in the list once. */
      for( i = 0; i < (SWAPPOOL_SIZE_RW/PAGE_SIZE); i++ )
      {
         if( dlpager_pagesm_is_page_dirty( swappool_node_get_va( node ) ) )
         { /* Stop searching at the first dirty page. */
            break;
         }
         node = node->prev;
      }
      qurt_pimutex_unlock( &dlpager_swappool_rw.mutex );
      
      if( i < (SWAPPOOL_SIZE_RW/PAGE_SIZE) )
      {
         dlpager_pagesm_handle_event( 
             HARD_CLEAN_PAGE, 
             swappool_node_get_va( node ),
             THREAD_NULL );
      }
      else
      {
         /* We should never get through the list and not find a dirty page.
            If we did then something is wrong with the clean page count semaphore. */
         ASSERT( i < (SWAPPOOL_SIZE_RW/PAGE_SIZE) );
      }
   }
}

void dlpager_swappool_rw_increment_clean_page_count( void )
{
   qurt_sem_up( &dlpager_swappool_rw.clean_page_count );
}

void dlpager_swappool_rw_insert_mapping( unsigned int va, unsigned int pa, void * token )
{
   swappool_insert_mapping( va, pa, token, &dlpager_swappool_rw );
}

void dlpager_swappool_rx_insert_mapping( unsigned int va, unsigned int pa, void * token )
{
   swappool_insert_mapping( va, pa, token, &dlpager_swappool_rx );
}

void dlpager_swappool_rw_get_pa( unsigned int va, unsigned int *pa )
{
   unsigned int idx;
   dlpager_swappool_node_t * node;

   idx = ( va - dlpager_swappool_rw.seg_addr_base ) >> PAGE_SHIFT;

   node = dlpager_swappool_rw.global_pages[ idx ];
   ASSERT( node );
   *pa  = swappool_get_swapmem_physaddr( &dlpager_swappool_rw, node );
}

void dlpager_swappool_rx_get_pa( unsigned int va, unsigned int *pa )
{
   unsigned int idx;
   dlpager_swappool_node_t * node;

   idx = ( va - dlpager_swappool_rx.seg_addr_base ) >> PAGE_SHIFT;
   node = dlpager_swappool_rx.global_pages[ idx ];
   ASSERT( node );
   *pa  = swappool_get_swapmem_physaddr( &dlpager_swappool_rx, node );
}

void dlpager_swappool_init( void )
{
   swappool_init_rx();
   swappool_init_rw();

   /* Assign values to guarantee that compiler doesnt optimize it out */
   dlpager_swappools[ 0 ] = &dlpager_swappool_rx;
   dlpager_swappools[ 1 ] = &dlpager_swappool_rw;
}

unsigned int dlpager_swappool_rx_evict_page( void ** token )
{
   dlpager_swappool_node_t *node = NULL;
   unsigned int pa = 0;

   qurt_pimutex_lock( &dlpager_swappool_rx.mutex );
   /* The tail is the least recently used node. Evict it. */
   node = dlpager_swappool_rx.swap_list_head->prev;
   pa = swappool_get_swapmem_physaddr( &dlpager_swappool_rx, node );
   qurt_pimutex_unlock( &dlpager_swappool_rx.mutex );

   //Start-up pages have the va == 0.
   if( swappool_node_get_va( node ) != 0 )
   {
      DLPAGER_TASKLOG_WRITE_L1( DLP_LOG_EVICT, pa, swappool_node_get_va( node ) );
      dlpager_pagesm_handle_event( EVICT_PAGE, swappool_node_get_va( node ), THREAD_NULL );
   }

   *token = ( void * )node;

   return pa & ~(unsigned int)( ( 1 << PAGE_SHIFT ) - 1 );
}

unsigned int dlpager_swappool_rw_evict_page( void ** token)
{
   dlpager_swappool_node_t *node = NULL;
   unsigned int pa = 0;
   unsigned int evictable = 0;

   /* No idea why this is necessary but it was in the original code.
      Blocks here if there are no clean pages in the system. But, the
      only way to unblock is if a compression event completes.*/
   qurt_sem_down( &dlpager_swappool_rw.clean_page_count );
   /* The down was just to block if there wasn't a clean page. Immediately up to restore the correct count
      since we didn't actually clean a page. */
   qurt_sem_up( &dlpager_swappool_rw.clean_page_count );

   qurt_pimutex_lock( &dlpager_swappool_rw.mutex );
   /* Have to walk the list until we find a page that can be evicted. */
   node = dlpager_swappool_rw.swap_list_head;
   do
   {
      /* Walk backwards from least to most recently used. */
      node = node->prev;
      if( swappool_node_get_va( node ) != 0 )
      {
         evictable = dlpager_pagesm_is_page_evictable( swappool_node_get_va( node ) );
      }
      else
      {
         /* If there isn't a VA mapped to this node, it is evictable. */
         evictable = 1;
      }
   } while( !evictable );

   pa = swappool_get_swapmem_physaddr( &dlpager_swappool_rw, node );
   qurt_pimutex_unlock( &dlpager_swappool_rw.mutex );

   //Start-up pages have the va == 0.
   if( swappool_node_get_va( node ) != 0 )
   {
      DLPAGER_TASKLOG_WRITE_L1( DLP_LOG_EVICT, pa, swappool_node_get_va( node ) );
      dlpager_pagesm_handle_event( EVICT_PAGE, swappool_node_get_va( node ), THREAD_NULL );
   }

   /* Swap node of the page being evicted is *opaquely* returned back to caller */
   *token = ( void * )node;

   return pa & ~(unsigned int)( ( 1 << PAGE_SHIFT ) - 1 );
}

void dlpager_swappool_rw_initiate_soft_clean( void )
{
   unsigned int page_is_dirty = 0;
   dlpager_swappool_node_t *node;

   qurt_pimutex_lock( &dlpager_swappool_rw.mutex );
   node = dlpager_swappool_rw.swap_list_head;
   for( unsigned i = 0; i < SOFT_CLEANING_THRESHOLD; i++ )
   {
      node = node->prev;
      page_is_dirty = dlpager_pagesm_is_page_dirty( swappool_node_get_va( node ) );
      if( page_is_dirty )
      {
         break;
      }
   }
   qurt_pimutex_unlock( &dlpager_swappool_rw.mutex );

   if( page_is_dirty )
   {
      dlpager_pagesm_handle_event( SOFT_CLEAN_PAGE, swappool_node_get_va( node ), THREAD_NULL );
   }
}

unsigned int dlpager_swappool_rx_reset (void)
{
    return swappool_reset_rx();
}

#if defined( DLPAGER_UNIT_TEST )
static unsigned int swappool_node_get_raw (dlpager_swappool_node_t const * node)
{
    return node->data.raw;
}

static unsigned int swappool_node_get_memidx (dlpager_swappool_node_t const * node)
{
    return node->data.fields.mem_idx;
}

static unsigned int swappool_node_get_pageidx (dlpager_swappool_node_t const * node)
{
    return node->data.fields.page_idx;
}

static void swappool_print (dlpager_swappool_t const * swappool, FILE * ofile)
{
    unsigned int i;
    dlpager_swappool_node_t const * node;
    qurt_addr_t vaddr;
    qurt_paddr_t paddr;

    fprintf( ofile, "region_size    = %u\n", swappool->region_size );
    fprintf( ofile, "seg_addr_base  = 0x%08x\n", swappool->seg_addr_base );
    fprintf( ofile, "max_swap_pages = %u\n", swappool->max_swap_pages );
    fprintf( ofile, "max_glob_pages = %u\n", swappool->max_glob_pages );
    fprintf( ofile, "clean_page_count = %u\n", qurt_sem_get_val( &swappool->clean_page_count ) );

    for ( i = 0; i < DLPAGER_MAX_SWAPMEMS; i++ )
    {
        if ( dlpager_swapmem_is_inited( &swappool->swapmems[ i ] ) )
        {
            fprintf( ofile, "swapmem[ %u ]:\n", i );
            dlpager_swapmem_print( &swappool->swapmems[ i ], ofile );
        }
    }

    fprintf( ofile, "Loaded pages:\n" );
    node = swappool->swap_list_head;
    for ( i = 0; i < swappool->max_swap_pages; i++ )
    {
        vaddr = swappool_node_get_va( node );

        if ( vaddr != 0 )
        {
            paddr = swappool_get_swapmem_physaddr( swappool, node );
            fprintf( stdout, "%3u: raw=0x%08x (memidx=%u pageidx=%u) va=0x%08x pa=0x%08x\n", 
                     i, swappool_node_get_raw( node ), 
                     swappool_node_get_memidx( node ),
                     swappool_node_get_pageidx( node ),
                     vaddr, paddr );
        }

        node = node->next;
    }

    fprintf( ofile, "Unloaded pages:\n" );
    node = swappool->swap_list_head;
    for ( i = 0; i < swappool->max_swap_pages; i++ )
    {
        vaddr = swappool_node_get_va( node );

        if ( vaddr == 0 )
        {
            paddr = swappool_get_swapmem_physaddr( swappool, node );
            fprintf( stdout, "%3u: raw=0x%08x (memidx=%u pageidx=%u) va=0x%08x pa=0x%08x\n", 
                     i, swappool_node_get_raw( node ), 
                     swappool_node_get_memidx( node ),
                     swappool_node_get_pageidx( node ),
                     vaddr, paddr );
        }

        node = node->next;
    }
}

static inline qurt_addr_t swappool_get_virtaddr (dlpager_swappool_t const * pool)
{
    return pool->seg_addr_base;
}

void dlpager_swappool_rx_print (FILE * ofile)
{
    fprintf( ofile, "RX swappool\n" );
    swappool_print( &dlpager_swappool_rx, ofile );
}

void dlpager_swappool_rw_print (FILE * ofile)
{
    fprintf( ofile, "RW swappool\n" );
    swappool_print( &dlpager_swappool_rw, ofile );
}

qurt_addr_t dlpager_swappool_rx_get_virtaddr (void)
{
    return swappool_get_virtaddr( &dlpager_swappool_rx );
}
#endif
