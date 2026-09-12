/*==========================================================================
 * FILE:         memload_handler.c
 *
 * SERVICES:     DL PAGER
 *
 * DESCRIPTION:  This file provides the implementation of pager replacement
 *               algorithm. Based on the fault address, missed page is
 *               identified and loaded to a swap page. swap page is identified
 *               using FIFO algorithm.
 *
 * Copyright (c) 2010-2013 Qualcomm Technologies Incorporated.
 * All Rights Reserved. QUALCOMM Proprietary and Confidential.
=============================================================================*/


/*===========================================================================

            EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/01/10   op     Header file for demand paging
07/01/13   bc     Re-write the header file
===========================================================================*/

#include <stdlib.h>
#include <assert.h>
#include <qurt.h>
#include <qurt_cycles.h>
#include <qurt_tlb.h>
#include <hexagon_protos.h>
#include <dlpager_main.h>
#include <memload_handler.h>
#include "dlpager_swapmem.h"
#include "dlpager_log_legacy.h"

extern unsigned int dlpager_log_idx;
extern struct dlpager_log_entry dlpager_log[];
extern int dlpager_log_enabled;
#define MAX_PHYS_PAGE_ENTRIES 64

/* swap pools */
struct swap_page_pool *swap_pools[DLPAGER_MAX_SWAP_POOLS];

void dlpager_remove_node(dlpager_node_t **ring, dlpager_node_t *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
	if (*ring == node) {
		*ring = node->next;
	}
}

void dlpager_insert_head(dlpager_node_t **ring, dlpager_node_t *node)
{
	node->next = *ring;
	node->prev = (*ring)->prev;
	node->prev->next = node;
	node->next->prev = node;
	
	*ring = node;
}

/*
 * init_active_page_pool
 *
 * Initialize swap_page_pool data structure for pager operation. This data
 * structure contains all the information needed for pager operation for
 * the particular swap pool.
 *
 */
void init_active_page_pool (int pool_index, 
                            unsigned int pool_size, unsigned int page_size,
                            unsigned int seg_base, unsigned int seg_size)
{
    int num_swap_pages, num_seg_pages;
    unsigned int shift, i;
    struct swap_page_pool *pool;

    shift = (unsigned short)Q6_R_ct0_R (page_size);
    num_swap_pages = pool_size >> shift;
    num_seg_pages = seg_size >> shift;

    /* Allocate data structure for swap pool managemenet */
    pool = (struct swap_page_pool *)malloc (sizeof (struct swap_page_pool) +
		          (sizeof (unsigned int) * ( num_seg_pages )) + (sizeof(dlpager_node_t) * num_swap_pages));				  

    assert (pool != NULL);

    swap_pools[pool_index] = pool;

    pool->seg_addr_base = seg_base;
    pool->page_shift = shift;
    pool->max_swap_pages = num_swap_pages;
    pool->max_glob_pages = num_seg_pages;
    pool->loaded_pages = (dlpager_node_t*)(pool + 1);
    pool->global_pages = (unsigned int*)&pool->loaded_pages[num_swap_pages];

    /* Initialize loadable pages to reflect no mapping */
    for (i = 0; i < num_swap_pages; i++) {
        pool->loaded_pages[i].next = &pool->loaded_pages[i+1];
        pool->loaded_pages[i].prev = &pool->loaded_pages[i-1];		
        pool->loaded_pages[i].vaddr = 0;
    }

    pool->loaded_pages[0].prev = &pool->loaded_pages[num_swap_pages-1];
    pool->loaded_pages[num_swap_pages-1].next = &pool->loaded_pages[0];
    pool->swap_list_head =  &pool->loaded_pages[0];
	

    /* Initialize global pages to reflect no mapping */
    for (i = 0; i < num_seg_pages; i++) {
        pool->global_pages[i] = 0;
    }

    /* Initialize log */
    dlpager_log_idx = 0;
    for (i = 0; i < DLPAGER_MAX_LOG_ENTRIES; i++) {
        dlpager_log[i].loaded_addr = 0;
        dlpager_log[i].evicted_addr = 0;
        dlpager_log[i].thread_id = 0;
    }
}

