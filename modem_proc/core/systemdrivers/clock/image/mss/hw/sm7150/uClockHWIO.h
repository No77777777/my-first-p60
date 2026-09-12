#ifndef __UCLOCKHWIO_H__
#define __UCLOCKHWIO_H__
/*
===========================================================================
*/
/**
  @file uClockHWIO.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SM7150 (Moorea) [moorea_v1.0_p3q3r71_MTO]
 
  This file contains HWIO register definitions for the following modules:
    MPSS_PERPH
    GCC_CLK_CTL_REG
    MSS_QDSP6V65SS_PUB

  'Include' filters applied: MSS_BUS_CRYPTO_CBCR[MPSS_PERPH] MSS_AXI_CRYPTO_CBCR[MPSS_PERPH] GCC_PRNG_AHB_CBCR[GCC_CLK_CTL_REG] GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE[GCC_CLK_CTL_REG] RCGR[MSS_QDSP6V65SS_PUB] PLL[MSS_QDSP6V65SS_PUB] 
  'Exclude' filters applied: GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_1[GCC_CLK_CTL_REG] PLL_RCG[MSS_QDSP6V65SS_PUB] 

  Generation parameters: 
  { u'filename': u'uClockHWIO.h',
    u'header': u'#include "msmhwiobase.h"',
    u'module-filter-exclude': { u'GCC_CLK_CTL_REG': [ u'GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_1'],
                                u'MSS_QDSP6V65SS_PUB': [u'PLL_RCG']},
    u'module-filter-include': { u'GCC_CLK_CTL_REG': [ u'GCC_PRNG_AHB_CBCR',
                                                      u'GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE'],
                                u'MPSS_PERPH': [ u'MSS_BUS_CRYPTO_CBCR',
                                                 u'MSS_AXI_CRYPTO_CBCR'],
                                u'MSS_QDSP6V65SS_PUB': [u'RCGR', u'PLL']},
    u'modules': [u'MPSS_PERPH', u'GCC_CLK_CTL_REG', u'MSS_QDSP6V65SS_PUB'],
    u'output-offsets': True,
    u'rmsk-input': True}
*/
/*
  ===========================================================================

  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

  ===========================================================================

  $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/sm7150/uClockHWIO.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  ===========================================================================
*/

#include "msmhwiobase.h"

/*----------------------------------------------------------------------------
 * MODULE: MPSS_PERPH
 *--------------------------------------------------------------------------*/

#define MPSS_PERPH_REG_BASE                                                         (MODEM_TOP_BASE      + 0x001a8000)
#define MPSS_PERPH_REG_BASE_SIZE                                                    0x8000
#define MPSS_PERPH_REG_BASE_USED                                                    0x2148
#define MPSS_PERPH_REG_BASE_OFFS                                                    0x001a8000

#define HWIO_MSS_BUS_CRYPTO_CBCR_ADDR                                               (MPSS_PERPH_REG_BASE      + 0x000020c8)
#define HWIO_MSS_BUS_CRYPTO_CBCR_OFFS                                               (MPSS_PERPH_REG_BASE_OFFS + 0x000020c8)
#define HWIO_MSS_BUS_CRYPTO_CBCR_RMSK                                               0x80000001
#define HWIO_MSS_BUS_CRYPTO_CBCR_IN          \
        in_dword_masked(HWIO_MSS_BUS_CRYPTO_CBCR_ADDR, HWIO_MSS_BUS_CRYPTO_CBCR_RMSK)
#define HWIO_MSS_BUS_CRYPTO_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_BUS_CRYPTO_CBCR_ADDR, m)
#define HWIO_MSS_BUS_CRYPTO_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_BUS_CRYPTO_CBCR_ADDR,v)
#define HWIO_MSS_BUS_CRYPTO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_BUS_CRYPTO_CBCR_ADDR,m,v,HWIO_MSS_BUS_CRYPTO_CBCR_IN)
#define HWIO_MSS_BUS_CRYPTO_CBCR_CLKOFF_BMSK                                        0x80000000
#define HWIO_MSS_BUS_CRYPTO_CBCR_CLKOFF_SHFT                                              0x1f
#define HWIO_MSS_BUS_CRYPTO_CBCR_CLKEN_BMSK                                                0x1
#define HWIO_MSS_BUS_CRYPTO_CBCR_CLKEN_SHFT                                                0x0

