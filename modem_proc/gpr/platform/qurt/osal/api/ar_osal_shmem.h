#ifndef AR_OSAL_SHMEM_H
#define AR_OSAL_SHMEM_H

/**
 * \file ar_osal_shmem.h
 * \brief
 *     Defines public APIs for shared memory allocation for DSP.
 * \copyright
 *  Copyright (c) 2020 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#include "ar_osal_types.h"
#include "ar_osal_sys_id.h"

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/


 /** enum for shmem memory type*/
typedef enum ar_shmem_memory_type
{
	/** 0  Shared physical memory allocation.*/
	AR_SHMEM_PHYSICAL_MEMORY = 0,
	/** 1  Shared virtual memory allocation */
	AR_SHMEM_VIRTUAL_MEMORY = 1
}ar_shmem_memory_type_t;

/** enum for shmem cache type*/
typedef enum ar_shmem_cache_type
{
	/** 0  cached.*/
	AR_SHMEM_CACHED = 0,
	/** 1  uncached.*/
	AR_SHMEM_UNCACHED = 1
}ar_shmem_cache_type_t;

/** enum for shmem offset/address buffer index type*/
typedef enum ar_shmem_buffer_index_type
{
	/** 0  use physical or virtual addresses.*/
	AR_SHMEM_BUFFER_ADDRESS = 0,
	/** 1  use offsets, the offset is from the base address.*/
	AR_SHMEM_BUFFER_OFFSET = 1
}ar_shmem_buffer_index_type_t;

 /**
 * Shared memory info structure
 */
typedef struct ar_shmem_info_t
{
	ar_shmem_cache_type_t          cache_type;     /**< in, cache type, cached or uncached memory */
	size_t                           buf_size;       /**< in, shared buffer size, should be a minimum of 4K and multiple of 4K only*/
	ar_shmem_memory_type_t         mem_type;       /**< out, shmem memory type, virtual or physical */
	ar_shmem_buffer_index_type_t   index_type;     /**< out, DSP to operate on buffer offsets or on address pointers.*/
	uint32_t                         ipa_lsw;        /**< out, ipa lsw, alignment requirements apply, like 4k */
	uint32_t                         ipa_msw;        /**< out, ipa msw, alignment requirements apply, like 4k  */ 
	uint32_t                         pa_lsw;         /**< out, physical address lsw, alignment requirements apply, like 4k  */
	uint32_t                         pa_msw;         /**< out, physical address msw, alignment requirements apply, like 4k  */
	void                            *vaddr;          /**< out, virtual address 64bit/32bit, alignment requirements apply, like 4k  */
	uint64_t                         metadata;       /**< out, pointer address to metadata structure defined by each platform.*/
	uint8_t                          num_sys_id;     /**< in, number of subsystem IDs provided */
	uint8_t                         *sys_id;         /**< in, pointer to array of size num_sys_id for sub-system Ids provided in ar_osal_sys_id.h, 
                                                          used to allocate shared memory between the given list of sys_id.*/
    uint32_t                         platform_info;  /**< in opt, optional field for passing platform specific data to OSAL, this can be
                                                          used for example to communicate some heap properties.*/
} ar_shmem_info;
 
/**
 * \brief ar_shmem_init
 *        initialize shared memory interface.
 * \return
 *  0 -- Success
 *  Nonzero -- Failure 
 */
int32_t ar_shmem_init(void);

/**
 * \brief Allocates shared memory.
 *  Only non cached memory allocation supported.
 *  Size if multiple of 4KB and the returned is aligned to 4KB boundary.
 *  Buffer start address should be atleast 64bit multiple aligned.
 *
 * \param[in_out] info: pointer to ar_shmem_info. 
 *
 * \return
 *  0 -- Success
 *  Nonzero -- Failure
 *
 */
int32_t ar_shmem_alloc(ar_shmem_info *info);

/**
 * Frees shared memory.
 *
 * \param[in] info: pointer to ar_shmem_info. 
 *
 * \return
 *  0 -- Success
 *  Nonzero -- Failure
 */
int32_t ar_shmem_free(ar_shmem_info *info);

/**
 * \brief ar_shmem_deinit. 
 *
 * \return
 *  0 -- Success
 *  Nonzero -- Failure
 */
int32_t ar_shmem_deinit(void);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* #ifndef AR_OSAL_SHMEM_H */

