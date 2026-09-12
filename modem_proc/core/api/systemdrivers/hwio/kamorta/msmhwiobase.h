#ifndef __MSMHWIOBASE_H__
#define __MSMHWIOBASE_H__
/*
===========================================================================
*/
/**
  @file msmhwiobase.h
  @brief Auto-generated HWIO base include file.
*/
/*
  ===========================================================================

  Copyright (c) 2020 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

  ===========================================================================

  $Header: //components/rel/core.mpss/10.0/api/systemdrivers/hwio/kamorta/msmhwiobase.h#6 $
  $DateTime: 2020/04/01 01:12:06 $
  $Author: pwbldsvc $

  ===========================================================================
*/

/*----------------------------------------------------------------------------
 * BASE: BOOT_ROM
 *--------------------------------------------------------------------------*/

#define BOOT_ROM_BASE                                               0xa0000000
#define BOOT_ROM_BASE_SIZE                                          0x00100000
#define BOOT_ROM_BASE_PHYS                                          0x00100000

/*----------------------------------------------------------------------------
 * BASE: CORE_TOP_CSR
 *--------------------------------------------------------------------------*/

#define CORE_TOP_CSR_BASE                                           0xa0100000
#define CORE_TOP_CSR_BASE_SIZE                                      0x00100000
#define CORE_TOP_CSR_BASE_PHYS                                      0x00300000

/*----------------------------------------------------------------------------
 * BASE: TLMM
 *--------------------------------------------------------------------------*/

#define TLMM_BASE                                                   0xa0200000
#define TLMM_BASE_SIZE                                              0x00c00000
#define TLMM_BASE_PHYS                                              0x00400000

/*----------------------------------------------------------------------------
 * BASE: CLK_CTL
 *--------------------------------------------------------------------------*/

#define CLK_CTL_BASE                                                0xa0e00000
#define CLK_CTL_BASE_SIZE                                           0x00200000
#define CLK_CTL_BASE_PHYS                                           0x01400000

/*----------------------------------------------------------------------------
 * BASE: QM_MPU_CFG_QM_MPU_WRAPPER
 *--------------------------------------------------------------------------*/

#define QM_MPU_CFG_QM_MPU_WRAPPER_BASE                              0xa1274000
#define QM_MPU_CFG_QM_MPU_WRAPPER_BASE_SIZE                         0x00001200
#define QM_MPU_CFG_QM_MPU_WRAPPER_BASE_PHYS                         0x01874000

/*----------------------------------------------------------------------------
 * BASE: CNOC_MS_MPU_CFG
 *--------------------------------------------------------------------------*/

#define CNOC_MS_MPU_CFG_BASE                                        0xa1378000
#define CNOC_MS_MPU_CFG_BASE_SIZE                                   0x00002400
#define CNOC_MS_MPU_CFG_BASE_PHYS                                   0x01878000

/*----------------------------------------------------------------------------
 * BASE: CNOC_SNOC_MS_MPU_CFG
 *--------------------------------------------------------------------------*/

#define CNOC_SNOC_MS_MPU_CFG_BASE                                   0xa147c000
#define CNOC_SNOC_MS_MPU_CFG_BASE_SIZE                              0x00001e00
#define CNOC_SNOC_MS_MPU_CFG_BASE_PHYS                              0x0187c000

/*----------------------------------------------------------------------------
 * BASE: MDSP_MPU_CFG_MPU32Q2N7S1V0_16_CL36M35L12_AHB
 *--------------------------------------------------------------------------*/

#define MDSP_MPU_CFG_MPU32Q2N7S1V0_16_CL36M35L12_AHB_BASE           0xa157e000
#define MDSP_MPU_CFG_MPU32Q2N7S1V0_16_CL36M35L12_AHB_BASE_SIZE      0x00001800
#define MDSP_MPU_CFG_MPU32Q2N7S1V0_16_CL36M35L12_AHB_BASE_PHYS      0x0187e000

/*----------------------------------------------------------------------------
 * BASE: SYSTEM_NOC
 *--------------------------------------------------------------------------*/

#define SYSTEM_NOC_BASE                                             0xa1680000
#define SYSTEM_NOC_BASE_SIZE                                        0x0005d080
#define SYSTEM_NOC_BASE_PHYS                                        0x01880000

/*----------------------------------------------------------------------------
 * BASE: CONFIG_NOC
 *--------------------------------------------------------------------------*/

#define CONFIG_NOC_BASE                                             0xa1700000
#define CONFIG_NOC_BASE_SIZE                                        0x00009200
#define CONFIG_NOC_BASE_PHYS                                        0x01900000

/*----------------------------------------------------------------------------
 * BASE: PKA_WRAPPER
 *--------------------------------------------------------------------------*/

#define PKA_WRAPPER_BASE                                            0xa1880000
#define PKA_WRAPPER_BASE_SIZE                                       0x00071280
#define PKA_WRAPPER_BASE_PHYS                                       0x01980000

