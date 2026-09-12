#ifndef ACCESS_CONTROL_TZ_H
#define ACCESS_CONTROL_TZ_H

/*===========================================================================
Copyright (c) 2010-2013 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/securemsm/accesscontrol/api/AccessControlMba.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/14/10   tk      First version.
============================================================================*/

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include <comdef.h>
#include "ACHALxpu.h"
#include "ACCommon.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define MEMORY_OWNERSHIP_TABLE_BUFFER_SIZE               0x180000
#define MEMORY_OWNERSHIP_TABLE_BUFFER_MAX_IDX            0x180000
#define PARITY_BUFFER_SIZE                             0x30000
#define PARITY_BUFFER_MAX_IDX                          0x30000 
#define MAGIC_COOKIE                                   0x41473ACD

/* Flag definitions up until 8996(xPU v2) */

#define TZBSP_XPU_ENABLE          0x01 /**< Enables an XPU. */
#define TZBSP_XPU_NO_INTERRUPTS   0x02 /**< Disables XPU error interrupts. */
#define TZBSP_XPU_MODEM_PROT      0x04 /**< Marks modem only items. */
#define TZBSP_XPU_SEC             0x08 /**< Secure partition */
#define TZBSP_XPU_NON_SEC         0x10 /**< Non Secure partition */
#define TZBSP_XPU_UMR_MSACLROE    (1 << 5) /**< MSA unmapped read-access  */
#define TZBSP_XPU_UMR_MSACLRWE    (1 << 6) /**< MSA unmapped read/write-access  */
#define TZBSP_XPU_CR_MSAE         (1 << 7) /**< Check MSA bit for non-secure registers */
#define TZBSP_RWGE                0x0100 /**< Read/write global enable. */
#define TZBSP_ROGE                0x0200 /**< Read-only global enable. */
#define TZBSP_RWE                 0x0400 /**< Read/write VMID enable. */
#define TZBSP_ROE                 0x0800 /**< Read-only VMID enable. */
#define TZBSP_VMIDCLROE           0x1000 /**< VMID based Read-Only enable for Secure/MSA resource. */
#define TZBSP_VMIDCLRWE           0x2000 /**< VMID based Read-Write enable for Secure/MSA resource. */
#define TZBSP_MSACLROE            0x4000 /**< MSA Read-only enable for secure partitions. */
#define TZBSP_MSACLRWE            0x8000 /**< MSA Read-Write enable for secure partitions. */

/* Flag definitions for 8998 onwards (xPU v3) */
#define TZBSP_XPU_ENABLE          0x01 /**< Enables an XPU. */
#define TZBSP_XPU_NO_INTERRUPTS   0x02 /**< Disables XPU error interrupts. */
#define TZBSP_XPU_MSA_OWNER       0x04 /**< Marks modem only items. */
#define TZBSP_XPU_TZ_OWNER        0x08 /**< Secure partition */
#define TZBSP_XPU_HYP_OWNER       0x10 /**< Non Secure partition */
#define TZBSP_XPU_SP_OWNER        0x20 /**< Non Secure partition */


#define TZBSP_XPU_READ_ACCESS     1 /**< Read only access */
#define TZBSP_XPU_RW_ACCESS       2 /**< Read/Write access */
#define TZBSP_XPU_WRITE_ACCESS    3 /**< Write only access */

#define TZBSP_XPU_DYN_AREA_CERT   1 /**< Dynamic area ID for PIL certificate. */

/* EBI channel interleaving related constants. */
#define TZBSP_EBI_CH_NONE             0x0
#define TZBSP_EBI_CH_0                0x1
#define TZBSP_EBI_CH_1                0x2
#define TZBSP_EBI_CH_BOTH             0x3 /* Interleaved. */

