/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               DL PAGER TOP LEVEL HEADER FILE

GENERAL DESCRIPTION

  Copyright (c) 2010 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

            EDIT HISTORY FOR MODULE

$Header: //source/qcom/qct/core/kernel/dl_pager_qurt/dev/async_pager/inc/dlpager.h

===========================================================================*/
#pragma once

#define THREAD_NULL (0xffffffff)

/* Align to page boundry with possible truncation 
 * size is a power of 2 */
#define DLPAGER_ALIGN_DOWN(addr, size) (addr & ~(size - 1))

/* Align to page boundry rounding up to a higher address page
 * size is a power of 2 */
#define DLPAGER_ALIGN_UP(addr, size) ((addr + size - 1) & ~(size - 1))

/** @brief Datatype for result of DL pager swap reset operation */
typedef struct
{
    /** @brief Number of pages in the swap that were reset/cleaned */
    unsigned int num_pages_reset;

    /** @brief Duration (in ticks) to complete the swap reset */
    unsigned int duration;

} dlpager_swap_reset_result_t;

int __attribute__((__constructor__)) dlpager_init(void);

/**
 * @brief Reset RX swap 
 * @warning This function has an expectation of execution in single threaded 
 * mode and therefore *MUST BE* invoked from dlpager_lpr_rx_swap_dump_enter 
 * only! 
 */
extern void dlpager_reset_rx (void);

#if defined( DLPAGER_TEST_RESET_RX_SWAP )
/**
 * @brief Test interface for dlpager_reset_rx()
 * 
 * @param [out] result Result of RX swap reset operation
 */
void dlpager_test_reset_rx (dlpager_swap_reset_result_t * result);
#endif