#define HWIO_MSS_AXI_CRYPTO_CBCR_ADDR                                               (MPSS_PERPH_REG_BASE      + 0x000020cc)
#define HWIO_MSS_AXI_CRYPTO_CBCR_OFFS                                               (MPSS_PERPH_REG_BASE_OFFS + 0x000020cc)
#define HWIO_MSS_AXI_CRYPTO_CBCR_RMSK                                               0x80000001
#define HWIO_MSS_AXI_CRYPTO_CBCR_IN          \
        in_dword_masked(HWIO_MSS_AXI_CRYPTO_CBCR_ADDR, HWIO_MSS_AXI_CRYPTO_CBCR_RMSK)
#define HWIO_MSS_AXI_CRYPTO_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_AXI_CRYPTO_CBCR_ADDR, m)
#define HWIO_MSS_AXI_CRYPTO_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_AXI_CRYPTO_CBCR_ADDR,v)
#define HWIO_MSS_AXI_CRYPTO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_AXI_CRYPTO_CBCR_ADDR,m,v,HWIO_MSS_AXI_CRYPTO_CBCR_IN)
#define HWIO_MSS_AXI_CRYPTO_CBCR_CLKOFF_BMSK                                        0x80000000
#define HWIO_MSS_AXI_CRYPTO_CBCR_CLKOFF_SHFT                                              0x1f
#define HWIO_MSS_AXI_CRYPTO_CBCR_CLKEN_BMSK                                                0x1
#define HWIO_MSS_AXI_CRYPTO_CBCR_CLKEN_SHFT                                                0x0

/*----------------------------------------------------------------------------
 * MODULE: GCC_CLK_CTL_REG
 *--------------------------------------------------------------------------*/

#define GCC_CLK_CTL_REG_REG_BASE                                                                   (CLK_CTL_BASE      + 0x00000000)
#define GCC_CLK_CTL_REG_REG_BASE_SIZE                                                              0x1f0000
#define GCC_CLK_CTL_REG_REG_BASE_USED                                                              0x1820b0
#define GCC_CLK_CTL_REG_REG_BASE_OFFS                                                              0x00000000

#define HWIO_GCC_PRNG_AHB_CBCR_ADDR                                                                (GCC_CLK_CTL_REG_REG_BASE      + 0x00034004)
#define HWIO_GCC_PRNG_AHB_CBCR_OFFS                                                                (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00034004)
#define HWIO_GCC_PRNG_AHB_CBCR_RMSK                                                                0x80100006
#define HWIO_GCC_PRNG_AHB_CBCR_IN          \
        in_dword_masked(HWIO_GCC_PRNG_AHB_CBCR_ADDR, HWIO_GCC_PRNG_AHB_CBCR_RMSK)
#define HWIO_GCC_PRNG_AHB_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_PRNG_AHB_CBCR_ADDR, m)
#define HWIO_GCC_PRNG_AHB_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_PRNG_AHB_CBCR_ADDR,v)
#define HWIO_GCC_PRNG_AHB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_PRNG_AHB_CBCR_ADDR,m,v,HWIO_GCC_PRNG_AHB_CBCR_IN)
#define HWIO_GCC_PRNG_AHB_CBCR_CLK_OFF_BMSK                                                        0x80000000
#define HWIO_GCC_PRNG_AHB_CBCR_CLK_OFF_SHFT                                                              0x1f
#define HWIO_GCC_PRNG_AHB_CBCR_IGNORE_RPMH_CLK_DIS_BMSK                                              0x100000
#define HWIO_GCC_PRNG_AHB_CBCR_IGNORE_RPMH_CLK_DIS_SHFT                                                  0x14
#define HWIO_GCC_PRNG_AHB_CBCR_CLK_ARES_BMSK                                                              0x4
#define HWIO_GCC_PRNG_AHB_CBCR_CLK_ARES_SHFT                                                              0x2
#define HWIO_GCC_PRNG_AHB_CBCR_HW_CTL_BMSK                                                                0x2
#define HWIO_GCC_PRNG_AHB_CBCR_HW_CTL_SHFT                                                                0x1

