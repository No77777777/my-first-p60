#ifndef __GPITGT_H__
#define __GPITGT_H__

/**
  @file  gpi_tgt.h
  @brief GPI target-spefic header file.

*/
/*
===============================================================================

Edit History

$Header:

when       who     what, where, why
--------   ---     ------------------------------------------------------------
05/30/18   pc      Updated for Talos
04/18/18   ah      File created.

===============================================================================
              Copyright (c) 2018 QUALCOMM Technologies, Inc.
                         All Rights Reserved.
            Qualcomm Technologies Proprietary and Confidential.
===============================================================================
*/

#include "msmhwiobase.h"
#include "HALhwio.h"
#include "gsihwio.h"

#define MAX_NUM_QUP_BLOCKS           2
#define NUM_ACTIVE_QUP_BLOCKS        1
#define MAX_GPII_PER_QUP             13
#define NUM_GPII                     (NUM_ACTIVE_QUP_BLOCKS*MAX_GPII_PER_QUP)

/* GSI HW Instance */
typedef enum
{
   QUPV3_0   = 0,
   QUPV3_SSC = 1,

   QUP_MAX_BLOCK_IDX = QUPV3_SSC,
}QUPV3_BLOCK_TYPE;

#define GSI_0_REG_BASE         (QUPV3_0_QUPV3_ID_3_BASE + 0x00004000)
#define GSI_REG_BASE           0x0

#endif /* __GPITGT_H__ */
