/*!
  @file
  minidump_mem_aperture.h

  @brief
  The is header for memory aperture calculations
*/

/*==============================================================================

  Copyright (c) 2019 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/stability.mpss/2.4/minidump/src/minidump_mem_aperture.h#2 $$ $DateTime: 2019/11/12 06:49:27 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
09/20/19   stv     Initial Revision

==============================================================================*/

#ifndef MINIDUMP_MEM_APERTURE_H
#define MINIDUMP_MEM_APERTURE_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include <comdef.h>
#include "minidump.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/

/* WLAN has seven sections and to be treated as reserved regions (7 as per WLAN MD team) */
#define TS_MD_RESERVE_REGION_MAX       10
/* Number of apertures max could be 1 more than the number of possible blobs and reserve regions */
#define TS_MD_MAX_APERTURE_ENTRIES     (HEAP_ENTRIES+TS_MD_RESERVE_REGION_MAX+1)

 /*----------------------------------------------------------------------------
 * Data types
 * -------------------------------------------------------------------------*/

/* Use below macros if needed to populate compr_algo of ts_md_compression_blob_info_struct_type */
#define MD_ZLIB_COMPR         ('Z' << 24 | 'L' << 16 | 'I' << 8 | 'B' << 0)
#define MD_LZ4_COMPR          ('0' << 24 | 'L' << 16 | 'Z' << 8 | '4' << 0)

  typedef struct
  {
    uint64   address;
    uint64   size;
  }ts_md_buf_info_struct_type;

  typedef struct
  {
    ts_md_buf_info_struct_type   regions[TS_MD_RESERVE_REGION_MAX];
    uint8                        count;
  }ts_md_reserve_regions_info_type;

  typedef struct
  {
    uint32                                 chunk_length;
    uint32                                 compression_method;
    ts_md_buf_info_struct_type             free_buf[TS_MD_MAX_APERTURE_ENTRIES];
  }ts_md_compression_scratch_buf_struct_type;

  typedef struct
  {
    uint64                 actual_address;
    uint64                 compr_address;
    uint32                 actual_size;
    uint32                 compr_size_before_encrypt;
    uint32                 padding_length;
    uint32                 padding_value;
    uint32                 status;
    /* Use MD_ZLIB_COMPR/MD_LZ4_COMPR macros in case to populate compr_algo field below */
    uint32                 compr_algo;
    /* Use MD_SS_ENABLED/MD_SS_DISABLED macros in case to populate valid field below*/
    uint32                 valid;
  }ts_md_compression_blob_info_struct_type;

  typedef struct
  {
    boolean                                   is_compression_enabled;
    ts_md_compression_scratch_buf_struct_type scratch_buf_info;
    ts_md_compression_blob_info_struct_type   blob_info[HEAP_ENTRIES];
  }ts_md_compression_artifact_type;
#endif /* MINIDUMP_MEM_APERTURE_H */