#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_ADDR                                                 (GCC_CLK_CTL_REG_REG_BASE      + 0x00035004)
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_OFFS                                                 (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00035004)
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_RMSK                                                  0x76fbd7f
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_IN          \
        in_dword_masked(HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_ADDR, HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_RMSK)
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_INM(m)      \
        in_dword_masked(HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_ADDR, m)
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_OUT(v)      \
        out_dword(HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_ADDR,v)
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_ADDR,m,v,HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_IN)
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_NPU_GPLL0_DIV_CLK_SRC_ENA_BMSK                        0x4000000
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_NPU_GPLL0_DIV_CLK_SRC_ENA_SHFT                             0x1a
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_NPU_GPLL0_CLK_SRC_ENA_BMSK                            0x2000000
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_NPU_GPLL0_CLK_SRC_ENA_SHFT                                 0x19
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_IMEM_AXI_CLK_ENA_BMSK                                 0x1000000
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_IMEM_AXI_CLK_ENA_SHFT                                      0x18
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_CPUSS_GNOC_CLK_ENA_BMSK                                0x400000
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_CPUSS_GNOC_CLK_ENA_SHFT                                    0x16
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_CPUSS_AHB_CLK_ENA_BMSK                                 0x200000
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_CPUSS_AHB_CLK_ENA_SHFT                                     0x15
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_DISP_GPLL0_DIV_CLK_SRC_ENA_BMSK                         0x80000
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_DISP_GPLL0_DIV_CLK_SRC_ENA_SHFT                            0x13
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_DISP_GPLL0_CLK_SRC_ENA_BMSK                             0x40000
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_DISP_GPLL0_CLK_SRC_ENA_SHFT                                0x12
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_MSS_GPLL0_DIV_CLK_SRC_ENA_BMSK                          0x20000
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_MSS_GPLL0_DIV_CLK_SRC_ENA_SHFT                             0x11
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_GPU_GPLL0_DIV_CLK_SRC_ENA_BMSK                          0x10000
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_GPU_GPLL0_DIV_CLK_SRC_ENA_SHFT                             0x10
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_GPU_GPLL0_CLK_SRC_ENA_BMSK                               0x8000
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_GPU_GPLL0_CLK_SRC_ENA_SHFT                                  0xf
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_PRNG_AHB_CLK_ENA_BMSK                                    0x2000
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_PRNG_AHB_CLK_ENA_SHFT                                       0xd
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_TLMM_EAST_AHB_CLK_ENA_BMSK                               0x1000
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_TLMM_EAST_AHB_CLK_ENA_SHFT                                  0xc
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_TLMM_SOUTH_AHB_CLK_ENA_BMSK                               0x800
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_TLMM_SOUTH_AHB_CLK_ENA_SHFT                                 0xb
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_BOOT_ROM_AHB_CLK_ENA_BMSK                                 0x400
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_BOOT_ROM_AHB_CLK_ENA_SHFT                                   0xa
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_TLMM_NORTH_AHB_CLK_ENA_BMSK                               0x100
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_TLMM_NORTH_AHB_CLK_ENA_SHFT                                 0x8
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_TLMM_CLK_ENA_BMSK                                          0x40
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_TLMM_CLK_ENA_SHFT                                           0x6
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_CE1_CLK_ENA_BMSK                                           0x20
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_CE1_CLK_ENA_SHFT                                            0x5
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_CE1_AXI_CLK_ENA_BMSK                                       0x10
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_CE1_AXI_CLK_ENA_SHFT                                        0x4
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_CE1_AHB_CLK_ENA_BMSK                                        0x8
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_CE1_AHB_CLK_ENA_SHFT                                        0x3
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_QDSS_CFG_AHB_CLK_ENA_BMSK                                   0x4
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_QDSS_CFG_AHB_CLK_ENA_SHFT                                   0x2
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_TCSR_AHB_CLK_ENA_BMSK                                       0x2
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_TCSR_AHB_CLK_ENA_SHFT                                       0x1
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_SYS_NOC_CPUSS_AHB_CLK_ENA_BMSK                              0x1
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_SYS_NOC_CPUSS_AHB_CLK_ENA_SHFT                              0x0

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6V65SS_PUB
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6V65SS_PUB_REG_BASE                                                 (MODEM_TOP_BASE      + 0x00080000)
#define MSS_QDSP6V65SS_PUB_REG_BASE_SIZE                                            0x4040
#define MSS_QDSP6V65SS_PUB_REG_BASE_USED                                            0x203c
#define MSS_QDSP6V65SS_PUB_REG_BASE_OFFS                                            0x00080000

