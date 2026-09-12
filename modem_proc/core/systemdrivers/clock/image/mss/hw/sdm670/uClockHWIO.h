#ifndef __UCLOCKHWIO_H__
#define __UCLOCKHWIO_H__
/*
===========================================================================
*/
/**
  @file uClockHWIO.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SDM670 (Warlock) [napaliq_v1.0_p3q2r105_MTO]
 
  This file contains HWIO register definitions for the following modules:
    MPSS_PERPH
    GCC_CLK_CTL_REG

  'Include' filters applied: MSS_BUS_CRYPTO_CBCR[MPSS_PERPH] MSS_AXI_CRYPTO_CBCR[MPSS_PERPH] GCC_PRNG_AHB_CBCR[GCC_CLK_CTL_REG] GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE[GCC_CLK_CTL_REG] 
  'Exclude' filters applied: GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_1[GCC_CLK_CTL_REG] 

  Generation parameters: 
  { u'filename': u'uClockHWIO.h',
    u'module-filter-exclude': { u'GCC_CLK_CTL_REG': [ u'GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_1']},
    u'module-filter-include': { u'GCC_CLK_CTL_REG': [ u'GCC_PRNG_AHB_CBCR',
                                                      u'GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE'],
                                u'MPSS_PERPH': [ u'MSS_BUS_CRYPTO_CBCR',
                                                 u'MSS_AXI_CRYPTO_CBCR']},
    u'modules': [u'MPSS_PERPH', u'GCC_CLK_CTL_REG'],
    u'output-offsets': True}
*/
/*
  ===========================================================================

  Copyright (c) 2017 Qualcomm Technologies, Inc.
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

  $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/sdm670/uClockHWIO.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  ===========================================================================
*/

#include "msmhwiobase.h"

/*----------------------------------------------------------------------------
 * MODULE: MPSS_PERPH
 *--------------------------------------------------------------------------*/

#define MPSS_PERPH_REG_BASE                                                         (MODEM_TOP_BASE      + 0x001a8000)
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
#define GCC_CLK_CTL_REG_REG_BASE_OFFS                                                              0x00000000

#define HWIO_GCC_PRNG_AHB_CBCR_ADDR                                                                (GCC_CLK_CTL_REG_REG_BASE      + 0x00034004)
#define HWIO_GCC_PRNG_AHB_CBCR_OFFS                                                                (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00034004)
#define HWIO_GCC_PRNG_AHB_CBCR_RMSK                                                                0x80000006
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
#define HWIO_GCC_PRNG_AHB_CBCR_CLK_ARES_BMSK                                                              0x4
#define HWIO_GCC_PRNG_AHB_CBCR_CLK_ARES_SHFT                                                              0x2
#define HWIO_GCC_PRNG_AHB_CBCR_HW_CTL_BMSK                                                                0x2
#define HWIO_GCC_PRNG_AHB_CBCR_HW_CTL_SHFT                                                                0x1

#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_ADDR                                                 (GCC_CLK_CTL_REG_REG_BASE      + 0x00035004)
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_OFFS                                                 (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00035004)
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_RMSK                                                  0x16fbd7f
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_IN          \
        in_dword_masked(HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_ADDR, HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_RMSK)
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_INM(m)      \
        in_dword_masked(HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_ADDR, m)
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_OUT(v)      \
        out_dword(HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_ADDR,v)
#define HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_ADDR,m,v,HWIO_GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE_IN)
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


#endif /* __UCLOCKHWIO_H__ */