void loaded_pages_update(int pool_index)
{
    struct swap_page_pool *pool;    
    unsigned int i,index;
	int num_active_pages;
    dlpager_node_t* update_node;
    unsigned int* dlpager_tlb_phys_addr;
	
    pool = swap_pools[pool_index];
	

    /*Request for Phys addrs of the Paged segments curretently in TLB*/
	num_active_pages = qurt_tlb_get_pager_physaddr(&dlpager_tlb_phys_addr);

	DL_DEBUG("the no of TLBentries:%d\n",num_active_pages);
    if(num_active_pages > 0){
		
        for (i = 0; i < num_active_pages; i++)
        {   
            DL_DEBUG("phys addr in TLB:0x%x \n", dlpager_tlb_phys_addr[i], i);
            index = dlpager_swapmem_getidx_from_pa(dlpager_tlb_phys_addr[i]);
            
			/*Get the node which needs to be touched*/
		    update_node = &pool->loaded_pages[index];
		    dlpager_remove_node(&(pool->swap_list_head), update_node);
		    dlpager_insert_head(&(pool->swap_list_head), update_node);		
        }

    }	
}

/*
 * insert_active_page
 *
 * Create mapping for the fault page and make it active. Set the next_victim
 * index appropriately.
 *
 * param[in]  pool_index   - swap pool index of the fault address
 *            fault_addr   - base address of the fault page
 *            phys_addr    - phys address where are the fault page is placed
 */
static int insert_active_page (int pool_index, unsigned int fault_addr,
                               unsigned int phys_addr)
{
    struct swap_page_pool *pool = swap_pools[pool_index];
    unsigned short shift;
    unsigned short load_idx, global_idx;
    int rc;
    dlpager_node_t*  victim_node;
	

    shift = pool->page_shift;
    /*Victim is always at the tail of the list
     * we removed the mapping of the tail node earlier */
    victim_node = pool->swap_list_head->prev;

    dlpager_remove_node(&(pool->swap_list_head),victim_node);
    load_idx = (victim_node - pool->loaded_pages) ;

    DL_DEBUG("victim_node:0x%x, v_addr_mapped:0x%x  \n",victim_node,victim_node->vaddr);

    //assert (phys_addr == (pool->p_addr_base + (load_idx << shift)));

    if (dlpager_is_addr_in_rx_range(fault_addr))
    rc = qurt_mapping_create (fault_addr, phys_addr, 1 << shift, QURT_MEM_CACHE_WRITEBACK, QURT_PERM_READ | QURT_PERM_EXECUTE);
    else if (dlpager_is_addr_in_rw_range(fault_addr))
      rc = qurt_mapping_create(fault_addr, phys_addr, 1 << shift, QURT_MEM_CACHE_WRITEBACK, QURT_PERM_READ | QURT_PERM_WRITE); 
    else
    {
      DL_DEBUG("fault_addr not in any range\n");
      assert(0);
    }

    DL_DEBUG ("Inserting Page: vaddr=0x%x paddr=0x%x index=%d\n", fault_addr, phys_addr, load_idx);

    /*Always insert at the head of the list*/
    dlpager_insert_head(&(pool->swap_list_head), victim_node);
    pool->loaded_pages[load_idx].vaddr = fault_addr;

    /* Add mapping to the global table */
    global_idx = (fault_addr - pool->seg_addr_base) >> shift;
    pool->global_pages[global_idx] = phys_addr;

    return rc;
}


/* This function removes a page entry from the active list only if the list is currently full
   Returns:  non-zero if list was full and entry was removed
             zero if list is not full */
