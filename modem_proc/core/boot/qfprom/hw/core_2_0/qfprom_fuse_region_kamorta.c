/*===========================================================================

                       QFPROM  Driver Source  Code

DESCRIPTION
 Contains target specific definitions and APIs to be used to read and write
 qfprom values for sec ctrl 3.0.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright 2018 by Qualcomm Technologies Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/boot/qfprom/hw/core_2_0/qfprom_fuse_region_kamorta.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/26/18   aus     Initial version for Nicobar

============================================================================*/

/*=============================================================================

                            INCLUDE FILES FOR MODULE

=============================================================================*/
#include QFPROM_HWIO_REG_INCLUDE_H
#include QFPROM_TARGET_INCLUDE_H
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
  QFPROM_CM_CORE_PRIVATE_OTP_REGION = 0,
  QFPROM_LCM_REGION,
  QFPROM_PRI_KEY_DERIVATION_KEY_REGION,
  QFPROM_CM_FEAT_CONFIG_REGION,  
  QFPROM_MRC_2_0_REGION,
  QFPROM_PTE_REGION,
  QFPROM_READ_PERMISSION_REGION,
  QFPROM_WRITE_PERMISSION_REGION,
  QFPROM_FEC_EN_REGION,
  QFPROM_OEM_CONFIG_REGION,
  QFPROM_FEAT_CONFIG_REGION,  
  QFPROM_ANTI_ROLLBACK_1_REGION,
  QFPROM_ANTI_ROLLBACK_2_REGION,
  QFPROM_ANTI_ROLLBACK_3_REGION,
  QFPROM_ANTI_ROLLBACK_4_REGION,
  QFPROM_PK_HASH0_REGION,
  QFPROM_CALIB_REGION,
  QFPROM_MEM_CONFIG_REGION,
  QFPROM_QC_SPARE_REG18_REGION,
  QFPROM_QC_SPARE_REG19_REGION,
  QFPROM_QC_SPARE_REG20_REGION,  
  QFPROM_OEM_IMAGE_ENCR_KEY_REGION,
  QFPROM_OEM_SEC_BOOT_REGION,
  QFPROM_SEC_KEY_DERIVATION_KEY_REGION,
  QFPROM_BOOT_ROM_PATCH_REGION,
  QFPROM_IMAGE_ENCR_KEY1_REGION,
  QFPROM_USER_DATA_KEY_REGION,
  QFPROM_OEM_SPARE_REG27_REGION,
  QFPROM_OEM_SPARE_REG28_REGION,
  QFPROM_OEM_SPARE_REG29_REGION,
  QFPROM_OEM_SPARE_REG30_REGION,
  QFPROM_OEM_SPARE_REG31_REGION,
  
  /* Add above this */
  QFPROM_LAST_REGION_DUMMY,
  QFPROM_MAX_REGION_ENUM                = 0x7FFF /* To ensure it's 16 bits wide */
} QFPROM_REGION_NAME;


const QFPROM_REGION_INFO qfprom_region_data[] =
{
  {
    QFPROM_ANTI_ROLLBACK_4_REGION,
    1,
    QFPROM_FEC_NONE,
    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_ADDR ,
    HWIO_QFPROM_CORR_ANTI_ROLLBACK_4_LSB_ADDR ,
    HWIO_QFPROM_CORR_RD_PERM_LSB_ANTI_ROLLBACK_4_BMSK,
    HWIO_QFPROM_CORR_WR_PERM_LSB_ANTI_ROLLBACK_4_BMSK,
    QFPROM_ROW_LSB,
    TRUE,
    14
  },

  {
    QFPROM_OEM_SPARE_REG27_REGION, 
    2,
    QFPROM_FEC_NONE,
    HWIO_QFPROM_RAW_OEM_SPARE_REGn_ROW0_LSB_ADDR(27),
    HWIO_QFPROM_CORR_OEM_SPARE_REGn_ROW0_LSB_ADDR(27),
    HWIO_QFPROM_CORR_RD_PERM_LSB_OEM_SPARE_REG27_BMSK,
    HWIO_QFPROM_CORR_WR_PERM_LSB_OEM_SPARE_REG27_BMSK,
    QFPROM_ROW_LSB,
    TRUE,
    27
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
