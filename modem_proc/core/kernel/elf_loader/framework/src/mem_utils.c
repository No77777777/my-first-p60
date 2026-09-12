/* =========================================================================

                              M E M U T I L S

DESCRIPTION
  malloc, realloc, calloc and free calls for Elf Loader.

Copyright (c) 2006-2014 by Qualcomm Technologies Incorporated.  All Rights Reserved.
============================================================================ */

/* =========================================================================

                             Edit History

$Header: //components/rel/core.mpss/10.0/kernel/elf_loader/framework/src/mem_utils.c#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------
09/07/15   ps      File Created
============================================================================ */


/*============================================================================
                             INCLUDE FILES
============================================================================*/
#include <stdlib.h>
#include <stdio.h>
/*============================================================================
                             DEFINES AND GLOBAL VARS
============================================================================*/
#define MALLOC_IMP_4K_SIZE 0x1000
#define MALLOC_IMP_1K_SIZE 0x800
#define MAX_NUM_4K_BLOCKS 4
#define MAX_TYPE_SIZE 8

#ifndef _UINT64_DEFINED
typedef unsigned long long  uint64;      /**< Unsigned 64-bit value. */
#define _UINT64_DEFINED
#endif



typedef struct mem_pool_4k
{
    unsigned short used;
    uint64 storage[MALLOC_IMP_4K_SIZE/MAX_TYPE_SIZE];
} mem_pool_4k_t;

typedef struct mem_pool_1k
{
    uint64 storage[MALLOC_IMP_1K_SIZE/MAX_TYPE_SIZE];
    uint64 *current_post_ptr;
    unsigned int bytes_left;
} mem_pool_1k_t;

static mem_pool_4k_t mem_pool_4k_q[MAX_NUM_4K_BLOCKS] = {{0}};
static mem_pool_1k_t mem_pool_1k_q = {{0}};

static void* malloc_imp_get_from_4k_block(void)
{
    int idx;
    for(idx =0; idx < MAX_NUM_4K_BLOCKS; idx++)
    {
        if(mem_pool_4k_q[idx].used == 0)
        {
            mem_pool_4k_q[idx].used = 1;
            return &mem_pool_4k_q[idx].storage;
        }
    }
    return NULL;
}

static void* malloc_imp_get_from_1k_block(size_t size)
{
    int idx = (size % MAX_TYPE_SIZE) ? ((size/MAX_TYPE_SIZE) + 1) : (size/MAX_TYPE_SIZE);
    if(size <= mem_pool_1k_q.bytes_left)
    {
        void *addr = mem_pool_1k_q.current_post_ptr;
        mem_pool_1k_q.current_post_ptr+=idx;
        mem_pool_1k_q.bytes_left -=  (idx * MAX_TYPE_SIZE);
        return addr;
    }
    return NULL;
}

void malloc_imp_init(void)
{
    mem_pool_1k_q.current_post_ptr = (uint64*) &mem_pool_1k_q.storage;
    mem_pool_1k_q.bytes_left = MALLOC_IMP_1K_SIZE;
}

void* malloc_cust (size_t size) {
    void * addr;
    if(MALLOC_IMP_1K_SIZE < size  && size <= MALLOC_IMP_4K_SIZE)
    {
        addr = malloc_imp_get_from_4k_block();
        return addr;
    }

    addr = malloc_imp_get_from_1k_block(size);
    return addr;
}

void free_cust (void* ptr) {
    return;
}