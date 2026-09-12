#include <sys/types.h>
#include <qurt_types.h>
#include <qurt.h>
#include <qurt_memory.h>
#include <errno.h>
#include "gateway_memory.h"


extern qurt_mem_pool_t qurt_mem_default_pool;

#define INVALID_ADDR 0xFFFFFFFFu

/*************************************************************************
* Static functions
*************************************************************************/



/*************************************************************************
* Public functions
*************************************************************************/

/*
* Create mem region of given size with RW protections.
* 
*/
void* gateway_memory_map(size_t len)
{
	int ret_val;
    void *buf = GATEWAY_MAP_FAILED;

    qurt_mem_region_attr_t attr;
    qurt_mem_region_t region;

    if (0 == len)
    {
        return buf;
    }

    qurt_mem_region_attr_init(&attr);

    ret_val= qurt_mem_region_create(&region, len, qurt_mem_default_pool, &attr);
    if (QURT_EOK == ret_val)
    {
        unsigned int vaddr;
        qurt_mem_region_attr_get(region, &attr);
        qurt_mem_region_attr_get_virtaddr(&attr, &vaddr);
        buf = (unsigned int *)vaddr;
    }
    else
    {
        errno = EAGAIN;
    }
    
    return buf;
}

/*
* unmap mem region containing gievn vaddr.
* 
*/
void gateway_memory_unmap(void *vaddr)
{	
    qurt_mem_region_t region;
    
    int ret_val = qurt_mem_region_query(&region, (qurt_addr_t)vaddr, INVALID_ADDR);
    if (QURT_EOK == ret_val)
    {
        qurt_mem_region_delete(region);
        return;
    }
    
    errno = EINVAL;
}

/*
* Change access-protections on given mem region.
* 
*/
int gateway_memory_change_prot(void *vaddr, size_t len, int prot)
{
    qurt_mem_region_t region;
    int ret_val = qurt_mem_region_query(&region, (qurt_addr_t)vaddr, INVALID_ADDR);
    if (QURT_EOK == ret_val)
    {
        if (GATEWAY_MAP_PROT_EXEC & prot)
        {
            /* Align to upper 32 bit value */
            len = ((len+0x1F) & (~0x1F));
            qurt_mem_cache_clean((qurt_addr_t) vaddr, len, QURT_MEM_CACHE_FLUSH, QURT_MEM_DCACHE);
            qurt_mem_cache_clean((qurt_addr_t)vaddr, len, QURT_MEM_CACHE_INVALIDATE, QURT_MEM_ICACHE);
        }
        return 0;
    }
    errno = EINVAL;
    return -1;
}