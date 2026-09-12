/*!
  @file
  minidump.h

  @brief
  The interface to the Qualcomm Sherlock Holmes feature implementation file.
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

$Header: //components/rel/stability.mpss/2.4/minidump/src/minidump.h#6 $$ $DateTime: 2020/02/18 08:34:18 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
02/18/20   stv     Align the enc artifacts by 8 byte boundary
12/15/19   stv    Full modem dump through minidump fw
11/05/19   stv    Modem aperture calculations and add large blobs to init 
06/25/19   stv    Selective heap variable enablement changes
04/15/19   stv    Check minidump status in pvt toc 
03/04/19   stv    Change AES key size to avoid truncation & Remove Data var from XML
             
==============================================================================*/

#ifndef MINIDUMP_H
#define MINIDUMP_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include <comdef.h>
#include <fs_public.h>
#include <err.h>
#include "memheap.h"
#include <qurt.h>
#include <stringl/stringl.h>
#include "smem.h" 
#include "dog.h"


/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define STABILITY_MINIDUMP_EFS_FILE \
 "/nv/item_files/modem/stability/minidump/minidump_mask"

#define HEAP_ENTRIES                      0x18

/* Increased the var entries by 200 from 2300 in version 0x12 */
#define TS_MD_MAX_DUMP_VAR_ENTRIES        0x9C4

#define MINI_DUMP_MEMORY_SIZE             0x400000
#define MINIDUMP_VERSION                  0x00000012
#define TS_MD_GET_DUMP_BUF_LEN            0x14

#define TS_MD_ATTR_REGULAR_VARIABLE       0x0
#define TS_MD_ATTR_CS_NEEDED_VARIABLE     0x1
#define TS_MD_ATTR_LARGE_VARIABLE         0x3

#define STABILITY_DEFAULT_MINIDUMP_MASK   0x0

#define MODEM_SW_AES_IV_SIZE              16
#define MODEM_SW_AES_MAC_SIZE             32
#define MODEM_RSA_KEY_LEN                 256

#define NUM_MD_ENC_ENTRIES                0x3
#define NUM_MD_SMEM_ENTRIES               (HEAP_ENTRIES)

#define  TS_MD_DISABLE                    0x0
#define  TS_MD_ENABLE                     0x1
#define  TS_MD_NOT_INIT                   0x2 /* If minidump status is fetched before minidump_init*/

#define TS_MD_MAX_MPSS_REGIONS            HEAP_ENTRIES
#define TS_MD_EFS_ENBL                    0x4
#define TS_MD_EFS_DSBL                    0x2

#define TS_MD_ALGINMENT_FACTOR            0x8

/* START Copied from ss_minidump_common.h */
#define MD_REGION_NAME_LENGTH             (16)
#define MINIDUMP_SMEM_ID                  (602)

#define MD_REGION_VALID                   ('V' << 24 | 'A' << 16 | 'L' << 8 | 'I' << 0)
#define MD_REGION_INVALID                 ('I' << 24 | 'N' << 16 | 'V' << 8 | 'A' << 0)
#define MD_REGION_INIT                    ('I' << 24 | 'N' << 16 | 'I' << 8 | 'T' << 0)
#define MD_REGION_NOINIT                  0

#define MD_SS_ENCR_REQ                    (0 << 24 | 'Y' << 16 | 'E' << 8 | 'S' << 0)
#define MD_SS_ENCR_NOTREQ                 (0 << 24 | 0 << 16 | 'N' << 8 | 'R' << 0)
#define MD_SS_ENCR_DONE                   ('D' << 24 | 'O' << 16 | 'N' << 8 | 'E' << 0)
#define MD_SS_ENCR_NOT_DONE               ('S' << 24 | 'T' << 16 | 'R' << 8 | 'T' << 0)
#define MD_SS_ENABLED                     ('E' << 24 | 'N' << 16 | 'B' << 8 | 'L' << 0)
#define MD_SS_DISABLED                    ('D' << 24 | 'S' << 16 | 'B' << 8 | 'L' << 0)

#define MD_SS_IMPL_DEFINE                 (0XFFFFFFFF)
#define MD_LEGACY_SMEM_SIZE               sizeof(md_global_toc_type)

/*
Enum type for various subsystems participating in the minidump.
*/
typedef  enum
{
  MD_SS_HLOS    = 0,
  MD_SS_QSEE    = 1,
  MD_SS_QHEE    = 2,
  MD_SS_MPSS    = 3,
  MD_SS_CNSS    = 4,
  MD_SS_LPASS   = 5,
  MD_SS_SLPI    = 6,
  MD_SS_CSS     = 7,
  MD_SS_SP      = 8,
  MD_SS_MAX     = 9,
  MD_SS_UNKNOWN = 0x7FFFFFFF
}md_subsys_type;

