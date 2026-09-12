
/*
===========================================================================
*/
/**
  @file HWIOBaseMap.c
  @brief Auto-generated HWIO Device Configuration base file.

  DESCRIPTION:
    This file contains Device Configuration data structures for mapping
    physical and virtual memory for HWIO blocks.
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

  $Header: //components/rel/core.mpss/10.0/settings/systemdrivers/hwio/config/kamorta/HWIOBaseMap.c#6 $
  $DateTime: 2020/04/01 01:12:06 $
  $Author: pwbldsvc $

  ===========================================================================
*/

/*=========================================================================
      Include Files
==========================================================================*/

#include "HWIOInterface.h"


/*=========================================================================
      Data Definitions
==========================================================================*/

static HWIOModuleType HWIOModules_BOOT_ROM[] =
{
  { "BOOT_ROM_MPU32Q2N7S1V0_4_CL36M17L10_AHB",     0x000fe000, 0x00001200 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_CORE_TOP_CSR[] =
{
  { "TCSR_MUTEX_RPU32Q2N7S1V0_64_CL36L12",         0x00000000, 0x00003000 },
  { "TCSR_TCSR_MUTEX",                             0x00040000, 0x00040000 },
  { "TCSR_REGS_RPU32Q2N7S1V0_64_CL36L12",          0x00080000, 0x00003000 },
  { "TCSR_TCSR_REGS",                              0x000c0000, 0x00040000 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_TLMM[] =
{
  { "TLMM_XPU_EAST_XE",                            0x00800000, 0x00006780 },
  { "TLMM_EAST",                                   0x00900000, 0x00300000 },
  { "TLMM_XPU_SOUTH_XS",                           0x00400000, 0x00006780 },
  { "TLMM_SOUTH",                                  0x00500000, 0x00300000 },
  { "TLMM_XPU_WEST_XW",                            0x00000000, 0x00006780 },
  { "TLMM_WEST",                                   0x00100000, 0x00300000 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_CLK_CTL[] =
{
  { "GCC_CLK_CTL_REG",                             0x00000000, 0x001f0000 },
  { "GCC_RPU_RPU32Q2N7S1V0_200_CL36L12",           0x001f0000, 0x00007400 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_QM_MPU_CFG_QM_MPU_WRAPPER[] =
{
  { "QM_MPU_CFG_QM_MPU_CFG_QM_MPU_CFG_MPU32Q2N7S1V0_4_CL36M23L12_AHB", 0x00000000, 0x00001200 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_CNOC_MS_MPU_CFG[] =
{
  { "CNOC_MS_MPU_CFG",                             0x00000000, 0x00002400 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_CNOC_SNOC_MS_MPU_CFG[] =
{
  { "CNOC_SNOC_MS_MPU_CFG",                        0x00000000, 0x00001e00 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_MDSP_MPU_CFG_MPU32Q2N7S1V0_16_CL36M35L12_AHB[] =
{
  { "MDSP_MPU_CFG_MPU32Q2N7S1V0_16_CL36M35L12_AHB", 0x00000000, 0x00001800 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_SYSTEM_NOC[] =
{
  { "SYSTEM_NOC",                                  0x00000000, 0x0005d080 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_CONFIG_NOC[] =
{
  { "CONFIG_NOC",                                  0x00000000, 0x00009200 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_PKA_WRAPPER[] =
{
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_CRYPTO0_CRYPTO_TOP[] =
{
  { "CRYPTO0_CRYPTO",                              0x0003a000, 0x00006000 },
  { "CRYPTO0_CRYPTO_BAM",                          0x00004000, 0x00024000 },
  { "CRYPTO0_CRYPTO_BAM_XPU3_BAM",                 0x00001000, 0x00003000 },
  { "CRYPTO0_CRYPTO_BAM_VMIDMT_BAM",               0x00000000, 0x00001000 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_SECURITY_CONTROL[] =
{
  { "SECURITY_CONTROL_CORE",                       0x00000000, 0x00007000 },
  { "SECURE_CHANNEL",                              0x00008000, 0x00003000 },
  { "SEC_CTRL_APU_APU32Q2N7S1V0_25_CL36",          0x0000e000, 0x00001c80 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_PRNG_CFG_PRNG_TOP[] =
{
  { "PRNG_CFG_CM_CM_PRNG_CM",                      0x00000000, 0x00001000 },
  { "PRNG_CFG_TZ_TZ_PRNG_TZ",                      0x00001000, 0x00001000 },
  { "PRNG_CFG_MSA_MSA_PRNG_SUB",                   0x00002000, 0x00001000 },
  { "PRNG_CFG_EE2_EE2_PRNG_SUB",                   0x00003000, 0x00001000 },
  { "PRNG_CFG_EE3_EE3_PRNG_SUB",                   0x00004000, 0x00001000 },
  { "PRNG_CFG_EE4_EE4_PRNG_SUB",                   0x00005000, 0x00001000 },
  { "PRNG_CFG_EE5_EE5_PRNG_SUB",                   0x00006000, 0x00001000 },
  { "PRNG_CFG_EE6_EE6_PRNG_SUB",                   0x00007000, 0x00001000 },
  { "PRNG_CFG_EE7_EE7_PRNG_SUB",                   0x00008000, 0x00001000 },
  { "PRNG_CFG_EE8_EE8_PRNG_SUB",                   0x00009000, 0x00001000 },
  { "PRNG_CFG_EE9_EE9_PRNG_SUB",                   0x0000a000, 0x00001000 },
  { "PRNG_CFG_EE10_EE10_PRNG_SUB",                 0x0000b000, 0x00001000 },
  { "PRNG_CFG_EE11_EE11_PRNG_SUB",                 0x0000c000, 0x00001000 },
  { "PRNG_CFG_EE12_EE12_PRNG_SUB",                 0x0000d000, 0x00001000 },
  { "PRNG_CFG_EE13_EE13_PRNG_SUB",                 0x0000e000, 0x00001000 },
  { "PRNG_CFG_EE14_EE14_PRNG_SUB",                 0x0000f000, 0x00001000 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_RAMBLUR_PIMEM[] =
{
  { "RAMBLUR_PIMEM_REGS",                          0x00000000, 0x00004000 },
  { "RAMBLUR_PIMEM_APU_APU32Q2N7S1V1_2_CL36",      0x00004000, 0x00001100 },
  { "RAMBLUR_PIMEM_MPU_MPU32Q2N7S1V1_8_CL36M25L10_AXI", 0x00006000, 0x00001400 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_OCIMEM_WRAPPER_CSR[] =
{
  { "OCIMEM_CSR",                                  0x00000000, 0x00000800 },
  { "OCIMEM_APU_WRAPPER",                          0x00002000, 0x00001100 },
  { "OCIMEM_APU",                                  0x00002000, 0x00001100 },
  { "OCIMEM_MPU_WRAPPER",                          0x00004000, 0x00001400 },
  { "OCIMEM_MPU",                                  0x00004000, 0x00001400 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_QC_DCC_KAMORTA_12KB4LL[] =
{
  { "DCC_APU",                                     0x00000000, 0x00001300 },
  { "DCC_CFG",                                     0x00002000, 0x00001000 },
  { "DCC_RAM",                                     0x0000d000, 0x00003000 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_PMIC_ARB[] =
{
  { "SPMI_CFG_TOP",                                0x00000000, 0x00030000 },
  { "SPMI_GENI_CFG",                               0x0000a000, 0x00000700 },
  { "SPMI_CFG",                                    0x0000a700, 0x00025900 },
  { "SPMI_PIC_OWNER",                              0x02300000, 0x000a0000 },
  { "PMIC_ARB_MGPI",                               0x00060000, 0x00080008 },
  { "PMIC_ARB_MPU32Q2N7S1V1_25_CL36M27L12_AHB",    0x00030000, 0x00001c80 },
  { "PMIC_ARB_CORE",                               0x00040000, 0x00010000 },
  { "PMIC_ARB_CORE_REGISTERS",                     0x00200000, 0x02000000 },
  { "PMIC_ARB_CORE_REGISTERS_OBS",                 0x02200000, 0x00100000 },
  { "OBS_ACC",                                     0x00050000, 0x00000400 },
  { "PMIC_ARB_XBL_SEC",                            0x00170000, 0x00000040 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_MAPSS[] =
{
  { "MPM",                                         0x00000000, 0x00001000 },
  { "G_CTRL_CNTR",                                 0x00001000, 0x00001000 },
  { "G_RD_CNTR",                                   0x00002000, 0x00001000 },
  { "SLP_CNTR",                                    0x00003000, 0x00001000 },
  { "QTIMR_AC",                                    0x00004000, 0x00001000 },
  { "QTIMR_V1",                                    0x00005000, 0x00001000 },
  { "TSYNC",                                       0x00006000, 0x00001000 },
  { "WDOG",                                        0x00007000, 0x00000020 },
  { "MAPSS_APU",                                   0x00008000, 0x00001880 },
  { "PSHOLD",                                      0x0000b000, 0x00001000 },
  { "MAPSS_CC",                                    0x0000c000, 0x00004000 },
  { "TSENS0",                                      0x00010000, 0x00001000 },
  { "TSENS0_TM",                                   0x00011000, 0x00001000 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_HWKM_MASTER_CFG_KEYMANAGER_MASTER_REGS[] =
{
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_DDR_SS[] =
{
  { "MCCC_MCCC",                                   0x00000000, 0x00000800 },
  { "MCCC_MCCC_CH0_MCCC_CH0_MCCC_CH_REGS",         0x00000000, 0x00000100 },
  { "MCCC_MCCC_REGS",                              0x00000200, 0x00000100 },
  { "BIMC",                                        0x00003000, 0x00080000 },
  { "BIMC_GLOBAL0",                                0x00003000, 0x00001000 },
  { "BIMC_GLOBAL1",                                0x00004000, 0x00001000 },
  { "BIMC_GLOBAL2",                                0x00005000, 0x00001000 },
  { "BIMC_CONFIG_APU",                             0x00007000, 0x00001800 },
  { "BIMC_PERFMON",                                0x00006000, 0x00001000 },
  { "BIMC_DTE",                                    0x00009000, 0x0000005c },
  { "BIMC_M_APP_MPORT",                            0x0000b000, 0x00001000 },
  { "BIMC_M_GPU_MPORT",                            0x0000f000, 0x00001000 },
  { "BIMC_M_MMSS_RT_MPORT",                        0x00013000, 0x00001000 },
  { "BIMC_M_MMSS_NRT_MPORT",                       0x00017000, 0x00001000 },
  { "BIMC_M_TCU_MPORT",                            0x0001b000, 0x00001000 },
  { "BIMC_M_MDSP_MPORT",                           0x0001f000, 0x00001000 },
  { "BIMC_M_SYS_MPORT",                            0x00023000, 0x00001000 },
  { "BIMC_M_APP_PROF",                             0x0000c000, 0x00001000 },
  { "BIMC_M_GPU_PROF",                             0x00010000, 0x00001000 },
  { "BIMC_M_MMSS_RT_PROF",                         0x00014000, 0x00001000 },
  { "BIMC_M_MMSS_NRT_PROF",                        0x00018000, 0x00001000 },
  { "BIMC_M_TCU_PROF",                             0x0001c000, 0x00001000 },
  { "BIMC_M_MDSP_PROF",                            0x00020000, 0x00001000 },
  { "BIMC_M_SYS_PROF",                             0x00024000, 0x00001000 },
  { "BIMC_S_DDR0_SCMO",                            0x00033000, 0x00001000 },
  { "BIMC_S_SYS_SWAY",                             0x0003f000, 0x00001000 },
  { "BIMC_S_DEFAULT_SWAY",                         0x00047000, 0x00001000 },
  { "BIMC_S_DDR0_ARB",                             0x00034000, 0x00001000 },
  { "BIMC_S_SYS_ARB",                              0x00040000, 0x00001000 },
  { "BIMC_S_DEFAULT_ARB",                          0x00048000, 0x00001000 },
  { "BIMC_S_DDR0",                                 0x00035000, 0x00001c80 },
  { "BIMC_S_DDR0_DPE",                             0x00037000, 0x00001000 },
  { "BIMC_S_DDR0_SHKE",                            0x00038000, 0x00001000 },
  { "BIMC_S_DDR0_DTTS_CFG",                        0x00039000, 0x00001000 },
  { "BIMC_S_DDR0_DTTS_SRAM",                       0x0003a000, 0x00004000 },
  { "CH0_CA0_DDR_PHY",                             0x00083000, 0x00001000 },
  { "CH0_CA1_DDR_PHY",                             0x00084000, 0x00001000 },
  { "CH0_DQ0_DDR_PHY",                             0x00085000, 0x00001000 },
  { "CH0_DQ1_DDR_PHY",                             0x00086000, 0x00001000 },
  { "CH0_DQ2_DDR_PHY",                             0x00087000, 0x00001000 },
  { "CH0_DQ3_DDR_PHY",                             0x00088000, 0x00001000 },
  { "CH0_DDR_CC",                                  0x00089000, 0x00001000 },
  { "DDR_REG_DDR_SS_REGS",                         0x00091000, 0x00000400 },
  { "DDRSS_AHB2PHY_SWMAN",                         0x000a1000, 0x00000400 },
  { "DDRSS_AHB2PHY_BROADCAST_SWMAN1",              0x000a2000, 0x00001000 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_RPM_SS_MSG_RAM_START_ADDRESS[] =
{
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_RPM[] =
{
  { "RPM_DEC",                                     0x00080000, 0x00002000 },
  { "RPM_QTMR_AC",                                 0x00082000, 0x00001000 },
  { "RPM_F0_QTMR_V1_F0",                           0x00083000, 0x00001000 },
  { "RPM_F1_QTMR_V1_F1",                           0x00084000, 0x00001000 },
  { "RPM_MSTR_MPU",                                0x00086000, 0x00001a00 },
  { "RPM_VMIDMT",                                  0x00088000, 0x00001000 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_PERIPH_SS_SDC1_SDCC5_TOP[] =
{
  { "PERIPH_SS_SDC1_SDCC_SDCC5_HC",                0x00004000, 0x00001000 },
  { "PERIPH_SS_SDC1_SDCC_SDCC5_HC_CMDQ",           0x00005000, 0x00001000 },
  { "PERIPH_SS_SDC1_SDCC_ICE",                     0x00008000, 0x00008000 },
  { "PERIPH_SS_SDC1_SDCC_ICE_REGS",                0x00008000, 0x00002000 },
  { "PERIPH_SS_SDC1_SDCC_ICE_LUT_KEYS",            0x0000a000, 0x00002000 },
  { "PERIPH_SS_SDC1_SDCC_ICE_XPU3",                0x0000c000, 0x00001200 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_UFS_MEM_UFS_REGS[] =
{
  { "UFS_MEM_UFS",                                 0x00004000, 0x00003000 },
  { "UFS_MEM_MPHY_UFS",                            0x00007000, 0x00000e00 },
  { "UFS_MEM_QSERDES_COM_QSERDES_COM_UFS_QSRV_COM", 0x00007000, 0x000001c4 },
  { "UFS_MEM_QSERDES_TX_QSERDES_TX_UFS_QMP_TX",    0x00007400, 0x0000012c },
  { "UFS_MEM_QSERDES_RX_QSERDES_RX_UFS_QMP_RX",    0x00007600, 0x00000200 },
  { "UFS_MEM_UFS_PHY_UFS_PHY_UFS_PCS",             0x00007c00, 0x000001b8 },
  { "UFS_MEM_ICE",                                 0x00010000, 0x00008000 },
  { "UFS_MEM_ICE_REGS",                            0x00010000, 0x00002000 },
  { "UFS_MEM_ICE_LUT_KEYS",                        0x00012000, 0x00002000 },
  { "UFS_MEM_ICE_XPU3",                            0x00014000, 0x00001200 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_QPIC_QPIC_XPU3[] =
{
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_QUPV3_0_QUPV3_ID_3[] =
{
  { "QUPV3_0_GSI_TOP",                             0x00000000, 0x00060000 },
  { "QUPV3_0_GSI",                                 0x00004000, 0x00050000 },
  { "QUPV3_0_XPU3",                                0x00000000, 0x00003000 },
  { "QUPV3_0_QUPV3_SE_WRAPPER",                    0x00080000, 0x00020000 },
  { "QUPV3_0_SE0",                                 0x00080000, 0x00004000 },
  { "QUPV3_0_SE0_GENI4_CFG",                       0x00080000, 0x00000100 },
  { "QUPV3_0_SE0_GENI4_IMAGE_REGS",                0x00080100, 0x00000500 },
  { "QUPV3_0_SE0_GENI4_DATA",                      0x00080600, 0x00000600 },
  { "QUPV3_0_SE0_QUPV3_SE_DMA",                    0x00080c00, 0x00000400 },
  { "QUPV3_0_SE0_GENI4_IMAGE",                     0x00081000, 0x00001000 },
  { "QUPV3_0_SE0_QUPV3_SEC",                       0x00082000, 0x00001000 },
  { "QUPV3_0_SE1",                                 0x00084000, 0x00004000 },
  { "QUPV3_0_SE1_GENI4_CFG",                       0x00084000, 0x00000100 },
  { "QUPV3_0_SE1_GENI4_IMAGE_REGS",                0x00084100, 0x00000500 },
  { "QUPV3_0_SE1_GENI4_DATA",                      0x00084600, 0x00000600 },
  { "QUPV3_0_SE1_QUPV3_SE_DMA",                    0x00084c00, 0x00000400 },
  { "QUPV3_0_SE1_GENI4_IMAGE",                     0x00085000, 0x00001000 },
  { "QUPV3_0_SE1_QUPV3_SEC",                       0x00086000, 0x00001000 },
  { "QUPV3_0_SE2",                                 0x00088000, 0x00004000 },
  { "QUPV3_0_SE2_GENI4_CFG",                       0x00088000, 0x00000100 },
  { "QUPV3_0_SE2_GENI4_IMAGE_REGS",                0x00088100, 0x00000500 },
  { "QUPV3_0_SE2_GENI4_DATA",                      0x00088600, 0x00000600 },
  { "QUPV3_0_SE2_QUPV3_SE_DMA",                    0x00088c00, 0x00000400 },
  { "QUPV3_0_SE2_GENI4_IMAGE",                     0x00089000, 0x00001000 },
  { "QUPV3_0_SE2_QUPV3_SEC",                       0x0008a000, 0x00001000 },
  { "QUPV3_0_SE3",                                 0x0008c000, 0x00004000 },
  { "QUPV3_0_SE3_GENI4_CFG",                       0x0008c000, 0x00000100 },
  { "QUPV3_0_SE3_GENI4_IMAGE_REGS",                0x0008c100, 0x00000500 },
  { "QUPV3_0_SE3_GENI4_DATA",                      0x0008c600, 0x00000600 },
  { "QUPV3_0_SE3_QUPV3_SE_DMA",                    0x0008cc00, 0x00000400 },
  { "QUPV3_0_SE3_GENI4_IMAGE",                     0x0008d000, 0x00001000 },
  { "QUPV3_0_SE3_QUPV3_SEC",                       0x0008e000, 0x00001000 },
  { "QUPV3_0_SE4",                                 0x00090000, 0x00004000 },
  { "QUPV3_0_SE4_GENI4_CFG",                       0x00090000, 0x00000100 },
  { "QUPV3_0_SE4_GENI4_IMAGE_REGS",                0x00090100, 0x00000500 },
  { "QUPV3_0_SE4_GENI4_DATA",                      0x00090600, 0x00000600 },
  { "QUPV3_0_SE4_QUPV3_SE_DMA",                    0x00090c00, 0x00000400 },
  { "QUPV3_0_SE4_GENI4_IMAGE",                     0x00091000, 0x00001000 },
  { "QUPV3_0_SE4_QUPV3_SEC",                       0x00092000, 0x00001000 },
  { "QUPV3_0_SE5",                                 0x00094000, 0x00004000 },
  { "QUPV3_0_SE5_GENI4_CFG",                       0x00094000, 0x00000100 },
  { "QUPV3_0_SE5_GENI4_IMAGE_REGS",                0x00094100, 0x00000500 },
  { "QUPV3_0_SE5_GENI4_DATA",                      0x00094600, 0x00000600 },
  { "QUPV3_0_SE5_QUPV3_SE_DMA",                    0x00094c00, 0x00000400 },
  { "QUPV3_0_SE5_GENI4_IMAGE",                     0x00095000, 0x00001000 },
  { "QUPV3_0_SE5_QUPV3_SEC",                       0x00096000, 0x00001000 },
  { "QUPV3_0_QUPV3_COMMON",                        0x000c0000, 0x00002000 },
  { "QUPV3_0_VMIDMT_SMR_48_SSD6_SID6_MA48_SP48_36", 0x000c6000, 0x00001000 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_IPA_0_IPA_WRAPPER[] =
{
  { "IPA_0_IPA_UC",                                0x00060000, 0x00014000 },
  { "IPA_0_IPA_UC_IPA_UC_RAM",                     0x00060000, 0x0000a000 },
  { "IPA_0_IPA_UC_IPA_UC_PER",                     0x00070000, 0x00002000 },
  { "IPA_0_IPA_UC_IPA_UC_MBOX",                    0x00072000, 0x00002000 },
  { "IPA_0_IPA_RAM",                               0x00047000, 0x00019000 },
  { "IPA_0_IPA_EE",                                0x00043000, 0x00004000 },
  { "IPA_0_IPA_DEBUG",                             0x00042000, 0x00001000 },
  { "IPA_0_IPA_CFG",                               0x00040000, 0x00002000 },
  { "IPA_0_IPA_VMIDMT",                            0x00030000, 0x00001000 },
  { "IPA_0_GSI_TOP",                               0x00000000, 0x00030000 },
  { "IPA_0_GSI_TOP_GSI",                           0x00004000, 0x00028000 },
  { "IPA_0_GSI_TOP_XPU3",                          0x00000000, 0x00002000 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_MSS_TOP[] =
{
  { "MSS_APU32Q2N7S1V0_5_CL36",                    0x00000000, 0x00001280 },
  { "MSS_RMB",                                     0x00020000, 0x00010000 },
  { "MSS_QDSP6V67SS_MSS",                          0x00080000, 0x00100000 },
  { "MSS_QDSP6V67SS",                              0x00080000, 0x00100000 },
  { "MSS_QDSP6V67SS_PUBLIC",                       0x00080000, 0x00020000 },
  { "MSS_QDSP6V67SS_PUB",                          0x00080000, 0x00010000 },
  { "MSS_QDSP6V67SS_PRIVATE",                      0x00100000, 0x00080000 },
  { "MSS_QDSP6V67SS_CSR",                          0x00100000, 0x0000d000 },
  { "MSS_QDSP6V67SS_L2VIC",                        0x00110000, 0x00001000 },
  { "MSS_QDSP6SS_QDSP6SS_QTMR_AC",                 0x00120000, 0x00001000 },
  { "MSS_QDSP6SS_QTMR_F0_0",                       0x00121000, 0x00001000 },
  { "MSS_QDSP6SS_QTMR_F1_1",                       0x00122000, 0x00001000 },
  { "MSS_QDSP6SS_QTMR_F2_2",                       0x00123000, 0x00001000 },
  { "MSS_QDSP6SS_QDSP6V67SS_RSCC",                 0x00130000, 0x00010000 },
  { "MSS_QDSP6SS_RSCC_RSCC_RSC",                   0x00130000, 0x00010000 },
  { "MSS_CC_MSS_CC_REG",                           0x00180000, 0x0000176d },
  { "MSS_PERPH_MSS_PERPH",                         0x00181770, 0x0000d8b0 },
  { "MSS_UIM0_UART_DM",                            0x00190000, 0x00000200 },
  { "MSS_UIM1_UART_DM",                            0x00198000, 0x00000200 },
  { "MSS_CXM_UART_DM",                             0x001a0000, 0x00000204 },
  { "MSS_CONF_BUS_TIMEOUT",                        0x001b0000, 0x00001000 },
  { "MSS_TXDAC_COMP",                              0x001b2000, 0x00001000 },
  { "MODEM_DTR_DAC_CALIB_0",                       0x001b2000, 0x00000400 },
  { "DAC_REGARRAY_0",                              0x001b2400, 0x00000400 },
  { "MSS_MGPI",                                    0x001b3000, 0x00000128 },
  { "MSS_CRYPTO_TOP",                              0x001c0000, 0x00040000 },
  { "MSS_CRYPTO",                                  0x001fa000, 0x00006000 },
  { "MSS_CRYPTO_BAM",                              0x001c4000, 0x00024000 },
  { "MSS_NAV_SS",                                  0x00200000, 0x000f888d },
  { "MSS_NAV_BASE_REGS",                           0x00200000, 0x000f888d },
  { "MSS_NAV_WTR_BASE",                            0x00200000, 0x00001900 },
  { "MSS_NAV_ADC_BASE",                            0x00201900, 0x00000100 },
  { "MSS_NAV_CC_NAV_CC_REG_BASE",                  0x00201a00, 0x00000200 },
  { "MSS_NAV_BB_BASE",                             0x00202000, 0x000c6000 },
  { "MODEM_TOP",                                   0x00300000, 0x000b8000 },
  { "TDEC_TOP",                                    0x003b4000, 0x00004000 },
  { "TDEC",                                        0x003b4000, 0x000000a8 },
  { "TD_CFG_TRIF",                                 0x003b5000, 0x00000a00 },
  { "TD_TRIF",                                     0x003b6000, 0x00000900 },
  { "TDECIB_MEM",                                  0x003b7000, 0x00001000 },
  { "TX_TOP",                                      0x003a0000, 0x00010000 },
  { "TX",                                          0x003a0000, 0x00001000 },
  { "TX_UNIFIED_TOP",                              0x003a1000, 0x00001000 },
  { "TX_UNIFIED",                                  0x003a1000, 0x00000800 },
  { "TX_UNIFIED_MEM",                              0x003a1800, 0x00000800 },
  { "TX_MEM",                                      0x003a2000, 0x00001000 },
  { "TX_BRDG",                                     0x003a3000, 0x00000050 },
  { "TXR_A0",                                      0x003a5000, 0x00001000 },
  { "TXC_A0",                                      0x003a4000, 0x00001000 },
  { "TXC_MEM",                                     0x003a8000, 0x00001800 },
  { "O_TX",                                        0x003aa000, 0x00000200 },
  { "O_TX_WMORE_TS_TRIF",                          0x003aa200, 0x00000400 },
  { "O_TX_ENC_TS_TRIF",                            0x003aa600, 0x00000200 },
  { "O_TX_MOD_TS_TRIF",                            0x003aa800, 0x00000700 },
  { "O_TX_CA",                                     0x003ab000, 0x00000200 },
  { "O_TX_WMORE_TS_TRIF_CA",                       0x003ab200, 0x00000400 },
  { "O_TX_ENC_TS_TRIF_CA",                         0x003ab600, 0x00000200 },
  { "O_TX_MOD_TS_TRIF_CA",                         0x003ab800, 0x00000700 },
  { "RXFE",                                        0x00380000, 0x00020000 },
  { "RXFE_TOP_CFG",                                0x00380000, 0x00001000 },
  { "RXFE_ADC_ADC0",                               0x00382000, 0x00000100 },
  { "RXFE_ADC_ADC1",                               0x00382100, 0x00000100 },
  { "RXFE_ADC_ADC2",                               0x00382200, 0x00000100 },
  { "RXFE_ADC_ADC3",                               0x00382300, 0x00000100 },
  { "RXFE_WB_WB0",                                 0x00384000, 0x00001000 },
  { "RXFE_WB_WB1",                                 0x00385000, 0x00001000 },
  { "RXFE_WB_WB2",                                 0x00386000, 0x00001000 },
  { "RXFE_WB_WB3",                                 0x00387000, 0x00001000 },
  { "RXFE_NB_NB0",                                 0x00390000, 0x00001000 },
  { "RXFE_NB_NB1",                                 0x00391000, 0x00001000 },
  { "RXFE_NB_NB2",                                 0x00392000, 0x00001000 },
  { "RXFE_NB_NB3",                                 0x00393000, 0x00001000 },
  { "RXFE_NB_NB4",                                 0x00394000, 0x00001000 },
  { "RXFE_NB_NB5",                                 0x00395000, 0x00001000 },
  { "RXFE_BRDG",                                   0x0039f000, 0x00000100 },
  { "DEMBACK_TOP",                                 0x00340000, 0x00040000 },
  { "DEMBACK_COMMON",                              0x00340000, 0x00000100 },
  { "DEMBACK_BRDG",                                0x00340100, 0x00000100 },
  { "LTE_DEMBACK",                                 0x00341000, 0x00000050 },
  { "UMTS_DEMBACK",                                0x00342000, 0x00000300 },
  { "TDS_DEMBACK",                                 0x00342400, 0x00000100 },
  { "CDMA_DEINT",                                  0x00342600, 0x00000100 },
  { "CDMA_WIDGET",                                 0x00342900, 0x00000100 },
  { "HDR_DEINT",                                   0x00342c00, 0x00000100 },
  { "TBVD_CCH_TRIF",                               0x00343000, 0x00000400 },
  { "DB_BUF",                                      0x00344000, 0x00004000 },
  { "SVD_TBVD",                                    0x00348000, 0x00000300 },
  { "LTE_DEMBACK_SCH_TRIF",                        0x00349000, 0x00000300 },
  { "LTE_DEMBACK_CCH_TRIF",                        0x0034a000, 0x00000c00 },
  { "LTE_REENC_TS_TRIF",                           0x0034b000, 0x00000400 },
  { "W_DBACK_HS_TRIF",                             0x0034c000, 0x00000400 },
  { "W_DBACK_NONHS_TRIF",                          0x0034d000, 0x00000400 },
  { "T_DBACK_TRIF",                                0x0034e000, 0x00000500 },
  { "HDR_DEINT_TS_TRIF",                           0x0034f000, 0x00000300 },
  { "DB_BUF_PAGE",                                 0x0035c000, 0x00004000 },
  { "DECOB",                                       0x00360000, 0x0000ce01 },
  { "MTC_TOP",                                     0x00300000, 0x00040000 },
  { "CCS",                                         0x00300000, 0x00020000 },
  { "PDMEM",                                       0x00300000, 0x00018000 },
  { "CONTROL",                                     0x0031fc00, 0x00000400 },
  { "MTC_CLK",                                     0x00320000, 0x00000400 },
  { "MCDMA",                                       0x00320400, 0x00000400 },
  { "A2",                                          0x00320800, 0x00000400 },
  { "DBG",                                         0x00320c00, 0x00000400 },
  { "MTC_BRDG",                                    0x00321000, 0x00000400 },
  { "A2_MEM",                                      0x00322000, 0x00002000 },
  { "MCDMA_TS_TRIF",                               0x00324000, 0x00000800 },
  { "DBG_TS_TRIF",                                 0x00324c00, 0x00000200 },
  { "STMR_EXPY",                                   0x00325000, 0x00000400 },
  { "ENCRYPT",                                     0x00325400, 0x000000fd },
  { "UNIV_STMR",                                   0x00328800, 0x00000400 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_MSS_L2_CFG[] =
{
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_QDSS_SOC_DBG[] =
{
  { "QDSS_APSS_APSS_APSS_DBG",                     0x01000000, 0x01000000 },
  { "QDSS_APSS_TGU_TGU_QC_TGU_APCLK_CS1EAD12DF",   0x01900000, 0x00001000 },
  { "QDSS_APSS_CTI_1_CTI_1_CSCTI",                 0x018f0000, 0x00001000 },
  { "QDSS_APSS_CTI_0_CTI_0_CSCTI",                 0x018e0000, 0x00001000 },
  { "QDSS_APSS_LLM_TPDA_SILVER_LLM_TPDA_SILVER_TPDA_S1_W32_D2_M32_CS78201FC2", 0x018c0000, 0x00001000 },
  { "QDSS_APSS_LLM_TPDM_SILVER_LLM_TPDM_SILVER_TPDM_ATB32_APCLK_CMB32_CSE533A018", 0x018a0000, 0x00001000 },
  { "QDSS_APSS_LLM_TPDM_SILVER_TPDM_ATB32_APCLK_CMB32_CSE533A018_SUB", 0x018a0280, 0x00000d80 },
  { "QDSS_APSS_LLM_TPDM_SILVER_TPDM_ATB32_APCLK_CMB32_CSE533A018_GPR", 0x018a0000, 0x0000027d },
  { "QDSS_APSS_APSS_APB_APSS_APB_APSS_APB_MDUMP",  0x01880000, 0x00010000 },
  { "QDSS_APSS_APSS_APB_APSS_APB_APSS_APB_DFD",    0x01870000, 0x00010000 },
  { "QDSS_APSS_DL_DL_DRAGONLINK_SLV_CS8254B64",    0x01860000, 0x00003000 },
  { "QDSS_APSS_DL_TPDM0_TPDM0_TPDM_ATB32_DSB64_CSF8B13A8B", 0x01860000, 0x00001000 },
  { "QDSS_APSS_DL_TPDM0_TPDM_ATB32_DSB64_CSF8B13A8B_SUB", 0x01860280, 0x00000d80 },
  { "QDSS_APSS_DL_TPDM0_TPDM_ATB32_DSB64_CSF8B13A8B_GPR", 0x01860000, 0x0000027d },
  { "QDSS_APSS_DL_CTI0_CTI0_QC_CTI_CORE",          0x01861000, 0x00001000 },
  { "QDSS_APSS_DL_TPDA_TPDA_TPDA_S1_W32_D2_M32_CS78201FC2_EMBED", 0x01862000, 0x00001000 },
  { "QDSS_APSS_GNOC_APB_GNOC_APB_GLADIATOR_NOC_APB", 0x01840000, 0x0000dc00 },
  { "QDSS_APSS_DL_ACPM_DL_ACPM_DRAGONLINK_SLV_CS2CE2C3AD", 0x01830000, 0x00003000 },
  { "QDSS_APSS_DL_ACPM_TPDM0_TPDM0_TPDM_ATB32_CMB64_CSE183A91F", 0x01830000, 0x00001000 },
  { "QDSS_APSS_DL_ACPM_TPDM0_TPDM_ATB32_CMB64_CSE183A91F_SUB", 0x01830280, 0x00000d80 },
  { "QDSS_APSS_DL_ACPM_TPDM0_TPDM_ATB32_CMB64_CSE183A91F_GPR", 0x01830000, 0x0000027d },
  { "QDSS_APSS_DL_ACPM_CTI0_CTI0_QC_CTI_CORE",     0x01831000, 0x00001000 },
  { "QDSS_APSS_DL_ACPM_TPDA_TPDA_TPDA_S1_W32_D2_M64_CSE87E6F2_EMBED", 0x01832000, 0x00001000 },
  { "QDSS_APSS_FUN_FUN_CXATBFUNNEL_128W8SP",       0x01810000, 0x00001000 },
  { "QDSS_APSS_FUN_ATB_FUN_ATB_CXATBFUNNEL_128W8SP", 0x01800000, 0x00001000 },
  { "QDSS_APSS_APSS_GOLD_APB_APSS_GOLD_APB_A53_APB", 0x01400000, 0x00341000 },
  { "QDSS_APSS_APSS_SILVER_APB_APSS_SILVER_APB_A53_APB", 0x01000000, 0x00341000 },
  { "QDSS_DLCT_DLCT_DRAGONLINK_SLV_POIPU_TILE_CENTER_ASYNC_CTM", 0x00b58000, 0x00005000 },
  { "QDSS_DLCT_TPDM0_TPDM0_TPDM_ATB32_DSB128_1",   0x00b58000, 0x00001000 },
  { "QDSS_DLCT_TPDM0_TPDM_ATB32_DSB128_1_SUB",     0x00b58280, 0x00000d80 },
  { "QDSS_DLCT_TPDM0_TPDM_ATB32_DSB128_1_GPR",     0x00b58000, 0x0000027d },
  { "QDSS_DLCT_CTI0_CTI0_QC_CTI_CORE",             0x00b59000, 0x00001000 },
  { "QDSS_DLCT_CTI1_CTI1_QC_CTI_CORE",             0x00b5a000, 0x00001000 },
  { "QDSS_DLCT_CTI2_CTI2_QC_CTI_CORE",             0x00b5b000, 0x00001000 },
  { "QDSS_DLCT_CTI3_CTI3_QC_CTI_CORE",             0x00b5c000, 0x00001000 },
  { "QDSS_RPM_M3_CTI_RPM_M3_CTI_CSCTI",            0x00b30000, 0x00001000 },
  { "QDSS_TPDMWT_TPDMWT_TPDM_ATB32_APCLK_DSB128_CSD9C89B96", 0x00a58000, 0x00001000 },
  { "QDSS_TPDMWT_TPDM_ATB32_APCLK_DSB128_CSD9C89B96_SUB", 0x00a58280, 0x00000d80 },
  { "QDSS_TPDMWT_TPDM_ATB32_APCLK_DSB128_CSD9C89B96_GPR", 0x00a58000, 0x0000027d },
  { "QDSS_DDRSS_DDRSS_DDR_SS_APB",                 0x00a48000, 0x00008000 },
  { "QDSS_DDRSS_CH0_TGU0_CH0_TGU0_QC_TGU_APCLK_TC_CS734CAB6E", 0x00a48000, 0x00001000 },
  { "QDSS_DDRSS_CH0_TGU1_CH0_TGU1_QC_TGU_APCLK_TC_CS734CAB6E", 0x00a49000, 0x00001000 },
  { "QDSS_DDRSS_CH0_TGU2_CH0_TGU2_QC_TGU_APCLK_TC_CSFA82A8A7", 0x00a4a000, 0x00001000 },
  { "QDSS_LPASS_LPI_LPASS_LPI_LPASS_LPI_APB_DBG",  0x00a20000, 0x00020000 },
  { "QDSS_LPASS_LPI_TRACE32_DBG",                  0x00a20000, 0x00001000 },
  { "QDSS_LPASS_LPI_CTI_1_CTI_1_CSCTI",            0x00a21000, 0x00001000 },
  { "QDSS_LPASS_LPI_REPLICATOR_REPLICATOR_CXATBREPLICATOR_64WP", 0x00a22000, 0x00001000 },
  { "QDSS_LPASS_LPI_STM_STM_CXSTM500_8_32_32_TRUE", 0x00a23000, 0x00001000 },
  { "QDSS_LPASS_LPI_FUN0_FUN0_CXATBFUNNEL_64W8SP", 0x00a24000, 0x00001000 },
  { "QDSS_LPASS_LPI_LPASS_ETFETB_LPASS_ETFETB_CXTMC_F64W32K", 0x00a25000, 0x00001000 },
  { "QDSS_LPASS_LPI_DL_DL_DRAGONLINK_SLV_LPASS_LPI", 0x00a26000, 0x00002000 },
  { "QDSS_LPASS_LPI_DL_TPDM0_TPDM0_TPDM_ATB64_APCLK_DSB256", 0x00a26000, 0x00001000 },
  { "QDSS_LPASS_LPI_DL_TPDM0_TPDM_ATB64_APCLK_DSB256_SUB", 0x00a26280, 0x00000d80 },
  { "QDSS_LPASS_LPI_DL_TPDM0_TPDM_ATB64_APCLK_DSB256_GPR", 0x00a26000, 0x0000027d },
  { "QDSS_LPASS_LPI_DL_TPDA_TPDA_TPDA_S1_W64_D8_M64_CS3BABB64A_EMBED", 0x00a27000, 0x00001000 },
  { "QDSS_LPASS_LPI_QDSP6_QDSP6_QDSP6_DBG",        0x00a28000, 0x00004000 },
  { "QDSS_LPASS_LPI_QDSP6_CTI_0_CTI_0_CSCTI",      0x00a2b000, 0x00001000 },
  { "QDSS_LPASS_LPI_LPASS_APB2AHB_REMAPPER0_DBG",  0x00a30000, 0x00004000 },
  { "QDSS_LPASS_LPI_LPASS_APB2AHB_REMAPPER1_DBG",  0x00a34000, 0x00004000 },
  { "QDSS_LPASS_LPI_LPASS_APB2AHB_REMAPPER2_DBG",  0x00a38000, 0x00004000 },
  { "QDSS_LPASS_LPI_LPASS_APB2AHB_REMAPPER3_DBG",  0x00a3c000, 0x00004000 },
  { "QDSS_AODBG_AODBG_AODBG",                      0x00a00000, 0x00010000 },
  { "QDSS_AODBG_TPDM_TPDM_TPDM_ATB32_CMB64_DSB128", 0x00a01000, 0x00001000 },
  { "QDSS_AODBG_TPDM_TPDM_ATB32_CMB64_DSB128_SUB", 0x00a01280, 0x00000d80 },
  { "QDSS_AODBG_TPDM_TPDM_ATB32_CMB64_DSB128_GPR", 0x00a01000, 0x0000027d },
  { "QDSS_AODBG_SWAO_CTI_QC_CTI_CORE",             0x00a02000, 0x00001000 },
  { "QDSS_AODBG_SWAOCSR_SWAOCSR_SWAOCSR",          0x00a03000, 0x00001000 },
  { "QDSS_AODBG_TPDA_TPDA_TPDA_S1_W32_D2_M32_CS78201FC2", 0x00a04000, 0x00001000 },
  { "QDSS_AODBG_PMIC_ARB_PMIC_ARB_PMIC_ARB_DEBUG", 0x00a08000, 0x0000005d },
  { "QDSS_QM_QM_TPDM_ATB32_APCLK_GPRCLK_BC32_DSB128_CSFE693679", 0x009d0000, 0x00001000 },
  { "QDSS_QM_TPDM_ATB32_APCLK_GPRCLK_BC32_DSB128_CSFE693679_SUB", 0x009d0280, 0x00000d80 },
  { "QDSS_QM_TPDM_ATB32_APCLK_GPRCLK_BC32_DSB128_CSFE693679_GPR", 0x009d0000, 0x0000027d },
  { "QDSS_WCSS_WCSS_WCSS_DBG",                     0x00998000, 0x00020000 },
  { "QDSS_WCSS_ROM_ROM_WCSS_DBG_DAPROM",           0x00998000, 0x00001000 },
  { "QDSS_WCSS_CSR_CSR_WCSS_DBG_CSR",              0x00999000, 0x00001000 },
  { "QDSS_WCSS_TSGEN_TSGEN_CXTSGEN",               0x0099a000, 0x00001000 },
  { "QDSS_WCSS_TPDM_TPDM_TPDM_ATB64_GPRCLK_CMB40_DSB256_CSC0E9EC9D", 0x0099c000, 0x00001000 },
  { "QDSS_WCSS_TPDM_TPDM_ATB64_GPRCLK_CMB40_DSB256_CSC0E9EC9D_SUB", 0x0099c280, 0x00000d80 },
  { "QDSS_WCSS_TPDM_TPDM_ATB64_GPRCLK_CMB40_DSB256_CSC0E9EC9D_GPR", 0x0099c000, 0x0000027d },
  { "QDSS_WCSS_TPDA_TPDA_TPDA_S1_W128_D8_M64_CS5CD79236", 0x0099d000, 0x00001000 },
  { "QDSS_WCSS_FUN_FUN_CXATBFUNNEL_128W2SP",       0x0099e000, 0x00001000 },
  { "QDSS_WCSS_ETFETB_ETFETB_CXTMC_F128W8K",       0x0099f000, 0x00001000 },
  { "QDSS_WCSS_CTI0_CTI0_CSCTI",                   0x009a4000, 0x00001000 },
  { "QDSS_WCSS_CTI1_CTI1_CSCTI",                   0x009a5000, 0x00001000 },
  { "QDSS_WCSS_CTI2_CTI2_CSCTI",                   0x009a6000, 0x00001000 },
  { "QDSS_WCSS_GPR_GPR_CXGPR_8M",                  0x009b6000, 0x00001000 },
  { "QDSS_WCSS_DSBMUX_DSBMUX_WCSS_DBG_DSBMUX",     0x009b7000, 0x00001000 },
  { "QDSS_A6X_A6X_GFX_DBG",                        0x00900000, 0x00080000 },
  { "QDSS_A6X_SP0_ISDB_SP0_ISDB_GFX_ISDB_0",       0x00900000, 0x00001000 },
  { "QDSS_A6X_HLSQ_ISDB_HLSQ_ISDB_ISDB_HLSQ_REG_DB", 0x00910000, 0x00001000 },
  { "QDSS_A6X_TPDM_TPDM_TPDM_ATB32_APCLK_DSB32_CS6666B348", 0x00940000, 0x00001000 },
  { "QDSS_A6X_TPDM_TPDM_ATB32_APCLK_DSB32_CS6666B348_SUB", 0x00940280, 0x00000d80 },
  { "QDSS_A6X_TPDM_TPDM_ATB32_APCLK_DSB32_CS6666B348_GPR", 0x00940000, 0x0000027d },
  { "QDSS_A6X_ISDB_CTI_ISDB_CTI_CSCTI",            0x00941000, 0x00001000 },
  { "QDSS_A6X_CX_DBGC_ATBF_CX_DBGC_ATBF_CXATBFUNNEL_128W4SP", 0x00943000, 0x00001000 },
  { "QDSS_A6X_CX_DRGN_ATBF_CX_DRGN_ATBF_QATBFUNNEL_32W8SP", 0x00944000, 0x00001000 },
  { "QDSS_DCC_DCC_QC_DCC_KAMORTA_12KB4LL_APB",     0x00870000, 0x00001000 },
  { "QDSS_DCC_DCC_TPDM_DCC_TPDM_TPDM_ATB8_ATCLK_CMB32_CS6DCF0636", 0x00870000, 0x00001000 },
  { "QDSS_DCC_DCC_TPDM_TPDM_ATB8_ATCLK_CMB32_CS6DCF0636_SUB", 0x00870280, 0x00000d80 },
  { "QDSS_DCC_DCC_TPDM_TPDM_ATB8_ATCLK_CMB32_CS6DCF0636_GPR", 0x00870000, 0x0000027d },
  { "QDSS_TURING_APB_TURING_APB_TURING_APB",       0x00860000, 0x00008000 },
  { "QDSS_TURING_APB_TURING_TURING_DRAGONLINK_SLV_CS4C56784E", 0x00860000, 0x00002000 },
  { "QDSS_TURING_APB_TURING_TPDM0_TPDM0_TPDM_ATB32_DSB32_CS3493730B", 0x00860000, 0x00001000 },
  { "QDSS_TURING_APB_TURING_TPDM0_TPDM_ATB32_DSB32_CS3493730B_SUB", 0x00860280, 0x00000d80 },
  { "QDSS_TURING_APB_TURING_TPDM0_TPDM_ATB32_DSB32_CS3493730B_GPR", 0x00860000, 0x0000027d },
  { "QDSS_TURING_APB_TURING_QATBFUNNEL_QATBFUNNEL_QATBFUNNEL_64W8SP", 0x00861000, 0x00001000 },
  { "QDSS_TURING_APB_TURING_CTI_TURING_CTI_CSCTI", 0x00867000, 0x00001000 },
  { "QDSS_APB2JTAG_APB2JTAG_APB2JTAG",             0x00858000, 0x00001000 },
  { "QDSS_PIMEM_PIMEM_RAMBLUR_PIMEM_APB",          0x00850000, 0x00001000 },
  { "QDSS_PIMEM_PIMEM_TPDM_PIMEM_TPDM_TPDM_ATB64_APCLK_GPRCLK_BC8_TC2_CMB64_DSB64_CSAF4CCF0", 0x00850000, 0x00001000 },
  { "QDSS_PIMEM_PIMEM_TPDM_TPDM_ATB64_APCLK_GPRCLK_BC8_TC2_CMB64_DSB64_CSAF4CCF0_SUB", 0x00850280, 0x00000d80 },
  { "QDSS_PIMEM_PIMEM_TPDM_TPDM_ATB64_APCLK_GPRCLK_BC8_TC2_CMB64_DSB64_CSAF4CCF0_GPR", 0x00850000, 0x0000027d },
  { "QDSS_PRNG_PRNG_PRNG_DBG",                     0x0084c000, 0x00004000 },
  { "QDSS_PRNG_PRNG_TPDM_PRNG_TPDM_TPDM_ATB32_APCLK_CMB32_CSA14715CD", 0x0084c000, 0x00001000 },
  { "QDSS_PRNG_PRNG_TPDM_TPDM_ATB32_APCLK_CMB32_CSA14715CD_SUB", 0x0084c280, 0x00000d80 },
  { "QDSS_PRNG_PRNG_TPDM_TPDM_ATB32_APCLK_CMB32_CSA14715CD_GPR", 0x0084c000, 0x0000027d },
  { "QDSS_VSENSE_VSENSE_VSENSE_CONTROLLER_DBG",    0x00840000, 0x00001000 },
  { "QDSS_VSENSE_TPDM_ATB8_ATCLK_CMB32_CS6DCF0636", 0x00840000, 0x00001000 },
  { "QDSS_VSENSE_TPDM_ATB8_ATCLK_CMB32_CS6DCF0636_SUB", 0x00840280, 0x00000d80 },
  { "QDSS_VSENSE_TPDM_ATB8_ATCLK_CMB32_CS6DCF0636_GPR", 0x00840000, 0x0000027d },
  { "QDSS_MSS_MSS_QDSP6V67SS_MSS_APB",             0x00830000, 0x00008000 },
  { "QDSS_MSS_Q6V67W_ETM",                         0x00830000, 0x00000800 },
  { "QDSS_MSS_QDSP6SS_QDSP6SS_CSCTI",              0x00833000, 0x00001000 },
  { "QDSS_QDSS",                                   0x00000000, 0x00800000 },
  { "QDSS_QDSS_QDSS_APB",                          0x00000000, 0x00080000 },
  { "QDSS_QDSS_QDSS_CSR",                          0x00001000, 0x00001000 },
  { "QDSS_QDSS_CXSTM_8_32_32_TRUE",                0x00002000, 0x00001000 },
  { "QDSS_QDSS_QDSS_DL_SLV_QDSS_DL_SLV_DRAGONLINK_SLV_KAMORTA_QDSS", 0x00004000, 0x00002000 },
  { "QDSS_QDSS_QDSS_DL_SLV_TPDA_TPDA_TPDA_S16_W64_D8_M64_EMBED", 0x00004000, 0x00001000 },
  { "QDSS_QDSS_QDSS_DL_SLV_QATBFUNNEL_QATBFUNNEL_QATBFUNNEL_64W8SP", 0x00005000, 0x00001000 },
  { "QDSS_QDSS_CTI0_CTI0_CSCTI",                   0x00010000, 0x00001000 },
  { "QDSS_QDSS_CTI1_CTI1_CSCTI",                   0x00011000, 0x00001000 },
  { "QDSS_QDSS_CTI2_CTI2_CSCTI",                   0x00012000, 0x00001000 },
  { "QDSS_QDSS_CTI3_CTI3_CSCTI",                   0x00013000, 0x00001000 },
  { "QDSS_QDSS_CTI4_CTI4_CSCTI",                   0x00014000, 0x00001000 },
  { "QDSS_QDSS_CTI5_CTI5_CSCTI",                   0x00015000, 0x00001000 },
  { "QDSS_QDSS_CTI6_CTI6_CSCTI",                   0x00016000, 0x00001000 },
  { "QDSS_QDSS_CTI7_CTI7_CSCTI",                   0x00017000, 0x00001000 },
  { "QDSS_QDSS_CTI8_CTI8_CSCTI",                   0x00018000, 0x00001000 },
  { "QDSS_QDSS_CTI9_CTI9_CSCTI",                   0x00019000, 0x00001000 },
  { "QDSS_QDSS_CTI10_CTI10_CSCTI",                 0x0001a000, 0x00001000 },
  { "QDSS_QDSS_CTI11_CTI11_CSCTI",                 0x0001b000, 0x00001000 },
  { "QDSS_QDSS_CTI12_CTI12_CSCTI",                 0x0001c000, 0x00001000 },
  { "QDSS_QDSS_CTI13_CTI13_CSCTI",                 0x0001d000, 0x00001000 },
  { "QDSS_QDSS_CTI14_CTI14_CSCTI",                 0x0001e000, 0x00001000 },
  { "QDSS_QDSS_CTI15_CTI15_CSCTI",                 0x0001f000, 0x00001000 },
  { "QDSS_QDSS_CSTPIU_CSTPIU_CSTPIU",              0x00040000, 0x00001000 },
  { "QDSS_QDSS_IN_FUN0_IN_FUN0_CXATBFUNNEL_128W8SP", 0x00041000, 0x00001000 },
  { "QDSS_QDSS_REPL64_REPL64_CXATBREPLICATOR_64WP", 0x00046000, 0x00001000 },
  { "QDSS_QDSS_ETR_ETR_CXTMC_R64W32D",             0x00048000, 0x00001000 },
  { "QDSS_QDSS_IN_FUN1_IN_FUN1_CXATBFUNNEL_128W8SP", 0x00042000, 0x00001000 },
  { "QDSS_QDSS_MERG_FUN_MERG_FUN_CXATBFUNNEL_128W2SP", 0x00045000, 0x00001000 },
  { "QDSS_QDSS_ETFETB_ETFETB_CXTMC_F128W16K",      0x00047000, 0x00001000 },
  { "QDSS_QDSS_VMIDETR_VMIDETR_VMIDMT_IDX_2_SSD1", 0x00049000, 0x00001000 },
  { "QDSS_QDSS_NDPBAM_NDPBAM_BAM_NDP_TOP_AUTO_SCALE_V2_0_XPU3", 0x00060000, 0x00019000 },
  { "QDSS_QDSS_NDPBAM_BAM",                        0x00064000, 0x00015000 },
  { "QDSS_QDSS_QDSS_AHB",                          0x00080000, 0x00009000 },
  { "QDSS_QDSS_VMIDDAP_VMIDDAP_VMIDMT_IDX_2_SSD1", 0x00088000, 0x00001000 },
  { "QDSS_QDSS_SPDM_SPDM_SPDM_WRAPPER_TOP",        0x00080000, 0x00008000 },
  { "QDSS_QDSS_SPDM_SPDM_SPDM_SPDM_CREG",          0x00080000, 0x00000120 },
  { "QDSS_QDSS_SPDM_SPDM_SPDM_SPDM_OLEM",          0x00081000, 0x0000015c },
  { "QDSS_QDSS_SPDM_SPDM_SPDM_SPDM_RTEM",          0x00082000, 0x00000318 },
  { "QDSS_QDSS_SPDM_SPDM_SPDM_SPDM_SREG",          0x00084000, 0x00000120 },
  { "QDSS_QDSS_SPDM_SPDM_SPDM_APU32Q2N7S1V0_1_CL36", 0x00086000, 0x00001080 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_SNOC_MSSNAV_MS_MPU_CFG[] =
{
  { "SNOC_MSS_NAV_MS_MPU_SNOC_MSSNAV_MS_MPU_CFG",  0x00000000, 0x00001400 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_SNOC_A2NOC_MS_MPU_CFG[] =
{
  { "SNOC_AGGRE_MS_MPU_SNOC_A2NOC_MS_MPU_CFG",     0x00000000, 0x00001800 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_SNOC_BOOTIMEM_SS_MPU_CFG[] =
{
  { "SNOC_BOOTIMEM_MS_MPU_SNOC_BOOTIMEM_SS_MPU_CFG", 0x00000000, 0x00001200 },
  { NULL, 0, 0 }
};

static HWIOModuleType HWIOModules_WCSS_WRAPPER[] =
{
  { "WCSS_ECAHB",                                  0x00000000, 0x00000400 },
  { "WCSS_ECAHB_TSLV",                             0x00010000, 0x00001000 },
  { "WCSS_SR",                                     0x00020000, 0x00020000 },
  { "WCSS_HM",                                     0x00080000, 0x00780000 },
  { "WCSS_HM_A_PMM_PMM_DEC",                       0x00080000, 0x0000030c },
  { "WCSS_HM_A_WCSS_WAHB_AHB_SS_WFSS_AHB_SS_DEC",  0x00100000, 0x00000400 },
  { "WCSS_HM_A_WCSS_WAHB_AHB_TSLV_WFSS_WAHB_AHB_TSLV", 0x00101000, 0x00001000 },
  { "WCSS_HM_A_NOC_CFG_WIFI_NOC",                  0x00180000, 0x00004080 },
  { "WCSS_HM_A_WCSS_CLK_CTL_WCSS_CC_REG_DEC",      0x001d0000, 0x00010000 },
  { "WCSS_HM_A_WIFI_APB_1",                        0x00200000, 0x00080000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_WMAC_TOP_REG_T0", 0x00200000, 0x00020000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_PDG_REG",  0x00210000, 0x00000400 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_TXDMA_REG", 0x00210400, 0x00000400 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_RXDMA_REG", 0x00210800, 0x00000800 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_MCMN_REG", 0x00211000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_RXPCU_REG", 0x00212000, 0x00004000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_TXPCU_REG", 0x00216000, 0x00004000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_AMPI_REG", 0x0021a000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_RXOLE_REG", 0x0021b000, 0x00000200 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_A_RXOLE_PARSER_MAC_RXOLE_PARSER_REG", 0x0021b200, 0x00000400 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_CCE_REG",  0x0021b600, 0x00000a00 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_TXOLE_REG", 0x0021c000, 0x00000400 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_A_TXOLE_PARSER_MAC_TXOLE_PARSER_REG", 0x0021c400, 0x00000c00 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_RRI_REG",  0x0021d000, 0x00002000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_CRYPTO_REG", 0x0021f000, 0x00000400 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_HWSCH_REG", 0x0021f400, 0x00000800 },
  { "WCSS_HM_A_WIFI_APB_1_A_WMAC_T0_MAC_MXI_REG",  0x0021fc00, 0x00000400 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE0_WFSS_CE_0_REG", 0x00240000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE1_WFSS_CE_1_REG", 0x00241000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE2_WFSS_CE_2_REG", 0x00242000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE3_WFSS_CE_3_REG", 0x00243000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE4_WFSS_CE_4_REG", 0x00244000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE5_WFSS_CE_5_REG", 0x00245000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE6_WFSS_CE_6_REG", 0x00246000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE7_WFSS_CE_7_REG", 0x00247000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE8_WFSS_CE_8_REG", 0x00248000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE9_WFSS_CE_9_REG", 0x00249000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE10_WFSS_CE_10_REG", 0x0024a000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE11_WFSS_CE_11_REG", 0x0024b000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE_COMMON_WRAPPER", 0x0024c000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_WFSS_CE_SECURE_WRAPPER", 0x0024d000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_MAC_CMN_PARSER_WFSS_CMN_PARSER_REG", 0x0024e000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_MAC_TRC_MAC_TRC_REG",  0x00250000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_1_A_CXC_TOP_CXC_TOP_REG",  0x00260000, 0x00002000 },
  { "WCSS_HM_A_WIFI_APB_1_A_CXC_TOP_CXC_BMH_REG",  0x00260000, 0x00000400 },
  { "WCSS_HM_A_WIFI_APB_1_A_CXC_TOP_CXC_LCMH_REG", 0x00260400, 0x00000400 },
  { "WCSS_HM_A_WIFI_APB_1_A_CXC_TOP_CXC_MCIBASIC_REG", 0x00260800, 0x00000200 },
  { "WCSS_HM_A_WIFI_APB_1_A_CXC_TOP_CXC_LMH_REG",  0x00260a00, 0x00000200 },
  { "WCSS_HM_A_WIFI_APB_1_A_CXC_TOP_CXC_SMH_REG",  0x00260c00, 0x00000200 },
  { "WCSS_HM_A_WIFI_APB_1_A_CXC_TOP_CXC_PMH_REG",  0x00260e00, 0x00000200 },
  { "WCSS_HM_A_WIFI_APB_1_A_WCSS_WAHB_APB_TSLV_WFSS_WAHB_APB_TSLV", 0x00272000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_2",                        0x002c0000, 0x00003000 },
  { "WCSS_HM_A_WIFI_APB_2_A_RFACTRL_CH0_RFACTRL_CH0_REG", 0x002c0000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_2_A_BBANA_RFACTRL_BBANA_RFACTRL_REG", 0x002c2000, 0x00001000 },
  { "WCSS_HM_WLANDAC_1",                           0x002c3000, 0x00000100 },
  { "WCSS_HM_A_WCSS_DBG",                          0x002d0000, 0x00020000 },
  { "WCSS_HM_A_WCSS_DBG_ROM_ROM_WCSS_DBG_DAPROM",  0x002d0000, 0x00001000 },
  { "WCSS_HM_A_WCSS_DBG_CSR_CSR_WCSS_DBG_CSR",     0x002d1000, 0x00001000 },
  { "WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN",      0x002d2000, 0x00001000 },
  { "WCSS_HM_A_WCSS_DBG_TPDM_TPDM_TPDM_ATB64_GPRCLK_CMB40_DSB256_CSC0E9EC9D", 0x002d4000, 0x00001000 },
  { "WCSS_HM_A_WCSS_DBG_TPDM_TPDM_ATB64_GPRCLK_CMB40_DSB256_CSC0E9EC9D_SUB", 0x002d4280, 0x00000d80 },
  { "WCSS_HM_A_WCSS_DBG_TPDM_TPDM_ATB64_GPRCLK_CMB40_DSB256_CSC0E9EC9D_GPR", 0x002d4000, 0x0000027d },
  { "WCSS_HM_A_WCSS_DBG_TPDA_TPDA_TPDA_S1_W128_D8_M64_CS5CD79236", 0x002d5000, 0x00001000 },
  { "WCSS_HM_A_WCSS_DBG_FUN_FUN_CXATBFUNNEL_128W2SP", 0x002d6000, 0x00001000 },
  { "WCSS_HM_A_WCSS_DBG_ETFETB_ETFETB_CXTMC_F128W8K", 0x002d7000, 0x00001000 },
  { "WCSS_HM_A_WCSS_DBG_CTI0_CTI0_CSCTI",          0x002dc000, 0x00001000 },
  { "WCSS_HM_A_WCSS_DBG_CTI1_CTI1_CSCTI",          0x002dd000, 0x00001000 },
  { "WCSS_HM_A_WCSS_DBG_CTI2_CTI2_CSCTI",          0x002de000, 0x00001000 },
  { "WCSS_HM_A_WCSS_DBG_GPR_GPR_CXGPR_8M",         0x002ee000, 0x00001000 },
  { "WCSS_HM_A_WCSS_DBG_DSBMUX_DSBMUX_WCSS_DBG_DSBMUX", 0x002ef000, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_3",                        0x002f0000, 0x00110000 },
  { "WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG",    0x002f0000, 0x00004000 },
  { "WCSS_HM_A_WIFI_APB_3_A_WFAC_T0_BB_REG_MAP_T0", 0x00300000, 0x00060300 },
  { "WCSS_HM_A_WIFI_APB_3_A_WFAC_T0_RXB_REG_MAP",  0x00300000, 0x00000048 },
  { "WCSS_HM_A_WIFI_APB_3_A_WFAC_T0_AGC_REG_MAP",  0x00300100, 0x00000900 },
  { "WCSS_HM_A_WIFI_APB_3_A_WFAC_T0_RXFE_REG_MAP", 0x00301900, 0x00000c00 },
  { "WCSS_HM_A_WIFI_APB_3_A_WFAC_T0_TXTD_REG_MAP", 0x00302500, 0x00002000 },
  { "WCSS_HM_A_WIFI_APB_3_A_WFAC_T0_TPC_REG_MAP",  0x00304500, 0x00001000 },
  { "WCSS_HM_A_WIFI_APB_3_A_WFAC_T0_RFCNTL_REG_MAP", 0x00305500, 0x00000e00 },
  { "WCSS_HM_A_WIFI_APB_3_A_WFAC_T0_RSFD_REG_MAP", 0x00340500, 0x00001400 },
  { "WCSS_HM_A_WIFI_APB_3_A_WFAC_T0_SM_REG_MAP",   0x00346300, 0x00000200 },
  { "WCSS_HM_A_WIFI_APB_3_A_WFAC_T0_TXBE_REG_MAP", 0x00346500, 0x00000800 },
  { "WCSS_HM_A_WIFI_APB_3_A_WFAC_T0_SVD_REG_MAP",  0x0035e000, 0x00002000 },
  { "WCSS_HM_A_WIFI_APB_3_A_WFAC_T0_DEC_REG_MAP",  0x00360100, 0x00000200 },
  { NULL, 0, 0 }
};

HWIOPhysRegionType HWIOBaseMap[] =
{
  {
    "BOOT_ROM",
    (DALSYSMemAddr)0x00100000,
    0x00100000,
    (DALSYSMemAddr)0xa0000000,
    HWIOModules_BOOT_ROM
  },
  {
    "CORE_TOP_CSR",
    (DALSYSMemAddr)0x00300000,
    0x00100000,
    (DALSYSMemAddr)0xa0100000,
    HWIOModules_CORE_TOP_CSR
  },
  {
    "TLMM",
    (DALSYSMemAddr)0x00400000,
    0x00c00000,
    (DALSYSMemAddr)0xa0200000,
    HWIOModules_TLMM
  },
  {
    "CLK_CTL",
    (DALSYSMemAddr)0x01400000,
    0x00200000,
    (DALSYSMemAddr)0xa0e00000,
    HWIOModules_CLK_CTL
  },
  {
    "QM_MPU_CFG_QM_MPU_WRAPPER",
    (DALSYSMemAddr)0x01874000,
    0x00001200,
    (DALSYSMemAddr)0xa1274000,
    HWIOModules_QM_MPU_CFG_QM_MPU_WRAPPER
  },
  {
    "CNOC_MS_MPU_CFG",
    (DALSYSMemAddr)0x01878000,
    0x00002400,
    (DALSYSMemAddr)0xa1378000,
    HWIOModules_CNOC_MS_MPU_CFG
  },
  {
    "CNOC_SNOC_MS_MPU_CFG",
    (DALSYSMemAddr)0x0187c000,
    0x00001e00,
    (DALSYSMemAddr)0xa147c000,
    HWIOModules_CNOC_SNOC_MS_MPU_CFG
  },
  {
    "MDSP_MPU_CFG_MPU32Q2N7S1V0_16_CL36M35L12_AHB",
    (DALSYSMemAddr)0x0187e000,
    0x00001800,
    (DALSYSMemAddr)0xa157e000,
    HWIOModules_MDSP_MPU_CFG_MPU32Q2N7S1V0_16_CL36M35L12_AHB
  },
  {
    "SYSTEM_NOC",
    (DALSYSMemAddr)0x01880000,
    0x0005d080,
    (DALSYSMemAddr)0xa1680000,
    HWIOModules_SYSTEM_NOC
  },
  {
    "CONFIG_NOC",
    (DALSYSMemAddr)0x01900000,
    0x00009200,
    (DALSYSMemAddr)0xa1700000,
    HWIOModules_CONFIG_NOC
  },
  {
    "PKA_WRAPPER",
    (DALSYSMemAddr)0x01980000,
    0x00071280,
    (DALSYSMemAddr)0xa1880000,
    HWIOModules_PKA_WRAPPER
  },
  {
    "CRYPTO0_CRYPTO_TOP",
    (DALSYSMemAddr)0x01b00000,
    0x00040000,
    (DALSYSMemAddr)0xa1900000,
    HWIOModules_CRYPTO0_CRYPTO_TOP
  },
  {
    "SECURITY_CONTROL",
    (DALSYSMemAddr)0x01b40000,
    0x00010000,
    (DALSYSMemAddr)0xa1a40000,
    HWIOModules_SECURITY_CONTROL
  },
  {
    "PRNG_CFG_PRNG_TOP",
    (DALSYSMemAddr)0x01b50000,
    0x00010000,
    (DALSYSMemAddr)0xa1b50000,
    HWIOModules_PRNG_CFG_PRNG_TOP
  },
  {
    "RAMBLUR_PIMEM",
    (DALSYSMemAddr)0x01b60000,
    0x00008000,
    (DALSYSMemAddr)0xa1c60000,
    HWIOModules_RAMBLUR_PIMEM
  },
  {
    "OCIMEM_WRAPPER_CSR",
    (DALSYSMemAddr)0x01bd0000,
    0x00006000,
    (DALSYSMemAddr)0xa1dd0000,
    HWIOModules_OCIMEM_WRAPPER_CSR
  },
  {
    "QC_DCC_KAMORTA_12KB4LL",
    (DALSYSMemAddr)0x01be0000,
    0x00010000,
    (DALSYSMemAddr)0xa1ee0000,
    HWIOModules_QC_DCC_KAMORTA_12KB4LL
  },
  {
    "PMIC_ARB",
    (DALSYSMemAddr)0x01c00000,
    0x023a0000,
    (DALSYSMemAddr)0xa1f00000,
    HWIOModules_PMIC_ARB
  },
  {
    "MAPSS",
    (DALSYSMemAddr)0x04400000,
    0x00040000,
    (DALSYSMemAddr)0xa4300000,
    HWIOModules_MAPSS
  },
  {
    "HWKM_MASTER_CFG_KEYMANAGER_MASTER_REGS",
    (DALSYSMemAddr)0x04440000,
    0x00020000,
    (DALSYSMemAddr)0xa4440000,
    HWIOModules_HWKM_MASTER_CFG_KEYMANAGER_MASTER_REGS
  },
  {
    "DDR_SS",
    (DALSYSMemAddr)0x0447d000,
    0x000a3000,
    (DALSYSMemAddr)0xa457d000,
    HWIOModules_DDR_SS
  },
  {
    "RPM_SS_MSG_RAM_START_ADDRESS",
    (DALSYSMemAddr)0x045f0000,
    0x00007000,
    (DALSYSMemAddr)0xa47f0000,
    HWIOModules_RPM_SS_MSG_RAM_START_ADDRESS
  },
  {
    "RPM",
    (DALSYSMemAddr)0x04600000,
    0x00100000,
    (DALSYSMemAddr)0xa4800000,
    HWIOModules_RPM
  },
  {
    "PERIPH_SS_SDC1_SDCC5_TOP",
    (DALSYSMemAddr)0x04740000,
    0x00020000,
    (DALSYSMemAddr)0xa4940000,
    HWIOModules_PERIPH_SS_SDC1_SDCC5_TOP
  },
  {
    "UFS_MEM_UFS_REGS",
    (DALSYSMemAddr)0x04800000,
    0x00020000,
    (DALSYSMemAddr)0xa4a00000,
    HWIOModules_UFS_MEM_UFS_REGS
  },
  {
    "QPIC_QPIC_XPU3",
    (DALSYSMemAddr)0x04840000,
    0x00040000,
    (DALSYSMemAddr)0xa4b40000,
    HWIOModules_QPIC_QPIC_XPU3
  },
  {
    "QUPV3_0_QUPV3_ID_3",
    (DALSYSMemAddr)0x04a00000,
    0x000c7000,
    (DALSYSMemAddr)0xa4c00000,
    HWIOModules_QUPV3_0_QUPV3_ID_3
  },
  {
    "IPA_0_IPA_WRAPPER",
    (DALSYSMemAddr)0x05800000,
    0x00100000,
    (DALSYSMemAddr)0xa4d00000,
    HWIOModules_IPA_0_IPA_WRAPPER
  },
  {
    "MSS_TOP",
    (DALSYSMemAddr)0x06000000,
    0x00580000,
    (DALSYSMemAddr)0xec000000,
    HWIOModules_MSS_TOP
  },
  {
    "MSS_L2_CFG",
    (DALSYSMemAddr)0x065a0000,
    0x00001000,
    (DALSYSMemAddr)0xa4ea0000,
    HWIOModules_MSS_L2_CFG
  },
  {
    "QDSS_SOC_DBG",
    (DALSYSMemAddr)0x08000000,
    0x02000000,
    (DALSYSMemAddr)0xa4f00000,
    HWIOModules_QDSS_SOC_DBG
  },
  {
    "SNOC_MSSNAV_MS_MPU_CFG",
    (DALSYSMemAddr)0x0c000000,
    0x00001400,
    (DALSYSMemAddr)0xa6f00000,
    HWIOModules_SNOC_MSSNAV_MS_MPU_CFG
  },
  {
    "SNOC_A2NOC_MS_MPU_CFG",
    (DALSYSMemAddr)0x0c002000,
    0x00001800,
    (DALSYSMemAddr)0xa7002000,
    HWIOModules_SNOC_A2NOC_MS_MPU_CFG
  },
  {
    "SNOC_BOOTIMEM_SS_MPU_CFG",
    (DALSYSMemAddr)0x0c006000,
    0x00001200,
    (DALSYSMemAddr)0xa7106000,
    HWIOModules_SNOC_BOOTIMEM_SS_MPU_CFG
  },
  {
    "WCSS_WRAPPER",
    (DALSYSMemAddr)0x0c800000,
    0x00800000,
    (DALSYSMemAddr)0xa7200000,
    HWIOModules_WCSS_WRAPPER
  },
  { NULL, 0, 0, 0, NULL }
};