/*
 * remove_active_page
 *
 * Identify the a victim page that can be used to map the fault page. To start
 * with none of the physical pages (swap pages) are mapped and they can be
 * used straight away. Once all the available swap pages are mapped we remove
 * existing mappngs using FIFO algorithm.
 * If an existing mapping is removed, we need to perform cache operations for
 * correct behavior.
 *
 * param[in]  pool_index   - swap pool index of the fault address
 *
 * param[out] virt_addr    - virtual address of the evicted page that can be
 *                           used to copy the fault page.
 *            phys_addr    - physical address of the evicted page
 *
 * return     0       - if no page from the segment is activley mapped.
 *            address - virtual addres  of the active page being evicted. This
 *            virtual address corresponds to the segment.
 */
static unsigned int remove_active_page (int pool_idx, unsigned int *virt_addr,
                                        unsigned int *phys_addr)
{
    struct swap_page_pool *pool = swap_pools[pool_idx];
    int victim_idx, global_idx, shift;
    unsigned int v_rw_addr, v_rx_addr, p_addr, v_addr_mapped;
    dlpager_node_t*  tail_node;

    tail_node = pool->swap_list_head->prev;
    shift = pool->page_shift;

    victim_idx = tail_node - pool->loaded_pages;

    DL_DEBUG("Tail_node:0x%x, swap_head:0x%x,  victim_idx:0x%d\n",tail_node, pool->swap_list_head, victim_idx);

    v_rw_addr = dlpager_swapmem_getrw(victim_idx);
    v_rx_addr = dlpager_swapmem_getrx(victim_idx);
    p_addr = dlpager_swapmem_getpa(victim_idx);
    v_addr_mapped = pool->loaded_pages[victim_idx].vaddr;

    *virt_addr = v_rw_addr;
    *phys_addr = p_addr;

    /* Check if we need to evict the page */
    if (v_addr_mapped) {
        DL_DEBUG ("Removing Page: vaddr=0x%x paddr=0x%x index=%d\n", v_addr_mapped, p_addr, victim_idx);
        qurt_mapping_remove (v_addr_mapped, p_addr, 1 << shift);

        /* Invalidate I-cache. We don't need to invalidate D-cache because
         * we will write to the page before we map and use */
        //qurt_mem_icache_inv ((const void *)v_rx_addr, 1 << shift);
        if (dlpager_is_addr_in_rx_range(v_addr_mapped))
        {
        qurt_mem_cache_clean ((qurt_addr_t)v_rx_addr, 1 << shift,
                          QURT_MEM_CACHE_INVALIDATE, QURT_MEM_ICACHE);
        }

        /* Remove mapping from the global table */
        global_idx = (v_addr_mapped - pool->seg_addr_base) >> shift;
        pool->global_pages[global_idx] = 0;
    }

    return v_addr_mapped;
}

/*
 * memload_fault_handler
 *
 * This function activates the fault page. If all physical pages are mapped, a
 * page is evicted using FIFO algorithm. This function uses utility functions
 * to help remove and insert mapping.
 *
 * param[in]  faul_addr - Address that caused page fault
 *            thread_id - Thread ID that accessed the memory
 *
 * return - 0 for SUCCESS
 */
