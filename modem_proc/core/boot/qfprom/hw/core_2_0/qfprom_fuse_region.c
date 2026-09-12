/*===========================================================================

                       QFPROM  Driver Source  Code

DESCRIPTION
 Contains target specific defintions and APIs to be used to read and write
 qfprom values.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright  2012 - 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/boot/qfprom/hw/core_2_0/qfprom_fuse_region.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/21/15   jz      Updated for 9x55
10/15/15   jz      Ported changes from 9x45
10/31/14   jz      Updated for 9x45
12/05/12   kedara  Initial revision for Qfprom driver in modem

============================================================================*/

/*=============================================================================

                            INCLUDE FILES FOR MODULE

=============================================================================*/
#include "qfprom_hwioreg.h"
#include "qfprom_target.h"
#include "qfprom_target_common.h"

/*=============================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

=============================================================================*/
/*
**  Array containing QFPROM data items that can be read and associated
**  registers, mask and shift values for the same.
**
**  Note: The table is image specific based on requirements to support only
**  regions which are required by that image.
**
*/
/*---------------------------------------------------------------------------
  QFPROM REGIONS 
---------------------------------------------------------------------------*/
typedef enum
  {
  QFPROM_JTAG_ID_REGION = 0,
  QFPROM_READ_WRITE_PERMISSION_REGION,
  QFPROM_ANTI_ROLLBACK_1_REGION,
  QFPROM_ANTI_ROLLBACK_2_REGION,
  QFPROM_ANTI_ROLLBACK_3_REGION,
  QFPROM_OEM_CONFIG_REGION,
  QFPROM_FEATURE_CONFIG_REGION,
  QFPROM_CM_FEATURE_CONFIG_REGION,
  QFPROM_PRI_KEY_DERIVATION_KEY_REGION,
  QFPROM_SEC_KEY_DERIVATION_KEY_REGION,
  QFPROM_OEM_SEC_BOOT_REGION,
  QFPROM_CALIB_REGION,
  QFPROM_PK_HASH_REGION,
  QFPROM_CALIB2_REGION,
  QFPROM_MEM_CONFIG_REGION,
  QFPROM_BOOT_ROM_PATCH,
  QFPROM_FEC_ENABLE_REGION,
  QFPROM_SPARE_REG18_REGION,
  QFPROM_SPARE_REG19_REGION,
  QFPROM_SPARE_REG20_REGION,
  QFPROM_SPARE_REG21_REGION,
  QFPROM_SPARE_REG22_REGION,
  QFPROM_SPARE_REG23_REGION,
  QFPROM_SPARE_REG24_REGION,
  QFPROM_SPARE_REG25_REGION,
  QFPROM_SPARE_REG26_REGION,

  /* Add above this */
  QFPROM_LAST_REGION_DUMMY,
  QFPROM_MAX_REGION_ENUM                = 0x7FFF /* To ensure it's 16 bits wide */
} QFPROM_REGION_NAME;

const QFPROM_REGION_INFO qfprom_region_data[] =
{
    {
        QFPROM_ANTI_ROLLBACK_3_REGION,
        1,
        QFPROM_FEC_NONE,
        HWIO_QFPROM_RAW_ANTI_ROLLBACK_3_LSB_ADDR,
        HWIO_QFPROM_CORR_ANTI_ROLLBACK_3_LSB_ADDR,
        HWIO_QFPROM_CORR_RD_WR_PERM_LSB_ANTI_ROLLBACK_3_BMSK,
        HWIO_QFPROM_CORR_RD_WR_PERM_MSB_ANTI_ROLLBACK_3_BMSK,
        QFPROM_ROW_LSB,
        TRUE,
        4
    },

    {
        QFPROM_SPARE_REG20_REGION,
        1,
        QFPROM_FEC_63_56,
        HWIO_QFPROM_RAW_SPARE_REG20_LSB_ADDR,
        HWIO_QFPROM_CORR_SPARE_REG20_LSB_ADDR,
        HWIO_QFPROM_CORR_RD_WR_PERM_LSB_SPARE20_BMSK,
        HWIO_QFPROM_CORR_RD_WR_PERM_MSB_SPARE20_BMSK,
        QFPROM_ROW_LSB,
        TRUE,
        20
    },
};

uint32 qfprom_region_data_size = sizeof(qfprom_region_data)/sizeof(QFPROM_REGION_INFO);

/*===========================================================================
**  Function :    : qfprom_update_fusemap

** ==========================================================================
*/
/*!
*
* @brief :  This function updates the qfprom_region_data table pointer if needed.
*
* @param  :
*   qfprom_fusemap_data *fusemap_data_ptr  - pointer to access the fusemap related variables

* @par Dependencies:
*
* @retval: None
*
*/
void qfprom_update_fusemap
( 
  qfprom_fusemap_data *fusemap_data_ptr
)
{
}