#define TZBSP_BLIST_DEVICE_MEMORY       0       /* Non-DDR memory, set at boot */
#define TZBSP_BLIST_DEVICE_MEMORY1      1       /* Device Memory, set at boot. */
#define TZBSP_BLIST_UNUSED              2       /* Unused */
#define TZBSP_BLIST_SECCHANNEL_MSS      3       /* MSS Secure Channel */
#define TZBSP_BLIST_SECCHANNEL_LPASS    4       /* LPASS Secure Channel */
#define TZBSP_BLIST_TZ_STATIC_MEMORY    5       /* Static Memory . */
#define TZBSP_BLIST_ALLOC_0             6       /* Dynamically allocated. */
#define TZBSP_BLIST_ALLOC_1             7       /* Dynamically allocated. */
#define TZBSP_BLIST_ALLOC_2             8       /* Dynamically allocated. */
#define TZBSP_BLIST_ALLOC_3             9       /* Dynamically allocated. */
#define TZBSP_BLIST_ALLOC_4             10      /* Dynamically allocated. */
#define TZBSP_BLIST_ALLOC_5             11      /* Dynamically allocated. */
#define TZBSP_BLIST_FIXED_SEC_DDR       12      /* Fixed secure DDR, static */
#define TZBSP_BLIST_SHARED_IMEM         13      /* Shared imem, static. */
#define TZBSP_BLIST_TZ_DDR              14      /* Statically allocated. */
#define TZBSP_BLIST_PIL_ALLOC_0			15		/* Dynamically allocated. */
#define TZBSP_BLIST_PIL_ALLOC_1			16		/* Dynamically allocated. */
#define TZBSP_BLIST_PIL_ALLOC_2			17		/* Dynamically allocated. */
#define TZBSP_BLIST_PIL_ALLOC_3			18		/* Dynamically allocated. */	
#define TZBSP_BLIST_PIL_ALLOC_4         19      /* Dynamically allocated. */
#define TZBSP_BLIST_PIL_ALLOC_5         20      /* Dynamically allocated. */


/* Just adding temporarily so compilation works: REMOVEME*/
#define TZBSP_BLIST_LPASS   TZBSP_BLIST_PIL_ALLOC_0
#define TZBSP_BLIST_VIDEO   TZBSP_BLIST_PIL_ALLOC_1
#define TZBSP_BLIST_WLAN    TZBSP_BLIST_PIL_ALLOC_2
#define TZBSP_BLIST_VPU     TZBSP_BLIST_PIL_ALLOC_3
#define TZBSP_BLIST_GPU     TZBSP_BLIST_PIL_ALLOC_4
#define TZBSP_BLIST_MSS     TZBSP_BLIST_PIL_ALLOC_5
#define TZBSP_BLIST_LPASS_DY_HEAP TZBSP_BLIST_ALLOC_5

#define TZBSP_UNMAPPED_PARTITION -1

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/
/**
 * Resource group configuration for MPUs. All MPUs are multi-VMID.
 */
typedef struct tzbsp_mpu_rg_s
{
  uint16 index;      /* Index of the MPU resource group. */
  /** XPU status bits, currently \c TZBSP_XPU_ENABLE, \c
   * TZBSP_XPU_NO_INTERRUPTS, \c TZBSP_XPU_SEC, \c TZBSP_XPU_NON_SEC and
   * \c TZBSP_XPU_MODEM_PROT are supported.
   */
  uint16 flags;
  uint32 read_vmid;  /* VMIDs able to read this partition. */
  uint32 write_vmid; /* VMIDs able to write this partition. */
  uint64 start;  /* Start of the partition. */
  uint64 end;    /* End of the partition, not included in the range */
} tzbsp_mpu_rg_t;

/**
 * Resource group configuration for APUs/RPUs. Depending on APU/RPU, a resource
 * group is single VMID or multi VMID.
 */
