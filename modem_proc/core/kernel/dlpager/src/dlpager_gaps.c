/*==============================================================================
 * FILE:         dlpager_gaps.c
 *
 * SERVICES:     DL Pager Gap Reclamation
 *
 * DESCRIPTION:  See dlpager_gaps.h
 *
 *  Copyright (c) 2016 by Qualcomm Technologies, Inc.  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/
#include <dlpager_gaps.h>
#include <dlpager_params.h>

/** @brief Virtually contiguous address range for gaps. Value is filled in by
    QuRT image builder *IFF* collection of gaps is configured in QuRT's
    configuruation (XML)
    @warning When using gap reclamation, please ensure that is it configured
    correctly in config/$TARGET/cust_config.xml */
unsigned int DLPager_rw_swap_vaddr  __attribute((section(".data"))) = 0;
unsigned int DLPager_rw_swap_size   __attribute((section(".data"))) = 0;
unsigned int DLPager_rx_swap_vaddr  __attribute((section(".data"))) = 0;
unsigned int DLPager_rx_swap_size   __attribute((section(".data"))) = 0;

/**
 * @brief Gather gaps created by QuRT image builder (a.k.a QuRT gap)
 * 
 * @param [in,out] gaps Gaps available for usage
 * @param [in,out] i Next available gap index
 * @param [in] max_gaps Maximum gaps
 * @param [in] va_gap Virtual address of QuRT gap
 * @param [in] size Size of QuRT gap
 */
static void gather_qurt_gaps (
    dlpager_gap_t *    gaps,
    unsigned int *     i,
    unsigned int const max_gaps,
    qurt_addr_t const  va_gap,
    unsigned int const size
)
{
    qurt_addr_t  va, va_base;
    qurt_paddr_t pa, pa_prev;

    if ( size < PAGE_SIZE )
    {
        /* Gap should be atleast one page */
        return;
    }

    va_base = va_gap;
    va      = va_base;
    pa      = qurt_lookup_physaddr( va );
    pa_prev = pa;

    while ( va < ( va_gap + size ) )
    {
        va += PAGE_SIZE;
        pa  = qurt_lookup_physaddr( va );
        if ( pa != ( pa_prev + PAGE_SIZE ) )
        {
            /* Number of pages in this gap is *ATLEAST* the minimum number of
               pages that is expected from a gap? */
            if ( ( ( va - va_base ) >> PAGE_SHIFT ) >= DLPAGER_GAPS_MIN_NUM_PAGES )
            {
                gaps[ *i ].kind = DLPAGER_GAP_KIND_MEMORY;
                gaps[ *i ].size = ( va - va_base );
                gaps[ *i ].detail.memory.addr = va_base;
                (*i)++;
            }

            va_base = va;

            if ( *i >= max_gaps )
            {
                break;
            }
        }
        pa_prev = pa;
    }

    if ( va_base != va )
    {
        /* Number of pages in this gap is *ATLEAST* the minimum number of
           pages that is expected from a gap? */
        if ( ( ( va - va_base ) >> PAGE_SHIFT ) >= DLPAGER_GAPS_MIN_NUM_PAGES )
        {
            gaps[ *i ].kind = DLPAGER_GAP_KIND_MEMORY;
            gaps[ *i ].size = ( va - va_base );
            gaps[ *i ].detail.memory.addr = va_base;
            (*i)++;
        }
    }
}

/**
 * @brief Gather a named QuRT memory pool as a gap
 *
 * @param [in,out] gaps Gaps available for use
 * @param [in,out] i Next available gap index
 * @param [in] max_gaps Maximum gaps
 * @param [in] pool_name Name of QuRT memory pool
 *
 * @warning Only the first 1MiB of the memory pool are useable as swap
 */
static void gather_pool_as_gap (
    dlpager_gap_t * gaps,
    unsigned int *  i,
    unsigned int    max_gaps,
    char const *    pool_name
)
{
    qurt_mem_pool_t pool;
    qurt_mem_pool_attr_t attr;
    int retval;

    if ( *i >= max_gaps )
    {
        /* We've already maxed gaps! */
        return;
    }

    retval = qurt_mem_pool_attach( ( char * )pool_name, &pool );
    if ( retval != QURT_EOK )
    {
        /* Either pool doesnt exist (ie not defined in configuration XML) or
           there was an error attaching to it. This pool cannot be gathered as
           a gap. */
        return;
    }

    retval = qurt_mem_pool_attr_get( pool, &attr );
    if ( retval != QURT_EOK )
    {
        /* We need to know the size (bytes) of this memory pool to fill in
           the gap information table. Since we are unable to query attributes,
           this pool cannot be gathered as a gap */
        return;
    }

    /* Now fill in the gap information table.
       @warning Statically defined memory pool has only one valid range at the
       first (zeroeth) index */
    gaps[ *i ].kind = DLPAGER_GAP_KIND_POOL;
    gaps[ *i ].size = attr.ranges[ 0 ].size;
    gaps[ *i ].detail.pool.name = pool_name;
    (*i)++;
}

void dlpager_gaps_rx_gather (
    dlpager_gap_t * gaps,
    unsigned int    max_gaps,
    unsigned int *  num_gaps
)
{
    unsigned int i = 0;

    /* Gather pools first. For now, we use a QuRT memory pool that is shared
       between RX swap and TCM dump functionalities.
       @warning Only those pool(s) that are budgeted to be used for RX must be
       used here! */
    gather_pool_as_gap( gaps, &i, max_gaps, DLPAGER_RX_SWAP_TCM_DUMP_SHARED_MEM_POOL_NAME );

    gather_qurt_gaps( gaps, &i, max_gaps, DLPager_rx_swap_vaddr, DLPager_rx_swap_size ); 

    *num_gaps = i;
}

void dlpager_gaps_rw_gather (
    dlpager_gap_t * gaps,
    unsigned int    max_gaps,
    unsigned int *  num_gaps
)
{
    unsigned int i;

    i = 0;
    gather_qurt_gaps( gaps, &i, max_gaps, DLPager_rw_swap_vaddr, DLPager_rw_swap_size ); 

    *num_gaps = i;
}
