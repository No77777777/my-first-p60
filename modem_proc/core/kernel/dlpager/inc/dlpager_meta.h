/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               DL PAGER TOP LEVEL HEADER FILE

GENERAL DESCRIPTION

  Copyright (c) 2010 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

            EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/09/14   rc,rr,cp     Header file metadata associated APIs
===========================================================================*/
#pragma once
#include <comdef.h>
#include <dlpager_types.h>

extern unsigned int __attribute__ ((weak)) __swapped_segments_start__;
extern unsigned int __attribute__ ((weak)) __swapped_segments_text_start__;
extern unsigned int __attribute__ ((weak)) __swapped_segments_text_end__;
extern unsigned int __attribute__ ((weak)) __swapped_segments_rodata_start__;
extern unsigned int __attribute__ ((weak)) __swapped_segments_rodata_end__;
extern unsigned int __attribute__ ((weak)) __swapped_segments_rw_start__;
extern unsigned int __attribute__ ((weak)) __swapped_segments_rwdata_start__;
extern unsigned int __attribute__ ((weak)) __swapped_segments_rwdata_end__;
extern unsigned int __attribute__ ((weak)) __swapped_segments_bss_start__;
extern unsigned int __attribute__ ((weak)) __swapped_segments_bss_end__;
extern unsigned int __attribute__ ((weak)) __swapped_segments_end__;

boolean dlpager_meta_is_addr_in_text_range(unsigned int va);
boolean dlpager_meta_is_addr_in_rodata_range(unsigned int va);
boolean dlpager_meta_is_addr_in_text_or_rodata_range(unsigned int va);
boolean dlpager_meta_is_addr_in_rwdata_range(unsigned int va);

dlpager_iovec_t dlpager_meta_get_compressed_block_addr (unsigned int uncompressed_page_addr);
void dlpager_meta_set_compressed_block(unsigned int va, dlpager_iovec_t rw_buffer);

char * dlpager_meta_q6zip_dictionary(void);
unsigned int dlpager_meta_init();

