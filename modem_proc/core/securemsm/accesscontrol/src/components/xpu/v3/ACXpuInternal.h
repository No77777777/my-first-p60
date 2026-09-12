#ifndef AC_XPU_INTERNAL_H
#define AC_XPU_INTERNAL_H

/*===========================================================================
Copyright (c) 2010 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/securemsm/accesscontrol/src/components/xpu/v3/ACXpuInternal.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/22/10   tk      First version.
============================================================================*/

#include <stdbool.h>
#if 0
#include "tzbsp_sys.h"
#endif
#include "HALxpu3.h"

#ifndef _UINT8_DEFINED
typedef  unsigned char      uint8;       /* Unsigned 8  bit value */
#define _UINT8_DEFINED
#endif

#define TZBSP_QAD_APPS_BMSK (1 << 0)
#define TZBSP_QAD_MSA_BMSK (1 << 1)
#define TZBSP_QAD_SP_BMSK  (1 << 2)

/*
 * Structure to map a bit position in XPU error interrupt status register
 * to corresponding HAL XPU index
 */
typedef struct
{
  /* Bit position in status register */
  uint8 bit_pos;
  /* Corresponding HAL XPU index */
  uint8 xpu;
} xpu_err_pos_to_hal_map_t;

/*
 * Structure to keep xPU Interrupt enable/status register info.
 */
typedef struct
{
  uint64 xpu_intr_reg_addr;
  uint32 xpu_intr_reg_mask; 
} xpu_intr_reg_dtls_t;

/*
 * Structure to keep BIMC xPU base address & HAL xPU index
 * for DDR xPU dump feature.
 */
typedef struct
{
  /* HAL XPU index */
  HAL_xpu2_XPU2Type xpu;

  /* BIMC xPU based address */
  uint64 bimc_xpu_base_addr;
} bimc_xpu_addr_map_t;

#endif /* AC_XPU_INTERNAL_H */