typedef struct tzbsp_rpu_rg_s
{
  /** Index of the APU/RPU resource group. */
  uint16 index;
  /**
   * Valid for single VMID resource groups only. Bits \c TZBSP_RWGE and \c
   * TZBSP_ROGE allow defining read/write global enable and read-only global
   * enable. Bit \c TZBSP_RWE enables the read/write access VMID(s). Bit \c
   * TZBSP_ROE enables the read access VMID(s).
   */
  uint16 flags;
  /**
   * For multi VMID resource groups contains a bitmap of VMIDs that can read
   * the resource. For single VMID resource groups contains a VMID value that
   * can read the resource.
   */
  uint32 read_vmid;
  /**
   * For multi VMID resource groups contains a bitmap of VMIDs that can
   * read/write the resource. For single VMID resource groups contains a VMID
   * value that can read/write the resource.
   */
  uint32 write_vmid;
} tzbsp_rpu_rg_t;

/**
 * Root level structure for XPU configuration.
 */
typedef struct
{
  /** Physical base address of the XPU. */
  uint64 phys_addr;
  /** Index of the XPU, @see \c HAL_xpu_XPUType. */
  uint16 id;
  /** XPU status bits, currently \c TZBSP_XPU_ENABLE, \c
   * TZBSP_XPU_NO_INTERRUPTS and \c TZBSP_XPU_MODEM_PROT are supported.
   */
  uint16 flags;
  /** Read access VMIDs for unmapped area */
  uint32 unmapped_rvmid;
  /** Write access VMIDs for unmapped area */
  uint32 unmapped_wvmid;
  /** Superusers */
  uint32 superuser_vmid;
  /** Number of configured resource groups. */
  uint16 nrg;
  union
  {
    const void* any;           /* A dummy to keep compiler happy. */
    const tzbsp_rpu_rg_t* rpu; /* Can be NULL. */
    const tzbsp_mpu_rg_t* mpu; /* Can be NULL. */
  } rg;
} tzbsp_xpu_cfg_t;

/**
 * Root level structure for QRIB configuration.
 */
typedef struct
{
  uint64 xpu2_qrib_init_addr; /* *_QRIB_XPU2_INIT register address */
  uint64 xpu2_qrib_acr_addr;  /* *_QRIB_XPU2_ACR register address */
  uint64 xpu2_qrib_vmid_en_init_addr; /* *_QRIB_VMIDEN_INIT register address */
  uint8 nsen_init; /* Value of *_XPU2_NSEN_INIT field in *_QRIB_XPU2_INIT register */ 
  uint8 en_tz;  /* Value of *_XPU2_EN_TZ field in *_QRIB_XPU2_INIT register */
  uint32 xpu_acr_vmid; /* Value of *_XPU2_ACR field in *_QRIB_XPU2_ACR register */
  uint8 vmiden_init;   /* Value of *_XPU2_VMIDEN_INIT field in *_QRIB_VMIDEN_INIT register */
  uint32 vmiden_init_en_hv;   /* Value of *_XPU2_VMIDEN_INIT_EN_HV field in *_QRIB_VMIDEN_INIT register */ 
}tzbsp_qrib_cfg_t;

typedef enum
{
  XPU_DISABLE_NONE,
  XPU_DISABLE_NON_MSS,
  XPU_DISABLE_ALL
} xpu_level_t;

typedef struct
{
   uint32 usecase_id;
   int32 rg_idx;   
} xpu_partition_mapping;

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/
/**
@brief ACInit - Initializes AC static configs 
@return AC_SUCCESS on success, failure values on Failure
*/
int ACMbaInit(void);


#if 0
/**
 * Locks an EBI1 memory region and enables it as a secure area in the
 * blacklist.
 *
 * @param [in] id  The area to be locked. Must be one of the blacklist
 *                 item IDs. Currently \c TZBSP_BLIST_MSS and
 *                 \c TZBSP_BLIST_LPASS are supported.
 * @param [in] start  Start address of the region to be locked.
 * @param [in] end    End address of the region to be locked.
 *
 * @return Zero on success, otherwise an error code.
 */