int memload_fault_handler (unsigned int fault_addr, qurt_thread_t thread_id)
{ /* XXX: Hard code pool index to "0" */
    int pool_index = 0;

    struct swap_page_pool *pool;
    unsigned int fault_addr_align;
    unsigned int shift, page_idx;
    uintptr_t victim_paddr;
    unsigned int load_addr; //this is vadd
    unsigned int evicted_addr;
    int log_idx = dlpager_log_idx;
   
    pool = swap_pools[pool_index];
    shift = pool->page_shift;

    page_idx = (fault_addr - pool->seg_addr_base) >> shift;
  
    /* check for duplicate page fault, if so don't handle it and return */
    if (pool->global_pages[page_idx] != 0) {
        DL_DEBUG ("duplicate page fault... mapping already exists \n");
	return 0;
    }

	/* updates the PLRU info of the swap pool. */
    loaded_pages_update(pool_index);
	
    /* Provide a load address. Remove an entry if needed. Returns
     * evicted page */
    evicted_addr = remove_active_page (pool_index, &load_addr, &victim_paddr);

    fault_addr_align = DLPAGER_ALIGN_DOWN (fault_addr, (1 << shift));

    if (dlpager_log_enabled) {
        dlpager_log_idx = ((dlpager_log_idx + 1) < DLPAGER_MAX_LOG_ENTRIES) ?
            dlpager_log_idx + 1 : 0;

        dlpager_log[log_idx].loaded_addr = fault_addr;
        dlpager_log[log_idx].evicted_addr = evicted_addr;
        dlpager_log[log_idx].thread_id = thread_id;
        dlpager_log[log_idx].pcycles = 0;
        dlpager_log[log_idx].ticks = 0;
    }
    if ((evicted_addr != 0)
		&& dlpager_is_addr_in_rw_range(evicted_addr))
    {
      dlpager_store_virtual_page(load_addr, evicted_addr);
    }
    dlpager_load_virtual_page (fault_addr_align, load_addr, 1 << shift);

    //qurt_mem_dcache_cleaninv ((const void *)load_addr, 1 << shift);
    qurt_mem_cache_clean ((qurt_addr_t)load_addr, 1 << shift,
                          QURT_MEM_CACHE_FLUSH_INVALIDATE, QURT_MEM_DCACHE);

    insert_active_page (pool_index, fault_addr_align, victim_paddr);            
    /* Fill log entry */
    if (dlpager_log_enabled) {
        dlpager_log[log_idx].pcycles = qurt_get_core_pcycles();
        dlpager_log[log_idx].ticks   = qurt_sysclock_get_hw_ticks();
    }

    return 0;
}

/*
 * dump_global_table
 *
 * Dump the page mapping of the entire segment of each swap pool
 */
void dump_global_table (void)
{
    int i, j;
    unsigned int seg_addr;
    struct swap_page_pool *pool;

    for (i = 0; i < DLPAGER_MAX_SWAP_POOLS; i++) {
        pool = swap_pools[i];
        printf ("seg page    --- swap page\n");
        for (j = 0; j < pool->max_glob_pages; j++) {
            seg_addr = pool->seg_addr_base + (j << pool->page_shift);
            printf ("0x%x  ---  ", seg_addr);
            if (pool->global_pages[j]) {
                printf ("0x%x\n", pool->global_pages[j]);
            }
            else {
                printf ("------\n");
            }
        }
    }
}

/*
 * dump_swap_table
 *
 * Dump the page mapping of the swap pool. There should be one-to-one
 * correspondence between swap table and global table.
 */
void dump_swap_table (void)
{
    int i, j;
    unsigned int phy_addr;
    char empty_string[] = "             ";
    struct swap_page_pool *pool;

    for (i = 0; i < DLPAGER_MAX_SWAP_POOLS; i++) {
        pool = swap_pools[i];
        printf ("swap pool: %d swap page   --- seg page\n", i);
        for (j = 0; j < pool->max_swap_pages; j++) {
            phy_addr = pool->p_addr_base + (j << pool->page_shift);
            printf ("%s0x%x  ---  ", empty_string, phy_addr);
            if (pool->loaded_pages[j].vaddr) {
                printf ("0x%x\n", pool->loaded_pages[j].vaddr);
            }
            else {
                printf ("------\n");
            }
        }
    }
}

/*
 * dump_log_table
 *
 * Dump the logs that indicate the activity of the pager.
 */
void dump_log_table (void)
{
    int i;
    char empty_string[] = "           ";

    printf ("Log index is at: %d\n\n", dlpager_log_idx);
    printf ("Log table: fault addr -- loaded page  --- thread ID\n");

    i = ((dlpager_log_idx + 1) < DLPAGER_MAX_LOG_ENTRIES) ? dlpager_log_idx + 1 : 0;
    do { 
        printf ("%s0x%x  --  0x%x  -- 0x%x\n", empty_string, dlpager_log[i].loaded_addr, dlpager_log[i].evicted_addr, dlpager_log[i].thread_id);
        
        i = ((i + 1) < DLPAGER_MAX_LOG_ENTRIES) ? i + 1 : 0;
    } while (i != dlpager_log_idx);

}
