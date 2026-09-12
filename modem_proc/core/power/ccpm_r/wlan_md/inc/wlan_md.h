/*
* Copyright (c) 2017 Qualcomm Technologies, Inc.
* All Rights Reserved.
* Qualcomm Technologies, Inc. Confidential and Proprietary.
* 
*   @file  wlan_minidump.h
*   @brief WLAN minidump interface definition
*/


#ifndef WLAN_MINIDUMP_H_
#define WLAN_MINIDUMP_H_

#include "comdef.h"
//#include <sys.h>
#include <qurt.h>
#include "qurt_qdi_constants.h"
#include "qurt_rmutex.h"
#include <err.h>
#include <stringl/stringl.h>
#include "smem.h"
#include "dog.h"
#include "rfs_api.h"
#include "rfs_utils.h"
#include "rfs_errno.h"
#include <msmhwiobase.h>

#define WLAN_MD_QDI_DRV_NAME "/dev/wlanmd"

#define WLAN_MD_REGISTER_REGION	(QDI_PRIVATE+0)
#define WLAN_MD_CONFIG			(QDI_PRIVATE+1)

/** QDI object for minidump resources
 */
qurt_qdi_obj_t wlan_md_qdi_opener;

typedef struct wlan_md_ctx_t {
    qurt_qdi_obj_t qdiobj;
    qurt_mutex_t mtx;
    uint8 crypto_init_done;
	uint8 encrypt_enable;
}wlan_md_ctx_t;

#define WLAN_MD_VERSION			0x00000011 
#define GET_DUMP_BUF_LEN 20
#define WLAN_MD_NUM_SMEM_ENTRIES    10

#define WLAN_MD_VERSION_MSB                     0x0
#define WLAN_MD_VERSION_LSB                     0x1
#define WLAN_MD_VERSION_SIZE                    2

#define WLAN_SW_AES_IV_SIZE     16
#define WLAN_SW_AES_MAC_SIZE     32
#define WLAN_RSA_KEY_LEN         256 //2048bits

#define WLAN_SW_AES_KEY_SIZE                          32
#define WLAN_SW_HMAC_KEY_SIZE                         32
#define WLAN_SW_AES_PLUS_HMAC_KEY_PLUS_VERSION_SIZE   (WLAN_SW_AES_KEY_SIZE + WLAN_SW_HMAC_KEY_SIZE + WLAN_MD_VERSION_SIZE)

#define WLAN_MD_SSR_WCSS              0x01 

#define WLAN_MD_NUM_SMEM_ENTRIES 10
#define WLAN_MD_NUM_ENC_ENTRIES 0x3

#define MPSS_PERPH_REG_BASE                     (MODEM_TOP_BASE      + 0x001a8000)
#define WLAN_MD_MSS_RELAY_MSG_SHADOW6           (MPSS_PERPH_REG_BASE      + 0x00000418)

#define  WLAN_SS_MD_ON  0x01
#define  WLAN_SS_MD_MASK (1<<WLAN_SS_MD_ON)

#define WLAN_MD_DEFAULT_EFS_MASK 0x0
#define WLAN_SS_MD_IMPL_DEF 0xFFFFFFFF

#define WLAN_MD_EFS_FILE \
 "/nv/item_files/wlan/minidump/minidump_mask"

#define  WLAN_MD_PHY_IMEM_POOL	"WLAN_MD_PHY_IMEM_POOL"  /*Ask Sevugan*/
#define  WLAN_MD_PHY_IMEM_POOL_SIZE	0x00001000

#define WLAN_MD_TOTAL_SMEM_SIZE                (10*1024)
#define WLAN_MD_REGION_NAME_LENGTH             (16)

#define WLAN_MD_REVISION                       (1)    // 1st revision of SMEM



#define struct_member_offset(type, m)     (&((( type *)0)->m))

#ifndef SBL_MINIDUMP_MODE_BIT_MASK
#define SBL_MINIDUMP_MODE_BIT_MASK        (0x00000100)
#endif

typedef struct
{
  char      region_name[WLAN_MD_REGION_NAME_LENGTH];
  uint64    region_va;
  uint64    region_pa;
  uint64    region_size;  
} wlan_md_blob_entry;


extern void wlan_md_update_smem_entry(uint32 paddr , char* name, uint32 size, uint32 entry_index);
extern int wlan_md_register_region(int client_handle, uint32 *buffer);
extern void wlan_md_config_callback(boolean);

extern void wlan_md_err_fatal_cb(int client_handle, uint32 thread_info, uint32 n_tcb, uint32 user_data);
extern void wlan_md_register_callback();
extern void wlan_md_init(void);

/* START Copied from ss_minidump_common.h */
#define WLAN_MD_SMEM_PRIVATE_ID		(618)
#define WLAN_MD_SMEM_GLOBAL_ID		(602)

#define WLAN_MD_REGION_VALID                   ('V' << 24 | 'A' << 16 | 'L' << 8 | 'I' << 0)
//#define WLAN_MD_REGION_INVALID                 ('I' << 24 | 'N' << 16 | 'V' << 8 | 'A' << 0)
//#define WLAN_MD_REGION_INIT                    ('I' << 24 | 'N' << 16 | 'I' << 8 | 'T' << 0)
//#define WLAN_MD_REGION_NOINIT                  0

#define WLAN_MD_SS_ENCR_REQ                    (0 << 24 | 'Y' << 16 | 'E' << 8 | 'S' << 0)
#define WLAN_MD_SS_ENCR_NOTREQ                 (0 << 24 | 0 << 16 | 'N' << 8 | 'R' << 0)
#define WLAN_MD_SS_ENCR_DONE                   ('D' << 24 | 'O' << 16 | 'N' << 8 | 'E' << 0)
#define WLAN_MD_SS_ENCR_NOT_DONE               ('S' << 24 | 'T' << 16 | 'R' << 8 | 'T' << 0)
#define WLAN_MD_SS_ENABLED                     ('E' << 24 | 'N' << 16 | 'B' << 8 | 'L' << 0)
#define WLAN_MD_SS_DISABLED                    ('D' << 24 | 'S' << 16 | 'B' << 8 | 'L' << 0)
#define WLAN_MD_SS_IMPL_DEFINE		           0xFFFFFFFF
#define WLAN_MD_SS_IMPL_DEFINE_NEW             ('I' << 24 | 'M' << 16 | 'P' << 8 | 'L' << 0)


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
}wlan_md_subsys_type;

/*
Minidump subsystem blob structure.
*/
typedef struct
{ 
  uint8    region_name[WLAN_MD_REGION_NAME_LENGTH]; /* name of region */ 
  uint32   Seq_num;
  /* To dump or not could be 0, INIT, VALI, INVA*/  
  uint32   md_valid;       
  uint64   region_base_address;
  uint64   region_size;
}wlan_md_ss_region;

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
}wlan_md_ss_toc_type;


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
  wlan_md_ss_toc_type  md_ss_toc[MD_SS_MAX];
}wlan_md_global_toc_type;


#define WLAN_MD_LEGACY_SMEM_SIZE   sizeof(wlan_md_global_toc_type)

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
}wlan_md_ss_pvt_toc_type;

#define WLAN_MD_NON_LEGACY_SMEM_SIZE   sizeof(wlan_md_ss_pvt_toc_type)
/* END Copied from ss_minidump_common.h */

#endif /* WLAN_MINIDUMP_H_ */