int tzbsp_xpu_lock_area(uint32 id, uint64 start, uint64 end,
                               uint32 rvmid, uint32 wvmid);

/**
 * Unlocks an EBI1 memory region and disables it as a secure area in the
 * blacklist.
 *
 * @param [in] id  The area to be unlocked. Must be one of the blacklist
 *                 item IDs. Currently \c TZBSP_BLIST_MSS and
 *                 \c TZBSP_BLIST_LPASS are supported.
 *
 * @return Zero on success, otherwise an error code.
 */
int tzbsp_xpu_unlock_area(uint32 id);

/**
 * Dynamically reconfigures an RPU/APU resource group VMID assignment. If the
 * configuration is for secure partition, then setting \c TZBSP_ROGE in the RPU
 * flags will enable \c VMIDCLROE and setting \c TZBSP_RWGE in the RPU flags
 * will enable \c VMIDCLRWE.
 *
 * @param [in] xpu_id   HAL ID of the RPU/APU.
 * @param [in] rpu      The RPU to reconfigure
 * @param [in] sec      If \c TRUE, then partition is configured as secure
 *                      partition. If \c FALSE, then partition is configured
 *                      as VMID based partition.
 *
 * @return Zero on success, otherwise an error code.
 */
int tzbsp_rpu_reconfigure(uint32 xpu_id, const tzbsp_rpu_rg_t *rpu,
                          boolean sec);

/**
 * Dynamically reconfigures an RPU/APU resource group VMID and domain assignment. 
 * This api is an extension to tzbsp_rpu_reconfigure() api. This API can be used
 * to assign the domain TZBSP_XPU_MODEM_PROT  or  TZBSP_XPU_SEC or TZBSP_XPU_NON_SEC
 * to a particular resource group or partition. 
 * Note : Domain cannot be changed by TZ if it is already TZBSP_XPU_MODEM_PROT.
 *
 * @param [in] xpu_id   HAL ID of the RPU/APU.
 * @param [in] rpu      The RPU to reconfigure
 * @param [in] domain   TZBSP_XPU_MODEM_PROT  or  TZBSP_XPU_SEC or TZBSP_XPU_NON_SEC
 *
 * @return Zero on success, otherwise an error code.
 */
int tzbsp_rpu_reconfigure_ext(uint32 xpu_id, const tzbsp_rpu_rg_t *rpu,
                              uint32 domain);


/**
 * Locks a dynamic XPU partition.
 *
 * @param [in] area_id  Dynamic area id, one of TZBSP_XPU_DYN_AREA_*.
 * @param [in] start    Start address of the dynamic area. Alignment is
 *                      dependent on the DDR memory MPU configuration at HW
 *                      level.
 * @param [in] end      End address of the dynamic area, exclusive of the
 *                      memory range.  Alignment is dependent on the DDR memory
 *                      MPU configuration at HW level.
 *
 * @return \c E_SUCCESS if successful, error code otherwise.
 */
int tzbsp_mpu_lock_dyn_area(uint32 area_id, uint64 start, uint64 end);

/**
 * Unlocks a dynamic XPU partition.
 *
 * @param [in] area_id  Dynamic area id, one of TZBSP_XPU_DYN_AREA_*.
 *
 * @return \c E_SUCCESS if successful, error code otherwise.
 */
int tzbsp_mpu_unlock_dyn_area(uint32 area_id);

/**
 * API to configure a MPU partition 
 *
 * @param [in] xpu_id   HAL ID of the MPU.
 * @param [in] mpu      The MPU to reconfigure
 *
 * @return  Zero on success.
 *          Negative error code otherwise.
 */
int tzbsp_xpu_mpu_reconfigure(uint32 xpu_id, const tzbsp_mpu_rg_t *mpu); 

#endif
#endif /* ACCESS_CONTROL_TZ_H */
