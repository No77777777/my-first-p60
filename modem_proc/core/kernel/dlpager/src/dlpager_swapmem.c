/*==============================================================================
 * FILE:         dlpager_swapmem.h
 *
 * SERVICES:     DL Pager Swap Memory
 *
 * DESCRIPTION:  See dlpager_swapmem.c
 *
 *  Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/

/*==============================================================================

  EDIT HISTORY FOR MODULE
  
  
when       who     what, where, why
-------- -------- --------------------------------------------------------------
07/01/16 anandj   Re-design to support gap reclamation in async DL pager
09/30/14 ao       Initial revision
==============================================================================*/

#include <assert.h>
#include <dlpager_params.h>
#include <dlpager_swapmem.h>
#include <qurt.h>

static void reserve_virtaddr_range (qurt_mem_region_attr_t * attr, unsigned int num_pages)
{
    qurt_mem_pool_t   pool;
    qurt_mem_region_t region;
    int retval;

    retval = qurt_mem_pool_attach( "DEFAULT_PHYSPOOL", &pool );
    ASSERT( QURT_EOK == retval );

    qurt_mem_region_attr_init( attr );
    qurt_mem_region_attr_set_mapping( attr, QURT_MEM_MAPPING_NONE );

    retval = qurt_mem_region_create( &region, num_pages * PAGE_SIZE, pool, attr );
    ASSERT( QURT_EOK == retval );

    retval = qurt_mem_region_attr_get( region, attr );
    ASSERT( QURT_EOK == retval );
}

void dlpager_swapmem_create_from (
    dlpager_swapmem_t * swapmem,
    qurt_addr_t         vaddr,
    unsigned int        size,
    qurt_perm_t         perms
)
{
    unsigned int num_pages;
    qurt_mem_region_attr_t attr;
    qurt_paddr_t paddr;
    qurt_size_t region_size;
    int retval;

    num_pages = size >> PAGE_SHIFT;
    if ( num_pages == 0 )
    {
        /* Size is smaller than a page? Anyways, the swapmem is uninitialized */
        return;
    }

    /* Cap the number of pages to the maximum this swap memory instance can handle */
    num_pages = DLPAGER_MIN( num_pages, DLPAGER_SWAPMEM_MAX_PAGES );

    paddr = qurt_lookup_physaddr( vaddr );

    /* Incoming virtual address is expected to be mapped! */
    ASSERT( paddr != 0 );

    reserve_virtaddr_range( &attr, num_pages );

    swapmem->paddr     = paddr;
    swapmem->num_pages = num_pages;
    swapmem->perms     = perms;
    qurt_mem_region_attr_get_virtaddr( &attr, &swapmem->vaddr_rw );

    retval = qurt_mapping_create(
        swapmem->vaddr_rw,
        swapmem->paddr,
        swapmem->num_pages * PAGE_SIZE,
        QURT_MEM_CACHE_WRITEBACK,
        QURT_PERM_READ | QURT_PERM_WRITE );
    ASSERT( QURT_EOK == retval );

    /* Invalidate as precaution against prior ownership of underlying physical
       memory */
    retval = qurt_mem_cache_clean(
        swapmem->vaddr_rw,
        swapmem->num_pages * PAGE_SIZE,
        QURT_MEM_CACHE_INVALIDATE,
        QURT_MEM_DCACHE );
    ASSERT( QURT_EOK == retval );

    if ( perms & QURT_PERM_EXECUTE )
    {
        reserve_virtaddr_range( &attr, num_pages );

        qurt_mem_region_attr_get_virtaddr( &attr, &swapmem->vaddr_rx );
        qurt_mem_region_attr_get_size( &attr, &region_size );

        ASSERT( region_size == ( num_pages << PAGE_SHIFT ) );

        retval = qurt_mapping_create(
            swapmem->vaddr_rx,
            swapmem->paddr,
            region_size,
            QURT_MEM_CACHE_WRITEBACK,
            QURT_PERM_READ | QURT_PERM_EXECUTE );
        ASSERT( QURT_EOK == retval );

        retval = qurt_mem_cache_clean(
            swapmem->vaddr_rx,
            region_size,
            QURT_MEM_CACHE_INVALIDATE,
            QURT_MEM_ICACHE );
        ASSERT( QURT_EOK == retval );
    }

    swapmem->is_inited = 1;
}