#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR                                         (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000028)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_OFFS                                         (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000028)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_RMSK                                         0x80000013
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR, HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_IN)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_OFF_BMSK                                0x80000000
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_OFF_SHFT                                      0x1f
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                0x10
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                 0x4
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_EN_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_EN_SHFT                                        0x1
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_UPDATE_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_UPDATE_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR                                         (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x0000002c)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_OFFS                                         (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x0000002c)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RMSK                                           0x11071f
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR, HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_IN)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_HW_CLK_CONTROL_BMSK                            0x100000
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_HW_CLK_CONTROL_SHFT                                0x14
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RCGLITE_DISABLE_BMSK                            0x10000
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RCGLITE_DISABLE_SHFT                               0x10
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_SEL_BMSK                                      0x700
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_SEL_SHFT                                        0x8
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_DIV_BMSK                                       0x1f
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_DIV_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_PLL_MODE_ADDR                                              (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000200)
#define HWIO_MSS_QDSP6SS_PLL_MODE_OFFS                                              (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000200)
#define HWIO_MSS_QDSP6SS_PLL_MODE_RMSK                                              0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_MODE_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_MODE_ADDR, HWIO_MSS_QDSP6SS_PLL_MODE_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_MODE_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_MODE_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_MODE_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_MODE_IN)
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_DET_BMSK                                 0x80000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_DET_SHFT                                       0x1f
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_ACTIVE_FLAG_BMSK                              0x40000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_ACTIVE_FLAG_SHFT                                    0x1e
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_ACK_LATCH_BMSK                                0x20000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_ACK_LATCH_SHFT                                      0x1d
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_DET_FINE_BMSK                            0x10000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_DET_FINE_SHFT                                  0x1c
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BITS27_24_BMSK                             0xf000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BITS27_24_SHFT                                  0x18
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_BMSK                     0x800000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_SHFT                         0x17
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_UPDATE_BMSK                                     0x400000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_UPDATE_SHFT                                         0x16
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_VOTE_FSM_RESET_BMSK                             0x200000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_VOTE_FSM_RESET_SHFT                                 0x15
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_VOTE_FSM_ENA_BMSK                               0x100000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_VOTE_FSM_ENA_SHFT                                   0x14
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_BIAS_COUNT_BMSK                                  0xfc000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_BIAS_COUNT_SHFT                                      0xe
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_COUNT_BMSK                                   0x3f00
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_COUNT_SHFT                                      0x8
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BITS7_3_BMSK                                    0xf8
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BITS7_3_SHFT                                     0x3
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_RESET_N_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_RESET_N_SHFT                                         0x2
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BIT1_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BIT1_SHFT                                        0x1
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_OUTCTRL_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_OUTCTRL_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR                                             (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000204)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_OFFS                                             (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000204)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_RMSK                                                 0xffff
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR, HWIO_MSS_QDSP6SS_PLL_L_VAL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_L_VAL_IN)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_PLL_L_BMSK                                           0xffff
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_PLL_L_SHFT                                              0x0

#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_ADDR                                         (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000208)
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_OFFS                                         (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000208)
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_RMSK                                             0xffff
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_ADDR, HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_IN)
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_PLL_CAL_L_BMSK                                   0xffff
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_PLL_CAL_L_SHFT                                      0x0

