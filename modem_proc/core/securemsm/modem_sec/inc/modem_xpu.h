#ifndef MODEM_XPU_INT_H
#define MODEM_XPU_INT_H

/**
* @file modem_xpu_int.h
* @brief Modem XPU Interrupt Handler
*
* This file implements the XPU interrupt Handling. All XPU violations are reported
* over DIAG (Subsystem QDSP6) and to a ring buffer referenced by 'modem_sec_xpu_buffer'.
*
*/
/*===========================================================================
   Copyright (c) 2011, 2020 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/core.mpss/10.0/securemsm/modem_sec/inc/modem_xpu.h#4 $
  $DateTime: 2020/04/30 00:07:34 $
  $Author: pwbldsvc $


when       who      what, where, why
--------   ---      ------------------------------------
01/04/2013 rs       Initial version
===========================================================================*/
#include <comdef.h>
#include "HALxpu3.h"

#define TZBSP_VMID_NOACCESS     0
#define TZBSP_VMID_VMID_0       0
#define TZBSP_VMID_RPM          1
#define TZBSP_VMID_TZ           2
#define TZBSP_VMID_AP           3
#define TZBSP_VMID_MSS          4
#define TZBSP_VMID_LPASS        5
#define TZBSP_VMID_CP           6
#define TZBSP_VMID_VIDEO        7
#define TZBSP_VMID_ZAP_SHADER   8
#define TZBSP_VMID_MDSS         9
/* Space for VMID 10 */
#define TZBSP_VMID_SSC          11

/* These values were obtained by the IPA team, and are documented in the
** 'IPAv2 Pipes - Istari SMMU VMID configuration' spreadsheet
** these shouldn't be changed as these are used by IPA to generate SID */
#define TZBSP_VMID_IPA_AP       (0b1100) /* 12 */
#define TZBSP_VMID_IPA_UC_PIPE  (0b1101) /* 13 */
#define TZBSP_VMID_IPA_UC       (0b1110) /* 14*/

#define TZBSP_VMID_WLAN         15
#define TZBSP_VMID_IPA_WIFI     16

#define TZBSP_VMID_NOACCESS_BIT     (1<<TZBSP_VMID_NOACCESS)
#define TZBSP_VMID_VMID_0_BIT       (1<<TZBSP_VMID_VMID_0)
#define TZBSP_VMID_TZ_BIT           (1<<TZBSP_VMID_TZ)
#define TZBSP_VMID_RPM_BIT          (1<<TZBSP_VMID_RPM)
#define TZBSP_VMID_LPASS_BIT        (1<<TZBSP_VMID_LPASS)
#define TZBSP_VMID_MSS_BIT          (1<<TZBSP_VMID_MSS)
#define TZBSP_VMID_AP_BIT           (1<<TZBSP_VMID_AP)
#define TZBSP_VMID_CP_BIT           (1<<TZBSP_VMID_CP)
#define TZBSP_VMID_VIDEO_BIT        (1<<TZBSP_VMID_VIDEO)
#define TZBSP_VMID_MDSS_BIT         (1<<TZBSP_VMID_MDSS)
#define TZBSP_VMID_SSC_BIT          (1<<TZBSP_VMID_SSC)

#define TZBSP_VMID_IPA_AP_BIT       (1 << TZBSP_VMID_IPA_AP)
#define TZBSP_VMID_IPA_UC_PIPE_BIT  (1 << TZBSP_VMID_IPA_UC_PIPE)
#define TZBSP_VMID_IPA_UC_BIT       (1 << TZBSP_VMID_IPA_UC)

#define TZBSP_VMID_WLAN_BIT         (1<<TZBSP_VMID_WLAN)
#define TZBSP_VMID_ZAP_SHADER_BIT   (1<<TZBSP_VMID_ZAP_SHADER)
#define TZBSP_VMID_IPA_WIFI_BIT     (1<<TZBSP_VMID_IPA_WIFI)

/* xPU raw registers size */
#define XPU_ESR_SIZE 5

/* Domain definitions. Start downwards from 31 so it doesn't conflict with VMIDs */
#define TZBSP_DOMAIN_MASK 0xFF000000 /* Reserve 8 bits for domains */
#define TZBSP_DOMAIN_MSA   31
#define TZBSP_DOMAIN_HYP   30
#define TZBSP_DOMAIN_SP    29
#define TZBSP_DOMAIN_TZ    28

#define TZBSP_ALL_VMID              ((~TZBSP_VMID_NOACCESS) & (~TZBSP_DOMAIN_MASK))