void dlpager_swapmem_create (
    dlpager_swapmem_t * swapmem, 
    unsigned short      num_pages,
    char const *        pool_name,
    unsigned int        pool_size,
    qurt_perm_t         perms
)
{
    qurt_mem_pool_t mem_pool;
    qurt_mem_region_attr_t attr;
    int retval;

    memset( swapmem, 0, sizeof( *swapmem ) );

    if ( num_pages == 0 )
    {
        return;
    }

    /* Cap the number of pages to the maximum this swap memory instance can handle */
    num_pages = DLPAGER_MIN( num_pages, DLPAGER_SWAPMEM_MAX_PAGES );

    if ( pool_name )
    {
        /* @todo Ugly cast. Ideally, QuRT API should accept immutable strings
           as const char * */
        retval = qurt_mem_pool_attach( ( char * ) pool_name, &mem_pool );
        if ( QURT_EOK != retval )
        {
            /* @warning Unable to attach to specified memory pool. Swap-memory
               is unitialized! */
            return;
        }

        /* Cap the number of pages to the maximum this memory pool can handle. If
           there is no sufficient physical memory to back the swap pages, it is the
           responsibility of swappool_create_swap() to detect this and abort */
        num_pages = DLPAGER_MIN( num_pages, pool_size >> PAGE_SHIFT );
    }
    else
    {
        retval = qurt_mem_pool_attach( "DEFAULT_PHYSPOOL", &mem_pool );
    }

    ASSERT( QURT_EOK == retval );

    qurt_mem_region_attr_init( &attr );
    qurt_mem_region_attr_set_mapping( &attr, QURT_MEM_MAPPING_VIRTUAL_RANDOM );

    retval = qurt_mem_region_create( &swapmem->region, num_pages * PAGE_SIZE, mem_pool, &attr );
    ASSERT( QURT_EOK == retval );

    swapmem->num_pages = num_pages;
    swapmem->perms     = perms;

    retval = qurt_mem_region_attr_get( swapmem->region, &attr );
    ASSERT( QURT_EOK == retval );

    qurt_mem_region_attr_get_virtaddr( &attr, &swapmem->vaddr_rw );
    qurt_mem_region_attr_get_physaddr( &attr, &swapmem->paddr );

    /* Invalidate as a precaution against prior ownership of underlying physical
       memory */
    retval = qurt_mem_cache_clean(
        swapmem->vaddr_rw,
        swapmem->num_pages * PAGE_SIZE,
        QURT_MEM_CACHE_INVALIDATE, 
        QURT_MEM_DCACHE );
    ASSERT( QURT_EOK == retval );

    if ( perms & QURT_PERM_EXECUTE )
    {
        qurt_mem_region_attr_init( &attr );
        qurt_mem_region_attr_set_mapping( &attr, QURT_MEM_MAPPING_NONE );
        retval = qurt_mem_region_create( &swapmem->region, num_pages * PAGE_SIZE, mem_pool, &attr );
        ASSERT( retval == QURT_EOK );

        qurt_mem_region_attr_get( swapmem->region, &attr );
        ASSERT( QURT_EOK == retval );

        qurt_mem_region_attr_get_virtaddr( &attr, &swapmem->vaddr_rx );

        /* Map the virtual memory addresses with RX permissions so we can 
           manipulate the I cache through the virtual addresses. */
        retval = qurt_mapping_create( 
            swapmem->vaddr_rx, 
            swapmem->paddr,
            num_pages * PAGE_SIZE,
            QURT_MEM_CACHE_WRITEBACK,
            QURT_PERM_READ | QURT_PERM_EXECUTE );
        ASSERT( QURT_EOK == retval );

        /* Invalidate as a precaution against prior ownership of underlying 
           physical memory */
        retval = qurt_mem_cache_clean(
            swapmem->vaddr_rx,
            swapmem->num_pages * PAGE_SIZE,
            QURT_MEM_CACHE_INVALIDATE, 
            QURT_MEM_ICACHE );
        ASSERT( QURT_EOK == retval );
    }

    swapmem->is_inited = 1;
}

#if defined( DLPAGER_UNIT_TEST )
void dlpager_swapmem_print (dlpager_swapmem_t const * swapmem, FILE * ofile)
{
    fprintf( ofile, "Swap memory\n" );
    fprintf( ofile, "num_pages = %u\n", swapmem->num_pages );
    fprintf( ofile, "perms     = 0x%04x\n", swapmem->perms );
    fprintf( ofile, "paddr     = 0x%08x\n", swapmem->paddr );
    fprintf( ofile, "vaddr_rw  = 0x%08x\n", swapmem->vaddr_rw );
    fprintf( ofile, "vaddr_rx  = 0x%08x\n", swapmem->vaddr_rx );
}

void dlpager_swapmem_reserve_virtaddr_range (
    qurt_mem_region_attr_t * attr,
    unsigned int             num_pages
)
{
    return reserve_virtaddr_range( attr, num_pages );
}

#endif