#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR                                          (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x0000020c)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OFFS                                          (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x0000020c)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RMSK                                          0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR, HWIO_MSS_QDSP6SS_PLL_USER_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_USER_CTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BITS31_19_BMSK                        0xfff80000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BITS31_19_SHFT                              0x13
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PRE_DIV_RATIO_BMSK                               0x70000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PRE_DIV_RATIO_SHFT                                  0x10
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_ODD_BMSK                           0xf000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_ODD_SHFT                              0xc
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_EVEN_BMSK                           0xf00
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_EVEN_SHFT                             0x8
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUT_CLK_POLARITY_BMSK                               0x80
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUT_CLK_POLARITY_SHFT                                0x7
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BITS6_5_BMSK                                0x60
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BITS6_5_SHFT                                 0x5
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_TEST_BMSK                                    0x10
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_TEST_SHFT                                     0x4
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT3_BMSK                                    0x8
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT3_SHFT                                    0x3
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_ODD_BMSK                                      0x4
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_ODD_SHFT                                      0x2
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_EVEN_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_EVEN_SHFT                                     0x1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_MAIN_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_MAIN_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR                                        (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000210)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_OFFS                                        (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000210)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR, HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_IN)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BITS31_19_BMSK                      0xfff80000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BITS31_19_SHFT                            0x13
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_USE_PREDIV_WHEN_CAL_BMSK                       0x40000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_USE_PREDIV_WHEN_CAL_SHFT                          0x12
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_USE_EXT_SAVERESTORE_BMSK                       0x20000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_USE_EXT_SAVERESTORE_SHFT                          0x11
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_LOW_JITTER_MODE_EN_BMSK                        0x10000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_LOW_JITTER_MODE_EN_SHFT                           0x10
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_FRAC_FORMAT_SEL_BMSK                            0x8000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_FRAC_FORMAT_SEL_SHFT                               0xf
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_BIAS_ON_IN_STANDBY_BMSK                         0x4000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_BIAS_ON_IN_STANDBY_SHFT                            0xe
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_REF_CLK_AT_OUT_BMSK                             0x2000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_REF_CLK_AT_OUT_SHFT                                0xd
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_SCALE_FREQ_ON_RESTART_BMSK                      0x1800
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_SCALE_FREQ_ON_RESTART_SHFT                         0xb
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_LATCH_INTERFACE_BYPASS_BMSK                      0x400
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_LATCH_INTERFACE_BYPASS_SHFT                        0xa
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_STATUS_REGISTER_BMSK                             0x3e0
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_STATUS_REGISTER_SHFT                               0x5
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_WRITE_STATE_EN_BMSK                               0x10
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_WRITE_STATE_EN_SHFT                                0x4
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_CALIB_CTRL_BMSK                                    0xe
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_CALIB_CTRL_SHFT                                    0x1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_USE_ONLY_FINE_LOCK_DET_BMSK                        0x1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_USE_ONLY_FINE_LOCK_DET_SHFT                        0x0

