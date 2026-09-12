/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               DL PAGER HEADER FILE

GENERAL DESCRIPTION

  Copyright (c) 2010 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

            EDIT HISTORY FOR MODULE

$Header: //source/qcom/qct/core/kernel/dl_pager_qurt/dev/async_pager/inc/dlpager.h

when       who          what, where, why
--------   --------     ----------------------------------------------------------
12/09/14   rc,rr,cp     Header file for pagesm handlers
===========================================================================*/
#include <qurt.h>

void dlpager_handler_tlb_miss_x_at_unmapped_clean( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_r_at_unmapped_clean( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_w_at_unmapped_clean( unsigned int, qurt_thread_t );

void dlpager_handler_tlb_miss_r_at_unmapped_bss( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_w_at_unmapped_bss( unsigned int, qurt_thread_t );

void dlpager_handler_tlb_miss_x_at_unmapped_clean_decompressing( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_r_at_unmapped_clean_decompressing( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_w_at_unmapped_clean_decompressing( unsigned int, qurt_thread_t );
void dlpager_handler_decompression_complete_at_unmapped_clean_decompressing( unsigned int, qurt_thread_t );

void dlpager_handler_tlb_miss_r_at_unmapped_dirty_decompressing( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_w_at_unmapped_dirty_decompressing( unsigned int, qurt_thread_t );
void dlpager_handler_decompression_complete_at_unmapped_dirty_decompressing( unsigned int, qurt_thread_t );

void dlpager_handler_tlb_miss_r_at_unmapped_dirty_compressing_allocated( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_w_at_unmapped_dirty_compressing_allocated( unsigned int, qurt_thread_t );
void dlpager_handler_compression_complete_at_unmapped_dirty_compressing_allocated( unsigned int, qurt_thread_t );
void dlpager_handler_soft_clean_failed_at_unmapped_dirty_compressing_allocated( unsigned int, qurt_thread_t );

void dlpager_handler_tlb_miss_r_at_unmapped_clean_allocated( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_w_at_unmapped_clean_allocated( unsigned int, qurt_thread_t );
void dlpager_handler_evict_page_at_unmapped_clean_allocated( unsigned int, qurt_thread_t );

void dlpager_handler_tlb_miss_r_at_unmapped_hard_clean_compressing( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_w_at_unmapped_hard_clean_compressing( unsigned int, qurt_thread_t );
void dlpager_handler_compression_complete_at_unmapped_hard_clean_compressing( unsigned int, qurt_thread_t );

void dlpager_handler_tlb_miss_x_at_mapped_clean( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_r_at_mapped_clean( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_w_at_mapped_clean( unsigned int, qurt_thread_t );
void dlpager_handler_evict_page_at_mapped_clean( unsigned int, qurt_thread_t );

void dlpager_handler_tlb_miss_r_at_mapped_dirty( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_w_at_mapped_dirty( unsigned int, qurt_thread_t );
void dlpager_handler_soft_clean_page_at_mapped_dirty( unsigned int, qurt_thread_t );
void dlpager_handler_hard_clean_page_at_mapped_dirty( unsigned int, qurt_thread_t );

void dlpager_handler_tlb_miss_r_at_mapped_dirty_compressing( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_w_at_mapped_dirty_compressing( unsigned int, qurt_thread_t );
void dlpager_handler_compression_complete_at_mapped_dirty_compressing( unsigned int, qurt_thread_t );

void dlpager_handler_tlb_miss_r_at_mapped_clean_compressing( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_w_at_mapped_clean_compressing( unsigned int, qurt_thread_t );
void dlpager_handler_compression_complete_at_mapped_clean_compressing( unsigned int, qurt_thread_t );
void dlpager_handler_soft_clean_failed_at_mapped_clean_compressing( unsigned int, qurt_thread_t );

void dlpager_handler_tlb_miss_r_at_mapped_clean_bss( unsigned int, qurt_thread_t );
void dlpager_handler_tlb_miss_w_at_mapped_clean_bss( unsigned int, qurt_thread_t );
void dlpager_handler_evict_page_at_mapped_clean_bss( unsigned int, qurt_thread_t );

   
