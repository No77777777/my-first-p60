#pragma once
/*==============================================================================
 * FILE:         dlpager_gaps.h
 *
 * SERVICES:     DL Pager Gap Reclamation
 *
 * DESCRIPTION:  This module implements gap reclamation support for DL Pager.
 *               Gap is a piece of physical memory that is unused due to
 *               alignments specified in linker script. QuRT image builder
 *               supports (when enabled via configuration) collecting these gaps
 *               and consolidating them under virtually contiguous mappings. This
 *               provides operations to traverse the virtual mapping breaking it
 *               down into sets of physically contiguous gaps for consumption
 *               by DL Pager Swap-pool (dlpager_swappool.c/h)
 *
 *  Copyright (c) 2016 by Qualcomm Technologies, Inc.  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/
#include <qurt.h>

/** @brief Kinds of gaps */
typedef enum
{
    DLPAGER_GAP_KIND_POOL,   /**< Gap from a pool (eg: sharing TCM dump pool) */
    DLPAGER_GAP_KIND_MEMORY, /**< Gap from a piece of memory (eg: QuRT gap)   */

} dlpager_gap_kind_t;

/** @brief Abstract datatype for a gap */
typedef struct
{
    /** @brief Kind of this gap */
    dlpager_gap_kind_t kind;

    /** @brief Size (bytes) of this gap */
    unsigned int       size;

    /** @brief Detail about this gap based on its kind */
    union
    {
        /** @brief Gap from a pool */
        struct
        {
            char const * name;
        } pool;

        /** @brief Gap from a piece of memory (typically consolidated by QuRT image
                   builder) */
        struct
        {
            qurt_addr_t addr;
        } memory;
    } detail;

} dlpager_gap_t;

/**
 * @brief Gather gaps for usage in RX (read-execute) swap-pool
 *  
 * @param [out] gaps Gaps available for usage
 * @param [in] max_gaps Maximum gaps that can be filled by callee
 * @param [out] num_gaps Number of gaps available for usage
 */
extern void dlpager_gaps_rx_gather (
    dlpager_gap_t * gaps,
    unsigned int    max_gaps, 
    unsigned int *  num_gaps
);

/**
 * @brief Gather gaps for usage in RW (read-write) swap-pool
 * 
 * @param [out] gaps Gaps available for usage
 * @param [in] max_gaps Maximum gaps that can be filled by callee
 * @param [out] num_gaps Number of gaps available for usage
 */
extern void dlpager_gaps_rw_gather (
    dlpager_gap_t * gaps,
    unsigned int      max_gaps,
    unsigned int *    num_gaps
);