#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR                                        (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000214)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_OFFS                                        (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000214)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR, HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FINE_LDC_THRESHOLD_BMSK                     0xfc000000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FINE_LDC_THRESHOLD_SHFT                           0x1a
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_COARSE_LDC_THRESHOLD_BMSK                    0x3c00000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_COARSE_LDC_THRESHOLD_SHFT                         0x16
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_COARSE_LOCK_DET_NEG_THRESHOLD_BMSK            0x3c0000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_COARSE_LOCK_DET_NEG_THRESHOLD_SHFT                0x12
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_COARSE_LOCK_DET_POS_THRESHOLD_BMSK             0x3c000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_COARSE_LOCK_DET_POS_THRESHOLD_SHFT                 0xe
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FINE_LOCK_DET_THRESHOLD_BMSK                    0x3800
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FINE_LOCK_DET_THRESHOLD_SHFT                       0xb
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FINE_LOCK_DET_SAMPLE_SIZE_BMSK                   0x700
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FINE_LOCK_DET_SAMPLE_SIZE_SHFT                     0x8
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FWD_GAIN_SLEWING_KFN_BMSK                         0xf0
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FWD_GAIN_SLEWING_KFN_SHFT                          0x4
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FWD_GAIN_KFN_BMSK                                  0xf
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FWD_GAIN_KFN_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_ADDR                                      (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000218)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_OFFS                                      (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000218)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_RMSK                                      0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_ADDR, HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_IN)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_BIST_CFG_BMSK                             0xfff00000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_BIST_CFG_SHFT                                   0x14
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_RESERVE_BITS19_12_BMSK                       0xff000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_RESERVE_BITS19_12_SHFT                           0xc
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_NUM_STEPS_FAST_LOCK_BMSK                       0xc00
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_NUM_STEPS_FAST_LOCK_SHFT                         0xa
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_PREDIV_WHEN_CAL_BMSK                           0x380
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_PREDIV_WHEN_CAL_SHFT                             0x7
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_INC_MIN_GLITCH_THRESHOLD_4X_BMSK                0x40
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_INC_MIN_GLITCH_THRESHOLD_4X_SHFT                 0x6
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_MIN_GLITCH_THRESHOLD_BMSK                       0x30
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_MIN_GLITCH_THRESHOLD_SHFT                        0x4
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_USE_BOTH_REF_CLK_EDGE_BMSK                       0x8
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_USE_BOTH_REF_CLK_EDGE_SHFT                       0x3
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_BIAS_WARMUP_TIME_BMSK                            0x6
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_BIAS_WARMUP_TIME_SHFT                            0x1
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_DIV_LOCK_DET_THRESHOLDS_BMSK                     0x1
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_DIV_LOCK_DET_THRESHOLDS_SHFT                     0x0