/*----------------------------------------------------------------------------
 * BASE: CRYPTO0_CRYPTO_TOP
 *--------------------------------------------------------------------------*/

#define CRYPTO0_CRYPTO_TOP_BASE                                     0xa1900000
#define CRYPTO0_CRYPTO_TOP_BASE_SIZE                                0x00040000
#define CRYPTO0_CRYPTO_TOP_BASE_PHYS                                0x01b00000

/*----------------------------------------------------------------------------
 * BASE: SECURITY_CONTROL
 *--------------------------------------------------------------------------*/

#define SECURITY_CONTROL_BASE                                       0xa1a40000
#define SECURITY_CONTROL_BASE_SIZE                                  0x00010000
#define SECURITY_CONTROL_BASE_PHYS                                  0x01b40000

/*----------------------------------------------------------------------------
 * BASE: PRNG_CFG_PRNG_TOP
 *--------------------------------------------------------------------------*/

#define PRNG_CFG_PRNG_TOP_BASE                                      0xa1b50000
#define PRNG_CFG_PRNG_TOP_BASE_SIZE                                 0x00010000
#define PRNG_CFG_PRNG_TOP_BASE_PHYS                                 0x01b50000

/*----------------------------------------------------------------------------
 * BASE: RAMBLUR_PIMEM
 *--------------------------------------------------------------------------*/

#define RAMBLUR_PIMEM_BASE                                          0xa1c60000
#define RAMBLUR_PIMEM_BASE_SIZE                                     0x00008000
#define RAMBLUR_PIMEM_BASE_PHYS                                     0x01b60000

/*----------------------------------------------------------------------------
 * BASE: OCIMEM_WRAPPER_CSR
 *--------------------------------------------------------------------------*/

#define OCIMEM_WRAPPER_CSR_BASE                                     0xa1dd0000
#define OCIMEM_WRAPPER_CSR_BASE_SIZE                                0x00006000
#define OCIMEM_WRAPPER_CSR_BASE_PHYS                                0x01bd0000

/*----------------------------------------------------------------------------
 * BASE: QC_DCC_KAMORTA_12KB4LL
 *--------------------------------------------------------------------------*/

#define QC_DCC_KAMORTA_12KB4LL_BASE                                 0xa1ee0000
#define QC_DCC_KAMORTA_12KB4LL_BASE_SIZE                            0x00010000
#define QC_DCC_KAMORTA_12KB4LL_BASE_PHYS                            0x01be0000

/*----------------------------------------------------------------------------
 * BASE: PMIC_ARB
 *--------------------------------------------------------------------------*/

#define PMIC_ARB_BASE                                               0xa1f00000
#define PMIC_ARB_BASE_SIZE                                          0x023a0000
#define PMIC_ARB_BASE_PHYS                                          0x01c00000

/*----------------------------------------------------------------------------
 * BASE: MAPSS
 *--------------------------------------------------------------------------*/

#define MAPSS_BASE                                                  0xa4300000
#define MAPSS_BASE_SIZE                                             0x00040000
#define MAPSS_BASE_PHYS                                             0x04400000

/*----------------------------------------------------------------------------
 * BASE: HWKM_MASTER_CFG_KEYMANAGER_MASTER_REGS
 *--------------------------------------------------------------------------*/

#define HWKM_MASTER_CFG_KEYMANAGER_MASTER_REGS_BASE                 0xa4440000
#define HWKM_MASTER_CFG_KEYMANAGER_MASTER_REGS_BASE_SIZE            0x00020000
#define HWKM_MASTER_CFG_KEYMANAGER_MASTER_REGS_BASE_PHYS            0x04440000

/*----------------------------------------------------------------------------
 * BASE: DDR_SS
 *--------------------------------------------------------------------------*/

#define DDR_SS_BASE                                                 0xa457d000
#define DDR_SS_BASE_SIZE                                            0x000a3000
#define DDR_SS_BASE_PHYS                                            0x0447d000

/*----------------------------------------------------------------------------
 * BASE: RPM_SS_MSG_RAM_START_ADDRESS
 *--------------------------------------------------------------------------*/

#define RPM_SS_MSG_RAM_START_ADDRESS_BASE                           0xa47f0000
#define RPM_SS_MSG_RAM_START_ADDRESS_BASE_SIZE                      0x00007000
#define RPM_SS_MSG_RAM_START_ADDRESS_BASE_PHYS                      0x045f0000

/*----------------------------------------------------------------------------
 * BASE: RPM
 *--------------------------------------------------------------------------*/

#define RPM_BASE                                                    0xa4800000
#define RPM_BASE_SIZE                                               0x00100000
#define RPM_BASE_PHYS                                               0x04600000