/*
Minidump subsystem blob structure.
*/
typedef struct
{
  uint8    region_name[MD_REGION_NAME_LENGTH]; /* name of region */ 
  uint32   Seq_num;
  /* To dump or not could be 0, INIT, VALI, INVA*/
  uint32   md_valid;
  uint64   region_base_address;
  uint64   region_size;
}md_ss_region;

/*
  MD_SS_TOC Minidump Subsystem TOC structure. This structure is allocated by XBL during coldboot.
*/
typedef struct
{
  uint32  md_ss_toc_init;
  /* Minidump enable status of the Subsystem */
  uint32  md_ss_enable_status;
  /*Minidump Subsystem blobs encryption status.
  Updated by the RoT while SDI pass#1 for system crash, or during subsystem SSR*/
  uint32  encryption_status; 
  /* Indicate XBL in pass #2 to encrypt the content */
  uint32  encryption_required; 
  /*Total number of subsystem minidump blobs*/   
  uint32  ss_region_count;
  /*Pointer to an array of structures or pointer to the linked list of structures. Implementation defined*/
  uint64  md_ss_smem_regions_baseptr;
}md_ss_toc_type;

/*
   MD_TOC Minidump global TOC structure. It is allocated by XBL during coldboot.
*/
typedef struct
{
  /*Magic value set by XBL after md init done*/
  uint32          md_toc_init;
  /*minidump revision value*/
  uint32          md_revision;
  /*global minidump enable status*/
  uint32          md_enable_status; 
  /*All Subsystems minidump TOCs*/
  md_ss_toc_type  md_ss_toc[MD_SS_MAX];
}md_global_toc_type;

/*
  MD_SS_PVT_TOC Minidump Private structure allocated by QSEE per SS.
  Private (PVT) structure is not shared via MD_TOC.
  This structure shall be an agreed memory between TZ/TZ -SDI and other images of interest
  o TZ - MBA, MPSS
  o TZ - ADSP
  o TZ - CDSP
*/
typedef struct
{ 
   uint32  md_ss_enable_status;
   /* updated by RoT to indicate if its mandatory for SS to encrypt minidumps */
   uint32  encryption_required; 
    /* encryption of the blobs complete */
   uint32  encryption_status;
   /* scribble of remaining memory under the SS is complete  */
   uint32  scribble_status; 
   uint32  ss_region_count;
   /*A pointer to an array of structures or pointer to the linked list of structures. Implementation defined*/
   uint64  md_ss_smem_regions_baseptr; 
}md_ss_pvt_toc;

/* END Copied from ss_minidump_common.h */

typedef struct
{
  md_ss_pvt_toc  *pvt_ss_toc;
  md_ss_toc_type *public_ss_toc;
  uint32          md_enabled;
  uint32          region_count;
  uint32          is_ns_dev;
  md_ss_region    md_regions[TS_MD_MAX_MPSS_REGIONS];
}ts_md_info_type;

typedef struct 
{
  uint32  address;
  uint32  size:24;
  uint32  attribute:8;
}minidump_var_table_type;

typedef struct 
{
  uint32  p_address;
  uint32  v_address;
  uint32  size;
}ts_md_map_info_type;

typedef struct 
{
  uint32 aperture_address;
  uint32 aperture_size;
}ts_md_mem_aperture_type;

typedef struct
{
  uint32                   dump_mem_location;
  uint32                   dump_mem_size;
  uint32                   dump_tab_offset;
  uint32                   version;
  ts_md_map_info_type      map_tab[HEAP_ENTRIES];
  minidump_var_table_type  minidump_var_tab[TS_MD_MAX_DUMP_VAR_ENTRIES];
  uint32                   magic;
}minidump_info_type;

/* Resets the WDOG circuit so that we have another N milliseconds 
   before the circuit will reset the system */
extern void dog_force_kick(void);

void minidump_init(void);

// This API is exposed to QSH to get the minidump status
uint8 ts_get_minidump_status(void);

// This API is exposed to QSH to get the selective heap var status
boolean ts_get_selective_heap_var_enable_status(void);

// This API is exposed to WLAN to update minidump reserve regions
void ts_md_update_reserve_regions(uint8 num_regions, ts_md_map_info_type *region_ptr);

#endif /* MINIDUMP_H */