#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR                                          (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x0000021c)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OFFS                                          (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x0000021c)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_RMSK                                          0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR, HWIO_MSS_QDSP6SS_PLL_TEST_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_TEST_CTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADD_REF_CYC_CALIB_STEPS_BMSK                  0xc0000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADD_REF_CYC_CALIB_STEPS_SHFT                        0x1e
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_GLITCH_DETECTOR_COUNT_LIMIT_BMSK              0x30000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_GLITCH_DETECTOR_COUNT_LIMIT_SHFT                    0x1c
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_GLITCH_PREVENTION_DIS_BMSK                     0x8000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_GLITCH_PREVENTION_DIS_SHFT                          0x1b
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_FINE_FCW_BMSK                                  0x7e00000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_FINE_FCW_SHFT                                       0x15
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OVERRIDE_FINE_FCW_BMSK                          0x100000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OVERRIDE_FINE_FCW_SHFT                              0x14
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_COARSE_FCW_BMSK                                  0xff000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_COARSE_FCW_SHFT                                      0xc
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OVERRIDE_COARSE_FCW_BMSK                           0x800
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OVERRIDE_COARSE_FCW_SHFT                             0xb
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_PROCESS_CALIB_WORD_BMSK                            0x700
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_PROCESS_CALIB_WORD_SHFT                              0x8
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OVERRIDE_CALIB_WORD_BMSK                            0x80
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OVERRIDE_CALIB_WORD_SHFT                             0x7
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DISABLE_LFSR_BMSK                                   0x40
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DISABLE_LFSR_SHFT                                    0x6
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST1_SEL_BMSK                                     0x30
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST1_SEL_SHFT                                      0x4
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST0_SEL_BMSK                                      0xc
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST0_SEL_SHFT                                      0x2
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST1_EN_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST1_EN_SHFT                                       0x1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST0_EN_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST0_EN_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR                                        (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000220)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OFFS                                        (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000220)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR, HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_IN)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ENABLE_TRAINING_SEQ_BMSK                    0x80000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ENABLE_TRAINING_SEQ_SHFT                          0x1f
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BYPASS_LOGIC_DEP_BMSK                       0x40000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BYPASS_LOGIC_DEP_SHFT                             0x1e
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BYPASS_BIAS_DET_BMSK                        0x20000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BYPASS_BIAS_DET_SHFT                              0x1d
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_MSB_CLOCK_SELECT_BMSK                       0x10000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_MSB_CLOCK_SELECT_SHFT                             0x1c
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BYPASS_FIRST_ORDER_DSM_BMSK                  0x8000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BYPASS_FIRST_ORDER_DSM_SHFT                       0x1b
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DCO_OFFSET_CURRENT_ADJUST_BMSK               0x6000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DCO_OFFSET_CURRENT_ADJUST_SHFT                    0x19
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_MIRROR_DEVICE_ADJUST_BMSK                    0x1c00000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_MIRROR_DEVICE_ADJUST_SHFT                         0x16
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BIAS_STARTUP_CIR_DIS_BMSK                     0x200000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BIAS_STARTUP_CIR_DIS_SHFT                         0x15
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DISABLE_CLAMP_BMSK                            0x100000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DISABLE_CLAMP_SHFT                                0x14
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BYPASS_MODE_OF_BIAS_BMSK                       0x80000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BYPASS_MODE_OF_BIAS_SHFT                          0x13
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BYPASS_MODE_FOR_BIAS_EN_BMSK                   0x40000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BYPASS_MODE_FOR_BIAS_EN_SHFT                      0x12
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BIAS_ADJUST_BMSK                               0x30000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_BIAS_ADJUST_SHFT                                  0x10
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DIV2_NMO_EN_BMSK                                0x8000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DIV2_NMO_EN_SHFT                                   0xf
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DIS_LEAK_CMP_BMSK                               0x4000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DIS_LEAK_CMP_SHFT                                  0xe
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_SINGLE_DMET_EN_BMSK                             0x2000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_SINGLE_DMET_EN_SHFT                                0xd
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DEMET_WINDOW_DIS_BMSK                           0x1000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DEMET_WINDOW_DIS_SHFT                              0xc
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NMO_OSC_SEL_BMSK                                 0xc00
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NMO_OSC_SEL_SHFT                                   0xa
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NOISE_MAG_BMSK                                   0x380
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NOISE_MAG_SHFT                                     0x7
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NOISE_GEN_EN_BMSK                                 0x40
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NOISE_GEN_EN_SHFT                                  0x6
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OSC_BIAS_GND_BMSK                                 0x20
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OSC_BIAS_GND_SHFT                                  0x5
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_PLL_TEST_OUT_SEL_BMSK                             0x18
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_PLL_TEST_OUT_SEL_SHFT                              0x3
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_STATUS_REG_EN_BMSK                                 0x4
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_STATUS_REG_EN_SHFT                                 0x2
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_SKIP_FINE_TUNE_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_SKIP_FINE_TUNE_SHFT                                0x1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_SKIP_FAST_LOCK_BMSK                                0x1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_SKIP_FAST_LOCK_SHFT                                0x0

#define HWIO_MSS_QDSP6SS_PLL_STATUS_ADDR                                            (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000224)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_OFFS                                            (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000224)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_RMSK                                            0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_STATUS_ADDR, HWIO_MSS_QDSP6SS_PLL_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_STATUS_31_0_BMSK                                0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_STATUS_STATUS_31_0_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR                                          (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000228)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_OFFS                                          (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000228)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_RMSK                                          0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR, HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_FREQUENCY_CTL_WORD_BMSK                       0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_FREQUENCY_CTL_WORD_SHFT                              0x0