/* Domain definitions */
#define TZBSP_DOMAIN_MSA_BIT   1 << TZBSP_DOMAIN_MSA
#define TZBSP_DOMAIN_HYP_BIT   1 << TZBSP_DOMAIN_HYP
#define TZBSP_DOMAIN_SP_BIT    1 << TZBSP_DOMAIN_SP
#define TZBSP_DOMAIN_TZ_BIT    1 << TZBSP_DOMAIN_TZ

#ifndef MIN
  #define  MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
#endif

#ifndef MAX
  #define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#define FIXED_INUSE_PART_MASK  ~0xFFFUL
#define TZ_MSS_MAGIC_COOKIE    0xABCDABC2UL

/* Structure for holding the ModemMSMPUShared Informaiton */
typedef struct modem_ms_mpu_shared_info
{
    /** last 8 bits is used for flags **/
    union s_addr
   {
	uint64 start;
        struct flags{
            uint8 fixed_bit:1;
            uint8 in_use:1;
            uint8 part_bit:1;
        }flags;
    }s_addr;
    /** last 8 bits is used for flags **/
    union e_addr
    {
	uint64 end;
        uint8 res_id;
    }e_addr;
	uint32 rvmid;
	uint32 wvmid;
} modem_ms_mpu_shared_info;

/* Structure for holding the Modem Partition Address */
typedef struct modem_ptrn_addr
{
    uint64 start_addr;
    uint64 end_addr;
} modem_ptrn_addr;

/* Structure for holding the TZMSSMSMMPU configuration details*/
typedef struct mdm_ms_mpu_cfg_detail
{
    uint32 bTzLockVal;
    uint32 bMssLockVal;
    uint32 version_magic_cookie;
    uint32 xpu_enable_status;
    uint32 ms_mpu_info_size;
    uint32 mdm_prtn_addr_size;
    uint64 mpu_shared_info_ptr;
    uint64 mdm_prtn_addr_ptr;
}mdm_ms_mpu_cfg_detail;

typedef struct modem_xpu_prtn_addr
{
	uint64 start;
	uint64 end;
}modem_xpu_prtn_addr;

typedef enum
{
  REGION_EQUAL,
  REGION_NO_OVERLAP,
  REGION_ADJACENT,
  REGION_OVERLAP_INNER,
  REGION_OVERLAP_INNER_ADJACENT, 
  REGION_OVERLAP_OUTER,
  REGION_OVERLAP_OUTER_ADJACENT,
  REGION_OVERLAP,
} overlap_t;


/* Modem EFS Partition */
typedef enum resource_id_s
{
 MBA_XPU_INVALID_PARTITION = -1,
 MBA_XPU_PARTITION_PMI=0,
 MBA_XPU_PARTITION_MODEM_EFS = 1,
 MBA_XPU_PARTITION_MODEM_UNCACHED_HEAP = 2,
 MBA_XPU_PARTITION_MODEM_RFSA = 3,
 MBA_XPU_PARTITION_RECLAIM_MBA = 4,
 MBA_XPU_PARTITION_DEBUG = 5,
 MBA_XPU_PARTITION_MAX_USAGE, //Please add new partitions above this
}resource_id_type;

typedef struct
{
    uint32 xpu_addr;
    uint32 err_count;
    uint32 uRawESR;
    uint32 uPALower32;
    uint32 uPAUpper32;
    uint32 uRawESYNR[XPU_ESR_SIZE];
}ACSilentErrorLog;

/**
 * @brief      Configure XPU Violation Interrupts and start IST
 *             
 *
 * @param[in]  none
 * @param[out] none
 *
 * @return     none
 */
void modem_sec_config_xpu_error_reporting (void);

/**
 * @brief      Initialize QURT Interrupt Service Thread 
 *             
 *
 * @param[in]  none
 * @param[out] none
 *
 * @return     none
 */
void modem_sec_xpu_IST_init (void);

/**
 * @brief      Thread that registers XPU Violation interrupts 
 *             
 *
 * @param[in]  none
 * @param[out] none
 *
 * @return     none
 */
void modem_sec_xpu_IST (void *arg);

/**
 * @brief      Interrupt Service Routine for XPU Violation 
 *             
 *
 * @param[in]  none
 * @param[out] none
 *
 * @return     none
 */
void modem_sec_xpu_isr(void);

int mba_xpu_lock_region(uint64 start, uint64 end, resource_id_type resource_id);
int mba_xpu_unlock_region(resource_id_type resource_id);

int modem_unlock_xpu_all(void);
int mba_xpu_scribble_unlock_region(resource_id_type resource_id);

#endif