/*----------------------------------------------------------------------------
 * BASE: PERIPH_SS_SDC1_SDCC5_TOP
 *--------------------------------------------------------------------------*/

#define PERIPH_SS_SDC1_SDCC5_TOP_BASE                               0xa4940000
#define PERIPH_SS_SDC1_SDCC5_TOP_BASE_SIZE                          0x00020000
#define PERIPH_SS_SDC1_SDCC5_TOP_BASE_PHYS                          0x04740000

/*----------------------------------------------------------------------------
 * BASE: UFS_MEM_UFS_REGS
 *--------------------------------------------------------------------------*/

#define UFS_MEM_UFS_REGS_BASE                                       0xa4a00000
#define UFS_MEM_UFS_REGS_BASE_SIZE                                  0x00020000
#define UFS_MEM_UFS_REGS_BASE_PHYS                                  0x04800000

/*----------------------------------------------------------------------------
 * BASE: QPIC_QPIC_XPU3
 *--------------------------------------------------------------------------*/

#define QPIC_QPIC_XPU3_BASE                                         0xa4b40000
#define QPIC_QPIC_XPU3_BASE_SIZE                                    0x00040000
#define QPIC_QPIC_XPU3_BASE_PHYS                                    0x04840000

/*----------------------------------------------------------------------------
 * BASE: QUPV3_0_QUPV3_ID_3
 *--------------------------------------------------------------------------*/

#define QUPV3_0_QUPV3_ID_3_BASE                                     0xa4c00000
#define QUPV3_0_QUPV3_ID_3_BASE_SIZE                                0x000c7000
#define QUPV3_0_QUPV3_ID_3_BASE_PHYS                                0x04a00000

/*----------------------------------------------------------------------------
 * BASE: IPA_0_IPA_WRAPPER
 *--------------------------------------------------------------------------*/

#define IPA_0_IPA_WRAPPER_BASE                                      0xa4d00000
#define IPA_0_IPA_WRAPPER_BASE_SIZE                                 0x00100000
#define IPA_0_IPA_WRAPPER_BASE_PHYS                                 0x05800000

/*----------------------------------------------------------------------------
 * BASE: MSS_TOP
 *--------------------------------------------------------------------------*/

#define MSS_TOP_BASE                                                0xec000000
#define MSS_TOP_BASE_SIZE                                           0x00580000
#define MSS_TOP_BASE_PHYS                                           0x06000000

/*----------------------------------------------------------------------------
 * BASE: MSS_L2_CFG
 *--------------------------------------------------------------------------*/

#define MSS_L2_CFG_BASE                                             0xa4ea0000
#define MSS_L2_CFG_BASE_SIZE                                        0x00001000
#define MSS_L2_CFG_BASE_PHYS                                        0x065a0000

/*----------------------------------------------------------------------------
 * BASE: QDSS_SOC_DBG
 *--------------------------------------------------------------------------*/

#define QDSS_SOC_DBG_BASE                                           0xa4f00000
#define QDSS_SOC_DBG_BASE_SIZE                                      0x02000000
#define QDSS_SOC_DBG_BASE_PHYS                                      0x08000000

/*----------------------------------------------------------------------------
 * BASE: SNOC_MSSNAV_MS_MPU_CFG
 *--------------------------------------------------------------------------*/

#define SNOC_MSSNAV_MS_MPU_CFG_BASE                                 0xa6f00000
#define SNOC_MSSNAV_MS_MPU_CFG_BASE_SIZE                            0x00001400
#define SNOC_MSSNAV_MS_MPU_CFG_BASE_PHYS                            0x0c000000

/*----------------------------------------------------------------------------
 * BASE: SNOC_A2NOC_MS_MPU_CFG
 *--------------------------------------------------------------------------*/

#define SNOC_A2NOC_MS_MPU_CFG_BASE                                  0xa7002000
#define SNOC_A2NOC_MS_MPU_CFG_BASE_SIZE                             0x00001800
#define SNOC_A2NOC_MS_MPU_CFG_BASE_PHYS                             0x0c002000

/*----------------------------------------------------------------------------
 * BASE: SNOC_BOOTIMEM_SS_MPU_CFG
 *--------------------------------------------------------------------------*/

#define SNOC_BOOTIMEM_SS_MPU_CFG_BASE                               0xa7106000
#define SNOC_BOOTIMEM_SS_MPU_CFG_BASE_SIZE                          0x00001200
#define SNOC_BOOTIMEM_SS_MPU_CFG_BASE_PHYS                          0x0c006000

/*----------------------------------------------------------------------------
 * BASE: WCSS_WRAPPER
 *--------------------------------------------------------------------------*/

#define WCSS_WRAPPER_BASE                                           0xa7200000
#define WCSS_WRAPPER_BASE_SIZE                                      0x00800000
#define WCSS_WRAPPER_BASE_PHYS                                      0x0c800000


#endif /* __MSMHWIOBASE_H__ */