#define HWIO_MSS_QDSP6SS_PLL_OPMODE_ADDR                                            (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x0000022c)
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_OFFS                                            (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x0000022c)
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_RMSK                                                   0x7
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_OPMODE_ADDR, HWIO_MSS_QDSP6SS_PLL_OPMODE_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_OPMODE_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_OPMODE_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_OPMODE_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_OPMODE_IN)
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_PLL_OPMODE_BMSK                                        0x7
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_PLL_OPMODE_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_PLL_STATE_ADDR                                             (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000230)
#define HWIO_MSS_QDSP6SS_PLL_STATE_OFFS                                             (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000230)
#define HWIO_MSS_QDSP6SS_PLL_STATE_RMSK                                                    0x7
#define HWIO_MSS_QDSP6SS_PLL_STATE_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_STATE_ADDR, HWIO_MSS_QDSP6SS_PLL_STATE_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_STATE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_STATE_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_STATE_PLL_STATE_BMSK                                          0x7
#define HWIO_MSS_QDSP6SS_PLL_STATE_PLL_STATE_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_PLL_DROOP_ADDR                                             (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000234)
#define HWIO_MSS_QDSP6SS_PLL_DROOP_OFFS                                             (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000234)
#define HWIO_MSS_QDSP6SS_PLL_DROOP_RMSK                                                   0xff
#define HWIO_MSS_QDSP6SS_PLL_DROOP_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_DROOP_ADDR, HWIO_MSS_QDSP6SS_PLL_DROOP_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_DROOP_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_DROOP_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_DROOP_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_DROOP_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_DROOP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_DROOP_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_DROOP_IN)
#define HWIO_MSS_QDSP6SS_PLL_DROOP_DROOP_CODE_BMSK                                        0xfe
#define HWIO_MSS_QDSP6SS_PLL_DROOP_DROOP_CODE_SHFT                                         0x1
#define HWIO_MSS_QDSP6SS_PLL_DROOP_DROOP_EN_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_PLL_DROOP_DROOP_EN_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_ADDR                                          (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000238)
#define HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_OFFS                                          (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000238)
#define HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_RMSK                                              0xffff
#define HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_ADDR, HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_IN)
#define HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_PLL_FRAC_VAL_BMSK                                 0xffff
#define HWIO_MSS_QDSP6SS_PLL_FRAC_VAL_PLL_FRAC_VAL_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_PLL_SPARE_ADDR                                             (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x0000023c)
#define HWIO_MSS_QDSP6SS_PLL_SPARE_OFFS                                             (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x0000023c)
#define HWIO_MSS_QDSP6SS_PLL_SPARE_RMSK                                                   0xff
#define HWIO_MSS_QDSP6SS_PLL_SPARE_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SPARE_ADDR, HWIO_MSS_QDSP6SS_PLL_SPARE_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_SPARE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SPARE_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_SPARE_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_SPARE_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_SPARE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_SPARE_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_SPARE_IN)
#define HWIO_MSS_QDSP6SS_PLL_SPARE_SPARE_OUTPUTS_BMSK                                     0xf0
#define HWIO_MSS_QDSP6SS_PLL_SPARE_SPARE_OUTPUTS_SHFT                                      0x4
#define HWIO_MSS_QDSP6SS_PLL_SPARE_SPARE_INPUTS_BMSK                                       0xf
#define HWIO_MSS_QDSP6SS_PLL_SPARE_SPARE_INPUTS_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR                                   (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000240)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_OFFS                                   (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000240)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_RMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR, HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_IN)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_RESERVE_BITS31_8_BMSK                  0xffffff00
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_RESERVE_BITS31_8_SHFT                         0x8
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_SSC_DELTA_ALPHA_BMSK                         0xff
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_SSC_DELTA_ALPHA_SHFT                          0x0

#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR                                   (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000244)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_OFFS                                   (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000244)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_RMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR, HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_IN)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_RESERVE_BITS31_4_BMSK                  0xfffffff0
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_RESERVE_BITS31_4_SHFT                         0x4
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_SSC_UPDATE_RATE_BMSK                          0xf
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_SSC_UPDATE_RATE_SHFT                          0x0

#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR                                     (MSS_QDSP6V65SS_PUB_REG_BASE      + 0x00000248)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_OFFS                                     (MSS_QDSP6V65SS_PUB_REG_BASE_OFFS + 0x00000248)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_RMSK                                     0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR, HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_IN)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_RESERVE_BITS31_4_BMSK                    0xfffffff0
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_RESERVE_BITS31_4_SHFT                           0x4
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_SSC_NUM_STEPS_BMSK                              0xf
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_SSC_NUM_STEPS_SHFT                              0x0


#endif /* __UCLOCKHWIO_H__ */
