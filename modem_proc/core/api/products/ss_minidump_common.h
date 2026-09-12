#ifndef _SS_MINIDUMP_COMMON_H
#define _SS_MINIDUMP_COMMON_H

/*=============================================================================
                        MINIDUMP driver source code

GENERAL DESCRIPTION
  This header file contains the minidump structures and subsystem minidump specific APIs.

    Copyright 2017  - 2018 by QUALCOMM Technologies, Inc.  All Rights Reserved.
=============================================================================*/

/*=============================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who          what, where, why
--------   ---          --------------------------------------------------

16/12/17     MarthaM       Initial revision for sdm670

===========================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#define MD_CHECK_SUBSYSTEM_TYPE(xx)      (((xx>=MD_SS_HLOS)&&(xx<MD_SS_MAX))?TRUE:FALSE)

#define MD_REVISION                		 (1)

#define MD_SS_TOC_MAGIC					 (0xDEEDDEED)
#define MD_REGION_NAME_LENGTH            (16)
#define MD_NUM_ENCR_ART_REGIONS          (3)

#define MD_NONLEGACY_SMEM_MPSS_SIZE       4096 
#define MD_NONLEGACY_SMEM_ADSP_SIZE       4096 
#define MD_NONLEGACY_SMEM_CDSP_SIZE       4096 
#define MINIDUMP_SMEM_ID                 (602)

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

/*
Enum type for various subsystems participating in the minidump.
*/
typedef  enum
{
	MD_SS_HLOS 		= 0,
	MD_SS_QSEE		= 1,
	MD_SS_QHEE		= 2,
	MD_SS_MPSS		= 3,
	MD_SS_CNSS 		= 4,
	MD_SS_LPASS		= 5,
	MD_SS_SLPI  	= 6,
	MD_SS_CSS   	= 7,
	MD_SS_SP    	= 8,
	MD_SS_MAX   	= 9,
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
   uint64 md_ss_smem_regions_baseptr;
}md_ss_toc_type;


/*
   MD_TOC Minidump global TOC structure. It is allocated by XBL during coldboot.
*/
typedef struct
{
  /*Magic value set by XBL after md init done*/
  uint32 md_toc_init;
  /*minidump revision value*/
  uint32 md_revision;
  /*global minidump enable status*/
  uint32 md_enable_status; 
  /*All Subsystems minidump TOCs*/
  md_ss_toc_type  md_ss_toc[MD_SS_MAX];
}md_global_toc_type;


#define MD_LEGACY_SMEM_SIZE   sizeof(md_global_toc_type)

/*
  MD_SS_PVT_TOC Minidump Private structure allocated by QSEE per SS.
  Private (PVT) structure is not shared via MD_TOC. 
  This structure shall be an agreed memory between TZ/TZ -SDI and other images of interest.
	o	TZ - MBA, MPSS
	o	TZ -ADSP 
	o	TZ -CDSP
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

#endif /* _SS_MINIDUMP_COMMON_H */
